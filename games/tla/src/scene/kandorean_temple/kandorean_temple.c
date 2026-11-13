/// @file
/// Scene code of overlay 0x653, in ROM order.

#include "../kandorean_temple.h"
#include "field.h"
#include "module/scene/kandorean_temple.h"
#include "module/scene/scene.h"
#include "text/msg_ids.h"

extern s32 gData_020062a4;

void Func_02000280(void) { Field_RequestDjinnBattle(11, 27); }

u32 Func_0200028c(struct Actor *object) {
  s32 left;

  left = --*(s16 *)&object->unk66;
  return (u32)(-left | left) >> 31;
}

s32 Func_020002a0(struct Actor *object) {
  if (object->timer != 0) {
    object->timer--;
  } else {
    Actor_ShowEmoteById((s16)object->unk66, 0x105, 0);
    object->timer = ((Random16Far() * 240) >> 16) + 240;
  }
  return 1;
}

u32 __umodsi3(u32 a, u32 b);

s32 Func_020002dc(struct Actor *object) {
  switch (object->timer) {
  case 6:
    object->scaleX -= 0x4000;
    {
      s32 scale = object->scaleY;

      object->scaleY = scale + 0x2000;
    }
    break;
  case 4:
    object->scaleX += 0x2000;
    object->scaleY -= 0x1000;
    break;
  case 2:
    object->scaleX += 0x1000;
    object->scaleY -= 0x800;
    break;
  case 0:
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    object->timer = Random16Far() % 90 + 60;
    break;
  }
  object->timer--;
  return 1;
}

s32 Func_0200035c(struct Actor *object) {
  object->motionFlags = 0;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  object->collisionFlags |= 8;
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

#include "battle.h"

#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern u32 gData_020056a0[];

u32 *Scene_GetRegions(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov653Room3)
    return gData_020056a0;
  if (scene == (s32)&SceneId_Ov653Room5)
    return gData_020056c0;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "event_inline.h"
#include "macros.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "version.h"
#include "task.h"

extern u32 gData_020057a8[];
extern u32 gData_020057c0[];
extern u32 gData_020058b0[];
extern u32 gData_02005958[];
extern u32 gData_02005aa8[];

void *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov653Room1)
    return gData_020057c0;
  if (scene == (s32)&SceneId_Ov653Room3)
    return gData_020058b0;
  if (scene == (s32)&SceneId_Ov653Room4)
    return gData_02005958;
  if (scene == (s32)&SceneId_Ov653Room7)
    return gData_02005aa8;
  if (scene == (s32)&SceneId_Ov653Room8) {
    if (GameFlag_Test(0x849)) {
      gData_02005b20[3].talkFacing = 0;
      gData_02005b20[4].talkFacing = 0;
      gData_02005b20[5].talkFacing = 0;
    }
    return gData_02005b20;
  }
  if (scene == (s32)&SceneId_Ov653Room9) {
    if (GameFlag_Test(0x849)) {
      gData_02005be0[5].talkFacing = 3;
      gData_02005be0[6].x = 0x1700000;
      gData_02005be0[6].z = 0x2b60000;
      gData_02005be0[7].talkFacing = 1;
    }
    return gData_02005be0;
  }
  return gData_020057a8;
}

void Func_020004a8(void) {
  struct Actor *object;

  if (GameFlag_Test(0x842) == 0) {
    GameFlag_SetBit(0x230);
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_WalkToAndWaitById(4, 104, 252);
    Actor_FaceDirection(4, FACING_NORTHWEST, 0);
    FieldActor_LaunchFromLeader(14, -16, -16, FACING_NORTH);
    Actor_WaitForMoveById(14);
    Event_Wait(20);
    Event_SetMessageId((s32)&MsgKandoreanTemple11);
    Event_ShowMessageAndWait(14, 0);
    Actor_FaceDirectionById(14, FACING_SOUTHEAST);
    Event_Wait(30);
    Actor_SetModeById(14, 3);
    Motion_SetModeAndWaitAnimation(4, 3);
    Event_Wait(20);
    Actor_SetSpeed(14, 0x13333, 0x9999);
    Actor_SetModeById(14, 2);
    object = Actor_GetById(4);
    if (object)
      Actor_SetDestinationById(14, ((s16 *)&object->x)[1],
                               ((s16 *)&object->z)[1]);
    Actor_WaitForMoveById(14);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Event_End();
  }
}

extern s32 gData_02005d48[];
extern s32 gData_02005d54[];
extern s32 gData_02005e20[];
extern s32 gData_02005e8c[];
extern s32 gData_02005ef8[];
extern s32 gData_02005f10[];
extern s32 gData_02005f40[];
extern s32 gData_02005fc4[];
extern s32 gData_0200615c[];

void *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov653Room1)
    return gData_02005d54;
  if (scene == (s32)&SceneId_Ov653Room2)
    return gData_02005de4;
  if (scene == (s32)&SceneId_Ov653Room3)
    return gData_02005e20;
  if (scene == (s32)&SceneId_Ov653Room4)
    return gData_02005e8c;
  if (scene == (s32)&SceneId_Ov653Room5)
    return gData_02005ef8;
  if (scene == (s32)&SceneId_Ov653Room6)
    return gData_02005f10;
  if (scene == (s32)&SceneId_Ov653Room7)
    return gData_02005f40;
  if (scene == (s32)&SceneId_Ov653Room8)
    return gData_02005fc4;
  if (scene == (s32)&SceneId_Ov653Room9)
    return gData_0200615c;
  return gData_02005d48;
}

void Func_0200063c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Func_02000668(leader, 0);
}

void Func_02000668(struct Actor *object, s32 step) {
  s32 speed = (object->velocityX + object->velocityZ) >> 1;
  s32 *counter = &gData_02006294;

  if (speed < 0)
    speed = -speed;
  if ((u32)speed > 0xbb80)
    *counter += 4;
  else if (speed != 0)
    *counter += 2;
  gData_02006294 += step;
  if (gData_02006294 > 40) {
    Func_020006b0(object);
    gData_02006294 = 0;
  }
}

