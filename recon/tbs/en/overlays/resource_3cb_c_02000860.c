#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SYSTEM.H"

/* DRAFT: 808/820 bytes, 118 differing halfwords over the complete owner.
 * First pool at +0x2d4 rather than +0x274; prologue scheduling and packet
 * value load order remain. Two structural corrections exhausted; not adopted. */

s32 LinkLobby_PeerSlotMatches(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);
s32 Local_020007b0(void);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 mode);
void UiWork_FinalizeFar(s32 window, s32 mode);
void Main_080000d8(void *buf);
void Main_080000d0(void *buf, s32 size);
void Main_080003b8(void *buf, s32 mode);
void Main_08009188(s32 mode);
void Main_08009190(s32 mode);
s32 Main_0808a020(void);
s32 Main_0808a260(s32 value, s32 index);
void Main_0808a268(s32 value, s32 index);
void Main_0808a250(s32 first, s32 second);

extern u8 Data_02008149[];
extern u8 Data_02018000[];
extern s32 Data_02002224[];
extern u8 Value_00000043, Value_00000045, Value_00000058;
extern u8 Value_000000be, Value_000007c7;

s32 Scene_RunScene3cbSequenceB(void)
{
    struct EventWork *work;
    s32 failed;
    s32 window;
    s32 cnt;
    s32 stop;
    u8 *buf;
    u8 *src;
    u8 *dst;
    u16 *tbl;
    u16 value;
    u32 i;

    work = gEventWork;
    failed = 0;
    window = 0;
    cnt = 0;
    if (Engine_GameFlagIsSet(0x173)) {
        Engine_EventBegin();
    } else {
        /* FAKEMATCH: these event callback exits have no defined result. */
        if (!Engine_GameFlagIsSet(0x200))
            return;
        if (Engine_GameFlagIsSet(0x205))
            return;
        Engine_EventBegin();
        Engine_GameFlagSet(0x203);
        LinkLobby_WriteSlotValue(2);
        if (!LinkLobby_PeerSlotMatches(2))
            window = UiText_OpenMessageWindowFar(0x2928, 5, 4, 1);
        while (!LinkLobby_PeerSlotMatches(2)) {
            Engine_TaskWait(1);
            stop = 0;
            if (!Engine_GameFlagIsSet(0x201))
                stop = 1;
            if (Engine_GameFlagIsSet(0x205))
                stop = 1;
            if (!LinkLobby_PeerSlotMatches(2) && !LinkLobby_PeerSlotMatches(1)) {
                if (++cnt > 25)
                    stop = 1;
            } else {
                cnt = 0;
            }
            if (stop) {
                work->raised_trigger = 2;
                Engine_GameFlagSet(0x205);
                Engine_GameFlagClear(0x201);
                Engine_GameFlagClear(0x202);
                LinkLobby_WriteSlotValue(4);
                failed = 1;
                Engine_GameFlagClear(0x200);
                break;
            }
        }
        if (window)
            UiWork_FinalizeFar(window, 1);
        Engine_TaskWait(5);
    }
    if (!failed) {
        buf = Engine_HeapAllocate(54, 0x7c8);
        Main_080000d8(Data_02008149);
        Main_08009188(5);
        Engine_TaskWait(8);
        Main_08009190(5);
        if (Engine_GameFlagIsSet(0x173)) {
            Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
            Engine_EventSetMessage(0x293b);
            Engine_EventOpenMessage(8, 0);
            Engine_TaskWait(45);
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Engine_ActorWalkTo(0, 216, 184);
            Engine_ActorWaitForMove(0);
            Engine_ActorWalkTo(0, 216, 168);
            Engine_ActorWaitForMove(0);
        } else {
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Engine_ActorWalkTo(0, 216, 200);
            Engine_ActorWaitForMove(0);
            Actor_SetSpeed(0, 0x1999, 0xccc);
            Engine_ActorWalkTo(0, 216, 168);
            if (Local_020007b0() < 0) {
                Actor_SetSpeed(0, 0x10000, 0x8000);
                Engine_ActorWalkTo(0, 216, 200);
                Main_08009188(5);
                Engine_TaskWait(8);
                Main_08009190(5);
                Engine_ActorWaitForMove(0);
                Runtime_ReleaseHeapBlock(54);
                LinkLobby_WriteSlotValue(0);
                LinkLobby_WriteSlotValue(4);
                Main_080000d0(Data_02008149, 0xc80);
                Main_080003b8(Data_02008149, 1);
                Engine_GameFlagClear(0x201);
                Engine_GameFlagClear(0x202);
                Engine_GameFlagClear(0x303);
                Engine_GameFlagClear(0x203);
                Engine_GameFlagClear(0x200);
                work->raised_trigger = 2;
                goto done;
            }
            Actor_SetSpeed(0, 0x8000, 0x4000);
            Engine_ActorWaitForMove(0);
        }
        if (Engine_GameFlagIsSet(0x173)) {
            Main_0808a260((s32)&Value_000000be, 8);
            Main_0808a268((s32)&Value_000000be, 9);
        } else {
            Main_0808a260((s32)&Value_000000be, 10);
            Main_0808a268((s32)&Value_000000be, 11);
        }
        gGameState.unknown_1f8[0x33] = 4;
        Main_0808a250(1, 1);
        tbl = (u16 *)Data_02002224;
        value = (u16)(s32)&Value_00000045;
        tbl[1] = (u32)&Value_00000058;
        tbl[0] = value;
        tbl[2] = value;
        tbl[3] = (u32)&Value_00000043;
        i = 0;
        src = buf;
        dst = Data_02018000;
        do {
            i++;
            *dst++ = *src++;
        } while (i <= (u32)&Value_000007c7);
        Runtime_ReleaseHeapBlock(54);
    }
done:
    return Main_0808a020();
}
