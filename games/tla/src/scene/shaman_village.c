/// @file
/// Shaman Village, in the middle of Hesperia, whose people will not talk to
/// outsiders by Moapa's order; only the priest in the sanctum, in the cave on
/// the east side, does. At Moapa's door in the north-west he and his Knights
/// come out and send the party away, until it shows him the Shaman's Rod; then
/// he sets it his test at the foot of Trial Road, north of the village. There
/// Sheba's Whirlwind on the purple twister rock blows the sand away, Moapa
/// tells the legend of Yegelos and Hoabna and the trial's rules, and after a
/// count of 3, 2, 1 and `GO` the party and Moapa race up the road by its left
/// and right paths. Once he is beaten the village's other doors and its tents
/// open: the general store, a family's tent, and the fortune-teller's in the
/// north-east, where Move drops a pillar into a gap on the way to Aroma, the
/// Jupiter Djinni, and the rope Lash ties across the river by Moapa's house
/// leads on. Growth's vine north-west of the inn climbs to the Spirit Gloves'
/// chest.
#include "djinn_ids.h"
#include "edition.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "resource_ids.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "macros.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_psynergy_state.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "field/field_sprite.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "game/item.h"
#include "graphics/window_sprite_list.h"
#include "graphics/glyph.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/affine.h"
#include "system/dma.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/main_routine_state.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "window/window.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"
#include "shop/shop.h"

/// The people and things at the foot of Trial Road, numbering the rows of
/// gShamanVillage_TrialGroundsPlacements; Moapa, his Knights and Kraden
/// keep the same rows in the village's table
/// (gShamanVillage_VillagePlacements).
enum ShamanVillageTrialActorId {
  /// The six torches about the start of Trial Road (SPRITE_TORCH).
  ACTOR_FIRST_TORCH = ACTOR_FIRST_PLACED,
  ACTOR_LAST_TORCH = ACTOR_FIRST_TORCH + 5,
  /// The purple twister rock in the sand, which Whirlwind is cast on.
  ACTOR_TWISTER_ROCK,
  /// Moapa, head of the Shaman tribe: "I cannot lose. I will not lose."
  ACTOR_MOAPA,
  /// The two Knights at Moapa's sides, who fight beside him at Trial Road's
  /// summit.
  ACTOR_WEST_KNIGHT,
  ACTOR_EAST_KNIGHT,
  ACTOR_KRADEN
};

/// Moapa at home, the fourth row of gShamanVillage_HousesPlacements (the
/// village's houses and inn), with the inn's counter and its innkeeper after.
enum ShamanVillageHousesActorId {
  ACTOR_MOAPA_AT_HOME = ACTOR_FIRST_PLACED + 3,
  ACTOR_INN_COUNTER,
  ACTOR_INNKEEPER
};

/// The rest of the houses: in Moapa's, his son, and the woman and the girl with
/// him, who talk of the party's names; and the two who plan a picnic and the
/// chef who cooks for them, there once Moapa is beaten.
enum {
  ACTOR_WOMAN_IN_MOAPAS_HOUSE = ACTOR_FIRST_PLACED,
  ACTOR_MOAPAS_SON,
  ACTOR_GIRL_IN_MOAPAS_HOUSE,
  ACTOR_PICNIC_SHAMAN_A = ACTOR_FIRST_PLACED + 9,
  ACTOR_PICNIC_SHAMAN_B,
  ACTOR_CHEF
};

/// The villager who asks whether the party has looked over the mountain.
#define ACTOR_MOUNTAIN_SIDE_SHAMAN (ACTOR_FIRST_PLACED + 5)
/// The girl who wanders below the mouth of the sanctum's cave, and the sprout
/// north-west of the inn that Growth grows into a vine, both drawn at the
/// leader's sprite priority.
#define ACTOR_GIRL_BELOW_THE_CAVE (ACTOR_FIRST_PLACED + 6)
#define ACTOR_SPROUT (ACTOR_FIRST_PLACED + 13)
/// The posts of the village's rope across the river by Moapa's house, which
/// Lash ties: one west of the river, at cell (11, 13), the other east of it,
/// at cell (17, 13).
enum {
  ACTOR_VILLAGE_ROPE_POST = ACTOR_FIRST_PLACED + 11,
  ACTOR_CLIFF_ROPE_POST
};
/// The sanctum's two villagers after the priest, there once Moapa is beaten.
enum {
  ACTOR_SANCTUM_SHAMAN_A = ACTOR_FIRST_PLACED + 1,
  ACTOR_SANCTUM_SHAMAN_B
};

/// The general store's keeper, in the west tent.
#define ACTOR_GENERAL_STOREKEEPER (ACTOR_FIRST_PLACED + 4)
/// The sanctum's priest, behind the counter in the cave.
#define ACTOR_PRIEST ACTOR_FIRST_PLACED
/// The north-east of the village, around the fortune-teller's tent: the
/// pillar Move drops into the gap, the fortune-teller and her fire, and Aroma,
/// the Jupiter Djinni, on the ledge east of the tent.
#define ACTOR_AROMA_ROOM_PILLAR (ACTOR_FIRST_PLACED + 6)
/// The small fire before the fortune-teller, which flares as she hops.
#define ACTOR_FORTUNE_TELLERS_FLAME (ACTOR_FIRST_PLACED + 8)
#define ACTOR_FORTUNE_TELLER (ACTOR_FIRST_PLACED + 9)
#define ACTOR_AROMA (ACTOR_FIRST_PLACED + 10)

/// Set as the party comes back from beating Moapa: the village's locked doors
/// and tents open.
#define FLAG_SHAMAN_VILLAGE_DOORS_OPEN 0x94d
/// Set once Move has dropped the pillar by the fortune-teller's tent into the
/// gap.
#define FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED 0x94e
/// Set once Moapa has told the party the rules of the trial: the Shaman's Rod
/// and eight pieces of equipment.
#define FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD 0x9e0
/// Set once Moapa has asked the party's names after the battle; he remembers
/// them from then on.
#define FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES 0x9ed
/// Set once the fortune-teller has read the party's fortune, for this visit.
#define FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD FLAG_AREA_TEMP_01
/// Set once Lash has tied the rope across the river by Moapa's house.
#define FLAG_SHAMAN_VILLAGE_ROPE_TIED FLAG_SCENE_TEMP_04
/// Set once Growth has grown the vine north-west of the inn.
#define FLAG_SHAMAN_VILLAGE_VINE_GROWN FLAG_SCENE_TEMP_05
/// Set once the party has shown the Shaman's Rod
/// (ShamanVillage_ShowShamansRod): Moapa's door stays open.
#define FLAG_SHAMAN_VILLAGE_MOAPAS_DOOR_OPEN FLAG_SCENE_TEMP_02
/// Set once the party has asked to hear Moapa's explanation again: he gives
/// only its rules.
#define FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN FLAG_SCENE_TEMP_00
/// Set once Moapa has accepted the party's challenge, until Felix steps back
/// from the trial: walking back to Moapa then has him offer to let the party
/// give up.
#define FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED FLAG_SCENE_TEMP_01
/// Set once the fortune-teller has called out to Felix, for this visit.
#define FLAG_SHAMAN_VILLAGE_FORTUNE_TELLER_CALLED_OUT FLAG_SCENE_TEMP_03

/// The cells that run an event; the village's door cells hold instead the
/// numbers of the exits they lead by.
enum ShamanVillageCell {
  /// At the foot of Trial Road, cells (9, 21) to (11, 21), on the way up to
  /// Moapa.
  BEFORE_MOAPA_CELL = 10,
  /// At the foot of Trial Road, the left path up, cells (10, 13) to (12, 13).
  LEFT_PATH_CELL = 11,
  /// The right path up, cells (15, 13) to (17, 13).
  RIGHT_PATH_CELL = 12,
  /// The sign at the gate of Trial Road, cell (13, 6).
  TRIAL_ROAD_SIGN_CELL = 10,
  /// Before the fire in front of the fortune-teller's tent, cells (37, 19)
  /// and (37, 20).
  FORTUNE_TELLERS_FIRE_CELL = 11,
  /// Before the door of a house, where pressing A while Moapa is unbeaten
  /// says it is locked.
  LOCKED_DOOR_CELL = 20,
  /// Before Moapa's door, cell (7, 11).
  MOAPAS_DOOR_CELL = 21,
  /// Before the entrance of a tent, closed while Moapa is unbeaten: cells
  /// (13, 24) and (17, 28) in the village, (37, 14) in the north-east.
  CLOSED_TENT_CELL = 22
};

#include "../field/field_effect_move.c"

struct ItemDefinition;

/// The whirlwind's swirl: how far each line of its picture is shifted.
s16 gShamanVillage_WaveLines[DISPLAY_HEIGHT] BSS_DATA ALIGNED(16);
s16 gShamanVillage_LetteringScaleFrames BSS_DATA;
s16 gShamanVillage_LetteringTileOffset BSS_DATA ALIGNED(4);
s16 gShamanVillage_LetteringScaleFrom BSS_DATA ALIGNED(4);
s16 gShamanVillage_LetteringSpeedFrames BSS_DATA ALIGNED(4);
s32 gShamanVillage_LetteringX BSS_DATA;
s16 gShamanVillage_LetteringScale BSS_DATA;
s16 gShamanVillage_LetteringSpeedFrame BSS_DATA ALIGNED(4);
s16 gShamanVillage_LetteringBlendFrame BSS_DATA ALIGNED(4);
s16 gShamanVillage_LetteringMode BSS_DATA ALIGNED(4);
s16 gShamanVillage_LetteringBlend BSS_DATA ALIGNED(4);
s16 gShamanVillage_LetteringBlendFrom BSS_DATA ALIGNED(4);
s16 gShamanVillage_LetteringWait BSS_DATA ALIGNED(4);
s16 *gShamanVillage_LetteringCursor BSS_DATA;
s16 gShamanVillage_LetteringBlendFrames BSS_DATA;
u32 gShamanVillage_LetteringSprites[12] BSS_DATA ALIGNED(16);
s16 gShamanVillage_LetteringSpeedFrom BSS_DATA;
s16 gShamanVillage_LetteringSpeed BSS_DATA ALIGNED(4);
s16 gShamanVillage_LetteringScaleFrame BSS_DATA ALIGNED(4);

/// The whirlwind's swirl: a VBlank task that wavers its lines.
void ShamanVillage_WaveWhirlwindLines(void);

extern u32 gShamanVillage_KradenStandsByRodScript[];
extern u32 gShamanVillage_PiersStandsByRodScript[];
extern u32 gShamanVillage_ShebaStandsByRodScript[];
extern u32 gShamanVillage_MoapaLeavesHouseScript[];
extern u32 gShamanVillage_WestKnightLeavesHouseScript[];
extern u32 gShamanVillage_ReturnToLeaderAtHouseScript[];

extern s16 gShamanVillage_LetteringVramEntry[];

/// The large lettering of the race's countdown before Trial Road: 3, 2, 1 and
/// `GO`, from Colosso's pictures (games/tla/src/graphics/lettering), moved by a
/// script of 16-bit words, a command and its values:
enum LetteringCommand {
  /// x, speed: put the lettering at x pixels from the centre, moving at speed
  /// 256ths of a pixel a frame
  LETTERING_PLACE = 16384,
  /// speed, frames: change the speed over that many frames
  LETTERING_ACCELERATE = 12288,
  /// scale, frames: grow or shrink to scale (256 is its own size)
  LETTERING_SCALE = 4096,
  /// weight, frames: fade to weight sixteenths over the screen
  LETTERING_FADE = 8192,
  /// frames: run the moves for that many frames before the next command
  LETTERING_WAIT = 32767,
  /// remove the lettering and free its VRAM block
  LETTERING_END = -1
};

/// The lettering pictures, in the sprite sheet's order
/// (ShamanVillage_LoadLettering); the village shows only the countdown and
/// `GO`.
enum LetteringPicture {
  LETTERING_PICTURE_LEND_A_HAND = 0,
  LETTERING_PICTURE_MATCH_TITLE_FIRST =
      1, ///< "The 1st Match", then the 2nd and 3rd
  LETTERING_PICTURE_WIN_LOSE = 4,
  LETTERING_PICTURE_COUNTDOWN_START = 5, ///< the countdown's 3 and 2
  LETTERING_PICTURE_COUNTDOWN_END = 6,   ///< the countdown's 1
  LETTERING_PICTURE_GO = 7,
  /// LETTERING_PICTURE_WIN_LOSE in the blue palette a loss shows it in.
  LETTERING_PICTURE_WIN_LOSE_FOR_LOSS = 8
};

/// How ShamanVillage_UpdateLettering lays a picture out
/// (gShamanVillage_LetteringMode); gShamanVillage_LetteringTileOffset picks the
/// second half of a picture.
enum LetteringShape {
  /// Four 32x16 pieces in a row: The 1st to 3rd Match, Lend a Hand!
  LETTERING_SHAPE_TITLE = 1,
  LETTERING_SHAPE_DIGIT = 2,  ///< one 32x32 piece: 3, 2, 1
  LETTERING_SHAPE_RESULT = 3, ///< two 32x16 pieces: WIN or LOSE
  LETTERING_SHAPE_GO = 4      ///< Go!: one 64x32 piece, or two 32x16 in French
};

/// The lettering's sprites: affine, drawn at twice their size so turning never
/// clips them, and blended; in LETTERING_PALETTE at priority 1, just behind
/// the windows.
#define LETTERING_OBJ_MODE                                                     \
  (OBJ_AFFINE | OBJ_DOUBLE_SIZE | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT))
#define LETTERING_PALETTE 15
#define LETTERING_ATTRIBUTES (OBJ_PALETTE(LETTERING_PALETTE) | OBJ_PRIORITY(1))
#define LETTERING_DEPTH (OAM_DEPTH_WINDOW - 4)
/// The lettering's x and speed are kept in 256ths of a pixel.
#define LETTERING_SUBPIXELS 256
/// The lettering's pieces are 32 pixels wide: piece i of a row of n lies this
/// far from the row's centre.
#define LETTERING_PIECE_X(i, n) ((i) * 32 - ((n) - 1) * 16)

extern s16 gShamanVillage_LetteringTitleScript[];
extern s16 gShamanVillage_LetteringDigitScript[];
extern s16 gShamanVillage_LetteringGoScript[];
extern s16 gShamanVillage_LetteringLoseScript[];
extern s16 gShamanVillage_LetteringWinScript[];
extern u8 gShamanVillage_LetteringPaletteOffsets[];
/// The word of gShamanVillage_LetteringTitleScript that holds the title still
/// in the middle: its second LETTERING_WAIT's frames.
#define LETTERING_TITLE_HOLD 15

void ShamanVillage_CastWhirlwindOnSand(void);
extern u32 gShamanVillage_KradenWatchesShebaScript[];
extern u32 gShamanVillage_LeaderWatchesShebaScript[];
extern u32 gShamanVillage_JennaWatchesShebaScript[];
extern u32 gShamanVillage_PiersWatchesShebaScript[];
extern u32 gShamanVillage_HeadForTrialRoadScript[];
extern u32 gShamanVillage_ReturnToLeaderAtSandScript[];
void ShamanVillage_SetUpTrialGrounds(void);
void ShamanVillage_SetUpNorthEnd(void);
void ShamanVillage_SetUpVillage(void);
void ShamanVillage_SetUpHouses(void);
void ShamanVillage_SetUpSanctum(void);
void Party_RestoreHpPp(s32 cureConditions);
void ShamanVillage_TalkToEastKnight(void);
void ShamanVillage_TellOutsidersToLeave(void);
void ShamanVillage_WarnOfMoapasPower(void);
void ShamanVillage_SendToMoapa(void);
void ShamanVillage_TalkToPriest(void);
void ShamanVillage_AskAboutOtherSide(void);
void ShamanVillage_TalkToMoapa(void);
void ShamanVillage_TalkToGeneralStorekeeper(void);
void ShamanVillage_TellFortune(void);
void ShamanVillage_OfferToGiveUp(void);
void ShamanVillage_ChallengeMoapa(void);
void ShamanVillage_TakeLeftPath(void);
void ShamanVillage_TakeRightPath(void);
void ShamanVillage_DropPillar(void);
void ShamanVillage_MeetMoapa(void);
void ShamanVillage_TalkToInnkeeper(void);

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Import_ActorSelectAnimation
#define EFFECT_SET_SCRIPT Import_ActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

void Actor_Release(struct Actor *actor);
void Rope_Initialize(void);
void Rope_Place(s32 index, s32 flag, s32 post, s32 otherPost);
void Actor_PlaceOnActor(s32 actorId, s32 target);
void InventorySnapshot_Save(void);