void Func_020006b0(struct Actor *object) {
  struct Actor *steam = Func_02005140_(0x10d, object->x, object->y, object->z);
  struct FieldSprite *sprite;

  if (steam) {
    sprite = steam->sprite;
    Actor_StartScript(steam, gData_020054e4);
    steam->motionFlags = 0;
    steam->terrainId = 1;
    steam->priorityFlags = 2;
    if (sprite) {
      Animation_ApplyChildArgument(sprite, 2);
      sprite->blendMode = 1;
      sprite->nextFrame = 0;
      sprite->priority = 3;
    }
  }
}

void Func_02000710(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *steam;

  if ((gFrameCount & 7) == 0) {
    steam = ((struct Actor *)Import_ActorCreate(
        14, leader->x, leader->y + 0x60000, leader->z));
    if (steam) {
      Actor_SetSpriteFlags(steam, 0);
      Actor_SelectAnimation(steam, 1);
      Actor_StartScript(steam, gData_020054f0);
    }
  }
  Func_02000778(leader, 0xd999);
}

/* VIEW(kandorean_temple): tag struct EventWork1cc as struct
 * EventWork1cc__kandorean_temple; module/tla.h has another definition; tried
 * with the definition the rest of the object has, its members read focus as
 * object, it does not compile in tla-ja:
 * games/tla/src/scene/kandorean_temple.c:268: dereferencing pointer to
 * incomplete type. */
struct EventWork1cc__kandorean_temple {
  u8 unk000[0x1cc];
  struct Actor *focus;
};

void Func_02000778(struct Actor *object, s32 dz) {
  struct Actor *focus =
      ((struct EventWork1cc__kandorean_temple *)Ram_HeapSlots->eventWork)
          ->focus;
  s32 position[3];

  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z + dz;
  MapGrid_CheckCollision(object, position);
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z + 0x80000;
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  position[0] = object->x + 0x5b333;
  position[1] = object->y;
  position[2] = object->z + 0x5b333;
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  position[0] = object->x - 0x5b333;
  position[1] = object->y;
  position[2] = object->z + 0x5b333;
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  focus->z += dz;
  object->z += dz;
}

s32 Func_02000824(struct Actor *a, struct Actor *b) {
  s32 left = b->x - 0xc0000;
  s32 right = b->x + 0xc0000;

  if (a->x > left && a->x < right && a->z > b->z - 0x80000 &&
      a->z < b->z + 0x80000)
    return 1;
  return 0;
}

