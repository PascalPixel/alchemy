/// @file
/// MapId_Vale, Isaac's village by day, outside the houses: the door of
/// Isaac's house, the houses of Garet's family and of Jenna's grandparents,
/// the Sanctum and the paths out to Mt. Aleph and Kraden's cottage. The
/// object event templates and map events come in three sets, chosen by the
/// story flags: before the party leaves Vale, after it has left
/// (FLAG_VALE_PARTY_LEFT) and back from Kalay (FLAG_KALAY_REACHED);
/// VALE_FROM_TITLE has a set of its own. In story order:
///
/// - The day the story begins, three years after the storm
///   (vale_interiors_inn.c), which took Jenna's family and Isaac's father,
///   Kyle. The game opens at VALE_FROM_TITLE with EventScript_MendingTheRoof:
///   under the caption "Three years later...", Isaac is mending the roof of his
///   house while Dora, his mother, calls up to him; Garet, who will travel with
///   him, trains his Psynergy nearby, and Jenna comes by, and the two speak of
///   that day before going to fetch Isaac to go to Kraden's.
///   VALE_TO_ROOF_MENDED brings the party back in at VALE_FROM_ROOF_MENDED,
///   where EventScript_RoofMended plays once (map section flag
///   FLAG_MAPSEC_TEMP_08): Dora praises the roof, remembers Kyle, and sends the
///   three off on their climb of Mt. Aleph with Kraden, the scholar who teaches
///   them Alchemy (FLAG_MAP_TEMP_02); for the rest of that visit Dora frets
///   about the hole clumsy Garet left.
/// - Vale before the party leaves. Until Kraden has planned the visit to Sol
///   Sanctum (FLAG_SANCTUM_VISIT_PLANNED), Jenna stops Isaac on the path to Mt.
///   Aleph; a bearded man in a long robe jumps out to warn him against sneaking
///   up the sacred peak (FLAG_VALE_WARNED_AGAINST_MT_ALEPH) and then keeps
///   watching him, asking again whenever he comes near (metatile behaviors 51
///   to 53). Three puppies (from LOCALID_FIRST_PUPPY) play; walking up to them
///   from either side (metatile behaviors 20 and 21) sets them barking, until
///   the party has met Saturos and Menardi, of the strangers staying at the inn
///   (FLAG_MET_SATUROS_AND_MENARDI); then the setup removes them. Villagers
///   talk of the mayor, Garet's grandfather, and of not going beyond Kraden's
///   cottage, Garet's sister scolds him, and Dora says Isaac can fix the hole
///   later.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT): Dora will not open
///   the door of Isaac's house to him ("My Isaac would keep his promise... Go
///   away."), and the villagers speak of the tremors and of Kraden, who has
///   not come back.
/// - Back from Kalay (FLAG_KALAY_REACHED): Garet meets his sister (map section
///   flag FLAG_MAPSEC_TEMP_03) and asks one of his family to protect it while
///   he is gone (FLAG_MAPSEC_TEMP_04), a young man asks whether Jenna is back
///   (FLAG_VALE_ASKED_IF_JENNA_IS_BACK), another feels a tremor, another asks
///   about the lack of travellers since the eruption (FLAG_MAPSEC_TEMP_07).
///   Isaac's house opens again: Dora lies ill inside (vale_interiors_inn.c).
///
/// In every set: a Nut to catch with Catch (FLAG_VALE_NUT_CAUGHT; LOCALID_NUT,
/// or LOCALID_NUT_AFTER_KALAY), a pillar to push or Move, whose column the
/// setup restores from FLAG_MAPSEC_TEMP_14 to _16, a ladder up onto the roof of
/// Isaac's house, climbed on metatile behaviors 100 and 101 (FLAG_MAP_TEMP_04
/// while on the roof), and cells of
/// an upper path that change while the leader is on it (FLAG_MAP_TEMP_10);
/// after a battle (FLAG_KEEP_PARTY_POSITION) the setup restores both. Warps:
/// VALE_TO_SOL_ENTRANCE up Mt. Aleph to Sol Entrance (MapId_SolEntrance),
/// VALE_TO_VALE_GATE to MapId_ValeGate, VALE_TO_VALE_KRADEN_NORTH and
/// VALE_TO_VALE_KRADEN_SOUTH to MapId_ValeKraden, the doors
/// VALE_TO_VALE_INTERIORS_SHOPS_EAST, VALE_TO_VALE_INTERIORS_SHOPS_SOUTH,
/// VALE_TO_VALE_INTERIORS_SHOPS_NORTH, VALE_TO_VALE_INTERIORS_SHOPS_WEST and
/// VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR into the houses
/// (MapId_ValeInteriorsShops, vale_interiors_shops.c),
/// VALE_TO_VALE_INTERIORS_INN into Isaac's house (MapId_ValeInteriorsInn) and
/// VALE_TO_SANCTUM into MapId_Sanctum. Two scenes no table reaches remain:
/// EventScript_UnusedRoofMended, a copy of the roof's end, and
/// EventScript_UnusedWaterBurstsByTheRiver, which sets map section flag
/// FLAG_MAPSEC_TEMP_05. FLAG_MAP_TEMP_41 pauses the man who watches Isaac while
/// a menu is open. The TBS_PATCH_4 editions also set Dora's sprite priority
/// before Kalay.
///
/// In the opening Garet trains his Psynergy on the pillar: his sprite flashes
/// white, then cycles through its colour modes with sparks about him, while
/// the hand of Move (LOCALID_OPENING_MOVE_HAND) pushes the pillar west, the
/// pillar cycling its colour modes too, and sheds sparkles
/// (https://alchemy.pascalpixel.com/places/vale/opening-move.gif: the screen,
/// Garet, the pillar and the hand boxed, beside the sprites alone and the 16
/// sprite palettes).
///
/// Open questions: none.

#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/map/vale.h"
#include "unit/map/common/object_orbit.h"
#include "unit/map/common/effect_psynergy.h"
#include "unit/map/common/world_map_linked_effects.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "global_cells.h"
#include "edition.h"
#include "vale.h"
#include "types.h"
#include "call.h"
#include "script_layer.h"
#include "field_service.h"
#include "map_tables.h"
#include "iwram_call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "object_event.h"
#include "field_effect.h"
#include "map_grid_scroll.h"
#include "fixed_math.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"

// Where Garet lands after his jump once Dora has gone in, at the roof's end
// (EventScript_RoofMended): two pixels further south in the TBS_PATCH_2
// editions.
#if TBS_PATCH_2
#define VALE_GARET_REUNION_Z 829
#else
#define VALE_GARET_REUNION_Z 827
#endif

// Where Dora stands by the house in the opening and before the party leaves:
// two pixels further north in the TBS_PATCH_4 editions.
#if TBS_PATCH_4
#define VALE_DORA_Z 887
#else
#define VALE_DORA_Z 889
#endif

static void Vale_HaveJennaStopTheAlephDetour(void);
static void Vale_AskAboutSneakingToAleph(const s32 *leaderActions,
                                         const s32 *guardActions);
static void Vale_MoveObjectEventAlongLowerZPath(s32 localId,
                                                s32 approachAnimationId,
                                                s32 departureAnimationId,
                                                s32 resumeMotion);
static void Vale_MoveObjectEventAlongHigherZPath(s32 localId,
                                                 s32 approachAnimationId,
                                                 s32 departureAnimationId,
                                                 s32 resumeMotion);
static void Vale_PlaceOrHideBoards(s32 first, u32 count, s32 mode);
static void EventScript_MendingTheRoof(void);
static void EventScript_RoofMended(void);
static void Vale_UpdateGaretPaletteAndEffects(void);
static void Vale_UpdateLeaderPaletteAndEffects(void);
static void Vale_CycleOpeningPillarPalette(void);
static void Vale_CycleFirstBoardPalette(void);
static void Vale_CycleSecondBoardPalette(void);
static void Vale_CycleThirdBoardPalette(void);
#ifdef UBFIX
static void Vale_HaveGuardWatchTheLeader(union FieldObject *fieldObject);
#else
static s32 Vale_HaveGuardWatchTheLeader(union FieldObject *fieldObject);
#endif
static void Vale_TrailSparkles(union FieldObject *fieldObject);
static void Vale_VanishMoveHand(struct ObjectEvent *emitter);
static void Vale_UpdateParticlePosition(struct ObjectEvent *particle,
                                        s32 distance, s32 angle);
static void EventScript_SettleMovedPillar(void);
static void Vale_PlayStepSound(void);
static void Vale_SynchronizeAlephGuardPause(void);
static void Vale_AdjustPillarPriorityForLeaderHeight(void);
static void
Vale_FlashObjectEventAndSpawnEffects(struct ObjectEvent *objectEvent);
static void
Vale_CycleObjectEventPaletteAndSpawnEffects(struct ObjectEvent *objectEvent);
static void Vale_CycleObjectEventPalette(struct ObjectEvent *objectEvent);

/// The placed object events of Vale by day, numbered in the order of
/// gVale_ObjectEventTemplates and gVale_ObjectEventTemplatesAfterLeaving, which
/// place the same people in the same rows. A row whose sprite is a party
/// member's (Garet's, Jenna's) takes that member's local id instead
/// (FieldScript_SpawnObjectEventTemplates), so the numbering skips it.
enum ValeLocalId {
  /// An old woman: "Mt. Aleph looks beautiful today. It's so peaceful."; after
  /// the party has left, "Vale is saved thanks to the Wise One!"
  /// ★ Named from the game: "Mt. Aleph looks beautiful today."
  /// (MsgValeMtAlephLooksBeautifulTodayIts)
  LOCALID_OLD_WOMAN_ADMIRING_MT_ALEPH = LOCALID_FIRST_PLACED,
  /// A woman in a green dress: "Vale stays pristine because of Mt. Aleph's
  /// purity."
  /// ★ Named from the game: "Vale stays pristine because of Mt. Aleph's
  /// purity." (MsgValeValeStaysPristineBecauseOfMt)
  LOCALID_WOMAN_ON_ALEPHS_PURITY,
  /// A little girl with orange pigtails: "I'm jealous of you, Garet, because
  /// your grandpa's the mayor." (EventScript_TalkAboutTheMayorAndTremors)
  /// ★ Named from the game: "I'm jealous of you,"
  /// (MsgValeYourGrandpaIsTheMayor)
  LOCALID_GIRL_JEALOUS_OF_GARET,
  /// A young man with brown hair: "This place looks just like it did three
  /// years ago... after the storm..."
  /// ★ Named from the game: "This place looks just like it did three years
  /// ago... after the storm..." (MsgValeThisPlaceLooksJustLikeIt)
  LOCALID_YOUNG_MAN_REMEMBERING_THE_STORM,
  /// A strongman by the river: "Isaac, watch out! Don't get too close to the
  /// river!"
  /// ★ Named from the game: "Don't get too close to the river!"
  /// (MsgValeIsaacWatchOutDontGetToo)
  LOCALID_STRONGMAN_BY_THE_RIVER,
  /// A burly grey-haired man: "Vale must remain hidden. Psynergy's secrets
  /// mustn't fall into the wrong hands."
  /// ★ Named from the game: "Vale must remain hidden."
  /// (MsgValeValeMustRemainHiddenPsynergysSecrets)
  LOCALID_MAN_KEEPING_VALE_HIDDEN,
  /// A woman with fair hair who stands by the way out to Kraden's cottage:
  /// "Do you need to go past here?", then "You mustn't go beyond Kraden's
  /// cottage." (EventScript_WarnAboutGoingBeyondKradensCottage)
  /// ★ Named from the game: "You mustn't go beyond Kraden's cottage."
  /// (MsgValeDontGoBeyondKradensCottage)
  LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN,
  /// The three puppies, small white dogs, removed once the party has met
  /// Saturos and Menardi. "Rrruff! Rrrruff!"
  /// ★ Named from what the player sees: small white dogs with brown ears (its
  /// render)
  LOCALID_FIRST_PUPPY,
  /// A young woman with an orange ponytail: "Garet, you make me so mad
  /// sometimes!" (EventScript_ArgueWithGaretsSister); Garet calls her "Sis"
  /// back
  /// from Kalay.
  /// ★ Named from the game: "Your sister's really mad!"
  /// (MsgValeYoureInTroubleNowGaretYour)
  LOCALID_GARETS_SISTER = LOCALID_FIRST_PUPPY + 3,
  /// "You're in trouble now, Garet... Your sister's really mad!"; after the
  /// party has left, "Please take care of my brother, Isaac."
  /// ★ Named from the game: "Please take care of my brother"
  /// (MsgValePleaseTakeCareOfMyBrother), said by a small boy with orange hair
  /// in a green shirt and shorts (its render)
  LOCALID_GARETS_BROTHER,
  /// SPRITE_STONE_PILLAR, as Goma Cave's pillars: pushed
  /// (EventScript_PushPillar) or
  /// moved with Move, its column kept in FLAG_MAPSEC_TEMP_14 to _16
  /// (EventScript_SettleMovedPillar).
  /// ★ Named from what the player sees: a stone pillar of brown rock standing
  /// below the cliff north of the village (its render)
  LOCALID_PILLAR,
  /// ★ Named from the game: "your mother, Dora, has been crying"
  /// (MsgValeDoraHasBeenCrying)
  LOCALID_DORA, ///< Isaac's mother
  /// A man with long brown hair and a beard in a long robe, who jumps out to
  /// warn Isaac off the sacred peak, then keeps watching him
  /// (EventScript_WarnAgainstSneakingUpMtAleph). "Mt. Aleph is a sacred peak.
  /// It is
  /// forbidden to climb it!"
  /// ★ Named from the game: "Mt. Aleph is a sacred peak."
  /// (MsgValeMtAlephIsASacredPeak)
  LOCALID_MAN_GUARDING_MT_ALEPH,
  /// The boards on the roof, sprite 0xce, which Isaac moves into place with
  /// Psynergy at the roof's end (EventScript_RoofMended).
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  LOCALID_FIRST_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  LOCALID_SECOND_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  LOCALID_THIRD_BOARD,
  /// ★ Named from the game: "Nut" (MsgItemName), the item Catch takes
  LOCALID_NUT ///< the Nut for Catch, until FLAG_VALE_NUT_CAUGHT is set
};

