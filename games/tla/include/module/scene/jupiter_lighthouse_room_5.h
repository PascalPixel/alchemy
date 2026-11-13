#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_5_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_5_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct SparkSprite;

enum {
  EFFECT_DEFAULT_TYPE = 285,
  EFFECT_SCALE_ONE = 0x10000,
  EFFECT_OPTION_MASK = 0xffff0000,
  EFFECT_SCRIPT_REPEAT_COUNT = 3
};

extern u32 gOrbScript[];

void Func_02001718(const s32 *from, const s32 *to);

void JupiterLighthouse_LoadPalettes(s32 value);

void Func_02001538(void);

void Func_020029c8(void *data);

void Func_020029e0(struct SparkSprite *sprite, s32 width, s32 height,
                   s32 attributes, s32 tile);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gData_02003448[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

void Leader_CenterInCell(void);

extern struct CellTarget gTeleporters;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void Func_020006ac(void);

void Func_02000724(void);

void Func_020008a4(void);

extern u32 gData_02002ebc[];
extern u32 gData_02002ef8[];
extern u32 gData_02002f34[];

struct FieldEffect;
struct SparkSprite__jupiter_lighthouse_room_5;

void Func_020029d8(void);
extern u16 gData_02003404[];
extern u16 gData_02003424[];
extern u16 gData_02003428[];
void Func_02002a30();
void Func_02002810(struct FieldEffect *effect, s32 mode);
void Func_020029e8(struct SparkSprite__jupiter_lighthouse_room_5 *sprite);

#endif
