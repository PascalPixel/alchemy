#include "ov670.h"

extern u8 Data_02006510[];
extern u8 Data_020066d8[];
extern u8 Data_020069a8[];
extern u8 Data_02006a98[];
extern u8 Data_02006cd8[];
extern u8 Data_020064f8[];
extern u8 Data_02006ef4[];
extern u8 Data_020070d4[];
extern u8 Data_02007434[];
extern u8 Data_020074dc[];
extern u8 Data_02007698[];
extern u8 Data_02006ee8[];
extern const u8 Data_02006324[];

u8 *Func_0200012c(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Kibonbo1)
        return Data_02006510;
    if (scene == (s32)&SceneId_Kibonbo4)
        return Data_020066d8;
    if (scene == (s32)&SceneId_Kibonbo2)
        return Data_020069a8;
    if (scene == (s32)&SceneId_Kibonbo5)
        return Data_02006a98;
    if (scene == (s32)&SceneId_Kibonbo3) {
        if (GameFlag_Test(0x8ee))
            Data_02006cd8[22] = 0;
        return Data_02006cd8;
    }
    return Data_020064f8;
}

u8 *Func_020001b4(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Kibonbo1)
        return Data_02006ef4;
    if (scene == (s32)&SceneId_Kibonbo4)
        return Data_020070d4;
    if (scene == (s32)&SceneId_Kibonbo2)
        return Data_02007434;
    if (scene == (s32)&SceneId_Kibonbo5)
        return Data_020074dc;
    if (scene == (s32)&SceneId_Kibonbo3)
        return Data_02007698;
    return Data_02006ee8;
}

void Func_02000228(void)
{
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgItLooksLikeThere);
    Func_02005b6c(15, 0);
    Func_02005aa4();
}

void Func_0200024c(void)
{
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgYouDidnT);
    Func_02005b6c(23, 0);
    Func_02005aa4();
}

void Func_02000270(void)
{
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgNowThatAkafubu);
    Func_02005b64(24, 0);
    ObjectMotion_SetAngleToward(24, gPartyState.current_owner, 0);
    Battle_WaitMode0(20);
    Func_02005b64(24, 0);
    Ov670_ObjTurn(24, 0x3000);
    Func_02005aa4();
}

void Func_020002c4(void)
{
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgAreYouGoing);
    Func_02005b6c(12, 0);
    Func_02005aa4();
}

s32 Func_020002e8(void)
{
    u16 turn;
    s32 facing;

    turn = Object_GetById(gPartyState.current_owner)->facing + 0x5fff;
    facing = 1;
    if (turn >= 0x3fff)
        facing = 0;
    return facing;
}

void Func_02000318(void)
{
    if (Func_020002e8()) {
        Func_02005c84(17, 20);
    } else {
        Func_02005a9c();
        Func_02005c3c(0);
        Func_02005b4c((s32)MsgTheWeaponsI);
        Func_02005b64(20, 0);
        Func_02005aa4();
    }
}

void Func_02000350(void)
{
    if (Func_020002e8()) {
        Func_02005c84(18, 21);
    } else {
        Func_02005a9c();
        Func_02005c3c(0);
        Func_02005b4c((s32)MsgTheOnlyDefenses);
        Func_02005b64(21, 0);
        Func_02005aa4();
    }
}

void Func_02000388(void)
{
    if (Func_020002e8()) {
        Func_02005c84(19, 22);
    } else {
        Func_02005a9c();
        Func_02005c3c(0);
        Func_02005b4c((s32)MsgIMSorryBut);
        Func_02005b64(22, 0);
        Func_02005aa4();
    }
}

void Func_020003c0(void)
{
    if (Func_020002e8()) {
        Func_02005c94(6, 23);
    } else {
        Func_02005a9c();
        Func_02005c3c(0);
        Func_02005b4c((s32)MsgDonTWorryAbout);
        Func_02005b64(23, 0);
        Func_02005aa4();
    }
}

void Func_020003f8(void)
{
    if (Func_020002e8()) {
        Func_02005c8c(25);
    } else {
        Func_02005a9c();
        Func_02005c3c(0);
        Func_02005b4c((s32)MsgKibomboSPeace);
        Func_02005b64(25, 0);
        Func_02005aa4();
    }
}

