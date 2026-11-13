#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_1_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_1_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct LightOam;

void Func_02001bcc(s32 owner);

extern s32 Data_02002254;

s32 Func_02001bbc(s32 x, s32 z, s32 layer);

extern s16 gData_02002298;
extern s16 gData_0200229a;
extern struct Actor *gData_0200229c[];
extern s16 gData_0200239c;
extern s16 gData_0200239e;

void Func_02001a64(struct LightOam *oam, s32 depth);

void Func_02001474(void);

void Func_020001b4(void);

void Func_020001d4(void);

extern u32 gData_02002050[];
extern u32 gData_0200208c[];
extern u32 gData_020020c8[];

#endif
