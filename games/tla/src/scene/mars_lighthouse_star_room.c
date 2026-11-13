/// @file
/// The Mars Lighthouse room of the Mars Star, in the rooms of this overlay:
/// the statue the party examines (MarsLighthouseStarRoom_ExamineStatue), the
/// Mars Star it makes and takes (MarsLighthouseStarRoom_CreateMarsStar,
/// MarsLighthouseStarRoom_TakeMarsStar) and uses to burst the ice
/// (MarsLighthouseStarRoom_UseMarsStar, MarsLighthouseStarRoom_BurstIce), the
/// warm hand after the battle (MarsLighthouseStarRoom_HoldWarmHand), and its
/// ice floors (MarsLighthouseStarRoom_SlideOnIce).

#include "shared/mars_lighthouse.h"
#include "shared/actors.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "actor.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "version.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "event_work.h"
#include "field_sprite.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "field/event_work.h"
#include "module/field/field.h"
#include "system/io_reg.h"
#include "battle/battle_unit.h"
#include "call.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "graphics/display_transition.h"
#include "item_icon.h"
#include "module/system/system.h"
#include "map.h"
#include "macros.h"
#include "system/input.h"
#include "system/field_effect.h"
#include "shared/cracked_tile.h"
#include "system/heap_release.h"
#include "field/cell_target.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "objcmd.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
#define MarsLighthouseStarRoom_SlideOnIce MarsLighthouseStarRoom_SlideOnIce

s32 Func_02000038(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000060(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[3384] = 1;
}

void Func_02000074(void) { Func_020014f4(); }

void Func_0200007c(void) {}

#ifndef TLA_MARS_LIGHTHOUSE_STAR_ROOM_H
#define TLA_MARS_LIGHTHOUSE_STAR_ROOM_H

#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "field/event_inline.h"
#include "system/input.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

s32 Trig_Cos(s32 angle);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_SetupScriptedPart(struct Actor *object, s32 priority);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ struct Actor *ObjectCreate_(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}
#include "system/main_routine_work.h"
#include "field_sprite.h"
#include "tla_edition.h"

void Actor_StartScript(struct Actor *object, const void *script);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
extern s16 gData_020047fc[];

#endif

extern u32 gMarsLighthouseStarRoom_ScatterIceChipsScript[];

/// Six chips of ice burst from an actor and bounce away.
void MarsLighthouseStarRoom_ScatterIceChips(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16() << 4) - (Random16() << 4);
    struct Actor *object =
        ObjectCreate_(0x145, x, (Random16() << 3) + source->y + 0x40000,
                      source->z + (Random16() << 4) - (Random16() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16();
      radius = Random16();
      scale = ((u32)Random16() >> 1) + 0x4000;
      Actor_StartScript(object, gMarsLighthouseStarRoom_ScatterIceChipsScript);
      Actor_SetSpriteFlags(object, 0);
      /* FAKEMATCH(dead): an empty once-only block holds the order around it; as
       * no block, 2 of 113 instructions differ in tla-en. */
      do {
      } while (0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius * 8, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = 0x8000;
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

void Func_02000188(void) {
  struct Actor *object = Actor_Get(10);

#if !TLA_PATCH_1
  if (Event_GetFlag(0xa4b) == 0) {
#elif !TLA_PATCH_2
  if (Event_GetFlag(0x238) == 0 || Event_GetFlag(0xa4b) == 0) {
#else
  if (Event_GetFlag(0x238) == 0 && Event_GetFlag(0xa4b) == 0) {
#endif
    Event_PlaySound(0x23d);
    object->x = object->timer << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    if (Event_GetFlag(0xa4a) == 0) {
      object->scaleX = 0x9999;
      object->scaleY = 0x9999;
    }
  }
}

void Func_020001e8(void) {
  struct Actor *object = Actor_Get(10);

#if !TLA_PATCH_1
  if (Event_GetFlag(0xa4b) == 0) {
#elif !TLA_PATCH_2
  if (Event_GetFlag(0x238) == 0 || Event_GetFlag(0xa4b) == 0) {
#else
  if (Event_GetFlag(0x238) == 0 && Event_GetFlag(0xa4b) == 0) {
#endif
    Event_PlaySound(0x23d);
    Event_SetFlag(0x233);
    object->x = object->timer << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    Event_SetFlag(0xa4a);
  }
}

void Func_02000d88(void);

struct EventDelay {
  u8 unk000[0x16a];
  u16 frames;
};

/// Burst on the frozen dragons, actor 10: a flash, the ice gone, chips flying
/// (flag 0x238).
void MarsLighthouseStarRoom_BurstIce(s32 phase, s32 frame) {
  if (phase == 1) {
    Event_PlaySound(574);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    Func_02000d88();
    Actor_SetPosition(10, 0, 0);
#if TLA_PATCH_1
    Event_SetFlag(0x238);
#endif
    ((struct EventDelay *)Ram_HeapSlots->eventWork)->frames = 20;
  }
  if ((u32)(frame - 97) <= 174 && frame % 30 == 0)
    MarsLighthouseStarRoom_ScatterIceChips(10);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

DEFINE_ACTOR_FREEZE(Actor_Freeze)

struct Overlay6abEventWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
  u8 unk1b0[0x1c];
  struct Actor *unk1cc;
};

void MarsLighthouseStarRoom_WakeAfterBattle(void);
void MarsLighthouseStarRoom_CreateMarsStar(s32 a);
void Func_02000db8(s32 actor);
void Func_02001d7c(void);
void CrackedTile_DropIn(void);
void Map_CenterOnFollowedPoint(void);
void FieldEffect_StartBlankOverlay(s32 a);
extern struct Actor *gMarsLighthouseStarRoom_MarsStar;
extern char SceneId_Ov6abRoom1;

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  ((struct Overlay6abEventWork *)Ram_HeapSlots->eventWork)->unk1ac = 0x204;
  gMarsLighthouseStarRoom_MarsStar = NULL;
  Event_SetFlag(0x112);
  Func_02001d7c();
  Func_02001df0(0, 0, 0, 88);
  if (scene == (s32)&SceneId_Ov6abRoom1) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(0xa30)) {
      MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
      MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
      MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
      MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
    }
    if (entrance == 11) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
      struct Overlay6abEventWork *work = Ram_HeapSlots->eventWork;
      /* FAKEMATCH(signature): GameFlag_GetByte declared returning s32, where
       * its definition returns u8; as u8, 17 of 255 instructions differ in
       * tla-en. */
      s32 column = GameFlag_GetByte(0x340);
      s32 row = GameFlag_GetByte(0x348);
      struct Actor *other;
      s32 x;

      x = (column << 20) + 0x80000;
      leader->x = x;
      leader->z = (row << 20) - 0x1280000;
      other = work->unk1cc;
      other->x = x;
      other->z = leader->z;
      Map_CenterOnFollowedPoint();
      if (!Event_GetFlag(0x109))
        CrackedTile_DropIn();
    }
    if (entrance == 9 || entrance == 10) {
      if (!Event_GetFlag(0x109))
        CrackedTile_DropIn();
    }
    if (entrance == 1 || entrance == 20) {
      if (!Event_GetFlag(0xa30)) {
        FieldActor_SetSpritePriority(11, 3);
        FieldActor_SetSpritePriority(12, 3);
        Actor_GetById(12)->scaleX = 0xffff0000;
      }
      if (Event_GetFlag(0xa4b)) {
        Func_02000d88();
        Actor_SetPosition(10, 0, 0);
      } else if (Event_GetFlag(0xa4a)) {
        Event_SetFlag(0x233);
      } else {
        Actor_SetPosition(10, 0, 0);
      }
      if (Event_GetFlag(0xa3b)) {
        if (!Event_GetFlag(0xa30)) {
          Actor_SetPosition(11, 0, 0);
          Actor_SetPosition(12, 0, 0);
        }
        Func_02000d88();
        System_Wait(10);
        if (!Event_GetFlag(0xa3a)) {
          MarsLighthouseStarRoom_CreateMarsStar(0);
          if (!Event_GetFlag(0x109)) {
            Func_02000db8(13);
            Func_02000db8(14);
          }
        }
        if (Actor_GetById(13)->x > 0x1900000) {
          MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, 13);
          MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
        }
      } else if (entrance == 20) {
        MarsLighthouseStarRoom_WakeAfterBattle();
      }
    }
  }
  return 0;
}

extern char SceneId_Ov6ACRoom3;
void TileMap_ConvertBuffer(s32 scene, s32 a1);

s32 Scene_PrepareMap(void) {
  if (Event_GetFlag(0xa30))
    TileMap_ConvertBuffer(gMainRoutineWork.map + ((s32)&SceneId_Ov6ACRoom3 -
                                                  (s32)&SceneId_Ov6a8Room2),
                          0x5000000);
  if (gMainRoutineWork.entrance == 20)
    Event_SetFlag(0xa4b);
  return 0;
}

void Event_Start(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
extern char SceneId_Ov66c;

void Func_020005b0(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02000d88();
  Event_PlaySound(0);
  FieldEvent_Wait(40);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, 8);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 4, 10);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, 16);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 20);
  Event_PlaySound(53);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 2);
  FieldEvent_Wait(24);
  Actor_SetPosition(11, PIXELS(696), PIXELS(232));
  Actor_SetPosition(12, PIXELS(664), PIXELS(232));
  Actor_SetAnimation(11, 3);
  Actor_SetAnimation(12, 3);
  Event_PlaySound(107);
  MapGrid_SetShake(0x20000, 0x40000, 0x10000);
  FieldEvent_Wait(20);
  Event_PlaySound(0x186);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldEvent_Wait(20);
  Event_PlaySound(148);
  Actor_SetAnimation(11, 4);
  Actor_SetAnimation(12, 4);
  FieldEvent_Wait(60);
  gMainRoutineWork.battleRule24b = 2;
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_Ov6abRoom1, 20);
  gMainRoutineWork.battleBackground = (s32)&SceneId_Ov66c;
  Field_RequestBattleFromEncounterTable(102, 0);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Event_End();
