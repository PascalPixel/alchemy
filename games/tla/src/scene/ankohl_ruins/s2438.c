#include "version.h"
#include "event_inline.h"

#if TLA_PATCH_1

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
};

extern struct AnkohlRuinsFloatWork Data_02004030;
void Func_020010f4(void);
void Func_020023d0(void);
void Func_02002ed4();
void Func_02000418(void);
void Func_02002e44(void);
void Event_StartScreenTransition(void);
void Func_02001420(s32 a0);
void Func_02002dec(void);

s32 Func_02002438(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov686Room1) {
    Call1((void (*)())Func_02002ed4, 0);
    if (GameFlag_Test(0x9d2))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Test(0x9d3))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Test(0x9d4))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Test(0x9d5))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Test(0x9d6))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Test(0x9d7))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Test(0x9d8))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Test(0x990))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Test(0x991))
      Actor_SetPosition(18, 0, 0);
    if (GameFlag_Test(0x992))
      Actor_SetPosition(19, 0, 0);
    if (GameFlag_Test(0x993))
      Actor_SetPosition(20, 0, 0);
    if (GameFlag_Test(0x994))
      Actor_SetPosition(21, 0, 0);
    if (GameFlag_Test(0x995))
      Actor_SetPosition(22, 0, 0);
    if (GameFlag_Test(0x996))
      Actor_SetPosition(23, 0, 0);
    if (GameFlag_Test(0x997))
      Actor_SetPosition(24, 0, 0);
    if (GameFlag_Test(0x998))
      Actor_SetPosition(25, 0, 0);
    if (GameFlag_Test(0x999))
      Actor_SetPosition(26, 0, 0);
    if (GameFlag_Test(0x99a))
      Actor_SetPosition(27, 0, 0);
    if (GameFlag_Test(0x99b))
      Actor_SetPosition(28, 0, 0);
    if (GameFlag_Test(0x99c))
      Actor_SetPosition(29, 0, 0);
    if (GameFlag_Test(0x99d))
      Actor_SetPosition(30, 0, 0);
    if (GameFlag_Test(0x9c6)) {
      Actor_SetPosition(8, 0x480000, 0x580000);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_Ov686Room2) {
    if (GameFlag_Test(0x99e))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Test(0x99f))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Test(0x9a0))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Test(0x9dc)) {
      s32 width;

      object = Actor_GetById(8);
      object->motionFlags = 3;
      Actor_SetPosition(8, 0xc80000, 0x580000);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        Event_PlaySound(0x23f);
    }
  } else if (scene == (s32)&SceneId_Ov686Room3) {
    if (GameFlag_Test(0x9c7)) {
      Actor_SetPosition(8, 0x2080000, 0x680000);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Test(0x9dd)) {
      Actor_SetPosition(10, 0, 0);
      Actor_SetAnimation(9, 4);
    }
    if (GameFlag_Test(0x201)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_Ov686Room4) {
    if (GameFlag_Test(0x9a1))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Test(0x9a2))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Test(0x9a3))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Test(0x9a4))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Test(0x9a5))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Test(0x9a6))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Test(0x9a7))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Test(0x9c8))
      Actor_SetPosition(8, 0x280000, 0x3080000);
    if (GameFlag_Test(0x9c9))
      Actor_SetPosition(18, 0x2780000, 0x1c80000);
    if (GameFlag_Test(0x9ca)) {
      Actor_SetPosition(19, 0x2680000, 0x880000);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Test(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      Actor_SetPosition(9, 0xc80000, 0x2d80000);
    }
    if (GameFlag_Test(0x9da)) {
      object = Actor_GetById(10);
      Actor_SetPosition(10, 0xe80000, 0x2d80000);
      object->motionFlags = 3;
    } else if (GameFlag_Test(0x9db) && !GameFlag_Test(0x109)) {
      Actor_SetPosition(10, 0x1480000, 0x2f80000);
    }
    if (GameFlag_Test(0x9da) && GameFlag_Test(0x9d9)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 4)
        Event_PlaySound(0x23f);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 17) {
      Func_02000418();
#if TLA_PATCH_4
      Event_ClearFlag(0x122);
#endif
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance == 20) {
        s32 frames;

        Func_02002e44();
        FieldEvent_ResolvePendingActions(0);
        FieldCamera_MoveTo(-1, -1, -1, 0);
        Actor_SetPosition(late->currentOwner, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        object = Actor_GetById(10);
        Actor_SetPosition(10, 0x1480000, 0x2f80000);
        object->y += 0x800000;
        frames = 60;
        object->motionFlags = 3;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (object->velocityY > -1)
            break;
        }
        Event_PlaySound(188);
        FieldEvent_Wait(60);
        Event_SetFlag(0x109);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_Ov686Room5) {
    if (GameFlag_Test(0x9db))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Test(0x9cb)) {
      Actor_SetPosition(8, 0x1180000, 0x1380000);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      Event_PlaySound(0x23f);
    task:
      Task_AddFrame(Func_020010f4, 0x480);
    }
  } else if (scene == (s32)&SceneId_Ov686Room6) {
    s16 *entrance = &work->entrance;

    if (work->entrance <= 4)
      Func_02001420(a0);
    if (GameFlag_Test(0x9e2)) {
      s32 x = 14;

      MapGrid_CopyCells(26, 67, 1, 3, x, 65);
      MapGrid_CopyCellAttributes(15, 5, 1, 1, x, 5);
    }
    if (GameFlag_Test(0x9e3)) {
      s32 x = 2;

      MapGrid_CopyCells(26, 67, 1, 3, x, 83);
      MapGrid_CopyCellAttributes(1, 23, 1, 1, x, 23);
    }
    if (GameFlag_Test(0x109) && GameFlag_Test(0x204)) {
      s32 level = 0x200000;

      Data_02004030.level = level;
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsFloatWork *)entrance)->unk00 - 1) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 5;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 69);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 7);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 71);
        }
        MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, z);
      }
      if ((u16)(*entrance - 3) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 23;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 87);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 25);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 89);
        }
        MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, z);
      }
      object = Actor_GetById(work->currentOwner);
      object->terrainHeight = level;
      object->y = level;
      FieldEvent_Wait(1);
      Func_02002dec();
    }
  } else if (scene == (s32)&SceneId_Ov686Room8) {
    object = Actor_GetById(64);
    if (object != NULL) {
      object->motionFlags = 0;
      object->terrainHeight = 0x480000;
      object->y = 0x480000;
      object->update = Func_020023d0;
    }
  }
  return 0;
}

