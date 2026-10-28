#include "shrine_of_the_sea_god.h"
#include "scene_ids.h"

struct ShrineOfTheSeaGodSpot {
  s32 x;
  s32 z;
  s32 field18;
  s32 rotation;
};

extern const struct ShrineOfTheSeaGodSpot gData_02003cc0[];
extern const u8 gData_02003cbc[];
extern const u8 gData_02003c10[];

void Actor_SetPartAttribute(struct ShrineOfTheSeaGodObject *object, s32 value);
void Func_02003b00(s32 value);
void Func_02003b18(void);
void Func_02003b20(s32 a, s32 b, s32 c, s32 d);
void Func_02003b30(const void *script);
void Func_02003854(s32 actor);
void Func_0200123c(void);
void Func_020010e8(void);
void Func_02001088(void);
void Func_02001128(void);
void Func_020011a0(void);
void Func_02000f18(s32 actor);
void Func_020027f8(void);
void Func_02002830(void);
void Func_0200291c(void);
void Func_0200296c(void);
void Func_020029a4(void);
void Func_020029dc(void);
void Func_02002a2c(void);
void Func_02002b18(void);
void Func_02002b50(void);
void Func_02002c18(void);
void Func_02002c50(void);
void Func_02002c88(void);
void Func_02003094(void);
void Func_02003134(void);
void Func_0200320c(void);
void Func_02003354(void);
void Func_0200338c(void);
void Func_02003504(void);
void Func_02003598(void);
void Func_02003740(void);
void Func_0200377c(void);
void Func_0200380c(void);

s32 Func_02002ec4(void) {
  s16 scene;

  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x204;
  scene = gAdventure.scene;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
    Func_02003094();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
    Func_02003134();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
    Func_0200320c();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
    Func_02003354();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
    Func_0200338c();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    Func_02003504();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
    Func_02003598();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
    Func_02003740();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
    Func_0200377c();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod11)
    Func_0200380c();
  return 0;
}

struct ShrineOfTheSeaGodObject *Func_02002f88(s32 id) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);

  object->priorityFlags |= 2;
  object->y = 0xc0000;
  Func_02003aa0(id);
  return object;
}

void Func_02002fb0(s32 id) {
  Func_02003aa0(id);
  Actor_SetPartAttribute(Actor_GetById(id), 15);
  Actor_SetSpritePriority(id, 1);
  Actor_GetById(id)->priorityFlags |= 2;
}

void Func_02002fe0(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(0x208) == 0)
      Func_0200123c();
  } else {
    GameFlag_SetBit(0x208);
  }
}

void Func_0200300c(void) {
  u32 i;

  for (i = 0; i <= 2; i++)
    Func_02002fb0(i + 9);
  Func_02002fe0();
}

void Func_02003024(void) {
  const struct ShrineOfTheSeaGodSpot *spot = gData_02003cc0;
  u32 i;

  /* FAKEMATCH: loads the spot table before it clears the counter. */
  __asm__("");
  for (i = 0; i <= 15; i++) {
    s32 id = i + 8;
    struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);
    struct ShrineOfTheSeaGodSprite *sprite;

    Func_02003aa0(id);
    Actor_SetModeById(id, 0);
    Actor_SetSpritePriority(id, 3);
    object->priorityFlags |= 2;
    Actor_SetPartAttribute(Actor_GetById(id), 12);
    sprite = object->sprite;
    object->x = spot->x;
    object->z = spot->z;
    object->field18 = spot->field18;
    sprite->rotation = spot->rotation;
    spot++;
  }
}

void Func_02003094(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;

  Func_02003854(8);
  Func_02003854(9);
  {
    s32 priority;

    map->mode17 = 1;
    priority = 0x480;
    Task_Add(Func_020010e8, priority);
  }
  Func_02003b18();
  Func_02003b20(0, 515, 10, 11);
  if (GameFlag_Test(0x872)) {
    Func_02003a30(12, 0, 0);
    Func_02003a30(13, 0, 0);
  }
}

void Func_020030fc(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gAdventure.currentOwner);
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(11);
  s32 x = object->x;
  u8 *flags = &object->collisionFlags;

  if (x > leader->x)
    /* FAKEMATCH: the native ties the flag or to the constant's register. */
    *flags = *flags + 8 - (*flags & 8);
  else
    *flags &= ~8;
}

void Func_02003134(void) {
  s32 angle;

  Func_02002f88(10)->y = -0x100000;
  Actor_SetSpritePriority(10, 3);
  Func_02003b30(gData_02003cbc);
  Func_02002e70();
  ShrineOfTheSeaGod_TaskAdd(Func_020011a0, 0x480);
  Func_02003aa0(11);
  Func_02003aa0(12);
  Actor_GetById(12)->motionFlags = 0;
  ShrineOfTheSeaGod_BlendStart(3, 15, 1, 1, 6, 15);
  if (GameFlag_Test(0x873)) {
    Func_02003a30(11, 0, 0);
  } else if (GameFlag_Test(0x872)) {
    /* FAKEMATCH: the function-scope angle temporary preserves its early
     * constant construction; a direct argument through the unbound helper
     * differs 2. */
    angle = 0x8000;
    ShrineOfTheSeaGod_ObjPlaceTurn(11, 328 << 16, 200 << 16, angle);
    wait(1);
    Actor_SetMovementTypeById(11, gData_02003c10);
    Actor_SetSingleChildFlags(Actor_GetById(11), 1);
    Actor_GetById(11)->collisionFlags = 1;
  }
  /* FAKEMATCH: the native builds the priority before it loads the task function. */
  SHRINE_OF_THE_SEA_GOD_TASK_ADD_LATE(Func_020030fc, 0x480);
}

