/// @file
/// Loho, the miners' village whose wall the party blows open with the
/// cannon, which the miners then give it; its ladder down to the mine, its
/// houses' doors and the treasure Scoop digs up.

#include "shared/tables.h"
#include "flags.h"
#include "system/io_reg.h"
#include "item_ids.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "system/dma.h"
#include "field/field_sequence_event.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "graphics/resource.h"
#include "objcmd.h"
#include "text/msg_ids.h"

extern u32 gLoho_VillagerMovesNorth[];
extern u32 gLoho_VillagerMovesWest[];

extern u32 gLoho_VillagerReturns[];

void Loho_ClimbDownLadder(void);
extern u32 gLoho_Regions[];
extern struct ActorPlacement gLoho_Placements[];
extern struct ActorPlacement gLoho_Room2Placements[];
extern u32 gLoho_FirstMinerGoesToWall[];
extern s32 gLoho_SecondMinerStepsUp[];

extern s32 gLoho_ThirdMinerStepsUp[];

extern u32 gLoho_FirstMinerWalksBack[];
extern u32 gLoho_FirstMinerFetchesCannon[];
extern u32 gLoho_FirstMinerReturns[];
extern s32 gLoho_ForemanComes[];
extern struct Trigger gLoho_Triggers[];
extern struct Trigger gLoho_Room1Triggers[];
extern struct Trigger gLoho_Room2Triggers[];
extern struct LohoPicture gLoho_HouseDoor;
extern u32 gLoho_VillagerPaces[];

extern s16 gLoho_DoorOpening[];

s32 Oam_AddSpriteAtDepthClamped();

/// One of the 96 sparks the cannon's blast throws over the screen: the link the
/// sprite list threads it by, its sprite, and its age in frames, negative while
/// it waits to appear.
struct LohoSpark {
  struct LohoSpark *next;
  union {
    u32 words[2];
    u16 halves[4];
    u8 bytes[8];
    struct {
      u16 y : 8;
      u16 attr0 : 8;
      u16 x : 9;
      u16 attr1 : 7;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
    } oam;
  } sprite;
  s32 age;
};

/// Lull, the Jupiter Djinni behind the wall the cannon breaks, joins the party
/// when spoken to.
void Loho_MeetLull(void) { Djinn_JoinParty(19, 3, 16); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const u32 *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Loho2)
    return gLoho_Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Loho2)
    return gLoho_Room2Placements;
  return gLoho_Placements;
}

void Loho_DrawBrokenWall(void) {
  MapGrid_CopyCellAttributes(69, 70, 10, 12, 24, 7);
  MapGrid_CopyCellsLayered(69, 70, 10, 12, 24, 7);
  MapGrid_CopyCellAttributes(79, 70, 10, 12, 88, 7);
  MapGrid_CopyCellsLayered(79, 70, 10, 12, 88, 7);
  MapGrid_CopyCellAttributes(69, 82, 10, 12, 24, 71);
  MapGrid_CopyCellsLayered(69, 82, 10, 12, 24, 71);
}

