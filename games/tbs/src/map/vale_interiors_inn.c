/// @file
/// Vale (MapId_ValeInteriorsInn): Isaac's house and the ground outside it, the
/// inn and the inside of Kraden's cottage, and at
/// VALE_INTERIORS_INN_FROM_BABI_PALACE a room of Babi's Palace: one overlay
/// whose tables and setup choose by the warp destination and the story flags.
/// In story order:
///
/// - The stormy night, three years before the journey, when a new game begins
///   at VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST (from menu/main_menu.c, or
///   ValeGate's VALE_GATE_TO_VALE_INTERIORS_INN_SOUTHEAST). The setup sets
///   FLAG_VALE_STORM and plays EventScript_DoraWakesIsaac: Dora, Isaac's
///   mother, wakes him because the Mt. Aleph Boulder is going to fall, makes
///   him take his tunic (FLAG_ISAAC_WEARS_TUNIC) and leads him downstairs
///   (VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA, to
///   VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA), where
///   EventScript_HurryToThePlaza plays: Kyle, Isaac's father, calls them to
///   hurry, LOCALID_FLAME shakes outside the house, then turns to smoke and
///   rises away, and they run for the plaza, where the village takes refuge, by
///   VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA into ValeStorm
///   (VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA). While FLAG_VALE_STORM is
///   set the villagers are put out of sight, lightning flashes, and
///   VALE_INTERIORS_INN_TO_VALE leads into the storm (ValeStorm) instead of
///   Vale (gValeInteriorsInn_StormWarps). Kyle dies that night, as the village
///   says later.
/// - Vale before the party leaves (FLAG_VALE_PARTY_LEFT clear): from Kraden's
///   cottage (ValeKraden's VALE_KRADEN_TO_VALE_INTERIORS_INN_MYTHRIL_BAG) the
///   party arrives at VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG, inside
///   it, where the Mythril Bag rises and wobbles before Kraden as he prepares
///   for the visit to Sol Sanctum (vale_kraden.c);
///   VALE_INTERIORS_INN_TO_VALE_KRADEN_SOUTH leads back out
///   (VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH). In a room of the inn the
///   masked man (LOCALID_MASKED_MAN) asks "Hey... Boy... Do you have business
///   with me?" and sends Isaac off whatever he answers; at the inn the talk is
///   of three strange travellers staying there, one of them masked, and the
///   innkeeper offers a room.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT): the villagers talk
///   of the eruption and of Jenna, whom the visitors kidnapped, and one asks
///   whether the masked man was Felix, Jenna's brother, and whether Jenna can
///   be trusted.
/// - Back from Kalay (FLAG_KALAY_REACHED): Dora lies ill. The innkeeper
///   welcomes Isaac home and a burly grey-haired man at the inn
///   (LOCALID_MAN_ISAAC_HAS_GROWN) sees how he has grown (Vale's map section
///   flags FLAG_VALE_INNKEEPER_WELCOMED_ISAAC and
///   FLAG_VALE_MAN_SAW_ISAAC_GROWN); Mind Read on either tells of Dora's
///   illness (FLAG_READ_THOUGHTS_ABOUT_DORA). Visiting her
///   (FLAG_VISITED_SICK_DORA) she reminds Isaac that he promised not to come
///   back until his quest was done, and gets up to show she is well; she stays
///   up for that visit (FLAG_MAP_TEMP_03), and the setup otherwise puts her
///   back in bed, where she dreams of Kyle.
/// - Babi's Palace (VALE_INTERIORS_INN_FROM_BABI_PALACE, from Babi's Palace's
///   BABI_PALACE_TO_VALE_INTERIORS_INN, MapId_BabiPalace), in Tolbi, whose
///   lord is Babi: a room of his scholars, who speak of Alchemy, of Kraden,
///   Babi's favourite scholar, and of the Venus Ruins under Babi Lighthouse
///   with a long tunnel beneath them. They talk differently once the Colosso
///   finals are over (FLAG_COLOSSO_OVER), when one asks whether the party knows
///   Kraden. The setup does not show the map section's name there.
///
/// The warps VALE_INTERIORS_INN_TO_VALE_GATE_NORTHWEST to
/// VALE_INTERIORS_INN_TO_VALE_STORM_EAST are taken by the metatile behaviors of
/// the same numbers, leading to the rest of Vale (ValeGate, ValeKraden, Vale
/// and ValeStorm) or between this map's own parts.
///
/// Open questions:
/// - XXX (no evidence): what LOCALID_BLANK_AFTER_KALAY, the blank object event
///   beside the burly man at the inn back from Kalay, was meant to do; it draws
///   nothing once the setup turns off its shadow, and though it is solid it
///   stands where the innkeeper and the burly man already close the way (the
///   leader walking north between them stops at the man with it and without it,
///   render probe vale-inn), so nothing the player can do shows a use.
/// - XXX (no evidence): whether the talk row of EventScript_RemarkIsaacHasGrown
/// was
///   meant to test FLAG_VALE_MAN_SAW_ISAAC_GROWN, as its Mind Read row does;
///   the tables show only what plays (the same line either way, the script
///   setting the man's flag only before the innkeeper's welcome), not what
///   was intended.

#include "save_block.h"
#include "vram_cache.h"
#include "unit/map/vale_interiors_inn.h"
#include "unit/map/common/effect_psynergy.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fxblend.h"
#include "map_grid_scroll.h"
#include "script_layer.h"
#include "map_tables.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "glyph.h"
#include "animspr.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "heap.h"
#include "ability_ids.h"
#include "songs.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "object_event.h"
#include "flags.h"
#include "fixed_math.h"
#include "fx_work.h"

// The arrival scenes are defined after the scripts.
static void EventScript_DoraWakesIsaac(void);
static void EventScript_HurryToThePlaza(void);
static void ValeInteriorsInn_RestoreRoomPatch(void);
static void EventScript_ShowMythrilBag(void);
static void ValeInteriorsInn_SpawnEffectPair(union FieldObject *fieldObject);
static void ValeInteriorsInn_DemonstrateCatch(void);

// Shared facing code retains its position before the overlay entry points.
#include "common/object_facing.h"

/// Vale's placed object events, numbered by the rows of
/// gValeInteriorsInn_ObjectEventTemplates. The tables after the party leaves
/// and after Kalay keep the rows up to the inn's, and differ after them
/// (ValeInteriorsInnAfterKalayLocalId).
enum ValeInteriorsInnLocalId {
  /// Isaac's mother, a woman with fair hair tied up. "Isaac, wake up!"
  /// ★ Named from the game: "your mother, Dora, has been crying"
  /// (MsgValeDoraHasBeenCrying)
  LOCALID_DORA = LOCALID_FIRST_PLACED,
  /// Isaac's father, a man with fair hair (SPRITE_KYLE, Kyle in vale_storm.c).
  /// "Isaac, Dora, hurry! The Boulder could fall at any second!"
  /// ★ Named from the game: "Kyle, watch out!"
  /// (MsgValeKyleWatchOutTheBoulderThe)
  LOCALID_KYLE,
  /// Sprite 0xd0, whose frames are a flame and wisps of smoke; it is placed
  /// outside the house only for EventScript_HurryToThePlaza, where it shakes,
  /// then
  /// flares up as its animation turns from flame to smoke, and rises 18 pixels
  /// as it shrinks away.
  /// ★ Named from what the player sees: a small flame, then wisps of smoke,
  /// spr_208.png (its render)
  LOCALID_FLAME,
  /// A young man with brown hair who lost his father in the storm and means to
  /// be a smith. "My father also died in that storm three years ago."; once the
  /// party has left, "By then, I'll surely be a fine smith."
  /// ★ Named from the game: "My father also died in that storm three years
  /// ago." (MsgValeMyFatherAlsoDiedInThat)
  LOCALID_YOUNG_SMITH,
  /// LOCALID_YOUNG_SMITH's mother, a woman in a long blue dress. "My son is
  /// still young, so I don't make him work too long."
  /// ★ Named from the game: "My son is still young"
  /// (MsgValeMySonIsStillYoungSo)
  LOCALID_YOUNG_SMITHS_MOTHER,
  /// A woman with red-brown hair in a pink dress and a white apron.
  /// ★ Named from the game: "Is it the innkeeper?" (MsgValeCoughCoughWho)
  LOCALID_INNKEEPER,
  /// A young man with brown hair who walks about the inn. "One of the men from
  /// that group staying here wore a mask over his face."
  /// ★ Named from the game: "wore a mask over his face"
  /// (MsgValeOneOfTheMenFromThat)
  LOCALID_YOUNG_MAN_SAW_MASK,
  /// A woman with fair hair at the inn. "Two of those three guests staying with
  /// us have gone out."; Mind Read later hears "I found his wallet when I was
  /// cleaning the room..."
  /// ★ Named from the game: "We're a small hotel, and we get few guests"
  /// (MsgValeWereASmallHotelAndWe)
  LOCALID_INN_WORKER,
  /// In a room of the inn, before the party leaves Vale: sprite 0x22, masked
  /// Felix's (sol_sanctum_eruption.h). "Hey... Boy... Do you have business with
  /// me?"
  /// ★ Named from the game: "That masked man..." (MsgValeTheMaskedManWasFelix)
  LOCALID_MASKED_MAN,
  /// SPRITE_TUNIC, set beside Isaac's bed on the stormy night: Catch reaches it
  /// and Isaac puts it on (FLAG_ISAAC_WEARS_TUNIC). Dora: "Don't go out without
  /// your tunic!"
  /// ★ Named from the game: "Don't go out without your tunic!"
  /// (MsgValeComeOnHurry)
  LOCALID_TUNIC,
  /// Kraden's sprite (0x1e, as in Sol Sanctum's scenes), seen only from
  /// VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG.
  /// ★ Named from the game: "from you, Kraden?" (MsgValeKradenOhIsaac)
  LOCALID_KRADEN
};

