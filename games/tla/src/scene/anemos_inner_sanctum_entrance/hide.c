#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"

extern s16 gData_02001cde;
extern struct Actor *gData_02001ce0[];
extern u16 gData_02001de2;
extern u16 gData_02001de4;
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

s32 Func_0200033c(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "anemos_inner_sanctum_entrance.h"

extern const u8 gData_020019dc[];
extern const u8 gData_020019f4[];
extern const u8 gData_02000e20[];
extern const u8 gData_02000e5c[];
extern const u8 gData_02000ea4[];
extern u8 MsgIAmTheShadow;

const u8 *Func_0200035c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6b1Room1)
    return gData_020019f4;
  return gData_020019dc;
}

void Func_02000388(void) {
  struct AnemosInnerSanctumEntranceParty *party;

  Func_02000cb4();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(8, 2);
  Event_Wait(20);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  Event_Wait(60);
  Func_0803ce64((s32)&MsgIAmTheShadow, 0, 6);
  party = (struct AnemosInnerSanctumEntranceParty *)&gMainRoutineWork;
  party->battleGroup = 0x1a9f;
  party->battleRule = 2;
  Func_02000d04(103, 3);
  GameFlag_SetBit(0x201);
}

void Func_02000408(void) {
  struct Actor *object = Actor_GetById(15);

  object->motionFlags = 0;
  object->scaleX = 0x4000;
  object->scaleY = 0x4000;
  Func_02000cb4();
  Event_ResolvePendingActions(0);
  Sound_Play(151);
  Actor_SetPartAttribute(Actor_GetById(8), 7);
  Actor_SetPartAttribute(Actor_GetById(15), 7);
  Event_Wait(20);
  Sound_Play(190);
  Func_02000cdc(15, 8);
  wait(1);
  Actor_SetMovementTypeById(8, gData_02000ea4);
  Actor_SetMovementTypeAndWaitById(15, gData_02000e20);
  Actor_SetMovementTypeAndWaitById(15, gData_02000e5c);
  Event_Wait(10);
  Func_02000cbc();
}

void Func_02000494(void) {
  s32 i;

  for (i = 9; i < 15; i++) {
    struct Actor *object = Actor_GetById(i);
    u8 *flag = (u8 *)object + 98;
    s32 none = 0;
    s32 on = 1;

    *flag = on;
    *(s32 *)((u8 *)object + 108) = none;
  }
}

void Func_020004b4(struct Actor *object) {
  if ((object->terrainHeight = Map_GetTerrainHeight(0, object->x, object->z)) >=
      -0x200000)
    Actor_SetSingleChildFlags(object, 1);
  else
    Actor_SetSingleChildFlags(object, 0);
}

extern const s8 gData_02000ee0[];

void Func_020004e4(s32 a, s32 b) {
  s32 i;

  const s8 *x = gData_02000ee0;
  const s8 *z = gData_02000ee0 + 1;

  for (i = 9; i <= 14; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x >> 20 != *x)
      return;
    {
      s32 cellZ = *z;

      z += 2;
      x += 2;
      if (object->z >> 20 != cellZ)
        return;
    }
  }
  GameFlag_SetBit(0x200);
  Sound_Play(201);
  Func_02000be0(1);
  Func_02000494();
}

void Func_02000534(s32 a) {
  struct MainRoutineWork *party;
  s32 held = Func_02000d4c();

  party = &gMainRoutineWork;
  if (held) {
    if (Func_02000d0c(party->currentOwner) != -1) {
      Func_02000d54();
      Func_020004e4(0, a);
    }
  } else {
    if (Actor_GetById(party->currentOwner)->y > -0x1c0000)
      Func_02000d14();
  }
}
