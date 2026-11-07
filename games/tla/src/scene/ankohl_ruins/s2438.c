#include "version.h"

// The Japanese form is the older revision and is still on its listing.
#if TLA_PATCH_1

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define Test GameFlag_Test
#define C1(f, a) Call1((void (*)())f, a)
#define C2(f, a, b) Call2((void (*)())f, a, b)
#define C3(f, a, b, c) Call3((void (*)())f, a, b, c)
#define C4(f, a, b, c, d) Call4((void (*)())f, a, b, c, d)
#define C6(f, a, b, c, d, e, g) Call6((void (*)())f, a, b, c, d, e, g)

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
s32 GameFlag_Test(s32 flag);
void Func_02002e6c();
void Func_02002dfc();
struct Actor *Actor_GetById(u32 id);
void Func_02002df4();
void Func_02002eec();
void Actor_SetModeById();
void Func_02000418(void);
void Func_02002e44(void);
void Func_02002edc();
void Camera_MoveWithinMapBounds();
void Event_StartScreenTransition(void);
void FieldEvent_Wait();
void wait();
void GameFlag_SetBit();
void GameFlag_ClearBit();
void Func_02002ebc();
s32 Task_Add();
void Func_02001420(s32 a0);
void Func_02002e04();
void Func_02002dec(void);