/// The rows of gValeInteriorsInn_ObjectEventTemplatesAfterKalay that differ
/// from the first table's.
enum ValeInteriorsInnAfterKalayLocalId {
  /// A burly grey-haired man at the inn. "Well if it isn't Isaac! You've grown
  /// so much, I almost didn't recognize you!"; Mind Read hears of Dora's
  /// illness.
  /// ★ Named from the game: "You've grown so much" (MsgValeYouveGrownSoMuch)
  LOCALID_MAN_ISAAC_HAS_GROWN = LOCALID_FIRST_PLACED + 8,
  /// SPRITE_BLANK, a solid object event that draws nothing, beside
  /// LOCALID_MAN_ISAAC_HAS_GROWN; no event answers it, the setup takes away the
  /// shadow its blank sprite would draw, and it closes no way the innkeeper
  /// and the man do not (an open question).
  LOCALID_BLANK_AFTER_KALAY
};

/// Babi's scholars, numbered by the rows of
/// gValeInteriorsInn_ScholarsObjectEventTemplates and named by what they say
/// before the Colosso finals.
enum ValeInteriorsInnScholarLocalId {
  /// An old scholar with a white beard in a blue cap, who tells of the ruins
  /// where Babi Lighthouse stands. "Originally, there were ancient ruins at the
  /// site of Babi Lighthouse."
  /// ★ Named from the game: "Babi makes his scholars perform all sorts of
  /// research." (MsgBabiBabiMakesHisScholarsPerformAll)
  LOCALID_SCHOLAR_ANCIENT_RUINS = LOCALID_FIRST_PLACED,
  /// A scholar with glasses in a blue cap, whose experiments fail. "No matter
  /// how many times I run the experiment, I never get the results I want!";
  /// after the finals he asks whether the party knows Kraden.
  /// ★ Named from the game: "No matter how many times I run the experiment"
  /// (MsgBabiNoMatterHowManyTimesI)
  LOCALID_SCHOLAR_EXPERIMENT,
  /// An old scholar with a white beard in a blue cap, who tells of Alchemy
  /// sealed away. "It is said that long ago, the power of Alchemy was sealed
  /// away."
  /// ★ Named from the game: "the power of Alchemy was sealed away."
  /// (MsgBabiItIsSaidThatLongAgo)
  LOCALID_SCHOLAR_SEALED_ALCHEMY,
  /// A scholar with glasses in a blue cap, who tells of the Venus Ruins.
  /// "Originally, the ancient Venus Ruins lay buried under Babi Lighthouse."
  /// ★ Named from the game: "Originally, the ancient Venus Ruins lay buried"
  /// (MsgBabiOriginallyTheAncientVenusRuinsLay)
  LOCALID_SCHOLAR_VENUS_RUINS,
  /// An old man with a cane who walks about the room. "Babi makes his scholars
  /// perform all sorts of research. I wonder where Babi's favorite scholar,
  /// Kraden, is now?"
  /// ★ Named from the game: "Babi makes his scholars perform all sorts of
  /// research." (MsgBabiBabiMakesHisScholarsPerformAll)
  LOCALID_OLD_MAN_ON_BABIS_RESEARCH
};

/// Vale's map section flags this map keeps while the party is in Vale.
enum ValeInteriorsInnMapSecFlag {
  /// Back from Kalay, the innkeeper welcomed Isaac home.
  /// ★ Named from the game: "You came back, huh?" (MsgValeCameBack2)
  FLAG_VALE_INNKEEPER_WELCOMED_ISAAC = 0x300,
  /// Back from Kalay, LOCALID_MAN_ISAAC_HAS_GROWN remarked how much Isaac
  /// has grown.
  /// ★ Named from the game: "You've grown so much" (MsgValeYouveGrownSoMuch)
  FLAG_VALE_MAN_SAW_ISAAC_GROWN = 0x301
};

/// Animations of Dora's and young Isaac's sprites beyond enum
/// ObjectEventAnimation, as their animation tables and frames under
/// graphics/sprite draw them.
enum ValeInteriorsInnAnimation {
  ANIM_DORA_IN_BED = 10, ///< lying down, ill (spr_031.png)
  /// Dora seen from behind, her head bowed and turned aside (spr_031.png).
  ANIM_DORA_HEAD_TURNED = 11,
  ANIM_DORA_HEAD_BOWED = 12, ///< seen from behind, her head bowed low
  /// ANIM_DORA_HEAD_TURNED and ANIM_DORA_HEAD_BOWED in turn, twice.
  ANIM_DORA_HEAD_SWAYING = 13,
  /// Dora straightening, still seen from behind (spr_031.png).
  ANIM_DORA_STRAIGHTEN = 14,
  /// Young Isaac asleep, only his head on the pillow (spr_018.png).
  ANIM_ISAAC_ASLEEP = 18,
  ANIM_ISAAC_GET_UP = 19 ///< sitting up, then jumping out of bed
};

// FLAG_MAP_TEMP_03: Dora got out of her sickbed on this visit.

/// Returns the warp destinations, among them the scenes played on arrival:
/// VALE_INTERIORS_INN_FROM_BABI_PALACE from Babi's palace
/// (BABI_PALACE_TO_VALE_INTERIORS_INN),
/// VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST where a new game begins
/// (menu/main_menu.c, and ValeGate's
/// VALE_GATE_TO_VALE_INTERIORS_INN_SOUTHEAST),
/// VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA from this map's
/// own VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA, and
/// VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG from Kraden's cottage
/// (VALE_KRADEN_TO_VALE_INTERIORS_INN_MYTHRIL_BAG).
const struct MapWarpDest *ValeInteriorsInn_GetWarpDests(void) {
  return gValeInteriorsInn_WarpDests;
}

const struct MapWarpRegion *ValeInteriorsInn_GetRegions(void) { return NULL; }

/// On the stormy night the warps lead into the storm.
const u32 *ValeInteriorsInn_GetWarps(void) {
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    return gValeInteriorsInn_StormWarps;
  }
  return gValeInteriorsInn_Warps;
}

/// Places Babi's scholars, or Vale's villagers as the story has left them.
const struct ObjectEventTemplate *
ValeInteriorsInn_GetObjectEventTemplates(void) {
  struct ObjectEventTemplate *table;

  if (gSaveBlock.warpDest == VALE_INTERIORS_INN_FROM_BABI_PALACE)
    return gValeInteriorsInn_ScholarsObjectEventTemplates;
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    table = gValeInteriorsInn_ObjectEventTemplatesAfterKalay;
  else if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0)
    table = gValeInteriorsInn_ObjectEventTemplatesAfterLeaving;
  else
    table = gValeInteriorsInn_ObjectEventTemplates;
  Map_AssignViewFlags(table);
  return table;
}

/// In Babi's Palace, one of Babi's scholars asks whether the party knows
/// Kraden. Reached by talking to LOCALID_SCHOLAR_EXPERIMENT in their room
/// (VALE_INTERIORS_INN_FROM_BABI_PALACE) once FLAG_COLOSSO_OVER is set;
/// speakerId is the object event spoken to.
static void EventScript_ScholarAsksAboutKraden(s32 speakerId) {
  s32 firstMessage = MsgValeFolksSeemKnow;

  FieldScript_SetMessageId(firstMessage);
  FieldMessage_ShowAtObject(speakerId, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(firstMessage + 1);
  } else {
    FieldScript_SetMessageId(firstMessage + 2);
  }
  FieldScript_ShowMessageAndWait(speakerId, 0);
}

/// Chooses the events: Babi's scholars talk differently once the Colosso
/// finals are over; Vale's villagers follow the storm and the story.
const struct MapEvent *ValeInteriorsInn_GetEvents(void) {
  if (gSaveBlock.warpDest == VALE_INTERIORS_INN_FROM_BABI_PALACE) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      return gValeInteriorsInn_ScholarsLaterEvents;
    }
    return gValeInteriorsInn_ScholarsEvents;
  }

  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    return gValeInteriorsInn_StormEvents;
  }
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    return gValeInteriorsInn_EventsAfterKalay;
  }
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    return gValeInteriorsInn_EventsAfterLeaving;
  }
  return gValeInteriorsInn_Events;
}

