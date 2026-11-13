#ifndef ALCHEMY_MODULE_SCENE_SHAMAN_VILLAGE_H
#define ALCHEMY_MODULE_SCENE_SHAMAN_VILLAGE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct ItemDefinition;

struct ShamanVillageSceneBlock {
  u8 unk00[22];
  u8 field16;
};

extern s16 gData_02006350[];

void Func_02000378(void);

extern u32 gData_02004cfc[];
extern u32 gData_02004d40[];
extern u32 gData_02004dc8[];
extern u32 gData_02004e0c[];
extern u32 gData_02004eac[];
extern u32 gData_02004f08[];

struct ItemDefinition *Func_02004894(s32 item);

extern u32 gData_020064d0[];

extern s16 gData_02005158[];

extern s16 gData_020064bc;
extern s16 *gData_020064c0;
extern s32 gData_020064a0;
extern s16 gData_02006504;
extern s16 gData_0200649c;
extern s16 gData_020064a8;
extern s16 gData_020064a4;
extern s16 gData_02006498;
extern s16 gData_02006490;
extern s16 gData_02006508;
extern s16 gData_020064b4;
extern s16 gData_020064b8;
extern s16 gData_020064c4;
extern s16 gData_020064ac;
extern s16 gData_020064b0;
extern s16 gData_02006494;

extern s16 gData_0200515a[];
extern s16 gData_02004b58[];
extern s16 gData_02005186[];
extern s16 gData_02004b80[];
extern u32 gData_02005204[];
extern u8 gData_02004bfe[];
extern char ResourceId_LetteringBanner;

void Func_0200479c(const void *source, void *destination);

void Func_02003b24(void);
extern u32 gData_02004f3c[];
extern u32 gData_02004f80[];

extern u32 gData_02004fc4[];

extern u32 gData_02005008[];
extern u32 gData_0200504c[];
extern u32 gData_020050bc[];
void Func_02004060(void);
void Func_020041b8(void);
void Func_020043f8(void);

void Func_02004534(void);

void Func_020046ec(void);

void Party_RestoreHpPp(s32 mode);

void Func_02000d68(void);

void Func_02000ef8(void);

void Func_02000f2c(void);

void Func_02000f60(void);

void Func_020010c8(void);

void Func_02001118(void);

void Func_0200113c(void);

void Func_020011c0(void);

void Func_0200122c(void);

void Func_02001644(void);

void Func_02001764(void);
void Func_02001880(void);

void Func_02001aa4(void);

void Func_02001cc8(void);

void Func_02001d3c(void);

void Func_02000fc4(void);

#endif
