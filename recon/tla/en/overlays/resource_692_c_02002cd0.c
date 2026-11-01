// Draft, differ 236 of 402, skeleton gap 94 (tla-en, 2026-11-01): a Magma Rock
// scene between objects a and b: sound 78, the camera to a, a shake and flash
// (sound 141), a rises on its action script (sound 194); sixteen blended
// objects of type 672 burst from a with sound 246 while the blend flickers;
// sixteen more spiral in around b (sound 195), b starting its own script at the
// fourth; then fifteen rise in a column above b (sound 220); the blend fades,
// the shaking stops, both take their closing scripts with sound 574, b takes
// the rising-effect update (Func_02002c48) for 120 frames and the scene ends
// with sound 390.
// Settled by measurement: all three loops are label-and-goto loops, since as
// for loops loop.c hoists the blend address, 0x100a, 0x7fff and 0xfff80000,
// none of which the native hoists. One Actor pointer per loop puts each in r5.
// Left: most of the gap is allocation. The native has a r9, b r11, first r7,
// second r10, i r8; here second takes r7, first r10, a r11, b r9, and the high
// base costs the extra movs and r4 reloads the listing lacks. Priorities here:
// second 25 refs over 334 insns beats first 13 over 140 and i 31 over 444;
// the native's order needs second at 23 or fewer and b at 6 or fewer, as if the
// i == 3 block's refs were not loop-weighted, which no C shape tried gives.
// The native's unused 12-byte frame says reload spilled there and reallocated
// three pseudos, and its pooled zero for motionFlags (ldr r7 in the spiral, ldr
// r1 in the column) says that zero lost its register; the noMotion local aims
// at that but keeps a register here, and set any earlier it spills a real
// argument (frame 4). Tried with no gain: register on first, one counter per
// loop (first's and second's order unchanged, gap 99), noMotion as s32.
// Func_02004d10 and Func_02004e48 share 0x20000 and 0x10000 here where the
// native builds each: Call3 and Call2 as s28ac.c writes them gain 9 of the gap
// and 7 of differ (measured before the pointers were split), left out as a
// device. Their arguments' shift order and the
// column's byte store order (reload's address reuse) follow from the same
// causes.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"

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
  u8 noMotion;

  Func_02004f30(78);
  FieldEvent_Wait(30);
  Func_02004e10(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&first->x)[1] << 16,
                             ((s16 *)&first->y)[1] << 16,
                             ((s16 *)&first->z)[1] << 16, 1);
  Func_02004f30(141);
  Func_02004d10(0x20000, 0x20000, 0x10000);
  Func_02004e48(0x10000, 0);
  Func_02004e40(0x10003, 1);
  Func_02004e50(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(a, gData_02005590);
  Func_02004f30(194);
  FieldEvent_Wait(45);
  Func_02004dc0(a, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = Import_ActorCreate(672, first->x, first->y, first->z);
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
  noMotion = 0;
  i = 0;
spiral:
  spark = Import_ActorCreate(672, second->x, second->y, second->z);
  Func_02004f30(195);
  spark->action = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((u8 *)spark)[0x62] = ((16 - i) << 3) + 15;
  Actor_SetPartAttribute(spark, 10);
  Scene_SetActorSpritePriority(spark, 1);
  Func_02004c98(spark, 7);
  Actor_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  Vector_AddPolarOffsetFar(spark->unk4c, spark->action, &spark->x);
  Func_02004ca0(spark, gData_02005560);
  if (i == 3) {
    Func_02004dc0(b, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetMovementTypeById(b, gData_020055b4);
  }
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Func_02004f30(220);
  wait(16);
  i = 2;
column:
  riser = Import_ActorCreate(672, second->x,
                               second->y + (i << 16) - 0x80000, second->z);
  riser->action = Random16Far();
  riser->unk66 = 0x800;
  ((u8 *)riser)[0x62] = i;
  ((u8 *)riser)[0x63] = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = noMotion;
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
