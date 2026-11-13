/// @file
/// The scene code of overlay 0x68b, in ROM order.

#include "shared/tables.h"
#include "shared/routines.h"
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "dma.h"
#include "frame.h"
#include "io_reg.h"
#include "map.h"
#include "event_work.h"
#include "actor_view.h"
#include "module/field/field.h"
#include "module/scene/lemuria.h"
#include "module/system/system.h"

DEFINE_FUNC_02000038_75(Func_02000038)

DEFINE_FUNC_02000070(Func_02000070, Func_02000038)

s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Func_02001b7c(struct Actor *object, s32 mode);

s32 Func_020000e8(struct Actor *self) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *state;
  u32 mode;
  s8 *effect;
  struct Actor *other;

  Actor_SetSpritePriorityDirect(self, BattleEffect_GetObjectPriority(Actor_Get(gMainRoutineWork.currentOwner)));
  mode = 0x3ff & *(u16 *)(event + 0x166);
  state = &self->movementState;
  if (mode == 141) {
    effect = Ram_HeapSlots->effectWork;
    if (effect[0x713] != 0)
      goto near;
  }
  if (self->timer == 1)
    goto near;
  other = Actor_Get(gMainRoutineWork.currentOwner);
  if (Func_02000038(&self->x, &other->x) <= 11)
    goto far;
  if (Func_02000070(self, self->watchedObject, 18) != 0)
    goto near;
  if (Func_02000070(self, Actor_Get(gMainRoutineWork.currentOwner), 26) != 0)
    goto near;
far:
  *state = 0;
  Func_02001b7c(self, 2);
  return 0;
near:
  *state = 1;
  Func_02001b7c(self, 1);
  return 0;
}

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

void Actor_ReturnHome(s32 a, s32 b, s32 c);
void Func_02001d44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);

extern struct WarpTarget gData_020021bc;
extern struct WarpTarget gData_020021c4;
extern struct WarpTarget gData_020021cc;
extern struct WarpTarget gData_020021d4;
extern struct WarpTarget gData_020021dc;
extern struct WarpTarget gData_020021e4;
extern struct WarpTarget gData_020021ec;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_020001bc(void) {
  Actor_SetPosition(12, PIXELS(488), PIXELS(1256));
  Animation_SetIndexAndInitializeObjects(Actor_Get(12), 157);
  Actor_SetAnimation(12, 3);
  Actor_FaceDirection(12, FACING_WEST, 0);
}

