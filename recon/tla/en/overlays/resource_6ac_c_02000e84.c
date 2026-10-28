// Draft, differ 4 of 237: rewritten from the listing. Sets the blend in rooms 2
// to 6, places objects 8 and 9 by the entrance in room 6, sets up room 1's
// cells for its entrances, and under flag 0xa30 loads the room's palette as
// Mars Lighthouse entrance's Func_02001930 does. Func_02000058 is declared
// returning s32 (it returns nothing; as void, its arguments are set r0 first:
// 36), which would be a signature device; an empty asm with a memory clobber
// between the two scene tests stops GCSE sharing room 6's id between them (22
// without), and a volatile asm on the task's priority builds it before the
// callback's pool load (6 without). Left: the native's pool holds
// SceneId_Ov6ACRoom3 twice, once for the first test and once for the palette,
// where this compile shares one entry, so the closing loads differ and the pool
// is a word short; a different symbol for one of them would explain it.
#include "../../../../games/tla/src/scene/mars_lighthouse_room_m/mars_lighthouse_room_m.h"

extern char SceneId_Ov6a8Room2;

void Func_020000c0(void);
void Func_020003c4(void);
s32 Func_02000058();
void Func_02000934(s32 entrance);
void Func_02002118(void);
void Func_02003150(s32 value);
void Func_02003170(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003178(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020031b8(s32 scene, s32 a1);
void Func_020031f8(s32 id, s32 a, s32 b);
void Func_02003270(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_020032b0(s32 a);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Task_Add(void (*func)(void), s32 priority);

static __inline__ void SetValue(s32 id, s32 value) {
  /* FAKEMATCH(wrapper): forwarded inline; see scene_call.h. */
  Func_02000058(id, value);
}

static __inline__ void SetBlend(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  /* FAKEMATCH(wrapper): forwarded inline; see scene_call.h. */
  Func_02003270(a, b, c, d, e, f, g);
}

s32 Func_02000e84(void) {
  s16 scene = gAdventure.scene;
  s32 entrance = gAdventure.entrance;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov6ACRoom6 || scene == (s32)&SceneId_Ov6ACRoom2 ||
      scene == (s32)&SceneId_Ov6ACRoom3 || scene == (s32)&SceneId_Ov6ACRoom4 ||
      scene == (s32)&SceneId_Ov6ACRoom5) {
    Func_020032b0(1);
    SetBlend(1, 0x10000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000);
  }
  __asm__("" : : : "memory");
  if (scene == (s32)&SceneId_Ov6ACRoom6) {
    switch (entrance) {
    case 1:
      Actor_SetPartAttribute(Actor_GetById(8), 1);
      Func_02000058(9, 4);
      Func_02003150(4);
      Func_02003150(5);
      break;
    case 2:
      Actor_SetPartAttribute(Actor_GetById(8), 3);
      Func_02000058(9, 5);
      Func_02003150(6);
      Func_02003150(7);
      break;
    case 3:
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Func_02000058(9, 2);
      Func_02003150(0);
      Func_02003150(1);
      break;
    case 4:
      Actor_SetPartAttribute(Actor_GetById(8), 4);
      Func_02000058(9, 3);
      Func_02003150(2);
      Func_02003150(3);
      break;
    }
    object = Actor_GetById(10);
    object->motionFlags = 0;
    object->z = 0x680000;
    Task_Add(Func_020000c0, 0x480);
  } else if (scene == (s32)&SceneId_Ov6ACRoom1) {
    if (entrance == 1 || entrance == 2 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      Func_02002118();
      if (GameFlag_Test(0xa31)) {
        Func_02003170(30, 12, 4, 3, 12, 12);
        Func_02003178(64, 64, 4, 3, 76, 12);
      }
      Func_02000934(entrance);
      {
        s32 priority = 0x480;

        asm volatile("" : "+l"(priority));
        Task_Add(Func_020003c4, priority);
      }
      Func_02000058(12, 4);
      Func_02000058(13, 5);
      Func_02000058(14, 2);
      SetValue(15, 3);
      if (!GameFlag_Test(0xa32))
        Func_020031f8(12, 0, 0);
      if (!GameFlag_Test(0xa33))
        Func_020031f8(13, 0, 0);
      if (!GameFlag_Test(0xa34))
        Func_020031f8(14, 0, 0);
      if (!GameFlag_Test(0xa35))
        Func_020031f8(15, 0, 0);
    }
  }
  if (GameFlag_Test(0xa30))
    Func_020031b8(gAdventure.map +
                      ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  return 0;
}
