/// @file
/// Scene code of overlay 0x65f, in ROM order, from s0aa8's offset up to the next object the overlay links.

#include "garoh.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "types.h"
#include "io_reg.h"

extern char SceneId_Ov65FRoom1;
extern char SceneId_Ov65FRoom2;
extern char SceneId_Ov65FRoom3;
extern char SceneId_Ov65FRoom4;
extern char SceneId_Ov65FRoom5;
extern char SceneId_Ov660Room3;

void Func_02000a68(void);
void Func_02000be4(void);
void Func_02000d30(void);
void Func_02000e00(void);
void Func_020052f4(s32 value);
void Func_0200538c(void);

s32 Func_02000aa8(void) {
  struct EventWork *work;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom4) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom5) {
    FieldActor_SetSpritePriority(13, 1);
    FieldActor_SetSpritePriority(14, 1);
    if (Event_GetFlag(0x91f)) {
      /* FAKEMATCH(temporary): the angle in its own local; as a plain 0x4000
       * argument, 4 of 128 instructions differ in tla-en. */
      s32 angle = 0x4000;

      Actor_SetPositionAndFacing(10, PIXELS(328), PIXELS(162), angle);
    }
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
    Func_020052f4(0);
    if (gMainRoutineWork.entrance == 6) {
      work = Ram_HeapSlots->eventWork;
      work->screenEffect = 0x209;
      Event_ClearFlag(0x120);
      Event_ClearFlag(0x121);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
    }
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom1) {
      Event_SetFlag(0x919);
      Func_02000be4();
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom2) {
      Func_02000d30();
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom3) {
      Func_02000e00();
      if (Event_GetFlag(0x919)) {
        gMainRoutineWork.savedScene = (s32)&SceneId_Ov660Room3;
        gMainRoutineWork.savedEntrance = 2;
      }
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000be4(void) {
  FieldTask_AddFrame(Func_02000a68, 0x480);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  if (Event_GetFlag(0x91b) == 0 || Event_GetFlag(0x310) != 0) {
    Event_SetFlag(0x310);
    PaletteFade_ApplyTarget(0x10007, 1);
    if (Event_GetFlag(0x311))
      PaletteFade_ApplyTarget(0x10003, 2);
    PaletteFade_Interpolate(1);
    Event_WaitTransitionFrames();
    Func_0200538c();
    {
      u8 *palette = *(u8 **)((u8 *)Ram_HeapSlots + 0x80);

      *(volatile u16 *)&BG_PLTT_COLOR(1, 14) = 0x7fff;
      *(u16 *)(palette + 60) = *(volatile u16 *)&BG_PLTT_COLOR(1, 14);
    }
    PaletteFade_ApplyTarget(0x10000, 1);
  } else {
    MapGrid_CopyCellsTo(10, 32, 10, 10, 1, 1);
    MapGrid_CopyCellsTo(12, 32, 25, 22, 1, 1);
    MapGrid_CopyCellsTo(11, 33, 8, 22, 1, 1);
    MapGrid_CopyCellsTo(74, 32, 74, 10, 1, 1);
    MapGrid_CopyCellsTo(76, 32, 89, 22, 1, 1);
    MapGrid_CopyCellsTo(75, 33, 72, 22, 1, 1);
  }
  if (gMainRoutineWork.entrance == 12) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->screenEffect = 0x209;
    Event_ClearFlag(0x120);
    Event_ClearFlag(0x121);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000280();
void Func_02000390();
void Func_020003d8();
void Func_0200535c();
void Func_02005364();
void Func_020002a4();

void Func_02000d30(void) {
  s32 t4;

  Task_AddFrame(Func_02000a68, 0x480);
  Func_0200535c();
  Func_02005364(0, 10, 11);
  t4 = Event_GetFlag(0x310);
  if (t4 == 0)
    goto L_02008da6;
  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 12)
    goto L_02008d72;
  PaletteFade_ApplyTarget(0x2048c9, 1);
  goto L_02008d7e;
L_02008d72:
  PaletteFade_ApplyTarget(0x10007, 1);
L_02008d7e:
  PaletteFade_Interpolate(1);
  Event_WaitTransitionFrames();
  Func_0200538c();
  Func_020003d8();
  PaletteFade_ApplyTarget(0x10000, 1);
  Task_AddFrame((s32)Func_020002a4, 0xc80);
  goto L_02008dea;
L_02008da6:
  MapGrid_CopyCellsTo(20, 7, 9, 7, 5, 5);
  MapGrid_CopyCellsTo(20, 71, 9, 71, 5, 5);
  MapGrid_CopyCellsTo(84, 7, 73, 7, 5, 5);
  Func_02000390();
  PaletteFade_ApplyTarget(0x10000, 1);
  Func_02000280();
L_02008dea:
}

extern u8 gData_02005450[];

void Func_02000e00(void) {
  FieldTask_AddFrame(Func_02000a68, 1152);
  if (Flag_IsSet(0x310)) {
    if (Flag_IsSet(0x318))
      PaletteFade_ApplyTarget(0x10003, 1);
    else
      PaletteFade_ApplyTarget(0x10007, 1);
  }
  Func_0200536c(gData_02005450);
  PaletteFade_Interpolate(1);
  Event_WaitTransitionFrames();
}

void Func_02000e58(void) { Func_02005374(gData_02005450); }

void Func_02000e68(void) { Func_02005374(gData_02005450); }

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

  raw = object->z;
  z = raw >> 20;
  if (!Flag_IsSet(0x201) && x == 10 && z == 29) {
    Func_02000e78(8, object);
    Flag_Set(0x201);
  }
}

void Func_02000eec(void) {
  struct Actor *object = Actor_Get(9);
  s32 x = object->x >> 20;
  s32 raw;
  s32 z;

  raw = object->z;
  z = raw >> 20;
  if (!Flag_IsSet(0x202) && x == 15 && z == 28) {
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