/// The masked man at the inn asks Isaac his business. In a room of Vale's inn,
/// the masked man asks whether Isaac has business with him, and sends him off
/// whatever he answers. "Hey... Boy..." Reached by talking to
/// LOCALID_MASKED_MAN before the party leaves Vale (gValeInteriorsInn_Events).
static void EventScript_AskIsaacHisBusiness(void) {
  FieldScript_Start();
  ObjectMotion_SetVariantCallback(LOCALID_MASKED_MAN, 2);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(MsgValeHeyBoy);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_MASKED_MAN, 10);
  Script_ShowMessageAndWait(LOCALID_MASKED_MAN, 0, 6);
  ObjectEvent_ShowEmote(LOCALID_MASKED_MAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectMotion_SetVariantCallback(LOCALID_MASKED_MAN, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MASKED_MAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_OpenMessage(LOCALID_MASKED_MAN, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  ObjectMotion_SetVariantCallback(LOCALID_MASKED_MAN, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MASKED_MAN, 0, 4);
  FieldScript_End();
}

/// A young man asks whether the masked man was Felix. In Vale, the young man
/// with brown hair also wonders whether Jenna can be trusted. Reached by
/// talking to LOCALID_YOUNG_MAN_SAW_MASK once the party has left Vale and until
/// it reaches Kalay (gValeInteriorsInn_EventsAfterLeaving).
static void EventScript_AskWhetherMaskedManWasFelix(void) {
  struct FieldWork *work;

  FieldScript_Start();
  ObjectMotion_SetAngleToward(LOCALID_YOUNG_MAN_SAW_MASK, LOCALID_PARTY_LEADER,
                              10);
  FieldScript_SetMessageId(MsgValeTheMaskedManWasFelix);
  FieldMessage_ShowAtObject(LOCALID_YOUNG_MAN_SAW_MASK, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_ShowMessageAndWait(LOCALID_YOUNG_MAN_SAW_MASK, 0);
  } else {
    work = gFieldWork;
    work->message = (work->message + 1);
    FieldScript_AskYesNoAndShowResponse(LOCALID_YOUNG_MAN_SAW_MASK, 0);
  }
  FieldScript_End();
}

/// Stops storm lightning before fading through the requested warp.
/// Called by the warps below.
static void ValeInteriorsInn_CloseScreenAndExit(s32 warpNumber) {
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    Weather_StopLightning();
  }
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(warpNumber);
}

// The warps in Vale: stepping on the metatile behavior of the same number in
// any of the Vale tables plays a step sound and takes that warp
// (gValeInteriorsInn_Warps, or gValeInteriorsInn_StormWarps on the stormy
// night).

/// Reached by metatile behavior 1; leads to ValeGate
/// (VALE_GATE_FROM_VALE_INTERIORS_INN_WEST).
static void EventScript_WarpToValeGateNorthwest(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  ValeInteriorsInn_CloseScreenAndExit(
      VALE_INTERIORS_INN_TO_VALE_GATE_NORTHWEST);
}

/// Reached by metatile behavior 2; leads to Kraden's cottage
/// (VALE_KRADEN_FROM_VALE_INTERIORS_INN_NORTH).
static void EventScript_WarpToValeKradenNorth(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  ValeInteriorsInn_CloseScreenAndExit(VALE_INTERIORS_INN_TO_VALE_KRADEN_NORTH);
}

/// Reached by metatile behavior 3; leads to ValeGate
/// (VALE_GATE_FROM_VALE_INTERIORS_INN_NORTH).
static void EventScript_WarpToValeGateNortheast(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  ValeInteriorsInn_CloseScreenAndExit(
      VALE_INTERIORS_INN_TO_VALE_GATE_NORTHEAST);
}

/// Reached by metatile behavior 4; leads to ValeGate
/// (VALE_GATE_FROM_VALE_INTERIORS_INN_SOUTH).
static void EventScript_WarpToValeGateWest(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  ValeInteriorsInn_CloseScreenAndExit(VALE_INTERIORS_INN_TO_VALE_GATE_WEST);
}

/// Reached by metatile behavior 5, the inn's stairs; climbs to its guest room
/// at VALE_INTERIORS_INN_INN_FROM_GROUND_FLOOR.
static void EventScript_WarpToInnUpstairs(void) {
  Sound_PlaySong(SE_STAIRS_UP);
  ValeInteriorsInn_CloseScreenAndExit(VALE_INTERIORS_INN_INN_TO_UPSTAIRS);
}

/// Reached by metatile behavior 6; leads to Vale
/// (VALE_FROM_VALE_INTERIORS_INN), or on the stormy night to ValeStorm
/// (VALE_STORM_FROM_VALE_INTERIORS_INN_ISAACS_HOUSE).
static void EventScript_WarpToVale(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  ValeInteriorsInn_CloseScreenAndExit(VALE_INTERIORS_INN_TO_VALE);
}

/// Reached by metatile behavior 7, the stairs of Isaac's house; climbs to its
/// upper floor.
static void EventScript_WarpToIsaacsHouseUpstairs(void) {
  Sound_PlaySong(SE_STAIRS_UP);
  ValeInteriorsInn_CloseScreenAndExit(
      VALE_INTERIORS_INN_ISAACS_HOUSE_TO_UPSTAIRS);
}

/// Reached by metatile behavior 8, the stairs of the inn's guest room; goes
/// back down.
static void EventScript_WarpToInnDownstairs(void) {
  Sound_PlaySong(SE_STAIRS_DOWN);
  ValeInteriorsInn_CloseScreenAndExit(VALE_INTERIORS_INN_INN_TO_GROUND_FLOOR);
}

/// Reached by metatile behavior 9, the stairs on the upper floor of Isaac's
/// house; goes back down.
static void EventScript_WarpToIsaacsHouseDownstairs(void) {
  Sound_PlaySong(SE_STAIRS_DOWN);
  ValeInteriorsInn_CloseScreenAndExit(
      VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR);
}

/// Reached by metatile behavior 10; leads to ValeStorm
/// (VALE_STORM_FROM_VALE_INTERIORS_INN_WEST).
static void EventScript_WarpToValeStormWest(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  ValeInteriorsInn_CloseScreenAndExit(VALE_INTERIORS_INN_TO_VALE_STORM_WEST);
}

/// Reached by metatile behavior 11; leads to ValeStorm
/// (VALE_STORM_FROM_VALE_INTERIORS_INN_EAST).
static void EventScript_WarpToValeStormEast(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  ValeInteriorsInn_CloseScreenAndExit(VALE_INTERIORS_INN_TO_VALE_STORM_EAST);
}

/// Sets up these rooms for the stormy night or the story's villagers. It also
/// plays the scenes of VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST,
/// VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA and
/// VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG. Called by Map_Run once
/// the object events are placed. VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST
/// sets FLAG_VALE_STORM before EventScript_DoraWakesIsaac; back from Kalay,
/// Dora is put back in her sickbed unless FLAG_MAP_TEMP_03 says she got up.
s32 ValeInteriorsInn_Initialize(void) {
  struct ObjectEvent *objectEvent;

  if (gSaveBlock.warpDest == VALE_INTERIORS_INN_FROM_BABI_PALACE) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  } else {
    if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
      Import_ObjectEventSetPosition(LOCALID_YOUNG_SMITH, 0, 0);
      Import_ObjectEventSetPosition(LOCALID_YOUNG_SMITHS_MOTHER, 0, 0);
      Import_ObjectEventSetPosition(LOCALID_INNKEEPER, 0, 0);
      Import_ObjectEventSetPosition(LOCALID_YOUNG_MAN_SAW_MASK, 0, 0);
      Import_ObjectEventSetPosition(LOCALID_INN_WORKER, 0, 0);
      Import_ObjectEventSetPosition(LOCALID_MASKED_MAN, 0, 0);
    } else {
      ValeInteriorsInn_RestoreRoomPatch();
    }
    ObjectMotion_SetActionVariant(LOCALID_INNKEEPER, 1);
    if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
      objectEvent = ObjectEvent_GetById(LOCALID_BLANK_AFTER_KALAY);
      ObjectEvent_SetSpriteFlags(objectEvent, 0);
      if (gSaveBlock.warpDest == VALE_INTERIORS_INN_FROM_VALE ||
          gSaveBlock.warpDest ==
              VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS) {
        if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
          if (GameFlag_Get(FLAG_MAP_TEMP_03) != 0) {
            TilesetAnim_Resume(12);
          }
        } else {
          TilesetAnim_Resume(11);
          objectEvent = ObjectEvent_GetById(LOCALID_DORA);
          ObjectEvent_SetSpriteFlags(objectEvent, 0);
          Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_DORA_IN_BED);
        }
      }
    } else {
      if (gSaveBlock.warpDest ==
          VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA) {
        EventScript_HurryToThePlaza();
      } else {
        if (gSaveBlock.warpDest ==
            VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST) {
          GameFlag_Set(FLAG_VALE_STORM);
          EventScript_DoraWakesIsaac();
        } else {
          if (gSaveBlock.warpDest ==
              VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG) {
            EventScript_ShowMythrilBag();
          } else {
            ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
                MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
            if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
              Weather_StartLightning();
              // Through its heap slot rather than gLightningWork, the spelling
              // this site matched with; the other is not measured.
              ((struct LightningWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_LIGHTNING))
                  ->loud = 1;
              Weather_LastLightning();
              WaitFrames(30);
              FieldScript_StartScreenTransition();
              FieldScript_WaitTransitionFrames();
              Weather_ResumeLightning();
            } else {
              MapGrid_CenterOnFollowedPoint();
              WaitFrames(1);
            }
          }
        }
      }
    }
  }
  return 0;
}

