/// @file
/// The Mars Lighthouse room of the Mars Star, in the rooms of this overlay:
/// the statue the party examines (MarsLighthouseStarRoom_ExamineStatue), the
/// Mars Star it makes and takes (MarsLighthouseStarRoom_CreateMarsStar,
/// MarsLighthouseStarRoom_TakeMarsStar) and uses to burst the ice
/// (MarsLighthouseStarRoom_UseMarsStar, MarsLighthouseStarRoom_BurstIce), the
/// warm hand after the battle (MarsLighthouseStarRoom_HoldWarmHand), and its
/// ice floors (MarsLighthouseRoomF_SlideOnIce).

#include "flags.h"
#include "item_ids.h"
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
        ObjectCreate_(0x145, x, (Random16() << 3) + source->y + px(4),
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
      /* FAKEMATCH(dead): an empty once-only block holds the order around it. */
      do {
      } while (0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius * 8, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = Q16_ONE / 2;
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
  if (Event_GetFlag(FLAG_SCENE_TEMP_38) == 0 || Event_GetFlag(0xa4b) == 0) {
#else
  if (Event_GetFlag(FLAG_SCENE_TEMP_38) == 0 && Event_GetFlag(0xa4b) == 0) {
#endif
    Event_PlaySound(SE_573);
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
  if (Event_GetFlag(FLAG_SCENE_TEMP_38) == 0 || Event_GetFlag(0xa4b) == 0) {
#else
  if (Event_GetFlag(FLAG_SCENE_TEMP_38) == 0 && Event_GetFlag(0xa4b) == 0) {
#endif
    Event_PlaySound(SE_573);
    Event_SetFlag(FLAG_SCENE_TEMP_33);
    object->x = object->timer << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
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
    Event_PlaySound(SE_574);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    Func_02000d88();
    Actor_SetPosition(10, 0, 0);
#if TLA_PATCH_1
    Event_SetFlag(FLAG_SCENE_TEMP_38);
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

#include "shared/actor_freeze.h"

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
extern char SceneId_MarsLighthouseStarRoom1;

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  ((struct Overlay6abEventWork *)Ram_HeapSlots->eventWork)->unk1ac = 0x204;
  gMarsLighthouseStarRoom_MarsStar = NULL;
  Event_SetFlag(FLAG_MARS_LIGHTHOUSE_LIT);
  Func_02001d7c();
  Func_02001df0(0, 0, 0, 88);
  if (scene == (s32)&SceneId_MarsLighthouseStarRoom1) {
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
       * its definition returns u8. */
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
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        CrackedTile_DropIn();
    }
    if (entrance == 9 || entrance == 10) {
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
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
        Event_SetFlag(FLAG_SCENE_TEMP_33);
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
          if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
            Func_02000db8(13);
            Func_02000db8(14);
          }
        }
        if (Actor_GetById(13)->x > px(400)) {
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

extern char SceneId_MarsLighthouseRoomM3;
void TileMap_ConvertBuffer(s32 scene, s32 a1);

s32 Scene_PrepareMap(void) {
  if (Event_GetFlag(0xa30))
    TileMap_ConvertBuffer(
        gMainRoutineWork.map +
            ((s32)&SceneId_MarsLighthouseRoomM3 - (s32)&SceneId_Prox2),
        0x5000000);
  if (gMainRoutineWork.entrance == 20)
    Event_SetFlag(0xa4b);
  return 0;
}

void Event_Start(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
extern char SceneId_GondowanCliffsHideout;

void Func_020005b0(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02000d88();
  Event_PlaySound(MUS_000);
  FieldEvent_Wait(40);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.6);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, 8);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 4, 10);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, 16);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 20);
  Event_PlaySound(MUS_053);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 2);
  FieldEvent_Wait(24);
  Actor_SetPosition(11, 696, 232);
  Actor_SetPosition(12, 664, 232);
  Actor_SetAnimation(11, 3);
  Actor_SetAnimation(12, 3);
  Event_PlaySound(SE_TREMOR);
  MapGrid_SetShake(2, 4, 1);
  FieldEvent_Wait(20);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(1, 2, 1);
  FieldEvent_Wait(20);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_BOSS_ROAR);
  Actor_SetAnimation(11, 4);
  Actor_SetAnimation(12, 4);
  FieldEvent_Wait(60);
  gMainRoutineWork.battleRule24b = 2;
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_MarsLighthouseStarRoom1,
                                       20);
  gMainRoutineWork.battleBackground = (s32)&SceneId_GondowanCliffsHideout;
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
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
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
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here.
   */
  do {
  } while (0);
  FIELD_AT(v8, s32, 24) = 0x4000;
  FIELD_AT(v8, s32, 28) = 0x4000;
  FieldCamera_MoveTo(680, CAMERA_KEEP, 228, 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 680, 264);
  System_Wait(1);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_LIGHT_APPEAR);
  t28 = Actor_Get(11);
  FieldActor_SetSpriteColor(t28, 7);
  t30 = Actor_Get(12);
  FieldActor_SetSpriteColor(t30, 7);
  t32 = Actor_Get(13);
  FieldActor_SetSpriteColor(t32, 7);
  t34 = Actor_Get(14);
  FieldActor_SetSpriteColor(t34, 7);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_WARP);
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
  Event_PlaySound(MUS_034);
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
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(13, 0);
  FieldActor_ShowEmote(14, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(14, 0);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_ShowMessageThenWait(13, 0, 20);
  Event_ShowMessage(13, 0);
  Actor_SetAnimation(14, 7);
  Event_ShowMessage(14, 0);
  Event_ShowMessage(13, 0);
  FieldEvent_ShowMessageThenWait(14, 0, 40);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_OpenMessage(13, 0);
  t87 = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t87 == 0) {
    Actor_WaitForAnimationChange(gMainRoutineWork.currentOwner);
    Actor_SetAnimation(13, 7);
    Event_ShowMessage(13, 0);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), s16, 0) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
  } else {
    FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), s16, 0) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
    /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here.
     */
    do {
    } while (0);
    Actor_WaitForAnimationChange(gMainRoutineWork.currentOwner);
    Actor_SetAnimation(13, 6);
    Event_ShowMessage(13, 0);
  }
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
void MarsLighthouseStarRoom_CreateMarsStar(s32 rise) {
  struct Actor **slot = &gMarsLighthouseStarRoom_MarsStar;
  u8 *block;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 zero;
  s32 size;

  zero = 0;
  object = FieldActor_Create(0x1e9, px(702), 0, px(224));
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
  Item_LoadIcon(ITEM_MARS_STAR);
  block += 0x400;
  Vram_Load(sprite->vramBlock, 128, block);
  HeapSlot_Release(68);
  if (rise) {
    System_Wait(1);
    (*slot)->velocityY = px(5) / 2;
    System_Wait(4);
    Event_PlaySound(SE_CLICK);
    System_Wait(8);
    Event_PlaySound(SE_CLICK);
  }
}

