#ifndef ALCHEMY_MODULE_SCENE_YAMPI_DESERT_H
#define ALCHEMY_MODULE_SCENE_YAMPI_DESERT_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

void Func_02001c5c(void);

extern u32 gData_020042b4[];

extern u32 gData_02005828[];

void Func_02002ca0(void);
// Const: the cell helper only computes from its argument.
s32 Func_020012d4(s32 cell) __attribute__((const));
extern u32 gData_020042f0[];
void Func_0200316c(void);
void Func_020031b4(void);
void Func_020032f4(void);
void Func_02003364(void);
void Func_02003404(void);

void Func_020035b8(void);

void Func_020035d8(void);
void Func_020036d0(void);
void Func_0200371c(void);
void Func_020037d4(void);
void Func_020037e8(void);

void YampiDesert_DropScoopGem(void);

void YampiDesert_TalkToElder(void);

void YampiDesert_ReadSignpost(void);

void Func_020007e0(void);

void YampiDesert_CastSand(void);

void Func_02000a10(void);

void Func_02000a8c(void);

void YampiDesert_StirScorpion(void);

void YampiDesert_StirScorpionAgain(void);

void Func_02000e78(void);

void YampiDesert_TrapKingScorpion(void);

void Func_020012dc(void);

void Func_020013f8(void);

void Func_02001518(void);

void Func_02001628(void);

void Func_02002e14(void);
void Func_02002e98(void);

extern u32 gData_020045d4[];

extern u32 gData_02004630[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Func_020041e8(s32 layer, s32 shape, s32 height);

void Func_02003da0(s32 unused, s32 value);

void Icon_DrawItem(s32 item);
void FieldEffect_SetAttached(s32 actor, s32 a);
void FieldCamera_WaitForArrival();

#endif