/// In their house in Vale on the stormy night, Dora wakes Isaac, makes him take
/// his tunic, and leads him out into the storm. "The Mt. Aleph Boulder is going
/// to fall!" Called by ValeInteriorsInn_Initialize on arrival by
/// VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST, where a new game begins. Sets
/// FLAG_ISAAC_WEARS_TUNIC and takes
/// VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA, back into this
/// map by VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA, where
/// EventScript_HurryToThePlaza follows.
static void EventScript_DoraWakesIsaac(void) {
  s32 firstMessage;
  struct Sprite *sprite;
  struct MapGridScrollWork *grid;
  struct ObjectEvent *viewCenter;
  struct FieldWork **eventSlot;

  eventSlot = (struct FieldWork **)Ram_FieldWork;
  /* FAKEMATCH(temporary): the event record read into a block-local before
     the map work and view centre; as reads in place, 11 of 430 instructions
     differ in tbs-en (13 with the view centre first). */
  {
    struct FieldWork *work = *eventSlot;

    grid = *(struct MapGridScrollWork **)Ram_MapGridWork;
    viewCenter = (struct ObjectEvent *)work->camera;
  }
  sprite = ObjectEvent_GetById(LOCALID_TUNIC)->sprite;
  // Present the bedside conversation before revealing the wider storm scene.
  FieldScript_Start();
  Import_ObjectEventSetPosition(LOCALID_YOUNG_SMITH, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_YOUNG_SMITHS_MOTHER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_INNKEEPER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_YOUNG_MAN_SAW_MASK, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_INN_WORKER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_MASKED_MAN, 0, 0);
  // Isaac lies asleep in bed; his tunic is set beside him.
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_ISAAC_ASLEEP);
  sprite->rotation = 1365;
  ObjectEvent_GetById(LOCALID_TUNIC)->motionFlags = 0;
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_TUNIC), 0);
  Import_ObjectEventSetPosition(LOCALID_TUNIC, PIXELS(576), PIXELS(650));
  TilesetAnim_Resume(7);
  Import_ObjectEventSetPosition(LOCALID_DORA, PIXELS(534), PIXELS(688));
  ObjectSystem_SuspendProjectionAndShowSprites();
  // On a dark screen, Dora's voice: "Isaac, wake up!" "Please, dear, wake up!"
  Message_SetVoiceFromObjectEvent(LOCALID_DORA);
  firstMessage = MsgValeWake;
  FieldMessage_ShowCentered(firstMessage, 1, 0);
  FieldScript_WaitFrames(40);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Message_SetVoiceFromObjectEvent(LOCALID_DORA);
  FieldMessage_ShowCentered(firstMessage + 1, 1, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  FieldScript_WaitFrames(40);
  // Widen the camera bounds and reveal the lightning outside.
  grid->minX = PIXELS(328);
  grid->minY = PIXELS(600);
  grid->maxX = PIXELS(624);
  grid->maxY = PIXELS(816);
  viewCenter->x.fixed = PIXELS(564);
  viewCenter->y.fixed = 0;
  viewCenter->z.fixed = PIXELS(691);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  (*eventSlot)->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  (*eventSlot)->transitionFrames = 64;
  Weather_StartLightning();
  (*(struct LightningWork **)((u8 *)eventSlot + Ram_LightningWorkOffset))
      ->loud = 1;
  Weather_LastLightning();
  WaitFrames(30);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  Weather_ResumeLightning();
  // "The Mt. Aleph Boulder is going to fall!" "Come on, Isaac. We have to go
  // now!"
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(firstMessage + 2);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | LOCALID_DORA, 0, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | LOCALID_DORA, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  // Isaac gets out of bed and jumps down beside Dora.
  TilesetAnim_Pause(7);
  FieldScript_WaitFrames(20);
  TilesetAnim_Resume(8);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_ISAAC_GET_UP);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 557, 679);
  TilesetAnim_Pause(8);
  TilesetAnim_Resume(9);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 555, 680);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 1);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 543, 674);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 40);
  // "Isaac! You forgot something!" Catch plays from Dora on the tunic.
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | LOCALID_DORA, 0);
  ValeInteriorsInn_DemonstrateCatch();
  // Resolve the Psynergy demonstration before the pair walks away together.
  // "It's pouring outside! Don't go out without your tunic!" She steps up to
  // him and he puts it on, with a flash and SE_RECOVERY.
  ObjectMotion_SetVariantCallback(LOCALID_DORA, 2);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | LOCALID_DORA, 0, 20);
  ObjectEvent_GetById(LOCALID_DORA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 542, 680);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(LOCALID_DORA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
  Object_RegisterSpriteMetadata(
      (struct Object *)ObjectEvent_GetById(LOCALID_PARTY_LEADER), SPRITE_TUNIC);
  GameFlag_Set(FLAG_ISAAC_WEARS_TUNIC);
  Sound_PlaySong(SE_RECOVERY);
  ObjectEvent_ConfigureSpriteColor(LOCALID_PARTY_LEADER, 7);
  FieldScript_WaitFrames(10);
  ObjectEvent_ConfigureSpriteColor(LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_GetById(LOCALID_DORA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 534, 688);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(LOCALID_DORA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  FieldCamera_FollowObjectEvent(LOCALID_DORA, 1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags |= 0x1;
  ObjectEvent_SetActionScript(LOCALID_DORA, gValeInteriorsInn_LeaveHouseScript);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gValeInteriorsInn_LeaveHouseScript);
  ObjectEvent_RefreshSelectorById(LOCALID_DORA);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 419, 661);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 408, 661);
  Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 10);
  // "Have you got everything you need?" Yes: "Good for you, Isaac."; no: "I'm
  // sorry, but we don't have time..."
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_DORA, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    (*eventSlot)->message++;
  }
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_DORA, 0, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_DORA, gValeInteriorsInn_DoraLeavesScript);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gValeInteriorsInn_IsaacLeavesScript);
  FieldScript_WaitFrames(20);
  (*eventSlot)->transition = MAP_TRANSITION(TRANSITION_WINDOW, 1);
  (*eventSlot)->transitionFrames = 16;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(
      VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA);
}

/// Selects the hardware blend targets for the storm sequence.
static __inline__ void SetBlendTarget(u32 value) { REG_BLDCNT = value; }

static __inline__ void SetBlendAlpha(u32 value) { REG_BLDALPHA = value; }

/// Outside the house in Vale on the stormy night, Kyle calls to Isaac and Dora
/// to hurry; LOCALID_FLAME shakes, then turns to smoke and rises away, as they
/// run to the plaza. Called by ValeInteriorsInn_Initialize on arrival by
/// VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA, after
/// EventScript_DoraWakesIsaac. Takes
/// VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA, to ValeStorm
/// (VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA), where
/// EventScript_DoraSendsIsaacToThePlaza follows.
static void EventScript_HurryToThePlaza(void) {
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  u32 i;
  volatile u16 *alpha;

  objectEvent = ObjectEvent_GetById(LOCALID_FLAME);
  sprite = objectEvent->sprite;
  FieldScript_Start();
  // The villagers stay out of sight; LOCALID_FLAME starts shaking.
  Import_ObjectEventSetPosition(LOCALID_YOUNG_SMITH, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_YOUNG_SMITHS_MOTHER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_INNKEEPER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_YOUNG_MAN_SAW_MASK, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_INN_WORKER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_MASKED_MAN, 0, 0);
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(431), PIXELS(391));
  Import_ObjectEventSetPosition(LOCALID_FLAME, PIXELS(463), PIXELS(404));
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FLAME), 0);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  objectEvent->motionFlags = 0;
  sprite->image.priority = 1;
  ObjectEvent_SetActionScript(LOCALID_FLAME,
                              gValeInteriorsInn_FlameShakesScript);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, 1);
  MapGrid_CopyCellsTo(83, 15, 83, 19, 5, 4);
  MapGrid_CopyCellsTo(90, 16, 90, 20, 5, 4);
  MapGrid_CopyCellsTo(77, 23, 82, 23, 5, 7);
  MapGrid_CopyCellsTo(83, 33, 85, 33, 2, 2);
  MapGrid_CopyCellsTo(91, 28, 90, 28, 1, 1);
  MapGrid_CopyCellsTo(91, 28, 88, 30, 1, 1);
  MapGrid_CopyCellsTo(94, 27, 94, 23, 6, 4);
  MapGrid_CopyCellsTo(92, 28, 87, 23, 4, 4);
  MapGrid_CopyCellsTo(65, 53, 88, 24, 2, 2);
  BlendScript_Suspend();
  SetBlendTarget(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  Io_Write16(BLEND_ALPHA(12, 16), &REG_BLDALPHA);
  Weather_StartLightning();
  ((struct LightningWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_LIGHTNING))->loud = 1;
  Weather_LastLightning();
  WaitFrames(30);
  // Dora and Isaac run out of the house (SE_DOOR_OPEN).
  FieldCamera_FollowObjectEvent(LOCALID_DORA, 1);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(LOCALID_KYLE, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gValeInteriorsInn_IsaacRunsAheadScript);
  ObjectEvent_SetActionScript(LOCALID_DORA,
                              gValeInteriorsInn_DoraRunsAheadScript);
  FieldScript_StartScreenTransition();
  ObjectEvent_RefreshSelectorById(LOCALID_DORA);
  Sound_PlaySong(SE_DOOR_OPEN);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 10);
  // The camera finds Kyle coming up the path: "Isaac, Dora, hurry! The
  // Boulder could fall at any second!"
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(414), -1, PIXELS(530), 1);
  ObjectEvent_SetPosition(LOCALID_KYLE, PIXELS(414), PIXELS(530));
  ObjectMotion_SetPositionAndReset(LOCALID_KYLE, 427, 483);
  FieldCamera_WaitForArrival();
  FieldScript_SetMessageId(MsgValeDoraHurryBoulder);
  FieldScript_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | LOCALID_KYLE, 0, 10);
  FieldCamera_SetSpeedParameters(PIXELS(3) / 2, PIXELS(3) / 16);
  FieldCamera_MoveWithinMapBounds(PIXELS(480), -1, PIXELS(444), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  // They nod, Kyle runs back down, and the two turn back (SE_234).
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_WEST, 20);
  Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_KYLE, ANIM_NOD);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_KYLE, 415, 589);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetActionScript(LOCALID_DORA,
                              gValeInteriorsInn_DoraRunsBackScript);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gValeInteriorsInn_IsaacRunsBackScript);
  Sound_PlaySong(SE_234);
  FieldScript_WaitFrames(20);
  // LOCALID_FLAME flares up, turns to smoke and rises away
  // (gValeInteriorsInn_FlameRisesScript) as SE_202 plays.
  ObjectEvent_SetActionScript(LOCALID_FLAME,
                              gValeInteriorsInn_FlameRisesScript);
  alpha = &REG_BLDALPHA;

  // The flame brightens over four frames, then fades away over sixteen.
  i = 0;
