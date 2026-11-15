/// @file
/// Loho, the miners' village whose wall the party blows open with the
/// cannon, which the miners then give it; its ladder down to the mine, its
/// houses' doors and the treasure Scoop digs up.

#include "shared/tables.h"
#include "sprite_ids.h"
#include "flags.h"
#include "system/io_reg.h"
#include "item_ids.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "system/dma.h"
#include "field/field_sequence_event.h"
#include "window/item_icon.h"
#include "scene/scene.h"
#include "scene_ids.h"
#include "graphics/resource.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"
#include "djinn_ids.h"

/// Loho, by the rows of gLoho_TownPlacements. Each digger is named for what he
/// says before the cannon breaches the wall.
enum LohoActorId {
  /// "Welcome to Loho! We've been excavating all sorts of ores..."
  ACTOR_MINER_WELCOMING_TO_LOHO = ACTOR_FIRST_PLACED,
  /// "We have a lot of important excavations going on..."
  ACTOR_MINER_ON_THE_EXCAVATIONS,
  /// "Some of the miners found an ancient cannon when they were digging..."
  ACTOR_MINER_WHO_FOUND_THE_CANNON,
  /// "That wall seems indestructible!"
  ACTOR_DIGGER_AT_THE_WALL,
  /// "Once we can get through the wall, there will be enough work for
  /// everyone in town!"
  ACTOR_MINER_AWAITING_WORK,
  /// "If you want to be a miner, you have to know the earth..."
  ACTOR_MINER_WHO_KNOWS_THE_EARTH,
  /// "This is the spot. A good hole here..."
  ACTOR_MINER_AT_HIS_SPOT,
  /// "I haven't found anything worth the trouble of digging up."
  ACTOR_DIGGER_FINDING_NOTHING,
  /// "We brought the cannon up here to break down the old wall..."
  ACTOR_MINER_BY_THE_CANNON,
  /// "If you're only here for the cannon, you can forget about it."
  ACTOR_MAN_KEEPING_THE_CANNON,
  /// "You can put metal balls in the barrel, but what makes them come out
  /// again?"
  ACTOR_MINER_PUZZLED_BY_THE_CANNON,
  ACTOR_LULL,
  ACTOR_CANNON,
  ACTOR_MAGMA_BALL, ///< shown as ITEM_MAGMA_BALL as it is loaded
  ACTOR_BOULDER     ///< raised by Lift
};

/// Loho's houses, by the rows of gLoho_InteriorsPlacements.
enum LohoInteriorsActorId {
  ACTOR_VILLAGER_ON_COMMERCE = ACTOR_FIRST_PLACED, ///< Loho_TalkAboutCommerce
  ACTOR_INNKEEPER,
  ACTOR_BEARDED_MAN_ON_THE_CANNON ///< Loho_TalkAboutCannon
};

/// The Golem Core under the rock, dug up.
#define FLAG_GOLEM_CORE_DUG 0xff7

extern s32 gLoho_VillagerMovesNorth[];
extern s32 gLoho_VillagerMovesWest[];

extern s32 gLoho_VillagerReturns[];

void Loho_ClimbDownLadder(void);
extern u32 gLoho_Regions[];
extern struct ActorPlacement gLoho_TownPlacements[];
extern struct ActorPlacement gLoho_InteriorsPlacements[];
extern s32 gLoho_FirstMinerGoesToWall[];
extern s32 gLoho_SecondMinerStepsUp[];

extern s32 gLoho_ThirdMinerStepsUp[];

extern s32 gLoho_FirstMinerWalksBack[];
extern s32 gLoho_FirstMinerFetchesCannon[];
extern s32 gLoho_FirstMinerReturns[];
extern s32 gLoho_ForemanComes[];
extern struct Trigger gLoho_Triggers[];
extern struct Trigger gLoho_TownTriggers[];
extern struct Trigger gLoho_InteriorsTriggers[];
extern struct LohoPicture gLoho_HouseDoor;
extern s32 gLoho_VillagerPaces[];

extern s16 gLoho_DoorOpening[];

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
void Loho_MeetLull(void) {
  Djinn_JoinParty(19, DJINNI_LULL / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_LULL));
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const u32 *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_LOHO_INTERIORS)
    return gLoho_Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_LOHO_INTERIORS)
    return gLoho_InteriorsPlacements;
  return gLoho_TownPlacements;
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
  s32 msg = (s32)MsgLohoDidYouWantThisCannon;

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
  // "It looks like it's empty."
  FieldMessage_Show((s32)MsgLohoItLooksLikeItsEmpty, 1);
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

  t0 = Actor_GetById(ACTOR_FELIX);
  Event_Start();
  Event_ResolvePendingActions(0);
  p = (struct LohoSpark *)t0;
  goto start;