/// Back from Kalay, the placed object events of
/// gVale_ObjectEventTemplatesAfterKalay; Garet's row takes his own local id.
enum ValeAfterKalayLocalId {
  /// Garet's sister. "Ah! It's been a while... You're back?"
  /// (EventScript_GaretMeetsHisSister)
  /// ★ Named from the game: "Thanks, Sis." (MsgValeAh), Garet answering her
  LOCALID_GARETS_SISTER_AFTER_KALAY = LOCALID_FIRST_PLACED,
  /// "I will do my best to show everyone that I am strong enough to protect
  /// our family and Vale." (EventScript_GaretAsksToProtectFamily)
  /// ★ Named from the game: "Please take care of my brother"
  /// (MsgValePleaseTakeCareOfMyBrother)
  LOCALID_GARETS_BROTHER_AFTER_KALAY,
  /// The young man of LOCALID_YOUNG_MAN_REMEMBERING_THE_STORM: "Is Jenna back
  /// yet?" (EventScript_AskWhetherJennaHasReturned)
  /// ★ Named from the game: "Is back yet?" (MsgValeIsJennaBackYet)
  LOCALID_YOUNG_MAN_ASKING_FOR_JENNA,
  /// The strongman of LOCALID_STRONGMAN_BY_THE_RIVER: "I still remember when
  /// Felix and I used to play here...*Sniff*"
  /// ★ Named from the game: "I still remember when" (MsgValeIUsedToPlayHere)
  LOCALID_STRONGMAN_REMEMBERING_FELIX,
  /// Three sprites of water, CrossboneIsleFloodgate's water sprite 0xd6. Only
  /// EventScript_UnusedWaterBurstsByTheRiver shows them: during a
  /// tremor each in turn grows from a tenth of its size at (266, 1230), north
  /// of LOCALID_STRONGMAN_REMEMBERING_FELIX, and vanishes
  /// (gVale_UnusedWaterGrows). The row after them places the woman with fair
  /// hair of LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN at the origin, and nothing
  /// names her.
  /// ★ Named from what the player sees: a spout of white water spraying up,
  /// spr_214.png (its render)
  LOCALID_FIRST_WATER,
  /// An old woman: "Mt. Aleph's eruption was so powerful, it caused the
  /// continent to crack!"
  /// ★ Named from the game: "Mt. Aleph's eruption was so powerful"
  /// (MsgValeMtAlephsEruptionWasSoPowerful)
  LOCALID_OLD_WOMAN_ON_THE_ERUPTION = LOCALID_FIRST_WATER + 4,
  /// A woman in a green dress: "Aghh! I felt another one!"
  /// (EventScript_ReactToAnotherTremor)
  /// ★ Named from the game: "Aghh! I felt another one!"
  /// (MsgValeIFeltAnotherOne)
  LOCALID_WOMAN_FEELING_TREMORS,
  /// A little girl with orange pigtails: "They say the danger is gone, even
  /// though the earthquakes continue..."
  /// ★ Named from the game: "They say the danger is gone, even though the
  /// earthquakes continue..." (MsgValeTheySayTheDangerIsGone)
  LOCALID_GIRL_DOUBTING_SAFETY,
  /// A burly grey-haired man: "I wonder why we haven't seen any travelers
  /// since the eruption..." (EventScript_AskAboutTheLackOfTravelers)
  /// ★ Named from the game: "I wonder why we haven't seen any travelers
  /// since the eruption..." (MsgValeNoTravelersSinceTheEruption)
  LOCALID_MAN_MISSING_TRAVELERS,
  /// ★ Named from the game: "Nut" (MsgItemName)
  LOCALID_NUT_AFTER_KALAY,
  /// ★ Named from what the player sees: a stone pillar of brown rock standing
  /// below the cliff north of the village (its render)
  LOCALID_PILLAR_AFTER_KALAY ///< the pillar of LOCALID_PILLAR
};

/// The game's opening (EventScript_MendingTheRoof), the placed object events of
/// gVale_RepairObjectEventTemplates; Jenna's and Garet's rows take their own
/// local ids.
enum ValeOpeningLocalId {
  /// A woman in a green dress who meets Jenna on her way.
  /// ★ Named from what the player sees: a woman with brown hair in a green
  /// dress and a white apron (its render)
  LOCALID_OPENING_WOMAN = LOCALID_FIRST_PLACED,
  /// The pillar of LOCALID_PILLAR, which Garet trains his Psynergy on.
  /// ★ Named from what the player sees: a stone pillar of brown rock standing
  /// below the cliff north of the village (its render)
  LOCALID_OPENING_PILLAR,
  /// ★ Named from the game: "your mother, Dora, has been crying"
  /// (MsgValeDoraHasBeenCrying)
  LOCALID_OPENING_DORA, ///< calls up to Isaac on the roof
  /// The boards on the roof, as LOCALID_FIRST_BOARD.
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  LOCALID_OPENING_FIRST_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  LOCALID_OPENING_SECOND_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  LOCALID_OPENING_THIRD_BOARD,
  /// Sprite 0xd7, the hand of Move (psynergy_effects/catch.c): it pushes the
  /// pillar for Garet, shedding sparkles.
  /// ★ Named from what the player sees: a large open hand,
  /// spr_215.png (its render)
  LOCALID_OPENING_MOVE_HAND
};

/// The leader's animation of hammering on the roof, beyond enum
/// ObjectEventAnimation's.
enum { ANIMATION_HAMMER = 11 };

/// Animations of Dora's and Garet's sprites beyond enum ObjectEventAnimation,
/// as their animation tables and frames under graphics/sprite draw them.
enum ValeAnimation {
  ANIMATION_DORA_HANDS_UP = 7, ///< both hands raised (spr_031.png)
  /// Only her head and hands showing, as she peeks back out (spr_031.png).
  ANIMATION_DORA_PEEK = 8,
  /// Garet throwing up his arms, then crouching, as he jumps in fright
  /// (spr_001.png).
  ANIMATION_GARET_STARTLED = 13
};

#include "common/object_staged_object_event.h"

const struct MapWarpDest *Vale_GetWarpDests(void) { return gVale_WarpDests; }

const struct MapWarpRegion *Vale_GetRegions(void) { return NULL; }

const u32 *Vale_GetWarps(void) { return gVale_Warps; }

/// Places the villagers for the story so far, or for the roof repair
/// (VALE_FROM_TITLE).
const struct ObjectEventTemplate *Vale_GetObjectEventTemplates(void) {
  u8 *gameStateBytes = (u8 *)&gGameState;
  if (((struct GameState *)gameStateBytes)->warpDest == VALE_FROM_TITLE)
    return gVale_RepairObjectEventTemplates;
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0)
    return gVale_ObjectEventTemplatesAfterKalay;
  if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0)
    return gVale_ObjectEventTemplatesAfterLeaving;
  return gVale_ObjectEventTemplates;
}

/// Catch on the Nut in Vale: the party gets it.
static void EventScript_CatchNut(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetPosition(LOCALID_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_VALE_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_ScriptEnd();
}

/// The same Nut after Kalay, where it is LOCALID_NUT_AFTER_KALAY.
static void EventScript_CatchNutAfterKalay(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetPosition(LOCALID_NUT_AFTER_KALAY, 0, 0);
  Engine_GameFlagSet(FLAG_VALE_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_ScriptEnd();
}

/// Vale's map events for the story so far: before leaving, after leaving, or
/// back from Kalay.
const struct MapEvent *Vale_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0)
    return gVale_EventsAfterKalay;
  if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0)
    return gVale_EventsAfterLeaving;
  return gVale_Events;
}

/// A little girl in Vale (LOCALID_GIRL_JEALOUS_OF_GARET) asks about the mayor
/// before the party leaves, and speaks of the tremors after.
static void EventScript_TalkAboutTheMayorAndTremors(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0) {
    Engine_ScriptSetMessage(MsgValeTheGroundStillShakes);
    Engine_ScriptShowMessage(LOCALID_GIRL_JEALOUS_OF_GARET, 0);
  } else {
    Engine_ScriptSetMessage(MsgValeYourGrandpaIsTheMayor);
    Engine_ObjectEventFaceEachOther(LOCALID_GIRL_JEALOUS_OF_GARET,
                                    LOCALID_PARTY_LEADER, 4);
    Engine_ScriptAskYesNo(LOCALID_GIRL_JEALOUS_OF_GARET, 0);
  }
  Engine_ScriptEnd();
}

/// A woman in Vale (LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN) asks whether Isaac
/// is going to Kraden's cottage and warns him not to go beyond it; after the
/// party has left, says Kraden has not come back.
static void EventScript_WarnAboutGoingBeyondKradensCottage(void) {
  s32 flag = FLAG_VALE_ASKED_IF_GOING_TO_KRADEN;
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0) {
    Engine_ScriptSetMessage(MsgValeKradenHasntComeBack);
    Engine_ScriptShowMessage(LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  } else if (Engine_GameFlagIsSet(flag) == 0) {
    Engine_GameFlagSet(flag);
    Engine_ScriptSetMessage(MsgValeDoYouNeedToGo);
    Engine_ObjectEventFaceEachOther(LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN,
                                    LOCALID_PARTY_LEADER, 4);
    Engine_ScriptAskYesNo(LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  } else {
    Engine_ScriptSetMessage(MsgValeDontGoBeyondKradensCottage);
    Engine_ObjectEventFaceEachOther(LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN,
                                    LOCALID_PARTY_LEADER, 4);
    Engine_ScriptShowMessage(LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  }
  Engine_ScriptEnd();
}

/// Garet's sister scolds him in Vale; afterwards she only threatens him.
/// "Garet, you make me so mad sometimes!"
static void EventScript_ArgueWithGaretsSister(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS) == 0) {
    GameFlag_Set(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS);
    Engine_ScriptSetMessage(MsgValeYouMakeMeSoMad);
    ObjectEvent_ShowEmote(LOCALID_GARETS_SISTER,
                          EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, LOCALID_GARETS_SISTER,
                                    20);
    Script_ShowMessageAndWait(LOCALID_GARETS_SISTER, 0, 6);
    ObjectEvent_FaceDirection(LOCALID_GARETS_SISTER, FACING_WEST, 30);
    Engine_ObjectEventJump(LOCALID_GARETS_SISTER, 2, 20);
    Script_ShowMessageAndWait(LOCALID_GARETS_SISTER, 0, 6);
    Engine_ObjectEventFaceEachOther(LOCALID_GARETS_SISTER, LOCALID_PARTY_LEADER,
                                    10);
    ObjectEvent_ShowEmote(LOCALID_GARETS_SISTER,
                          EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Script_ShowMessageAndWait(LOCALID_GARETS_SISTER, 0, 10);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          60);
  } else {
    ObjectEvent_ShowEmote(LOCALID_GARETS_SISTER,
                          EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Engine_ScriptSetMessage(MsgValeIllGetYouForMy);
    Script_ShowMessageAndWait(LOCALID_GARETS_SISTER, 0, 20);
  }
  Engine_ScriptEnd();
}

/// Talking to Dora in Vale before the party leaves: right after the roof is
/// mended (FLAG_MAP_TEMP_02) she frets about the hole clumsy Garet left;
/// otherwise, since it will not rain for a while, Isaac can fix it later.
static void EventScript_DoraFretsAboutTheRoof(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_02) != 0) {
    Engine_ScriptSetMessage(MsgValeIToldGaretItWas);
  } else {
    Engine_ScriptSetMessage(MsgValeItWontRainForSome);
  }
  Engine_ScriptShowMessage(LOCALID_DORA, 0);
  Engine_ScriptEnd();
}

/// Back from Kalay, a young man in Vale (LOCALID_YOUNG_MAN_ASKING_FOR_JENNA)
/// asks whether Jenna is back.
static void EventScript_AskWhetherJennaHasReturned(void) {
  Engine_ScriptStart();
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER,
                                  LOCALID_YOUNG_MAN_ASKING_FOR_JENNA, 20);
  Engine_ScriptSetMessage(MsgValeIsJennaBackYet);
  Engine_ScriptShowMessage(LOCALID_YOUNG_MAN_ASKING_FOR_JENNA, 0);
  Engine_GameFlagSet(FLAG_VALE_ASKED_IF_JENNA_IS_BACK);
  Engine_ScriptEnd();
}

/// Back from Kalay, a woman in Vale (LOCALID_WOMAN_FEELING_TREMORS) feels
/// another tremor, and the view shakes.
static void EventScript_ReactToAnotherTremor(void) {
  Engine_ScriptStart();
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_TaskWait(10);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_ScriptSetMessage(MsgValeIFeltAnotherOne);
  Script_ShowMessageAndWait(LOCALID_WOMAN_FEELING_TREMORS, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_WOMAN_FEELING_TREMORS,
                              LOCALID_PARTY_LEADER, 20);
  Script_ShowMessage(LOCALID_WOMAN_FEELING_TREMORS, 0);
  Engine_ScriptEnd();
}

/// A burly grey-haired man asks about the lack of travellers since the
/// eruption. He (LOCALID_MAN_MISSING_TRAVELERS) is in Vale back from Kalay, and
/// the talk sets map section flag FLAG_MAPSEC_TEMP_07.
static void EventScript_AskAboutTheLackOfTravelers(void) {
  Engine_ScriptStart();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MAN_MISSING_TRAVELERS, 2);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceObjectEvent(LOCALID_MAN_MISSING_TRAVELERS,
                                    LOCALID_PARTY_LEADER, 20);
  Engine_ScriptSetMessage(MsgValeNoTravelersSinceTheEruption);
  Engine_ScriptAskYesNo(LOCALID_MAN_MISSING_TRAVELERS, 0);
  Engine_GameFlagSet(FLAG_MAPSEC_TEMP_07);
  Engine_ScriptEnd();
}

/// Walking onto the upper path in Vale (metatile behavior 15): its cells'
/// metatile behaviors change, remembered in FLAG_MAP_TEMP_10.
static void EventScript_SetPathCellOverride(void) {
  Engine_GameFlagSet(FLAG_MAP_TEMP_10);
  MapGrid_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

/// Walking off the upper path in Vale (metatile behavior 14) restores its
/// cells' metatile behaviors.
static void EventScript_ClearPathCellOverride(void) {
  Engine_GameFlagClear(FLAG_MAP_TEMP_10);
  MapGrid_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

/// The door of Vale Sanctum (metatile behavior 1): it opens, the leader walks
/// in, and VALE_TO_SANCTUM leads into the Sanctum.
static void EventScript_EnterSanctum(void) {
  Script_PlaySong(SE_188);
  MapGrid_AnimateCells(gVale_SanctumDoorOpens, 45, 11);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 257, 420);
  Engine_ScriptRequestWarp(VALE_TO_SANCTUM);
}

/// The path to Mt. Aleph in Vale (metatile behavior 2): Jenna stops Isaac until
/// Kraden has planned the visit (FLAG_SANCTUM_VISIT_PLANNED); then
/// VALE_TO_SOL_ENTRANCE leads up to Sol Entrance. Jenna says "Kraden is waiting
/// for us".
static void EventScript_ApproachMtAleph(void) {
  if (Engine_GameFlagIsSet(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
    Vale_HaveJennaStopTheAlephDetour();
  } else {
    Engine_AudioPlayCue(SE_MAP_EXIT);
    Engine_ScriptRequestWarp(VALE_TO_SOL_ENTRANCE);
  }
}

/// Metatile behavior 3 in Vale takes the northern way out to MapId_ValeKraden.
static void EventScript_WarpToValeKradenNorth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(VALE_TO_VALE_KRADEN_NORTH);
}

/// Metatile behavior 4 in Vale takes the southern way out to MapId_ValeKraden.
static void EventScript_WarpToValeKradenSouth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(VALE_TO_VALE_KRADEN_SOUTH);
}