#include "text/msg_ids.h"

void Event_Start(void);
void Event_End(void);

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
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
    object = gMarsLighthouseStarRoom_MarsStar;
    object->motionFlags = 0;
    object->speedLimit = px(4);
    object->acceleration = px(2);
    FieldActor_SetMoveTargetFixed(object, leader->x, leader->y + px(32),
                                  leader->z);
    FieldActor_WaitForMove(gMarsLighthouseStarRoom_MarsStar);
    Event_PlaySound(MUS_083);
    PartyInventory_Remove(224);
    Inventory_AddItem(4, ITEM_MYTHRIL_BAG);
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
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveTo(680, CAMERA_KEEP, 146, 1);
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
      OBJ_PLTT_COLOR(0, 15) = white.value;
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
    Actor_SetPosition(13, 700, 224);
    Actor_SetAnimation(13, 10);
    MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, id);
  } else {
    Actor_SetPosition(14, 660, 216);
    Actor_SetAnimation(14, 9);
    MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
  }
}

struct EventNote {
  u8 unk000[0x15a];
  u16 unk15a;
};

void MarsLighthouseStarRoom_ExamineStatue(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar07, 1);
  ((struct EventNote *)work)->unk15a = 1;
  Event_End();
}

extern char SceneId_MarsLighthouseRoomM3;
extern char SceneId_Naribwe;

void Scene_PlayMusic(void);
void Func_02001424(void);