void Func_02000864(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *other = Actor_GetById(8);
  s32 vx = object->bounce;
  s32 vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  sprite->rotation += object->timer;
  if (leader->y == 0) {
    if (Func_02000824(object, leader)) {
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
    if (leader->y == 0 && Func_02000824(object, other)) {
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
  }
}

void Func_02000930(struct Actor *object, s32 priority) {
  struct FieldSprite *sprite;

  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  sprite = object->sprite;
  sprite->priority = priority;
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, 2);
  Actor_StartScript(object, gData_02005494);
  Actor_SetSpriteColor(object, 9);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_0200098c(s32 *position, s32 value) {
  struct Actor *object =
      Func_02005140_(0x11d, position[0], position[1], position[2] - 0x20000);

  if (object) {
    s32 timer;

    object->bounce = Trig_Cos(((Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() * 2) + value;
    timer = (u16)Random16Far() - 0x8000;
    object->timer = timer;
    Func_02000930(object, 3);
    object->update = (void (*)(void))Func_02000864;
    Actor_SetBlendMode(object, 1);
  }
}

void Func_02000a0c(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *other = Actor_GetById(12);
  s32 vx = object->bounce;
  s32 vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  /* FAKEMATCH(dead): an empty once-only block, so the Z speed is stored before
   * the scale rate loads, as in Func_02000864; without it, 2 of 97 instructions
   * differ in tla-en. */
  do {
  } while (0);
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->sprite;
  object->scaleX = scale + rate;
  {
    /* FAKEMATCH(pin): the call's first argument pinned to r0, so it is copied
     * before the Y scale store; as a plain local, copied after the action's
     * halfword load, 4 of 97 instructions differ in tla-en. */
    register struct Actor *first asm("r0") = object;

    object->scaleY += object->acceleration;
    /* FAKEMATCH(dead): an empty once-only block, so the Y scale is stored
     * before the action is addressed, as in Func_02000864; without it, 2 of 97
     * instructions differ in tla-en. */
    do {
    } while (0);
    sprite->rotation += object->timer;
    if (Func_02000824(first, leader)) {
      ((struct FieldSprite *)object->sprite)->priority = 3;
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
  }
  if (Func_02000824(object, other)) {
    ((struct FieldSprite *)object->sprite)->priority = 3;
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 3;
    object->unk4c = 0;
  }
}

void Func_02000adc(void) {
  struct Actor *object = ((struct Actor *)Import_ActorCreate(
      0x11d, 0x2b80000, 0x1c0000, 0x9c0000));

  if (object) {
    s32 timer;

    object->bounce = Trig_Cos(((Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() * 2) + 0x58000;
    timer = (u16)Random16Far() - 0x8000;
    object->timer = timer;
    Func_02000930(object, 1);
    object->update = (void (*)(void))Func_02000a0c;
    Actor_SetBlendMode(object, 1);
  }
}

void Func_02000b5c(void) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
}

void Func_02000b78(s32 id) {
  struct Actor *object = Actor_GetById(id);

  *((u8 *)object + 98) = 1;
  object->priorityFlags |= 2;
  Actor_SetModeById(id, 1);
  Actor_SetSpritePriority(id, 3);
  Func_020052c0(id);
}

void Func_02000bac(s32 id) {
  *((u8 *)Actor_GetById(id) + 98) = 1;
  Actor_SetSpritePriority(id, 2);
  Func_020052c0(id);
}

void Func_02000bcc(void) {
  BattleEffect_LoadActionEffectResources(133, 1);
  FieldPsynergy_SetupActorPair(12, 10);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
}

#define DEFINE_KANDOREAN_TEMPLE_SPAWN_EVERY(name, data1, n1, n2, n3, n4)       \
  void name(void) {                                                            \
    s32 position[3];                                                           \
                                                                               \
    if (data1 != 0 && gFrameCount % n1 == 0) {                                 \
      SET_VECTOR(position, n2, 0, n3);                                         \
      Func_0200098c(position, n4);                                             \
    }                                                                          \
  }

DEFINE_KANDOREAN_TEMPLE_SPAWN_EVERY(Func_02000bf4, gData_02006298, 9, 0x1280000,
                                    0xb00000, 0x20000)

DEFINE_KANDOREAN_TEMPLE_SPAWN_EVERY(Func_02000c34, gData_0200629c, 5, 0x1580000,
                                    0xa00000, 0x40000)

void Func_02000c74(void) {
  if (gFrameCount % 5 == 0)
    Func_02000adc();
}

void Func_02000c90(s32 id) {
  Actor_GetById(id)->scaleY = 0x800;
  Actor_SetPositionAndResetMotion_(Actor_GetById(id), PIXELS(216), 0,
                                   PIXELS(216));
}

void Func_02000cb4(s32 id) {
  Actor_GetById(id)->scaleY = 0x800;
  Actor_SetPositionAndResetMotion_(Actor_GetById(id), PIXELS(376), 0,
                                   PIXELS(248));
}

void Func_02000cd8(s32 id, s32 dy, s32 scale) {
  struct Actor *object = Actor_GetById(id);

  object->scaleY += scale;
  object->y += dy;
  object->z -= dy;
}

void Func_02000cf8(s32 id, s32 dy, s32 scale) {
  struct Actor *object = Actor_GetById(id);

  object->scaleY -= scale;
  object->y -= dy;
  object->z += dy;
}

s32 Func_02000d18(void) {
  if (GameFlag_Test(0x20a) || GameFlag_Test(0x300))
    return 1;
  return 0;
}

s32 Func_02000d3c(void) {
  if (GameFlag_Test(0x20a) || GameFlag_Test(0x301))
    return 1;
  return 0;
}

void Func_02000d64(void) {
  Actor_SetModeById(11, 2);
  Actor_SetModeById(13, 3);
  Actor_SetModeById(14, 3);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(16, 3);
  Actor_SetModeById(9, 0);
  Actor_SetSpritePriority(9, 2);
  Actor_GetById(9)->priorityFlags &= ~2;
  Actor_SetPositionAndResetMotion_(Actor_GetById(9), PIXELS(216), PIXELS(8),
                                   PIXELS(216));
  if (GameFlag_Test(0x208)) {
    Actor_SetModeById(9, 4);
    Actor_SetPositionAndResetMotion_(
        Actor_GetById(gMainRoutineWork.currentOwner), PIXELS(216), PIXELS(12),
        PIXELS(216));
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
  Actor_GetById(11)->scaleY = 0x10000;
  Actor_SetPositionAndResetMotion_(Actor_GetById(11), PIXELS(216), 0,
                                   PIXELS(216));
  Func_02000c90(13);
  Func_02000c90(14);
  if (GameFlag_Test(0x301)) {
    Func_02000c90(15);
    Func_02000c90(16);
  }
  gData_02006298 = 1;
  Event_SetFlag(0x20c);
}

void Func_02000e60(void) {
  Actor_SetModeById(12, 2);
  Actor_SetModeById(17, 3);
  Actor_SetModeById(18, 3);
  Actor_SetModeById(19, 3);
  Actor_SetModeById(20, 3);
  Actor_SetModeById(10, 0);
  Actor_SetSpritePriority(10, 2);
  Actor_GetById(10)->priorityFlags &= ~2;
  Actor_SetPositionAndResetMotion_(Actor_GetById(10), PIXELS(376), PIXELS(8),
                                   PIXELS(248));
  if (GameFlag_Test(0x209)) {
    Actor_SetModeById(10, 4);
    Actor_SetPositionAndResetMotion_(
        Actor_GetById(gMainRoutineWork.currentOwner), PIXELS(376), PIXELS(12),
        PIXELS(248));
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
  Actor_GetById(12)->scaleY = 0x10000;
  Actor_SetPositionAndResetMotion_(Actor_GetById(12), PIXELS(376), 0,
                                   PIXELS(248));
  Func_02000cb4(17);
  Func_02000cb4(18);
  if (GameFlag_Test(0x300)) {
    Func_02000cb4(19);
    Func_02000cb4(20);
  }
  gData_0200629c = 1;
  Event_SetFlag(0x20d);
}

void Func_02000f60(void) {
  s32 *timer = &gData_020062a0;
  struct Actor *statue;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 timerEnd;
  s32 t;
  s32 flag;

  if (*timer < 300) {
    if (Func_02000d18() != 0)
      return;
    goto count;
  }
  if (*timer == 300) {
    MapGrid_CopyCellAttributes(15, 15, 1, 1, 13, 13);
    MapGrid_CopyCellAttributes(65, 15, 1, 1, 63, 13);
    Func_02000d64();
    goto count;
  }
  timerEnd = 2999;
  if (*timer <= timerEnd) {
    statue = Actor_GetById(9);
    flag = GameFlag_Test(0x301);
    t = *timer;
    if (flag) {
      if (t <= 316) {
        statue->y += 0x48000;
        limit = 0x1000;
        Func_02000cd8(11, 0x20000, limit);
        Func_02000cd8(13, 0x18000, limit);
        Func_02000cd8(14, 0x10000, limit);
        Func_02000cd8(15, 0x8000, limit);
        Func_02000cd8(16, 0, limit);
      } else if (t < 600) {
        if (GameFlag_Test(0x208) && *timer == 400) {
          MapGrid_CopyCellAttributes(22, 0, 1, 1, 13, 13);
          Task_RemoveFar(Func_02000f60);
        }
        phase = &gData_020062a8;
        statue->y += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else if (t <= 999) {
        phase = &gData_020062a8;
        if (*phase == 0) {
          gData_02006298 = 0;
          *timer = 999;
          GameFlag_ClearBit(0x20c);
        }
        statue->y += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else {
        statue->y += -0x6c000;
        limit = 0x1800;
        Func_02000cf8(11, 0x30000, limit);
        Func_02000cf8(13, 0x24000, limit);
        Func_02000cf8(14, 0x18000, limit);
        Func_02000cf8(15, 0xc000, limit);
        Func_02000cf8(16, 0, limit);
        if (Actor_GetById(16)->scaleY <= limit)
          *timer = timerEnd;
      }
    } else {
      if (t <= 316) {
        statue->y += 0x27999;
        limit = 0x1000;
        Func_02000cd8(11, 0x10000, limit);
        Func_02000cd8(13, 0x8000, limit);
        Func_02000cd8(14, 0, limit);
      } else if (t < 600) {
        phase = &gData_020062a8;
        statue->y += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else if (t <= 999) {
        phase = &gData_020062a8;
        if (*phase == 0) {
          gData_02006298 = 0;
          *timer = 999;
          GameFlag_ClearBit(0x20c);
        }
        statue->y += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else {
        statue->y += -0x3b666;
        limit = 0x1800;
        Func_02000cf8(11, 0x18000, limit);
        Func_02000cf8(13, 0xc000, limit);
        Func_02000cf8(14, 0, limit);
        if (Actor_GetById(14)->scaleY <= limit)
          *timer = timerEnd;
      }
    }
    if (GameFlag_Test(0x208) == 0)
      goto count;
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    leader->y = statue->y + 0x40000;
    leader->targetY = 0x80000000;
    leader->velocityY = 0;
    goto count;
  }
  if (*timer == 3000) {
    MapGrid_CopyCellAttributes(8, 0, 1, 1, 13, 13);
    MapGrid_CopyCellAttributes(58, 0, 1, 1, 63, 13);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
    Actor_SetModeById(9, 1);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(9), 0xd80000, 0,
                                         0xd80000);
    Actor_GetById(9)->priorityFlags |= 2;
    Actor_SetSpritePriority(9, 3);
    if (GameFlag_Test(0x208)) {
      leader = Actor_GetById(gMainRoutineWork.currentOwner);
      Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
      Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
    }
    goto count;
  }
  {
    s32 none = Func_02000d18();

    if (none != 0)
      return;
    *timer += (Random16Far() << 7) >> 16;
    if ((u32)*timer > ((Random16Far() * 10000) >> 16) + 10000)
      *timer = none;
  }
count:
  gData_020062a0++;
}

// Bob the statue on a sine.
#define BOB(statue, phase)                                                     \
  do {                                                                         \
    (statue)->y += Trig_Sin(*(phase)) >> 1;                                    \
    *(phase) += 0x1000;                                                        \
  } while (0)

void Func_020012c4(void) {
  s32 *timer = &gData_020062a4;
  struct Actor *statue;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 raised;
  s32 last;
  s32 t;

  t = *timer;
  if (t < 300) {
    if (Func_02000d3c() != 0)
      return;
  } else if (t == 300) {
    MapGrid_CopyCellAttributes(15, 15, 1, 1, 23, 15);
    MapGrid_CopyCellAttributes(65, 15, 1, 1, 73, 15);
    Func_02000e60();
  } else {
    last = 2999;
    if (t <= last) {
      statue = Actor_GetById(10);
      raised = GameFlag_Test(0x300);
      t = *timer;
      if (raised) {
        if (t <= 316) {
          statue->y += 0x48000;
          limit = 0x1000;
          Func_02000cd8(12, 0x20000, limit);
          Func_02000cd8(17, 0x18000, limit);
          Func_02000cd8(18, 0x10000, limit);
          Func_02000cd8(19, 0x8000, limit);
          Func_02000cd8(20, 0, limit);
        } else if (t < 600) {
          if (GameFlag_Test(0x209) && *timer == 400) {
            MapGrid_CopyCellAttributes(23, 0, 1, 1, 23, 15);
            Task_RemoveFar(Func_020012c4);
          }
          phase = &gData_020062aa;
          BOB(statue, phase);
        } else if (t <= 999) {
          phase = &gData_020062aa;
          if (*phase == 0) {
            gData_0200629c = 0;
            *timer = 999;
            GameFlag_ClearBit(0x20d);
          }
          BOB(statue, phase);
        } else {
          statue->y += -0x6c000;
          limit = 0x1800;
          Func_02000cf8(12, 0x30000, limit);
          Func_02000cf8(17, 0x24000, limit);
          Func_02000cf8(18, 0x18000, limit);
          Func_02000cf8(19, 0xc000, limit);
          Func_02000cf8(20, 0, limit);
          if (Actor_GetById(20)->scaleY <= limit)
            *timer = last;
        }
      } else {
        if (t <= 316) {
          statue->y += 0x27999;
          limit = 0x1000;
          Func_02000cd8(12, 0x10000, limit);
          Func_02000cd8(17, 0x8000, limit);
          Func_02000cd8(18, 0, limit);
        } else if (t < 600) {
          phase = &gData_020062aa;
          BOB(statue, phase);
        } else if (t <= 999) {
          phase = &gData_020062aa;
          if (*phase == 0) {
            gData_0200629c = 0;
            *timer = 999;
            GameFlag_ClearBit(0x20d);
          }
          BOB(statue, phase);
        } else {
          statue->y += -0x3b666;
          limit = 0x1800;
          Func_02000cf8(12, 0x18000, limit);
          Func_02000cf8(17, 0xc000, limit);
          Func_02000cf8(18, 0, limit);
          if (Actor_GetById(18)->scaleY <= limit)
            *timer = last;
        }
      }
      if (GameFlag_Test(0x209)) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        leader->y = statue->y + 0x40000;
        leader->targetY = 0x80000000;
        leader->velocityY = 0;
      }
    } else if (t == 3000) {
      MapGrid_CopyCellAttributes(8, 0, 1, 1, 23, 15);
      MapGrid_CopyCellAttributes(58, 0, 1, 1, 73, 15);
      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
      Actor_SetModeById(10, 1);
      FieldActor_SetPositionAndResetMotion(Actor_GetById(10), 0x1780000, 0,
                                           0xf80000);
      Actor_GetById(10)->priorityFlags |= 2;
      Actor_SetSpritePriority(10, 3);
      if (GameFlag_Test(0x209)) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
        Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
      }
    } else {
      s32 none = Func_02000d3c();

      if (none != 0)
        return;
      *timer += (Random16Far() << 7) >> 16;
      if ((u32)*timer > ((Random16Far() * 10000) >> 16) + 10000)
        *timer = none;
    }
  }
  gData_020062a4++;
}

void Func_02001634(void) {
  struct Actor *lift = Actor_GetById(13);
  struct Actor *base = Actor_GetById(15);
  s32 *timer = &gData_020062a0;

  if (*timer == 300) {
    Scene_QueueMusic(162);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(13), PIXELS(440),
                                         PIXELS(-20), PIXELS(204));
    Actor_SetPositionAndResetMotion(Actor_GetById(15), PIXELS(440), PIXELS(-20),
                                    PIXELS(200));
    Actor_SetSpritePriority(13, 3);
    Actor_SetSpritePriority(15, 3);
    base->priorityFlags |= 2;
    base->scaleY = 0x400;
    Actor_SetModeById(13, 0);
    Actor_SetModeById(15, 2);
  } else if ((u32)(*timer - 301) <= 1698) {
    lift->y += PIXELS(4);
    Func_02000cd8(15, 0x4ccc, 0x4ccc);
    // y read as long, another alias set than the reads around it: GCSE then
    // keeps the reload after this call; as lift->y, 2 instructions move.
    if (*(long *)&lift->y > -0x50000)
      Actor_SetSpritePriority(13, 2);
    if (lift->y >= 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 27, 12);
      *timer = 2000;
    }
  } else if (*timer < 2800) {
    u16 *phase = &gData_020062a8;

    lift->y += Trig_Sin(*phase) >> 1;
    *phase += 0x1000;
    if (*timer == 2280)
      MapGrid_CopyCellAttributes(27, 10, 1, 1, 27, 12);
    if (*timer > 2300 && *phase == 0) {
      if (GameFlag_Test(0x208)) {
        MapGrid_CopyCellAttributes(21, 0, 1, 1, 27, 12);
        Task_RemoveFar(Func_02001634);
      } else {
        *timer = 2800;
      }
    }
  } else {
    lift->y += PIXELS(-4);
    Func_02000cf8(15, 0x4ccc, 0x4ccc);
    // y read as long, as above.
    if (*(long *)&lift->y < -0x50000)
      Actor_SetSpritePriority(13, 3);
    if (lift->y <= -0x140000) {

      /* FAKEMATCH(temporary): the timer pointer's own address taken, which
       * orders the stack arguments of the 2280 call as the native does; as
       * *timer = 0, 6 of 193 instructions differ in tla-en. */
      s32 **timerRef = &timer;

      Scene_QueueMusic(245);
      Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
      **timerRef = 0;
    }
  }
  gData_020062a0++;
}

void Func_02001804(void) {
  struct Actor *lift = Actor_GetById(14);
  struct Actor *base = Actor_GetById(16);
  s32 *timer = &gData_020062a4;

  if (*timer == 300) {
    Scene_QueueMusic(162);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(14), PIXELS(600),
                                         PIXELS(-20), PIXELS(236));
    Actor_SetPositionAndResetMotion(Actor_GetById(16), PIXELS(600), PIXELS(-20),
                                    PIXELS(232));
    Actor_SetSpritePriority(14, 3);
    Actor_SetSpritePriority(16, 3);
    base->priorityFlags |= 2;
    base->scaleY = 0x400;
    Actor_SetModeById(14, 0);
    Actor_SetModeById(16, 2);
  } else if ((u32)(*timer - 301) <= 1698) {
    lift->y += PIXELS(4);
    Func_02000cd8(16, 0x4ccc, 0x4ccc);
    // y read as long, another alias set than the reads around it: GCSE then
    // keeps the reload after this call; as lift->y, 2 instructions move.
    if (*(long *)&lift->y > -0x50000)
      Actor_SetSpritePriority(14, 2);
    if (lift->y >= 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 37, 14);
      *timer = 2000;
    }
  } else if (*timer < 2800) {
    u16 *phase = &gData_020062aa;

    lift->y += Trig_Sin(*phase) >> 1;
    *phase += 0x1000;
    if (*timer == 2280)
      MapGrid_CopyCellAttributes(37, 12, 1, 1, 37, 14);
    if (*timer > 2300 && *phase == 0) {
      if (GameFlag_Test(0x209)) {
        MapGrid_CopyCellAttributes(22, 0, 1, 1, 37, 14);
        Task_RemoveFar(Func_02001804);
      } else {
        *timer = 2800;
      }
    }
  } else {
    lift->y += PIXELS(-4);
    Func_02000cf8(16, 0x4ccc, 0x4ccc);
    // y read as long, as above.
    if (*(long *)&lift->y < -0x50000)
      Actor_SetSpritePriority(14, 3);
    if (lift->y <= -0x140000) {

      /* FAKEMATCH(temporary): the timer pointer's own address taken, which
       * orders the stack arguments of the 2280 call as the native does; as
       * *timer = 0, 6 of 193 instructions differ in tla-en. */
      s32 **timerRef = &timer;

      Scene_QueueMusic(245);
      Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
      **timerRef = 0;
    }
  }
  gData_020062a4++;
}

void Func_020019d4(void) {
  struct Actor *leader;
  s32 cellX, cellZ, onCell;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  cellX = leader->x >> 20;
  cellZ = leader->z >> 20;
  onCell = 0;
  if (cellX == 13 && cellZ == 13) {
    Event_SetFlag(0x208);
    onCell = 1;
  } else {
    Event_ClearFlag(0x208);
  }
  if (cellX == 23 && cellZ == 15) {
    Event_SetFlag(0x209);
    onCell = 1;
  } else {
    Event_ClearFlag(0x209);
  }
  if (onCell)
    Event_SetFlag(0x17d);
  else
    Event_ClearFlag(0x17d);
  if (gData_02006298 != 0 && leader->y == 0 && leader->x > 0x11c0000 &&
      leader->x < 0x1340000 && leader->z < 0xc80000 && !Event_GetFlag(0x20b))
    Func_02000778(leader, 0x26666);
  if (gData_0200629c != 0 && leader->y == 0 && leader->x > 0x14a0000 &&
      leader->x <= 0x165ffff && leader->z < 0xd80000 && !Event_GetFlag(0x20b))
    Func_02000778(leader, 0x39999);
  if (Event_GetFlag(0x20e)) {
    if (!Event_GetFlag(0x20c) && !Event_GetFlag(0x20d)) {
      Scene_QueueMusic(-1);
      Event_ClearFlag(0x20e);
    }
  } else if (Event_GetFlag(0x20c) || Event_GetFlag(0x20d)) {
    Scene_QueueMusic(162);
    Event_SetFlag(0x20e);
  }
}

void Func_02001b50(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x;
  s32 cellX = x >> 20;
  s32 z = leader->z;
  s32 cellZ = z >> 20;
  s32 riding = 0;

  if (leader->y > 0x100000)
    return;
  if ((u32)(x - 0x1900000) > 0xf00000)
    return;
  if ((u32)(z - 0xb40000) > 0x640000)
    return;
  if (cellX == 27 && cellZ == 12) {
    struct Actor *lift = Actor_GetById(13);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    Actor_SetModeById(13, 4);
    leader->motionFlags = riding;
    leader->y = lift->y;
    GameFlag_SetBit(0x208);
    riding = 1;
  }
  if (cellX == 37 && cellZ == 14) {
    struct Actor *lift = Actor_GetById(14);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    Actor_SetModeById(14, 4);
    leader->motionFlags = 0;
    leader->y = lift->y;
    GameFlag_SetBit(0x209);
    riding = 1;
  }
  if (riding) {
    GameFlag_SetBit(0x17d);
    return;
  }
  Actor_SetModeById(13, 0);
  Actor_SetModeById(14, 0);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  leader->motionFlags = 3;
  GameFlag_ClearBit(0x208);
  GameFlag_ClearBit(0x209);
  GameFlag_ClearBit(0x17d);
}

/* VIEW(kandorean_temple): tag struct MapCell as struct
 * MapCell__kandorean_temple; map.h has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/kandorean_temple.c:1106: arithmetic on pointer to an
 * incomplete type. */
struct MapCell__kandorean_temple {
  u8 unk0[2];
  u8 unk2;
  u8 unk3;
};

struct MapWork170 {
  u8 unk000[0x170];
  struct MapCell__kandorean_temple *cells;
};

void Func_02001c8c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 priority = ((struct FieldSprite *)leader->sprite)->priority;

  Actor_SetSpritePriority(13, priority);
  Actor_SetSpritePriority(14, priority);
  Actor_SetSpritePriority(15, priority);
}

void Func_02001cc4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork170 *map = (struct MapWork170 *)Ram_HeapSlots->mapWork;

  struct MapCell__kandorean_temple *cell = &map->cells[Func_020053b0()];

  if (cell->unk2 != 0) {
    leader->y = 0;
    leader->terrainHeight = 0;
  }
}

void Func_02001d00(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgKandoreanTemple10, 1);
  Event_End();
}

void Func_02001d20(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgKandoreanTemple01, 1);
  Event_End();
}

void Func_02001d40(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(9);
  wait(1);
  Actor_FaceActorById(9, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple02);
  Event_AskYesNoAndShowResponse(9, 0);
  Actor_GetById(9)->facing = FACING_SOUTHEAST + FACING_STEP;
  wait(1);
  Actor_SetMovementTypeById(9, gData_02005514);
  wait(1);
  Event_End();
  GameFlag_SetBit(0x845);
}

void Func_02001db4(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(10, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple03);
  Event_ShowMessageAndWait(10, 0);
  Actor_GetById(10)->facing = FACING_SOUTH + FACING_STEP;
  wait(1);
  Event_End();
  GameFlag_SetBit(0x846);
}

void Func_02001e0c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgKandoreanTemple09);
  Event_AskYesNoAndShowResponse(13, 0);
  Event_End();
}

