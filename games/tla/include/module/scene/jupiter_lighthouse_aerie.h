#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

struct EffectOptions;
struct EffectSprite;
struct LightOam;

void LightSprites_SetPriority(s32 value);

void Func_0200105c(struct Actor *object);

void Func_020017a8(s32 a, s32 b, s32 side);
void LightSprites_InitB(void);
void LightSprites_Add(s32 id, s32 value);

void MapGrid_SetAttributeBAt();

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

extern char ResourceId_GoldenTrident;

void Func_02002b30(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile);

void Func_02001080(void);

extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

struct CellTarget;
struct CellTargetEntry;

void JupiterLighthouse_LoadPalettesZero(s32 mode);

void JupiterLighthouse_PlaceRaisedActors(void);

void Leader_CenterInCell(void);

void Teleporter_Uncover(struct CellTarget *work, struct CellTargetEntry *entry);

void Teleporter_Sink(struct CellTarget *work, struct CellTargetEntry *entry);

void Teleporter_DropIn(void);

struct BeamSpark;
struct BeamSprite;
struct FieldEffect;

void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void Func_02002b38(struct BeamSprite *sprite);
void EffectSprite_DrawAtWorldPosition(struct BeamSprite *sprite,
                                      struct BeamSpark *spark);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

#endif
