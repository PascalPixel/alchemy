/// @file
/// SceneId_Vale, Isaac's village by day, outside the houses: the door of
/// Isaac's house, the houses of Garet's family and of Jenna's grandparents, the
/// Sanctum and the paths out to Mt. Aleph and Kraden's cottage. The
/// actor placements and triggers come in three sets, chosen by the story
/// flags: before the party leaves Vale, after it has left
/// (FLAG_VALE_PARTY_LEFT) and back from Kalay (FLAG_KALAY_REACHED);
/// VALE_FROM_TITLE has a set of its own. In story order:
///
/// - The day the story begins, three years after the storm
///   (cottage_rooms_b.c), which took Jenna's family and Isaac's father,
///   Kyle. The game opens at VALE_FROM_TITLE with Vale_MendingTheRoof:
///   under the caption "Three years later...", Isaac is mending the roof of his
///   house while Dora, his mother, calls up to him; Garet, who will travel with
///   him, trains his Psynergy nearby, and Jenna comes by, and the two speak of
///   that day before going to fetch Isaac to go to Kraden's.
///   VALE_TO_ROOF_MENDED brings the party back in at VALE_FROM_ROOF_MENDED,
///   where Vale_RoofMended plays once (area flag
///   FLAG_VALE_ROOF_MENDED_EVENT_PLAYED): Dora praises the roof, remembers
///   Kyle, and sends the three off on their climb of Mt. Aleph with Kraden, the
///   scholar who teaches them Alchemy (FLAG_VALE_ROOF_JUST_MENDED); for the
///   rest of that visit Dora frets about the hole clumsy Garet left.
/// - Vale before the party leaves. Until Kraden has planned the visit to Sol
///   Sanctum (FLAG_SANCTUM_VISIT_PLANNED), Jenna stops Isaac on the path to Mt.
///   Aleph; a bearded man in a long robe jumps out to warn him against sneaking
///   up the sacred peak (FLAG_VALE_WARNED_AGAINST_MT_ALEPH) and then keeps
///   watching him, asking again whenever he comes near (cell behaviors 51
///   to 53). Three puppies (from ACTOR_FIRST_PUPPY) play; walking up to them
///   from either side (cell behaviors 20 and 21) sets them barking, until
///   the party has met Saturos and Menardi, of the strangers staying at the inn
///   (FLAG_MET_SATUROS_AND_MENARDI); then the setup removes them. Villagers
///   talk of the mayor, Garet's grandfather, and of not going beyond Kraden's
///   cottage, Garet's sister scolds him, and Dora says Isaac can fix the hole
///   later.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT): Dora will not open
///   the door of Isaac's house to him ("My Isaac would keep his promise... Go
///   away."), and the villagers speak of the tremors and of Kraden, who has not
///   come back.
/// - Back from Kalay (FLAG_KALAY_REACHED): Garet meets his sister (area
///   flag FLAG_VALE_GARET_MET_SISTER) and asks one of his family to protect it
///   while he is gone (FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY), a young man
///   asks whether Jenna is back (FLAG_VALE_ASKED_IF_JENNA_IS_BACK), another
///   feels a tremor, another asks about the lack of travellers since the
///   eruption (FLAG_VALE_ASKED_ABOUT_TRAVELERS). Isaac's house opens again:
///   Dora lies ill inside (cottage_rooms_b.c).
///
/// In every set: a Nut to catch with Catch (FLAG_VALE_NUT_CAUGHT; ACTOR_NUT,
/// or ACTOR_NUT_AFTER_KALAY), a pillar to push or Move, whose column the
/// setup restores from FLAG_VALE_PILLAR_IN_WEST_COLUMN, _IN_MIDDLE_COLUMN and
/// _IN_EAST_COLUMN, a ladder up onto the roof of Isaac's house, climbed on
/// cell behaviors 100 and 101 (FLAG_VALE_ON_ROOF while on the roof), and
/// cells of the bridge over the river that change while the leader is on it
/// (FLAG_VALE_ON_BRIDGE); after a battle (FLAG_KEEP_PARTY_POSITION) the
/// setup restores both. Exits: VALE_TO_SOL_ENTRANCE up Mt. Aleph to Sol
/// Entrance (SceneId_SolEntrance), VALE_TO_VALE_PLAZA to SceneId_ValePlaza,
/// VALE_TO_VALE_KRADENS_COTTAGE_NORTH and VALE_TO_VALE_KRADENS_COTTAGE_SOUTH to
/// SceneId_ValeKradensCottage, the doors VALE_TO_COTTAGE_ROOMS_A_EAST,
/// VALE_TO_COTTAGE_ROOMS_A_SOUTH, VALE_TO_COTTAGE_ROOMS_A_NORTH,
/// VALE_TO_COTTAGE_ROOMS_A_WEST and
/// VALE_TO_COTTAGE_ROOMS_A_NORTH_SECOND_DOOR into the houses
/// (SceneId_CottageRoomsA, cottage_rooms_a.c),
/// VALE_TO_COTTAGE_ROOMS_B into Isaac's house (SceneId_CottageRoomsB) and
/// VALE_TO_SANCTUM into SceneId_Sanctum. Two events no table reaches remain:
/// Vale_UnusedRoofMended, a copy of the roof's end, and
/// Vale_UnusedWaterBurstsByTheRiver, which sets area flag
/// FLAG_VALE_UNUSED_WATER_BURST_SEEN. FLAG_VALE_MT_ALEPH_GUARD_PAUSED pauses
/// the man who watches Isaac while a menu is open. The TBS_PATCH_4 editions
/// also set Dora's sprite priority before Kalay.
///
/// In the opening Garet trains his Psynergy on the pillar: his sprite flashes
/// white, then cycles through its colour modes with sparks about him, while the
/// hand of Move (ACTOR_OPENING_MOVE_HAND) pushes the pillar west, the pillar
/// cycling its colour modes too, and sheds sparkles.

#include "adventure.h"
#include "vram_cache.h"
#include "module/import.h"
#include "module/scene/vale.h"
#include "module/scene/shared/actor_orbit.h"
#include "module/scene/shared/effect_psynergy.h"
#include "module/scene/shared/world_map_attached_effects.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/runtime/runtime.h"
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
#include "heap_slots.h"
#include "edition.h"
#include "vale.h"
#include "types.h"
#include "call.h"
#include "event.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "actor.h"
#include "field_effect.h"
#include "map_grid_scroll.h"
#include "fixed_math.h"
#include "macros.h"
#include "flags.h"
#include "sounds.h"
#include "heap.h"

// Where Garet lands after his jump once Dora has gone in, at the roof's end
// (Vale_RoofMended): two pixels further south in the TBS_PATCH_2
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
static void Vale_MoveActorAlongLowerZPath(s32 actorId, s32 approachAnimationId,
                                          s32 departureAnimationId,
                                          s32 resumeMotion);
static void Vale_MoveActorAlongHigherZPath(s32 actorId, s32 approachAnimationId,
                                           s32 departureAnimationId,
                                           s32 resumeMotion);
static void Vale_PlaceOrHideBoards(s32 first, u32 count, s32 mode);
static void Vale_MendingTheRoof(void);
static void Vale_RoofMended(void);
static void Vale_UpdateGaretPaletteAndEffects(void);
static void Vale_UpdateLeaderPaletteAndEffects(void);
static void Vale_CycleOpeningPillarPalette(void);
static void Vale_CycleFirstBoardPalette(void);
static void Vale_CycleSecondBoardPalette(void);
static void Vale_CycleThirdBoardPalette(void);
#ifdef UBFIX
static void Vale_HaveGuardWatchTheLeader(struct Actor *actor);
#else
static s32 Vale_HaveGuardWatchTheLeader(struct Actor *actor);
#endif
static void Vale_TrailSparkles(struct Actor *actor);
static void Vale_VanishMoveHand(struct Actor *emitter);
static void Vale_UpdateParticlePosition(struct Actor *particle, s32 distance,
                                        s32 angle);
static void Vale_MovePillarBelowCliff(void);
static void Vale_PlayStepSound(void);
static void Vale_SynchronizeAlephGuardPause(void);
static void Vale_AdjustPillarPriorityForLeaderHeight(void);
static void Vale_FlashActorAndSpawnEffects(struct Actor *actor);
static void Vale_CycleActorPaletteAndSpawnEffects(struct Actor *actor);
static void Vale_CycleActorPalette(struct Actor *actor);

/// The placed actors of Vale by day, numbered in the order of
/// gVale_ActorPlacements and gVale_ActorPlacementsAfterLeaving, which
/// place the same people in the same rows. A row whose sprite is a party
/// member's (Garet's, Jenna's) takes that party member's actor id instead
/// (Event_SpawnActorPlacements), so the numbering skips it.
enum ValeActorId {
  /// An old woman: "Mt. Aleph looks beautiful today. It's so peaceful."; after
  /// the party has left, "Vale is saved thanks to the Wise One!"
  /// ★ Named from the game: "Mt. Aleph looks beautiful today."
  /// (MsgValeMtAlephLooksBeautifulTodayIts)
  ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH = ACTOR_FIRST_PLACED,
  /// A woman in a green dress: "Vale stays pristine because of Mt. Aleph's
  /// purity."
  /// ★ Named from the game: "Vale stays pristine because of Mt. Aleph's
  /// purity." (MsgValeValeStaysPristineBecauseOfMt)
  ACTOR_WOMAN_ON_ALEPHS_PURITY,
  /// A little girl with orange pigtails: "I'm jealous of you, Garet, because
  /// your grandpa's the mayor." (Vale_TalkAboutTheMayorAndTremors)
  /// ★ Named from the game: "I'm jealous of you,"
  /// (MsgValeYourGrandpaIsTheMayor)
  ACTOR_GIRL_JEALOUS_OF_GARET,
  /// A young man with brown hair: "This place looks just like it did three
  /// years ago... after the storm..."
  /// ★ Named from the game: "This place looks just like it did three years
  /// ago... after the storm..." (MsgValeThisPlaceLooksJustLikeIt)
  ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM,
  /// A strongman by the river: "Isaac, watch out! Don't get too close to the
  /// river!"
  /// ★ Named from the game: "Don't get too close to the river!"
  /// (MsgValeIsaacWatchOutDontGetToo)
  ACTOR_STRONGMAN_BY_THE_RIVER,
  /// A burly grey-haired man: "Vale must remain hidden. Psynergy's secrets
  /// mustn't fall into the wrong hands."
  /// ★ Named from the game: "Vale must remain hidden."
  /// (MsgValeValeMustRemainHiddenPsynergysSecrets)
  ACTOR_MAN_KEEPING_VALE_HIDDEN,
  /// A woman with fair hair who stands by the way out to Kraden's cottage:
  /// "Do you need to go past here?", then "You mustn't go beyond Kraden's
  /// cottage." (Vale_WarnAboutGoingBeyondKradensCottage)
  /// ★ Named from the game: "You mustn't go beyond Kraden's cottage."
  /// (MsgValeDontGoBeyondKradensCottage)
  ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN,
  /// The three puppies, small white dogs, removed once the party has met
  /// Saturos and Menardi. "Rrruff! Rrrruff!"
  /// ★ Named from what the player sees: small white dogs with brown ears (its
  /// render)
  ACTOR_FIRST_PUPPY,
  /// A young woman with an orange ponytail: "Garet, you make me so mad
  /// sometimes!" (Vale_ArgueWithGaretsSister); Garet calls her "Sis"
  /// back
  /// from Kalay.
  /// ★ Named from the game: "Your sister's really mad!"
  /// (MsgValeYoureInTroubleNowGaretYour)
  ACTOR_GARETS_SISTER = ACTOR_FIRST_PUPPY + 3,
  /// "You're in trouble now, Garet... Your sister's really mad!"; after the
  /// party has left, "Please take care of my brother, Isaac."
  /// ★ Named from the game: "Please take care of my brother"
  /// (MsgValePleaseTakeCareOfMyBrother), said by a small boy with orange hair
  /// in a green shirt and shorts (its render)
  ACTOR_GARETS_BROTHER,
  /// SPRITE_STONE_PILLAR, as Goma Cave's pillars: pushed
  /// (Vale_PushPillar) or moved with Move, its column kept in
  /// FLAG_VALE_PILLAR_IN_WEST_COLUMN, _IN_MIDDLE_COLUMN and _IN_EAST_COLUMN
  /// (Vale_MovePillarBelowCliff).
  /// ★ Named from what the player sees: a stone pillar of brown rock standing
  /// below the cliff north of the village (its render)
  ACTOR_PILLAR,
  /// ★ Named from the game: "your mother, Dora, has been crying"
  /// (MsgValeDoraHasBeenCrying)
  ACTOR_DORA, ///< Isaac's mother
  /// A man with long brown hair and a beard in a long robe, who jumps out to
  /// warn Isaac off the sacred peak, then keeps watching him
  /// (Vale_WarnAgainstSneakingUpMtAleph). "Mt. Aleph is a sacred peak.
  /// It is
  /// forbidden to climb it!"
  /// ★ Named from the game: "Mt. Aleph is a sacred peak."
  /// (MsgValeMtAlephIsASacredPeak)
  ACTOR_MAN_GUARDING_MT_ALEPH,
  /// The boards on the roof, sprite 0xce, which Isaac moves into place with
  /// Psynergy at the roof's end (Vale_RoofMended).
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  ACTOR_FIRST_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  ACTOR_SECOND_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  ACTOR_THIRD_BOARD,
  /// ★ Named from the game: "Nut" (MsgItemName), the item Catch takes
  ACTOR_NUT ///< the Nut for Catch, until FLAG_VALE_NUT_CAUGHT is set
};

