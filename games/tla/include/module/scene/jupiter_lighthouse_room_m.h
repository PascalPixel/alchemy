#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_M_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_M_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;

void PaletteCycle_SetPausesInEvents(s32 value);

extern u32 gData_02005524[];
extern u32 gData_02005548[];
extern u32 gData_020056ec[];
extern u32 gData_020057d0[];
extern u32 gData_02005800[];
extern u32 gData_0200d830[];
extern u32 gData_020058d8[];
extern u32 gData_02005920[];

extern u32 gData_020049cc[];
extern u32 gData_020049ec[];
extern u32 gData_020049fc[];
extern u32 gData_02004a18[];
extern u32 gData_02004a34[];
extern u32 gData_02004a50[];

extern u32 gData_0200599c[];

extern u32 gData_02004a6c[];
extern u32 gData_02004a90[];
extern u32 gData_02004ab4[];
extern u32 gData_02004ae8[];
extern u32 gData_02004b1c[];

extern u32 gData_020059a4[];
extern u32 gData_02005a00[];

void JupiterLighthouse_LoadPalettes(s32 mode);

void Func_02000060(void);

void Func_02000080(void);

void Func_0200060c(void);

void Func_020008c8(void);

void Whirlwind_CarryLeader(void);

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

extern struct CellTarget gTeleporters;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void Teleporter_Sink(struct CellTarget *table, struct CellTargetEntry *entry);

s32 Event_ClearInvalidPackedValues();
void Func_02004994(s32 id, s32 mode);
void Func_02004984(void);
void Func_0200498c(s32 a, s32 b, s32 c, s32 d);
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void Func_020049b4();
s32 Func_020049a4(s32 x, s32 z, s32 layer);

#endif