/// Metatile behavior 5 in Vale takes the way out to MapId_ValeGate.
static void EventScript_WarpToValeGate(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(VALE_TO_VALE_GATE);
}

/// The door of the house of Garet's family in Vale (metatile behavior 6): it
/// opens and VALE_TO_VALE_INTERIORS_SHOPS_EAST leads in.
static void EventScript_EnterGaretsHouse(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_IsaacsDoorOpens, 54, 32);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 406, 727);
  Engine_ScriptRequestWarp(VALE_TO_VALE_INTERIORS_SHOPS_EAST);
}

/// A house door in Vale (metatile behavior 7): it opens and
/// VALE_TO_VALE_INTERIORS_SHOPS_SOUTH leads in, to the room ValeInteriorsShops
/// enters from VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH.
static void EventScript_EnterValeInteriorsShops(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 45, 39);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 262, 805);
  Engine_ScriptRequestWarp(VALE_TO_VALE_INTERIORS_SHOPS_SOUTH);
}

/// The door of Isaac's house in Vale (metatile behavior 8): after the party has
/// left and before Kalay, Dora will not open. "My Isaac would keep his
/// promise... Go away." Otherwise VALE_TO_VALE_INTERIORS_INN leads in.
static void EventScript_EnterIsaacsHouse(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0 &&
      Engine_GameFlagIsSet(FLAG_KALAY_REACHED) == 0) {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgValeYouCantBeIsaac);
    Engine_ScriptOpenMessage(LOCALID_DORA, 0);
    if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
      Engine_ScriptShowMessageAndWait(LOCALID_DORA, 0, 60);
      Engine_ScriptShowMessage(LOCALID_DORA, 0);
    } else {
      u16 *message = &gFieldWork->message;

      *message = *message + 2;
      Engine_ScriptWait(40);
      Engine_ScriptShowMessage(LOCALID_DORA, 0);
    }
    Engine_ScriptEnd();
  } else {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapGridAnimateCells(gVale_IsaacsDoorOpens, 50, 44);
    Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 340, 888);
    Engine_ScriptRequestWarp(VALE_TO_VALE_INTERIORS_INN);
  }
}

/// The door of the house of Jenna's grandparents in Vale (metatile behavior 9):
/// it opens and VALE_TO_VALE_INTERIORS_SHOPS_NORTH leads in.
static void EventScript_EnterJennasGrandparentsHouse(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 49, 69);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 326, 1126);
  Engine_ScriptRequestWarp(VALE_TO_VALE_INTERIORS_SHOPS_NORTH);
}

/// A house door in Vale (metatile behavior 10): it opens and
/// VALE_TO_VALE_INTERIORS_SHOPS_WEST leads in, to the room ValeInteriorsShops
/// enters from VALE_INTERIORS_SHOPS_FROM_VALE_WEST.
static void EventScript_EnterValeInteriorsShopsWest(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_WestHouseDoorOpens, 52, 76);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 374, 1238);
  Engine_ScriptRequestWarp(VALE_TO_VALE_INTERIORS_SHOPS_WEST);
}

/// Unused: its row answers metatile behavior 11, which no cell of Vale's layout
/// (layout/layout_000, the stormy night's too) carries, as
/// EventScript_UnusedLeaveToValeInteriorsShopsWestBySecondDoor in vale_storm.c.
/// It would have opened Isaac's door, walked the leader to the spot before it
/// and taken VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR into the house of
/// Jenna's grandparents, whose way out returns by the other door.
static void EventScript_UnusedEnterJennasGrandparentsHouseBySecondDoor(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_IsaacsDoorOpens, 35, 74);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 102, 1206);
  Engine_ScriptRequestWarp(VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR);
}

/// On the path to Mt. Aleph in Vale, before the visit is planned: Jenna comes
/// out to remind Isaac that Kraden is waiting, and he turns back.
static void Vale_HaveJennaStopTheAlephDetour(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *jenna;
  struct ObjectEvent *objectEvent;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  jenna = ObjectEvent_GetById(JENNA);
  Engine_ScriptStart();
  jenna->x.fixed = leader->x.fixed;
  jenna->y.fixed = leader->y.fixed;
  jenna->z.fixed = leader->z.fixed;
  jenna->targetX = OBJ_EVENT_NO_TARGET;
  jenna->targetY = OBJ_EVENT_NO_TARGET;
  jenna->targetZ = OBJ_EVENT_NO_TARGET;
  SET_VELOCITY(jenna, 0, 0, 0);
  jenna->groundHeight = leader->y.fixed;
  Engine_TaskWait(1);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(JENNA, 110, 283);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, JENNA, 2);
  Engine_ScriptSetMessage(MsgValeKradenIsWaitingForUs);
  if (leader->x.fixed < jenna->x.fixed) {
    Script_ShowMessageAndWait(
        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | JENNA, 0, 2);
  } else {
    Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | JENNA, 0, 2);
  }
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(2);
  Engine_ObjectEventSetAnimation(JENNA, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetDestination(JENNA, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(JENNA);
  ObjectEvent_SetPosition(JENNA, 0, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 110, 303);
  Engine_ScriptEnd();
}

/// The puppies in Vale (metatile behavior 20), until the party has met Saturos
/// and Menardi: they bark, the camera pans down to them and back, and the
/// leader turns away.
static void EventScript_WatchThePuppies(void) {
  struct MapGridScrollWork *grid;
  s32 *savedOrigin;
  struct ObjectEvent *leader;
  s32 frame;
  s32 cameraPosition[3];

  if (Engine_GameFlagIsSet(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    grid = gMapGridWork[0];
    Engine_ScriptStart();
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
    Engine_ScriptWait(2);
    Engine_ScriptSetMessage(MsgValeRrruffRrrruff);
    Engine_ScriptShowMessageAndWait(LOCALID_FIRST_PUPPY, 0, 2);
    Engine_ScriptShowMessageAndWait(LOCALID_FIRST_PUPPY + 1, 0, 2);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = grid->origin;
    grid->origin = cameraPosition;
    frame = 0;
    // Pan down to the puppies while the leader stays in place.
    do {
      cameraPosition[2] += PIXELS(2);
      Engine_ScriptWait(1);
      frame++;
      Engine_MapGridRedraw();
    } while (frame != 40);
    Engine_ScriptWait(60);
    Engine_MessageShowCentered(MsgValePuppiesPlayingOver, 1);
    frame = 0;
    Engine_ScriptWait(6);
    do {
      cameraPosition[2] -= PIXELS(2);
      Engine_ScriptWait(1);
      frame++;
      Engine_MapGridRedraw();
    } while (frame != 40);
    // Return camera ownership to the normal follow target.
    grid->origin = savedOrigin;
    Engine_ScriptWait(60);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 70, 741);
    Engine_ScriptEnd();
  }
}

/// The puppies in Vale from the other side (metatile behavior 21): the same
/// barking, without the camera pan.
static void EventScript_TurnBackFromThePuppies(void) {
  s32 msg;

  if (GameFlag_IsSet(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    Engine_ScriptStart();
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    msg = MsgValeRrruffRrrruff;
    Engine_ScriptSetMessage(msg);
    Script_ShowMessageAndWait(LOCALID_FIRST_PUPPY, 0, 2);
    Script_ShowMessageAndWait(LOCALID_FIRST_PUPPY + 1, 0, 2);
    Engine_MessageShowCentered(msg + 2, 1);
    Engine_ScriptWait(6);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 69, 870);
    Engine_ScriptEnd();
  }
}

/// Metatile behavior 100 in Vale, at the foot of the ladder beside Isaac's
/// house: walking north into it climbs the leader up onto the roof, whose cells
/// change so it can be walked (FLAG_MAP_TEMP_04 set while there).
static void EventScript_ClimbLadderOntoRoof(void) {
  Engine_ScriptStart();
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Vale_MoveObjectEventAlongLowerZPath(LOCALID_PARTY_LEADER, 10, 11, 1);
  Engine_GameFlagSet(FLAG_MAP_TEMP_04);
  Engine_ScriptEnd();
}

/// Metatile behavior 101 in Vale, at the top of the ladder beside Isaac's
/// house: walking south into it climbs the leader back down, and the roof's
/// cells are put back (FLAG_MAP_TEMP_04 cleared).
static void EventScript_ClimbDownFromRoof(void) {
  Engine_ScriptStart();
  Vale_MoveObjectEventAlongHigherZPath(LOCALID_PARTY_LEADER, 13, 10, 1);
  Engine_GameFlagClear(FLAG_MAP_TEMP_04);
  MapGrid_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  Engine_ScriptEnd();
}

/// A bearded man in a long robe in Vale (LOCALID_MAN_GUARDING_MT_ALEPH,
/// metatile behavior 50) jumps out in front of Isaac, then keeps watching him;
/// sets FLAG_VALE_WARNED_AGAINST_MT_ALEPH. "You're not planning on sneaking up
/// to Mt. Aleph...?"
static void EventScript_WarnAgainstSneakingUpMtAleph(void) {
  struct ObjectEvent *guard;
  struct ObjectEvent *objectEvent;

  guard = ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH);
  Engine_ScriptStart();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 5, 0);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 215, 403);
  guard->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetPosition(LOCALID_MAN_GUARDING_MT_ALEPH, PIXELS(166),
                          PIXELS(375));
  ObjectEvent_FaceDirection(LOCALID_MAN_GUARDING_MT_ALEPH, FACING_SOUTHEAST,
                            20);
  guard->steeringFlags = (guard->steeringFlags ^ 1);
  ObjectEvent_SetSpeed(LOCALID_MAN_GUARDING_MT_ALEPH, PIXELS(5) / 2,
                       PIXELS(5) / 2);
  Engine_ObjectEventJump(LOCALID_MAN_GUARDING_MT_ALEPH, 4, 0);
  ObjectEvent_WalkToAndWait(LOCALID_MAN_GUARDING_MT_ALEPH, 202, 395);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MAN_GUARDING_MT_ALEPH,
                            FACING_SOUTHEAST + FACING_STEP, 24);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_MAN_GUARDING_MT_ALEPH, PIXELS(3) / 2, PIXELS(1));
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gVale_FirstWarningLeaderPath);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_MAN_GUARDING_MT_ALEPH,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  ObjectEvent_SetActionScript(LOCALID_MAN_GUARDING_MT_ALEPH,
                              gVale_FirstWarningGuardPath);
  ObjectEvent_RefreshSelectorById(LOCALID_PARTY_LEADER);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 256, 474);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_RefreshSelectorById(LOCALID_MAN_GUARDING_MT_ALEPH);
  ObjectEvent_WalkToAndWait(LOCALID_MAN_GUARDING_MT_ALEPH, 256, 456);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  ObjectEvent_FaceDirection(LOCALID_MAN_GUARDING_MT_ALEPH, FACING_SOUTH, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_MAN_GUARDING_MT_ALEPH, 2);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgValeNotSneakingUpMtAleph);
  Script_AskYesNo(LOCALID_MAN_GUARDING_MT_ALEPH, 0);
  objectEvent = ObjectEvent_Get(LOCALID_MAN_GUARDING_MT_ALEPH);
#ifdef UBFIX
  objectEvent->update = Vale_HaveGuardWatchTheLeader;
#else
  objectEvent->update =
      (void (*)(union FieldObject *))Vale_HaveGuardWatchTheLeader;
#endif
  ObjectEvent_SetActionScript(LOCALID_MAN_GUARDING_MT_ALEPH,
                              gVale_AlephGuardPatrol);
  GameFlag_Set(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
  Engine_ScriptEnd();
}

/// Repeats the warning when the man reaches the first pause.
static void EventScript_RepeatMtAlephWarningFromFirstApproach(void) {
  struct ObjectEvent *guard =
      ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH);
  if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (guard->timer == 1) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderFromWestPath,
                                   gVale_GuardFromFirstPausePath);
    }
  }
}

/// Asks about the Aleph detour, then resumes each object event's response
/// actions.
static void Vale_AskAboutSneakingToAleph(const s32 *leaderActions,
                                         const s32 *guardActions) {
  const s32 *replyActions;
  struct ObjectEvent *guard;
  struct ObjectEvent *objectEvent;

  replyActions = guardActions;
  guard = ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH);
  Engine_ScriptStart();
  Engine_ObjectEventStartRepeatedMotion(LOCALID_MAN_GUARDING_MT_ALEPH, 2);
  ObjectEvent_ShowEmote(LOCALID_MAN_GUARDING_MT_ALEPH,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, leaderActions);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_MAN_GUARDING_MT_ALEPH,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  ObjectEvent_RunActionScriptAndWait(LOCALID_MAN_GUARDING_MT_ALEPH,
                                     replyActions);
  ObjectEvent_RefreshSelectorById(LOCALID_PARTY_LEADER);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MAN_GUARDING_MT_ALEPH, 2);
  guard->scaleX = Q16_ONE;
  guard->scaleY = Q16_ONE;
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->scaleX = Q16_ONE;
  objectEvent->scaleY = Q16_ONE;
  Engine_ScriptSetMessage(MsgValeNotSneakingUpMtAleph);
  Script_AskYesNo(LOCALID_MAN_GUARDING_MT_ALEPH, 0);
  objectEvent = ObjectEvent_Get(LOCALID_MAN_GUARDING_MT_ALEPH);
#ifdef UBFIX
  objectEvent->update = Vale_HaveGuardWatchTheLeader;
#else
  objectEvent->update =
      (void (*)(union FieldObject *))Vale_HaveGuardWatchTheLeader;
#endif
  ObjectEvent_SetActionScript(LOCALID_MAN_GUARDING_MT_ALEPH,
                              gVale_AlephGuardPatrol);
  Engine_ScriptEnd();
}

/// Repeats the warning when the man reaches the second pause.
static void EventScript_RepeatMtAlephWarningFromSecondApproach(void) {
  struct ObjectEvent *guard =
      ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH);
  if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (guard->timer == 2) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderPushedBackPath,
                                   gVale_GuardFromSecondPausePath);
    }
  }
}

/// Chooses the warning approach matching the man's current pause.
static void EventScript_RepeatMtAlephWarningAtEitherPause(void) {
  struct ObjectEvent *guard =
      ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH);
  if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    s32 pause;
    pause = guard->timer;
    if (pause == 1) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderPushedBackPath,
                                   gVale_GuardFromFirstPausePath);
    } else if (pause == 2) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderPushedBackPath,
                                   gVale_GuardFromSecondPausePath);
    }
  }
}

