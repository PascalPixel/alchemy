/// @file
/// SceneId_Vale, Isaac's village by day, outside the houses: the door of
/// Isaac's house, the houses of Garet's family and of Jenna's grandparents,
/// the Sanctum and the paths out to Mt. Aleph and Kraden's cottage. The
/// placements and events come in three sets, chosen by the story flags: before
/// the party leaves Vale, after it has left (FLAG_PARTY_LEFT_VALE) and back
/// from Kalay (FLAG_REACHED_KALAY); VALE_FROM_TITLE has a set of its own. In
/// story order:
///
/// - The day the story begins, three years after the storm
///   (vale_interiors_inn.c), which took Jenna's family and Isaac's father,
///   Kyle. The game opens at VALE_FROM_TITLE with Event_MendingTheRoof: under
///   the caption "Three years later...", Isaac is mending the roof of his house
///   while Dora, his mother, calls up to him; Garet, who will travel with him,
///   trains his Psynergy nearby, and Jenna comes by, and the two speak of that
///   day before going to fetch Isaac to go to Kraden's. VALE_TO_ROOF_MENDED
///   brings the party back in at VALE_FROM_ROOF_MENDED, where Event_RoofMended
///   plays once (group flag FLAG_PLACE_TEMP_08): Dora praises the roof,
///   remembers Kyle, and sends the three off on their climb of Mt. Aleph with
///   Kraden, the scholar who teaches them Alchemy (FLAG_SCENE_TEMP_02); for the
///   rest of that visit Dora frets about the hole clumsy Garet left.
/// - Vale before the party leaves. Until Kraden has planned the visit to Sol
///   Sanctum (FLAG_SANCTUM_VISIT_PLANNED), Jenna stops Isaac on the path to Mt.
///   Aleph; a bearded man in a long robe jumps out to warn him against sneaking
///   up the sacred peak (FLAG_VALE_WARNED_AGAINST_MT_ALEPH) and then keeps
///   watching him, asking again whenever he comes near (touch cells 51 to 53).
///   Three puppies (from ACTOR_FIRST_PUPPY) play; walking up to them from
///   either side (touch cells 20 and 21) sets them barking, until the party has
///   met Saturos and Menardi, of the strangers staying at the inn
///   (FLAG_MET_SATUROS_AND_MENARDI); then the setup removes them. Villagers
///   talk of the mayor, Garet's grandfather, and of not going beyond Kraden's
///   cottage, Garet's sister scolds him, and Dora says Isaac can fix the hole
///   later.
/// - After the party has left Vale (FLAG_PARTY_LEFT_VALE): Dora will not open
///   the door of Isaac's house to him ("My Isaac would keep his promise... Go
///   away."), and the villagers speak of the tremors and of Kraden, who has
///   not come back.
/// - Back from Kalay (FLAG_REACHED_KALAY): Garet meets his sister (group flag
///   FLAG_PLACE_TEMP_03) and asks one of his family to protect it while he is
///   gone (FLAG_PLACE_TEMP_04), a young man asks whether Jenna is back
///   (FLAG_VALE_ASKED_IF_JENNA_IS_BACK), another feels a tremor, another asks
///   about the lack of travellers since the eruption (FLAG_PLACE_TEMP_07).
///   Isaac's house opens again: Dora lies ill inside (vale_interiors_inn.c).
///
/// In every set: a Nut to catch with Catch (FLAG_VALE_NUT_CAUGHT; ACTOR_NUT, or
/// ACTOR_NUT_AFTER_KALAY), a pillar to push or Move, whose column the setup
/// restores from FLAG_PLACE_TEMP_14 to _16, a passage the leader moves through
/// on touch cells 100 and 101 (FLAG_SCENE_TEMP_04 while inside), and cells of
/// an upper path that change while the leader is on it (FLAG_SCENE_TEMP_10);
/// after a battle (FLAG_KEEP_PARTY_POSITION) the setup restores both. Exits:
/// VALE_TO_SOL_ENTRANCE up Mt. Aleph to Sol Entrance (SceneId_SolEntrance),
/// VALE_TO_VALE_GATE to SceneId_ValeGate, VALE_TO_VALE_KRADEN_NORTH and
/// VALE_TO_VALE_KRADEN_SOUTH to SceneId_ValeKraden, the doors
/// VALE_TO_VALE_INTERIORS_SHOPS_EAST, VALE_TO_VALE_INTERIORS_SHOPS_SOUTH,
/// VALE_TO_VALE_INTERIORS_SHOPS_NORTH, VALE_TO_VALE_INTERIORS_SHOPS_WEST and
/// VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR into the houses
/// (SceneId_ValeInteriorsShops, vale_interiors_shops.c),
/// VALE_TO_VALE_INTERIORS_INN into Isaac's house (SceneId_ValeInteriorsInn) and
/// VALE_TO_SANCTUM into SceneId_Sanctum. Two scenes no table reaches remain:
/// Event_UnusedRoofMended, a copy of the roof's end, and
/// Event_UnusedWaterBurstsByTheRiver, which sets group flag FLAG_PLACE_TEMP_05.
/// FLAG_SCENE_TEMP_41 pauses the man who watches Isaac while a menu is open.
/// The TBS_PATCH_4 editions also set Dora's sprite priority before Kalay.
///
/// In the opening Garet trains his Psynergy on the pillar: his sprite flashes
/// white, then cycles through its colour modes with sparks about him, while
/// the hand of Move (ACTOR_OPENING_MOVE_HAND) pushes the pillar west, the
/// pillar cycling its colour modes too, and sheds sparkles
/// (https://alchemy.pascalpixel.com/places/vale/opening-move.gif: the screen,
/// Garet, the pillar and the hand boxed, beside the sprites alone and the 16
/// sprite palettes).
///
/// Open questions: none.

#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/vale.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/scene/common/world_map_linked_effects.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "field_effect.h"
#include "map_scroll.h"
#include "fixed_math.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"

// Where Garet lands after his jump once Dora has gone in, at the roof's end
// (Event_RoofMended): two pixels further south in the TBS_PATCH_2 editions.
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
static void Event_MendingTheRoof(void);
static void Event_RoofMended(void);
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
static void Vale_VanishMoveHand(struct FieldActor *emitter);
static void Vale_UpdateParticlePosition(struct FieldActor *particle,
                                        s32 distance, s32 angle);
static void Event_SettleMovedPillar(void);
static void Vale_PlayStepSound(void);
static void Vale_SynchronizeAlephGuardPause(void);
static void Vale_AdjustPillarPriorityForLeaderHeight(void);
static void Vale_FlashActorAndSpawnEffects(struct FieldActor *actor);
static void Vale_CycleActorPaletteAndSpawnEffects(struct FieldActor *actor);
static void Vale_CycleActorPalette(struct FieldActor *actor);

/// The placed actors of Vale by day, numbered in the order of gVale_Placements
/// and gVale_PlacementsAfterLeaving, which place the same people in the same
/// rows. A row whose sprite is a party member's (Garet's, Jenna's) takes that
/// member's actor id instead (Event_SpawnObjectTable), so the numbering skips
/// it.
enum ValeActor {
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
  /// your grandpa's the mayor." (Event_TalkAboutTheMayorAndTremors)
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
  /// cottage." (Event_WarnAboutGoingBeyondKradensCottage)
  /// ★ Named from the game: "You mustn't go beyond Kraden's cottage."
  /// (MsgValeDontGoBeyondKradensCottage)
  ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN,
  /// The three puppies, small white dogs, removed once the party has met
  /// Saturos and Menardi. "Rrruff! Rrrruff!"
  /// ★ Named from what the player sees: small white dogs with brown ears (its
  /// render)
  ACTOR_FIRST_PUPPY,
  /// A young woman with an orange ponytail: "Garet, you make me so mad
  /// sometimes!" (Event_ArgueWithGaretsSister); Garet calls her "Sis" back
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
  /// SPRITE_STONE_PILLAR, as Goma Cave's pillars: pushed (Event_PushPillar) or
  /// moved with Move, its column kept in FLAG_PLACE_TEMP_14 to _16
  /// (Event_SettleMovedPillar).
  /// ★ Named from what the player sees: a stone pillar of brown rock standing
  /// below the cliff north of the village (its render)
  ACTOR_PILLAR,
  /// ★ Named from the game: "your mother, Dora, has been crying"
  /// (MsgValeDoraHasBeenCrying)
  ACTOR_DORA, ///< Isaac's mother
  /// A man with long brown hair and a beard in a long robe, who jumps out to
  /// warn Isaac off the sacred peak, then keeps watching him
  /// (Event_WarnAgainstSneakingUpMtAleph). "Mt. Aleph is a sacred peak. It is
  /// forbidden to climb it!"
  /// ★ Named from the game: "Mt. Aleph is a sacred peak."
  /// (MsgValeMtAlephIsASacredPeak)
  ACTOR_MAN_GUARDING_MT_ALEPH,
  /// The boards on the roof, sprite 0xce, which Isaac moves into place with
  /// Psynergy at the roof's end (Event_RoofMended).
  /// ★ Named from what the player sees: a short plank of yellow wood
  /// (spr_206.png)
  ACTOR_FIRST_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood
  /// (spr_206.png)
  ACTOR_SECOND_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood
  /// (spr_206.png)
  ACTOR_THIRD_BOARD,
  /// ★ Named from the game: "Nut" (MsgItemName), the item Catch takes
  ACTOR_NUT ///< the Nut for Catch, until FLAG_VALE_NUT_CAUGHT is set
};

/// Back from Kalay, the placed actors of gVale_PlacementsAfterKalay; Garet's
/// row takes his own actor id.
enum ValeAfterKalayActor {
  /// Garet's sister. "Ah! It's been a while... You're back?"
  /// (Event_GaretMeetsHisSister)
  /// ★ Named from the game: "Thanks, Sis." (MsgValeAh), Garet answering her
  ACTOR_GARETS_SISTER_AFTER_KALAY = ACTOR_FIRST_PLACED,
  /// "I will do my best to show everyone that I am strong enough to protect
  /// our family and Vale." (Event_GaretAsksToProtectFamily)
  /// ★ Named from the game: "Please take care of my brother"
  /// (MsgValePleaseTakeCareOfMyBrother)
  ACTOR_GARETS_BROTHER_AFTER_KALAY,
  /// The young man of ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM: "Is Jenna back
  /// yet?" (Event_AskWhetherJennaHasReturned)
  /// ★ Named from the game: "Is back yet?" (MsgValeIsJennaBackYet)
  ACTOR_YOUNG_MAN_ASKING_FOR_JENNA,
  /// The strongman of ACTOR_STRONGMAN_BY_THE_RIVER: "I still remember when
  /// Felix and I used to play here...*Sniff*"
  /// ★ Named from the game: "I still remember when" (MsgValeIUsedToPlayHere)
  ACTOR_STRONGMAN_REMEMBERING_FELIX,
  /// Three sprites of water, CrossboneIsleFloodgate's water sprite 0xd6. Only
  /// Event_UnusedWaterBurstsByTheRiver shows them: during a
  /// tremor each in turn grows from a tenth of its size at (266, 1230), north
  /// of ACTOR_STRONGMAN_REMEMBERING_FELIX, and vanishes
  /// (gVale_UnusedWaterGrows). The row after them places the woman with fair
  /// hair of ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN at the origin, and nothing
  /// names her.
  /// ★ Named from what the player sees: a spout of white water spraying up
  /// (spr_214.png)
  ACTOR_FIRST_WATER,
  /// An old woman: "Mt. Aleph's eruption was so powerful, it caused the
  /// continent to crack!"
  /// ★ Named from the game: "Mt. Aleph's eruption was so powerful"
  /// (MsgValeMtAlephsEruptionWasSoPowerful)
  ACTOR_OLD_WOMAN_ON_THE_ERUPTION = ACTOR_FIRST_WATER + 4,
  /// A woman in a green dress: "Aghh! I felt another one!"
  /// (Event_ReactToAnotherTremor)
  /// ★ Named from the game: "Aghh! I felt another one!"
  /// (MsgValeIFeltAnotherOne)
  ACTOR_WOMAN_FEELING_TREMORS,
  /// A little girl with orange pigtails: "They say the danger is gone, even
  /// though the earthquakes continue..."
  /// ★ Named from the game: "They say the danger is gone, even though the
  /// earthquakes continue..." (MsgValeTheySayTheDangerIsGone)
  ACTOR_GIRL_DOUBTING_SAFETY,
  /// A burly grey-haired man: "I wonder why we haven't seen any travelers
  /// since the eruption..." (Event_AskAboutTheLackOfTravelers)
  /// ★ Named from the game: "I wonder why we haven't seen any travelers
  /// since the eruption..." (MsgValeNoTravelersSinceTheEruption)
  ACTOR_MAN_MISSING_TRAVELERS,
  /// ★ Named from the game: "Nut" (MsgItemName)
  ACTOR_NUT_AFTER_KALAY,
  /// ★ Named from what the player sees: a stone pillar of brown rock standing
  /// below the cliff north of the village (its render)
  ACTOR_PILLAR_AFTER_KALAY ///< the pillar of ACTOR_PILLAR
};