/// "Did you want this cannon?"
void Loho_AskWantCannon(s32 actor) {
  s32 msg = (s32)MsgLoho23;

  Event_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// "It looks like it's empty."
void Loho_ExamineCannon(void) {
  FieldMessage_Show((s32)MsgLoho27, 1);
  gFieldWork->fieldMenu = 1;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void FieldCamera_WaitForArrival();

/// The Magma Ball used on the cannon: it is loaded and fired at the ancient
/// wall north of town, which is breached, and the miners give the party the
/// cannon for its ship.
/// "Is everyone all right?"
void Loho_FireCannon(void) {
  s32 v_r1, v_r2, v_r0;
  struct Actor *t147;
  struct Actor *t0;
  struct Actor *t226;
  struct Actor *t233;
  struct LohoSpark *p;
  struct LohoSpark *table;
  s32 tiles, entry, i, value, zero;
  s32 t38, t47, t48, t49, t50, t51;
  s32 t189;
  s32 t190, t193, t194;
  u8 unused[4]; // Unused: gives the stack frame its 4 bytes.

  t0 = Actor_GetById(4);
  Event_Start();
  Event_ResolvePendingActions(0);
  p = (struct LohoSpark *)t0;
  goto start;
approach:
  if (v_r1 == 0)
    goto turned;
  if (i == 0)
    goto fromEast;
  Actor_WalkToAndWait(4, 471, 346);
  goto turned;
fromEast:
  Actor_WalkToAndWait(4, 506, 346);
turned:
  if (i == 0)
    goto fromNorth;
  goto fromSouth;
fromNorth:
  Actor_WalkToAndWait(4, 506, 373);
  goto inPlace;
start:
  i = 0;
  v_r1 = 0;
  v_r2 = 0;
  if (FIELD_AT(p, s32, 8) > 0x1e8ffff)
    goto checkZ;
  i = 1;
checkZ:
  v_r0 = FIELD_AT(p, s32, 16);
  if (v_r0 > 0x15affff)
    goto checkSouth;
  v_r1 = 1;
  goto placed;
checkSouth:
  if (v_r0 <= 0x1720000)
    goto placed;
  v_r2 = 1;
placed:
  if (v_r2 == 0)
    goto approach;
inPlace:
  FieldCamera_MoveTo(488, CAMERA_KEEP, 372, 1);
  Event_SetFlag(0xa5f);
  Actor_MoveToAndWait(4, 487, 377);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(30);
  Event_PlaySound(SE_SPIN);
  FieldActor_StartRepeatedMotion(16, 2);
  FieldActor_StartRepeatedMotion(18, 2);
  Actor_RunRepeatedMotion(17, 2);
  Actor_FaceActor(16, 4, 0);
  Actor_FaceActor(17, 4, 0);
  Actor_FaceDirection(18, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(25);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_WalkByAndWait(4, 24, 12);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(30);
  Event_PlaySound(SE_CLATTER);
  FieldActor_ShowItemIconById(21, ITEM_MAGMA_BALL);
  Actor_SetPosition(21, 488, 330);
  Actor_SetDestinationOffset(20, 0, 8);
  Actor_SetSpeed(21, 3);
  MapGrid_SetShake(2, 2, 1);
  Actor_SetDestination(21, 488, 300);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 308, 1);
  Actor_WaitForMoveById(21);
  Actor_SetPosition(21, 0, 0);
  MapGrid_SetShake(7, 7, 1);
  table = (struct LohoSpark *)Heap_AllocEwram(0x600);
  p = (struct LohoSpark *)Heap_Alloc(0x1000);
  t38 = Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion);
  Decompress_TaggedLz(t38, p);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, p);
  Heap_Release(p);
  p = (struct LohoSpark *)&table->age;
  i = 95;
seedSparks:
  *(s32 *)p = -((u32)(Random16() << 5) >> 16);
  i = (i - 1);
  p = (struct LohoSpark *)((s32)p + 16);
  if (i >= 0)
    goto seedSparks;
  Dma_Set((const void *)(s32)Field_Palette685, (void *)0x50003e0,
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  i = 0;
  /* FAKEMATCH(dead): this test that never passes, with its edge into the loop.
   */
  if (((entry & 255) >> 8) == 0)
    goto frame;
  i = 1;
  goto ageSpark;
frame:
  if ((31 & i) != 0)
    goto checkFlash;
  Event_PlaySound(SE_CRASH);
checkFlash:
  if (i != 30)
    goto sparks;
  ScreenFade_ToWhite(90);
  Event_PlaySound(SE_RUMBLE);
sparks:
  p = table;
spark:
  if (p->age != 0)
    goto drawSpark;
  p->sprite.words[0] = 0x40000000;
  p->sprite.words[1] = 0;
  t47 = (u16)((((u32)(Random16() * 96) >> 16) + 72) & 0x1ff);
  t48 = p->sprite.halves[1];
  p->sprite.halves[1] = (t48 & -0x200) | t47;
  p->sprite.oam.y = (u32)Random16() >> 10;
  t51 = p->sprite.bytes[5];
  t51 |= 240;
  p->sprite.bytes[5] = t51 & -13;
drawSpark:
  value = *(long *)&p->age;
  if (value < 0)
    goto ageSpark;
  t49 = (u16)(((value >> 2) * 4 + tiles) & 0x3ff);
  t50 = p->sprite.halves[2];
  p->sprite.halves[2] = (t50 & -0x400) | t49;
  Oam_AddSpriteAtDepthClamped(p, 100);
ageSpark:
  p->age += 1;
  if (p->age <= 31)
    goto nextSpark;
  p->age = -((u32)(Random16() * 7) >> 16);
nextSpark:
  p++;
  if ((s32)p <= (s32)&table[95])
    goto spark;
  wait(1);
  i = (i + 1);
  if (i <= 119)
    goto frame;
  VramCache_FreeEntry(entry);
  Heap_Release(table);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 390, 1);
  Actor_SetPositionAndFacing(14, px(248), px(480), FACING_EAST);
  Loho_DrawBrokenWall();
  MapGrid_SetShake(0, 0, 1);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(180);
  ScreenFade_FromWhite(120);
  Event_SetMessageId((s32)MsgLoho28);
  Event_Wait(120);
  Actor_RunRepeatedMotion(18, 2);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(17, FACING_SOUTHEAST, 0);
  Actor_SetAnimation(17, 3);
  Actor_SetAnimation(16, 3);
  Event_Wait(30);
  FieldActor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
  Event_ShowMessage(17, 0);
  FieldActor_ShowEmote(16, EMOTE_IN_FRONT | EMOTE_SCOWL, 35);
  Actor_WalkByAndWait(16, -12, 24);
  Actor_RunRepeatedMotion(16, 2);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(4, 3);
  Actor_FaceDirectionAndWait(18, FACING_NORTH_NORTHWEST);
  Event_Wait(30);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 35);
  Event_ShowMessage(18, 0);
  FieldActor_ShowEmote(16, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
  Actor_FaceActor(16, 18, 0);
  Actor_FaceDirection(17, FACING_SOUTHEAST, 0);
  Event_Wait(60);
  Actor_FaceDirection(18, FACING_WEST, 0);
  Event_Wait(60);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
  Event_ShowMessage(18, 0);
  Actor_FaceEachOther(16, 17, 0);
  Event_Wait(40);
  Actor_FaceDirection(16, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTH, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHWEST, 0);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 326, 1);
  Event_Wait(60);
  FieldActor_ShowEmote(16, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  FieldActor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(16, 3);
  Event_ShowMessage(16, 0);
  Actor_FaceActor(18, 4, 0);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 35);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 366, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(18, FACING_SOUTH_SOUTHWEST, 0);
  Event_ShowMessage(18, 0);
  Actor_SetSpeed(17, 0.8);
  Actor_WalkBy(17, 0, 16);
  Actor_WaitForMoveById(17);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHEAST);
  Actor_SetAnimation(17, 4);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(16, 3);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(18, 3);
  Actor_FaceDirection(17, FACING_SOUTH_SOUTHEAST, 0);
  Event_ShowMessage(17, 0);
  FieldActor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
  Event_Wait(60);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_SOUTH);
  Event_Wait(20);
  Actor_RunRepeatedMotion(16, 2);
  Event_ShowMessage(16, 0);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldActor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldActor_ShowEmote(16, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 35);
  Event_ShowMessage(18, 0);
  Actor_SetAnimation(17, 3);
  Event_Wait(40);
  FieldActor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 35);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(16, 3);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(542, CAMERA_KEEP, 256, 1);
  zero = 0;
  t147 = Actor_GetById(16);
  t147->motionFlags = zero;
  Actor_SetSpeed(16, 1.5);
  Actor_SetScript(16, (s32)gLoho_FirstMinerGoesToWall);
  Actor_SetSpeed(18, 0.5);
  Actor_SetScript(18, (s32)gLoho_SecondMinerStepsUp);
  Actor_SetSpeed(17, 0.4);
  Actor_SetScript(17, (s32)gLoho_ThirdMinerStepsUp);
  Actor_RefreshSelectorById(16);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHWEST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_WEST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_NORTHWEST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_NORTHEAST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_EAST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_SOUTH_SOUTHWEST);
  Event_Wait(40);
  Actor_SetAnimation(16, 3);
  Event_Wait(60);
  Event_ShowMessage(16, 0);
  Actor_RefreshSelectorById(17);
  Actor_RefreshSelectorById(18);
  Actor_FaceEachOther(17, 18, 0);
  Event_Wait(20);
  goto askCannon;