void ShamanVillage_BlowAwaySand(void);
void ShamanVillage_HopFortuneTeller(void);
void ShamanVillage_StartLettering(s32 shape, s32 secondHalf);
void ShamanVillage_LoadLettering(s32 picture);
void ShamanVillage_EnterDoor(s32 sourceX, s32 sourceY, s32 destinationX,
                             s32 destinationY, s32 width, s32 height);
s32 ShamanVillage_IsFacingCounter(void);
void ShamanVillage_ExplainTrialRules(void);
void ShamanVillage_StepBackFromTrial(void);
void ShamanVillage_CheckTrialEntry(void);
void ShamanVillage_CountDownToStart(void);

extern s32 gLeader_WalkThroughDoorScript[];
extern s32 gShamanVillage_HopFortuneTellerScript[];
extern struct SceneEntrance gSceneEntrances[];
extern struct SceneEntrance gShamanVillage_TentsEntrances[];
extern struct SceneEntrance gShamanVillage_SanctumEntrances[];

/// Felix talks to Aroma, the Jupiter Djinni on the ledge east of the
/// fortune-teller's tent, and it joins the party: "Felix found the Jupiter
/// Djinni Aroma!"
void ShamanVillage_MeetAroma(void) {
  Djinn_JoinParty(ACTOR_AROMA, DJINNI_AROMA / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_AROMA));
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_SHAMAN_VILLAGE_TENTS)
    return gShamanVillage_TentsEntrances;
  if (scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
    return gShamanVillage_SanctumEntrances;
  return gSceneEntrances;
}

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gShamanVillage_Placements[];
extern struct ActorPlacement gShamanVillage_TrialGroundsPlacements[];
extern struct ActorPlacement gShamanVillage_NorthEndPlacements[];

extern struct ActorPlacement gShamanVillage_VillagePlacements[];
extern struct ActorPlacement gShamanVillage_HousesPlacements[];
extern struct ActorPlacement gShamanVillage_TentsPlacements[];
extern struct ActorPlacement gShamanVillage_SanctumPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
    return gShamanVillage_TrialGroundsPlacements;
  if (scene == SCENE_SHAMAN_VILLAGE_NORTH_END)
    return gShamanVillage_NorthEndPlacements;
  if (scene == SCENE_SHAMAN_VILLAGE) {
    if (Event_GetFlag(FLAG_MOAPA_BEATEN))
      gShamanVillage_VillagePlacements[0].talkFacing =
          TALK_FACING_PARTY_AND_BACK;
    return gShamanVillage_VillagePlacements;
  }
  if (scene == SCENE_SHAMAN_VILLAGE_HOUSES)
    return gShamanVillage_HousesPlacements;
  if (scene == SCENE_SHAMAN_VILLAGE_TENTS)
    return gShamanVillage_TentsPlacements;
  if (scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
    return gShamanVillage_SanctumPlacements;
  return gShamanVillage_Placements;
}

/// The whirlwind's swirl: its lines waver from side to side, as a jittered
/// sine wave.
#define WAVE_SCROLL ShamanVillage_WaveWhirlwindLines
#define WAVE_SCROLL_LINES gShamanVillage_WaveLines
#include "../field/field_effect_wave_scroll.c"
#undef WAVE_SCROLL
#undef WAVE_SCROLL_LINES

/// Drifts the sand layer, the third, while Whirlwind blows: its offset moves
/// two pixels a frame, back in x and on in z, and starts again once z passes
/// 1040.
void ShamanVillage_DriftSandLayer(void) {
  struct MapLayer *sand =
      &((struct MapState *)Ram_HeapSlots->mapGrid)->layers[2];

  sand->offsetX -= toFIXED(2);

  sand->offsetY += toFIXED(2);
  if (sand->offsetY > toFIXED(1040)) {
    sand->offsetX = 0;
    sand->offsetY = toFIXED(1024);
  }
}

/// The twister rock while Whirlwind blows: it flickers blue.
void ShamanVillage_FlickerRock(struct Actor *actor) {
  s32 bit = (((u32)gFrameCount & 3) >> actor->flickerShift) & 1;

  FieldActor_SetSpriteColor(actor, (u8)(bit * actor->flickerColor));
}

/// A spark of the gathering Whirlwind: it circles the centre it keeps in bounce
/// and sceneWord (x, z), twice as wide as it is deep, its radius growing as it
/// rises by gravity a frame, a lap every 32 frames counted in speedLimit; it
/// blinks blue every other two frames.
void ShamanVillage_SpiralSpark(struct Actor *actor) {
  s32 radius = actor->y.fixed;
  s32 step = actor->speedLimit;

  radius >>= 2;
  radius += toFIXED(12);
  actor->x.fixed =
      actor->bounce +
      Iwram_MulFX(Trig_Cos((step & 0xff) * (TRIG_TURN / 32)), radius) * 2;
  actor->z.fixed =
      actor->sceneWord +
      Iwram_MulFX(Trig_Sin((actor->speedLimit & 0xff) * (TRIG_TURN / 32)),
                  radius);
  actor->y.fixed += actor->gravity;
  actor->speedLimit++;
  FieldActor_SetSpriteColor(actor,
                            ((gFrameCount & 2) >> 1) * SPRITE_COLOR_BLUE_TINT);
}

/// The twister rock's x, which the whirlwind's sand swirls about.
#define TWISTER_ROCK_X 168

/// A grain of the whirlwind's sand, swung about the twister rock: left of it
/// the grain speeds up rightward and grows, right of it leftward and shrinks.
/// It takes sprite priority 1 while it moves right, 2 otherwise, and spins.
void ShamanVillage_SwirlSandGrain(struct Actor *actor) {
  actor->x.fixed += actor->bounce;
  actor->y.fixed += actor->gravity;
  actor->z.fixed += actor->sceneWord;
  if (FIXEDtoINT(actor->x.fixed) < TWISTER_ROCK_X) {
    actor->bounce += FIXED_ONE * 3 / 2;
    actor->scaleY = actor->scaleX += FIXED_ONE * 5 / 100;
  } else {
    actor->bounce -= FIXED_ONE * 3 / 2;
    actor->scaleY = actor->scaleX -= FIXED_ONE * 2 / 100;
  }
  if (actor->bounce > 0)
    Scene_SetActorSpritePriority(actor, 1);
  else
    Scene_SetActorSpritePriority(actor, 2);
  ((struct FieldSprite *)actor->sprite)->rotation += TRIG_TURN * 3 / 32;
}

extern u32 gFieldEffect_MediumScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void ShamanVillage_DriftSandLayer(void);

/// Sheba's Whirlwind at the foot of Trial Road: the purple twister rock
/// flickers blue and sheds sparks, a whirlwind swirls up over it in a rush of
/// wind that shakes the ground, sand flies about it, and the sand is gone from
/// the ground.
void ShamanVillage_BlowAwaySand(void) {
  u16 *buffer;
  u16 *cell;
  struct Actor *spark;
  u32 i;
  u32 j;

  buffer = Heap_Alloc(1664);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHIRLWIND_TILES), buffer);
  Dma_Set(buffer, BG_CHAR_BLOCK(3) + WHIRLWIND_TOP_TILE * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(WHIRLWIND_ROW_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
  Dma_Set((u8 *)buffer + WHIRLWIND_ROW_TILES * TILE_SIZE_4BPP,
          BG_CHAR_BLOCK(3) + WHIRLWIND_BOTTOM_TILE * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(WHIRLWIND_ROW_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
  Heap_Release(buffer);
  Actor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
  Actor_SetSpritePriority(ACTOR_TWISTER_ROCK, 1);
  Io_Write16(BGCNT_SCREEN_BASE(4) | BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(1),
             &REG_BG0CNT);
  REG_BG0CNT &= ~BGCNT_PRIORITY(3);
  REG_BG0CNT |= BGCNT_PRIORITY(1);
  Dma_Fill16(WHIRLWIND_FILL_TILE, WINDOW_TILEMAP_VRAM,
             DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(u16));
  buffer = Heap_Alloc(576);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHIRLWIND_MAP), buffer);
  for (i = 0, cell = buffer; i != WHIRLWIND_MAP_WIDTH * WHIRLWIND_MAP_HEIGHT;
       i++, cell++) {
    s32 entry = (*cell + (u16)(WHIRLWIND_TOP_TILE - WHIRLWIND_MAP_FIRST_TILE)) &
                (TILE_NUMBER_MASK | TILE_HFLIP | TILE_VFLIP);

    *cell = entry | BG_TILE_PALETTE(1);
  }
  // A word copy drops the address's two low bits: each row starts at column 6.
  for (cell = buffer, i = 0; i != WHIRLWIND_MAP_HEIGHT;
       i++, cell += WHIRLWIND_MAP_WIDTH)
    Dma_Set(cell, WINDOW_TILEMAP_VRAM + 15 + i * BG_SCREEN_WIDTH * sizeof(u16),
            DMA_COPY_WORDS(WHIRLWIND_MAP_WIDTH / 2), REG_DMA3);
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  Actor_GetById(ACTOR_TWISTER_ROCK)->flickerColor = SPRITE_COLOR_BLUE_TINT;
  Actor_GetById(ACTOR_TWISTER_ROCK)->flickerShift = 1;
  Actor_GetById(ACTOR_TWISTER_ROCK)->update = ShamanVillage_FlickerRock;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(
          SPRITE_PARTICLE, Actor_GetById(ACTOR_TWISTER_ROCK)->x.fixed,
          Actor_GetById(ACTOR_TWISTER_ROCK)->y.fixed,
          Actor_GetById(ACTOR_TWISTER_ROCK)->z.fixed);
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_GetById(ACTOR_TWISTER_ROCK)->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(3) / 2;
      spark->sceneWord = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Import_ActorSelectAnimation((struct Actor *)spark, 7);
      Import_ActorStartScript((struct Actor *)spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
      spark->update = ShamanVillage_SpiralSpark;
    }
    Event_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  Task_Add(ShamanVillage_WaveWhirlwindLines, TASK_PASS_VBLANK);
  Event_PlaySound(SE_RUSH);
  REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX - 1);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_SetShake(1, 1, 1);
  i = 0;
  do {
    REG_BLDALPHA = ((BLEND_WEIGHT_MAX - 1 - (i >> 2)) << 8) | (i >> 2);
    Actor_GetById(ACTOR_TWISTER_ROCK)->flickerColor = Random16() & 7;
    i++;
    Event_Wait(1);
  } while (i != 64);
  Event_Wait(60);
  MapGrid_SetShake(3, 3, 1);
  REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  MapGrid_CopyCellsTo(2, 83, 36, 64, 16, 16);
  MapGrid_CopyCellsTo(64, 64, 2, 83, 16, 16);
  REG_BG1CNT &= ~BGCNT_PRIORITY(3);
  Task_Add(ShamanVillage_DriftSandLayer, TASK_PASS_FRAME);
  i = 0;
  do {
    REG_BLDALPHA = ((BLEND_WEIGHT_MAX - (i >> 2)) << 8) | (i >> 2);
    Actor_GetById(ACTOR_TWISTER_ROCK)->flickerColor = Random16() & 7;
    i++;
    Event_Wait(1);
  } while (i != 68);
  {
    s32 width = 13;
    s32 height = 11;

    MapGrid_CopyCellsTo(45, 90, 4, 19, width, height);
  }
  Io_Write16(BGCNT_SCREEN_BASE(6) | BGCNT_PRIORITY(2), &REG_BG2CNT);
  Dma_Set(BG_SCREEN_BLOCK(5), BG_SCREEN_BLOCK(6), DMA_COPY_WORDS(512),
          REG_DMA3);
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG3;
  i = 0;
  do {
    MapGrid_CopyCellsTo((i - (i / 4) * 4) * 14 + 45, (i / 4) * 12 + 90, 4, 19,
                        13, 11);
    j = 0;
    do {
      if ((j & 3) == 0) {
        s32 velocity[3];
        struct EffectOptions options;

        options.priority = 1;
        options.update = ShamanVillage_SwirlSandGrain;
        velocity[0] = 0;
        velocity[2] = -toFIXED(2);
        velocity[1] = 0;
        {
          FIXED x = INTtoFIXED(Random16() & 127) + toFIXED(104);

          FieldEffect_Spawn(x, 0, INTtoFIXED(472 - (Random16() & 127)),
                            velocity[0], velocity[1], velocity[2],
                            EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY |
                                EFFECT_SCRIPT_MEDIUM,
                            &options);
        }
      }
      REG_BLDALPHA = ((j >> 2) << 8) | (BLEND_WEIGHT_MAX - (j >> 2));
      Actor_GetById(ACTOR_TWISTER_ROCK)->flickerColor = Random16() & 7;
      wait(1);
      j++;
    } while (j != 64);
    Dma_Set(BG_SCREEN_BLOCK(5), BG_SCREEN_BLOCK(6), DMA_COPY_WORDS(512),
            REG_DMA3);
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
    i++;
  } while (i != 8);
  {
    s32 control = BGCNT_SCREEN_BASE(6) | BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(2);

    REG_BG2CNT = control;
  }
  REG_DISPCNT &= ~DISPCNT_BG2_ON;
  {
    s32 width = 9;
    s32 height = 11;

    MapGrid_CopyCellsTo(47, 19, 38, 19, width, height);
  }
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  i = 0;
  do {
    REG_BLDALPHA = ((i >> 2) << 8) | (BLEND_WEIGHT_MAX - (i >> 2));
    Actor_GetById(ACTOR_TWISTER_ROCK)->flickerColor = Random16() & 7;
    i++;
    Event_Wait(1);
  } while (i != 68);
  Task_Remove(ShamanVillage_DriftSandLayer);
  REG_BG1CNT &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT |= BGCNT_PRIORITY(1);
  MapGrid_CopyCellsTo(36, 64, 2, 83, 16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(15, 0);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Actor_GetById(ACTOR_TWISTER_ROCK)->flickerColor = SPRITE_COLOR_GREY;
  for (i = 0; i != 65; i++) {
    u32 q = i >> 2;

    REG_BLDALPHA = (q << 8) | (BLEND_WEIGHT_MAX - q);
    Event_Wait(1);
  }
  Task_Remove(ShamanVillage_WaveWhirlwindLines);
  Actor_GetById(ACTOR_TWISTER_ROCK)->flickerShift = 0;
  wait(30);
  Actor_GetById(ACTOR_TWISTER_ROCK)->update = (void *)0;
  FieldActor_SetSpriteColor(Actor_GetById(ACTOR_TWISTER_ROCK),
                            SPRITE_COLOR_GREY);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(60);
  REG_BLDCNT = 0;
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineState.windowHue,
                         gMainRoutineState.windowBrightness);
  Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags &=
      ~ACTOR_PRIORITY_IGNORES_RISES;
  Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_TWISTER_ROCK)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  REG_DISPCNT |= DISPCNT_BG2_ON;
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  {
    s32 width = 6;
    s32 height = 29;

    MapGrid_CopyCellAttributes(1, 5, 9, 1, width, height);
  }
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED);
}

extern struct Trigger gShamanVillage_Triggers[];
extern struct Trigger gShamanVillage_TrialGroundsTriggers[];
extern struct Trigger gShamanVillage_NorthEndTriggers[];
extern struct Trigger gShamanVillage_VillageTriggers[];
extern struct Trigger gShamanVillage_HousesTriggers[];
extern struct Trigger gShamanVillage_TentsTriggers[];
extern struct Trigger gShamanVillage_SanctumTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
    return gShamanVillage_TrialGroundsTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE_NORTH_END)
    return gShamanVillage_NorthEndTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE)
    return gShamanVillage_VillageTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE_HOUSES)
    return gShamanVillage_HousesTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE_TENTS)
    return gShamanVillage_TentsTriggers;
  if (scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
    return gShamanVillage_SanctumTriggers;
  return gShamanVillage_Triggers;
}

#include "shared/scene_show_entrance_locked.h"

/// Before Moapa is beaten, Felix tries the entrance of one of the village's
/// tents, the general store in the west (cell (13, 24)), the family's in the
/// east (cell (17, 28)) or the fortune-teller's in the north-east: "The
/// entrance is closed. You probably shouldn't go in."
void ShamanVillage_StopAtClosedEntrance(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  // "The entrance is closed. You probably shouldn't go in."
  FieldMessage_Show((s32)MsgShamanEntranceClosed, 1);
  Event_End();
}

/// Felix reads the sign at the gate of Trial Road, between its two torches:
/// "Trial Road. None may enter without permission from Moapa."
void ShamanVillage_ReadTrialRoadSign(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  // "Trial Road None may enter without permission from Moapa."
  FieldMessage_Show((s32)MsgShamanTrialRoadPermissionSign, 1);
  Event_End();
}