/// Unused: another cut of EventScript_RoofMended, the end of the morning the
/// story begins.
///
/// No table names it. It plays the same lines as the scene the game uses,
/// `Good job, Garet.` and `I was devastated when Kyle died...` among them, but
/// frames them with its own camera and map grid cells, and sets
/// FLAG_MAP_TEMP_02 as that scene does.
UNUSED static void EventScript_UnusedRoofMended(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  u8 leaderMotionFlags;
  u8 *leaderMotion;
  s32 i;

  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  objectEvent = Engine_ScriptGetViewCenter();
  objectEvent->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
  Engine_ScriptWait(1);
  Engine_MapGridRedraw();
  MapGrid_CopyCellAttributes(49, 41, 7, 3, 20, 50);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  leaderMotion = &leader->motionFlags;
  leaderMotionFlags = *leaderMotion;
  *leaderMotion = 0;
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(407), PIXELS(690));
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(392), PIXELS(896));
  ObjectEvent_SetPosition(GARET, PIXELS(298), PIXELS(736));
  ObjectEvent_SetPosition(JENNA, PIXELS(298), PIXELS(760));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  Vale_PlaceOrHideBoards(LOCALID_FIRST_BOARD, 2, 1);
  gFieldWork->transition = 0;
  gFieldWork->transitionFrames = 32;
  Engine_ScriptOpenScreen();
  ObjectEvent_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetActionScript(JENNA, gVale_JennaWalksToTheHouse);
  ObjectEvent_SetActionScript(GARET, gVale_GaretWalksToTheHouse);

  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 404, 843);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(20);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_SetChildValue(LOCALID_FIRST_BOARD, 2);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_BOARD, 390, 832);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_BOARD, 402, 828);
  Engine_ScriptWait(80);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_SetChildValue(LOCALID_FIRST_BOARD, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BOARD, PIXELS(390), PIXELS(842));
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  Engine_ScriptWait(200);
  MapGrid_CopyCellsTo(7, 102, 84, 41, 2, 1);

  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 377, 843);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 20);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_SetChildValue(LOCALID_FIRST_BOARD, 2);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_BOARD, 390, 832);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_BOARD, 377, 828);
  Engine_ScriptWait(80);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_SetChildValue(LOCALID_FIRST_BOARD, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BOARD, 0, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  Engine_ScriptWait(200);
  MapGrid_CopyCellsTo(6, 102, 83, 41, 1, 1);

  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 360, 855);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTHWEST + FACING_STEP, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            20);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_SetChildValue(LOCALID_SECOND_BOARD, 2);
  ObjectEvent_SetSpeed(LOCALID_SECOND_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_MoveToAndWait(LOCALID_SECOND_BOARD, 390, 832);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_MoveToAndWait(LOCALID_SECOND_BOARD, 360, 842);
  Engine_ScriptWait(80);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_SetChildValue(LOCALID_SECOND_BOARD, 0);
  ObjectEvent_SetPosition(LOCALID_SECOND_BOARD, 0, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  Engine_ScriptWait(200);
  MapGrid_CopyCellsTo(5, 103, 82, 42, 1, 1);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;

  Engine_ScriptSetMessage(MsgValeGoodJob);
  Engine_ObjectEventJump(LOCALID_DORA, 2, 20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_STEP, 20);
  Vale_MoveObjectEventAlongLowerZPath(LOCALID_DORA, 5, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 397, 832);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 372, 832);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 40);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_WEST, 40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_OpenMessage(LOCALID_DORA, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    gFieldWork->message++;
  }
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ScriptSetMessage(MsgValeWorkingYourselvesBone);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 386, 841);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 30);
  Script_OpenMessage(LOCALID_DORA, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
  }
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgValeDevastatedWhenKyle);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 386, 825);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(60);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 372, 832);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);

  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(GARET, 369, 904);
  ObjectEvent_WalkToAndWait(JENNA, 392, 904);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  Vale_MoveObjectEventAlongLowerZPath(JENNA, 10, 11, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Script_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_STEP, 30);
  Engine_ObjectEventJump(JENNA, 4, 0);
  ObjectEvent_WalkToAndWait(JENNA, 392, 843);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Vale_MoveObjectEventAlongLowerZPath(GARET, 10, 11, 0);
  ObjectEvent_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_WalkTo(GARET, 392, 843);
  ObjectEvent_Get(JENNA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(JENNA, 408, 843);
  Engine_ScriptWait(1);
  ObjectEvent_Get(JENNA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ObjectEventWaitForMove(GARET);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ObjectEventJump(LOCALID_DORA, 4, 30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 2, 30);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 30);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            40);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(100);
  Engine_ObjectEventFaceEachOther(JENNA, GARET, 30);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(30);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 364, 816);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 40);
  Engine_ObjectEventFaceEachOther(JENNA, GARET, 30);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 30);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 30);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  Script_OpenMessage(LOCALID_DORA, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
  }
  Engine_ScriptWait(40);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);

  Engine_ScriptSetMessage(MsgValeAsStubbornAsYourFather);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 3);
  Engine_ScriptWait(30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventJump(LOCALID_DORA, 4, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 3);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIMATION_DORA_HANDS_UP);
  Engine_ScriptWait(5);
  Script_ShowTwoMessagesAndWait(LOCALID_DORA, 14, 2, 24, 2, GARET, 10, 14, 4,
                                14, 0);
  objectEvent = ObjectEvent_Get(LOCALID_DORA);
  objectEvent->sprite->flags = 0;
  objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3), PIXELS(3) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 364, 815);
  Engine_ScriptWait(4);
  for (i = 0; i != 4; i++) {
    objectEvent->z.fixed += PIXELS(3) / 2;
    objectEvent->scaleY -= 0x1999;
    Engine_ScriptWait(1);
  }
  ObjectEvent_SetPosition(LOCALID_DORA, 0, 0);
  ObjectEvent_SetSpeed(GARET, PIXELS(3), PIXELS(3) / 2);
  Engine_ObjectEventJump(GARET, 6, 0);
  ObjectEvent_WalkToAndWait(GARET, 374, 827);
  Script_ShowMessage(JENNA, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventStartRepeatedMotion(JENNA, 2);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_GARET_STARTLED);
  Engine_ObjectEventJump(GARET, 2, 5);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(GARET), 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            10);
  Engine_ObjectEventStartRepeatedMotion(GARET, 3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIMATION_DORA_PEEK);
  objectEvent->scaleY = 0x8000;
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(364), PIXELS(811));
  for (i = 0; i != 5; i++) {
    objectEvent->scaleY += 0x1999;
    Engine_ScriptWait(1);
  }
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  FieldCamera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3), PIXELS(3) / 2);
  Engine_ObjectEventJump(LOCALID_DORA, 6, 0);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 359, 835);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(30);
  objectEvent = ObjectEvent_Get(LOCALID_DORA);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 80);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 3);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(GARET), 1);
  Engine_ObjectEventJump(GARET, 6, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  ObjectEvent_SetSpeed(GARET, PIXELS(4), PIXELS(2));
  objectEvent = ObjectEvent_Get(GARET);
  objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(GARET, 403, 827);
  ObjectEvent_SetAttachedEffect(JENNA, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTH, 20);
  Script_ShowMessageAndWait(JENNA, 0, 1);
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_GARET_STARTLED);
  Engine_ObjectEventJump(GARET, 2, 5);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(GARET), 0);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  ObjectEvent_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_WalkToAndWait(JENNA, 408, 855);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ObjectEventStartRepeatedMotion(JENNA, 3);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 3);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 30);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(80);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 80);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 70);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 30);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 3);
  Engine_ScriptWait(30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(GARET), 0);
  Engine_ObjectEventJump(GARET, 4, 0);
  ObjectEvent_WalkToAndWait(GARET, 398, 828);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(60);

  objectEvent = ObjectEvent_Get(GARET);
  objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  objectEvent = ObjectEvent_Get(JENNA);
  objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_WalkTo(JENNA, objectEvent->x.part.pixel + 16,
                     objectEvent->z.part.pixel);
  ObjectEvent_WalkToAndWait(GARET, objectEvent->x.part.pixel + 16,
                            objectEvent->z.part.pixel - 16);
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(JENNA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(40);
  ObjectEvent_WalkToAndWait(JENNA, objectEvent->x.part.pixel,
                            objectEvent->z.part.pixel);
  ObjectEvent_SetPosition(JENNA, 0, 0);
  ObjectEvent_WalkToAndWait(GARET, objectEvent->x.part.pixel,
                            objectEvent->z.part.pixel);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Engine_PartyAddMembers(GARET, JENNA);
  FieldCamera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
  Vale_MoveObjectEventAlongHigherZPath(LOCALID_PARTY_LEADER, 13, 10, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 376, 912);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_Get(LOCALID_DORA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  Vale_MoveObjectEventAlongHigherZPath(LOCALID_DORA, 6, 5, 0);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 373, 887);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(100);
  GameFlag_Set(FLAG_MAP_TEMP_02);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  *leaderMotion = leaderMotionFlags;
  Engine_ScriptEnd();
}

/// Vale's setup: plays the roof mending (VALE_FROM_TITLE) or its end (warp
/// destination 17, once), floats the Nut, places the pillar by its column, sets
/// the watching man going, removes the puppies once Saturos and Menardi have
/// been met, and after a battle restores the roof's and the upper path's
/// cells.
s32 Vale_Initialize(void) {
  struct ObjectEvent *objectEvent;
  s32 set;

  if (gGameState.warpDest == VALE_FROM_TITLE) {
    WindowColor_Apply(((u8 *)&gGameState)[517], ((u8 *)&gGameState)[518]);
    EventScript_MendingTheRoof();
  } else {
    if (!Engine_GameFlagIsSet(FLAG_VALE_NUT_CAUGHT)) {
      if (!Engine_GameFlagIsSet(FLAG_KALAY_REACHED)) {
        Map_FloatNut(LOCALID_NUT);
      } else {
        Map_FloatNut(LOCALID_NUT_AFTER_KALAY);
      }
    }
    MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
    MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
    objectEvent = ObjectEvent_GetById(
        (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) + LOCALID_PILLAR);
    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    if (Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_14)) {
      objectEvent->x.fixed = 181 << 17;
    } else if (Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_16)) {
      objectEvent->x.fixed = 197 << 17;
    } else {
      objectEvent->x.fixed = 189 << 17;
    }
    objectEvent->z.fixed = PIXELS(584);
    objectEvent->y.fixed = PIXELS(192);
    EventScript_SettleMovedPillar();
    objectEvent->layer = 3;
    objectEvent->motionFlags = 0;
    FieldScheduler_AddCallback(Vale_AdjustPillarPriorityForLeaderHeight,
                               TASK_PRIORITY_MAP);
#if TBS_PATCH_4
    if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED)) {
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_04)) {
          MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_10)) {
          EventScript_SetPathCellOverride();
        }
      }
    } else {
#else
    if (!Engine_GameFlagIsSet(FLAG_KALAY_REACHED)) {
#endif
      if (GameFlag_IsSet(FLAG_VALE_PARTY_LEFT)) {
        objectEvent = ObjectEvent_GetById(LOCALID_DORA);
        Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_DORA), 0);
        objectEvent->scaleX = 0x28f;
        objectEvent->scaleY = 0x28f;
      }
      if (Engine_GameFlagIsSet(FLAG_MET_SATUROS_AND_MENARDI)) {
        Engine_ObjectEventSetPosition(LOCALID_FIRST_PUPPY, 0, 0);
        Engine_ObjectEventSetPosition(LOCALID_FIRST_PUPPY + 1, 0, 0);
        Engine_ObjectEventSetPosition(LOCALID_FIRST_PUPPY + 2, 0, 0);
      }
      set = Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT);
      if (set == 0) {
        if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
          if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH)) {
            ObjectEvent_SetPosition(LOCALID_MAN_GUARDING_MT_ALEPH, PIXELS(256),
                                    PIXELS(456));
#ifdef UBFIX
            ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH)->update =
                Vale_HaveGuardWatchTheLeader;
#else
            ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH)->update =
                (void *)Vale_HaveGuardWatchTheLeader;
#endif
            ObjectEvent_SetActionScript(LOCALID_MAN_GUARDING_MT_ALEPH,
                                        gVale_AlephGuardPatrol);
          }
        } else {
          ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH)->paused = set;
          Engine_GameFlagClear(FLAG_MAP_TEMP_41);
        }
        if (gGameState.warpDest != VALE_FROM_TITLE &&
            !Engine_GameFlagIsSet(FLAG_KALAY_REACHED)) {
          FieldScheduler_AddCallback(Vale_SynchronizeAlephGuardPause,
                                     TASK_PRIORITY_MAP);
        }
      }
      if (!GameFlag_IsSet(FLAG_MAPSEC_TEMP_08) &&
          gGameState.warpDest == VALE_FROM_ROOF_MENDED) {
        EventScript_RoofMended();
        Engine_GameFlagSet(FLAG_MAPSEC_TEMP_08);
      }
#if TBS_PATCH_4
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_04)) {
          MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_10)) {
          EventScript_SetPathCellOverride();
        }
      }
      {
        s32 priority;
        s32 localId;

        priority = ((u32)(u8)set >> 8) + 2;
        localId = LOCALID_DORA;
        Engine_ObjectEventSetSpritePriority(localId, priority);
      }
#endif
    }
#if !TBS_PATCH_4
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_04)) {
        MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
      }
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_10)) {
        EventScript_SetPathCellOverride();
      }
    }
#endif
    Map_QueueMusic(SE_170);
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
  }
  return 0;
}