#if !TLA_PATCH_1

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
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(1);
  Event_PlaySound(SE_GEM_SET);
  slot = &gMarsLighthouseStarRoom_MarsStar;
  object = FieldActor_Create(0x1e9, px(680), px(8), px(147));
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
    Item_LoadIcon(ITEM_MARS_STAR);
    Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 680, 186);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 40);
  Event_PlaySound(MUS_000);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10006, 0);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(Func_02001424, 0x480);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
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
  TileMap_ConvertBuffer(
      gMainRoutineWork.map +
          ((s32)&SceneId_MarsLighthouseRoomM3 - (s32)&SceneId_Prox2),
      screen);
  zero = 0;
  *screen = zero;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 186, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  object = *slot;
  if (object != 0) {
    object->motionFlags = 3;
    object->velocityY = px(2);
    object->speedLimit = px(2);
    object->acceleration = px(1);
    FieldActor_SetMoveTarget(object, 672, 0, 160);
    FieldActor_WaitForMove((s32)*slot);
    Event_PlaySound(SE_CLICK);
    object = *slot;
    object->acceleration = px(1) / 2;
    object->velocityY = px(4);
    object->speedLimit = px(1);
    FieldActor_SetMoveTarget(object, 664, 0, 158);
    FieldActor_WaitForMove((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.6);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 670, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object = *slot;
  if (object != 0)
    Actor_Release(object);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 680, 186);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Scene_PlayMusic();
  Event_SetFlag(0xa30);
  gMainRoutineWork.battleBackground = (s32)&SceneId_Naribwe;
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
  if (flag == 0) {
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered((s32)MsgMarsStar09, 1);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    System_Wait(1);
    Event_PlaySound(SE_GEM_SET);
    slot = &gMarsLighthouseStarRoom_MarsStar;
    object = (struct Actor *)FieldActor_Create(0x1e9, px(680), px(8), px(147));
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
      Item_LoadIcon(ITEM_MARS_STAR);
      Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
      HeapSlot_Release(68);
    }
    FieldEvent_Wait(20);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 680, 186);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 80);
    Actor_ShowEmote(gMainRoutineWork.currentOwner,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    object = *slot;
    if (object != 0) {
      object->motionFlags = 3;
      object->velocityY = px(2);
      object->speedLimit = px(2);
      object->acceleration = px(1);
      FieldActor_SetMoveTarget(object, 672, 0, 160);
      FieldActor_WaitForMove((s32)*slot);
      Event_PlaySound(SE_CLICK);
      object = *slot;
      object->acceleration = px(1) / 2;
      object->velocityY = px(4);
      object->speedLimit = px(1);
      FieldActor_SetMoveTarget(object, 664, 0, 158);
      FieldActor_WaitForMove((s32)*slot);
    }
    FieldEvent_Wait(20);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.6);
    Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 670, 170);
    FieldEvent_Wait(1);
    Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
    object = *slot;
    if (object != 0)
      Actor_Release(object);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
    Scene_PlayMusic();
    Event_End();
    goto L_020093b0;
  }
  screen = (u16 *)Ram_HeapSlots->block32;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(1);
  Event_PlaySound(SE_GEM_SET);
  slot2 = &gMarsLighthouseStarRoom_MarsStar;
  object2 = FieldActor_Create(0x1e9, px(680), px(8), px(147));
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
    Item_LoadIcon(ITEM_MARS_STAR);
    Vram_Load(sprite2->vramBlock, 128, buffer2 + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 680, 186);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 40);
  Event_PlaySound(MUS_000);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10006, 0);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(Func_02001424, 0x480);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
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
  TileMap_ConvertBuffer(
      gMainRoutineWork.map +
          ((s32)&SceneId_MarsLighthouseRoomM3 - (s32)&SceneId_Prox2),
      screen);
  *screen = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 186, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  object2 = *slot2;
  if (object2 != 0) {
    object2->motionFlags = 3;
    object2->velocityY = px(2);
    object2->speedLimit = px(2);
    object2->acceleration = px(1);
    FieldActor_SetMoveTarget(object2, 672, 0, 160);
    FieldActor_WaitForMove((s32)*slot2);
    Event_PlaySound(SE_CLICK);
    object2 = *slot2;
    object2->acceleration = px(1) / 2;
    object2->velocityY = px(4);
    object2->speedLimit = px(1);
    FieldActor_SetMoveTarget(object2, 664, 0, 158);
    FieldActor_WaitForMove((s32)*slot2);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.6);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 670, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object2 = *slot2;
  if (object2 != 0)
    Actor_Release(object2);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Scene_PlayMusic();
  Event_SetFlag(0xa30);
  Event_SetFlag(0xa9e);
  gMainRoutineWork.battleBackground = (s32)&SceneId_Naribwe;
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
    /* FAKEMATCH(barrier): an empty asm clobbering r0. */
    asm("" : : : "r0");
    object->unk5c[0] = 2;
    object->motionFlags = 0;
    object->timer = 0;
    object->priorityFlags &= 0xfe;
    sprite = object->sprite;
    sprite->priority = 2;
    Actor_SetSpriteFlags(object, 0);
    object->scaleX = Q16_ONE / 2;
    object->scaleY = Q16_ONE / 2;
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

