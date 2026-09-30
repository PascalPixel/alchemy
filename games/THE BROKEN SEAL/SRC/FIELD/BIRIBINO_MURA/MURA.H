/* Declarations shared by Bilibin village's sources (resource_38b). */
#ifndef MURA_H
#define MURA_H

#include "TYPES.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

enum {
    /* Message 0x182 + 194. */
    ITEM_HARD_NUT = 194
};

/* The scene's tables, laid out after the code. */
extern u8 Placement_Messages[];
/* The village's three facing villagers' action table. */
extern u8 Mura_VillagerActions[];
/* Per door: the animated cell steps and where they are drawn. */
extern const u16 *const Mura_DoorCellSteps[];
extern const s16 Mura_DoorCellOrigins[];
/* The ten cells the push puzzle repaints, as tile x and z pairs. */
extern u8 Mura_RepaintCells[];
/* The spawned kind-24 effect's animation script. */
extern const s32 Mura_SpawnScript[];

s32 ArcTan2(s32, s32);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
void RunSceneTransitionEffect();
void Object_SetTargetAndCallback();
void AnimationObjects_SelectAnimation();

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{

    return *(u16 *)(base + idx);
}

static __inline__ void bump_step_020001ec(s32 amount)
{
    gEventWork->message += amount;
}

#endif
