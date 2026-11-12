#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"

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

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02002148(void);
void Func_0200236c(void);
void Func_02002ad0(struct Actor *object);

s32 Func_02000038(struct Actor *object) {
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

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000060(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[3384] = 1;
}

void Func_020014f4(void);

void Func_02000074(void) { Func_020014f4(); }

void Func_0200007c(void) {}

#include "mars_lighthouse_star_room/mars_lighthouse_star_room.h"
#include "iwram_call.h"

extern const u8 gData_02004474[];

void Func_02000080(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16Far() << 4) - (Random16Far() << 4);
    struct Actor *object =
        ObjectCreate_(0x145, x, (Random16Far() << 3) + source->y + 0x40000,
                      source->z + (Random16Far() << 4) - (Random16Far() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16Far();
      radius = Random16Far();
      scale = ((u32)Random16Far() >> 1) + 0x4000;
      Func_020041dc(object, gData_02004474);
      Actor_SetSingleChildFlags(object, 0);
      /* FAKEMATCH(dead): an empty once-only block holds the order around it; as
       * no block, 2 of 113 instructions differ in tla-en. */
      do {
      } while (0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius * 8, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = 0x8000;
      Animation_ApplyChildValues(object, 1);
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

void Func_0200024c(s32 phase, s32 frame) {
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
    Func_02000080(10);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_020002d4(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    Actor_SetSingleChildFlags(object, 0);
    object->collisionFlags = 0;
    object->motionFlags = 0;
  }
}

#include "main_routine_work.h"

struct Overlay6abEventWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
  u8 unk1b0[0x1c];
  struct Actor *unk1cc;
};

void Func_02000714(void);
void Func_02000ae4(s32 a);
void Func_02000db8(s32 actor);
void Func_02001d7c(void);
void Func_02001df0(s32 a, s32 b, s32 c, s32 d);
void Func_02003e04(void);
void Func_020041fc(void);
void Func_0200443c(s32 a);
extern struct Actor *Data_02004cec;
extern char SceneId_Ov6abRoom1;

s32 Func_020002f4(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  ((struct Overlay6abEventWork *)Ram_HeapSlots->eventWork)->unk1ac = 0x204;
  Data_02004cec = NULL;
  GameFlag_SetBit(0x112);
  Func_02001d7c();
  Func_02001df0(0, 0, 0, 88);
  if (scene == (s32)&SceneId_Ov6abRoom1) {
    Func_0200443c(0);
    if (GameFlag_Test(0xa30)) {
      /* FAKEMATCH(wrapper): Map_CopyCellAttributeRect and Func_0802b450 called through
       * Call6, here and below; as direct calls, 12 of 255 instructions differ
       * in tla-en. */
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
      Func_020041fc();
      if (!GameFlag_Test(0x109))
        Func_02003e04();
    }
    if (entrance == 9 || entrance == 10) {
      if (!GameFlag_Test(0x109))
        Func_02003e04();
    }
    if (entrance == 1 || entrance == 20) {
      if (!GameFlag_Test(0xa30)) {
        FieldActor_SetSpritePriority(11, 3);
        FieldActor_SetSpritePriority(12, 3);
        Actor_GetById(12)->scaleX = 0xffff0000;
      }
      if (GameFlag_Test(0xa4b)) {
        Func_02000d88();
        Actor_SetPosition(10, 0, 0);
      } else if (GameFlag_Test(0xa4a)) {
        GameFlag_SetBit(0x233);
      } else {
        Actor_SetPosition(10, 0, 0);
      }
      if (GameFlag_Test(0xa3b)) {
        if (!GameFlag_Test(0xa30)) {
          Actor_SetPosition(11, 0, 0);
          Actor_SetPosition(12, 0, 0);
        }
        Func_02000d88();
        System_Wait(10);
        if (!GameFlag_Test(0xa3a)) {
          Func_02000ae4(0);
          if (!GameFlag_Test(0x109)) {
            Func_02000db8(13);
            Func_02000db8(14);
          }
        }
        if (Actor_GetById(13)->x > 0x1900000) {
          MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, 13);
          MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
        }
      } else if (entrance == 20) {
        Func_02000714();
      }
    }
  }
  return 0;
}

#include "scene_ids.h"

extern char SceneId_Ov6ACRoom3;
void Func_02004294(s32 scene, s32 a1);

s32 Func_0200055c(void) {
  if (Event_GetFlag(0xa30))
    Func_02004294(gMainRoutineWork.map +
                      ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  if (gMainRoutineWork.entrance == 20)
    Event_SetFlag(0xa4b);
  return 0;
}

void Func_020042dc(void);
void Func_020043e4(s32 a, s32 b);
extern char SceneId_Ov66c;

void Func_020005b0(void) {
  Func_020042dc();
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
  Event_SetAfterBattleDestination((s32)&SceneId_Ov6abRoom1, 20);
  gMainRoutineWork.special = (s32)&SceneId_Ov66c;
  Func_020043e4(102, 0);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020042e4();
void Func_02004334();
void Func_0200445c();
extern u8 MsgMarsStar01[];
extern u8 gData_0200491c[];
extern u8 gData_02004958[];
extern u8 gData_020049a0[];
extern u8 gData_020049dc[];

void Func_02000714(void) {
  s32 t1, t2, t3, t8, t10, t12, t14, t28, t30, t32, t34, t50, t52, t87, v5, v6,
      v8;

  t1 = Actor_Get(11);
  v5 = t1;
  t2 = Actor_Get(15);
  v6 = t2;
  t3 = Actor_Get(16);
  v8 = t3;
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02000d88();
  t8 = Actor_Get(13);
  Actor_SetSingleChildFlags(t8, 0);
  t10 = Actor_Get(14);
  Actor_SetSingleChildFlags(t10, 0);
  t12 = Actor_Get(15);
  Actor_SetSpriteColor(t12, 7);
  t14 = Actor_Get(16);
  Actor_SetSpriteColor(t14, 7);
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
  Func_020041fc();
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(78);
  FieldEvent_Wait(20);
  Event_PlaySound(151);
  t28 = Actor_Get(11);
  Actor_SetSpriteColor(t28, 7);
  t30 = Actor_Get(12);
  Actor_SetSpriteColor(t30, 7);
  t32 = Actor_Get(13);
  Actor_SetSpriteColor(t32, 7);
  t34 = Actor_Get(14);
  Actor_SetSpriteColor(t34, 7);
  FieldEvent_Wait(20);
  Event_PlaySound(190);
  Func_02004334(15, 11);
  Func_02004334(16, 12);
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
  Actor_SetSpriteColor(t50, 0);
  t52 = Actor_Get(14);
  Actor_SetSpriteColor(t52, 0);
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
  Func_02000ae4(1);
  Event_ShowMessage(14, 0);
  Actor_SetAnimationAndWait(13, 7);
  Event_ShowMessage(13, 0);
  Actor_SetAnimation(13, 10);
  Actor_SetAnimation(14, 9);
  Event_SetFlag(0xa3b);
  Func_0200445c();
  Func_020042e4();
}

struct SpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

struct SpriteAttributes {
  u8 unk00[9];
  struct SpriteBits bits;
};

void Func_02000ab4(struct Actor *object) {
  struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);
  struct SpriteAttributes *to = (struct SpriteAttributes *)object->sprite;
  struct SpriteAttributes *from = (struct SpriteAttributes *)owner->sprite;
  struct SpriteBits source = from->bits;
  struct SpriteBits bits;

  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}