void Func_02001e30(void) {
  Actor_ShowEmoteById(9, 0x100, 0);
  Func_02001d40();
}

void Func_02001e44(void) {
  Actor_ShowEmoteById(10, 0x100, 0);
  Func_02001db4();
}

void Func_02001e58(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  struct MainRoutineWork *party;
  u8 *flags;
  s32 zero;

  if (z == 5) {
    if (x == 9 && (gInput.held & 16) == 0)
      return;
    if (x == 11 && (gInput.held & 32) == 0)
      return;
  } else if (x == 10 && z == 6 && (gInput.held & 64) == 0) {
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(temporary): the flags address and the zero held in locals, with
   * gMainRoutineWork's address taken in the block below, so the byte store
   * follows the address load; as leader->motionFlags = 0, which stores first,
   * 2 of 92 instructions differ in tla-en. */
  flags = &leader->motionFlags;
  zero = 0;
  party = &gMainRoutineWork;
  *flags = zero;
  Actor_SetSpeedParameters(party->currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWaitById(party->currentOwner, 168, 80);
  Actor_FaceDirectionById(party->currentOwner, FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(party->currentOwner), 0);
  Actor_SetModeById(party->currentOwner, 13);
  Actor_SetMoveTarget(leader, PIXELS(168), PIXELS(-12), PIXELS(80));
  Actor_WaitForMove(leader);
  Actor_SetModeById(party->currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(8);
}

void Func_02001f3c(void) {
  MapGrid_CopyCellPictureIndicesRect(39, 39, 24, 38, 2, 2);
  wait(10);
  MapGrid_CopyCellPictureIndicesRect(39, 41, 24, 38, 2, 2);
  wait(20);
  GameFlag_SetBit(0x212);
}

void Func_02001f7c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 400, 648);
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_NORTH);
  if (GameFlag_Test(0x212) == 0) {
    Sound_Play(188);
    Func_02001f3c();
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_WalkToById(gMainRoutineWork.currentOwner, 400, 640);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(9);
}

void Func_02002014(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 416, 288);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  leader->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  for (i = 0; i < 8; i++) {
    leader->z += PIXELS(1);
    wait(1);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  for (i = 0; i < 8; i++) {
    leader->y -= PIXELS(1);
    wait(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(4);
}

void Func_020020c8(void) { Func_020053c0(gData_020054dc); }

void Func_020020d8(void) {
  s32 x = Actor_GetById(12)->x >> 20;

  Func_020053c8();
  if (x == 43)
    GameFlag_SetBit(0x211);
  else
    GameFlag_ClearBit(0x211);
  if (x != 39)
    MapGrid_CopyCellAttributes(39, 0, 1, 1, 39, 10);
}

void Func_02002120(void) {
  if ((u32)((Actor_GetById(8)->x >> 20) - 15) > 6)
    GameFlag_SetBit(0x20a);
  GameFlag_SetBit(0x20b);
  Func_020053c0(gData_020054e0);
}

void Func_02002154(void) {
  struct Actor *object = Actor_GetById(8);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  Func_020053c8();
  if (x == 13 && z == 13)
    GameFlag_SetBit(0x300);
  else
    GameFlag_ClearBit(0x300);
  if (x == 23 && z == 15)
    GameFlag_SetBit(0x301);
  else
    GameFlag_ClearBit(0x301);
  GameFlag_ClearBit(0x20a);
  GameFlag_ClearBit(0x20b);
}

void Func_020021b8(void) {
  GameFlag_ClearBit(0x302);
  GameFlag_ClearBit(0x303);
  Func_020053c0(gData_020054e0);
}

void Func_020021dc(void) {
  s32 x = Actor_GetById(8)->x >> 20;

  Func_020053c8();
  if (x == 19) {
    GameFlag_SetBit(0x302);
    GameFlag_SetBit(0x304);
    Map_CopyCellAttributeRect(0, 0, 3, 3, x, 28);
  } else if (x == 21) {
    GameFlag_SetBit(0x303);
    GameFlag_SetBit(0x304);
    MapGrid_CopyCellAttributes(0, 4, 3, 3, 19, 28);
  }
}

void Func_02002248(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner, 0x100, 20);
  Sound_Play(132);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 10);
  leader->velocityY = PIXELS(6);
  wait(20);
  while (leader->velocityY != 0)
    wait(1);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Event_End();
}

void Func_020022c0(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(188);
  MapGrid_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
  MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  wait(10);
  GameFlag_SetBit(0x212);
  Event_End();
}

void Func_02002314(void) {
  struct Actor *object;

  MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  GameFlag_SetBit(0x842);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(4, 89, 237);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Actor_LaunchFromLeader(14, 16, 16, FACING_NORTHWEST);
  Actor_WaitForMoveById(14);
  Event_Wait(20);
  Event_SetMessageId((s32)&MsgKandoreanTemple12);
  Event_ShowMessageAndWait(14, 0);
  Event_Wait(30);
  Actor_SetModeById(14, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_SetSpeed(14, 0x13333, 0x9999);
  Actor_SetModeById(14, 2);
  object = Actor_GetById(4);
  if (object)
    Actor_SetDestinationById(14, *(s16 *)((u8 *)object + 10),
                             *(s16 *)((u8 *)object + 18));
  Actor_WaitForMoveById(14);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Event_End();
}

void Func_020023e8(void) {
  s16 angle = *(s16 *)&Actor_GetById(gMainRoutineWork.currentOwner)->facing;
  struct Actor *object;
  struct FieldSprite *sprite;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  ((void (*)())Actor_ArmCallback)(12, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple08);
  Event_ShowMessage(0x800c, 0);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 12, 0);
  Actor_SetAnimation(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimation(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, (u16)angle, 10);
  if (gData_020062ac != 0)
    ItemDisplay_ShowPicture(gData_020062ac, 3);
  Party_GiveItem(198, 0);
  if (gData_020062ac != 0)
    Import_ActorRelease(gData_020062ac);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(0x800c, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(0x1a80000, 0, 0x1900000, 1);
  object = Actor_GetById(12);
  sprite = object->sprite;
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 424, 376);
  FieldActor_SetSpritePriority(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  FieldActor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 444, 376);
  Actor_SetAnimation(12, 7);
  object->scaleX = 0xffff0000;
  sprite->rotation = 0xe000;
  object->motionFlags = 0;
  object->y += 0x40000;
  Actor_SetSpriteFlags(Actor_GetById(12), 0);
  Actor_SetSpeed(12, 0x6666, 0x3333);
  FieldActor_SetMoveTarget(object, 0x1ec0000, 0x80000, 0x1780000);
  FieldActor_WaitForMoveById(12);
  object->motionFlags = 3;
  object->terrainHeight = 0;
  object->scaleX = 0x10000;
  sprite->rotation = 0;
  Actor_SetAnimation(12, 1);
  Actor_SetSpriteFlags(Actor_GetById(12), 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  FieldActor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 504, 376);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 496, 394);
  Actor_WalkToAndWait(12, 432, 394);
  FieldActor_WatchAndSetScript(12, gMainRoutineWork.currentOwner);
  FieldActor_SetSpritePriority(12, 2);
  Event_SetFlag(0x84a);
  Event_End();
}

void Func_02002674(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
}

void Func_0200269c(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
}

void Func_020026cc(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveWithinMapBounds(PIXELS(328), -1, PIXELS(174), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 328, 174);
  Motion_SetVarCbAndRefresh(11, 2);
  Sound_Play(146);
  Actor_SetModeById(11, 2);
  Actor_GetById(11)->velocityY = PIXELS(4);
  Event_Wait(10);
  Sound_Play(146);
  Actor_SetModeById(12, 2);
  Actor_SetModeById(13, 2);
  Actor_GetById(12)->velocityY = PIXELS(6);
  Actor_GetById(13)->velocityY = PIXELS(6);
  Actor_SetDestinationById(12, 300, 164);
  Actor_MoveToAndWaitById(13, 356, 164);
  Event_Wait(40);
  /* FAKEMATCH(dead): an empty once-only block, whose loop note ends the
   * scheduling block before the battle rule is set, so the last position
   * call's r1 is shifted before r2 is loaded; without it, 2 of 87
   * instructions differ in tla-en. */
  do {
  } while (0);
  gMainRoutineWork.battleRule24b = 2;
  Event_SetAfterBattleDestination((s32)&SceneId_Ov653Room3, 3);
  Field_RequestBattleFromEncounterTable(9, 4);
  Event_End();
}

void Func_020027b4(void) { Func_020053d0_(76, 0, 2, 2, 73, 14, 1); }

void Func_020027d4(void) { Func_020053d0_(76, 0, 1, 1, 75, 16, 1); }

void Func_020027f4(void) { Func_020053d0_(76, 0, 1, 1, 74, 18, 1); }

void Func_02002814(void) { Func_020053d0_(76, 0, 1, 1, 80, 18, 1); }

void Func_02002834(void) { Func_020053d0_(76, 0, 1, 1, 87, 15, 1); }

s32 Scene_Initialize(void) {
  s32 scene;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x204;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov653Room1)
    Func_0200290c();
  else if (scene == (s32)&SceneId_Ov653Room2)
    Func_02002a5c();
  else if (scene == (s32)&SceneId_Ov653Room3)
    Func_02002a84();
  else if (scene == (s32)&SceneId_Ov653Room4)
    Func_02002c1c();
  else if (scene == (s32)&SceneId_Ov653Room5)
    Func_02003a14();
  else if (scene == (s32)&SceneId_Ov653Room6)
    Func_02003a5c();
  else if (scene == (s32)&SceneId_Ov653Room7)
    Func_02003a8c();
  else if (scene == (s32)&SceneId_Ov653Room8)
    Func_02003b2c();
  else if (scene == (s32)&SceneId_Ov653Room9)
    Func_02003c8c();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200290c(void) {
  Func_02005398();
  Func_020053a0(0, 8, 9);
  Func_020053a0(1, 10, 11);
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 4);
  Actor_SetModeById(10, 4);
  Actor_SetModeById(11, 4);
  Func_020053b8(gData_020054dc);
  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = 0x3f10;
  Scene_QueueMusic(245);
  Task_Add(Func_02000c74, TASK_PASS_VBLANK);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Func_02000b78(13);
  Func_02000b78(14);
  Func_02000bac(15);
  Func_02000bac(16);
  gData_020062a0 = 0;
  gData_020062a4 = 0;
  gData_020062a8 = 0;
  gData_020062aa = 0;
  if (GameFlag_Test(0x109) == 0) {
    if (gMainRoutineWork.entrance == 5)
      Func_02003684();
    else if (gMainRoutineWork.entrance == 6)
      Func_020037e0();
  }
  if (GameFlag_Test(0x301))
    Task_Add(Func_02001634, TASK_PASS_VBLANK);
  else if (GameFlag_Test(0x300))
    Task_Add(Func_02001804, TASK_PASS_VBLANK);
  Task_Add(Func_02001b50, TASK_PASS_VBLANK);
  Func_02005338(0);
}

