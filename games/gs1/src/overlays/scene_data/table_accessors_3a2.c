/* Contiguous unnamed leaf-owner run for resource_3a2. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000004a;
extern u8 Data_02009844[];
extern u8 Data_020097b4[];

#define SceneData_SelectTable97b4ByState Func_0200007c

s32 SceneData_SelectTable97b4ByState(void) {
    if (Data_02000240[224] == (s32)&Value_0000004a) {
        return (s32)Data_02009844;
    }
    return (s32)Data_020097b4;
}

#include "types.h"

#define SceneData_ReturnZero Func_020000ac

s32 SceneData_ReturnZero(void)
{
    return 0;
}

#include "types.h"

/*
 * resource_3a2 owner at 0x020000b0, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20000b4 holding 0x20098ec.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x020000b0 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20098ec is image offset
 * 0x18ec under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

#define SceneData_GetTable98ec Func_020000b0

u8 *SceneData_GetTable98ec(void)
{
    return (u8 *)0x020098ec;
}

#include "types.h"

#define SceneData_SelectTable9918ByState Func_020000b8

extern s16 Data_02000240[];
extern u8 Value_0000004a;
extern u8 Data_02009a38[];
extern u8 Data_02009918[];

s32 SceneData_SelectTable9918ByState(void) {
    if (Data_02000240[224] == (s32)&Value_0000004a) {
        return (s32)Data_02009a38;
    }
    return (s32)Data_02009918;
}
