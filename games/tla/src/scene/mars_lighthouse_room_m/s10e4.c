#include "mars_lighthouse_room_m.h"
#include "edition.h"
#include "scene_tables.h"
#include "io_reg.h"

s32 PartyInventory_FindOwner(s32 item);
void Func_02003218(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Func_02003228(void);
void Func_0200080c(void);

void Func_020010e4(void) {
  volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Func_02003280(0);
  Call2((void (*)())Func_02003250, 0x10000, 0);
  Call2((void (*)())Func_02003248, 0x10003, 0);
  Func_02003258(20);
  wait(20);
  *color = 0x7fff;
  Call1((void (*)())wait, 1);
  Func_020031c0((s32)MsgMankindWithThe, 0, 0);
  Call2((void (*)())Func_02003248, 0x10000, 0);
  Func_02003258(20);
  wait(20);
  Func_020031e8();
}

void Func_02001154(void) {
  volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Func_02003280(0);
  Call2((void (*)())Func_02003250, 0x10000, 0);
  Call2((void (*)())Func_02003248, 0x10003, 0);
  Func_02003258(20);
  wait(20);
  *color = 0x7fff;
  Call1((void (*)())wait, 1);
  Func_020031c0((s32)MsgFishWithCold, 0, 0);
  Call2((void (*)())Func_02003248, 0x10000, 0);
  Func_02003258(20);
  wait(20);
  Func_020031e8();
}

void Func_020011c4(void) {
  volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Func_02003280(0);
  Call2((void (*)())Func_02003250, 0x10000, 0);
  Call2((void (*)())Func_02003248, 0x10003, 0);
  Func_02003258(20);
  wait(20);
  *color = 0x7fff;
  Call1((void (*)())wait, 1);
  Func_020031c0((s32)MsgDragonsBurningWith, 0, 0);
  Call2((void (*)())Func_02003248, 0x10000, 0);
  Func_02003258(20);
  wait(20);
  Func_020031e8();
}

void Func_02001234(void) {
  volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Func_02003280(0);
  Call2((void (*)())Func_02003250, 0x10000, 0);
  Call2((void (*)())Func_02003248, 0x10003, 0);
  Func_02003258(20);
  wait(20);
  *color = 0x7fff;
  Call1((void (*)())wait, 1);
  Func_020031c0((s32)MsgBirdOnThe, 0, 0);
  Call2((void (*)())Func_02003248, 0x10000, 0);
  Func_02003258(20);
  wait(20);
  Func_020031e8();
}

void Func_020012a4(s32 x, s32 z, s32 kind) {
  struct Actor *object = (struct Actor *)Value4(
      (s32 (*)())Func_02003128, 403, x << 16, 0, z << 16);
  struct MarsLighthouseRoomMAnimation *animation;

  if (object != 0) {
    Actor_SetSingleChildFlags(object, 0);
    Func_02003118(object, 3);
    animation = object->animation;
    if (kind != 0) {
      animation->layer = 2;
      Func_02003120(object, gData_0200387c);
    } else {
      animation->layer = 1;
      object->velocityX = (((u32)(Random16Far() * 8) >> 16) - 4) << 16;
      object->velocityY = PIXELS(4);
      Func_02003120(object, gData_02003900);
    }
  }
}

void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f) {
  u32 i;
  s32 px;
  s32 pz;

  Func_020032b8(144);
  Func_020012a4(x, z + 12, 1);
  for (i = 0; i < 3; i++) {
    px = x + ((u32)(Random16Far() * 12) >> 16) - 6;
    pz = z + ((u32)(Random16Far() * 12) >> 16) - 6;
    Func_020012a4(px, pz, 0);
  }
  wait(4);
  Func_02003148(c, d, e + 64, f, 1, 1);
}

/// No one holding item 222: MsgIfYouDo, once (flag 0xa2f). Someone holding it
/// (and flag 0xa30 set, outside the Japanese edition): MsgYouWhoHold and the
/// message after it, the camera moves on and Func_0200080c runs, then flag
/// 0xa2e is set. Each opens as Func_020010e4 does.
void Func_020013a0(void) {
  if (PartyInventory_FindOwner(222) == -1) {
    if (!Value1((s32 (*)())GameFlag_Test, 0xa2f)) {
      volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

      Func_020031e0();
      Func_02003280(0);
      Call2((void (*)())Func_02003218, 0x20000, 0x4000);
      Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(224), PIXELS(64),
            PIXELS(216), 1);
      Func_02003228();
      Call1((void (*)())FieldEvent_Wait, 20);
      Call2((void (*)())Func_02003250, 0x10000, 0);
      Call2((void (*)())Func_02003248, 0x10003, 0);
      Call1((void (*)())Func_02003258, 20);
      Call1((void (*)())wait, 20);
      *color = 0x7fff;
      Call1((void (*)())wait, 1);
      Func_020031c0((s32)MsgIfYouDo, 0, 4);
      Call2((void (*)())Func_02003248, 0x10000, 0);
      Call1((void (*)())Func_02003258, 20);
      Call1((void (*)())wait, 20);
      Call1((void (*)())GameFlag_SetBit, 0xa2f);
      Func_020031e8();
    }
  }
#if (LANGUAGE == LANGUAGE_JAPANESE)
  else {
#else
  else if (Value1((s32 (*)())GameFlag_Test, 0xa30)) {
#endif
    volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);
    s32 message;

    Func_020031e0();
    Func_02003280(0);
    Call2((void (*)())Func_02003218, 0x20000, 0x4000);
    Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(224), PIXELS(64), PIXELS(216),
          1);
    Func_02003228();
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Func_02003250, 0x10000, 0);
    Call2((void (*)())Func_02003248, 0x10003, 0);
    Call1((void (*)())Func_02003258, 20);
    Call1((void (*)())wait, 20);
    *color = 0x7fff;
    Call1((void (*)())wait, 1);
    message = (s32)MsgYouWhoHold;
    Func_020031c0(message, 0, 4);
    message++;
    Call1((void (*)())wait, 10);
    Func_020031c0(message, 0, 3);
    Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(224), PIXELS(88), PIXELS(216),
          1);
    Func_02003228();
    Call1((void (*)())FieldEvent_Wait, 20);
    Call1((void (*)())Func_020032b8, 560);
    Func_0200080c();
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Func_02003248, 0x10000, 0);
    Call1((void (*)())Func_02003258, 20);
    Call1((void (*)())wait, 40);
    Call1((void (*)())GameFlag_SetBit, 0xa2e);
    Func_020031e8();
  }
}
