#include "garoh.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern struct MainRoutineWork gMainRoutineWork;

struct GarohEventWork {
  u8 unk000[0xca4];
  s8 fadeLock;
};

void Func_02000e78(s32 id, struct Actor *object) {
  s32 i;
  s32 saved;

  object->motionFlags = 0;
  saved = id;
  for (i = 16; i > 0; i--) {
    object->y -= PIXELS(2);
    FieldEvent_Wait(1);
  }
  Event_PlaySound(136);
  Func_020012a8(saved);
}

void Func_02000eb0(void) {
  struct Actor *object = Actor_Get(8);
  s32 x = object->x >> 20;
  s32 raw;
  s32 z;
  s32 flag = 129;

  raw = object->z;
  /* FAKEMATCH: the native starts the flag id 0x201, loads z and then finishes
   * the id; without the barrier and the id built in two steps the id is shifted
   * before the load (one reordered pair, differ 2). */
  __asm__ volatile("" : "+r"(flag));
  z = raw >> 20;
  flag <<= 1;
  flag += 255;
  if (!Flag_IsSet(flag) && x == 10 && z == 29) {
    Func_02000e78(8, object);
    Flag_Set(0x201);
  }
}

void Func_02000eec(void) {
  struct Actor *object = Actor_Get(9);
  s32 x = object->x >> 20;
  s32 raw;
  s32 z;
  s32 flag = 128;

  raw = object->z;
  /* FAKEMATCH: the native starts the flag id 0x202, loads z and then finishes
   * the id; without the barrier and the id built in two steps the id is shifted
   * before the load (one reordered pair, differ 2). */
  __asm__ volatile("" : "+r"(flag));
  z = raw >> 20;
  flag <<= 2;
  flag += 2;
  if (!Flag_IsSet(flag) && x == 15 && z == 28) {
    Func_02000e78(9, object);
    Flag_Set(0x202);
  }
}

void Func_02000f28(void) {
  Actor_Get(10);
  Flag_IsSet(0x203);
}

void Func_02000f3c(void) {
  struct Actor *object = Actor_Get(11);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (!Flag_IsSet(0x204) && x == 15 && z == 25) {
    Func_02000e78(11, object);
    Flag_Set(0x204);
  }
}

void Func_02000f74(void) {
  Func_0200537c();
  Func_02000eb0();
}

void Func_02000f80(void) {
  Func_0200537c();
  Func_02000eec();
}

void Func_02000f8c(void) {
  Func_0200537c();
  Func_02000f28();
}

void Func_02000f98(void) {
  Func_0200537c();
  Func_02000f3c();
}

void Func_02000fa4(void) {
  Func_0200537c();
  Func_02000eb0();
  Func_02000eec();
  Func_02000f28();
  Func_02000f3c();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void CopyRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
  Map_CopyCellPictureIndicesRect(a0, a1, a2, a3, a4, a5);
}

static inline void BlendRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
  Map_CopyCellAttributeRect(a0, a1, a2, a3, a4, a5);
}

void Func_02000fbc(void) {
  CopyRect(98, 8, 86, 8, 1, 2);
  BlendRect(34, 9, 1, 1, 22, 11);
}

void Func_02000fec(void) {
  CopyRect(25, 47, 11, 33, 1, 3);
  BlendRect(25, 47, 1, 1, 11, 33);
}

void Func_0200101c(void) {
  MapGrid_CopyCellsTo(44, 5, 19, 5, 3, 3);
  MapGrid_CopyCellsTo(108, 5, 83, 5, 3, 2);
  BlendRect(44, 5, 3, 3, 19, 5);
}

void Func_0200105c(void) {
  CopyRect(102, 8, 86, 8, 1, 2);
  BlendRect(22, 10, 1, 1, 22, 11);
}

s32 Func_0200108c(void) { return Flag_IsSet(0x310); }

void Func_02001098(void) {
  if (Flag_IsSet(0x310)) {
    if (Flag_IsSet(0x318))
      PaletteFade_ApplyTarget(0x10003, 1);
    else
      PaletteFade_ApplyTarget(0x10007, 1);
    PaletteFade_Interpolate(16);
  }
}

void Func_020010d4(void) {
  CopyRect(8, 30, 11, 33, 1, 3);
  BlendRect(8, 30, 1, 1, 11, 33);
}

void Func_02001104(void) {
  MapGrid_CopyCellsTo(39, 5, 19, 5, 3, 3);
  MapGrid_CopyCellsTo(103, 5, 83, 5, 3, 2);
  BlendRect(39, 5, 3, 3, 19, 5);
}

void Func_02001144(void) {
  struct GarohEventWork *work;

  if (!Flag_IsSet(0x91b)) {
    Flag_Set(0x210);
    if (gMainRoutineWork.entrance == 1) {
      PaletteFade_ApplyTarget(0x10003, 1);
      Flag_Set(0x310);
      Flag_Set(0x318);
    } else {
      PaletteFade_ApplyTarget(0x10000, 1);
      Flag_Clear(0x310);
      Flag_Clear(0x318);
    }
    work = Ram_HeapSlots->eventWork;
    if (work->fadeLock == 0)
      PaletteFade_Interpolate(120);
  }
}

void Func_020011c8(void) {
  struct GarohEventWork *work;

  if (!Flag_IsSet(0x91b)) {
    Flag_Set(0x211);
    if (gMainRoutineWork.entrance == 2) {
      PaletteFade_ApplyTarget(0x10003, 1);
      Flag_Set(0x310);
      Flag_Set(0x318);
    } else {
      PaletteFade_ApplyTarget(0x10007, 1);
      Flag_Set(0x310);
      Flag_Clear(0x318);
    }
    work = Ram_HeapSlots->eventWork;
    if (work->fadeLock == 0)
      PaletteFade_Interpolate(120);
  }
}
