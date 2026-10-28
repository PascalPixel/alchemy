#include "types.h"
#include "actor.h"
#include "flag_call.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_02000404(void);
void Func_02000490(void);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002b90(s32 id, s32 x, s32 z, s32 angle);
void Func_02002bf8(s32 id, s32 a);
void Func_02002c30(s32 a);
void Func_02002c58(s32 a);
void Func_02002c90(s32 a);

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002b90(id, x, z, angle);
}

/* FAKEMATCH: the angle passes through a block temporary, as overlay 6a3's PLACE
 * does: the native builds it first and shifts it before the positions (direct:
 * differ 4 and 30 with shared constants). */
#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    s32 angle_ = (angle);                                                      \
    ObjPlace(id, x, z, angle_);                                                \
  }

static __inline__ void ObjSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjSetPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void ObjCall2bf8(s32 id, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002bf8(id, a);
}

void Func_0200051c(void) {
  if (Flag_IsSet(0x910)) {
    Func_02002c90(123);
    Func_02002c30(16);
  } else if (!Flag_IsSet(0x280)) {
    Func_02002b28();
    Func_02002c58(0);
    PLACE(26, PIXELS(744), PIXELS(257), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    ObjSpeed(4, 0x20000, 0x10000);
    ObjSetPos(4, 720, 244);
    ObjCall2bf8(4, FACING_SOUTHEAST);
    PLACE(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Flag_Set(0x280);
    Func_02000404();
    Func_02002b30();
  }
}

void Func_020005b8(void) {
  if (!Flag_IsSet(0x280)) {
    Func_02002b28();
    Func_02002c58(0);
    PLACE(26, PIXELS(744), PIXELS(256), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    ObjSpeed(4, 0x20000, 0x10000);
    ObjSetPos(4, 720, 244);
    ObjCall2bf8(4, FACING_SOUTHEAST);
    PLACE(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Flag_Set(0x280);
    Func_02000490();
    Func_02002b30();
  }
}
