#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

extern u8 MsgFieldDoorWontBudge;
extern const u8 gData_020013c0[];
extern const u8 gData_020013c4[];

void Func_0200110c(void);
void Func_02001114(void);
void Func_020011e4(const void *data);
void Func_020011cc(const void *data);
void Func_020011ec(void);

void Func_02000474(void) {
  struct EventWork15a *work = Ram_HeapSlots->eventWork;

  Func_0200110c();
  Event_ResolvePendingActions(0);
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
  Sound_Play(188);
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
      Event_ResolvePendingActions(0);
      Func_020004a4(object);
      Map_CopyCellAttributeRect(41, 13, 2, 1, 39, z);
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

void PartyInventory_Remove(s32 item);
void Func_0200110c(void);
void Func_02001114(void);
void Func_02001174(void);
void Func_02000958(void);

void Func_02000568(void) {
  struct Actor *left = Actor_GetById(8);
  struct Actor *right = Actor_GetById(9);
  u32 i;

  Func_0200110c();
  Event_ResolvePendingActions(0);
  Sound_Play(183);
  for (i = 0; i < 120; i++) {
    left->x += -0x1999;
    /* FAKEMATCH(signature): read and written as long, another alias set than
     * the left door's int store, so the load need not wait for that store; as
     * int, 2 of 175 instructions differ in tla-en. */
    *(long *)&right->x += 0x1999;
    wait(1);
  }
  FieldCamera_SetSpeed(0x33333, 0x6666);
  FieldCamera_MoveTo(PIXELS(160), -1, PIXELS(240), 1);
  Func_02001174();
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(160), -1, PIXELS(186), 1);
  Sound_Play(168);
  Actor_SetPosition(14, PIXELS(136), PIXELS(258));
  Actor_SetPosition(19, PIXELS(184), PIXELS(258));
  Actor_SetMovementTypeById(14, gData_02001264);
  Actor_SetMovementTypeById(19, gData_02001264);
  Event_Wait(40);
  Sound_Play(168);
  Actor_SetPosition(13, PIXELS(120), PIXELS(226));
  Actor_SetPosition(18, PIXELS(200), PIXELS(226));
  Actor_SetMovementTypeById(13, gData_02001264);
  Actor_SetMovementTypeById(18, gData_02001264);
  Event_Wait(40);
  Sound_Play(168);
  Actor_SetPosition(12, PIXELS(120), PIXELS(194));
  Actor_SetPosition(17, PIXELS(200), PIXELS(194));
  Actor_SetMovementTypeById(12, gData_02001264);
  Actor_SetMovementTypeById(17, gData_02001264);
  Event_Wait(60);
  Sound_Play(229);
  Actor_SetPosition(11, PIXELS(136), PIXELS(162));
  Actor_SetPosition(16, PIXELS(184), PIXELS(162));
  Actor_SetPosition(10, PIXELS(136), PIXELS(138));
  Actor_SetPosition(15, PIXELS(184), PIXELS(138));
  Actor_SetMovementTypeById(11, gData_020012f4);
  Actor_SetMovementTypeById(16, gData_020012f4);
  Actor_SetMovementTypeById(10, gData_020012f4);
  Actor_SetMovementTypeById(15, gData_020012f4);
  Event_Wait(80);
  Func_02000958();
  PartyInventory_Remove(459);
  GameFlag_SetBit(0x8c9);
  Func_02001114();
}
