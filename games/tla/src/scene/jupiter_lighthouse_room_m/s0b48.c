#include "jupiter_lighthouse_room_m.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u32 gData_02005524[];
extern u32 gData_02005548[];
extern u32 gData_0200558c[];
extern u32 gData_020055ec[];
extern u32 gData_02005644[];
extern u32 gData_0200569c[];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Func_02000a78(void);

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 30 of 57). */
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

static __inline__ void ObjSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: the speeds are held in r5 and r6 across
   * the calls, four rows fewer). */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

/* FAKEMATCH: the angle passes through a block temporary: the native holds each
 * run of equal angles in r5 (0x2000, then 0xc000), building the second while
 * the third call is set up (a plain argument: differ 31 of 57). */
#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    s32 angle_ = (angle);                                                      \
    ObjPlace(id, (x) << 16, (z) << 16, angle_);                                \
  }

/// Sets four actors' speeds, places them and starts their action scripts.
void Func_02000b48(void) {
  ObjSpeed(26, 0x30000, 0x18000);
  ObjSpeed(27, 0x30000, 0x18000);
  ObjSpeed(0, 0x30000, 0x18000);
  ObjSpeed(2, 0x30000, 0x18000);
  PLACE(0, 776, 202, FACING_WEST);
  PLACE(2, 731, 232, FACING_NORTH);
  PLACE(27, 731, 213, FACING_SOUTH);
  PLACE(26, 737, 202, FACING_EAST);
  Actor_SetScript(0, gData_0200558c);
  Actor_SetScript(2, gData_02005644);
  Actor_SetScript(26, gData_020055ec);
  Actor_SetScript(27, gData_0200569c);
  Actor_SetScript(3, gData_02005548);
}

void Func_02000c08(void) {
  PLACE(2, 757, 216, FACING_SOUTHEAST);
  PLACE(0, 761, 219, FACING_SOUTHEAST);
  PLACE(3, 816, 285, FACING_SOUTHEAST);
  PLACE(1, 824, 302, FACING_NORTH);
  PLACE(29, 821, 302, FACING_NORTH);
  Actor_SetSingleChildFlags(Actor_Get(1), 0);
  Actor_SetScript(1, gData_02005524);
  {
    /* FAKEMATCH: the priority passes through a block temporary, so 0x480 is
     * built before the task function's address is loaded; as an argument, 2 of
     * 57 instructions differ in tla-en. */
    s32 priority = 0x480;

    FieldTask_AddFrame((void (*)(void))Func_02000a78, priority);
  }
  Actor_SetAnimation(0, 38);
  Actor_SetAnimation(2, 31);
  Actor_SetAnimation(3, 9);
}