/// Talking to the east Knight at the foot of Trial Road: "Do you need to hear
/// Moapa's explanation again?" Yes, and Felix steps up to Moapa, who gives it
/// again; no, and the Knight tells the party to forget the Hover Jade.
void ShamanVillage_TalkToEastKnight(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanKnightRepeatsRulesBeforeTrial);
  // "Do you need to hear Moapa's explanation again?" Yes: "Thick-witted
  // outsiders." No: "Then you must leave Shaman Village and forget..."
  Event_OpenMessage(ACTOR_EAST_KNIGHT, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 246, 292);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 204, 292);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 182, 280);
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_EAST);
    // "Thick-witted outsiders."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN);
    ShamanVillage_ExplainTrialRules();
  } else {
    gFieldState->message++;
    // "Then you must leave Shaman Village and forget about the Hover Jade."
    Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  }
  Event_End();
}

/// Talking to the west Knight at the foot of Trial Road: "Do you need to hear
/// Moapa's explanation again?" Yes, and Felix steps up to Moapa, who gives it
/// again; no, and the Knight says his people are not free while the party
/// stays.
void ShamanVillage_TalkToWestKnight(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanOtherKnightRepeatsRulesBeforeTrial);
  // "Do you need to hear Moapa's explanation again?" Yes: "Thick-witted
  // outsiders." No: "As long as you remain in our village, my..."
  Event_OpenMessage(ACTOR_WEST_KNIGHT, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 182, 280);
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_EAST);
    // "Thick-witted outsiders."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN);
    ShamanVillage_ExplainTrialRules();
  } else {
    gFieldState->message++;
    // "As long as you remain in our village, my people are not free to do..."
    Event_ShowMessage(ACTOR_WEST_KNIGHT, 0);
  }
  Event_End();
}

/// Talking to Moapa at his door: "What's wrong? Nothing to say? Then what are
/// you standing around for? Get out of town!" The menu then opens, as if he
/// were waiting to be shown something.
void ShamanVillage_TellOutsidersToLeave(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanOutsidersLeaveTown);
  // "What's wrong? Nothing to say? Then what are you standing around for?..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  state->fieldMenu = 1;
  Event_End();
}

/// Talking to the east Knight at Moapa's door before Moapa is beaten: "If you
/// anger Moapa, you'll be in for a little taste of his awesome power", and the
/// menu opens.
void ShamanVillage_WarnOfMoapasPower(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanFearMoapaPower);
  // "If you anger Moapa, you'll be in for a little taste of his awesome..."
  Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  state->fieldMenu = 1;
  Event_End();
}

/// Talking to the west Knight at Moapa's door before Moapa is beaten: "If
/// there's something you want to say, say it to Moapa, not to us", and the
/// menu opens.
void ShamanVillage_SendToMoapa(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanSpeakToMoapa);
  // "If there's something you want to say, say it to Moapa, not to us. We..."
  Event_ShowMessage(ACTOR_WEST_KNIGHT, 0);
  state->fieldMenu = 1;
  Event_End();
}

/// Whether Felix faces the counter, north: the innkeeper, priest and
/// storekeeper serve only across it.
s32 ShamanVillage_IsFacingCounter(void) {
  u16 turn;
  s32 facing;

  turn =
      ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId))->facing +
      (DEGtoANG(135) - 1);
  facing = 1;
  if (turn >= DEGtoANG(90) - 1)
    facing = 0;
  return facing;
}

u32 Inn_QuoteRoomPrice(s32 shop);
void Inn_PlaySleep(s32 roomPrice);

/// The general store, as Shop_Run numbers the shops.
enum { SHOP_SHAMAN_VILLAGE = 29 };

/// The Shaman Village's inn, as Inn_CheckIn numbers the inns.
enum { SHAMAN_VILLAGE_INN = 11 };

/// Felix talks to the innkeeper across the inn's counter. Before Moapa is
/// beaten the innkeeper says nothing, and a card on the counter reads "Put ...
/// coins on the counter, and you can stay." Paying, the party sleeps; without
/// the coins, the card says to leave. After, the innkeeper serves as any
/// inn's does. From beside the counter he says only "... ...", or after Moapa
/// is beaten asks whether they have traveled far.
void ShamanVillage_TalkToInnkeeper(void) {
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    if (ShamanVillage_IsFacingCounter()) {
      Inn_CheckIn(SHAMAN_VILLAGE_INN, ACTOR_INNKEEPER);
    } else {
      Event_Start();
      Event_EndLastingPsynergy(0);
      Event_SetMessageId((s32)MsgShamanVisitorsHaveTraveledFar);
      Event_AskYesNo(ACTOR_INNKEEPER, 0);
      Event_End();
    }
  } else {
    Event_Start();
    Event_EndLastingPsynergy(0);
    if (ShamanVillage_IsFacingCounter()) {
      u32 price = Inn_QuoteRoomPrice(SHAMAN_VILLAGE_INN);

      Actor_SetAnimation(ACTOR_INNKEEPER, 5);
      Event_Wait(30);
      Event_SetMessageId((s32)MsgShamanInnPaymentPrompt);
      MessageArgument_Push(price, MESSAGE_ARGUMENT_NUMBER);
      MessageArgument_Push(price, MESSAGE_ARGUMENT_NUMBER);
      // "The card reads "Put ... coins on the counter, and you can stay."..."
      Event_OpenMessage(ACTOR_INN_COUNTER, 0);
      if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                           0) == 0) {
        if ((u32)gMainRoutineState.coins < price) {
          Actor_SetAnimation(ACTOR_INNKEEPER, 6);
          Event_Wait(20);
          Actor_SetAnimation(ACTOR_INNKEEPER, 5);
          Event_Wait(30);
          Event_SetMessageId((s32)MsgShamanInnPaymentPrompt + 3);
          // "There's something written on the card: "If you don't have any..."
          Event_ShowMessage(ACTOR_INN_COUNTER, 0);
        } else {
          Inn_PlaySleep(price);
        }
      }
      Actor_SetAnimation(ACTOR_INNKEEPER, 6);
    } else {
      Event_SetMessageId((s32)MsgShamanSilentVillager);
      // "... ..."
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    }
    Event_End();
  }
}

/// Felix talks to the priest across the counter of the sanctum in the cave,
/// and the sanctum serves the party, as it does before Moapa is beaten too.
/// From beside the counter the priest says that nobody in town will talk to
/// them, by a law of the Shaman tribe, or once Moapa is beaten: "You beat
/// Moapa? Incredible!"
void ShamanVillage_TalkToPriest(void) {
  if (ShamanVillage_IsFacingCounter()) {
    Sanctum_Run(ACTOR_PRIEST);
  } else {
    Event_Start();
    Event_EndLastingPsynergy(0);
    if (Event_GetFlag(FLAG_MOAPA_BEATEN))
      Event_SetMessageId((s32)MsgShamanMoapaDefeatIncredible);
    else
      Event_SetMessageId((s32)MsgShamanTribeForbidsTalking);
    // "Nothing personal, but nobody in town will talk to you. It may seem..."
    Event_ShowMessage(ACTOR_PRIEST, 0);
    Event_End();
  }
}

/// Talking to the villager by the mountain: has the party looked over its
/// other side, for the spring only Hoabna could find?
void ShamanVillage_AskAboutOtherSide(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanCheckOtherMountainSide);
  Event_AskYesNo(ACTOR_MOUNTAIN_SIDE_SHAMAN, 0);
  Event_End();
}

/// Talking to Moapa at home after the battle: he asks the party's names once,
/// "Felix, Jenna, Sheba, and Piers. I shall remember those names", and after
/// that greets Felix as the hero of Contigo.
void ShamanVillage_TalkToMoapa(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES)) {
    Event_SetMessageId((s32)MsgShamanMoapaNeverForgetsVictor);
    // "Felix, the hero of Contigo! I shall never forget you, the only one..."
    Event_ShowMessage(ACTOR_MOAPA_AT_HOME, 0);
  } else {
    Event_SetMessageId((s32)MsgShamanMoapaAsksVictorsNames);
    // "You have defeated me, and yet I still don't know your names... Would..."
    Event_OpenMessage(ACTOR_MOAPA_AT_HOME, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        1) {
      // "A warrior who does not share his name is a humble warrior indeed."
      Event_ShowMessage(ACTOR_MOAPA_AT_HOME, 0);
    } else {
      gFieldState->message++;
      // "Felix, Jenna, Sheba, and Piers. I shall remember those names. We..."
      Event_ShowMessage(ACTOR_MOAPA_AT_HOME, 0);
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES);
    }
  }
  Event_End();
}

/// Felix talks to the storekeeper across the counter in the general store's
/// tent, and the shop opens. From beside the counter: "Everyone is depressed
/// that Moapa lost his big fight. They should be glad the hero came!"
void ShamanVillage_TalkToGeneralStorekeeper(void) {
  if (ShamanVillage_IsFacingCounter()) {
    Shop_Run(SHOP_SHAMAN_VILLAGE, ACTOR_GENERAL_STOREKEEPER);
  } else {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_SetMessageId((s32)MsgShamanWelcomeContigoHero);
    // "Everyone is depressed that Moapa lost his big fight. They should be..."
    Event_ShowMessage(ACTOR_GENERAL_STOREKEEPER, 0);
    Event_End();
  }
}

/// The fortune-teller hops with a splash, and the fire before her flares.
void ShamanVillage_HopFortuneTeller(void) {
  Actor_Stop(ACTOR_FORTUNE_TELLER);
  wait(1);
  Actor_Jump(ACTOR_FORTUNE_TELLER, 4, 20);
  Event_PlaySound(SE_SPLASH);
  wait(2);
  Actor_RunScriptAndWait(ACTOR_FORTUNE_TELLERS_FLAME,
                         gShamanVillage_HopFortuneTellerScript);
}

/// Talking to the fortune-teller at the fire before her tent in the
/// north-east: "So, you'd like to hear your fortune, would you?" Yes, and she
/// chants "Hoolabaloo! Ballabahoo! Hoolabaloola!", hopping by the fire, and
/// warns the party not to go south; once Jupiter Lighthouse is lit she chants
/// otherwise and warns against north by northeast. She tells it once a visit;
/// after, she only repeats the warning.
void ShamanVillage_TellFortune(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD)) {
    if (Event_GetFlag(FLAG_JUPITER_BEACON_LIT))
      Event_SetMessageId((s32)MsgShamanFortuneAvoidNortheast);
    else
      Event_SetMessageId((s32)MsgShamanFortuneAvoidSouth);
    // "You should absolutely not go south. Do you hear me? I'm serious..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  } else {
    Event_PlaySound(MUS_SILENCE);
    if (Event_GetFlag(FLAG_JUPITER_BEACON_LIT))
      Event_SetMessageId((s32)MsgShamanFortuneOfferAfterTrial);
    else
      Event_SetMessageId((s32)MsgShamanFortuneOfferBeforeTrial);
    // "So, you'd like to hear your fortune, would you?"
    Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        1) {
      // "That's probably good. The things I see are not for everyone to hear."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    } else {
      gFieldState->message++;
      // "Well, you caught me in a good mood... I'll do it!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      Event_PlaySound(MUS_RITUALS);
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5);
      // "Hoolabaloo!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      ShamanVillage_HopFortuneTeller();
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 6);
      // "Ballabahoo!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      ShamanVillage_HopFortuneTeller();
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5);
      // "Hoolabaloola!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      ShamanVillage_HopFortuneTeller();
      Event_Wait(20);
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 1);
      Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 3);
      // "I can see it clearly... Your life is thick with danger..."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      Actor_SetAttachedEffect(ACTOR_FORTUNE_TELLER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(40);
      // "Ooo... Wow, that was a strong one... Listen, don't go south. Bad..."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      Actor_RunRepeatedMotion(ACTOR_FORTUNE_TELLER, 1);
      // "Oh, it's terrible! Awful beyond words! No more, no more!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD);
    }
    Scene_PlayMusic();
  }
  Event_End();
}

/// Felix first walks up to Moapa at the foot of Trial Road, and Moapa
/// explains it, the camera looking up the road: the legend of Yegelos and
/// Hoabna, who raced to the summit and fought there, and the trial's rules
/// (only the rules when asked again). Then the Knights leave up the road and
/// the screen fades.
void ShamanVillage_ExplainTrialRules(void) {
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN)) {
    Event_SetMessageId((s32)MsgShamanTrialRulesAreSimple);
  } else {
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    Event_Start();
    Event_EndLastingPsynergy(0);
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.2);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 182, 280);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
    Event_PlaySound(MUS_TRIAL_ROAD);
    Actor_FaceDirection(ACTOR_MOAPA, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_WEST, 0);
    Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
    Event_SetMessageId((s32)MsgShamanListenCarefully);
    // "Listen carefully to me."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
    FieldCamera_SetSpeed(3.2);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 182, 1);
    FieldCamera_WaitForArrival();
    // "This cave provides you with an opportunity to practice your skills."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Actor_SetAttachedEffect(ACTOR_MOAPA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "Legends say that Yegelos and Hoabna once fought here. These columns..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
    // "They raced one another to the summit..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    FieldCamera_MoveTo(178, CAMERA_KEEP, 220, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(10);
    FieldCamera_MoveTo(270, CAMERA_KEEP, 220, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    // "And there, they fought with all their might on the peak of the..."
    Event_ShowMessage(ACTOR_MOAPA, 0);
    FieldCamera_MoveTo(226, CAMERA_KEEP, 280, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
    // "In honor of their great battle, the leaders of Shaman created this..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
  // "The rules are simple."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 3);
  Actor_SetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
  Actor_SetSpeed(ACTOR_WEST_KNIGHT, 1.6);
  Actor_SetSpeed(ACTOR_EAST_KNIGHT, 1.6);
  Actor_WalkTo(ACTOR_WEST_KNIGHT, 184, 256);
  Actor_WalkToAndWait(ACTOR_EAST_KNIGHT, 264, 256);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTH, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 182, 1);
  Actor_WalkTo(ACTOR_WEST_KNIGHT, 184, 128);
  Actor_WalkToAndWait(ACTOR_EAST_KNIGHT, 264, 128);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 1);
  wait(1);
  Actor_SetPosition(ACTOR_WEST_KNIGHT, 0, 0);
  Actor_SetPosition(ACTOR_EAST_KNIGHT, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD);
  Event_SetFlag(FLAG_KEEP_MUSIC_HELD);
  Import_EventSetExitRequest(
      SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_D);
}

/// Felix walks back to Moapa at the foot of Trial Road with the challenge
/// accepted: "Have you decided to give up this foolishness once and for
/// all?" Yes, and Felix walks away and the challenge is off; no, and he is
/// told to choose a road.
void ShamanVillage_OfferToGiveUp(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(182, CAMERA_KEEP, 304, 1);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgShamanGiveUpTrialPrompt);
  // "Have you decided to give up this foolishness once and for all?" Yes: "You
  // have chosen wisely, and for your wisdom..." No: "Choose the road of your
  // liking. It matters..."
  Event_OpenMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    // "You have chosen wisely, and for your wisdom, you must leave Shaman..."
    Event_ShowMessage(ACTOR_MOAPA, 0);
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 368, 1);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 168, 368);
    Event_ClearFlag(FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED);
  } else {
    gFieldState->message++;
    // "Choose the road of your liking. It matters not to us."
    Event_ShowMessage(ACTOR_MOAPA, 0);
    FieldCamera_SetSpeed(1.2);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 280, 1);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 168, 280);
  }
  Event_End();
}

/// Felix walks up to Moapa at the foot of Trial Road once the rules are
/// heard: "Do you want to challenge me on Trial Road?" Yes, and Moapa offers
/// his explanation again; declined, he checks the party has the Shaman's Rod
/// and the equipment the trial needs. No, and Felix steps back.
void ShamanVillage_ChallengeMoapa(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 182, 280);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
  Event_SetMessageId((s32)MsgShamanChallengeTrialRoadPrompt);
  // "Do you want to challenge me on Trial Road?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      1) {
    // "You have chosen wisely, and for your wisdom, you must leave Shaman..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    gFieldState->message++;
    // "Do you need to hear Moapa's explanation again?"
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        0) {
      Event_PlaySound(MUS_TRIAL_ROAD);
      // "Thick-witted outsiders."
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN);
      ShamanVillage_ExplainTrialRules();
    } else {
      gFieldState->message++;
      ShamanVillage_CheckTrialEntry();
    }
  }
  Event_End();
}

extern u32 gShamanVillage_WalkToLeftStartScript[];
extern u32 gShamanVillage_WalkToRightStartScript[];