void Func_02002a5c(void) {
  Func_02005338(0);
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance == 1)
    GameFlag_ClearBit(0x12f);
#endif
}

void Func_02002a84(void) {
  struct Actor *object = Actor_GetById(9);

  Func_020052c0(9);
  object->priorityFlags |= 2;
  Actor_SetModeById(9, 2);
  object = Actor_GetById(8);
  object->motionFlags = 0;
  if (GameFlag_Test(0x302)) {
    Actor_SetPositionAndResetMotion_(Actor_GetById(8), 0x1380000, 0x200000,
                                     0x1d80000);
    MapGrid_CopyCellAttributes(0, 0, 3, 3, 19, 28);
  } else if (GameFlag_Test(0x303)) {
    Actor_SetPositionAndResetMotion_(Actor_GetById(8), 0x1580000, 0x200000,
                                     0x1d80000);
    MapGrid_CopyCellAttributes(0, 4, 3, 3, 19, 28);
  }
  Func_020053b8(gData_020054e0);
  object = Actor_GetById(10);
  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->collisionFlags = 0;
  Func_02005338(0);
  if (Event_GetFlag(0x92d)) {
    {
      s32 e = 1;
      s32 f = 4;

      MapGrid_CopyCellPictureIndicesRect(41, 0, 20, 0, e, f);
    }
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  }
  if (GameFlag_Test(0x109) == 0) {
    if (gMainRoutineWork.entrance == 2) {
      Actor_GetById(gMainRoutineWork.currentOwner)->y = 0x400000;
    } else if (gMainRoutineWork.entrance == 3 && Event_GetFlag(0x92d) == 0) {
      Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
      Func_02005028();
    }
  }
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(0x12f);
#endif
}

