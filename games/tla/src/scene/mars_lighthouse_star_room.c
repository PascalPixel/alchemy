/// @file
/// The scene code of overlay 0x6ab, in ROM order.

#include "shared/mars_lighthouse.h"
#include "shared/actors.h"
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/scene/scene.h"
#include "module/tla.h"

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
void MarsLighthouseStarRoom_SlideOnIce(void);

s32 Func_02000038(struct Actor *object) {
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

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000060(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[3384] = 1;
}

void Func_02000074(void) { Func_020014f4(); }

void Func_0200007c(void) {}

#include "mars_lighthouse_star_room.h"
#include "iwram_call.h"

extern const u8 gMarsLighthouseStarRoom_ScatterIceChipsScript[];

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

#include "version.h"

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

DEFINE_ACTOR_FREEZE(Func_020002d4)

#include "main_routine_work.h"

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
void Func_02001df0(s32 a, s32 b, s32 c, s32 d);
void Func_02003e04(void);
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
        Func_02003e04();
    }
    if (entrance == 9 || entrance == 10) {
      if (!Event_GetFlag(0x109))
        Func_02003e04();
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

#include "scene_ids.h"

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
extern u8 gData_0200491c[];
extern u8 gData_02004958[];
extern u8 gData_020049a0[];
extern u8 gData_020049dc[];

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

#include "event_work.h"

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

#include "field_sprite.h"

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

extern const u8 gData_020044b8[];
extern const u8 gData_020045b8[];

s32 Func_02002a58(s32 id, s32 a, s32 b, s32 angle, const void *path);

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

#include "shared/mars_lighthouse_entrance_room.h"
#include "field/event_work.h"

/* VIEW(s1424): tag struct RideSlot as struct RideSlot__s1424;
 * mars_lighthouse_star_room.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1223: structure has no member
 * named `id'. */
struct RideSlot__s1424 {
  u16 id;
  u16 blocked;
};

/* VIEW(s1424): tag struct RideWork as struct RideWork__s1424;
 * mars_lighthouse_star_room.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1191: structure has no member
 * named `value68'. */
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

DEFINE_RIDE_UPDATE_CARRIED(Func_020015ec, RideWork__s1424, Func_02001528,
                           Func_0200158c)

extern const u8 gData_02004744[];

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct Actor *object, const void *script);

DEFINE_ACTOR_RESET_SCRIPTED(Func_02001750, gData_02004744)

void Func_02001780(s32 *position, s32 speed, s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    ((void (*)())Func_02001750)(object, 2);
    object->update = Func_020015ec;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

#include "shared/mars_lighthouse_entrance_room.h"

/* VIEW(spawn): tag struct RideSlot as struct RideSlot__spawn;
 * mars_lighthouse_star_room.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1340: structure has no member
 * named `id'. */
struct RideSlot__spawn {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(spawn): tag struct RideWork as struct RideWork__spawn;
 * mars_lighthouse_star_room.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1318: structure has no member
 * named `direction'. */
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

DEFINE_RIDE_UPDATE_LANDING(Func_020017c8, RideWork__spawn, Func_02001528,
                           Func_0200158c)

void Func_02001928(struct Actor *object, s32 priority);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02004744; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
DEFINE_ACTOR_SETUP_SCRIPTED_PART(Func_02001928, gData_02004744)

#define DEFINE_FUNC_0200196C_97(name, call1)                                   \
  void name(s32 *position, s32 speed, s32 flag) {                              \
    struct Actor *object =                                                     \
        FieldActor_Create(366, position[0], position[1], position[2]);         \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = 0;                                                      \
      object->gravity = 0;                                                     \
      object->unk4c = -speed;                                                  \
      Func_02001928(object, 3);                                                \
      object->update = call1;                                                  \
      if (flag != 0)                                                           \
        object->unk5c[0] = 2;                                                  \
      Actor_SelectAnimation(object, 2);                                        \
    }                                                                          \
  }

DEFINE_FUNC_0200196C_97(Func_0200196c, Func_020015ec)

void Func_020019b4(s32 *position, s32 speed, s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Func_02001928(object, 3);
    object->update = Func_020017c8;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

DEFINE_FUNC_0200196C_97(Func_020019fc, Func_020017c8)

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

DEFINE_RIDE_UPDATE_DRIFT(Func_02001a9c, Func_02001a44)

extern const u8 gData_02004744[];

DEFINE_ACTOR_SETUP_SCRIPTED(Func_02001b38, Actor_SelectAnimation,
                            gData_02004744)

void Actor_SetBlendMode(struct Actor *object, u32 value);
void Func_02001a9c();
/* VIEW(s1b78): routine Func_02001b38 as void Func_02001b38(struct Actor
 * *object, s32 priority); the definition has void Func_02001b38(struct Actor
 * *object); tried as the definition's void Func_02001b38(void), which the
 * object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1504: too many arguments to
 * function `Func_02001b38'. */
extern void Func_02001b38__s1b78(struct Actor *object,
                                 s32 priority) __asm__("Func_02001b38");

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
    object->update = Func_02001a9c;
    Actor_SetBlendMode(object, 1);
  }
}

