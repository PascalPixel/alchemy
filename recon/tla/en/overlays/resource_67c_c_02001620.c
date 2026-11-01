// Matches: differ 0 (tla-en, 2026-11-01): the Psynergy Stone's power passing
// between two objects: the first rises glowing while sixteen sparks burst from it
// with the screen flashing, sixteen sparks then spiral down onto the second as it
// rises, fifteen more stand in a column over it, the screen whitens and both
// settle back while the second keeps sparkling for two seconds.
// The listing equals overlay 681's Func_020015d0 and 692's Func_02002cd0 but for
// addresses; 664's Func_02004138 is the same shape. This is 692's form, which has
// no asm, where 681 needs three barriers.
// What closed it: all three loops are goto loops. The native's allocation (first
// r7, second r10, i r8, firstId r9, secondId r11) needs the spiral's refs at
// weight 1, so a jump into its middle has to exist at flow and be gone after: the
// tagged dead test, which only combine can fold. The same weight gives script its
// two refs, so its pool load moves to the use; that needs the load in an earlier
// block. Bytes 0x62 and 0x63 read through a union with struct Actor so the store
// order holds; noMotion and (u16)0 are the pooled zeros; unused[3] is the frame.
// Tried for the one-pass block, none better than 2: i = 0 above the blend stores,
// the stores swapped, a zero local, a block after the label, a static inline for
// the stores or the zero, a forced temporary for the values.
#include "types.h"
#include "actor.h"
#include "call.h"
#include "system/io_reg.h"

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};
extern const u8 gData_02004034[], gData_02003fe8[], gData_02004004[],
    gData_02004058[], gData_02004014[], gData_0200407c[], gData_020040ac[];
struct Actor *Actor_GetById(u32 id);
void Func_02003ef4(s32 cue);
void FieldEvent_Wait(s32 frames);
void Func_02003e44(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02003d44(s32 a, s32 b, s32 c);
void Func_02003e7c(s32 a, s32 b);
s32 Func_02003e74(s32 a, s32 b);
s32 Func_02003e84(s32 frames);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Func_02003e04(s32 actor, s32 a);
void *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Actor_SetPartAttribute(void *object, s32 value);
void Scene_SetActorSpritePriority(void *object, s32 priority);
void Func_02003cc4(void *object, s32 mode);
void Actor_SetSingleChildFlags(void *object, u32 value);
void Animation_SetStateFlags(void *object, s32 flags);
void Func_02003ccc(void *object, const u8 *script);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
void wait(s32 frames);
void Func_02001598(void);
void Func_02003ebc(void);

void Func_02001620(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gData_02004058;
  Func_02003ef4(78);
  FieldEvent_Wait(30);
  Func_02003e44(0x8000, 0x1000);
  Camera_MoveWithinMapBounds((first->x >> 16) << 16,
                             (first->y >> 16) << 16,
                             (first->z >> 16) << 16, 1);
  Func_02003ef4(141);
  /* FAKEMATCH(wrapper): forwarded, so each argument builds its own 0x20000; as a
   * direct call, 17 of 402 instructions differ in tla-en. */
  Call3((void (*)())Func_02003d44, 0x20000, 0x20000, 0x10000);
  Func_02003e7c(0x10000, 0);
  Func_02003e74(0x10003, 1);
  Func_02003e84(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(firstId, gData_02004034);
  Func_02003ef4(194);
  FieldEvent_Wait(45);
  /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2 of
   * 402 instructions differ in tla-en. */
  Call2((void (*)())Func_02003e04, firstId, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  /* FAKEMATCH(one-pass): the empty block keeps i's zero after the blend store; as
   * i = 0 with no block, 2 of 402 instructions differ in tla-en. */
  do {
  } while (0);
  i = 0;
burst:
  object = Import_ActorCreate(672, first->x, first->y, first->z);
  Func_02003ef4(246);
  object->x += ((Random16Far() & 31) - 16) << 16;
  object->y += ((Random16Far() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Func_02003cc4(object, 7);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02003ccc(object, gData_02003fe8);
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
   * weight 1; as the spiral alone, 174 of 402 instructions differ in tla-en. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  /* FAKEMATCH(wrapper): forwarded, so second->z takes r0 for its reload; as a
   * direct call, 105 of 402 instructions differ in tla-en. */
  spark = (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 672, second->x,
                                 second->y, second->z);
  Func_02003ef4(195);
  spark->action = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  Actor_SetPartAttribute(spark, 2);
  Scene_SetActorSpritePriority(spark, 1);
  Func_02003cc4(spark, 7);
  Actor_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2 of
   * 402 instructions differ in tla-en. */
  Call3((void (*)())Vector_AddPolarOffsetFar, spark->unk4c, spark->action,
        (s32)&spark->x);
  Func_02003ccc(spark, gData_02004004);
  if (i == 3) {
    /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2 of
     * 402 instructions differ in tla-en. */
    Call2((void (*)())Func_02003e04, secondId, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetMovementTypeById(secondId, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Func_02003ef4(220);
  wait(16);
  i = 2;
column:
  /* FAKEMATCH(wrapper): forwarded, so second->z takes r0 for its reload; as a
   * direct call, 116 of 402 instructions differ in tla-en. */
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
  Actor_SetPartAttribute(riser, 2);
  Func_02003cc4(riser, 7);
  Actor_SetSingleChildFlags(riser, 0);
  Animation_SetStateFlags(riser, 1);
  Func_02003ccc(riser, gData_02004014);
  i += 2;
  if (i != 32)
    goto column;
  Func_02003ef4(220);
  wait(50);
  Func_02003e74(0x7fff, 0);
  Func_02003e84(8);
  wait(16);
  Func_02003d44(0, 0, 0);
  Func_02003e04(firstId, 0);
  Func_02003e04(secondId, 0);
  Func_02003ef4(574);
  Func_02003e74(0x10000, 0);
  Func_02003e84(80);
  Actor_SetMovementTypeById(firstId, gData_0200407c);
  Actor_SetMovementTypeById(secondId, gData_020040ac);
  second->update = Func_02001598;
  wait(120);
  second->update = 0;
  Func_02003ef4(390);
  Func_02003ebc();
  wait(1);
}
