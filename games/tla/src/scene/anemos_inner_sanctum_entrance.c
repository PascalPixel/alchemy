// ---- games/tla/src/scene/anemos_inner_sanctum_entrance/hide.c ----
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

#include "anemos_inner_sanctum_entrance/anemos_inner_sanctum_entrance.h"

extern const u8 gData_020019dc[];
extern const u8 gData_020019f4[];
extern s32 gData_02000e20[];
extern s32 gData_02000e5c[];
extern s32 gData_02000ea4[];
extern u8 MsgIAmTheShadow;

const u8 *Scene_GetPlacements(void) {
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

/* VIEW(hide): variable gData_02000ee0 as extern const s8 gData_02000ee0[]; the definition has u32 gData_02000ee0[] = { 0x1c0b1b0b, 0x1c0c1b0c, 0x1c0d1b0d, }; tried as the definition's extern u32 gData_02000ee0[], Func_020004e4 moves: 1 of 32 instructions in tla-ja. */
extern const s8 gData_02000ee0__hide[] __asm__("gData_02000ee0");

void Func_020004e4(s32 a, s32 b) {
  s32 i;

  const s8 *x = gData_02000ee0__hide;
  const s8 *z = gData_02000ee0__hide + 1;

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

// ---- games/tla/src/scene/anemos_inner_sanctum_entrance/s0588.c ----
#include "version.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

extern s8 gData_02000eec[];

void Func_02000388(void);
/* VIEW(s0588): routine Func_020004b4 as void Func_020004b4(void); the definition has void Func_020004b4(struct Actor *object). */
extern void Func_020004b4__s0588(void) __asm__("Func_020004b4");
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
void Script_WaitForEventTimeout(struct Actor *actor);
s32 GetMapCellCollision(s32 layer, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 a);

static __inline__ void MoveTo(struct Actor *object, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): MoveTo forwarded inline; as direct calls to
   * Actor_SetMoveTarget, 6 of 157 instructions differ in tla-en. */
  Actor_SetMoveTarget(object, x, y, z);
}

void Func_02000588(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 shift = 0x1300000;
  s32 i;

  if (object->z >> 20 <= 9)
    shift = -shift;
  Event_ResolvePendingActions(0);
  if (!GameFlag_Test(0x200))
    return;
  if (GetMapCellCollision(object->terrainId, object->x, object->z) != 77)
    return;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
#if TLA_PATCH_1
  MoveTo(object, (object->x & 0xfff00000) + 0x80000, object->y,
         (object->z & 0xfff00000) + 0x80000);
#endif
  {
    s32 id = gMainRoutineWork.currentOwner;

    if (shift > 0)
      Actor_ArmCallback(id, 0xc000, 0);
    else
      Actor_ArmCallback(id, 0x4000, 0);
  }
  wait(10);
  i = 0;
next:
  {
    object = Actor_GetById(gData_02000eec[i]);
    object->motionFlags = 0;
    object->y += 0x8000;
    object->acceleration = 0x1000;
    object->speedLimit = 0x20000;
    Actor_SetSingleChildFlags(object, 1);
    Actor_SetMoveTarget(object, object->x, 0xfff80000, object->z);
    object->update = Func_020004b4__s0588;
    Sound_Play(152);
    wait((Random16Far() * 4 >> 16) + 3);
  }
  if (++i <= 6)
    goto next;
  wait(40);
  Sound_Play(198);
  i = 9;
lower:
  {
    object = Actor_GetById(i);
    {
      s32 z = object->z - shift;

      Actor_SetMoveTarget(object, object->x, 0xfff80000, z);
    }
  }
  if (++i <= 14)
    goto lower;
  Script_WaitForEventTimeout(Actor_GetById(9));
  wait(40);
  i = 9;
land:
  {
    object = Actor_GetById(i);
    object->z = (object->z & 0xfff00000) + 0x80000;
    object->motionFlags = 3;
    object->update = 0;
  }
  if (++i <= 14)
    goto land;
  wait(3);
  Sound_Play(188);
  wait(10);
  if (!GameFlag_Test(0xa9f) && shift > 0)
    Func_02000388();
}

void Func_02000710(void) {}

extern const u8 gData_02001c1c[];
extern const u8 gData_02001c28[];

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6b1Room1)
    return gData_02001c28;
  return gData_02001c1c;
}

struct AnemosInnerSanctumEntranceEventWork {
  u8 unk000[0x1ac];
  s32 route;
};

void Func_02000408(void);
void Func_02000b18(void);
void Func_02000ba4(s32 id, s32 value);
void Func_02000bd4(u16 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

static __inline__ void SetObjectValue(s32 id, s32 value) {
  /* FAKEMATCH(pin): the value pinned to r1, which the ROM loads before the id
   * (movs r1; movs r0); as a direct call or a plain forwarding inline, the id
   * comes first, 10 of 110 instructions of Scene_Initialize differ in tla-en.
   */
  register s32 v asm("r1") = value;

  Func_02000ba4(id, v);
}

/// Sets the event route to 0x204. In the first room, gives objects 9 to 14
/// their values and priority bits, and under flag 0x200 runs Func_02000494.
/// Under flag 0x201, runs a screen transition and Func_02000408, then clears
/// the flag.
s32 Scene_Initialize(void) {
  s32 *route =
      &((struct AnemosInnerSanctumEntranceEventWork *)Ram_HeapSlots->eventWork)
           ->route;
  s32 value = 0x204;

  /* FAKEMATCH(barrier): an empty asm after the address and the value, so
   * gMainRoutineWork's address loads just before the store; as one assignment,
   * 2 of 110 instructions differ in tla-en. */
  __asm__("");
  *route = value;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6b1Room1) {
    Func_02000b18();
    Func_02000bd4(2);
    SetObjectValue(9, 0);
    SetObjectValue(10, 1);
    SetObjectValue(11, 2);
    SetObjectValue(12, 3);
    SetObjectValue(13, 128);
    SetObjectValue(14, 129);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
    Actor_GetById(9)->priorityFlags |= 0xc0;
    Actor_GetById(10)->priorityFlags |= 0x80;
    Actor_GetById(11)->priorityFlags |= 0x80;
    Actor_GetById(12)->priorityFlags |= 0x80;
    Actor_GetById(13)->priorityFlags |= 0x80;
    Actor_GetById(14)->priorityFlags |= 0x80;
    if (GameFlag_Test(0x200)) {
      Func_02000be0(1);
      Func_02000494();
    }
  }
  if (Flag_Test(0x201)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000408();
    GameFlag_ClearBit(0x201);
  }
  return 0;
}


