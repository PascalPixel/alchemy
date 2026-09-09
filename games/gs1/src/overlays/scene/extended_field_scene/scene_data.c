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
 * Table getter for resource_374. The owner at 0x02000088 is eight bytes and
 * includes its one pool word at 0x0200008c: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
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
