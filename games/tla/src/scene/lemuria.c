/// @file
/// Lemuria, the hidden city beyond the Sea of Time: its doors and houses, the
/// children, the spring, the vine Growth raises, the treasure Scoop finds, and
/// the water's rippling scroll and cycling colours.

#include "shared/tables.h"
#include "flags.h"
#include "types.h"
#include "system/iwram_call.h"
#include "actor.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "call.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "map.h"
#include "event_work.h"
#include "actor_view.h"
#include "module/field/field.h"
#include "module/system/system.h"
#include "objcmd.h"
#include "text/msg_ids.h"

void FieldCamera_WaitForArrival();

void Rope_Lash();

void Rope_Place();

void Lemuria_TakeLeaderLayer(void);
extern u32 gLemuria_FirstWalkerPaces[];
extern u32 gLemuria_SecondWalkerPaces[];
extern s16 gLemuria_DoorOpening[];
extern s16 gLemuria_WideDoorOpening[];

#include "shared/position_distance.h"

s32 Actor_IsFacingNear(struct Actor *self, struct Actor *other, s32 range) {
  s32 result = 0;
  s32 *to;
  s32 *from;

  to = &other->x;
  from = &self->x;
  if (Position_Distance(to, from) < range) {
    u32 angle = (u16)ArcTan2(other->z - self->z, *to - *from);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = self->facing & 0xf000;

    if (ahead == facing || right == facing || left == facing)
      result = 1;
  }
  return result;
}

s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Actor_SelectAnimation(struct Actor *object, s32 mode);

s32 Lemuria_NoticeLeader(struct Actor *self) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *state;
  u32 mode;
  s8 *effect;
  struct Actor *other;

  Actor_SetSpritePriorityDirect(self, BattleEffect_GetObjectPriority(Actor_Get(
                                          gMainRoutineWork.currentOwner)));
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
  if (Position_Distance(&self->x, &other->x) <= 11)
    goto far;
  if (Actor_IsFacingNear(self, self->watchedObject, 18) != 0)
    goto near;
  if (Actor_IsFacingNear(self, Actor_Get(gMainRoutineWork.currentOwner), 26) !=
      0)
    goto near;
far:
  *state = 0;
  Actor_SelectAnimation(self, 2);
  return 0;
near:
  *state = 1;
  Actor_SelectAnimation(self, 1);
  return 0;
}

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

void Actor_ReturnHome(s32 a, s32 b, s32 c);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
s32 MapGrid_GetAttributeBAt(s32 x, s32 z, s32 layer);

extern struct WarpTarget gLemuria_FirstDoor;
extern struct WarpTarget gLemuria_SecondDoor;
extern struct WarpTarget gLemuria_PalaceDoor;
extern struct WarpTarget gLemuria_Door12;
extern struct WarpTarget gLemuria_Door11;
extern struct WarpTarget gLemuria_Door10;
extern struct WarpTarget gLemuria_Door16;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Lemuria_PoseActor12(void) {
  Actor_SetPosition(12, 488, 1256);
  Actor_SetFirstSpriteAnimation(Actor_Get(12), 157);
  Actor_SetAnimation(12, 3);
  Actor_FaceDirection(12, FACING_WEST, 0);
}