/// Back from Kalay, the placed actors of
/// gVale_ActorPlacementsAfterKalay; Garet's row takes his own actor id.
enum ValeAfterKalayActorId {
  /// Garet's sister. "Ah! It's been a while... You're back?"
  /// (Vale_GaretMeetsHisSister)
  /// ★ Named from the game: "Thanks, Sis." (MsgValeAh), Garet answering her
  ACTOR_GARETS_SISTER_AFTER_KALAY = ACTOR_FIRST_PLACED,
  /// "I will do my best to show everyone that I am strong enough to protect
  /// our family and Vale." (Vale_GaretAsksToProtectFamily)
  /// ★ Named from the game: "Please take care of my brother"
  /// (MsgValePleaseTakeCareOfMyBrother)
  ACTOR_GARETS_BROTHER_AFTER_KALAY,
  /// The young man of ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM: "Is Jenna back
  /// yet?" (Vale_AskWhetherJennaHasReturned)
  /// ★ Named from the game: "Is back yet?" (MsgValeIsJennaBackYet)
  ACTOR_YOUNG_MAN_ASKING_FOR_JENNA,
  /// The strongman of ACTOR_STRONGMAN_BY_THE_RIVER: "I still remember when
  /// Felix and I used to play here...*Sniff*"
  /// ★ Named from the game: "I still remember when" (MsgValeIUsedToPlayHere)
  ACTOR_STRONGMAN_REMEMBERING_FELIX,
  /// Three sprites of water, CrossboneIsleFloodgate's water sprite 0xd6. Only
  /// Vale_UnusedWaterBurstsByTheRiver shows them: during a
  /// tremor each in turn grows from a tenth of its size at (266, 1230), north
  /// of ACTOR_STRONGMAN_REMEMBERING_FELIX, and vanishes
  /// (gVale_UnusedWaterGrows). The row after them places the woman with fair
  /// hair of ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN at the origin, and nothing
  /// names her.
  /// ★ Named from what the player sees: a spout of white water spraying up,
  /// spr_214.png (its render)
  ACTOR_FIRST_WATER,
  /// An old woman: "Mt. Aleph's eruption was so powerful, it caused the
  /// continent to crack!"
  /// ★ Named from the game: "Mt. Aleph's eruption was so powerful"
  /// (MsgValeMtAlephsEruptionWasSoPowerful)
  ACTOR_OLD_WOMAN_ON_THE_ERUPTION = ACTOR_FIRST_WATER + 4,
  /// A woman in a green dress: "Aghh! I felt another one!"
  /// (Vale_ReactToAnotherTremor)
  /// ★ Named from the game: "Aghh! I felt another one!"
  /// (MsgValeIFeltAnotherOne)
  ACTOR_WOMAN_FEELING_TREMORS,
  /// A little girl with orange pigtails: "They say the danger is gone, even
  /// though the earthquakes continue..."
  /// ★ Named from the game: "They say the danger is gone, even though the
  /// earthquakes continue..." (MsgValeTheySayTheDangerIsGone)
  ACTOR_GIRL_DOUBTING_SAFETY,
  /// A burly grey-haired man: "I wonder why we haven't seen any travelers
  /// since the eruption..." (Vale_AskAboutTheLackOfTravelers)
  /// ★ Named from the game: "I wonder why we haven't seen any travelers
  /// since the eruption..." (MsgValeNoTravelersSinceTheEruption)
  ACTOR_MAN_MISSING_TRAVELERS,
  /// ★ Named from the game: "Nut" (MsgItemName)
  ACTOR_NUT_AFTER_KALAY,
  /// ★ Named from what the player sees: a stone pillar of brown rock standing
  /// below the cliff north of the village (its render)
  ACTOR_PILLAR_AFTER_KALAY ///< the pillar of ACTOR_PILLAR
};

/// The game's opening (Vale_MendingTheRoof), the placed actors of
/// gVale_RepairActorPlacements; Jenna's and Garet's rows take their own
/// actor ids.
enum ValeOpeningActorId {
  /// A woman in a green dress who meets Jenna on her way.
  /// ★ Named from what the player sees: a woman with brown hair in a green
  /// dress and a white apron (its render)
  ACTOR_OPENING_WOMAN = ACTOR_FIRST_PLACED,
  /// The pillar of ACTOR_PILLAR, which Garet trains his Psynergy on.
  /// ★ Named from what the player sees: a stone pillar of brown rock standing
  /// below the cliff north of the village (its render)
  ACTOR_OPENING_PILLAR,
  /// ★ Named from the game: "your mother, Dora, has been crying"
  /// (MsgValeDoraHasBeenCrying)
  ACTOR_OPENING_DORA, ///< calls up to Isaac on the roof
  /// The boards on the roof, as ACTOR_FIRST_BOARD.
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  ACTOR_OPENING_FIRST_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  ACTOR_OPENING_SECOND_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood,
  /// spr_206.png (its render)
  ACTOR_OPENING_THIRD_BOARD,
  /// Sprite 0xd7, the hand of Move (event_commands/catch.c): it pushes the
  /// pillar for Garet, shedding sparkles.
  /// ★ Named from what the player sees: a large open hand,
  /// spr_215.png (its render)
  ACTOR_OPENING_MOVE_HAND
};

/// The leader's animation of hammering on the roof, beyond enum
/// ActorAnimation's.
enum { ANIM_HAMMER = 11 };

/// Animations of Dora's and Garet's sprites beyond enum ActorAnimation,
/// as their animation tables and frames under graphics/sprite draw them.
enum ValeAnimation {
  ANIM_DORA_HANDS_UP = 7, ///< both hands raised (spr_031.png)
  /// Only her head and hands showing, as she peeks back out (spr_031.png).
  ANIM_DORA_PEEK = 8,
  /// Garet throwing up his arms, then crouching, as he jumps in fright
  /// (spr_001.png).
  ANIM_GARET_STARTLED = 13
};

#include "shared/staged_actor.h"

/// The roof of Isaac's house has just been mended; for the rest of the visit
/// Dora frets about the hole Garet left.
#define FLAG_VALE_ROOF_JUST_MENDED FLAG_SCENE_TEMP_02
/// The leader has climbed the ladder onto the roof of Isaac's house, whose
/// cells are changed so it can be walked.
/// ★ Named from the game: "Haven't you finished mending the roof yet?"
/// (MsgValeRepairCaption)
#define FLAG_VALE_ON_ROOF FLAG_SCENE_TEMP_04
/// The leader is on the rope bridge over the river. While it is set, the
/// bridge's cells take the deck's shapes and cell behaviors: its two middle
/// rows walkable at the deck's height and its rails walls; stepping off puts
/// back the river's, walled water between open banks.
/// ★ Named from what the player sees: a wooden bridge with rope rails over the
/// river, above the waterfall (its render)
#define FLAG_VALE_ON_BRIDGE FLAG_SCENE_TEMP_10
/// The man guarding Mt. Aleph is paused while a menu is open.
/// ★ Named from the game: "Mt. Aleph is a sacred peak."
/// (MsgValeMtAlephIsASacredPeak)
#define FLAG_VALE_MT_ALEPH_GUARD_PAUSED FLAG_SCENE_TEMP_41
/// Back from Kalay, Garet has met his sister.
#define FLAG_VALE_GARET_MET_SISTER FLAG_AREA_TEMP_03
/// Back from Kalay, Garet has asked his brother to protect the family while he
/// is gone.
#define FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY FLAG_AREA_TEMP_04
/// Unused: the water has burst up by the river and the strongman has remembered
/// Felix (Vale_UnusedWaterBurstsByTheRiver, which nothing reaches).
/// ★ Named from the game: "I still remember when" (MsgValeIUsedToPlayHere)
#define FLAG_VALE_UNUSED_WATER_BURST_SEEN FLAG_AREA_TEMP_05
/// Back from Kalay, the burly man has asked about the lack of travellers since
/// the eruption.
#define FLAG_VALE_ASKED_ABOUT_TRAVELERS FLAG_AREA_TEMP_07
/// Vale_RoofMended has played, in which Dora praises the roof and sends
/// the three off to Mt. Aleph.
#define FLAG_VALE_ROOF_MENDED_EVENT_PLAYED FLAG_AREA_TEMP_08
/// The stone pillar below the cliff stands in its west column.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_VALE_PILLAR_IN_WEST_COLUMN FLAG_AREA_TEMP_14
/// The stone pillar below the cliff stands in its middle column, where it
/// starts.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN FLAG_AREA_TEMP_15
/// The stone pillar below the cliff stands in its east column.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_VALE_PILLAR_IN_EAST_COLUMN FLAG_AREA_TEMP_16

const struct SceneEntrance *Vale_GetEntrances(void) { return gVale_Entrances; }

const struct SceneExitRegion *Vale_GetRegions(void) { return NULL; }

const u32 *Vale_GetExits(void) { return gVale_Exits; }

/// Places the villagers for the story so far, or for the roof repair
/// (VALE_FROM_TITLE).
const struct ActorPlacement *Vale_GetActorPlacements(void) {
  u8 *gameStateBytes = (u8 *)&gAdventure;
  if (((struct Adventure *)gameStateBytes)->entrance == VALE_FROM_TITLE)
    return gVale_RepairActorPlacements;
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    return gVale_ActorPlacementsAfterKalay;
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0)
    return gVale_ActorPlacementsAfterLeaving;
  return gVale_ActorPlacements;
}

/// Catch on the Nut in Vale: the party gets it.
static void Vale_CatchNut(void) {
  Event_Start();
  Import_ActorSetPosition(ACTOR_NUT, 0, 0);
  GameFlag_Set(FLAG_VALE_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  Event_End();
}

/// The same Nut after Kalay, where it is ACTOR_NUT_AFTER_KALAY.
static void Vale_CatchNutAfterKalay(void) {
  Event_Start();
  Import_ActorSetPosition(ACTOR_NUT_AFTER_KALAY, 0, 0);
  GameFlag_Set(FLAG_VALE_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  Event_End();
}

/// Vale's triggers for the story so far: before leaving, after leaving, or
/// back from Kalay.
const struct Trigger *Vale_GetTriggers(void) {
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    return gVale_TriggersAfterKalay;
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0)
    return gVale_TriggersAfterLeaving;
  return gVale_Triggers;
}

/// A little girl in Vale (ACTOR_GIRL_JEALOUS_OF_GARET) asks about the mayor
/// before the party leaves, and speaks of the tremors after.
static void Vale_TalkAboutTheMayorAndTremors(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    Event_SetMessageId(MsgValeTheGroundStillShakes);
    Event_ShowMessageAndWait(ACTOR_GIRL_JEALOUS_OF_GARET, 0);
  } else {
    Event_SetMessageId(MsgValeYourGrandpaIsTheMayor);
    Actor_TurnToFaceEachOther(ACTOR_GIRL_JEALOUS_OF_GARET, ACTOR_PARTY_LEADER,
                              4);
    Event_AskYesNoAndShowResponse(ACTOR_GIRL_JEALOUS_OF_GARET, 0);
  }
  Event_End();
}