/// Back from Kalay, Garet meets his sister in Vale; sets map section flag
/// FLAG_MAPSEC_TEMP_03. "Ah! It's been a while... You're back?" ("Sis...").
static void EventScript_GaretMeetsHisSister(void) {
  struct ObjectEvent *record;
  s32 msg;

  Engine_ScriptStart();
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_SISTER_AFTER_KALAY,
                              LOCALID_PARTY_LEADER, 20);
  msg = MsgValeAh;
  Engine_ScriptSetMessage(msg);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_GARETS_SISTER_AFTER_KALAY, 2);
  Script_ShowMessageAndWait(LOCALID_GARETS_SISTER_AFTER_KALAY, 0, 20);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(398), -1, PIXELS(582), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 420, 608);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARETS_SISTER_AFTER_KALAY,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (record != 0) {
    ObjectEvent_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  ObjectEvent_WalkToAndWait(GARET, 402, 608);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessage(SPEAKER_PORTRAIT_LEFT | GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_GARETS_SISTER_AFTER_KALAY,
                            FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_SISTER_AFTER_KALAY,
                                        ANIMATION_NOD);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_SISTER_AFTER_KALAY,
                     0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_SISTER_AFTER_KALAY, 2);
  Script_OpenMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_SISTER_AFTER_KALAY,
                     0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
    Engine_ObjectEventStartRepeatedMotion(LOCALID_GARETS_SISTER_AFTER_KALAY, 1);
  }
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_SISTER_AFTER_KALAY, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_GARETS_SISTER_AFTER_KALAY,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ScriptSetMessage(msg + 6);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_SISTER_AFTER_KALAY, 1);
  ObjectEvent_FaceDirection(LOCALID_GARETS_SISTER_AFTER_KALAY,
                            FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_SISTER_AFTER_KALAY,
                     0);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 120);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_SISTER_AFTER_KALAY, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_SISTER_AFTER_KALAY,
                                        ANIMATION_SHAKE_HEAD);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_GARETS_SISTER_AFTER_KALAY,
                            FACING_SOUTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_SISTER_AFTER_KALAY, 0, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_SISTER_AFTER_KALAY,
                                        ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (record != 0) {
    ObjectEvent_SetDestination(GARET, record->x.part.pixel,
                               record->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  GameFlag_Set(FLAG_MAPSEC_TEMP_03);
  Engine_ScriptEnd();
}

/// Back from Kalay, Garet asks one of his family in Vale
/// (LOCALID_GARETS_BROTHER_AFTER_KALAY) to protect it while he is gone; sets
/// map section flag FLAG_MAPSEC_TEMP_04. "WHATEVER YOU DO, you must protect the
/// family while I'm gone."
static void EventScript_GaretAsksToProtectFamily(void) {
  struct ObjectEvent *record;

  Engine_ScriptStart();
  FieldCamera_MoveTo(PIXELS(357), -1, PIXELS(738), 1);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 367, 745);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (record != 0) {
    ObjectEvent_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  ObjectEvent_WalkToAndWait(GARET, 346, 745);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Engine_ScriptSetMessage(MsgValeHowHaveYouBeen);
  Script_ShowMessage(GARET, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_BROTHER_AFTER_KALAY, 2);
  ObjectEvent_ShowEmote(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_FaceDirection(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                            FACING_SOUTHEAST + FACING_STEP, 10);
  ObjectEvent_FaceDirection(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                            FACING_SOUTH + FACING_STEP, 10);
  ObjectEvent_FaceDirection(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                            FACING_SOUTHEAST + FACING_STEP, 40);
  Script_ShowMessageAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_BROTHER_AFTER_KALAY, 1);
  ObjectEvent_FaceDirection(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                            FACING_SOUTH + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Script_ShowMessageAndWait(GARET, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                                        ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                                        ANIMATION_SHAKE_HEAD);
  Script_ShowMessage(LOCALID_GARETS_BROTHER_AFTER_KALAY, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 10);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(GARET, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                                        ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Script_ShowMessageAndWait(GARET, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_BROTHER_AFTER_KALAY, 2);
  Script_ShowMessageAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 20);
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  } else {
    gFieldWork->message++;
  }
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 10);
  Engine_ScriptSetMessage(MsgValeEverProtectFamily);
  Script_ShowMessage(GARET, 0);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY,
                                        ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  record = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (record != 0) {
    ObjectEvent_SetDestination(GARET, record->x.part.pixel,
                               record->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  GameFlag_Set(FLAG_MAPSEC_TEMP_04);
  Engine_ScriptEnd();
}

/// Unused: water bursts up by the river after a tremor. Back from Kalay, a
/// tremor shakes Vale and water bursts up three times north of the strongman by
/// the river who remembers Felix.
///
/// No table names it. He would have started, sniffled through
/// `I still remember when Felix and I used to play here...*Sniff*`, turned to
/// the leader to say that Felix must be protecting Jenna, and set map section
/// flag FLAG_MAPSEC_TEMP_05. His talk and Mind Read rows show the same two
/// lines.
UNUSED static void EventScript_UnusedWaterBurstsByTheRiver(void) {
  struct ObjectEvent *objectEvent;
  const s32 *actions;

  Engine_ScriptStart();
  objectEvent = ObjectEvent_Get(LOCALID_FIRST_WATER);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_Get(LOCALID_FIRST_WATER + 1);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_Get(LOCALID_FIRST_WATER + 2);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER + 1, 0);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER + 2, 0);
  Engine_TaskWait(20);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  actions = gVale_UnusedWaterGrows;
  ObjectEvent_SetActionScript(LOCALID_FIRST_WATER, actions);
  Engine_TaskWait(10);
  ObjectEvent_SetActionScript(LOCALID_FIRST_WATER + 1, actions);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_TaskWait(20);
  ObjectEvent_RunActionScriptAndWait(LOCALID_FIRST_WATER + 2, actions);
  ObjectEvent_ShowEmote(LOCALID_STRONGMAN_REMEMBERING_FELIX,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_STRONGMAN_REMEMBERING_FELIX, 2);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_REMEMBERING_FELIX,
                            FACING_NORTH + FACING_STEP, 10);
  Engine_ScriptSetMessage(MsgValeIUsedToPlayHere);
  Script_ShowMessageAndWait(LOCALID_STRONGMAN_REMEMBERING_FELIX, 0, 40);
  ObjectEvent_FaceObjectEvent(LOCALID_STRONGMAN_REMEMBERING_FELIX,
                              LOCALID_PARTY_LEADER, 20);
  Script_ShowMessage(LOCALID_STRONGMAN_REMEMBERING_FELIX, 0);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_REMEMBERING_FELIX, FACING_WEST,
                            10);
  GameFlag_Set(FLAG_MAPSEC_TEMP_05);
  Engine_ScriptEnd();
}

/// Walks an object event to the foot of the ladder beside Isaac's house and
/// climbs it onto the roof, in `approachAnimationId` on the rungs and
/// `departureAnimationId` over the eaves.
static void Vale_MoveObjectEventAlongLowerZPath(s32 localId,
                                                s32 approachAnimationId,
                                                s32 departureAnimationId,
                                                s32 resumeMotion) {
  s32 departureAnimation;
  s32 approachAnimation;
  s32 restoreMotion;
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;

  restoreMotion = resumeMotion;
  approachAnimation = approachAnimationId;
  departureAnimation = departureAnimationId;
  objectEvent = ObjectEvent_GetById(localId);
  sprite = objectEvent->sprite;
  ObjectEvent_SetSpeed(localId, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(localId, 392, 886);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 10);
  objectEvent->motionFlags = 0;
  sprite->flags = 0;
  Engine_ObjectEventSetAnimation(localId, approachAnimation);
  ObjectEvent_SetSpeed(localId, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_MoveToAndWait(localId, 392, 875);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(localId, departureAnimation);
  ObjectEvent_SetSpeed(localId, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_MoveToAndWait(localId, 392, 859);
  sprite->flags = 1;
  if (restoreMotion != 0) {
    objectEvent->motionFlags = 3;
  }
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(localId, ANIMATION_STAND);
}

/// Climbs an object event down the ladder beside Isaac's house from the roof,
/// and restores falling motion if requested.
static void Vale_MoveObjectEventAlongHigherZPath(s32 localId,
                                                 s32 approachAnimationId,
                                                 s32 departureAnimationId,
                                                 s32 resumeMotion) {
  s32 departureAnimation;
  s32 approachAnimation;
  s32 restoreMotion;
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;

  restoreMotion = resumeMotion;
  approachAnimation = approachAnimationId;
  departureAnimation = departureAnimationId;
  objectEvent = ObjectEvent_GetById(localId);
  sprite = objectEvent->sprite;
  ObjectEvent_SetSpeed(localId, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(localId, 392, 859);
  ObjectEvent_FaceDirection(localId, FACING_NORTH, 10);
  objectEvent->motionFlags = 0;
  sprite->flags = 0;
  Engine_ObjectEventSetAnimation(localId, approachAnimation);
  ObjectEvent_SetSpeed(localId, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_MoveToAndWait(localId, 392, 875);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(localId, PIXELS(3) / 10, PIXELS(3) / 20);
  Engine_ObjectEventSetAnimation(localId, departureAnimation);
  ObjectEvent_MoveToAndWait(localId, 392, 890);
  objectEvent->velocityY = 0x20000;
  sprite->flags = 1;
  if (restoreMotion != 0) {
    objectEvent->motionFlags = 3;
  }
  Engine_ObjectEventSetAnimation(localId, ANIMATION_STAND);
}

/// Mode 0 sets count boards from first on the roof by the hole, still and
/// without shadows; any other mode puts them out of sight.
static void Vale_PlaceOrHideBoards(s32 first, u32 count, s32 mode) {
  s32 selector = first;
  u32 i;

  if (mode == 0) {
    for (i = 0; i < count; i++) {
      struct ObjectEvent *objectEvent = ObjectEvent_GetById(selector);

      objectEvent->motionFlags = 0;
      Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
      objectEvent->x.fixed = PIXELS(390);
      objectEvent->y.fixed = PIXELS(160);
      objectEvent->z.fixed = PIXELS(842);
      selector++;
    }
    return;
  }

  for (i = 0; i < count; i++) {
    Engine_ObjectEventSetPosition(selector, 0, 0);
    selector++;
  }
}

/// Arriving by VALE_FROM_TITLE, the game's opening in Vale: Isaac is mending
/// the roof of his house, Garet trains his Psynergy, and Jenna comes by; the
/// two speak of the storm three years ago before going to fetch Isaac. "Three
/// years later... Isaac! Haven't you finished mending the roof yet?"
static void EventScript_MendingTheRoof(void) {
  u32 i;
  /* FAKEMATCH(dead): repairObjectEvent's zero initializer, never read, keeps
   * the scene's register allocation; without it, 59 of 1024 instructions differ
   * in tbs-en. */
  struct ObjectEvent *repairObjectEvent = NULL;
  struct ObjectEvent *leader;
  struct ObjectEvent *moveHand;
  struct ObjectEvent *objectEvent;
  /* FAKEMATCH(temporary): one zero for the hand's flags and update and
   * the message call; as literal zeros, 7 of 1024 instructions differ in
   * tbs-en. */
  s32 stoppedMotionFlags;
  s32 message;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  moveHand = ObjectEvent_GetById(LOCALID_OPENING_MOVE_HAND);

  // Prepare the damaged house and start Isaac's hammering behind the caption.
  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  ((struct ObjectEvent *)Engine_ScriptGetViewCenter())->motionFlags = 0;
  Engine_TaskWait(1);
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  repairObjectEvent = ObjectEvent_GetById(LOCALID_OPENING_FIRST_BOARD);
  repairObjectEvent->motionFlags = 0;
  repairObjectEvent->x.fixed = PIXELS(388);
  repairObjectEvent->y.fixed = PIXELS(160);
  repairObjectEvent->z.fixed = PIXELS(840);
  Engine_ObjectEventSetSpriteFlags(repairObjectEvent, 0);
  repairObjectEvent = ObjectEvent_GetById(LOCALID_OPENING_SECOND_BOARD);
  repairObjectEvent->motionFlags = 0;
  repairObjectEvent->y.fixed = PIXELS(160);
  repairObjectEvent->z.fixed = PIXELS(844);
  repairObjectEvent->x.fixed = PIXELS(388);
  Engine_ObjectEventSetSpriteFlags(repairObjectEvent, 0);
  repairObjectEvent = ObjectEvent_GetById(LOCALID_OPENING_THIRD_BOARD);
  repairObjectEvent->motionFlags = 0;
  repairObjectEvent->z.fixed = PIXELS(848);
  repairObjectEvent->y.fixed = PIXELS(160);
  repairObjectEvent->x.fixed = PIXELS(388);
  Engine_ObjectEventSetSpriteFlags(repairObjectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_OPENING_FIRST_BOARD);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_OPENING_SECOND_BOARD);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  ObjectSystem_SuspendProjectionAndShowSprites();
  message = MsgValeRepairCaption;
  FieldMessage_ShowCentered(message, 0, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();

  // Follow Jenna through the village until she notices the repair work.
  FieldCamera_MoveTo(PIXELS(339), PIXELS(160), PIXELS(1173), 0);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  FieldCamera_SetSpeed(PIXELS(33) / 100, PIXELS(33) / 800);
  FieldCamera_MoveTo(PIXELS(296), PIXELS(160), PIXELS(921), 1);
  ObjectEvent_SetPosition(JENNA, PIXELS(409), PIXELS(1134));
  Engine_TaskWait(1);
  ObjectEvent_SetSpeed(JENNA, PIXELS(7) / 10, PIXELS(7) / 20);
  ObjectEvent_WalkTo(JENNA, 420, 1068);
  Map_PlayMusic();
  gFieldWork->transitionFrames = 60;
  Engine_ScriptOpenScreen();
  Engine_ObjectEventWaitForMove(JENNA);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(JENNA, 341, 1064);
  ObjectEvent_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(JENNA, 359, 1033);
  ObjectEvent_SetSpeed(LOCALID_OPENING_WOMAN, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkTo(LOCALID_OPENING_WOMAN, 318, 947);
  Engine_ObjectEventSetAnimation(LOCALID_OPENING_WOMAN, ANIMATION_WALK);
  ObjectEvent_WalkToAndWait(JENNA, 412, 1033);
  ObjectEvent_WalkToAndWait(JENNA, 412, 1019);
  ObjectEvent_WalkToAndWait(JENNA, 374, 1008);
  ObjectEvent_WalkToAndWait(JENNA, 347, 955);
  ObjectEvent_WalkToAndWait(LOCALID_OPENING_WOMAN, 318, 947);
  Engine_ObjectEventFaceEachOther(JENNA, LOCALID_OPENING_WOMAN, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_OPENING_WOMAN, 2);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  ObjectEvent_WalkTo(LOCALID_OPENING_WOMAN, 379, 1017);
  FieldCamera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  ObjectEvent_WalkToAndWait(JENNA, 333, 920);
  ObjectEvent_WalkToAndWait(JENNA, 299, 924);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHEAST + FACING_STEP, 30);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(20);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(387), PIXELS(160), PIXELS(866), 1);
  Engine_CameraWaitForMove();
  Engine_ObjectEventJump(LOCALID_OPENING_DORA, 2, 20);
  Engine_ScriptSetMessage(message + 1);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_OPENING_DORA, 0,
                            10);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  Engine_ObjectEventFaceEachOther(LOCALID_OPENING_DORA, LOCALID_PARTY_LEADER,
                                  40);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(40);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_OPENING_DORA, 2);
  Engine_ScriptWait(40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_OPENING_DORA, 0,
                            40);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  Engine_CameraFollowObjectEvent(JENNA, 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTH + FACING_STEP, 10);
  ObjectEvent_WalkToAndWait(JENNA, 312, 759);
  ObjectEvent_WalkToAndWait(JENNA, 361, 760);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 40);
  ObjectEvent_FaceDirection(JENNA, FACING_EAST, 40);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | GARET, 0, 10);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventJump(JENNA, 4, 40);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTH, 30);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(40);
  Script_ShowMessageAndWait(JENNA, 0, 30);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(396), -1, PIXELS(588), 1);
  ObjectEvent_WalkToAndWait(JENNA, 456, 739);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);

  // Garet trains his Psynergy: the hand of Move pushes the pillar, shedding
  // sparkles.
  Map_StartCutscenePsynergy();
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_CAST_PSYNERGY);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | GARET, 0, 20);
  Script_PlaySong(SE_131);
  for (i = 0; i < 60; i++) {
    Vale_FlashObjectEventAndSpawnEffects(ObjectEvent_GetById(GARET));
    Engine_TaskWait(1);
  }
  Engine_ObjectEventSetSpritePriority(GARET, 1);
  Task_AddCallback(Vale_UpdateGaretPaletteAndEffects, TASK_PRIORITY_MAP);
  Task_AddCallback(Vale_CycleOpeningPillarPalette, TASK_PRIORITY_MAP);
  objectEvent = ObjectEvent_GetById(LOCALID_OPENING_MOVE_HAND);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  stoppedMotionFlags = 0;
  moveHand->motionFlags = stoppedMotionFlags;
  moveHand->x.fixed = PIXELS(428);
  moveHand->y.fixed = PIXELS(208);
  moveHand->z.fixed = PIXELS(584);
  moveHand->facing = FACING_WEST;
  moveHand->update = Vale_TrailSparkles;
  Engine_ScriptWait(4);
  ObjectEvent_SetSpeed(LOCALID_OPENING_MOVE_HAND, PIXELS(2), PIXELS(2));
  Object_SetPosition(moveHand, PIXELS(408), PIXELS(208), PIXELS(584));
  Engine_ScriptWait(40);
  ObjectEvent_SetSpeed(LOCALID_OPENING_PILLAR, PIXELS(3) / 20, PIXELS(3) / 40);
  ObjectEvent_SetSpeed(LOCALID_OPENING_MOVE_HAND, PIXELS(3) / 20,
                       PIXELS(3) / 40);
  ObjectEvent_GetById(LOCALID_OPENING_PILLAR);
  Engine_ObjectSetPosition(moveHand, PIXELS(392), PIXELS(208), PIXELS(584));
  ObjectEvent_MoveToAndWait(LOCALID_OPENING_PILLAR, 378, 584);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA, 0, 10);
  moveHand->update = (void (*)(union FieldObject *))stoppedMotionFlags;
  Engine_ObjectEventSetSpritePriority(GARET, 2);
  ObjectEvent_Get(GARET)->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  Task_Remove(Vale_UpdateGaretPaletteAndEffects);
  Task_Remove(Vale_CycleOpeningPillarPalette);
  Engine_TaskWait(1);
  ObjectEvent_SetChildValue(GARET, 0);
  ObjectEvent_SetChildValue(LOCALID_OPENING_PILLAR, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  Vale_VanishMoveHand(moveHand);
  Map_EndCutscenePsynergy();

  // Finish the reunion conversation after the repair effects settle.
  Engine_ScriptWait(10);
  ObjectEvent_WalkToAndWait(JENNA, 424, 624);
  Engine_ObjectEventFaceEachOther(GARET, JENNA, 60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | GARET, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 1);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(10);
  ObjectEvent_WalkToAndWait(JENNA, 388, 604);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST, 20);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 40);
  Script_ShowTwoMessagesAndWait(GARET, 1, 2, 25, 2, JENNA, 10, 14, 4, 14,
                                stoppedMotionFlags);
  Engine_ScriptWait(40);
  ObjectEvent_SetAttachedEffect(JENNA, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(80);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(80);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 10);
  ObjectEvent_FaceDirection(JENNA, FACING_EAST + FACING_STEP, 40);
  ObjectEvent_FaceObjectEvent(JENNA, GARET, 40);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(10);
  ObjectEvent_WalkToAndWait(JENNA, 380, 620);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(80);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | GARET, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(GARET, 412, 604);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 1);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ObjectEventJump(JENNA, 4, 30);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | GARET, 0, 20);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHEAST, 40);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ObjectEvent_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(JENNA, 428, 628);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ObjectEventWaitForMove(JENNA);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | GARET, 0);
  Engine_ObjectEventSetAnimation(JENNA, ANIMATION_STAND);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  ObjectEvent_WalkToAndWait(JENNA, 428, 628);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA, 0, 20);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(40);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | GARET, 0, 20);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkTo(JENNA, 450, 750);
  ObjectEvent_WalkTo(GARET, 450, 750);
  Engine_ScriptWait(60);
  gFieldWork->transitionFrames = 60;
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(VALE_TO_ROOF_MENDED);
  Engine_ScriptEnd();
}