/// The game's opening (Event_MendingTheRoof), the placed actors of
/// gVale_RepairPlacements; Jenna's and Garet's rows take their own actor ids.
enum ValeOpeningActor {
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
  /// ★ Named from what the player sees: a short plank of yellow wood
  /// (spr_206.png)
  ACTOR_OPENING_FIRST_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood
  /// (spr_206.png)
  ACTOR_OPENING_SECOND_BOARD,
  /// ★ Named from what the player sees: a short plank of yellow wood
  /// (spr_206.png)
  ACTOR_OPENING_THIRD_BOARD,
  /// Sprite 0xd7, the hand of Move (psynergy_effects/catch.c): it pushes the
  /// pillar for Garet, shedding sparkles.
  /// ★ Named from what the player sees: a large open hand (spr_215.png)
  ACTOR_OPENING_MOVE_HAND
};

/// The leader's animation of hammering on the roof, beyond enum
/// ActorAnimation's.
enum { ANIM_HAMMER = 11 };

/// Animations of Dora's and Garet's sprites beyond enum ActorAnimation, as
/// their animation tables and frames under graphics/sprite draw them.
enum ValeAnimation {
  ANIM_DORA_HANDS_UP = 7, ///< both hands raised (spr_031.png)
  /// Only her head and hands showing, as she peeks back out (spr_031.png).
  ANIM_DORA_PEEK = 8,
  /// Garet throwing up his arms, then crouching, as he jumps in fright
  /// (spr_001.png).
  ANIM_GARET_STARTLED = 13
};

#include "common/object_staged_actor.h"

const struct SceneEntrance *Vale_GetEntrances(void) { return gVale_Entrances; }

const struct SceneRegion *Vale_GetRegions(void) { return NULL; }

const u32 *Vale_GetExits(void) { return gVale_Exits; }

/// Places the villagers for the story so far, or for the roof repair
/// (VALE_FROM_TITLE).
const struct ScenePlacement *Vale_GetPlacements(void) {
  u8 *scene_state = (u8 *)&gGameState;
  if (((struct GameState *)scene_state)->entrance == VALE_FROM_TITLE)
    return gVale_RepairPlacements;
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    return gVale_PlacementsAfterKalay;
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    return gVale_PlacementsAfterLeaving;
  return gVale_Placements;
}

/// Catch on the Nut in Vale: the party gets it.
static void Event_CatchNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_VALE_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// The same Nut after Kalay, where it is ACTOR_NUT_AFTER_KALAY.
static void Event_CatchNutAfterKalay(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_NUT_AFTER_KALAY, 0, 0);
  Engine_GameFlagSet(FLAG_VALE_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// Vale's events for the story so far: before leaving, after leaving, or back
/// from Kalay.
const struct SceneEvent *Vale_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    return gVale_EventsAfterKalay;
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    return gVale_EventsAfterLeaving;
  return gVale_Events;
}

/// A little girl in Vale (ACTOR_GIRL_JEALOUS_OF_GARET) asks about the mayor
/// before the party leaves, and speaks of the tremors after.
static void Event_TalkAboutTheMayorAndTremors(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_EventSetMessage(MsgValeTheGroundStillShakes);
    Engine_EventShowMessage(ACTOR_GIRL_JEALOUS_OF_GARET, 0);
  } else {
    Engine_EventSetMessage(MsgValeYourGrandpaIsTheMayor);
    Engine_ActorFaceEachOther(ACTOR_GIRL_JEALOUS_OF_GARET, ACTOR_PARTY_LEADER,
                              4);
    Engine_EventAskYesNo(ACTOR_GIRL_JEALOUS_OF_GARET, 0);
  }
  Engine_EventEnd();
}

/// A woman in Vale (ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN) asks whether Isaac
/// is going to Kraden's cottage and warns him not to go beyond it; after the
/// party has left, says Kraden has not come back.
static void Event_WarnAboutGoingBeyondKradensCottage(void) {
  s32 flag = FLAG_VALE_ASKED_IF_GOING_TO_KRADEN;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_EventSetMessage(MsgValeKradenHasntComeBack);
    Engine_EventShowMessage(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  } else if (Engine_GameFlagIsSet(flag) == 0) {
    Engine_GameFlagSet(flag);
    Engine_EventSetMessage(MsgValeDoYouNeedToGo);
    Engine_ActorFaceEachOther(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN,
                              ACTOR_PARTY_LEADER, 4);
    Engine_EventAskYesNo(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  } else {
    Engine_EventSetMessage(MsgValeDontGoBeyondKradensCottage);
    Engine_ActorFaceEachOther(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN,
                              ACTOR_PARTY_LEADER, 4);
    Engine_EventShowMessage(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
  }
  Engine_EventEnd();
}

/// Garet's sister scolds him in Vale; afterwards she only threatens him.
/// "Garet, you make me so mad sometimes!"
static void Event_ArgueWithGaretsSister(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS) == 0) {
    GameFlag_Set(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS);
    Engine_EventSetMessage(MsgValeYouMakeMeSoMad);
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_SISTER, 20);
    Event_ShowMessageAndWait(ACTOR_GARETS_SISTER, 0, 6);
    Actor_FaceDirection(ACTOR_GARETS_SISTER, FACING_WEST, 30);
    Engine_ActorJump(ACTOR_GARETS_SISTER, 2, 20);
    Event_ShowMessageAndWait(ACTOR_GARETS_SISTER, 0, 6);
    Engine_ActorFaceEachOther(ACTOR_GARETS_SISTER, ACTOR_PARTY_LEADER, 10);
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_ShowMessageAndWait(ACTOR_GARETS_SISTER, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  } else {
    Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Engine_EventSetMessage(MsgValeIllGetYouForMy);
    Event_ShowMessageAndWait(ACTOR_GARETS_SISTER, 0, 20);
  }
  Engine_EventEnd();
}

/// Talking to Dora in Vale before the party leaves: right after the roof is
/// mended (FLAG_SCENE_TEMP_02) she frets about the hole clumsy Garet left;
/// otherwise, since it will not rain for a while, Isaac can fix it later.
static void Event_DoraFretsAboutTheRoof(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
    Engine_EventSetMessage(MsgValeIToldGaretItWas);
  } else {
    Engine_EventSetMessage(MsgValeItWontRainForSome);
  }
  Engine_EventShowMessage(ACTOR_DORA, 0);
  Engine_EventEnd();
}

/// Back from Kalay, a young man in Vale (ACTOR_YOUNG_MAN_ASKING_FOR_JENNA) asks
/// whether Jenna is back.
static void Event_AskWhetherJennaHasReturned(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER,
                            ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, 20);
  Engine_EventSetMessage(MsgValeIsJennaBackYet);
  Engine_EventShowMessage(ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, 0);
  Engine_GameFlagSet(FLAG_VALE_ASKED_IF_JENNA_IS_BACK);
  Engine_EventEnd();
}

/// Back from Kalay, a woman in Vale (ACTOR_WOMAN_FEELING_TREMORS) feels
/// another tremor, and the view shakes.
static void Event_ReactToAnotherTremor(void) {
  Engine_EventBegin();
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_TaskWait(10);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventSetMessage(MsgValeIFeltAnotherOne);
  Event_ShowMessageAndWait(ACTOR_WOMAN_FEELING_TREMORS, 0, 20);
  Actor_FaceActor(ACTOR_WOMAN_FEELING_TREMORS, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(ACTOR_WOMAN_FEELING_TREMORS, 0);
  Engine_EventEnd();
}

/// A burly grey-haired man asks about the lack of travellers since the
/// eruption. He (ACTOR_MAN_MISSING_TRAVELERS) is in Vale back from Kalay, and
/// the talk sets group flag FLAG_PLACE_TEMP_07.
static void Event_AskAboutTheLackOfTravelers(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_MAN_MISSING_TRAVELERS, 2);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_MAN_MISSING_TRAVELERS, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgValeNoTravelersSinceTheEruption);
  Engine_EventAskYesNo(ACTOR_MAN_MISSING_TRAVELERS, 0);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_07);
  Engine_EventEnd();
}

/// Walking onto the upper path in Vale (touch cell 15): its collision cells
/// change, remembered in FLAG_SCENE_TEMP_10.
static void Event_SetPathCellOverride(void) {
  Engine_GameFlagSet(FLAG_SCENE_TEMP_10);
  Map_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

/// Walking off the upper path in Vale (touch cell 14) restores its collision
/// cells.
static void Event_ClearPathCellOverride(void) {
  Engine_GameFlagClear(FLAG_SCENE_TEMP_10);
  Map_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

/// The door of Vale Sanctum (touch cell 1): it opens, the leader walks in, and
/// VALE_TO_SANCTUM leads into the Sanctum.
static void Event_EnterSanctum(void) {
  Event_PlayCue(SE_188);
  Map_AnimateCells(gVale_SanctumDoorOpens, 45, 11);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 257, 420);
  Engine_EventRequestExit(VALE_TO_SANCTUM);
}

/// The path to Mt. Aleph in Vale (touch cell 2): Jenna stops Isaac until
/// Kraden has planned the visit (FLAG_SANCTUM_VISIT_PLANNED); then
/// VALE_TO_SOL_ENTRANCE leads up to Sol Entrance. Jenna says "Kraden is waiting
/// for us".
static void Event_ApproachMtAleph(void) {
  if (Engine_GameFlagIsSet(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
    Vale_HaveJennaStopTheAlephDetour();
  } else {
    Engine_AudioPlayCue(SE_MAP_EXIT);
    Engine_EventRequestExit(VALE_TO_SOL_ENTRANCE);
  }
}

/// Touch cell 3 in Vale takes the northern way out to SceneId_ValeKraden.
static void Event_TakeExitToValeKradenNorth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(VALE_TO_VALE_KRADEN_NORTH);
}

/// Touch cell 4 in Vale takes the southern way out to SceneId_ValeKraden.
static void Event_TakeExitToValeKradenSouth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(VALE_TO_VALE_KRADEN_SOUTH);
}

/// Touch cell 5 in Vale takes the way out to SceneId_ValeGate.
static void Event_TakeExitToValeGate(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(VALE_TO_VALE_GATE);
}

