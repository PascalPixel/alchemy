#ifndef TLA_GAROH_INTERIORS_H
#define TLA_GAROH_INTERIORS_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "scene_ids.h"
#include "scene_call.h"
#include "text/msg_ids.h"

struct EventWork1c4 {
  u8 unk000[0x1c4];
  u16 count;
};

extern const u8 gData_0200094c[];
extern const u8 gData_02000964[];
extern const u8 gData_02000ab4[];
extern const u8 gData_02000b2c[];
extern const u8 gData_02000b5c[];
extern const u8 gData_02000b80[];
extern const u8 gData_02000d18[];
extern const u8 gData_02000de4[];
extern const u8 gData_02000e44[];
extern const u8 gData_02000fe8[];

struct Actor *Actor_GetById(s32 id);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Func_020007bc(void);
void Func_020007cc(void);
void Func_020007d4(void);
void Func_020007e4(s32 message, s32 a, s32 b);
void Func_020007f4(void);
void Func_020007fc(void);
void Func_02000844(s32 message);
void Func_02000854(s32 id, s32 a, s32 b);
void Func_0200085c(s32 id, s32 flags);
void Func_02000884(s32 a);
s32 Func_0200088c(void);
void Func_0200089c(s32 slot, s32 actor);
void Func_020008a4(s32 id);
void Func_020008ac(s32 slot, s32 actor);
void Func_020008b4(s32 cue);

#endif
