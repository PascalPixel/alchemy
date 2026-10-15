/* Draft of overlay 6b5's Func_0200145c (0x0200945c, 0x46c bytes with its
 * pool), the lobby's set-up by entrance: differ 28 of 438. Devices in place:
 * one scalar holds the score row's address, then its score, in entrances 8
 * and 9, and entrance 10's best streak reuses that slot. Left: the flag
 * loop's counter and its flag number swap registers (native: the counter in
 * r5, cleared after the name pointer loads, and 0x2f0 + i in r6; here the
 * counter takes r6, cleared at the top beside the stores' zero, and the
 * flag number r5), and four sched2 ties of one or two instructions each:
 * linkTallies[3]'s offset is built before the first tally's store, not
 * after; in entrance 9 the 0x80 of its offset comes before the pool load;
 * in entrance 10 1000 is built before Ram_HeapSlots loads; and the streak's
 * shift comes before the best streak's load. */
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "flag_call.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct LobbyEventWork {
    u8 unk000[0x16a];
    s16 raisedTrigger;
    u8 unk16c[0x40];
    s32 startTransition;
};

extern s32 Data_020022b8;
extern s32 Data_020022bc;
extern u16 Data_02003a74[];
extern u8 gAutoSleepEnabled;

s32 Func_02000134(void);
void Func_02000044(void);
s32 Func_02000f6c(s32 owner);
s32 Func_020013ec(void);
s32 Func_02001400(void);
void Func_02001414(s32 value);
void Func_02001914(void);
void Func_02001944(void *callback, s32 mask);
void Func_02001994(s32 entry);
void Func_0200199c(s32 x, s32 y, s32 width, s32 height, s32 destX, s32 destY);
void Func_020019e4(s32 value, s32 digits);
void Func_02001a04(s32 value);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
void Func_02001a74(s32 actor, s32 flags);
void Func_02001ab4(s32 skip);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void Sound_LoadPresetParameters(s32 preset);
void LinkLobby_WriteSlotValue(s32 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void WaitFrames(s32 frames);
void Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);

static inline s32 Flag_GetByte(s32 flag)
{
    return GameFlag_GetByte(flag);
}

static inline void Flag_SetByte(s32 flag, s32 value)
{
    GameFlag_SetByte(flag, value);
}

static inline void Map_CopyCells(s32 x, s32 y, s32 width, s32 height, s32 destX, s32 destY)
{
    Func_0200199c(x, y, width, height, destX, destY);
}