ramp:
  *alpha = BLEND_ALPHA(14, 16) + i;
  WaitFrames(1);
  if (++i <= 3)
    goto ramp;
  Sound_PlaySong(SE_202);
  WaitFrames(10);
  for (i = 0; i <= 15; i++) {
    SetBlendAlpha(BLEND_ALPHA(15, 16) - i);
    WaitFrames(1);
  }
  // Dora and Isaac look back, startled, then nod to each other.
  ObjectEvent_RefreshSelectorById(LOCALID_PARTY_LEADER);
  Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_STAND);
  ObjectMotion_SetVariantCallback(LOCALID_DORA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  FieldEffect_ConfigureLinkedObject(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(80);
  Import_ObjectEventSetPosition(LOCALID_KYLE, 0, 0);
  ObjectEvent_LinkPair(LOCALID_DORA, LOCALID_PARTY_LEADER, 20);
  Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(40);
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_FollowObjectEvent(LOCALID_DORA, 1);
  ObjectEvent_SetActionScript(LOCALID_DORA, gValeInteriorsInn_RunToPlazaScript);
  ObjectEvent_RunActionScriptAndWait(LOCALID_PARTY_LEADER,
                                     gValeInteriorsInn_RunToPlazaScript);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
      MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transitionFrames = 32;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA);
}

/// Vale's innkeeper offers a room from behind the counter; elsewhere talks of
/// the travelers, the eruption, or, back from Kalay, welcomes Isaac home.
/// Reached by talking to LOCALID_INNKEEPER in every Vale table but the
/// storm's, and back from Kalay by Mind Read on him until he has welcomed
/// Isaac. Sets FLAG_VALE_INNKEEPER_WELCOMED_ISAAC on that welcome.
static void EventScript_TalkToInnkeeper(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if ((u32)(objectEvent->facing + -0x2000) > 0x9000) {
    Inn_CheckIn(0, LOCALID_INNKEEPER);
  } else {
    FieldScript_Start();
    if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
      ObjectMotion_SetVarCbAndRefresh(LOCALID_INNKEEPER, 2);
      ObjectEvent_FaceObjectEvent(LOCALID_INNKEEPER, LOCALID_PARTY_LEADER, 10);
      if (Script_GetFlag(FLAG_VALE_INNKEEPER_WELCOMED_ISAAC) == 0) {
        FieldScript_SetMessageId(MsgValeCameBack2);
        Script_ShowMessage(LOCALID_INNKEEPER, 0);
        GameFlag_Set(FLAG_VALE_INNKEEPER_WELCOMED_ISAAC);
      }
      FieldScript_SetMessageId(MsgValeHomeJustToStay);
      Script_AskYesNo(LOCALID_INNKEEPER, 0);
      ObjectEvent_FaceDirection(LOCALID_INNKEEPER, FACING_WEST + FACING_STEP,
                                10);
    } else {
      if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
        FieldScript_SetMessageId(MsgValeTheVisitorsCausedTheEruption);
      } else {
        FieldScript_SetMessageId(MsgValeTheThreeTravelersSeemOdd);
      }
      Script_ShowMessage(LOCALID_INNKEEPER, 0);
    }
    FieldScript_End();
  }
}

/// Back from Kalay, a burly grey-haired man at Vale's inn
/// (LOCALID_MAN_ISAAC_HAS_GROWN) remarks how much Isaac has grown. Reached by
/// talking to him while FLAG_VALE_INNKEEPER_WELCOMED_ISAAC is clear, or by Mind
/// Read on him until FLAG_VALE_MAN_SAW_ISAAC_GROWN is set, which this sets.
/// The talk row tests the innkeeper's flag, not this one: after his welcome
/// the row below plays the same line as a plain message, which leaves
/// FLAG_VALE_MAN_SAW_ISAAC_GROWN clear for Mind Read to play this once.
static void EventScript_RemarkIsaacHasGrown(void) {
  FieldScript_Start();
  ObjectMotion_SetAngleToward(LOCALID_MAN_ISAAC_HAS_GROWN, LOCALID_PARTY_LEADER,
                              10);
  FieldScript_SetMessageId(MsgValeYouveGrownSoMuch);
  FieldScript_ShowMessageAndWait(LOCALID_MAN_ISAAC_HAS_GROWN, 0);
  ObjectMotion_ArmCallback(LOCALID_MAN_ISAAC_HAS_GROWN,
                           FACING_NORTHWEST + FACING_STEP, 10);
  GameFlag_Set(FLAG_VALE_MAN_SAW_ISAAC_GROWN);
  FieldScript_End();
}

/// Mind Read on Vale's innkeeper: Dora would not let Isaac stay in Vale.
/// Reached back from Kalay once he has welcomed Isaac. Sets
/// FLAG_READ_THOUGHTS_ABOUT_DORA.
static void EventScript_ReadInnkeepersThoughts(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeDoraWouldntLetHimStay);
  FieldScript_ShowMessageAndWait(LOCALID_INNKEEPER, 0);
  GameFlag_Set(FLAG_READ_THOUGHTS_ABOUT_DORA);
  FieldScript_End();
}

/// Mind Read on LOCALID_MAN_ISAAC_HAS_GROWN at Vale's inn: Dora was struck
/// with illness. Reached back from Kalay once he has remarked on Isaac. Sets
/// FLAG_READ_THOUGHTS_ABOUT_DORA.
static void EventScript_ReadThoughtsAboutDorasIllness(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeDoraWasStruckWithIllness);
  FieldScript_ShowMessageAndWait(LOCALID_MAN_ISAAC_HAS_GROWN, 0);
  GameFlag_Set(FLAG_READ_THOUGHTS_ABOUT_DORA);
  FieldScript_End();
}

/// Back from Kalay, Isaac visits Dora in her sickbed at home in Vale; she asks
/// who told him she was ill, then gets up to show she feels well. Reached by
/// talking to Dora, or by Mind Read on her, until FLAG_VISITED_SICK_DORA is
/// set. Sets it and FLAG_MAP_TEMP_03.
static void EventScript_VisitSickDora(void) {
  struct ObjectEvent *leader;
  s32 firstMessage;

  FieldScript_Start();
  // Isaac steps up to the bed. "Cough, cough! Who is it? Is it the innkeeper?"
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 569, 393);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 40);
  ObjectMotion_SetVariantCallback(LOCALID_DORA, 2);
  FieldScript_SetMessageId(MsgValeCoughCoughWho);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 80);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectMotion_SetVariantCallback(LOCALID_DORA, 1);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
  FieldScript_WaitFrames(80);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  // Bring Dora out of bed before asking about her illness.
  ObjectMotion_ResetAndSetPosition(LOCALID_DORA, 584, 406);
  TilesetAnim_Pause(11);
  TilesetAnim_Resume(12);
  Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_DORA_HEAD_BOWED);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
  FieldScript_WaitFrames(40);
  // "Isaac..." "Are you finally home? Have you completed your mission!?"
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_DORA_HEAD_SWAYING);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_DORA_HEAD_TURNED);
  FieldScript_WaitFrames(40);
  // "You haven't, have you?" "Have you forgotten your promise, Isaac?"
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_DORA_HEAD_BOWED);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_DORA_HEAD_SWAYING);
  // "You promised you wouldn't come back until your quest was complete,
  // right?" Either reply has its own answer; she is less surprised when Mind
  // Read has told Isaac of her illness.
  FieldMessage_ShowAtObject(LOCALID_DORA, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message += 1;
  }
  if (Script_GetFlag(FLAG_READ_THOUGHTS_ABOUT_DORA) != 0) {
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  }
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_DORA, 0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  // Branch on Isaac's replies and whether Mind Read revealed her illness.
  firstMessage = MsgValeWhoToldIll;
  FieldScript_SetMessageId(firstMessage);
  FieldMessage_ShowAtObject(LOCALID_DORA, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message += 1;
  }
  if (GameFlag_Get(FLAG_READ_THOUGHTS_ABOUT_DORA) != 0) {
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  }
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_DORA, 0);
  FieldEffect_SpawnLinked(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  // "You can't get out of your promises just because you're worried about
  // me." "I just caught a cold." "But I'm fine now. Look!"
  FieldScript_SetMessageId(firstMessage + 3);
  FieldScript_ShowMessageAndWait(LOCALID_DORA, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_DORA_HEAD_SWAYING);
  ObjectMotion_SetVariantCallback(LOCALID_DORA, 2);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
  FieldScript_WaitFrames(40);
  // Make room for Dora to walk and demonstrate that she feels well.
  // Isaac steps back as Dora gets up, turns about and jumps three times.
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  leader->facing = 0;
  WaitFrames(1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 558, 388);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(6) / 5, PIXELS(3) / 5);
  Import_ObjectEventSetAnimation(LOCALID_DORA, ANIM_DORA_STRAIGHTEN);
  ObjectMotion_SetPositionAndCommit(LOCALID_DORA, 586, 400);
  FieldScript_WaitFrames(40);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 580, 382);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_WEST, 40);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectMotion_ArmCallback(LOCALID_DORA, FACING_NORTH, 8);
  ObjectMotion_ArmCallback(LOCALID_DORA, FACING_EAST, 8);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 8);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_WEST, 10);
  ObjectMotion_Launch(LOCALID_DORA, 4, 20);
  ObjectMotion_Launch(LOCALID_DORA, 6, 40);
  ObjectMotion_Launch(LOCALID_DORA, 4, 20);
  // "See? I'm fine!" "Don't worry about me. Go on."
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 40);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 572, 384);
  FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, ANIM_NOD);
  GameFlag_Set(FLAG_VISITED_SICK_DORA);
  GameFlag_Set(FLAG_MAP_TEMP_03);
  FieldScript_End();
}