void Ride_UpdateCarried(struct Actor *object) {
  struct RideWork__s1424 *work =
      (struct RideWork__s1424 *)(gInventorySnapshot + 0x188);
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  s32 direction;
  s32 id;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (actor->y == actor->terrainHeight && Func_02001528(object, actor) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->value68 = 0;
      {
        u16 *cell = &work->direction;

        if (object->unk4c > 0) {
          direction = 1;
          *cell = direction;
        } else {
          direction = 0xffff;
          *cell = direction;
        }
      }
    }
    object->priorityFlags = 2;
    {
      s32 speed = object->unk4c;

      object->unk4c = 0;
      object->z -= speed;
      object->gravity += speed >> 2;
    }
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {
    actor = Actor_Get(id);

    if (actor->y == actor->terrainHeight && Func_0200158c(object, actor)) {
      if (work->slots[i].blocked == 0) {
        s32 speed;

        object->priorityFlags = 2;
        speed = object->unk4c;
        object->unk4c = 0;
        object->z -= speed;
        object->gravity += speed >> 2;
      } else {
        work->blocked = 1;
      }
    }
  }
}

extern u32 gData_02004744[];

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct Actor *object, const void *script);

#include "shared/actor_reset_scripted.h"

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

void Ride_UpdateLanding(struct Actor *object) {
  struct RideWork__spawn *work =
      (struct RideWork__spawn *)(gInventorySnapshot + 0x188);
  struct Actor *other = Actor_Get(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  struct RideDirection *direction;

  object->z += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->x += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (other->y == other->terrainHeight && Func_02001528(object, other) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->direction.unk02 = 0;
      direction = &work->direction;
      if (object->unk4c > 0)
        direction->unk00 = 1;
      else
        direction->unk00 = 0xffff;
    }
    object->priorityFlags = 2;
    object->z -= object->bounce;
    object->gravity += object->bounce >> 2;
    object->unk4c = 0;
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->slots[i].id;

    if (id == 0)
      break;
    other = Actor_Get(id);
    if (other->y == other->terrainHeight && Func_0200158c(object, other)) {
      if (work->slots[i].blocked == 0) {
        object->priorityFlags = 2;
        object->z -= object->unk4c;
        object->gravity += object->unk4c >> 2;
        object->unk4c = 0;
      } else {
        work->blocked = 1;
      }
    }
  }
}

void Actor_SetupScriptedPart(struct Actor *object, s32 priority);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02004744; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Actor_SetupScriptedPart(struct Actor *object, s32 part) {
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteParts *)object->sprite)->part = part;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gData_02004744);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

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

