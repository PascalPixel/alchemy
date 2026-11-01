// Draft, differ 214 of 402, skeleton gap 56 (tla-en, 2026-11-01): the Psynergy
// Stone's power passing between two objects: the first rises glowing while
// sixteen sparks burst from it with the screen flashing, sixteen sparks then
// spiral down onto the second as it rises, sixteen orbiting sparks circle it,
// the screen whitens and both settle back while the second keeps sparkling for
// two seconds. Overlay 681's Func_020015d0 is a near copy.
// Left: nearly all allocation. The native has first r7, second r10, i r8,
// firstId r9, secondId r11; here first r8, second r7, i r10, firstId r11,
// secondId r9, so the high-register movs differ and the code is shorter, which
// also moves the literal pools and turns the native's beq plus b at the second
// loop's end into bne. global.c ranks first .291, i .285, second .313 and
// secondId above firstId; the native's order fits only if the two refs of
// secondId and the three of second inside `if (i == 3)` are not doubled by loop
// depth, and no form tried did that (an inline helper is undone by CSE; the
// loop forms below). The native's sub sp, #12 is unexplained: no stack slot is
// ever read, and no local was added for it.
// Also left: the zeros stored in the second and third loops come from the pool
// in the native, inside the loop; a label loop keeps them there but the loop
// notes of for (;;) and do-while (better skeleton) hoist them. The first loop
// must stay a label loop or its constants and the register pointer hoist.
// Store plain constants: a halfword constant becomes a pool load only when no
// local holds it. One spark per loop is what gives r5 to the spark.
#include "iwram_call.h"
#include "call.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "system/io_reg.h"


/// The field object fields these sparks use, with the two spark bytes at 0x62.
struct Spark {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x23 - 0x14];
  u8 priorityFlags;
  u8 unk24[0x30 - 0x24];
  s32 speedLimit;
  s32 acceleration;
  u8 unk38[0x44 - 0x38];
  s32 bounce;
  s32 gravity;
  s32 unk4c;
  u8 unk50[0x55 - 0x50];
  u8 motionFlags;
  u8 unk56[0x62 - 0x56];
  u8 unk62;
  u8 unk63;
  s16 action;
  u16 unk66;
  struct Spark *watchedObject;
  void (*update)(void);
};

extern u8 gData_02004034[];
extern u8 gData_02003fe8[];
extern u8 gData_02004004[];
extern u8 gData_02004058[];
extern u8 gData_02004014[];
extern u8 gData_0200407c[];
extern u8 gData_020040ac[];
void Func_02001598(void);
struct Spark *Actor_GetById(s32 id);
void Func_02003ef4();
void FieldEvent_Wait();
void Func_02003e44();
void Camera_MoveWithinMapBounds();
void Func_02003d44();
void Func_02003e7c();
void Func_02003e74();
void Func_02003e84();
void Actor_SetMovementTypeById();
void Func_02003e04();
struct Spark *Import_ActorCreate(s32 kind, s32 x, s32 y, s32 z);
s32 Random16Far(void);
void Actor_SetPartAttribute();
void Scene_SetActorSpritePriority();
void Func_02003cc4();
void Actor_SetSingleChildFlags();
void Animation_SetStateFlags();
void Func_02003ccc();
void wait();
void Vector_AddPolarOffsetFar();
void Func_02003ebc(void);

