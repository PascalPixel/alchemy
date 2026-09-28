/* The Lunpa fortress: the other supports, the cell doors, the guards' items
 * and the cell key. */
#include "FORTRESS.H"

void FieldScene_UpdateObjectPairB(void)
{

    struct EventWork *work;
    s32 trigger;

    work = gEventWork;
    if (PartyInventory_FindOwner(234) != -1) {
        trigger = work->touched_trigger;
        FieldScene_SetPositionPairs(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x32d);
    }
}

void PlaceSceneObjectPairFromTableB(s32 table_index)
{

    s32 position_x = gRunpaJoPairTableB[table_index * 2];
    s32 position_z = gRunpaJoPairTableB[table_index * 2 + 1];

    Map_CopyCells(0x37, 0x79, 1, 3, position_x, position_z);
    Map_CopyCells(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Map_CopyCells(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateTableBObjectPair(void)
{

    struct EventWork *work;
    s32 trigger;

    work = gEventWork;
    if (PartyInventory_FindOwner(234) != -1) {
        trigger = work->touched_trigger;
        PlaceSceneObjectPairFromTableB(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x330);
    }
}

void PlaceSceneObjectPairFromTableC(s32 table_index)
{

    s32 position_x = gRunpaJoPairTableC[table_index * 2];
    s32 position_z = gRunpaJoPairTableC[table_index * 2 + 1];

    Map_CopyCells(1, 0x50, 1, 3, position_x, position_z);
    Map_CopyCells(2, 0x50, 1, 1, position_x + 1, position_z);
    Map_CopyCellAttributes(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateObjectPairC(void)
{

    struct EventWork *work;
    s32 trigger;

    work = gEventWork;
    if (PartyInventory_FindOwner(234) != -1) {
        trigger = work->touched_trigger;
        PlaceSceneObjectPairFromTableC(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x332);
    }
}

void CellDoor_Touch(void)
{
    if (PartyInventory_FindOwner(ITEM_CELL_KEY) == -1) {
        Message_ShowCentered(0x953, 1);
    }
}

void LockedDoor_Touch(void)
{
    Message_ShowCentered(0x953, 1);
}

void Actor8_Interact(void)
{
    if (TryStartActorInteraction(8, 8) != 0) {
        GameFlag_Set(0xf2a);
    }
}

void Actor9_Interact(void)
{
    if (TryStartActorInteraction(9, 7) != 0) {
        GameFlag_Set(0xf2b);
    }
}

void Actor10_Interact(void)
{
    if (TryStartActorInteraction(10, 6) != 0) {
        GameFlag_Set(0xf2c);
    }
}

void Actor11_Interact(void)
{
    if (TryStartActorInteraction(11, 5) != 0) {
        GameFlag_Set(0xf2d);
    }
}

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
    s32 started = 0;
    s32 interaction;

    Event_Begin();
    interaction = BattleFx_PlayCueAndStartEmitterOnTarget(0, actor_id, interaction_id);
    if (Party_GiveItem(interaction_id, 0) != -1) {
        Actor_SetAnimation(actor_id, 2);
        started = 1;
    } else {
        Audio_PlayCue(0x7d);
        Actor_SetAnimation(actor_id, 5);
    }
    Engine_ObjectDispatchRelease(interaction);
    Event_End();
    return started;
}

void NoOpSceneCallbackA(void)
{
}

void NoOpSceneCallbackB(void)
{
}

void NoOpSceneCallbackC(void)
{
}

void NoOpSceneCallbackD(void)
{
}

void CellKey_PickUp(void)
{

    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x108, 0x318);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Item_ShowFound(ITEM_CELL_KEY, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Party_GiveItem(ITEM_CELL_KEY, 0);
    GameFlag_Set(0xf2e);
    Actor_SetPosition(8, 0, 0);
}

s32 IsPlayerInAccidentTriggerArea(void)
{
    SceneActor *player = Object_GetById(0);
    s32 z = player->z;
    s32 x;
    s32 zz, xx;

    if (z < 0) {
        z += 0xfffff;
    }
    x = player->x;
    zz = z >> 20;
    if (x < 0) {
        x += 0xfffff;
    }
    xx = x >> 20;
    if ((u32)(zz - 5) <= 2 && xx <= 10) {
        return 1;
    }
    if ((u32)(xx - 8) <= 1 && zz > 22) {
        return 1;
    }
    return 0;
}