/* VIEW(s1bf8): tag struct Actor as struct Actor__s1bf8; actor.h has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1601:
 * structure has no member named `flag5b'. */
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
 * it has another declaration; tried as the rest of the object declares it, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1636: too many arguments to
 * function `Func_02001b78'. */
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
 * mars_lighthouse_star_room.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room.c:1684: structure has no member
 * named `value0c'. */
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

// ---- scene/mars_lighthouse_star_room/ride_handle.c ----

/// Scene code of overlay 0x6ab, in ROM order.

#include "shared/mars_lighthouse.h"
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/scene/scene.h"
#include "module/tla.h"

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
void Actor_Release(struct Actor *object);
void MarsLighthouseStarRoom_SlideOnIce(void);

DEFINE_RIDE_RELEASE_HANDLE(Func_02001dd0, Actor_Release)

void Func_02001de4(s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->value0e = value;
}

#include "io_reg.h"
#include "field.h"

/* VIEW(ride_handle): tag struct MarsLighthouseBeam as struct
 * MarsLighthouseBeam__ride_handle; module/scene/scene.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/mars_lighthouse_star_room/ride_handle.c:135:
 * structure has no member named `stopCount'. */
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

DEFINE_RIDE_ADVANCE_COUNTER(Func_02001f50)

DEFINE_RIDE_SET_ENTRY(Func_02001f78)

void Func_02001f98(s32 index, s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->entries[index].c = value;
}

