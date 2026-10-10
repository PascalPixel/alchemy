#ifndef MURA_H
#define MURA_H

#include "types.h"

#define NULL ((void *)0)

#include "facing_object.h"
#include "field_event.h"
#include "field_scene.h"
#include "select_overlay_data_by_runtime_selector.h"

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

#endif
