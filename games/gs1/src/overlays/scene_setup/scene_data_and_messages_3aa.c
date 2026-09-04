/* Contiguous unnamed leaf-owner run for resource_3aa. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009c04[];
extern u8 Data_02009bd4[];

#define SceneData_SelectTable9bd4ByState Func_02000030

s32 SceneData_SelectTable9bd4ByState(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        return (s32)Data_02009c04;
    }
    return (s32)Data_02009bd4;
}

#include "types.h"

#define SceneData_ReturnZero Func_02000060

s32 SceneData_ReturnZero(void)
{
    return 0;
}

#include "types.h"

/*
 * resource_3aa owner at 0x02000064, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000068 holding 0x2009d9c.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000064 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009d9c is image offset
 * 0x1d9c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

#define SceneData_GetTable9d9c Func_02000064

u8 *SceneData_GetTable9d9c(void)
{
    return (u8 *)0x02009d9c;
}

#include "types.h"

#define SceneDialogue_RunActor16Message1b88 Func_020000f4

#define SceneDialogue_RunActor13Message1b83 Func_020000d4

#define SceneData_SelectTable9f2cByState Func_020000a4

#define SceneData_SelectTable9ddcByStateWithInit Func_0200006c

extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009df4[];
extern u8 Data_02009ddc[];

extern void Func_02001b1a();

s32 SceneData_SelectTable9ddcByStateWithInit(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        Func_02001b1a(Data_02009df4);
        return (s32)Data_02009df4;
    }
    return (s32)Data_02009ddc;
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009f38[];
extern u8 Data_02009f2c[];

s32 SceneData_SelectTable9f2cByState(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        return (s32)Data_02009f38;
    }
    return (s32)Data_02009f2c;
}

#include "types.h"

extern void Func_02001b5c(void);
extern void Func_02001bea(s32);
extern s32 Func_02001c12(s32, s32);
extern void Func_02001b76(void);

void SceneDialogue_RunActor13Message1b83(void)
{
    Func_02001b5c();
    Func_02001bea(0x1B83);
    Func_02001c12(13, 0);
    Func_02001b76();
}

#include "types.h"

extern void Func_02001b7c(void);
extern void Func_02001c0a(s32);
extern s32 Func_02001c32(s32, s32);
extern void Func_02001b96(void);

void SceneDialogue_RunActor16Message1b88(void)
{
    Func_02001b7c();
    Func_02001c0a(0x1B88);
    Func_02001c32(16, 0);
    Func_02001b96();
}
