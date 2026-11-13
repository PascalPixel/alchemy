#ifndef ALCHEMY_MODULE_SCENE_AQUA_ROCK_INSIDE_H
#define ALCHEMY_MODULE_SCENE_AQUA_ROCK_INSIDE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

struct WaterOutlet {
  u16 state;
  u16 index;
  s32 x;
  s32 y;
  s32 z;
  s32 level;
};

struct WaterWork {
  u32 unk00;
  void (*handler)(void);
  struct WaterOutlet outlets[16];
  u16 levels[4];
};

void Func_0200349c(u32 index, s32 height);

void Func_0200061c(struct Actor *object);

void Leader_RiseFromWater(void);
void Func_0200107c(s32 a, s32 b);
void Func_02003278(void);
void Func_020032d4(u32 first, s32 level, s32 baseLevel);
void Func_020007a8(void);
void Func_02003420(s32 id, s32 kind, s32 height);
void Func_02000ad8(void);

void Func_020004b4(void);

void Func_02000690(void);

void Func_0200071c(void);
void Func_020007d4(void);
void Func_02000a24(void);

void Func_02000a98(void);

void Func_02000bac(void);

void Func_02000bd4(void);

void Func_02000de4(void);

void Func_02001060(void);
void Func_020010ec(void);
void Func_02001170(void);
void Func_02001328(void);
void Func_02001c68(void);
void Func_02001cd8(void);
void Func_02001d40(void);
void Func_02002ca4(void);

void Func_02003cc4(struct Actor *object, s32 mode);
void Func_02003ea4();
void Func_02003ee4();

#endif