/// Felix steps onto the left path up Trial Road (cells (10, 13) to (12, 13)):
/// the Knights go to Moapa, who takes the right path, "On the count of
/// three, the challenge begins!", the countdown, and the race starts.
void ShamanVillage_TakeLeftPath(void) {
  struct Actor *owner;
  struct Actor *shaman;
  struct Actor *target;

  owner = ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId));
  shaman = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_Stop(ACTOR_WEST_KNIGHT);
  Actor_Stop(ACTOR_EAST_KNIGHT);
  Actor_Stop(ACTOR_MOAPA);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_WEST_KNIGHT, 3);
  Actor_SetSpeed(ACTOR_WEST_KNIGHT, 1.6);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 2);
  target = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  if (target != NULL)
    Actor_SetDestination(ACTOR_WEST_KNIGHT, target->x.part[0].integer,
                         target->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_WEST_KNIGHT);
  Actor_SetPosition(ACTOR_WEST_KNIGHT, 0, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 3);
  Event_SetMessageId((s32)MsgShamanKnightsTakeOtherPath);
  // "If you have chosen, then we shall take the other path."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceActor(gMainRoutineState.leaderActorId, ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
  Actor_SetAnimation(ACTOR_EAST_KNIGHT, 2);
  target = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  if (target != NULL)
    Actor_SetDestination(ACTOR_EAST_KNIGHT, target->x.part[0].integer,
                         target->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_EAST_KNIGHT);
  Actor_SetPosition(ACTOR_EAST_KNIGHT, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  // "On the count of three, the challenge begins!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 200, 1);
  owner->sceneByte = 0;
  shaman->sceneByte = 0;
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gShamanVillage_WalkToLeftStartScript);
  Actor_SetScript(ACTOR_MOAPA, gShamanVillage_WalkToRightStartScript);
  do {
    wait(1);
  } while (owner->sceneByte == 0 || shaman->sceneByte == 0);
  ShamanVillage_CountDownToStart();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 168, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
  Actor_SetSpeed(ACTOR_MOAPA, 1.6);
  Actor_WalkTo(gMainRoutineState.leaderActorId, 184, 128);
  Actor_WalkToAndWait(ACTOR_MOAPA, 264, 128);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  wait(1);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 0, 0);
  Actor_SetPosition(ACTOR_MOAPA, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  InventorySnapshot_Save();
  Import_EventSetExitRequest(
      SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_E);
}

/// Felix steps onto the right path up Trial Road (cells (15, 13) to
/// (17, 13)): the Knights go to Moapa, who takes the left path, "On the count
/// of three, the challenge begins!", the countdown, and the race starts.
void ShamanVillage_TakeRightPath(void) {
  struct Actor *owner;
  struct Actor *shaman;
  struct Actor *target;

  owner = ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId));
  shaman = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_Stop(ACTOR_WEST_KNIGHT);
  Actor_Stop(ACTOR_EAST_KNIGHT);
  Actor_Stop(ACTOR_MOAPA);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
  Actor_SetSpeed(ACTOR_EAST_KNIGHT, 1.6);
  Actor_SetAnimation(ACTOR_EAST_KNIGHT, 2);
  target = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  if (target != NULL)
    Actor_SetDestination(ACTOR_EAST_KNIGHT, target->x.part[0].integer,
                         target->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_EAST_KNIGHT);
  Actor_SetPosition(ACTOR_EAST_KNIGHT, 0, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 3);
  Event_SetMessageId((s32)MsgShamanKnightsTakeOtherPath);
  // "If you have chosen, then we shall take the other path."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceActor(gMainRoutineState.leaderActorId, ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  Actor_SetAnimationAndWait(ACTOR_WEST_KNIGHT, 3);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 2);
  target = ((struct Actor *)Actor_GetById(ACTOR_MOAPA));
  if (target != NULL)
    Actor_SetDestination(ACTOR_WEST_KNIGHT, target->x.part[0].integer,
                         target->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_WEST_KNIGHT);
  Actor_SetPosition(ACTOR_WEST_KNIGHT, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  // "On the count of three, the challenge begins!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 200, 1);
  owner->sceneByte = 0;
  shaman->sceneByte = 0;
  Actor_SetScript(ACTOR_MOAPA, gShamanVillage_WalkToLeftStartScript);
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gShamanVillage_WalkToRightStartScript);
  do {
    wait(1);
  } while (owner->sceneByte == 0 || shaman->sceneByte == 0);
  ShamanVillage_CountDownToStart();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 168, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
  Actor_SetSpeed(ACTOR_MOAPA, 1.6);
  Actor_WalkTo(ACTOR_MOAPA, 184, 128);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 264, 128);
  Actor_SetAnimation(ACTOR_MOAPA, 1);
  wait(1);
  Actor_SetPosition(ACTOR_MOAPA, 0, 0);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  InventorySnapshot_Save();
  Import_EventSetExitRequest(
      SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_F);
}

/// The gap west of the pillar by the fortune-teller's tent, cell (32, 11),
/// which the pillar fills, and the ground beside it whose cell it then
/// copies.
#define PILLAR_BRIDGE_CELL_X 32
#define PILLAR_BRIDGE_CELL_Z 11
#define PILLAR_BRIDGE_ATTRIBUTES_CELL_X 30

/// Felix moves the stone pillar on the ledge west of the fortune-teller's
/// tent into the gap beside it (cell (32, 11)): it drops in with a thump and
/// fills the gap, so the party can walk across.
void ShamanVillage_DropPillar(void) {
  struct Actor *actor =
      ((struct Actor *)Actor_GetById(ACTOR_AROMA_ROOM_PILLAR));

  if (FIELD_CELL(actor->x.fixed) == PILLAR_BRIDGE_CELL_X &&
      FIELD_CELL(actor->z.fixed) == PILLAR_BRIDGE_CELL_Z) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    do {
      wait(1);
    } while (actor->velocityY != 0);
    Event_PlaySound(SE_THUMP);
    wait(10);
    actor->motionFlags = 0;
    MapGrid_CopyCellAttributes(PILLAR_BRIDGE_ATTRIBUTES_CELL_X,
                               PILLAR_BRIDGE_CELL_Z, 1, 1, PILLAR_BRIDGE_CELL_X,
                               PILLAR_BRIDGE_CELL_Z);
    wait(1);
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED);
    Event_End();
  }
}

/// Felix walks up to (or presses A at) Moapa's door, under the horned skull
/// in the north-west of the village, before meeting him: the door opens,
/// Felix hops back, and Moapa and his two Knights come out. "This is my house.
/// If you're looking for Moapa, here I am. Back up. Give me some room." Felix
/// backs down the steps; strangers are not welcome, and "If there's something
/// you want to say, hurry up and say it."
void ShamanVillage_MeetMoapa(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  Event_Wait(20);
  Actor_SetPosition(ACTOR_MOAPA, 120, 194);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 2.4);
  Actor_Jump(gMainRoutineState.leaderActorId, 4, 0);
  ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId))
      ->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 120, 220);
  Event_Wait(1);
  {
    u8 *flags =
        &((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId))
             ->steeringFlags;

    *flags |= ACTOR_FACES_MOTION;
  }
  Actor_SetSpeed(ACTOR_MOAPA, 0.4);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 208);
  Actor_PlaceOnActor(ACTOR_WEST_KNIGHT, ACTOR_MOAPA);
  Actor_PlaceOnActor(ACTOR_EAST_KNIGHT, ACTOR_MOAPA);
  wait(1);
  Actor_SetSpeed(ACTOR_WEST_KNIGHT, 0.8);
  Actor_SetSpeed(ACTOR_EAST_KNIGHT, 0.8);
  Actor_WalkTo(ACTOR_WEST_KNIGHT, 104, 212);
  Actor_WalkToAndWait(ACTOR_EAST_KNIGHT, 136, 212);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 1);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
  Actor_StartRepeatedMotion(ACTOR_MOAPA, 1);
  Event_SetMessageId((s32)MsgShamanMoapaGiveMeRoom);
  // "This is my house. If you're looking for Moapa, here I am. Back up...."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldCamera_SetSpeed(0.8);
  ((struct Actor *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_MoveTo(120, 50, 224, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 120, 256);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.4);
  ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId))
      ->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 120, 264);
  Event_Wait(1);
  ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId))
      ->steeringFlags |= ACTOR_FACES_MOTION;
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "Who have we here? Strangers in Shaman Village? What is your purpose..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOAPA, 0);
  Actor_StartRepeatedMotion(ACTOR_EAST_KNIGHT, 1);
  FieldActor_ShowEmote(ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "Shaman Village does not appreciate intruders."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_KNIGHT, 0);
  Actor_StartRepeatedMotion(ACTOR_WEST_KNIGHT, 1);
  FieldActor_ShowEmote(ACTOR_WEST_KNIGHT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "You're not welcome here, outlander. Leave now, and never come back."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WEST_KNIGHT, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "If there's something you want to say, hurry up and say it. I'll..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOAPA, 0);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_MET);
  Event_End();
}

#include "shared/emote_pop_tilted.c"

// The Shaman Village's events after Moapa's house: the rod, the trial's rules
// and paths, the race's lettering, and each place's set-up and tables.

/// The Shaman's Rod's icon, held up over Felix's head.
struct Actor *ShamanVillage_CreateRodIcon(void) {
  struct Actor *actor = ((struct Actor *)Import_ActorCreate(
      SPRITE_UNSEEN_MARKER, toFIXED(120), toFIXED(64), toFIXED(264)));
  IconTransfer *transfer = NULL;

  if (actor == NULL)
    return NULL;
  {
    struct FieldSprite *sprite = actor->sprite;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    actor->motionFlags = 0;
    actor->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
    sprite->flags = 0;
    sprite->count = 0;
    transfer = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
    Icon_DrawItem(ITEM_SHAMANS_ROD);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
    HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  }
  return actor;
}

extern u32 gShamanVillage_JennaStandsByRodScript[];
extern u32 gShamanVillage_EastKnightLeavesHouseScript[];

/// Felix shows Moapa the Shaman's Rod at his door, holding it up: Moapa knows
/// it but will not give up the Hover Jade to the party, and at a
/// Knight's word sets them a test at the foot of Trial Road. He leaves his
/// door open.
void ShamanVillage_ShowShamansRod(void) {
  struct Actor *rod;
  struct Actor *actor;
  s32 angle;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  angle = FACING_NORTH;
  ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId))->facing =
      angle;
  wait(1);
  FieldCamera_SetSpeed(0.8);
  ((struct Actor *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_MoveTo(120, 40, 246, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.2);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 120, 264);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  rod = ShamanVillage_CreateRodIcon();
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetSpeed(ACTOR_MOAPA, 2.4);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 242);
  Event_PlaySound(MUS_TUNDARIA_TOWER);
  Event_SetMessageId((s32)MsgShamanRecognizeShamanRod);
  // "That's the Shaman's Rod!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Event_Wait(20);
  if (rod != NULL)
    Actor_Release(rod);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, angle);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineState.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetScript(ACTOR_KRADEN, gShamanVillage_KradenStandsByRodScript);
  Actor_SetScript(ACTOR_PIERS, gShamanVillage_PiersStandsByRodScript);
  Actor_SetScript(ACTOR_JENNA, gShamanVillage_JennaStandsByRodScript);
  Actor_SetScript(ACTOR_SHEBA, gShamanVillage_ShebaStandsByRodScript);
  FieldActor_ShowEmote(ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       0);
  Actor_SetSpeed(ACTOR_EAST_KNIGHT, 1.2);
  Actor_WalkToAndWait(ACTOR_EAST_KNIGHT, 136, 232);
  // "We thought it was lost forever... It once belonged to the great Hoabna."
  Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  Actor_SetSpeed(ACTOR_WEST_KNIGHT, 1.2);
  Actor_WalkToAndWait(ACTOR_WEST_KNIGHT, 104, 232);
  // "Hoabna's staff was a gift from his great friend in Contigo, Yegelos...."
  Event_ShowMessage(ACTOR_WEST_KNIGHT, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "He vowed to care for the sacred treasure of Contigo, the Hover..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Have you outlanders come to claim the Hover Jade?"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "I suppose we have, yes."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_MOAPA, 0.8);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 252);
  Event_Wait(40);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_WalkToAndWait(ACTOR_MOAPA, 136, 252);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_WalkToAndWait(ACTOR_MOAPA, 104, 252);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_WalkToAndWait(ACTOR_MOAPA, 152, 252);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_WalkToAndWait(ACTOR_MOAPA, 88, 252);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 40);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 252);
  Actor_WalkToAndWait(ACTOR_MOAPA, 120, 242);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_MOAPA, 4);
  // "That's a shame. We'll never give the Hover Jade to you!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_Jump(ACTOR_KRADEN, 4, 0);
  // "But...we brought you the Shaman's Rod... What's stopping you from..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Hoabna handed the Hover Jade down from generation to generation for..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 4);
  // "We are to return it to the descendants of Yegelos. And you are..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // "That's preposterous! We returned your rod! We've come so far already!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTHWEST, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "What difference does it make if we're from Contigo or not!?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Make no mistake about it. We are the intended recipients of the..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTHEAST, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Is there some way we can get our hands on the Hover Jade?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "No. You ask the impossible."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                       20);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
  // "What about...the test? Would that be a bad idea, Moapa?"
  Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 20);
  Actor_SetAnimation(ACTOR_EAST_KNIGHT, 3);
  // "You know, Trial Road? Only Yegelos could walk that path."
  Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  {
    FIXED y = toFIXED(80);
    FIXED z = toFIXED(242);

    Emote_PopTilted(toFIXED(116), y, z, DEGtoANG(315));
    Event_Wait(10);
    Emote_PopTilted(toFIXED(124), y, z, DEGtoANG(45));
    Event_Wait(30);
    FieldActor_ShowEmote(ACTOR_WEST_KNIGHT, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                         20);
    Actor_FaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_EAST);
    // "The way of the vanishing sand!"
    Event_ShowMessage(ACTOR_WEST_KNIGHT, 0);
    Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
    FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
    Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
    // "Yes, if you outlanders intend to claim the Hover Jade, you must..."
    Event_ShowMessage(ACTOR_MOAPA, 0);
    Actor_SetAnimation(ACTOR_MOAPA, 3);
    // "If you people have the courage, follow me!"
    Event_ShowMessage(ACTOR_MOAPA, 0);
    FieldCamera_SetSpeed(0.4);
    FieldCamera_MoveToFX(toFIXED(180), toFIXED(32), z, 1);
  }
  actor = ((struct Actor *)Actor_GetById(ACTOR_WEST_KNIGHT));
  actor->sceneByte = 0;
  Actor_WatchAndSetScript(gMainRoutineState.leaderActorId, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_MOAPA);
  Actor_SetSpeed(ACTOR_MOAPA, 1.6);
  Actor_SetSpeed(ACTOR_WEST_KNIGHT, 1.6);
  Actor_SetSpeed(ACTOR_EAST_KNIGHT, 1.6);
  Actor_SetScript(ACTOR_MOAPA, gShamanVillage_MoapaLeavesHouseScript);
  Event_Wait(20);
  Actor_SetScript(ACTOR_EAST_KNIGHT,
                  gShamanVillage_EastKnightLeavesHouseScript);
  Actor_SetScript(ACTOR_WEST_KNIGHT,
                  gShamanVillage_WestKnightLeavesHouseScript);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  do {
    wait(1);
  } while (actor->sceneByte == 0);
  Actor_Stop(gMainRoutineState.leaderActorId);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  wait(1);
  Scene_PlayMusic();
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(120, CAMERA_KEEP, 260, 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(ACTOR_MOAPA, 0, 0);
  Actor_SetPosition(ACTOR_EAST_KNIGHT, 0, 0);
  Actor_SetPosition(ACTOR_WEST_KNIGHT, 0, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  // "If you'd told us there was a test to begin with, we'd have been out..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
  // "What kind of test is this?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  // "I understand that you're a little concerned, but unless you try it..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  // "Give it your best, Felix!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_EAST);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_SetScript(ACTOR_SHEBA, gShamanVillage_ReturnToLeaderAtHouseScript);
  Actor_SetScript(ACTOR_JENNA, gShamanVillage_ReturnToLeaderAtHouseScript);
  Actor_SetScript(ACTOR_PIERS, gShamanVillage_ReturnToLeaderAtHouseScript);
  Actor_RunScriptAndWait(ACTOR_KRADEN,
                         gShamanVillage_ReturnToLeaderAtHouseScript);
  Event_Wait(20);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_MOAPAS_DOOR_OPEN);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN);
  Event_End();
}

/// Felix shows Moapa any other item: "If you're here offering charity, I
/// won't have it... Or did you want to show me something?"
void ShamanVillage_ShowItemToMoapa(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanShowMeSomething);
  // "If you're here offering charity, I won't have it... Or did you want..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Event_End();
}

/// An item shown to the west Knight: "Did you want to sell it?"
void ShamanVillage_ShowItemToWestKnight(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanCannotBuyGift);
  // "Yes, yes. That's very nice. Did you want to sell it? Because I don't..."
  Event_ShowMessage(ACTOR_WEST_KNIGHT, 0);
  Event_End();
}

