#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_7_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_7_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;
struct LightOam;

void Whirlwind_CarryLeader(void);

void JupiterLighthouse_OpenDoor(s32 value);

void JupiterLighthouse_CloseDoor(void);

void JupiterLighthouse_CheckBlockOnCell(void);

void JupiterLighthouse_StepOffButton(void);

void Func_02000184(void);

void Func_020009d8(void);
void Func_02000a74(void);

void Func_02000c7c(void);

void Func_02000cec(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

s32 Func_02002c1c(struct Actor *object);

extern s16 gLightSpriteTile;
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;

void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void Teleporter_Sink(struct CellTarget *table, struct CellTargetEntry *entry);

extern s16 gLightSpriteCount;
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02002bec(void);

#endif
