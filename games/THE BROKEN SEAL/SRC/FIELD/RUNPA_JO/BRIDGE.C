/* The Lunpa fortress: actor 12's drop and the first bridge supports. */
#include "FORTRESS.H"
#include "CALL.H"

void ConfigureSceneActor12(void)
{

    s32 actor_slot = 15;
    u8 *actor;

    Map_CopyCellAttributes(15, 20, 1, 1, actor_slot, 22);
    Map_CopyCellAttributes(17, 23, 1, 3, actor_slot, 23);
    actor = Object_GetById(12);
    if (actor != 0) {
        Actor_SetSpriteFlags(actor, 0);
        actor[0x55] = 0;
        actor[0x23] = 2;
    }
}

void RunSceneObjectSetup(void)
{

    StagedActor_AdvancePair();
}

void FieldScene_StartActorTwelveTransition(void)
{

    Actor_SetSpeed(12, 0x10000, 0x8000);
    Actor_SetDestination(12, 248, 0x178);
    Actor_WaitForMove(12);
    Audio_PlayCue(215);
    Event_Wait(60);
    ConfigureSceneActor12();
    GameFlag_Set(0x943);
}

void FieldScene_UpdateActorTwelveTransition(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Object_GetById(12);
    if ((actor->z.fixed >> 20) > 22) {
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Audio_PlayCue(144);
        Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
        Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
        actor = (struct FieldActor *)Value1(Object_GetById, 12);
        if (actor != NULL) {
            Actor_SetSpriteFlags(actor, 0);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        }
        GameFlag_Set(0x943);
    }
}

void PlaceActorTwelveAndFinishScene(void)
{
    PlaceActor(12, 0x00f80000, 0x01780000);
    ConfigureSceneActor12();
}

void PlaceSceneObjectPairFromTableA(s32 table_index)
{

    s32 position_x = gRunpaJoPairTableA[table_index * 2];
    s32 position_z = gRunpaJoPairTableA[table_index * 2 + 1];

    Map_CopyCells(0, 0x4d, 1, 3, position_x, position_z);
    Map_CopyCells(1, 0x4d, 1, 1, position_x + 1, position_z);
    Map_CopyCells(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void FieldScene_UpdateObjectPairA(void)
{

    struct EventWork *work;
    s32 trigger;
    s32 index;

    work = gEventWork;
    if (PartyInventory_FindOwner(234) != -1) {
        trigger = work->touched_trigger;
        index = trigger - 40;
        if (GameFlag_IsSet(0x941) == 0 || index != 4) {
            PlaceSceneObjectPairFromTableA(index);
            Audio_PlayCue(157);
            Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            GameFlag_Set(trigger + 0x328);
        }
    }
}
