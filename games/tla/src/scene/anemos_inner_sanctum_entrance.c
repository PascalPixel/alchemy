
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"
#include "shared/actors.h"
#include "anemos_inner_sanctum_entrance/anemos_inner_sanctum_entrance.h"
#include "field.h"

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);

DEFINE_SCENE_CREATE_IMPORTED_ACTOR(Scene_CreateImportedActor, 0x22)

#include "../field/field_effect/move.c"

// ---- from scene/anemos_inner_sanctum_entrance.c ----
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

// ---- scene/anemos_inner_sanctum_entrance.c ----

// ---- games/tla/src/scene/anemos_inner_sanctum_entrance/hide.c ----
#include "shared/jupiter_lighthouse.h"
#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"
#include "event.h"
#include "event_work.h"
#include "item_icon.h"

/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

extern s32 gData_02000e20[];
extern s32 gData_02000e5c[];
extern s32 gData_02000ea4[];
extern s8 gData_02000eec[];
void Func_02000b18(void);
void Func_02000ba4(s32 id, s32 value);
void Func_02000bd4(u16 value);
void Func_02000588(void);

extern s32 gData_02001970[];

struct Actor;

extern s16 gData_02001cde;
extern struct Actor *gLightSpriteActors[];
extern u16 gData_02001de2;
extern u16 gData_02001de4;
extern s16 gLightSpriteTile;
extern s16 gData_02001de0;

#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_call.h"
#include "text/msg_ids.h"

s32 Func_0200033c(struct Actor *object) {
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
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "anemos_inner_sanctum_entrance/anemos_inner_sanctum_entrance.h"

extern struct ActorPlacement gData_020019dc[];
extern struct ActorPlacement gData_020019f4[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6b1Room1)
    return gData_020019f4;
  return gData_020019dc;
}

void Func_02000388(void) {
  struct AnemosInnerSanctumEntranceParty *party;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(8, 2);
  Event_Wait(20);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)&MsgIAmTheShadow, 0, 6);
  party = (struct AnemosInnerSanctumEntranceParty *)&gMainRoutineWork;
  party->battleGroup = 0x1a9f;
  party->battleRule = 2;
  Field_RequestBattleFromEncounterTable(103, 3);
  GameFlag_Set(0x201);
}

void Func_02000408(void) {
  struct Actor *object = Actor_GetById(15);

  object->motionFlags = 0;
  object->scaleX = 0x4000;
  object->scaleY = 0x4000;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(151);
  Actor_SetSpriteColor(Actor_GetById(8), 7);
  Actor_SetSpriteColor(Actor_GetById(15), 7);
  Event_Wait(20);
  Sound_Play(190);
  Actor_MoveToObject(15, 8);
  wait(1);
  Actor_SetMovementTypeById(8, gData_02000ea4);
  Actor_SetMovementTypeAndWaitById(15, gData_02000e20);
  Actor_SetMovementTypeAndWaitById(15, gData_02000e5c);
  Event_Wait(10);
  Event_End();
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
    Actor_SetSpriteFlags(object, 1);
  else
    Actor_SetSpriteFlags(object, 0);
}

/* VIEW(hide): variable gData_02000ee0 as extern const s8 gData_02000ee0[]; the
 * definition has u32 gData_02000ee0[] = { 0x1c0b1b0b, 0x1c0c1b0c, 0x1c0d1b0d,
 * }; tried as the definition's extern u32 gData_02000ee0[], Func_020004e4
 * moves: 1 of 32 instructions in tla-ja. */
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
  GameFlag_Set(0x200);
  Sound_Play(201);
  Func_02000be0(1);
  Func_02000494();
}

void Func_02000534(s32 a) {
  struct MainRoutineWork *party;
  s32 held = Leader_CanPushObjectAhead();

  party = &gMainRoutineWork;
  if (held) {
    if (Field_FindNearbyTalkTarget(party->currentOwner) != -1) {
      Leader_PushObjectAhead();
      Func_020004e4(0, a);
    }
  } else {
    if (Actor_GetById(party->currentOwner)->y > -0x1c0000)
      Field_TryJumpForward();
  }
}