/// Talking to Dora at home in Vale, back from Kalay: in her sickbed she murmurs
/// Kyle's name; once up, she tells Isaac not to worry. Reached by talking to
/// her back from Kalay once FLAG_VISITED_SICK_DORA is set; she is up while
/// FLAG_MAP_TEMP_03 is set.
static void EventScript_TalkToDora(void) {
  s32 message;

  FieldScript_Start();
  if (GameFlag_Get(FLAG_MAP_TEMP_03) != 0) {
    ObjectEvent_SetTargetAndCallback(
        LOCALID_DORA, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
        gValeInteriorsInn_DoraFacesIsaacScript);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgValeDontWorryBest);
    FieldScript_ShowMessageAndWait(LOCALID_DORA, 0);
  } else {
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
    FieldScript_WaitFrames(40);
    message = MsgValeUnnOhhKyle;
    FieldScript_SetMessageId(message);
    FieldScript_ShowMessageThenWait(LOCALID_DORA, 0, 40);
    FieldMessage_Show((message + 1), 1);
  }
  FieldScript_End();
}

/// Restores the room's tiles and metatile behaviors outside the storm sequence.
/// Called by ValeInteriorsInn_Initialize when FLAG_VALE_STORM is clear.
static void ValeInteriorsInn_RestoreRoomPatch(void) {
  MapGrid_CopyCellsTo(22, 85, 25, 85, 2, 2);
  MapGrid_CopyCellAttributes(25, 15, 2, 2, 25, 25);
}

/// In Vale, the Mythril Bag rises and wobbles before Kraden, with no text.
/// Called by ValeInteriorsInn_Initialize on arrival by
/// VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG, from Kraden's cottage,
/// while he prepares for the first Sol Sanctum visit
/// (EventScript_TalkToKraden). Takes VALE_INTERIORS_INN_TO_VALE_KRADEN_SOUTH
/// back outside (VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH), where
/// EventScript_KradenSetsOff gathers the party.
static void EventScript_ShowMythrilBag(void) {
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  GlyphTransfer *glyph;
  u8 clear;

  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(480), PIXELS(202));
  WaitFrames(1);
  FieldCamera_FollowObjectEvent(LOCALID_KRADEN, 1);
  clear = 0;
  objectEvent =
      ObjectEvent_Create(SPRITE_BLANK, PIXELS(328), PIXELS(2), PIXELS(195));
  objectEvent->motionFlags = clear;
  sprite = objectEvent->sprite;
  objectEvent->y.fixed = PIXELS(5);
  sprite->count = clear;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  // Load the item icon into the temporary object event before animating the
  // bag.
  glyph = AllocHeapSlot(17, sizeof(*glyph));
  Icon_DrawItem(ITEM_MYTHRIL_BAG);
  VramCache_Load(sprite->slot, 128, glyph->tiles);
  FreeHeapSlot(17);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 2);
  FieldScript_StartScreenTransition();
  // Walk the presenter into view, then wait for both bag scripts to finish.
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 480, 176);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 420, 164);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 326, 185);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH, 10);
  ObjectEvent_SetScript(objectEvent, gValeInteriorsInn_MythrilBagRisesScript);
  Object_WaitForValue16(objectEvent);
  ObjectEvent_SetScript(objectEvent, gValeInteriorsInn_MythrilBagWobblesScript);
  Object_WaitForValue16(objectEvent);
  FieldScript_WaitFrames(20);
  ObjectEvent_Release(objectEvent);
  ObjectMotion_Launch(LOCALID_KRADEN, 2, 20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_EAST, 40);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(VALE_INTERIORS_INN_TO_VALE_KRADEN_SOUTH);
}

/// Unused: an update hook that flashes between two palettes and gives off a
/// pair of effects every sixteen frames; no object event is given it.
UNUSED static void
ValeInteriorsInn_FlashPaletteAndSpawnPair(union FieldObject *fieldObject) {
  if ((gFrameCount & 2) != 0) {
    ObjectEvent_SetColorMode(&fieldObject->objectEvent, 7);
  } else {
    ObjectEvent_SetColorMode(&fieldObject->objectEvent, 0);
  }
  if ((gFrameCount & 15) == 0) {
    ValeInteriorsInn_SpawnEffectPair(fieldObject);
  }
}

/// Cycles six palettes on odd frames and periodically emit paired effects.
static void
ValeInteriorsInn_CyclePaletteAndSpawnPair(union FieldObject *fieldObject) {
  if ((gFrameCount & 1) != 0) {
    ObjectEvent_SetColorMode(&fieldObject->objectEvent, (gFrameCount >> 1) % 6);
  }
  if ((gFrameCount & 15) == 0) {
    ValeInteriorsInn_SpawnEffectPair(fieldObject);
  }
}

/// Advances deterministically through six palettes on odd frames.
static void
ValeInteriorsInn_CyclePaletteOnOddFrames(union FieldObject *fieldObject) {
  if ((gFrameCount & 1) != 0) {
    s32 slot = (gFrameCount >> 1) % 6;

    ObjectEvent_SetColorMode(&fieldObject->objectEvent, slot);
  }
}

// Shared vertical-effect callbacks precede their paired-effect creator.
#include "common/effect_vertical.h"

/// Copies the parent's ground height into two oppositely moving vertical
/// effects.
static void ValeInteriorsInn_SpawnEffectPair(union FieldObject *fieldObject) {
  struct ObjectEvent *parent = &fieldObject->objectEvent;
  struct ObjectEvent *pair[2];
  struct ObjectEvent *child;
  struct Sprite *part;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 effectIndex;

  for (effectIndex = 0; effectIndex < 2; ++effectIndex) {
    child = ObjectEvent_Create(26, parent->x.fixed, parent->y.fixed,
                               parent->z.fixed);
    pair[effectIndex] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      part = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (part != NULL) {
        sprite = part;
        Sprite_StartAnim(part, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        part->image.size = 2;
        part->entries[0]->frame = 0;
      }
    }
  }
  {
    struct ObjectEvent *effectObjectEvent = pair[0];
    struct Sprite *effectSprite = effectObjectEvent->sprite;

    effectObjectEvent->update = FieldEffect_AnimateVerticalNegative;
    effectSprite->image.priority = 2;
  }
  {
    struct ObjectEvent *effectObjectEvent = pair[1];
    struct Sprite *effectSprite = effectObjectEvent->sprite;

    effectSprite->image.priority = 2;
    effectObjectEvent->update = FieldEffect_AnimateVerticalPositive;
    effectObjectEvent->priorityFlags = 2;
  }
}

// Shared Psynergy setup precedes the map's effect wrappers.
#include "common/effect_psynergy.h"

/// Unused: cycles Dora's palette with particles; nothing calls it.
UNUSED static void ValeInteriorsInn_CyclePaletteWithParticles(void) {
  ValeInteriorsInn_CyclePaletteAndSpawnPair(
      (union FieldObject *)ObjectEvent_GetById(LOCALID_DORA));
}

/// Unused: cycles Dora's palette without particles; nothing calls it.
UNUSED static void ValeInteriorsInn_CyclePaletteWithoutParticles(void) {
  ValeInteriorsInn_CyclePaletteOnOddFrames(
      (union FieldObject *)ObjectEvent_GetById(LOCALID_TUNIC));
}

/// Catch plays from Dora on the tunic, then its objects are released.
/// Called by EventScript_DoraWakesIsaac as Dora hands Isaac his tunic.
static void ValeInteriorsInn_DemonstrateCatch(void) {
  FieldEffect_LoadActionEffectResources(ABILITY_CATCH, 1);
  FieldEffect_SetupObjectPair(LOCALID_DORA, LOCALID_TUNIC);
  FieldEffect_StartCast();
  Field_DispatchTypeHandler(1);
  FieldEffect_StopCasterHook();
  FieldEffect_End();
}