/// The door of the house of Garet's family in Vale (touch cell 6): it opens and
/// VALE_TO_VALE_INTERIORS_SHOPS_EAST leads in.
static void Event_EnterGaretsHouse(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gVale_IsaacsDoorOpens, 54, 32);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 406, 727);
  Engine_EventRequestExit(VALE_TO_VALE_INTERIORS_SHOPS_EAST);
}

/// A house door in Vale (touch cell 7): it opens and
/// VALE_TO_VALE_INTERIORS_SHOPS_SOUTH leads in, to the room ValeInteriorsShops
/// enters from VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH.
static void Event_EnterValeInteriorsShops(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gVale_GrandparentsDoorOpens, 45, 39);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 262, 805);
  Engine_EventRequestExit(VALE_TO_VALE_INTERIORS_SHOPS_SOUTH);
}

/// The door of Isaac's house in Vale (touch cell 8): after the party has left
/// and before Kalay, Dora will not open. "My Isaac would keep his promise...
/// Go away." Otherwise VALE_TO_VALE_INTERIORS_INN leads in.
static void Event_EnterIsaacsHouse(void) {
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0 &&
      Engine_GameFlagIsSet(FLAG_REACHED_KALAY) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgValeYouCantBeIsaac);
    Engine_EventOpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 60);
      Engine_EventShowMessage(ACTOR_DORA, 0);
    } else {
      u16 *message = &gEventWork->message;

      *message = *message + 2;
      Engine_EventWait(40);
      Engine_EventShowMessage(ACTOR_DORA, 0);
    }
    Engine_EventEnd();
  } else {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gVale_IsaacsDoorOpens, 50, 44);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 340, 888);
    Engine_EventRequestExit(VALE_TO_VALE_INTERIORS_INN);
  }
}

/// The door of the house of Jenna's grandparents in Vale (touch cell 9): it
/// opens and VALE_TO_VALE_INTERIORS_SHOPS_NORTH leads in.
static void Event_EnterJennasGrandparentsHouse(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gVale_GrandparentsDoorOpens, 49, 69);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 326, 1126);
  Engine_EventRequestExit(VALE_TO_VALE_INTERIORS_SHOPS_NORTH);
}

/// A house door in Vale (touch cell 10): it opens and
/// VALE_TO_VALE_INTERIORS_SHOPS_WEST leads in, to the room ValeInteriorsShops
/// enters from VALE_INTERIORS_SHOPS_FROM_VALE_WEST.
static void Event_EnterValeInteriorsShopsWest(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gVale_WestHouseDoorOpens, 52, 76);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 1238);
  Engine_EventRequestExit(VALE_TO_VALE_INTERIORS_SHOPS_WEST);
}

/// Unused: its row answers cell code 11, which no cell of Vale's map
/// (map/map_000, the stormy night's too) carries, as
/// Event_UnusedLeaveToValeInteriorsShopsWestBySecondDoor in vale_storm.c. It
/// would have opened Isaac's door, walked the leader to the spot before it and
/// taken VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR into the house of
/// Jenna's grandparents, whose way out returns by the other door.
static void Event_UnusedEnterJennasGrandparentsHouseBySecondDoor(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gVale_IsaacsDoorOpens, 35, 74);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Engine_EventRequestExit(VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR);
}

/// On the path to Mt. Aleph in Vale, before the visit is planned: Jenna comes
/// out to remind Isaac that Kraden is waiting, and he turns back.
static void Vale_HaveJennaStopTheAlephDetour(void) {
  struct FieldActor *leader;
  struct FieldActor *jenna;
  struct FieldActor *actor;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  jenna = Object_GetById(JENNA);
  Engine_EventBegin();
  jenna->x.fixed = leader->x.fixed;
  jenna->y.fixed = leader->y.fixed;
  jenna->z.fixed = leader->z.fixed;
  jenna->targetX = ACTOR_NO_TARGET;
  jenna->targetY = ACTOR_NO_TARGET;
  jenna->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(jenna, 0, 0, 0);
  jenna->terrainHeight = leader->y.fixed;
  Engine_TaskWait(1);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(JENNA, 110, 283);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 2);
  Engine_EventSetMessage(MsgValeKradenIsWaitingForUs);
  if (leader->x.fixed < jenna->x.fixed) {
    Event_ShowMessageAndWait(
        SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | JENNA, 0, 2);
  } else {
    Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | JENNA, 0, 2);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(2);
  Engine_ActorSetAnimation(JENNA, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(JENNA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(JENNA);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 110, 303);
  Engine_EventEnd();
}

/// The puppies in Vale (touch cell 20), until the party has met Saturos and
/// Menardi: they bark, the camera pans down to them and back, and the leader
/// turns away.
static void Event_WatchThePuppies(void) {
  struct MapScrollWork *map;
  s32 *savedOrigin;
  struct FieldActor *leader;
  s32 frame;
  s32 cameraPosition[3];

  if (Engine_GameFlagIsSet(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    map = gMapWork[0];
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_EventWait(2);
    Engine_EventSetMessage(MsgValeRrruffRrrruff);
    Engine_EventShowMessageAndWait(ACTOR_FIRST_PUPPY, 0, 2);
    Engine_EventShowMessageAndWait(ACTOR_FIRST_PUPPY + 1, 0, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = map->origin;
    map->origin = cameraPosition;
    frame = 0;
    // Pan down to the puppies while the leader stays in place.
    do {
      cameraPosition[2] += PIXELS(2);
      Engine_EventWait(1);
      frame++;
      Engine_MapRedraw();
    } while (frame != 40);
    Engine_EventWait(60);
    Engine_MessageShowCentered(MsgValePuppiesPlayingOver, 1);
    frame = 0;
    Engine_EventWait(6);
    do {
      cameraPosition[2] -= PIXELS(2);
      Engine_EventWait(1);
      frame++;
      Engine_MapRedraw();
    } while (frame != 40);
    // Return camera ownership to the normal follow target.
    map->origin = savedOrigin;
    Engine_EventWait(60);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 70, 741);
    Engine_EventEnd();
  }
}

/// The puppies in Vale from the other side (touch cell 21): the same barking,
/// without the camera pan.
static void Event_TurnBackFromThePuppies(void) {
  s32 msg;

  if (GameFlag_IsSet(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    msg = MsgValeRrruffRrrruff;
    Engine_EventSetMessage(msg);
    Event_ShowMessageAndWait(ACTOR_FIRST_PUPPY, 0, 2);
    Event_ShowMessageAndWait(ACTOR_FIRST_PUPPY + 1, 0, 2);
    Engine_MessageShowCentered(msg + 2, 1);
    Engine_EventWait(6);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 870);
    Engine_EventEnd();
  }
}

/// Touch cell 100 in Vale: the passage's collision changes and the leader
/// moves through it from its northern end (FLAG_SCENE_TEMP_04 set).
static void Event_EnterNorthernPassage(void) {
  Engine_EventBegin();
  Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Vale_MoveActorAlongLowerZPath(ACTOR_PARTY_LEADER, 10, 11, 1);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_04);
  Engine_EventEnd();
}

/// Touch cell 101 in Vale: the leader moves back through the passage, and its
/// collision is restored (FLAG_SCENE_TEMP_04 cleared).
static void Event_ReturnThroughSouthernPassage(void) {
  Engine_EventBegin();
  Vale_MoveActorAlongHigherZPath(ACTOR_PARTY_LEADER, 13, 10, 1);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_04);
  Map_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  Engine_EventEnd();
}

/// A bearded man in a long robe in Vale (ACTOR_MAN_GUARDING_MT_ALEPH, touch
/// cell 50) jumps out in front of Isaac, then keeps watching him; sets
/// FLAG_VALE_WARNED_AGAINST_MT_ALEPH. "You're not planning on sneaking up to
/// Mt. Aleph...?"
static void Event_WarnAgainstSneakingUpMtAleph(void) {
  struct FieldActor *guard;
  struct FieldActor *actor;

  guard = Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Engine_ActorJump(ACTOR_PARTY_LEADER, 5, 0);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 215, 403);
  guard->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetPosition(ACTOR_MAN_GUARDING_MT_ALEPH, PIXELS(166), PIXELS(375));
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTHEAST, 20);
  guard->steeringFlags = (guard->steeringFlags ^ 1);
  Actor_SetSpeed(ACTOR_MAN_GUARDING_MT_ALEPH, PIXELS(5) / 2, PIXELS(5) / 2);
  Engine_ActorJump(ACTOR_MAN_GUARDING_MT_ALEPH, 4, 0);
  Actor_WalkToAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, 202, 395);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH,
                      FACING_SOUTHEAST + FACING_STEP, 24);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_MAN_GUARDING_MT_ALEPH, PIXELS(3) / 2, PIXELS(1));
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_FirstWarningLeaderPath);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_SetActionScript(ACTOR_MAN_GUARDING_MT_ALEPH,
                        gVale_FirstWarningGuardPath);
  Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 474);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Object_RefreshSelectorById(ACTOR_MAN_GUARDING_MT_ALEPH);
  Actor_WalkToAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, 256, 456);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTH, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgValeNotSneakingUpMtAleph);
  Event_AskYesNo(ACTOR_MAN_GUARDING_MT_ALEPH, 0);
  actor = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
#ifdef UBFIX
  actor->update = Vale_HaveGuardWatchTheLeader;
#else
  actor->update = (void (*)(union FieldObject *))Vale_HaveGuardWatchTheLeader;
#endif
  Actor_SetActionScript(ACTOR_MAN_GUARDING_MT_ALEPH, gVale_AlephGuardPatrol);
  GameFlag_Set(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
  Engine_EventEnd();
}

/// Repeats the warning when the man reaches the first pause.
static void Event_RepeatMtAlephWarningFromFirstApproach(void) {
  struct FieldActor *guard = Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (guard->timer == 1) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderFromWestPath,
                                   gVale_GuardFromFirstPausePath);
    }
  }
}

/// Asks about the Aleph detour, then resumes each actor's response actions.
static void Vale_AskAboutSneakingToAleph(const s32 *leaderActions,
                                         const s32 *guardActions) {
  const s32 *replyActions;
  struct FieldActor *guard;
  struct FieldActor *actor;

  replyActions = guardActions;
  guard = Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  Engine_EventBegin();
  Engine_ActorStartRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, leaderActions);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_MAN_GUARDING_MT_ALEPH,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_RunActionScriptAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, replyActions);
  Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
  guard->scaleX = Q16_ONE;
  guard->scaleY = Q16_ONE;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Engine_EventSetMessage(MsgValeNotSneakingUpMtAleph);
  Event_AskYesNo(ACTOR_MAN_GUARDING_MT_ALEPH, 0);
  actor = Actor_Get(ACTOR_MAN_GUARDING_MT_ALEPH);
#ifdef UBFIX
  actor->update = Vale_HaveGuardWatchTheLeader;
#else
  actor->update = (void (*)(union FieldObject *))Vale_HaveGuardWatchTheLeader;
#endif
  Actor_SetActionScript(ACTOR_MAN_GUARDING_MT_ALEPH, gVale_AlephGuardPatrol);
  Engine_EventEnd();
}

/// Repeats the warning when the man reaches the second pause.
static void Event_RepeatMtAlephWarningFromSecondApproach(void) {
  struct FieldActor *guard = Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
  if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (guard->timer == 2) {
      Vale_AskAboutSneakingToAleph(gVale_LeaderPushedBackPath,
                                   gVale_GuardFromSecondPausePath);
    }
  }
}

/// Chooses the warning approach matching the man's current pause.
static void Event_RepeatMtAlephWarningAtEitherPause(void) {
  struct FieldActor *guard = Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH);
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