// ---- games/tla/src/scene/anemos_inner_sanctum_entrance/s0588.c ----
#include "version.h"

void Func_02000588(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 shift = 0x1300000;
  s32 i;

  if (object->z >> 20 <= 9)
    shift = -shift;
  Event_ResolvePendingActions(0);
  if (!GameFlag_Get(0x200))
    return;
  if (MapGrid_GetCellBehaviorAt(object->terrainId, object->x, object->z) != 77)
    return;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
#if TLA_PATCH_1
  FieldActor_SetMoveTarget(object, (object->x & 0xfff00000) + 0x80000,
                           object->y, (object->z & 0xfff00000) + 0x80000);
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
    Actor_SetSpriteFlags(object, 1);
    Actor_SetMoveTarget(object, object->x, 0xfff80000, object->z);
    object->update = Func_020004b4;
    Sound_Play(152);
    wait((Random16() * 4 >> 16) + 3);
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
  Actor_WaitForMove(Actor_GetById(9));
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
  if (!GameFlag_Get(0xa9f) && shift > 0)
    Func_02000388();
}

void Func_02000710(void) {}

extern struct Trigger gData_02001c1c[];
extern struct Trigger gData_02001c28[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6b1Room1)
    return gData_02001c28;
  return gData_02001c1c;
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Get(flag); }

static __inline__ void SetObjectValue(s32 id, s32 value) {
  /* FAKEMATCH(pin): the value pinned to r1, which the ROM loads before the id
   * (movs r1; movs r0); as a direct call or a plain forwarding inline, the id
   * comes first, 10 of 110 instructions of Scene_Initialize differ in tla-en.
   */
  register s32 v asm("r1") = value;

  Func_02000ba4(id, v);
}