askCannon:
  Actor_SetAnimation(18, 3);
  Actor_SetAnimation(17, 3);
  Event_Wait(30);
  Actor_FaceActor(17, 4, 0);
  Actor_SetSpeed(16, 1);
  Actor_SetScript(16, (s32)gLoho_FirstMinerWalksBack);
  FieldCamera_SetSpeed(1.8);
  FieldCamera_MoveTo(542, CAMERA_KEEP, 350, 1);
  FieldCamera_WaitForArrival();
  Event_OpenMessage(17, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0))
    goto answeredNo;
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(18, 0);
  t189 = (s32)Ram_HeapSlots->fieldWork;
  t190 = FIELD_AT(t189, u16, 452);
  FIELD_AT(t189, u16, 452) = (t190 + 1);
  goto crewLeaves;
fromSouth:
  Actor_WalkToAndWait(4, 471, 373);
  goto inPlace;
answeredNo:
  Actor_SetAnimation(18, 4);
  t193 = (s32)Ram_HeapSlots->fieldWork;
  t194 = FIELD_AT(t193, u16, 452);
  FIELD_AT(t193, u16, 452) = (t194 + 1);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(18, 0);
crewLeaves:
  Actor_WalkBy(18, 0, 24);
  Actor_WalkByAndWait(17, 0, 24);
  Actor_FaceDirection(17, FACING_EAST, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Actor_RefreshSelectorById(16);
  Actor_SetScript(16, (s32)gLoho_FirstMinerFetchesCannon);
  Actor_RefreshSelectorById(16);
  Actor_FaceDirection(18, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_EAST, 0);
  Actor_FaceDirection(16, FACING_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_SetAnimation(18, 3);
  Actor_SetAnimation(17, 3);
  Actor_SetAnimation(16, 3);
  Event_Wait(60);
  FieldActor_WatchAndSetScript(4, 16);
  Actor_WalkTo(18, 504, 370);
  Actor_WalkTo(17, 504, 352);
  Actor_SetScript(16, (s32)gLoho_FirstMinerReturns);
  Actor_WaitForMoveById(17);
  Actor_WaitForMoveById(18);
  Actor_RefreshSelectorById(16);
  Actor_FaceDirectionAndWait(16, FACING_EAST);
  Actor_SetSpeed(16, 0.3);
  Actor_SetSpeed(17, 0.3);
  Actor_SetSpeed(18, 0.3);
  Event_Wait(40);
  Actor_SetSpeed(20, 0.1);
  t226 = Actor_GetById(20);
  t226->motionFlags = 0;
  Actor_SetScript(20, (s32)gLoho_ForemanComes);
  Actor_RefreshSelectorById(20);
  Actor_SetSpeed(20, 0.3);
  FieldCamera_SetSpeed(0.3);
  FieldCamera_MoveTo(460, CAMERA_KEEP, 350, 1);
  Event_Wait(30);
  t233 = Actor_GetById(16);
  t233->actionFlags &= 254;
  Actor_WalkBy(18, -40, -20);
  Actor_WalkBy(17, -40, -20);
  Actor_WalkBy(16, -40, -20);
  Actor_WalkByAndWait(20, -40, -20);
  Actor_WalkBy(18, -138, 0);
  Actor_WalkBy(17, -138, 0);
  Actor_WalkBy(16, -138, 0);
  Actor_WalkByAndWait(20, -138, 0);
  Actor_SetPosition(16, 0, 0);
  Actor_SetPosition(18, 0, 0);
  Actor_SetPosition(17, 0, 0);
  Actor_SetPosition(20, 0, 0);
  Event_End();
  return;
}

/// "Now that the wall's been breached, we can get on with the mining! Good
/// work!"
void Loho_TalkAboutBreachedWall(void) {
  if (Event_GetFlag(0x90a))
    Event_SetMessageId((s32)MsgLoho32);
  else
    Event_SetMessageId((s32)MsgLoho55);
  Event_ShowMessage(11, 0);
}

void Loho_MindReadMiner(void) {
  if (Event_GetFlag(0x90a))
    Event_SetMessageId((s32)MsgLoho36);
  else
    Event_SetMessageId((s32)MsgLoho56);
  Event_ShowMessage(11, 0);
}

/// The Scoop spots: each digs up its treasure actor and sets its flag.
void Loho_ScoopFirstTreasure(s32 id) { Event_DigUpTreasure(id, 64, 0x300); }

#define DEFINE_SCOOP_TREASURE(name, n1, n2)                                    \
  void name(s32 id) { Event_DigUpTreasure(id, n1, 0x300 + n2); }

DEFINE_SCOOP_TREASURE(Loho_ScoopSecondTreasure, 65, 1)

DEFINE_SCOOP_TREASURE(Loho_ScoopThirdTreasure, 66, 2)

/// Lift raises the rock (flag 0x201), and the Golem Core under it shows until
/// it is dug up (flag 0xff7).
void Loho_LiftRock(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_01);
  Actor_GetById(22)->collisionFlags &= 0xfe;
  if (Event_GetFlag(0xff7) == 0)
    Actor_SetPosition(66, 520, 136);
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Loho1)
    return gLoho_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Loho2)
    return gLoho_Room2Triggers;
  return gLoho_Triggers;
}