/// Arriving by VALE_FROM_ROOF_MENDED after the opening, in Vale: Dora praises
/// the mended roof, remembers Kyle, and sends Isaac, Garet and Jenna off to Mt.
/// Aleph with Kraden. "Good job, Isaac. You patched it nicely."
static void EventScript_RoofMended(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  u32 i;
  s32 turnBack;
  s32 turnSide;
  struct ObjectEvent *turned;
  s32 none;
  s32 flag;
  TaskFunc callbackA;
  TaskFunc callbackB;
  TaskFunc callbackC;
  TaskFunc callbackD;
  TaskFunc callbackE;
  TaskFunc callbackF;

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(392), PIXELS(896));
  turned = ObjectEvent_Get(LOCALID_DORA);

  turned->facing = FACING_EAST;
  turnBack = FACING_NORTH;
  turned->facing = turnBack;
  ObjectEvent_SetPosition(GARET, PIXELS(298), PIXELS(736));
  turned = ObjectEvent_Get(GARET);
  turnSide = FACING_SOUTH;
  turned->facing = turnSide;
  ObjectEvent_SetPosition(JENNA, PIXELS(298), PIXELS(760));
  turned = ObjectEvent_Get(JENNA);
  turned->facing = turnSide;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  objectEvent = ObjectEvent_Get(LOCALID_FIRST_BOARD);
  objectEvent->motionFlags = 0;
  objectEvent->x.fixed = PIXELS(388);
  objectEvent->y.fixed = PIXELS(160);
  objectEvent->z.fixed = PIXELS(840);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_Get(LOCALID_SECOND_BOARD);
  objectEvent->motionFlags = 0;
  objectEvent->x.fixed = PIXELS(388);
  objectEvent->y.fixed = PIXELS(160);
  objectEvent->z.fixed = PIXELS(844);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_Get(LOCALID_THIRD_BOARD);
  objectEvent->motionFlags = 0;
  objectEvent->x.fixed = PIXELS(388);
  objectEvent->y.fixed = PIXELS(160);
  objectEvent->z.fixed = PIXELS(848);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  ((u8 *)Engine_ScriptGetViewCenter())[85] = 0;
  Engine_TaskWait(1);
  FieldCamera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  gFieldWork->transitionFrames = 32;
  Engine_ScriptOpenScreen();
  ObjectEvent_SetSpeed(JENNA, PIXELS(1) / 2, turnSide);
  ObjectEvent_SetSpeed(GARET, PIXELS(1) / 2, turnSide);
  ObjectEvent_SetActionScript(JENNA, gVale_JennaWalksToTheHouse);
  ObjectEvent_SetActionScript(GARET, gVale_GaretWalksToTheHouse);
  Engine_ScriptWait(40);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 400, 840);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, turnBack, 30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 40);
  Map_StartCutscenePsynergy();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_CAST_PSYNERGY);
  FieldScheduler_AddCallback(Vale_PlayStepSound, TASK_PRIORITY_MAP);
  for (i = 0; i < 40; i++) {
    Vale_FlashObjectEventAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 1);

  callbackA = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackA, TASK_PRIORITY_MAP);
  callbackB = Vale_CycleFirstBoardPalette;
  Task_AddCallback(callbackB, TASK_PRIORITY_MAP);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_BOARD, 390, 832);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_BOARD, 400, 826);
  Engine_ScriptWait(20);
  {
    struct ObjectEvent *record = ObjectEvent_Get(LOCALID_PARTY_LEADER);

    record->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Task_Remove(Vale_PlayStepSound);
  Task_Remove(callbackA);
  Task_Remove(callbackB);
  Engine_TaskWait(1);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_SetChildValue(LOCALID_FIRST_BOARD, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BOARD, 0, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  Engine_ScriptWait(120);
  MapGrid_CopyCellsTo(7, 102, 84, 41, 2, 1);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 377, 843);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_CAST_PSYNERGY);
  Task_AddCallback(Vale_PlayStepSound, TASK_PRIORITY_MAP);
  for (i = 0; i < 40; i++) {
    Vale_FlashObjectEventAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 1);
  callbackC = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackC, TASK_PRIORITY_MAP);
  callbackD = Vale_CycleSecondBoardPalette;
  Task_AddCallback(callbackD, TASK_PRIORITY_MAP);
  ObjectEvent_SetSpeed(LOCALID_SECOND_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_MoveToAndWait(LOCALID_SECOND_BOARD, 390, 832);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_MoveToAndWait(LOCALID_SECOND_BOARD, 377, 828);
  Engine_ScriptWait(20);
  {
    struct ObjectEvent *record = ObjectEvent_Get(LOCALID_PARTY_LEADER);

    record->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Task_Remove(Vale_PlayStepSound);
  Task_Remove(callbackC);
  Task_Remove(callbackD);
  Engine_TaskWait(1);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_SetChildValue(LOCALID_SECOND_BOARD, 0);
  ObjectEvent_SetPosition(LOCALID_SECOND_BOARD, 0, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  Engine_ScriptWait(120);
  MapGrid_CopyCellsTo(6, 102, 83, 41, 1, 1);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 360, 855);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTHWEST + FACING_STEP, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_CAST_PSYNERGY);
  Task_AddCallback(Vale_PlayStepSound, TASK_PRIORITY_MAP);
  for (i = 0; i < 40; i++) {
    Vale_FlashObjectEventAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 1);
  callbackE = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackE, TASK_PRIORITY_MAP);
  callbackF = Vale_CycleThirdBoardPalette;
  FieldScheduler_AddCallback(callbackF, TASK_PRIORITY_MAP);
  ObjectEvent_SetSpeed(LOCALID_THIRD_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_MoveToAndWait(LOCALID_THIRD_BOARD, 390, 832);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_MoveToAndWait(LOCALID_THIRD_BOARD, 360, 837);
  Engine_ScriptWait(20);
  {
    struct ObjectEvent *record = ObjectEvent_Get(LOCALID_PARTY_LEADER);

    record->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Task_Remove(Vale_PlayStepSound);
  Task_Remove(callbackE);
  Task_Remove(callbackF);
  Engine_TaskWait(1);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_SetChildValue(LOCALID_THIRD_BOARD, 0);
  ObjectEvent_SetPosition(LOCALID_THIRD_BOARD, 0, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_HAMMER);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gVale_LeaderHammers);
  Engine_ScriptWait(120);
  Map_EndCutscenePsynergy();
  MapGrid_CopyCellsTo(5, 103, 82, 42, 1, 1);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ObjectEventJump(LOCALID_DORA, 2, 20);
  Engine_ScriptSetMessage(MsgValeGoodJob);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST + FACING_STEP,
                            10);
  Vale_MoveObjectEventAlongLowerZPath(LOCALID_DORA, 5, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 397, 832);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 372, 832);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 40);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_WEST, 40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Script_OpenMessage(LOCALID_DORA, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
    gFieldWork->message++;
  } else {
    Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  }
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ScriptSetMessage(MsgValeWorkingYourselvesBone);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 386, 841);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 30);
  Script_OpenMessage(LOCALID_DORA, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
  }
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptSetMessage(MsgValeDevastatedWhenKyle);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 386, 825);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(60);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 372, 832);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(GARET, 369, 904);
  ObjectEvent_WalkToAndWait(JENNA, 392, 904);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  Vale_MoveObjectEventAlongLowerZPath(JENNA, 10, 11, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Script_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST + FACING_STEP,
                            20);
  Engine_ObjectEventJump(JENNA, 4, 0);
  ObjectEvent_WalkToAndWait(JENNA, 392, 843);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Script_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Vale_MoveObjectEventAlongLowerZPath(GARET, 10, 11, 0);
  ObjectEvent_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_WalkTo(GARET, 392, 843);
  ObjectEvent_GetById(JENNA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(JENNA, 408, 843);
  Engine_ScriptWait(1);
  {
    struct ObjectEvent *record = ObjectEvent_Get(JENNA);

    record->steeringFlags = OBJ_EVENT_FACES_MOTION | record->steeringFlags;
  }
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ObjectEventWaitForMove(GARET);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ObjectEventJump(LOCALID_DORA, 4, 30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 2, 30);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 30);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(80);
  Engine_ObjectEventFaceEachOther(JENNA, GARET, 30);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 10);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(40);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 364, 816);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 40);
  Engine_ObjectEventFaceEachOther(JENNA, GARET, 30);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Script_OpenMessage(LOCALID_DORA, 0);
  none = 0;

  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
  }
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ScriptSetMessage(MsgValeAsStubbornAsYourFather);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 3);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  Engine_ObjectEventJump(LOCALID_DORA, 4, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 3);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIMATION_DORA_HANDS_UP);
  Engine_ScriptWait(5);
  Script_ShowTwoMessagesAndWait(LOCALID_DORA, 14, 2, 24, 2, GARET, 10, 14, 4,
                                14, none);
  Script_PlaySong(SE_161);
  objectEvent = ObjectEvent_Get(LOCALID_DORA);
  objectEvent->sprite->flags = none;
  objectEvent->steeringFlags &= 0xfe;
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3), PIXELS(3) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 364, 815);
  Engine_ScriptWait(4);
  for (i = 0; i != 4; i++) {
    objectEvent->z.fixed += PIXELS(3) / 2;
    objectEvent->scaleY += -0x1999;
    Engine_ScriptWait(1);
  }
  ObjectEvent_SetPosition(LOCALID_DORA, 0, 0);
  ObjectEvent_SetSpeed(GARET, PIXELS(3), PIXELS(3) / 2);
  Engine_ObjectEventJump(GARET, 6, 0);
  ObjectEvent_WalkToAndWait(GARET, 374, VALE_GARET_REUNION_Z);
#if TBS_PATCH_2
  ObjectEvent_SetPosition(GARET, PIXELS(374), PIXELS(VALE_GARET_REUNION_Z));