void Func_02002ec4();
void Func_0200306c();
void Func_020032c8();

void Func_02002c1c(void) {
  s32 t1, t2, t9, t19, t21, t39, t44, p_r3;

  t1 = GameFlag_Test(0x109);
  if (t1 == 0)
    goto L_0200acd2;
  t2 = ((s32)Actor_GetById(9));
  FieldActor_SetPositionAndResetMotion(t2, PIXELS(216), 0, PIXELS(216));
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  t9 = ((s32)Actor_GetById(10));
  Actor_SetPositionAndResetMotion(t9, PIXELS(376), 0, PIXELS(248));
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  GameFlag_ClearBit(0x20e);
  GameFlag_ClearBit(0x20c);
  GameFlag_ClearBit(0x20d);
L_0200acd2:
  t19 = GameFlag_Test(0x300);
  if (t19 == 0)
    goto L_0200acee;
  Actor_SetPosition(8, PIXELS(216), PIXELS(216));
  goto L_0200ad0a;
L_0200acee:
  t21 = GameFlag_Test(0x301);
  if (t21 == 0)
    goto L_0200ad0a;
  Actor_SetPosition(8, PIXELS(376), PIXELS(248));
L_0200ad0a:
  (*(s16 *)0x4000052) = 0xc08;
  (*(s16 *)0x4000050) = 0x3f10;
  gData_02006298 = 0;
  gData_0200629c = 0;
  Task_AddFrame(Func_02000bf4, 0x480);
  Task_AddFrame(Func_02000c34, 0x480);
  Task_Add((s32)Func_02001cc4, 0x480);
  Func_02000b78(9);
  Func_02000b78(10);
  Func_02000bac(11);
  Func_02000bac(13);
  Func_02000bac(14);
  Func_02000bac(15);
  Func_02000bac(16);
  Func_02000bac(12);
  Func_02000bac(17);
  Func_02000bac(18);
  Func_02000bac(19);
  Func_02000bac(20);
  Func_020053b8(gData_020054e0);
  t39 = GameFlag_Test(0x109);
  if (t39 != 0)
    goto L_0200adda;
  if (gMainRoutineWork.entrance != 3) {
    p_r3 = gMainRoutineWork.entrance;
    goto L_0200add2;
  }
  Func_0200306c();
  goto L_0200adda;
L_0200add2:
  if (p_r3 != 4)
    goto L_0200adda;
  Func_020032c8();
L_0200adda:
  gData_020062a0 = 0;
  gData_020062a4 = 0xfa0;
  gData_020062a8 = 0;
  gData_020062aa = 0;
  Task_AddFrame(Func_02000f60, 0x480);
  Task_Add((s32)Func_020012c4, 0x480);
  t44 = GameFlag_Test(0x109);
  if (t44 != 0)
    goto L_0200ae24;
  if (gMainRoutineWork.entrance != 5)
    goto L_0200ae24;
  Func_02002ec4();
L_0200ae24:
  Task_AddFrame(Func_020019d4, 0x480);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Func_02005338(0);
}