approach:
  if (v_r1 == 0)
    goto turned;
  if (i == 0)
    goto fromEast;
  Actor_WalkToAndWait(ACTOR_FELIX, 471, 346);
  goto turned;
fromEast:
  Actor_WalkToAndWait(ACTOR_FELIX, 506, 346);
turned:
  if (i == 0)
    goto fromNorth;
  goto fromSouth;
fromNorth:
  Actor_WalkToAndWait(ACTOR_FELIX, 506, 373);
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
  Actor_MoveToAndWait(ACTOR_FELIX, 487, 377);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  Event_Wait(30);
  Event_PlaySound(SE_SPIN);
  FieldActor_StartRepeatedMotion(ACTOR_MINER_BY_THE_CANNON, 2);
  FieldActor_StartRepeatedMotion(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 2);
  Actor_RunRepeatedMotion(ACTOR_MAN_KEEPING_THE_CANNON, 2);
  Actor_FaceActor(ACTOR_MINER_BY_THE_CANNON, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_MAN_KEEPING_THE_CANNON, ACTOR_FELIX, 0);
  Actor_FaceDirection(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_SOUTH_SOUTHWEST,
                      0);
  Event_Wait(25);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_WalkByAndWait(ACTOR_FELIX, 24, 12);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  Event_Wait(30);
  Event_PlaySound(SE_CLATTER);
  FieldActor_ShowItemIconById(ACTOR_MAGMA_BALL, ITEM_MAGMA_BALL);
  Actor_SetPosition(ACTOR_MAGMA_BALL, 488, 330);
  Actor_SetDestinationOffset(ACTOR_CANNON, 0, 8);
  Actor_SetSpeed(ACTOR_MAGMA_BALL, 3);
  MapGrid_SetShake(2, 2, 1);
  Actor_SetDestination(ACTOR_MAGMA_BALL, 488, 300);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 308, 1);
  Actor_WaitForMoveById(ACTOR_MAGMA_BALL);
  Actor_SetPosition(ACTOR_MAGMA_BALL, 0, 0);
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
  Dma_Set((const void *)(s32)Field_ExplosionPalette, (void *)&OBJ_PLTT_COLOR(15, 0),
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
  t47 = (u16)(((Random_Below(96)) + 72) & 0x1ff);
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
  p->age = -(Random_Below(7));
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
  Actor_SetPositionAndFacing(ACTOR_MINER_AT_HIS_SPOT, px(248), px(480),
                             FACING_EAST);
  Loho_DrawBrokenWall();
  MapGrid_SetShake(0, 0, 1);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(180);
  ScreenFade_FromWhite(120);
  Event_SetMessageId((s32)MsgLohoIsEveryoneAllRight);
  Event_Wait(120);
  Actor_RunRepeatedMotion(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 2);
  Actor_FaceDirectionAndWait(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_WEST);
  // "Is everyone all right?"
  Event_ShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  Actor_FaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_SOUTHEAST, 0);
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, 3);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, 3);
  Event_Wait(30);
  FieldActor_ShowEmote(ACTOR_MAN_KEEPING_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_WINCE, 35);
  // "Did you hear that? That sound was incredible!"
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  FieldActor_ShowEmote(ACTOR_MINER_BY_THE_CANNON, EMOTE_IN_FRONT | EMOTE_SCOWL,
                       35);
  Actor_WalkByAndWait(ACTOR_MINER_BY_THE_CANNON, -12, 24);
  Actor_RunRepeatedMotion(ACTOR_MINER_BY_THE_CANNON, 2);
  // "What do you think you're doing, making all that noise?"
  Event_ShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_FaceDirectionAndWait(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                             FACING_NORTH_NORTHWEST);
  Event_Wait(30);
  FieldActor_ShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 35);
  // "Hey, look at that!"
  Event_ShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  FieldActor_ShowEmote(ACTOR_MINER_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_MAN_KEEPING_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
  Actor_FaceActor(ACTOR_MINER_BY_THE_CANNON, ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                  0);
  Actor_FaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_SOUTHEAST, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_WEST, 0);
  Event_Wait(60);
  FieldActor_ShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  FieldActor_ShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_WINCE, 35);
  // "What are you looking at? It wasn't me! It was the wall! Look at the..."
  Event_ShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  Actor_FaceEachOther(ACTOR_MINER_BY_THE_CANNON, ACTOR_MAN_KEEPING_THE_CANNON,
                      0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_MINER_BY_THE_CANNON, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_NORTH_NORTHWEST,
                      0);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 326, 1);
  Event_Wait(60);
  FieldActor_ShowEmote(ACTOR_MINER_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_MAN_KEEPING_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  FieldActor_ShowEmote(ACTOR_MAN_KEEPING_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_WINCE, 35);
  // "The wall... It's gone!"
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, 3);
  // "Well, it's mostly gone, yeah... You can get through to the other..."
  Event_ShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
  Actor_FaceActor(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ACTOR_FELIX, 0);
  FieldActor_ShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 35);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 366, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_SOUTH_SOUTHWEST,
                      0);
  // "How did you manage to break down the wall?"
  Event_ShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  Actor_SetSpeed(ACTOR_MAN_KEEPING_THE_CANNON, 0.8);
  Actor_WalkBy(ACTOR_MAN_KEEPING_THE_CANNON, 0, 16);
  Actor_WaitForMoveById(ACTOR_MAN_KEEPING_THE_CANNON);
  Actor_FaceDirectionAndWait(ACTOR_MAN_KEEPING_THE_CANNON, FACING_SOUTHEAST);
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, 4);
  // "Ah, never mind... What's important here is that we can dig on the..."
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, 3);
  // "Yeah, who cares how it happened? We can dig again!"
  Event_ShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 3);
  Actor_FaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_SOUTH_SOUTHEAST, 0);
  // "Well, you did what we asked, so the cannon's all yours!"
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
  Event_Wait(60);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTH);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MINER_BY_THE_CANNON, 2);
  // "Hmmm... It's gonna be tough carrying that thing around..."
  Event_ShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
  FieldActor_ShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldActor_ShowEmote(ACTOR_MAN_KEEPING_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldActor_ShowEmote(ACTOR_MINER_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldActor_ShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 35);
  // "Those guys are new in town, aren't they? Do you think they came from..."
  Event_ShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, 3);
  Event_Wait(40);
  FieldActor_ShowEmote(ACTOR_MAN_KEEPING_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 35);
  // "I don't know, but I can tell you they came by boat..."
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, 3);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(542, CAMERA_KEEP, 256, 1);
  zero = 0;
  t147 = Actor_GetById(ACTOR_MINER_BY_THE_CANNON);
  t147->motionFlags = zero;
  Actor_SetSpeed(ACTOR_MINER_BY_THE_CANNON, 1.5);
  Actor_SetScript(ACTOR_MINER_BY_THE_CANNON, gLoho_FirstMinerGoesToWall);
  Actor_SetSpeed(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0.5);
  Actor_SetScript(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
                  (s32)gLoho_SecondMinerStepsUp);
  Actor_SetSpeed(ACTOR_MAN_KEEPING_THE_CANNON, 0.4);
  Actor_SetScript(ACTOR_MAN_KEEPING_THE_CANNON, (s32)gLoho_ThirdMinerStepsUp);
  Actor_WaitForScriptEndById(ACTOR_MINER_BY_THE_CANNON);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTHWEST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_WEST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_NORTHWEST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_NORTHEAST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_EAST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTH_SOUTHWEST);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, 3);
  Event_Wait(60);
  // "Yeah, I saw one anchored off the beach!"
  Event_ShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
  Actor_WaitForScriptEndById(ACTOR_MAN_KEEPING_THE_CANNON);
  Actor_WaitForScriptEndById(ACTOR_MINER_PUZZLED_BY_THE_CANNON);
  Actor_FaceEachOther(ACTOR_MAN_KEEPING_THE_CANNON,
                      ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  Event_Wait(20);
  goto askCannon;
askCannon:
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 3);
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, 3);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_MAN_KEEPING_THE_CANNON, ACTOR_FELIX, 0);
  Actor_SetSpeed(ACTOR_MINER_BY_THE_CANNON, 1);
  Actor_SetScript(ACTOR_MINER_BY_THE_CANNON, gLoho_FirstMinerWalksBack);
  FieldCamera_SetSpeed(1.8);
  FieldCamera_MoveTo(542, CAMERA_KEEP, 350, 1);
  FieldCamera_WaitForArrival();
  // "That boat's yours, isn't it?"
  Event_OpenMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0))
    goto answeredNo;
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 3);
  Event_ShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  t189 = (s32)Ram_HeapSlots->fieldWork;
  t190 = FIELD_AT(t189, u16, 452);
  FIELD_AT(t189, u16, 452) = (t190 + 1);
  goto crewLeaves;
