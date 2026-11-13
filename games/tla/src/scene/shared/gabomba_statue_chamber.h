#ifndef TLA_GABOMBA_STATUE_CHAMBER_H
#define TLA_GABOMBA_STATUE_CHAMBER_H

#include "types.h"
#include "system.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "field/field.h"
#include "frame.h"

struct GabombaStatueChamberObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
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

struct GabombaStatueChamberEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[0x44];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

void Func_02001da0(void);
void Event_End(void);
void Func_02001e80(s32 actor);
void Func_02001d30(struct GabombaStatueChamberObject *object);
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Func_02001ea8(void);

#include "event_work.h"

void Actor_MoveToObject(s32 actor, s32 target);

// ---- this overlay

void Func_02000a98(s32 x, s32 z);

extern u8 gData_020020f8[];
extern u8 gData_020021e8[];
extern u8 gData_02002458[];
extern u8 gData_02002470[];
extern u8 gData_020024a0[];
extern u8 gData_020024dc[];
extern u8 gData_02002530[];
extern u8 gData_0200a08c[];

#endif
