#ifndef ALCHEMY_MODULE_SCENE_PROX_H
#define ALCHEMY_MODULE_SCENE_PROX_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern s16 gData_02004b2c[];
extern u32 gProx_Room2Placements[];
extern s16 gData_02004d9c[];
extern s32 gData_02004f04[];
extern u32 gData_02005444[];
extern u32 gData_020054a4[];
extern u32 gProx_Room6Placements[];
extern struct Trigger gProx_Triggers[];
extern struct Trigger gProx_Room1Triggers[];
extern struct Trigger gProx_Room2Triggers[];
extern struct Trigger gProx_Room3Triggers[];
extern struct Trigger gProx_Room5Triggers[];
extern struct Trigger gProx_Room6Triggers[];

extern u32 gData_02004980[];

extern u32 gData_02004a08[];
extern s32 gProx_EnterDoorScript[];

void Func_020027e4(void);

void Func_02002878(void);

void Func_020029c8(void);

void Func_02002a38(void);
extern u32 gData_020047ac[];
extern u32 gData_02004848[];
extern u32 gData_020048e4[];
void Func_02001c24(s32 value);
extern u32 gData_02005f2c[];
extern u32 gData_02005f70[];
extern u32 gData_02005fac[];
extern u32 gData_02005fe8[];
extern u32 gData_020060cc[];
extern u32 gData_0200617c[];
extern u32 gData_020061fc[];
extern u32 gData_0200622c[];
void Func_020038e8(void);
void Particles_UpdateFalling(void);

void Func_02000290(void);

void Prox_PraiseReturnedWarrior(void);

void Prox_FearForWeyard(void);
void Prox_TalkToPlayingChild(void);

void Prox_TalkToTrainingChild(void);

void Prox_SearchForParents(void);

void Prox_DecideToGoToLighthouse(void);

void Prox_MeetPuelle(void);

void Func_020015d0(void);

void Func_02001770(void);
void Func_020017e4(void);
void Func_02001800(void);

void Func_02001a9c(void);

void Func_02001ab8(void);
void Func_02001ad4(void);
void Func_02001af0(void);

void Func_02001b0c(void);

void Func_02001b28(void);

void Func_02001b60(void);
void Func_02001b7c(void);
void Prox_SayFarewell(void);
void Func_0200391c(void);
extern u32 gData_02004644[];
extern u32 gData_02004680[];

void Func_020018e0(void);

#endif