void Func_02001620(s32 firstId, s32 secondId) {
  struct Spark *first = Actor_GetById(firstId);
  struct Spark *second = Actor_GetById(secondId);
  struct Spark *spark;
  struct Spark *orbit;
  struct Spark *ring;
  volatile u16 *alpha;
  s16 *angle;
  s32 i;
  s32 zero32;

  Call1((void (*)())Func_02003ef4, 78);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Func_02003e44, 0x8000, 0x1000);
  Call4((void (*)())Camera_MoveWithinMapBounds, *(s16 *)((u8 *)first + 10) << 16,
        *(s16 *)((u8 *)first + 14) << 16, *(s16 *)((u8 *)first + 18) << 16, 1);
  Call1((void (*)())Func_02003ef4, 141);
  Call3((void (*)())Func_02003d44, 0x20000, 0x20000, 0x10000);
  Call2((void (*)())Func_02003e7c, 0x10000, 0);
  Call2((void (*)())Func_02003e74, 0x10003, 1);
  Call1((void (*)())Func_02003e84, 60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Call2((void (*)())Actor_SetMovementTypeById, firstId, (s32)gData_02004034);
  Call1((void (*)())Func_02003ef4, 194);
  Call1((void (*)())FieldEvent_Wait, 45);
  Call2((void (*)())Func_02003e04, firstId, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;

  i = 0;
loop1:
    spark = Import_ActorCreate(0x2a0, first->x, first->y, first->z);
    Call1((void (*)())Func_02003ef4, 246);
    spark->x += ((Random16Far() & 31) - 16) << 16;
    spark->y += ((Random16Far() & 15) - 8) << 16;
    spark->speedLimit = 0x80000;
    spark->acceleration = 0x3333;
    spark->motionFlags = 0;
    Call2((void (*)())Actor_SetPartAttribute, (s32)spark, 2);
    Call2((void (*)())Scene_SetActorSpritePriority, (s32)spark, 1);
    Call2((void (*)())Func_02003cc4, (s32)spark, 7);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)spark, 0);
    Call2((void (*)())Animation_SetStateFlags, (s32)spark, 1);
    Call2((void (*)())Func_02003ccc, (s32)spark, (s32)gData_02003fe8);
    alpha = &REG_BLDALPHA;
    *alpha = 0x1004;
    Call1((void (*)())wait, 2);
    *alpha = 0x100a;
    Call1((void (*)())wait, 2);
    *alpha = 0x1010;
    Call1((void (*)())wait, 2);
    *alpha = 0x100a;
    Call1((void (*)())wait, 2);
    i++;
  if (i != 16)
    goto loop1;
  *alpha = 0x1008;
  Call1((void (*)())wait, 30);

  i = 0;
  for (;;) {
    orbit = Import_ActorCreate(0x2a0, second->x, second->y, second->z);
    Call1((void (*)())Func_02003ef4, 195);
    angle = &orbit->action;
    *angle = (u16)(Random16Far() & 0x7fff) + 0x8000;
    orbit->unk66 = ((i & 1) * 2 - 1) << ((Random16Far() & 3) + 9);
    orbit->motionFlags = 0;
    orbit->unk4c = 0x7a0000;
    orbit->watchedObject = second;
    orbit->unk62 = (16 - i) * 8 + 15;
    Call2((void (*)())Actor_SetPartAttribute, (s32)orbit, 2);
    Call2((void (*)())Scene_SetActorSpritePriority, (s32)orbit, 1);
    Call2((void (*)())Func_02003cc4, (s32)orbit, 7);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)orbit, 0);
    Call2((void (*)())Animation_SetStateFlags, (s32)orbit, 1);
    Call3((void (*)())Vector_AddPolarOffsetFar, orbit->unk4c, *angle, (s32)&orbit->x);
    Call2((void (*)())Func_02003ccc, (s32)orbit, (s32)gData_02004004);
    if (i == 3) {
      Call2((void (*)())Func_02003e04, secondId, 0x100);
      second->motionFlags = 0;
      second->acceleration = 0x3333;
      second->speedLimit = 0x3333;
      Call2((void (*)())Actor_SetMovementTypeById, secondId, (s32)gData_02004058);
    }
    Call1((void (*)())wait, 8);
    i++;
    if (i == 16)
      break;
  }

  Call1((void (*)())Func_02003ef4, 220);
  Call1((void (*)())wait, 16);

  i = 2;
  do {
    ring = Import_ActorCreate(0x2a0, second->x, (i << 16) + second->y - 0x80000,
                               second->z);
    ring->action = Random16Far();
    ring->unk66 = 0x800;
    ring->unk62 = i;
    ring->unk63 = 1;
    zero32 = 0;
    ring->bounce = ring->x;
    ring->unk4c = zero32;
    ring->gravity = ring->z;
    ring->watchedObject = second;
    ring->motionFlags = 0;
    ring->priorityFlags = 1;
    Call2((void (*)())Actor_SetPartAttribute, (s32)ring, 2);
    Call2((void (*)())Func_02003cc4, (s32)ring, 7);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)ring, 0);
    Call2((void (*)())Animation_SetStateFlags, (s32)ring, 1);
    Call2((void (*)())Func_02003ccc, (s32)ring, (s32)gData_02004014);
    i += 2;
  } while (i != 32);

  Call1((void (*)())Func_02003ef4, 220);
  Call1((void (*)())wait, 50);
  Call2((void (*)())Func_02003e74, 0x7fff, 0);
  Call1((void (*)())Func_02003e84, 8);
  Call1((void (*)())wait, 16);
  Call3((void (*)())Func_02003d44, 0, 0, 0);
  Call2((void (*)())Func_02003e04, firstId, 0);
  Call2((void (*)())Func_02003e04, secondId, 0);
  Call1((void (*)())Func_02003ef4, 0x23e);
  Call2((void (*)())Func_02003e74, 0x10000, 0);
  Call1((void (*)())Func_02003e84, 80);
  Call2((void (*)())Actor_SetMovementTypeById, firstId, (s32)gData_0200407c);
  Call2((void (*)())Actor_SetMovementTypeById, secondId, (s32)gData_020040ac);
  second->update = Func_02001598;
  Call1((void (*)())wait, 120);
  second->update = (void (*)(void))zero32;
  Call1((void (*)())Func_02003ef4, 0x186);
  Func_02003ebc();
  Call1((void (*)())wait, 1);
}