// Sets the event route to 0x204. In the first room, gives objects 9 to 14
// their values and priority bits, and under flag 0x200 runs Func_02000494.
// Under flag 0x201, runs a screen transition and Func_02000408, then clears
// the flag.
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
    if (GameFlag_Get(0x200)) {
      Func_02000be0(1);
      Func_02000494();
    }
  }
  if (Flag_Test(0x201)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000408();
    GameFlag_Clear(0x201);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "frame.h"

extern struct LightOam gData_02001d20[];
/* VIEW(s0588): variable gData_02001de2 as extern s16 gData_02001de2; hide has
 * extern u16 gData_02001de2; tried as the rest of the object declares it,
 * LightSprites_Draw moves: 149 of 260 instructions, LightSprites_InitA moves: 0
 * of 36 instructions, Func_02000b18 moves: 0 of 39 instructions in tla-ja. */
extern s16 gData_02001de2__s0588 __asm__("gData_02001de2");
/* VIEW(s0588): variable gData_02001de4 as extern s16 gData_02001de4; hide has
 * extern u16 gData_02001de4; tried as the rest of the object declares it,
 * LightSprites_Draw moves: 2 of 260 instructions in tla-ja. */
extern s16 gData_02001de4__s0588 __asm__("gData_02001de4");

void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, picture, tile, viewY;
  struct LightOam *oam;
  s32 i;

  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = gData_02001d20;
  picture = gData_02001de4__s0588 * ((gFrameCount >> 2) % 5 * 32 + 32);

  for (i = 0; i < gData_02001cde; i++) {
    struct Actor *actor = gLightSpriteActors[i];

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
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gData_02001de2__s0588;
        Oam_AddSpriteAtDepthClamped(oam, depth);
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
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gData_02001de2__s0588;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

#include "iwram_call.h"
#include "task.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw, gData_02001cde,
                            gData_02001de0, gData_02001de2__s0588)

void Func_02000b18(void) {
  u8 *buffer = BumpAlloc(0x1800);

  Iwram_ClearWordsEntry(gLightSpriteActors, 64);
  Decompress_TaggedLz(gField_CachedTilesD, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 0x1800, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2__s0588 = -1;
  gData_02001de4__s0588 = 0;
}

void Func_02000ba4(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object->timer = value;
    gLightSpriteActors[gData_02001cde] = object;
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

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
// ---- from scene/anemos_inner_sanctum_entrance/data2.c ----

#include "macros.h"
#include "shared/tables.h"
#include "types.h"
#include "scene_tables.h"

/* VIEW(data2): routine Func_0200033c as this part declared it; the joined file
 * has another. */
void Func_0200033c__data2(void) __asm__("Func_0200033c");
#define Func_0200033c Func_0200033c__data2
/* VIEW(data2): routine Func_020004e4 as this part declared it; the joined file
 * has another. */
void Func_020004e4__data2(void) __asm__("Func_020004e4");
#define Func_020004e4 Func_020004e4__data2
/* VIEW(data2): routine Func_02000534 as this part declared it; the joined file
 * has another. */
void Func_02000534__data2(void) __asm__("Func_02000534");
#define Func_02000534 Func_02000534__data2

/* VIEW(data2): routine gData_02000d6c as this part declared it; the joined file
 * has another. */
extern u8 gData_02000d6c__data2[] __asm__("gData_02000d6c");
#define gData_02000d6c gData_02000d6c__data2
/* VIEW(data2): routine gData_02000da8 as this part declared it; the joined file
 * has another. */
extern u8 gData_02000da8__data2[] __asm__("gData_02000da8");
#define gData_02000da8 gData_02000da8__data2
/* VIEW(data2): routine gData_02000de4 as this part declared it; the joined file
 * has another. */
extern u8 gData_02000de4__data2[] __asm__("gData_02000de4");
#define gData_02000de4 gData_02000de4__data2

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gData_02000d6c,
    gData_02000da8,
    gData_02000de4,
};

LATE_DATA s32 gData_02001970[] = {
    46,
    (s32)Func_0200033c,
    17,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA DEFINE_G_SCENE_EXITS(308, 239, 23, 303, 1, 239, 22, 24, 306, 5)

    LATE_DATA struct ActorPlacement gData_020019dc[] = {
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_020019f4[] = {
    {180, 0xa9f, 1, PIXELS(200), 0, PIXELS(104), FACING_EAST, 1, 0},
    {430, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(424), FACING_EAST, 1, 0},
    {430, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(456), FACING_EAST, 1, 1},
    {430, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(472), FACING_EAST, 1, 1},
    {430, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(424), FACING_EAST, 1, 1},
    {430, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(456), FACING_EAST, 1, 1},
    {430, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(472), FACING_EAST, 1, 1},
    {303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(152), 0, PIXELS(168),
     FACING_WEST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(152), 0, PIXELS(216),
     FACING_WEST, 1, 1},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(248), 0, PIXELS(168),
     FACING_EAST, 1, 1},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(248), 0, PIXELS(216),
     FACING_EAST, 1, 1},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(696), 0, PIXELS(240),
     FACING_WEST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(744), 0, PIXELS(240),
     FACING_EAST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(424), 0, PIXELS(232),
     FACING_WEST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(424), 0, PIXELS(288),
     FACING_WEST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(504), 0, PIXELS(232),
     FACING_EAST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(504), 0, PIXELS(288),
     FACING_EAST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(424), 0, PIXELS(712),
     FACING_WEST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(424), 0, PIXELS(768),
     FACING_WEST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(504), 0, PIXELS(712),
     FACING_EAST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_02001970, PIXELS(504), 0, PIXELS(768),
     FACING_EAST, 1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gData_02001c1c[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gData_02001c28[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x202, 33, CONDITION_ALWAYS, EVENT(Func_02000534)},
    {0x202, 77, CONDITION_ALWAYS, EVENT(Func_02000534)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, 0x200, EVENT(Func_020004e4)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 77,
     IF_FLAG_SET(0x200), EVENT(Func_02000588)},
    {TRIGGER_TABLE_END},
};
#undef Func_0200033c
#undef Func_020004e4
#undef Func_02000534
#undef gData_02000d6c
#undef gData_02000da8
#undef gData_02000de4