void Func_0200320c(void) {
  struct ShrineOfTheSeaGodObject *block;
  s32 angle;

  Func_02002f88(9);
  Func_02002f88(10);
  Func_02002f88(11);
  Func_02002f88(12);
  Func_02002f88(13);
  block = Func_02002f88(14);
  Actor_SetModeById(9, 0);
  Actor_SetModeById(13, 0);
  Actor_SetModeById(14, 2);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  Actor_SetSpritePriority(11, 1);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Actor_SetSpritePriority(14, 1);
  ShrineOfTheSeaGod_TaskAdd(Func_02001088, 0x480);
  if (GameFlag_Test(0x230)) {
    ShrineOfTheSeaGod_ObjSetPos(block, 408 << 16, -0xd0000, 336 << 16);
    ((struct ShrineOfTheSeaGodSprite *)block->sprite)->rotation = 0x1000;
    Actor_SetSpritePriority(14, 3);
    block->priorityFlags |= 2;
    ShrineOfTheSeaGod_BlendStart(0, 50, 3, 1, 24, 70);
  }
  if (GameFlag_Test(0x84c))
    ShrineOfTheSeaGod_ObjSetPos(Actor_GetById(8), 200 << 16, 0, 296 << 16);
  Func_02003b30(gData_02003cbc);
  if (GameFlag_Test(0x875))
    Func_02003a30(15, 0, 0);
  else if (GameFlag_Test(0x874)) {
    /* FAKEMATCH: the function-scope angle temporary preserves its early
     * constant construction; a direct argument through the unbound helper
     * differs 4. */
    angle = 0x4000;
    ShrineOfTheSeaGod_ObjPlaceTurn(15, 580 << 16, 344 << 16, angle);
  }
  Func_02003b00(0);
  wait(1);
}

void Func_02003354(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;

  if (GameFlag_Test(0x876) == 0)
    Func_02003024();
  {
    s32 priority;

    map->mode17 = 1;
    priority = 0x480;
    Task_Add(Func_020010e8, priority);
  }
}

void Func_0200338c(void) {
  s32 angle;
  struct ShrineOfTheSeaGodObject *object = Func_02002f88(13);
  u8 *flags = &object->priorityFlags;

  if ((u16)(gAdventure.entrance - 2) <= 1) {
    Actor_SetSpritePriority(13, 3);
    /* FAKEMATCH: the native ties the flag or to the constant's register. */
    *flags = *flags + 2 - (*flags & 2);
  } else {
    Actor_SetSpritePriority(13, 1);
    *flags &= ~2;
  }
  Func_02000f18(10);
  Func_02000f18(11);
  Func_02000f18(12);
  if (GameFlag_Test(0x200))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 8, 18);
  if (GameFlag_Test(0x201))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 18);
  if (GameFlag_Test(0x202))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 17);
  ShrineOfTheSeaGod_TaskAdd(Func_02001128, 0x480);
  Func_02003b30(gData_02003cbc);
  Func_02002e70();
  ShrineOfTheSeaGod_TaskAdd(Func_020011a0, 0x480);
  if (GameFlag_Test(115) || GameFlag_Test(0x340)) {
    GameFlag_SetBit(0x205);
  } else if (GameFlag_Test(0x87c)) {
    struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);
    s32 moved = GameFlag_Test(265);

    if (moved == 0) {
      block->state = moved;
      ShrineOfTheSeaGod_ObjPlace(14, 392 << 16, 296 << 16);
    }
    Actor_WatchAndSetScript(14, gAdventure.currentOwner);
  } else if (GameFlag_Test(0x877)) {
    /* FAKEMATCH: the function-scope angle temporary preserves its early
     * constant construction; a direct argument through the unbound helper
     * differs 4. */
    angle = 0x4000;
    ShrineOfTheSeaGod_ObjPlaceTurn(14, 280 << 16, 328 << 16, angle);
  }
}

void Func_02003504(void) {
  s32 open;

  Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 0x20;
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  open = GameFlag_Test(0x301);
  if (open) {
    ShrineOfTheSeaGod_ObjPlace(8, 248 << 16, 264 << 16);
    Actor_SetSpritePriority(8, 3);
    Actor_GetById(8)->motionFlags = 0;
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, 16);
    Func_02003aa0(8);
  } else {
    Actor_SetSpritePriority(8, 1);
    Actor_GetById(8)->motionFlags = open;
  }
  Func_0200300c();
}
