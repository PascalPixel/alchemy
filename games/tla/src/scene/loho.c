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
  ACTOR_BOULDER     ///< raised by Lift, over the Golem Core
};

/// Loho's houses, by the rows of gLoho_RoomsPlacements.
enum LohoRoomsActorId {
  /// The woman who keeps the shop and the inn (Loho_TalkAboutCommerce).
  ACTOR_VILLAGER_ON_COMMERCE = ACTOR_FIRST_PLACED,
  /// An unseen marker on the inn's counter, so that talking across it reaches
  /// the inn (Loho_TalkToInnkeeper).
  ACTOR_INN_COUNTER,
  ACTOR_BEARDED_MAN_ON_THE_CANNON ///< Loho_TalkAboutCannon
};

/// The Golem Core under the boulder, dug up.
#define FLAG_LOHO_GOLEM_CORE_DUG 0xff7
/// Set once Lift has raised the boulder, this visit.
#define FLAG_LOHO_BOULDER_LIFTED FLAG_SCENE_TEMP_01

extern s32 gLoho_VillagerMovesNorth[];
extern s32 gLoho_VillagerMovesWest[];

extern s32 gLoho_VillagerReturns[];

void Loho_ClimbDownLadder(void);
extern struct SceneRegion gLoho_Regions[];
extern struct ActorPlacement gLoho_TownPlacements[];
extern struct ActorPlacement gLoho_RoomsPlacements[];
extern s32 gLoho_FirstMinerGoesToWall[];
extern s32 gLoho_SecondMinerStepsUp[];

extern s32 gLoho_ThirdMinerStepsUp[];

extern s32 gLoho_FirstMinerWalksBack[];
extern s32 gLoho_FirstMinerFetchesCannon[];
extern s32 gLoho_FirstMinerReturns[];
extern s32 gLoho_CannonRises[];
extern struct Trigger gLoho_Triggers[];
extern struct Trigger gLoho_TownTriggers[];
extern struct Trigger gLoho_RoomsTriggers[];
extern struct MapGridCellAnimation gLoho_HouseDoor;
extern s32 gLoho_VillagerPaces[];

extern s16 gLoho_DoorOpening[];

/// The sparks the cannon's blast throws over the screen.
#define LOHO_SPARK_COUNT 96
/// How long the blast throws sparks, in frames.
#define LOHO_BLAST_FRAMES 120
/// A spark's life in frames: the explosion sheet's eight pictures, four frames
/// each.
#define LOHO_SPARK_FRAMES 32
/// The sprite palette the blast loads the sparks' colours into
/// (Field_ExplosionPalette).
#define LOHO_SPARK_PALETTE 15
/// The sparks' depth slot among the field's sprites.
#define LOHO_SPARK_DEPTH 100

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
  Djinn_JoinParty(ACTOR_LULL, DJINNI_LULL / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_LULL));
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_LOHO_ROOMS)
    return gLoho_Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_LOHO_ROOMS)
    return gLoho_RoomsPlacements;
  return gLoho_TownPlacements;
}

/// Draws the ancient wall north of town as the cannon left it, breached.
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

void FieldCamera_WaitForArrival();

