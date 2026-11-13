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

extern u32 gData_02003214[];

void Func_02001718(const s32 *from, const s32 *to);

void JupiterLighthouse_LoadPalettes(s32 value);

void Func_02001538(void);

void Func_020029c8(void *data);

void Func_020029e0(struct SparkSprite *sprite, s32 width, s32 height,
                   s32 attributes, s32 tile);

extern s16 gData_02003444;
extern s16 gData_02003446;
extern struct Actor *gData_02003448[];
extern s16 gData_02003548;
extern s16 gData_0200354a;

void Leader_CenterInCell(void);

extern struct CellTarget Data_0200354c;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void Func_020006ac(void);

void Func_02000724(void);

void Func_020008a4(void);

extern u32 gData_02002ebc[];
extern u32 gData_02002ef8[];
extern u32 gData_02002f34[];

#endif
