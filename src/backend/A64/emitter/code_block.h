// Copyright 2014 Dolphin Emulator Project / 2018 dynarmic project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <cstddef>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

#include "common/assert.h"
#include "common/common_types.h"

namespace Dynarmic::BackendA64 {
// Everything that needs to generate code should inherit from this.
// You get memory management for free, plus, you can use all emitter functions
// without having to prefix them with gen-> or something similar. Example
// implementation: class JIT : public CodeBlock<ARMXEmitter> {}
template <class T>
class CodeBlock : public T {
private:
    // A privately used function to set the executable RAM space to something
    // invalid. For debugging usefulness it should be used to set the RAM to a
    // host specific breakpoint instruction
    virtual void PoisonMemory() = 0;

protected:
    u8* region = nullptr;
    // Size of region we can use.
    size_t region_size = 0;
    // Original size of the region we allocated.
    size_t total_region_size = 0;

    bool m_is_child = false;
    std::vector<CodeBlock*> m_children;

public:
    CodeBlock() = default;
    virtual ~CodeBlock() {
        if (region)
            FreeCodeSpace();
    }
    CodeBlock(const CodeBlock&) = delete;
    CodeBlock& operator=(const CodeBlock&) = delete;
    CodeBlock(CodeBlock&&) = delete;
    CodeBlock& operator=(CodeBlock&&) = delete;

    // Call this before you generate any code.
    void AllocCodeSpace(size_t size) {
        region_size = size;
        total_region_size = size;
#if defined(_WIN32)
        void* ptr = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
        void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);

        if (ptr == MAP_FAILED)
            ptr = nullptr;
#endif
        ASSERT_MSG(ptr != nullptr, "Failed to allocate executable memory");
        region = static_cast<u8*>(ptr);
        T::SetCodePtr(region);
    }

    // Always clear code space with breakpoints, so that if someone accidentally
    // executes uninitialized, it just breaks into the debugger.
    void ClearCodeSpace() {
        PoisonMemory();
        ResetCodePtr();
    }

    // Call this when shutting down. Don't rely on the destructor, even though
    // it'll do the job.
    void FreeCodeSpace() {
        ASSERT(!m_is_child);
        ASSERT(munmap(region, total_region_size) == 0);
        region = nullptr;
        region_size = 0;
        total_region_size = 0;
        for (CodeBlock* child : m_children) {
            child->region = nullptr;
            child->region_size = 0;
            child->total_region_size = 0;
        }
    }

    bool IsInSpace(const u8* ptr) const {
        return ptr >= region && ptr < (region + region_size);
    }
    // Cannot currently be undone. Will write protect the entire code region.
    // Start over if you need to change the code (call FreeCodeSpace(),
    // AllocCodeSpace()).
    void WriteProtect() {
        ASSERT(mprotect(region, region_size, PROT_READ | PROT_EXEC) != 0);
    }
    void ResetCodePtr() {
        T::SetCodePtr(region);
    }
    size_t GetSpaceLeft() const {
        ASSERT(static_cast<size_t>(T::GetCodePtr() - region) < region_size);
        return region_size - (T::GetCodePtr() - region);
    }

    bool IsAlmostFull() const {
        // This should be bigger than the biggest block ever.
        return GetSpaceLeft() < 0x10000;
    }

    bool HasChildren() const {
        return region_size != total_region_size;
    }

    u8* AllocChildCodeSpace(size_t child_size) {
        ASSERT_MSG(child_size < GetSpaceLeft(), "Insufficient space for child allocation.");
        u8* child_region = region + region_size - child_size;
        region_size -= child_size;
        return child_region;
    }

    void AddChildCodeSpace(CodeBlock* child, size_t child_size) {
        u8* child_region = AllocChildCodeSpace(child_size);
        child->m_is_child = true;
        child->region = child_region;
        child->region_size = child_size;
        child->total_region_size = child_size;
        child->ResetCodePtr();
        m_children.emplace_back(child);
    }
};
} // namespace Dynarmic::BackendA64
