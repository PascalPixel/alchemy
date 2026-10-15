#include "room.h"

extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 Data_02005af4[];
extern const u8 Data_02005b0c[];
extern const u8 Data_02005b54[];
extern const u8 Data_02005bcc[];
extern const u8 Data_02005d4c[];
extern const u8 Data_02005eb4[];
extern const u8 Data_02005f74[];
extern const u8 Data_020060c4[];

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

void Func_02000590(void)
{
    if (!Flag_IsSet(0xa36)) {
        Func_020046b4();
        Func_02004ca8();
        Func_02004da0(0);
        Func_02004dd0();
        WORD_AT(Object_GetById(gPartyState.currentOwner), 108) = 0;
        Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 0);
        Func_02004cd8_(gPartyState.currentOwner, 0x2b00000, 0x1680000);
        Camera_SetBounds(0x2b00000, -1, 0x1680000, 1);
        Func_02004d20();
        Func_02004dc8();
        Battle_WaitMode0(40);
        Func_02004718();
        BattleFx_PrepareBufferInterpolation();
        WaitFrames(10);
        Func_02004cb0();
        Flag_Set(0xa36);
    } else {
        Func_020046b4();
        Func_02004dd0();
        {
            u8 *party = (u8 *)&gPartyState;

            /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
            do {
            } while (0);
            party[0x2c2] = 2;
        }
        Func_02004d30(18);
        Battle_WaitMode0(40);
        Func_02004718();
    }
}

void Func_02000648(void)
{
    Func_020046b4();
    Func_02004dd0();
    {
        u8 *party = (u8 *)&gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do {
        } while (0);
        party[0x2c2] = 2;
    }
    Func_02004d30(17);
    Battle_WaitMode0(40);
    Func_02004718();
}

void Func_02000678(s32 state, s32 frame)
{
    if (state == 1) {
        Func_02004de0(0x23e);
        Func_02004d58(0x10000, 0);
        Func_02004d60(60);
        Func_02004c30_(47, 94, 10, 6, 47, 100);
        Func_02004c28_(110, 40, 12, 4, 46, 40);
        Func_02004cd8(18, 0, 0);
        Flag_Set(0xa3f);
    }
    if (frame > 96 && frame < 272 && frame % 30 == 0)
        Func_02000414(18);
    if (frame == 276) {
        Func_02004d58(0x7fff, 0);
        Func_02004d60(20);
    }
}
