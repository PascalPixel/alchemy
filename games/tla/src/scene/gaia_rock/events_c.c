#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "call.h"
#include "event_work.h"

struct SceneEntrance;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void FieldEvent_Wait(s32 frames);
void Actor_StartRepeatedMotion(s32 id, s32 variant);
void FieldMessage_Show(const u8 *message, s32 flags);
void Func_02000e44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000e74(void);
void Func_02000e7c(void);
void Func_02000e8c(s32 id, s32 x, s32 z);
void Func_02000ea4(const u8 *message);
void Func_02000eac(s32 id, s32 flags);
s32 Func_02000eb4(s32 id, s32 flags);
void Func_02000edc(s32 a);
void Func_02000ef4(s32 a);
void Func_02000f04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern const u8 MsgCheckedTheAltar[];
extern const u8 MsgIMSensing[];
extern const u8 MsgICanT[];
extern const u8 MsgYouReGoing[];
extern const u8 MsgIfYouRe[];
extern const u8 MsgKushinadaSSafe[];
extern const u8 MsgIReallyGot[];

extern const u8 gData_020013c8[];
extern const u8 gData_020014b8[];
extern const u8 gData_02001554[];

void Func_02000258(void) {
  Func_02000e44(13, 18, 1, 1, 13, Actor_GetById(8)->z >> 20);
}

void Func_0200027c(void) {
  Func_02000e44(11, 14, 1, 1, 13, Actor_GetById(8)->z >> 20);
}

void Func_020002a0(void) {
  Call7((void (*)())Func_02000f04, 110, 64, 14, 12, 110, 47, 1);
}

void Func_020002c0(void) {
  struct Actor *object = Actor_GetById(64);
  s32 done;

  if (object == NULL)
    return;
  done = GameFlag_Test(0xf74);
  if (done == 0) {
    Call3((void (*)())Func_02000e8c, 64, 0x2910000, 0xf60000);
    object->motionFlags = done;
    object->y = 0x500000;
  }
}

void Func_020002fc(void) {
  if (Actor_GetById(64) != NULL)
    Func_02000e8c(64, 0, 0);
}

void Func_02000314(void) {
  struct Actor *object = Actor_GetById(64);

  FieldMessage_Show(MsgCheckedTheAltar, 1);
  if (object != NULL && object->x != 0) {
    Func_02000edc(0x1040);
  } else if (GameFlag_Test(0xf74) == 0) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    /* FAKEMATCH: a one-pass block keeps the work pointer load before the
     * message arguments (one reordered row). */
    do {
    } while (0);
    FieldMessage_Show(MsgIMSensing, 1);
    work->unk15a = 1;
  } else {
    FieldMessage_Show(MsgICanT, 1);
  }
}

void Func_02000378(void) {
  if (GameFlag_Test(0x927)) {
    Func_02000e74();
    Func_02000ef4(0);
    Func_02000ea4(MsgYouReGoing);
    Call2((void (*)())Actor_StartRepeatedMotion, 20, 1);
    FieldEvent_Wait(15);
    Func_02000eb4(20, 0);
    GameFlag_SetBit(0x9ab);
    Func_02000e7c();
  }
}

void Func_020003c4(void) {
  if (GameFlag_Test(0x9ab)) {
    Func_02000ea4(MsgYouReGoing);
    Func_02000eb4(20, 0);
  } else {
    Func_02000ea4(MsgIfYouRe);
    Func_02000eac(20, 0);
  }
}

void Func_020003fc(void) {
  if (GameFlag_Test(0x9ab)) {
    Func_02000ea4(MsgKushinadaSSafe);
    Func_02000eac(20, 0);
  } else {
    Func_02000ea4(MsgIReallyGot);
    Func_02000eac(20, 0);
  }
}

const struct SceneEntrance *Func_02000434(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov680Room1)
    return (const struct SceneEntrance *)gData_020013c8;
  if (gAdventure.scene == (s32)&SceneId_Ov680Room2)
    return (const struct SceneEntrance *)gData_020014b8;
  return (const struct SceneEntrance *)gData_02001554;
}