/// The Magma Ball used on the cannon: it is loaded and fired at the ancient
/// wall north of town, which is breached, and the miners give the party the
/// cannon for its ship.
/// "Is everyone all right?"
void Loho_FireCannon(void) {
  s32 west, north, south, z;
  struct Actor *miner;
  struct Actor *felix;
  struct Actor *cannon;
  struct Actor *walkingMiner;
  struct LohoSpark *spark;
  struct LohoSpark *table;
  s32 *timer;
  s32 tiles, entry, i, zero;
  s32 sheet;
  u8 unused[4]; // Unused: gives the stack frame its 4 bytes.

  felix = Actor_GetById(ACTOR_FELIX);
  Event_Start();
  Event_ResolvePendingActions(0);
  goto start;
approach:
  if (north == 0)
    goto turned;
  if (west == 0)
    goto toNorthEast;
  Actor_WalkToAndWait(ACTOR_FELIX, 471, 346);
  goto turned;
toNorthEast:
  Actor_WalkToAndWait(ACTOR_FELIX, 506, 346);
turned:
  if (west == 0)
    goto toEast;
  goto toWest;
toEast:
  Actor_WalkToAndWait(ACTOR_FELIX, 506, 373);
  goto inPlace;
start:
  west = 0;
  north = 0;
  south = 0;
  if (felix->x.fixed >= toFIXED(489))
    goto checkZ;
  west = 1;
checkZ:
  z = felix->z.fixed;
  if (z >= toFIXED(347))
    goto checkSouth;
  north = 1;
  goto placed;
checkSouth:
  if (z <= toFIXED(370))
    goto placed;
  south = 1;
placed:
  if (south == 0)
    goto approach;
inPlace:
  FieldCamera_MoveTo(488, CAMERA_KEEP, 372, 1);
  Event_SetFlag(FLAG_LOHO_WALL_BREACHED);
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
  table = (struct LohoSpark *)Heap_AllocEwram(LOHO_SPARK_COUNT *
                                              sizeof(struct LohoSpark));
  {
    void *buffer = Heap_Alloc(4096);

    sheet = Resource_GetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION);
    Decompress_TaggedLz(sheet, buffer);
    entry = VramCache_FindFreeEntry();
    tiles = VramCache_Load(entry, 1024, buffer);
    Heap_Release(buffer);
  }
  timer = &table->age;
  for (i = 0; i < LOHO_SPARK_COUNT; i++) {
    *timer = -(Random16() << 5 >> 16);
    timer += 4;
  }
  Dma_Set((const void *)(s32)Field_ExplosionPalette,
          (void *)&OBJ_PLTT_COLOR(LOHO_SPARK_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  for (i = 0; i < LOHO_BLAST_FRAMES; i++) {
    if ((i & 31) == 0) {
      Event_PlaySound(SE_CRASH);
    }
    if (i == 30) {
      ScreenFade_ToWhite(90);
      Event_PlaySound(SE_RUMBLE);
    }
    spark = table;
    do {
      if (spark->age == 0) {
        spark->sprite.words[0] = OBJ_SIZE_16X16;
        spark->sprite.words[1] = 0;
        spark->sprite.oam.x = Random_Below(96) + 72;
        spark->sprite.oam.y = Random16() >> 10;
        spark->sprite.oam.palette = LOHO_SPARK_PALETTE;
        spark->sprite.oam.priority = 0;
      }
      if (spark->age >= 0) {
        spark->sprite.oam.tile = tiles + (spark->age >> 2) * 4;
        Oam_AddSpriteAtDepthClamped(spark, LOHO_SPARK_DEPTH);
      }
      spark->age++;
      if (spark->age >= LOHO_SPARK_FRAMES) {
        spark->age = -(Random_Below(7));
      }
      spark++;
    } while ((s32)spark <= (s32)&table[LOHO_SPARK_COUNT - 1]);
    wait(1);
  }
  VramCache_FreeEntry(entry);
  Heap_Release(table);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 390, 1);
  Actor_SetPositionAndFacing(ACTOR_MINER_AT_HIS_SPOT, toFIXED(248),
                             toFIXED(480), FACING_EAST);
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
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_SHAKE_HEAD);
  // "Ah, never mind... What's important here is that we can dig on the..."
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
  // "Yeah, who cares how it happened? We can dig again!"
  Event_ShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_NOD);
  Event_Wait(40);
  FieldActor_ShowEmote(ACTOR_MAN_KEEPING_THE_CANNON,
                       EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 35);
  // "I don't know, but I can tell you they came by boat..."
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(542, CAMERA_KEEP, 256, 1);
  zero = 0;
  miner = Actor_GetById(ACTOR_MINER_BY_THE_CANNON);
  miner->motionFlags = zero;
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
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
  Event_Wait(60);
  // "Yeah, I saw one anchored off the beach!"
  Event_ShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
  Actor_WaitForScriptEndById(ACTOR_MAN_KEEPING_THE_CANNON);
  Actor_WaitForScriptEndById(ACTOR_MINER_PUZZLED_BY_THE_CANNON);
  Actor_FaceEachOther(ACTOR_MAN_KEEPING_THE_CANNON,
                      ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
  Event_ShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
  gFieldWork->message++;
  goto crewLeaves;
toWest:
  Actor_WalkToAndWait(ACTOR_FELIX, 471, 373);
  goto inPlace;
answeredNo:
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_SHAKE_HEAD);
  gFieldWork->message++;
  Event_ShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
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
  cannon = Actor_GetById(ACTOR_CANNON);
  cannon->motionFlags = 0;
  Actor_SetScript(ACTOR_CANNON, (s32)gLoho_CannonRises);
  Actor_WaitForScriptEndById(ACTOR_CANNON);
  Actor_SetSpeed(ACTOR_CANNON, 0.3);
  FieldCamera_SetSpeed(0.3);
  FieldCamera_MoveTo(460, CAMERA_KEEP, 350, 1);
  Event_Wait(30);
  walkingMiner = Actor_GetById(ACTOR_MINER_BY_THE_CANNON);
  walkingMiner->steeringFlags &= ~ACTOR_FACES_MOTION;
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

