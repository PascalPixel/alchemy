#include "types.h"

#define NULL ((void *)0)
#define SceneData_GetTableAfa0 Func_02000088
#define SceneData_SelectTableByFlag834 Func_02000094
#define SceneData_SelectTableByFlags834And87a Func_020000b8
#define SceneData_SelectTableByFlags87a_815_834 Func_02000158

extern u8 Data_0200b144[];
extern u8 Data_0200b108[];
extern s16 Data_02000240[];
extern u8 Data_0200b380[];
extern u8 Data_0200b560[];
extern u8 Data_0200b7d0[];
extern u8 Data_0200b170[];
extern u8 Data_0200bcec[];
extern u8 Data_0200bb3c[];
extern u8 Data_0200bb30[];
extern u8 Data_0200ba64[];
extern u8 Data_0200b938[];

s32 Func_0200270e(s32);
s32 Func_02002732(s32);
s32 Func_02002754(s32);
s32 Func_020027d2(s32);
s32 Func_020027e0(s32);
s32 Func_02002802(s32);

/*
 * resource_374 owner at 0x02000088, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x200008c holding 0x200afa0.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000088 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200afa0 is image offset
 * 0x2fa0 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */
u8 *SceneData_GetTableAfa0(void)
{
    return (u8 *)0x0200afa0;
}

void *SceneData_SelectTableByFlag834(void)
{
    if (Func_0200270e(0x834) != 0) {
        return Data_0200b144;
    }
    return Data_0200b108;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (Func_02002732(0x834) != 0) {
        return Data_0200b380;
    }
    if (Data_02000240[225] == 12) {
        return Data_0200b560;
    }
    if (Func_02002754(0x87a) != 0) {
        return Data_0200b7d0;
    }
    return Data_0200b170;
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (Func_020027d2(0x87a) != 0) {
        return Data_0200bcec;
    }
    if (Func_020027e0(0x815) != 0) {
        return Data_0200bb3c;
    }
    if (Data_02000240[225] == 12) {
        return Data_0200bb30;
    }
    if (Func_02002802(0x834) != 0) {
        return Data_0200ba64;
    }
    return Data_0200b938;
}
