#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "scene_tables.h"

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

extern u8 MsgFieldDoorWontBudge;
extern const u8 gData_020013c0[];
extern const u8 gData_020013c4[];

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldMessage_Show(s32 message, s32 mode);
void Func_0200110c(void);
void Func_020011ac(s32 skip);
void Func_02001114(void);
void Func_02001214(s32 cue);
void Func_020010dc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020011e4(const void *data);
void Func_020011cc(const void *data);
void Func_020011ec(void);

void Func_02000474(void) {
  struct EventWork15a *work = Ram_HeapSlots->eventWork;

  Func_0200110c();
  Func_020011ac(0);
  FieldMessage_Show((s32)&MsgFieldDoorWontBudge, 1);
  work->flag = 1;
  Func_02001114();
}

void Func_020004a4(struct Actor *object) {
  u8 *flags = &object->motionFlags;

  *flags = 3;
  do {
    wait(1);
  } while (object->velocityY != 0);
  Func_02001214(188);
  wait(10);
  *flags = 0;
}

void Func_020004cc(void) {
  struct Actor *object = Actor_GetById(11);
  s32 z;

  if ((object->x >> 20) == 40) {
    z = object->z >> 20;
    if (z == 13) {
      Func_0200110c();
      Func_020011ac(0);
      Func_020004a4(object);
      Func_020010dc(41, 13, 2, 1, 39, z);
      GameFlag_SetBit(0x8c8);
      Func_02001114();
    }
  }
}

void Func_0200051c(void) { Func_020011e4(gData_020013c0); }

void Func_0200052c(void) {
  struct Actor *object = Actor_GetById(12);

  Func_020011ec();
  if ((object->x >> 20) == 44 && (object->z >> 20) == 26)
    GameFlag_SetBit(0x8ca);
}

void Func_02000558(void) { Func_020011cc(gData_020013c4); }

extern const u8 gData_02001264[];
extern const u8 gData_020012f4[];

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Actor_SetMovementTypeById(s32 id, const void *script);
void PartyInventory_Remove(s32 item);
void Func_0200110c(void);
void Func_020011ac(s32 skip);
void Func_02001114(void);
void Func_02001214(s32 cue);
void Func_02001134(s32 id, s32 x, s32 z);
void Func_02001164(s32 speed, s32 acceleration);
void Func_02001174(void);
void Func_02000958(void);

static __inline__ void Actor_Place(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001134(id, x, z);
}

static __inline__ void Speed_Set(s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001164(speed, acceleration);
}

static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

void Func_02000568(void) {
  struct Actor *left = Actor_GetById(8);
  struct Actor *right = Actor_GetById(9);
  u32 i;

  Func_0200110c();
  Func_020011ac(0);
  Func_02001214(183);
  for (i = 0; i < 120; i++) {
    /* FAKEMATCH: the native keeps the counter in r5 and the right door in r6;
     * global.c ranks them 14/24 against 10/17 without the barrier and gives the
     * door r5. */
    __asm__("" : : "r"(i));
    left->x += -0x1999;
    /* FAKEMATCH(signature): read and written as long, another alias set than
     * the left door's int store, so the load need not wait for that store; as
     * int, 2 of 175 instructions differ in tla-en. */
    *(long *)&right->x += 0x1999;
    wait(1);
  }
  Speed_Set(0x33333, 0x6666);
  Camera_SetBounds(PIXELS(160), -1, PIXELS(240), 1);
  Func_02001174();
  Speed_Set(0x6666, 0xccc);
  Camera_SetBounds(PIXELS(160), -1, PIXELS(186), 1);
  Func_02001214(168);
  Actor_Place(14, PIXELS(136), PIXELS(258));
  Actor_Place(19, PIXELS(184), PIXELS(258));
  Actor_SetMovementTypeById(14, gData_02001264);
  Actor_SetMovementTypeById(19, gData_02001264);
  FieldEvent_Wait(40);
  Func_02001214(168);
  Actor_Place(13, PIXELS(120), PIXELS(226));
  Actor_Place(18, PIXELS(200), PIXELS(226));
  Actor_SetMovementTypeById(13, gData_02001264);
  Actor_SetMovementTypeById(18, gData_02001264);
  FieldEvent_Wait(40);
  Func_02001214(168);
  Actor_Place(12, PIXELS(120), PIXELS(194));
  Actor_Place(17, PIXELS(200), PIXELS(194));
  Actor_SetMovementTypeById(12, gData_02001264);
  Actor_SetMovementTypeById(17, gData_02001264);
  FieldEvent_Wait(60);
  Func_02001214(229);
  Actor_Place(11, PIXELS(136), PIXELS(162));
  Actor_Place(16, PIXELS(184), PIXELS(162));
  Actor_Place(10, PIXELS(136), PIXELS(138));
  Actor_Place(15, PIXELS(184), PIXELS(138));
  Actor_SetMovementTypeById(11, gData_020012f4);
  Actor_SetMovementTypeById(16, gData_020012f4);
  Actor_SetMovementTypeById(10, gData_020012f4);
  Actor_SetMovementTypeById(15, gData_020012f4);
  FieldEvent_Wait(80);
  Func_02000958();
  PartyInventory_Remove(459);
  GameFlag_SetBit(0x8c9);
  Func_02001114();
}
