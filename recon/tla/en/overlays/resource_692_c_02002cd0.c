// Matches: differ 0 (tla-en, 2026-11-01): a Magma Rock scene between objects a
// and b: sound 78, the camera to a, a shake and flash (sound 141), a rises on
// its action script (sound 194); sixteen blended objects of type 672 burst from
// a with sound 246 while the blend flickers; sixteen more spiral in around b
// (sound 195), b starting its own script at the fourth; then fifteen rise in a
// column above b (sound 220); the blend fades, the shaking stops, both take
// their closing scripts with sound 574, b takes the rising-effect update
// (Func_02002c48) for 120 frames and the scene ends with sound 390.
// What settled it, for the next function of this shape:
// - The native's allocation (a r9, b r11, first r7, second r10, i r8) needs the
//   spiral's refs at weight 1, burst and column at weight 2: first, then i,
//   second, a, b. flow.c counts a loop only if its header dominates its latch,
//   so a jump into the spiral's middle that exists at flow and is gone after
//   makes it an irreducible, depth-0 loop. The native leaves no trace of that
//   jump; here it is the dead test below, which only combine can fold (nonzero
//   bits of a byte), so it must have a body, or jump.c threads it and it stays.
// - The same weight 1 gives script its 2 refs, so update_equiv_regs moves the
//   pool load to the use, after r0; that needs the load in an earlier block.
// - Value4, Call2 and Call3 from call.h give the argument orders, with reload's
//   spill set r0, r2, r3 (672 set first would hold r0 and bring in r1 and r4).
// - u16 noMotion and (u16)0 are the pooled zeros (halfword constants).
// - Bytes 0x62 and 0x63 read through a union with Actor: address after value,
//   and an alias set conflicting with Actor's keeps the store's order.
// - i * 0x10000 ties the sum to the shift, where << 16 ties it to the load.
// Devices: one dead test, one one-pass block, each tagged; both reach differ 0
// only together. call.h's wrappers carry their own tags.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"
#include "call.h"

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};

extern const u8 gData_02005590[];
extern const s32 gData_02005544[];
extern const s32 gData_02005560[];
extern const u8 gData_020055b4[];
extern const s32 gData_02005570[];
extern const u8 gData_020055d8[];
extern const u8 gData_02005608[];

void Func_02004f30(s32 sound);
void Func_02004e10(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02004d10(s32 a, s32 b, s32 c);
void Func_02004e48(s32 a, s32 b);
void Func_02004e40(s32 a, s32 b);
void Func_02004e50(s32 frames);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Func_02004dc0(s32 id, s32 value);
u32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *vector);
void Func_02004ea0(void);
void Func_02002c48(struct Actor *object);

void Func_02002cd0(s32 a, s32 b) {
  struct Actor *first = Actor_GetById(a);
  struct Actor *second = Actor_GetById(b);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gData_020055b4;
  Func_02004f30(78);
  FieldEvent_Wait(30);
  Func_02004e10(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&first->x)[1] << 16,
                             ((s16 *)&first->y)[1] << 16,
                             ((s16 *)&first->z)[1] << 16, 1);
  Func_02004f30(141);
  Call3((void (*)())Func_02004d10, 0x20000, 0x20000, 0x10000);
  Call2((void (*)())Func_02004e48, 0x10000, 0);
  Func_02004e40(0x10003, 1);
  Func_02004e50(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(a, gData_02005590);
  Func_02004f30(194);
  FieldEvent_Wait(45);
  Call2((void (*)())Func_02004dc0, a, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  /* FAKEMATCH(one-pass): the empty block keeps i's zero after the blend store; as
   * i = 0 with no block, 2 of 402 instructions differ in tla-en. */
  do {
  } while (0);
  i = 0;
burst:
  object = (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 672, first->x,
                                  first->y, first->z);
  Func_02004f30(246);
  object->x += ((Random16Far() & 31) - 16) << 16;
  object->y += ((Random16Far() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 10);
  Scene_SetActorSpritePriority(object, 1);
  Func_02004c98(object, 7);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02004ca0(object, gData_02005544);
  REG_BLDALPHA = 0x1004;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  REG_BLDALPHA = 0x1010;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  i++;
  if (i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
  /* FAKEMATCH(dead): the never-taken jump into the spiral keeps its refs at
   * weight 1; as the spiral alone, 201 of 402 instructions differ in tla-en. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  spark = (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 672, second->x,
                                 second->y, second->z);
  Func_02004f30(195);
  spark->action = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  Actor_SetPartAttribute(spark, 10);
  Scene_SetActorSpritePriority(spark, 1);
  Func_02004c98(spark, 7);
  Actor_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  Call3((void (*)())Vector_AddPolarOffsetFar, spark->unk4c, spark->action,
        (s32)&spark->x);
  Func_02004ca0(spark, gData_02005560);
  if (i == 3) {
    Call2((void (*)())Func_02004dc0, b, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetMovementTypeById(b, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Func_02004f30(220);
  wait(16);
  i = 2;
column:
  riser = (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 672, second->x,
                                 second->y + i * 0x10000 - 0x80000, second->z);
  riser->action = Random16Far();
  riser->unk66 = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = (u16)0;
  riser->priorityFlags = 1;
  Actor_SetPartAttribute(riser, 10);
  Func_02004c98(riser, 7);
  Actor_SetSingleChildFlags(riser, 0);
  Animation_SetStateFlags(riser, 1);
  Func_02004ca0(riser, gData_02005570);
  i += 2;
  if (i != 32)
    goto column;
  Func_02004f30(220);
  wait(50);
  Func_02004e40(0x7fff, 0);
  Func_02004e50(8);
  wait(16);
  Func_02004d10(0, 0, 0);
  Func_02004dc0(a, 0);
  Func_02004dc0(b, 0);
  Func_02004f30(574);
  Func_02004e40(0x10000, 0);
  Func_02004e50(80);
  Actor_SetMovementTypeById(a, gData_020055d8);
  Actor_SetMovementTypeById(b, gData_02005608);
  second->update = (void (*)(void))Func_02002c48;
  wait(120);
  second->update = 0;
  Func_02004f30(390);
  Func_02004ea0();
  wait(1);
}
