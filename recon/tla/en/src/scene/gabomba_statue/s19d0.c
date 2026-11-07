#include "types.h"
#include "actor.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Func_020056b4(struct Actor *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
struct Actor *Func_020057bc(void);
void Func_020057a4(s32 speed, s32 acceleration);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020057b4(void);
void Func_02005854(s32 sound);
void Func_020020f0(s32 value);
void FieldEvent_Wait(s32 frames);
void GameFlag_SetBit(s32 flag);

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020019d0(void) {
  struct Actor *actor = Actor_GetById(26);

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_020057bc()->motionFlags = 0;
  Func_020057a4(0x9999, 0x1333);
  Camera_SetBounds(180 << 17, 0, 248 << 17, 1);
  Func_020057b4();
  Func_02005854(586);
  Func_020020f0(0);
  FieldEvent_Wait(40);
  Func_02005854(582);
  actor->motionFlags = 0;
  actor->y = PIXELS(8);
  FieldEvent_Wait(10);
  actor->y = 0;
  FieldEvent_Wait(10);
  Func_020020f0(-1);
  GameFlag_SetBit(0x8cd);
  FieldEvent_Wait(20);
  Func_020056bc();
}
