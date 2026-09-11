#include "types.h"

#define SceneState_ApplyFlag300 Func_020002bc
#define SceneState_SetFlag953 Func_020002cc
#define SceneState_ApplyRectAndMarkActor16 Func_0200050c
#define SceneState_ApplyRectAndSetupActor19 Func_020005ec
#define SceneActor_MarkSlot21AndSetFlag205 Func_02000690
void Func_020013cc(s32);
void Func_020013cc_a(s32, s32);
u8 *Func_02001644(s32);
void Func_020015fa(s32, s32, s32, s32, s32, s32);
u8 *Func_02001664(s32);
void Func_02001652(s32);
u8 *Func_02001724(s32);
void Func_020016da(s32, s32, s32, s32, s32, s32);
void Func_020016f6(u8 *, s32);
u8 *Func_0200174c(s32);
void Func_02001738(s32);
u8 *Func_020017c8(s32);
void Func_0200177e(s32, s32, s32, s32, s32, s32);
void Func_0200179a(u8 *, s32);
u8 *Func_020017f0(s32);
void Func_020017dc(s32);

void SceneState_ApplyFlag300(void)
{
    Func_020013cc(0x300);
}

void SceneState_SetFlag953(void)
{
    Func_020013cc_a(0x953, 1);
}

void SceneState_ApplyRectAndMarkActor16(void)
{
    u8 *rec = Func_02001644(16);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 23;
    s32 sixth = 32;

    Func_020015fa(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        /* The rec is reloaded with the same selector before this store. */
        Func_02001664(16)[85] = 0;
        rec[35] = 1;
    }

    Func_02001652(0x200);
}

void SceneState_ApplyRectAndSetupActor19(void)
{
    u8 *p = Func_02001724(19);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a5 = 26;
    s32 a6 = 32;

    Func_020016da(26, 30, 1, 1, a5, a6);

    if (p != 0) {
        Func_020016f6(p, 0);
        /* The record is reloaded with the same selector before this store. */
        Func_0200174c(19)[85] = 0;
        p[35] = 1;
    }

    Func_02001738(0x203);
}

void SceneActor_MarkSlot21AndSetFlag205(void)
{
    u8 *record = Func_020017c8(21);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 28;
    s32 sixth = 33;

    Func_0200177e(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        Func_0200179a(record, 0);
        /* The record is reloaded with the same selector before this store. */
        Func_020017f0(21)[85] = 0;
        record[35] = 1;
    }

    Func_020017dc(0x205);
}
