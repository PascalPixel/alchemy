#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_2_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_2_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

void Import_ActorRelease(struct Actor *object);

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

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];
extern u32 gFieldEffect_LongLifeScript[];

void Func_0200230c(s32 actor);
extern s32 gData_02002b88[3];
extern const u8 gData_02002b60[];
void Func_02002324(const void *script, s32 a, s32 b, s32 c);
extern u16 gData_02002b98[];
extern u16 gData_02002bbc[];

#endif