fromSouth:
  Actor_WalkToAndWait(ACTOR_FELIX, 471, 373);
  goto inPlace;
answeredNo:
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 4);
  t193 = (s32)Ram_HeapSlots->fieldWork;
  t194 = FIELD_AT(t193, u16, 452);
  FIELD_AT(t193, u16, 452) = (t194 + 1);
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 3);
  Event_ShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
crewLeaves:
  Actor_WalkBy(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0, 24);
  Actor_WalkByAndWait(ACTOR_MAN_KEEPING_THE_CANNON, 0, 24);
  Actor_FaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_NORTH_NORTHEAST,
                      0);
  Actor_WaitForScriptEndById(ACTOR_MINER_BY_THE_CANNON);
  Actor_SetScript(ACTOR_MINER_BY_THE_CANNON, gLoho_FirstMinerFetchesCannon);
  Actor_WaitForScriptEndById(ACTOR_MINER_BY_THE_CANNON);
  Actor_FaceDirection(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 3);
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, 3);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, 3);
  Event_Wait(60);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_MINER_BY_THE_CANNON);
  Actor_WalkTo(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 504, 370);
  Actor_WalkTo(ACTOR_MAN_KEEPING_THE_CANNON, 504, 352);
  Actor_SetScript(ACTOR_MINER_BY_THE_CANNON, gLoho_FirstMinerReturns);
  Actor_WaitForMoveById(ACTOR_MAN_KEEPING_THE_CANNON);
  Actor_WaitForMoveById(ACTOR_MINER_PUZZLED_BY_THE_CANNON);
  Actor_WaitForScriptEndById(ACTOR_MINER_BY_THE_CANNON);
  Actor_FaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_EAST);
  Actor_SetSpeed(ACTOR_MINER_BY_THE_CANNON, 0.3);
  Actor_SetSpeed(ACTOR_MAN_KEEPING_THE_CANNON, 0.3);
  Actor_SetSpeed(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0.3);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_CANNON, 0.1);
  t226 = Actor_GetById(ACTOR_CANNON);
  t226->motionFlags = 0;
  Actor_SetScript(ACTOR_CANNON, (s32)gLoho_ForemanComes);
  Actor_WaitForScriptEndById(ACTOR_CANNON);
  Actor_SetSpeed(ACTOR_CANNON, 0.3);
  FieldCamera_SetSpeed(0.3);
  FieldCamera_MoveTo(460, CAMERA_KEEP, 350, 1);
  Event_Wait(30);
  t233 = Actor_GetById(ACTOR_MINER_BY_THE_CANNON);
  t233->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_MINER_PUZZLED_BY_THE_CANNON, -40, -20);
  Actor_WalkBy(ACTOR_MAN_KEEPING_THE_CANNON, -40, -20);
  Actor_WalkBy(ACTOR_MINER_BY_THE_CANNON, -40, -20);
  Actor_WalkByAndWait(ACTOR_CANNON, -40, -20);
  Actor_WalkBy(ACTOR_MINER_PUZZLED_BY_THE_CANNON, -138, 0);
  Actor_WalkBy(ACTOR_MAN_KEEPING_THE_CANNON, -138, 0);
  Actor_WalkBy(ACTOR_MINER_BY_THE_CANNON, -138, 0);
  Actor_WalkByAndWait(ACTOR_CANNON, -138, 0);
  Actor_SetPosition(ACTOR_MINER_BY_THE_CANNON, 0, 0);
  Actor_SetPosition(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0, 0);
  Actor_SetPosition(ACTOR_MAN_KEEPING_THE_CANNON, 0, 0);
  Actor_SetPosition(ACTOR_CANNON, 0, 0);
  Event_End();
  return;
}