void Actor_MoveToObject();
void Scene_PlayMusic();
extern u8 MsgMarsStar01[];
extern s32 gData_0200491c[];
extern s32 gData_02004958[];
extern s32 gData_020049a0[];
extern s32 gData_020049dc[];

/// After the Flame Dragons' battle, the two Adepts lie wounded.
/// ★ Named from the game: "Why... Why am I lying here?" (MsgMarsStar01)
void MarsLighthouseStarRoom_WakeAfterBattle(void) {
  s32 t1, t2, t3, t8, t10, t12, t14, t28, t30, t32, t34, t50, t52, t87, v5, v6,
      v8;

  t1 = Actor_Get(11);
  v5 = t1;
  t2 = Actor_Get(15);
  v6 = t2;
  t3 = Actor_Get(16);
  v8 = t3;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02000d88();
  t8 = Actor_Get(13);
  Actor_SetSpriteFlags(t8, 0);
  t10 = Actor_Get(14);
  Actor_SetSpriteFlags(t10, 0);
  t12 = Actor_Get(15);
  FieldActor_SetSpriteColor(t12, 7);
  t14 = Actor_Get(16);
  FieldActor_SetSpriteColor(t14, 7);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(v5, FIELD_AT(v5, s32, 8), 0,
                                       (FIELD_AT(v5, s32, 16) + -0x80000));
  FIELD_AT(v6, u8, 85) = 0;
  FIELD_AT(v6, s32, 24) = 0x4000;
  FIELD_AT(v6, s32, 28) = 0x4000;
  FIELD_AT(v8, u8, 85) = 0;
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 10 of 343 instructions differ in
   * tla-en. */
  do {
  } while (0);
  FIELD_AT(v8, s32, 24) = 0x4000;
  FIELD_AT(v8, s32, 28) = 0x4000;
  FieldCamera_MoveTo(PIXELS(680), -1, PIXELS(228), 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(680), PIXELS(264));
  System_Wait(1);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(78);
  FieldEvent_Wait(20);
  Event_PlaySound(151);
  t28 = Actor_Get(11);
  FieldActor_SetSpriteColor(t28, 7);
  t30 = Actor_Get(12);
  FieldActor_SetSpriteColor(t30, 7);
  t32 = Actor_Get(13);
  FieldActor_SetSpriteColor(t32, 7);
  t34 = Actor_Get(14);
  FieldActor_SetSpriteColor(t34, 7);
  FieldEvent_Wait(20);
  Event_PlaySound(190);
  Actor_MoveToObject(15, 11);
  Actor_MoveToObject(16, 12);
  System_Wait(1);
  Actor_SetScript(11, (s32)&gData_020049a0);
  Actor_SetScript(12, (s32)&gData_020049dc);
  Actor_SetScript(15, (s32)&gData_0200491c);
  Actor_RunScriptAndWait(16, (s32)&gData_0200491c);
  Func_02000db8(13);
  Func_02000db8(14);
  Actor_SetScript(15, (s32)&gData_02004958);
  Actor_RunScriptAndWait(16, (s32)&gData_02004958);
  FieldEvent_Wait(10);
  t50 = Actor_Get(13);
  FieldActor_SetSpriteColor(t50, 0);
  t52 = Actor_Get(14);
  FieldActor_SetSpriteColor(t52, 0);
  FieldEvent_Wait(80);
  Actor_RunRepeatedMotion(13, 2);
  Actor_SetAnimation(13, 5);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)&MsgMarsStar01);
  FieldEvent_ShowMessageThenWait(13, 0, 80);
  Actor_RunRepeatedMotion(13, 1);
  FieldEvent_Wait(20);
  Event_PlaySound(34);
  FieldEvent_ShowMessageThenWait(13, 0, 20);
  Actor_RunRepeatedMotion(14, 1);
  Actor_SetAnimation(14, 5);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(14, 0, 20);
  Event_ShowMessage(14, 0);
  Actor_RunRepeatedMotion(13, 2);
  Actor_SetAnimation(13, 6);
  Event_ShowMessage(13, 0);
  Actor_SetAnimation(14, 6);
  FieldEvent_ShowMessageThenWait(14, 0, 20);
  Actor_ShowEmote(13, 0x105, 40);
  Event_ShowMessage(13, 0);
  FieldActor_ShowEmote(14, 0x105, 20);
  Event_ShowMessage(14, 0);
  Actor_ShowEmote(13, 0x100, 0);
  FieldEvent_ShowMessageThenWait(13, 0, 20);
  Event_ShowMessage(13, 0);
  Actor_SetAnimation(14, 7);
  Event_ShowMessage(14, 0);
  Event_ShowMessage(13, 0);
  FieldEvent_ShowMessageThenWait(14, 0, 40);
  Actor_ShowEmote(13, 0x100, 20);
  Event_OpenMessage(13, 0);
  t87 = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t87 != 0)
    goto L_02008a04;
  Actor_WaitForAnimationChange(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(13, 7);
  Event_ShowMessage(13, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), s16, 0) =
      (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
  goto L_02008a2c;
L_02008a04:
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), s16, 0) =
      (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 2 of 343 instructions differ in
   * tla-en. */
  do {
  } while (0);
  Actor_WaitForAnimationChange(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(13, 6);
  Event_ShowMessage(13, 0);
L_02008a2c:
  FieldEvent_Wait(40);
  Event_ShowMessage(13, 0);
  FieldActor_StartRepeatedMotion(14, 2);
  Event_ShowMessage(14, 0);
  Event_ShowMessage(13, 0);
  MarsLighthouseStarRoom_CreateMarsStar(1);
  Event_ShowMessage(14, 0);
  Actor_SetAnimationAndWait(13, 7);
  Event_ShowMessage(13, 0);
  Actor_SetAnimation(13, 10);
  Actor_SetAnimation(14, 9);
  Event_SetFlag(0xa3b);
  Scene_PlayMusic();
  Event_End();
}

struct SpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

struct SpriteAttributes {
  u8 unk00[9];
  struct SpriteBits bits;
};

void MarsLighthouseStarRoom_KeepStarAtLeaderPriority(struct Actor *object) {
  struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);
  struct SpriteAttributes *to = (struct SpriteAttributes *)object->sprite;
  struct SpriteAttributes *from = (struct SpriteAttributes *)owner->sprite;
  struct SpriteBits source = from->bits;
  struct SpriteBits bits;

  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}
