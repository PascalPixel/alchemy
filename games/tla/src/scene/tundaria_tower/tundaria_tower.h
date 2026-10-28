#ifndef TLA_TUNDARIA_TOWER_H
#define TLA_TUNDARIA_TOWER_H

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_work.h"
#include "resource.h"
#include "mem.h"
#include "call.h"
#include "field_effect.h"

struct Trigger;

struct TundariaTowerEffectParams {
  u8 unk00[0x18];
  u16 kind;
  u8 unk1a[0xe];
};

struct TundariaTowerSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

struct TundariaTowerPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Random16Far(void);
void FieldMessage_Show(const u8 *message, s32 flags);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Task_RemoveFar(void (*func)(void));
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value);
void Func_020000b8(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct TundariaTowerEffectParams *params);
void Func_02000708(struct TundariaTowerPosition *position, s32 count,
                   s32 speed);
void Func_02002c1c(s32 a, s32 b, s32 owner);
void Func_02002cc0(void);
void Func_02002fc0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002fe8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003048(void);
void Func_02003050(void);
void Func_02003088(s32 id, s32 x, s32 z);
void Func_020030c0(s32 id);
void Func_02003100(void);
void Func_02003140(void);
void Func_02003148(s32 a);
void Func_02003170(const u8 *script);

extern const u8 MsgThereSSomething[];
extern const u8 gData_02003264[];
extern const u8 gData_0200326e[];
extern u16 gData_02003cd0;

void Func_02002e00(void);
void Func_02002d44(s32 x, s32 y, s32 z, s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_0200137c(void);
extern const u8 gData_020037f8[];
extern const u8 gData_0200381c[];
extern const u8 gData_0200393c[];
extern const u8 gData_020039e4[];
extern const u8 gData_02003a5c[];
extern const u8 gData_02003b28[];
extern const u8 gData_02003c90[];

void Func_02000840(struct Actor *object);
void Func_02002ebc(void);

void Func_02003000(s32 a, s32 b, s32 c);
void Func_02003108(s32 color, s32 b);
void Func_02003110(s32 color, s32 b);
void Actor_ResetAndSetPosition(s32 actor, s32 x, s32 z);

static inline void TundariaTower_FadeTo(s32 color, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003108(color, b);
}

static inline void TundariaTower_FadeFrom(s32 color, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003110(color, b);
}

static inline void TundariaTower_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer; as a
   * direct call the actor loads before the coordinates. */
  Actor_ResetAndSetPosition(actor, x, z);
}

static inline void TundariaTower_SetWork(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003000(a, b, c);
}

#endif