s32 Func_02002438(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov686Room1) {
    C1(Func_02002ed4, 0);
    if (Test(0x9d2))
      C3(Func_02002e6c, 10, 0, 0);
    if (Test(0x9d3))
      C3(Func_02002e6c, 11, 0, 0);
    if (Test(0x9d4))
      C3(Func_02002e6c, 12, 0, 0);
    if (Test(0x9d5))
      C3(Func_02002e6c, 13, 0, 0);
    if (Test(0x9d6))
      C3(Func_02002e6c, 14, 0, 0);
    if (Test(0x9d7))
      C3(Func_02002e6c, 15, 0, 0);
    if (Test(0x9d8))
      C3(Func_02002e6c, 16, 0, 0);
    if (Test(0x990))
      C3(Func_02002e6c, 17, 0, 0);
    if (Test(0x991))
      C3(Func_02002e6c, 18, 0, 0);
    if (Test(0x992))
      C3(Func_02002e6c, 19, 0, 0);
    if (Test(0x993))
      C3(Func_02002e6c, 20, 0, 0);
    if (Test(0x994))
      C3(Func_02002e6c, 21, 0, 0);
    if (Test(0x995))
      C3(Func_02002e6c, 22, 0, 0);
    if (Test(0x996))
      C3(Func_02002e6c, 23, 0, 0);
    if (Test(0x997))
      C3(Func_02002e6c, 24, 0, 0);
    if (Test(0x998))
      C3(Func_02002e6c, 25, 0, 0);
    if (Test(0x999))
      C3(Func_02002e6c, 26, 0, 0);
    if (Test(0x99a))
      C3(Func_02002e6c, 27, 0, 0);
    if (Test(0x99b))
      C3(Func_02002e6c, 28, 0, 0);
    if (Test(0x99c))
      C3(Func_02002e6c, 29, 0, 0);
    if (Test(0x99d))
      C3(Func_02002e6c, 30, 0, 0);
    if (Test(0x9c6)) {
      C3(Func_02002e6c, 8, 0x480000, 0x580000);
      C6(Func_02002dfc, 4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_Ov686Room2) {
    if (Test(0x99e))
      C3(Func_02002e6c, 9, 0, 0);
    if (Test(0x99f))
      C3(Func_02002e6c, 10, 0, 0);
    if (Test(0x9a0))
      C3(Func_02002e6c, 11, 0, 0);
    if (Test(0x9dc)) {
      s32 width;

      object = Actor_GetById(8);
      object->motionFlags = 3;
      C3(Func_02002e6c, 8, 0xc80000, 0x580000);
      width = 10;
      C6(Func_02002df4, 35, 6, 7, 16, width, 7);
      C6(Func_02002df4, 35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        C1(Func_02002eec, 0x23f);
    }
  } else if (scene == (s32)&SceneId_Ov686Room3) {
    if (Test(0x9c7)) {
      C3(Func_02002e6c, 8, 0x2080000, 0x680000);
      C6(Func_02002dfc, 32, 69, 1, 1, 33, 69);
    }
    if (Test(0x9dd)) {
      C3(Func_02002e6c, 10, 0, 0);
      C2(Actor_SetModeById, 9, 4);
    }
    if (Test(0x201)) {
      s32 width = 13;

      C6(Func_02002dfc, 13, 30, 3, 2, width, 24);
      C6(Func_02002dfc, 14, 87, 1, 1, 14, 88);
      C6(Func_02002dfc, 41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_Ov686Room4) {
    if (Test(0x9a1))
      C3(Func_02002e6c, 11, 0, 0);
    if (Test(0x9a2))
      C3(Func_02002e6c, 12, 0, 0);
    if (Test(0x9a3))
      C3(Func_02002e6c, 13, 0, 0);
    if (Test(0x9a4))
      C3(Func_02002e6c, 14, 0, 0);
    if (Test(0x9a5))
      C3(Func_02002e6c, 15, 0, 0);
    if (Test(0x9a6))
      C3(Func_02002e6c, 16, 0, 0);
    if (Test(0x9a7))
      C3(Func_02002e6c, 17, 0, 0);
    if (Test(0x9c8))
      C3(Func_02002e6c, 8, 0x280000, 0x3080000);
    if (Test(0x9c9))
      C3(Func_02002e6c, 18, 0x2780000, 0x1c80000);
    if (Test(0x9ca)) {
      C3(Func_02002e6c, 19, 0x2680000, 0x880000);
      C6(Func_02002df4, 37, 70, 1, 1, 37, 71);
    }
    if (Test(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      C3(Func_02002e6c, 9, 0xc80000, 0x2d80000);
    }
    if (Test(0x9da)) {
      object = Actor_GetById(10);
      C3(Func_02002e6c, 10, 0xe80000, 0x2d80000);
      object->motionFlags = 3;
    } else if (Test(0x9db) && !Test(0x109)) {
      C3(Func_02002e6c, 10, 0x1480000, 0x2f80000);
    }
    if (Test(0x9da) && Test(0x9d9)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 4)
        C1(Func_02002eec, 0x23f);
      width = 12;
      C6(Func_02002df4, 16, 27, 3, 13, width, 47);
      C6(Func_02002df4, 16, 91, 3, 13, width, 111);
    }
    if (!Test(0x109) && gMainRoutineWork.entrance == 17) {
      Func_02000418();
#if TLA_PATCH_4
      C1(GameFlag_ClearBit, 0x122);
#endif
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance == 20) {
        s32 frames;

        Func_02002e44();
        C1(Func_02002edc, 0);
        C4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
        C3(Func_02002e6c, late->currentOwner, 0, 0);
        C3(Func_02002e6c, 10, 0, 0);
        Event_StartScreenTransition();
        C1(FieldEvent_Wait, 15);
        object = Actor_GetById(10);
        C3(Func_02002e6c, 10, 0x1480000, 0x2f80000);
        object->y += 0x800000;
        frames = 60;
        object->motionFlags = 3;
        for (; frames != 0; frames--) {
          C1(wait, 1);
          if (object->velocityY > -1)
            break;
        }
        C1(Func_02002eec, 188);
        C1(FieldEvent_Wait, 60);
        C1(GameFlag_SetBit, 0x109);
        C1(Func_02002ebc, 20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_Ov686Room5) {
    if (Test(0x9db))
      C3(Func_02002e6c, 9, 0, 0);
    if (Test(0x9cb)) {
      C3(Func_02002e6c, 8, 0x1180000, 0x1380000);
      C6(Func_02002dfc, 16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      C1(Func_02002eec, 0x23f);
    task:
      C2(Task_Add, (s32)Func_020010f4, 0x480);
    }
  } else if (scene == (s32)&SceneId_Ov686Room6) {
    s16 *entrance = &work->entrance;

    if (work->entrance <= 4)
      Func_02001420(a0);
    if (Test(0x9e2)) {
      s32 x = 14;

      C6(Func_02002df4, 26, 67, 1, 3, x, 65);
      C6(Func_02002dfc, 15, 5, 1, 1, x, 5);
    }
    if (Test(0x9e3)) {
      s32 x = 2;

      C6(Func_02002df4, 26, 67, 1, 3, x, 83);
      C6(Func_02002dfc, 1, 23, 1, 1, x, 23);
    }
    if (Test(0x109) && Test(0x204)) {
      s32 level = 0x200000;

      Data_02004030.level = level;
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsFloatWork *)entrance)->unk00 - 1) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 5;
        C6(Func_02002dfc, 25, 22, 9, 2, x, z);
        C6(Func_02002dfc, 25, 86, 9, 2, x, 69);
        {
          s32 x = 1;

          C6(Func_02002dfc, 22, 24, 15, 7, x, 7);
          C6(Func_02002dfc, 22, 88, 15, 7, x, 71);
        }
        C6(Func_02002e04, 69, 6, 1, 2, 68, z);
      }
      if ((u16)(*entrance - 3) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 23;
        C6(Func_02002dfc, 25, 22, 9, 2, x, z);
        C6(Func_02002dfc, 25, 86, 9, 2, x, 87);
        {
          s32 x = 1;

          C6(Func_02002dfc, 22, 24, 15, 7, x, 25);
          C6(Func_02002dfc, 22, 88, 15, 7, x, 89);
        }
        C6(Func_02002e04, 69, 24, 1, 4, 68, z);
      }
      object = Actor_GetById(work->currentOwner);
      object->terrainHeight = level;
      object->y = level;
      C1(FieldEvent_Wait, 1);
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
