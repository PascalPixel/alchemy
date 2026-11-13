#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_4_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_4_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;

void Actor_RaiseFromCell(s32 id);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct LightOam gLightSpriteOam[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

void Func_02001790(void);

extern struct CellTarget gTeleporters;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void Func_020000c0(void);

void Func_02000164(void);

void Func_02000170(void);

void Func_0200021c(void);

extern u32 gData_020023fc[];

void LightSprites_InitA(void);

#endif