/// A woman in Vale (ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN) asks whether Isaac
/// is going to Kraden's cottage and warns him not to go beyond it; after the
/// party has left, says Kraden has not come back.
static void Vale_WarnAboutGoingBeyondKradensCottage(void) {
  s32 flag = FLAG_VALE_ASKED_IF_GOING_TO_KRADEN;
  Event_Start();
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    Event_SetMessageId(MsgValeKradenHasntComeBack);
    Event_ShowMessageAndWait(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  } else if (GameFlag_Get(flag) == 0) {
    GameFlag_Set(flag);
    Event_SetMessageId(MsgValeDoYouNeedToGo);
    Actor_TurnToFaceEachOther(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN,
                              ACTOR_PARTY_LEADER, 4);
    Event_AskYesNoAndShowResponse(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  } else {
    Event_SetMessageId(MsgValeDontGoBeyondKradensCottage);
    Actor_TurnToFaceEachOther(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN,
                              ACTOR_PARTY_LEADER, 4);
    Event_ShowMessageAndWait(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  }
  Event_End();
}

/// Garet's sister scolds him in Vale; afterwards she only threatens him.
/// "Garet, you make me so mad sometimes!"
static void Vale_ArgueWithGaretsSister(void) {
  Event_Start();
  if (Event_GetFlag(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS) == 0) {
    GameFlag_Set(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS);
    Event_SetMessageId(MsgValeYouMakeMeSoMad);
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_SISTER, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 6);
    Actor_FaceDirection(ACTOR_GARETS_SISTER, FACING_WEST, 30);
    Actor_Jump(ACTOR_GARETS_SISTER, 2, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 6);
    Actor_TurnToFaceEachOther(ACTOR_GARETS_SISTER, ACTOR_PARTY_LEADER, 10);
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    FieldEvent_ShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  } else {
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_SetMessageId(MsgValeIllGetYouForMy);
    FieldEvent_ShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 20);
  }
  Event_End();
}

/// Talking to Dora in Vale before the party leaves: right after the roof is
/// mended (FLAG_VALE_ROOF_JUST_MENDED) she frets about the hole clumsy Garet
/// left; otherwise, since it will not rain for a while, Isaac can fix it later.
static void Vale_DoraFretsAboutTheRoof(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VALE_ROOF_JUST_MENDED) != 0) {
    Event_SetMessageId(MsgValeIToldGaretItWas);
  } else {
    Event_SetMessageId(MsgValeItWontRainForSome);
  }
  Event_ShowMessageAndWait(ACTOR_DORA, 0);
  Event_End();
}

/// Back from Kalay, a young man in Vale (ACTOR_YOUNG_MAN_ASKING_FOR_JENNA)
/// asks whether Jenna is back.
static void Vale_AskWhetherJennaHasReturned(void) {
  Event_Start();
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER,
                            ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, 20);
  Event_SetMessageId(MsgValeIsJennaBackYet);
  Event_ShowMessageAndWait(ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, 0);
  GameFlag_Set(FLAG_VALE_ASKED_IF_JENNA_IS_BACK);
  Event_End();
}

/// Back from Kalay, a woman in Vale (ACTOR_WOMAN_FEELING_TREMORS) feels
/// another tremor, and the view shakes.
static void Vale_ReactToAnotherTremor(void) {
  Event_Start();
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  wait(10);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_SetMessageId(MsgValeIFeltAnotherOne);
  FieldEvent_ShowMessageThenWait(ACTOR_WOMAN_FEELING_TREMORS, 0, 20);
  Actor_FaceActor(ACTOR_WOMAN_FEELING_TREMORS, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(ACTOR_WOMAN_FEELING_TREMORS, 0);
  Event_End();
}

/// A burly grey-haired man asks about the lack of travellers since the
/// eruption. He (ACTOR_MAN_MISSING_TRAVELERS) is in Vale back from Kalay, and
/// the talk sets area flag FLAG_VALE_ASKED_ABOUT_TRAVELERS.
static void Vale_AskAboutTheLackOfTravelers(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_MAN_MISSING_TRAVELERS, 2);
  Event_Wait(20);
  Actor_SetAngleToward(ACTOR_MAN_MISSING_TRAVELERS, ACTOR_PARTY_LEADER, 20);
  Event_SetMessageId(MsgValeNoTravelersSinceTheEruption);
  Event_AskYesNoAndShowResponse(ACTOR_MAN_MISSING_TRAVELERS, 0);
  GameFlag_Set(FLAG_VALE_ASKED_ABOUT_TRAVELERS);
  Event_End();
}

/// Walking onto the bridge in Vale (cell behavior 15): its cells'
/// cell behaviors change, remembered in FLAG_VALE_ON_BRIDGE.
static void Vale_StepOntoBridge(void) {
  GameFlag_Set(FLAG_VALE_ON_BRIDGE);
  MapGrid_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

/// Walking off the bridge in Vale (cell behavior 14) restores its
/// cells' cell behaviors.
static void Vale_StepOffBridge(void) {
  GameFlag_Clear(FLAG_VALE_ON_BRIDGE);
  MapGrid_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

/// The door of Vale Sanctum (cell behavior 1): it opens, the leader walks
/// in, and VALE_TO_SANCTUM leads into the Sanctum.
static void Vale_EnterSanctum(void) {
  Event_PlaySound(SE_THUMP);
  MapGrid_AnimateCells(gVale_SanctumDoorOpens, 45, 11);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 257, 420);
  Import_EventSetExitRequest(VALE_TO_SANCTUM);
}

/// The path to Mt. Aleph in Vale (cell behavior 2): Jenna stops Isaac until
/// Kraden has planned the visit (FLAG_SANCTUM_VISIT_PLANNED); then
/// VALE_TO_SOL_ENTRANCE leads up to Sol Entrance. Jenna says "Kraden is waiting
/// for us".
static void Vale_ApproachMtAleph(void) {
  if (GameFlag_Get(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
    Vale_HaveJennaStopTheAlephDetour();
  } else {
    Sound_Play(SE_MAP_EXIT);
    Import_EventSetExitRequest(VALE_TO_SOL_ENTRANCE);
  }
}

/// Cell behavior 3 in Vale takes the northern way out to
/// SceneId_ValeKradensCottage.
static void Vale_ExitToValeKradensCottageNorth(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(VALE_TO_VALE_KRADENS_COTTAGE_NORTH);
}

/// Cell behavior 4 in Vale takes the southern way out to
/// SceneId_ValeKradensCottage.
static void Vale_ExitToValeKradensCottageSouth(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(VALE_TO_VALE_KRADENS_COTTAGE_SOUTH);
}

/// Cell behavior 5 in Vale takes the way out to SceneId_ValePlaza.
static void Vale_ExitToValePlaza(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(VALE_TO_VALE_PLAZA);
}

/// The door of the house of Garet's family in Vale (cell behavior 6): it
/// opens and VALE_TO_COTTAGE_ROOMS_A_EAST leads in.
static void Vale_EnterGaretsHouse(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_IsaacsDoorOpens, 54, 32);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 406, 727);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_EAST);
}

/// A house door in Vale (cell behavior 7): it opens and
/// VALE_TO_COTTAGE_ROOMS_A_SOUTH leads in, to the room CottageRoomsA
/// enters from COTTAGE_ROOMS_A_FROM_VALE_SOUTH.
static void Vale_EnterCottageRoomsA(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 45, 39);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 262, 805);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_SOUTH);
}

/// The door of Isaac's house in Vale (cell behavior 8): after the party has
/// left and before Kalay, Dora will not open. "My Isaac would keep his
/// promise... Go away." Otherwise VALE_TO_COTTAGE_ROOMS_B leads in.
static void Vale_EnterIsaacsHouse(void) {
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0 &&
      GameFlag_Get(FLAG_KALAY_REACHED) == 0) {
    Event_Start();
    Event_SetMessageId(MsgValeYouCantBeIsaac);
    FieldMessage_ShowAtActor(ACTOR_DORA, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_ShowMessageThenWait(ACTOR_DORA, 0, 60);
      Event_ShowMessageAndWait(ACTOR_DORA, 0);
    } else {
      u16 *message = &gFieldWork->message;

      *message = *message + 2;
      Event_Wait(40);
      Event_ShowMessageAndWait(ACTOR_DORA, 0);
    }
    Event_End();
  } else {
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gVale_IsaacsDoorOpens, 50, 44);
    Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 340, 888);
    Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_B);
  }
}

/// The door of the house of Jenna's grandparents in Vale (cell behavior 9):
/// it opens and VALE_TO_COTTAGE_ROOMS_A_NORTH leads in.
static void Vale_EnterJennasGrandparentsHouse(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 49, 69);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 326, 1126);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_NORTH);
}

/// A house door in Vale (cell behavior 10): it opens and
/// VALE_TO_COTTAGE_ROOMS_A_WEST leads in, to the room CottageRoomsA
/// enters from COTTAGE_ROOMS_A_FROM_VALE_WEST.
static void Vale_EnterCottageRoomsAWest(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_WestHouseDoorOpens, 52, 76);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 1238);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_WEST);
}

/// Unused: its row answers cell behavior 11, which no cell of Vale's layout
/// (map/map_000, the stormy night's too) carries, as
/// ValeStormyNight_UnusedLeaveToCottageRoomsAWestBySecondDoor in
/// vale_stormy_night.c. It would have opened Isaac's door, walked the leader to
/// the spot before it and taken VALE_TO_COTTAGE_ROOMS_A_NORTH_SECOND_DOOR into
/// the house of Jenna's grandparents, whose way out returns by the other door.
static void Vale_UnusedEnterJennasGrandparentsHouseBySecondDoor(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_IsaacsDoorOpens, 35, 74);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_NORTH_SECOND_DOOR);
}

/// On the path to Mt. Aleph in Vale, before the visit is planned: Jenna comes
/// out to remind Isaac that Kraden is waiting, and he turns back.
static void Vale_HaveJennaStopTheAlephDetour(void) {
  struct Actor *leader;
  struct Actor *jenna;
  struct Actor *actor;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  jenna = Actor_GetById(ACTOR_JENNA);
  Event_Start();
  jenna->x.fixed = leader->x.fixed;
  jenna->y.fixed = leader->y.fixed;
  jenna->z.fixed = leader->z.fixed;
  jenna->targetX = ACTOR_NO_TARGET;
  jenna->targetY = ACTOR_NO_TARGET;
  jenna->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(jenna, 0, 0, 0);
  jenna->groundHeight = leader->y.fixed;
  wait(1);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_JENNA, 110, 283);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 2);
  Event_SetMessageId(MsgValeKradenIsWaitingForUs);
  if (leader->x.fixed < jenna->x.fixed) {
    FieldEvent_ShowMessageThenWait(
        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 2);
  } else {
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 2);
  }
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(2);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_JENNA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 110, 303);
  Event_End();
}

/// The puppies in Vale (cell behavior 20), until the party has met Saturos
/// and Menardi: they bark, the camera pans down to them and back, and the
/// leader turns away.
static void Vale_WatchThePuppies(void) {
  struct MapGridScrollWork *grid;
  s32 *savedOrigin;
  struct Actor *leader;
  s32 frame;
  s32 cameraPosition[3];

  if (GameFlag_Get(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    grid = gMapGridWork[0];
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_Wait(2);
    Event_SetMessageId(MsgValeRrruffRrrruff);
    Event_ShowMessageThenWait(ACTOR_FIRST_PUPPY, 0, 2);
    Event_ShowMessageThenWait(ACTOR_FIRST_PUPPY + 1, 0, 2);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = grid->origin;
    grid->origin = cameraPosition;
    frame = 0;
    // Pan down to the puppies while the leader stays in place.
    do {
      cameraPosition[2] += PIXELS(2);
      Event_Wait(1);
      frame++;
      MapGrid_CenterOnFollowedPoint();
    } while (frame != 40);
    Event_Wait(60);
    FieldMessage_Show(MsgValePuppiesPlayingOver, 1);
    frame = 0;
    Event_Wait(6);
    do {
      cameraPosition[2] -= PIXELS(2);
      Event_Wait(1);
      frame++;
      MapGrid_CenterOnFollowedPoint();
    } while (frame != 40);
    // Return camera ownership to the normal follow target.
    grid->origin = savedOrigin;
    Event_Wait(60);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 70, 741);
    Event_End();
  }
}

/// The puppies in Vale from the other side (cell behavior 21): the same
/// barking, without the camera pan.
static void Vale_TurnBackFromThePuppies(void) {
  s32 msg;

  if (GameFlag_Get(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    msg = MsgValeRrruffRrrruff;
    Event_SetMessageId(msg);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_PUPPY, 0, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_PUPPY + 1, 0, 2);
    FieldMessage_Show(msg + 2, 1);
    Event_Wait(6);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 870);
    Event_End();
  }
}

