#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "sequence_parent_work.h"
#include "call.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

extern s32 gData_02002618;

struct Actor *Actor_GetById();
void Actor_SetSpritePriority();
s32 Func_02001978();
s32 Func_02001c10();
s32 Func_02001c50();
s32 Func_02001c90();
struct Actor *Func_02000910();

void Func_02000054(void) {
  struct SequenceParentWork *party;

  party = &gSequenceParentWork;
  /* FAKEMATCH: ⚓️ loads the party's address before it builds the field offset.
   */
  __asm__ volatile("" : "+l"(party));
  if (party->renderMode == 10)
    Value4(Func_02001c90, 0x2200000, 0x1100000, 0, 255);
}

void Func_0200007c(void) { Value4(Func_02001c90, 0x2200000, 0x1100000, 0, 6); }

void Func_02000090(s32 id) {
  struct Actor *object;

  if (gData_02002618 == 0) {
    object = Actor_GetById(8);
    if (id == 30) {
      Value3(Func_02001c10, 8, 0x1280000, 0x2580000);
      object->y = 0;
      object->angle = 0x8000;
    } else if (id == 31) {
      Value3(Func_02001c10, 8, 0x880000, 0x2a80000);
      object->y = 0;
      object->angle = 0;
    }
    Func_02000910(object);
    gData_02002618 = 1;
  }
}

void Func_020000e8(s32 id) {
  struct Actor *object;
  struct Actor *motion;

  if (gData_02002618 == 0 && Actor_GetById(gSequenceParentWork.currentOwner)->y > 0) {
    object = Actor_GetById(8);
    if (id == 50) {
      Value3(Func_02001c10, 8, 0x1e80000, 0x1180000);
      object->y = 0x200000;
      object->angle = 0x8000;
    } else if (id == 51) {
      Value3(Func_02001c10, 8, 0x1d80000, 0xb80000);
      object->y = 0x200000;
      object->angle = 0x8000;
    } else if (id == 52) {
      Value3(Func_02001c10, 8, 0x2680000, 0x1180000);
      object->y = 0x200000;
      object->angle = 0;
    } else if (id == 53) {
      Value3(Func_02001c10, 8, 0x2780000, 0xb80000);
      object->y = 0x200000;
      object->angle = 0;
    }
    motion = Func_02000910(object);
    if (motion != NULL)
      motion->speedLimit = 0x10000;
    gData_02002618 = 1;
  }
}

void Func_0200018c(s32 arg) {
  if (Actor_GetById(gSequenceParentWork.currentOwner)->y == 0 &&
      Func_02001978(arg) != 0)
    Func_02001c50(13);
}

void Func_020001bc(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  if (Actor_GetById(gSequenceParentWork.currentOwner)->y == 0) {
    leader = Actor_GetById(gSequenceParentWork.currentOwner);
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 18 && z == 34)
      Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 3);
    if (x == 8 && z == 39)
      Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 3);
    if (Func_02001978(arg) != 0)
      Func_02001c50(14);
  }
}

void Func_02000228(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  leader = Actor_GetById(gSequenceParentWork.currentOwner);
  if (leader->y <= 0) {
    Func_0200018c(arg);
  } else {
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 25) {
      if (z == 17)
        Func_020000e8(50);
      if (z == 11)
        Func_020000e8(51);
    }
    if (x == 43) {
      if (z == 17)
        Func_020000e8(52);
      if (z == 11)
        Func_020000e8(53);
    }
  }
}