/// Mind Read on the digger at the wall: where the cannon went, before the ship
/// carries it and after.
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
  Event_DigUpTreasure(id, FIELD_TREASURE_ACTOR, FLAG_AREA_TEMP_00);
}

/// The second Scoop spot.
void Loho_ScoopSecondTreasure(s32 id) {
  Event_DigUpTreasure(id, FIELD_TREASURE_ACTOR + 1, FLAG_AREA_TEMP_01);
}

/// The third Scoop spot, under the boulder Lift raises.
void Loho_ScoopThirdTreasure(s32 id) {
  Event_DigUpTreasure(id, FIELD_TREASURE_ACTOR + 2, FLAG_AREA_TEMP_02);
}

/// Lift raises the boulder, and the Golem Core under it shows until it is dug
/// up (FLAG_LOHO_GOLEM_CORE_DUG).
void Loho_LiftBoulder(void) {
  Event_SetFlag(FLAG_LOHO_BOULDER_LIFTED);
  Actor_GetById(ACTOR_BOULDER)->collisionFlags &= ~ACTOR_SOLID;
  if (Event_GetFlag(FLAG_LOHO_GOLEM_CORE_DUG) == 0)
    Actor_SetPosition(FIELD_TREASURE_ACTOR + 2, 520, 136);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_LOHO)
    return gLoho_TownTriggers;
  if (gMainRoutineWork.next == SCENE_LOHO_ROOMS)
    return gLoho_RoomsTriggers;
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

/// The inn's counter: the woman who keeps the shop checks the party in, so the
/// inn's lines are hers; the counter itself is an unseen marker.
void Loho_TalkToInnkeeper(void) {
  Inn_CheckIn(LOHO_INN, ACTOR_VILLAGER_ON_COMMERCE);
}

/// "Commerce really took off when we started this mining colony in Loho."
void Loho_TalkAboutCommerce(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId);
  if (Event_GetFlag(FLAG_AREA_TEMP_20)) {
    Shop_Run(SHOP_LOHO, 8);
  } else {
    if (Event_GetFlag(FLAG_LOHO_WALL_BREACHED))
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
    if (Event_GetFlag(FLAG_LOHO_WALL_BREACHED))
      Event_SetMessageId((s32)MsgLohoYouveAcquiredSplendidCannonJust);
    else
      Event_SetMessageId((s32)MsgLohoIfYoureOnlyHere);
    // "If you're only here for the cannon, you can forget about it."
    Event_ShowMessage(actor, 0);
  }
}

/// The villager by the shop steps north, out of the way; spoken
/// to then, she opens the shop.
void Loho_VillagerStepsAsideNorth(void) {
  Actor_SetScript(ACTOR_VILLAGER_ON_COMMERCE, gLoho_VillagerMovesNorth);
  Event_SetFlag(FLAG_AREA_TEMP_20);
}

/// The villager by the shop steps west, out of the way; spoken
/// to then, she opens the shop.
void Loho_VillagerStepsAsideWest(void) {
  Actor_SetScript(ACTOR_VILLAGER_ON_COMMERCE, gLoho_VillagerMovesWest);
  Event_SetFlag(FLAG_AREA_TEMP_20);
}

/// The villager by the shop returns to her place.
void Loho_VillagerStepsBack(void) {
  Event_ClearFlag(FLAG_AREA_TEMP_20);
  Actor_SetScript(ACTOR_VILLAGER_ON_COMMERCE, gLoho_VillagerReturns);
}