void Func_0200042c(void)
{
    if (Ov670_FlagChk(0x9df)) {
        Func_02005a9c();
        Func_02005c3c(0);
        ObjectMotion_SetAngleToward(36, gPartyState.current_owner, 0);
        Battle_WaitMode0(10);
        Func_02005c8c(36);
        Ov670_ObjArm(36, 0xb000, 0);
        Func_02005aa4();
    } else {
        Func_02005a9c();
        Func_02005c3c(0);
        Func_02005b4c((s32)MsgOhOhMy);
        Func_02005b5c(36, 0, 40);
        Func_02005b64(36, 0);
        ObjectMotion_SetAngleToward(36, gPartyState.current_owner, 0);
        Battle_WaitMode0(10);
        Func_02005c8c(36);
        Ov670_ObjArm(36, 0xb000, 0);
        Ov670_FlagSet(0x9df);
        Func_02005aa4();
    }
}

void Func_020004dc(void)
{
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgOhMyBack);
    Func_02005b64(16, 0);
    Func_02005aa4();
}

void Func_02000500(void)
{
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b4c((s32)MsgISwearThat);
    Func_02005b64(16, 0);
    Func_02005aa4();
}

void Func_02000524(s32 show)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Kibonbo2 || scene == (s32)&SceneId_Kibonbo5)
        Func_02000590(show, 15, 2);
    else if (scene == (s32)&SceneId_Kibonbo3)
        Func_02000590(show, 26, 2);
}

void Func_02000568(void)
{
    Func_02000524(1);
    GameFlag_SetBit(0x200);
}

void Func_0200057c(void)
{
    Func_02000524(0);
    GameFlag_ClearBit(0x200);
}

void Func_02000590(s32 show, s32 first, u32 count)
{
    u32 i;

    for (i = 0; i < count;) {
        u8 *flags = &Object_GetById(first + i)->priority_flags;
        u32 next = i + 1;

        if (show)
            *flags &= ~0x10;
        else
            /* FAKEMATCH: the native ties the flag or to the constant's register. */
            *flags = *flags + 0x10 - (*flags & 0x10);
        i = next;
    }
}

void Func_020005d0(void)
{
    Func_02005c6c(Data_02006324);
}

void Func_020005e0(void)
{
    Func_02005a64(34, 33, 23, 18, 1, 1);
    Ov670_BlendStart(34, 33, 1, 1, 23, 18);
    GameFlag_SetBit(0x86a);
}

void Func_02000618(s32 mode)
{
    if (mode == 1)
        Func_020005e0();
}

void Func_02000624(void)
{
    Func_02005a64(36, 34, 16, 7, 1, 1);
    Ov670_BlendStart(36, 34, 1, 1, 16, 7);
    GameFlag_SetBit(0x204);
}

void Func_0200065c(s32 mode)
{
    if (mode == 1)
        Func_02000624();
}

void Func_02000668(void)
{
    Func_02005a64(38, 34, 16, 11, 1, 1);
    Ov670_BlendStart(38, 34, 1, 1, 16, 11);
    GameFlag_SetBit(0x205);
}

void Func_020006a0(s32 mode)
{
    if (mode == 1)
        Func_02000668();
}

void Func_020006ac(void)
{
    Func_02005a64(36, 36, 14, 9, 1, 1);
    Ov670_BlendStart(36, 36, 1, 1, 14, 9);
    GameFlag_SetBit(0x206);
}

void Func_020006e4(s32 mode)
{
    if (mode == 1)
        Func_020006ac();
}

void Func_020006f0(void)
{
    Func_02005a64(38, 36, 18, 9, 1, 1);
    Ov670_BlendStart(38, 36, 1, 1, 18, 9);
    GameFlag_SetBit(0x207);
}

void Func_02000728(s32 mode)
{
    if (mode == 1)
        Func_020006f0();
}

void Func_02000734(void)
{
    struct Ov670Object *object = Object_GetById(11);

    if (object->x >> 20 == 6 && object->z >> 20 == 18) {
        Func_02005a9c();
        Func_02005c3c(0);
        Object_GetById(11)->motion_flags = 3;
        object->motion_flags = 3;
        do
            WaitFrames(1);
        while (object->velocity_y != 0);
        Func_02005c9c(188);
        WaitFrames(10);
        object->motion_flags = 0;
        Object_GetById(11)->motion_flags = 0;
        Ov670_BlendStart(5, 18, 1, 1, 6, 18);
        GameFlag_SetBit(0x9ea);
        if (!GameFlag_Test(0x8f4))
            Func_02001318();
        Func_02005aa4();
    }
}
