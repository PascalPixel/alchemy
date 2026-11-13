// ---- games/tla/src/scene/mars_lighthouse_star_room.c ----
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
#undef FIELD_AT

// ---- games/tla/src/scene/mars_lighthouse_star_room/s0ae4.c ----
extern struct Actor *Data_02004cec;

/* VIEW(s0ae4): routine Func_02000ab4 as void Func_02000ab4(void); the definition has void Func_02000188(void) { struct Actor *object = Actor_Get(10); if(Event_GetFlag(0xa4b) == 0) { if(Event_GetFlag(0x238) == 0 || Event_GetFl.... */
extern void Func_02000ab4__s0ae4(void) __asm__("Func_02000ab4");
u8 *HeapSlot_AllocFar(s32 slot, s32 size);

void Func_02000ae4(s32 rise) {
  struct Actor **slot = &Data_02004cec;
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
  object->update = Func_02000ab4__s0ae4;
  size = 0x608;
  sprite->nextFrame = zero;
  sprite->unk1b[0] = zero;
  block = HeapSlot_AllocFar(68, size);
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

void Func_020042dc(void);
void Func_020042e4(void);

void Func_02000ba4(void) {
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(14, 5);
  FieldEvent_SetMessageId((s32)MsgMarsStar03);
  Event_ShowMessage(14, 0);
  Actor_SetAnimation(14, 9);
  Func_020042e4();
}

void PartyInventory_Remove(s32 item);
s32 Inventory_AddItem(s32 owner, s32 item);
void *Unit_Get(s32 owner);
void Func_020042c4(s32 owner, s32 slot);
void Func_020041ec(struct Actor *object);
void Func_020043d4(void);

void Func_02000bd8(void) {
  struct Actor *leader;
  struct Actor *object;
  s32 i;
  s32 offset;

  Func_020042dc();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(13, 5);
  Event_SetMessageId((s32)MsgMarsStar02);
  Event_ShowMessageAndWait(13, 0);
  Actor_SetModeById(13, 10);
  if (!GameFlag_Test(0xa3a)) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
    object = Data_02004cec;
    object->motionFlags = 0;
    object->speedLimit = 0x40000;
    object->acceleration = 0x20000;
    FieldActor_SetMoveTarget(object, leader->x, leader->y + 0x200000, leader->z);
    Actor_WaitForMove(Data_02004cec);
    Event_PlaySound(83);
    PartyInventory_Remove(224);
    Inventory_AddItem(4, 222);
    Message_ShowCentered((s32)MsgMarsStar08, 1);
    i = 0;
    offset = 216;
    for (; i <= 14; offset += 2, i++) {
      if ((*(u16 *)(Unit_Get(4) + offset) & 0x1ff) == 222) {
        Func_020042c4(4, i);
        break;
      }
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    Event_SetFlag(0xa3a);
    FieldActor_SetPositionAndResetMotion(Data_02004cec, 0, 0, 0);
    System_Wait(1);
    Func_020041ec(Data_02004cec);
    System_Wait(1);
    FieldCamera_SetSpeed(0x4cccc, 0x9999);
    FieldCamera_MoveTo(170 << 18, -1, 146 << 16, 1);
    Func_020043d4();
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
  Func_020042e4();
}

void Func_02000d88(void) {
  MapGrid_CopyCellsTo(46, 68, 40, 73, 5, 6);
  MapGrid_CopyCellAttributes(104, 9, 5, 6, 40, 9);
}


void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_0200432c_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_02000db8(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSingleChildFlags(Actor_Get(id), 0);
  FieldActor_SetSpritePriority(id, 3);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  object->collisionFlags = 0;
  if (id == 13) {
    Func_0200432c_(13, 175 << 18, PIXELS(224));
    Actor_SetAnimation(13, 10);
    MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, id);
  } else {
    Func_0200432c_(14, 165 << 18, PIXELS(216));
    Actor_SetAnimation(14, 9);
    Func_0200423c_(31, 21, 2, 2, 40, 12);
  }
}

#include "event_work.h"

struct EventNote {
  u8 unk000[0x15a];
  u16 unk15a;
};

void Func_02000e50(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar07, 1);
  ((struct EventNote *)work)->unk15a = 1;
  Func_020042e4();
}