/// Cell behavior 100 in Vale, at the foot of the ladder beside Isaac's
/// house: walking north into it climbs the leader up onto the roof, whose cells
/// change so it can be walked (FLAG_VALE_ON_ROOF set while there).
static void Vale_ClimbLadderOntoRoof(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Vale_MoveActorAlongLowerZPath(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB,
                                ANIM_LEADER_CLIMB_OVER_TOP, 1);
  GameFlag_Set(FLAG_VALE_ON_ROOF);
  Event_End();
}

/// Cell behavior 101 in Vale, at the top of the ladder beside Isaac's
/// house: walking south into it climbs the leader back down, and the roof's
/// cells are put back (FLAG_VALE_ON_ROOF cleared).
static void Vale_ClimbDownFromRoof(void) {
  Event_Start();
  Vale_MoveActorAlongHigherZPath(ACTOR_PARTY_LEADER,
                                 ANIM_LEADER_CLIMB_DOWN_FROM_TOP,
                                 ANIM_LEADER_CLIMB, 1);
  GameFlag_Clear(FLAG_VALE_ON_ROOF);
  MapGrid_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  Event_End();
}

/// A bearded man in a long robe in Vale (ACTOR_MAN_GUARDING_MT_ALEPH,
/// cell behavior 50) jumps out in front of Isaac, then keeps watching him;
/// sets FLAG_VALE_WARNED_AGAINST_MT_ALEPH. "You're not planning on sneaking up
/// to Mt. Aleph...?"
static void Vale_WarnAgainstSneakingUpMtAleph(void) {
  struct Actor *guard;
  struct Actor *actor;

  guard = Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Actor_Jump(ACTOR_PARTY_LEADER, 5, 0);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 215, 403);
  guard->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetPosition(ACTOR_MAN_GUARDING_MT_ALEPH, PIXELS(166), PIXELS(375));
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTHEAST, 20);
  guard->steeringFlags = (guard->steeringFlags ^ 1);
  Actor_SetSpeed(ACTOR_MAN_GUARDING_MT_ALEPH, PIXELS(5) / 2, PIXELS(5) / 2);
  Actor_Jump(ACTOR_MAN_GUARDING_MT_ALEPH, 4, 0);
  Actor_WalkToAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, 202, 395);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH,
                      FACING_SOUTHEAST + FACING_STEP, 24);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_MAN_GUARDING_MT_ALEPH, PIXELS(3) / 2, PIXELS(1));
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_FirstWarningLeaderPath);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_SetScript(ACTOR_MAN_GUARDING_MT_ALEPH, gVale_FirstWarningGuardPath);
  Actor_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 474);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_RefreshSelectorById(ACTOR_MAN_GUARDING_MT_ALEPH);
  Actor_WalkToAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, 256, 456);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTH, 20);
  Actor_StartRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgValeNotSneakingUpMtAleph);
  Event_AskYesNo(ACTOR_MAN_GUARDING_MT_ALEPH, 0);
  actor = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
#ifdef UBFIX
  actor->update = Vale_HaveGuardWatchTheLeader;
#else
  actor->update = (void (*)(struct Actor *))Vale_HaveGuardWatchTheLeader;
#endif
  Actor_SetScript(ACTOR_MAN_GUARDING_MT_ALEPH, gVale_AlephGuardPatrol);
  GameFlag_Set(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
  Event_End();
}

/// Repeats the warning when the man reaches the first pause.
static void Vale_RepeatMtAlephWarningFromFirstApproach(void) {
  struct Actor *guard = Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  if (GameFlag_Get(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (guard->timer == 1) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderFromWestPath,
                                   gVale_GuardFromFirstPausePath);
    }
  }
}

/// Asks about the Aleph detour, then resumes each actor's response
/// actions.
static void Vale_AskAboutSneakingToAleph(const s32 *leaderActions,
                                         const s32 *guardActions) {
  const s32 *replyActions;
  struct Actor *guard;
  struct Actor *actor;

  replyActions = guardActions;
  guard = Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  Event_Start();
  Actor_StartRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetScript(ACTOR_PARTY_LEADER, leaderActions);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_RunScriptAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, replyActions);
  Actor_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  guard->scaleX = Q16_ONE;
  guard->scaleY = Q16_ONE;
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Event_SetMessageId(MsgValeNotSneakingUpMtAleph);
  Event_AskYesNo(ACTOR_MAN_GUARDING_MT_ALEPH, 0);
  actor = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
#ifdef UBFIX
  actor->update = Vale_HaveGuardWatchTheLeader;
#else
  actor->update = (void (*)(struct Actor *))Vale_HaveGuardWatchTheLeader;
#endif
  Actor_SetScript(ACTOR_MAN_GUARDING_MT_ALEPH, gVale_AlephGuardPatrol);
  Event_End();
}

/// Repeats the warning when the man reaches the second pause.
static void Vale_RepeatMtAlephWarningFromSecondApproach(void) {
  struct Actor *guard = Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  if (GameFlag_Get(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (guard->timer == 2) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderPushedBackPath,
                                   gVale_GuardFromSecondPausePath);
    }
  }
}

/// Chooses the warning approach matching the man's current pause.
static void Vale_RepeatMtAlephWarningAtEitherPause(void) {
  struct Actor *guard = Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  if (GameFlag_Get(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
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

/// Unused: another cut of Vale_RoofMended, the end of the morning the
/// story begins.
///
/// No table names it. It plays the same lines as the event the game uses, `Good
/// job, Garet.` and `I was devastated when Kyle died...` among them, but frames
/// them with its own camera and map grid cells, and sets
/// FLAG_VALE_ROOF_JUST_MENDED as that event does.
UNUSED static void Vale_UnusedRoofMended(void) {
  struct Actor *leader;
  struct Actor *actor;
  u8 leaderMotionFlags;
  u8 *leaderMotion;
  s32 i;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  actor = FieldCamera_GetActor();
  actor->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
  Event_Wait(1);
  MapGrid_CenterOnFollowedPoint();
  MapGrid_CopyCellAttributes(49, 41, 7, 3, 20, 50);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leaderMotion = &leader->motionFlags;
  leaderMotionFlags = *leaderMotion;
  *leaderMotion = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(407), PIXELS(690));
  Actor_SetPosition(ACTOR_DORA, PIXELS(392), PIXELS(896));
  Actor_SetPosition(ACTOR_GARET, PIXELS(298), PIXELS(736));
  Actor_SetPosition(ACTOR_JENNA, PIXELS(298), PIXELS(760));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Vale_PlaceOrHideBoards(ACTOR_FIRST_BOARD, 2, 1);
  gFieldWork->transition = 0;
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetScript(ACTOR_JENNA, gVale_JennaWalksToTheHouse);
  Actor_SetScript(ACTOR_GARET, gVale_GaretWalksToTheHouse);

  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 404, 843);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_RBG);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 402, 828);
  Event_Wait(80);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_FIRST_BOARD, PIXELS(390), PIXELS(842));
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(200);
  MapGrid_CopyCellsTo(7, 102, 84, 41, 2, 1);

  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_RBG);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 377, 828);
  Event_Wait(80);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_FIRST_BOARD, 0, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(200);
  MapGrid_CopyCellsTo(6, 102, 83, 41, 1, 1);

  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
  Actor_SetSpriteColorById(ACTOR_SECOND_BOARD, SPRITE_COLOR_RBG);
  Actor_SetSpeed(ACTOR_SECOND_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 390, 832);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 360, 842);
  Event_Wait(80);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_SECOND_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_SECOND_BOARD, 0, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(200);
  MapGrid_CopyCellsTo(5, 103, 82, 42, 1, 1);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;

  Event_SetMessageId(MsgValeGoodJob);
  Actor_Jump(ACTOR_DORA, 2, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 20);
  Vale_MoveActorAlongLowerZPath(ACTOR_DORA, 5, 6, 0);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    gFieldWork->message++;
  }
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Event_SetMessageId(MsgValeWorkingYourselvesBone);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
  }
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgValeDevastatedWhenKyle);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);

  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_GARET, 369, 904);
  Actor_WalkToAndWait(ACTOR_JENNA, 392, 904);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Vale_MoveActorAlongLowerZPath(ACTOR_JENNA, ANIM_LEADER_CLIMB,
                                ANIM_LEADER_CLIMB_OVER_TOP, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 30);
  Actor_Jump(ACTOR_JENNA, 4, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 392, 843);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Vale_MoveActorAlongLowerZPath(ACTOR_GARET, ANIM_LEADER_CLIMB,
                                ANIM_LEADER_CLIMB_OVER_TOP, 0);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkTo(ACTOR_GARET, 392, 843);
  Actor_Get(ACTOR_JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JENNA, 408, 843);
  Event_Wait(1);
  Actor_Get(ACTOR_JENNA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_Jump(ACTOR_DORA, 4, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(100);
  Actor_TurnToFaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 40);
  Actor_TurnToFaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
  }
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);

  Event_SetMessageId(MsgValeAsStubbornAsYourFather);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_Jump(ACTOR_DORA, 4, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_DORA_HANDS_UP);
  Event_Wait(5);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, ACTOR_GARET, 10,
                                    14, 4, 14, 0);
  actor = Actor_Get(ACTOR_DORA);
  actor->sprite->flags = 0;
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
  Event_Wait(4);
  for (i = 0; i != 4; i++) {
    actor->z.fixed += PIXELS(3) / 2;
    actor->scaleY -= 0x1999;
    Event_Wait(1);
  }
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3), PIXELS(3) / 2);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 374, 827);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
  Actor_Jump(ACTOR_GARET, 2, 5);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_DORA_PEEK);
  actor->scaleY = 0x8000;
  Actor_SetPosition(ACTOR_DORA, PIXELS(364), PIXELS(811));
  for (i = 0; i != 5; i++) {
    actor->scaleY += 0x1999;
    Event_Wait(1);
  }
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  FieldCamera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Actor_Jump(ACTOR_DORA, 6, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(30);
  actor = Actor_Get(ACTOR_DORA);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 1);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4), PIXELS(2));
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_GARET, 403, 827);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 1);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
  Actor_Jump(ACTOR_GARET, 2, 5);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_JENNA, 408, 855);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 70);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 398, 828);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(60);

  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_JENNA);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(ACTOR_JENNA, actor->x.part.pixel + 16, actor->z.part.pixel);
  Actor_WalkToAndWait(ACTOR_GARET, actor->x.part.pixel + 16,
                      actor->z.part.pixel - 16);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 30);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_JENNA, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WalkToAndWait(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_PrepareTwoActorsAndApply(GARET, JENNA);
  FieldCamera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
  Vale_MoveActorAlongHigherZPath(ACTOR_PARTY_LEADER,
                                 ANIM_LEADER_CLIMB_DOWN_FROM_TOP,
                                 ANIM_LEADER_CLIMB, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_Get(ACTOR_DORA)->steeringFlags |= ACTOR_FACES_MOTION;
  Vale_MoveActorAlongHigherZPath(ACTOR_DORA, 6, 5, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(100);
  GameFlag_Set(FLAG_VALE_ROOF_JUST_MENDED);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  *leaderMotion = leaderMotionFlags;
  Event_End();
}

/// Vale's setup: plays the roof mending (VALE_FROM_TITLE) or its end (
/// entrance 17, once), floats the Nut, places the pillar by its column, sets
/// the watching man going, removes the puppies once Saturos and Menardi have
/// been met, and after a battle restores the roof's and the bridge's
/// cells.
s32 Vale_Initialize(void) {
  struct Actor *actor;
  s32 set;

  if (gAdventure.entrance == VALE_FROM_TITLE) {
    WindowColor_Apply(((u8 *)&gAdventure)[517], ((u8 *)&gAdventure)[518]);
    Vale_MendingTheRoof();
  } else {
    if (!GameFlag_Get(FLAG_VALE_NUT_CAUGHT)) {
      if (!GameFlag_Get(FLAG_KALAY_REACHED)) {
        Scene_FloatNut(ACTOR_NUT);
      } else {
        Scene_FloatNut(ACTOR_NUT_AFTER_KALAY);
      }
    }
    MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
    MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
    actor =
        Actor_GetById((GameFlag_Get(FLAG_KALAY_REACHED) != 0) + ACTOR_PILLAR);
    Actor_SetSpriteFlags(actor, 0);
    if (GameFlag_Get(FLAG_VALE_PILLAR_IN_WEST_COLUMN)) {
      actor->x.fixed = 181 << 17;
    } else if (GameFlag_Get(FLAG_VALE_PILLAR_IN_EAST_COLUMN)) {
      actor->x.fixed = 197 << 17;
    } else {
      actor->x.fixed = 189 << 17;
    }
    actor->z.fixed = PIXELS(584);
    actor->y.fixed = PIXELS(192);
    Vale_MovePillarBelowCliff();
    actor->layer = 3;
    actor->motionFlags = 0;
    Task_AddFrame(Vale_AdjustPillarPriorityForLeaderHeight, TASK_PASS_FRAME);
#if TBS_PATCH_4
    if (GameFlag_Get(FLAG_KALAY_REACHED)) {
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        if (GameFlag_Get(FLAG_VALE_ON_ROOF)) {
          MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (GameFlag_Get(FLAG_VALE_ON_BRIDGE)) {
          Vale_StepOntoBridge();
        }
      }
    } else {
#else
    if (!GameFlag_Get(FLAG_KALAY_REACHED)) {
#endif
      if (Event_GetFlag(FLAG_VALE_PARTY_LEFT)) {
        actor = Actor_GetById(ACTOR_DORA);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_DORA), 0);
        actor->scaleX = 0x28f;
        actor->scaleY = 0x28f;
      }
      if (GameFlag_Get(FLAG_MET_SATUROS_AND_MENARDI)) {
        Import_ActorSetPosition(ACTOR_FIRST_PUPPY, 0, 0);
        Import_ActorSetPosition(ACTOR_FIRST_PUPPY + 1, 0, 0);
        Import_ActorSetPosition(ACTOR_FIRST_PUPPY + 2, 0, 0);
      }
      set = GameFlag_Get(FLAG_VALE_PARTY_LEFT);
      if (set == 0) {
        if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
          if (GameFlag_Get(FLAG_VALE_WARNED_AGAINST_MT_ALEPH)) {
            Actor_SetPosition(ACTOR_MAN_GUARDING_MT_ALEPH, PIXELS(256),
                              PIXELS(456));
#ifdef UBFIX
            Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
                Vale_HaveGuardWatchTheLeader;
#else
            Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
                (void *)Vale_HaveGuardWatchTheLeader;
#endif
            Actor_SetScript(ACTOR_MAN_GUARDING_MT_ALEPH,
                            gVale_AlephGuardPatrol);
          }
        } else {
          Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = set;
          GameFlag_Clear(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
        }
        if (gAdventure.entrance != VALE_FROM_TITLE &&
            !GameFlag_Get(FLAG_KALAY_REACHED)) {
          Task_AddFrame(Vale_SynchronizeAlephGuardPause, TASK_PASS_FRAME);
        }
      }
      if (!Event_GetFlag(FLAG_VALE_ROOF_MENDED_EVENT_PLAYED) &&
          gAdventure.entrance == VALE_FROM_ROOF_MENDED) {
        Vale_RoofMended();
        GameFlag_Set(FLAG_VALE_ROOF_MENDED_EVENT_PLAYED);
      }
#if TBS_PATCH_4
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        if (GameFlag_Get(FLAG_VALE_ON_ROOF)) {
          MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (GameFlag_Get(FLAG_VALE_ON_BRIDGE)) {
          Vale_StepOntoBridge();
        }
      }
      {
        s32 priority;
        s32 actorId;

        priority = ((u32)(u8)set >> 8) + 2;
        actorId = ACTOR_DORA;
        Actor_SetSpritePriority(actorId, priority);
      }
#endif
    }
#if !TBS_PATCH_4
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if (GameFlag_Get(FLAG_VALE_ON_ROOF)) {
        MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
      }
      if (GameFlag_Get(FLAG_VALE_ON_BRIDGE)) {
        Vale_StepOntoBridge();
      }
    }
#endif
    Scene_QueueMusic(SE_AMBIENT_WATER);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  return 0;
}

/// Back from Kalay, Garet meets his sister in Vale; sets area flag
/// FLAG_VALE_GARET_MET_SISTER. "Ah! It's been a while... You're back?"
/// ("Sis...").
static void Vale_GaretMeetsHisSister(void) {
  struct Actor *record;
  s32 msg;

  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_FaceActor(ACTOR_GARETS_SISTER_AFTER_KALAY, ACTOR_PARTY_LEADER, 20);
  msg = MsgValeAh;
  Event_SetMessageId(msg);
  Actor_StartRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(398), -1, PIXELS(582), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 420, 608);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY,
                      FACING_SOUTHEAST + FACING_STEP, 0);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Actor_SetPosition(ACTOR_GARET, record->x.fixed, record->z.fixed);
  }
  Actor_WalkToAndWait(ACTOR_GARET, 402, 608);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY,
                      FACING_SOUTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 2);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
    Actor_StartRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 1);
  }
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 40);
  Actor_ShowEmote(ACTOR_GARETS_SISTER_AFTER_KALAY,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_SetMessageId(msg + 6);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 1);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY,
                      FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 120);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY,
                      FACING_SOUTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Actor_SetDestination(ACTOR_GARET, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  GameFlag_Set(FLAG_VALE_GARET_MET_SISTER);
  Event_End();
}