#endif
  Script_ShowMessage(JENNA, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventStartRepeatedMotion(JENNA, 2);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_GARET_STARTLED);
  Engine_ObjectEventJump(GARET, 2, 5);
  Script_PlaySong(SE_143);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(GARET), 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            10);
  Engine_ObjectEventStartRepeatedMotion(GARET, 3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIMATION_DORA_PEEK);
  objectEvent->scaleY = 0x8000;
  ObjectEvent_SetPosition(LOCALID_DORA, PIXELS(364), PIXELS(811));
  for (i = 0; i != 5; i++) {
    objectEvent->scaleY += 0x1999;
    Engine_ScriptWait(1);
  }
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  FieldCamera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(3), PIXELS(3) / 2);
  Engine_ObjectEventJump(LOCALID_DORA, 6, 0);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 359, 835);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  objectEvent->priorityFlags &= 0xfe;
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 80);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 3);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 40);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(GARET), 1);
  Engine_ObjectEventJump(GARET, 6, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  ObjectEvent_SetSpeed(GARET, PIXELS(4), PIXELS(2));
  objectEvent = ObjectEvent_Get(GARET);
  objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(GARET, 403, VALE_GARET_REUNION_Z);
  ObjectEvent_SetAttachedEffect(JENNA, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTH, 20);
  Script_ShowMessageAndWait(JENNA, 0, 1);
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_GARET_STARTLED);
  Engine_ObjectEventJump(GARET, 2, 5);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(GARET), 0);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Script_PlaySong(SE_143);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  ObjectEvent_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_WalkToAndWait(JENNA, 408, 855);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ObjectEventStartRepeatedMotion(JENNA, 3);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 3);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 30);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(80);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 10);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 80);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 60);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 30);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 3);
  Engine_ScriptWait(30);
  Script_ShowMessageAndWait(LOCALID_DORA, 0, 60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(GARET), 0);
  Engine_ObjectEventJump(GARET, 4, 0);
  Engine_ObjectEventWalkToAndWait(GARET, 398, 828);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, ANIMATION_NOD);
  Engine_ScriptWait(60);
  objectEvent = ObjectEvent_Get(GARET);
  flag = 1;
  objectEvent->steeringFlags |= flag;
  objectEvent = ObjectEvent_Get(JENNA);
  objectEvent->steeringFlags |= flag;
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_WalkTo(JENNA, objectEvent->x.part.pixel + 16,
                     objectEvent->z.part.pixel);
  ObjectEvent_WalkToAndWait(GARET, objectEvent->x.part.pixel + 16,
                            objectEvent->z.part.pixel - 16);
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(JENNA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(40);
  ObjectEvent_WalkToAndWait(JENNA, objectEvent->x.part.pixel,
                            objectEvent->z.part.pixel);
  ObjectEvent_SetPosition(JENNA, 0, 0);
  ObjectEvent_WalkToAndWait(GARET, objectEvent->x.part.pixel,
                            objectEvent->z.part.pixel);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Engine_PartyAddMembers(GARET, JENNA);
  FieldCamera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
  Vale_MoveObjectEventAlongHigherZPath(LOCALID_PARTY_LEADER, 13, 10, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 376, 912);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  {
    struct ObjectEvent *record = ObjectEvent_Get(LOCALID_DORA);

    record->steeringFlags = flag | record->steeringFlags;
  }
  Vale_MoveObjectEventAlongHigherZPath(LOCALID_DORA, 6, 5, 0);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 373, 887);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(100);
  MapGrid_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  GameFlag_Set(FLAG_MAP_TEMP_02);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
#if TBS_PATCH_1
  leader->motionFlags = 3;
  leader->y.fixed = PIXELS(160);
  leader->targetY = 0x80000000;
  leader->velocityY = 0;
#else
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 3;
#endif
  Engine_ScriptEnd();
}

#include "common/effect_psynergy.h"

/// Cycles palettes on Garet and periodically spawns attached effects.
static void Vale_UpdateGaretPaletteAndEffects(void) {
  Vale_CycleObjectEventPaletteAndSpawnEffects(ObjectEvent_GetById(GARET));
}

/// Cycles the leader palette and periodically spawns attached effects.
static void Vale_UpdateLeaderPaletteAndEffects(void) {
  Vale_CycleObjectEventPaletteAndSpawnEffects(
      ObjectEvent_GetById(LOCALID_PARTY_LEADER));
}

/// Cycles the pillar's colour modes while Garet's Psynergy moves it in the
/// opening.
static void Vale_CycleOpeningPillarPalette(void) {
  Vale_CycleObjectEventPalette(ObjectEvent_GetById(LOCALID_OPENING_PILLAR));
}

/// Cycles the first board palette while its repair effect runs.
static void Vale_CycleFirstBoardPalette(void) {
  Vale_CycleObjectEventPalette(ObjectEvent_GetById(LOCALID_FIRST_BOARD));
}

/// Cycles the second board palette while its repair effect runs.
static void Vale_CycleSecondBoardPalette(void) {
  Vale_CycleObjectEventPalette(ObjectEvent_GetById(LOCALID_SECOND_BOARD));
}

/// Cycles the third board's palette while its repair effect runs.
static void Vale_CycleThirdBoardPalette(void) {
  Vale_CycleObjectEventPalette(ObjectEvent_GetById(LOCALID_THIRD_BOARD));
}

/// The distance in whole pixels between two positions of 16.16 fixed-point x,
/// y and z.
static s32 Vale_DistanceInPixels(s32 *first_position, s32 *second_position) {
  s32 delta_x = (*first_position++ - *second_position++) >> 16;
  s32 delta_y = (*first_position++ - *second_position++) >> 16;
  s32 delta_z = (*first_position - *second_position) >> 16;
  s32 delta_x_squared = delta_x * delta_x;
  s32 delta_y_squared = delta_y * delta_y;
  s32 delta_z_squared = delta_z * delta_z;

  return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

/// Whether the man guarding Mt. Aleph sees the target: within range pixels and
/// in the sixteenth of a turn he faces or the one either side of it, or always
/// when force is set. Seeing it stops him where he stands (paused, mode 1, with
/// riseCounter marking why) until it is out of sight again; paused without
/// riseCounter, as Vale_SynchronizeAlephGuardPause leaves him while a menu is
/// open, he just stands. Returns 1 when he is stopped.
static s32 Vale_TestFacing(struct ObjectEvent *objectEvent,
                           struct ObjectEvent *target, s32 range, s32 force) {
  s32 result;
  u32 angle;
  u32 left;
  u32 right;
  u32 dir;

  result = 0;
  if (objectEvent->paused == 1) {
    if (objectEvent->riseCounter == 0) {
      ObjectEvent_SetMode(objectEvent, 1);
      return 1;
    }
  }
  if (Vale_DistanceInPixels(&target->x.fixed, &objectEvent->x.fixed) < range ||
      force != 0) {
    angle = (u16)ArcTan2(target->z.fixed - objectEvent->z.fixed,
                         target->x.fixed - objectEvent->x.fixed);
    left = (angle - 0x1000) & 0xf000;
    right = (angle + 0x1000) & 0xf000;
    angle &= 0xf000;
    dir = objectEvent->facing & 0xf000;
    if (angle == dir || right == dir || left == dir || force != 0) {
      objectEvent->paused = 1;
      ObjectEvent_SetMode(objectEvent, 1);
      result = 1;
      objectEvent->riseCounter = result;
    } else {
      objectEvent->paused = 0;
      ObjectEvent_SetMode(objectEvent, 2);
      objectEvent->riseCounter = 0;
    }
  } else {
    objectEvent->paused = 0;
    ObjectEvent_SetMode(objectEvent, 2);
    objectEvent->riseCounter = 0;
  }
  return result;
}

/// The update hook of the man guarding Mt. Aleph: while he walks, he stops
/// when the leader comes within 18 pixels in front of him (Vale_TestFacing).
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 48 to 44 bytes in tbs-en.
#ifdef UBFIX
static void Vale_HaveGuardWatchTheLeader(union FieldObject *fieldObject) {
#else
static s32 Vale_HaveGuardWatchTheLeader(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *guard = &fieldObject->objectEvent;
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (guard->targetX == OBJ_EVENT_NO_TARGET &&
      guard->targetZ == OBJ_EVENT_NO_TARGET)
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  Vale_TestFacing(guard, leader, 18, 0);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// The Move hand's update hook in the opening: each frame it sheds a sparkle
/// (SPRITE_SPARKLE) just below and around it, with SE_ITEM_BREAK.
static void Vale_TrailSparkles(union FieldObject *fieldObject) {
  struct ObjectEvent *source = &fieldObject->objectEvent;
  s32 spawnPosition[3];
  s32 randomDistance;
  struct ObjectEvent *particle;
  SET_VECTOR(spawnPosition, source->x.fixed,
             source->y.fixed - (Engine_RandomNext() << 4) - PIXELS(8),
             source->z.fixed);
  randomDistance = Engine_RandomNext();
  Vector_AddPolarOffset(((randomDistance << 1) + randomDistance) << 4,
                        Engine_RandomNext(), spawnPosition);
  particle = Engine_ObjectCreate(SPRITE_SPARKLE, spawnPosition[0],
                                 spawnPosition[1], spawnPosition[2]);
  if (particle != 0) {
    particle->motionFlags = 2;
    particle->gravity = 0x1999;
    particle->wait = 12;
    Engine_ObjectEventSetSpriteFlags(particle, 0);
    ObjectEvent_SetMode(particle, 0);
    Engine_ObjectSetScript(particle, gVale_SparkleScript);
    {
      struct Sprite *record = particle->sprite;
      s32 priorityFlags = ~12;
      priorityFlags &= ((u8 *)&record->image)[9];
      priorityFlags |= 4;
      ((u8 *)&record->image)[9] = priorityFlags;
    }
  }
  Engine_AudioPlayCue(SE_ITEM_BREAK);
}

/// The Move hand rises, spins and shrinks for 31 frames, bursts into eight
/// sparkles with SE_131, and is put out of sight.
static void Vale_VanishMoveHand(struct ObjectEvent *emitter) {
  s32 frameCountdown;

  Engine_AudioPlayCue(SE_CAST);

  for (frameCountdown = 30; frameCountdown >= 0; frameCountdown--) {
    emitter->y.fixed += PIXELS(1);
    emitter->facing = (u16)(emitter->facing + 0x2000);
    emitter->scaleX += -2048;
    emitter->scaleY += -2048;
    Engine_TaskWait(1);
  }

  for (frameCountdown = 7; frameCountdown >= 0; frameCountdown--) {
    struct ObjectEvent *particle = Engine_ObjectCreate(
        SPRITE_SPARKLE, emitter->x.fixed, emitter->y.fixed, emitter->z.fixed);

    if (particle != 0) {
      s32 verticalSpeed;

      Engine_ObjectEventSetSpriteFlags(particle, 0);
      Engine_ObjectSetScript(particle, (const void *)&gVale_SparkleScript[1]);

      verticalSpeed = Engine_RandomNext() + PIXELS(1);
      particle->acceleration = Q16_ONE;
      particle->speed = verticalSpeed;
      particle->motionFlags = 2;
      particle->gravity = 0x0a3d;

      particle->velocityY = Engine_RandomNext() - Engine_RandomNext();

      Vale_UpdateParticlePosition(particle,
                                  ((Engine_RandomNext() * 3) << 3) + PIXELS(8),
                                  Engine_RandomNext());
    }
  }

  Engine_AudioPlayCue(SE_131);

  emitter->x.fixed = 0;
  emitter->y.fixed = 0;
  emitter->z.fixed = 0;
  emitter->targetX = OBJ_EVENT_NO_TARGET;
  emitter->targetY = OBJ_EVENT_NO_TARGET;
  emitter->targetZ = OBJ_EVENT_NO_TARGET;
  SET_VELOCITY(emitter, 0, 0, 0);
}

/// Moves a particle outward in the horizontal plane without changing its
/// height.
static void Vale_UpdateParticlePosition(struct ObjectEvent *particle,
                                        s32 distance, s32 angle) {
  s32 position[3];
  if (particle != 0) {
    SET_VECTOR(position, particle->x.fixed, particle->y.fixed,
               particle->z.fixed);
    Vector_AddPolarOffset(distance, angle, position);
    Engine_ObjectSetPosition(particle, position[0], position[1], position[2]);
  }
}

/// Pushing the pillar in Vale (metatile behaviors 30 and 31) moves it a cell.
static void EventScript_PushPillar(void) {
#if !TBS_PATCH_1
  // Japanese edition only: look up the pillar but do not use the return value.
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED))
    ObjectEvent_GetById(LOCALID_PILLAR_AFTER_KALAY);
  else
    ObjectEvent_GetById(LOCALID_PILLAR);
#endif
  MapGrid_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  PushedObject_AdvancePair();
  EventScript_SettleMovedPillar();
}

/// Records where the pillar now stands and blocks its cell. After Move or a
/// push, it records the pillar's column in FLAG_MAPSEC_TEMP_14 to _16 and
/// blocks the cell the pillar now stands on.
static void EventScript_SettleMovedPillar(void) {
  struct ObjectEvent *objectEvent;
  s32 column;

  MapGrid_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED))
    objectEvent = ObjectEvent_GetById(LOCALID_PILLAR_AFTER_KALAY);
  else
    objectEvent = ObjectEvent_GetById(LOCALID_PILLAR);
  if (objectEvent == NULL)
    return;
  GameFlag_Clear(FLAG_MAPSEC_TEMP_14);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_15);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_16);
  column = objectEvent->x.fixed >> 20;
  if (column == 22) {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, column, 36);
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_14);
  } else if (column == 23) {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, column, 36);
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_15);
  } else {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, 24, 36);
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_16);
  }
}

/// The callback that plays SE_131 every 16 frames while Isaac's Psynergy lifts
/// a board at the roof's end.
static void Vale_PlayStepSound(void) {
  if ((*(u32 *)&gFrameCount & 15) == 0)
    Engine_AudioPlayCue(SE_131);
}

/// Searching a locked door in Vale (metatile behavior 24).
static void EventScript_SearchTheLockedDoor(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgValeDoorWontOpen, 1);
  Engine_ScriptEnd();
}

/// Pauses the man guarding Mt. Aleph while the corresponding event flag is set.
static void Vale_SynchronizeAlephGuardPause(void) {
  s32 pauseFlag;

  if (GameFlag_IsSet(FLAG_MAP_TEMP_41) != 0) {
    pauseFlag = GameFlag_IsSet(FLAG_MENU_OPEN);
    if (pauseFlag == 0) {
      ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH)->paused = pauseFlag;
      GameFlag_Clear(FLAG_MAP_TEMP_41);
    }
  } else {
    if (GameFlag_IsSet(FLAG_MENU_OPEN) != 0) {
      ObjectEvent_GetById(LOCALID_MAN_GUARDING_MT_ALEPH)->paused = 1;
      GameFlag_Set(FLAG_MAP_TEMP_41);
    }
  }
}

/// Keeps the raised pillar's priority in step with the leader's height.
static void Vale_AdjustPillarPriorityForLeaderHeight(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct ObjectEvent *pillar;
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0)
    pillar = ObjectEvent_GetById(LOCALID_PILLAR_AFTER_KALAY);
  else
    pillar = ObjectEvent_GetById(LOCALID_PILLAR);
  if (pillar != 0) {
    if (leader->y.fixed > PIXELS(200))
      pillar->priorityFlags =
          OBJ_EVENT_PRIORITY_AUTOMATIC | OBJ_EVENT_PRIORITY_UNDERFOOT;
    else
      pillar->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }
}

#include "common/object_orbit.h"

/// Someone calling up Psynergy: drawn SPRITE_COLOR_WHITE (7) for two frames in
/// four, and the world map's linked sparks every 16 frames.
static void
Vale_FlashObjectEventAndSpawnEffects(struct ObjectEvent *objectEvent) {

  if ((gFrameCount & 2) != 0)
    Engine_ObjectSetPartPalettes(objectEvent, 7);
  else
    Engine_ObjectSetPartPalettes(objectEvent, 0);
  if ((gFrameCount & 0xf) == 0)
    WorldMap_CreateLinkedEffects(objectEvent);
}

/// Someone using Psynergy: colour modes SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG
/// (0 to 5), the sprite and its five swaps of colour channels, in turn, and
/// sparks every 16 frames.
static void
Vale_CycleObjectEventPaletteAndSpawnEffects(struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 1) != 0)
    Engine_ObjectSetPartPalettes(objectEvent, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 0xf) == 0)
    WorldMap_CreateLinkedEffects(objectEvent);
}