/// "People staying at the inn can check in at the counter."
void Loho_ReadInnSign(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000)
    FieldMessage_Show((s32)MsgLoho57, 1);
}

void Loho_TalkToInnkeeper(void) {
  s32 count = 8;

  /* FAKEMATCH(barrier): the empty asm on count. */
  asm("" : "+l"(count));
  Inn_CheckIn(13, count);
}

/// "Commerce really took off when we started this mining colony in Loho."
void Loho_TalkAboutCommerce(void) {
  Actor_GetById(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(FLAG_AREA_TEMP_20)) {
    Shop_Run(30, 8);
  } else {
    if (Event_GetFlag(0xa5f))
      Event_SetMessageId((s32)MsgLoho51);
    else
      Event_SetMessageId((s32)MsgLoho17);
    Event_ShowMessage(8, 0);
  }
}

/// "If you're only here for the cannon, you can forget about it."
void Loho_TalkAboutCannon(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000) {
    Sanctum_Run(actor);
  } else {
    if (Event_GetFlag(0xa5f))
      Event_SetMessageId((s32)MsgLoho53);
    else
      Event_SetMessageId((s32)MsgLoho19);
    Event_ShowMessage(actor, 0);
  }
}

void Loho_VillagerStepsAsideNorth(void) {
  Actor_SetScript(8, gLoho_VillagerMovesNorth);
  Event_SetFlag(FLAG_AREA_TEMP_20);
}

