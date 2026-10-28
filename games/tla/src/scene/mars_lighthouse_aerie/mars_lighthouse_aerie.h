#ifndef TLA_MARS_LIGHTHOUSE_AERIE_H
#define TLA_MARS_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "adventure.h"
#include "call.h"

struct Trigger;

struct Actor *Actor_GetById(s32 id);
void GameFlag_ClearBit(s32 flag);
s32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void FieldEvent_WaitFrames(s32 frames);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_Stop(s32 id);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Task_RemoveFar(void (*func)(void));
void Func_02000038(struct Actor *object);
void Func_02004d24(s32 id);
void Func_02004d50(s32 id);
void Func_02004fac(void);
void Func_02005010(void);
void Func_020050b0(void);
void Func_0200560c(void);
void Func_02005614(void);
void Func_020056dc(const u8 *message);
void Func_020056f4(s32 id, s32 flags);
void Func_02005704(s32 id, s32 angle);
void Func_02005744(s32 a);
void Func_02005784(s32 a);
void Func_020057cc(void);

extern const u8 MsgUhhhhnnnICan[];
extern const u8 MsgKoffKoffYou[];
extern const u8 MsgUnnnnIAm[];
extern const u8 MsgIUnderstandYour[];
extern const u8 gData_0200626c[];
extern const u8 gData_0200644c[];
extern const u8 gData_020066bc[];
extern u32 gData_02006b64;
extern u32 gData_02006b6c;

void Func_02004e80(s32 from, s32 to);

extern const u8 MsgDoNotLet[];
extern u16 gData_02006cd0;
extern u16 gData_02006cd8;
extern u16 gData_02006cdc;
extern u16 *gData_02006ce0;
extern u16 gData_02006cec;
extern u16 gData_02006cf0;

struct MarsLighthouseAerieParty {
  u8 unk000[0x26a];
  s16 held;
};

s32 Task_Add(void (*func)(void), s32 priority);
s32 Func_0200508c(u16 *list);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);

static inline void MarsLighthouseAerie_ObjTurn(s32 id, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005704(id, angle);
}

#endif