/// "Now that the wall's been breached, we can get on with the mining! Good
/// work!"
void Loho_TalkAboutBreachedWall(void) {
  if (Event_GetFlag(FLAG_SHIP_CARRIES_CANNON))
    Event_SetMessageId((s32)MsgLohoNowThatWallsBeenBreached);
  else
    Event_SetMessageId((s32)MsgLohoISawSomeOfVillage);
  // "I saw some of the village men carrying the cannon out of town..."
  Event_ShowMessage(ACTOR_DIGGER_AT_THE_WALL, 0);
}

void Loho_MindReadMiner(void) {
  if (Event_GetFlag(FLAG_SHIP_CARRIES_CANNON))
    Event_SetMessageId((s32)MsgLohoWeDontHaveAnything);
  else
    Event_SetMessageId((s32)MsgLohoIDontKnowWhereThey);
  // "I don't know where they were taking it, but I'm glad the cannon's..."
  Event_ShowMessage(ACTOR_DIGGER_AT_THE_WALL, 0);
}

/// The Scoop spots: each digs up its treasure actor and sets its flag.
void Loho_ScoopFirstTreasure(s32 id) {
  Event_DigUpTreasure(id, FIELD_TREASURE_ACTOR, 0x300);
}

#define DEFINE_SCOOP_TREASURE(name, n1, n2)                                    \
  void name(s32 id) { Event_DigUpTreasure(id, n1, 0x300 + n2); }

DEFINE_SCOOP_TREASURE(Loho_ScoopSecondTreasure, 65, 1)

DEFINE_SCOOP_TREASURE(Loho_ScoopThirdTreasure, 66, 2)

