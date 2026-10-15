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

s32 Func_02000070(void)
{
    Func_0200246c(0, 65);
    return 0;
}

const u8 *Func_02000080(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom1)
        return Data_02005b0c;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom2)
        return Data_02005b54;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom3)
        return Data_02005bcc;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom4)
        return Data_02005d4c;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom5)
        return Data_02005eb4;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom6)
        return Data_02005f74;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom7)
        return Data_020060c4;
    return Data_02005af4;
}

void Func_02000118(void)
{
    u8 *event = Ram_HeapSlots->eventWork;

    /* FAKEMATCH: ⚓️ loads the event work before it builds the offset */
    asm volatile("");
    event[0xd38] = 1;
}