/// An item shown to the east Knight: "Show it to Moapa if you must"
void ShamanVillage_ShowItemToEastKnight(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanShowItToMoapa);
  // "What makes you think I would be interested in this? Show it to Moapa..."
  Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  Event_End();
}

/// Felix walks up to the fire before the fortune-teller's tent in the
/// north-east (cell (37, 19)) once Moapa is beaten: she calls out
/// "Hoolabaloo!" and hops, once a visit.
void ShamanVillage_FortuneTellerCallsOut(void) {
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    Event_PlaySound(MUS_SILENCE);
    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_SetMessageId((s32)MsgShamanFortuneGreeting);
    Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5);
    // "Hoolabaloo!"
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 20);
    ShamanVillage_HopFortuneTeller();
    Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 1);
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_FORTUNE_TELLER_CALLED_OUT);
    Event_End();
  }
}

/// Felix casts Growth on the sprout north-west of the inn (cell (2, 18)), and
/// it grows into a vine he can climb, up to the Spirit Gloves' chest.
void ShamanVillage_GrowVine(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 2, 17);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_VINE_GROWN);
}

/// Felix walks back from Moapa, and the challenge is off.
void ShamanVillage_StepBackFromTrial(void) {
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
  FieldCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 168, 304);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_SOUTH, 0);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 168, 368);
  Event_ClearFlag(FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED);
}

/// The pieces of equipment the trial's chests take from a party last to every
/// door: "you will lose eight items to the chests!"
#define TRIAL_ROAD_CHEST_ITEMS 8

/// Moapa takes the challenge only from a party with the Shaman's Rod and
/// TRIAL_ROAD_CHEST_ITEMS weapons, armor, shields or helms among them.
void ShamanVillage_CheckTrialEntry(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u8 remaining;
  u16 *items;
  s16 *owner;

  Event_SetMessageId((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindMember(ITEM_SHAMANS_ROD) == -1) {
    FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // "But what's this? You no longer have the Shaman's Rod? We cannot..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_StepBackFromTrial();
    return;
  }
  count = FieldParty_ListMembers(owners);
  owner = owners;
  remaining = TRIAL_ROAD_CHEST_ITEMS;
  for (i = 0; i < count; i++) {
    // slot holds the owner's id until the item loop reuses it.
    slot = *owner++;
    items = Unit_Get(slot)->inventory;
    for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
      s32 item = items[slot];
      if (item == 0)
        break;
      if ((u8)(Item_GetDirect(item)->type - ITEM_TYPE_WEAPON) <=
          ITEM_TYPE_HELM - ITEM_TYPE_WEAPON)
        remaining--;
      if (remaining == 0)
        break;
    }
    if (remaining == 0)
      break;
  }
  gFieldState->message++;
  if (remaining != 0) {
    FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // "It looks as though you are missing the item you need if you are to..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    gFieldState->message++;
    // "Choose the road of your liking. It matters not to us."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    Actor_WatchAndSetScript(ACTOR_WEST_KNIGHT, gMainRoutineState.leaderActorId);
    Actor_WatchAndSetScript(ACTOR_EAST_KNIGHT, gMainRoutineState.leaderActorId);
    Actor_WatchAndSetScript(ACTOR_MOAPA, gMainRoutineState.leaderActorId);
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD);
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED);
  }
}

/// The lettering's task: run its script, move, scale and fade it, and draw it
/// as double-size affine sprites blended over the screen, in the shape of
/// gShamanVillage_LetteringMode.
void ShamanVillage_UpdateLettering(void) {
  u32 *write = gShamanVillage_LetteringSprites;
  // Each lettering piece is queued as a sprite part written as three words:
  // the queue link, attributes 0 and 1, and attribute 2.
  struct AnimationSpritePart *sprite = (struct AnimationSpritePart *)write;
  s32 tile =
      gVramCacheEntries[gShamanVillage_LetteringVramEntry[0]].offset >> 5;
  u32 flags;
  s32 blend, scale, speed, centerX;
  s32 matrix, i, x, y, left;
  struct AffineTransform state;

commands:
  if (gShamanVillage_LetteringWait == 0) {
    switch (*gShamanVillage_LetteringCursor++) {
    case LETTERING_PLACE:
      gShamanVillage_LetteringX = *gShamanVillage_LetteringCursor++ << 8;
      gShamanVillage_LetteringSpeed = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrames = 0;
      break;
    case LETTERING_ACCELERATE:
      gShamanVillage_LetteringSpeedFrom = gShamanVillage_LetteringSpeed;
      gShamanVillage_LetteringSpeed = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringSpeedFrame = 0;
      break;
    case LETTERING_SCALE:
      gShamanVillage_LetteringScaleFrom = gShamanVillage_LetteringScale;
      gShamanVillage_LetteringScale = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringScaleFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringScaleFrame = 0;
      break;
    case LETTERING_FADE:
      gShamanVillage_LetteringBlendFrom = gShamanVillage_LetteringBlend;
      gShamanVillage_LetteringBlend = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringBlendFrames = *gShamanVillage_LetteringCursor++;
      gShamanVillage_LetteringBlendFrame = 0;
      break;
    case LETTERING_WAIT:
      gShamanVillage_LetteringWait = *gShamanVillage_LetteringCursor++;
      break;
    case LETTERING_END:
      Task_Remove(ShamanVillage_UpdateLettering);
      VramCache_FreeEntry(gShamanVillage_LetteringVramEntry[0]);
      return;
    }
    goto commands;
  }
  {
    s32 duration;
    gShamanVillage_LetteringWait--;
    duration = gShamanVillage_LetteringScaleFrames;
    if (duration == 0) {
      scale = gShamanVillage_LetteringScale;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringScaleFrom;
      target = gShamanVillage_LetteringScale;
      progress = ++gShamanVillage_LetteringScaleFrame;
      scale = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringScaleFrames = 0;
    }
  }
  {
    s32 duration;
    duration = gShamanVillage_LetteringBlendFrames;
    if (duration == 0) {
      blend = gShamanVillage_LetteringBlend;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringBlendFrom;
      target = gShamanVillage_LetteringBlend;
      progress = ++gShamanVillage_LetteringBlendFrame;
      blend = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringBlendFrames = 0;
    }
  }
  {
    s32 duration;
    duration = gShamanVillage_LetteringSpeedFrames;
    if (duration == 0) {
      speed = gShamanVillage_LetteringSpeed;
    } else {
      s32 start, progress, target;
      start = gShamanVillage_LetteringSpeedFrom;
      target = gShamanVillage_LetteringSpeed;
      progress = ++gShamanVillage_LetteringSpeedFrame;
      speed = start + (target - start) * progress / duration;
      if (progress >= duration)
        gShamanVillage_LetteringSpeedFrames = 0;
    }
    state.angle = 0;
    state.scaleX = scale;
    state.scaleY = scale;
    matrix = (s16)OamMatrix_AllocFromTransform(&state);
  }
  gShamanVillage_LetteringX += speed;
  centerX = gShamanVillage_LetteringX / LETTERING_SUBPIXELS;
  switch (gShamanVillage_LetteringMode) {
  case LETTERING_SHAPE_TITLE: {
    for (i = 0; i < 4; i++) {
      y = 56;
      flags = OBJ_SIZE_32X16;
      x = centerX + scale * LETTERING_PIECE_X(i, 4) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & OBJ_X_MASK;
        *write++ = 0;
        *write++ = OBJ_X(x) | y | flags | OBJ_AFFINE_MATRIX(matrix) |
                   LETTERING_OBJ_MODE;
        *write++ = LETTERING_ATTRIBUTES | tile;
        Oam_AddSpriteAtDepth(sprite++, LETTERING_DEPTH);
      }
      tile += 8;
    }
    break;
  }
  case LETTERING_SHAPE_RESULT: {
    for (i = 0; i < 2; i++) {
      y = 48;
      flags = OBJ_SIZE_32X16;
      x = centerX + scale * LETTERING_PIECE_X(i, 2) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & OBJ_X_MASK;
        *write++ = 0;
        *write++ = OBJ_X(x) | y | flags | OBJ_AFFINE_MATRIX(matrix) |
                   LETTERING_OBJ_MODE;
        *write++ =
            LETTERING_ATTRIBUTES | (tile + gShamanVillage_LetteringTileOffset);
        Oam_AddSpriteAtDepth(sprite++, LETTERING_DEPTH);
      }
      tile += 8;
    }
    break;
  }
#if LANGUAGE == LANGUAGE_FRENCH
  case LETTERING_SHAPE_GO: {
    for (i = 0; i < 2; i++) {
      y = 64;
      flags = OBJ_SIZE_32X16;
      x = centerX + scale * LETTERING_PIECE_X(i, 2) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & OBJ_X_MASK;
        *write++ = 0;
        *write++ = OBJ_X(x) | y | flags | OBJ_AFFINE_MATRIX(matrix) |
                   LETTERING_OBJ_MODE;
        *write++ =
            LETTERING_ATTRIBUTES | (tile + gShamanVillage_LetteringTileOffset);
        Oam_AddSpriteAtDepth(sprite++, LETTERING_DEPTH);
      }
      tile += 8;
    }
    break;
  }
#else
  case LETTERING_SHAPE_GO:
    y = 48;
    flags = OBJ_SIZE_64X32;
    left = centerX + 56;
    if ((u32)(centerX + 120) < 304) {
      x = left & OBJ_X_MASK;
      *write++ = 0;
      *write++ =
          OBJ_X(x) | y | flags | OBJ_AFFINE_MATRIX(matrix) | LETTERING_OBJ_MODE;
      *write++ =
          LETTERING_ATTRIBUTES | (tile + gShamanVillage_LetteringTileOffset);
      Oam_AddSpriteAtDepth(sprite, LETTERING_DEPTH);
    }
    break;
#endif
  case LETTERING_SHAPE_DIGIT:
    y = 48;
    flags = OBJ_SIZE_32X32;
    left = centerX + 88;
    if ((u32)(centerX + 152) < 304) {
      x = left & OBJ_X_MASK;
      *write++ = 0;
      *write++ =
          OBJ_X(x) | y | flags | OBJ_AFFINE_MATRIX(matrix) | LETTERING_OBJ_MODE;
      *write++ =
          LETTERING_ATTRIBUTES | (tile + gShamanVillage_LetteringTileOffset);
      Oam_AddSpriteAtDepth(sprite, LETTERING_DEPTH);
    }
    break;
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      ((BLEND_WEIGHT_MAX - blend) << 8) | blend,
                      IO_WRITE_HALFWORD);
}

/// Starts the lettering task on the picture last loaded, laid out in shape
/// (enum LetteringShape) and moved by that shape's script; secondHalf picks the
/// second half of the picture: 2 rather than 3, or LOSE rather than WIN.
void ShamanVillage_StartLettering(s32 shape, s32 secondHalf) {
  s32 script;

  gShamanVillage_LetteringMode = shape;
  gShamanVillage_LetteringTileOffset = secondHalf << 4;

  Task_Add(ShamanVillage_UpdateLettering, TASK_PASS_FRAME);

  script = (s32)gShamanVillage_LetteringTitleScript;
  if (shape == LETTERING_SHAPE_DIGIT) {
    script = (s32)gShamanVillage_LetteringDigitScript;
  } else if (shape == LETTERING_SHAPE_GO) {
    script = (s32)gShamanVillage_LetteringGoScript;
  } else if (shape == LETTERING_SHAPE_RESULT) {
    if (secondHalf != 0) {
      script = (s32)gShamanVillage_LetteringLoseScript;
    } else {
      script = (s32)gShamanVillage_LetteringWinScript;
    }
  }

  gShamanVillage_LetteringWait = 0;
  gShamanVillage_LetteringCursor = (s16 *)script;
  gShamanVillage_LetteringSpeed = 0;
  gShamanVillage_LetteringSpeedFrames = 0;
  gShamanVillage_LetteringX = 0;
}

/// The European editions' lettering sprite sheet has one more palette row.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_PALETTE_ROWS 6
#else
#define LETTERING_PALETTE_ROWS 5
#endif

/// The decoded lettering sprite sheet: its palette rows, then eight pictures of
/// 32 tiles each.
struct LetteringSheet {
  u8 palettes[LETTERING_PALETTE_ROWS][32];
  u8 pictures[8][1024];
};

/// Loads lettering picture into the lettering's VRAM block (taken once) and its
/// palette into sprite palette LETTERING_PALETTE.
void ShamanVillage_LoadLettering(s32 picture) {
  // BUG: this holds seven pictures of the sprite sheet's eight, so decoding the
  // sprite sheet writes 1024 bytes past the block.
#ifdef BUGFIX
  struct LetteringSheet *sheet = Heap_AllocEwram(sizeof(struct LetteringSheet));
#else
  struct LetteringSheet *sheet =
      Heap_AllocEwram((224 + LETTERING_PALETTE_ROWS) * 32);
#endif
  struct DmaRegisters volatile *dma;
  s32 palette;

  if (gShamanVillage_LetteringVramEntry[0] == -1)
    gShamanVillage_LetteringVramEntry[0] = VramCache_FindFreeEntry();
  palette = gShamanVillage_LetteringPaletteOffsets[picture];
  if (picture == LETTERING_PICTURE_WIN_LOSE_FOR_LOSS)
    picture = LETTERING_PICTURE_WIN_LOSE;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LETTERING_BANNER), sheet);
  Dma_Set(sheet->palettes[0] + palette,
          (void *)&OBJ_PLTT_COLOR(LETTERING_PALETTE, 0), DMA_COPY_WORDS(8),
          REG_DMA3);
  Vram_Load(gShamanVillage_LetteringVramEntry[0], sizeof(sheet->pictures[0]),
            sheet->pictures[picture]);
  dma = (struct DmaRegisters volatile *)REG_DMA3;
  while ((s32)dma->control < 0)
    ;
  Heap_Release(sheet);
}

