#ifndef MURA_H
#define MURA_H

#include "TYPES.H"

#define NULL ((void *)0)

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

enum {
    ITEM_HARD_NUT = 194
};

extern u8 Placement_Messages[];
extern u8 Mura_VillagerActions[];
extern const u16 *const Mura_DoorCellSteps[];
extern const s16 Mura_DoorCellOrigins[];
extern u8 Mura_RepaintCells[];
extern const s32 Mura_SpawnScript[];

s32 ArcTan2(s32, s32);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
void RunSceneTransitionEffect();
void Object_SetTargetAndCallback();
void AnimationObjects_SelectAnimation();

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{

    return *(u16 *)(base + idx);
}

#endif
