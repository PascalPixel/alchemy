#ifndef ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_ROOM_M_H
#define ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_ROOM_M_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct Vector3;

void Func_020024e4(void);

s32 MarsLighthouseRoomM_SlideOnIce(void);
void Func_02002e6c(struct Actor *self);

void Func_02001608(void);

void Func_0200055c(void);

void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f);

void Func_020000c0(void);

void Func_020003c4(void);

void Func_02000934(s32 entrance);

void Func_02002118(void);

void Func_0200080c(void);

s32 Import_FieldCheckSpheresOverlap(s32 *from, s32 radius, struct Vector3 *to,
                                    s32 radius2);

void Func_020024bc(struct Actor *object);

extern u32 gData_02003304[];
extern u32 gData_02003348[];

void Func_020002c8(void);

void Func_020002e8(void);

void Func_020002fc(void);

void Func_02000abc(void);

void MarsLighthouseRoomM_ReadEarthInscription(void);
void MarsLighthouseRoomM_ReadWaterInscription(void);
void MarsLighthouseRoomM_ReadFireInscription(void);

void MarsLighthouseRoomM_ReadWindInscription(void);

void MarsLighthouseRoomM_HearDragonStatue(void);

extern s32 gData_020032c8[];

#endif