/// Unused: another cut of Event_RoofMended, the end of the morning the story
/// begins.
///
/// No table names it. It plays the same lines as the scene the game uses,
/// `Good job, Garet.` and `I was devastated when Kyle died...` among them, but
/// frames them with its own camera and map cells, and sets FLAG_SCENE_TEMP_02
/// as that scene does.
UNUSED static void Event_UnusedRoofMended(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  u8 leaderMotionFlags;
  u8 *leaderMotion;
  s32 i;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  actor = Engine_EventGetViewCenter();
  actor->motionFlags = 0;
  Camera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
  Engine_EventWait(1);
  Engine_MapRedraw();
  Map_CopyCellAttributes(49, 41, 7, 3, 20, 50);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leaderMotion = &leader->motionFlags;
  leaderMotionFlags = *leaderMotion;
  *leaderMotion = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(407), PIXELS(690));
  Actor_SetPosition(ACTOR_DORA, PIXELS(392), PIXELS(896));
  Actor_SetPosition(GARET, PIXELS(298), PIXELS(736));
  Actor_SetPosition(JENNA, PIXELS(298), PIXELS(760));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Vale_PlaceOrHideBoards(ACTOR_FIRST_BOARD, 2, 1);
  gEventWork->startTransition = 0;
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetActionScript(JENNA, gVale_JennaWalksToTheHouse);
  Actor_SetActionScript(GARET, gVale_GaretWalksToTheHouse);

  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 404, 843);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
  Actor_SetChildValue(ACTOR_FIRST_BOARD, 2);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 402, 828);
  Engine_EventWait(80);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_FIRST_BOARD, 0);
  Actor_SetPosition(ACTOR_FIRST_BOARD, PIXELS(390), PIXELS(842));
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Engine_EventWait(200);
  Map_CopyCellsTo(7, 102, 84, 41, 2, 1);

  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
  Actor_SetChildValue(ACTOR_FIRST_BOARD, 2);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 377, 828);
  Engine_EventWait(80);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_FIRST_BOARD, 0);
  Actor_SetPosition(ACTOR_FIRST_BOARD, 0, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Engine_EventWait(200);
  Map_CopyCellsTo(6, 102, 83, 41, 1, 1);

  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
  Actor_SetChildValue(ACTOR_SECOND_BOARD, 2);
  Actor_SetSpeed(ACTOR_SECOND_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 390, 832);
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 360, 842);
  Engine_EventWait(80);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_SECOND_BOARD, 0);
  Actor_SetPosition(ACTOR_SECOND_BOARD, 0, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Engine_EventWait(200);
  Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;

  Engine_EventSetMessage(MsgValeGoodJob);
  Engine_ActorJump(ACTOR_DORA, 2, 20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 20);
  Vale_MoveActorAlongLowerZPath(ACTOR_DORA, 5, 6, 0);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    gEventWork->message++;
  }
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_EventSetMessage(MsgValeWorkingYourselvesBone);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    gEventWork->message++;
  }
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgValeDevastatedWhenKyle);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);

  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(GARET, 369, 904);
  Actor_WalkToAndWait(JENNA, 392, 904);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Vale_MoveActorAlongLowerZPath(JENNA, 10, 11, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 30);
  Engine_ActorJump(JENNA, 4, 0);
  Actor_WalkToAndWait(JENNA, 392, 843);
  Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Vale_MoveActorAlongLowerZPath(GARET, 10, 11, 0);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetSpeed(GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkTo(GARET, 392, 843);
  Actor_Get(JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(JENNA, 408, 843);
  Engine_EventWait(1);
  Actor_Get(JENNA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorJump(ACTOR_DORA, 4, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(100);
  Engine_ActorFaceEachOther(JENNA, GARET, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
  Engine_ActorFaceEachOther(JENNA, GARET, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(30);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    gEventWork->message++;
  }
  Engine_EventWait(40);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);

  Engine_EventSetMessage(MsgValeAsStubbornAsYourFather);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorJump(ACTOR_DORA, 4, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_DORA_HANDS_UP);
  Engine_EventWait(5);
  Event_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, GARET, 10, 14, 4, 14,
                               0);
  actor = Actor_Get(ACTOR_DORA);
  actor->sprite->flags = 0;
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
  Engine_EventWait(4);
  for (i = 0; i != 4; i++) {
    actor->z.fixed += PIXELS(3) / 2;
    actor->scaleY -= 0x1999;
    Engine_EventWait(1);
  }
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetSpeed(GARET, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorJump(GARET, 6, 0);
  Actor_WalkToAndWait(GARET, 374, 827);
  Event_ShowMessage(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Engine_ActorSetAnimation(GARET, ANIM_GARET_STARTLED);
  Engine_ActorJump(GARET, 2, 5);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(GARET, 3);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_DORA_PEEK);
  actor->scaleY = 0x8000;
  Actor_SetPosition(ACTOR_DORA, PIXELS(364), PIXELS(811));
  for (i = 0; i != 5; i++) {
    actor->scaleY += 0x1999;
    Engine_EventWait(1);
  }
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Camera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  Camera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorJump(ACTOR_DORA, 6, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(30);
  actor = Actor_Get(ACTOR_DORA);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 1);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_SetSpeed(GARET, PIXELS(4), PIXELS(2));
  actor = Actor_Get(GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(GARET, 403, 827);
  Actor_SetAttachedEffect(JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(JENNA, FACING_NORTH, 20);
  Event_ShowMessageAndWait(JENNA, 0, 1);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorSetAnimation(GARET, ANIM_GARET_STARTLED);
  Engine_ActorJump(GARET, 2, 5);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(JENNA, 408, 855);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorStartRepeatedMotion(JENNA, 3);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_SOUTH, 30);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 70);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_WEST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Engine_ActorJump(GARET, 4, 0);
  Actor_WalkToAndWait(GARET, 398, 828);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(60);

  actor = Actor_Get(GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(JENNA);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(JENNA, actor->x.part.pixel + 16, actor->z.part.pixel);
  Actor_WalkToAndWait(GARET, actor->x.part.pixel + 16,
                      actor->z.part.pixel - 16);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  Actor_WalkToAndWait(JENNA, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_WalkToAndWait(GARET, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(GARET, 0, 0);
  Engine_PartyAddMembers(GARET, JENNA);
  Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
  Vale_MoveActorAlongHigherZPath(ACTOR_PARTY_LEADER, 13, 10, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_Get(ACTOR_DORA)->steeringFlags |= ACTOR_FACES_MOTION;
  Vale_MoveActorAlongHigherZPath(ACTOR_DORA, 6, 5, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(100);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  *leaderMotion = leaderMotionFlags;
  Engine_EventEnd();
}

/// Vale's setup: plays the roof mending (VALE_FROM_TITLE) or its end (entrance
/// 17, once), floats the Nut, places the pillar by its column, sets the
/// watching man going, removes the puppies once Saturos and Menardi have
/// been met, and after a battle restores the passage's and the upper path's
/// cells.
s32 Vale_Initialize(void) {
  struct FieldActor *actor;
  s32 set;

  if (gGameState.entrance == VALE_FROM_TITLE) {
    PaletteGlow_Update(((u8 *)&gGameState)[517], ((u8 *)&gGameState)[518]);
    Event_MendingTheRoof();
  } else {
    if (!Engine_GameFlagIsSet(FLAG_VALE_NUT_CAUGHT)) {
      if (!Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
        Scene_FloatNut(ACTOR_NUT);
      } else {
        Scene_FloatNut(ACTOR_NUT_AFTER_KALAY);
      }
    }
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    actor = Object_GetById((Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) +
                           ACTOR_PILLAR);
    Engine_ActorSetSpriteFlags(actor, 0);
    if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_14)) {
      actor->x.fixed = 181 << 17;
    } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_16)) {
      actor->x.fixed = 197 << 17;
    } else {
      actor->x.fixed = 189 << 17;
    }
    actor->z.fixed = PIXELS(584);
    actor->y.fixed = PIXELS(192);
    Event_SettleMovedPillar();
    actor->terrainId = 3;
    actor->motionFlags = 0;
    FieldScheduler_AddCallback(Vale_AdjustPillarPriorityForLeaderHeight,
                               TASK_PRIORITY_SCENE);
#if TBS_PATCH_4
    if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04)) {
          Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_10)) {
          Event_SetPathCellOverride();
        }
      }
    } else {
#else
    if (!Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
#endif
      if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE)) {
        actor = Object_GetById(ACTOR_DORA);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_DORA), 0);
        actor->scaleX = 0x28f;
        actor->scaleY = 0x28f;
      }
      if (Engine_GameFlagIsSet(FLAG_MET_SATUROS_AND_MENARDI)) {
        Engine_ActorSetPosition(ACTOR_FIRST_PUPPY, 0, 0);
        Engine_ActorSetPosition(ACTOR_FIRST_PUPPY + 1, 0, 0);
        Engine_ActorSetPosition(ACTOR_FIRST_PUPPY + 2, 0, 0);
      }
      set = Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE);
      if (set == 0) {
        if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
          if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH)) {
            Actor_SetPosition(ACTOR_MAN_GUARDING_MT_ALEPH, PIXELS(256),
                              PIXELS(456));
#ifdef UBFIX
            Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
                Vale_HaveGuardWatchTheLeader;
#else
            Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
                (void *)Vale_HaveGuardWatchTheLeader;
#endif
            Actor_SetActionScript(ACTOR_MAN_GUARDING_MT_ALEPH,
                                  gVale_AlephGuardPatrol);
          }
        } else {
          Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = set;
          Engine_GameFlagClear(FLAG_SCENE_TEMP_41);
        }
        if (gGameState.entrance != VALE_FROM_TITLE &&
            !Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
          FieldScheduler_AddCallback(Vale_SynchronizeAlephGuardPause,
                                     TASK_PRIORITY_SCENE);
        }
      }
      if (!GameFlag_IsSet(FLAG_PLACE_TEMP_08) &&
          gGameState.entrance == VALE_FROM_ROOF_MENDED) {
        Event_RoofMended();
        Engine_GameFlagSet(FLAG_PLACE_TEMP_08);
      }
#if TBS_PATCH_4
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04)) {
          Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_10)) {
          Event_SetPathCellOverride();
        }
      }
      {
        s32 priority;
        s32 actor;

        priority = ((u32)(u8)set >> 8) + 2;
        actor = ACTOR_DORA;
        Engine_ActorSetSpritePriority(actor, priority);
      }
#endif
    }
#if !TBS_PATCH_4
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04)) {
        Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
      }
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_10)) {
        Event_SetPathCellOverride();
      }
    }
#endif
    FieldFx_SetQueuedSoundAndPlay(SE_170);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
  return 0;
}

/// Back from Kalay, Garet meets his sister in Vale; sets group flag
/// FLAG_PLACE_TEMP_03. "Ah! It's been a while... You're back?" ("Sis...").
static void Event_GaretMeetsHisSister(void) {
  struct FieldActor *record;
  s32 msg;

  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_GARETS_SISTER_AFTER_KALAY, ACTOR_PARTY_LEADER, 20);
  msg = MsgValeAh;
  Engine_EventSetMessage(msg);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 2);
  Event_ShowMessageAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(398), -1, PIXELS(582), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 420, 608);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY,
                      FACING_SOUTHEAST + FACING_STEP, 0);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 402, 608);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | GARET, 0);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY,
                      FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 2);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    gEventWork->message++;
    Engine_ActorStartRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 1);
  }
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 40);
  Actor_ShowEmote(ACTOR_GARETS_SISTER_AFTER_KALAY,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_EventSetMessage(msg + 6);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 1);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY,
                      FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 120);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY,
                                  ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_GARETS_SISTER_AFTER_KALAY,
                      FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  GameFlag_Set(FLAG_PLACE_TEMP_03);
  Engine_EventEnd();
}