s32 gValeInteriorsInn_LeaveHouseScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(522), 0, PIXELS(748)),
    SC_SYNC,         SC_MOVE(PIXELS(419), 0, PIXELS(748)),
    SC_SYNC,         SC_MOVE(PIXELS(419), 0, PIXELS(691)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValeInteriorsInn_DoraLeavesScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(387), 0, PIXELS(661)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gValeInteriorsInn_IsaacLeavesScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(408), 0, PIXELS(661)),
    SC_SYNC,         SC_MOVE(PIXELS(387), 0, PIXELS(661)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gValeInteriorsInn_DoraRunsAheadScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(468), 0, PIXELS(390)),
    SC_SYNC,         SC_MOVE(PIXELS(508), 0, PIXELS(402)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValeInteriorsInn_IsaacRunsAheadScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(431), 0, PIXELS(391)),
    SC_SYNC,         SC_MOVE(PIXELS(468), 0, PIXELS(390)),
    SC_SYNC,         SC_MOVE(PIXELS(486), 0, PIXELS(393)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValeInteriorsInn_DoraRunsBackScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(525), 0, PIXELS(414)),
    SC_SYNC,         SC_MOVE(PIXELS(509), 0, PIXELS(442)),
    SC_SYNC,         SC_MOVE(PIXELS(455), 0, PIXELS(450)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValeInteriorsInn_IsaacRunsBackScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(525), 0, PIXELS(414)),
    SC_SYNC,         SC_MOVE(PIXELS(509), 0, PIXELS(442)),
    SC_SYNC,         SC_MOVE(PIXELS(481), 0, PIXELS(443)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gValeInteriorsInn_RunToPlazaScript[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(428), 0, PIXELS(460)),
    SC_SYNC,         SC_MOVE(PIXELS(415), 0, PIXELS(513)),
    SC_SYNC,         SC_MOVE(PIXELS(415), 0, PIXELS(589)),
    SC_SYNC,         SC_END,
};

s32 gValeInteriorsInn_FlameShakesScript[] = {
    SC_SET(SW_SCALE_X, 0xe666),  SC_SET(SW_SCALE_Y, 0x10000), SC_WAIT(4),
    SC_SET(SW_SCALE_X, 0xcccc),  SC_SET(SW_SCALE_Y, 0xcccc),  SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeInteriorsInn_FlameRisesScript[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x13333),
    SC_WAIT(3),
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 1310),
    SC_ADD(SW_SCALE_Y, 1310),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_Y, -0x1999),
    SC_ADD(SW_TARGET_Y, -0x1999),
    SC_ADD(SW_SCALE_X, -655),
    SC_ADD(SW_SCALE_Y, 2621),
    SC_WAIT(1),
    SC_LOOP(14, SC_TO(2)),
    SC_SET(SW_Y, PIXELS(8)),
    SC_SET(SW_TARGET_Y, PIXELS(8)),
    SC_LABEL(3),
    SC_ADD(SW_Y, 0x7333),
    SC_ADD(SW_TARGET_Y, 0x7333),
    SC_ADD(SW_SCALE_X, 65),
    SC_ADD(SW_SCALE_Y, -1310),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(3)),
    SC_POSITION(0, 0, 0),
    SC_ANIMATION(1),
    SC_END,
};

s32 gValeInteriorsInn_DoraFacesIsaacScript[] = {
    SC_CALL(Map_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_MOVE(PIXELS(567), 0, PIXELS(690)),
    SC_SYNC,
    SC_MOVE(PIXELS(543), 0, PIXELS(674)),
    SC_SYNC,
    SC_END,
};

s32 gValeInteriorsInn_MythrilBagRisesScript[] = {
    SC_ADD(SW_Y, 0x8000),
    SC_WAIT(1),
    SC_LOOP(16, SC_TOP),
};

s32 gValeInteriorsInn_MythrilBagWobblesScript[] = {
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_END,
};

struct MapWarpDest gValeInteriorsInn_WarpDests[] = {
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHWEST, CONDITION_ALWAYS, 160, 0, 228,
     FACING_NORTH, 0, 24, 0, 248, 256, 0},
    {VALE_INTERIORS_INN_FROM_VALE_KRADEN_NORTH, CONDITION_ALWAYS, 480, 0, 176,
     FACING_NORTH, 0, 264, 8, 560, 240, 0},
    {VALE_INTERIORS_INN_FROM_VALE_GATE_NORTHEAST, CONDITION_ALWAYS, 721, 0, 192,
     FACING_NORTH, 0, 584, 0, 848, 224, 0},
    {VALE_INTERIORS_INN_FROM_VALE_GATE_WEST, CONDITION_ALWAYS, 144, 0, 494,
     FACING_NORTH, 0, 8, 256, 320, 528, 0},
    {VALE_INTERIORS_INN_INN_FROM_UPSTAIRS, CONDITION_ALWAYS, 119, 0, 340,
     FACING_SOUTH, 0, 8, 256, 320, 528, 0},
    {VALE_INTERIORS_INN_FROM_VALE, CONDITION_ALWAYS, 416, 0, 528, FACING_NORTH,
     0, 344, 272, 640, 560, 0},
    {VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS, CONDITION_ALWAYS, 408, 0,
     371, FACING_SOUTH, 0, 344, 272, 640, 560, 0},
    {VALE_INTERIORS_INN_INN_FROM_GROUND_FLOOR, CONDITION_ALWAYS, 184, 0, 633,
     FACING_EAST, 0, 8, 576, 320, 736, 0},
    {VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_GROUND_FLOOR, CONDITION_ALWAYS, 408,
     0, 665, FACING_EAST, 0, 328, 600, 624, 816, 0},
    {10, CONDITION_ALWAYS, 528, 0, 683, FACING_SOUTH, 0, 328, 600, 624, 816, 0},
    {VALE_INTERIORS_INN_FROM_VALE_STORM_WEST, CONDITION_ALWAYS, 705, 0, 817,
     FACING_NORTH, 0, 640, 552, 952, 864, 0},
    {VALE_INTERIORS_INN_FROM_VALE_STORM_EAST, CONDITION_ALWAYS, 879, 0, 765,
     FACING_NORTH, 0, 640, 552, 952, 864, 0},
    {16, CONDITION_ALWAYS, 538, 0, 683, FACING_SOUTH, 0, 328, 600, 624, 816, 0},
    {VALE_INTERIORS_INN_FROM_BABI_PALACE, CONDITION_ALWAYS, 760, 0, 328,
     FACING_SOUTH, 0, 672, 272, 960, 488, 0},
    {VALE_INTERIORS_INN_FROM_VALE_GATE_SOUTHEAST, CONDITION_ALWAYS, 567, 0, 676,
     FACING_EAST, 0, 1024, 576, 1024, 576, 0},
    {VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA, CONDITION_ALWAYS,
     408, 0, 371, FACING_SOUTH, 0, 344, 272, 640, 560, 0},
    {VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG, CONDITION_ALWAYS, 480, 0,
     176, FACING_NORTH, 0, 264, 8, 560, 240, 0},
    {MAP_TABLE_END},
};

u32 gValeInteriorsInn_Warps[] = {
    MAP_WARPS(ValeInteriorsInn),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_GATE_NORTHWEST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_INN_WEST),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_KRADEN_NORTH, ValeKraden,
             VALE_KRADEN_FROM_VALE_INTERIORS_INN_NORTH),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_GATE_NORTHEAST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_INN_NORTH),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_GATE_WEST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_INN_SOUTH),
    MAP_WARP(VALE_INTERIORS_INN_INN_TO_UPSTAIRS, ValeInteriorsInn,
             VALE_INTERIORS_INN_INN_FROM_GROUND_FLOOR),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE, Vale, VALE_FROM_VALE_INTERIORS_INN),
    MAP_WARP(VALE_INTERIORS_INN_ISAACS_HOUSE_TO_UPSTAIRS, ValeInteriorsInn,
             VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_GROUND_FLOOR),
    MAP_WARP(VALE_INTERIORS_INN_INN_TO_GROUND_FLOOR, ValeInteriorsInn,
             VALE_INTERIORS_INN_INN_FROM_UPSTAIRS),
    MAP_WARP(VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR, ValeInteriorsInn,
             VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_STORM_WEST, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_INN_WEST),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_STORM_EAST, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_INN_EAST),
    MAP_WARP(VALE_INTERIORS_INN_TO_BABI_PALACE, BabiPalace,
             BABI_PALACE_FROM_VALE_INTERIORS_INN),
    MAP_WARP(VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA,
             ValeInteriorsInn,
             VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_KRADEN_SOUTH, ValeKraden,
             VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH),
    MAP_WARPS_END,
};

