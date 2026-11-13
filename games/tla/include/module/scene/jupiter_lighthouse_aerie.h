#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

struct EffectOptions;
struct EffectSprite;
struct LightOam;

void Func_02001dec(s32 value);

void Func_0200105c(struct Actor *object);

void Func_020017a8(s32 a, s32 b, s32 side);
void LightSprites_InitB(void);
void LightSprites_Add(s32 id, s32 value);

void Func_02002b58();

void Func_02001e78(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

extern char ResourceId_GoldenTrident;

void Func_02002b30(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile);

void Func_02001080(void);

extern s16 gLightSpriteCount;
extern struct Actor *gData_02003384[];
extern struct LightOam Data_020033c4[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void Import_OamAddSpriteAtDepth(struct LightOam *oam, s32 depth);

extern u32 gData_02003020[];
extern u32 gData_0200305c[];

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

void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Import_FieldCameraWaitForArrival(void);
extern u16 gData_02003360[];
extern u16 gData_02003364[];
void Func_02002b38(struct BeamSprite *sprite);
void EffectSprite_DrawAtWorldPosition(struct BeamSprite *sprite,
                                      struct BeamSpark *spark);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

#endif
