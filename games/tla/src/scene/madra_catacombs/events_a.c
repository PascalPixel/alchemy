#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

extern u8 MsgFieldDoorWontBudge;
extern const u8 gData_020013c0[];
extern const u8 gData_020013c4[];

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);
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
  UiText_ShowPositionedMessageAndWait((s32)&MsgFieldDoorWontBudge, 1);
  work->flag = 1;
  Func_02001114();
}

void Func_020004a4(struct ObjectRuntime *object) {
  u8 *flags = &object->motionFlags;

  *flags = 3;
  do {
    WaitFrames(1);
  } while (object->velocityY != 0);
  Func_02001214(188);
  WaitFrames(10);
  *flags = 0;
}

void Func_020004cc(void) {
  struct ObjectRuntime *object = Object_GetById(11);
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
  struct ObjectRuntime *object = Object_GetById(12);

  Func_020011ec();
  if ((object->x >> 20) == 44 && (object->z >> 20) == 26)
    GameFlag_SetBit(0x8ca);
}

void Func_02000558(void) { Func_020011cc(gData_020013c4); }

extern const u8 gData_02001264[];
extern const u8 gData_020012f4[];

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
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
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001134(id, x, z);
}

static __inline__ void Speed_Set(s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001164(speed, acceleration);
}

static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

void Func_02000568(void) {
  struct ObjectRuntime *left = Object_GetById(8);
  struct ObjectRuntime *right = Object_GetById(9);
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
    right->x += 0x1999;
    WaitFrames(1);
  }
  Speed_Set(0x33333, 0x6666);
  Camera_SetBounds(160 << 16, -1, 240 << 16, 1);
  Func_02001174();
  Speed_Set(0x6666, 0xccc);
  Camera_SetBounds(160 << 16, -1, 186 << 16, 1);
  Func_02001214(168);
  Actor_Place(14, 136 << 16, 258 << 16);
  Actor_Place(19, 184 << 16, 258 << 16);
  ObjectMotion_EnableActionAndSetCallback(14, gData_02001264);
  ObjectMotion_EnableActionAndSetCallback(19, gData_02001264);
  FieldEvent_WaitFrames(40);
  Func_02001214(168);
  Actor_Place(13, 120 << 16, 226 << 16);
  Actor_Place(18, 200 << 16, 226 << 16);
  ObjectMotion_EnableActionAndSetCallback(13, gData_02001264);
  ObjectMotion_EnableActionAndSetCallback(18, gData_02001264);
  FieldEvent_WaitFrames(40);
  Func_02001214(168);
  Actor_Place(12, 120 << 16, 194 << 16);
  Actor_Place(17, 200 << 16, 194 << 16);
  ObjectMotion_EnableActionAndSetCallback(12, gData_02001264);
  ObjectMotion_EnableActionAndSetCallback(17, gData_02001264);
  FieldEvent_WaitFrames(60);
  Func_02001214(229);
  Actor_Place(11, 136 << 16, 162 << 16);
  Actor_Place(16, 184 << 16, 162 << 16);
  Actor_Place(10, 136 << 16, 138 << 16);
  Actor_Place(15, 184 << 16, 138 << 16);
  ObjectMotion_EnableActionAndSetCallback(11, gData_020012f4);
  ObjectMotion_EnableActionAndSetCallback(16, gData_020012f4);
  ObjectMotion_EnableActionAndSetCallback(10, gData_020012f4);
  ObjectMotion_EnableActionAndSetCallback(15, gData_020012f4);
  FieldEvent_WaitFrames(80);
  Func_02000958();
  PartyInventory_Remove(459);
  GameFlag_SetBit(0x8c9);
  Func_02001114();
}