/// Plays a map cell animation at its place on the map grid.
static __inline__ void Loho_AnimateCells(const void *steps, s32 x, s32 y) {
  MapGrid_AnimateCells(steps, x, y);
}

/// The leader opens a house door and walks in, the town's actors held still.
void Loho_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 *trigger;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct Actor *actor = Actor_GetById(i);

    if (actor != NULL)
      actor->motionFlags = 0;
  }
  trigger = &work->stepTrigger;
  Event_PlaySound(SE_DOOR_OPEN);
  Loho_AnimateCells(gLoho_HouseDoor.steps, gLoho_HouseDoor.x,
                    gLoho_HouseDoor.y);
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

/// The leader walks through an open doorway into the room behind it.
void Loho_EnterDoor(void) {
  struct FieldWork *work = gFieldWork;

  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Event_PlaySound(SE_MAP_EXIT);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -4);
  Import_EventSetExitRequest(work->stepTrigger);
}

/// The leader climbs down the ladder to the mine below, by the step trigger
/// that brought him there.
void Loho_ClimbDownLadder(void) {
  s32 trigger = gFieldWork->stepTrigger;

  if (trigger == 2)
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
  Import_EventSetExitRequest(trigger);
  Event_End();
}

/// The leader steps up and east onto the ladder and climbs down.
void Loho_StepOntoLadderFacingEast(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

/// The leader steps up and west onto the ladder and climbs down.
void Loho_StepOntoLadderFacingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

/// The leader turns to the ladder and climbs down.
void Loho_TurnAndClimbDownLadder(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Loho_ClimbDownLadder();
}

/// The town opens with the backdrop fade; the cannon casts no shadow.
void Loho_SetUpTown(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
}

/// The houses open with the window transition; the inn counter's marker casts
/// no shadow.
void Loho_SetUpRooms(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_INN_COUNTER), 0);
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
    Loho_SetUpRooms();
  }
  if (Event_GetFlag(FLAG_LOHO_WALL_BREACHED)) {
    Actor_SetPosition(ACTOR_CANNON, 0, 0);
    Actor_SetPositionAndFacing(ACTOR_MINER_AT_HIS_SPOT, toFIXED(248),
                               toFIXED(480), FACING_EAST);
    if (!Event_GetFlag(FLAG_SHIP_CARRIES_CANNON)) {
      Actor_SetPosition(ACTOR_MAN_KEEPING_THE_CANNON, 0, 0);
      Actor_SetPosition(ACTOR_MINER_BY_THE_CANNON, 0, 0);
      Actor_SetPosition(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0, 0);
    } else {
      Loho_PlaceFacing(ACTOR_MINER_BY_THE_CANNON, toFIXED(459), toFIXED(254),
                       FACING_NORTHEAST);
      Loho_PlaceFacing(ACTOR_MINER_PUZZLED_BY_THE_CANNON, toFIXED(442),
                       toFIXED(203), FACING_WEST);
      Loho_PlaceFacing(ACTOR_MAN_KEEPING_THE_CANNON, toFIXED(490), toFIXED(188),
                       FACING_SOUTHEAST);
      Actor_GetById(ACTOR_MAN_KEEPING_THE_CANNON)->timer =
          FIXEDtoINT(Actor_GetById(ACTOR_MAN_KEEPING_THE_CANNON)->x.fixed);
      Actor_GetById(ACTOR_MAN_KEEPING_THE_CANNON)->userValue =
          FIXEDtoINT(Actor_GetById(ACTOR_MAN_KEEPING_THE_CANNON)->z.fixed);
      Actor_SetScript(ACTOR_MAN_KEEPING_THE_CANNON, 2);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_LOHO &&
      Event_GetFlag(FLAG_LOHO_WALL_BREACHED))
    Loho_DrawBrokenWall();
  return 0;
}

// clang-format off
/// The miner by the cannon walks up to the breached wall.
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

