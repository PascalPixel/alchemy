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

s32 Scene_Initialize(s32 a0) {
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

s32 Scene_Initialize(s32 a0) {
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

void Func_02000384(void);
void Func_020005c0(void);
void Func_02000698(void);
void Func_020006b4(void);
void Func_020006d0(void);
void Func_02000850(void);
void Func_02000a68(void);
void Func_02000c24(void);
void Func_02000e60(void);
void Func_0200104c(void);
void Func_020011c0(void);
void Func_0200124c(void);
void Func_02001264(void);
void Func_0200160c(void);
void Func_020019a0(void);
void Func_02001d34(void);
void Func_02001ddc(void);
void Func_02001e54(void);
void Func_02001f04(void);
void Func_020020fc(void);
void Func_020021d4(void);
void Func_02002260(void);
void Func_020022a8(void);
void Func_020022e0(void);
void Func_02002318(void);
void Func_02002328(void);

extern u32 gData_02002ef4[];
extern u32 gData_02002f30[];
extern u32 gData_02002f6c[];
extern s32 gData_02003268[];
extern u32 gData_02003280[];
extern u32 gData_02003290[];

// clang-format off
u32 gData_02002ef4[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002f30[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002f6c[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002fa8[] = {
    0x1040f00, 8, 0x4030300, 5, 0x7020600, 6,
    0xa010100, 5, 0xc010100, 5, 0xb010102, 5,
    0x1010f02, 12, 0x1010f04, 13, 99, 0,
};

u32 gData_02002ff0[] = {
    0x1020f00, 28, 0x4020c00, 26, 0x4020800, 24,
    0x5010200, 23, 0xa010200, 23, 0xf010100, 25,
    0x4010102, 23, 0x1010f02, 30, 0x1010f04, 31,
    99, 0,
};

s32 gData_02003040[] = {
    (s32)gData_02002ef4,
    (s32)gData_02002f30,
    (s32)gData_02002f6c,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 136, 0, 120, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_0200307c[] = {
    0xee0333, 0x33b0354, 0x35c00f6, 0xcffff, 0xffff, 0,
    0, 0,
};

u32 gData_0200309c[] = {
    0xffe600b4, 0xbc00a4, 0xacffee, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(195),
    SCENE_EXIT(1, 2, 41),
    SCENE_EXIT(2, 189, 1),
    SCENE_EXITS(189),
    SCENE_EXIT(1, 195, 2),
    SCENE_EXIT(2, 189, 11),
    SCENE_EXIT(3, 189, 5),
    SCENE_EXIT(4, 189, 6),
    SCENE_EXIT(5, 189, 3),
    SCENE_EXIT(6, 189, 4),
    SCENE_EXIT(7, 190, 1),
    SCENE_EXIT(8, 189, 12),
    SCENE_EXIT(9, 189, 10),
    SCENE_EXIT(10, 189, 9),
    SCENE_EXIT(11, 189, 2),
    SCENE_EXIT(12, 189, 8),
    SCENE_EXIT(13, 189, 20),
    SCENE_EXIT(14, 189, 21),
    SCENE_EXIT(15, 189, 22),
    SCENE_EXIT(16, 194, 23),
    SCENE_EXIT(17, 189, 24),
    SCENE_EXIT(18, 189, 25),
    SCENE_EXIT(19, 193, 26),
    SCENE_EXIT(20, 189, 13),
    SCENE_EXIT(21, 189, 14),
    SCENE_EXIT(22, 189, 15),
    SCENE_EXIT(24, 189, 17),
    SCENE_EXIT(25, 189, 18),
    SCENE_EXIT(27, 189, 30),
    SCENE_EXIT(29, 189, 31),
    SCENE_EXIT(30, 189, 27),
    SCENE_EXIT(31, 189, 29),
    SCENE_EXIT(32, 190, 3),
    SCENE_EXIT(33, 190, 2),
    SCENE_EXITS(190),
    SCENE_EXIT(1, 189, 7),
    SCENE_EXIT(2, 189, 33),
    SCENE_EXIT(3, 189, 32),
    SCENE_EXIT(4, 194, 28),
    SCENE_EXIT(6, 191, 9),
    SCENE_EXIT(7, 191, 4),
    SCENE_EXIT(10, 191, 3),
    SCENE_EXIT(11, 190, 12),
    SCENE_EXIT(12, 190, 11),
    SCENE_EXIT(14, 190, 15),
    SCENE_EXIT(15, 190, 14),
    SCENE_EXIT(16, 190, 17),
    SCENE_EXIT(17, 190, 16),
    SCENE_EXIT(18, 192, 13),
    SCENE_EXIT(19, 192, 10),
    SCENE_EXITS(191),
    SCENE_EXIT(1, 192, 1),
    SCENE_EXIT(2, 192, 9),
    SCENE_EXIT(3, 190, 10),
    SCENE_EXIT(4, 190, 7),
    SCENE_EXIT(5, 191, 8),
    SCENE_EXIT(6, 191, 10),
    SCENE_EXIT(7, 194, 1),
    SCENE_EXIT(8, 191, 5),
    SCENE_EXIT(9, 190, 6),
    SCENE_EXIT(10, 191, 6),
    SCENE_EXIT(11, 192, 5),
    SCENE_EXIT(12, 192, 6),
    SCENE_EXITS(192),
    SCENE_EXIT(1, 191, 1),
    SCENE_EXIT(2, 193, 8),
    SCENE_EXIT(3, 192, 14),
    SCENE_EXIT(4, 193, 7),
    SCENE_EXIT(5, 191, 11),
    SCENE_EXIT(6, 191, 12),
    SCENE_EXIT(7, 194, 4),
    SCENE_EXIT(8, 192, 11),
    SCENE_EXIT(9, 191, 2),
    SCENE_EXIT(10, 190, 19),
    SCENE_EXIT(11, 192, 8),
    SCENE_EXIT(12, 194, 3),
    SCENE_EXIT(13, 190, 18),
    SCENE_EXIT(14, 192, 3),
    SCENE_EXIT(15, 193, 4),
    SCENE_EXIT(16, 193, 3),
    SCENE_EXIT(20, 193, 4),
    SCENE_EXITS(193),
    SCENE_EXIT(1, 193, 2),
    SCENE_EXIT(2, 193, 1),
    SCENE_EXIT(3, 192, 16),
    SCENE_EXIT(4, 192, 15),
    SCENE_EXIT(5, 193, 6),
    SCENE_EXIT(6, 193, 5),
    SCENE_EXIT(7, 192, 4),
    SCENE_EXIT(8, 192, 2),
    SCENE_EXIT(9, 194, 2),
    SCENE_EXIT(10, 193, 11),
    SCENE_EXIT(11, 193, 10),
    SCENE_EXIT(12, 196, 1),
    SCENE_EXIT(13, 192, 17),
    SCENE_EXIT(20, 192, 20),
    SCENE_EXIT(26, 189, 19),
    SCENE_EXITS(194),
    SCENE_EXIT(1, 191, 7),
    SCENE_EXIT(2, 193, 9),
    SCENE_EXIT(3, 192, 12),
    SCENE_EXIT(4, 192, 7),
    SCENE_EXIT(23, 189, 16),
    SCENE_EXIT(28, 190, 4),
    SCENE_EXITS(196),
    SCENE_EXIT(1, 193, 12),
    SCENE_EXITS_END,
};

s32 gData_02003268[] = {
    22,
    11,
    0x18000,
    46,
    (s32)Func_020005c0,
    17,
};

u32 gData_02003280[] = {
    22, 26, 0, 17,
};

u32 gData_02003290[] = {
    22, 30, 0, 17,
};

u32 gData_020032a0[] = {
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_020032b8[] = {
    (s32)0xffff0110,
    1,
    0x380000,
    0,
    0x580000,
    0x24000,
    (s32)0xffff01e9,
    1,
    0x80000,
    0,
    0x80000,
    0x24000,
    (s32)0xffff0188,
    1,
    0x380000,
    0,
    0x1e80000,
    0x24000,
    (s32)0xffff0188,
    1,
    0x780000,
    0,
    0x1e80000,
    0x24000,
    (s32)0xffff0188,
    1,
    0xb80000,
    0,
    0x1e80000,
    0x24000,
    (s32)0xffff0188,
    1,
    0,
    0,
    0,
    0x24000,
    (s32)0xffff0188,
    1,
    0x1380000,
    0,
    0x1e80000,
    0x24000,
    (s32)0xffff0188,
    1,
    0x1780000,
    0,
    0x1e80000,
    0x24000,
    (s32)0xffff0188,
    1,
    0x1b80000,
    0,
    0x1e80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x1c80000,
    0,
    0x3480000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2b80000,
    0,
    0x3480000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2180000,
    0,
    0x2d80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2380000,
    0,
    0x2d80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2580000,
    0,
    0x2d80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2880000,
    0,
    0xf80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2480000,
    0,
    0x380000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0xa80000,
    0,
    0x3980000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x1080000,
    0,
    0x3980000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x1280000,
    0,
    0x2e80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0xb80000,
    0,
    0x2e80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x3480000,
    0,
    0x1a80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x3a80000,
    0,
    0x1a80000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x3580000,
    0,
    0x2680000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_020034f8[] = {
    (s32)0xffff0187,
    (s32)gData_02003280,
    0x780000,
    0,
    0x580000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x1d80000,
    0,
    0x3080000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2080000,
    0,
    0x3080000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2580000,
    0,
    0x3080000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02003570[] = {
    (s32)0xffff0110,
    1,
    0x2180000,
    0,
    0x680000,
    0x24000,
    (s32)0xffff0186,
    (s32)gData_02003290,
    0xe80000,
    0,
    0x1880000,
    0x24000,
    (s32)0xffff0189,
    (s32)gData_02003280,
    0x1280000,
    0,
    0x1880000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_020035d0[] = {
    (s32)0xffff0110,
    1,
    0x380000,
    0,
    0x3080000,
    0x24000,
    (s32)0xffff0187,
    (s32)gData_02003280,
    0x680000,
    0,
    0x2f80000,
    0x24000,
    (s32)0xffff0189,
    (s32)gData_02003280,
    0,
    0,
    0,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x1a80000,
    0,
    0x780000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x1f80000,
    0,
    0x780000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2c80000,
    0,
    0x780000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x3180000,
    0,
    0x780000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2880000,
    0,
    0x3980000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x2d80000,
    0,
    0x3980000,
    0x24000,
    (s32)0xffff0133,
    (s32)gData_02003268,
    0x3280000,
    0,
    0x3980000,
    0x24000,
    (s32)0xffff0110,
    1,
    0x2880000,
    0,
    0x1c80000,
    0x24000,
    (s32)0xffff0110,
    1,
    0x2580000,
    0,
    0x880000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02003708[] = {
    (s32)0xffff0110,
    1,
    0x1280000,
    0,
    0x1380000,
    0x24000,
    (s32)0xffff0189,
    (s32)gData_02003280,
    0x1b80000,
    0,
    0x2480000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_02003750[] = {
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02003768[] = {
    0xffff, 0, 0, 0, 0, 0,
};

u16 gData_02003780[] = {
    10,
};

u16 gData_02003782[] = {
    0,
};

u32 gData_02003784[] = {
    0,
};

u32 gData_02003788[] = {
    0x8000,
};

u32 gData_0200378c[] = {
    0,
};

s32 gData_02003790[] = {
    1, (s32)0xffff0001, 1, 1, (s32)0xffff0002, 2,
    -1, 0, 0,
};
// clang-format on

s32 gData_020037b4[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    33,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    1,
    (s32)0xffff000d,
    13,
    1,
    (s32)0xffff000e,
    14,
    1,
    (s32)0xffff000f,
    15,
    1,
    (s32)0xffff0010,
    16,
    1,
    (s32)0xffff0011,
    17,
    1,
    (s32)0xffff0012,
    18,
    1,
    (s32)0xffff0013,
    19,
    1,
    (s32)0xffff0014,
    20,
    1,
    (s32)0xffff0015,
    21,
    1,
    (s32)0xffff0016,
    22,
    1,
    (s32)0xffff0017,
    23,
    1,
    (s32)0xffff0018,
    24,
    1,
    (s32)0xffff0019,
    25,
    1,
    (s32)0xffff001a,
    26,
    1,
    (s32)0xffff001b,
    27,
    33,
    (s32)0xffff001c,
    28,
    1,
    (s32)0xffff001d,
    29,
    1,
    (s32)0xffff001e,
    30,
    1,
    (s32)0xffff001f,
    31,
    33,
    (s32)0xffff0020,
    32,
    33,
    (s32)0xffff0021,
    33,
    0x8c15,
    0x9c60008,
    (s32)Func_02000850,
    0x10008c15,
    0x9c60008,
    (s32)Func_020006d0,
    0x8c15,
    0x9d2000a,
    (s32)Func_02000a68,
    0x8c15,
    0x9d3000b,
    (s32)Func_02000a68,
    0x8c15,
    0x9d4000c,
    (s32)Func_02000a68,
    0x8c15,
    0x9d5000d,
    (s32)Func_02000a68,
    0x8c15,
    0x9d6000e,
    (s32)Func_02000a68,
    0x8c15,
    0x9d7000f,
    (s32)Func_02000a68,
    0x8c15,
    0x9d80010,
    (s32)Func_02000a68,
    0x4e15,
    (s32)0xffff0011,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0012,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0013,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0014,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0015,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0016,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0017,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0018,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0019,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff001a,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff001b,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff001c,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff001d,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff001e,
    (s32)Func_02001264,
    0x8b85,
    -0x10000,
    (s32)Func_02000698,
    0x40008b85,
    -0x10000,
    (s32)Func_020006b4,
    -1,
    0,
    0,
};

s32 gData_02003a78[] = {
    49,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    49,
    (s32)0xffff0003,
    3,
    49,
    (s32)0xffff0004,
    4,
    33,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    33,
    (s32)0xffff000d,
    13,
    1,
    (s32)0xffff000e,
    14,
    1,
    (s32)0xffff000f,
    15,
    1,
    (s32)0xffff0010,
    16,
    1,
    (s32)0xffff0011,
    17,
    1,
    (s32)0xffff0012,
    18,
    33,
    (s32)0xffff0013,
    19,
    0x8c15,
    0x9dc0008,
    (s32)Func_02000e60,
    9,
    0x9dc0008,
    (s32)Func_02000e60,
    0x4e15,
    (s32)0xffff0009,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff000a,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff000b,
    (s32)Func_02001264,
    0x8b85,
    -0x10000,
    (s32)Func_02000698,
    0x40008b85,
    -0x10000,
    (s32)Func_020006b4,
    -1,
    0,
    0,
};

s32 gData_02003b98[] = {
    33,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    49,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    2,
    (s32)0xffff0028,
    (s32)Func_020020fc,
    2,
    (s32)0xffff0029,
    (s32)Func_020021d4,
    0x10008c15,
    0x9c70008,
    (s32)Func_020006d0,
    0x8c15,
    0x9c70008,
    (s32)Func_02000850,
    0x8c15,
    0x9dd000a,
    (s32)Func_0200104c,
    9,
    0x9dd000a,
    (s32)Func_0200104c,
    0x8b85,
    -0x10000,
    (s32)Func_02000698,
    0x40008b85,
    -0x10000,
    (s32)Func_020006b4,
    -1,
    0,
    0,
};

s32 gData_02003c94[] = {
    49,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    33,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    49,
    (s32)0xffff0009,
    9,
    49,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    1,
    (s32)0xffff000d,
    13,
    1,
    (s32)0xffff000e,
    14,
    33,
    (s32)0xffff000f,
    15,
    33,
    (s32)0xffff0010,
    16,
    0x4e15,
    (s32)0xffff000b,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff000c,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff000d,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff000e,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff000f,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0010,
    (s32)Func_02001264,
    0x4e15,
    (s32)0xffff0011,
    (s32)Func_02001264,
    9,
    -0x10000,
    (s32)Func_0200124c,
    0x8c15,
    (s32)0xffff0009,
    (s32)Func_02000c24,
    0x8c15,
    (s32)0xffff000a,
    (s32)Func_02000c24,
    0x10008c15,
    0x9c80008,
    (s32)Func_020006d0,
    0x8c15,
    0x9c80008,
    (s32)Func_02000850,
    0x10008c15,
    0x9c90012,
    (s32)Func_020006d0,
    0x8c15,
    0x9c90012,
    (s32)Func_02000850,
    0x10008c15,
    0x9ca0013,
    (s32)Func_020006d0,
    0x8c15,
    0x9ca0013,
    (s32)Func_02000850,
    0x8b85,
    -0x10000,
    (s32)Func_02000698,
    0x40008b85,
    -0x10000,
    (s32)Func_020006b4,
    -1,
    0,
    0,
};

s32 gData_02003e38[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    49,
    (s32)0xffff0003,
    3,
    49,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    49,
    (s32)0xffff0007,
    7,
    49,
    (s32)0xffff0008,
    8,
    49,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    0x4602,
    (s32)0xffff000d,
    (s32)Func_02000384,
    1,
    (s32)0xffff001a,
    26,
    2,
    (s32)0xffff0042,
    (s32)Func_020011c0,
    0x10008c15,
    0x9cb0008,
    (s32)Func_020006d0,
    0x8c15,
    0x9cb0008,
    (s32)Func_02000850,
    0x10008c15,
    0x9db0009,
    (s32)Func_020006d0,
    0x8c15,
    0x9db0009,
    (s32)Func_02002260,
    0x50009085,
    -0x10000,
    (s32)Func_020022a8,
    0x40009085,
    -0x10000,
    (s32)Func_020022e0,
    0x8b85,
    -0x10000,
    (s32)Func_02000698,
    0x40008b85,
    -0x10000,
    (s32)Func_020006b4,
    -1,
    0,
    0,
};

s32 gData_02003f4c[] = {
    1,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0017,
    23,
    33,
    (s32)0xffff001c,
    28,
    2,
    0x2020028,
    (s32)Func_02001d34,
    2,
    (s32)0xffff0029,
    (s32)Func_02001ddc,
    2,
    0x203002a,
    (s32)Func_02001e54,
    2,
    (s32)0xffff002b,
    (s32)Func_02001f04,
    6,
    (s32)0xffff0044,
    (s32)Func_0200160c,
    6,
    (s32)0xffff0045,
    (s32)Func_020019a0,
    0x8b85,
    -0x10000,
    (s32)Func_02000698,
    0x40008b85,
    -0x10000,
    (s32)Func_020006b4,
    0x50009085,
    -0x10000,
    (s32)Func_02002318,
    0x40009085,
    -0x10000,
    (s32)Func_02002328,
    -1,
    0,
    0,
};

s32 gData_02004018[] = {
    -1,
    0,
    0,
};