void Func_020001f0(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(158);
  MapGrid_AnimateCells(warp->scene, warp->x, warp->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// The party leader walks into one of Lemuria's doors; the palace's is locked
/// while the council meets.
/// "The door is locked... It sounds as if there's a raucous caucus going on
/// inside..."
void Lemuria_EnterDoor(s32 door) {
  const struct WarpTarget *warp = &gData_020021bc;
  s32 mode = 0;

  if (door == 3 && Event_GetFlag(0x96a)) {
    FieldEvent_SetMessageId((s32)&MsgTheDoorIs);
    Event_ShowMessage(-1, 0);
    return;
  }
  if (door == 1) {
    warp = &gData_020021bc;
    mode = 0;
  }
  if (door == 2) {
    warp = &gData_020021c4;
    mode = 0;
  }
  if (door == 3) {
    warp = &gData_020021cc;
    mode = 1;
  }
  if (door == 12) {
    warp = &gData_020021d4;
    mode = 0;
  }
  if (door == 11) {
    warp = &gData_020021dc;
    mode = 1;
  }
  if (door == 10) {
    warp = &gData_020021e4;
    mode = 1;
  }
  if (door == 16) {
    warp = &gData_020021ec;
    mode = 0;
  }
  Func_020001f0(warp, door, mode);
}

/// Actor 18's talk: one message, and Actor_TurnToFaceEachOther with the party
/// leader, while the event work's halfword at 0x166 is 0, another once it is
/// set; the actor's angle is restored after.
void Lemuria_TalkToSpoiledChild(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_Get(18);
  s16 angle = object->facing;

  object->timer = 1;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (*(s16 *)(work + 0x166) == 0) {
    FieldEvent_SetMessageId((s32)&MsgItHasBeen);
    Actor_FaceEachOther(18, gMainRoutineWork.currentOwner, 2);
  } else {
    FieldEvent_SetMessageId((s32)&MsgEveryoneSpoilsMe);
  }
  Actor_SetAnimation(18, 0);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Event_End();
  object->timer = 0;
  object->facing = angle;
  System_Wait(1);
}

/// The same for actor 19.
void Lemuria_TalkToLonelyChild(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_Get(19);
  s16 angle = object->facing;

  object->timer = 1;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (*(s16 *)(work + 0x166) == 0) {
    FieldEvent_SetMessageId((s32)&MsgWeAreThe);
    Actor_FaceEachOther(19, gMainRoutineWork.currentOwner, 2);
  } else {
    FieldEvent_SetMessageId((s32)&MsgAreThereOther);
  }
  Actor_SetAnimation(19, 0);
  FieldEvent_ShowMessageThenWait(19, 0, 10);
  Event_End();
  object->timer = 0;
  object->facing = angle;
  System_Wait(1);
}

static __inline__ void Func_02001d44_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_02001d44(a, b, c, d, e, f, g);
}

void Func_02000428(void) { Func_02001d44_(37, 72, 2, 2, 39, 71, 2); }

void Func_02000448(void) { Func_02001d44_(59, 64, 4, 3, 49, 71, 2); }

void Func_02000468(void) { Func_02001d44_(52, 69, 1, 1, 53, 69, 2); }

void Func_02000488(void) { Func_02001d44_(53, 64, 1, 1, 52, 64, 2); }

void Func_020004a8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02001d44_(59, 64, 4, 3, 49, 71, 2);
  if (Event_GetFlag(0xfbb) == 0) {
    Actor_SetPosition(69, PIXELS(808), PIXELS(1176));
    if (leader->x >> 20 == 50 && leader->z >> 20 == 73) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = PIXELS(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Func_02001d74(void);

void Func_02000538(void) {
  struct MainRoutineWork *party;

  Func_02001d74();
  Event_SetDestination((s32)&SceneId_Ov68b, 2);
  party = &gMainRoutineWork;

  party->unk2c2 = 2;
}

void Func_02000560(void) {
  s32 x;
  s32 z;
  s32 flag;
  s32 value;
  s32 old;

  flag = 0x900;
  /* FAKEMATCH(barrier): the empty asm on flag, so the flag's high part is
   * shifted before the two coordinates start; as the plain constant, the shift
   * comes after them, 2 of 33 instructions differ in tla-en. */
  asm volatile("" : "+l"(flag));
  x = 0x3580000;
  z = 0x3f80000;
  Event_SetFlag(flag + 0x63);
  value = Func_02001d5c(x, z, 0);
  /* FAKEMATCH(temporary): the first z copied to old, so the second z starts
   * before the first store call's arguments; as z reassigned after that call,
   * its shift comes late, 2 of 33 instructions differ in tla-en. */
  old = z;
  z = 0x4080000;
  Func_02001d64(x, old, 0, value | 64);
  Func_02001d64(x, z, 0, Func_02001d5c(x, z, 0) | 64);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02001cf4_(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

void Func_020005ac(s32 a) { Func_02001cf4_(a, 64, 0x300); }

void Func_020005bc(s32 a) { Func_02001cf4_(a, 65, 0x301); }

void Func_020005cc(s32 a) { Func_02001cf4_(a, 66, 0x302); }

void Func_020005dc(s32 a) { Func_02001cf4_(a, 67, 0x303); }

void Func_020005ec(s32 a) { Func_02001cf4_(a, 68, 0x304); }

void Func_020005fc(s32 a) { Func_02001cf4_(a, 70, 0x306); }

void Func_0200060c(s32 a) { Func_02001cf4_(a, 71, 0x307); }

void Func_0200061c(s32 a) { Func_02001cf4_(a, 72, 0x308); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Party_FindOwnerWithAction();
void Func_02001d2c();

/// A door that will not open, until a Lemurian inside takes Piers's companions
/// in another way.
/// "The door doesn't want to open."
void Lemuria_KnockAtBrokenDoor(void) {
  s32 t1, t2, t3, t4, t12, t30, t31, v5;

  t1 = (s32)Actor_Get(gMainRoutineWork.currentOwner);
  if (FIELD_AT(t1, s32, 12) > 0x600000)
    goto L_02008654;
  t2 = Event_GetFlag(0x969);
  if (t2 == 0)
    goto L_02008666;
L_02008654:
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x15a), s16, 0) = 1;
  goto L_020088d4;
L_02008666:
  t3 = Event_GetFlag(0x966);
  if (t3 == 0)
    goto L_02008682;
  t4 = Event_GetFlag(0x968);
  if (t4 == 0)
    goto L_02008694;
L_02008682:
  FieldEvent_SetMessageId((s32)&MsgTheDoorDoesn);
  Event_ShowMessage(-1, 0);
  goto L_020088d4;
L_02008694:
  Event_SetFlag(0x968);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgAreYouKids);
  Actor_SetPosition(12, PIXELS(872), PIXELS(880));
  t12 = (s32)Actor_Get(12);
  Animation_SetIndexAndInitializeObjects(t12, 43);
  Actor_SetAnimation(12, 3);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(776), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(12, 0x109, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(12, 4);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(856), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(928), 1);
  FieldCamera_WaitForArrival();
  t30 = Party_FindOwnerWithAction(133);
  v5 = t30;
  if (v5 == -1)
    goto L_02008778;
  goto L_020088cc;
L_02008778:
  t31 = Event_GetFlag(0x962);
  if (t31 == 0)
    goto L_02008788;
  goto L_020088cc;
L_02008788:
  FieldEvent_SetMessageId((s32)&MsgWhatSThe);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(4, 856, 952);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(4, 0x105, 60);
  FieldCamera_MoveTo(PIXELS(872), v5, PIXELS(792), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(12, 0x101, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(PIXELS(872), v5, PIXELS(856), 1);
  FieldActor_Jump(12, 4, 0);
  Actor_WalkToAndWait(12, 872, 912);
  FieldEvent_Wait(20);
  Actor_SetSpeed(12, 0x10000, 0x8000);
  Actor_WalkByAndWait(12, -32, 0);
  Actor_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP);
  FieldCamera_MoveTo(PIXELS(848), v5, PIXELS(904), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_SetPosition(28, PIXELS(840), PIXELS(920));
  Actor_SetPosition(27, PIXELS(840), PIXELS(936));
  Value3((s32 (*)())Func_02001d2c, 1, 28, 27);
  Func_02001d34(1, 28, 27);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_WalkByAndWait(12, 32, 0);
  FieldCamera_FollowActor(4, 1);
  Actor_WalkByAndWait(12, 0, -32);
  Event_SetFlag(0x96f);
L_020088cc:
  Func_020001bc();
  Event_End();
L_020088d4:
}

void Party_RestoreHpPp(s32 value);

/// "Felix and the others drank of the spring. Their HP and PP were fully
/// replenished!"
void Lemuria_DrinkFromSpring(void) {
  s32 y = Actor_Get(gMainRoutineWork.currentOwner)->y;

  if (y < 0x5a0000) {
    Message_ShowCentered((s32)MsgAndTheOthers, 1);
    Event_PlaySound(126);
    Party_RestoreHpPp(1);
  } else {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  }
}

/// Actor 30 offers a test of luck at the Spring of Lemuria; flag 0x908
/// records that the party took it.
void Lemuria_OfferSpringLuck(void) {
  if (Event_GetFlag(0x908)) {
    Actor_FaceActor(30, gMainRoutineWork.currentOwner, 0);
    FieldActor_RefreshSelectorById(30);
    FieldEvent_SetMessageId((s32)MsgYouLookLike);
    Event_ShowMessage(30, 0);
    Actor_FaceDirectionAndWait(30, FACING_NORTH + FACING_STEP);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgDoYouWish);
  Actor_ShowEmote(30, 0x100, 30);
  Actor_FaceActor(30, gMainRoutineWork.currentOwner, 0);
  FieldActor_RefreshSelectorById(30);
  FieldEvent_Wait(5);
  Event_OpenMessage(30, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(30, 0, 0);
  } else {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(30, 3);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    FieldEvent_ShowMessageThenWait(30, 0, 5);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 464, 1040);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
    FieldEvent_Wait(5);
    Actor_SetSpeed(30, 0x13333, 0x9999);
    Actor_WalkToAndWait(30, 440, 1048);
    Event_SetFlag(0x908);
  }
  Actor_FaceDirectionAndWait(30, FACING_NORTH + FACING_STEP);
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern u8 gData_02002590;
extern s16 gData_020025a0[2][160];

/// Every fourth frame rotates colours 1-14 of background palettes 7 and 8 by
/// one place. The loops are written with goto, as overlay 689's Func_020002f8:
/// the bytes rebuild the palette address on every pass.
void Func_02000a6c(void) {
  if ((gFrameCount & 3) == 0) {
    volatile u16 *palette;
    volatile u16 *slot;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(7, 14);
    index = 14;
  first:
    palette = (volatile u16 *)&BG_PLTT_COLOR(7, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    slot = (volatile u16 *)&BG_PLTT_COLOR(7, 1);
    *slot = saved;
    slot += 29;
    saved = *slot;
    index = 14;
  second:
    palette = (volatile u16 *)&BG_PLTT_COLOR(8, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)&BG_PLTT_COLOR(8, 1) = saved;
  }
}

/// Puts the per-line BG2 horizontal scroll on an HBlank DMA, as overlay 674's
/// FieldEffect_SetAttached does for BG1.
void Func_02000ad8(void) {
  s16 *lines = gData_020025a0[gData_02002590];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH(barrier): the empty asm reading first, so the first line's load
   * comes before the register address is built; as no asm, the load follows
   * it, 6 of 34 instructions differ in tla-en. */
  __asm__("" : : "r"(first));
  scroll = &REG_BG2HOFS;
  *scroll = first;
  dma = REG_DMA0;

  dma[5] &= 0xc5ff; // Clear the start timing and repeat
  dma[5] &= 0x7fff; // Stop
  lines++;
  (void)dma[5];
  Dma_Set(lines, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

void Func_02000b28(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  s16 *lines = gData_020025a0[gData_02002590 ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    sine = Math_Sin((*count + (u8)gFrameCount + (layer->y >> 16)) << 9);
    v[0] = sine;
    *lines++ = (layer->x >> 16) + (sine >> 15) + *count % 3 - 1;
  }
  gData_02002590 ^= 1;
}

void Func_02000bbc(void) {
  s32 priority;

  priority = 0x480;
  Task_AddFrame(Func_02000b28, priority);
  priority = 0xc80;
  Task_AddFrame(Func_02000ad8, priority);
}

void Func_02001cfc();
void Func_02001d4c();
void Actor_SetWideSprite();

s32 Scene_Initialize(void) {
  s32 t3, t7, t11, t12, t15, t16, t17, t19, t20, t23, t25, t26, t27, t32, t33,
      t34, t40, t42, t45, t47, t49, t51, t53, t55, v5, v6;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x201;
  FieldTask_AddFrame((s32)Func_02000a6c, 0xc80);
  Func_02001cfc(0);
  t3 = Actor_Get(11);
  Actor_SetSingleChildFlags(t3, 0);
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(29, 3);
  t7 = Actor_Get(21);
  FIELD_AT(t7, s32, 108) = (s32)Func_02001b3c;
  Actor_SetAnimation(22, 3);
  Actor_SetPosition(18, PIXELS(40), PIXELS(952));
  Actor_SetScript(18, (s32)&gData_02001d84);
  t11 = Actor_Get(18);
  v6 = t11;
  t12 = Actor_Get(19);
  FIELD_AT(v6, s32, 104) = t12;
  FIELD_AT(v6, s32, 108) = (s32)Func_020000e8;
  Actor_SetPosition(19, PIXELS(88), PIXELS(904));
  Actor_SetScript(19, (s32)&gData_02001e04);
  t15 = Actor_Get(19);
  v6 = t15;
  t16 = Actor_Get(18);
  FIELD_AT(v6, s32, 104) = t16;
  FIELD_AT(v6, s32, 108) = (s32)Func_020000e8;
  t17 = Event_GetFlag(0x908);
  if (t17 == 0)
    goto L_02008c9c;
  Actor_SetPosition(30, PIXELS(440), PIXELS(1048));
L_02008c9c:
  t19 = Event_GetFlag(0x964);
  if (t19 == 0)
    goto L_02008cd4;
  t20 = Event_GetFlag(0x969);
  if (t20 != 0)
    goto L_02008cd4;
  Actor_SetPosition(24, PIXELS(456), PIXELS(520));
  Actor_SetPosition(25, PIXELS(472), PIXELS(520));
L_02008cd4:
  t23 = Event_GetFlag(0x969);
  if (t23 != 0)
    goto L_02008cee;
  Actor_SetPosition(26, 0, 0);
  goto L_02008cf8;
L_02008cee:
  t25 = Actor_Get(26);
  FIELD_AT(t25, s32, 108) = (s32)Func_02001b3c;
L_02008cf8:
  t26 = Event_GetFlag(0x966);
  if (t26 == 0)
    goto L_02008d2a;
  t27 = Event_GetFlag(0x969);
  if (t27 != 0)
    goto L_02008d2a;
  Actor_SetPosition(7, PIXELS(56), PIXELS(1016));
  Actor_SetAnimation(7, 9);
L_02008d2a:
  Func_02001d24();
  Func_02001d3c(0, 0x962, 8, 9);
  t32 = Actor_Get(8);
  FIELD_AT(t32, u8, 35) = (FIELD_AT(t32, u8, 35) | 32);
  t33 = Actor_Get(9);
  v5 = (32 | FIELD_AT(t33, u8, 35));
  FIELD_AT(t33, u8, 35) = v5;
  t34 = Event_GetFlag(0x96f);
  if (t34 == 0)
    goto L_02008da6;
  FieldActor_SetSpritePriority(28, 2);
  FieldActor_SetSpritePriority(27, 2);
  Actor_SetPosition(28, PIXELS(840), PIXELS(920));
  Actor_SetPosition(27, PIXELS(840), PIXELS(936));
  Call4(Func_02001d3c, 1, 0x96f, 28, 27);
L_02008da6:
  t40 = Actor_Get(28);
  Actor_SetSingleChildFlags(t40, 0);
  t42 = Event_GetFlag(0x963);
  if (t42 == 0)
    goto L_02008dcc;
  Func_02001d4c(10, 1);
  Func_02000560();
L_02008dcc:
  t45 = Event_GetFlag(0x303);
  if (t45 != 0)
    goto L_02008de2;
  Actor_SetWideSprite(67, 0);
L_02008de2:
  t47 = Event_GetFlag(0x304);
  if (t47 != 0)
    goto L_02008df6;
  Actor_SetWideSprite(68, 0);
L_02008df6:
  t49 = Event_GetFlag(0x306);
  if (t49 != 0)
    goto L_02008e0c;
  Actor_SetWideSprite(70, 0);
L_02008e0c:
  t51 = Event_GetFlag(0x307);
  if (t51 != 0)
    goto L_02008e22;
  Actor_SetWideSprite(71, 0);
L_02008e22:
  t53 = Event_GetFlag(0x966);
  if (t53 == 0)
    goto L_02008e44;
  MapGrid_CopyCellsLayered(3, 1, 1, 1, 54, 45);
L_02008e44:
  t55 = Actor_Get(31);
  v6 = t55;
  FIELD_AT(v6, u8, 85) = 0;
  FIELD_AT(v6, s32, 20) = 0x400000;
  FIELD_AT(v6, s32, 12) = 0x400000;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// "Oh! Goodness! Felix! Look over here! This palace is magnificient!"
void Lemuria_AdmirePalace(s32 id) {
  struct Actor *leader;
  s32 a;
  s32 b;
  s32 c;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgOh2);
  Animation_SetIndexAndInitializeObjects((s32)Actor_Get(12), 56);
  if (id == 21) {
    Actor_WalkToAndWait(4, 376, 968);
    Actor_FaceDirection(4, FACING_EAST, 0);
    FieldActor_LaunchFromLeader(12, 16, -16, FACING_EAST);
    a = 0;
    b = 0x800a;
    c = 0x5000;
  } else {
    Actor_WalkToAndWait(4, 552, 968);
    Actor_FaceDirection(4, FACING_WEST, 0);
    FieldActor_LaunchFromLeader(12, -16, -16, FACING_WEST);
    a = 0x8000;
    b = -10;
    c = 0x3000;
  }
  FieldActor_WaitForMoveById(12);
  FieldActor_WatchAndSetScript(4, 12);
  Actor_SetSpeed(12, 0x10000, 0x8000);
  FieldCamera_FollowActor(12, 1);
  Actor_WalkToAndWait(12, 464, 952);
  Actor_FaceDirectionAndWait(12, a);
  FieldEvent_Wait(10);
  Actor_ShowEmote(12, 0x100, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_FaceDirectionAndWait(12, b);
  FieldEvent_Wait(10);
  Actor_ShowEmote(12, 0x100, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_WalkToAndWait(12, 464, 784);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldActor_Jump(12, 6, 15);
  FieldActor_Jump(12, 6, 23);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(12, c);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  FieldEvent_Wait(20);
  FieldActor_SetSpritePriority(4, 1);
  Actor_SetPosition(4, PIXELS(464), PIXELS(856));
  Actor_WalkToAndWait(4, 464, 796);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(12, 3);
  FieldEvent_Wait(10);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetAnimation(12, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  FieldEvent_Wait(10);
  Actor_Get(4)->priorityFlags |= 1;
  Actor_WalkToAndWait(4, 464, 776);
  Func_020001bc();
  Event_End();
}

/// "Where are you going, Felix? Let's take a longer look around!"
void Lemuria_KeepLookingAround(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhereAreYou);
  Animation_SetIndexAndInitializeObjects((s32)Actor_Get(12), 56);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldActor_SetSpritePriority(12, 1);
  FieldActor_LaunchFromLeader(12, 0, 16, FACING_NORTH + FACING_STEP);
  FieldActor_WaitForMoveById(12);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetAnimation(12, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(4, 0, -16);
  Func_020001bc();
  Event_End();
}

struct EventWork1c4 {
  u8 unk000[0x1c4];
  u16 count;
};

/// The party marvels at the city, and is stopped by a Lemurian who has never
/// seen it before.
/// "This place is remarkable... Truly remarkable..."
void Lemuria_MarvelAtCity(void) {
  struct Actor *leader;
  s32 result;

  Event_SetFlag(0x964);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgThisPlaceIs);
  Actor_SetSpeed(24, 0x10000, 0x8000);
  Actor_SetSpeed(25, 0x10000, 0x8000);
  Animation_SetIndexAndInitializeObjects((s32)Actor_Get(12), 56);
  Actor_WalkToAndWait(4, 472, 568);
  FieldActor_LaunchFromLeader(12, -16, 0, FACING_NORTH + FACING_STEP);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(12);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(544), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_Jump(12, 6, 15);
  FieldActor_Jump(12, 6, 23);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(24, 0x100, 40);
  Actor_WalkByAndWait(24, 16, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(12, FACING_EAST, 0);
  Actor_FaceDirection(4, FACING_WEST, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(25, -16, 0);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_WalkToAndWait(25, 464, 552);
  Actor_FaceDirection(25, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(40);
  FieldActor_SetSpritePriority(25, 2);
  Actor_WalkByAndWait(25, 16, 0);
  Actor_FaceDirection(25, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(25, -8, 0);
  Actor_FaceDirection(25, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(25, 472, 520);
  Actor_FaceDirection(25, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(12, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(24, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(24, 0x101, 30);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  FieldActor_LaunchFromLeader(7, -8, -16, FACING_NORTH);
  FieldActor_WaitForMoveById(7);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_StartRepeatedMotion(24, 3);
  Actor_SetAttachedEffect(24, 0x102);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, 0x102);
  FieldEvent_Wait(45);
  Actor_FaceDirection(24, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(25, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(24, FACING_EAST, 0);
  Actor_FaceDirection(25, FACING_WEST, 0);
  FieldEvent_Wait(35);
  Actor_FaceDirection(25, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_ShowEmote(7, 0x101, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(24, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_ShowEmote(7, 0x102, 40);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, 0x101, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, 0x100, 60);
  FieldActor_WatchAndSetScript(4, 12);
  FieldActor_WatchAndSetScript(7, 12);
  FieldActor_WatchAndSetScript(24, 12);
  FieldActor_WatchAndSetScript(25, 12);
  Actor_SetSpeed(12, 0x10000, 0x8000);
  FieldCamera_FollowActor(12, 1);
  Actor_WalkToAndWait(12, 424, 600);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(PIXELS(400), -1, PIXELS(648), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(40);
  FieldCamera_FollowActor(12, 1);
  Actor_WalkToAndWait(12, 432, 536);
  FieldEvent_Wait(20);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(288), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(528), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(12, 3);
  FieldEvent_Wait(10);
  FieldCamera_FollowActor(12, 1);
  Actor_WalkToAndWait(12, 456, 568);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(544), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(25, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetSpeed(7, 0xb333, 0x5999);
  Actor_WalkByAndWait(7, 8, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Event_OpenMessage(7, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    FieldEvent_Wait(30);
    Actor_SetAnimationAndWait(7, 4);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(7, 3);
    FieldEvent_Wait(10);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    FieldEvent_ShowMessageThenWait(7, 0, 10);
  }
  FieldActor_WatchAndSetScript(4, 7);
  FieldActor_WatchAndSetScript(12, 7);
  FieldActor_WatchAndSetScript(24, 7);
  FieldActor_WatchAndSetScript(25, 7);
  FieldActor_SetSpritePriority(7, 2);
  Actor_WalkByAndWait(7, 32, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(25, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(7, 0x102, 40);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(12, FACING_EAST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(12, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(12, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(7, 0x109, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, 0x100, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldActor_Jump(12, 6, 15);
  FieldActor_Jump(12, 6, 23);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_RunRepeatedMotion(24, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(12, FACING_EAST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(25, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(12, 0x101, 0);
  Actor_ShowEmote(4, 0x101, 40);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_ShowEmote(7, 0x109, 45);
  Actor_WalkByAndWait(7, -16, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_SetAnimationAndWait(25, 4);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_WalkByAndWait(7, -16, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(12, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(12, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(7, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetAnimation(12, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  Func_020001bc();
  Event_End();
}

void Func_02001d6c(void);

void Func_02001b3c(void) { Func_02001d6c(); }

#include "objcmd.h"

extern s16 gData_020021b0[];

// clang-format off
DEFINE_G_DATA_02001D84(gData_02001d84, 40, 952, 904, 88)

DEFINE_G_DATA_02001D84(gData_02001e04, 88, 904, 952, 40)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 460, 0, 550, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(202),
    SCENE_EXIT_IF(1, 203, 1, -1),
    SCENE_EXIT_IF(2, 203, 2, -1),
    SCENE_EXIT_IF(3, 203, 3, -1),
    SCENE_EXIT_IF(4, 201, 3, -1),
    SCENE_EXIT_IF(5, 205, 1, -1),
    SCENE_EXIT_IF(12, 203, 12, -1),
    SCENE_EXIT_IF(15, 203, 15, -1),
    SCENE_EXIT_IF(16, 203, 16, -1),
    SCENE_EXIT_IF(11, 204, 1, -1),
    SCENE_EXIT_IF(10, 204, 10, -1),
    SCENE_EXIT_IF(17, 204, 17, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(952), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(920), FACING_SOUTH, 2, 1},
    {372, CONDITION_ALWAYS, 1, PIXELS(856), 0, PIXELS(1016), FACING_SOUTH, 2, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(880), FACING_SOUTH, 2, 0},
    {157, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(1256), FACING_WEST, 1, 0},
    {156, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(1040), FACING_SOUTH, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(1064), FACING_SOUTH, 1, 0},
    {165, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(856), FACING_SOUTH + FACING_STEP, 1, 0},
    {164, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(920), FACING_WEST, 1, 0},
    {166, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(920), FACING_SOUTH + FACING_STEP, 1, 0},
    {167, CONDITION_ALWAYS, 1, PIXELS(40), 0, PIXELS(952), FACING_NORTH, 2, 0},
    {168, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(904), FACING_SOUTH, 2, 0},
    {216, CONDITION_ALWAYS, 2, PIXELS(80), 0, PIXELS(1104), FACING_SOUTH, 0, 0},
    {189, CONDITION_ALWAYS, 2, PIXELS(392), 0, PIXELS(712), FACING_SOUTH, 0, 0},
    {80, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(1080), FACING_EAST, 2, 0},
    {203, CONDITION_ALWAYS, 2, PIXELS(864), 0, PIXELS(984), FACING_SOUTH, 0, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(520), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(520), FACING_SOUTH + FACING_STEP, 1, 0},
    {166, CONDITION_ALWAYS, 2, PIXELS(552), 0, PIXELS(440), FACING_SOUTH, 0, 0},
    {7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {407, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {157, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(1008), FACING_WEST, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(464), 0, PIXELS(1016), FACING_NORTH + FACING_STEP, 2, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(488), 0, PIXELS(984), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

s16 gData_02002184[] = {
    2, 1, 1, 1, 6, 2, 2, 1,
    1, 6, -1,
};

s16 gData_0200219a[] = {
    0, 1, 2, 1, 6, 0, 2, 2,
    1, 6, -1,
};

s16 gData_020021b0[] = {
    3, 1, 1, 1, 0, -1,
};
// clang-format on

struct WarpTarget gData_020021bc = {(s32)gData_02002184, 22, 47};

struct WarpTarget gData_020021c4 = {(s32)gData_02002184, 35, 47};

struct WarpTarget gData_020021cc = {(s32)gData_0200219a, 9, 40};

struct WarpTarget gData_020021d4 = {(s32)gData_020021b0, 54, 45};

struct WarpTarget gData_020021dc = {(s32)gData_0200219a, 28, 20};

struct WarpTarget gData_020021e4 = {(s32)gData_0200219a, 28, 13};

struct WarpTarget gData_020021ec = {(s32)gData_02002184, 54, 50};

struct Trigger gSceneEvents[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {49, 4, CONDITION_ALWAYS, 4},
    {0xc401, 5, 0x1908, 5},
    {0xc602, 12, 0x1966, EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0xc602, 16, 0x1969, EVENT(Lemuria_EnterDoor)},
    {0xc602, 11, CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 20, 0x964, EVENT(Lemuria_MarvelAtCity)},
    {TRIGGER_STEP, 21, 0x964, EVENT(Lemuria_AdmirePalace)},
    {TRIGGER_STEP, 22, 0x964, EVENT(Lemuria_AdmirePalace)},
    {TRIGGER_STEP, 23, 0x964, EVENT(Lemuria_KeepLookingAround)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgICannotRemember)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaHasNo)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgInLemuriaWe)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLifeNeverChanges)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgIOughtTo)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgHowOldAm)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Lemuria_TalkToSpoiledChild)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Lemuria_TalkToLonelyChild)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgMeowMrrrfle2)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWoofRrrrrroof2)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgMoo2)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgBukbukbuk2)},
    {TRIGGER_TALK, 7, CONDITION_ALWAYS, MSGNO(MsgBlank4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgMoreOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgIHavenT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgIVeAged)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgSometimesIThink)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLongAgoA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKingHydrosIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT(Lemuria_TalkToSpoiledChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, EVENT(Lemuria_TalkToLonelyChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgNoOneIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgThereSSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgLemuriansAreSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgIfItWeren)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 7, CONDITION_ALWAYS, MSGNO(MsgMotherWhyHave)},
    {TRIGGER_TALK, 24, 0x196a, MSGNO(MsgIVeNever)},
    {TRIGGER_TALK, 25, 0x196a, MSGNO(MsgLordConservatoHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x196a, MSGNO(MsgIMSureThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x196a, MSGNO(MsgHowCouldLord)},
    {TRIGGER_TALK, 24, 0x1969, MSGNO(MsgKingHydrosAwaits)},
    {TRIGGER_TALK, 25, 0x1969, MSGNO(MsgHisHighnessThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x1969, MSGNO(MsgTheyAreClearly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x1969, MSGNO(MsgWhatDoesHis)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTheKingIs)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgTheKingWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgHowMuchLongerWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgIHadHeard)},
    {TRIGGER_TALK, 26, 0x196a, MSGNO(MsgIfYouAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, 0x196a, MSGNO(MsgIKnowWhy)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTheSenateHolds)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgITrustThe)},
    {0xc400, 11, CONDITION_ALWAYS, MSGNO(MsgHouseOfThe)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgDrinkingFromThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgTheseTurtlesHave)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(Lemuria_OfferSpringLuck)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgNoOneIs)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Lemuria_DrinkFromSpring)},
    {TRIGGER_EXAMINE, 25, CONDITION_ALWAYS, EVENT(Lemuria_DrinkFromSpring)},
    {TRIGGER_EXAMINE, 40, CONDITION_ALWAYS, EVENT(Lemuria_KnockAtBrokenDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x962, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 10, 0x963, EVENT(Func_02000560)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_02000448)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_02000468)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_02000488)},
    {0x50008905, 38, CONDITION_ALWAYS, EVENT(Func_020004a8)},
    {0x50008805, 104, 0x303, EVENT(Func_020005dc)},
    {0x50008805, 105, 0x304, EVENT(Func_020005ec)},
    {0x50008805, 107, 0x306, EVENT(Func_020005fc)},
    {0x50008805, 108, 0x307, EVENT(Func_0200060c)},
    {TRIGGERS_END},
};
