#ifndef SETTLEMENTS_LOCAL_H
#define SETTLEMENTS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "event_inline.h"

s32 Trig_Cos(s32 angle);

extern u8 MsgSettlements01[];

#include "frame.h"

extern s16 gData_0200226c[];
extern u16 gData_02002ce8;

void Func_02000758(s32 x, s32 y, s32 z);
void Func_02000d14(void);
void Scroll_StartHBlank(void);
void Func_02000f2c(void);
void Func_02000fd0(void);
void Func_0200122c(void);
void Func_0200138c(void);
void Func_02001770(void);
void Leader_StopMoving(void);
void Event_ResolveAllPendingActions(void);
void Scene_QueueMusic(s32 a);
void Func_02002178(s32 a, s32 b, s32 c);
void Func_02002190(const void *data);
void Func_02002198(void);
void Func_0200052c(void);
void Func_02000714(struct Actor *object);
void Import_ActorSelectAnimation(struct Actor *object, s32 a);
void Actor_Release(struct Actor *object);
extern s32 gData_02002ce0;
extern u16 gData_02002ce4;
extern u16 gData_02002ce6;
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Irq_SetHandler(s32 a, s32 b, void (*callback)(void));
void Event_End(void);
void ItemDisplay_ShowItem(s32 a, s32 b);
void Func_020021a0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern u32 gData_02002380[];
extern u32 gData_020023a0[];

static __inline__ void Window(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  Func_020021a0(a, b, c, d, e, f, g);
}

#endif
