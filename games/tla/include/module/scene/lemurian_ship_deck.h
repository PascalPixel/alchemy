#ifndef ALCHEMY_MODULE_SCENE_LEMURIAN_SHIP_DECK_H
#define ALCHEMY_MODULE_SCENE_LEMURIAN_SHIP_DECK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

struct MapMode {
  u8 unk00[23];
  struct MapModeBits {
    u8 low : 2;
    u8 mode : 2;
  } __attribute__((packed)) bits;
};

extern u32 gData_0200218c[];

extern u32 gData_020021e8[];

extern u32 gData_0200221c[];

void Func_02000188(void);

void Func_02000258(void);

void Func_02000288(void);

void Func_020002cc(void);

void FieldCamera_WaitForArrival(void);

extern s16 gData_020020dc[];

void Func_020020c4(const void *data);

void Func_02000210(void);

void Func_020020cc(const void *data);

extern u32 gData_0200247c[];
extern u32 gData_02002120[];

void Actor_Release(struct Actor *object);

extern u32 gData_020020e4[];
extern u32 gData_02002498[];

void Func_02000078(void);

void Func_02000168(void);

#endif
