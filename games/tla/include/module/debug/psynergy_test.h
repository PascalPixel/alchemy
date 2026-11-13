#ifndef ALCHEMY_MODULE_DEBUG_PSYNERGY_TEST_H
#define ALCHEMY_MODULE_DEBUG_PSYNERGY_TEST_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct SparkleSprite;

extern s16 gData_02001300[];

void Func_02000f34(s32 *, s32 *);

void Func_02000f2c(s32, s32);

void Func_0200092c(s32, s32, s32, s32, s32, s32, s32);
void Func_02000950(s32, s32, s32, s32);
void Func_020009c0(s32, s32, s32, s32);
void Func_02000a10(s32, s32, s32, s32, s32);
void Func_02000d4c(struct Actor *);

void Func_02000edc(s32 actor, s32 angle, s32 frames);

void Func_02000f0c(s32 actor);

void Func_02000eec(struct SparkleSprite *sprite);

extern s32 gData_02000f44[];

s32 Func_02000ef4(struct Actor *object);

#endif