#undef FIELD_AT

extern struct Actor *gMarsLighthouseStarRoom_MarsStar;

/// The Mars Star (item 247's icon) as an actor on the floor, rising first when
/// asked.
/// ★ Named from the game: "Mars Star" (item 247's name)
void MarsLighthouseStarRoom_CreateMarsStar(s32 rise) {
  struct Actor **slot = &gMarsLighthouseStarRoom_MarsStar;
  u8 *block;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 zero;
  s32 size;

  zero = 0;
  object = FieldActor_Create(0x1e9, 0x2be0000, 0, 0xe00000);
  *slot = object;
  if (object == NULL)
    return;
  sprite = object->sprite;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  object->motionFlags = 3;
  object->unk5c[0] = 1;
  object->update = MarsLighthouseStarRoom_KeepStarAtLeaderPriority;
  size = 0x608;
  sprite->nextFrame = zero;
  sprite->unk1b[0] = zero;
  block = HeapSlot_Alloc(68, size);
  Item_LoadIcon(247);
  block += 0x400;
  Vram_Load(sprite->vramBlock, 128, block);
  HeapSlot_Release(68);
  if (rise) {
    System_Wait(1);
    (*slot)->velocityY = 0x28000;
    System_Wait(4);
    Event_PlaySound(135);
    System_Wait(8);
    Event_PlaySound(135);
  }
}

#include "text/msg_ids.h"

void Event_Start(void);
void Event_End(void);

/// ★ Named from the game: "Your hand...so warm... People have such warm
/// hands... I had forgotten..." (MsgMarsStar03)
void MarsLighthouseStarRoom_HoldWarmHand(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(14, 5);
  FieldEvent_SetMessageId((s32)MsgMarsStar03);
  Event_ShowMessage(14, 0);
  Actor_SetAnimation(14, 9);
  Event_End();
}

void PartyInventory_Remove(s32 item);
s32 Inventory_AddItem(s32 owner, s32 item);
void *Unit_Get(s32 owner);
void Inventory_Equip(s32 owner, s32 slot);
void Actor_Release(struct Actor *object);
void FieldCamera_WaitForArrival(void);

/// Actor 13 hands over the Mars Star, which Felix puts in the Mythril Bag, and
/// the voice of the statue speaks.
/// ★ Named from the game: " got the Mars Star!" (MsgMarsStar08)
void MarsLighthouseStarRoom_TakeMarsStar(void) {
  struct Actor *leader;
  struct Actor *object;
  s32 i;
  s32 offset;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAnimation(13, 5);
  Event_SetMessageId((s32)MsgMarsStar02);
  Event_ShowMessage(13, 0);
  Actor_SetAnimation(13, 10);
  if (!Event_GetFlag(0xa3a)) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
    object = gMarsLighthouseStarRoom_MarsStar;
    object->motionFlags = 0;
    object->speedLimit = 0x40000;
    object->acceleration = 0x20000;
    FieldActor_SetMoveTarget(object, leader->x, leader->y + 0x200000,
                             leader->z);
    FieldActor_WaitForMove(gMarsLighthouseStarRoom_MarsStar);
    Event_PlaySound(83);
    PartyInventory_Remove(224);
    Inventory_AddItem(4, 222);
    Message_ShowCentered((s32)MsgMarsStar08, 1);
    i = 0;
    offset = 216;
    for (; i <= 14; offset += 2, i++) {
      if ((*(u16 *)(Unit_Get(4) + offset) & 0x1ff) == 222) {
        Inventory_Equip(4, i);
        break;
      }
    }
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    Event_SetFlag(0xa3a);
    FieldActor_SetPositionAndResetMotion(gMarsLighthouseStarRoom_MarsStar, 0, 0,
                                         0);
    System_Wait(1);
    Actor_Release(gMarsLighthouseStarRoom_MarsStar);
    System_Wait(1);
    FieldCamera_SetSpeed(0x4cccc, 0x9999);
    FieldCamera_MoveTo(170 << 18, -1, 146 << 16, 1);
    FieldCamera_WaitForArrival();
    PaletteFade_ApplySource(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 0);
    PaletteFade_Interpolate(20);
    System_Wait(20);
    {
      // A halfword in its own local, so the address takes the register first.
      struct {
        u16 value;
      } white;

      white.value = 0x7fff;
      *(u16 *)0x0500021e = white.value;
    }
    System_Wait(1);
    FieldMessage_ShowCentered((s32)MsgMarsStar06, 0, 0);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(20);
    System_Wait(20);
  }
  Event_End();
}

void Func_02000d88(void) {
  MapGrid_CopyCellsTo(46, 68, 40, 73, 5, 6);
  MapGrid_CopyCellAttributes(104, 9, 5, 6, 40, 9);
}

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

