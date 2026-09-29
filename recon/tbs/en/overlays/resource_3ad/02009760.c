/* Draft of resource_3ad 0x02009760..0x02009a0c (684 bytes with pool),
 * Party_RidesWagon; the listing keeps the rows. Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines
 * (Engine_ActorFaceDirection, Engine_ActorSetAnimationAndWait,
 * Engine_EventWait, Engine_ActorSetDestinationOffset,
 * Engine_ActorWaitForMove, Engine_ActorWalkTo, ...). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/CAVE.H"
extern u8 MsgRunpaDontUnfinishedBusiness[];

/*
 * Everyone heads for the wagon: Hammet and Bunza lead, the party falls in
 * behind the leader, and the leader follows them out of the cave.
 */
void Party_RidesWagon(void)
{
    struct FieldActor *leader;
    s32 departure;

    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    departure = (s32)MsgRunpaDontUnfinishedBusiness;
    Event_SetMessage(departure + DEPARTURE_GERALD_HEADS_FOR_KALAY);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_SetMessage(departure + DEPARTURE_IVAN_THINKS_OF_LAYANA);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Event_Wait(20);
    Event_SetMessage(departure + DEPARTURE_HAMMET_LONGS_FOR_LAYANA);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessage(departure + DEPARTURE_BUNZA_SETS_OFF);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Event_Wait(80);
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
    Actor_WalkTo(ACTOR_BUNZA, 152, 528);
    Event_Wait(20);
    Actor_WalkTo(ACTOR_HAMMET, 160, 528);
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
    Event_Wait(200);
    Actor_SetPosition(ACTOR_HAMMET, 0, 0);
    Actor_SetPosition(ACTOR_BUNZA, 0, 0);

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
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -16, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Camera_MoveToActor(ACTOR_PARTY_LEADER, 1);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 640);
    Event_Wait(60);
    Event_CloseScreen();
    Event_RequestExit(CAVE_EXIT_BY_WAGON);
}
