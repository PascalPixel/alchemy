#ifndef IRIGUCHI_H
#define IRIGUCHI_H

/* The staged-actor code linked here names the imports by the main image's
   names; the entrance's scenes reach them by the same names. */
#define FIELD_STAGED_ACTOR_IMPORTS
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

/* The entrance's exits, in the overlay's data. */
extern u8 gBabiIriguchiExits[];

/* DOOR.C: the door of truth's two leaves slide apart or together. */
void BabiIriguchi_OpenTruthDoor(void);
void BabiIriguchi_CloseTruthDoor(void);

/* The occupant one square ahead of an actor. */
struct StagedActor *BabiIriguchi_FindActorAhead(struct StagedActor *actor);

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
void Engine_MapCopyCellsLayered(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width,
                                s32 height);
void BattleFx_SetPhaseRequest();
void Battle_ResetEffectCounter();
void Motion_LaunchFromFocusedObject();
void BattleFx_RunRisingObjectSequence();
s32 FieldEffect_UpdateGridPlacement();
s32 battle_owner_69();
void BabiIriguchi_JumpFromLedge();

/* The event services the entrance's scripts use, wrapped as the field event
 * header wraps its own but reaching the imports by the names above. */

static __inline__ void Iriguchi_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Iriguchi_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                                   s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dest_x, dest_y);
}

static __inline__ s32 Iriguchi_Divide(s32 dividend, s32 divisor)
{
    return dividend / divisor;
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

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001238(s32 amount)
{
    gEventWork->message += amount;
}

#endif