/// What Psynergy moves: colour modes SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG in
/// turn, a step every second frame.
static void Vale_CycleObjectEventPalette(struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 1) != 0)
    Engine_ObjectSetPartPalettes(objectEvent, (gFrameCount >> 1) % 6);
}

#include "common/effect_vertical.h"
#include "common/world_map_linked_effects.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

/// The man guarding Mt. Aleph walks his beat below the path once he has warned
/// Isaac, looking about at each stop. SW_TIMER says which stretch he is on,
/// 1 to 3, of which the warning scripts act on the two pauses, 1 and 2.
s32 gVale_AlephGuardPatrol[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 4),
    SC_SET(SW_TIMER, 2),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(296), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_TIMER, 3),
    SC_MOVE(PIXELS(296), 0, PIXELS(420)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(296), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_TIMER, 2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(15),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(60),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_MOVE(PIXELS(212), 0, PIXELS(388)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(40),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(429)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_TIMER, 2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(40),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/// Isaac on the roof, his sprite stretching and settling with each blow of
/// the hammer.
s32 gVale_LeaderHammers[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 1966),
    SC_WAIT(12),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(12),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

// Garet and Jenna walk to the front of Isaac's house to watch him on the roof.

s32 gVale_GaretWalksToTheHouse[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(300), 0, PIXELS(880)),
    SC_SYNC,
    SC_MOVE(PIXELS(300), 0, PIXELS(904)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

s32 gVale_JennaWalksToTheHouse[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(300), 0, PIXELS(904)),
    SC_SYNC,
    SC_MOVE(PIXELS(321), 0, PIXELS(904)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

/// A spout of water grows from a tenth of its size north of the strongman by
/// the river and vanishes (EventScript_UnusedWaterBurstsByTheRiver).
s32 gVale_UnusedWaterGrows[] = {
    SC_ANIMATION(1),
    SC_POSITION(PIXELS(266), 0, PIXELS(1230)),
    SC_SET(SW_SCALE_X, 0x1999),
    SC_SET(SW_SCALE_Y, 0x1999),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 3932),
    SC_ADD(SW_SCALE_Y, 0x147a),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// A sparkle falls and vanishes after 10 frames; the burst's sparkles start at
/// its second word, past the first SC_FREE.
s32 gVale_SparkleScript[] = {
    SC_FREE,
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(1) / 16),
    SC_ANIMATION(0),
    SC_WAIT(10),
    SC_FREE,
};

struct MapWarpDest gVale_WarpDests[] = {
    { 0, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_SANCTUM, CONDITION_ALWAYS, 256, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_KRADEN_NORTH, CONDITION_ALWAYS, 38, 0, 636, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_KRADEN_SOUTH, CONDITION_ALWAYS, 29, 0, 792, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_GATE, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_INTERIORS_SHOPS_EAST, CONDITION_ALWAYS, 406, 0, 743, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_INTERIORS_SHOPS_SOUTH, CONDITION_ALWAYS, 262, 0, 821, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_INTERIORS_INN, CONDITION_ALWAYS, 340, 0, 904, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_INTERIORS_SHOPS_NORTH, CONDITION_ALWAYS, 326, 0, 1142, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_INTERIORS_SHOPS_WEST, CONDITION_ALWAYS, 374, 0, 1254, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_TITLE, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_ROOF_MENDED, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gVale_Warps[] = {
    MAP_WARPS(Vale),
    MAP_WARP(VALE_TO_SOL_ENTRANCE, SolEntrance, SOL_ENTRANCE_FROM_VALE),
    MAP_WARP(VALE_TO_VALE_GATE, ValeGate, VALE_GATE_FROM_VALE),
    MAP_WARP(VALE_TO_VALE_KRADEN_NORTH, ValeKraden, VALE_KRADEN_FROM_VALE_NORTH),
    MAP_WARP(VALE_TO_VALE_KRADEN_SOUTH, ValeKraden, VALE_KRADEN_FROM_VALE_SOUTH),
    MAP_WARP(VALE_TO_VALE_INTERIORS_SHOPS_EAST, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_EAST),
    MAP_WARP(VALE_TO_VALE_INTERIORS_SHOPS_SOUTH, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH),
    MAP_WARP(VALE_TO_VALE_INTERIORS_INN, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE),
    MAP_WARP(VALE_TO_VALE_INTERIORS_SHOPS_NORTH, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_NORTH),
    MAP_WARP(VALE_TO_VALE_INTERIORS_SHOPS_WEST, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_WEST),
    MAP_WARP(VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_NORTH),
    MAP_WARP(VALE_TO_SANCTUM, Sanctum, SANCTUM_FROM_VALE),
    MAP_WARP(VALE_TO_ROOF_MENDED, Vale, VALE_FROM_ROOF_MENDED),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gVale_ObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(19), 0, PIXELS(614), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(50), 0, PIXELS(795), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(65), 0, PIXELS(800), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(57), 0, PIXELS(805), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(398), 0, PIXELS(616), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(373), 0, PIXELS(VALE_DORA_Z), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, 7,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gVale_ObjectEventTemplatesAfterLeaving[] = {
    { JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(19), 0, PIXELS(614), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(50), 0, PIXELS(795), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(65), 0, PIXELS(800), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(57), 0, PIXELS(805), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_WEST, 3, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(398), 0, PIXELS(616), FACING_NORTH, 3, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(335), 0, PIXELS(869), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, 7,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gVale_ObjectEventTemplatesAfterKalay[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(356), 0, PIXELS(727), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACING_KEEP, 0 },
    { 0xd6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xd6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xd6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gVale_RepairObjectEventTemplates[] = {
    { JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(409), 0, PIXELS(1134), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(428), 0, PIXELS(588), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(204), 0, PIXELS(924), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(394), 0, PIXELS(584), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(373), 0, PIXELS(VALE_DORA_Z), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xd7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gVale_Events[] = {
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephLooksBeautifulTodayIts) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeValeStaysPristineBecauseOfMt) },
    { MAP_EVENT_TALK, LOCALID_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutTheMayorAndTremors) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeThisPlaceLooksJustLikeIt) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacWatchOutDontGetToo) },
    { MAP_EVENT_TALK, LOCALID_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeValeMustRemainHiddenPsynergysSecrets) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarnAboutGoingBeyondKradensCottage) },
    { MAP_EVENT_TALK, LOCALID_GARETS_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ArgueWithGaretsSister) },
    { MAP_EVENT_TALK, LOCALID_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeYoureInTroubleNowGaretYour) },
    { MAP_EVENT_TALK, LOCALID_DORA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DoraFretsAboutTheRoof) },
    { MAP_EVENT_TALK, LOCALID_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephIsASacredPeak) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterSanctum) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ApproachMtAleph) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenNorth) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenSouth) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGate) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterGaretsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterValeInteriorsShops) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterIsaacsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterJennasGrandparentsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterValeInteriorsShopsWest) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { MAP_EVENT_COORD, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetPathCellOverride) },
    { MAP_EVENT_COORD, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClearPathCellOverride) },
    { MAP_EVENT_COORD, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WatchThePuppies) },
    { MAP_EVENT_COORD, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TurnBackFromThePuppies) },
    { MAP_EVENT_COORD, 50, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT_SCRIPT(EventScript_WarnAgainstSneakingUpMtAleph) },
    { MAP_EVENT_COORD, 51, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RepeatMtAlephWarningFromFirstApproach) },
    { MAP_EVENT_COORD, 52, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RepeatMtAlephWarningFromSecondApproach) },
    { MAP_EVENT_COORD, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RepeatMtAlephWarningAtEitherPause) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbLadderOntoRoof) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbDownFromRoof) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_NUT, FLAG_VALE_NUT_CAUGHT, EVENT_SCRIPT(EventScript_CatchNut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SettleMovedPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { BG_EVENT(BG_EVENT_SILENT), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchTheLockedDoor) },
    { MAP_EVENTS_END },
};

struct MapEvent gVale_EventsAfterLeaving[] = {
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeIsSavedThanksToThe) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeEverSinceTheTremorsStartedMt) },
    { MAP_EVENT_TALK, LOCALID_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutTheMayorAndTremors) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeSighNoLongerWillISee) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIDontBlameFelixIfOnly) },
    { MAP_EVENT_TALK, LOCALID_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeTheMayorWasTooKindTo) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarnAboutGoingBeyondKradensCottage) },
    { MAP_EVENT_TALK, LOCALID_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreSoLuckyGaret) },
    { MAP_EVENT_TALK, LOCALID_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseTakeCareOfMyBrother) },
    { MAP_EVENT_TALK, LOCALID_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeGeeIWishICouldSee) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeMyBrothersStrengthIsHisBest) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeImGladThatFelixIsAlive) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeFelixWasAGoodPersonI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, MSGNO(MsgValePoorKradenWasTakenToSome) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeWasSavedBecausePeopleLike) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeICantBelieveAnyoneWouldClimb) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, MSGNO(MsgValeIJustDontFeelSafeKnowing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeThePeopleOfValeWillNever) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterSanctum) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ApproachMtAleph) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenNorth) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenSouth) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGate) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterGaretsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterValeInteriorsShops) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterIsaacsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterJennasGrandparentsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterValeInteriorsShopsWest) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { MAP_EVENT_COORD, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetPathCellOverride) },
    { MAP_EVENT_COORD, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClearPathCellOverride) },
    { MAP_EVENT_COORD, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WatchThePuppies) },
    { MAP_EVENT_COORD, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TurnBackFromThePuppies) },
    { MAP_EVENT_COORD, 50, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT_SCRIPT(EventScript_WarnAgainstSneakingUpMtAleph) },
    { MAP_EVENT_COORD, 51, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RepeatMtAlephWarningFromFirstApproach) },
    { MAP_EVENT_COORD, 52, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RepeatMtAlephWarningFromSecondApproach) },
    { MAP_EVENT_COORD, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RepeatMtAlephWarningAtEitherPause) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbLadderOntoRoof) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbDownFromRoof) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_NUT, FLAG_VALE_NUT_CAUGHT, EVENT_SCRIPT(EventScript_CatchNut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SettleMovedPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { BG_EVENT(BG_EVENT_SILENT), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchTheLockedDoor) },
    { MAP_EVENTS_END },
};

struct MapEvent gVale_EventsAfterKalay[] = {
    { MAP_EVENT_TALK, LOCALID_GARETS_SISTER_AFTER_KALAY, FLAG_MAPSEC_TEMP_03, EVENT_SCRIPT(EventScript_GaretMeetsHisSister) },
    { MAP_EVENT_TALK, LOCALID_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICanTellJustByLooking) },
    { MAP_EVENT_TALK, LOCALID_GARETS_BROTHER_AFTER_KALAY, FLAG_MAPSEC_TEMP_04, EVENT_SCRIPT(EventScript_GaretAsksToProtectFamily) },
    { MAP_EVENT_TALK, LOCALID_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeIWillDoMyBestTo) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT_SCRIPT(EventScript_AskWhetherJennaHasReturned) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValePromiseMeIsaacPromiseMeYoull) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeIUsedToPlayHere) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephsEruptionWasSoPowerful) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReactToAnotherTremor) },
    { MAP_EVENT_TALK, LOCALID_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheDangerIsGone) },
    { MAP_EVENT_TALK, LOCALID_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_GARETS_SISTER_AFTER_KALAY, FLAG_MAPSEC_TEMP_03, EVENT_SCRIPT(EventScript_GaretMeetsHisSister) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreWorkingSoHard) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_GARETS_BROTHER_AFTER_KALAY, FLAG_MAPSEC_TEMP_04, EVENT_SCRIPT(EventScript_GaretAsksToProtectFamily) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICouldGoWithYouIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT_SCRIPT(EventScript_AskWhetherJennaHasReturned) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValeYouDidntBringJennaWithYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeFelixMustBeProtectingJennaIm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeTheEarthquakesArentCausedByEruptions) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, MSGNO(MsgValeAhhICantStandTheseEarthquakes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheSmokeFromMtAlephIsnt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_MAN_MISSING_TRAVELERS, FLAG_MAPSEC_TEMP_07, EVENT_SCRIPT(EventScript_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgValeHavingNoTravelersIsNoFun) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterSanctum) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ApproachMtAleph) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenNorth) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeKradenSouth) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarpToValeGate) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterGaretsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterValeInteriorsShops) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterIsaacsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterJennasGrandparentsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterValeInteriorsShopsWest) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { MAP_EVENT_COORD, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetPathCellOverride) },
    { MAP_EVENT_COORD, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClearPathCellOverride) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbLadderOntoRoof) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbDownFromRoof) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_NUT_AFTER_KALAY, FLAG_VALE_NUT_CAUGHT, EVENT_SCRIPT(EventScript_CatchNutAfterKalay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_PILLAR_AFTER_KALAY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SettleMovedPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { BG_EVENT(BG_EVENT_SILENT), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchTheLockedDoor) },
    { MAP_EVENTS_END },
};
// clang-format on

// The doors' cell animations, from the layout's cells below the village, each
// picture shown for two frames; vale_storm.c has the same four kinds. Each
// table takes the name of a door it opens. The two of Jenna's grandparents'
// kind and the west house's show their one open picture twice, where the
// stormy night's show two.

u16 gVale_SanctumDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVale_IsaacsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVale_GrandparentsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVale_WestHouseDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};

// The man guarding Mt. Aleph drives Isaac back down the path: at the first
// warning (EventScript_WarnAgainstSneakingUpMtAleph), and again from the west
// or from either of the guard's pauses (Vale_AskAboutSneakingToAleph).

s32 gVale_FirstWarningLeaderPath[] = {
    SC_ANIMATION(5), SC_MOVE(PIXELS(199), 0, PIXELS(446)),
    SC_SYNC,         SC_MOVE(PIXELS(215), 0, PIXELS(472)),
    SC_SYNC,         SC_MOVE(PIXELS(239), 0, PIXELS(461)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gVale_FirstWarningGuardPath[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(199), 0, PIXELS(446)),
    SC_SYNC,         SC_MOVE(PIXELS(215), 0, PIXELS(472)),
    SC_SYNC,         SC_MOVE(PIXELS(239), 0, PIXELS(461)),
    SC_SYNC,         SC_ANIMATION(1),
    SC_END,
};

s32 gVale_LeaderFromWestPath[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_ANIMATION(5),
    SC_MOVE(PIXELS(224), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(474)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

s32 gVale_GuardFromFirstPausePath[] = {
    SC_SET(SW_SPEED, PIXELS(3) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(1)),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

s32 gVale_LeaderPushedBackPath[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCELERATION, PIXELS(2)),
    SC_ANIMATION(5),
    SC_MOVE(PIXELS(256), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(474)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

s32 gVale_GuardFromSecondPausePath[] = {
    SC_SET(SW_SPEED, PIXELS(3) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(1)),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};