/// Back from Kalay, Garet asks one of his family in Vale
/// (ACTOR_GARETS_BROTHER_AFTER_KALAY) to protect it while he is gone; sets
/// group flag FLAG_PLACE_TEMP_04. "WHATEVER YOU DO, you must protect the family
/// while I'm gone."
static void Event_GaretAsksToProtectFamily(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Camera_MoveTo(PIXELS(357), -1, PIXELS(738), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 367, 745);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 346, 745);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Engine_EventSetMessage(MsgValeHowHaveYouBeen);
  Event_ShowMessage(GARET, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 2);
  Actor_ShowEmote(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                      FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                      FACING_SOUTH + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                      FACING_SOUTHEAST + FACING_STEP, 40);
  Event_ShowMessageAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 1);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                      FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY,
                                  ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 2);
  Event_ShowMessageAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 20);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  } else {
    gEventWork->message++;
  }
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 10);
  Engine_EventSetMessage(MsgValeEverProtectFamily);
  Event_ShowMessage(GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  if (record != 0) {
    Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  GameFlag_Set(FLAG_PLACE_TEMP_04);
  Engine_EventEnd();
}

/// Unused: water bursts up by the river after a tremor. Back from Kalay, a
/// tremor shakes Vale and water bursts up three times north of the strongman by
/// the river who remembers Felix.
///
/// No table names it. He would have started, sniffled through
/// `I still remember when Felix and I used to play here...*Sniff*`, turned to
/// the leader to say that Felix must be protecting Jenna, and set group flag
/// FLAG_PLACE_TEMP_05. His talk and Mind Read rows show the same two lines.
UNUSED static void Event_UnusedWaterBurstsByTheRiver(void) {
  struct FieldActor *actor;
  const s32 *actions;

  Engine_EventBegin();
  actor = Actor_Get(ACTOR_FIRST_WATER);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_FIRST_WATER + 1);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_FIRST_WATER + 2);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_WATER, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_WATER + 1, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_WATER + 2, 0);
  Engine_TaskWait(20);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  actions = gVale_UnusedWaterGrows;
  Actor_SetActionScript(ACTOR_FIRST_WATER, actions);
  Engine_TaskWait(10);
  Actor_SetActionScript(ACTOR_FIRST_WATER + 1, actions);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_TaskWait(20);
  Actor_RunActionScriptAndWait(ACTOR_FIRST_WATER + 2, actions);
  Actor_ShowEmote(ACTOR_STRONGMAN_REMEMBERING_FELIX,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_STRONGMAN_REMEMBERING_FELIX, 2);
  Actor_FaceDirection(ACTOR_STRONGMAN_REMEMBERING_FELIX,
                      FACING_NORTH + FACING_STEP, 10);
  Engine_EventSetMessage(MsgValeIUsedToPlayHere);
  Event_ShowMessageAndWait(ACTOR_STRONGMAN_REMEMBERING_FELIX, 0, 40);
  Actor_FaceActor(ACTOR_STRONGMAN_REMEMBERING_FELIX, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(ACTOR_STRONGMAN_REMEMBERING_FELIX, 0);
  Actor_FaceDirection(ACTOR_STRONGMAN_REMEMBERING_FELIX, FACING_WEST, 10);
  GameFlag_Set(FLAG_PLACE_TEMP_05);
  Engine_EventEnd();
}

/// Traverses the passage toward lower Z, changing animations at its lip.
static void Vale_MoveActorAlongLowerZPath(s32 actorId, s32 approachAnimationId,
                                          s32 departureAnimationId,
                                          s32 resumeMotion) {
  s32 departureAnimation;
  s32 approachAnimation;
  s32 restoreMotion;
  struct FieldActor *actor;
  struct AnimationObject *sprite;

  restoreMotion = resumeMotion;
  approachAnimation = approachAnimationId;
  departureAnimation = departureAnimationId;
  actor = Object_GetById(actorId);
  sprite = actor->sprite;
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(actorId, 392, 886);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  actor->motionFlags = 0;
  sprite->flags = 0;
  Engine_ActorSetAnimation(actorId, approachAnimation);
  Actor_SetSpeed(actorId, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(actorId, 392, 875);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(actorId, departureAnimation);
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_MoveToAndWait(actorId, 392, 859);
  sprite->flags = 1;
  if (restoreMotion != 0) {
    actor->motionFlags = 3;
  }
  Engine_EventWait(10);
  Engine_ActorSetAnimation(actorId, ANIM_STAND);
}

/// Traverses the passage toward higher Z and restores falling motion if
/// requested.
static void Vale_MoveActorAlongHigherZPath(s32 actorId, s32 approachAnimationId,
                                           s32 departureAnimationId,
                                           s32 resumeMotion) {
  s32 departureAnimation;
  s32 approachAnimation;
  s32 restoreMotion;
  struct FieldActor *actor;
  struct AnimationObject *sprite;

  restoreMotion = resumeMotion;
  approachAnimation = approachAnimationId;
  departureAnimation = departureAnimationId;
  actor = Object_GetById(actorId);
  sprite = actor->sprite;
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(actorId, 392, 859);
  Actor_FaceDirection(actorId, FACING_NORTH, 10);
  actor->motionFlags = 0;
  sprite->flags = 0;
  Engine_ActorSetAnimation(actorId, approachAnimation);
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_MoveToAndWait(actorId, 392, 875);
  Engine_EventWait(10);
  Actor_SetSpeed(actorId, PIXELS(3) / 10, PIXELS(3) / 20);
  Engine_ActorSetAnimation(actorId, departureAnimation);
  Actor_MoveToAndWait(actorId, 392, 890);
  actor->velocityY = 0x20000;
  sprite->flags = 1;
  if (restoreMotion != 0) {
    actor->motionFlags = 3;
  }
  Engine_ActorSetAnimation(actorId, ANIM_STAND);
}

/// Mode 0 sets count boards from first on the roof by the hole, still and
/// without shadows; any other mode puts them out of sight.
static void Vale_PlaceOrHideBoards(s32 first, u32 count, s32 mode) {
  s32 selector = first;
  u32 i;

  if (mode == 0) {
    for (i = 0; i < count; i++) {
      struct FieldActor *actor = Object_GetById(selector);

      actor->motionFlags = 0;
      Engine_ActorSetSpriteFlags(actor, 0);
      actor->x.fixed = PIXELS(390);
      actor->y.fixed = PIXELS(160);
      actor->z.fixed = PIXELS(842);
      selector++;
    }
    return;
  }

  for (i = 0; i < count; i++) {
    Engine_ActorSetPosition(selector, 0, 0);
    selector++;
  }
}

/// Arriving by VALE_FROM_TITLE, the game's opening in Vale: Isaac is mending
/// the roof of his house, Garet trains his Psynergy, and Jenna comes by; the
/// two speak of the storm three years ago before going to fetch Isaac. "Three
/// years later... Isaac! Haven't you finished mending the roof yet?"
static void Event_MendingTheRoof(void) {
  u32 i;
  /* FAKEMATCH(dead): repairActor's zero initializer, never read, keeps the
   * scene's register allocation; without it, 59 of 1024 instructions differ
   * in tbs-en. */
  struct FieldActor *repairActor = NULL;
  struct FieldActor *leader;
  struct FieldActor *moveHand;
  struct FieldActor *actor;
  /* FAKEMATCH(temporary): one zero for the hand's flags and update and
   * the message call; as literal zeros, 7 of 1024 instructions differ in
   * tbs-en. */
  s32 stoppedMotionFlags;
  s32 message;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  moveHand = Object_GetById(ACTOR_OPENING_MOVE_HAND);

  // Prepare the damaged house and start Isaac's hammering behind the caption.
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  ((struct FieldActor *)Engine_EventGetViewCenter())->motionFlags = 0;
  Engine_TaskWait(1);
  Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
  repairActor = Object_GetById(ACTOR_OPENING_FIRST_BOARD);
  repairActor->motionFlags = 0;
  repairActor->x.fixed = PIXELS(388);
  repairActor->y.fixed = PIXELS(160);
  repairActor->z.fixed = PIXELS(840);
  Engine_ActorSetSpriteFlags(repairActor, 0);
  repairActor = Object_GetById(ACTOR_OPENING_SECOND_BOARD);
  repairActor->motionFlags = 0;
  repairActor->y.fixed = PIXELS(160);
  repairActor->z.fixed = PIXELS(844);
  repairActor->x.fixed = PIXELS(388);
  Engine_ActorSetSpriteFlags(repairActor, 0);
  repairActor = Object_GetById(ACTOR_OPENING_THIRD_BOARD);
  repairActor->motionFlags = 0;
  repairActor->z.fixed = PIXELS(848);
  repairActor->y.fixed = PIXELS(160);
  repairActor->x.fixed = PIXELS(388);
  Engine_ActorSetSpriteFlags(repairActor, 0);
  actor = Object_GetById(ACTOR_OPENING_FIRST_BOARD);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_OPENING_SECOND_BOARD);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  ObjectSystem_SuspendProjectionAndShowSprites();
  message = MsgValeRepairCaption;
  UiText_ShowCenteredMessage(message, 0, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();

  // Follow Jenna through the village until she notices the repair work.
  Camera_MoveTo(PIXELS(339), PIXELS(160), PIXELS(1173), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Camera_SetSpeed(PIXELS(33) / 100, PIXELS(33) / 800);
  Camera_MoveTo(PIXELS(296), PIXELS(160), PIXELS(921), 1);
  Actor_SetPosition(JENNA, PIXELS(409), PIXELS(1134));
  Engine_TaskWait(1);
  Actor_SetSpeed(JENNA, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_WalkTo(JENNA, 420, 1068);
  Audio_PlayCueFromEventWork();
  gEventWork->transitionFrames = 60;
  Engine_EventOpenScreen();
  Engine_ActorWaitForMove(JENNA);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(JENNA, 341, 1064);
  Actor_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(JENNA, 359, 1033);
  Actor_SetSpeed(ACTOR_OPENING_WOMAN, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_OPENING_WOMAN, 318, 947);
  Engine_ActorSetAnimation(ACTOR_OPENING_WOMAN, ANIM_WALK);
  Actor_WalkToAndWait(JENNA, 412, 1033);
  Actor_WalkToAndWait(JENNA, 412, 1019);
  Actor_WalkToAndWait(JENNA, 374, 1008);
  Actor_WalkToAndWait(JENNA, 347, 955);
  Actor_WalkToAndWait(ACTOR_OPENING_WOMAN, 318, 947);
  Engine_ActorFaceEachOther(JENNA, ACTOR_OPENING_WOMAN, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_OPENING_WOMAN, 2);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Actor_WalkTo(ACTOR_OPENING_WOMAN, 379, 1017);
  Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Actor_WalkToAndWait(JENNA, 333, 920);
  Actor_WalkToAndWait(JENNA, 299, 924);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST + FACING_STEP, 30);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(387), PIXELS(160), PIXELS(866), 1);
  Engine_CameraWaitForMove();
  Engine_ActorJump(ACTOR_OPENING_DORA, 2, 20);
  Engine_EventSetMessage(message + 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_OPENING_DORA, 0, 10);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  Engine_ActorFaceEachOther(ACTOR_OPENING_DORA, ACTOR_PARTY_LEADER, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_OPENING_DORA, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_OPENING_DORA, 0, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Engine_CameraFollowActor(JENNA, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Actor_FaceDirection(JENNA, FACING_NORTH + FACING_STEP, 10);
  Actor_WalkToAndWait(JENNA, 312, 759);
  Actor_WalkToAndWait(JENNA, 361, 760);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_WEST, 40);
  Actor_FaceDirection(JENNA, FACING_EAST, 40);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | GARET, 0, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorJump(JENNA, 4, 40);
  Actor_FaceDirection(JENNA, FACING_NORTH, 30);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(JENNA, 0, 30);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(396), -1, PIXELS(588), 1);
  Actor_WalkToAndWait(JENNA, 456, 739);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);

  // Garet trains his Psynergy: the hand of Move pushes the pillar, shedding
  // sparkles.
  Scene_BeginCutscenePsynergy();
  Engine_ActorSetAnimation(GARET, ANIM_CAST_PSYNERGY);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | GARET, 0, 20);
  Event_PlayCue(SE_131);
  for (i = 0; i < 60; i++) {
    Vale_FlashActorAndSpawnEffects(Object_GetById(GARET));
    Engine_TaskWait(1);
  }
  Engine_ActorSetSpritePriority(GARET, 1);
  Task_AddCallback(Vale_UpdateGaretPaletteAndEffects, TASK_PRIORITY_SCENE);
  Task_AddCallback(Vale_CycleOpeningPillarPalette, TASK_PRIORITY_SCENE);
  actor = Object_GetById(ACTOR_OPENING_MOVE_HAND);
  Engine_ActorSetSpriteFlags(actor, 0);
  stoppedMotionFlags = 0;
  moveHand->motionFlags = stoppedMotionFlags;
  moveHand->x.fixed = PIXELS(428);
  moveHand->y.fixed = PIXELS(208);
  moveHand->z.fixed = PIXELS(584);
  moveHand->facing = FACING_WEST;
  moveHand->update = Vale_TrailSparkles;
  Engine_EventWait(4);
  Actor_SetSpeed(ACTOR_OPENING_MOVE_HAND, PIXELS(2), PIXELS(2));
  FieldObject_SetPosition(moveHand, PIXELS(408), PIXELS(208), PIXELS(584));
  Engine_EventWait(40);
  Actor_SetSpeed(ACTOR_OPENING_PILLAR, PIXELS(3) / 20, PIXELS(3) / 40);
  Actor_SetSpeed(ACTOR_OPENING_MOVE_HAND, PIXELS(3) / 20, PIXELS(3) / 40);
  Object_GetById(ACTOR_OPENING_PILLAR);
  Engine_ObjectSetPosition(moveHand, PIXELS(392), PIXELS(208), PIXELS(584));
  Actor_MoveToAndWait(ACTOR_OPENING_PILLAR, 378, 584);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA, 0, 10);
  moveHand->update = (void (*)(union FieldObject *))stoppedMotionFlags;
  Engine_ActorSetSpritePriority(GARET, 2);
  Actor_Get(GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Scheduler_RemoveCallback(Vale_UpdateGaretPaletteAndEffects);
  Scheduler_RemoveCallback(Vale_CycleOpeningPillarPalette);
  Engine_TaskWait(1);
  Actor_SetChildValue(GARET, 0);
  Actor_SetChildValue(ACTOR_OPENING_PILLAR, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Vale_VanishMoveHand(moveHand);
  Scene_EndCutscenePsynergy();

  // Finish the reunion conversation after the repair effects settle.
  Engine_EventWait(10);
  Actor_WalkToAndWait(JENNA, 424, 624);
  Engine_ActorFaceEachOther(GARET, JENNA, 60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Actor_WalkToAndWait(JENNA, 388, 604);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 40);
  Event_ShowTwoMessagesAndWait(GARET, 1, 2, 25, 2, JENNA, 10, 14, 4, 14,
                               stoppedMotionFlags);
  Engine_EventWait(40);
  Actor_SetAttachedEffect(JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 10);
  Actor_FaceDirection(JENNA, FACING_EAST + FACING_STEP, 40);
  Actor_FaceActor(JENNA, GARET, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Actor_WalkToAndWait(JENNA, 380, 620);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 10);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 412, 604);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ActorJump(JENNA, 4, 30);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | GARET, 0, 20);
  Engine_EventWait(30);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 40);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(JENNA, 428, 628);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorWaitForMove(JENNA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | GARET, 0);
  Engine_ActorSetAnimation(JENNA, ANIM_STAND);
  Engine_EventWait(60);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_WalkToAndWait(JENNA, 428, 628);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA, 0, 20);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | GARET,
                           0, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(JENNA, 450, 750);
  Actor_WalkTo(GARET, 450, 750);
  Engine_EventWait(60);
  gEventWork->transitionFrames = 60;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(VALE_TO_ROOF_MENDED);
  Engine_EventEnd();
}

