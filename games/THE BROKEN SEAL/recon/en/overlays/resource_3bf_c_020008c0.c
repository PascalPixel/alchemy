#include "STAGED_ACTOR.H"
#include "FIXED_POINT_POSITION.H"

/* Linked into several field overlays; each overlay has its own copy of the
 * footprint and direction tables. */

extern u8 *gWork;
/* A word, not a pointer: an integer read shares the alias set of the request
 * fields SceneActor_MoveAndRedraw spills, which keeps those spills first. */
extern u32 gCam;
extern s32 StagedActor_DirectionSteps[];
extern s32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];

extern void WaitFrames(s32);
extern void Object_SetMode(void *, s32);
extern void Object_SetPosition(void *, s32, s32, s32);
extern void Object_CommitPosition(void *);
extern s32 Object_CheckMovementCollision(void *, s32 *);
extern void *Object_GetById(u32);
extern void Battle_WaitMode0(s32);
extern void ObjectMotion_SetSpeedParameters(s32, s32, s32);
extern void ObjectMotion_OffsetPositionAndResetMotion(s32, s32, s32);
extern void ObjectMotion_CommitCurrentPositionAndActivate(s32);
extern void Object_SetModeById(s32, s32);
extern void BattleFx_PlayQueuedSound(void);
extern void Map_CopyCellAttributeRect(s32, s32, s32, s32, s32, s32);
extern void Audio_PlayCue(s32);

static __inline__ void Scene_Call6(
    void (*func)(s32, s32, s32, s32, s32, s32),
    s32 x, s32 z, s32 width, s32 height, s32 dst_x, s32 dst_z)
{
    func(x, z, width, height, dst_x, dst_z);
}

extern s32 StagedActor_FillGridAttributeRectangle(u32, s32, s32, u32, u32, s32);

s32 FieldScene_RedrawActorFootprint(s32 id)
{
    s32 *work = (s32 *)gCam;
    struct StagedActor *actor = Object_GetById(id);
    struct StagedActorProbe probe;
    u32 idx = 0;
    s32 offset;
    s32 width;
    s32 height;
    s32 x0;
    s32 x1;
    s32 z0;
    s32 z1;
    s32 dst_z;

    if (*STAGED_ACTOR_PROBE_DETAILS(actor)->unknown_28 != StagedActor_FootprintKinds[idx])
        goto search;
    probe.footprint_index = idx;
    goto draw;
search:
    {
        struct StagedActorProbe *buf = &probe;

        do {
            ++idx;
            buf->footprint_index = 7;
            if (idx > 5)
                goto draw;
        } while (*STAGED_ACTOR_PROBE_DETAILS(actor)->unknown_28 != StagedActor_FootprintKinds[idx]);
    }
    probe.footprint_index = idx;
draw:
    if ((u32)probe.footprint_index > 6)
        return 0;
    probe.position_x = actor->x.value;
    probe.position_y = actor->y;
    probe.position_z = actor->z.value;
    offset = probe.footprint_index * 4;
    z0 = StagedActor_FootprintBounds[offset + 1];
    if (z0 < 0)
        z0 = -z0;
    z1 = StagedActor_FootprintBounds[offset + 3];
    if (z1 < 0)
        z1 = -z1;
    height = (z0 + z1) >> 4;
    x0 = StagedActor_FootprintBounds[offset];
    if (x0 < 0)
        x0 = -x0;
    x1 = StagedActor_FootprintBounds[offset + 2];
    if (x1 < 0)
        x1 = -x1;
    width = (x0 + x1) >> 4;
    probe.position_x += StagedActor_FootprintBounds[offset] << 16;
    probe.position_z += StagedActor_FootprintBounds[offset + 1] << 16;
    probe.position_x >>= 20;
    probe.position_z >>= 20;
    idx = work[0x13c / 4] >> 20;
    dst_z = work[0x140 / 4] >> 20;
    Scene_Call6(Map_CopyCellAttributeRect, probe.position_x, probe.position_z,
        width, height, idx + probe.position_x, dst_z + probe.position_z);
    StagedActor_FillGridAttributeRectangle(0, probe.position_x, probe.position_z, width, height, 255);
    StagedActor_FillGridAttributeRectangle(2, probe.position_x, probe.position_z, width, height, 255);
    return 1;
}