#else

// The Japanese release's older form.

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
};

extern struct AnkohlRuinsFloatWork Data_02004030;
void Func_020010f4(void);
void Func_020023d0(void);
void Func_02002ed4();
void Func_02002e44(void);
void Event_StartScreenTransition(void);
void Func_02001420(s32 a0);
void Func_02002dec(void);

s32 Func_02002438(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov686Room1) {
    Call1((void (*)())Func_02002ed4, 0);
    if (GameFlag_Test(0x9d2))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Test(0x9d3))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Test(0x9d4))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Test(0x9d5))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Test(0x9d6))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Test(0x9d7))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Test(0x9d8))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Test(0x990))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Test(0x991))
      Actor_SetPosition(18, 0, 0);
    if (GameFlag_Test(0x992))
      Actor_SetPosition(19, 0, 0);
    if (GameFlag_Test(0x993))
      Actor_SetPosition(20, 0, 0);
    if (GameFlag_Test(0x994))
      Actor_SetPosition(21, 0, 0);
    if (GameFlag_Test(0x995))
      Actor_SetPosition(22, 0, 0);
    if (GameFlag_Test(0x996))
      Actor_SetPosition(23, 0, 0);
    if (GameFlag_Test(0x997))
      Actor_SetPosition(24, 0, 0);
    if (GameFlag_Test(0x998))
      Actor_SetPosition(25, 0, 0);
    if (GameFlag_Test(0x999))
      Actor_SetPosition(26, 0, 0);
    if (GameFlag_Test(0x99a))
      Actor_SetPosition(27, 0, 0);
    if (GameFlag_Test(0x99b))
      Actor_SetPosition(28, 0, 0);
    if (GameFlag_Test(0x99c))
      Actor_SetPosition(29, 0, 0);
    if (GameFlag_Test(0x99d))
      Actor_SetPosition(30, 0, 0);
    if (GameFlag_Test(0x9c6)) {
      Actor_SetPosition(8, 0x480000, 0x580000);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_Ov686Room2) {
    if (GameFlag_Test(0x99e))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Test(0x99f))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Test(0x9a0))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Test(0x9dc)) {
      s32 width;

      object = Actor_GetById(8);
      object->motionFlags = 3;
      Actor_SetPosition(8, 0xc80000, 0x580000);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        Event_PlaySound(0x23f);
    }
  } else if (scene == (s32)&SceneId_Ov686Room3) {
    if (GameFlag_Test(0x9c7)) {
      Actor_SetPosition(8, 0x2080000, 0x680000);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Test(0x9dd)) {
      Actor_SetPosition(10, 0, 0);
      Actor_SetAnimation(9, 4);
    }
    if (GameFlag_Test(0x201)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_Ov686Room4) {
    if (GameFlag_Test(0x9a1))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Test(0x9a2))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Test(0x9a3))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Test(0x9a4))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Test(0x9a5))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Test(0x9a6))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Test(0x9a7))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Test(0x9c8))
      Actor_SetPosition(8, 0x280000, 0x3080000);
    if (GameFlag_Test(0x9c9))
      Actor_SetPosition(18, 0x2780000, 0x1c80000);
    if (GameFlag_Test(0x9ca)) {
      Actor_SetPosition(19, 0x2680000, 0x880000);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Test(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      Actor_SetPosition(9, 0xc80000, 0x2d80000);
    }
    if (GameFlag_Test(0x9da)) {
      object = Actor_GetById(10);
      Actor_SetPosition(10, 0xe80000, 0x2d80000);
      object->motionFlags = 3;
    } else if (GameFlag_Test(0x9db) && !GameFlag_Test(0x109)) {
      Actor_SetPosition(10, 0x1480000, 0x2f80000);
    }
    if (GameFlag_Test(0x9da) && GameFlag_Test(0x9d9)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
        Event_PlaySound(0x23f);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance == 20) {
        s32 frames;

        Func_02002e44();
        FieldEvent_ResolvePendingActions(0);
        FieldCamera_MoveTo(-1, -1, -1, 0);
        Actor_SetPosition(late->currentOwner, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        object = Actor_GetById(10);
        Actor_SetPosition(10, 0x1480000, 0x2f80000);
        object->y += 0x800000;
        frames = 60;
        object->motionFlags = 3;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (object->velocityY > -1)
            break;
        }
        Event_PlaySound(188);
        FieldEvent_Wait(60);
        Event_SetFlag(0x109);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_Ov686Room5) {
    if (GameFlag_Test(0x9db))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Test(0x9cb)) {
      Actor_SetPosition(8, 0x1180000, 0x1380000);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      Event_PlaySound(0x23f);
    task:
      Task_AddFrame(Func_020010f4, 0x480);
    }
  } else if (scene == (s32)&SceneId_Ov686Room6) {
    s16 *entrance = &work->entrance;

    if (work->entrance <= 4)
      Func_02001420(a0);
    if (GameFlag_Test(0x9e2)) {
      s32 x = 14;

      MapGrid_CopyCells(26, 67, 1, 3, x, 65);
      MapGrid_CopyCellAttributes(15, 5, 1, 1, x, 5);
    }
    if (GameFlag_Test(0x9e3)) {
      s32 x = 2;

      MapGrid_CopyCells(26, 67, 1, 3, x, 83);
      MapGrid_CopyCellAttributes(1, 23, 1, 1, x, 23);
    }
    if (GameFlag_Test(0x109) && GameFlag_Test(0x204)) {
      s32 level = 0x200000;

      Data_02004030.level = level;
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsFloatWork *)entrance)->unk00 - 1) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 5;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 69);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 7);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 71);
        }
        MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, z);
      }
      if ((u16)(*entrance - 3) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 23;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 87);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 25);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 89);
        }
        MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, z);
      }
      object = Actor_GetById(work->currentOwner);
      object->terrainHeight = level;
      object->y = level;
      FieldEvent_Wait(1);
      Func_02002dec();
    }
  } else if (scene == (s32)&SceneId_Ov686Room8) {
    object = Actor_GetById(64);
    if (object != NULL) {
      object->motionFlags = 0;
      object->terrainHeight = 0x480000;
      object->y = 0x480000;
      object->update = Func_020023d0;
    }
  }
  return 0;
}

#endif

#include "types.h"
#include "scene_tables.h"

s32 Scene_PrepareMap(void) { return 0; }
