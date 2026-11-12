#ifndef TLA_LEMURIAN_SHIP_H
#define TLA_LEMURIAN_SHIP_H

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "frame.h"

struct LemurianShipObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[2];
  u8 field22;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x18];
  s32 field44;
  s32 field48;
  u8 unk4c[4];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[2];
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

struct LemurianShipEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[2];
  u16 field16a;
  u8 unk16c[0x40];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

u32 Random16Far(void);

void Func_02002c68(void);
void Func_02002c70(void);
void Func_02002dc0(const void *script);
void Func_02002bf0(struct LemurianShipObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct LemurianShipObject *object,
                                        u32 value);
void Animation_SetStateFlags(struct LemurianShipObject *object, s32 flags);
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02002db8(const void *script);
void Event_StartScreenTransition(void);

void Func_02002cd8(s32 actor, s32 target);

#define LEMURIAN_SHIP_TASK_ADD(func, priority)                                 \
  Task_Add((func), (priority))

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void LemurianShip_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

void Func_02002c10(void);
void Func_02002d78(s32 a, s32 b);
void Func_02002dd0(const void *script);
void Func_02002dd8(void);

// ---- this overlay

void Func_02000d10(s32 id);
void Func_02000e2c(void);
void Func_02001380(s32 id);
void Func_020013b8(s32 id);

extern const u8 gData_02002e30[];
extern const u8 gData_02002e6c[];
extern const u8 gData_02002e9c[];
extern const u8 gData_02003010[];
extern const u8 gData_02003014[];
extern const u8 gData_0200301e[];
extern const u8 gData_02003034[];
extern const u8 gData_0200304e[];

void Func_02001a04(void);
void Func_02001650(void);
void Func_02002bf8(struct LemurianShipObject *object, const void *script);
void Func_02002dc8(const void *script);
extern const u8 gData_02003ef8[];

#endif
