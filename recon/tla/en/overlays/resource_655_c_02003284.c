// Draft of Func_02003284 (overlay 655, 696 bytes), differ 77 of 276: the room
// entry written in the scene layer of Func_02003164 and Func_0200390c. The
// statements are the native's; what differs is where a zero and the 0xfe
// mask live (the native keeps the mask in r6 and builds the zero in r7 right
// after the x read of object 17; this hoists the zero into r8 and takes r7
// for the mask), and the order of a few argument rows: after a byte store
// the native builds the constant argument before it reloads the owner
// (strb; movs r1, #2; ldr r0, [r5]), and builds the link slot last
// (movs r1, #8; movs r2, #9; movs r0, #0).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "map.h"
#include "callback_scheduler.h"

extern const u8 gData_02003f80[];
extern const u8 gData_02003fc4[];
extern const u8 gData_02003fc8[];
extern s32 gData_02005678;
extern s32 gData_0200567c;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020001d0(const void *data);
void Func_02000c64(const void *data);
void Func_020013b0(void);
void Func_0200172c(void);
void Func_02001930(void);
void Func_02001ee8(void);
void Func_02001fc4(void);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c70(s32 id, s32 x, s32 z, s32 angle);
void Func_02003cc0(s32 id);
void Func_02003d90(void);
void Func_02003d98(void);
void Func_02003da0(s32 a, s32 b, s32 c);
void Func_02003da8(s32 a, s32 b, s32 c, s32 d);
s32 Func_02003db0(void);
void Func_02003db8(const void *data);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_SetBit(flag);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003bf0(a, b, c, d, e, f);
}

static __inline__ void Actor_SetPosition(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Object_SetPositionAndResetMotion(object, x, y, z);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003c70(id, x, z, angle);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003be0(a, b, c, d, e, f);
}

static __inline__ void Actor_SetVariant(s32 id, s32 variant) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ void Actor_Link(s32 slot, s32 first, s32 second) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003da0(slot, first, second);
}

void Func_02003284(void) {
  struct ObjectRuntime *object;
  s32 zero;

  if (Flag_Test(265) == 0) {
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
    Actor_SetVariant(gPartyState.currentOwner, 2);
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 1;
  }
  Actor_SetVariant(8, 1);
  Actor_SetVariant(9, 1);
  Actor_SetVariant(10, 1);
  Actor_SetVariant(11, 1);
  Func_02003d98();
  Actor_Link(0, 8, 9);
  Actor_Link(1, 10, 11);
  Actor_SetVariant(12, 1);
  Actor_SetVariant(13, 1);
  Func_02003d90();
  Func_02003da8(0, 520, 12, 13);
  Func_02003db8(gData_02003fc4);
  if (Object_GetById(19)->y > (s32)0xfff80000)
    Actor_SetVariant(19, 1);
  else
    Actor_SetVariant(19, 2);
  Func_02003cc0(20);
  Object_GetById(20)->priorityFlags |= 32;
  Actor_SetVariant(20, 2);
  if (Flag_Test(0x841) != 0)
    Func_0200172c();
  else
    Map_CopyAttributes(38, 32, 1, 1, 38, 33);
  Object_GetById(14)->priorityFlags &= ~1;
  Object_GetById(15)->priorityFlags &= ~1;
  Object_GetById(16)->priorityFlags &= ~1;
  Object_GetById(17)->priorityFlags &= ~1;
  Func_02000c64(gData_02003fc8);
  object = Object_GetById(17);
  zero = 0;
  if ((object->x >> 20) == 54) {
    struct MapCell *cell = Ram_HeapSlots->mapWork->layers[0].cells;

    cell += Func_02003db0();
    cell->attributeB &= 0x7f;
    cell->collisionCode = 11;
  }
  Actor_SetVariant(14, 1);
  Actor_SetVariant(15, 1);
  Func_02001930();
  if (Flag_Test(0x86e) != 0) {
    /* FAKEMATCH: the named angle is started before the two coordinates, as in
     * Func_02003164. */
    s32 angle = 0x4000;

    Actor_PlaceFacing(25, 212 << 17, 208 << 15, angle);
  }
  Object_SetModeById(18, 3);
  if (Flag_Test(0x86f) != 0) {
    /* FAKEMATCH: the named angle is started before the two coordinates, as in
     * Func_02003164. */
    s32 angle = 0x4000;

    object = Object_GetById(18);
    Map_CopyAttributes(27, 8, 1, 1, 27, 9);
    Object_SetModeById(18, 4);
    object->priorityFlags |= 2;
    *((u8 *)object + 89) &= ~1;
    Flag_Set(517);
    Flag_Set(577);
    Map_CopyCells(69, 51, 92, 11, 1, 1);
    Actor_PlaceFacing(25, 154 << 18, 140 << 17, angle);
  }
  if (Flag_Test(0x870) != 0) {
    Flag_Set(528);
    Flag_Set(534);
    Func_02003c68(25, 0, 0);
  }
  Func_020001d0(gData_02003f80);
  Func_020013b0();
  Func_02001fc4();
  object = Object_GetById(27);
  *((u8 *)object + 85) = zero;
  object->y = zero;
  gData_0200567c = zero;
  gData_02005678 = zero;
  Scheduler_AddOrUpdateCallback(Func_02001ee8, 0x480);
}
