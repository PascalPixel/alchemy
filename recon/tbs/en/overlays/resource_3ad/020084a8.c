/* Draft of resource_3ad 0x020084a8..0x02008828 (896 bytes with pool),
 * Reunion_Begin; the listing keeps the rows. Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (Engine_EventBegin,
 * Engine_ActorSetPosition, Engine_ActorSetSpeed, Engine_ActorWalkTo,
 * Engine_ActorWaitForMove, Engine_ActorFaceDirection, ...). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/CAVE.H"
extern u8 MsgRunpaMoment[];
extern u8 MsgRunpaThinkSawSomeone[];

/*
 * Once Hammet is free, Bunza steps out from where he hid, Hammet and the
 * party gather around him, and the two recognize each other.
 */
void Reunion_Begin(void)
{
    struct FieldActor *leader;
    s32 sighting;
    s32 recognition;

    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        GameFlag_Set(FLAG_LUNPA_CAVE_REUNION_SEEN);
        Event_Begin();
        Actor_SetPosition(ACTOR_BUNZA, PIXELS(144), PIXELS(400));
        Actor_SetSpeed(ACTOR_BUNZA, 0x18000, 0xc000);
        Actor_WalkTo(ACTOR_BUNZA, 184, 400);
        Actor_WaitForMove(ACTOR_BUNZA);
        Actor_SetAnimation(ACTOR_BUNZA, ANIM_STAND);
        Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
        Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Event_Wait(30);
        Camera_SetSpeed(0x8000, 0x1000);
        Camera_MoveTo(PIXELS(192), -1, PIXELS(432), 1);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Actor_SetPosition(ACTOR_HAMMET, leader->x.fixed, leader->z.fixed);
        }
        Actor_SetSpeed(ACTOR_HAMMET, 0x14ccc, 0xa666);
        Actor_WalkTo(ACTOR_HAMMET, 168, 464);
        Actor_WaitForMove(ACTOR_HAMMET);
        Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH, 0);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
        }
        Actor_SetSpeed(ACTOR_IVAN, 0x14ccc, 0xa666);
        Actor_WalkTo(ACTOR_IVAN, 152, 488);
        Actor_WaitForMove(ACTOR_IVAN);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
        }
        Actor_SetSpeed(ACTOR_MIA, 0x14ccc, 0xa666);
        Actor_WalkTo(ACTOR_MIA, 168, 488);
        Actor_WaitForMove(ACTOR_MIA);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Actor_SetPosition(ACTOR_GERALD, leader->x.fixed, leader->z.fixed);
        }
        Actor_SetSpeed(ACTOR_GERALD, 0x14ccc, 0xa666);
        Actor_WalkTo(ACTOR_GERALD, 184, 488);
        Actor_WaitForMove(ACTOR_GERALD);
        Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
        Event_Wait(30);

        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        sighting = (s32)MsgRunpaThinkSawSomeone;
        Event_SetMessage(sighting + SIGHTING_GERALD_SAW_SOMEONE);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        Event_Wait(10);
        Event_SetMessage(sighting + SIGHTING_MIA_SAW_SOMETHING);
        Event_ShowMessage(ACTOR_MIA, 0);
        Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 70);
        Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Event_SetMessage(sighting + SIGHTING_IVAN_ASKS_IF_FOUND);
        Event_OpenMessage(ACTOR_IVAN, 0);
        Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
        Event_Wait(30);
        if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            Event_SetMessage(sighting + SIGHTING_GERALD_WILL_FIGHT);
            Event_ShowMessage(ACTOR_GERALD, 0);
        } else {
            Event_SetMessage(sighting + SIGHTING_GERALD_ASKS_WHAT_ELSE);
            Event_ShowMessage(ACTOR_GERALD, 0);
        }

        Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 70);
        recognition = (s32)MsgRunpaMoment;
        Event_SetMessage(recognition + RECOGNITION_HAMMET_CALLS_OUT);
        Event_ShowMessage(ACTOR_HAMMET, 0);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
        Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
        Actor_StartRepeatedMotion(ACTOR_MIA, 2);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Actor_SetAnimation(ACTOR_HAMMET, ANIM_WALK);
        Actor_WalkBy(ACTOR_HAMMET, 0, -16);
        Actor_WaitForMove(ACTOR_HAMMET);
        Actor_SetAnimation(ACTOR_HAMMET, ANIM_STAND);
        Event_SetMessage(recognition + RECOGNITION_HAMMET_NAMES_BUNZA);
        Event_ShowMessage(ACTOR_HAMMET, 0);
        Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 65);
        Event_SetMessage(recognition + RECOGNITION_BUNZA_KNOWS_VOICE);
        Event_ShowMessage(ACTOR_BUNZA, 0);
        Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
        Event_Wait(80);
        Actor_SetSpeed(ACTOR_BUNZA, 0x6666, 0x3333);
        Actor_WalkBy(ACTOR_BUNZA, -13, 0);
        Actor_WaitForMove(ACTOR_BUNZA);
        Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
        Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
        Event_SetMessage(recognition + RECOGNITION_BUNZA_NAMES_HAMMET);
        Event_ShowMessage(ACTOR_BUNZA, 0);
        Actor_WalkTo(ACTOR_BUNZA, 168, 432);
        Event_Wait(40);
        Event_CloseScreen();
        Event_WaitForScreen();
        Event_Wait(20);
        Event_End();
        Reunion_Converse();
    }
}