#include "field_sprite.h"

extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov66d;

/* VIEW(s0ae4): routine Func_02004294 as void Func_02004294(s32 map, void *destination); the definition has void Func_02000188(void) { struct Actor *object = Actor_Get(10); if(Event_GetFlag(0xa4b) == 0) { if(Event_GetFlag(0x238) == 0 || Event_GetFl.... */
extern void Func_02004294__s0ae4(s32 map, void *destination) __asm__("Func_02004294");
void Func_0200445c(void);
void Func_02001424(void);

#if !TLA_PATCH_1

void Func_02000e80(void) {
  struct Actor **slot;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *buffer;
  u16 *screen;
  s32 zero;

  screen = (u16 *)Ram_HeapSlots->block32;
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot = &Data_02004cec;
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
    buffer = HeapSlot_AllocFar(68, 0x608);
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
  Func_020043d4();
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
  Func_02004294__s0ae4(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                screen);
  // The zero sits in a local: as a plain `*screen = 0` 21 instructions differ.
  zero = 0;
  *screen = zero;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(0x2a80000, -1, 0xba0000, 1);
  Func_020043d4();
  FieldEvent_Wait(40);
  object = *slot;
  if (object != 0) {
    object->motionFlags = 3;
    object->velocityY = 0x20000;
    object->speedLimit = 0x20000;
    object->acceleration = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2a00000, 0, 0xa00000);
    Actor_WaitForMove((s32)*slot);
    Event_PlaySound(135);
    object = *slot;
    object->acceleration = 0x8000;
    object->velocityY = 0x40000;
    object->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2980000, 0, 0x9e0000);
    Actor_WaitForMove((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object = *slot;
  if (object != 0)
    Func_020041ec(object);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Func_0200445c();
  Event_SetFlag(0xa30);
  gMainRoutineWork.special = (s32)&SceneId_Ov66d;
  Func_020042e4();
}

#else

void Func_02000e80(void) {
  s32 flag;
  struct Actor **slot, **slot2;
  struct Actor *object, *object2;
  struct FieldSprite *sprite, *sprite2;
  u8 *buffer, *buffer2;
  u16 *screen;

  flag = GameFlag_Test(0xa9e);
  if (flag != 0)
    goto L_02008ea2;
  goto L_0200905c;
L_02008ea2:
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot = &Data_02004cec;
  object = (struct Actor *)FieldActor_Create(0x1e9, 0x2a80000, 0x80000, 0x930000);
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
    buffer = HeapSlot_AllocFar(68, 0x608);
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
    Actor_WaitForMove((s32)*slot);
    Event_PlaySound(135);
    object = *slot;
    object->acceleration = 0x8000;
    object->velocityY = 0x40000;
    object->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2980000, 0, 0x9e0000);
    Actor_WaitForMove((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object = *slot;
  if (object != 0)
    Func_020041ec(object);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Func_0200445c();
  Func_020042e4();
  goto L_020093b0;
L_0200905c:
  screen = (u16 *)Ram_HeapSlots->block32;
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot2 = &Data_02004cec;
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
    buffer2 = HeapSlot_AllocFar(68, 0x608);
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
  Func_020043d4();
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
  Func_02004294__s0ae4(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                screen);
  *screen = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(0x2a80000, -1, 0xba0000, 1);
  Func_020043d4();
  FieldEvent_Wait(40);
  object2 = *slot2;
  if (object2 != 0) {
    object2->motionFlags = 3;
    object2->velocityY = 0x20000;
    object2->speedLimit = 0x20000;
    object2->acceleration = 0x10000;
    FieldActor_SetMoveTarget(object2, 0x2a00000, 0, 0xa00000);
    Actor_WaitForMove((s32)*slot2);
    Event_PlaySound(135);
    object2 = *slot2;
    object2->acceleration = 0x8000;
    object2->velocityY = 0x40000;
    object2->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object2, 0x2980000, 0, 0x9e0000);
    Actor_WaitForMove((s32)*slot2);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object2 = *slot2;
  if (object2 != 0)
    Func_020041ec(object2);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Func_0200445c();
  Event_SetFlag(0xa30);
  Event_SetFlag(0xa9e);
  gMainRoutineWork.special = (s32)&SceneId_Ov66d;
  Func_020042e4();
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
    Func_020041ec(object);
}

// ---- games/tla/src/scene/mars_lighthouse_star_room/s1424.c ----
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Animation_SetStateFlags(struct Actor *object, u32 value);

/* VIEW(s1424): routine Func_020013e4 as void Func_020013e4(void); the definition has void Func_020013e4(struct Actor *object). */
extern void Func_020013e4__s1424(void) __asm__("Func_020013e4");

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
    r = Random16Far();
    x = r << 3;
    x -= r;
    z = Random16Far() * 15;
    x <<= 2;
    x >>= 16;
    column = 155;
  } else {
    r = Random16Far();
    x = r << 3;
    x -= r;
    z = Random16Far() * 15;
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
    Actor_SetSingleChildFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    *(s32 *)&object->watchedObject = 60;
    object->update = Func_020013e4__s1424;
    Func_020041cc(object, 5);
    Animation_SetStateFlags(object, 1);
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

#include "mars_lighthouse_entrance/room.h"
#include "field/event_work.h"

/* VIEW(s1424): tag struct RideSlot as struct RideSlot__s1424; mars_lighthouse_star_room.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1231: structure has no member named `id'. */
struct RideSlot__s1424 {
  u16 id;
  u16 blocked;
};

/* VIEW(s1424): tag struct RideWork as struct RideWork__s1424; mars_lighthouse_star_room.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1199: structure has no member named `value68'. */
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

extern u8 gData_020023c4[];

void Func_020015ec(struct Actor *object) {
  struct RideWork__s1424 *work = (struct RideWork__s1424 *)(gData_020023c4 + 0x188);
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

// ---- games/tla/src/scene/mars_lighthouse_star_room/spawn.c ----
extern const u8 gData_02004744[];

void Func_020041cc(struct Actor *object, s32 mode);
void Func_020041dc(struct Actor *object, const void *script);
/* VIEW(spawn): routine Func_020015ec as void Func_020015ec(void); the definition has void Func_020015ec(struct Actor *object). */
extern void Func_020015ec__spawn(void) __asm__("Func_020015ec");

void Func_02001750(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_020041dc(object, gData_02004744);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_02001780(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    ((void (*)())Func_02001750)(object, 2);
    object->update = Func_020015ec__spawn;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"

/* VIEW(spawn): tag struct RideSlot as struct RideSlot__spawn; mars_lighthouse_star_room.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1348: structure has no member named `id'. */
struct RideSlot__spawn {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(spawn): tag struct RideWork as struct RideWork__spawn; mars_lighthouse_star_room.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1326: structure has no member named `direction'. */
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

extern u8 gData_020023c4[];

s32 Func_02001528(struct Actor *a, struct Actor *b);
s32 Func_0200158c(struct Actor *a, struct Actor *b);

void Func_020017c8(struct Actor *object) {
  struct RideWork__spawn *work = (struct RideWork__spawn *)(gData_020023c4 + 0x188);
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

void Func_02001928(struct Actor *object, s32 priority);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02004744; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Func_02001928(struct Actor *object, s32 part) {
  /* FAKEMATCH(temporary): one zero for every cleared field; as six stores of
   * 0, 29 of 30 instructions differ in tla-en. */
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteParts *)object->sprite)->part = part;
  Actor_SetSingleChildFlags(object, 0);
  Func_020041dc(object, gData_02004744);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_0200196c(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_02001928(object, 3);
    object->update = Func_020015ec__spawn;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

// ---- games/tla/src/scene/mars_lighthouse_star_room/s19b4.c ----
/* VIEW(s19b4): routine Func_020017c8 as void Func_020017c8(void); the definition has void Func_020017c8(struct Actor *object). */
extern void Func_020017c8__s19b4(void) __asm__("Func_020017c8");

void Func_020019b4(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Func_02001928(object, 3);
    object->update = Func_020017c8__s19b4;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

void Func_020019fc(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_02001928(object, 3);
    object->update = Func_020017c8__s19b4;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

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


void Func_02001a9c(struct Actor *object) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 dx = object->bounce;
  s32 dz;
  struct SpriteParts *parts;
  s32 rate;
  s32 scale;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  parts = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  parts->unk12 += object->timer;
  if (leader->y == leader->terrainHeight && Func_02001a44(object, leader)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}


extern const u8 gData_02004744[];

void Func_02001b38(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_020041cc(object, 2);
  Func_020041dc(object, gData_02004744);
  Actor_SetSpriteColor(object, 10);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

// ---- games/tla/src/scene/mars_lighthouse_star_room/s1b78.c ----
void Animation_SetStateFlags(struct Actor *object, u32 value);
void Func_02001a9c();
/* VIEW(s1b78): routine Func_02001b38 as void Func_02001b38(struct Actor *object, s32 priority); the definition has void Func_02001b38(struct Actor *object); tried as the definition's void Func_02001b38(void), it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1497: conflicting types for `Func_02001b38'. */
extern void Func_02001b38__s1b78(struct Actor *object, s32 priority) __asm__("Func_02001b38");

void Func_02001b78(s32 *position, s32 base) {
  struct Actor *object =
      ObjectCreate_(0x11d, position[0], position[1], position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce =
        Math_Cosine(((u32)(Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() << 1) + base;
    /* FAKEMATCH(temporary): the angle held in an s32 local keeps the zero
     * extension and the pool word 0xffff8000 (a direct store narrows the add
     * to 16 bits: movs, lsls for 0x8000, no extension); as a direct store, 10
     * of 50 instructions differ in tla-en. */
    angle = (u16)Random16Far() - 0x8000;
    object->timer = angle;
    Func_02001b38__s1b78(object, 2);
    object->update = Func_02001a9c;
    Animation_SetStateFlags(object, 1);
  }
}

// ---- games/tla/src/scene/mars_lighthouse_star_room/s1bf8.c ----
/* VIEW(s1bf8): tag struct Actor as struct Actor__s1bf8; actor.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1611: structure has no member named `flag5b'. */
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
/* VIEW(s1bf8): routine Func_02001b78 as void Func_02001b78(); the parts before it has another declaration; tried as the one declaration void Func_02001b78(struct Actor *object, const s32 *script), it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1556: conflicting types for `Func_02001b78'. */
extern void Func_02001b78__s1bf8() __asm__("Func_02001b78");
extern u8 gData_020023c4[];

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

void Func_02001bf8(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  Camera *camera;
  Position *position;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)(gData_020023c4 + 0x188);
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
    flag = GameFlag_Test(0x106);
    if (*(s16 *)(saved[0] + 358) != 0)
      flag |= 1;
    mark = &actor->flag5b;
    *mark = flag;
    if (state->unk_e != 0)
      *mark = GameFlag_Test(0x106);
    dx = actor->x;
    dz = actor->z;
    dx -= cameraX;
    /* FAKEMATCH(barrier): the empty asm on dx changes its allocation rank; as
     * no asm, 94 of 182 instructions differ in tla-en. */
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

// ---- games/tla/src/scene/mars_lighthouse_star_room/s1d7c.c ----
struct RideAnimation {
  u8 unk00[0x10];
  u8 value10;
  u8 flag11 : 1;
};

/* VIEW(s1d7c): tag struct RideWork as struct RideWork__s1d7c; mars_lighthouse_star_room.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_star_room.c:1694: structure has no member named `value0c'. */
struct RideWork__s1d7c {
  u8 unk00[0xc];
  u16 value0c;
  u8 unk0e[0x62];
  struct Actor *handle;
};

extern u8 gData_020023c4[];

void Func_02001d7c(void) {
  struct RideWork__s1d7c *work = (struct RideWork__s1d7c *)(gData_020023c4 + 0x188);
  struct Actor *object;
  struct RideAnimation *animation;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWords(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Func_020041cc(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}
