#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

/*
 * Not exact (measured-draft, see 08017658.json). Control flow, call targets,
 * call-argument shapes, and every RENDER_* field offset below are traced
 * directly from games/gs1/asm/08017658.s and match the reference exactly:
 * this candidate reaches the reference's own 276-byte length and its
 * instruction stream is the same 121 instructions in the same order and the
 * same literal-pool constants (RENDER_RESULT_OFS=0x12F4, +2, RENDER_ENTRY_TBL_OFS
 * =0xEB0, mask 0xFFFF, RENDER_BUSY_OFS+1/+2=0x12FA/0x12FB, plus the
 * ADDR_03001E8C base). The residual is a register-allocation/promotion
 * choice: the reference keeps 3 long-lived values in r8/r9/sl (base, the
 * &local28 out-pointer, and the render-entry index) across both inner calls,
 * where this compiler's allocator only promotes 2 (index, &local28) and
 * keeps base resident in r7 instead -- shifting every subsequent register
 * letter and the frame's 4-byte alignment pad. Several source respellings
 * (bitfield-vs-explicit-mask arg encoding, named out-pointer locals, local
 * declaration/statement order) were tried; all reproduce the same register
 * count gap. Func_08018038 mirrors UiText_BuildRenderEntries's exact
 * "build render entries, then check RENDER_ENTRY_TBL_OFS[index] != 0" idiom
 * (ui/text/prepare_message_work.c); Func_08016418 IS UiWork_Finalize
 * (ui/render/finalize.c, called the same way: Finalize(work, 1) on failure);
 * Func_080162d4/Func_080165d8 are argument-shape-compatible with
 * UiWindow_Create/UiWork_ActivateChannel but not confirmed identical. The
 * family-matcher's best guess, ui/text/prepare_message_work.c (7382/10000,
 * below threshold), is a genuine sibling by shared idiom and shared
 * UiWork_Finalize callee, not a shape-only false positive, but its own body
 * differs (different call targets/argument counts) so it was not transplanted.
 */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_08018038(s32 key, s32 mode);
s32 Func_0801868c(s32 index, s32 *arg1, s32 *arg2, s32 *out16, s32 *out12,
                   s32 *out28, s32 zero);
void *Func_080162d4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 flags);
s32 Func_080165d8(void *work, s32 index, s32 a, s32 b, s32 *out28, s32 c);
void UiWork_Finalize(void *work, s32 release);

void *Func_08017658(s32 key, s32 arg1, s32 arg2, u32 packed)
{
    s32 index;
    s32 local12;
    s32 local16;
    s32 local28;
    s32 flags;
    s32 flag_bits;
    s32 result;
    s32 *out28;
    void *work;
    u8 *base;

    flag_bits = packed & 0xFFFF;
    base = *(u8 **)ADDR_03001E8C;

    FIELD(base, u16, RENDER_RESULT_OFS) = (u16)((packed << 4) >> 20);
    FIELD(base, u16, RENDER_RESULT_OFS + 2) = 0;

    index = Func_08018038(key, 1);

    if (FIELD(base, u16, RENDER_ENTRY_TBL_OFS + index * 2) == 0)
        return 0;
    out28 = &local28;
    flags = 0;

    Func_0801868c(index, &arg1, &arg2, &local16, &local12, out28, 0);

    if (local16 == 0 && local12 == 0)
        return 0;

    if (!(flag_bits & 1))
        flags |= 2;
    if (flag_bits & 8)
        flags |= 8;
    if (flag_bits & 16)
        flags |= 128;
    if (flag_bits & 32)
        flags |= 256;

    work = Func_080162d4(arg1, arg2, local16, local12, flags);
    if (work == 0)
        return 0;

    result = Func_080165d8(work, index, 0, 0, out28, 0);
    if (result == 0) {
        UiWork_Finalize(work, 1);
        return 0;
    }

    FIELD(base, u8, RENDER_BUSY_OFS + 1) = 0;
    FIELD(base, u8, RENDER_BUSY_OFS + 2) = 0;
    return work;
}