s32 Func_0200145c(void)
{
    s32 i;
    u32 score;
    struct LobbyEventWork *work;
    void *callback;

    Flag_Set(0x17e);
    gPartyState.packedEvents[0] = 0;
    gPartyState.packedEvents[1] = 0;
    Data_020022bc = 0;
    Data_020022b8 = 0;
    ((struct LobbyEventWork *)Ram_HeapSlots->eventWork)->startTransition = 0x201;
    Sound_LoadPresetParameters(2);
    Func_02001414(gPartyState.linkTallies[6]);
    Map_CopyCells(11, 11, 1, 1, 13, 10);
    LinkLobby_WriteSlotValue(4);
    WaitFrames(1);
    Func_02001994(5);
    {
        u16 *name = Data_02003a74;

        name[4] = 'T';
        name[5] = 'A';
        name[6] = 'L';
        name[7] = 'K';
    }
    for (i = 0; i < 8; i++) {
        Flag_Clear(0x2f0 + i);
        if (Func_02000f6c(i))
            Flag_Set(0x2f0 + i);
    }
    if (gPartyState.entrance == 8) {
        s32 wins;
        s32 msg;
        s32 v;

        Func_02001a14();
        Func_02001ab4(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        LinkLobby_WriteSlotValue(5);
        gPartyState.linkTallies[0]++;
        gPartyState.linkTallies[3]++;
        wins = (s8)Flag_GetByte(0x3f8);
        msg = wins * 2 + 2;
        if (msg > 14)
            msg = 14;
        v = Flag_GetByte(1000);
        if (v == 2) {
            Flag_SetByte(1000, 0);
            wins++;
            msg++;
        } else {
            Flag_SetByte(1000, v + 1);
        }
        ObjectMotion_SetAngleToward(8, gPartyState.currentOwner, 0);
        Func_02001a64((s32)MsgNotBadWould + msg);
        UiText_OpenMessageAtObject(8, 0);
        if (Inventory_PromptAndSetObjectMode(4, 0) == 0) {
            if (wins > 90)
                wins = 90;
            Flag_SetByte(0x3f8, wins);
        } else {
            Flag_Clear(0x173);
            Flag_SetByte(0x3f8, -1);
            /* FAKEMATCH: one scalar holds the row address, then its score, as in ☀️'s entrance 9 */
            score = (u32)&gPartyState.linkTallies[3];
            Func_020019e4(*(u16 *)score, 5);
            score = *(u16 *)score;
            if (gPartyState.linkTallies[2] < score) {
                gPartyState.linkTallies[2] = score;
                Func_02001a64((s32)MsgYouWonBattle);
                UiText_OpenMessageAtObject(8, 0);
                Func_02001400();
            } else {
                Func_02001a64((s32)MsgYouWonBattlePlease);
                UiText_OpenMessageAtObject(8, 0);
            }
            LinkLobby_WriteSlotValue(0);
        }
        Func_02001a1c();
    } else if (gPartyState.entrance == 9) {
        gPartyState.linkTallies[1]++;
        Func_02001a14();
        Func_02001ab4(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        LinkLobby_WriteSlotValue(5);
        ObjectMotion_SetAngleToward(8, gPartyState.currentOwner, 0);
        /* FAKEMATCH: one scalar holds the row address, then its score, as in ☀️'s entrance 9 */
        score = (u32)&gPartyState.linkTallies[3];
        Func_020019e4(*(u16 *)score, 5);
        score = *(u16 *)score;
        if (gPartyState.linkTallies[2] < score) {
            gPartyState.linkTallies[2] = score;
            Func_02001a64((s32)MsgYouWonBattle);
            UiText_OpenMessageAtObject(8, 0);
            Func_02001400();
        } else {
            Func_02001a64((s32)MsgYouWonBattleI);
            UiText_OpenMessageAtObject(8, 0);
        }
        gPartyState.linkTallies[3] = 0;
        Flag_Clear(0x173);
        Flag_SetByte(0x3f8, -1);
        LinkLobby_WriteSlotValue(0);
        Func_02001a1c();
    } else if (gPartyState.entrance == 10) {
        s32 v;

        Func_02001a14();
        Func_02001ab4(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        if (Flag_IsSet(1000)) {
            work = Ram_HeapSlots->eventWork;
            Flag_Clear(1000);
            work->raisedTrigger = 2;
            Flag_Clear(0x304);
            WaitFrames(20);
            Func_02000044();
            LinkLobby_WriteSlotValue(0);
            LinkLobby_WriteSlotValue(4);
        } else {
            gPartyState.linkTallies[4]++;
            v = gPartyState.linkTallies[7] + 1;
            gPartyState.linkTallies[7] = v;
            /* FAKEMATCH: reuse the other score branches' address slot, as ☀️ does */
            score = (u32)&gPartyState.linkTallies[6];
            if (*(u16 *)score < (u16)v)
                *(u16 *)score = v;
            Func_02001414(*(u16 *)score);
            Func_020013ec();
            Flag_Set(0x304);
            Flag_Set(0x305);
        }
        Func_02001a1c();
    } else if (gPartyState.entrance == 11) {
        s32 v;

        Func_02001a14();
        Func_02001ab4(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        if ((v = Flag_IsSet(0x173)) == 0) {
            gPartyState.linkTallies[5]++;
            gPartyState.linkTallies[7] = v;
            Func_020013ec();
        }
        Flag_Set(0x304);
        Flag_Clear(0x305);
        Func_02001a1c();
    } else {
        Func_02001914();
        Flag_Clear(0x172);
        Flag_SetByte(0x3f8, -1);
        if (gPartyState.unk244[6] != 0) {
            Func_02001a14();
            Func_02001ab4(0);
            Event_StartScreenTransition();
            Event_WaitTransitionFrames();
            ObjectMotion_SetAngleToward(8, gPartyState.currentOwner, 0);
            Func_02001a64((s32)MsgNoteYouCan);
            Func_02001a74(8, 0);
            Func_02001a1c();
        }
        gPartyState.unk244[6] = 0;
        gAutoSleepEnabled = 0;
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
    }
    callback = Func_02000134;
    Scheduler_AddOrUpdateCallback(callback, 0x480);
    Func_02001944(callback, 1);
    if (gPartyState.entrance != 8 || !Flag_IsSet(0x173)) {
        Func_02001a04(1);
        BattlePlacement_UpdateTimedEntriesTwentyTimes();
    }
    return 0;
}