u32 gValeInteriorsInn_StormWarps[] = {
    MAP_WARPS(ValeInteriorsInn),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_GATE_NORTHWEST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_INN_WEST),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_KRADEN_NORTH, ValeKraden,
             VALE_KRADEN_FROM_VALE_INTERIORS_INN_NORTH),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_GATE_NORTHEAST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_INN_NORTH),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_GATE_WEST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_INN_SOUTH),
    MAP_WARP(VALE_INTERIORS_INN_INN_TO_UPSTAIRS, ValeInteriorsInn,
             VALE_INTERIORS_INN_INN_FROM_GROUND_FLOOR),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_INN_ISAACS_HOUSE),
    MAP_WARP(VALE_INTERIORS_INN_ISAACS_HOUSE_TO_UPSTAIRS, ValeInteriorsInn,
             VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_GROUND_FLOOR),
    MAP_WARP(VALE_INTERIORS_INN_INN_TO_GROUND_FLOOR, ValeInteriorsInn,
             VALE_INTERIORS_INN_INN_FROM_UPSTAIRS),
    MAP_WARP(VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR, ValeInteriorsInn,
             VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_STORM_WEST, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_INN_WEST),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_STORM_EAST, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_INN_EAST),
    MAP_WARP(VALE_INTERIORS_INN_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA,
             ValeInteriorsInn,
             VALE_INTERIORS_INN_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA),
    MAP_WARP(VALE_INTERIORS_INN_TO_VALE_STORM_WITH_DORA, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_INN_WITH_DORA),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gValeInteriorsInn_ObjectEventTemplates[] = {
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { 0xd0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_BLUE_DRESS, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_FELIX_MASKED, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(54), 0, PIXELS(669), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_TUNIC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTHEAST, TALK_FACING_KEEP, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValeInteriorsInn_ObjectEventTemplatesAfterLeaving[] = {
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_KYLE, 0x0, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { 0xd0, 0x0, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_BLUE_DRESS, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValeInteriorsInn_ObjectEventTemplatesAfterKalay[] = {
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(586), 0, PIXELS(406), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_KYLE, 0x0, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { 0xd0, 0x0, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_BLUE_DRESS, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(373), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(378), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValeInteriorsInn_ScholarsObjectEventTemplates[] = {
    { SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(336), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_GLASSES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(864), 0, PIXELS(360), FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(904), 0, PIXELS(376), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_GLASSES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(728), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(832), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gValeInteriorsInn_StormEvents[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateNorthwest) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenNorth) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateNortheast) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateWest) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToInnUpstairs) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToVale) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToIsaacsHouseUpstairs) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToInnDownstairs) },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToIsaacsHouseDownstairs) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeStormWest) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeStormEast) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeWoodenBoxEmpty)) },
    { BG_EVENT(BG_EVENT_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { BG_EVENT(BG_EVENT_BARREL), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeInteriorsInn_Events[] = {
    { MAP_EVENT_TALK, LOCALID_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeMyFatherAlsoDiedInThat) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeMySonIsStillYoungSo) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, MSGNO(MsgValeOneOfTheMenFromThat) },
    { MAP_EVENT_TALK, LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeTwoOfThoseThreeGuestsStaying) },
    { MAP_EVENT_TALK, LOCALID_MASKED_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskIsaacHisBusiness) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateNorthwest) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenNorth) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateNortheast) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateWest) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToInnUpstairs) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToVale) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToIsaacsHouseUpstairs) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToInnDownstairs) },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToIsaacsHouseDownstairs) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeStormWest) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeStormEast) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), 100, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { BG_EVENT(BG_EVENT_BARREL), 102, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeInteriorsInn_EventsAfterLeaving[] = {
    { MAP_EVENT_TALK, LOCALID_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeComeBackAsAStrongWarrior) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIWantedJennaToBeMy) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskWhetherMaskedManWasFelix) },
    { MAP_EVENT_TALK, LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeWereASmallHotelAndWe) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeIWillThinkOfJennaAs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeNowThatJennaIsGoneWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, MSGNO(MsgValeIWonderIfFelixChangedMuch) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeFelixAndHisGangMustHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeWhoCouldHaveKnownThatMasked) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateNorthwest) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenNorth) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateNortheast) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateWest) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToInnUpstairs) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToVale) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToIsaacsHouseUpstairs) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToInnDownstairs) },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToIsaacsHouseDownstairs) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeStormWest) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeStormEast) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), 100, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { BG_EVENT(BG_EVENT_BARREL), 102, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeInteriorsInn_EventsAfterKalay[] = {
    { MAP_EVENT_TALK, LOCALID_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeOwItsGonnaBeYearsBefore) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHomeIsaacOhWhatHappened) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, MSGNO(MsgValeYouTraveledAroundTheEasternEdge) },
    { MAP_EVENT_TALK, LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeArentYouGladToSeeThe) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_MAN_ISAAC_HAS_GROWN, FLAG_VALE_INNKEEPER_WELCOMED_ISAAC, EVENT_SCRIPT(EventScript_RemarkIsaacHasGrown) },
    { MAP_EVENT_TALK, LOCALID_MAN_ISAAC_HAS_GROWN, CONDITION_ALWAYS, MSGNO(MsgValeYouveGrownSoMuch) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeIPromisedIdBecomeAGreat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeThatShyGirlWithIsaacSure) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, MSGNO(MsgValeIWonderIfIsaacWillContinue) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeValeWillNeverChangeSoYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_INNKEEPER, FLAG_VALE_INNKEEPER_WELCOMED_ISAAC, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadInnkeepersThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_MAN_ISAAC_HAS_GROWN, FLAG_VALE_MAN_SAW_ISAAC_GROWN, EVENT_SCRIPT(EventScript_RemarkIsaacHasGrown) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ISAAC_HAS_GROWN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadThoughtsAboutDorasIllness) },
    { MAP_EVENT_TALK, LOCALID_DORA, FLAG_VISITED_SICK_DORA, EVENT_SCRIPT(EventScript_VisitSickDora) },
    { MAP_EVENT_TALK, LOCALID_DORA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToDora) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_DORA, FLAG_VISITED_SICK_DORA, EVENT_SCRIPT(EventScript_VisitSickDora) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_DORA, FLAG_MAP_TEMP_03, MSGNO(MsgValeKyleWatchOutTheBoulderThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_DORA, CONDITION_ALWAYS, MSGNO(MsgValeImSorryISeemedSoHarsh) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateNorthwest) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenNorth) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateNortheast) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGateWest) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToInnUpstairs) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToVale) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToIsaacsHouseUpstairs) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToInnDownstairs) },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToIsaacsHouseDownstairs) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeStormWest) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeStormEast) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), 100, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { BG_EVENT(BG_EVENT_BARREL), 102, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { BG_EVENT(BG_EVENT_JAR), 108, FLAG_VALE_JAR_VIAL, FIND_ITEM(ITEM_VIAL) },
    { BG_EVENT(BG_EVENT_BARREL), 107, FLAG_VALE_BARREL_COINS, FIND_COINS(123) },
    { BG_EVENT(BG_EVENT_SILENT), 109, FLAG_VALE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { MAP_EVENTS_END },
};

struct MapEvent gValeInteriorsInn_ScholarsEvents[] = {
    { MAP_EVENT_TALK, LOCALID_SCHOLAR_ANCIENT_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiOriginallyThereWereAncientRuinsAt) },
    { MAP_EVENT_TALK, LOCALID_SCHOLAR_EXPERIMENT, CONDITION_ALWAYS, MSGNO(MsgBabiNoMatterHowManyTimesI) },
    { MAP_EVENT_TALK, LOCALID_SCHOLAR_SEALED_ALCHEMY, CONDITION_ALWAYS, MSGNO(MsgBabiItIsSaidThatLongAgo) },
    { MAP_EVENT_TALK, LOCALID_SCHOLAR_VENUS_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiOriginallyTheAncientVenusRuinsLay) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_BABIS_RESEARCH, CONDITION_ALWAYS, MSGNO(MsgBabiBabiMakesHisScholarsPerformAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCHOLAR_ANCIENT_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiNoOneHasEverReachedThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCHOLAR_EXPERIMENT, CONDITION_ALWAYS, MSGNO(MsgBabiStillMyExperimentsInAlchemyAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCHOLAR_SEALED_ALCHEMY, CONDITION_ALWAYS, MSGNO(MsgBabiKradensJourneyWasMeantToRevive) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCHOLAR_VENUS_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiTheVenusRuinsAreTheRemnants) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_BABIS_RESEARCH, CONDITION_ALWAYS, MSGNO(MsgBabiLordBabiExpectedGreatResultsFrom) },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 20, CONDITION_ALWAYS, VALE_INTERIORS_INN_TO_BABI_PALACE },
    { BG_EVENT(BG_EVENT_JAR), 103, FLAG_BABI_PALACE_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresABookEntitledEncyclopediaOf)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledExperimentsIn)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledStudyOf)) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeInteriorsInn_ScholarsLaterEvents[] = {
    { MAP_EVENT_TALK, LOCALID_SCHOLAR_ANCIENT_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiOnceBabiLighthouseIsCompleteId) },
    { MAP_EVENT_TALK, LOCALID_SCHOLAR_EXPERIMENT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ScholarAsksAboutKraden) },
    { MAP_EVENT_TALK, LOCALID_SCHOLAR_SEALED_ALCHEMY, CONDITION_ALWAYS, MSGNO(MsgBabiAlchemyTeachesThatTheFourElements) },
    { MAP_EVENT_TALK, LOCALID_SCHOLAR_VENUS_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiIfTheRiddleOfTheVenus) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_BABIS_RESEARCH, CONDITION_ALWAYS, MSGNO(MsgBabiImSureKradenWouldStopBy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCHOLAR_ANCIENT_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiTheRelationshipBetweenAlchemyAndThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCHOLAR_EXPERIMENT, CONDITION_ALWAYS, MSGNO(MsgBabiIWantToSeeKradenAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCHOLAR_SEALED_ALCHEMY, CONDITION_ALWAYS, MSGNO(MsgBabiTheresGotToBeAGood) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCHOLAR_VENUS_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiWhatSortOfUnfathomableRiddlesAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_BABIS_RESEARCH, CONDITION_ALWAYS, MSGNO(MsgBabiThereMustBeAReasonWhy) },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 20, CONDITION_ALWAYS, VALE_INTERIORS_INN_TO_BABI_PALACE },
    { BG_EVENT(BG_EVENT_JAR), 103, FLAG_BABI_PALACE_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresABookEntitledEncyclopediaOf)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledExperimentsIn)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledStudyOf)) },
    { MAP_EVENTS_END },
};
// clang-format on
