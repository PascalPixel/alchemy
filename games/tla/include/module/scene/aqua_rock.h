#ifndef ALCHEMY_MODULE_SCENE_AQUA_ROCK_H
#define ALCHEMY_MODULE_SCENE_AQUA_ROCK_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;

void Func_02002834(s32 id);
void Func_020025a8(s32 id);
void Func_02001e00(void);
void Func_02001d30(struct Actor *object);
void Func_02001f60(s32 id);
void Actor_EmitWaterDrop(struct Actor *object);

void Func_02002a58(s32 a);

void Func_020013f8(void);
void Func_02002550(void);
void Func_0200276c(void);
void Func_0200169c(void);
void Func_02000844(void);
void Func_02002158(void);
void Func_02001460(void);
void Func_0200153c(void);

extern s16 gGroundParticleFrames[];

s32 Func_02000478(struct Actor *object);
void Func_02000d80(void);
void Func_02000f8c(void);

void Func_02001010(void);

void Func_0200106c(void);

struct Unk02003450 {
  s16 unk00;
  s16 unk02;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0A;
  s16 unk0C;
  s16 unk0E;
  s16 unk10;
  s16 unk12;
  s16 unk14;
  s16 unk16[2];
  s16 unk1A[2];
  s16 unk1E;
};

extern struct Unk02003450 Data_02003450[];

#endif
