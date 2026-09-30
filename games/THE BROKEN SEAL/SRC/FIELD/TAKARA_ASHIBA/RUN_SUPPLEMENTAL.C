#include "TYPES.H"
#include "FIELD_EVENT.H"
extern s16 Data_02000240[];
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
#include "CALL.H"

enum {
    /* Message 0x182 + 243. */
    ITEM_RED_KEY = 243,
    /* Message 0x182 + 244. */
    ITEM_BLUE_KEY = 244
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

extern s32 TakaraAshiba_SlotColumns[];

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

s32 *Engine_GetTriggerActor(s32 slot);
s32 Engine_TestTriggerFlag(s32 flag);
void Engine_SetTriggerFlag(s32 flag);

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
    Actor_SetSpeed(8, 0x1e666, 0xf333);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(8);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
    Event_Wait(4);
    Audio_PlayCue(188);
    Actor_SetDestinationOffset(8, 0, 16);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(8, 0x168, 152);
    Actor_WaitForMove(8);
    Event_End();
    GameFlag_Clear(0x220);
}

void SceneState_TriggerColumnTen(void)
{
    extern u8 *Data_03001ebc;
    s32 *pos = Engine_GetTriggerActor(0);
    s32 x = pos[2] / 0x100000;
    s32 z = pos[4] / 0x100000;
    u8 *work = Data_03001ebc;

    if (Value1(Engine_TestTriggerFlag, 0x220) == 0 &&
        Data_02000240[0x24c / 2] == 0 && Data_02000240[0x24a / 2] != 9 &&
        x == 10 && (u32)(z - 16) <= 2) {
        Call1(Engine_SetTriggerFlag, 0x220);
        SceneState_StoreStep((s16 *)(work + 386), 92);
    }
}

void FieldScene_RunScene3b4SequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
    Actor_SetSpeed(9, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(9);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
    Audio_PlayCue(188);
    Event_Wait(4);
    Actor_SetDestinationOffset(9, 0, 16);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(9, 168, 0x108);
    Actor_WaitForMove(9);
    Event_End();
    GameFlag_Clear(0x220);
}

void SceneActor_TrackOriginColumnForSlot(s32 no)
{

    s32 *pos = Engine_GetTriggerActor(0);
    s32 col = pos[2] / 0x100000;   /* +8  */
    s32 row = pos[4] / 0x100000;      /* +16 */
    s32 slot = no + 10;

    if (Data_02000240[293] == slot) return;
    if (col == TakaraAshiba_SlotColumns[no]) return;

    Actor_SetSpeed(slot, 0x48000, 0x24000);
    Audio_PlayCue(188);
    Actor_SetDestination(slot, (col << 4) + 8, 360);

    TakaraAshiba_SlotColumns[no] = col;

    if (row <= 22) {
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
    }
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
}

void FieldScene_RunLateIndexedStep0(void)
{
    SceneActor_TrackOriginColumnForSlot(0);
}

void FieldScene_RunLateIndexedStep1(void)
{
    SceneActor_TrackOriginColumnForSlot(1);
}

void FieldScene_RunLateIndexedStep2(void)
{
    SceneActor_TrackOriginColumnForSlot(2);
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 record;
    s32 base3_2000240;

    base3_2000240 = (s32)Data_02000240;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 10) {
        Event_Begin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
        Actor_SetSpeed(10, 0x1b333, 0xd999);
        Audio_PlayCue(188);
        record = Engine_GetTriggerActor(0);
        if (record != 0) {
            Actor_SetDestination(10, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(10);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
        Event_Wait(4);
        Audio_PlayCue(188);
        Actor_SetDestinationOffset(10, 0, 16);
        Actor_WaitForMove(ACTOR_PARTY_LEADER);
        Actor_SetDestination(10, 0x108, 0x168);
        Actor_WaitForMove(10);
        Event_Wait(10);
        Event_End();
    }
}

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);
