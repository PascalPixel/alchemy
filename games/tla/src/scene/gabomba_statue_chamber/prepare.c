/// @file
/// Scene code of overlay 0x672, in ROM order, from prepare's offset up to the next object the overlay links.

#include "types.h"
#include "scene_tables.h"
#include "gabomba_statue_chamber.h"
#include "event_inline.h"

void Func_02000f00(void);
void Func_02001010(void);
void Func_02001064(void);
void Func_020010d4(void);
void Func_02001710(void);
void Func_02001b14(void);
void Func_02001ba8(void);
void Func_02001c44(void);

s32 Func_02001aa4(void) {
  struct GabombaStatueChamberEventWork *work;
  s16 scene;

  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 521;
  ((struct GabombaStatueChamberObject *)Actor_GetById(Party_GetLeaderActor()))
      ->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov672Room1)
    Func_02001b14();
  else if (scene == (s32)&SceneId_Ov672Room2)
    Func_02001ba8();
  else if (scene == (s32)&SceneId_Ov672Room3)
    Func_02001c44();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02001b14(void) {
  Func_02001e80(8);
  Func_02001e80(9);
  Func_02001e80(10);
  wait(1);
  if (GameFlag_Test(0x8fd)) {
    Func_02001010();
    Func_02001064();
    Actor_SetPosition(8, PIXELS(424), PIXELS(152));
    wait(1);
    Func_02000f00();
  } else {
    s32 entrance;

    entrance = gMainRoutineWork.entrance;
    if (entrance == 5) {
      Func_020010d4();
      return;
    }
    if (entrance == 6) {
      Func_02001710();
      return;
    }
  }
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->entrance == 3) {
      GameFlag_ClearBit(0xa27);
      GameFlag_SetBit(0xa28);
    }
  }
}

#include "../../../include/system/io_reg.h"

struct GabombaStatueChamberLayer {
  u8 unk00[12];
  s32 y;
  u8 unk10[0x28];
};

struct GabombaStatueChamberMapWork {
  u8 unk000[0x108];
  struct GabombaStatueChamberLayer layers[3];
};

void Func_02001d40(void);
void Func_02001d60(void);
void Func_02000be8(void);

void Func_02001ba8(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(519)) {
      struct GabombaStatueChamberMapWork *map =
          (struct GabombaStatueChamberMapWork *)Ram_HeapSlots->mapWork;
      struct GabombaStatueChamberLayer *front = &map->layers[0];
      struct GabombaStatueChamberLayer *back = &map->layers[2];
      s32 blend = 1;
      s32 i;

      Func_02001d60();
      i = 0;
    loop:
      if (i >= 17 && i <= 79) {
        u32 alpha = ((blend + 1) / 4) << 8;

        alpha |= 16 - (blend + 1) / 4;
        REG_BLDALPHA = alpha;
        blend++;
      }
      front->y += 0x10000;
      back->y += 0x10000;
      i++;
      if (i < 352)
        goto loop;
      Func_02001d40();
      wait(1);
    }
  } else if (gMainRoutineWork.entrance == 2) {
    Func_02000be8();
  }
}

#include "actor.h"
#include "main_routine_work.h"
#include "version.h"

extern char SceneId_Ov671Room7, SceneId_Ov672Room1;
void Func_02001094(void);
void Func_02000f88(void);

/// Gives actors 15 to 32 action variant 2, runs Func_02001e80 for actor 8 and
/// grounds it, then Func_02001094 with actor 10 placed when flag 0x8ff is set,
/// else Func_02000f88; arriving by entrance 2 it sets the retreat to
/// Ov671Room7's entrance 1 (flag 0xa27) or Ov672Room1's entrance 3 (flag
/// 0xa28).
void Func_02001c44(void) {
  u32 i;
  char *scene;

  for (i = 0; i <= 17; i++)
    Actor_SetSpritePriority(i + 15, 2);
  Func_02001e80(8);
  Actor_GetById(8)->y = 0;
  if (GameFlag_Test(0x8ff)) {
    Func_02001094();
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  } else {
    Func_02000f88();
  }
#if TLA_PATCH_1 /* The Japanese edition does not set the retreat. */
  if (gMainRoutineWork.entrance == 2) {
    if (GameFlag_Test(0xa27)) {
      /* FAKEMATCH(temporary): the scene id through a local, which issues its
       * literal load first; as plain stores, 4 of 71 instructions differ in
       * tla-en. */
      scene = &SceneId_Ov671Room7;
      gMainRoutineWork.retreatScene = (s32)scene;
        gMainRoutineWork.retreatEntrance = 1;
    } else if (GameFlag_Test(0xa28)) {
      /* FAKEMATCH(temporary): the scene id through a local, as in the first
       * branch; as plain stores, 4 of 71 instructions differ in tla-en. */
      scene = &SceneId_Ov672Room1;
      gMainRoutineWork.retreatScene = (s32)scene;
        gMainRoutineWork.retreatEntrance = 3;
    }
  }
#endif
}