/// The count of 3, 2, 1 and `GO` before the race up Trial Road, after which
/// the race music starts.
void ShamanVillage_CountDownToStart(void) {
  // The title's hold in the middle, as Colosso's third match sets it; the
  // village never shows the title.
  gShamanVillage_LetteringTitleScript[LETTERING_TITLE_HOLD] =
      3 * FRAMES_PER_SECOND;
  Event_Wait(30);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  ShamanVillage_LoadLettering(LETTERING_PICTURE_COUNTDOWN_START);
  ShamanVillage_StartLettering(LETTERING_SHAPE_DIGIT, 0);
  Event_PlaySound(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  ShamanVillage_StartLettering(LETTERING_SHAPE_DIGIT, 1);
  Event_PlaySound(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  ShamanVillage_LoadLettering(LETTERING_PICTURE_COUNTDOWN_END);
  ShamanVillage_StartLettering(LETTERING_SHAPE_DIGIT, 0);
  Event_PlaySound(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  ShamanVillage_LoadLettering(LETTERING_PICTURE_GO);
  ShamanVillage_StartLettering(LETTERING_SHAPE_GO, 0);
  Event_PlaySound(SE_COLOSSO_GO);
  Event_PlaySound(MUS_TRIAL_ROAD);
}

/// Moapa's first test: Sheba steps up and casts Whirlwind on the purple rock,
/// and the sand is gone.
void ShamanVillage_ShebaClearsSand(void) {
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct Actor *actor = ((struct Actor *)Actor_GetById(ACTOR_WEST_KNIGHT));

  s32 result;
  s32 owner;
  s32 angle;
  u8 *done;

  effect->flashCaster = 0;
  effect->announceName = 0;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_PlaySound(MUS_SILENCE);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 528, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 168, 536);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineState.leaderActorId);
  wait(1);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1);
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gShamanVillage_LeaderWatchesShebaScript);
  Actor_SetScript(ACTOR_JENNA, gShamanVillage_JennaWatchesShebaScript);
  Actor_SetScript(ACTOR_KRADEN, gShamanVillage_KradenWatchesShebaScript);
  Actor_RunScriptAndWait(ACTOR_PIERS, gShamanVillage_PiersWatchesShebaScript);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
  Event_SetMessageId((s32)MsgShamanCheerForCompanion);
  // "You can do it, Sheba!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "Leave it to me, Jenna. This will be a breeze."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_SHEBA, 0.4);
  Actor_WalkToAndWait(ACTOR_SHEBA, 168, 524);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_NORTH, 0);
  FieldCamera_SetSpeed(1.6);
  ShamanVillage_CastWhirlwindOnSand();
  Event_PlaySound(MUS_TUNDARIA_TOWER);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 540, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_MOAPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "I don't believe it. They did it! The sand vanished!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAttachedEffect(ACTOR_EAST_KNIGHT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "The legends... They were all true!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_KNIGHT, 0);
  Actor_SetAttachedEffect(ACTOR_WEST_KNIGHT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "She's the one! She must have come from Contigo!"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_WEST_KNIGHT, 0,
                                 20);
  Actor_RunRepeatedMotion(ACTOR_MOAPA, 1);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  // "Her?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
  // "She's the one that did it? But she's...just a girl!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_Jump(ACTOR_SHEBA, 4, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "Well, you wanted the sand to vanish, so..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  // "Can we have the Hover Jade now?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_MOAPA, 4);
  // "Nope, you still can't have it."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
  // "What? Why not?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  // "This was just a test to see if you earned the right to take the test..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "So, there's more that they have to do before you'll give them the stone?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
  // "You will have to reach the end of Trial Road."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       0);
  FieldActor_ShowEmote(ACTOR_WEST_KNIGHT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       0);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(ACTOR_EAST_KNIGHT, 1);
  // "The girls, too? Surely you can't make them..."
  Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  Actor_StartRepeatedMotion(ACTOR_WEST_KNIGHT, 1);
  // "I thought that only the chosen hero could travel the road..."
  Event_ShowMessage(ACTOR_WEST_KNIGHT, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
  // "What, are you saying I can't be the chosen hero?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAttachedEffect(ACTOR_MOAPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "The heroes have always been men, ever since the time of Hoabna. It..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_Jump(ACTOR_SHEBA, 2, 10);
  Actor_Jump(ACTOR_SHEBA, 4, 10);
  // "I don't like your attitude, mister! I can be every bit as heroic as..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_NORTHWEST);
  Actor_SetAnimation(ACTOR_EAST_KNIGHT, 4);
  // "That may be true, but you must respect our customs."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_KNIGHT, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 4);
  // "It's the same in Contigo, you know. If you don't like it, take it up..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WEST_KNIGHT, 0);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 3);
  Actor_SetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 560, 1);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_SOUTH);
  Actor_SetSpeed(ACTOR_EAST_KNIGHT, 1);
  Actor_SetSpeed(ACTOR_WEST_KNIGHT, 1);
  Actor_WalkTo(ACTOR_WEST_KNIGHT, 120, 560);
  Actor_WalkToAndWait(ACTOR_EAST_KNIGHT, 216, 560);
  Actor_SetAnimation(ACTOR_EAST_KNIGHT, 1);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_EAST);
  Actor_WalkTo(ACTOR_WEST_KNIGHT, 152, 560);
  Actor_WalkToAndWait(ACTOR_EAST_KNIGHT, 184, 560);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 1);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
  Actor_SetSpeed(ACTOR_MOAPA, 1);
  Actor_SetAnimation(ACTOR_MOAPA, 2);
  Actor_SetAnimation(ACTOR_EAST_KNIGHT, 2);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 2);
  Actor_SetDestinationOffset(ACTOR_MOAPA, 0, 16);
  Actor_SetDestinationOffset(ACTOR_EAST_KNIGHT, 0, 16);
  Actor_SetDestinationOffset(ACTOR_WEST_KNIGHT, 0, 16);
  Actor_WaitForMoveById(ACTOR_MOAPA);
  Actor_SetAnimation(ACTOR_MOAPA, 1);
  Actor_SetAnimation(ACTOR_EAST_KNIGHT, 1);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 1);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  // "Hold it, hold it... Moapa, what were you just saying about Trial Road?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_NORTHWEST, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "If we reach the end of Trial Road, won't our names join the ranks of..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_WEST_KNIGHT, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "Kraden's right... It shouldn't matter if a girl does it... All that..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  // "If you give us the chance, we will prove it to you."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "Unless you're afraid that a girl like me might become your town's..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 4);
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
  // "Have they earned the right? Shall we let them onto Trial Road?"
  Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  Actor_FaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST);
  // "I don't like the way these outsiders were talking to me."
  Event_ShowMessage(ACTOR_WEST_KNIGHT, 0);
  Actor_SetAttachedEffect(ACTOR_MOAPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
  Actor_Jump(ACTOR_EAST_KNIGHT, 4, 10);
  // "I think they need to learn exactly what it takes to be a hero around..."
  Event_ShowMessage(ACTOR_EAST_KNIGHT, 0);
  Actor_FaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_EAST);
  Actor_SetAnimationAndWait(ACTOR_WEST_KNIGHT, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST);
  // "Yeah, if these guys are so stubborn, let's give them a shot! They'll..."
  Event_ShowMessage(ACTOR_WEST_KNIGHT, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  // "You said it yourselves. We're too stubborn to quit."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "Yeah. If you want us "outlanders" to leave, you'd better give us a..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAttachedEffect(ACTOR_MOAPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_MOAPA, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_WEST_KNIGHT, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "Trial Road is a difficult one... When you reach the top, a terrible..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 4);
  // "It's difficult enough for me. I expect you will find it quite..."
  // Yes: "You seem confident, but it is your ignorance speaking."
  // No: "So... You are not as sure of yourselves as you first seemed."
  Event_OpenMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  result = Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  done = &actor->sceneByte;
  owner = gMainRoutineState.leaderActorId;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 4);
    // "You seem confident, but it is your ignorance speaking."
    Event_ShowMessage(ACTOR_MOAPA, 0);
  } else {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationAndWait(ACTOR_MOAPA, 4);
    gFieldState->message++;
    // "So... You are not as sure of yourselves as you first seemed."
    Event_ShowMessage(ACTOR_MOAPA, 0);
    Actor_SetAnimation(ACTOR_MOAPA, 3);
    // "But I will permit this. Follow me, and I shall explain."
    Event_ShowMessage(ACTOR_MOAPA, 0);
  }
  Actor_WalkToAndWait(ACTOR_MOAPA, 212, 582);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 500, 1);
  Actor_WatchAndSetScript(gMainRoutineState.leaderActorId, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_MOAPA);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_MOAPA);
  *done = 0;
  Actor_SetScript(ACTOR_MOAPA, gShamanVillage_HeadForTrialRoadScript);
  Event_Wait(20);
  Actor_SetScript(ACTOR_EAST_KNIGHT, gShamanVillage_HeadForTrialRoadScript);
  Actor_SetScript(ACTOR_WEST_KNIGHT, gShamanVillage_HeadForTrialRoadScript);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  do {
    wait(1);
  } while (*done == 0);
  Actor_Stop(gMainRoutineState.leaderActorId);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_SHEBA);
  Scene_PlayMusic();
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 540, 1);
  Event_Wait(20);
  angle = FACING_SOUTH;
  Actor_SetPositionAndFacing(ACTOR_MOAPA, toFIXED(224), toFIXED(280), angle);
  Actor_SetPositionAndFacing(ACTOR_WEST_KNIGHT, toFIXED(208), toFIXED(264),
                             angle);
  Actor_SetPositionAndFacing(ACTOR_EAST_KNIGHT, toFIXED(240), toFIXED(264),
                             angle);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_Stop(gMainRoutineState.leaderActorId);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_SHEBA);
  ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId))->update =
      NULL;
  ((struct Actor *)Actor_GetById(ACTOR_KRADEN))->update = NULL;
  ((struct Actor *)Actor_GetById(ACTOR_JENNA))->update = NULL;
  ((struct Actor *)Actor_GetById(ACTOR_PIERS))->update = NULL;
  ((struct Actor *)Actor_GetById(ACTOR_SHEBA))->update = NULL;
  wait(1);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetScript(ACTOR_PIERS, gShamanVillage_ReturnToLeaderAtSandScript);
  Actor_SetScript(ACTOR_SHEBA, gShamanVillage_ReturnToLeaderAtSandScript);
  Actor_SetScript(ACTOR_JENNA, gShamanVillage_ReturnToLeaderAtSandScript);
  Actor_RunScriptAndWait(ACTOR_KRADEN,
                         gShamanVillage_ReturnToLeaderAtSandScript);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED);
  Event_End();
}

/// Sheba casts Whirlwind on the twister rock and blows the sand away.
void ShamanVillage_CastWhirlwindOnSand(void) {
  Actor_SetSpritePriority(ACTOR_SHEBA, 1);
  Event_PlaySound(SE_PSYNERGY_SPARKLE);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_SHEBA),
                                   CASTER_LOOK_CYCLING_COLORS);
  Event_Wait(40);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_SHEBA), CASTER_LOOK_NORMAL);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  ShamanVillage_BlowAwaySand();
  Actor_SetSpritePriority(ACTOR_SHEBA, 2);
}

/// Felix walks into a house's door: it opens (its open picture copied, width
/// by height cells from sourceX, sourceY to destinationX, destinationY), with
/// a thump for a door two cells wide and SE_DOOR_OPEN for one; through a door
/// one cell wide Felix first steps to the middle of its cell and faces north.
/// He walks in, the screen fades, and the party leaves by the exit whose
/// number the cell he stood on holds.
void ShamanVillage_EnterDoor(s32 sourceX, s32 sourceY, s32 destinationX,
                             s32 destinationY, s32 width, s32 height) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor;
  s32 cell;

  Event_Start();
  Event_EndLastingPsynergy(0);
  if (width == 2)
    Event_PlaySound(SE_THUMP);
  else
    Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(sourceX, sourceY, destinationX, destinationY, width,
                      height);
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  if (width == 1) {
    actor = ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId));
    cell = FIXEDtoINT(actor->x.fixed);
    Actor_SetMoveTarget(
        actor,
        toFIXED(MAPGRID_CELL_PIXELS / 2 - cell % MAPGRID_CELL_PIXELS) +
            actor->x.fixed,
        actor->y.fixed, actor->z.fixed);
    FieldActor_WaitForMove(actor);
    actor->facing = FACING_NORTH;
  }
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gLeader_WalkThroughDoorScript);
  wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(state->stepTrigger);
}

/// Felix walks north into the inn's door, beside the INN sign in the south-west
/// of the village (cell (8, 26)), and goes in to the innkeeper's counter.
void ShamanVillage_EnterInn(void) {
  ShamanVillage_EnterDoor(43, 40, 8, 22, 1, 2);
}

/// Felix walks north into the door of the house in the far south-west of the
/// village (cell (4, 28)) and goes in.
void ShamanVillage_EnterSouthwestHouse(void) {
  ShamanVillage_EnterDoor(43, 42, 4, 26, 1, 2);
}

/// Once Moapa is beaten, Felix walks north into the door of the lower of the
/// two houses on the village's east side (cell (27, 25)) and goes in to a
/// room with beds. Before, the door is locked.
void ShamanVillage_EnterLowerEastHouse(void) {
  ShamanVillage_EnterDoor(43, 42, 27, 21, 1, 2);
}

/// Once Moapa is beaten, Felix walks north into the door of the upper of the
/// two houses on the village's east side (cell (28, 23)) and goes in; its
/// stairs lead up and out to the north-east, on the way to Aroma. Before, the
/// door is locked.
void ShamanVillage_EnterUpperEastHouse(void) {
  ShamanVillage_EnterDoor(43, 40, 28, 17, 1, 2);
}

/// Felix walks north into the door just east of Moapa's, by the river (cell
/// (10, 12)), and goes in to a room with beds.
void ShamanVillage_EnterRiversideHouse(void) {
  ShamanVillage_EnterDoor(43, 42, 10, 8, 1, 2);
}

/// Felix walks north into Moapa's door, under the horned skull, once he has
/// met Moapa: left open since the party showed the Shaman's Rod, it is walked
/// through at once; until then it opens as the other doors do.
void ShamanVillage_EnterMoapasHouse(void) {
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_MOAPAS_DOOR_OPEN)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_PlaySound(SE_MAP_EXIT);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(
        SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTHWEST);
  } else {
    ShamanVillage_EnterDoor(43, 40, 7, 6, 1, 2);
  }
}

/// Coming back down to the foot of Trial Road during the trial, Moapa offers
/// it again.
void ShamanVillage_RestartTrial(void) {
  s32 angle;

  Event_ClearFlag(FLAG_KEEP_MUSIC_HELD);
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetPosition(ACTOR_WEST_KNIGHT, 0, 0);
  Actor_SetPosition(ACTOR_EAST_KNIGHT, 0, 0);
  ((struct Actor *)Actor_GetById(ACTOR_MOAPA))->facing = FACING_NORTH;
  FieldCamera_MoveTo(224, CAMERA_KEEP, 182, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  angle = FACING_SOUTH;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPositionAndFacing(ACTOR_WEST_KNIGHT, toFIXED(184), toFIXED(128),
                             angle);
  Actor_SetPositionAndFacing(ACTOR_EAST_KNIGHT, toFIXED(264), toFIXED(128),
                             angle);
  Actor_SetSpeed(ACTOR_WEST_KNIGHT, 1.6);
  Actor_SetSpeed(ACTOR_EAST_KNIGHT, 1.6);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 1);
  Actor_WalkTo(ACTOR_WEST_KNIGHT, 184, 256);
  Actor_WalkToAndWait(ACTOR_EAST_KNIGHT, 264, 256);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 0);
  Actor_WalkTo(ACTOR_WEST_KNIGHT, 208, 264);
  Actor_WalkToAndWait(ACTOR_EAST_KNIGHT, 240, 264);
  Actor_SetAnimation(ACTOR_WEST_KNIGHT, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WEST_KNIGHT, angle, 0);
  Actor_FaceDirection(ACTOR_EAST_KNIGHT, angle, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgShamanMustFinishTrialRoad);
  // "In order to reach the battle at the summit, you cannot fail on Trial..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 3);
  // "Do you understand?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      1) {
    // "Would you like me to explain it to you again?"
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        0) {
      Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
      Event_PlaySound(MUS_TRIAL_ROAD);
      // "Do all outlanders share your thickwittedness?"
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN);
      ShamanVillage_ExplainTrialRules();
      return;
    }
    gFieldState->message++;
  } else {
    gFieldState->message += 2;
  }
  // "Then you understand the treacherous path of Trial Road. Will you..."
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      1) {
    // "You have chosen wisely, and for your wisdom, you must leave Shaman..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    gFieldState->message++;
    // "You have heard the rules, and you wish to continue. You have been..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
    ShamanVillage_CheckTrialEntry();
  }
  Scene_PlayMusic();
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Coming back down to the foot of Trial Road by one way from the road, Moapa
/// says it would be wise of the party to quit, and Felix steps back.
void ShamanVillage_SuggestQuitting(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgShamanMoapaAdvisesLeaving);
  ((struct Actor *)Actor_GetById(ACTOR_MOAPA))->facing = FACING_WEST;
  ((struct Actor *)Actor_GetById(ACTOR_WEST_KNIGHT))->facing = FACING_WEST;
  ((struct Actor *)Actor_GetById(ACTOR_EAST_KNIGHT))->facing = FACING_WEST;
  FieldCamera_MoveTo(224, CAMERA_KEEP, 280, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  // "It would be wise of you to quit. Even if you reach the top, you..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
  ShamanVillage_StepBackFromTrial();
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Sets up the place being entered. Returns 0.
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
    ShamanVillage_SetUpTrialGrounds();
  else if (scene == SCENE_SHAMAN_VILLAGE_NORTH_END)
    ShamanVillage_SetUpNorthEnd();
  else if (scene == SCENE_SHAMAN_VILLAGE)
    ShamanVillage_SetUpVillage();
  else if (scene == SCENE_SHAMAN_VILLAGE_HOUSES)
    ShamanVillage_SetUpHouses();
  else if (scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
    ShamanVillage_SetUpSanctum();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The foot of Trial Road: the sand gone, or Moapa and his Knights waiting
/// by the twister rock.
void ShamanVillage_SetUpTrialGrounds(void) {
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED)) {
    MapGrid_CopyCellsTo(47, 19, 38, 19, 9, 11);
    MapGrid_CopyCellAttributes(1, 5, 9, 1, 6, 29);
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(ACTOR_TWISTER_ROCK)), SPRITE_COLOR_GREY);
  } else if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Actor_SetPositionAndFacing(ACTOR_MOAPA, toFIXED(168), toFIXED(568),
                               FACING_SOUTH);
    Actor_SetPositionAndFacing(ACTOR_EAST_KNIGHT, toFIXED(216), toFIXED(536),
                               FACING_SOUTH);
    Actor_SetPositionAndFacing(ACTOR_WEST_KNIGHT, toFIXED(120), toFIXED(536),
                               FACING_SOUTH);
    wait(1);
  }
  Actor_SetAnimation(ACTOR_FIRST_TORCH, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 1, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 2, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 3, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 4, 2);
  Actor_SetAnimation(ACTOR_LAST_TORCH, 2);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 entrance = gMainRoutineState.entrance;

    if (entrance == SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_E)
      ShamanVillage_RestartTrial();
    else if (entrance == SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_C)
      ShamanVillage_SuggestQuitting();
  }
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    Actor_SetPosition(ACTOR_MOAPA, 0, 0);
    Actor_SetPosition(ACTOR_EAST_KNIGHT, 0, 0);
    Actor_SetPosition(ACTOR_WEST_KNIGHT, 0, 0);
    MapGrid_CopyCellAttributes(10, 12, 3, 1, 10, 13);
    MapGrid_CopyCellAttributes(15, 12, 3, 1, 15, 13);
    MapGrid_CopyCellAttributes(9, 20, 3, 1, 9, 21);
  }
}