s32 Func_02001fa8(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

DEFINE_RIDE_SET_SLOT(Func_02001fb8)

#include "main_routine_work.h"
#include "battle_unit.h"

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

struct BattleUnit *Unit_Get(s32 owner);

DEFINE_PARTY_TAKE_FALL_DAMAGE(Func_02001fcc)

#include "../field/field_effect/advance_spin.c"

DEFINE_ACTOR_BLINK(Func_02002120, 10, 4, 0)

// ---- scene/mars_lighthouse_star_room/s2148.c ----

/// Scene code of overlay 0x6ab, in ROM order.

#include "shared/mars_lighthouse.h"
#include "version.h"

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "vector3.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "display_transition.h"
#include "event_work.h"
#include "field.h"
#include "item_icon.h"
#include "mars_lighthouse_star_room.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/system/system.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
void Func_02002120(void);
extern u8 gInventorySnapshot[];

#define SHARED_F02002148_NAME Func_02002148
#define SHARED_F02002148_ID2 Func_02001fcc
#define SHARED_F02002148_ID3 Func_02002120
#define SHARED_F02002148_ID4 Actor_SelectAnimation
#define SHARED_F02002148_ID5 MapGrid_GetCellAttributeLowNibbleOrBump
#define SHARED_F02002148_ID6 MapGrid_CheckCollision
#define SHARED_F02002148_ID7 Field_CheckSpheresOverlap
#include "shared/func_02002148.c"
#undef SHARED_F02002148_NAME
#undef SHARED_F02002148_ID2
#undef SHARED_F02002148_ID3
#undef SHARED_F02002148_ID4
#undef SHARED_F02002148_ID5
#undef SHARED_F02002148_ID6
#undef SHARED_F02002148_ID7

#include "frame.h"

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

extern u16 gData_02004cf0[];

void Func_02002354(void) {
  if (((struct RideWork *)(gInventorySnapshot + 0x188))->active)
    Func_02002148();
}

#include "mars_lighthouse_star_room.h"

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
  Func_02001fcc();
#else
  /* FAKEMATCH(wrapper): Func_02001fcc called through a cast to
   * s32 (*)(s32, s32) with -10 and 0; as Func_02001fcc(), 3 of 138
   * instructions differ in tla-en. */
  ((s32 (*)(s32, s32))Func_02001fcc)(-10, 0);
#endif
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Event_SetFlag(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(0x106);
  leader->update = (void (*)(void))Func_02002120;
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

#include "system.h"

extern u16 gData_02004cf0[];

DEFINE_RIDE_IF_ACTIVE(MarsLighthouseStarRoom_ContinueSlide,
                      MarsLighthouseStarRoom_SlideOnIce)

s32 MarsLighthouseStarRoom_IsSliding(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->active;
}

#include "iwram_call.h"
#include "field.h"

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

extern u8 gData_02004774[];

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

extern const u8 gData_020047b8[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
DEFINE_EMBERS_SPAWN(Func_02002658, gData_020047b8)

void Func_02002738(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#include "map.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s236c): tag struct Work020023c4 as struct Work020023c4__s236c; s2148.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room/s2148.c:619: structure has no
 * member named `unk6c'. */
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

DEFINE_ACTOR_ROLL_AND_CRUMBLE(Func_0200274c, Work020023c4__s236c,
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

#include "field_sprite.h"
#include "scene_tables.h"
#include "system/vector3.h"

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_0200274c as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// The twin of overlay 6ac's Func_02002e6c.
DEFINE_ACTOR_LAUNCH_AHEAD(Func_02002ad0, Func_0200274c)

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

DEFINE_ACTOR_POP_UP_SLOW(Func_02002b98, Actor_SelectAnimation)

DEFINE_ACTOR_POP_UP(Func_02002c44, Actor_SelectAnimation)

#include "macros.h"

extern u8 gInventorySnapshot[];
extern u16 gData_02004cf0[];

DEFINE_ACTOR_PARK(Func_02002ce8, Func_02002ad0)

DEFINE_ACTOR_STEER_BY_PAD(Func_02002d04, gData_020047fc, Actor_SelectAnimation)

void Func_02002d90(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Event_SetFlag(0x106);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Event_ClearFlag(0x106);
  }
}

#include "system/iwram_call.h"
#include "system/input.h"
#include "system/main_routine_work.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gData_0200483c[];

DEFINE_LEADER_SLIDE_ON_ICE(Func_02002dc4, Func_02002d90, gData_0200483c,
                           Actor_SelectAnimation, Func_02002d04,
                           Field_CheckSpheresOverlap)

#include "input.h"

/* VIEW(s3070): variable gData_02004cf0 as extern volatile s16 gData_02004cf0;
 * s2a58 has extern u16 gData_02004cf0[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room/s2148.c:1014:
 * `gData_02004cf0__s3070' undeclared (first use in this function). */
extern volatile s16 gData_02004cf0__s3070 __asm__("gData_02004cf0");
extern s16 gData_0200483c[];
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Actor_HasArrived(struct Actor *object);
/* VIEW(s3070): routine Field_CheckSpheresOverlap as s32
 * Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
 * s2dc4 has s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, struct
 * Position *b, s32 radiusB); tried as the rest of the object declares it, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room/s2148.c:1093:
 * `Func_0200428c__s3070' undeclared (first use in this function). */
extern s32
Func_0200428c__s3070(s32 *a, s32 aRadius, s32 *b,
                     s32 bRadius) __asm__("Field_CheckSpheresOverlap");

#define SHARED_F02003070_NAME Func_02003070
#define SHARED_F02003070_ID2 gData_02004cf0__s3070
#define SHARED_F02003070_ID3 Func_02002d90
#define SHARED_F02003070_ID4 gData_0200483c
#define SHARED_F02003070_ID5 Actor_SelectAnimation
#define SHARED_F02003070_ID6 Actor_HasArrived
#define SHARED_F02003070_ID7 Func_02002d04
#define SHARED_F02003070_ID8 Func_0200428c__s3070
#include "shared/func_02003070.c"
#undef SHARED_F02003070_NAME
#undef SHARED_F02003070_ID2
#undef SHARED_F02003070_ID3
#undef SHARED_F02003070_ID4
#undef SHARED_F02003070_ID5
#undef SHARED_F02003070_ID6
#undef SHARED_F02003070_ID7
#undef SHARED_F02003070_ID8

// ---- scene/mars_lighthouse_star_room/set_data.c ----

/// Scene code of overlay 0x6ab, in ROM order.

#include "shared/mars_lighthouse.h"
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "mars_lighthouse_star_room.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/scene/scene.h"
#include "module/tla.h"

extern u16 gData_02004cf0[];

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void MarsLighthouseStarRoom_SlideOnIce(void);

void Func_02003384(u16 value) { gData_02004cf0[0] = value; }

#include "fixed_math.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(Func_02003390, Func_02002d90,
                                 Actor_SelectAnimation, Func_02002d04,
                                 Field_CheckSpheresOverlap)

#include "../field/object/set_sprite_priority.c"
