#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

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

s32 PartyInventory_FindOwnerFar();
void PartyInventory_RemoveFar();
s32 PartyInventory_FindOwnerFar(s32);
void TakaraAshiba_OpenPassage(s32);

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

void FieldScene_RunScene3b4_02000fdc(s32 a0)
{
    u32 i;
    s32 record;

    if ((a0 & 0x100) != 0) {
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Map_CopyCells(84, 29, 1, 3, 70, 49);
        Task_Wait(60);
    }
    Map_CopyCells(85, 29, 1, 3, 70, 49);
    Map_CopyCellAttributes(6, 49, 1, 1, 6, 50);
    Map_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

void FieldScene_RunScene3b4SequenceC(void)
{
    u32 i;
    s32 record;

    record = Engine_GetTriggerActor(0);
    if (*(u16 *)(record + 6) == 0xc000) {
        if (GameFlag_IsSet(0x9c4) == 0) {
            if (PartyInventory_FindOwnerFar(243) != -1) {
                GameFlag_Set(0x9c4);
                FieldScene_RunScene3b4_02000fdc(0x100);
                PartyInventory_RemoveFar(243);
            }
        }
    }
}

/* Four sites of one import, so four names. */
void SceneState_SetSelectorFlagWhenFacingC000(s32 selector)
{
    u8 *slot = Engine_GetTriggerActor(0);
    s32 flag;

    if (*(u16 *)(slot + 6) != 0xC000) {
        return;
    }
    flag = selector + 2496;
    if (GameFlag_IsSet(flag)!= 0) {
        return;
    }
    if (PartyInventory_FindOwnerFar(244) == -1) {
        return;
    }
    GameFlag_Set(flag);
    TakaraAshiba_OpenPassage(0x100 | selector);
    PartyInventory_RemoveFar(244);
}

void FieldScene_RunIndexedStep0(void)
{
    SceneState_SetSelectorFlagWhenFacingC000(0);
}

void FieldScene_RunIndexedStep1(void)
{
    SceneState_SetSelectorFlagWhenFacingC000(1);
}

void SceneState_ResetCounter412OnHeading4000B(void)
{
    extern u8 *Data_03001ebc;

    u8 *slot;
    s16 *cnt;
    s32 reset;

    SceneState_SetSelectorFlagWhenFacingC000(2);
    slot = Engine_GetTriggerActor(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
        if (*cnt > 12) {
            Leader_CheckAhead();
            reset = 0;
            *cnt = reset;
        }
    }
}

void SceneState_ResetCounter412OnHeading4000(void)
{
    extern u8 *Data_03001ebc;

    u8 *p;
    s16 *cnt;
    s32 zero;

    SceneState_SetSelectorFlagWhenFacingC000(3);
    p = Engine_GetTriggerActor(0);
    if (*(u16 *)(p + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
        if (*cnt > 12) {
            Leader_CheckAhead();
            zero = 0;
            *cnt = zero;
        }
    }
}

void SceneState_ApplyRectAndPlaceSlot12(void)
{
    s32 a = 25;
    s32 b = 48;
    s32 slot = 12;
    s32 x = 0x1980000;
    s32 z = 0x3080000;

    Map_CopyCells(25, 45, 1, 2, a, b);
    if (GameFlag_IsSet(0xeeb) == 0)
        Actor_SetPosition(slot, x, z);
    Event_Wait(1);
}

void ConfigureAndPlaceActorTwelve(void)
{
    s32 a = 25, b = 48;
    Map_CopyCells(24, 48, 1, 2, a, b);
    PlaceActor(12, 0x00080000, 0x00080000);
}

void FieldScene_RunStep8ValueEe7(void)
{
    Item_ShowFound(ITEM_BLUE_KEY, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Party_GiveItem(ITEM_BLUE_KEY, 0);
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(0xEE7);
}

void FieldScene_RunStep9ValueEe8(void)
{
    Item_ShowFound(ITEM_BLUE_KEY, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Party_GiveItem(ITEM_BLUE_KEY, 0);
    Actor_SetPosition(9, 0, 0);
    GameFlag_Set(0xEE8);
}

void FieldScene_RunStep10ValueEe9(void)
{
    Item_ShowFound(ITEM_BLUE_KEY, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Party_GiveItem(ITEM_BLUE_KEY, 0);
    Actor_SetPosition(0xA, 0, 0);
    GameFlag_Set(0xEE9);
}

void FieldScene_RunStep11ValueEea(void)
{
    Item_ShowFound(ITEM_BLUE_KEY, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Party_GiveItem(ITEM_BLUE_KEY, 0);
    Actor_SetPosition(0xB, 0, 0);
    GameFlag_Set(0xEEA);
}

void FieldScene_RunStep12ValueEeb(void)
{
    Item_ShowFound(ITEM_RED_KEY, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Party_GiveItem(ITEM_RED_KEY, 0);
    Actor_SetPosition(0xC, 0, 0);
    GameFlag_Set(0xEEB);
}

s32 *Engine_GetTriggerActor(s32 slot);
s32 Engine_TestTriggerFlag(s32 flag);
void Engine_SetTriggerFlag(s32 flag);

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);