/// The north-east of the village and the gate of Trial Road: the torches lit,
/// the pillar in the gap once Move has dropped it, and the fortune-teller and
/// her tent's entrance there only once Moapa is beaten. Until the Shaman's Rod
/// is shown, a wooden gate with a notice on it stands between the torches at
/// the foot of Trial Road; once it is shown the gate is gone and the way
/// north between the torches is open (the three cells from (12, 6) take those
/// of the row above them).
void ShamanVillage_SetUpNorthEnd(void) {
  struct Actor *actor;

  Actor_SetAnimation(ACTOR_FIRST_TORCH, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 1, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 2, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 3, 2);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 4, 2);
  Actor_SetAnimation(ACTOR_LAST_TORCH, 2);
  ((struct Actor *)Actor_GetById(ACTOR_AROMA_ROOM_PILLAR))->motionFlags = 0;
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED)) {
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_AROMA_ROOM_PILLAR)),
        FIELD_CELL_CENTER(PILLAR_BRIDGE_CELL_X), toFIXED(32),
        FIELD_CELL_CENTER(PILLAR_BRIDGE_CELL_Z));
    wait(1);
    MapGrid_CopyCellAttributes(PILLAR_BRIDGE_ATTRIBUTES_CELL_X,
                               PILLAR_BRIDGE_CELL_Z, 1, 1, PILLAR_BRIDGE_CELL_X,
                               PILLAR_BRIDGE_CELL_Z);
    wait(10);
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN)) {
    MapGrid_CopyCellsTo(64, 0, 76, 2, 3, 3);
    MapGrid_CopyCellAttributes(12, 5, 3, 1, 12, 6);
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_DOORS_OPEN))
    MapGrid_CopyCellsTo(48, 40, 101, 11, 1, 2);
  else
    Actor_SetPosition(ACTOR_FORTUNE_TELLER, 0, 0);
  actor = ((struct Actor *)Actor_GetById(ACTOR_FORTUNE_TELLERS_FLAME));
  actor->collisionFlags = ACTOR_NOT_TARGETED;
  actor->scaleX = FIXED_ONE * 8 / 10;
  actor->scaleY = FIXED_ONE * 8 / 10;
  Actor_SetSpriteFlags(
      ((struct Actor *)Actor_GetById(ACTOR_FORTUNE_TELLERS_FLAME)), 0);
}

#include "shared/smoke_drift.h"

/// Puffs a see-through white cloud of chimney smoke at x, y, z, half its size,
/// as Smoke_Spawn does but drawn under the actors around it; Smoke_Drift moves
/// it for SMOKE_FRAMES.
void ShamanVillage_SpawnSmoke(s32 x, s32 y, s32 z) {
  struct Actor *actor =
      ((struct Actor *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z));

  if (actor != NULL) {
    actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
    actor->motionFlags = 0;
    actor->timer = 0;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteFlags(actor, 0);
    actor->scaleX = FIXED_ONE / 2;
    actor->scaleY = FIXED_ONE / 2;
    // Its frames left, which Smoke_Drift counts down.
    *(s32 *)&actor->watchedActor = SMOKE_FRAMES;
    actor->update = Smoke_Drift;
    Import_ActorSelectAnimation(actor, 5);
    Actor_SetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
  }
}

/// Every 64th frame, a puff of smoke from each of the village's two chimneys.
void ShamanVillage_BlowChimneySmoke(void) {
  if ((gFrameCount & 63) == 0) {
    ShamanVillage_SpawnSmoke(toFIXED(72), toFIXED(66), toFIXED(376));
    ShamanVillage_SpawnSmoke(toFIXED(104), toFIXED(98), toFIXED(152));
  }
}

/// Defines a task in which two actors take the leader's sprite priority each
/// frame, so they pass behind what he passes behind.
#define DEFINE_LEADER_LAYER_TASK(name, first, second)                          \
  void name(void) {                                                            \
    struct FieldSprite *sprite =                                               \
        Actor_GetById(gMainRoutineState.leaderActorId)->sprite;                \
    s32 variant = sprite->priority;                                            \
                                                                               \
    Actor_SetSpritePriority(first, variant);                                   \
    Actor_SetSpritePriority(second, variant);                                  \
  }

/// A task: the sprout and the girl below the cave take the leader's sprite
/// priority each frame, so they pass behind what he passes behind.
DEFINE_LEADER_LAYER_TASK(ShamanVillage_LayerVillageActors, ACTOR_SPROUT,
                         ACTOR_GIRL_BELOW_THE_CAVE)

/// The village: Moapa and his Knights at his door once met, until the
/// Shaman's Rod is shown, and the Knights alone there once he is beaten; his
/// door left open; the tents' entrances open once he is beaten; the rope, the
/// vine, and smoke from the two chimneys.
void ShamanVillage_SetUpVillage(void) {
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
      Actor_SetPosition(ACTOR_WEST_KNIGHT, 104, 200);
      Actor_SetPosition(ACTOR_EAST_KNIGHT, 136, 200);
    } else if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_MET) &&
               !Event_GetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN)) {
      Actor_SetPosition(ACTOR_MOAPA, 120, 208);
      Actor_SetPosition(ACTOR_WEST_KNIGHT, 104, 212);
      Actor_SetPosition(ACTOR_EAST_KNIGHT, 136, 212);
      wait(1);
    }
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_MOAPAS_DOOR_OPEN))
    MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_DOORS_OPEN)) {
    s32 width = 1;
    s32 height = 2;

    MapGrid_CopyCellsTo(48, 40, 77, 21, width, height);
    MapGrid_CopyCellsTo(48, 40, 81, 25, width, height);
  }
  Rope_Initialize();
  Rope_Place(0, FLAG_SHAMAN_VILLAGE_ROPE_TIED, ACTOR_VILLAGE_ROPE_POST,
             ACTOR_CLIFF_ROPE_POST);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_VINE_GROWN))
    ShamanVillage_GrowVine();
  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  FieldTask_AddFrame(ShamanVillage_LayerVillageActors, TASK_PASS_FRAME);
  FieldTask_AddFrame(ShamanVillage_BlowChimneySmoke, TASK_PASS_FRAME);
}

/// The houses: Moapa and his household as the story leaves them. Brought back
/// from the battle with Moapa, the party wakes in a bed with its HP and PP
/// restored and Felix gets up and walks on; the first time sets Moapa beaten.
void ShamanVillage_SetUpHouses(void) {
  u32 i;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  ((struct Actor *)Actor_GetById(ACTOR_INN_COUNTER))->collisionFlags =
      ACTOR_NOT_TARGETED;
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    Actor_SetPositionAndFacing(ACTOR_WOMAN_IN_MOAPAS_HOUSE, toFIXED(88),
                               toFIXED(92), FACING_SOUTH_SOUTHEAST);
    Actor_SetPosition(ACTOR_MOAPA_AT_HOME, 156, 72);
  } else {
    Actor_SetPosition(ACTOR_PICNIC_SHAMAN_A, 0, 0);
    Actor_SetPosition(ACTOR_PICNIC_SHAMAN_B, 0, 0);
    Actor_SetPosition(ACTOR_CHEF, 0, 0);
  }
  if ((gMainRoutineState.entrance ==
           SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_A ||
       gMainRoutineState.entrance ==
           SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_B) &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Party_RestoreHpPp(TRUE); // poison and haunting cured too
    Event_Start();
    Event_EndLastingPsynergy(0);
    Actor_SetAnimation(gMainRoutineState.leaderActorId,
                       ANIM_LEADER_LIE_ON_BACK);
    Actor_SetSpriteFlags(
        ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId)), 0);
    wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    for (i = 0; i < 60; i++) {
      wait(1);
      if (gInput.held != 0)
        break;
    }
    Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_SIT_UP);
    Event_Wait(10);
    for (i = 0; i < 30; i++) {
      wait(1);
      if (gInput.held != 0)
        break;
    }
    Actor_Jump(gMainRoutineState.leaderActorId, 4, 0);
    Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
    Actor_MoveToAndWait(gMainRoutineState.leaderActorId, 813, 120);
    Actor_SetSpriteFlags(
        ((struct Actor *)Actor_GetById(gMainRoutineState.leaderActorId)), 1);
    Event_End();
    if (gMainRoutineState.entrance ==
        SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_A) {
      Event_SetFlag(FLAG_MOAPA_BEATEN);
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_DOORS_OPEN);
    }
  }
}

/// A task: the sanctum's two shamans take the leader's sprite priority each
/// frame.
DEFINE_LEADER_LAYER_TASK(ShamanVillage_LayerSanctumActors,
                         ACTOR_SANCTUM_SHAMAN_A, ACTOR_SANCTUM_SHAMAN_B)

/// The sanctum: its two villagers there only once Moapa is beaten.
void ShamanVillage_SetUpSanctum(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (Event_GetFlag(FLAG_MOAPA_BEATEN)) {
    Task_Add(ShamanVillage_LayerSanctumActors, TASK_PASS_FRAME);
  } else {
    Actor_SetPosition(ACTOR_SANCTUM_SHAMAN_A, 0, 0);
    Actor_SetPosition(ACTOR_SANCTUM_SHAMAN_B, 0, 0);
  }
}

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// A countdown digit: it shrinks from twice its size as it fades in, holds
/// half a second and fades out.
s16 gShamanVillage_LetteringDigitScript[] = {
    LETTERING_SCALE, 512, 1,
    LETTERING_FADE, 0, 1,
    LETTERING_SCALE, 256, 6,
    LETTERING_FADE, 16, 6,
    LETTERING_WAIT, 30,
    LETTERING_FADE, 0, 30,
    LETTERING_WAIT, 30,
    LETTERING_END,
};

/// Unused: the scripts of the WIN or LOSE and the match title shapes, which
/// the village never starts.
s16 gShamanVillage_LetteringLoseScript[] = {
    LETTERING_SCALE, 128, 1,
    LETTERING_FADE, 0, 1,
    LETTERING_SCALE, 256, 6,
    LETTERING_FADE, 16, 6,
    LETTERING_WAIT, 60,
    LETTERING_SCALE, 384, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 240, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 368, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 224, 6,
    LETTERING_WAIT, 6,
    LETTERING_FADE, 0, 30,
    LETTERING_SCALE, 352, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 208, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 336, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 192, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 320, 6,
    LETTERING_WAIT, 6,
    LETTERING_END,
};

/// `GO` has a palette row of its own in the European editions' sprite sheet.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_GO_PALETTE_OFFSET (5 * 32)
#else
#define LETTERING_GO_PALETTE_OFFSET (2 * 32)
#endif

/// The byte offset of each lettering picture's palette (enum LetteringPicture)
/// among the lettering palettes.
u8 gShamanVillage_LetteringPaletteOffsets[] = {
    0, 32, 32, 32, 96, 64, 64, LETTERING_GO_PALETTE_OFFSET, 128, 0,
};
// clang-format on

// clang-format off
#include "shared/leader_walk_through_door_script.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

u32 gShamanVillage_WalkToLeftStartScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(184, 0, 216),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_SET(SW_SCENE_BYTE, 1),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_WalkToRightStartScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(264, 0, 216),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_SET(SW_SCENE_BYTE, 1),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_KradenStandsByRodScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_PiersStandsByRodScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_JennaStandsByRodScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(104, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_ShebaStandsByRodScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(88, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_MoapaLeavesHouseScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 120),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gShamanVillage_EastKnightLeavesHouseScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 120),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gShamanVillage_WestKnightLeavesHouseScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(104, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 252),
    SC_SYNC,
    SC_MOVE(252, 0, 120),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_SCENE_BYTE, 1),
    SC_END,
};

u32 gShamanVillage_ReturnToLeaderAtHouseScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 264),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_END,
};

u32 gShamanVillage_KradenWatchesShebaScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 552),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_LeaderWatchesShebaScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(184, 0, 536),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_JennaWatchesShebaScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 536),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_PiersWatchesShebaScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(200, 0, 552),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gShamanVillage_HeadForTrialRoadScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(216, 0, 564),
    SC_SYNC,
    SC_MOVE(216, 0, 470),
    SC_SYNC,
    SC_MOVE(168, 0, 434),
    SC_SYNC,
    SC_MOVE(168, 0, 380),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_SCENE_BYTE, 1),
    SC_END,
};

u32 gShamanVillage_ReturnToLeaderAtSandScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(184, 0, 536),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gShamanVillage_HopFortuneTellerScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 4),
    SC_WAIT(1),
    SC_LOOP(4, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 8),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_END,
};

/// The VRAM cache entry the lettering's picture takes; -1 for none.
s16 gShamanVillage_LetteringVramEntry[] = {
    -1,
};

/// The title's and `GO`'s scales, which fit each language's lettering.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define LETTERING_TITLE_SCALE 384
#define LETTERING_GO_START_SCALE 512
#define LETTERING_GO_SMALL_SCALE 384
#define LETTERING_GO_LARGE_SCALE 512
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_TITLE_SCALE 256
#define LETTERING_GO_START_SCALE 384
#define LETTERING_GO_SMALL_SCALE 256
#define LETTERING_GO_LARGE_SCALE 384
#else
#define LETTERING_TITLE_SCALE 256
#define LETTERING_GO_START_SCALE 512
#define LETTERING_GO_SMALL_SCALE 256
#define LETTERING_GO_LARGE_SCALE 384
#endif

/// Unused: the match title, placed and faded in, set moving and stopped, then
/// sent on after a second.
s16 gShamanVillage_LetteringTitleScript[] = {
    LETTERING_PLACE, -188, 2048,
    LETTERING_SCALE, LETTERING_TITLE_SCALE, 1,
    LETTERING_FADE, 16, 1,
    LETTERING_WAIT, 14,
    LETTERING_ACCELERATE, 0, 20,
    LETTERING_WAIT, 60,
    LETTERING_ACCELERATE, 2048, 20,
    LETTERING_WAIT, 60,
    LETTERING_END,
};

/// GO shrinks in as it fades in, then pulses between its two sizes, fading out
/// as it goes on.
s16 gShamanVillage_LetteringGoScript[] = {
    LETTERING_SCALE, LETTERING_GO_START_SCALE, 1,
    LETTERING_FADE, 0, 1,
    LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
    LETTERING_FADE, 16, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_FADE, 0, 30,
    LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
    LETTERING_WAIT, 6,
    LETTERING_END,
};
// clang-format on

void ShamanVillage_MeetAroma(void);
void Scene_ShowEntranceLocked(void);
void ShamanVillage_StopAtClosedEntrance(void);
void ShamanVillage_ReadTrialRoadSign(void);
void ShamanVillage_TalkToWestKnight(void);

// clang-format off
/// Unused: WIN grows in as it fades in, holds a second, then bounces between
/// sizes.
s16 gShamanVillage_LetteringWinScript[] ALIGNED(4) = {
    LETTERING_SCALE, 128, 1,
    LETTERING_FADE, 0, 1,
    LETTERING_SCALE, 256, 6,
    LETTERING_FADE, 16, 6,
    LETTERING_WAIT, 60,
    LETTERING_SCALE, 384, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 272, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 400, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 288, 6,
    LETTERING_WAIT, 6,
    LETTERING_FADE, 0, 30,
    LETTERING_SCALE, 416, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 304, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 432, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 320, 6,
    LETTERING_WAIT, 6,
    LETTERING_SCALE, 448, 6,
    LETTERING_WAIT, 6,
    LETTERING_END,
};

#include "shared/scene_entrances_default.c"

