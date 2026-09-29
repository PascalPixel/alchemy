/* Draft of resource_3ad 0x02009448..0x02009760 (792 bytes with pool),
 * Party_StaysBehind; the listing keeps the rows. Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (Engine_ActorShowEmote,
 * Engine_ActorRunRepeatedMotion, Engine_EventWait,
 * Engine_ActorSetAnimationAndWait, Engine_ActorFaceDirection,
 * Engine_ActorSetDestinationOffset, ...). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/CAVE.H"
extern u8 MsgRunpaInsistStick[];

/*
 * Hammet and Bunza say goodbye and leave for the wagon; the party watches
 * them go, then falls in behind the leader.
 */
void Party_StaysBehind(void)
{
    struct FieldActor *leader;
    s32 farewell;

    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 5, 60);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    farewell = (s32)MsgRunpaInsistStick;
    Event_SetMessage(farewell + FAREWELL_GERALD_STAYS);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_SetMessage(farewell + FAREWELL_MIA_STAYS);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    Event_Wait(60);
    Event_SetMessage(farewell + FAREWELL_IVAN_STAYS);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 5, 70);
    Event_SetMessage(farewell + FAREWELL_HAMMET_LETS_IVAN_GO);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_SetMessage(farewell + FAREWELL_BUNZA_SAYS_GOODBYE);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Event_Wait(60);
    Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
    Actor_WaitForMove(ACTOR_HAMMET);
    Actor_SetAnimation(ACTOR_HAMMET, ANIM_STAND);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_WalkTo(ACTOR_BUNZA, 156, 528);
    Event_Wait(20);
    Actor_WalkTo(ACTOR_HAMMET, 164, 528);
    Actor_WaitForMove(ACTOR_BUNZA);
    Actor_WalkTo(ACTOR_BUNZA, 168, 640);
    Actor_WaitForMove(ACTOR_HAMMET);
    Actor_WalkTo(ACTOR_HAMMET, 168, 640);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Event_Wait(60);
    Actor_SetPosition(ACTOR_HAMMET, 0, 0);
    Actor_SetPosition(ACTOR_BUNZA, 0, 0);
    Event_Wait(110);

    Event_SetMessage(farewell + FAREWELL_GERALD_SIGHS);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessage(farewell + FAREWELL_MIA_HOPES_FOR_SAFETY);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_SetMessage(farewell + FAREWELL_IVAN_REASSURES);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(140);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage(farewell + FAREWELL_GERALD_MOVES_ON);
    Event_ShowMessage(ACTOR_GERALD, 0);

    Actor_SetAnimation(ACTOR_GERALD, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_Wait(30);
    Camera_MoveToActor(ACTOR_PARTY_LEADER, 1);
    Camera_WaitForMove();
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    GameFlag_Set(FLAG_PARTY_STAYED_IN_LUNPA);
}