/// Back from Kalay, Garet asks one of his family in Vale
/// (ACTOR_GARETS_BROTHER_AFTER_KALAY) to protect it while he is gone; sets
/// area flag FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY. "WHATEVER YOU DO,
/// you must protect the family while I'm gone."
static void Vale_GaretAsksToProtectFamily(void) {
  struct Actor *record;

  Event_Start();
  FieldCamera_MoveTo(PIXELS(357), -1, PIXELS(738), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 367, 745);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Actor_SetPosition(ACTOR_GARET, record->x.fixed, record->z.fixed);
  }
  Actor_WalkToAndWait(ACTOR_GARET, 346, 745);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 20);
  Event_SetMessageId(MsgValeHowHaveYouBeen);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_RunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 2);
  Actor_ShowEmote(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                      FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                      FACING_SOUTH + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                      FACING_SOUTHEAST + FACING_STEP, 40);
  FieldEvent_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 1);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                      FACING_SOUTH + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST + FACING_STEP, 40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST + FACING_STEP, 20);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  } else {
    gFieldWork->message++;
  }
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 10);
  Event_SetMessageId(MsgValeEverProtectFamily);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Actor_SetDestination(ACTOR_GARET, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  GameFlag_Set(FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY);
  Event_End();
}

/// Unused: water bursts up by the river after a tremor. Back from Kalay, a
/// tremor shakes Vale and water bursts up three times north of the strongman by
/// the river who remembers Felix.
///
/// No table names it. He would have started, sniffled through `I still remember
/// when Felix and I used to play here...*Sniff*`, turned to the leader to say
/// that Felix must be protecting Jenna, and set area flag
/// FLAG_VALE_UNUSED_WATER_BURST_SEEN. His talk and Mind Read rows show the same
/// two lines.
UNUSED static void Vale_UnusedWaterBurstsByTheRiver(void) {
  struct Actor *actor;
  const s32 *actions;

  Event_Start();
  actor = Actor_Get(ACTOR_FIRST_WATER);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_FIRST_WATER + 1);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_FIRST_WATER + 2);
  Actor_SetSpriteFlags(actor, 0);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER, 0);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER + 1, 0);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER + 2, 0);
  wait(20);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  actions = gVale_UnusedWaterGrows;
  Actor_SetScript(ACTOR_FIRST_WATER, actions);
  wait(10);
  Actor_SetScript(ACTOR_FIRST_WATER + 1, actions);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  wait(20);
  Actor_RunScriptAndWait(ACTOR_FIRST_WATER + 2, actions);
  Actor_ShowEmote(ACTOR_STRONGMAN_REMEMBERING_FELIX,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_RunRepeatedMotion(ACTOR_STRONGMAN_REMEMBERING_FELIX, 2);
  Actor_FaceDirection(ACTOR_STRONGMAN_REMEMBERING_FELIX,
                      FACING_NORTH + FACING_STEP, 10);
  Event_SetMessageId(MsgValeIUsedToPlayHere);
  FieldEvent_ShowMessageThenWait(ACTOR_STRONGMAN_REMEMBERING_FELIX, 0, 40);
  Actor_FaceActor(ACTOR_STRONGMAN_REMEMBERING_FELIX, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(ACTOR_STRONGMAN_REMEMBERING_FELIX, 0);
  Actor_FaceDirection(ACTOR_STRONGMAN_REMEMBERING_FELIX, FACING_WEST, 10);
  GameFlag_Set(FLAG_VALE_UNUSED_WATER_BURST_SEEN);
  Event_End();
}

/// Walks an actor to the foot of the ladder beside Isaac's house and
/// climbs it onto the roof, in `approachAnimationId` on the rungs and
/// `departureAnimationId` over the eaves.
static void Vale_MoveActorAlongLowerZPath(s32 actorId, s32 approachAnimationId,
                                          s32 departureAnimationId,
                                          s32 resumeMotion) {
  s32 departureAnimation;
  s32 approachAnimation;
  s32 restoreMotion;
  struct Actor *actor;
  struct Sprite *sprite;

  restoreMotion = resumeMotion;
  approachAnimation = approachAnimationId;
  departureAnimation = departureAnimationId;
  actor = Actor_GetById(actorId);
  sprite = actor->sprite;
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(actorId, 392, 886);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  actor->motionFlags = 0;
  sprite->flags = 0;
  Import_ActorSetAnimation(actorId, approachAnimation);
  Actor_SetSpeed(actorId, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(actorId, 392, 875);
  Event_Wait(10);
  Import_ActorSetAnimation(actorId, departureAnimation);
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_MoveToAndWait(actorId, 392, 859);
  sprite->flags = 1;
  if (restoreMotion != 0) {
    actor->motionFlags = 3;
  }
  Event_Wait(10);
  Import_ActorSetAnimation(actorId, ANIM_STAND);
}

/// Climbs an actor down the ladder beside Isaac's house from the roof,
/// and restores falling motion if requested.
static void Vale_MoveActorAlongHigherZPath(s32 actorId, s32 approachAnimationId,
                                           s32 departureAnimationId,
                                           s32 resumeMotion) {
  s32 departureAnimation;
  s32 approachAnimation;
  s32 restoreMotion;
  struct Actor *actor;
  struct Sprite *sprite;

  restoreMotion = resumeMotion;
  approachAnimation = approachAnimationId;
  departureAnimation = departureAnimationId;
  actor = Actor_GetById(actorId);
  sprite = actor->sprite;
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(actorId, 392, 859);
  Actor_FaceDirection(actorId, FACING_NORTH, 10);
  actor->motionFlags = 0;
  sprite->flags = 0;
  Import_ActorSetAnimation(actorId, approachAnimation);
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_MoveToAndWait(actorId, 392, 875);
  Event_Wait(10);
  Actor_SetSpeed(actorId, PIXELS(3) / 10, PIXELS(3) / 20);
  Import_ActorSetAnimation(actorId, departureAnimation);
  Actor_MoveToAndWait(actorId, 392, 890);
  actor->velocityY = 0x20000;
  sprite->flags = 1;
  if (restoreMotion != 0) {
    actor->motionFlags = 3;
  }
  Import_ActorSetAnimation(actorId, ANIM_STAND);
}

/// Mode 0 sets count boards from first on the roof by the hole, still and
/// without shadows; any other mode puts them out of sight.
static void Vale_PlaceOrHideBoards(s32 first, u32 count, s32 mode) {
  s32 selector = first;
  u32 i;

  if (mode == 0) {
    for (i = 0; i < count; i++) {
      struct Actor *actor = Actor_GetById(selector);

      actor->motionFlags = 0;
      Actor_SetSpriteFlags(actor, 0);
      actor->x.fixed = PIXELS(390);
      actor->y.fixed = PIXELS(160);
      actor->z.fixed = PIXELS(842);
      selector++;
    }
    return;
  }

  for (i = 0; i < count; i++) {
    Import_ActorSetPosition(selector, 0, 0);
    selector++;
  }
}

/// Arriving by VALE_FROM_TITLE, the game's opening in Vale: Isaac is mending
/// the roof of his house, Garet trains his Psynergy, and Jenna comes by; the
/// two speak of the storm three years ago before going to fetch Isaac. "Three
/// years later... Isaac! Haven't you finished mending the roof yet?"
static void Vale_MendingTheRoof(void) {
  u32 i;
  /* FAKEMATCH(dead): repairActor's zero initializer, never read, keeps
   * the event's register allocation; without it, 59 of 1024 instructions differ
   * in tbs-en. */
  struct Actor *repairActor = NULL;
  struct Actor *leader;
  struct Actor *moveHand;
  struct Actor *actor;
  /* FAKEMATCH(temporary): one zero for the hand's flags and update and
   * the message call; as literal zeros, 7 of 1024 instructions differ in
   * tbs-en. */
  s32 stoppedMotionFlags;
  s32 message;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  moveHand = Actor_GetById(ACTOR_OPENING_MOVE_HAND);

  // Prepare the damaged house and start Isaac's hammering behind the caption.
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  (FieldCamera_GetActor())->motionFlags = 0;
  wait(1);
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  repairActor = Actor_GetById(ACTOR_OPENING_FIRST_BOARD);
  repairActor->motionFlags = 0;
  repairActor->x.fixed = PIXELS(388);
  repairActor->y.fixed = PIXELS(160);
  repairActor->z.fixed = PIXELS(840);
  Actor_SetSpriteFlags(repairActor, 0);
  repairActor = Actor_GetById(ACTOR_OPENING_SECOND_BOARD);
  repairActor->motionFlags = 0;
  repairActor->y.fixed = PIXELS(160);
  repairActor->z.fixed = PIXELS(844);
  repairActor->x.fixed = PIXELS(388);
  Actor_SetSpriteFlags(repairActor, 0);
  repairActor = Actor_GetById(ACTOR_OPENING_THIRD_BOARD);
  repairActor->motionFlags = 0;
  repairActor->z.fixed = PIXELS(848);
  repairActor->y.fixed = PIXELS(160);
  repairActor->x.fixed = PIXELS(388);
  Actor_SetSpriteFlags(repairActor, 0);
  actor = Actor_GetById(ACTOR_OPENING_FIRST_BOARD);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_GetById(ACTOR_OPENING_SECOND_BOARD);
  Actor_SetSpriteFlags(actor, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  ActorSystem_SuspendProjectionAndShowSprites();
  message = MsgValeRepairCaption;
  FieldMessage_ShowCentered(message, 0, 0);
  ActorSystem_ResumeProjectionAndHideLayers();

  // Follow Jenna through the village until she notices the repair work.
  FieldCamera_MoveTo(PIXELS(339), PIXELS(160), PIXELS(1173), 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  FieldCamera_SetSpeed(PIXELS(33) / 100, PIXELS(33) / 800);
  FieldCamera_MoveTo(PIXELS(296), PIXELS(160), PIXELS(921), 1);
  Actor_SetPosition(ACTOR_JENNA, PIXELS(409), PIXELS(1134));
  wait(1);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_WalkTo(ACTOR_JENNA, 420, 1068);
  Scene_PlayMusic();
  gFieldWork->transitionFrames = 60;
  Event_StartScreenTransition();
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_JENNA, 341, 1064);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_JENNA, 359, 1033);
  Actor_SetSpeed(ACTOR_OPENING_WOMAN, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_OPENING_WOMAN, 318, 947);
  Import_ActorSetAnimation(ACTOR_OPENING_WOMAN, ANIM_WALK);
  Actor_WalkToAndWait(ACTOR_JENNA, 412, 1033);
  Actor_WalkToAndWait(ACTOR_JENNA, 412, 1019);
  Actor_WalkToAndWait(ACTOR_JENNA, 374, 1008);
  Actor_WalkToAndWait(ACTOR_JENNA, 347, 955);
  Actor_WalkToAndWait(ACTOR_OPENING_WOMAN, 318, 947);
  Actor_TurnToFaceEachOther(ACTOR_JENNA, ACTOR_OPENING_WOMAN, 40);
  Actor_RunRepeatedMotion(ACTOR_OPENING_WOMAN, 2);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_WalkTo(ACTOR_OPENING_WOMAN, 379, 1017);
  FieldCamera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Actor_WalkToAndWait(ACTOR_JENNA, 333, 920);
  Actor_WalkToAndWait(ACTOR_JENNA, 299, 924);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST + FACING_STEP, 30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(387), PIXELS(160), PIXELS(866), 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(ACTOR_OPENING_DORA, 2, 20);
  Event_SetMessageId(message + 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_OPENING_DORA, 0,
                                 10);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  Actor_TurnToFaceEachOther(ACTOR_OPENING_DORA, ACTOR_PARTY_LEADER, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_OPENING_DORA, 2);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_OPENING_DORA, 0,
                                 40);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  FieldCamera_FollowActor(ACTOR_JENNA, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH + FACING_STEP, 10);
  Actor_WalkToAndWait(ACTOR_JENNA, 312, 759);
  Actor_WalkToAndWait(ACTOR_JENNA, 361, 760);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 40);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_JENNA, 4, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(396), -1, PIXELS(588), 1);
  Actor_WalkToAndWait(ACTOR_JENNA, 456, 739);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);

  // Garet trains his Psynergy: the hand of Move pushes the pillar, shedding
  // sparkles.
  Scene_StartEventPsynergy();
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_CAST_PSYNERGY);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
  Event_PlaySound(SE_PSYNERGY_SPARKLE);
  for (i = 0; i < 60; i++) {
    Vale_FlashActorAndSpawnEffects(Actor_GetById(ACTOR_GARET));
    wait(1);
  }
  Actor_SetSpritePriority(ACTOR_GARET, 1);
  Task_AddFrame(Vale_UpdateGaretPaletteAndEffects, TASK_PASS_FRAME);
  Task_AddFrame(Vale_CycleOpeningPillarPalette, TASK_PASS_FRAME);
  actor = Actor_GetById(ACTOR_OPENING_MOVE_HAND);
  Actor_SetSpriteFlags(actor, 0);
  stoppedMotionFlags = 0;
  moveHand->motionFlags = stoppedMotionFlags;
  moveHand->x.fixed = PIXELS(428);
  moveHand->y.fixed = PIXELS(208);
  moveHand->z.fixed = PIXELS(584);
  moveHand->facing = FACING_WEST;
  moveHand->update = Vale_TrailSparkles;
  Event_Wait(4);
  Actor_SetSpeed(ACTOR_OPENING_MOVE_HAND, PIXELS(2), PIXELS(2));
  FieldActor_SetMoveTarget(moveHand, PIXELS(408), PIXELS(208), PIXELS(584));
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_OPENING_PILLAR, PIXELS(3) / 20, PIXELS(3) / 40);
  Actor_SetSpeed(ACTOR_OPENING_MOVE_HAND, PIXELS(3) / 20, PIXELS(3) / 40);
  Actor_GetById(ACTOR_OPENING_PILLAR);
  FieldActor_SetMoveTarget(moveHand, PIXELS(392), PIXELS(208), PIXELS(584));
  Actor_MoveToAndWait(ACTOR_OPENING_PILLAR, 378, 584);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
  moveHand->update = (void (*)(struct Actor *))stoppedMotionFlags;
  Actor_SetSpritePriority(ACTOR_GARET, 2);
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Task_Remove(Vale_UpdateGaretPaletteAndEffects);
  Task_Remove(Vale_CycleOpeningPillarPalette);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_GARET, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_OPENING_PILLAR, SPRITE_COLOR_NORMAL);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Vale_VanishMoveHand(moveHand);
  Scene_EndEventPsynergy();

  // Finish the reunion conversation after the repair effects settle.
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_JENNA, 424, 624);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_JENNA, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_JENNA, 388, 604);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_GARET, 1, 2, 25, 2, ACTOR_JENNA, 10,
                                    14, 4, 14, stoppedMotionFlags);
  Event_Wait(40);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST + FACING_STEP, 40);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_GARET, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_JENNA, 380, 620);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 30);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_GARET, 412, 604);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_Jump(ACTOR_JENNA, 4, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_JENNA, 428, 628);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_GARET,
                    0);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_WalkToAndWait(ACTOR_JENNA, 428, 628);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_JENNA, 450, 750);
  Actor_WalkTo(ACTOR_GARET, 450, 750);
  Event_Wait(60);
  gFieldWork->transitionFrames = 60;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(VALE_TO_ROOF_MENDED);
  Event_End();
}