struct SceneEntrance gShamanVillage_TentsEntrances[] = {
    {15, CONDITION_ALWAYS, 544, 0, 680, FACING_NORTH, 0, 420, 560, 660, 720, 0},
    {16, CONDITION_ALWAYS, 752, 0, 680, FACING_NORTH, 0, 630, 560, 870, 720, 0},
    {17, CONDITION_ALWAYS, 544, 0, 904, FACING_NORTH, 0, 420, 780, 660, 940, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gShamanVillage_SanctumEntrances[] = {
    {6, CONDITION_ALWAYS, 720, 0, 544, FACING_NORTH, 0, 600, 380, 840, 600, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(ShamanVillageTrialGrounds),
    SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_A, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_WEST),
    SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_B, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_EAST),
    SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_SHAMAN_VILLAGE_NORTH_END, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS),
    SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_D, TrialRoadStages, TRIAL_ROAD_STAGES_FOR_TRIAL_RULES),
    SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_E, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_WEST),
    SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_F, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_EAST),
    SCENE_EXITS(ShamanVillageNorthEnd),
    SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_SHAMAN_VILLAGE_NORTH_END),
    SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_SOUTH, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_NORTH_END_SOUTH),
    SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_NORTHWEST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_NORTH_END_NORTHWEST),
    SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_TENTS, ShamanVillageTents, SHAMAN_VILLAGE_TENTS_FROM_SHAMAN_VILLAGE_NORTH_END),
    SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_NORTHEAST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_NORTH_END_NORTHEAST),
    SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_SOUTHEAST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_NORTH_END_SOUTHEAST),
    SCENE_EXITS(ShamanVillage),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SHAMAN_VILLAGE),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_TENTS_EAST, ShamanVillageTents, SHAMAN_VILLAGE_TENTS_FROM_SHAMAN_VILLAGE_EAST),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_TENTS_WEST, ShamanVillageTents, SHAMAN_VILLAGE_TENTS_FROM_SHAMAN_VILLAGE_WEST),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_A, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_SOUTHWEST_A),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_B, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_SOUTHWEST_B),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_EAST_A, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_EAST_A),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_EAST_B, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_EAST_B),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_NORTH_END_SOUTH, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_SOUTH),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_NORTH_END_SOUTHEAST, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_SOUTHEAST),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_SANCTUM, ShamanVillageSanctum, SHAMAN_VILLAGE_SANCTUM_FROM_SHAMAN_VILLAGE),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_NORTH_END_NORTHWEST, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_NORTHWEST),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTH, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_NORTH),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTHWEST, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_NORTHWEST),
    SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_NORTH_END_NORTHEAST, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_NORTHEAST),
    SCENE_EXITS(ShamanVillageHouses),
    // The houses' own stairs between their floors join exits and entrances 2
    // and 4, 5 and 9, 7 and 10, and 12 and 14.
    SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_NORTHWEST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_NORTHWEST),
    SCENE_EXIT(2, ShamanVillageHouses, 4),
    SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_NORTH, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_NORTH),
    SCENE_EXIT(4, ShamanVillageHouses, 2),
    SCENE_EXIT(5, ShamanVillageHouses, 9),
    SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_SOUTHWEST_A, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_A),
    SCENE_EXIT(7, ShamanVillageHouses, 10),
    SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_SOUTHWEST_B, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_B),
    SCENE_EXIT(9, ShamanVillageHouses, 5),
    SCENE_EXIT(10, ShamanVillageHouses, 7),
    SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_EAST_A, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_EAST_A),
    SCENE_EXIT(12, ShamanVillageHouses, 14),
    SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_EAST_B, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_EAST_B),
    SCENE_EXIT(14, ShamanVillageHouses, 12),
    SCENE_EXITS(ShamanVillageTents),
    SCENE_EXIT(SHAMAN_VILLAGE_TENTS_TO_SHAMAN_VILLAGE_EAST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_TENTS_EAST),
    SCENE_EXIT(SHAMAN_VILLAGE_TENTS_TO_SHAMAN_VILLAGE_WEST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_TENTS_WEST),
    SCENE_EXIT(SHAMAN_VILLAGE_TENTS_TO_SHAMAN_VILLAGE_NORTH_END, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_TENTS),
    SCENE_EXITS(ShamanVillageSanctum),
    SCENE_EXIT(SHAMAN_VILLAGE_SANCTUM_TO_SHAMAN_VILLAGE, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_SANCTUM),
    SCENE_EXITS_END,
};

struct ActorPlacement gShamanVillage_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_TrialGroundsPlacements[] = {
    {SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 224, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 264, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 264, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_NorthEndPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 120, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 120, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 336, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MASKED_SHAMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_AROMA), MOVEMENT_TYPE_STAND, 680, 0, 248, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_VillagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 428, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 248, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 364, 0, 248, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 254, 0, 434, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 434, 0, 446, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 412, 0, 230, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 40, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_HousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 90, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 110, 0, 172, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 126, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 264, 0, 381, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 381, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 140, 0, 388, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 694, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 144, 0, 658, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 164, 0, 432, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 460, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 114, 0, 364, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_TentsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 678, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 508, 0, 656, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 542, 0, 628, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 550, 0, 640, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ARTIFACT_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 752, 0, 648, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 724, 0, 628, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_SanctumPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 720, 0, 466, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 746, 0, 516, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 700, 0, 524, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gShamanVillage_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_TrialGroundsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_TWISTER_ROCK | TRIGGER_SUBJECT_QUIET,
     FLAG_SHAMAN_VILLAGE_SAND_CLEARED, EVENT(ShamanVillage_ShebaClearsSand)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND),
     ACTOR_TWISTER_ROCK | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_SHAMAN_VILLAGE_SAND_CLEARED, 0},
    {TRIGGER_STEP, BEFORE_MOAPA_CELL, FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD,
     EVENT(ShamanVillage_ExplainTrialRules)},
    {TRIGGER_STEP, BEFORE_MOAPA_CELL, FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED,
     EVENT(ShamanVillage_ChallengeMoapa)},
    {TRIGGER_STEP, BEFORE_MOAPA_CELL, CONDITION_ALWAYS,
     EVENT(ShamanVillage_OfferToGiveUp)},
    {TRIGGER_STEP, LEFT_PATH_CELL, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TakeLeftPath)},
    {TRIGGER_STEP, RIGHT_PATH_CELL, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TakeRightPath)},
    {TRIGGER_TALK, ACTOR_MOAPA, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanYegelosSandChallenge)},
    {TRIGGER_TALK, ACTOR_MOAPA, CONDITION_ALWAYS,
     MSGNO(MsgShamanChooseEitherRoad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA,
     FLAG_SHAMAN_VILLAGE_SAND_CLEARED, MSGNO(MsgShamanRodWasStolen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaCannotLose)},
    {TRIGGER_TALK, ACTOR_EAST_KNIGHT, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanTrialRoadAncientBattle)},
    {TRIGGER_TALK, ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToEastKnight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_KNIGHT,
     FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanNeedContigoOrShamanPower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaGreatestWarrior)},
    {TRIGGER_TALK, ACTOR_WEST_KNIGHT, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
     MSGNO(MsgShamanAncientHeroesVanishedSand)},
    {TRIGGER_TALK, ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToWestKnight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_KNIGHT,
     FLAG_SHAMAN_VILLAGE_SAND_CLEARED, MSGNO(MsgShamanSandChallengeWillFail)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
     MSGNO(MsgShamanJadeNeedsYegelosHeir)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_NorthEndTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     4, IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN), 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_AROMA, CONDITION_ALWAYS,
     EVENT(ShamanVillage_MeetAroma)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_AROMA_ROOM_PILLAR,
     FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED,
     EVENT(ShamanVillage_DropPillar)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CLOSED_TENT_CELL,
     FLAG_SHAMAN_VILLAGE_DOORS_OPEN, EVENT(ShamanVillage_StopAtClosedEntrance)},
    {TRIGGER_EXAMINE, TRIAL_ROAD_SIGN_CELL, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ReadTrialRoadSign)},
    {TRIGGER_STEP, FORTUNE_TELLERS_FIRE_CELL,
     FLAG_SHAMAN_VILLAGE_FORTUNE_TELLER_CALLED_OUT,
     EVENT(ShamanVillage_FortuneTellerCallsOut)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TellFortune)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
     FLAG_JUPITER_BEACON_LIT, MSGNO(MsgShamanCloudyFortuneBeforeTrial)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
     CONDITION_ALWAYS, MSGNO(MsgShamanCloudyFortuneAfterTrial)},
    {TRIGGER_TALK, 15, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanNewLegendaryBattle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanAncestorsDistrustOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanContigoHeroVictorySurprise)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_VillageTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     2, IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN), 2},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     3, IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN), 3},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_A, CONDITION_ALWAYS,
     EVENT(ShamanVillage_EnterInn)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_B, CONDITION_ALWAYS,
     EVENT(ShamanVillage_EnterSouthwestHouse)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_EAST_A,
     IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN),
     EVENT(ShamanVillage_EnterLowerEastHouse)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_EAST_B,
     IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN),
     EVENT(ShamanVillage_EnterUpperEastHouse)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTH, CONDITION_ALWAYS,
     EVENT(ShamanVillage_EnterRiversideHouse)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTHWEST,
     FLAG_SHAMAN_VILLAGE_MOAPA_MET, EVENT(ShamanVillage_MeetMoapa)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     EVENT(ShamanVillage_EnterMoapasHouse)},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_VILLAGE_ROPE_POST,
     FLAG_SHAMAN_VILLAGE_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT,
     FLAG_SHAMAN_VILLAGE_VINE_GROWN, EVENT(ShamanVillage_GrowVine)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), LOCKED_DOOR_CELL,
     FLAG_SHAMAN_VILLAGE_DOORS_OPEN, EVENT(Scene_ShowEntranceLocked)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CLOSED_TENT_CELL,
     FLAG_SHAMAN_VILLAGE_DOORS_OPEN, EVENT(ShamanVillage_StopAtClosedEntrance)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), MOAPAS_DOOR_CELL,
     FLAG_SHAMAN_VILLAGE_MOAPA_MET, EVENT(ShamanVillage_MeetMoapa)},
    {TRIGGER_TALK, 8, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaImpressedByChallenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanWaitForOutsidersToLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaPraisesOpponents)},
    {TRIGGER_TALK, 9, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanRespectForeignWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanChiefForbidsTalkingToOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgShamanDoubtMoapaDefeat)},
    {TRIGGER_TALK, 10, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanVictoryProvesSkill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanWhoInvitedOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanHopeForStrongerShamanChild)},
    {TRIGGER_TALK, 11, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanForeignersCanBeHeroes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanRefuseToTalkToOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanWorldShouldWelcomeStrangers)},
    {TRIGGER_TALK, 12, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     MSGNO(MsgShamanHoabnaPromiseFulfilled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanTrustContigoFriends)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgShamanRemainFriendsWithContigo)},
    {TRIGGER_TALK, ACTOR_MOUNTAIN_SIDE_SHAMAN, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_MOUNTAIN_SIDE_SHAMAN, CONDITION_ALWAYS,
     EVENT(ShamanVillage_AskAboutOtherSide)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOUNTAIN_SIDE_SHAMAN,
     FLAG_MOAPA_BEATEN, MSGNO(MsgShamanDistrustFriendlyOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOUNTAIN_SIDE_SHAMAN,
     CONDITION_ALWAYS, MSGNO(MsgShamanWishToBreakSandWalls)},
    {TRIGGER_TALK, ACTOR_GIRL_BELOW_THE_CAVE, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_GIRL_BELOW_THE_CAVE, CONDITION_ALWAYS,
     MSGNO(MsgShamanChosenFewHavePower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_BELOW_THE_CAVE,
     FLAG_MOAPA_BEATEN, MSGNO(MsgShamanShutDoorsAgainstOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_BELOW_THE_CAVE,
     CONDITION_ALWAYS, MSGNO(MsgShamanOutskirtsHaveDifferentPowers)},
    {TRIGGER_TALK, ACTOR_MOAPA, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TellOutsidersToLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA, CONDITION_ALWAYS,
     MSGNO(MsgShamanOutsidersAreNotContigo)},
    {TRIGGER_TALK, ACTOR_EAST_KNIGHT, FLAG_MOAPA_BEATEN,
     EVENT(ShamanVillage_WarnOfMoapasPower)},
    {TRIGGER_TALK, ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
     MSGNO(MsgShamanKnightPromisesRematchVictory)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_KNIGHT, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanContigoTempleTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
     MSGNO(MsgShamanClimbingTrialRoadIsHard)},
    {TRIGGER_TALK, ACTOR_WEST_KNIGHT, FLAG_MOAPA_BEATEN,
     EVENT(ShamanVillage_SendToMoapa)},
    {TRIGGER_TALK, ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
     MSGNO(MsgShamanVillagersBlameKnightsDefeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_KNIGHT, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanMoapaSparedOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
     MSGNO(MsgShamanKnightBlamesMountainClimb)},
    {USE_ITEM_ON_ACTOR(ITEM_SHAMANS_ROD), ACTOR_MOAPA,
     FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN,
     EVENT(ShamanVillage_ShowShamansRod)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), ACTOR_MOAPA, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ShowItemToMoapa)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ShowItemToWestKnight)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
     EVENT(ShamanVillage_ShowItemToEastKnight)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_HousesTriggers[] = {
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
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_TALK, ACTOR_WOMAN_IN_MOAPAS_HOUSE, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_WOMAN_IN_MOAPAS_HOUSE, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaWantsToKnowNames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_MOAPAS_HOUSE,
     FLAG_MOAPA_BEATEN, MSGNO(MsgShamanMoapaLeftDoorOpen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_MOAPAS_HOUSE,
     CONDITION_ALWAYS, MSGNO(MsgShamanShameNotKnowingVictors)},
    {TRIGGER_TALK, ACTOR_MOAPAS_SON, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_MOAPAS_SON, CONDITION_ALWAYS,
     MSGNO(MsgShamanFormalBattleNeedsNames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_SON, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanFatherNeverLoses)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_SON, CONDITION_ALWAYS,
     MSGNO(MsgShamanFatherLostToRudeOutsiders)},
    {TRIGGER_TALK, ACTOR_GIRL_IN_MOAPAS_HOUSE, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, ACTOR_GIRL_IN_MOAPAS_HOUSE, CONDITION_ALWAYS,
     MSGNO(MsgShamanNamesDoNotMatter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_IN_MOAPAS_HOUSE,
     FLAG_MOAPA_BEATEN, MSGNO(MsgShamanFirstForeignVisitors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_IN_MOAPAS_HOUSE,
     CONDITION_ALWAYS, MSGNO(MsgShamanNamesBeforeBattleOldFashioned)},
    {TRIGGER_TALK, ACTOR_MOAPA_AT_HOME, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToMoapa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA_AT_HOME,
     FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES,
     MSGNO(MsgShamanVictorsDeserveRespect)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA_AT_HOME,
     CONDITION_ALWAYS, MSGNO(MsgShamanVictorsNamesOnTrialColumns)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanPayingStrangersMayStay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgShamanMoapaMiserableAfterDefeat)},
    {TRIGGER_TALK, 14, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanForgiveUnkindVillagers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanMoapaAllowsStrangersToStay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanVillagersWorryTooMuch)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanFinallyOpenLockedDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgShamanQuietWhileLockedInside)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanMissedTrialRoadChallenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgShamanHidingLawPreventedWatching)},
    {TRIGGER_TALK, ACTOR_PICNIC_SHAMAN_A, CONDITION_ALWAYS,
     MSGNO(MsgShamanEnjoyGettingOutside)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PICNIC_SHAMAN_A,
     CONDITION_ALWAYS, MSGNO(MsgShamanChildrenIgnoreTradition)},
    {TRIGGER_TALK, ACTOR_PICNIC_SHAMAN_B, CONDITION_ALWAYS,
     MSGNO(MsgShamanPlanPicnicMeal)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PICNIC_SHAMAN_B,
     CONDITION_ALWAYS, MSGNO(MsgShamanEagerForDinner)},
    {TRIGGER_TALK, ACTOR_CHEF, CONDITION_ALWAYS,
     MSGNO(MsgShamanSimpleMeatPotatoesBeans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
     MSGNO(MsgShamanSaltAndPepperOnly)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_TentsTriggers[] = {
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanAncientHesperiaAttekaWar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgShamanNewContigoHeroStronger)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaYegelosDecideWar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgShamanDoubtShamanSuperiority)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanTownsChooseChampions)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanGrandparentsUpsetByDefeat)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanYegelosBraveJourney)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanGrandparentsTiredOfWarStories)},
    {TRIGGER_TALK, ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToGeneralStorekeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GENERAL_STOREKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgShamanHusbandWorriesAboutWeapons)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgShamanWarehouseOwnerFeelsResponsible)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgShamanWeaponsDidNotCauseLoss)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGER_TABLE_END},
};

struct Trigger gShamanVillage_SanctumTriggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
     EVENT(ShamanVillage_TalkToPriest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, FLAG_MOAPA_BEATEN,
     MSGNO(MsgShamanVisitorsMustFindMoapa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgShamanFearRenewedContigoWar)},
    {TRIGGER_TALK, ACTOR_SANCTUM_SHAMAN_A, CONDITION_ALWAYS,
     MSGNO(MsgShamanThreatenContigoRematch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_SHAMAN_A,
     CONDITION_ALWAYS, MSGNO(MsgShamanMoapaMustHaveBeenHungry)},
    {TRIGGER_TALK, ACTOR_SANCTUM_SHAMAN_B, CONDITION_ALWAYS,
     MSGNO(MsgShamanPrayForShamanWarrior)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_SHAMAN_B,
     CONDITION_ALWAYS, MSGNO(MsgShamanPrayForContigoConqueror)},
    {TRIGGER_TABLE_END},
};

u32 gEmote_PopTiltedScript[] = {
    SC_WAIT(46),
    SC_RELEASE,
};