/// Lift raises the rock (flag 0x201), and the Golem Core under it shows until
/// it is dug up (FLAG_GOLEM_CORE_DUG).
void Loho_LiftRock(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_01);
  Actor_GetById(ACTOR_BOULDER)->collisionFlags &= ~ACTOR_SOLID;
  if (Event_GetFlag(FLAG_GOLEM_CORE_DUG) == 0)
    Actor_SetPosition(FIELD_TREASURE_ACTOR + 2, 520, 136);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_LOHO)
    return gLoho_TownTriggers;
  if (gMainRoutineWork.next == SCENE_LOHO_INTERIORS)
    return gLoho_InteriorsTriggers;
  return gLoho_Triggers;
}

/// "People staying at the inn can check in at the counter."
void Loho_ReadInnSign(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH)
    // "Felix checked the sign It reads "People staying at the inn can..."
    FieldMessage_Show((s32)MsgLohoCheckedSignItReadsPeople, 1);
}

/// Loho's shops, as Shop_Run numbers them.
enum { SHOP_LOHO = 30 };

/// Loho's inn, as Inn_CheckIn numbers the inns.
enum { LOHO_INN = 13 };

void Loho_TalkToInnkeeper(void) {
  // XXX: the lines take actor 8's portrait, not the innkeeper's (9); a render
  // of the inn's counter would show whose face they carry.
  Inn_CheckIn(LOHO_INN, ACTOR_VILLAGER_ON_COMMERCE);
}

/// "Commerce really took off when we started this mining colony in Loho."
void Loho_TalkAboutCommerce(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId);
  if (Event_GetFlag(FLAG_AREA_TEMP_20)) {
    Shop_Run(SHOP_LOHO, 8);
  } else {
    if (Event_GetFlag(0xa5f))
      Event_SetMessageId((s32)MsgLohoIHearYouTookOur);
    else
      Event_SetMessageId((s32)MsgLohoCommerceReallyTookOffWhen);
    // "Commerce really took off when we started this mining colony in Loho...."
    Event_ShowMessage(ACTOR_VILLAGER_ON_COMMERCE, 0);
  }
}

/// "If you're only here for the cannon, you can forget about it."
void Loho_TalkAboutCannon(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    if (Event_GetFlag(0xa5f))
      Event_SetMessageId((s32)MsgLohoYouveAcquiredSplendidCannonJust);
    else
      Event_SetMessageId((s32)MsgLohoIfYoureOnlyHere);
    // "If you're only here for the cannon, you can forget about it."
    Event_ShowMessage(actor, 0);
  }
}

void Loho_VillagerStepsAsideNorth(void) {
  Actor_SetScript(ACTOR_VILLAGER_ON_COMMERCE, gLoho_VillagerMovesNorth);
  Event_SetFlag(FLAG_AREA_TEMP_20);
}

void Loho_VillagerStepsAsideWest(void) {
  Actor_SetScript(ACTOR_VILLAGER_ON_COMMERCE, gLoho_VillagerMovesWest);
  Event_SetFlag(FLAG_AREA_TEMP_20);
}

void Loho_VillagerStepsBack(void) {
  Event_ClearFlag(FLAG_AREA_TEMP_20);
  Actor_SetScript(ACTOR_VILLAGER_ON_COMMERCE, gLoho_VillagerReturns);
}

static __inline__ void Loho_AnimateCells(const void *data, s32 a, s32 b) {
  MapGrid_AnimateCells(data, a, b);
}

void Loho_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 *trigger;
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  i = ACTOR_FIRST_PLACED;
loop:
  {
    struct Actor *object = Actor_GetById(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i < FIELD_TREASURE_ACTOR)
    goto loop;
  trigger = &work->stepTrigger;
  Event_PlaySound(SE_DOOR_OPEN);
  Loho_AnimateCells(gLoho_HouseDoor.data, gLoho_HouseDoor.a, gLoho_HouseDoor.b);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -4);
  Event_PlaySound(SE_MAP_EXIT);
  Event_Wait(6);
  Import_EventSetExitRequest(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Loho_EnterDoor(void) {
  struct FieldWork *work = gFieldWork;

  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Event_PlaySound(SE_MAP_EXIT);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -4);
  Import_EventSetExitRequest(work->stepTrigger);
}

void Loho_ClimbDownLadder(void) {
  s32 value = gFieldWork->stepTrigger;

  if (value == 2)
    Actor_SetPosition(ACTOR_MINER_WELCOMING_TO_LOHO, 144, 224);
  else
    Actor_SetPosition(ACTOR_MINER_WELCOMING_TO_LOHO, 224, 224);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 4);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX), 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 16);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB);
  Event_Wait(14);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(value);
  Event_End();
}

void Loho_StepOntoLadderFacingEast(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

void Loho_StepOntoLadderFacingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

void Loho_TurnAndClimbDownLadder(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

void Loho_SetUpTown(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
}

void Loho_SetUpInteriors(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_INNKEEPER), 0);
}