/// Arriving by VALE_FROM_ROOF_MENDED after the opening, in Vale: Dora praises
/// the mended roof, remembers Kyle, and sends Isaac, Garet and Jenna off to Mt.
/// Aleph with Kraden. "Good job, Isaac. You patched it nicely."
static void Vale_RoofMended(void) {
  struct Actor *leader;
  struct Actor *actor;
  u32 i;
  s32 turnBack;
  s32 turnSide;
  struct Actor *turned;
  s32 none;
  s32 flag;
  TaskFunc funcA;
  TaskFunc funcB;
  TaskFunc funcC;
  TaskFunc funcD;
  TaskFunc funcE;
  TaskFunc funcF;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  MapGrid_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  MapGrid_CopyCellsTo(0, 103, 82, 42, 1, 1);
  Actor_SetPosition(ACTOR_DORA, PIXELS(392), PIXELS(896));
  turned = Actor_Get(ACTOR_DORA);

  turned->facing = FACING_EAST;
  turnBack = FACING_NORTH;
  turned->facing = turnBack;
  Actor_SetPosition(ACTOR_GARET, PIXELS(298), PIXELS(736));
  turned = Actor_Get(ACTOR_GARET);
  turnSide = FACING_SOUTH;
  turned->facing = turnSide;
  Actor_SetPosition(ACTOR_JENNA, PIXELS(298), PIXELS(760));
  turned = Actor_Get(ACTOR_JENNA);
  turned->facing = turnSide;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  actor = Actor_Get(ACTOR_FIRST_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(840);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_SECOND_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(844);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_THIRD_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(848);
  Actor_SetSpriteFlags(actor, 0);
  ((u8 *)FieldCamera_GetActor())[85] = 0;
  wait(1);
  FieldCamera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1) / 2, turnSide);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1) / 2, turnSide);
  Actor_SetScript(ACTOR_JENNA, gVale_JennaWalksToTheHouse);
  Actor_SetScript(ACTOR_GARET, gVale_GaretWalksToTheHouse);
  Event_Wait(40);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 400, 840);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, turnBack, 30);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Scene_StartEventPsynergy();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  Task_AddFrame(Vale_PlayStepSound, TASK_PASS_FRAME);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    wait(1);
  }
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);

  funcA = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddFrame(funcA, TASK_PASS_FRAME);
  funcB = Vale_CycleFirstBoardPalette;
  Task_AddFrame(funcB, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 400, 826);
  Event_Wait(20);
  {
    struct Actor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Vale_PlayStepSound);
  Task_Remove(funcA);
  Task_Remove(funcB);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_FIRST_BOARD, 0, 0);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(120);
  MapGrid_CopyCellsTo(7, 102, 84, 41, 2, 1);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  Task_AddFrame(Vale_PlayStepSound, TASK_PASS_FRAME);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    wait(1);
  }
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  funcC = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddFrame(funcC, TASK_PASS_FRAME);
  funcD = Vale_CycleSecondBoardPalette;
  Task_AddFrame(funcD, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_SECOND_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 377, 828);
  Event_Wait(20);
  {
    struct Actor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Vale_PlayStepSound);
  Task_Remove(funcC);
  Task_Remove(funcD);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_SECOND_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_SECOND_BOARD, 0, 0);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(120);
  MapGrid_CopyCellsTo(6, 102, 83, 41, 1, 1);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  Task_AddFrame(Vale_PlayStepSound, TASK_PASS_FRAME);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    wait(1);
  }
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  funcE = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddFrame(funcE, TASK_PASS_FRAME);
  funcF = Vale_CycleThirdBoardPalette;
  Task_AddFrame(funcF, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_THIRD_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_THIRD_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_THIRD_BOARD, 360, 837);
  Event_Wait(20);
  {
    struct Actor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Vale_PlayStepSound);
  Task_Remove(funcE);
  Task_Remove(funcF);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_THIRD_BOARD, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_THIRD_BOARD, 0, 0);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Event_Wait(120);
  Scene_EndEventPsynergy();
  MapGrid_CopyCellsTo(5, 103, 82, 42, 1, 1);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_Jump(ACTOR_DORA, 2, 20);
  Event_SetMessageId(MsgValeGoodJob);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST + FACING_STEP, 10);
  Vale_MoveActorAlongLowerZPath(ACTOR_DORA, 5, 6, 0);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    gFieldWork->message++;
  } else {
    Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  }
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Event_SetMessageId(MsgValeWorkingYourselvesBone);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
  }
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_SetMessageId(MsgValeDevastatedWhenKyle);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_GARET, 369, 904);
  Actor_WalkToAndWait(ACTOR_JENNA, 392, 904);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Vale_MoveActorAlongLowerZPath(ACTOR_JENNA, ANIM_LEADER_CLIMB,
                                ANIM_LEADER_CLIMB_OVER_TOP, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST + FACING_STEP, 20);
  Actor_Jump(ACTOR_JENNA, 4, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 392, 843);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Vale_MoveActorAlongLowerZPath(ACTOR_GARET, ANIM_LEADER_CLIMB,
                                ANIM_LEADER_CLIMB_OVER_TOP, 0);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkTo(ACTOR_GARET, 392, 843);
  Actor_GetById(ACTOR_JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JENNA, 408, 843);
  Event_Wait(1);
  {
    struct Actor *record = Actor_Get(ACTOR_JENNA);

    record->steeringFlags = ACTOR_FACES_MOTION | record->steeringFlags;
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_Jump(ACTOR_DORA, 4, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(80);
  Actor_TurnToFaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 40);
  Actor_TurnToFaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_OpenMessage(ACTOR_DORA, 0);
  none = 0;

  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
  }
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Event_SetMessageId(MsgValeAsStubbornAsYourFather);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_Jump(ACTOR_DORA, 4, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_DORA_HANDS_UP);
  Event_Wait(5);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, ACTOR_GARET, 10,
                                    14, 4, 14, none);
  Event_PlaySound(SE_RATTLE);
  actor = Actor_Get(ACTOR_DORA);
  actor->sprite->flags = none;
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
  Event_Wait(4);
  for (i = 0; i != 4; i++) {
    actor->z.fixed += PIXELS(3) / 2;
    actor->scaleY += -0x1999;
    Event_Wait(1);
  }
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3), PIXELS(3) / 2);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 374, VALE_GARET_REUNION_Z);
#if TBS_PATCH_2
  Actor_SetPosition(ACTOR_GARET, PIXELS(374), PIXELS(VALE_GARET_REUNION_Z));