s32 Actor_IsAbove(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x, b->x) < 0xc0000) {
    if ((u32)(a->y - b->y - 1) <= 0xffffe) {
      if (DIFF(a->z, b->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}

#define ABS(value) ((value) < 0 ? -(value) : (value))

#include "shared/ride_update_drift.h"

extern u32 gData_02004744[];

#include "shared/actor_setup_scripted.h"

void Actor_SetBlendMode(struct Actor *object, u32 value);
void Ride_UpdateDrift();
/* VIEW(s1b78): routine Actor_SetupScripted as void Actor_SetupScripted(struct
 * Actor *object, s32 priority); the definition has void
 * Actor_SetupScripted(struct Actor *object); not tried: Actor_SetupScripted's
 * declaration holds a FAKEMATCH tag. */
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
     * extension and the pool word 0xffff8000. */
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

void Beam_DrawTargets(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  Camera *camera;
  Position *position;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)(gInventorySnapshot + 0x188);
  camera = IWRAM->camera;
  entry = state->entries;
  position = &camera->position;
  cameraX = position->x;
  cameraX &= 0xffff0000;
  cameraZ = position->z & 0xffff0000;
  saved[1] = (*(s32 **)camera)[1];
  saved[0] = IWRAM->unk_6c.word;
  if (state->unk_6 != 0 && state->unk_8 == 0 && state->callback != 0)
    state->callback();
  v = (u16)state->unk_6;
  state->unk_8 = v;
  state->unk_6 = 0;
  for (i = 3; i >= 0; i--, entry++) {
    Actor *actor;
    s32 flag, x, y, unk6, value, dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Get(FLAG_MENU_OPEN);
    if (*(s16 *)(saved[0] + 358) != 0)
      flag |= 1;
    mark = &actor->flag5b;
    *mark = flag;
    if (state->unk_e != 0)
      *mark = GameFlag_Get(FLAG_MENU_OPEN);
    dx = actor->x;
    dz = actor->z;
    dx -= cameraX;
    /* FAKEMATCH(barrier): the empty asm on dx changes its allocation rank.
     */
    asm("" : : "r"(dx));
    y = actor->y;
    dz -= cameraZ;
    y = dz - y;
    unk6 = entry->unk_6;
    x = dx >> 16;
    y = (y >> 16) - 8;
    if (unk6 == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & 240;
      switch (y) {
      case 128:
        Func_02001b78__s1bf8(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 48:
        Func_0200196c(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 32:
        Func_020019fc(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 16:
        Func_020019b4(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 0:
        Func_02001780(&entry->actor->x, entry->unk_c, unk6);
        break;
      }
      value = 8;
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}
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

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

#undef MarsLighthouseRoomF_SlideOnIce

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_Release(struct Actor *object);

#include "shared/ride_release_handle.h"

void Func_02001de4(s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->value0e = value;
}

/* VIEW(ride_handle): tag struct MarsLighthouseBeam as struct
 * MarsLighthouseBeam__ride_handle; module/scene/scene.h has another definition;
 * not tried: struct MarsLighthouseBeam__ride_handle was not found. */
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

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    i = 0;
    id = *targets++;
    {
      /* FAKEMATCH(dead): a test combine folds, keeping the first loop's refs
       * unweighted. */
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
         * unweighted. */
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
  Task_AddFrame(Beam_DrawTargets, 0x480);
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

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

void Func_02001f98(s32 index, s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->entries[index].c = value;
}

s32 Func_02001fa8(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

#include "shared/ride_set_slot.h"

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

/* VIEW(ride_handle): routine Unit_Get as struct BattleUnit *Unit_Get(s32
 * owner); scene/mars_lighthouse_star_room has void *Unit_Get(s32 owner); not
 * tried: Unit_Get's declaration holds a FAKEMATCH tag. */
struct BattleUnit *Unit_Get__ride_handle(s32 owner) __asm__("Unit_Get");
#define Unit_Get Unit_Get__ride_handle

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect/advance_spin.c"

#include "shared/actor_blink.h"

#undef Unit_Get
#undef MarsLighthouseRoomF_SlideOnIce
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
s32 MarsLighthouseRoomF_SlideOnIce(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  s32 frame[3]; // Unused: gives the stack frame its 12 bytes.

  if (event->started == 0) {
    ScreenTransition_Start(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Event_ClearFlag(FLAG_MENU_OPEN);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  /* FAKEMATCH(wrapper): Party_TakeFallDamage called through a cast to s32. */
  ((s32 (*)(s32, s32))Party_TakeFallDamage)(-10, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Event_SetFlag(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(FLAG_MENU_OPEN);
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
#ifdef UBFIX
  return 0;
#endif
}

#include "shared/ride_if_active.h"

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
    y = c * 8 + actor->y + px(4);
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
      object->bounce = Q16_ONE / 2;
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
void Embers_Spawn(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  i = 1;
next:
  {
    struct Actor *object = FieldActor_Create(325, x, y, z);

    if (object != NULL) {
      s32 angle;
      s32 rise;
      s32 radius;
      u32 size;
      s32 scale;

      rise = speed + (Random16() >> 4) - (Random16() >> 4);
      angle = 0x4000 + rise;
      radius = Random16() * 8;
      size = Random16();
      Actor_StartScript(object, gData_020047b8);
      Actor_SetSpriteFlags(object, 0);
      /* FAKEMATCH(temporary): the scale is taken before the velocity
       * stores. */
      scale = (size >> 1) + 0x4000;
      object->velocityY = rise + 0x14000;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = object->scaleX;
      object->bounce = Q16_ONE / 2;
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

void Func_02002738(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s236c): tag struct Work020023c4 as struct Work020023c4__s236c; s2148.c
 * has another definition; not tried: struct Work020023c4__s236c was not found.
 */
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

void Actor_RollAndCrumble(struct Actor *actor) {
  struct Vector3 position;
  struct WorkEvent *work = Ram_HeapSlots->eventWork;
  s32 state = actor->timer;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *cellsA;
    struct MapCell *cellsB;
    s32 widthA;
    s32 widthB;
    s32 offset;
    s32 zB;
    s32 tileZ;
    s32 heightA;
    s32 heightB;
    s32 x;
    s32 z;
    s32 xA;
    s32 xB;
    s32 zA;
    s32 flag;
    struct MapWork *map;
    struct MapCell *cell;
    struct WorkEvent *work2;
    struct Work020023c4__s236c *other;
    struct Actor *target;

    flag = Event_GetFlag(FLAG_MENU_OPEN);
    if (work->unk166 != 0)
      flag |= 1;
    actor->movementState = flag;

    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    FieldVector_AddPolarOffset(px(8), actor->facing, &position);
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, position.x, position.z) ==
        7) {
      actor->targetZ = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetX = 0x80000000;
      actor->acceleration = px(1) / 4;
      actor->timer = 1;
      Event_PlaySound(SE_CRASH);
      MapGrid_SetShake(5, 5, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (actor->watchedObject != NULL)
        ((void (*)(struct Actor *))actor->watchedObject)(actor);
    }

    map = Ram_HeapSlots->mapWork;
    tileX = actor->x >> 20;
    tileZ = actor->z >> 20;
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;
    heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;
    widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;
    heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;

    for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,
        zA = tileZ + heightA - 1;
         dz <= 1; zB++, z++, zA++, dz++) {
      for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,
          xA = tileX + widthA - 1;
           dx <= 1; xB++, x++, xA++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)cellsA);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_573);
          Embers_Spawn((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                       actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_572);
          Embers_Spawn((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                       actor->facing);
        }
      }
    }

    if (actor->scaleX < Q16_ONE) {
      actor->scaleX += Q16_ONE / 16;
      actor->scaleY += Q16_ONE / 16;
    }
    FieldActor_SetMoveTargetFixed(actor, position.x, position.y, position.z);

    work2 = Ram_HeapSlots->eventWork;
    other = (struct Work020023c4__s236c *)(gInventorySnapshot + 0x188);
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if (ABS(actor->x - target->x) < 0x140000 &&
        (u32)(actor->y - target->y + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z - target->z) < 0x140000) {
      other->unk04 = 2;
      other->unk6c = actor;
      work2->unk154 = other->unk0a + 2;
    }
  } else if (state == 1) {
    actor->movementState = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= Q16_ONE / 16;
      actor->scaleY -= Q16_ONE / 16;
      Func_020024d4(actor);
    } else {
      actor->z = 0;
      actor->y = 0;
      actor->x = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}
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
/// Event_PlaySound(SE_104). The twin of overlay 6ac's Func_02002e6c.
#include "shared/actor_launch_ahead.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

extern u8 gInventorySnapshot[];

#include "shared/actor_park.h"

void Actor_SteerByPad(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->timer != 0) {
    object->timer--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (Event_GetFlag(FLAG_MENU_OPEN))
    angle = -1;
  else
    angle = gData_020047fc[(gInput.held >> 4) & 15];
  if (angle == -1) {
    Actor_SelectAnimation(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

void Func_02002d90(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Event_SetFlag(FLAG_MENU_OPEN);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Event_ClearFlag(FLAG_MENU_OPEN);
  }
}

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gData_0200483c[];

void Leader_SlideOnIce(void) {
  struct Vector3 probe;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 startX;
  s32 startZ;
  u8 *layer;
  s32 radius;
  struct Actor *actor;
  struct Vector3 *position;
  struct Vector3 *next;
  s32 heading;
  s32 nextBehavior;
  s32 distance;
  struct Actor *other;
  u8 *flags;
  s32 i;
  s32 dx;
  s32 dz;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);

top:
  Func_02002d90();
  actor->speedLimit = px(1);
  actor->acceleration = px(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  position = &probe;
  position->x = startX = (actor->x & 0xfff00000) + px(8);
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + px(8);
  x = startX;
  z = startZ;
  layer = &actor->terrainId;
  startBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, x, z);
  dz = z - actor->z;
  dx = x - actor->x;
  distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz);
  if (distance < px(4)) {
    heading = gData_0200483c[(gInput.held >> 4) & 15];
    if ((heading << 16) == -0x10000) {
      return;
    }
  } else {
    heading = (s16)ArcTan2(z - actor->z, x - actor->x);
    heading = (s16)((heading + 0x2000) & -0x4000);
  }
  FieldVector_AddPolarOffset(px(16), heading, &probe);
  nextBehavior =
      FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >
          px(8)) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = startX;
  position->z = startZ;
  actor->speedLimit = px(2);
  actor->acceleration = 0x1999;
  actor->timer = 0;
  FieldActor_SetMoveTargetFixed(actor, startX, actor->y, startZ);
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  FieldActor_WaitForMove(actor);
  actor->update = Actor_SteerByPad;
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > px(8)) {
    goto stop;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (i = 0; i < 64; i++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading
       * next and actor. */
      asm(""
          :
          : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
            "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)
          : SFP_CLOBBER, "r4");
      if (Field_CheckSpheresOverlap(&other->x, radius, &probe,
                                    other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = px(2);
  actor->acceleration = 0x1999;
  FieldActor_SetMoveTargetFixed(actor, next->x, next->y, next->z);
  FieldActor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  {
    struct Vector3 *step;

    step = &probe;
    FieldVector_AddPolarOffset(px(16), heading, step);
    /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the
     * address. */
    step = NULL;
  }
  next = &probe;
  nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = px(2);
  actor->acceleration = px(1);
  FieldActor_SetMoveTargetFixed(actor, x, actor->y, z);
  FieldActor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = px(1) / 4;
  Actor_SelectAnimation(actor, 1);
}

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

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

void Leader_SetSlideStopped(s32 value) {
  gMarsLighthouseStarRoom_SlideStopped = value;
}

void Leader_SlideOnIceAhead(void) {
  struct Vector3 probe;
  s32 heading;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 radius;
  struct Actor *actor;
  struct Vector3 *next;
  s32 nextBehavior;
  u8 *layer;
  struct Actor *other;
  struct Vector3 *position;
  u8 *flags;
  s32 i;
  s32 cell;
  s32 angle;
  s32 zero;
  struct Vector3 *step;
  s32 startX;
  s32 startZ;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
  heading = (s16)((actor->facing + 0x2000) & -0x4000);

top:
  Func_02002d90();
  actor->speedLimit = px(1);
  actor->acceleration = px(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  /* Default: one zero, spilled and reloaded at each use. */
  zero = 0;
  actor->velocityX = zero;
  actor->velocityZ = zero;
  /* FAKEMATCH(temporary): cell holds the masked x. */
  cell = actor->x & 0xfff00000;
  i = px(8);
  position = &probe;
  position->x = startX = cell + i;
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + i;
  x = startX;
  z = startZ;
  startBehavior = FieldMapGrid_GetCellBehaviorAt(*(layer = &actor->terrainId),
                                                 startX, startZ);
  FieldVector_AddPolarOffset(px(16), heading, position);
  nextBehavior =
      FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y > i) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = x;
  position->z = z;
  actor->speedLimit = px(2);
  actor->acceleration = 0x1999;
  actor->timer = zero;
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  actor->update = Actor_SteerByPad;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading
   * nextBehavior twice. */
  asm("" : : "r"(nextBehavior), "r"(nextBehavior) : SFP_CLOBBER);
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > px(8)) {
    goto stop;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (i = 0; i < 64; i++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading
       * next and actor. */
      asm(""
          :
          : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
            "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)
          : SFP_CLOBBER, "r4");
      if (Field_CheckSpheresOverlap(&other->x, radius, &probe,
                                    other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = px(2);
  actor->acceleration = 0x1999;
  FieldActor_SetMoveTargetFixed(actor, next->x, next->y, next->z);
  FieldActor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  /* FAKEMATCH(barrier): the asm tied to heading, with an r4 clobber. */
  asm("" : "=r"(angle) : "0"(heading) : "r4");
  step = &probe;
  FieldVector_AddPolarOffset(px(16), angle, step);
  /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address.
   */
  step = NULL;
  next = &probe;
  nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = px(2);
  actor->acceleration = px(1);
  FieldActor_SetMoveTargetFixed(actor, x, actor->y, z);
  FieldActor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = px(1) / 4;
  Actor_SelectAnimation(actor, 1);
}

#include "../field/object/set_sprite_priority.c"

#undef MarsLighthouseRoomF_SlideOnIce

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

s32 CrackedTiles_Find(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gData_02004cf4;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *cells;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 x0, z0, width, height, i, j;
  u32 *buffer;
  u32 event, last;
  s32 cx, cz;
  s32 k;

  Iwram_ClearWordsEntry(table, 4);
  table->kind = kind;
  table->firstFlag = flag;
  table->flagByte = flagByte;
  table->x = x;
  table->z = z;
  table->current = -1;
  table->count = 0;
  map = (struct CellMapWork *)Ram_HeapSlots->mapWork;
  layer = &map->layers[0];
  cells = layer->cells;
  x0 = (layer->originX + map->minX) >> 20;
  z0 = (layer->originZ + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  /* Goto loops: as for loops (either one) loop.c moves the 64 of the cell
   */
  /* copy's masks out of them and 321 of 268 instructions differ. */
  j = 0;
  rows = cells;
  if (j < height) {
  row:
    cell = rows + (j << 7);
    i = 0;
    if (i < width) {
    column:
      event = cell->event;
      if (event != 0 && event >= kind) {
        last = kind + 1;
        if (event <= last) {
          cx = i + x0;
          entry->x = cx;
          cz = j + z0;
          entry->z = cz;
          entry->flag = flag;
          flag++;
          if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
            if (event == last)
              Event_SetFlag(entry->flag);
          } else if (Event_GetFlag(entry->flag)) {
            MapGrid_CopyCells(table->x, table->z + 1, 1, 1,
                              cx | (s16)(table->x & 64),
                              cz | (s16)(table->z & 64));
          }
          entry++;
          table->count++;
        }
      }
      i++;
      cell++;
      if (i < width)
        goto column;
    }
    j++;
    if (j < height)
      goto row;
  }
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;

    entry = table->entries;
    k = 0;
    if (k < table->count) {
    search:
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = k;
      entry++;
      k++;
      if (k < table->count)
        goto search;
    }
  }
  buffer = Heap_Alloc(0x100);
  {
    u32 *word = buffer;

    k = 63;
  fill:
    *word++ = 0x11111111;
    if (--k >= 0)
      goto fill;
  }
  table->tile = VramCache_FindFreeEntry();
  Vram_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(CrackedTiles_Update, 0x480);
  return table->count;
}

#include "shared/leader_center_in_cell.h"

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall_event.h"

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveToFixed(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetShakeFixed(a, b, c);
}

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(s3e04): routine CrackedTile_FallEvent as this part declared it; the
 * joined file has another; not tried: CrackedTile_FallEvent's declaration holds
 * a FAKEMATCH tag. */
void CrackedTile_FallEvent__s3e04(
    struct CellTarget *work,
    struct CellTargetEntry *entry) __asm__("CrackedTile_FallEvent");
#define CrackedTile_FallEvent CrackedTile_FallEvent__s3e04

s32 CrackedTiles_Step(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &gData_02004cf4;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        CrackedTile_Crack(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      CrackedTile_FallEvent(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = FieldGameFlag_GetByte(gData_02004cf4.flagByte);
  s32 dz = FieldGameFlag_GetByte(gData_02004cf4.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gData_02004cf4.flagByte, 0);
    GameFlag_SetByte(gData_02004cf4.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Map_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}

#include "text/msg_ids.h"

void MarsLighthouseStarRoom_ExamineStatue(void);

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

u32 gData_02004744[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_FREE,                                                                 
  };

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

u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

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
    SCENE_EXITS(MarsLighthouseStarRoom1),
    SCENE_EXIT(1, MarsLighthouseStarRoom1, 2),
    SCENE_EXIT(2, MarsLighthouseStarRoom1, 1),
    SCENE_EXIT(3, MarsLighthouseStarRoom1, 4),
    SCENE_EXIT(4, MarsLighthouseStarRoom1, 3),
    SCENE_EXIT(5, MarsLighthouseEntrance7, 5),
    SCENE_EXIT(6, MarsLighthouseEntrance6, 6),
    SCENE_EXIT(7, MarsLighthouseEntrance7, 7),
    SCENE_EXIT(8, MarsLighthouseEntrance6, 8),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gData_02004910, 712, 0, 264,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gData_02004910, 648, 0, 264,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(291, CONDITION_ALWAYS, 7, 680, 0, 240, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(181, 0xa30, 7, 664, 0, 216, FACING_SOUTH, TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(181, 0xa30, 7, 696, 0, 216, FACING_SOUTH, TALK_FACING_KEEP,
                    1),
    ACTOR_PLACEMENT(33, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(23, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {81, 2, CONDITION_ALWAYS, 2},
    {81, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 90, 0x1a4b, EVENT(MarsLighthouseRoomF_SlideOnIce)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Func_02000060)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 9,
     CONDITION_ALWAYS, EVENT(Func_02000074)},
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
    {USE_ITEM(ITEM_MYTHRIL_BAG), 1045, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_UseMarsStar)},
    {TRIGGER_TABLE_END},
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};
#undef CrackedTile_FallEvent
#undef MarsLighthouseRoomF_SlideOnIce
