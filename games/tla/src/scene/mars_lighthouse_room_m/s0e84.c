#include "../../../../games/tla/src/scene/mars_lighthouse_room_m/mars_lighthouse_room_m.h"
#include "event_inline.h"

extern char SceneId_Ov6a8Room2;

void Func_020000c0(void);
void Func_020003c4(void);
s32 Func_02000058();
void Func_02000934(s32 entrance);
void Func_02002118(void);
void Func_02003150(s32 value);
void Func_020031b8(s32 scene, s32 a1);
void Func_02003270(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_020032b0(s32 a);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void SetBlend(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  Func_02003270(a, b, c, d, e, f, g);
}

s32 Func_02000e84(void) {
  s16 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov6ACRoom6 || scene == (s32)&SceneId_Ov6ACRoom2 ||
      scene == (s32)&SceneId_Ov6ACRoom3 || scene == (s32)&SceneId_Ov6ACRoom4 ||
      scene == (s32)&SceneId_Ov6ACRoom5) {
    Func_020032b0(1);
    SetBlend(1, 0x10000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000);
  }
  if (scene == (s32)&SceneId_Ov6ACRoom6) {
    switch (entrance) {
    case 1:
      Actor_SetSpriteColor(Actor_Get(8), 1);
      Func_02000058(9, 4);
      Func_02003150(4);
      Func_02003150(5);
      break;
    case 2:
      Actor_SetSpriteColor(Actor_Get(8), 3);
      Func_02000058(9, 5);
      Func_02003150(6);
      Func_02003150(7);
      break;
    case 3:
      Actor_SetSpriteColor(Actor_Get(8), 0);
      Func_02000058(9, 2);
      Func_02003150(0);
      Func_02003150(1);
      break;
    case 4:
      Actor_SetSpriteColor(Actor_Get(8), 4);
      Func_02000058(9, 3);
      Func_02003150(2);
      Func_02003150(3);
      break;
    }
    object = Actor_Get(10);
    object->motionFlags = 0;
    object->z = 0x680000;
    FieldTask_AddFrame(Func_020000c0, 0x480);
  } else if (scene == (s32)&SceneId_Ov6ACRoom1) {
    if (entrance == 1 || entrance == 2 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      Func_02002118();
      if (Event_GetFlag(0xa31)) {
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        MapGrid_CopyCellsLayered(64, 64, 4, 3, 76, 12);
      }
      Func_02000934(entrance);
      {
        s32 priority = 0x480;

        FieldTask_AddFrame(Func_020003c4, priority);
      }
      Func_02000058(12, 4);
      Func_02000058(13, 5);
      Func_02000058(14, 2);
      Func_02000058(15, 3);
      if (!Event_GetFlag(0xa32))
        Actor_SetPosition(12, 0, 0);
      if (!Event_GetFlag(0xa33))
        Actor_SetPosition(13, 0, 0);
      if (!Event_GetFlag(0xa34))
        Actor_SetPosition(14, 0, 0);
      if (!Event_GetFlag(0xa35))
        Actor_SetPosition(15, 0, 0);
    }
  }
  if (Event_GetFlag(0xa30)) {
    // A second name of the one scene id: the ROM keeps a literal for each
    // use, and with one name they share one.
    extern char SceneId_Ov6ACRoom3Second __asm__("SceneId_Ov6ACRoom3");

    Func_020031b8(gMainRoutineWork.map +
                      ((s32)&SceneId_Ov6ACRoom3Second - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  }
  return 0;
}
