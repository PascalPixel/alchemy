#include "types.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02000404(void);
void Func_02000490(void);
void Func_02002b28(void);
void Func_02002b30(void);

/* FAKEMATCH: the angle passes through a block temporary, as overlay 6a3's PLACE
 * does: the native builds it first and shifts it before the positions (direct:
 * differ 4 and 30 with shared constants). */
#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    s32 angle_ = (angle);                                                      \
    Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle_);          \
  }

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void ObjSpeed(s32 id, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjSetPos(s32 id, s32 x, s32 z) {
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void ObjCall2bf8(s32 id, s32 a) {
  Actor_ArmCallbackAndRefresh(id, a);
}

void Func_0200051c(void) {
  if (Event_GetFlag(0x910)) {
    Sound_Play(123);
    Import_EventSetExitRequest(16);
  } else if (!Event_GetFlag(0x280)) {
    Func_02002b28();
    Event_ResolvePendingActions(0);
    PLACE(26, PIXELS(744), PIXELS(257), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    ObjSpeed(4, 0x20000, 0x10000);
    ObjSetPos(4, 720, 244);
    ObjCall2bf8(4, FACING_SOUTHEAST);
    PLACE(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Event_SetFlag(0x280);
    Func_02000404();
    Func_02002b30();
  }
}

void Func_020005b8(void) {
  if (!Event_GetFlag(0x280)) {
    Func_02002b28();
    Event_ResolvePendingActions(0);
    PLACE(26, PIXELS(744), PIXELS(256), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    ObjSpeed(4, 0x20000, 0x10000);
    ObjSetPos(4, 720, 244);
    ObjCall2bf8(4, FACING_SOUTHEAST);
    PLACE(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Event_SetFlag(0x280);
    Func_02000490();
    Func_02002b30();
  }
}
