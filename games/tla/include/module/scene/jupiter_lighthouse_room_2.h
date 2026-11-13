#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_2_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_2_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

void Import_ActorDestroy(struct Actor *object);

void Actor_RaiseFromCell(s32 id);

void LightSprites_InitB(void);

void Func_020015a0(s32 id, s32 value);

void BattleEffect_FillOverlayTiles(s32 a);

void JupiterLighthouse_LoadPalettes(s32 mode);

extern s32 gWhirlwindRiding;

s32 Func_020022fc(s32 x, s32 z, s32 layer);

s32 Position_FlatDistance(s32 *a, s32 *b);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gData_02002bdc[];
extern struct LightOam Data_02002c1c[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void Leader_CenterInCell(void);

extern struct CellTarget gTeleporters;

void Func_020003f4(void);
void Func_02000410(void);

void Func_02000440(void);

void Func_02000454(void);

void Func_020006dc(void);

void Func_02000704(void);

extern u32 gData_020027a8[];
extern u32 gData_020027e4[];
extern u32 gData_02002820[];

#endif
