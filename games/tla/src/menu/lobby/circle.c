#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct LobbyEventWork
{
    u8  unk000[0x16a];
    s16 raisedTrigger;
};

extern s32 gData_020022bc;

void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
void Func_02001ab4(s32 skip);

static __inline__ s32 Flag_Test(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

s32 Func_020002d8(void)
{
    s32 set;

    set = GameFlag_Test(0x203);
    if (set != 0)
    {
        return set;
    }
    if (++gData_020022bc == 300)
    {
        gData_020022bc = 0;
        GameFlag_ClearBit(0x200);
    }
    set = Flag_Test(0x200);
    if (set != 0)
    {
        return set;
    }
    Func_02001a14();
    Func_02001ab4(0);
    Func_02001a64((s32)MsgIfYouWantToParticipate);
    UiText_OpenMessageAtObject(8, 0);
    WaitFrames(5);
    GameFlag_SetBit(0x200);
    Func_02001a1c();
}

s32 Func_02000344(void)
{
    struct LobbyEventWork *work = Ram_HeapSlots->eventWork;

    LinkLobby_WriteSlotValue(4);
    GameFlag_ClearBit(0x200);
    GameFlag_ClearBit(0x203);
    Func_02001a14();
    Func_02001ab4(0);
    work->raisedTrigger = 0;
    Func_02001a64((s32)MsgTheLinkWas);
    UiText_OpenMessageAtObject(8, 0);
    GameFlag_ClearBit(0x205);
    Func_02001a1c();
}