void Actor_SetWideSprite(s32 id, s32 wide);

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is; the helper, our name, Madra and its houses use too.
static __inline__ void Loho_PlaceFacing(s32 id, s32 x, s32 z, u16 facing) {
  Actor_SetPositionAndFacing(id, x, z, facing);
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.next == SCENE_LOHO) {
    if (!Event_GetFlag(FLAG_AREA_TEMP_00))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR, 0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_01))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR + 1, 1);
    if (!Event_GetFlag(FLAG_AREA_TEMP_02))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR + 2, 1);
    Loho_SetUpTown();
  } else {
    Loho_SetUpInteriors();
  }
  if (Event_GetFlag(0xa5f)) {
    Actor_SetPosition(ACTOR_CANNON, 0, 0);
    Actor_SetPositionAndFacing(ACTOR_MINER_AT_HIS_SPOT, px(248), px(480),
                               FACING_EAST);
    if (!Event_GetFlag(FLAG_SHIP_CARRIES_CANNON)) {
      Actor_SetPosition(ACTOR_MAN_KEEPING_THE_CANNON, 0, 0);
      Actor_SetPosition(ACTOR_MINER_BY_THE_CANNON, 0, 0);
      Actor_SetPosition(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0, 0);
    } else {
      Loho_PlaceFacing(16, px(459), px(254), FACING_NORTHEAST);
      Loho_PlaceFacing(18, px(442), px(203), FACING_WEST);
      Loho_PlaceFacing(17, px(490), px(188), FACING_SOUTHEAST);
      Actor_GetById(ACTOR_MAN_KEEPING_THE_CANNON)->timer =
          Actor_GetById(ACTOR_MAN_KEEPING_THE_CANNON)->x.fixed >> 16;
      Actor_GetById(ACTOR_MAN_KEEPING_THE_CANNON)->userValue =
          Actor_GetById(ACTOR_MAN_KEEPING_THE_CANNON)->z.fixed >> 16;
      Actor_SetScript(ACTOR_MAN_KEEPING_THE_CANNON, 2);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_LOHO && Event_GetFlag(0xa5f))
    Loho_DrawBrokenWall();
  return 0;
}

// clang-format off
s32 gLoho_FirstMinerGoesToWall[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(539, 327),
    SC_SYNC,
    SC_MOVE_XZ(583, 327),
    SC_SYNC,
    SC_MOVE_XZ(583, 300),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_WAIT(150),
    SC_SET(SW_MOTION, 1),
    SC_POSITION(582, 0, 266),
    SC_MOVE(582, 0, 248),
    SC_SYNC,
    SC_MOVE_XZ(570, 248),
    SC_SYNC,
    SC_MOVE_XZ(570, 278),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_END,
};