/// Arriving by VALE_FROM_ROOF_MENDED after the opening, in Vale: Dora praises
/// the mended roof, remembers Kyle, and sends Isaac, Garet and Jenna off to Mt.
/// Aleph with Kraden. "Good job, Isaac. You patched it nicely."
static void Event_RoofMended(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  u32 i;
  s32 turnBack;
  s32 turnSide;
  struct FieldActor *turned;
  s32 none;
  s32 flag;
  SchedulerCallback callbackA;
  SchedulerCallback callbackB;
  SchedulerCallback callbackC;
  SchedulerCallback callbackD;
  SchedulerCallback callbackE;
  SchedulerCallback callbackF;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
  Actor_SetPosition(ACTOR_DORA, PIXELS(392), PIXELS(896));
  turned = Actor_Get(ACTOR_DORA);

  turned->facing = FACING_EAST;
  turnBack = FACING_NORTH;
  turned->facing = turnBack;
  Actor_SetPosition(GARET, PIXELS(298), PIXELS(736));
  turned = Actor_Get(GARET);
  turnSide = FACING_SOUTH;
  turned->facing = turnSide;
  Actor_SetPosition(JENNA, PIXELS(298), PIXELS(760));
  turned = Actor_Get(JENNA);
  turned->facing = turnSide;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  actor = Actor_Get(ACTOR_FIRST_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(840);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_SECOND_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(844);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_THIRD_BOARD);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(848);
  Engine_ActorSetSpriteFlags(actor, 0);
  ((u8 *)Engine_EventGetViewCenter())[85] = 0;
  Engine_TaskWait(1);
  Camera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Actor_SetSpeed(JENNA, PIXELS(1) / 2, turnSide);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, turnSide);
  Actor_SetActionScript(JENNA, gVale_JennaWalksToTheHouse);
  Actor_SetActionScript(GARET, gVale_GaretWalksToTheHouse);
  Engine_EventWait(40);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 400, 840);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, turnBack, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Scene_BeginCutscenePsynergy();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  FieldScheduler_AddCallback(Vale_PlayStepSound, TASK_PRIORITY_SCENE);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);

  callbackA = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackA, TASK_PRIORITY_SCENE);
  callbackB = Vale_CycleFirstBoardPalette;
  Task_AddCallback(callbackB, TASK_PRIORITY_SCENE);
  Actor_SetSpeed(ACTOR_FIRST_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_FIRST_BOARD, 400, 826);
  Engine_EventWait(20);
  {
    struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Scheduler_RemoveCallback(Vale_PlayStepSound);
  Scheduler_RemoveCallback(callbackA);
  Scheduler_RemoveCallback(callbackB);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_FIRST_BOARD, 0);
  Actor_SetPosition(ACTOR_FIRST_BOARD, 0, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Engine_EventWait(120);
  Map_CopyCellsTo(7, 102, 84, 41, 2, 1);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  Task_AddCallback(Vale_PlayStepSound, TASK_PRIORITY_SCENE);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  callbackC = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackC, TASK_PRIORITY_SCENE);
  callbackD = Vale_CycleSecondBoardPalette;
  Task_AddCallback(callbackD, TASK_PRIORITY_SCENE);
  Actor_SetSpeed(ACTOR_SECOND_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_SECOND_BOARD, 377, 828);
  Engine_EventWait(20);
  {
    struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Scheduler_RemoveCallback(Vale_PlayStepSound);
  Scheduler_RemoveCallback(callbackC);
  Scheduler_RemoveCallback(callbackD);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_SECOND_BOARD, 0);
  Actor_SetPosition(ACTOR_SECOND_BOARD, 0, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Engine_EventWait(120);
  Map_CopyCellsTo(6, 102, 83, 41, 1, 1);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
  Task_AddCallback(Vale_PlayStepSound, TASK_PRIORITY_SCENE);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  callbackE = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackE, TASK_PRIORITY_SCENE);
  callbackF = Vale_CycleThirdBoardPalette;
  FieldScheduler_AddCallback(callbackF, TASK_PRIORITY_SCENE);
  Actor_SetSpeed(ACTOR_THIRD_BOARD, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_THIRD_BOARD, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_THIRD_BOARD, 360, 837);
  Engine_EventWait(20);
  {
    struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Scheduler_RemoveCallback(Vale_PlayStepSound);
  Scheduler_RemoveCallback(callbackE);
  Scheduler_RemoveCallback(callbackF);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_THIRD_BOARD, 0);
  Actor_SetPosition(ACTOR_THIRD_BOARD, 0, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVale_LeaderHammers);
  Engine_EventWait(120);
  Scene_EndCutscenePsynergy();
  Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorJump(ACTOR_DORA, 2, 20);
  Engine_EventSetMessage(MsgValeGoodJob);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST + FACING_STEP, 10);
  Vale_MoveActorAlongLowerZPath(ACTOR_DORA, 5, 6, 0);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    gEventWork->message++;
  } else {
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  }
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_EventSetMessage(MsgValeWorkingYourselvesBone);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    gEventWork->message++;
  }
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventSetMessage(MsgValeDevastatedWhenKyle);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(GARET, 369, 904);
  Actor_WalkToAndWait(JENNA, 392, 904);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Vale_MoveActorAlongLowerZPath(JENNA, 10, 11, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST + FACING_STEP, 20);
  Engine_ActorJump(JENNA, 4, 0);
  Actor_WalkToAndWait(JENNA, 392, 843);
  Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Vale_MoveActorAlongLowerZPath(GARET, 10, 11, 0);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetSpeed(GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkTo(GARET, 392, 843);
  Object_GetById(JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(JENNA, 408, 843);
  Engine_EventWait(1);
  {
    struct FieldActor *record = Actor_Get(JENNA);

    record->steeringFlags = ACTOR_FACES_MOTION | record->steeringFlags;
  }
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorJump(ACTOR_DORA, 4, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(80);
  Engine_ActorFaceEachOther(JENNA, GARET, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 10);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
  Engine_ActorFaceEachOther(JENNA, GARET, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_OpenMessage(ACTOR_DORA, 0);
  none = 0;

  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    gEventWork->message++;
  }
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_EventSetMessage(MsgValeAsStubbornAsYourFather);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorJump(ACTOR_DORA, 4, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_DORA_HANDS_UP);
  Engine_EventWait(5);
  Event_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, GARET, 10, 14, 4, 14,
                               none);
  Event_PlayCue(SE_161);
  actor = Actor_Get(ACTOR_DORA);
  actor->sprite->flags = none;
  actor->steeringFlags &= 0xfe;
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
  Engine_EventWait(4);
  for (i = 0; i != 4; i++) {
    actor->z.fixed += PIXELS(3) / 2;
    actor->scaleY += -0x1999;
    Engine_EventWait(1);
  }
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetSpeed(GARET, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorJump(GARET, 6, 0);
  Actor_WalkToAndWait(GARET, 374, VALE_GARET_REUNION_Z);
#if TBS_PATCH_2
  Actor_SetPosition(GARET, PIXELS(374), PIXELS(VALE_GARET_REUNION_Z));
#endif
  Event_ShowMessage(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Engine_ActorSetAnimation(GARET, ANIM_GARET_STARTLED);
  Engine_ActorJump(GARET, 2, 5);
  Event_PlayCue(SE_143);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(GARET, 3);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_DORA_PEEK);
  actor->scaleY = 0x8000;
  Actor_SetPosition(ACTOR_DORA, PIXELS(364), PIXELS(811));
  for (i = 0; i != 5; i++) {
    actor->scaleY += 0x1999;
    Engine_EventWait(1);
  }
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Camera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  Camera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorJump(ACTOR_DORA, 6, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  actor->priorityFlags &= 0xfe;
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_DORA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 40);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 1);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_SetSpeed(GARET, PIXELS(4), PIXELS(2));
  actor = Actor_Get(GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(GARET, 403, VALE_GARET_REUNION_Z);
  Actor_SetAttachedEffect(JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(JENNA, FACING_NORTH, 20);
  Event_ShowMessageAndWait(JENNA, 0, 1);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorSetAnimation(GARET, ANIM_GARET_STARTLED);
  Engine_ActorJump(GARET, 2, 5);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Event_PlayCue(SE_143);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(JENNA, 408, 855);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorStartRepeatedMotion(JENNA, 3);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_SOUTH, 30);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_WEST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Engine_ActorJump(GARET, 4, 0);
  Engine_ActorWalkToAndWait(GARET, 398, 828);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(60);
  actor = Actor_Get(GARET);
  flag = 1;
  actor->steeringFlags |= flag;
  actor = Actor_Get(JENNA);
  actor->steeringFlags |= flag;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(JENNA, actor->x.part.pixel + 16, actor->z.part.pixel);
  Actor_WalkToAndWait(GARET, actor->x.part.pixel + 16,
                      actor->z.part.pixel - 16);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  Actor_WalkToAndWait(JENNA, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_WalkToAndWait(GARET, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(GARET, 0, 0);
  Engine_PartyAddMembers(GARET, JENNA);
  Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
  Vale_MoveActorAlongHigherZPath(ACTOR_PARTY_LEADER, 13, 10, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  {
    struct FieldActor *record = Actor_Get(ACTOR_DORA);

    record->steeringFlags = flag | record->steeringFlags;
  }
  Vale_MoveActorAlongHigherZPath(ACTOR_DORA, 6, 5, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(100);
  Map_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
#if TBS_PATCH_1
  leader->motionFlags = 3;
  leader->y.fixed = PIXELS(160);
  leader->targetY = 0x80000000;
  leader->velocityY = 0;
#else
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 3;
#endif
  Engine_EventEnd();
}

#include "common/effect_psynergy.h"

/// Cycles palettes on Garet and periodically spawns attached effects.
static void Vale_UpdateGaretPaletteAndEffects(void) {
  Vale_CycleActorPaletteAndSpawnEffects(Object_GetById(GARET));
}

/// Cycles the leader palette and periodically spawns attached effects.
static void Vale_UpdateLeaderPaletteAndEffects(void) {
  Vale_CycleActorPaletteAndSpawnEffects(Object_GetById(ACTOR_PARTY_LEADER));
}

/// Cycles the pillar's colour modes while Garet's Psynergy moves it in the
/// opening.
static void Vale_CycleOpeningPillarPalette(void) {
  Vale_CycleActorPalette(Object_GetById(ACTOR_OPENING_PILLAR));
}

/// Cycles the first board palette while its repair effect runs.
static void Vale_CycleFirstBoardPalette(void) {
  Vale_CycleActorPalette(Object_GetById(ACTOR_FIRST_BOARD));
}

/// Cycles the second board palette while its repair effect runs.
static void Vale_CycleSecondBoardPalette(void) {
  Vale_CycleActorPalette(Object_GetById(ACTOR_SECOND_BOARD));
}

/// Cycles the third board's palette while its repair effect runs.
static void Vale_CycleThirdBoardPalette(void) {
  Vale_CycleActorPalette(Object_GetById(ACTOR_THIRD_BOARD));
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
static s32 Vale_TestFacing(struct FieldActor *obj, struct FieldActor *target,
                           s32 range, s32 force) {
  s32 result;
  u32 angle;
  u32 left;
  u32 right;
  u32 dir;

  result = 0;
  if (obj->paused == 1) {
    if (obj->riseCounter == 0) {
      Object_SetMode(obj, 1);
      return 1;
    }
  }
  if (Vale_DistanceInPixels(&target->x.fixed, &obj->x.fixed) < range ||
      force != 0) {
    angle = (u16)ArcTan2(target->z.fixed - obj->z.fixed,
                         target->x.fixed - obj->x.fixed);
    left = (angle - 0x1000) & 0xf000;
    right = (angle + 0x1000) & 0xf000;
    angle &= 0xf000;
    dir = obj->facing & 0xf000;
    if (angle == dir || right == dir || left == dir || force != 0) {
      obj->paused = 1;
      Object_SetMode(obj, 1);
      result = 1;
      obj->riseCounter = result;
    } else {
      obj->paused = 0;
      Object_SetMode(obj, 2);
      obj->riseCounter = 0;
    }
  } else {
    obj->paused = 0;
    Object_SetMode(obj, 2);
    obj->riseCounter = 0;
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
  struct FieldActor *guard = &fieldObject->actor;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
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

/// The Move hand's update hook in the opening: each frame it sheds a sparkle
/// (SPRITE_SPARKLE) just below and around it, with SE_ITEM_BREAK.
static void Vale_TrailSparkles(union FieldObject *fieldObject) {
  struct FieldActor *source = &fieldObject->actor;
  s32 spawnPosition[3];
  s32 randomDistance;
  struct FieldActor *particle;
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
    Engine_ActorSetSpriteFlags(particle, 0);
    Object_SetMode(particle, 0);
    Engine_ObjectSetScript(particle, gVale_SparkleScript);
    {
      struct AnimationObject *record = particle->sprite;
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
static void Vale_VanishMoveHand(struct FieldActor *emitter) {
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
    struct FieldActor *particle = Engine_ObjectCreate(
        SPRITE_SPARKLE, emitter->x.fixed, emitter->y.fixed, emitter->z.fixed);

    if (particle != 0) {
      s32 verticalSpeed;

      Engine_ActorSetSpriteFlags(particle, 0);
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
  emitter->targetX = ACTOR_NO_TARGET;
  emitter->targetY = ACTOR_NO_TARGET;
  emitter->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(emitter, 0, 0, 0);
}

/// Moves a particle outward in the horizontal plane without changing its
/// height.
static void Vale_UpdateParticlePosition(struct FieldActor *particle,
                                        s32 distance, s32 angle) {
  s32 position[3];
  if (particle != 0) {
    SET_VECTOR(position, particle->x.fixed, particle->y.fixed,
               particle->z.fixed);
    Vector_AddPolarOffset(distance, angle, position);
    Engine_ObjectSetPosition(particle, position[0], position[1], position[2]);
  }
}

/// Pushing the pillar in Vale (touch cells 30 and 31) moves it a cell.
static void Event_PushPillar(void) {
#if !TBS_PATCH_1
  // Japanese edition only: look up the pillar but do not use the return value.
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY))
    Object_GetById(ACTOR_PILLAR_AFTER_KALAY);
  else
    Object_GetById(ACTOR_PILLAR);
#endif
  Map_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  StagedActor_AdvancePair();
  Event_SettleMovedPillar();
}

/// Records where the pillar now stands and blocks its cell. After Move or a
/// push, it records the pillar's column in FLAG_PLACE_TEMP_14 to _16 and blocks
/// the cell the pillar now stands on.
static void Event_SettleMovedPillar(void) {
  struct FieldActor *actor;
  s32 column;

  Map_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY))
    actor = Object_GetById(ACTOR_PILLAR_AFTER_KALAY);
  else
    actor = Object_GetById(ACTOR_PILLAR);
  if (actor == NULL)
    return;
  GameFlag_Clear(FLAG_PLACE_TEMP_14);
  GameFlag_Clear(FLAG_PLACE_TEMP_15);
  GameFlag_Clear(FLAG_PLACE_TEMP_16);
  column = actor->x.fixed >> 20;
  if (column == 22) {
    Map_CopyCellAttributes(17, 1, 1, 1, column, 36);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_14);
  } else if (column == 23) {
    Map_CopyCellAttributes(17, 1, 1, 1, column, 36);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_15);
  } else {
    Map_CopyCellAttributes(17, 1, 1, 1, 24, 36);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_16);
  }
}

/// The callback that plays SE_131 every 16 frames while Isaac's Psynergy lifts
/// a board at the roof's end.
static void Vale_PlayStepSound(void) {
  if ((*(u32 *)&gFrameCount & 15) == 0)
    Engine_AudioPlayCue(SE_131);
}

/// Searching a locked door in Vale (search code 24).
static void Event_SearchTheLockedDoor(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeDoorWontOpen, 1);
  Engine_EventEnd();
}

/// Pauses the man guarding Mt. Aleph while the corresponding event flag is set.
static void Vale_SynchronizeAlephGuardPause(void) {
  s32 pauseFlag;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_41) != 0) {
    pauseFlag = GameFlag_IsSet(FLAG_MENU_OPEN);
    if (pauseFlag == 0) {
      Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = pauseFlag;
      GameFlag_Clear(FLAG_SCENE_TEMP_41);
    }
  } else {
    if (GameFlag_IsSet(FLAG_MENU_OPEN) != 0) {
      Object_GetById(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = 1;
      GameFlag_Set(FLAG_SCENE_TEMP_41);
    }
  }
}

/// Keeps the raised pillar's priority in step with the leader's height.
static void Vale_AdjustPillarPriorityForLeaderHeight(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *pillar;
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    pillar = Object_GetById(ACTOR_PILLAR_AFTER_KALAY);
  else
    pillar = Object_GetById(ACTOR_PILLAR);
  if (pillar != 0) {
    if (leader->y.fixed > PIXELS(200))
      pillar->priorityFlags =
          ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
    else
      pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
}

#include "common/object_orbit.h"

/// Someone calling up Psynergy: drawn SPRITE_COLOR_WHITE (7) for two frames in
/// four, and the world map's linked sparks every 16 frames.
static void Vale_FlashActorAndSpawnEffects(struct FieldActor *actor) {

  if ((gFrameCount & 2) != 0)
    Engine_ObjectSetPartPalettes(actor, 7);
  else
    Engine_ObjectSetPartPalettes(actor, 0);
  if ((gFrameCount & 0xf) == 0)
    WorldMap_CreateLinkedEffects(actor);
}

/// Someone using Psynergy: colour modes SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG
/// (0 to 5), the sprite and its five swaps of colour channels, in turn, and
/// sparks every 16 frames.
static void Vale_CycleActorPaletteAndSpawnEffects(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0)
    Engine_ObjectSetPartPalettes(actor, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 0xf) == 0)
    WorldMap_CreateLinkedEffects(actor);
}

/// What Psynergy moves: colour modes SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG in
/// turn, a step every second frame.
static void Vale_CycleActorPalette(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0)
    Engine_ObjectSetPartPalettes(actor, (gFrameCount >> 1) % 6);
}

#include "common/effect_vertical.h"
#include "common/world_map_linked_effects.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

/// The man guarding Mt. Aleph walks his beat below the path once he has warned
/// Isaac, looking about at each stop. SW_TIMER says which stretch he is on,
/// 1 to 3, of which the warning events act on the two pauses, 1 and 2.
s32 gVale_AlephGuardPatrol[] = {
    SC_SET(SW_SPEED, PIXELS(1) / 2),
    SC_SET(SW_ACCEL, PIXELS(1) / 4),
    SC_SET(SW_TIMER, 2),
    SC_LABEL(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(296), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_TIMER, 3),
    SC_MOVE(PIXELS(296), 0, PIXELS(420)),
    SC_SYNC,
    SC_ARG(1),
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
    SC_ARG(2),
    SC_MOVE(PIXELS(296), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_TIMER, 2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(15),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(60),
    SC_ARG(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_MOVE(PIXELS(212), 0, PIXELS(388)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(429)),
    SC_SYNC,
    SC_ARG(1),
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
    SC_ARG(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_TIMER, 2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ARG(1),
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
    SC_ARG(2),
    SC_MOVE(PIXELS(300), 0, PIXELS(880)),
    SC_SYNC,
    SC_MOVE(PIXELS(300), 0, PIXELS(904)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

s32 gVale_JennaWalksToTheHouse[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(300), 0, PIXELS(904)),
    SC_SYNC,
    SC_MOVE(PIXELS(321), 0, PIXELS(904)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

/// A spout of water grows from a tenth of its size north of the strongman by
/// the river and vanishes (Event_UnusedWaterBurstsByTheRiver).
s32 gVale_UnusedWaterGrows[] = {
    SC_ARG(1),
    SC_POS(PIXELS(266), 0, PIXELS(1230)),
    SC_SET(SW_SCALE_X, 0x1999),
    SC_SET(SW_SCALE_Y, 0x1999),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 3932),
    SC_ADD(SW_SCALE_Y, 0x147a),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// A sparkle falls and vanishes after 10 frames; the burst's sparkles start at
/// its second word, past the first SC_FREE.
s32 gVale_SparkleScript[] = {
    SC_FREE,
    SC_SYNC,
    SC_SET(SW_SPEED, PIXELS(1) / 16),
    SC_ARG(0),
    SC_WAIT(10),
    SC_FREE,
};

struct SceneEntrance gVale_Entrances[] = {
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
    { SCENE_TABLE_END },
};

u32 gVale_Exits[] = {
    SCENE_EXITS(Vale),
    SCENE_EXIT(VALE_TO_SOL_ENTRANCE, SolEntrance, SOL_ENTRANCE_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_GATE, ValeGate, VALE_GATE_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_KRADEN_NORTH, ValeKraden, VALE_KRADEN_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_VALE_KRADEN_SOUTH, ValeKraden, VALE_KRADEN_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_TO_VALE_INTERIORS_SHOPS_EAST, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_EAST),
    SCENE_EXIT(VALE_TO_VALE_INTERIORS_SHOPS_SOUTH, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_TO_VALE_INTERIORS_INN, ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_INTERIORS_SHOPS_NORTH, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_VALE_INTERIORS_SHOPS_WEST, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_WEST),
    SCENE_EXIT(VALE_TO_VALE_INTERIORS_SHOPS_NORTH_SECOND_DOOR, ValeInteriorsShops, VALE_INTERIORS_SHOPS_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_SANCTUM, Sanctum, SANCTUM_FROM_VALE),
    SCENE_EXIT(VALE_TO_ROOF_MENDED, Vale, VALE_FROM_ROOF_MENDED),
    SCENE_EXITS_END,
};

struct ScenePlacement gVale_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(19), 0, PIXELS(614), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(50), 0, PIXELS(795), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(65), 0, PIXELS(800), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(57), 0, PIXELS(805), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(398), 0, PIXELS(616), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(373), 0, PIXELS(VALE_DORA_Z), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, 7,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVale_PlacementsAfterLeaving[] = {
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(19), 0, PIXELS(614), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(50), 0, PIXELS(795), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(65), 0, PIXELS(800), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, 4,
        PIXELS(57), 0, PIXELS(805), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_WEST, 3, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(398), 0, PIXELS(616), FACING_NORTH, 3, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(335), 0, PIXELS(869), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, 7,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVale_PlacementsAfterKalay[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(356), 0, PIXELS(727), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVale_RepairPlacements[] = {
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(409), 0, PIXELS(1134), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(428), 0, PIXELS(588), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(204), 0, PIXELS(924), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(394), 0, PIXELS(584), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(373), 0, PIXELS(VALE_DORA_Z), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd7, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVale_Events[] = {
    { EVENT_TALK, ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephLooksBeautifulTodayIts) },
    { EVENT_TALK, ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeValeStaysPristineBecauseOfMt) },
    { EVENT_TALK, ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutTheMayorAndTremors) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeThisPlaceLooksJustLikeIt) },
    { EVENT_TALK, ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacWatchOutDontGetToo) },
    { EVENT_TALK, ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeValeMustRemainHiddenPsynergysSecrets) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnAboutGoingBeyondKradensCottage) },
    { EVENT_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ArgueWithGaretsSister) },
    { EVENT_TALK, ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeYoureInTroubleNowGaretYour) },
    { EVENT_TALK, ACTOR_DORA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DoraFretsAboutTheRoof) },
    { EVENT_TALK, ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephIsASacredPeak) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSanctum) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ApproachMtAleph) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeKradenNorth) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeKradenSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeGate) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGaretsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterValeInteriorsShops) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterIsaacsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterValeInteriorsShopsWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetPathCellOverride) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearPathCellOverride) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WatchThePuppies) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TurnBackFromThePuppies) },
    { EVENT_TOUCH, 50, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT_SCRIPT(Event_WarnAgainstSneakingUpMtAleph) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningFromFirstApproach) },
    { EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningFromSecondApproach) },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningAtEitherPause) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthernPassage) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReturnThroughSouthernPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, EVENT_SCRIPT(Event_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleMovedPillar) },
    { TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchTheLockedDoor) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVale_EventsAfterLeaving[] = {
    { EVENT_TALK, ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeIsSavedThanksToThe) },
    { EVENT_TALK, ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeEverSinceTheTremorsStartedMt) },
    { EVENT_TALK, ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutTheMayorAndTremors) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeSighNoLongerWillISee) },
    { EVENT_TALK, ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIDontBlameFelixIfOnly) },
    { EVENT_TALK, ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeTheMayorWasTooKindTo) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnAboutGoingBeyondKradensCottage) },
    { EVENT_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreSoLuckyGaret) },
    { EVENT_TALK, ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseTakeCareOfMyBrother) },
    { EVENT_TALK, ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
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
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSanctum) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ApproachMtAleph) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeKradenNorth) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeKradenSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeGate) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGaretsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterValeInteriorsShops) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterIsaacsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterValeInteriorsShopsWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetPathCellOverride) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearPathCellOverride) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WatchThePuppies) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TurnBackFromThePuppies) },
    { EVENT_TOUCH, 50, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT_SCRIPT(Event_WarnAgainstSneakingUpMtAleph) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningFromFirstApproach) },
    { EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningFromSecondApproach) },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningAtEitherPause) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthernPassage) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReturnThroughSouthernPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, EVENT_SCRIPT(Event_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleMovedPillar) },
    { TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchTheLockedDoor) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVale_EventsAfterKalay[] = {
    { EVENT_TALK, ACTOR_GARETS_SISTER_AFTER_KALAY, FLAG_PLACE_TEMP_03, EVENT_SCRIPT(Event_GaretMeetsHisSister) },
    { EVENT_TALK, ACTOR_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICanTellJustByLooking) },
    { EVENT_TALK, ACTOR_GARETS_BROTHER_AFTER_KALAY, FLAG_PLACE_TEMP_04, EVENT_SCRIPT(Event_GaretAsksToProtectFamily) },
    { EVENT_TALK, ACTOR_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeIWillDoMyBestTo) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT_SCRIPT(Event_AskWhetherJennaHasReturned) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValePromiseMeIsaacPromiseMeYoull) },
    { EVENT_TALK, ACTOR_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeIUsedToPlayHere) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephsEruptionWasSoPowerful) },
    { EVENT_TALK, ACTOR_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReactToAnotherTremor) },
    { EVENT_TALK, ACTOR_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheDangerIsGone) },
    { EVENT_TALK, ACTOR_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_GARETS_SISTER_AFTER_KALAY, FLAG_PLACE_TEMP_03, EVENT_SCRIPT(Event_GaretMeetsHisSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreWorkingSoHard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_GARETS_BROTHER_AFTER_KALAY, FLAG_PLACE_TEMP_04, EVENT_SCRIPT(Event_GaretAsksToProtectFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICouldGoWithYouIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT_SCRIPT(Event_AskWhetherJennaHasReturned) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValeYouDidntBringJennaWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeFelixMustBeProtectingJennaIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeTheEarthquakesArentCausedByEruptions) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, MSGNO(MsgValeAhhICantStandTheseEarthquakes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheSmokeFromMtAlephIsnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_MAN_MISSING_TRAVELERS, FLAG_PLACE_TEMP_07, EVENT_SCRIPT(Event_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgValeHavingNoTravelersIsNoFun) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSanctum) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ApproachMtAleph) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeKradenNorth) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeKradenSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExitToValeGate) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGaretsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterValeInteriorsShops) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterIsaacsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterValeInteriorsShopsWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UnusedEnterJennasGrandparentsHouseBySecondDoor) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetPathCellOverride) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearPathCellOverride) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthernPassage) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReturnThroughSouthernPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT_AFTER_KALAY, FLAG_VALE_NUT_CAUGHT, EVENT_SCRIPT(Event_CatchNutAfterKalay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_AFTER_KALAY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleMovedPillar) },
    { TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchTheLockedDoor) },
    { SCENE_EVENTS_END },
};
// clang-format on

