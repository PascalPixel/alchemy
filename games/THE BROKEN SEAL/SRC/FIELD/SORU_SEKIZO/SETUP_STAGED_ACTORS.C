#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/*
 * The statue hall of Sol Sanctum, after Robin drops the statue into the
 * hole it opened. Sukureta comes down to look, Gerald and Jasmine tell him
 * what happened, and he decides the trap is disarmed before withdrawing to
 * the Luna room to watch from safety.
 */

enum StatueHallActor {
    ACTOR_JASMINE = 5,
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

/* Each line follows the one before; only the first is set. */
enum StatueHallMessage {
    MSG_SUKURETA_YOU_FOUND_IT = 0x101a,
    MSG_JASMINE_SUKURETA,
    MSG_SUKURETA_EXCELLENT_WORK,
    MSG_SUKURETA_WHERE_DID_THE_HOLE_COME_FROM,
    MSG_GERALD_BEAMS_OF_LIGHT_OPENED_IT,
    MSG_JASMINE_THEN_THE_STATUE_DROPPED,
    MSG_SUKURETA_GOOD_WORK_EVERYONE,
    MSG_SUKURETA_THE_PUZZLE_WAS_THE_TRAP,
    MSG_SUKURETA_THE_STATUE_DISARMED_IT,
    MSG_SUKURETA_WE_SHOULD_BE_ALL_RIGHT,
    MSG_SUKURETA_I_WILL_WATCH_FROM_LUNA
};

/* Cleared once the hall is safe. */
enum {
    FLAG_STATUE_HALL_TRAP_ARMED = 0x12f
};

void Event_SayThenWait(s32 speaker, s32 frames);
/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_SetupStagedActors(void)
{
    struct FieldActor *leader;

    Event_Begin();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0xe80000);
    Event_Wait(1);
    Event_SetMessage(MSG_SUKURETA_YOU_FOUND_IT);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0x1180000);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 216);
    Event_Wait(20);
    Actor_Jump(ACTOR_JASMINE, 2, 0);
    Event_Wait(30);
    Event_SayThenWait(ACTOR_JASMINE, 6);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(6);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST + FACING_STEP, 0);
    Event_Wait(10);
    Camera_SetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
    Camera_WaitForMove();
    Event_Wait(60);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Event_Wait(10);
    Actor_Jump(ACTOR_SUKURETA, 6, 0);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x30000, 0x20000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 184);
    Event_Wait(40);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 0);
    Event_Wait(40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    gEventWork->transition_frames = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1a80000, 0xc80000);
    Task_Wait(1);
    Event_SayThenWait(ACTOR_SUKURETA, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0xb80000);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0x23e0000, -1, 0x9d0000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SayThenWait(ACTOR_GERALD, 20);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Event_SayThenWait(ACTOR_JASMINE, 6);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_SHAKE_HEAD);
    Event_SayThenWait(ACTOR_SUKURETA, 80);
    Actor_ShowEmote(ACTOR_SUKURETA, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Event_Wait(30);
    Event_SayThenWait(ACTOR_SUKURETA, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_JASMINE, EMOTE_IN_FRONT | 1, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_SHAKE_HEAD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 1, 0);
    Event_Wait(40);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 5, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_JASMINE, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x2400000, -1, 0xd70000, 1);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_SOUTH, 0);
    Event_Wait(10);
    Actor_Jump(ACTOR_SUKURETA, 6, 0);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 217);
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 0x141);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Camera_SetSpeed(0x39999, 0x7333);
    Camera_MoveTo(0x2400000, -1, 0x880000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_JASMINE, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_JASMINE, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_JASMINE);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    GameFlag_Clear(FLAG_STATUE_HALL_TRAP_ARMED);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    gEventWork->transition_frames = 16;
    Event_End();
}
