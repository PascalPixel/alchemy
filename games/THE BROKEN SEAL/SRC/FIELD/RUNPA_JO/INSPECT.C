/* The Lunpa fortress: the interaction regions and the searchable objects. */
#include "FORTRESS.H"
extern u8 MsgFieldFlippedSwitch[];
extern u8 MsgRunpaPrepareBecomeMonster[];

void ConfigureInteractionRegionA(void)
{
    Map_CopyCells(2, 82, 1, 2, 21, 81);
    Map_CopyCellAttributes(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
    Map_CopyCells(2, 84, 1, 2, 6, 55);
    Map_CopyCellAttributes(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
    Map_CopyCells(2, 86, 1, 2, 27, 62);
    Map_CopyCellAttributes(26, 16, 1, 1, 27, 17);
}

void InspectVillageWell(void)
{

    if (*(s16 *)(((u8*)gEventWork) + 0xcb8) != 0) {
        if (GameFlag_IsSet(0x947) == 0) {
            Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(188);
            Event_Wait(1);
            Map_CopyCells(6, 77, 1, 2, 17, 82);
            Event_Wait(5);
            Map_CopyCells(7, 77, 1, 2, 17, 82);
            Event_Wait(1);
            ConfigureInteractionRegionA();
            GameFlag_Set(0x947);
        }
    }
}

void RunSecondaryMapInteraction(void)
{

    if (*(s16 *)(((u8*)gEventWork) + 0xcb8) != 0) {
        if (GameFlag_IsSet(0x948) == 0) {
            Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(188);
            Event_Wait(1);
            Map_CopyCells(6, 77, 1, 2, 3, 55);
            Event_Wait(5);
            Map_CopyCells(7, 77, 1, 2, 3, 55);
            Event_Wait(1);
            ConfigureInteractionRegionB();
            GameFlag_Set(0x948);
        }
    }
}

void ConfigurePrimaryInteractionRegions(void)
{
    Map_CopyCells(5, 77, 1, 2, 17, 82);
    Map_CopyCells(5, 77, 1, 2, 3, 55);
    Map_CopyCellAttributes(15, 33, 1, 1, 17, 35);
    Map_CopyCellAttributes(3, 8, 1, 1, 3, 10);
}

void ConfigureSecondaryInteractionRegions(void)
{
    Map_CopyCells(8, 77, 1, 2, 17, 82);
    Map_CopyCells(8, 77, 1, 2, 3, 55);
    Map_CopyCellAttributes(18, 35, 1, 1, 17, 35);
    Map_CopyCellAttributes(2, 10, 1, 1, 3, 10);
}

void InspectWardrobe(void)
{
    GameFlag_Set(2372);
    GameFlag_Clear(535);
    Actor_SetPosition(8, 0, 0);
}

void InspectFirewood(void)
{
    GameFlag_Set(2373);
    ConfigureInteractionRegionC();
    Actor_SetPosition(9, 0, 0);
}

void InspectBooks(void)
{
    GameFlag_Set(2374);
    GameFlag_Clear(536);
    Actor_SetPosition(10, 0, 0);
}

void NoOpInteractionCallback(void)
{
}

void FieldScene_RunScene3bf_0200252c(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetSpeed(ACTOR_IVAN, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_IVAN, 0x1c8, 192);
    Actor_SetSpeed(ACTOR_MIA, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_MIA, 0x1b8, 184);
    Actor_SetSpeed(ACTOR_GERALD, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_GERALD, 0x1c0, 240);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceActor(ACTOR_IVAN, 12, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Actor_FaceActor(ACTOR_MIA, 12, 0);
    Event_Wait(15);
}

void FieldScene_RunScene3bf_020025f8(void)
{
    u32 i;
    s32 record;

    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Audio_PlayCue(141);
    Event_Wait(80);
    Audio_PlayCue(0x120);
    Event_Wait(5);
    Audio_PlayCue(145);
    Map_CopyCells(16, 75, 7, 4, 26, 55);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Engine_ActorShowEmote(12, 0x100, 0);
    Event_Wait(60);
}

void FieldScene_RunScene3bf_0200269c(void)
{
    u32 i;
    s32 record;

    Camera_MoveToActor(11, 1);
    Camera_WaitForMove();
    Event_Wait(60);
    Event_SetMessage((s32)MsgRunpaPrepareBecomeMonster);
    Event_ShowMessage(13, 0);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(15, 0x10000, 0x8000);
    Actor_WalkTo(11, 0x1d8, 180);
    Actor_WalkTo(15, 0x1d8, 180);
    Camera_FollowActor(11, 1);
    Actor_WaitForMove(11);
    Actor_SetAnimation(11, 4);
    Event_Wait(30);
}

void FieldScene_RunScene3bf_02002718(void)
{
    u32 i;
    s32 record;

    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_IVAN, 0x1f8, 216);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_MIA, 0x1b8, 232);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 0x1e0, 224);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
}