// The doors' cell animations, from the map's cells below the village, each
// picture shown for two frames; vale_storm.c has the same four kinds. Each
// table takes the name of a door it opens. The two of Jenna's grandparents'
// kind and the west house's show their one open picture twice, where the
// stormy night's show two.

u16 gVale_SanctumDoorOpens[] = {
    MAP_CELL_STEP(0, 98, 2, 2, 2),
    MAP_CELL_STEP(2, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gVale_IsaacsDoorOpens[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(2, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gVale_GrandparentsDoorOpens[] = {
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gVale_WestHouseDoorOpens[] = {
    MAP_CELL_STEP(4, 96, 2, 2, 2),
    MAP_CELL_STEP(4, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

// The man guarding Mt. Aleph drives Isaac back down the path: at the first
// warning (Event_WarnAgainstSneakingUpMtAleph), and again from the west or
// from either of the guard's pauses (Vale_AskAboutSneakingToAleph).

s32 gVale_FirstWarningLeaderPath[] = {
    SC_ARG(5), SC_MOVE(PIXELS(199), 0, PIXELS(446)),
    SC_SYNC,   SC_MOVE(PIXELS(215), 0, PIXELS(472)),
    SC_SYNC,   SC_MOVE(PIXELS(239), 0, PIXELS(461)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gVale_FirstWarningGuardPath[] = {
    SC_ARG(2), SC_MOVE(PIXELS(199), 0, PIXELS(446)),
    SC_SYNC,   SC_MOVE(PIXELS(215), 0, PIXELS(472)),
    SC_SYNC,   SC_MOVE(PIXELS(239), 0, PIXELS(461)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gVale_LeaderFromWestPath[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_ARG(5),
    SC_MOVE(PIXELS(224), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(474)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

s32 gVale_GuardFromFirstPausePath[] = {
    SC_SET(SW_SPEED, PIXELS(3) / 2),
    SC_SET(SW_ACCEL, PIXELS(1)),
    SC_ARG(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

s32 gVale_LeaderPushedBackPath[] = {
    SC_SET(SW_SPEED, PIXELS(2)),
    SC_SET(SW_ACCEL, PIXELS(2)),
    SC_ARG(5),
    SC_MOVE(PIXELS(256), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(474)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

s32 gVale_GuardFromSecondPausePath[] = {
    SC_SET(SW_SPEED, PIXELS(3) / 2),
    SC_SET(SW_ACCEL, PIXELS(1)),
    SC_ARG(2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};