#endif
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
  Actor_Jump(ACTOR_GARET, 2, 5);
  Event_PlaySound(SE_CRACK);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  MapGrid_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_DORA_PEEK);
  actor->scaleY = 0x8000;
  Actor_SetPosition(ACTOR_DORA, PIXELS(364), PIXELS(811));
  for (i = 0; i != 5; i++) {
    actor->scaleY += 0x1999;
    Event_Wait(1);
  }
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  FieldCamera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Actor_Jump(ACTOR_DORA, 6, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 1);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4), PIXELS(2));
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_GARET, 403, VALE_GARET_REUNION_Z);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 1);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
  Actor_Jump(ACTOR_GARET, 2, 5);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  MapGrid_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Event_PlaySound(SE_CRACK);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_JENNA, 408, 855);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_DORA, 3);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_DORA, 0, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_SetPositionAndReset(ACTOR_GARET, 398, 828);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(60);
  actor = Actor_Get(ACTOR_GARET);
  flag = 1;
  actor->steeringFlags |= flag;
  actor = Actor_Get(ACTOR_JENNA);
  actor->steeringFlags |= flag;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(ACTOR_JENNA, actor->x.part.pixel + 16, actor->z.part.pixel);
  Actor_WalkToAndWait(ACTOR_GARET, actor->x.part.pixel + 16,
                      actor->z.part.pixel - 16);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 30);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_JENNA, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WalkToAndWait(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_PrepareTwoActorsAndApply(GARET, JENNA);
  FieldCamera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
  Vale_MoveActorAlongHigherZPath(ACTOR_PARTY_LEADER,
                                 ANIM_LEADER_CLIMB_DOWN_FROM_TOP,
                                 ANIM_LEADER_CLIMB, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  {
    struct Actor *record = Actor_Get(ACTOR_DORA);

    record->steeringFlags = flag | record->steeringFlags;
  }
  Vale_MoveActorAlongHigherZPath(ACTOR_DORA, 6, 5, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Import_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(100);
  MapGrid_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  GameFlag_Set(FLAG_VALE_ROOF_JUST_MENDED);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#if TBS_PATCH_1
  leader->motionFlags = 3;
  leader->y.fixed = PIXELS(160);
  leader->targetY = 0x80000000;
  leader->velocityY = 0;
#else
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 3;
#endif
  Event_End();
}

#include "shared/effect_psynergy.h"

/// Cycles palettes on Garet and periodically spawns attached effects.
static void Vale_UpdateGaretPaletteAndEffects(void) {
  Vale_CycleActorPaletteAndSpawnEffects(Actor_GetById(ACTOR_GARET));
}

/// Cycles the leader palette and periodically spawns attached effects.
static void Vale_UpdateLeaderPaletteAndEffects(void) {
  Vale_CycleActorPaletteAndSpawnEffects(Actor_GetById(ACTOR_PARTY_LEADER));
}

/// Cycles the pillar's colour modes while Garet's Psynergy moves it in the
/// opening.
static void Vale_CycleOpeningPillarPalette(void) {
  Vale_CycleActorPalette(Actor_GetById(ACTOR_OPENING_PILLAR));
}

/// Cycles the first board palette while its repair effect runs.
static void Vale_CycleFirstBoardPalette(void) {
  Vale_CycleActorPalette(Actor_GetById(ACTOR_FIRST_BOARD));
}

/// Cycles the second board palette while its repair effect runs.
static void Vale_CycleSecondBoardPalette(void) {
  Vale_CycleActorPalette(Actor_GetById(ACTOR_SECOND_BOARD));
}

/// Cycles the third board's palette while its repair effect runs.
static void Vale_CycleThirdBoardPalette(void) {
  Vale_CycleActorPalette(Actor_GetById(ACTOR_THIRD_BOARD));
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
static s32 Vale_TestFacing(struct Actor *actor, struct Actor *target, s32 range,
                           s32 force) {
  s32 result;
  u32 angle;
  u32 left;
  u32 right;
  u32 dir;

  result = 0;
  if (actor->paused == 1) {
    if (actor->riseCounter == 0) {
      Actor_SelectAnimation(actor, 1);
      return 1;
    }
  }
  if (Vale_DistanceInPixels(&target->x.fixed, &actor->x.fixed) < range ||
      force != 0) {
    angle = (u16)ArcTan2(target->z.fixed - actor->z.fixed,
                         target->x.fixed - actor->x.fixed);
    left = (angle - 0x1000) & 0xf000;
    right = (angle + 0x1000) & 0xf000;
    angle &= 0xf000;
    dir = actor->facing & 0xf000;
    if (angle == dir || right == dir || left == dir || force != 0) {
      actor->paused = 1;
      Actor_SelectAnimation(actor, 1);
      result = 1;
      actor->riseCounter = result;
    } else {
      actor->paused = 0;
      Actor_SelectAnimation(actor, 2);
      actor->riseCounter = 0;
    }
  } else {
    actor->paused = 0;
    Actor_SelectAnimation(actor, 2);
    actor->riseCounter = 0;
  }
  return result;
}

/// The update routine of the man guarding Mt. Aleph: while he walks, he stops
/// when the leader comes within 18 pixels in front of him (Vale_TestFacing).
// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 48 to 44 bytes in tbs-en.
#ifdef UBFIX
static void Vale_HaveGuardWatchTheLeader(struct Actor *updated) {
#else
static s32 Vale_HaveGuardWatchTheLeader(struct Actor *updated) {
#endif
  struct Actor *guard = updated;
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (guard->targetX == ACTOR_NO_TARGET && guard->targetZ == ACTOR_NO_TARGET)
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

/// The Move hand's update routine in the opening: each frame it sheds a sparkle
/// (SPRITE_SPARKLE) just below and around it, with SE_ITEM_BREAK.
static void Vale_TrailSparkles(struct Actor *updated) {
  struct Actor *source = updated;
  s32 spawnPosition[3];
  s32 randomDistance;
  struct Actor *particle;
  SET_VECTOR(spawnPosition, source->x.fixed,
             source->y.fixed - (Import_RandomNext() << 4) - PIXELS(8),
             source->z.fixed);
  randomDistance = Import_RandomNext();
  Vector_AddPolarOffset(((randomDistance << 1) + randomDistance) << 4,
                        Import_RandomNext(), spawnPosition);
  particle = FieldActor_Create(SPRITE_SPARKLE, spawnPosition[0],
                               spawnPosition[1], spawnPosition[2]);
  if (particle != 0) {
    particle->motionFlags = 2;
    particle->gravity = 0x1999;
    particle->wait = 12;
    Actor_SetSpriteFlags(particle, 0);
    Actor_SelectAnimation(particle, 0);
    Actor_StartScript(particle, gVale_SparkleScript);
    {
      struct Sprite *record = particle->sprite;
      s32 priorityFlags = ~12;
      priorityFlags &= ((u8 *)&record->image)[9];
      priorityFlags |= 4;
      ((u8 *)&record->image)[9] = priorityFlags;
    }
  }
  Sound_Play(SE_ITEM_BREAK);
}

/// The Move hand rises, spins and shrinks for 31 frames, bursts into eight
/// sparkles with SE_PSYNERGY_SPARKLE, and is put out of sight.
static void Vale_VanishMoveHand(struct Actor *emitter) {
  s32 frameCountdown;

  Sound_Play(SE_CAST);

  for (frameCountdown = 30; frameCountdown >= 0; frameCountdown--) {
    emitter->y.fixed += PIXELS(1);
    emitter->facing = (u16)(emitter->facing + 0x2000);
    emitter->scaleX += -2048;
    emitter->scaleY += -2048;
    wait(1);
  }

  for (frameCountdown = 7; frameCountdown >= 0; frameCountdown--) {
    struct Actor *particle = FieldActor_Create(
        SPRITE_SPARKLE, emitter->x.fixed, emitter->y.fixed, emitter->z.fixed);

    if (particle != 0) {
      s32 verticalSpeed;

      Actor_SetSpriteFlags(particle, 0);
      Actor_StartScript(particle, (const void *)&gVale_SparkleScript[1]);

      verticalSpeed = Import_RandomNext() + PIXELS(1);
      particle->acceleration = Q16_ONE;
      particle->speedLimit = verticalSpeed;
      particle->motionFlags = 2;
      particle->gravity = 0x0a3d;

      particle->velocityY = Import_RandomNext() - Import_RandomNext();

      Vale_UpdateParticlePosition(particle,
                                  ((Import_RandomNext() * 3) << 3) + PIXELS(8),
                                  Import_RandomNext());
    }
  }

  Sound_Play(SE_PSYNERGY_SPARKLE);

  emitter->x.fixed = 0;
  emitter->y.fixed = 0;
  emitter->z.fixed = 0;
  emitter->targetX = ACTOR_NO_TARGET;
  emitter->targetY = ACTOR_NO_TARGET;
  emitter->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(emitter, 0, 0, 0);
}

/// Moves a particle outward in the horizontal plane without changing its
/// height.
static void Vale_UpdateParticlePosition(struct Actor *particle, s32 distance,
                                        s32 angle) {
  s32 position[3];
  if (particle != 0) {
    SET_VECTOR(position, particle->x.fixed, particle->y.fixed,
               particle->z.fixed);
    Vector_AddPolarOffset(distance, angle, position);
    FieldActor_SetMoveTarget(particle, position[0], position[1], position[2]);
  }
}

/// Pushing the pillar in Vale (cell behaviors 30 and 31) moves it a cell.
static void Vale_PushPillar(void) {
#if !TBS_PATCH_1
  // Japanese edition only: look up the pillar but do not use the return value.
  if (GameFlag_Get(FLAG_KALAY_REACHED))
    Actor_GetById(ACTOR_PILLAR_AFTER_KALAY);
  else
    Actor_GetById(ACTOR_PILLAR);
#endif
  MapGrid_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  PushedActor_AdvancePair();
  Vale_MovePillarBelowCliff();
}

/// Records where the pillar now stands and blocks its cell. After Move or a
/// push, it records the pillar's column in FLAG_VALE_PILLAR_IN_WEST_COLUMN,
/// _IN_MIDDLE_COLUMN and _IN_EAST_COLUMN and blocks the cell the pillar now
/// stands on.
static void Vale_MovePillarBelowCliff(void) {
  struct Actor *actor;
  s32 column;

  MapGrid_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  if (GameFlag_Get(FLAG_KALAY_REACHED))
    actor = Actor_GetById(ACTOR_PILLAR_AFTER_KALAY);
  else
    actor = Actor_GetById(ACTOR_PILLAR);
  if (actor == NULL)
    return;
  Event_ClearFlag(FLAG_VALE_PILLAR_IN_WEST_COLUMN);
  Event_ClearFlag(FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN);
  Event_ClearFlag(FLAG_VALE_PILLAR_IN_EAST_COLUMN);
  column = actor->x.fixed >> 20;
  if (column == 22) {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, column, 36);
    GameFlag_Set(FLAG_VALE_PILLAR_IN_WEST_COLUMN);
  } else if (column == 23) {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, column, 36);
    GameFlag_Set(FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN);
  } else {
    MapGrid_CopyCellAttributes(17, 1, 1, 1, 24, 36);
    GameFlag_Set(FLAG_VALE_PILLAR_IN_EAST_COLUMN);
  }
}

/// The task that plays SE_PSYNERGY_SPARKLE every 16 frames while Isaac's
/// Psynergy lifts a board at the roof's end.
static void Vale_PlayStepSound(void) {
  if ((*(u32 *)&gFrameCount & 15) == 0)
    Sound_Play(SE_PSYNERGY_SPARKLE);
}

/// Searching a locked door in Vale (cell behavior 24).
static void Vale_SearchTheLockedDoor(void) {
  Event_Start();
  FieldMessage_Show(MsgValeDoorWontOpen, 1);
  Event_End();
}

/// Pauses the man guarding Mt. Aleph while the corresponding event flag is set.
static void Vale_SynchronizeAlephGuardPause(void) {
  s32 pauseFlag;

  if (Event_GetFlag(FLAG_VALE_MT_ALEPH_GUARD_PAUSED) != 0) {
    pauseFlag = GameFlag_Get(FLAG_MENU_OPEN);
    if (pauseFlag == 0) {
      Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = pauseFlag;
      GameFlag_Clear(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
    }
  } else {
    if (GameFlag_Get(FLAG_MENU_OPEN) != 0) {
      Actor_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = 1;
      GameFlag_Set(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
    }
  }
}

/// Keeps the raised pillar's priority in step with the leader's height.
static void Vale_AdjustPillarPriorityForLeaderHeight(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor *pillar;
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    pillar = Actor_GetById(ACTOR_PILLAR_AFTER_KALAY);
  else
    pillar = Actor_GetById(ACTOR_PILLAR);
  if (pillar != 0) {
    if (leader->y.fixed > PIXELS(200))
      pillar->priorityFlags =
          ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
    else
      pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
}

#include "shared/actor_orbit.h"

/// Someone calling up Psynergy: drawn SPRITE_COLOR_WHITE (7) for two frames in
/// four, and the World Map's attached sparks every 16 frames.
static void Vale_FlashActorAndSpawnEffects(struct Actor *actor) {

  if ((gFrameCount & 2) != 0)
    Actor_SetColorMode(actor, 7);
  else
    Actor_SetColorMode(actor, 0);
  if ((gFrameCount & 0xf) == 0)
    WorldMap_CreateAttachedEffects(actor);
}

/// Someone using Psynergy: colour modes SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG
/// (0 to 5), the sprite and its five swaps of colour channels, in turn, and
/// sparks every 16 frames.
static void Vale_CycleActorPaletteAndSpawnEffects(struct Actor *actor) {
  if ((gFrameCount & 1) != 0)
    Actor_SetColorMode(actor, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 0xf) == 0)
    WorldMap_CreateAttachedEffects(actor);
}

/// What Psynergy moves: colour modes SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG in
/// turn, a step every second frame.
static void Vale_CycleActorPalette(struct Actor *actor) {
  if ((gFrameCount & 1) != 0)
    Actor_SetColorMode(actor, (gFrameCount >> 1) % 6);
}

#include "shared/effect_vertical.h"
#include "shared/world_map_attached_effects.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
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
/// the river and vanishes (Vale_UnusedWaterBurstsByTheRiver).
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
/// its second word, past the first SC_RELEASE.
s32 gVale_SparkleScript[] = {
    SC_RELEASE,
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(1) / 16),
    SC_ANIMATION(0),
    SC_WAIT(10),
    SC_RELEASE,
};

struct SceneEntrance gVale_Entrances[] = {
    { 0, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_SANCTUM, CONDITION_ALWAYS, 256, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, 38, 0, 636, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_KRADENS_COTTAGE_SOUTH, CONDITION_ALWAYS, 29, 0, 792, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_VALE_PLAZA, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, 406, 0, 743, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, 262, 0, 821, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_B, CONDITION_ALWAYS, 340, 0, 904, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, 326, 0, 1142, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, 374, 0, 1254, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_COTTAGE_ROOMS_A_NORTH_SECOND_DOOR, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_TITLE, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { VALE_FROM_ROOF_MENDED, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gVale_Exits[] = {
    SCENE_EXITS(Vale),
    SCENE_EXIT(VALE_TO_SOL_ENTRANCE, SolEntrance, SOL_ENTRANCE_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_KRADENS_COTTAGE_NORTH, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_VALE_KRADENS_COTTAGE_SOUTH, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_EAST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_EAST),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_SOUTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_B, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_WEST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_WEST),
    SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_NORTH_SECOND_DOOR, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_SANCTUM, Sanctum, SANCTUM_FROM_VALE),
    SCENE_EXIT(VALE_TO_ROOF_MENDED, Vale, VALE_FROM_ROOF_MENDED),
    SCENE_EXITS_END,
};

struct ActorPlacement gVale_ActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SCENE_TABLE_END },
};

struct ActorPlacement gVale_ActorPlacementsAfterLeaving[] = {
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SCENE_TABLE_END },
};

struct ActorPlacement gVale_ActorPlacementsAfterKalay[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SCENE_TABLE_END },
};

struct ActorPlacement gVale_RepairActorPlacements[] = {
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(409), 0, PIXELS(1134), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SCENE_TABLE_END },
};