/// The miner puzzled by the cannon steps up to look at the wall.
s32 gLoho_SecondMinerStepsUp[] = {
    SC_WAIT(50),
    SC_ANIMATION(2),
    SC_MOVE_BY(4, 0, -40),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

/// The man keeping the cannon steps up to look at the wall.
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

/// The miner by the cannon walks back from the wall.
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

/// The miner by the cannon goes to fetch it.
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

/// The miner by the cannon comes back with it.
s32 gLoho_FirstMinerReturns[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(472, 331),
    SC_SYNC,
    SC_MOVE_XZ(472, 361),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

/// Run on the cannon as the crew sets off with it: it rises two pixels off the
/// ground. The commands after the first SC_END are never reached.
s32 gLoho_CannonRises[] = {
    SC_MOVE_BY(0, 2, 0),
    SC_SYNC,
    SC_END,
    SC_ANIMATION(5),
    SC_WAIT(1),
    SC_END,
    SC_WAIT(5),
    SC_ANIMATION(5),
    SC_SET(SW_SCALE_X, -FIXED_ONE),
    SC_END,
};

/// The villager by the shop pacing in her house.
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

/// The villager by the shop going back to her place.
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

/// The villager by the shop stepping west, out of the way.
s32 gLoho_VillagerMovesWest[] = {                                                               
      SC_SPEED(0.9),                                                
      SC_ANIMATION(2),                                                               
      SC_MOVE(128, 0, 144),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_END,                                                                  
  };

/// The villager by the shop stepping north, out of the way.
s32 gLoho_VillagerMovesNorth[] = {                                                               
      SC_SPEED(0.9),                                                
      SC_ANIMATION(2),                                                               
      SC_MOVE(159, 0, 128),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_END,                                                                  
  };

#include "shared/scene_entrances_default.c"

/// The houses' exit regions.
struct SceneRegion gLoho_Regions[] = {
    {452, 28, 740, 460, 36, 748, CONDITION_ALWAYS, 6},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Loho),
    SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_NORTHEAST, LohoRooms, LOHO_ROOMS_FROM_LOHO_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_WEST, LohoRooms, LOHO_ROOMS_FROM_LOHO_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_NORTHWEST, LohoRooms, LOHO_ROOMS_FROM_LOHO_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_SOUTHWEST, LohoRooms, LOHO_ROOMS_FROM_LOHO_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_EAST, LohoRooms, LOHO_ROOMS_FROM_LOHO_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_SOUTH_A, LohoRooms, LOHO_ROOMS_FROM_LOHO_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_SOUTH_B, LohoRooms, LOHO_ROOMS_FROM_LOHO_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_SOUTH_C, LohoRooms, LOHO_ROOMS_FROM_LOHO_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LOHO, EXIT_FLAG_NONE),
    SCENE_EXITS(LohoRooms),
    SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_NORTHEAST, Loho, LOHO_FROM_LOHO_ROOMS_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_WEST, Loho, LOHO_FROM_LOHO_ROOMS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_NORTHWEST, Loho, LOHO_FROM_LOHO_ROOMS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_SOUTHWEST, Loho, LOHO_FROM_LOHO_ROOMS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_EAST, Loho, LOHO_FROM_LOHO_ROOMS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_SOUTH, Loho, LOHO_FROM_LOHO_ROOMS_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, LohoRooms, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, LohoRooms, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LohoRooms, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, LohoRooms, 9, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The town's actors, by enum LohoActorId.
struct ActorPlacement gLoho_TownPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 304, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 392, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 264, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_PURPLE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 320, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 416, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 568, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 544, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_PURPLE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 544, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 336, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_PURPLE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 336, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_LULL), MOVEMENT_TYPE_STAND, 232, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_CANNON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 360, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The houses' actors, by enum LohoRoomsActorId.
struct ActorPlacement gLoho_RoomsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, gLoho_VillagerPaces, 128, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The triggers of neither the town nor the houses: none.
struct Trigger gLoho_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The town's triggers.
struct Trigger gLoho_TownTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Loho_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 6, CONDITION_ALWAYS, EVENT(Loho_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 8, CONDITION_ALWAYS, EVENT(Loho_StepOntoLadderFacingWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 9, CONDITION_ALWAYS, EVENT(Loho_StepOntoLadderFacingEast)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_MINER_WELCOMING_TO_LOHO, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoWasThatYouThatFired)},
    {TRIGGER_TALK, ACTOR_MINER_ON_THE_EXCAVATIONS, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoThatCannonSureGivesOff)},
    {TRIGGER_TALK, ACTOR_MINER_WHO_FOUND_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoWhenWeFoundThatCannon)},
    {TRIGGER_TALK, ACTOR_DIGGER_AT_THE_WALL, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), EVENT(Loho_TalkAboutBreachedWall)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WELCOMING_TO_LOHO, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoThatCannonCanShatterAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_ON_THE_EXCAVATIONS, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoThatCannonWasReallyLoud)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_FOUND_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIDontThinkWeShould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DIGGER_AT_THE_WALL, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), EVENT(Loho_MindReadMiner)},
    {TRIGGER_TALK, ACTOR_MINER_AWAITING_WORK, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIveGottaGoStakeMyself)},
    {TRIGGER_TALK, ACTOR_MINER_WHO_KNOWS_THE_EARTH, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoYouProbablyOnlyWantCannon)},
    {TRIGGER_TALK, ACTOR_MINER_AT_HIS_SPOT, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoAhSoYouHaveQuest)},
    {TRIGGER_TALK, ACTOR_DIGGER_FINDING_NOTHING, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIThinkWeveDugUp)},
    {TRIGGER_TALK, ACTOR_MINER_BY_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoThanksForOpeningUpWall)},
    {TRIGGER_TALK, ACTOR_MINER_PUZZLED_BY_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIKeepDiggingUpLittle)},
    {TRIGGER_TALK, ACTOR_MAN_KEEPING_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoHaHaHaIveGot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AWAITING_WORK, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIJustNeedToStick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_KNOWS_THE_EARTH, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoTheseGuysMustBeReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AT_HIS_SPOT, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoWhenTheyGetWhereverTheyre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DIGGER_FINDING_NOTHING, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoEvenAtMyAge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_BY_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIOnlyJustStartedDigging)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_PUZZLED_BY_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIveHitMotherLode)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoItDoesntMatterHowOld)},
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 101, FLAG_AREA_TEMP_00, EVENT(Loho_ScoopFirstTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 102, FLAG_AREA_TEMP_01, EVENT(Loho_ScoopSecondTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 103, FLAG_AREA_TEMP_02, EVENT(Loho_ScoopThirdTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, FLAG_LOHO_BOULDER_LIFTED, EVENT(Loho_LiftBoulder)},
    {TRIGGER_TABLE_END},
};

/// The houses' triggers.
struct Trigger gLoho_RoomsTriggers[] = {
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
    {STEP_TRIGGER_FACING(FACING_NORTH), 20, FLAG_AREA_TEMP_20, EVENT(Loho_VillagerStepsAsideWest)},
    {STEP_TRIGGER_FACING(FACING_WEST), 22, FLAG_AREA_TEMP_20, EVENT(Loho_VillagerStepsAsideNorth)},
    {TRIGGER_STEP, 21, IF_FLAG_SET(FLAG_AREA_TEMP_20), EVENT(Loho_VillagerStepsBack)},
    {TRIGGER_STEP, 23, IF_FLAG_SET(FLAG_AREA_TEMP_20), EVENT(Loho_VillagerStepsBack)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_COMMERCE, CONDITION_ALWAYS, EVENT(Loho_TalkAboutCommerce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_COMMERCE, FLAG_LOHO_WALL_BREACHED, MSGNO(MsgLohoWheneverSomeoneStrikesGold)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_COMMERCE, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoNowWeDontHaveSymbol)},
    {TRIGGER_TALK, ACTOR_INN_COUNTER, IF_FLAG_SET(FLAG_AREA_TEMP_20), EVENT(Loho_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_BEARDED_MAN_ON_THE_CANNON, CONDITION_ALWAYS, EVENT(Loho_TalkAboutCannon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BEARDED_MAN_ON_THE_CANNON, FLAG_LOHO_WALL_BREACHED, MSGNO(MsgLohoIfCannonIsTruly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BEARDED_MAN_ON_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoItWorriesMeToNo)},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(Loho_TalkToInnkeeper)},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(Loho_ReadInnSign)},
    {TRIGGER_TABLE_END},
};

/// The steps of a house door opening.
s16 gLoho_DoorOpening[] = {
    64, 50, 2, 2, 2, 64, 52, 2,
    2, 2, 64, 54, 2, 2, 2, -1,
};
// clang-format on

/// A house door's opening, at its place on the map grid.
struct MapGridCellAnimation gLoho_HouseDoor = {gLoho_DoorOpening, 79, 15};
