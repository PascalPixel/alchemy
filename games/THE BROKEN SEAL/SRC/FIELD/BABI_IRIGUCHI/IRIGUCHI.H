#ifndef IRIGUCHI_H
#define IRIGUCHI_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#include "FIELD_EFFECT.H"
#include "STAGED_ACTOR.H"
#include "SPAWN_CONFIGURED_EFFECT.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
extern u8 gEffectWork[];

enum BranchingEventMessage {
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528,
    MSG_SEEMS_LOCKED = 0x2693,
    MSG_YOURE_SURE_THEY_WENT_THROUGH = 0x2702,
    MSG_THE_DOOR = 0x272f,
    MSG_TRUTH_DOOR_OPEN_THOSE_SEEING = 0x2756
};

struct LevelCheckRecord {
    u8 unknown_00[12];
    s32 y;                          /* +0x0c */
};

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

/*
 * Per-frame integrator for an effect record -- resource_3c5. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
struct MotionEffect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Obj {
    u8 pad00[6];
    u16 f06;
};

/* The entrance's exits, in the overlay's data. */
extern u8 gBabiIriguchiExits[];

/* The occupant one square ahead of an actor. */
struct LevelCheckRecord *BabiIriguchi_FindActorAhead();

/* Main-image services reached through the overlay's import veneers. The
 * staged-actor code linked into this overlay names several of them, so the
 * entrance's scenes call them by those names too. */
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Object_SetModeById(s32 actor, s32 mode);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height,
                               s32 dest_x, s32 dest_y);
s32 IwramSignedDivideEntry(s32 dividend, s32 divisor);
void BattleFx_SetPhaseRequest();
void Battle_ResetEffectCounter();
void Motion_LaunchFromFocusedObject();
void BattleFx_RunRisingObjectSequence();
s32 FieldEffect_UpdateGridPlacement();
s32 battle_owner_69();
void BabiIriguchi_JumpFromLedge();

/* The event services the entrance's scripts use, wrapped as the field event
 * header wraps its own but reaching the imports by the names above. */
static __inline__ void Iriguchi_Wait(s32 frames)
{
    Battle_WaitMode0(frames);
}

static __inline__ void Iriguchi_TaskWait(s32 frames)
{
    WaitFrames(frames);
}

static __inline__ void Iriguchi_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Iriguchi_WaitForMove(s32 actor)
{
    ObjectMotion_CommitCurrentPositionAndActivate(actor);
}

static __inline__ void Iriguchi_SetAnimation(s32 actor, s32 animation)
{
    Object_SetModeById(actor, animation);
}

static __inline__ void Iriguchi_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                                   s32 dest_x, s32 dest_y)
{
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dest_x, dest_y);
}

static __inline__ s32 Iriguchi_Divide(s32 dividend, s32 divisor)
{
    return IwramSignedDivideEntry(dividend, divisor);
}

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

/* Copy the low two mode bits into the object's owner record. */

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */
static __inline__ void ResetSceneParameters(s32 a, s32 b, s32 c, s32 mode)
{
    Camera_MoveTo(a, b, c, mode);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001238(s32 amount)
{
    gEventWork->message += amount;
}


#endif
