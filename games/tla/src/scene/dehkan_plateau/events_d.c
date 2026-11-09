#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003de8(void);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_02001554(s32 a, s32 b) { Func_020000a4(gData_02003f98, a, b); }

void Func_02001568(s32 a, s32 b) { Func_020000a4(gData_02003fa0, a, b); }

void Func_0200157c(s32 a, s32 b) { Func_020000a4(gData_02003fa8, a, b); }

void Func_02001590(s32 a, s32 b) { Func_020000a4(gData_02003fb0, a, b); }

void Func_020015a4(s32 a, s32 b) { Func_020000a4(gData_02003fb8, a, b); }

void Func_020015b8(struct Actor *object) {
  u8 *flags = &object->motionFlags;

  *flags = 3;
  do {
    wait(1);
  } while (object->velocityY != 0);
  Sound_Play(188);
  wait(10);
  *flags = 0;
}

void Func_020015e0(void) {
  struct Actor *object = Actor_GetById(12);
  s32 x;

  x = object->x >> 20;
  if (x == 56 && (object->z >> 20) == 32) {
    Func_02003c28();
    Event_ResolvePendingActions(0);
    Func_020015b8(object);
    Map_CopyCellAttributeRect(54, 32, 2, 1, x, 64);
    GameFlag_SetBit(0x301);
    Func_02003c30();
  }
}

void Func_02001630(void) { Func_02003dc0(gData_02003fc0); }

void Func_02001640(void) {
  struct Actor *object = Actor_GetById(23);
  s32 x = object->x >> 20;

  Func_02003dc8();
  if (x == 52) {
    Func_02003c28();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(51, 7, 1, 1, x, 7);
    wait(1);
    Func_020015b8(object);
    Map_CopyCellAttributeRect(50, 7, 1, 1, x, 7);
    GameFlag_SetBit(0x840);
    Func_02003c30();
  }
}

void Func_020016a0(void) { Func_02003dc0(gData_02003fc4); }

void Func_020016b0(void) {
  struct Actor *object = Actor_GetById(19);
  s32 x = object->x >> 20;

  Func_02003dc8();
  if (x == 47) {
    Func_02003c28();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(47, 12, 1, 1, x, 13);
    wait(1);
    Func_020015b8(object);
    Map_CopyCellAttributeRect(50, 16, 1, 1, x, 13);
    Actor_SetSpritePriority(19, 2);
    Func_02003c30();
  } else if (x == 46) {
    Map_CopyAttributes(44, 13, 1, 1, 48, 13);
  }
}

static __inline__ void Actor_PlaceOnTerrain(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_0200172c(void) {
  Actor_PlaceOnTerrain(20, 150 << 18, 134 << 18);
  Map_CopyAttributes(38, 32, 1, 1, 37, 33);
  Map_CopyAttributes(30, 32, 1, 2, 38, 32);
  Map_CopyAttributes(40, 64, 4, 3, 36, 64);
  GameFlag_SetBit(0x841);
}

void Func_02001788(void) {
  if ((Actor_GetById(20)->x >> 20) == 37)
    Func_0200172c();
}

void Func_020017a0(void) {
  u8 *flags;

  if ((Actor_GetById(20)->x >> 20) == 38) {
    flags = &Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags;
    Func_02003c28();
    Event_ResolvePendingActions(0);
    *flags = 0;
    wait(1);
    Func_02003de8();
    Func_0200172c();
    wait(1);
    *flags = 3;
    Func_02003c30();
  }
}

void Func_020017f4(void) { GameFlag_ClearBit(0x201); }

void Func_02001804(void) {
  s32 flag;

  flag = 0x320;
loop:
  GameFlag_ClearBit(flag);
  if (flag != 0x35f) {
    flag++;
    goto loop;
  }
}