s32 Scene_PrepareMap(void) { return 0; }

#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "frame.h"

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_02001cdc;
extern s16 gData_02001cde;
extern struct Actor *gData_02001ce0[];
extern struct LightOam gData_02001d20[];
extern s16 gData_02001de0;
/* VIEW(s0588): variable gData_02001de2 as extern s16 gData_02001de2; hide has extern u16 gData_02001de2; tried as the one declaration extern s16 gData_02001de2, it does not compile in tla-ja: games/tla/src/scene/anemos_inner_sanctum_entrance.c:355: conflicting types for `gData_02001de2'. */
extern s16 gData_02001de2__s0588 __asm__("gData_02001de2");
/* VIEW(s0588): variable gData_02001de4 as extern s16 gData_02001de4; hide has extern u16 gData_02001de4; tried as the one declaration extern s16 gData_02001de4, it does not compile in tla-ja: games/tla/src/scene/anemos_inner_sanctum_entrance.c:357: conflicting types for `gData_02001de4'. */
extern s16 gData_02001de4__s0588 __asm__("gData_02001de4");

s32 Func_02000d5c(struct Actor *actor);
void Func_02000c3c(struct LightOam *oam, s32 depth);

void Func_02000858(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, picture, tile, viewY;
  struct LightOam *oam;
  s32 i;

  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02001cdc].offset >> 5;
  viewY = map->origin[1];
  oam = gData_02001d20;
  picture = gData_02001de4__s0588 * ((gFrameCount >> 2) % 5 * 32 + 32);

  for (i = 0; i < gData_02001cde; i++) {
    struct Actor *actor = gData_02001ce0[i];

    if (actor != 0 && actor->x != 0) {
      s32 ox = actor->x - viewX;
      s32 y = actor->y - viewY + 0x80000;
      s32 z = actor->z - viewZ - viewY;
      s32 oy = z - y;
      s32 depth = ((y + z) >> 16) + 58;
      s32 mode = gData_02001de0;
      u32 *word;

      if (mode == 0) {
        s32 frame = actor->timer;
        s32 x = ox >> 16;
        s32 sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | (frame & 0x80) << 21 | 0x40002000;
        *word = (tile + picture + (frame & 7) * 8) | 0x800;
        if (gData_02001de2__s0588 == -1)
          oam->priority = Func_02000d5c(actor);
        else
          oam->priority = gData_02001de2__s0588;
        Func_02000c3c(oam, depth);
        oam++;
      } else {
        s32 frame = actor->timer;
        s32 x = ox >> 16;
        s32 sy = oy >> 16;
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_02001de2__s0588 == -1)
          oam->priority = Func_02000d5c(actor);
        else
          oam->priority = gData_02001de2__s0588;
        Func_02000c3c(oam, depth);
        oam++;
      }
    }
  }
}

#include "iwram_call.h"
#include "task.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesD[];

void Func_02000c24(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000a98(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02001ce0, 64);
  Func_02000c24(gField_CachedTilesA, buffer);
  gData_02001cdc = Resource_FindFreeEntry();
  VramCache_Load(gData_02001cdc, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000858, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2__s0588 = -1;
}

void Func_02000b18(void) {
  u8 *buffer = BumpAlloc(0x1800);

  Iwram_ClearWords(gData_02001ce0, 64);
  Func_02000c24(gField_CachedTilesD, buffer);
  gData_02001cdc = Resource_FindFreeEntry();
  VramCache_Load(gData_02001cdc, 0x1800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000858, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2__s0588 = -1;
  gData_02001de4__s0588 = 0;
}


s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Func_02000ba4(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object->timer = value;
    gData_02001ce0[gData_02001cde] = object;
    gData_02001cde++;
  }
}

void Func_02000bd4(u16 value) { gData_02001de2__s0588 = value; }

void Func_02000be0(u16 value) { gData_02001de4__s0588 = value; }

// clang-format off
u32 gData_02000d6c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02000da8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02000de4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s32 gData_02000e20[] = {
    23, 9, 0x400, 23, 10, 0x400,
    23, 6, -0x4000, 0, 1, 13,
    40, 0, 17,
};

s32 gData_02000e5c[] = {
    23, 9, -2048, 23, 10, -2048,
    23, 6, 0x4000, 0, 1, 13,
    24, 0, 3, 0, 1, 17,
};

s32 gData_02000ea4[] = {
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};
// clang-format on

u32 gData_02000ee0[] = {
    0x1c0b1b0b,
    0x1c0c1b0c,
    0x1c0d1b0d,
};

s8 gData_02000eec[] = {
    12, 14, 9, 11, 13, 10,
};