void Loho_VillagerStepsAsideWest(void) {
  Actor_SetScript(8, gLoho_VillagerMovesWest);
  Event_SetFlag(FLAG_AREA_TEMP_20);
}

void Loho_VillagerStepsBack(void) {
  Event_ClearFlag(FLAG_AREA_TEMP_20);
  Actor_SetScript(8, gLoho_VillagerReturns);
}

static __inline__ void Loho_AnimateCells(const void *data, s32 a, s32 b) {
  MapGrid_AnimateCells(data, a, b);
}

void Loho_OpenDoorAndEnter(void) {
  u8 *work = Ram_HeapSlots->fieldWork;
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  i = 8;
loop:
  {
    struct Actor *object = Actor_GetById(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  work = (u8 *)&((struct FieldWork *)work)->stepTrigger;
  Event_PlaySound(SE_DOOR_OPEN);
  Loho_AnimateCells(gLoho_HouseDoor.data, gLoho_HouseDoor.a, gLoho_HouseDoor.b);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -4);
  Event_PlaySound(SE_MAP_EXIT);
  Event_Wait(6);
  Import_EventSetExitRequest(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Loho_EnterDoor(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  /* FAKEMATCH(dead): an empty once-only block. */
  do {
  } while (0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Event_PlaySound(SE_MAP_EXIT);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -4);
  Import_EventSetExitRequest(work->stepTrigger);
}

void Loho_ClimbDownLadder(void) {
  s32 value = gFieldWork->stepTrigger;

  if (value == 2)
    Actor_SetPosition(8, 144, 224);
  else
    Actor_SetPosition(8, 224, 224);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= 0xfe;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSpriteFlags(Actor_GetById(4), 0);
  Actor_SetAnimation(4, 13);
  Actor_SetDestinationOffset(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetAnimation(4, 10);
  Event_Wait(14);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(value);
  Event_End();
}

void Loho_StepOntoLadderFacingEast(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWait(4, 16, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

void Loho_StepOntoLadderFacingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWait(4, -16, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

void Loho_TurnAndClimbDownLadder(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

void Loho_SetUpTown(void) {
  gFieldWork->transition = 256;
  Actor_SetSpriteFlags(Actor_GetById(20), 0);
}

void Loho_SetUpRoom2(void) {
  gFieldWork->transition = 521;
  Actor_SetSpriteFlags(Actor_GetById(9), 0);
}

void Actor_SetWideSprite(s32 id, s32 wide);

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is; the helper, our name, Madra and its houses use too.
static __inline__ void Loho_PlaceFacing(s32 id, s32 x, s32 z, u16 facing) {
  Actor_SetPositionAndFacing(id, x, z, facing);
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Loho1) {
    if (!Event_GetFlag(FLAG_AREA_TEMP_00))
      Actor_SetWideSprite(64, 0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_01))
      Actor_SetWideSprite(65, 1);
    if (!Event_GetFlag(FLAG_AREA_TEMP_02))
      Actor_SetWideSprite(66, 1);
    Loho_SetUpTown();
  } else {
    Loho_SetUpRoom2();
  }
  if (Event_GetFlag(0xa5f)) {
    Actor_SetPosition(20, 0, 0);
    Actor_SetPositionAndFacing(14, px(248), px(480), FACING_EAST);
    if (!Event_GetFlag(0x90a)) {
      Actor_SetPosition(17, 0, 0);
      Actor_SetPosition(16, 0, 0);
      Actor_SetPosition(18, 0, 0);
    } else {
      Loho_PlaceFacing(16, px(459), px(254), FACING_NORTHEAST);
      Loho_PlaceFacing(18, px(442), px(203), FACING_WEST);
      Loho_PlaceFacing(17, px(490), px(188), FACING_SOUTHEAST);
      Actor_GetById(17)->timer = Actor_GetById(17)->x >> 16;
      Actor_GetById(17)->unk66 = Actor_GetById(17)->z >> 16;
      Actor_SetScript(17, 2);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Loho1 && Event_GetFlag(0xa5f))
    Loho_DrawBrokenWall();
  return 0;
}

// clang-format off
u32 gLoho_FirstMinerGoesToWall[] = {
    39, 2, 48, 0x21b0000, 0x1470000, 1,
    48, 0x2470000, 0x1470000, 1, 48, 0x2470000,
    0x12c0000, 1, 3, 0, 150, 22,
    26, 1, 2, 0x2460000, 0, 0x10a0000,
    4, 0x2460000, 0, 0xf80000, 1, 48,
    0x23a0000, 0xf80000, 1, 48, 0x23a0000, 0x1160000,
    1, 39, 1, 22, 3, 0x6000,
    17,
};

s32 gLoho_SecondMinerStepsUp[] = {
    SC_WAIT(50),
    SC_ARG(2),
    SC_MOVE_BY(px(4), 0, px(-40)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gLoho_ThirdMinerStepsUp[] = {
    SC_WAIT(70),
    SC_ARG(2),
    SC_MOVE_BY(0, 0, px(-40)),
    SC_SYNC,
    SC_MOVE_XZ(px(512), px(303)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gLoho_FirstMinerWalksBack[] = {
    39, 2, 4, 0x23a0000, 0, 0xf80000,
    1, 4, 0x2470000, 0, 0xf80000, 1,
    4, 0x2460000, 0, 0x10a0000, 1, 3,
    0, 20, 17,
};

u32 gLoho_FirstMinerFetchesCannon[] = {
    SC_POSITION(px(583), 0, px(327)),
    SC_SET(SW_MOTION, 0),
    SC_POSITION(px(583), 0, px(300)),
    SC_MOVE_XZ(px(583), px(327)),
    SC_SYNC,
    SC_MOVE_XZ(px(554), px(332)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gLoho_FirstMinerReturns[] = {
    SC_ARG(2),
    SC_MOVE_XZ(px(472), px(331)),
    SC_SYNC,
    SC_MOVE_XZ(px(472), px(361)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gLoho_ForemanComes[] = {
    SC_MOVE_BY(0, px(2), 0),
    SC_SYNC,
    SC_END,
    SC_ARG(5),
    SC_WAIT(1),
    SC_END,
    SC_WAIT(5),
    SC_ARG(5),
    SC_SET(SW_SCALE_X, -0x10000),
    SC_END,
};

u32 gLoho_VillagerPaces[] = {
    SC_LABEL(1),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 gLoho_VillagerReturns[] = {
    SC_ARG(2),
    SC_MOVE(px(128), 0, px(120)),
    SC_SYNC,
    SC_ARG(1),
    SC_LABEL(1),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 gLoho_VillagerMovesWest[] = {                                                               
      SC_SET(SW_SPEED, 0xe666),                                                
      SC_ARG(2),                                                               
      SC_MOVE(px(128), 0, px(144)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_END,                                                                  
  };

u32 gLoho_VillagerMovesNorth[] = {                                                               
      SC_SET(SW_SPEED, 0xe666),                                                
      SC_ARG(2),                                                               
      SC_MOVE(px(159), 0, px(128)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gLoho_Regions[] = {
    0x1c01c4, 0x1cc02e4, 0x2ec0024, 0x6ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(Loho1),
    SCENE_EXIT_UNLESS(1, Loho2, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Loho2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Loho2, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Loho2, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Loho2, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Loho2, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Loho2, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, Loho2, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, WorldMap, 59, EXIT_FLAG_NONE),
    SCENE_EXITS(Loho2),
    SCENE_EXIT_UNLESS(1, Loho1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Loho1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Loho1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Loho1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Loho1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Loho1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Loho2, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Loho2, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, Loho2, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Loho2, 9, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gLoho_Placements[] = {
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 304, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 392, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 264, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(154, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 320, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 416, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 568, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 544, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(154, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 544, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 336, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(154, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 336, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(246, 0x7c, MOVEMENT_TYPE_STAND, 232, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(421, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 360, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(329, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLoho_Room2Placements[] = {
    ACTOR_PLACEMENT(130, CONDITION_ALWAYS, (s32)gLoho_VillagerPaces, 128, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gLoho_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gLoho_Room1Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Loho_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 6, CONDITION_ALWAYS, EVENT(Loho_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 8, CONDITION_ALWAYS, EVENT(Loho_StepOntoLadderFacingWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 9, CONDITION_ALWAYS, EVENT(Loho_StepOntoLadderFacingEast)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, 0x1a5f, MSGNO(MsgLoho29)},
    {TRIGGER_TALK, 9, 0x1a5f, MSGNO(MsgLoho30)},
    {TRIGGER_TALK, 10, 0x1a5f, MSGNO(MsgLoho31)},
    {TRIGGER_TALK, 11, 0x1a5f, EVENT(Loho_TalkAboutBreachedWall)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x1a5f, MSGNO(MsgLoho33)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x1a5f, MSGNO(MsgLoho34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x1a5f, MSGNO(MsgLoho35)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x1a5f, EVENT(Loho_MindReadMiner)},
    {TRIGGER_TALK, 12, 0x1a5f, MSGNO(MsgLoho37)},
    {TRIGGER_TALK, 13, 0x1a5f, MSGNO(MsgLoho38)},
    {TRIGGER_TALK, 14, 0x1a5f, MSGNO(MsgLoho39)},
    {TRIGGER_TALK, 15, 0x1a5f, MSGNO(MsgLoho40)},
    {TRIGGER_TALK, 16, 0x1a5f, MSGNO(MsgLoho41)},
    {TRIGGER_TALK, 18, 0x1a5f, MSGNO(MsgLoho42)},
    {TRIGGER_TALK, 17, 0x1a5f, MSGNO(MsgLoho43)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x1a5f, MSGNO(MsgLoho44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x1a5f, MSGNO(MsgLoho45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x1a5f, MSGNO(MsgLoho46)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x1a5f, MSGNO(MsgLoho47)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x1a5f, MSGNO(MsgLoho48)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x1a5f, MSGNO(MsgLoho49)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x1a5f, MSGNO(MsgLoho50)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLoho01)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLoho02)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLoho03)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLoho04)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLoho09)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLoho10)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLoho11)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLoho12)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLoho21)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgLoho22)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Loho_AskWantCannon)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Loho_ExamineCannon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLoho05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLoho06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLoho07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLoho08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLoho13)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLoho14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLoho15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLoho16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLoho24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLoho25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLoho26)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLoho58))},
    {USE_ITEM_ON_ACTOR(ITEM_MAGMA_BALL), 20, CONDITION_ALWAYS, EVENT(Loho_FireCannon)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Loho_MeetLull)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 101, 0x300, EVENT(Loho_ScoopFirstTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 102, 0x301, EVENT(Loho_ScoopSecondTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 103, 0x302, EVENT(Loho_ScoopThirdTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 22, 0x201, EVENT(Loho_LiftRock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLoho_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {STEP_TRIGGER_FACING(FACING_NORTH), 20, 0x320, EVENT(Loho_VillagerStepsAsideWest)},
    {STEP_TRIGGER_FACING(FACING_WEST), 22, 0x320, EVENT(Loho_VillagerStepsAsideNorth)},
    {TRIGGER_STEP, 21, 0x1320, EVENT(Loho_VillagerStepsBack)},
    {TRIGGER_STEP, 23, 0x1320, EVENT(Loho_VillagerStepsBack)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Loho_TalkAboutCommerce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa5f, MSGNO(MsgLoho18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x1a5f, MSGNO(MsgLoho52)},
    {TRIGGER_TALK, 9, 0x1320, EVENT(Loho_TalkToInnkeeper)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Loho_TalkAboutCannon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa5f, MSGNO(MsgLoho20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x1a5f, MSGNO(MsgLoho54)},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(Loho_TalkToInnkeeper)},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(Loho_ReadInnSign)},
    {TRIGGER_TABLE_END},
};

s16 gLoho_DoorOpening[] = {
    64, 50, 2, 2, 2, 64, 52, 2,
    2, 2, 64, 54, 2, 2, 2, -1,
};
// clang-format on

struct LohoPicture gLoho_HouseDoor = {gLoho_DoorOpening, 79, 15};
