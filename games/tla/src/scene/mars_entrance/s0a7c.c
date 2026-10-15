#include "room.h"

extern char     SceneId_Ov6aaRoom1;
extern char     SceneId_Ov6aaRoom2;
extern char     SceneId_Ov6aaRoom3;
extern char     SceneId_Ov6aaRoom4;
extern char     SceneId_Ov6aaRoom5;
extern char     SceneId_Ov6aaRoom6;
extern char     SceneId_Ov6aaRoom7;
extern const u8 gData_02005af4[];
extern const u8 gData_02005b0c[];
extern const u8 gData_02005b54[];
extern const u8 gData_02005bcc[];
extern const u8 gData_02005d4c[];
extern const u8 gData_02005eb4[];
extern const u8 gData_02005f74[];
extern const u8 gData_020060c4[];

void Func_02000414(s32 id);
void Func_02000798(void);
void Func_020019c4(void);
void Func_020046b4(void);
void Func_02004718(void);
void Func_02004bf8(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02004d58(s32 a0, s32 a1);
void Func_02004d60(s32 frames);
void Func_02004dc8(void);
void Func_02004dd0(void);
void BattleFx_PrepareBufferInterpolation(void);

void Func_02000a7c(void)
{
    SCENE_WORK->unk166 = 0;
    Flag_Clear(0x106);
    Flag_Clear(0x104);
    Func_02004ca8();
    Func_02004da0(0);
    Func_02004d10(0x10000, 0x2000);
    Camera_SetBounds(0x3680000, -1, 0xa80000, 1);
    Func_02004d20();
    Battle_WaitMode0(40);
    Camera_SetBounds(0x3680000, -1, 0x1080000, 1);
    Func_02004d20();
    Battle_WaitMode0(120);
    Func_02004cb0();
}

void Func_02000af4(void)
{
    Func_020046b4();
    Func_02004dd0();
    {
        u8 *party = (u8 *)&gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do
        {
        } while (0);
        party[0x2c2] = 2;
    }
    Func_02004d30(14);
    Battle_WaitMode0(40);
    Func_02004718();
}

void Func_02000b24(void)
{
    Func_02004c20(53, 30, 4, 3, 53, 48);
    Func_02004c30(117, 30, 4, 3, 117, 48);
}

void Func_02000b54(void)
{
    Func_02004c20(53, 40, 4, 3, 53, 48);
    Func_02004c30(117, 45, 4, 3, 117, 48);
}

void Func_02000b84(s32 state, s32 frame)
{
    if (state == 1)
    {
        Func_02004de0(0x23e);
        Func_02004d58(0x10000, 0);
        Func_02004d60(60);
        Func_02004c30(12, 95, 8, 6, 12, 101);
        Func_02004c28(76, 40, 8, 5, 12, 40);
        Func_02004cd8(12, 0, 0);
        Flag_Set(0xa4d);
    }
    if (frame > 96 && frame < 272 && frame % 30 == 0)
        Func_02000414(12);
    if (frame == 276)
    {
        Func_02004d58(0x7fff, 0);
        Func_02004d60(20);
    }
}

void Func_02000c1c(void)
{
    Func_02004c28(35, 11, 3, 6, 53, 11);
    Func_02004c30(35, 75, 3, 6, 117, 11);
    Func_02004c30(35, 75, 3, 6, 53, 75);
    Flag_Set(0xa49);
}
