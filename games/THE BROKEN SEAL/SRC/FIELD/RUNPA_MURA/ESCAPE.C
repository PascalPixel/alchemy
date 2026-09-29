#include "VILLAGE.H"
extern u8 MsgRunpaGuardForbidsReturn[];

void Gateway_Reopen(void)
{
    Map_CopyCellAttributes(7, 12, 1, 1, 7, 11);
    Map_CopyCellAttributes(7, 12, 1, 1, 8, 11);
    Map_CopyCellAttributes(7, 12, 1, 1, 9, 11);
}

/* A party still near the gate when Cloak fades is spotted. */
void Cloak_End(void)
{
    struct EventWork *work;
    struct FieldActor *leader;

    work = gEventWork;
    GameFlag_Clear(FLAG_GATE_CLOAK_CAST);
    GameFlag_Clear(FLAG_GATE_GUARDS_BLOCKING);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader->x.fixed > PIXELS(104) && leader->x.fixed < PIXELS(240)
        && leader->z.fixed > PIXELS(160) && leader->z.fixed < PIXELS(248)) {
        Task_RemoveCallback(Guards_Watch);
        work->raised_trigger = TRIGGER_PARTY_SPOTTED;
    }
    Gateway_Reopen();
    GameFlag_Clear(FLAG_GATE_PARTY_CAUGHT);
}

/* An uncloaked party that walks up to the gate is spotted. */
void Guards_Watch(void)
{
    struct EventWork *work;
    struct FieldActor *leader;

    work = gEventWork;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (gGameState.cloaked == 0 && (u32)(leader->x.fixed - PIXELS(144)) <= PIXELS(32)
        && leader->z.fixed >= PIXELS(168) && leader->z.fixed < PIXELS(176)) {
        Task_RemoveCallback(Guards_Watch);
        work->raised_trigger = TRIGGER_PARTY_SPOTTED;
    }
}

/* Every sixteenth frame the dragged leader kicks up a puff of dust. */
void Leader_KickUpDust(void)
{
    struct FieldActor *leader;
    s32 angle;
    s32 velocity[3];

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if ((gFrameCount & 15) == 0) {
        angle = (((u32)Random_Next() * 52) >> 16) * 64 + 230;
        velocity[0] = Math_Cos(angle) / 4;
        velocity[1] = 0;
        velocity[2] = Math_Sin(angle) / 2;
        Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocity[0], velocity[1],
                     velocity[2], 0, NULL);
    }
}

/*
 * The guards drag the party caught inside the fortress down to the gate
 * road and throw it; the leader bounces twice and lies sprawled.
 */
void Party_ThrownOut(void)
{
    Event_Begin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(160), PIXELS(128));
    Actor_SetPosition(ACTOR_LEFT_GUARD, PIXELS(152), PIXELS(112));
    Actor_SetPosition(ACTOR_RIGHT_GUARD, PIXELS(168), PIXELS(112));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    /* The script also turns the village guards, whom the gate does not place. */
    Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Event_OpenScreen();
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1cccc, 0xe666);
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x1cccc, 0xe666);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x1cccc, 0xe666);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 288);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 288);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 160, 296);
    Task_AddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
    Event_Wait(1);
    Audio_PlayCue(SOUND_SCUFFLE);
    Event_Wait(20);
    Actor_SetSpritePriority(ACTOR_LEFT_GUARD, 3);
    Actor_SetSpritePriority(ACTOR_RIGHT_GUARD, 3);
    Audio_PlayCue(SOUND_SCUFFLE);
    Event_Wait(30);
    Actor_Get(ACTOR_LEFT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_RIGHT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    Audio_PlayCue(SOUND_SCUFFLE);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    Task_RemoveCallback(Leader_KickUpDust);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags |= ACTOR_FALLS;
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = PIXELS(6);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_z = PIXELS(6);
    Event_Wait(1);
    while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
        Event_Wait(1);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
    Audio_PlayCue(SOUND_LANDING_THUD);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Task_AddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
    Event_Wait(2);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = PIXELS(3);
    Event_Wait(1);
    while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
        Event_Wait(1);
    }
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Task_RemoveCallback(Leader_KickUpDust);
    Event_Wait(50);
    Event_SetMessage((s32)MsgRunpaGuardForbidsReturn);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_Get(ACTOR_LEFT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_RIGHT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 144, 200);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 176, 200);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Event_End();
}

/* The leader leaving the fortress casts Cloak and slips around the guards. */
void Leader_SneaksOut(void)
{
    Event_Begin();
    Event_OpenScreen();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 152, 168);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_Wait(20);
    Psynergy_Begin(ABILITY_CLOAK, 1);
    Psynergy_SetTarget(ACTOR_PARTY_LEADER, 0);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 144, 184);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 184);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 200);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 200);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 288);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 288);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_End();
}
