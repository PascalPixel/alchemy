#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;

void Func_0200045c(void);

void SceneActor_ParkRecord(struct Actor *actor);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gData_02006a70[];
extern struct LightOam Data_02006ab0[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
extern void Actor_SelectAnimation(struct FieldEffect *effect, s32 mode);

/// The cells Teleporters_Step watches, and the cell it copies over the one the
/// leader steps on.
extern struct CellTarget gTeleporters;

void Func_0200581c(void);

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

/// The halves of a 16.16 coordinate. Read through an s16 pointer instead, the
/// high half's offset folds into the coordinate's (differ 11).
struct FixedHalves {
  u16 fraction;
  s16 whole;
};

/// What a field sprite's record points to.
struct SpriteRecord {
  u8 unk00[5];
  u8 unk05; ///< XXX: set to 1 here; the resource metadata code would say why.
};

struct UnitItems {
  u8 unk00[0xd8];
  u16 items[15];
};

struct SceneActor {
  u8 pad0[16];
  s32 unk_10;
  u8 pad14[35 - 20];
  u8 unk_23;
  u8 pad24[64 - 36];
  s32 unk_40;
  u8 pad44[85 - 68];
  u8 unk_55;
};

struct ScrollWork {
  s32 unk00[3];
  s32 height;
};

/// The object this update drives: an Actor with its own bytes over the callback
/// word.
struct Object6a5 {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 speed;
  u8 unk34[0x1c];
  s8 *sprite;
  u8 unk54[0x0e];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(void);
};

/// The scene's event work, as far as this update reads it.
struct EventWork6a5 {
  u8 unk000[346];
  s16 unk15a;
  u8 unk15c[2];
  s16 unk15e;
  u8 unk160[6];
  s16 unk166;
  s16 unk168;
  s16 unk16a;
};

extern s32 gData_02006a24;

extern s32 gData_0200641c[];
extern s32 gData_02006470[];

void Import_FieldCameraWaitForArrival(void);

void Func_02003a6c(s32 mode);
void Func_02003c58(s32 mode);

extern s32 Data_02006a1c;

s32 Func_020059b4();

extern u16 gData_02006a2c[];
extern u16 gLighthousePalette13[];
extern u16 gLighthouseUnlitColors[];

void Func_020059ec();
s32 Func_020059e4();

extern s32 Data_02006a28;

#endif