void Func_02000db8(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSpriteFlags(Actor_Get(id), 0);
  FieldActor_SetSpritePriority(id, 3);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  object->collisionFlags = 0;
  if (id == 13) {
    Actor_SetPosition(13, 175 << 18, PIXELS(224));
    Actor_SetAnimation(13, 10);
    MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, id);
  } else {
    Actor_SetPosition(14, 165 << 18, PIXELS(216));
    Actor_SetAnimation(14, 9);
    MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
  }
}

struct EventNote {
  u8 unk000[0x15a];
  u16 unk15a;
};

/// ★ Named from the game: "There's a space for you to place something."
/// (MsgMarsStar07)
void MarsLighthouseStarRoom_ExamineStatue(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar07, 1);
  ((struct EventNote *)work)->unk15a = 1;
  Event_End();
}

extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov66d;

void Scene_PlayMusic(void);
void Func_02001424(void);

#if !TLA_PATCH_1

/// ★ Named from the game: " used the Mars Star!" (MsgMarsStar09)
void MarsLighthouseStarRoom_UseMarsStar(void) {
  struct Actor **slot;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *buffer;
  u16 *screen;
  s32 zero;

  screen = (u16 *)Ram_HeapSlots->block32;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot = &gMarsLighthouseStarRoom_MarsStar;
  object = FieldActor_Create(0x1e9, 0x2a80000, 0x80000, 0x930000);
  *slot = object;
  if (object != 0) {
    sprite = object->sprite;
    sprite->nextFrame = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = 0;
    object->unk5c[0] = 1;
    sprite->unk1b[0] = 0;
    object->priorityFlags |= 2;
    buffer = HeapSlot_Alloc(68, 0x608);
    Item_LoadIcon(247);
    Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 40);
  Event_PlaySound(0);
  Event_PlaySound(141);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10006, 0);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(Func_02001424, 0x480);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(0x2a80000, -1, 0x1e00000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(0x186);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(80);
  System_Wait(60);
  FieldTask_Remove(Func_02001424);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  TileMap_ConvertBuffer(gMainRoutineWork.map + ((s32)&SceneId_Ov6ACRoom3 -
                                                (s32)&SceneId_Ov6a8Room2),
                        screen);
  // The zero sits in a local: as a plain `*screen = 0` 21 instructions differ.
  zero = 0;
  *screen = zero;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(0x2a80000, -1, 0xba0000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  object = *slot;
  if (object != 0) {
    object->motionFlags = 3;
    object->velocityY = 0x20000;
    object->speedLimit = 0x20000;
    object->acceleration = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2a00000, 0, 0xa00000);
    FieldActor_WaitForMove((s32)*slot);
    Event_PlaySound(135);
    object = *slot;
    object->acceleration = 0x8000;
    object->velocityY = 0x40000;
    object->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2980000, 0, 0x9e0000);
    FieldActor_WaitForMove((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object = *slot;
  if (object != 0)
    Actor_Release(object);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Scene_PlayMusic();
  Event_SetFlag(0xa30);
  gMainRoutineWork.battleBackground = (s32)&SceneId_Ov66d;
  Event_End();
}

#else

void MarsLighthouseStarRoom_UseMarsStar(void) {
  s32 flag;
  struct Actor **slot, **slot2;
  struct Actor *object, *object2;
  struct FieldSprite *sprite, *sprite2;
  u8 *buffer, *buffer2;
  u16 *screen;

  flag = Event_GetFlag(0xa9e);
  if (flag != 0)
    goto L_02008ea2;
  goto L_0200905c;
L_02008ea2:
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot = &gMarsLighthouseStarRoom_MarsStar;
  object =
      (struct Actor *)FieldActor_Create(0x1e9, 0x2a80000, 0x80000, 0x930000);
  *slot = object;
  if (object != 0) {
    sprite = object->sprite;
    sprite->nextFrame = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = 0;
    object->unk5c[0] = 1;
    sprite->unk1b[0] = 0;
    object->priorityFlags |= 2;
    buffer = HeapSlot_Alloc(68, 0x608);
    Item_LoadIcon(247);
    Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 80);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x101, 40);
  object = *slot;
  if (object != 0) {
    object->motionFlags = 3;
    object->velocityY = 0x20000;
    object->speedLimit = 0x20000;
    object->acceleration = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2a00000, 0, 0xa00000);
    FieldActor_WaitForMove((s32)*slot);
    Event_PlaySound(135);
    object = *slot;
    object->acceleration = 0x8000;
    object->velocityY = 0x40000;
    object->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2980000, 0, 0x9e0000);
    FieldActor_WaitForMove((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object = *slot;
  if (object != 0)
    Actor_Release(object);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Scene_PlayMusic();
  Event_End();
  goto L_020093b0;
L_0200905c:
  screen = (u16 *)Ram_HeapSlots->block32;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot2 = &gMarsLighthouseStarRoom_MarsStar;
  object2 = FieldActor_Create(0x1e9, 0x2a80000, 0x80000, 0x930000);
  *slot2 = object2;
  if (object2 != 0) {
    sprite2 = object2->sprite;
    sprite2->nextFrame = 0;
    sprite2->unk1b[0] = 0;
    sprite2->fullColor = 0;
    sprite2->palette = 0;
    sprite2->priority = 2;
    object2->motionFlags = 0;
    object2->unk5c[0] = 1;
    object2->priorityFlags |= 2;
    buffer2 = HeapSlot_Alloc(68, 0x608);
    Item_LoadIcon(247);
    Vram_Load(sprite2->vramBlock, 128, buffer2 + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 40);
  Event_PlaySound(0);
  Event_PlaySound(141);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10006, 0);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(Func_02001424, 0x480);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(0x2a80000, -1, 0x1e00000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(0x186);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(80);
  System_Wait(60);
  FieldTask_Remove(Func_02001424);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  TileMap_ConvertBuffer(gMainRoutineWork.map + ((s32)&SceneId_Ov6ACRoom3 -
                                                (s32)&SceneId_Ov6a8Room2),
                        screen);
  *screen = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(0x2a80000, -1, 0xba0000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  object2 = *slot2;
  if (object2 != 0) {
    object2->motionFlags = 3;
    object2->velocityY = 0x20000;
    object2->speedLimit = 0x20000;
    object2->acceleration = 0x10000;
    FieldActor_SetMoveTarget(object2, 0x2a00000, 0, 0xa00000);
    FieldActor_WaitForMove((s32)*slot2);
    Event_PlaySound(135);
    object2 = *slot2;
    object2->acceleration = 0x8000;
    object2->velocityY = 0x40000;
    object2->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object2, 0x2980000, 0, 0x9e0000);
    FieldActor_WaitForMove((s32)*slot2);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object2 = *slot2;
  if (object2 != 0)
    Actor_Release(object2);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Scene_PlayMusic();
  Event_SetFlag(0xa30);
  Event_SetFlag(0xa9e);
  gMainRoutineWork.battleBackground = (s32)&SceneId_Ov66d;
  Event_End();
L_020093b0:
  return;
}

#endif

struct RiseTimer {
  u8 unk00[0x68];
  s32 frames;
};

void Func_020013e4(struct Actor *object) {
  struct RiseTimer *timer = (struct RiseTimer *)object;

  s32 growth;

  object->y += 0x8000;
  growth = 0x51e;
  object->scaleX += growth;
  object->scaleY += growth;
  timer->frames--;
  if (timer->frames == 1)
    FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
  else if (timer->frames == 0)
    Actor_Release(object);
}

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
void Actor_SetBlendMode(struct Actor *object, u32 value);

void Func_02001424(void) {
  u32 frame = gFrameCount;
  struct Actor *object;
  struct FieldSprite *sprite;
  u32 x, r;
  s32 z;
  s32 column;

  if (frame & 1)
    return;
  if ((frame >> 1) & 1) {
    r = Random16();
    x = r << 3;
    x -= r;
    z = Random16() * 15;
    x <<= 2;
    x >>= 16;
    column = 155;
  } else {
    r = Random16();
    x = r << 3;
    x -= r;
    z = Random16() * 15;
    x <<= 2;
    x >>= 16;
    column = 178;
  }
  column <<= 18;
  z <<= 3;
  x <<= 16;
  x += column;
  z = (u32)z >> 16;
  z <<= 16;
  z += 0x1cc0000;
  object = ObjectCreate_(0x11d, x, 0x200000, z);
  if (object != NULL) {
    /* FAKEMATCH(barrier): an empty asm clobbering r0, so the child flags call
     * copies object in again; as no asm, 7 of 91 instructions differ in tla-en.
     */
    asm("" : : : "r0");
    object->unk5c[0] = 2;
    object->motionFlags = 0;
    object->timer = 0;
    object->priorityFlags &= 0xfe;
    sprite = object->sprite;
    sprite->priority = 2;
    Actor_SetSpriteFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    *(s32 *)&object->watchedObject = 60;
    object->update = Func_020013e4;
    Actor_SelectAnimation(object, 5);
    Actor_SetBlendMode(object, 1);
  }
}

extern s32 gData_020044b8[];
extern s32 gData_020045b8[];

void Func_020014f4(void) {
  Func_02002a58(9, 18, 20, 0, gData_020044b8);
  Func_02002a58(9, 17, 19, 0x8000, gData_020045b8);
}

s32 Func_02001528(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x, b->x) < 0x80000) {
    if (DIFF(a->y, b->y) < 0x80000) {
      if (DIFF(a->z, b->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}

s32 Func_0200158c(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x, b->x) < 0xc0000) {
    if ((u32)(a->y - b->y + 0x7ffff) <= 0x1ffffe) {
      if (DIFF(a->z, b->z) < 0xc0000)
        return 1;
    }
  }
  return 0;
}

/* VIEW(s1424): tag struct RideSlot as struct RideSlot__s1424;
 * mars_lighthouse_star_room.c has another definition; not tried: struct
 * RideSlot__s1424 was not found. */
struct RideSlot__s1424 {
  u16 id;
  u16 blocked;
};

/* VIEW(s1424): tag struct RideWork as struct RideWork__s1424;
 * mars_lighthouse_star_room.c has another definition; not tried: struct
 * RideWork__s1424 was not found. */
struct RideWork__s1424 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1424 slots[4];
  u8 unk64[4];
  u16 value68;
  u16 direction;
};

extern u8 gInventorySnapshot[];

DEFINE_RIDE_UPDATE_CARRIED(Ride_UpdateCarried, RideWork__s1424, Func_02001528,
                           Func_0200158c)

extern u32 gData_02004744[];

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct Actor *object, const void *script);

DEFINE_ACTOR_RESET_SCRIPTED(Actor_ResetScripted, gData_02004744)

void Func_02001780(s32 *position, s32 speed, s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    ((void (*)())Actor_ResetScripted)(object, 2);
    object->update = Ride_UpdateCarried;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

/* VIEW(spawn): tag struct RideSlot as struct RideSlot__spawn;
 * mars_lighthouse_star_room.c has another definition; not tried: struct
 * RideSlot__spawn was not found. */
struct RideSlot__spawn {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(spawn): tag struct RideWork as struct RideWork__spawn;
 * mars_lighthouse_star_room.c has another definition; not tried: struct
 * RideWork__spawn was not found. */
struct RideWork__spawn {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__spawn slots[4];
  u8 unk64[4];
  struct RideDirection direction;
};

extern u8 gInventorySnapshot[];

s32 Func_02001528(struct Actor *a, struct Actor *b);
s32 Func_0200158c(struct Actor *a, struct Actor *b);

DEFINE_RIDE_UPDATE_LANDING(Ride_UpdateLanding, RideWork__spawn, Func_02001528,
                           Func_0200158c)

void Actor_SetupScriptedPart(struct Actor *object, s32 priority);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02004744; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
DEFINE_ACTOR_SETUP_SCRIPTED_PART(Actor_SetupScriptedPart, gData_02004744)

#define DEFINE_FUNC_0200196C_97(name, call1)                                   \
  void name(s32 *position, s32 speed, s32 flag) {                              \
    struct Actor *object =                                                     \
        FieldActor_Create(366, position[0], position[1], position[2]);         \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = 0;                                                      \
      object->gravity = 0;                                                     \
      object->unk4c = -speed;                                                  \
      Actor_SetupScriptedPart(object, 3);                                      \
      object->update = call1;                                                  \
      if (flag != 0)                                                           \
        object->unk5c[0] = 2;                                                  \
      Actor_SelectAnimation(object, 2);                                        \
    }                                                                          \
  }

DEFINE_FUNC_0200196C_97(Func_0200196c, Ride_UpdateCarried)

void Func_020019b4(s32 *position, s32 speed, s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateLanding;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

DEFINE_FUNC_0200196C_97(Func_020019fc, Ride_UpdateLanding)

s32 Func_02001a44(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x, b->x) < 0xc0000) {
    if ((u32)(a->y - b->y - 1) <= 0xffffe) {
      if (DIFF(a->z, b->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}

#define ABS(value) ((value) < 0 ? -(value) : (value))

DEFINE_RIDE_UPDATE_DRIFT(Ride_UpdateDrift, Func_02001a44)

extern u32 gData_02004744[];

DEFINE_ACTOR_SETUP_SCRIPTED(Actor_SetupScripted, Actor_SelectAnimation,
                            gData_02004744)

void Actor_SetBlendMode(struct Actor *object, u32 value);
void Ride_UpdateDrift();
/* VIEW(s1b78): routine Actor_SetupScripted as void Actor_SetupScripted(struct
 * Actor *object, s32 priority); the definition has void
 * Actor_SetupScripted(struct Actor *object); not tried: Actor_SetupScripted's
 * bound declaration was not found. */
extern void Func_02001b38__s1b78(struct Actor *object,
                                 s32 priority) __asm__("Actor_SetupScripted");

void Func_02001b78(s32 *position, s32 base) {
  struct Actor *object =
      ObjectCreate_(0x11d, position[0], position[1], position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce = Trig_Cos(((u32)(Random16() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16();
    object->unk4c = (u16)(Random16() << 1) + base;
    /* FAKEMATCH(temporary): the angle held in an s32 local keeps the zero
     * extension and the pool word 0xffff8000 (a direct store narrows the add
     * to 16 bits: movs, lsls for 0x8000, no extension); as a direct store, 10
     * of 50 instructions differ in tla-en. */
    angle = (u16)Random16() - 0x8000;
    object->timer = angle;
    Func_02001b38__s1b78(object, 2);
    object->update = Ride_UpdateDrift;
    Actor_SetBlendMode(object, 1);
  }
}

/* VIEW(s1bf8): tag struct Actor as struct Actor__s1bf8; actor.h has another
 * definition; not tried: struct Actor__s1bf8 was not found. */
typedef struct Actor__s1bf8 {
  u8 pad0[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[91 - 20];
  u8 flag5b;
} Actor;

typedef struct Entry {
  s16 active;
  s16 timer;
  u16 flags;
  s16 unk_6;
  Actor *actor;
  s32 unk_c;
} Entry;

typedef struct State {
  u8 pad0[6];
  s16 unk_6;
  s16 unk_8;
  u8 pada[4];
  s16 unk_e;
  void (*callback)();
  Entry entries[4];
} State;

void Func_02001780();
void Func_0200196c();
void Func_020019b4();
void Func_020019fc();
/* VIEW(s1bf8): routine Func_02001b78 as void Func_02001b78(); the parts before
 * it has another declaration; not tried: Func_02001b78's declaration holds a
 * FAKEMATCH tag. */
extern void Func_02001b78__s1bf8() __asm__("Func_02001b78");
extern u8 gInventorySnapshot[];

typedef struct Position {
  s32 x;
  s32 z;
} Position;

typedef struct Camera {
  u8 pad0[228];
  Position position;
} Camera;

typedef struct IwramGlobals {
  u8 pad0[32];
  Camera *camera;
  u8 pad24[108 - 36];
  union {
    s32 word;
    void *pointer;
  } unk_6c;
} IwramGlobals;

#define IWRAM ((IwramGlobals *)Ram_HeapSlots)

DEFINE_BEAM_DRAW_TARGETS(Func_02001bf8, Func_02001b78__s1bf8, Func_0200196c,
                         Func_020019fc, Func_020019b4, Func_02001780)
#undef IWRAM

struct RideAnimation {
  u8 unk00[0x10];
  u8 value10;
  u8 flag11 : 1;
};

/* VIEW(s1d7c): tag struct RideWork as struct RideWork__s1d7c;
 * mars_lighthouse_star_room.c has another definition; not tried: struct
 * RideWork__s1d7c was not found. */
struct RideWork__s1d7c {
  u8 unk00[0xc];
  u16 value0c;
  u8 unk0e[0x62];
  struct Actor *handle;
};

extern u8 gInventorySnapshot[];

void Func_02001d7c(void) {
  struct RideWork__s1d7c *work =
      (struct RideWork__s1d7c *)(gInventorySnapshot + 0x188);
  struct Actor *object;
  struct RideAnimation *animation;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

#undef MarsLighthouseStarRoom_SlideOnIce

// ---- scene/mars_lighthouse_star_room/ride_handle.c ----

/// Scene code of overlay 0x6ab, in ROM order.

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_Release(struct Actor *object);
#define MarsLighthouseStarRoom_SlideOnIce MarsLighthouseStarRoom_SlideOnIce

DEFINE_RIDE_RELEASE_HANDLE(Ride_ReleaseHandle, Actor_Release)

void Func_02001de4(s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->value0e = value;
}

/* VIEW(ride_handle): tag struct MarsLighthouseBeam as struct
 * MarsLighthouseBeam__ride_handle; module/scene/scene.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1544: structure has
 * no member named `stopCount'. */
struct MarsLighthouseBeam__ride_handle {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  u16 stopCount;
  u16 stops[7];
};

void Func_02001df0(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam__ride_handle *beam =
      (struct MarsLighthouseBeam__ride_handle *)(gInventorySnapshot + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;

  if (Event_GetFlag(0x109) == 0) {
    i = 0;
    id = *targets++;
    {
      /* FAKEMATCH(dead): a test combine folds, keeping the first loop's refs
       * unweighted; as no test, 90 of 145 instructions differ in tla-en. */
      s32 never = id & 0x40;
      if (never < 0) {
        id = 0;
        goto afterTarget;
      }
    }
    if (id > 0) {
    nextTarget:
      {
        s32 flags = *targets;
        s32 height = flags & 15;
        struct Actor *object;
        s32 one;

        flags &= 0xf0;
        object = Actor_Get(id);
        targets++;
        if (object != NULL) {
          Actor_SetSpriteFlags(object, 0);
          ((u8 *)object)[98] = one = 1;
          if ((flags & 0x80) == 0)
            object->collisionFlags = 0;
          target->active = one;
          target->flags = flags;
          target->object = Actor_Get(id);
          target->height = height << 16;
          target->index = i;
          target++;
        }
      }
    afterTarget:
      i++;
      if (i <= 3) {
        if ((id = *targets++) > 0)
          goto nextTarget;
      }
    }
    if (stops != NULL) {
      s32 count;
      s32 stopOffset;
      u16 *p;

      i = 0;
      count = *stops++;
      stopOffset = 84;
      *(u16 *)((u8 *)beam + stopOffset) = count;
      {
        /* FAKEMATCH(dead): a test combine folds, keeping the second loop's refs
         * unweighted; as no test, 133 of 145 instructions differ in tla-en. */
        s32 never = count & 0x40;
        if (never < 0) {
          p = stops;
          goto afterStop;
        }
      }
      if (count > 0) {
        p = &beam->stopCount;
      nextStop:
        p[1] = *stops;
      afterStop:
        stops++;
        i++;
        p += 2;
        if (i <= 3) {
          s32 stop = *stops++;

          *p = stop;
          if (stop > 0)
            goto nextStop;
        }
      }
    }
  }
  beam->unk10 = a2;
  beam->unk0a = a3;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 control = 0x3f10;
      REG_BLDCNT = control;
    }
  }
  Task_AddFrame(Func_02001bf8, 0x480);
}

s32 Func_02001f24(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a;
}

void Func_02001f34(s32 index, s16 value) {
  ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a = value;
}

s32 Func_02001f44(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->counter;
}

DEFINE_RIDE_ADVANCE_COUNTER(Ride_AdvanceCounter)

DEFINE_RIDE_SET_ENTRY(Ride_SetEntry)

void Func_02001f98(s32 index, s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->entries[index].c = value;
}

s32 Func_02001fa8(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

DEFINE_RIDE_SET_SLOT(Ride_SetSlot)

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

/* VIEW(ride_handle): routine Unit_Get as struct BattleUnit *Unit_Get(s32
 * owner); scene/mars_lighthouse_star_room has void *Unit_Get(s32 owner); tried
 * as the definition's void *Unit_Get(s32 owner), which the object already
 * declares, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1621: request for member `hp'
 * in something not a structure or union. */
struct BattleUnit *Unit_Get__ride_handle(s32 owner) __asm__("Unit_Get");
#define Unit_Get Unit_Get__ride_handle

DEFINE_PARTY_TAKE_FALL_DAMAGE(Party_TakeFallDamage)

#include "../field/field_effect/advance_spin.c"

DEFINE_ACTOR_BLINK(Actor_Blink, 10, 4, 0)

#undef Unit_Get
#undef MarsLighthouseStarRoom_SlideOnIce
// ---- scene/mars_lighthouse_star_room/s2148.c ----

/// Scene code of overlay 0x6ab, in ROM order.

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
extern u8 gInventorySnapshot[];

#define KNOCK_BACK Leader_KnockBack
#define KNOCK_BACK_TAKE_DAMAGE Party_TakeFallDamage
#define KNOCK_BACK_BLINK Actor_Blink
#define KNOCK_BACK_SET_ANIMATION Actor_SelectAnimation
#define KNOCK_BACK_CELL_ATTRIBUTE MapGrid_GetCellAttributeLowNibbleOrBump
#define KNOCK_BACK_CHECK_COLLISION MapGrid_CheckCollision
#define KNOCK_BACK_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/leader_knock_back.c"
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION
#undef KNOCK_BACK_SPHERES_OVERLAP

void Func_02002354(void) {
  if (((struct RideWork *)(gInventorySnapshot + 0x188))->active)
    Leader_KnockBack();
}

extern u8 gInventorySnapshot[];
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

struct RideEvent {
  u8 unk000[0x15a];
  u16 field15a;
  u16 field15c;
  u16 field15e;
  u8 unk160[4];
  u16 field164;
  u16 field166;
  u8 unk168[0x44];
  s32 field1ac;
  u8 unk1b0[2];
  u16 started;
  s32 field1b4;
};

// XXX: is the car an actor (its stopped halfword sits where an actor's action
// is, its steps in unk70)? The code that stores the car's pointer at the ride
// state's 0x6c would settle it.
struct RideCar {
  u8 unk00[0x64];
  s16 stopped;
  u8 unk66[0xa];
  s32 stepX;
  s32 unk74;
  s32 stepZ;
};

struct RideState {
  s32 counter;
  s16 active;
  u8 unk06[0x66];
  struct RideCar *car;
};

/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
s32 MarsLighthouseStarRoom_SlideOnIce(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  s32 frame[3]; // Unused: gives the stack frame its 12 bytes.

  /* FAKEMATCH(signature): typed returning s32, though it returns nothing; as
   * void, 2 of 138 instructions differ in tla-en. */

  if (event->started == 0) {
    ScreenTransition_Start(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Event_ClearFlag(0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  /* FAKEMATCH(wrapper): Party_TakeFallDamage called through a cast to
   * s32 (*)(s32, s32) with -10 and 0; as Party_TakeFallDamage(), 3 of 138
   * instructions differ in tla-en. */
  ((s32 (*)(s32, s32))Party_TakeFallDamage)(-10, 0);
#endif
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Event_SetFlag(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(0x106);
  leader->update = (void (*)(void))Actor_Blink;
  leader->facing = 0x4000;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->stopped == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z) != 7) {
      leader->x += ride->car->stepX;
      leader->z += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (ride->car->stepZ != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
}

DEFINE_RIDE_IF_ACTIVE(MarsLighthouseStarRoom_ContinueSlide,
                      MarsLighthouseStarRoom_SlideOnIce)

s32 MarsLighthouseStarRoom_IsSliding(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->active;
}

struct Sprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2Rest : 6;
  u8 unk0a[2];
  s32 scale; ///< as field_sprite.h's FieldSprite has it
  u8 vramBlock;
  u8 keepEntry : 1;
  u8 unk11 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

extern u32 gData_02004774[];

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void Func_020024d4(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x + a * 16;
    x -= b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + 0x40000;
    object = (struct Actor *)Import_ActorCreate(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;
      s32 velocity;

      angle = Random16();
      radius = Random16();
      sum = Random16();
      sum += Random16();
      scale = (sum >> 2) + 0x8000;
      Actor_StartScript(object, gData_02004774);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      velocity = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = 0x8000;
      sprite = object->sprite;
      source = work->owner->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->keepEntry = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
}

extern u32 gData_020047b8[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
DEFINE_EMBERS_SPAWN(Func_02002658, gData_020047b8)

void Func_02002738(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s236c): tag struct Work020023c4 as struct Work020023c4__s236c; s2148.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1890: structure has no member
 * named `unk6c'. */
struct Work020023c4__s236c {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

struct WorkEvent {
  u8 unk000[340];
  u16 unk154;
  u8 unk156[358 - 342];
  s16 unk166;
};

DEFINE_ACTOR_ROLL_AND_CRUMBLE(Actor_RollAndCrumble, Work020023c4__s236c,
                              MapGrid_GetCellAttributeLowNibbleOrBump,
                              Func_02002658, Func_020024d4)
#undef WORK
#undef ABS

void Func_02002a58(s32 sourceId, s32 id, s32 kind, s32 value,
                   const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  *(struct Actor **)((u8 *)object + 0x68) = source;
  object->timer = 0;
  *(s16 *)((u8 *)object + 0x66) = value;
  *(u8 *)((u8 *)object + 0x62) = kind;
  object->update = Func_02002738;
}

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Actor_RollAndCrumble as its update hook and its speed, clears its
/// scale and action, has it watch what self watches, and calls
/// Event_PlaySound(104). The twin of overlay 6ac's Func_02002e6c.
DEFINE_ACTOR_LAUNCH_AHEAD(Actor_LaunchAhead, Actor_RollAndCrumble)

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

DEFINE_ACTOR_POP_UP_SLOW(Actor_PopUpSlow, Actor_SelectAnimation)

DEFINE_ACTOR_POP_UP(Actor_PopUp, Actor_SelectAnimation)

extern u8 gInventorySnapshot[];

DEFINE_ACTOR_PARK(Actor_Park, Actor_LaunchAhead)

DEFINE_ACTOR_STEER_BY_PAD(Actor_SteerByPad, gData_020047fc,
                          Actor_SelectAnimation)

void Func_02002d90(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Event_SetFlag(0x106);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Event_ClearFlag(0x106);
  }
}

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gData_0200483c[];

DEFINE_LEADER_SLIDE_ON_ICE(Leader_SlideOnIce, Func_02002d90, gData_0200483c,
                           Actor_SelectAnimation, Actor_SteerByPad,
                           Field_CheckSpheresOverlap)

extern volatile s16 gMarsLighthouseStarRoom_SlideStopped;
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Actor_HasArrived(struct Actor *object);

#define SLIDE_FROM_EDGE Leader_SlideOnIceFromEdge
#define SLIDE_STOPPED gMarsLighthouseStarRoom_SlideStopped
#define SLIDE_STOP_LEADER Func_02002d90
#define SLIDE_ANGLES gData_0200483c
#define SLIDE_SET_ANIMATION Actor_SelectAnimation
#define SLIDE_HAS_ARRIVED Actor_HasArrived
#define SLIDE_KEEP_SLIDING Actor_SteerByPad
#define SLIDE_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/slide_on_ice_from_edge.c"
#undef SLIDE_FROM_EDGE
#undef SLIDE_STOPPED
#undef SLIDE_STOP_LEADER
#undef SLIDE_ANGLES
#undef SLIDE_SET_ANIMATION
#undef SLIDE_HAS_ARRIVED
#undef SLIDE_KEEP_SLIDING
#undef SLIDE_SPHERES_OVERLAP

// ---- scene/mars_lighthouse_star_room/set_data.c ----

/// Scene code of overlay 0x6ab, in ROM order.

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
#define MarsLighthouseStarRoom_SlideOnIce MarsLighthouseStarRoom_SlideOnIce

void Leader_SetSlideStopped(s32 value) {
  gMarsLighthouseStarRoom_SlideStopped = value;
}

DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(Leader_SlideOnIceAhead, Func_02002d90,
                                 Actor_SelectAnimation, Actor_SteerByPad,
                                 Field_CheckSpheresOverlap)

#include "../field/object/set_sprite_priority.c"

#undef MarsLighthouseStarRoom_SlideOnIce

#include "../field/field_effect/move.c"

#undef TLA_MARS_LIGHTHOUSE_STAR_ROOM_H
#undef DIFF
#undef FIELD_AT
#undef DEFINE_FUNC_0200196C_97
#undef ABS
#undef IWRAM
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION
#undef KNOCK_BACK_SPHERES_OVERLAP
#undef WORK
#undef ABS
#undef SLIDE_FROM_EDGE
#undef SLIDE_STOPPED
#undef SLIDE_STOP_LEADER
#undef SLIDE_ANGLES
#undef SLIDE_SET_ANIMATION
#undef SLIDE_HAS_ARRIVED
#undef SLIDE_KEEP_SLIDING
#undef SLIDE_SPHERES_OVERLAP
#undef Unit_Get
#undef Position
#undef Position
// @file
// Scene code of overlay 0x6ab, in ROM order.

void Actor_SelectAnimation(struct Actor *object, s32 mode);
extern s32 gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      (struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE gData_02004cf4
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepth
#include "../field/map/cell_sprite.c"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gData_02004cf4,
                          CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

DEFINE_CRACKED_TILE_FALL_EVENT(CrackedTile_FallEvent, Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
// Scene code of overlay 0x6ab, in ROM order.

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveTo(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(s3e04): routine CrackedTile_FallEvent as this part declared it; the
 * joined file has another; tried as the one declaration void
 * CrackedTile_FallEvent(), it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:2170: too many arguments to
 * function `CrackedTile_FallEvent'. */
void CrackedTile_FallEvent__s3e04(
    struct CellTarget *work,
    struct CellTargetEntry *entry) __asm__("CrackedTile_FallEvent");
#define CrackedTile_FallEvent CrackedTile_FallEvent__s3e04

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gData_02004cf4, CrackedTile_Crack,
                          CrackedTile_FallEvent)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gData_02004cf4,
                                  CrackedTile_DropIn)

#include "text/msg_ids.h"

void MarsLighthouseStarRoom_ExamineStatue(void);
#define MarsLighthouseStarRoom_SlideOnIce MarsLighthouseStarRoom_SlideOnIce

extern s32 gData_02004910[];

// clang-format off
u32 gMarsLighthouseStarRoom_ScatterIceChipsScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_020044b8[] = {
    46,
    (s32)Actor_PopUp,
    4,
    0x2680000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x2680000,
    0,
    0xf80000,
    1,
    0,
    2,
    4,
    0x2380000,
    0,
    0xf00000,
    1,
    0,
    2,
    4,
    0x2380000,
    0x200000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x2380000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2400000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2400000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2500000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    22,
    37,
    (s32)Func_02000188,
    46,
    (s32)Actor_Park,
    17,
};

s32 gData_020045b8[] = {
    46,
    (s32)Actor_PopUp,
    4,
    0x2a80000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x2a80000,
    0,
    0x1280000,
    1,
    0,
    2,
    4,
    0x2d80000,
    0,
    0x1280000,
    1,
    0,
    2,
    4,
    0x2d80000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x3280000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x3280000,
    0,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3280000,
    0x180000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x180000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x200000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3100000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3100000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3000000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    22,
    37,
    (s32)Func_020001e8,
    46,
    (s32)Actor_Park,
    17,
};

DEFINE_G_DATA_02001540(gData_02004744, 30)

u32 gData_02004774[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_020047b8[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_020047fc[] = {
    -1, 0, -32768, -1, -16384, -8192, -24576, -16384,
    16384, 8192, 24576, 16384, -1, 0, -32768, -1,
    -1, 0, -32768, -1, -16384, -1, -1, -16384,
    16384, -1, -1, 16384, -1, 0, -32768, -1,
};

s16 gData_0200483c[] = {
    -1, 0, -32768, -1, -16384, 0, -32768, -16384,
    16384, 0, -32768, 16384, -1, 0, -32768, -1,
};

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

s32 gData_02004910[] = {
    SC_CALL(Func_02000038),
    SC_END,
};

s32 gData_0200491c[] = {
    SC_ADD(SW_SCALE_X, 0x400),
    SC_ADD(SW_SCALE_Y, 0x400),
    SC_ADD(SW_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_END,
};

s32 gData_02004958[] = {
    23, 9, -2048, 23, 10, -2048,
    23, 6, 0x4000, 0, 1, 13,
    24, 0, 3, 0, 1, 17,
};

s32 gData_020049a0[] = {
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};

s32 gData_020049dc[] = {
    23, 9, 0x400, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(286),      SCENE_EXIT(1, 286, 2), SCENE_EXIT(2, 286, 1),
    SCENE_EXIT(3, 286, 4), SCENE_EXIT(4, 286, 3), SCENE_EXIT(5, 285, 5),
    SCENE_EXIT(6, 284, 6), SCENE_EXIT(7, 285, 7), SCENE_EXIT(8, 284, 8),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {418, CONDITION_ALWAYS, (s32)gData_02004910, PIXELS(712), 0, PIXELS(264),
     FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_02004910, PIXELS(648), 0, PIXELS(264),
     FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(680), 0, PIXELS(240), FACING_SOUTH, 2, 0},
    {181, 0xa30, 7, PIXELS(664), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {181, 0xa30, 7, PIXELS(696), 0, PIXELS(216), FACING_SOUTH, 2, 1},
    {33, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {23, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {81, 2, CONDITION_ALWAYS, 2},
    {81, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {33, 5, CONDITION_ALWAYS, 5},
    {33, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {33, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 90, 0x1a4b, EVENT(MarsLighthouseStarRoom_SlideOnIce)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Func_02000060)},
    {0x60009a15, 9, CONDITION_ALWAYS, EVENT(Func_02000074)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 9,
     CONDITION_ALWAYS, (u32)(Func_0200007c)},
    {TRIGGER_RAISED, 20, CONDITION_ALWAYS, EVENT(Func_020005b0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 20, 0x1233,
     EVENT(MarsLighthouseStarRoom_BurstIce)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_HoldWarmHand)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgMarsStar05)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_TakeMarsStar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgMarsStar04)},
    {TRIGGER_EXAMINE, 21, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_ExamineStatue)},
    {0xde04, 1045, CONDITION_ALWAYS, EVENT(MarsLighthouseStarRoom_UseMarsStar)},
    {TRIGGER_TABLE_END},
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};
#undef CrackedTile_FallEvent
#undef MarsLighthouseStarRoom_SlideOnIce
