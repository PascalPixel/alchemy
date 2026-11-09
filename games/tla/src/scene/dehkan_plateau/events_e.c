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
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

extern s32 gData_0200567c;
extern s32 gData_02005678;

void Func_02001ee8(void) {
  struct Actor *object;

  if (Flag_Test(0x201) != 0) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (gData_0200567c != (object->x >> 20) ||
        gData_02005678 != (object->z >> 20))
      GameFlag_ClearBit(0x201);
  }
}

void Func_02003cc0(s32 id);

void Func_02001f38(s32 id) {
  Func_02003cc0(id);
  Actor_SetPartAttribute(Actor_GetById(id), 15);
  Actor_SetSpritePriority(id, 1);
  Actor_GetById(id)->priorityFlags |= 2;
}

void Func_02001f68(void) {
  if (GameFlag_Test(0x109) != 0) {
    if (GameFlag_Test(0x200) == 0)
      Func_02001500();
  } else {
    GameFlag_SetBit(0x200);
  }
}

void Func_02001f94(void) {
  u32 i;

  for (i = 0; i <= 13; i++)
    Func_02001f38(i + 13);
  Func_02001f68();
}

void Func_02001fac(void) {
  u32 i;

  for (i = 0; i <= 12; i++)
    Func_02001f38(i + 25);
  Func_02001f68();
}

void Func_02001fc4(void) {
  u32 i;

  for (i = 0; i <= 9; i++)
    Func_02001f38(i + 26);
  Func_02001f68();
}

void Func_02001fdc(void) {
  u32 i;

  for (i = 0; i <= 2; i++)
    Func_02001f38(i + 8);
  Func_02001f68();
}

void Func_02001ff4(void) {
  u32 i;

  for (i = 0; i <= 5; i++)
    Func_02001f38(i + 10);
  Func_02001f68();
}

struct MapCells {
  u8 unk000[0x138];
  u8 *cells;
};

void Func_02000e9c(struct Actor *object);
s32 Func_02003db0(struct Actor *object);

void Func_0200200c(struct Actor *object) {
  s32 x;
  u8 *cell;

  Func_02000e9c(object);
  x = object->x >> 20;
  if (x == 12) {
    Func_02003c28();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(0, 32, 1, 1, x, 11);
    wait(1);
    Func_020015b8(object);
    Map_CopyCellAttributeRect(1, 32, 1, 1, x, 11);
    Func_02003c30();
  } else if (x == 18) {
    cell = ((struct MapCells *)Ram_HeapSlots->mapWork)->cells;
    cell += Func_02003db0(object) * 4;
    cell[3] &= 0x7f;
  } else if (x == 54) {
    cell = ((struct MapCells *)Ram_HeapSlots->mapWork)->cells;
    cell += Func_02003db0(object) * 4;
    cell[3] &= 0x7f;
    cell[2] = 11;
  } else if (x == 55) {
    Map_CopyAttributes(53, 20, 1, 1, 54, 20);
    Map_CopyCellAttributeRect(57, 20, 1, 1, 56, 20);
  }
}

s32 Func_02003de0(void);
s32 Func_02003d10(s32 owner);
void Func_02000d78(const void *data);
void Func_0200200c(struct Actor *object);
extern const u8 gData_02003fc8[];

void Func_020020dc(void) {
  struct Actor *object;

  if (Func_02003de0() != 0) {
    object = Actor_GetById(Func_02003d10(gMainRoutineWork.currentOwner));
    Func_02000d78(gData_02003fc8);
    Func_02003de8();
    Func_0200200c(object);
  }
}

void Func_02000d78(const void *data);
extern const u8 gData_02003fc8[];

void Func_02002114(void) { Func_02000d78(gData_02003fc8); }
