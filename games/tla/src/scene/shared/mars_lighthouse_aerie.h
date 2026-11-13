#ifndef TLA_MARS_LIGHTHOUSE_AERIE_H
#define TLA_MARS_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "scene_tables.h"

struct Trigger;

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Func_02000038(struct Actor *object);
void Func_02004d24(s32 id);
void Func_02004d50(s32 id);
void Func_02004fac(void);
void Func_02005010(void);
void Func_020050b0(void);
void Event_End(void);
void Func_020057cc(void);

extern u8 MsgUhhhhnnnICan[];
extern u8 MsgKoffKoffYou[];
extern u8 MsgUnnnnIAm[];
extern u8 MsgIUnderstandYour[];
extern s16 gData_0200626c[];
extern struct ActorPlacement gData_0200644c[];
extern s16 gData_020066bc[];
extern u32 gData_02006b64;
extern u32 gData_02006b6c;

void Func_02004e80(s32 from, s32 to);

extern u8 MsgDoNotLet[];
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

s32 Func_0200508c(u16 *list);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void MarsLighthouseAerie_ObjTurn(s32 id, s32 angle) {
  Actor_ArmCallbackAndRefresh(id, angle);
}

#endif