struct Trigger gVale_Triggers[] = {
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephLooksBeautifulTodayIts) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeValeStaysPristineBecauseOfMt) },
    { TRIGGER_TALK, ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, EVENT(Vale_TalkAboutTheMayorAndTremors) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeThisPlaceLooksJustLikeIt) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacWatchOutDontGetToo) },
    { TRIGGER_TALK, ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeValeMustRemainHiddenPsynergysSecrets) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_WarnAboutGoingBeyondKradensCottage) },
    { TRIGGER_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, EVENT(Vale_ArgueWithGaretsSister) },
    { TRIGGER_TALK, ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeYoureInTroubleNowGaretYour) },
    { TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS, EVENT(Vale_DoraFretsAboutTheRoof) },
    { TRIGGER_TALK, ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephIsASacredPeak) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Vale_EnterSanctum) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Vale_ApproachMtAleph) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageSouth) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Vale_ExitToValePlaza) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Vale_EnterGaretsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsA) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Vale_EnterIsaacsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Vale_EnterJennasGrandparentsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsAWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT(Vale_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Vale_StepOntoBridge) },
    { TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(Vale_StepOffBridge) },
    { TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Vale_WatchThePuppies) },
    { TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(Vale_TurnBackFromThePuppies) },
    { TRIGGER_STEP, 50, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT(Vale_WarnAgainstSneakingUpMtAleph) },
    { TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(Vale_RepeatMtAlephWarningFromFirstApproach) },
    { TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(Vale_RepeatMtAlephWarningFromSecondApproach) },
    { TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(Vale_RepeatMtAlephWarningAtEitherPause) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT(Vale_ClimbLadderOntoRoof) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT(Vale_ClimbDownFromRoof) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, EVENT(Vale_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(Vale_MovePillarBelowCliff) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 24, CONDITION_ALWAYS, EVENT(Vale_SearchTheLockedDoor) },
    { TRIGGERS_END },
};

struct Trigger gVale_TriggersAfterLeaving[] = {
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeIsSavedThanksToThe) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeEverSinceTheTremorsStartedMt) },
    { TRIGGER_TALK, ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, EVENT(Vale_TalkAboutTheMayorAndTremors) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeSighNoLongerWillISee) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIDontBlameFelixIfOnly) },
    { TRIGGER_TALK, ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeTheMayorWasTooKindTo) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT(Vale_WarnAboutGoingBeyondKradensCottage) },
    { TRIGGER_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreSoLuckyGaret) },
    { TRIGGER_TALK, ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseTakeCareOfMyBrother) },
    { TRIGGER_TALK, ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeGeeIWishICouldSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeMyBrothersStrengthIsHisBest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeImGladThatFelixIsAlive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeFelixWasAGoodPersonI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, MSGNO(MsgValePoorKradenWasTakenToSome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeWasSavedBecausePeopleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeICantBelieveAnyoneWouldClimb) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, MSGNO(MsgValeIJustDontFeelSafeKnowing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeThePeopleOfValeWillNever) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Vale_EnterSanctum) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Vale_ApproachMtAleph) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageSouth) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Vale_ExitToValePlaza) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Vale_EnterGaretsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsA) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Vale_EnterIsaacsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Vale_EnterJennasGrandparentsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsAWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT(Vale_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Vale_StepOntoBridge) },
    { TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(Vale_StepOffBridge) },
    { TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Vale_WatchThePuppies) },
    { TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(Vale_TurnBackFromThePuppies) },
    { TRIGGER_STEP, 50, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT(Vale_WarnAgainstSneakingUpMtAleph) },
    { TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(Vale_RepeatMtAlephWarningFromFirstApproach) },
    { TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(Vale_RepeatMtAlephWarningFromSecondApproach) },
    { TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(Vale_RepeatMtAlephWarningAtEitherPause) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT(Vale_ClimbLadderOntoRoof) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT(Vale_ClimbDownFromRoof) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, EVENT(Vale_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(Vale_MovePillarBelowCliff) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 24, CONDITION_ALWAYS, EVENT(Vale_SearchTheLockedDoor) },
    { TRIGGERS_END },
};

struct Trigger gVale_TriggersAfterKalay[] = {
    { TRIGGER_TALK, ACTOR_GARETS_SISTER_AFTER_KALAY, FLAG_VALE_GARET_MET_SISTER, EVENT(Vale_GaretMeetsHisSister) },
    { TRIGGER_TALK, ACTOR_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICanTellJustByLooking) },
    { TRIGGER_TALK, ACTOR_GARETS_BROTHER_AFTER_KALAY, FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY, EVENT(Vale_GaretAsksToProtectFamily) },
    { TRIGGER_TALK, ACTOR_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeIWillDoMyBestTo) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT(Vale_AskWhetherJennaHasReturned) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValePromiseMeIsaacPromiseMeYoull) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeIUsedToPlayHere) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephsEruptionWasSoPowerful) },
    { TRIGGER_TALK, ACTOR_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, EVENT(Vale_ReactToAnotherTremor) },
    { TRIGGER_TALK, ACTOR_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheDangerIsGone) },
    { TRIGGER_TALK, ACTOR_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, EVENT(Vale_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_GARETS_SISTER_AFTER_KALAY, FLAG_VALE_GARET_MET_SISTER, EVENT(Vale_GaretMeetsHisSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreWorkingSoHard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_GARETS_BROTHER_AFTER_KALAY, FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY, EVENT(Vale_GaretAsksToProtectFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICouldGoWithYouIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT(Vale_AskWhetherJennaHasReturned) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValeYouDidntBringJennaWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeFelixMustBeProtectingJennaIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeTheEarthquakesArentCausedByEruptions) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, MSGNO(MsgValeAhhICantStandTheseEarthquakes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheSmokeFromMtAlephIsnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_MAN_MISSING_TRAVELERS, FLAG_VALE_ASKED_ABOUT_TRAVELERS, EVENT(Vale_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgValeHavingNoTravelersIsNoFun) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Vale_EnterSanctum) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Vale_ApproachMtAleph) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Vale_ExitToValeKradensCottageSouth) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Vale_ExitToValePlaza) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Vale_EnterGaretsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsA) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Vale_EnterIsaacsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Vale_EnterJennasGrandparentsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Vale_EnterCottageRoomsAWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT(Vale_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Vale_StepOntoBridge) },
    { TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(Vale_StepOffBridge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT(Vale_ClimbLadderOntoRoof) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT(Vale_ClimbDownFromRoof) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT_AFTER_KALAY, FLAG_VALE_NUT_CAUGHT, EVENT(Vale_CatchNutAfterKalay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_AFTER_KALAY, CONDITION_ALWAYS, EVENT(Vale_MovePillarBelowCliff) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT(Vale_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 24, CONDITION_ALWAYS, EVENT(Vale_SearchTheLockedDoor) },
    { TRIGGERS_END },
};
// clang-format on

// The doors' cell animations, from the layout's cells below the village, each
// picture shown for two frames; vale_stormy_night.c has the same four kinds.
// Each table takes the name of a door it opens. The two of Jenna's
// grandparents' kind and the west house's show their one open picture twice,
// where the stormy night's show two.

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
// warning (Vale_WarnAgainstSneakingUpMtAleph), and again from the west
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