void Lemuria_OpenDoorAndEnter(const struct WarpTarget *warp, s32 door,
                              s32 mode) {
  s32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(warp->scene, warp->x, warp->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
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
  const struct WarpTarget *warp = &gLemuria_FirstDoor;
  s32 mode = 0;

  if (door == 3 && Event_GetFlag(0x96a)) {
    FieldEvent_SetMessageId((s32)&MsgTheDoorIs);
    Event_ShowMessage(-1, 0);
    return;
  }
  if (door == 1) {
    warp = &gLemuria_FirstDoor;
    mode = 0;
  }
  if (door == 2) {
    warp = &gLemuria_SecondDoor;
    mode = 0;
  }
  if (door == 3) {
    warp = &gLemuria_PalaceDoor;
    mode = 1;
  }
  if (door == 12) {
    warp = &gLemuria_Door12;
    mode = 0;
  }
  if (door == 11) {
    warp = &gLemuria_Door11;
    mode = 1;
  }
  if (door == 10) {
    warp = &gLemuria_Door10;
    mode = 1;
  }
  if (door == 16) {
    warp = &gLemuria_Door16;
    mode = 0;
  }
  Lemuria_OpenDoorAndEnter(warp, door, mode);
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
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

void Lemuria_DigFirstSpot(void) { Func_02001d44_(37, 72, 2, 2, 39, 71, 2); }

void Lemuria_DigSecondSpot(void) { Func_02001d44_(59, 64, 4, 3, 49, 71, 2); }

void Lemuria_DigThirdSpot(void) { Func_02001d44_(52, 69, 1, 1, 53, 69, 2); }

void Lemuria_DigFourthSpot(void) { Func_02001d44_(53, 64, 1, 1, 52, 64, 2); }

void Lemuria_DigFifthSpot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02001d44_(59, 64, 4, 3, 49, 71, 2);
  if (Event_GetFlag(0xfbb) == 0) {
    Actor_SetPosition(69, 808, 1176);
    if (leader->x >> 20 == 50 && leader->z >> 20 == 73) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = px(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Teleport_PlaySparksAndFade(void);

void Lemuria_LeaveForAncientLemuria(void) {
  struct MainRoutineWork *party;

  Teleport_PlaySparksAndFade();
  Event_SetDestination((s32)&SceneId_Lemuria, 2);
  party = &gMainRoutineWork;

  party->unk2c2 = 2;
}

void Lemuria_GrowVine(void) {
  s32 x;
  s32 z;
  s32 flag;
  s32 value;
  s32 old;

  flag = 0x900;
  /* FAKEMATCH(barrier): the empty asm on flag. */
  asm volatile("" : "+l"(flag));
  x = 0x3580000;
  z = 0x3f80000;
  Event_SetFlag(flag + 0x63);
  value = MapGrid_GetAttributeBAt(x, z, 0);
  /* FAKEMATCH(temporary): the first z copied to old. */
  old = z;
  z = 0x4080000;
  MapGrid_SetAttributeBAt(x, old, 0, value | 64);
  MapGrid_SetAttributeBAt(x, z, 0, MapGrid_GetAttributeBAt(x, z, 0) | 64);
}

/// Scoop digs up a buried treasure (Actor_ReturnHome): the helper, our name,
/// Loho's, Taopo Swamp's and the Yampi Desert Cave's Scoop spots use too.
static __inline__ void Lemuria_DigUpTreasure(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

void Lemuria_ScoopFirstTreasure(s32 a) { Lemuria_DigUpTreasure(a, 64, 0x300); }

void Lemuria_ScoopSecondTreasure(s32 a) { Lemuria_DigUpTreasure(a, 65, 0x301); }

void Lemuria_ScoopThirdTreasure(s32 a) { Lemuria_DigUpTreasure(a, 66, 0x302); }

void Lemuria_ScoopFourthTreasure(s32 a) { Lemuria_DigUpTreasure(a, 67, 0x303); }

void Lemuria_ScoopFifthTreasure(s32 a) { Lemuria_DigUpTreasure(a, 68, 0x304); }

void Lemuria_ScoopSixthTreasure(s32 a) { Lemuria_DigUpTreasure(a, 70, 0x306); }

void Lemuria_ScoopSeventhTreasure(s32 a) {
  Lemuria_DigUpTreasure(a, 71, 0x307);
}

void Lemuria_ScoopEighthTreasure(s32 a) { Lemuria_DigUpTreasure(a, 72, 0x308); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Party_FindOwnerWithAction();
void Rope_Coil();

/// A door that will not open, until a Lemurian inside takes Piers's companions
/// in another way.
/// "The door doesn't want to open."
void Lemuria_KnockAtBrokenDoor(void) {
  s32 v5;
  struct Actor *object;

  if (Actor_Get(gMainRoutineWork.currentOwner)->y > px(96) ||
      Event_GetFlag(0x969)) {
    FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x15a), s16, 0) = 1;
    return;
  }
  if (!Event_GetFlag(0x966) || Event_GetFlag(0x968)) {
    FieldEvent_SetMessageId((s32)&MsgTheDoorDoesn);
    Event_ShowMessage(-1, 0);
    return;
  }
  Event_SetFlag(0x968);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgAreYouKids);
  Actor_SetPosition(12, 872, 880);
  object = Actor_Get(12);
  Actor_SetFirstSpriteAnimation(object, 43);
  Actor_SetAnimation(12, 3);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 776, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(12, 4);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 856, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 928, 1);
  FieldCamera_WaitForArrival();
  v5 = Party_FindOwnerWithAction(133);
  if (v5 != -1 || Event_GetFlag(0x962))
    goto done;
  FieldEvent_SetMessageId((s32)&MsgWhatSThe);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(4, 856, 952);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldCamera_MoveToFixed(px(872), v5, px(792), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveToFixed(px(872), v5, px(856), 1);
  FieldActor_Jump(12, 4, 0);
  Actor_WalkToAndWait(12, 872, 912);
  FieldEvent_Wait(20);
  Actor_SetSpeed(12, 1);
  Actor_WalkByAndWait(12, -32, 0);
  Actor_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP);
  FieldCamera_MoveToFixed(px(848), v5, px(904), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_SetPosition(28, 840, 920);
  Actor_SetPosition(27, 840, 936);
  Value3((s32 (*)())Rope_Coil, 1, 28, 27);
  Rope_Lash(1, 28, 27);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_WalkByAndWait(12, 32, 0);
  EventCamera_FollowActor(4, 1);
  Actor_WalkByAndWait(12, 0, -32);
  Event_SetFlag(0x96f);
done:
  Lemuria_PoseActor12();
  Event_End();
}

void Party_RestoreHpPp(s32 value);

/// "Felix and the others drank of the spring. Their HP and PP were fully
/// replenished!"
void Lemuria_DrinkFromSpring(void) {
  s32 y = Actor_Get(gMainRoutineWork.currentOwner)->y;

  if (y < 0x5a0000) {
    Message_ShowCentered((s32)MsgAndTheOthers, 1);
    Event_PlaySound(SE_RECOVERY);
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
  Actor_ShowEmote(30, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
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
    Actor_SetSpeed(30, 1.2);
    Actor_WalkToAndWait(30, 440, 1048);
    Event_SetFlag(0x908);
  }
  Actor_FaceDirectionAndWait(30, FACING_NORTH + FACING_STEP);
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern u8 gLemuria_RippleBuffer;
extern s16 gLemuria_RippleLines[2][160];

/// Every fourth frame rotates colours 1-14 of background palettes 7 and 8 by
/// one place. The loops are written with goto:
/// the bytes rebuild the palette address on every pass.
void Lemuria_CycleWaterColors(void) {
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

/// Puts the per-line BG2 horizontal scroll on an HBlank DMA.
void Lemuria_StartRippleDma(void) {
  s16 *lines = gLemuria_RippleLines[gLemuria_RippleBuffer];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH(barrier): the empty asm reading first. */
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

void Lemuria_ComputeRipple(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  s16 *lines = gLemuria_RippleLines[gLemuria_RippleBuffer ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    sine = Math_Sin((*count + (u8)gFrameCount + (layer->y >> 16)) << 9);
    v[0] = sine;
    *lines++ = (layer->x >> 16) + (sine >> 15) + *count % 3 - 1;
  }
  gLemuria_RippleBuffer ^= 1;
}

void Lemuria_StartRipple(void) {
  s32 priority;

  priority = 0x480;
  Task_AddFrame(Lemuria_ComputeRipple, priority);
  priority = 0xc80;
  Task_AddFrame(Lemuria_StartRippleDma, priority);
}

void FieldEffect_StartBlankOverlay();
void HangingVine_Place();
void Actor_SetWideSprite();

s32 Scene_Initialize(void) {
  struct Actor *actor;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x201;
  FieldTask_AddFrame((s32)Lemuria_CycleWaterColors, 0xc80);
  FieldEffect_StartBlankOverlay(0);
  Actor_SetSpriteFlags(Actor_Get(11), 0);
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(29, 3);
  Actor_Get(21)->update = Lemuria_TakeLeaderLayer;
  Actor_SetAnimation(22, 3);
  Actor_SetPosition(18, 40, 952);
  Actor_SetScript(18, (s32)&gLemuria_FirstWalkerPaces);
  actor = Actor_Get(18);
  actor->watchedObject = Actor_Get(19);
  actor->update = Lemuria_NoticeLeader;
  Actor_SetPosition(19, 88, 904);
  Actor_SetScript(19, (s32)&gLemuria_SecondWalkerPaces);
  actor = Actor_Get(19);
  actor->watchedObject = Actor_Get(18);
  actor->update = Lemuria_NoticeLeader;
  if (Event_GetFlag(0x908)) {
    Actor_SetPosition(30, 440, 1048);
  }
  if (Event_GetFlag(0x964) && !Event_GetFlag(0x969)) {
    Actor_SetPosition(24, 456, 520);
    Actor_SetPosition(25, 472, 520);
  }
  if (!Event_GetFlag(0x969)) {
    Actor_SetPosition(26, 0, 0);
  } else {
    Actor_Get(26)->update = Lemuria_TakeLeaderLayer;
  }
  if (Event_GetFlag(0x966) && !Event_GetFlag(0x969)) {
    Actor_SetPosition(7, 56, 1016);
    Actor_SetAnimation(7, 9);
  }
  Rope_Initialize();
  Rope_Place(0, 0x962, 8, 9);
  Actor_Get(8)->priorityFlags |= 32;
  Actor_Get(9)->priorityFlags |= 32;
  if (Event_GetFlag(0x96f)) {
    FieldActor_SetSpritePriority(28, 2);
    FieldActor_SetSpritePriority(27, 2);
    Actor_SetPosition(28, 840, 920);
    Actor_SetPosition(27, 840, 936);
    Call4(Rope_Place, 1, 0x96f, 28, 27);
  }
  Actor_SetSpriteFlags(Actor_Get(28), 0);
  if (Event_GetFlag(0x963)) {
    HangingVine_Place(10, 1);
    Lemuria_GrowVine();
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_03)) {
    Actor_SetWideSprite(67, 0);
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_04)) {
    Actor_SetWideSprite(68, 0);
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_06)) {
    Actor_SetWideSprite(70, 0);
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_07)) {
    Actor_SetWideSprite(71, 0);
  }
  if (Event_GetFlag(0x966)) {
    MapGrid_CopyCellsLayered(3, 1, 1, 1, 54, 45);
  }
  actor = Actor_Get(31);
  actor->motionFlags = 0;
  actor->terrainHeight = px(64);
  actor->y = px(64);
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
  Actor_SetFirstSpriteAnimation((s32)Actor_Get(12), 56);
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
  Actor_SetSpeed(12, 1);
  EventCamera_FollowActor(12, 1);
  Actor_WalkToAndWait(12, 464, 952);
  Actor_FaceDirectionAndWait(12, a);
  FieldEvent_Wait(10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_FaceDirectionAndWait(12, b);
  FieldEvent_Wait(10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_WalkToAndWait(12, 464, 784);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_Jump(12, 6, 15);
  FieldActor_Jump(12, 6, 23);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(12, c);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  FieldEvent_Wait(20);
  FieldActor_SetSpritePriority(4, 1);
  Actor_SetPosition(4, 464, 856);
  Actor_WalkToAndWait(4, 464, 796);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(12, 3);
  FieldEvent_Wait(10);
  Actor_SetSpeed(12, 1.2);
  Actor_SetAnimation(12, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  FieldEvent_Wait(10);
  Actor_Get(4)->priorityFlags |= 1;
  Actor_WalkToAndWait(4, 464, 776);
  Lemuria_PoseActor12();
  Event_End();
}

/// "Where are you going, Felix? Let's take a longer look around!"
void Lemuria_KeepLookingAround(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhereAreYou);
  Actor_SetFirstSpriteAnimation((s32)Actor_Get(12), 56);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldActor_SetSpritePriority(12, 1);
  FieldActor_LaunchFromLeader(12, 0, 16, FACING_NORTH + FACING_STEP);
  FieldActor_WaitForMoveById(12);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_SetSpeed(12, 1.2);
  Actor_SetAnimation(12, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(4, 0, -16);
  Lemuria_PoseActor12();
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
  Actor_SetSpeed(24, 1);
  Actor_SetSpeed(25, 1);
  Actor_SetFirstSpriteAnimation((s32)Actor_Get(12), 56);
  Actor_WalkToAndWait(4, 472, 568);
  FieldActor_LaunchFromLeader(12, -16, 0, FACING_NORTH + FACING_STEP);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(12);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 544, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_Jump(12, 6, 15);
  FieldActor_Jump(12, 6, 23);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(24, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
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
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldActor_SetSpritePriority(25, 2);
  Actor_WalkByAndWait(25, 16, 0);
  Actor_FaceDirection(25, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
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
  Actor_ShowEmote(24, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  FieldActor_LaunchFromLeader(7, -8, -16, FACING_NORTH);
  FieldActor_WaitForMoveById(7);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_StartRepeatedMotion(24, 3);
  Actor_SetAttachedEffect(24, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
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
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(24, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  FieldActor_WatchAndSetScript(4, 12);
  FieldActor_WatchAndSetScript(7, 12);
  FieldActor_WatchAndSetScript(24, 12);
  FieldActor_WatchAndSetScript(25, 12);
  Actor_SetSpeed(12, 1);
  EventCamera_FollowActor(12, 1);
  Actor_WalkToAndWait(12, 424, 600);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(400, CAMERA_KEEP, 648, 1);
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
  EventCamera_FollowActor(12, 1);
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
  FieldCamera_MoveTo(440, CAMERA_KEEP, 288, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 528, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(12, 3);
  FieldEvent_Wait(10);
  EventCamera_FollowActor(12, 1);
  Actor_WalkToAndWait(12, 456, 568);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 544, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(25, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetSpeed(7, 0.7);
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
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
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
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
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
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 45);
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
  Actor_SetSpeed(7, 1.2);
  Actor_SetAnimation(7, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(7, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Actor_SetSpeed(12, 1.2);
  Actor_SetAnimation(12, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  Lemuria_PoseActor12();
  Event_End();
}

void Actor_TakeLeaderLayer(void);

/// An actor's per-frame hook: it is drawn on the party leader's layer.
void Lemuria_TakeLeaderLayer(void) { Actor_TakeLeaderLayer(); }

extern s16 gLemuria_ThirdDoorOpening[];

// clang-format off
u32 gLemuria_FirstWalkerPaces[] = {                                                               
      SC_ARG(2),                                                               
      SC_SET(SW_SPEED, 0x10000),                                               
      SC_SET(SW_ACCEL, 0x10000),                                               
      SC_LABEL(1),                                                             
      SC_MOVE(px(40), 0, px(952)),                                      
      SC_SYNC,                                                                 
      SC_MOVE(px(40), 0, px(904)),                                      
      SC_SYNC,                                                                 
      SC_MOVE(px(88), 0, px(904)),                                      
      SC_SYNC,                                                                 
      SC_MOVE(px(88), 0, px(952)),                                      
      SC_SYNC,                                                                 
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
  };

u32 gLemuria_SecondWalkerPaces[] = {                                                               
      SC_ARG(2),                                                               
      SC_SET(SW_SPEED, 0x10000),                                               
      SC_SET(SW_ACCEL, 0x10000),                                               
      SC_LABEL(1),                                                             
      SC_MOVE(px(88), 0, px(904)),                                      
      SC_SYNC,                                                                 
      SC_MOVE(px(88), 0, px(952)),                                      
      SC_SYNC,                                                                 
      SC_MOVE(px(40), 0, px(952)),                                      
      SC_SYNC,                                                                 
      SC_MOVE(px(40), 0, px(904)),                                      
      SC_SYNC,                                                                 
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 460, 0, 550, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Lemuria),
    SCENE_EXIT_UNLESS(1, LemuriaInteriors, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, LemuriaInteriors, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, LemuriaInteriors, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AncientLemuria, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, LemuriaSpring, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, LemuriaInteriors, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, LemuriaInteriors, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, LemuriaInteriors, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, LemuriaPalace, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, LemuriaPalace, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, LemuriaPalace, 17, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 952, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 880, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(157, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 1256, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(156, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 1040, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(160, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 1064, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(165, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 856, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(164, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 920, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(166, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 920, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(167, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 952, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(168, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 904, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(216, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 80, 0, 1104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(189, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 392, 0, 712, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(80, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 1080, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(203, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 984, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 520, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 520, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(166, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 552, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(157, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 1008, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(160, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 464, 0, 1016, FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 488, 0, 984, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

s16 gLemuria_DoorOpening[] = {
    2, 1, 1, 1, 6, 2, 2, 1,
    1, 6, -1,
};

s16 gLemuria_WideDoorOpening[] = {
    0, 1, 2, 1, 6, 0, 2, 2,
    1, 6, -1,
};

s16 gLemuria_ThirdDoorOpening[] = {
    3, 1, 1, 1, 0, -1,
};
// clang-format on

struct WarpTarget gLemuria_FirstDoor = {(s32)gLemuria_DoorOpening, 22, 47};

struct WarpTarget gLemuria_SecondDoor = {(s32)gLemuria_DoorOpening, 35, 47};

struct WarpTarget gLemuria_PalaceDoor = {(s32)gLemuria_WideDoorOpening, 9, 40};

struct WarpTarget gLemuria_Door12 = {(s32)gLemuria_ThirdDoorOpening, 54, 45};

struct WarpTarget gLemuria_Door11 = {(s32)gLemuria_WideDoorOpening, 28, 20};

struct WarpTarget gLemuria_Door10 = {(s32)gLemuria_WideDoorOpening, 28, 13};

struct WarpTarget gLemuria_Door16 = {(s32)gLemuria_DoorOpening, 54, 50};

struct Trigger gSceneEvents[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 5, 0x1908, 5},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, 0x1966,
     EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, 0x1969,
     EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgMoreOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgIHavenT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgIVeAged)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgSometimesIThink)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgLongAgoA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgKingHydrosIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     EVENT(Lemuria_TalkToSpoiledChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     EVENT(Lemuria_TalkToLonelyChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgNoOneIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgThereSSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgLemuriansAreSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgIfItWeren)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 7, CONDITION_ALWAYS,
     MSGNO(MsgMotherWhyHave)},
    {TRIGGER_TALK, 24, 0x196a, MSGNO(MsgIVeNever)},
    {TRIGGER_TALK, 25, 0x196a, MSGNO(MsgLordConservatoHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x196a, MSGNO(MsgIMSureThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x196a, MSGNO(MsgHowCouldLord)},
    {TRIGGER_TALK, 24, 0x1969, MSGNO(MsgKingHydrosAwaits)},
    {TRIGGER_TALK, 25, 0x1969, MSGNO(MsgHisHighnessThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x1969,
     MSGNO(MsgTheyAreClearly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x1969, MSGNO(MsgWhatDoesHis)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTheKingIs)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgTheKingWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgHowMuchLongerWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgIHadHeard)},
    {TRIGGER_TALK, 26, 0x196a, MSGNO(MsgIfYouAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, 0x196a, MSGNO(MsgIKnowWhy)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTheSenateHolds)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgITrustThe)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 11, CONDITION_ALWAYS,
     MSGNO(MsgHouseOfThe)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgDrinkingFromThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgTheseTurtlesHave)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(Lemuria_OfferSpringLuck)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgNoOneIs)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Lemuria_DrinkFromSpring)},
    {TRIGGER_EXAMINE, 25, CONDITION_ALWAYS, EVENT(Lemuria_DrinkFromSpring)},
    {TRIGGER_EXAMINE, 40, CONDITION_ALWAYS, EVENT(Lemuria_KnockAtBrokenDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x962, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 10, 0x963, EVENT(Lemuria_GrowVine)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(Lemuria_DigFirstSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(Lemuria_DigSecondSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32,
     CONDITION_ALWAYS, EVENT(Lemuria_DigThirdSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(Lemuria_DigFourthSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 38,
     CONDITION_ALWAYS, EVENT(Lemuria_DigFifthSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 104, 0x303,
     EVENT(Lemuria_ScoopFourthTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 105, 0x304,
     EVENT(Lemuria_ScoopFifthTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 107, 0x306,
     EVENT(Lemuria_ScoopSixthTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 108, 0x307,
     EVENT(Lemuria_ScoopSeventhTreasure)},
    {TRIGGER_TABLE_END},
};
