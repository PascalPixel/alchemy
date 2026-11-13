#include "flags.h"
#include "version.h"
#include "event_inline.h"
#include "module/scene/ankohl_ruins.h"

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
void Event_StartScreenTransition(void);
void Func_02001420(s32 a0);
void Map_CenterOnFollowedPoint(void);

s32 Scene_Initialize(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov686Room1) {
    Call1((void (*)())Func_02002ed4, 0);
    if (GameFlag_Get(0x9d2))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Get(0x9d3))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9d4))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Get(0x9d5))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Get(0x9d6))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Get(0x9d7))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Get(0x9d8))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Get(0x990))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Get(0x991))
      Actor_SetPosition(18, 0, 0);
    if (GameFlag_Get(0x992))
      Actor_SetPosition(19, 0, 0);
    if (GameFlag_Get(0x993))
      Actor_SetPosition(20, 0, 0);
    if (GameFlag_Get(0x994))
      Actor_SetPosition(21, 0, 0);
    if (GameFlag_Get(0x995))
      Actor_SetPosition(22, 0, 0);
    if (GameFlag_Get(0x996))
      Actor_SetPosition(23, 0, 0);
    if (GameFlag_Get(0x997))
      Actor_SetPosition(24, 0, 0);
    if (GameFlag_Get(0x998))
      Actor_SetPosition(25, 0, 0);
    if (GameFlag_Get(0x999))
      Actor_SetPosition(26, 0, 0);
    if (GameFlag_Get(0x99a))
      Actor_SetPosition(27, 0, 0);
    if (GameFlag_Get(0x99b))
      Actor_SetPosition(28, 0, 0);
    if (GameFlag_Get(0x99c))
      Actor_SetPosition(29, 0, 0);
    if (GameFlag_Get(0x99d))
      Actor_SetPosition(30, 0, 0);
    if (GameFlag_Get(0x9c6)) {
      Actor_SetPosition(8, 0x480000, 0x580000);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_Ov686Room2) {
    if (GameFlag_Get(0x99e))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x99f))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Get(0x9a0))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9dc)) {
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
    if (GameFlag_Get(0x9c7)) {
      Actor_SetPosition(8, 0x2080000, 0x680000);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Get(0x9dd)) {
      Actor_SetPosition(10, 0, 0);
      Actor_SetAnimation(9, 4);
    }
    if (GameFlag_Get(0x201)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_Ov686Room4) {
    if (GameFlag_Get(0x9a1))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9a2))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Get(0x9a3))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Get(0x9a4))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Get(0x9a5))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Get(0x9a6))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Get(0x9a7))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Get(0x9c8))
      Actor_SetPosition(8, 0x280000, 0x3080000);
    if (GameFlag_Get(0x9c9))
      Actor_SetPosition(18, 0x2780000, 0x1c80000);
    if (GameFlag_Get(0x9ca)) {
      Actor_SetPosition(19, 0x2680000, 0x880000);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      Actor_SetPosition(9, 0xc80000, 0x2d80000);
    }
    if (GameFlag_Get(0x9da)) {
      object = Actor_GetById(10);
      Actor_SetPosition(10, 0xe80000, 0x2d80000);
      object->motionFlags = 3;
    } else if (GameFlag_Get(0x9db) && !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(10, 0x1480000, 0x2f80000);
    }
    if (GameFlag_Get(0x9da) && GameFlag_Get(0x9d9)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 4)
        Event_PlaySound(0x23f);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) && gMainRoutineWork.entrance == 17) {
      Func_02000418();
#if TLA_PATCH_4
      Event_ClearFlag(0x122);
#endif
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance == 20) {
        s32 frames;

        Event_Start();
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
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_Ov686Room5) {
    if (GameFlag_Get(0x9db))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x9cb)) {
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
    if (GameFlag_Get(0x9e2)) {
      s32 x = 14;

      MapGrid_CopyCells(26, 67, 1, 3, x, 65);
      MapGrid_CopyCellAttributes(15, 5, 1, 1, x, 5);
    }
    if (GameFlag_Get(0x9e3)) {
      s32 x = 2;

      MapGrid_CopyCells(26, 67, 1, 3, x, 83);
      MapGrid_CopyCellAttributes(1, 23, 1, 1, x, 23);
    }
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) && GameFlag_Get(0x204)) {
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
      Map_CenterOnFollowedPoint();
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
void Event_Start(void);
void Event_StartScreenTransition(void);
void Func_02001420(s32 a0);
void Map_CenterOnFollowedPoint(void);

s32 Scene_Initialize(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov686Room1) {
    Call1((void (*)())Func_02002ed4, 0);
    if (GameFlag_Get(0x9d2))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Get(0x9d3))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9d4))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Get(0x9d5))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Get(0x9d6))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Get(0x9d7))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Get(0x9d8))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Get(0x990))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Get(0x991))
      Actor_SetPosition(18, 0, 0);
    if (GameFlag_Get(0x992))
      Actor_SetPosition(19, 0, 0);
    if (GameFlag_Get(0x993))
      Actor_SetPosition(20, 0, 0);
    if (GameFlag_Get(0x994))
      Actor_SetPosition(21, 0, 0);
    if (GameFlag_Get(0x995))
      Actor_SetPosition(22, 0, 0);
    if (GameFlag_Get(0x996))
      Actor_SetPosition(23, 0, 0);
    if (GameFlag_Get(0x997))
      Actor_SetPosition(24, 0, 0);
    if (GameFlag_Get(0x998))
      Actor_SetPosition(25, 0, 0);
    if (GameFlag_Get(0x999))
      Actor_SetPosition(26, 0, 0);
    if (GameFlag_Get(0x99a))
      Actor_SetPosition(27, 0, 0);
    if (GameFlag_Get(0x99b))
      Actor_SetPosition(28, 0, 0);
    if (GameFlag_Get(0x99c))
      Actor_SetPosition(29, 0, 0);
    if (GameFlag_Get(0x99d))
      Actor_SetPosition(30, 0, 0);
    if (GameFlag_Get(0x9c6)) {
      Actor_SetPosition(8, 0x480000, 0x580000);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_Ov686Room2) {
    if (GameFlag_Get(0x99e))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x99f))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Get(0x9a0))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9dc)) {
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
    if (GameFlag_Get(0x9c7)) {
      Actor_SetPosition(8, 0x2080000, 0x680000);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Get(0x9dd)) {
      Actor_SetPosition(10, 0, 0);
      Actor_SetAnimation(9, 4);
    }
    if (GameFlag_Get(0x201)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_Ov686Room4) {
    if (GameFlag_Get(0x9a1))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9a2))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Get(0x9a3))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Get(0x9a4))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Get(0x9a5))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Get(0x9a6))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Get(0x9a7))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Get(0x9c8))
      Actor_SetPosition(8, 0x280000, 0x3080000);
    if (GameFlag_Get(0x9c9))
      Actor_SetPosition(18, 0x2780000, 0x1c80000);
    if (GameFlag_Get(0x9ca)) {
      Actor_SetPosition(19, 0x2680000, 0x880000);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      Actor_SetPosition(9, 0xc80000, 0x2d80000);
    }
    if (GameFlag_Get(0x9da)) {
      object = Actor_GetById(10);
      Actor_SetPosition(10, 0xe80000, 0x2d80000);
      object->motionFlags = 3;
    } else if (GameFlag_Get(0x9db) && !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(10, 0x1480000, 0x2f80000);
    }
    if (GameFlag_Get(0x9da) && GameFlag_Get(0x9d9)) {
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

        Event_Start();
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
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_Ov686Room5) {
    if (GameFlag_Get(0x9db))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x9cb)) {
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
    if (GameFlag_Get(0x9e2)) {
      s32 x = 14;

      MapGrid_CopyCells(26, 67, 1, 3, x, 65);
      MapGrid_CopyCellAttributes(15, 5, 1, 1, x, 5);
    }
    if (GameFlag_Get(0x9e3)) {
      s32 x = 2;

      MapGrid_CopyCells(26, 67, 1, 3, x, 83);
      MapGrid_CopyCellAttributes(1, 23, 1, 1, x, 23);
    }
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) && GameFlag_Get(0x204)) {
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
      Map_CenterOnFollowedPoint();
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
void Func_020006d0(void);
void Func_02000850(void);
void Func_02000a68(void);
void Func_02000c24(void);
void Func_02000e60(void);
void Func_0200104c(void);
void Func_0200124c(void);
void Func_02001264(void);
void Func_0200160c(void);
void Func_020019a0(void);
void Func_020022e0(void);

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

s32 gFieldEffect_DurationScripts[] = {
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

struct ActorPlacement gData_020032a0[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020032b8[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(88), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(456), 0, PIXELS(840), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(696), 0, PIXELS(840), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(536), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(568), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(600), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(648), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(584), 0, PIXELS(56), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(168), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(264), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(296), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(184), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(840), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(936), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(856), 0, PIXELS(616), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020034f8[] = {
    {391, CONDITION_ALWAYS, (s32)gData_02003280, PIXELS(120), 0, PIXELS(88), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(472), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(520), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(600), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003570[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {390, CONDITION_ALWAYS, (s32)gData_02003290, PIXELS(232), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {393, CONDITION_ALWAYS, (s32)gData_02003280, PIXELS(296), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020035d0[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {391, CONDITION_ALWAYS, (s32)gData_02003280, PIXELS(104), 0, PIXELS(760), FACING_SOUTH, 2, 0},
    {393, CONDITION_ALWAYS, (s32)gData_02003280, 0, 0, 0, FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(424), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(504), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(712), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(792), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(648), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(728), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(808), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(648), 0, PIXELS(456), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(136), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003708[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(312), FACING_SOUTH, 2, 0},
    {393, CONDITION_ALWAYS, (s32)gData_02003280, PIXELS(440), 0, PIXELS(584), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003750[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003768[] = {
    {SCENE_TABLE_END},
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

struct Trigger gData_02003790[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020037b4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TRIGGER_EXIT, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_EXIT, 27, CONDITION_ALWAYS, 27},
    {0x21, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_EXIT, 29, CONDITION_ALWAYS, 29},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {TRIGGER_EXIT, 31, CONDITION_ALWAYS, 31},
    {0x21, 32, CONDITION_ALWAYS, 32},
    {0x21, 33, CONDITION_ALWAYS, 33},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c6, EVENT(Func_02000850)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x9c6, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x9d2, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9d3, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x9d4, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x9d5, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, 0x9d6, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x9d7, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0x9d8, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 18, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 19, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 21, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 22, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 23, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 24, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 25, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 26, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 27, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 28, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 29, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 30, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003a78[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x21, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {0x21, 19, CONDITION_ALWAYS, 19},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9dc, EVENT(Func_02000e60)},
    {0x9, 8, 0x9dc, EVENT(Func_02000e60)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003b98[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(Func_020020fc)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(Func_020021d4)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x9c7, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c7, EVENT(Func_02000850)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x9dd, EVENT(Func_0200104c)},
    {0x9, 10, 0x9dd, EVENT(Func_0200104c)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003c94[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {0x31, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x21, 15, CONDITION_ALWAYS, 15},
    {0x21, 16, CONDITION_ALWAYS, 16},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 16, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_0200124c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT(Func_02000c24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_02000c24)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x9c8, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c8, EVENT(Func_02000850)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 18, 0x9c9, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, 0x9c9, EVENT(Func_02000850)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 19, 0x9ca, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, 0x9ca, EVENT(Func_02000850)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003e38[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0x31, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x4602, 13, CONDITION_ALWAYS, EVENT(Func_02000384)},
    {TRIGGER_EXIT, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_STEP, 66, CONDITION_ALWAYS, EVENT(Func_020011c0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x9cb, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9cb, EVENT(Func_02000850)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 9, 0x9db, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x9db, EVENT(Func_02002260)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_020022a8)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020022e0)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003f4c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {0x21, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_STEP, 40, 0x202, EVENT(Func_02001d34)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(Func_02001ddc)},
    {TRIGGER_STEP, 42, 0x203, EVENT(Func_02001e54)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(Func_02001f04)},
    {TRIGGER_RAISED, 68, CONDITION_ALWAYS, EVENT(Func_0200160c)},
    {TRIGGER_RAISED, 69, CONDITION_ALWAYS, EVENT(Func_020019a0)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_02002318)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02002328)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02004018[] = {
    {TRIGGER_TABLE_END},
};