s32 gLoho_SecondMinerStepsUp[] = {
    SC_WAIT(50),
    SC_ANIMATION(2),
    SC_MOVE_BY(4, 0, -40),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLoho_ThirdMinerStepsUp[] = {
    SC_WAIT(70),
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, -40),
    SC_SYNC,
    SC_MOVE_XZ(512, 303),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLoho_FirstMinerWalksBack[] = {
    SC_ANIMATION(2),
    SC_MOVE(570, 0, 248),
    SC_SYNC,
    SC_MOVE(583, 0, 248),
    SC_SYNC,
    SC_MOVE(582, 0, 266),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_WAIT(20),
    SC_END,
};

s32 gLoho_FirstMinerFetchesCannon[] = {
    SC_POSITION(583, 0, 327),
    SC_SET(SW_MOTION, 0),
    SC_POSITION(583, 0, 300),
    SC_MOVE_XZ(583, 327),
    SC_SYNC,
    SC_MOVE_XZ(554, 332),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLoho_FirstMinerReturns[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(472, 331),
    SC_SYNC,
    SC_MOVE_XZ(472, 361),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLoho_ForemanComes[] = {
    SC_MOVE_BY(0, 2, 0),
    SC_SYNC,
    SC_END,
    SC_ANIMATION(5),
    SC_WAIT(1),
    SC_END,
    SC_WAIT(5),
    SC_ANIMATION(5),
    SC_SET(SW_SCALE_X, -0x10000),
    SC_END,
};

s32 gLoho_VillagerPaces[] = {
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

s32 gLoho_VillagerReturns[] = {
    SC_ANIMATION(2),
    SC_MOVE(128, 0, 120),
    SC_SYNC,
    SC_ANIMATION(1),
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

s32 gLoho_VillagerMovesWest[] = {                                                               
      SC_SPEED(0.9),                                                
      SC_ANIMATION(2),                                                               
      SC_MOVE(128, 0, 144),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_END,                                                                  
  };

s32 gLoho_VillagerMovesNorth[] = {                                                               
      SC_SPEED(0.9),                                                
      SC_ANIMATION(2),                                                               
      SC_MOVE(159, 0, 128),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
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
    SCENE_EXITS(Loho),
    SCENE_EXIT_UNLESS(1, LohoInteriors, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, LohoInteriors, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, LohoInteriors, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, LohoInteriors, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, LohoInteriors, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, LohoInteriors, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, LohoInteriors, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LohoInteriors, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, WorldMap, 59, EXIT_FLAG_NONE),
    SCENE_EXITS(LohoInteriors),
    SCENE_EXIT_UNLESS(1, Loho, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Loho, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Loho, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Loho, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Loho, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Loho, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, LohoInteriors, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, LohoInteriors, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LohoInteriors, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, LohoInteriors, 9, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gLoho_TownPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 304, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 392, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 264, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(154, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 320, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 416, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 568, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 544, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(154, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 544, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 336, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(154, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 336, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, 0x7c, MOVEMENT_TYPE_STAND, 232, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(421, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 360, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLoho_InteriorsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, gLoho_VillagerPaces, 128, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gLoho_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gLoho_TownTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Loho_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 6, CONDITION_ALWAYS, EVENT(Loho_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), ACTOR_MINER_WELCOMING_TO_LOHO, CONDITION_ALWAYS, EVENT(Loho_StepOntoLadderFacingWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), ACTOR_MINER_ON_THE_EXCAVATIONS, CONDITION_ALWAYS, EVENT(Loho_StepOntoLadderFacingEast)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_MINER_WELCOMING_TO_LOHO, 0x1a5f, MSGNO(MsgLohoWasThatYouThatFired)},
    {TRIGGER_TALK, ACTOR_MINER_ON_THE_EXCAVATIONS, 0x1a5f, MSGNO(MsgLohoThatCannonSureGivesOff)},
    {TRIGGER_TALK, ACTOR_MINER_WHO_FOUND_THE_CANNON, 0x1a5f, MSGNO(MsgLohoWhenWeFoundThatCannon)},
    {TRIGGER_TALK, ACTOR_DIGGER_AT_THE_WALL, 0x1a5f, EVENT(Loho_TalkAboutBreachedWall)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WELCOMING_TO_LOHO, 0x1a5f, MSGNO(MsgLohoThatCannonCanShatterAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_ON_THE_EXCAVATIONS, 0x1a5f, MSGNO(MsgLohoThatCannonWasReallyLoud)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_FOUND_THE_CANNON, 0x1a5f, MSGNO(MsgLohoIDontThinkWeShould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DIGGER_AT_THE_WALL, 0x1a5f, EVENT(Loho_MindReadMiner)},
    {TRIGGER_TALK, ACTOR_MINER_AWAITING_WORK, 0x1a5f, MSGNO(MsgLohoIveGottaGoStakeMyself)},
    {TRIGGER_TALK, ACTOR_MINER_WHO_KNOWS_THE_EARTH, 0x1a5f, MSGNO(MsgLohoYouProbablyOnlyWantCannon)},
    {TRIGGER_TALK, ACTOR_MINER_AT_HIS_SPOT, 0x1a5f, MSGNO(MsgLohoAhSoYouHaveQuest)},
    {TRIGGER_TALK, ACTOR_DIGGER_FINDING_NOTHING, 0x1a5f, MSGNO(MsgLohoIThinkWeveDugUp)},
    {TRIGGER_TALK, ACTOR_MINER_BY_THE_CANNON, 0x1a5f, MSGNO(MsgLohoThanksForOpeningUpWall)},
    {TRIGGER_TALK, ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0x1a5f, MSGNO(MsgLohoIKeepDiggingUpLittle)},
    {TRIGGER_TALK, ACTOR_MAN_KEEPING_THE_CANNON, 0x1a5f, MSGNO(MsgLohoHaHaHaIveGot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AWAITING_WORK, 0x1a5f, MSGNO(MsgLohoIJustNeedToStick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_KNOWS_THE_EARTH, 0x1a5f, MSGNO(MsgLohoTheseGuysMustBeReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AT_HIS_SPOT, 0x1a5f, MSGNO(MsgLohoWhenTheyGetWhereverTheyre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DIGGER_FINDING_NOTHING, 0x1a5f, MSGNO(MsgLohoEvenAtMyAge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_BY_THE_CANNON, 0x1a5f, MSGNO(MsgLohoIOnlyJustStartedDigging)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0x1a5f, MSGNO(MsgLohoIveHitMotherLode)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_THE_CANNON, 0x1a5f, MSGNO(MsgLohoItDoesntMatterHowOld)},
    {TRIGGER_TALK, ACTOR_MINER_WELCOMING_TO_LOHO, CONDITION_ALWAYS, MSGNO(MsgLohoWelcomeToLohoWeveBeen)},
    {TRIGGER_TALK, ACTOR_MINER_ON_THE_EXCAVATIONS, CONDITION_ALWAYS, MSGNO(MsgLohoWeHaveLotOfImportant)},
    {TRIGGER_TALK, ACTOR_MINER_WHO_FOUND_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoSomeOfMinersFoundAncient)},
    {TRIGGER_TALK, ACTOR_DIGGER_AT_THE_WALL, CONDITION_ALWAYS, MSGNO(MsgLohoThatWallSeemsIndestructibleWeve)},
    {TRIGGER_TALK, ACTOR_MINER_AWAITING_WORK, CONDITION_ALWAYS, MSGNO(MsgLohoOnceWeCanGetThrough)},
    {TRIGGER_TALK, ACTOR_MINER_WHO_KNOWS_THE_EARTH, CONDITION_ALWAYS, MSGNO(MsgLohoIfYouWant)},
    {TRIGGER_TALK, ACTOR_MINER_AT_HIS_SPOT, CONDITION_ALWAYS, MSGNO(MsgLohoThisIsSpotGoodHole)},
    {TRIGGER_TALK, ACTOR_DIGGER_FINDING_NOTHING, CONDITION_ALWAYS, MSGNO(MsgLohoIHaventFoundAnythingWorth)},
    {TRIGGER_TALK, ACTOR_MINER_BY_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoWeBroughtCannonUpHere)},
    {TRIGGER_TALK, ACTOR_MINER_PUZZLED_BY_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoYouCanPutMetalBalls)},
    {TRIGGER_TALK, ACTOR_MAN_KEEPING_THE_CANNON, CONDITION_ALWAYS, EVENT(Loho_AskWantCannon)},
    {TRIGGER_TALK, ACTOR_CANNON, CONDITION_ALWAYS, EVENT(Loho_ExamineCannon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WELCOMING_TO_LOHO, CONDITION_ALWAYS, MSGNO(MsgLohoWeOnlySettledHereShort)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_ON_THE_EXCAVATIONS, CONDITION_ALWAYS, MSGNO(MsgLohoEveryoneInLohoSureLoves)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_FOUND_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoAllWeNeedIsWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DIGGER_AT_THE_WALL, CONDITION_ALWAYS, MSGNO(MsgLohoIfWeCantGetThrough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AWAITING_WORK, CONDITION_ALWAYS, MSGNO(MsgLohoIfYouLiveInLoho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_KNOWS_THE_EARTH, CONDITION_ALWAYS, MSGNO(MsgLohoRealExpertsCanFindBest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AT_HIS_SPOT, CONDITION_ALWAYS, MSGNO(MsgLohoOnceYouveBeenMining)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DIGGER_FINDING_NOTHING, CONDITION_ALWAYS, MSGNO(MsgLohoIThinkImStarting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_BY_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoCannonsAreSoHeavyWhy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_PUZZLED_BY_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoWhatIdiotSaidThatWed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoAllRightWeFiguredOut)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLohoThisWellLooksReallyOld))},
    {USE_ITEM_ON_ACTOR(ITEM_MAGMA_BALL), ACTOR_CANNON, CONDITION_ALWAYS, EVENT(Loho_FireCannon)},
    {TRIGGER_TALK, ACTOR_LULL, CONDITION_ALWAYS, EVENT(Loho_MeetLull)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 101, 0x300, EVENT(Loho_ScoopFirstTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 102, 0x301, EVENT(Loho_ScoopSecondTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 103, 0x302, EVENT(Loho_ScoopThirdTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, 0x201, EVENT(Loho_LiftRock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLoho_InteriorsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, ACTOR_VILLAGER_ON_COMMERCE, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, ACTOR_INNKEEPER, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, ACTOR_BEARDED_MAN_ON_THE_CANNON, CONDITION_ALWAYS, 10},
    {STEP_TRIGGER_FACING(FACING_NORTH), 20, 0x320, EVENT(Loho_VillagerStepsAsideWest)},
    {STEP_TRIGGER_FACING(FACING_WEST), 22, 0x320, EVENT(Loho_VillagerStepsAsideNorth)},
    {TRIGGER_STEP, 21, 0x1320, EVENT(Loho_VillagerStepsBack)},
    {TRIGGER_STEP, 23, 0x1320, EVENT(Loho_VillagerStepsBack)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_COMMERCE, CONDITION_ALWAYS, EVENT(Loho_TalkAboutCommerce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_COMMERCE, 0xa5f, MSGNO(MsgLohoWheneverSomeoneStrikesGold)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_COMMERCE, 0x1a5f, MSGNO(MsgLohoNowWeDontHaveSymbol)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, 0x1320, EVENT(Loho_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_BEARDED_MAN_ON_THE_CANNON, CONDITION_ALWAYS, EVENT(Loho_TalkAboutCannon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BEARDED_MAN_ON_THE_CANNON, 0xa5f, MSGNO(MsgLohoIfCannonIsTruly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BEARDED_MAN_ON_THE_CANNON, 0x1a5f, MSGNO(MsgLohoItWorriesMeToNo)},
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
