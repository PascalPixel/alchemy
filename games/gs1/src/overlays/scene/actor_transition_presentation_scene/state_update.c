#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_ApplyValues8And2And1 Func_02000030
#define SceneState_ApplyValues11And62 Func_02000040
#define SceneState_SetServiceZeroValue06 Func_02000fcc
#define SceneState_StoreValueToWorkspaceWord24WhenFlagged Func_0200170c
#define SceneState_ClearWorkWord24 Func_02001730
#define FieldScene_ApplyOffset0Neg32 Func_02001ab0
#define SceneState_ApplyOffsetMinus32 Func_02001ac0
#define SceneState_SetValue17e1 Func_02001f04
#define SceneState_SetWorkspace370ByFlag820 Func_02001f58

/* Prepare service zero, set its +6 halfword, and run the follow-up sequence. */
struct SceneService_02000fcc {
    u16 unknown00[3];
    u16 value06;
};

extern u8 *Data_03001ee0;
extern u8 *Data_0200b328;
extern u8 *Data_03001ebc;

void Func_02002442(s32, s32, s32);
void Func_020023f0(s32, s32);
void Func_02003308(void);
struct SceneService_02000fcc *Func_02003326(s32 index);
void Func_020033f2(s32 value);
void Func_020033b6(void);
void Func_020033c2(void);
void Func_02003390(s32 value);
s32 Func_02003a6e(s32);
void Func_0200358a(s32, s32);
void Func_0200359a(s32, s32);
void Func_02004240(void);
void Func_02004210(s32, s32);
void Func_02004254(void);
void Func_02004294();
s32 Func_0200427c();
void Func_02004270();
void Func_0200427a();
s32 Func_02004290();
void Func_020042da();

/* 0x03001ebc is the overlay's workspace pointer. */

/*
 * Imports. Each alias names the call word its site encodes, not a runtime
 * address. Only those used for their return value are typed, and the
 * declarations are old-style because one name is reached with different
 * argument counts.
 */

void SceneState_ApplyValues8And2And1(void)
{
    Func_02002442(8, 2, 1);
}

void SceneState_ApplyValues11And62(void)
{
    Func_020023f0(0xB, 0x3E);
}

void SceneState_SetServiceZeroValue06(void)
{
    struct SceneService_02000fcc *work;

    Func_02003308();
    work = Func_02003326(0);
    work->value06 = 0x4000;
    Func_020033f2(123);
    Func_020033b6();
    Func_020033c2();
    Func_02003390(1);
}

void SceneState_StoreValueToWorkspaceWord24WhenFlagged(void)
{
    s32 *flag = (s32 *)0x0200B328;

    if (*flag != 0) {
        u8 *state = Data_03001ee0;

        *(s32 *)(state + 24) = Func_02003a6e(0);
    }
}

void SceneState_ClearWorkWord24(void)
{
    if (Data_0200b328 != 0) {
        *(s32 *)(Data_03001ee0 + 24) = 0;
    }
}

void FieldScene_ApplyOffset0Neg32(void)
{
    Func_0200358a(0, -32);
}

void SceneState_ApplyOffsetMinus32(void)
{
    Func_0200359a(-32, 0);
}

void SceneState_SetValue17e1(void)
{
    Func_02004240();
    Func_02004210(0x17E1, 1);
    Func_02004254();
}

/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void SceneState_SetWorkspace370ByFlag820(void)
{
    Func_02004294();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (Func_0200427c((s32)0x820) != 0) {
        Func_02004270((s32)0x17e5, 1);
    } else {
        Func_0200427a((s32)0x17e4, 1);
        if (Func_02004290((s32)0xe6) != -1) {
            u8 *workspace = Data_03001ebc;

            /* movs r1,#0xb9 / lsls r1,#1 gives the byte offset 370. */
            /*
             * The store goes through a pointer local and an s32 value local,
             * in that order. Storing the literal directly builds the constant
             * in HImode and loads it from the literal pool, costing a pool
             * word; splitting the address out first also fixes which register
             * holds it.
             */
            {
                u16 *slot = (u16 *)(workspace + 370);
                s32 one = 1;

                *slot = (u16)one;
            }
        }
    }
    Func_020042da();
}
