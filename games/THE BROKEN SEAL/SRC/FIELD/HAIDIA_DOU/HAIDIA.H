#ifndef ALCHEMY_HAIDIA_H
#define ALCHEMY_HAIDIA_H

/* The Vale sanctum (resource_3a6): what its files share. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

/* resource_3a6 owner at 0x02001938, 76 bytes.
 * Fetches a record for the caller's id and, when it exists, marks it and
 * writes two 20.12 fixed-point coordinates biased by half a unit. */
struct Rec_3a6 {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[14];
    u8 f34;                     /* +34 */
    u8 f35;                     /* +35 */
};

/* Tables laid out after the code. */
extern const u32 HaidiaDou_Exits[];
extern const s32 HaidiaDou_WalkTargets[];

s32 SceneActor_FaceActorZero(u8 *obj);

/* Main-image services reached through the veneers the staged-actor module
   names. */
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Object_SetModeById(s32 actor, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void Map_CopyCellAttributeRect(s32 x, s32 z, s32 width, s32 height, s32 dest_x, s32 dest_z);

/* Motion and cell services in the shape of the engine header's, through
   those names. */
static __inline__ void Actor_SetMotionSpeed(s32 actor, s32 speed, s32 acceleration)
{
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Actor_OffsetDestination(s32 actor, s32 dx, s32 dz)
{
    ObjectMotion_OffsetPositionAndResetMotion(actor, dx, dz);
}

static __inline__ void Map_CopyCellRect(s32 x, s32 z, s32 width, s32 height, s32 dest_x,
                                        s32 dest_z)
{
    Map_CopyCellAttributeRect(x, z, width, height, dest_x, dest_z);
}

void Object_SetPosition(Obj *, s32, s32, s32);
void Object_CommitPosition(Obj *);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
s32 ArcTan2(s32 dz, s32 dx);
void HaidiaDou_SinkPillarColumn27();
s32 Object_CheckMovementCollision();
void SceneActor_WaitActorDescent(u8 *obj);
void SceneActor_PositionPair();
void SceneActor_PlaceAtTile(s32 width, s32 top, s32 bottom);

/*
 * Camera-shift beat for overlay resource_3a6.  Each Func_ name spells the
 * overlay's own relocated call word, not a runtime address.
 */

/* Mirror the preceding transition on the negative grid axis, latch its event
 * flag, wait for the map update, and close on the paired sound cue. */

s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);

#endif
