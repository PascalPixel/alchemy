#ifndef ALCHEMY_MODULE_SCENE_CLEAR_H
#define ALCHEMY_MODULE_SCENE_CLEAR_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

// The two secret button codes: unlocked, held and progress of each.
extern s16 gData_020024f6;

extern u16 gData_0200271c[];

struct Panel_02001804 {
  u8 unk00[8];
  u16 stride;
  u8 unk0a[2];
  s16 x;
  s16 y;
};

extern u16 gData_020024f4[];
extern s16 gData_020024f8;
extern s16 gData_020024fa;
extern s16 gData_020024fc;
extern s16 gData_020024fe;
extern s16 gData_02002500;
extern u16 gData_02002410[];
extern u16 gData_02002418[];

s32 Func_02000b24(void);

s32 Func_02000e44(void);

extern s8 gData_02002438[];
void Func_020018e0(s32 mode, u8 *data);
s32 Func_02002068(u8 *src, s32 length, u8 *dst);
extern u16 gData_02002490[];

#endif
