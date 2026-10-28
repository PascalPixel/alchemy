/// @file
/// Kalay, the merchants' town Lord Hammet founded, the front of his palace, and
/// the tunnel beneath them: four areas in six scenes.
///
/// - Kalay (SceneId_Kalay), the town. Eight doors (step triggers 1 to 8) open
/// into
///   the houses, the weapon and armor shops, the inn and the Kalay Sanctum
///   (stucco_rooms_a.c), and KALAY_TO_WEAPON_SHOP_COUNTER and
///   KALAY_TO_ARMOR_SHOP_COUNTER into the same two shops from behind their
///   counters; KALAY_TO_HAMMET_PALACE_GROUNDS leads to Hammet Palace, and
///   step triggers 9 and 10 out to the World Map (KALAY_TO_WORLD_MAP and
///   KALAY_TO_WORLD_MAP_WEST, to its WORLD_MAP_FROM_KALAY_EAST and
///   WORLD_MAP_FROM_KALAY_WEST). The weapon, armor and item sellers open their
///   shops only when spoken to across the counter. A Nut circles in the air
///   until Catch pulls it in (FLAG_KALAY_TOWN_NUT_CAUGHT), and a statue at the
///   north edge (ACTOR_TOWN_STATUE), which Move pushes aside, covers the way
///   down into the Kalay Tunnel (KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM); coming
///   up from it (KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM) finds the statue moved.
///
/// Once the party has heard of the Colosso tour at the inn
/// (FLAG_KALAY_HEARD_OF_COLOSSO_TOUR), leaving town plays the tour group's
/// departure, once (FLAG_KALAY_COLOSSO_TOUR_LEFT): the guides count the group,
/// find a man missing and lead it off without him, and he runs after it. The
/// second guide is the head guide's sprite with its green and blue swapped
/// (SPRITE_COLOR_RBG), here and in stucco_rooms_a.c and kalay_tolbi_docks.c.
/// - Hammet Palace (SceneId_HammetPalaceGrounds), before the door of the
/// palace,
///   where two soldiers of Kalay stand guard; the palace's rooms are
///   SceneId_HammetPalace (hammet_palace.c). The first time the party comes,
///   before Ivan has reported to Layana (FLAG_IVAN_REPORTED_TO_LAYANA), the
///   guards recognize Ivan and take everyone in to Lady Layana
///   (HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM, to
///   HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS); afterwards they
///   have other lines. Walking into the door (step trigger 2) opens it into the
///   palace (HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE);
///   HAMMET_PALACE_GROUNDS_TO_KALAY leads back to town.
/// - Kalay Tunnel Entrance (SceneId_KalayTunnelEntrance), outside the town (the
///   World Map's WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE leads in at
///   KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH). A guard
///   (ACTOR_TUNNEL_GUARD) bars `the cave up ahead` by Hammet and Layana's
///   command, and a Nut floats here until Catch takes it
///   (FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT). Arriving with Hammet in Bunza's
///   wagon (SPRITE_LARGE_WAGON) after Lunpa
///   (KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH) plays his homecoming: the
///   guard opens the way, Hammet goes in, and the party rides off
///   (KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH, to
///   WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH). Once FLAG_KALAY_TUNNEL_OPEN
///   is set the guard stands turned east.
///   KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_CAVE leads into the Kalay Tunnel's
///   cave (KalayTunnelCave).
/// - Kalay Tunnel (SceneId_KalayTunnelCave to SceneId_KalayTunnelTreasureRoom),
/// the
///   passage to Hammet Palace and his treasure, which Layana opens to the party
///   in thanks once Hammet is home (FLAG_KALAY_TUNNEL_OPEN). The cave
///   (KalayTunnelCave) joins the Kalay Tunnel Entrance to the passage
///   (KalayTunnelPassage), and a chest holds an Apple. The passage opens with a
///   window transition, its way through drawn open once FLAG_KALAY_TUNNEL_OPEN
///   is set. The treasure room (KalayTunnelTreasureRoom) holds the puzzle: two
///   statues, pushed by walking into them or moved with Move, each open a
///   passage step by step once far enough west (FLAG_TUNNEL_FIRST_PASSAGE_OPEN
///   and FLAG_TUNNEL_SECOND_PASSAGE_OPEN, area temp flags, so the
///   statues are back once the party leaves the tunnel's scenes); while a
///   passage is closed, particles stream from its statue as it nears its place.
///   The first passage raises a puddle (ACTOR_PUDDLE) that Frost freezes and
///   sinks into the floor, freeing its cells; that is a temporary flag, so the
///   puddle stands again on each entry. Scorch, a Mars Djinni, joins the party
///   when spoken to, without a battle, and five chests hold Spirit Gloves, a
///   Sleep Bomb, a Lucky Medal, 200 coins and a Potion. Once
///   FLAG_KALAY_TUNNEL_OPEN is set a man (ACTOR_WAITING_MAN) waits here,
///   thinking `The things beyond these doors belong to you all.` The fade
///   overlay (overworld/overworld/fade_overlay.c) follows the leader through
///   the treasure room unless he comes down from the town
///   (KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY); step triggers 10 and 11 stop and
///   restart it, and 18 and 19 fix and free his depth priority.
///   KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE leads into Hammet Palace
///   (HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM),
///   KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY up into the town
///   (KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM).
///
/// Every scene sets FLAG_KALAY_REACHED on entry. Kalay and Hammet Palace's
/// front also catch the palace's story up with Lunpa's: hearing in Lunpa that
/// Hammet is held there (FLAG_LUNPA_HEARD_OF_PRISONER) sets area temp flag
/// FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA, which changes what Layana says, and
/// once Hammet is free (FLAG_DODONPA_LOCKED_UP, set as Donpa judges Dodonpa in
/// Lunpa) Ivan's report and Layana's scolding count as seen and
/// FLAG_KALAY_TUNNEL_OPEN opens the passage. The getters choose each scene's
/// tables; the cave and the passage have no actor placements, and an
/// empty set serves any other scene.

#include "main_routine_work.h"
#include "module/field/field.h"
#include "system_call.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/kalay.h"
#include "module/scene/shared/actor_orbit.h"
#include "module/overworld/overworld.h"
#include "module/system/main_routine.h"
#include "module/overworld/event_commands.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "task.h"
#include "event.h"
#include "scene_tables.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "script_operands.h"
#include "macros.h"

#include "actor.h"
#include "actor_effect.h"
#include "orbit.h"
#include "fixed_math.h"
#include "scene.h"
#include "heap.h"

#include "flags.h"
#include "djinn_ids.h"
#include "sounds.h"
#include "sound_driver_symbols.h"

/// Kalay's actors (Kalay), numbering the rows of
/// gKalay_TownActorPlacements from ACTOR_FIRST_PLACED, then the tour
/// group, which Kalay_WatchTourGroupDeparture places after them
/// (gKalay_TourGroupActorPlacements).
enum KalayTownActorId {
  /// The two soldiers of Kalay at the town's entrance from the World Map.
  /// ★ Named from the game: "Welcome to Kalay! You may enter, so long as you
  /// do not disrupt the peace." (MsgKalayWelcomeInPeace)
  ACTOR_WELCOMING_SOLDIER = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "We soldiers protect the peace in Kalay with our
  /// lives." (MsgKalaySoldiersProtectPeace)
  ACTOR_SOLDIER_PROTECTING_THE_PEACE,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "Lord Hammet, the founder of Kalay, is a great
  /// merchant." (MsgKalayHammetGenerousMerchant)
  ACTOR_YOUNG_MAN_ON_HAMMET_THE_MERCHANT,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Lord Hammet is still out with his caravans on
  /// business." (MsgKalayHammetOnJourney)
  ACTOR_WOMAN_ON_HAMMETS_CARAVANS,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  /// ★ Named from the game: "Thankfully, it has been rebuilt since then."
  /// (MsgKalayNorthernBridgeRebuilt)
  ACTOR_MAN_ON_THE_REBUILT_BRIDGE,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "I heard a rumor that there's trouble in Lamakan
  /// Desert." (MsgKalayLamakanTrouble)
  ACTOR_WOMAN_ON_LAMAKAN_DESERT,
  /// The tourist the guides are missing, an old man with a cane
  /// (SPRITE_OLD_MAN_WITH_CANE), who stands in town until then and takes his
  /// place again after.
  /// ★ Named from the game: "Uh-oh... We're short one person... Now where
  /// could he be?" (MsgKalayShortOnePerson)
  ACTOR_LATE_TOURIST,
  /// An old woman (SPRITE_OLD_WOMAN). "The guests at the inn are tourists on
  /// their way to see Colosso."
  /// ★ Named from the game: "I should have saved my money. Then I'd be on
  /// that tour, too." (MsgKalayWishSavedForFestival)
  ACTOR_WOULD_BE_TOURIST,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR). "I hear Colosso
  /// is incredible. I'd love to see such a huge carnival."
  /// ★ Named from the game: "Maybe when I'm older, I'll even get to go fight
  /// in Colosso!" (MsgKalayChildFestivalDream)
  ACTOR_BOY_DREAMING_OF_COLOSSO,
  /// A little girl with pigtails (SPRITE_GIRL_WITH_PIGTAILS).
  /// ★ Named from the game: "I think it was called Mt. Aleph."
  /// (MsgKalayTraderTownNews)
  ACTOR_GIRL_ON_MT_ALEPH,
  /// A strongman (SPRITE_STRONGMAN).
  /// ★ Named from the game: "Lord Hammet's knack for business is the reason
  /// Kalay has flourished." (MsgKalayHammetBusinessSkill)
  ACTOR_STRONGMAN_ON_HAMMETS_KNACK,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN) who came to Kalay from
  /// Tolbi, afraid of Babi (Kalay_TalkAboutLeavingHome).
  /// ★ Named from the game: "I came to Kalay because I didn't like Tolbi..."
  /// (MsgKalayCameKalayBecauseDidntLike)
  ACTOR_MAN_WHO_LEFT_TOLBI,
  /// The Colosso tour's guides, men carrying packs
  /// (SPRITE_MAN_CARRYING_A_PACK), who stand in town and lead the tour off
  /// when it leaves.
  /// ★ Named from the game: "Are we all listening? Let's stop right here."
  /// (MsgKalayOkListeningLets)
  ACTOR_HEAD_GUIDE,
  /// ★ Named from the game: "We're just checking to make sure no one is
  /// missing!" (MsgKalayWhyWeStoppingAtPlace)
  ACTOR_SECOND_GUIDE,
  /// ★ Named from the game: "These weapons are good, but they're not the best
  /// Kalay makes." (MsgKalayLordHammetSellsHisBest)
  ACTOR_WEAPON_MERCHANT,
  /// ★ Named from the game: "Ask me anything about armor."
  /// (MsgArmorShopWelcome)
  ACTOR_ARMOR_MERCHANT,
  /// ★ Named from the game: "Welcome! We have just the gear you need."
  /// (MsgItemShopWelcome)
  ACTOR_ITEM_MERCHANT,
  /// The statue (with the Kalay Tunnel's sprite) that Move pushes aside from
  /// the way down into the Kalay Tunnel.
  /// ★ Named from what the player sees: a stone statue on a pedestal at the
  /// town's north edge (its render)
  ACTOR_TOWN_STATUE,
  /// Circles in the air until Catch pulls it in.
  /// ★ Named from the game: "Nut" (MsgItemName)
  ACTOR_TOWN_NUT,
  /// The tour group, ACTOR_FIRST_TOURIST and the six after it, then
  /// ACTOR_OURANOS: in row order a chef (SPRITE_CHEF), a young woman in
  /// green, a young man with brown hair, a woman with fair hair, a woman with a
  /// sword, a bald, muscular man, a young man with blue hair, and Ouranos, a
  /// young man with red hair.
  /// ★ Named from the game: "Why are we stopping at a place like this?"
  /// (MsgKalayWhyWeStoppingAtPlace)
  ACTOR_FIRST_TOURIST,
  /// The tourist the one beside him calls Ouranos.
  /// ★ Named from the game: "It's just a shame a man like you can't ride on a
  /// boat, Ouranos." (MsgKalayDecideEnteringColosso)
  ACTOR_OURANOS = ACTOR_FIRST_TOURIST + 7
};

/// Hammet Palace's front (HammetPalaceGrounds), numbering
/// gKalay_HammetPalaceActorPlacements, whose first row places the party's
/// Ivan.
enum KalayPalaceActorId {
  /// The soldiers of Kalay before the palace door. "This is Lord Hammet's
  /// palace. The lord is away."
  /// ★ Named from what the player sees: two soldiers of Kalay standing
  /// guard at the palace door (its render)
  ACTOR_PALACE_GUARD = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: the second soldier at the palace
  /// door (its render)
  ACTOR_SECOND_PALACE_GUARD
};

/// The Kalay Tunnel Entrance (KalayTunnelEntrance), numbering
/// gKalay_TunnelEntranceActorPlacements after its rows for Garet, Ivan and
/// Mia.
enum KalayTunnelEntranceActorId {
  /// A soldier of Kalay who keeps the passage closed by Hammet and Layana's
  /// command.
  /// ★ Named from the game: "Do you want to go to the cave up ahead?"
  /// (MsgKalayDoWantGoCaveUp)
  ACTOR_TUNNEL_GUARD = ACTOR_FIRST_PLACED,
  /// Bunza's.
  /// ★ Named from what the player sees: a covered wagon drawn by a horse
  /// (its render)
  ACTOR_WAGON,
  /// ★ Named from the game: "We've arrived, Hammet."
  /// (MsgKalayWeveArrivedHammet)
  ACTOR_HAMMET,
  /// ★ Named from the game: "Is that you, Bunza?" (MsgLunpaMoment)
  ACTOR_BUNZA,
  /// Floats here until Catch takes it.
  /// ★ Named from the game: "Nut" (MsgItemName)
  ACTOR_TUNNEL_ENTRANCE_NUT
};

/// The Kalay Tunnel's treasure room (KalayTunnelTreasureRoom), numbering
/// gKalay_TunnelActorPlacements.
enum KalayTunnelActorId {
  /// The statues that open the two passages.
  /// ★ Named from what the player sees: two stone statues on pedestals that
  /// Move slides west (its render)
  ACTOR_FIRST_STATUE = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: the second stone statue, east of the
  /// first (its render)
  ACTOR_SECOND_STATUE,
  // XXX (no evidence): whether he is Bunza; likely, because every other scene
  // places SPRITE_BUNZA beside Hammet as Bunza, but his lines never name him.
  /// A man drawn with SPRITE_BUNZA who waits here once FLAG_KALAY_TUNNEL_OPEN
  /// is set.
  /// ★ Named from the game: "Layana was very hard on all of you, wasn't she?"
  /// (MsgKalayLayanaWasVeryHardOn)
  ACTOR_WAITING_MAN,
  /// The sprite of the puddle that Frost freezes into an ice pillar in
  /// bilibin_cave.c.
  /// ★ Named from what the player sees: a puddle of water, SPRITE_PUDDLE
  /// (its render)
  ACTOR_PUDDLE,
  /// A Mars Djinni.
  /// ★ Named from the game: "Scorch" (MsgDjinnName)
  ACTOR_SCORCH
};

/// SPRITE_PUDDLE's animation once Frost has frozen it into an ice pillar.
enum { PUDDLE_ANIMATION_FROZEN = 5 };

/// The Kalay Tunnel's own flags: the temporary ones last one entry, the
/// passages (area temp flags) while the party stays in the tunnel's
/// scenes.
enum KalayTunnelFlag {
  /// The fade overlay follows the leader.
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER = FLAG_SCENE_TEMP_00,
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
  /// Each statue's particles have started their sound.
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED = FLAG_SCENE_TEMP_02,
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED = FLAG_SCENE_TEMP_03,
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_FIRST_PASSAGE_OPEN = FLAG_AREA_TEMP_02,
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_SECOND_PASSAGE_OPEN = FLAG_AREA_TEMP_03
};

static void Kalay_PrepareTown(void);
static void Kalay_PrepareHammetPalace(void);
static void Kalay_PrepareTunnelEntrance(void);
static void Kalay_StartWindowTransition(void);
static void Kalay_PrepareTunnel(void);
static void Kalay_StartTouristsFidgeting(void);
static void Kalay_StartTouristFidgeting(s32 actorId);
static void Kalay_StartGuidesPacing(void);
static void Kalay_AlternateGuidesPacing(void);
static void Kalay_IvanMeetsPalaceGuards(void);
static void Kalay_HammetReturnsToPalace(void);
static void Kalay_UpdateTunnelPassages(void);
static void Kalay_UpdateTunnelEmitters(void);
static void Kalay_SpawnTunnelParticle(s32 x, s32 height, s32 z);
static void Kalay_UpdateNutOrbit(void);

/// Clears the flag a guide's script sets when it is done.
static __inline__ void Kalay_ClearScriptDone(struct Actor *actor) {
  s32 completed = 0;

  actor->timer = completed;
}

/// Talking to Scorch, a Mars Djinni, in the Kalay Tunnel
/// (KalayTunnelTreasureRoom) adds it to the party, without a battle.
static void Kalay_ScorchJoins(void) {
  Djinn_JoinParty(ACTOR_SCORCH, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH));
}

/// In Kalay, while the tour waits, a tourist now and then nods, shakes his head
/// or turns.
static s32 Kalay_UpdateTouristFidget(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  u32 action;
  u32 nextTimer;

  if ((s16)actor->userValue == 0) {
    action = (u32)(Import_RandomNext() * 8) >> 16;
    switch (action) {
    case 0:
      Actor_SelectAnimation(actor, 3);
      break;
    case 1:
      Actor_SelectAnimation(actor, 4);
      break;
    case 3:
    case 4:
      actor->facing += (u32)(Import_RandomNext() << 15) >> 16;
      break;
    }
    nextTimer = (u32)(Import_RandomNext() * 80) >> 16;
    actor->userValue = nextTimer;
  }
  if ((s16)actor->userValue != 0) {
    actor->userValue = (s16)actor->userValue - 1;
  }
  return 1;
}

// Shared source, included in link order.
#include "shared/actor_heading.h"

/// The party has heard in Lunpa that Hammet is held there, or he is already
/// free; it changes what Layana says.
/// ★ Named from the game: "you sneaked into Lunpa? That's far too dangerous!"
/// (MsgKalayIsaacSneakedIntoLunpaThats)
#define FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA FLAG_AREA_TEMP_21

const struct SceneEntrance *Kalay_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  if (sceneId == (s32)&SceneId_Kalay) {
    return gKalay_TownEntrances;
  }
  if (sceneId == (s32)&SceneId_HammetPalaceGrounds) {
    return gKalay_HammetPalaceEntrances;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelEntrance) {
    return gKalay_TunnelEntranceEntrances;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelCave) {
    return gKalay_TunnelCaveEntrances;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelPassage) {
    return gKalay_TunnelPassageEntrances;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelTreasureRoom) {
    return gKalay_TunnelEntrances;
  }
  return gKalay_NoEntrances;
}

const struct SceneExitRegion *Kalay_GetRegions(void) { return NULL; }

/// The numbered exits of all six scenes.
const u32 *Kalay_GetExits(void) { return gKalay_Exits; }

/// The people of Kalay, Hammet Palace's front, the Kalay Tunnel Entrance and
/// the Kalay Tunnel's treasure room.
const struct ActorPlacement *Kalay_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  if (sceneId == (s32)&SceneId_Kalay) {
    return gKalay_TownActorPlacements;
  }
  if (sceneId == (s32)&SceneId_HammetPalaceGrounds) {
    return gKalay_HammetPalaceActorPlacements;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelEntrance) {
    return gKalay_TunnelEntranceActorPlacements;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelTreasureRoom) {
    return gKalay_TunnelActorPlacements;
  }
  return gKalay_NoActorPlacements;
}

/// Searching the well in Kalay: the party peers in, and it is very clean.
static void Kalay_SearchWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgKalayVeryCleanMaintained, 1);
  Event_End();
}

/// Catch at the Kalay Tunnel Entrance pulls in the Nut floating there, until it
/// is caught (FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT).
static void Kalay_CatchTunnelEntranceNut(void) {
  Event_Start();
  Import_ActorSetPosition(ACTOR_TUNNEL_ENTRANCE_NUT, 0, 0);
  GameFlag_Set(FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  Event_End();
}

/// The conversations, exits and Psynergy targets of each scene.
const struct Trigger *Kalay_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  if (sceneId == (s32)&SceneId_Kalay) {
    return gKalay_TownTriggers;
  }
  if (sceneId == (s32)&SceneId_HammetPalaceGrounds) {
    return gKalay_HammetPalaceTriggers;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelEntrance) {
    return gKalay_TunnelEntranceTriggers;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelCave) {
    return gKalay_TunnelCaveTriggers;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelPassage) {
    return gKalay_TunnelPassageTriggers;
  }
  if (sceneId == (s32)&SceneId_KalayTunnelTreasureRoom) {
    return gKalay_TunnelTriggers;
  }
  return gKalay_NoTriggers;
}

/// A man in Kalay tells why he moved there: he left Tolbi, afraid of Babi, who
/// has ruled it for more than a hundred years.
static void Kalay_TalkAboutLeavingHome(void) {
  Event_Start();
  Event_SetMessageId(MsgKalayCameKalayBecauseDidntLike);
  Event_AskYesNoAndShowResponse(ACTOR_MAN_WHO_LEFT_TOLBI, 0);
  Event_End();
}

/// Spoken to from any side but the north, the guard at the Kalay Tunnel
/// Entrance asks `Do you want to go to the cave up ahead?`, and turns the
/// party away either way.
static void Kalay_AskAboutCaveAccess(void) {
  struct Actor *leader;
  u32 relativeFacing;
  u16 *message;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing + 0xfffff000) << 16;
  if (relativeFacing > 0x60000000) {
    Event_Start();
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_TUNNEL_GUARD, 0);
    Event_Wait(10);
    Event_SetMessageId(MsgKalayDoWantGoCaveUp);
    FieldMessage_ShowAtActor(ACTOR_TUNNEL_GUARD, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Actor_SetAnimationAndWait(ACTOR_TUNNEL_GUARD, ANIM_SHAKE_HEAD);
      Event_ShowMessageAndWait(ACTOR_TUNNEL_GUARD, 0);
    } else {
      message = &gFieldWork->message;
      *message = *message + 1;
      Actor_SetAnimationAndWait(ACTOR_TUNNEL_GUARD, ANIM_NOD);
      Event_ShowMessageAndWait(ACTOR_TUNNEL_GUARD, 0);
    }
    Event_End();
  }
}

/// A man asks whether Layana was too hard on the party. He's in the Kalay
/// Tunnel (KalayTunnelTreasureRoom) once Hammet is home, and means the time she
/// told the party to leave.
static void Kalay_TalkAboutLayana(void) {
  Event_Start();
  Event_SetMessageId(MsgKalayLayanaWasVeryHardOn);
  Event_AskYesNoAndShowResponse(ACTOR_WAITING_MAN, 0);
  Event_End();
}

/// In Kalay, across the counter, the leader facing north, the weapon shop
/// opens; from elsewhere the merchant grumbles that Hammet sells his best
/// weapons away.
static void Kalay_TalkToWeaponMerchant(void) {
  struct Actor *leader;
  u32 relativeFacing;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x2000) << 16;
  if (relativeFacing > 0x80000000) {
    Shop_Run(22, ACTOR_WEAPON_MERCHANT);
  } else {
    Event_Start();
    Event_SetMessageId(MsgKalayLordHammetSellsHisBest);
    Event_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
    Event_End();
  }
}

/// In Kalay, across the counter, the leader facing west to north, the armor
/// shop opens; from elsewhere the merchant asks whether the party found its
/// weapons.
static void Kalay_TalkToArmorMerchant(void) {
  struct Actor *leader;
  u32 relativeFacing;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x6001) << 16;
  if (relativeFacing <= 0x7ffe0000) {
    Shop_Run(23, ACTOR_ARMOR_MERCHANT);
  } else {
    Event_Start();
    Event_SetMessageId(MsgKalayDidFindNeededInWeapon);
    Event_AskYesNoAndShowResponse(ACTOR_ARMOR_MERCHANT, 0);
    Event_End();
  }
}

/// In Kalay, across the counter, the leader facing east, the item shop opens;
/// from elsewhere the merchant praises Lady Layana.
static void Kalay_TalkToItemMerchant(void) {
  struct Actor *leader;
  u32 relativeFacing;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x2000) << 16;
  if (relativeFacing > 0xC0000000) {
    Shop_Run(24, ACTOR_ITEM_MERCHANT);
  } else {
    Event_Start();
    Event_SetMessageId(MsgKalayLadyLayanaSharedInLord);
    Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
    Event_End();
  }
}

/// Walking north into one of Kalay's eight doors (step triggers 1 to 8), into
/// the houses, the inn or the Sanctum: everyone stops, the door opens and the
/// leader walks in.
static void Kalay_OpenDoorAndExit(void) {
  struct FieldWork *work;
  struct Actor *actor;
  s16 *trigger;
  u32 i;
  s32 door;

  work = gFieldWork;
  Event_Start();
  for (i = ACTOR_FIRST_PLACED; i <= ACTOR_LAST_ID; i++) {
    actor = Actor_GetById(i);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  trigger = &work->stepTrigger;
  door = *trigger - 1;
  Sound_Play(SE_DOOR_OPEN);
  {
    s32 x = gKalay_DoorCells[door].x;
    s32 y = gKalay_DoorCells[door].y;

    MapGrid_AnimateCells(gKalay_DoorCells[door].steps, x, y);
  }
  Actor_SetSpeedParameters(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// The palace door opens in two frames.
static __inline__ void Kalay_AnimatePalaceDoor(void) {
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(36, 23, 43, 12, 2, 2);
  wait(5);
  MapGrid_CopyCellsTo(39, 23, 43, 12, 2, 2);
  wait(5);
}

/// Walking north into the door of Hammet Palace (HammetPalaceGrounds,
/// step trigger 2): it opens and the leader walks in.
static void Kalay_EnterPalace(void) {
  Event_Start();
  Kalay_AnimatePalaceDoor();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Leaving Kalay (step triggers 9 and 10) after hearing of the Colosso tour at
/// the inn, until the tour has left: the party watches the tour group stop. The
/// guides count everyone, pace back and forth while the tourists grumble, give
/// up on the missing man and lead the group off; the late tourist then runs
/// after it. Otherwise the party leaves.
static void Kalay_WatchTourGroupDeparture(void) {
  s32 message;
  struct Actor *headWait;
  struct Actor *lastWait;
  struct Actor *closing;
  struct Actor *late;
  struct Actor *headEnd;
  struct Actor *lastEnd;
  struct Actor *leader;
  struct Actor *last;
  struct Actor *head;
  struct Actor *headBack;
  struct Actor *headAgain;
  struct Actor *headHome;
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  if (GameFlag_Get(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR) != 0 &&
      GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0) {
    Event_CallWithLastActiveActorId(gKalay_TourGroupActorPlacements);
    Actor_SetPosition(ACTOR_HEAD_GUIDE, PIXELS(252), PIXELS(272));
    Actor_SetPosition(ACTOR_FIRST_TOURIST, PIXELS(284), PIXELS(264));
    Actor_SetPosition(ACTOR_FIRST_TOURIST + 1, PIXELS(284), PIXELS(280));
    Actor_SetPosition(ACTOR_FIRST_TOURIST + 2, PIXELS(300), PIXELS(264));
    Actor_SetPosition(ACTOR_FIRST_TOURIST + 3, PIXELS(300), PIXELS(280));
    Actor_SetPosition(ACTOR_FIRST_TOURIST + 5, PIXELS(316), PIXELS(264));
    Actor_SetPosition(ACTOR_FIRST_TOURIST + 4, PIXELS(316), PIXELS(280));
    Actor_SetPosition(ACTOR_FIRST_TOURIST + 6, PIXELS(332), PIXELS(264));
    Actor_SetPosition(ACTOR_OURANOS, PIXELS(332), PIXELS(280));
    Actor_SetPosition(ACTOR_SECOND_GUIDE, PIXELS(364), PIXELS(272));
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    Message_SetVoiceFromActor(ACTOR_HEAD_GUIDE);
    FieldMessage_ShowCentered(MsgKalayOkListeningLets, 1, 0);
    Event_PlaySound(MUS_DJINN_LESSON);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    if (work->stepTrigger == 9) {
      FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
    } else {
      FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
    }
    // Stagger the speeds so the group arrives in rows.
    Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(11) / 10, PIXELS(11) / 20);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 1, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 2, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 3, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 5, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 4, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 6, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_SetSpeed(ACTOR_OURANOS, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_SetScript(ACTOR_HEAD_GUIDE, gKalay_TourGroupWalkScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST, gKalay_TourGroupWalkScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 1, gKalay_TourGroupWalkScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 2, gKalay_TourGroupWalkScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 3, gKalay_TourGroupWalkScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 5, gKalay_TourGroupWalkScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 4, gKalay_TourGroupWalkScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 6, gKalay_TourGroupWalkScript);
    Actor_SetScript(ACTOR_OURANOS, gKalay_TourGroupWalkScript);
    last = Actor_Get(ACTOR_SECOND_GUIDE);
    last->timer = 0;
    Actor_SetScript(ACTOR_SECOND_GUIDE, gKalay_TourGroupWalkScript);
    FieldCamera_MoveTo(PIXELS(186), -1, PIXELS(272), 1);
    Actor_RefreshSelectorById(ACTOR_HEAD_GUIDE);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
    do {
      wait(1);
    } while (last->timer == 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST, 2);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST, FACING_SOUTH + FACING_STEP, 20);
    Event_SetMessageId(MsgKalayWhyWeStoppingAtPlace);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST + 1, 2);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_NORTHWEST + FACING_STEP,
                        10);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST + 1, ANIM_NOD);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 1, 0, 10);
    Actor_SetAttachedEffect(ACTOR_FIRST_TOURIST + 5,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 5, 0, 10);
    Actor_ShowEmote(ACTOR_FIRST_TOURIST + 4, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    40);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_NORTHWEST + FACING_STEP,
                        10);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 4, 0, 10);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_WEST, 10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST + 4, 2);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST + 4, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 4, 0, 10);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_NORTHWEST + FACING_STEP,
                        0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_SOUTH + FACING_STEP,
                        20);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST + 4, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_FIRST_TOURIST + 5, ANIM_NOD);
    Actor_RunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    Actor_SetAttachedEffect(ACTOR_HEAD_GUIDE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    head = Actor_Get(ACTOR_HEAD_GUIDE);
    head->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 172, 264);
    Event_Wait(1);
    headBack = Actor_Get(ACTOR_HEAD_GUIDE);
    headBack->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_FIRST_TOURIST, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_WEST, 20);
    Actor_SetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    Event_Wait(20);
    headAgain = Actor_Get(ACTOR_HEAD_GUIDE);
    headAgain->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 172, 272);
    Event_Wait(1);
    headHome = Actor_Get(ACTOR_HEAD_GUIDE);
    headHome->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 180, 272);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_ShowEmote(ACTOR_OURANOS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_RunRepeatedMotion(ACTOR_OURANOS, 1);
    Actor_SetAnimationAndWait(ACTOR_OURANOS, ANIM_NOD);
    FieldEvent_ShowMessageThenWait(ACTOR_OURANOS, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST + 6, 1);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 6, 0, 10);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST + 6, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 6, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_SECOND_GUIDE, 2);
    Actor_ShowEmote(ACTOR_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldEvent_ShowMessageThenWait(ACTOR_SECOND_GUIDE, 0, 10);
    Actor_Jump(ACTOR_HEAD_GUIDE, 2, 20);
    Actor_Jump(ACTOR_HEAD_GUIDE, 4, 40);
    Actor_RunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 265, 282);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 251, 284);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 246, 296);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 0);
    // The tourists fidget while the guides count them.
    Kalay_StartTouristsFidgeting();
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 228, 296);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 212, 296);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 192, 296);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
    Actor_StartRepeatedMotion(ACTOR_SECOND_GUIDE, 2);
    Actor_ShowEmote(ACTOR_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 184, 286);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_SetAttachedEffect(ACTOR_SECOND_GUIDE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_SHAKE_HEAD);
    Actor_ShowEmote(ACTOR_HEAD_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_SetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    Actor_RunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_EAST, 60);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_SetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_EAST, 0);
    Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_RunScriptAndWait(ACTOR_HEAD_GUIDE, gKalay_GuidePaceScript);
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 228, 296);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 212, 296);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 192, 296);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_SetAttachedEffect(ACTOR_HEAD_GUIDE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_SHAKE_HEAD);
    message = MsgKalayShortOnePerson;
    Event_SetMessageId(message);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 40);
    // The guides pace while the tourists complain one by one.
    Kalay_StartGuidesPacing();
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 178, 272);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
    Event_Wait(240);
    Actor_Stop(ACTOR_FIRST_TOURIST);
    wait(1);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST, FACING_WEST, 10);
    Actor_ShowEmote(ACTOR_FIRST_TOURIST, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST, 0, 10);
    Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST);
    Event_Wait(80);
    Actor_Stop(ACTOR_FIRST_TOURIST + 1);
    wait(1);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_NORTH + FACING_STEP,
                        20);
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST + 1, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 1, 0, 10);
    Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 1);
    Event_Wait(160);
    Actor_Stop(ACTOR_FIRST_TOURIST + 5);
    wait(1);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_SOUTH + FACING_STEP,
                        10);
    Actor_ShowEmote(ACTOR_FIRST_TOURIST + 5, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    60);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 5, 0, 10);
    Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 5);
    Event_Wait(80);
    Actor_Stop(ACTOR_FIRST_TOURIST + 3);
    wait(1);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 3, FACING_NORTHWEST + FACING_STEP,
                        10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST + 3, 1);
    Event_SetMessageId(message + 6);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 3, 0, 10);
    Task_Remove(Kalay_AlternateGuidesPacing);
    Actor_Stop(ACTOR_HEAD_GUIDE);
    Actor_Stop(ACTOR_SECOND_GUIDE);
    wait(1);
    headWait = Actor_Get(ACTOR_HEAD_GUIDE);
    headWait->timer = 0;
    lastWait = Actor_Get(ACTOR_SECOND_GUIDE);
    lastWait->timer = 0;
    Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetScript(ACTOR_HEAD_GUIDE, gKalay_HeadGuideStopScript);
    Actor_SetScript(ACTOR_SECOND_GUIDE, gKalay_SecondGuideStopScript);
    Actor_Stop(ACTOR_FIRST_TOURIST + 2);
    wait(1);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 2, FACING_SOUTH + FACING_STEP,
                        10);
    message += 5;
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST + 2, 2);
    Event_SetMessageId(message);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 2, 0, 20);
    Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 2);
    Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 3);
    do {
      wait(1);
    } while (Actor_Get(ACTOR_HEAD_GUIDE)->timer == 0 ||
             Actor_Get(ACTOR_SECOND_GUIDE)->timer != 1);
    Actor_SetScript(ACTOR_HEAD_GUIDE, gKalay_HeadGuideReturnScript);
    Actor_SetScript(ACTOR_SECOND_GUIDE, gKalay_SecondGuideReturnScript);
    Actor_Stop(ACTOR_FIRST_TOURIST + 4);
    wait(1);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_SOUTH + FACING_STEP,
                        10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST + 4, 1);
    Actor_SetAnimationAndWait(ACTOR_FIRST_TOURIST + 4, ANIM_SHAKE_HEAD);
    message = MsgKalayDecideEnteringColosso;
    Event_SetMessageId(message);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 4, 0, 10);
    Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 4);
    Actor_Stop(ACTOR_OURANOS);
    Actor_Stop(ACTOR_FIRST_TOURIST + 6);
    wait(1);
    Actor_ShowEmote(ACTOR_OURANOS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    Actor_ShowEmote(ACTOR_FIRST_TOURIST + 6, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                    60);
    Actor_FaceDirection(ACTOR_OURANOS, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 6, FACING_SOUTH + FACING_STEP,
                        10);
    message += 3;
    Actor_SetAnimationAndWait(ACTOR_OURANOS, ANIM_SHAKE_HEAD);
    Event_SetMessageId(message);
    FieldEvent_ShowMessageThenWait(ACTOR_OURANOS, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_FIRST_TOURIST + 6, 1);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST + 6, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 6, 0, 10);
    Actor_ShowEmote(ACTOR_OURANOS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_ShowEmote(ACTOR_HEAD_GUIDE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_RunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    Event_SetMessageId(MsgKalayThatsWeCantWaitAny);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_Stop(ACTOR_FIRST_TOURIST);
    Actor_Stop(ACTOR_FIRST_TOURIST + 1);
    Actor_Stop(ACTOR_FIRST_TOURIST + 2);
    Actor_Stop(ACTOR_FIRST_TOURIST + 3);
    Actor_Stop(ACTOR_FIRST_TOURIST + 5);
    Actor_Stop(ACTOR_FIRST_TOURIST + 4);
    Actor_Stop(ACTOR_FIRST_TOURIST + 6);
    Actor_Stop(ACTOR_OURANOS);
    Actor_Stop(ACTOR_HEAD_GUIDE);
    Actor_Stop(ACTOR_SECOND_GUIDE);
    wait(1);
    Actor_Jump(ACTOR_FIRST_TOURIST, 2, 0);
    Actor_Jump(ACTOR_FIRST_TOURIST + 1, 2, 0);
    Actor_Jump(ACTOR_FIRST_TOURIST + 2, 2, 0);
    Actor_Jump(ACTOR_FIRST_TOURIST + 3, 2, 0);
    Actor_Jump(ACTOR_FIRST_TOURIST + 5, 2, 0);
    Actor_Jump(ACTOR_FIRST_TOURIST + 4, 2, 0);
    Actor_Jump(ACTOR_FIRST_TOURIST + 6, 2, 0);
    Actor_Jump(ACTOR_OURANOS, 2, 0);
    Actor_Jump(ACTOR_SECOND_GUIDE, 2, 40);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 2, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 3, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 6, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_OURANOS, FACING_WEST, 40);
    Actor_Jump(ACTOR_SECOND_GUIDE, 4, 40);
    FieldEvent_ShowMessageThenWait(ACTOR_SECOND_GUIDE, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_HEAD_GUIDE, 1);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
    FieldEvent_ShowMessageThenWait(ACTOR_SECOND_GUIDE, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_ShowEmote(ACTOR_FIRST_TOURIST, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Actor_StartRepeatedMotion(ACTOR_FIRST_TOURIST, 1);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST, 0, 10);
    Actor_ShowEmote(ACTOR_FIRST_TOURIST + 1, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    FieldEvent_ShowMessageThenWait(ACTOR_FIRST_TOURIST + 1, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_SHAKE_HEAD);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
    FieldEvent_ShowMessageThenWait(ACTOR_SECOND_GUIDE, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_NORTHWEST + FACING_STEP,
                        4);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 2, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 3, FACING_NORTHWEST + FACING_STEP,
                        4);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_NORTHWEST + FACING_STEP,
                        4);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 6, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_OURANOS, FACING_NORTHWEST + FACING_STEP, 4);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_FIRST_TOURIST + 1, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST + 2, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_FIRST_TOURIST + 3, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST + 5, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_FIRST_TOURIST + 4, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_FIRST_TOURIST + 6, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_OURANOS, ANIM_NOD);
    Actor_Jump(ACTOR_HEAD_GUIDE, 2, 40);
    FieldEvent_ShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_WEST, 4);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 2, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 3, FACING_WEST, 4);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_WEST, 4);
    Actor_FaceDirection(ACTOR_FIRST_TOURIST + 6, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_OURANOS, FACING_WEST, 4);
    Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(11) / 10, PIXELS(11) / 20);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST, PIXELS(21) / 20, PIXELS(21) / 40);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 1, PIXELS(21) / 20, PIXELS(21) / 40);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 2, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 3, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 5, PIXELS(19) / 20, PIXELS(19) / 40);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 4, PIXELS(19) / 20, PIXELS(19) / 40);
    Actor_SetSpeed(ACTOR_FIRST_TOURIST + 6, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(ACTOR_OURANOS, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(17) / 20, PIXELS(17) / 40);
    Actor_SetSpritePriority(ACTOR_FIRST_TOURIST, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_TOURIST + 1, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_TOURIST + 2, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_TOURIST + 3, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_TOURIST + 5, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_TOURIST + 4, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_TOURIST + 6, 1);
    Actor_SetSpritePriority(ACTOR_OURANOS, 1);
    Actor_SetSpritePriority(ACTOR_HEAD_GUIDE, 1);
    Actor_SetSpritePriority(ACTOR_SECOND_GUIDE, 1);
    Actor_Stop(ACTOR_FIRST_TOURIST);
    Actor_Stop(ACTOR_FIRST_TOURIST + 1);
    Actor_Stop(ACTOR_FIRST_TOURIST + 2);
    Actor_Stop(ACTOR_FIRST_TOURIST + 3);
    Actor_Stop(ACTOR_FIRST_TOURIST + 5);
    Actor_Stop(ACTOR_FIRST_TOURIST + 4);
    Actor_Stop(ACTOR_FIRST_TOURIST + 6);
    Actor_Stop(ACTOR_OURANOS);
    Actor_Stop(ACTOR_HEAD_GUIDE);
    Actor_Stop(ACTOR_SECOND_GUIDE);
    wait(1);
    // Send the group onward, then bring in the missing tourist.
    Actor_SetScript(ACTOR_HEAD_GUIDE, gKalay_TourGroupLeaveScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST, gKalay_TourGroupLeaveScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 1, gKalay_TourGroupLeaveScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 2, gKalay_TourGroupLeaveScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 3, gKalay_TourGroupLeaveScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 5, gKalay_TourGroupLeaveScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 4, gKalay_TourGroupLeaveScript);
    Actor_SetScript(ACTOR_FIRST_TOURIST + 6, gKalay_TourGroupLeaveScript);
    Actor_SetScript(ACTOR_OURANOS, gKalay_TourGroupLeaveScript);
    closing = Actor_Get(ACTOR_SECOND_GUIDE);
    closing->timer = 0;
    Actor_SetScript(ACTOR_SECOND_GUIDE, gKalay_TourGroupLeaveScript);
    do {
      wait(1);
    } while (Actor_Get(ACTOR_SECOND_GUIDE)->timer != 1);
    Event_Wait(80);
    // The late tourist finds that the rest of the group has already left.
    Actor_SetPosition(ACTOR_LATE_TOURIST, PIXELS(340), PIXELS(274));
    wait(1);
    Actor_SetSpeed(ACTOR_LATE_TOURIST, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_LATE_TOURIST, 224, 274);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_NORTH, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_SOUTH + FACING_STEP, 40);
    Actor_ShowEmote(ACTOR_LATE_TOURIST, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    FieldEvent_ShowMessageThenWait(ACTOR_LATE_TOURIST, 0, 10);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_NORTH, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_WEST, 40);
    Actor_SetAttachedEffect(ACTOR_LATE_TOURIST,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_Jump(ACTOR_LATE_TOURIST, 4, 40);
    FieldEvent_ShowMessageThenWait(ACTOR_LATE_TOURIST, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_LATE_TOURIST, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_LATE_TOURIST, 0, 10);
    Actor_Jump(ACTOR_LATE_TOURIST, 4, 40);
    Actor_SetSpeed(ACTOR_LATE_TOURIST, PIXELS(6) / 5, PIXELS(3) / 5);
    closing = Actor_Get(ACTOR_LATE_TOURIST);
    closing->timer = 0;
    Actor_SetScript(ACTOR_LATE_TOURIST, gKalay_TourGroupLeaveScript);
    do {
      wait(1);
    } while (Actor_Get(ACTOR_LATE_TOURIST)->timer != 1);
    Actor_SetPosition(ACTOR_LATE_TOURIST, PIXELS(359), PIXELS(314));
    late = Actor_Get(ACTOR_LATE_TOURIST);
    late->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(ACTOR_HEAD_GUIDE, PIXELS(455), PIXELS(434));
    headEnd = Actor_Get(ACTOR_HEAD_GUIDE);
    headEnd->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(ACTOR_SECOND_GUIDE, PIXELS(464), PIXELS(416));
    lastEnd = Actor_Get(ACTOR_SECOND_GUIDE);
    lastEnd->facing = FACING_SOUTH + FACING_STEP;
    Actor_ReleaseById(ACTOR_FIRST_TOURIST);
    Actor_ReleaseById(ACTOR_FIRST_TOURIST + 1);
    Actor_ReleaseById(ACTOR_FIRST_TOURIST + 2);
    Actor_ReleaseById(ACTOR_FIRST_TOURIST + 3);
    Actor_ReleaseById(ACTOR_FIRST_TOURIST + 4);
    Actor_ReleaseById(ACTOR_FIRST_TOURIST + 5);
    Actor_ReleaseById(ACTOR_FIRST_TOURIST + 6);
    Actor_ReleaseById(ACTOR_OURANOS);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    if (work->stepTrigger == 9) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 224, 458);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      leader->facing = FACING_NORTH;
    } else {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 248);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      leader->facing = FACING_SOUTH;
    }
    Scene_PlayMusic();
    GameFlag_Set(FLAG_KALAY_COLOSSO_TOUR_LEFT);
  } else {
    Event_PlaySound(SE_MAP_EXIT);
    Import_EventSetExitRequest(work->stepTrigger);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
  }
  Event_End();
}

/// Touching step trigger 10 in the Kalay Tunnel (KalayTunnelTreasureRoom) stops
/// the fade overlay following the leader.
static void Kalay_StopTrackingLeader(void) {
  if (Event_GetFlag(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER) != 0) {
    gFadeOverlayWork->actor = NULL;
    GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
}

/// Touching step trigger 11 in the Kalay Tunnel (KalayTunnelTreasureRoom) makes
/// the fade overlay follow the leader.
static void Kalay_StartTrackingLeader(void) {
  struct FadeOverlayWork *work;

  if (Event_GetFlag(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER) == 0) {
    work = gFadeOverlayWork;
    work->actor = Actor_GetById(ACTOR_PARTY_LEADER);
    GameFlag_Set(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
}

/// Step trigger 19 in the Kalay Tunnel (KalayTunnelTreasureRoom): the leader's
/// depth priority follows his position.
static void Kalay_EnableLeaderAutomaticPriority(void) {
  u8 *priorityFlags;
  s32 flags;

  priorityFlags = &Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags;
  flags = ACTOR_PRIORITY_AUTOMATIC;
  flags |= *priorityFlags;
  *priorityFlags = flags;
}

/// Step trigger 18 in the Kalay Tunnel (KalayTunnelTreasureRoom): the leader's
/// depth priority stays fixed.
static void Kalay_DisableLeaderAutomaticPriority(void) {
  u8 *priorityFlags;
  s32 flags;

  priorityFlags = &Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags;
  flags = (u8)~ACTOR_PRIORITY_AUTOMATIC;
  flags &= *priorityFlags;
  *priorityFlags = flags;
}

/// Catch in Kalay pulls in the Nut circling there, until it is caught.
static void Kalay_CatchTownNut(void) {
  Task_Remove(Kalay_UpdateNutOrbit);
  wait(1);
  Import_ActorSetPosition(ACTOR_TOWN_NUT, 0, 0);
  GameFlag_Set(FLAG_KALAY_TOWN_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
}

/// Every scene of the overlay sets FLAG_KALAY_REACHED, then prepares its own
/// people and cells.
s32 Kalay_Initialize(void) {
  s32 sceneId;

  GameFlag_Set(FLAG_KALAY_REACHED);
  sceneId = gMainRoutineWork.sceneId;
  if (sceneId == (s32)&SceneId_Kalay) {
    Kalay_PrepareTown();
  } else if (sceneId == (s32)&SceneId_HammetPalaceGrounds) {
    Kalay_PrepareHammetPalace();
  } else if (sceneId == (s32)&SceneId_KalayTunnelEntrance) {
    Kalay_PrepareTunnelEntrance();
  } else if (sceneId == (s32)&SceneId_KalayTunnelPassage) {
    Kalay_StartWindowTransition();
  } else if (sceneId == (s32)&SceneId_KalayTunnelTreasureRoom) {
    Kalay_PrepareTunnel();
  }
  return 0;
}

/// Sets up Kalay. Having heard in Lunpa that Hammet is held there sets
/// area temp flag FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA; once Hammet is
/// free (FLAG_DODONPA_LOCKED_UP) the palace's FLAG_IVAN_REPORTED_TO_LAYANA,
/// FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS and FLAG_KALAY_TUNNEL_OPEN are
/// set too, so its earlier events count as seen and the passage is open. Coming
/// up from the Kalay Tunnel (KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM) finds its
/// statue moved aside, and until it is caught the Nut circles in the air.
static void Kalay_PrepareTown(void) {
  OrbitingMapActor *nut;
  s32 found;
  u8 *heap;
  OrbitingMapActorSprite *sprite;

  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    GameFlag_Set(FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA);
    GameFlag_Set(FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS);
    GameFlag_Set(FLAG_IVAN_REPORTED_TO_LAYANA);
    GameFlag_Set(FLAG_KALAY_TUNNEL_OPEN);
  }
  if (GameFlag_Get(FLAG_LUNPA_HEARD_OF_PRISONER) != 0) {
    GameFlag_Set(FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA);
  }
  if (gMainRoutineWork.entrance == KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM) {
    Actor_SetPosition(ACTOR_TOWN_STATUE, PIXELS(424), PIXELS(88));
  }
  // The second guide shares the first's sprite, drawn with its green and blue
  // swapped to tell them apart.
  Actor_ConfigureSpriteColor(ACTOR_SECOND_GUIDE, SPRITE_COLOR_RBG);
  found = GameFlag_Get(FLAG_KALAY_TOWN_NUT_CAUGHT);
  if (found != 0) {
    Import_ActorSetPosition(ACTOR_TOWN_NUT, 0, 0);
  } else {
    nut = (OrbitingMapActor *)Actor_GetById(ACTOR_TOWN_NUT);
    sprite = nut->sprite;
    sprite->priority = 1;
    sprite->fullColor = 0;
    sprite->paletteNumber = 0;
    sprite->state = found;
    nut->active = 1;
    nut->mode = found;
    nut->y = PIXELS(10);
    nut->visible = 1;
    heap = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(ITEM_NUT);
    VramCache_Load(sprite->palette, 128, heap + 0x400);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
    nut->orbitAngle = found;
    nut->orbitCenterX = nut->x;
    nut->orbitCenterY = nut->y;
    nut->orbitCenterZ = nut->z;
    Task_AddFrame(Kalay_UpdateNutOrbit, TASK_PASS_FRAME);
  }
}

/// Sets up Hammet Palace's front (HammetPalaceGrounds): the palace flags as in
/// the town, and, the first time the party comes before Ivan has reported to
/// Layana, the guards event.
static void Kalay_PrepareHammetPalace(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    GameFlag_Set(FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA);
    GameFlag_Set(FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS);
    GameFlag_Set(FLAG_IVAN_REPORTED_TO_LAYANA);
    GameFlag_Set(FLAG_KALAY_TUNNEL_OPEN);
  }
  if (GameFlag_Get(FLAG_LUNPA_HEARD_OF_PRISONER) != 0) {
    GameFlag_Set(FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA);
  }
  if (gMainRoutineWork.entrance != ENTRANCE_DEFAULT) {
    if (GameFlag_Get(FLAG_IVAN_REPORTED_TO_LAYANA) == 0) {
      Kalay_IvanMeetsPalaceGuards();
    }
  }
}

/// Sets up the Kalay Tunnel Entrance (KalayTunnelEntrance): the Nut floats
/// until caught, the guard turns east once FLAG_KALAY_TUNNEL_OPEN opens the
/// passage, and arriving by KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH brings
/// Hammet home in Bunza's wagon.
static void Kalay_PrepareTunnelEntrance(void) {
  struct Actor *guard;

  if (GameFlag_Get(FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT) == 0) {
    Scene_FloatNut(ACTOR_TUNNEL_ENTRANCE_NUT);
  }
  if (GameFlag_Get(FLAG_KALAY_TUNNEL_OPEN) != 0) {
    guard = Actor_GetById(ACTOR_TUNNEL_GUARD);
    guard->facing = 0;
  }
  if (gMainRoutineWork.entrance == KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH) {
    Kalay_HammetReturnsToPalace();
  }
}

/// Sets up the Kalay Tunnel's passage (KalayTunnelPassage): it opens with a
/// window transition, its way through is drawn open once FLAG_KALAY_TUNNEL_OPEN
/// is set, and arriving from the treasure room
/// (KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_WEST and
/// KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_EAST) or by the stairs
/// from the north-east room
/// (KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM) starts
/// SE_AMBIENT_WATER.
static void Kalay_StartWindowTransition(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_KALAY_TUNNEL_OPEN) != 0) {
    MapGrid_CopyCellsTo(58, 5, 58, 8, 2, 3);
    {
      MapGrid_CopyCellAttributes(8, 11, 2, 1, 8, 10);
    }
    MapGrid_CopyCellsTo(8, 12, 8, 11, 2, 1);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  if (gMainRoutineWork.entrance <=
      KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM) {
    Scene_QueueMusic(SE_AMBIENT_WATER);
  }
}

/// Sets up the Kalay Tunnel's treasure room (KalayTunnelTreasureRoom): the fade
/// overlay follows the leader unless he came down from the town
/// (KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY); the pushed statues, the frozen
/// puddle and the opened passages are restored, and once FLAG_KALAY_TUNNEL_OPEN
/// is set the man waits here and some of the cells are redrawn.
static void Kalay_PrepareTunnel(void) {
#if TBS_PATCH_1
  s32 resumedMap;
#endif
  struct Actor *actor;
  s32 trackingValue;
  s32 column;
  struct FadeOverlayWork *effectWork;
  // HEAP_SLOT_FADE_OVERLAY is nine heap slots after HEAP_SLOT_FIELD; index
  // the actual heap slot array, not adjacent storage reached past the scalar
  // gFieldWork declaration.
  void **workSlots = gHeapSlots + HEAP_SLOT_FIELD;

  ((struct FieldWork *)workSlots[0])->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  FieldEffect_StartFadeOverlay(0);
#if TBS_PATCH_1
  resumedMap = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (resumedMap != 0) {
    effectWork = workSlots[HEAP_SLOT_FADE_OVERLAY - HEAP_SLOT_FIELD];
    trackingValue = GameFlag_Get(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
#ifdef UBFIX
    if (trackingValue != 0) {
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
    } else {
      actor = NULL;
    }
    effectWork->actor = actor;
#else
    // The original integer temporary carries an actor pointer too. The
    // typed branch adds a jump and a zero load in the native English trial.
    if (trackingValue != 0) {
      trackingValue = Actor_GetById(ACTOR_PARTY_LEADER);
    }
    effectWork->actor = (struct Actor *)trackingValue;
#endif
  } else {
    Event_SetFlag(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
    if (gMainRoutineWork.entrance == KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY) {
      ((struct FadeOverlayWork *)
           workSlots[HEAP_SLOT_FADE_OVERLAY - HEAP_SLOT_FIELD])
          ->actor = NULL;
      GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
    }
  }
#else
  Event_SetFlag(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) != 0) {
    effectWork = workSlots[HEAP_SLOT_FADE_OVERLAY - HEAP_SLOT_FIELD];
    trackingValue = Event_GetFlag(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
#ifdef UBFIX
    if (trackingValue != 0) {
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
    } else {
      actor = NULL;
    }
    effectWork->actor = actor;
#else
    // As in the earlier-edition branch, the integer also carries a pointer.
    if (trackingValue != 0) {
      trackingValue = Actor_GetById(ACTOR_PARTY_LEADER);
    }
    effectWork->actor = (struct Actor *)trackingValue;
#endif
  } else if (gMainRoutineWork.entrance ==
             KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY) {
    ((struct FadeOverlayWork *)
         workSlots[HEAP_SLOT_FADE_OVERLAY - HEAP_SLOT_FIELD])
        ->actor = NULL;
    GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
#endif
  if (GameFlag_Get(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) != 0) {
    Actor_SetPosition(ACTOR_PUDDLE, PIXELS(150), PIXELS(728));
    if (GameFlag_Get(FLAG_TUNNEL_PUDDLE_FROZEN) != 0) {
      Actor_Get(ACTOR_PUDDLE);
      column = 9;
      Import_ActorSetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_FROZEN);
      MapGrid_CopyCellAttributes(0, 0, 1, 1, column, 14);
      MapGrid_CopyCellAttributes(0, 0, 1, 1, column, 45);
      {
        u8 *p = &Actor_GetById(ACTOR_PUDDLE)->priorityFlags;
        s32 v = ACTOR_PRIORITY_UNDERFOOT;

        v |= *p;
        *p = v;
      }
    }
  }
  actor = Actor_Get(ACTOR_FIRST_STATUE);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_SECOND_STATUE);
  Actor_SetSpriteFlags(actor, 0);
  Task_AddFrame(Kalay_UpdateTunnelEmitters, TASK_PASS_FRAME);
  if (GameFlag_Get(FLAG_KALAY_TUNNEL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_WAITING_MAN, PIXELS(426), PIXELS(730));
    actor = Actor_Get(ACTOR_WAITING_MAN);
    {
      u16 shown = FACING_SOUTH + FACING_STEP;

      actor->facing = shown;
    }
    MapGrid_CopyCellsTo(88, 48, 88, 45, 2, 3);
    MapGrid_CopyCellsTo(24, 49, 24, 48, 2, 1);
    MapGrid_CopyCellsTo(25, 42, 25, 47, 1, 1);
    MapGrid_CopyCellAttributes(22, 50, 2, 1, 24, 49);
  }
  if (GameFlag_Get(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) != 0) {
    Actor_SetPosition(ACTOR_FIRST_STATUE, PIXELS(232), PIXELS(732));
    MapGrid_CopyCellAttributes(7, 44, 1, 1, 0, 1);
    MapGrid_CopyCellsTo(74, 58, 78, 41, 1, 5);
    MapGrid_CopyCellsTo(16, 109, 13, 109, 3, 2);
    MapGrid_CopyCellsTo(67, 64, 71, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 72, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 68, 73, 43, 1, 2);
    MapGrid_CopyCellsTo(67, 68, 74, 43, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 75, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 66, 76, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 77, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 78, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 79, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 66, 80, 44, 1, 2);
    MapGrid_CopyCellsTo(2, 0, 9, 42, 2, 2);
    MapGrid_CopyCellsTo(68, 64, 71, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 72, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 68, 73, 43, 1, 2);
    MapGrid_CopyCellsTo(68, 68, 74, 43, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 75, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 66, 76, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 77, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 78, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 79, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 66, 80, 44, 1, 2);
    MapGrid_CopyCellsTo(4, 0, 9, 42, 2, 2);
    MapGrid_CopyCellsTo(7, 11, 7, 42, 10, 8);
    MapGrid_CopyCellsTo(71, 12, 71, 43, 10, 13);
    MapGrid_CopyCellAttributes(6, 13, 12, 12, 6, 44);
    MapGrid_CopyCellAttributes(0, 1, 1, 1, 7, 44);
  } else {
    switch (gMainRoutineWork.entrance) {
    case KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE:
    case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    }
  }
  if (GameFlag_Get(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) != 0) {
    Actor_SetPosition(ACTOR_SECOND_STATUE, PIXELS(696), PIXELS(732));
    MapGrid_CopyCellsTo(74, 58, 107, 41, 1, 5);
    MapGrid_CopyCellsTo(45, 109, 42, 109, 3, 2);
    MapGrid_CopyCellsTo(67, 64, 102, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 103, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 104, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 66, 105, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 106, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 107, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 64, 108, 44, 1, 2);
    MapGrid_CopyCellsTo(67, 66, 109, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 102, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 103, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 104, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 66, 105, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 106, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 107, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 64, 108, 44, 1, 2);
    MapGrid_CopyCellsTo(68, 66, 109, 44, 1, 2);
    MapGrid_CopyCellsTo(38, 14, 38, 44, 8, 4);
    MapGrid_CopyCellsTo(102, 14, 102, 44, 8, 12);
    MapGrid_CopyCellAttributes(37, 13, 10, 12, 37, 43);
  } else {
    switch (gMainRoutineWork.entrance) {
    case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST:
    case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    }
  }
}

/// All eight tourists in Kalay start fidgeting.
static void Kalay_StartTouristsFidgeting(void) {
  Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST);
  Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 1);
  Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 2);
  Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 3);
  Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 5);
  Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 4);
  Kalay_StartTouristFidgeting(ACTOR_FIRST_TOURIST + 6);
  Kalay_StartTouristFidgeting(ACTOR_OURANOS);
}

/// A tourist starts fidgeting after a random pause.
static void Kalay_StartTouristFidgeting(s32 actorId) {
  struct Actor *actor;
  u32 random;

  actor = Actor_GetById(actorId);
  actor->timer = actorId;
  random = Import_RandomNext();
  actor->userValue = (random * 5) >> 12;
  Actor_StartScript(actor, gKalay_TouristFidgetScript);
}

/// The guides speed up and start pacing in turn.
static void Kalay_StartGuidesPacing(void) {
  gKalay_DanceStep = 0;
  Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
  Task_AddFrame(Kalay_AlternateGuidesPacing, TASK_PASS_FRAME);
}

/// Each guide paces in turn, starting when the other finishes.
static void Kalay_AlternateGuidesPacing(void) {
  switch (gKalay_DanceStep) {
  case 0:
    Kalay_ClearScriptDone(Actor_GetById(ACTOR_SECOND_GUIDE));
    Actor_SetScript(ACTOR_SECOND_GUIDE, gKalay_GuideCountScript);
    gKalay_DanceStep++;
    break;
  case 1:
    if (Actor_GetById(ACTOR_SECOND_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(Actor_GetById(ACTOR_HEAD_GUIDE));
      Actor_SetScript(ACTOR_HEAD_GUIDE, gKalay_GuidePaceScript);
      gKalay_DanceStep++;
    }
    break;
  case 2:
    if (Actor_GetById(ACTOR_HEAD_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(Actor_GetById(ACTOR_HEAD_GUIDE));
      Actor_SetScript(ACTOR_HEAD_GUIDE, gKalay_GuideCountScript);
      gKalay_DanceStep++;
    }
    break;
  case 3:
    if (Actor_GetById(ACTOR_HEAD_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(Actor_GetById(ACTOR_SECOND_GUIDE));
      Actor_SetScript(ACTOR_SECOND_GUIDE, gKalay_GuidePaceScript);
      gKalay_DanceStep++;
    }
    break;
  case 4:
    if (Actor_GetById(ACTOR_SECOND_GUIDE)->timer != 0) {
      gKalay_DanceStep = 0;
    }
    break;
  }
}

/// Before the door of Hammet Palace (HammetPalaceGrounds), the first time the
/// party comes: the guards turn the party away until they recognize Ivan,
/// tanned and grown, and learn that he does not know Hammet is a prisoner in
/// Lunpa. Ivan insists his friends hear everything, and the guards open the
/// door and take the party in to Lady Layana
/// (HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM).
static void Kalay_IvanMeetsPalaceGuards(void) {
  struct Actor *actor;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(192), PIXELS(342));
  wait(1);
  FieldCamera_SetSpeed(PIXELS(1) / 5, PIXELS(1) / 40);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(252), 1);
  gFieldWork->transitionFrames = 40;
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 278);
  Actor_SetSpeed(ACTOR_PALACE_GUARD, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_SECOND_PALACE_GUARD, PIXELS(1), PIXELS(1) / 2);
  actor = Actor_Get(ACTOR_PALACE_GUARD);
  actor->facing = FACING_SOUTHEAST + FACING_STEP;
  actor = Actor_Get(ACTOR_SECOND_PALACE_GUARD);
  actor->facing = FACING_SOUTH + FACING_STEP;
  wait(1);
  Actor_Get(ACTOR_PALACE_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SECOND_PALACE_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_PALACE_GUARD, 184, 232);
  Actor_WalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 198, 232);
  Import_ActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_STAND);
  Event_Wait(20);
  Actor_Get(ACTOR_PALACE_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SECOND_PALACE_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_SHAKE_HEAD);
  Event_SetMessageId(MsgKalayLordHammetsPalaceLordAway);
  FieldEvent_ShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTH + FACING_STEP,
                      10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_IVAN, 212, 268);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_IVAN, 202, 254);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 10);
  Actor_ShowEmote(ACTOR_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SECOND_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  40);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PALACE_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_StartRepeatedMotion(ACTOR_PALACE_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_PALACE_GUARD, 1);
  Actor_SetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 1);
  Actor_SetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP,
                      10);
  Actor_RunRepeatedMotion(ACTOR_PALACE_GUARD, 1);
  Event_OpenMessage(ACTOR_PALACE_GUARD, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
  }
  Actor_RunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PALACE_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  FieldEvent_ShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PALACE_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_SECOND_PALACE_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_PALACE_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 10);
  Actor_SetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_NOD);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessage(ACTOR_PALACE_GUARD, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP,
                      10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Actor_SetSpeed(ACTOR_PALACE_GUARD, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeedParameters(ACTOR_SECOND_PALACE_GUARD, PIXELS(4) / 5,
                           PIXELS(2) / 5);
  actor = Actor_Get(ACTOR_PALACE_GUARD);
  actor->facing = 0;
  actor = Actor_Get(ACTOR_SECOND_PALACE_GUARD);
  actor->facing = FACING_WEST;
  Actor_Get(ACTOR_PALACE_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SECOND_PALACE_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_PALACE_GUARD, 168, 232);
  Actor_WalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 212, 232);
  Import_ActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_STAND);
  Event_Wait(20);
  Actor_Get(ACTOR_PALACE_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SECOND_PALACE_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_IVAN, 192, 232);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Kalay_AnimatePalaceDoor();
  Actor_WalkToAndWait(ACTOR_IVAN, 192, 222);
  // Lead the party and guards through the opened palace door.
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 222);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Import_ActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
  Actor_WalkTo(ACTOR_PALACE_GUARD, 184, 232);
  Actor_WalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 198, 232);
  Actor_WalkTo(ACTOR_PALACE_GUARD, 188, 212);
  Actor_WalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 194, 212);
  Actor_SetPosition(ACTOR_PALACE_GUARD, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_PALACE_GUARD, 0, 0);
  gFieldWork->transitionFrames = 24;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM);
  Event_End();
}

/// Arriving at the Kalay Tunnel Entrance by
/// KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH, after the party has freed Hammet
/// from Lunpa: Hammet comes home with the party in the wagon of Bunza, a Kalay
/// merchant. Hammet explains the underground passage to the palace and asks
/// Bunza to take the party round through the town, so the passage stays secret;
/// the guard opens it, Hammet goes in and the party rides off.
static void Kalay_HammetReturnsToPalace(void) {
  u8 *leaderPhase;
  u32 random;
  u32 motionPhase;
  struct Actor *actor;
  struct FieldWork *work;
  const s32 *motionActions;
  s32 southFacing;
  const s32 *exitActions;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_WAGON, PIXELS(440), PIXELS(522));
  actor = Actor_Get(ACTOR_WAGON);
  Actor_SetSpriteFlags(actor, 0);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(522), 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(400), 1);
  Event_PlaySound(SE_RUMBLE);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_WAGON, 440, 400);
  FieldCamera_SetSpeed(PIXELS(3) / 4, PIXELS(3) / 32);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(300), 1);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_WAGON, 440, 300);
  Import_ActorSetAnimation(ACTOR_WAGON, 0);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_BUNZA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_BUNZA, PIXELS(439), PIXELS(306));
  Actor_Jump(ACTOR_BUNZA, 4, 0);
  Actor_WalkToAndWait(ACTOR_BUNZA, 439, 312);
  Actor_WalkToAndWait(ACTOR_BUNZA, 416, 312);
  Actor_WalkToAndWait(ACTOR_BUNZA, 400, 256);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTHEAST + FACING_STEP, 40);
  FieldCamera_MoveTo(PIXELS(410), -1, PIXELS(280), 1);
  Actor_SetSpeed(ACTOR_HAMMET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetPosition(ACTOR_HAMMET, PIXELS(439), PIXELS(306));
  Actor_Jump(ACTOR_HAMMET, 4, 0);
  Actor_WalkToAndWait(ACTOR_HAMMET, 439, 312);
  Actor_WalkToAndWait(ACTOR_HAMMET, 416, 312);
  Actor_WalkToAndWait(ACTOR_HAMMET, 388, 270);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(439), PIXELS(306));
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 416, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 388, 300);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  // Seed the party idle phases for the arrival conversation.
  leaderPhase = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
  random = Random_Next();
  motionPhase = random * 5;
  motionActions = gKalay_PartyFidgetScript;
  leaderPhase += offsetof(struct Actor, userValue);
  *(u16 *)leaderPhase = motionPhase >> 12;
  Actor_SetScript(ACTOR_PARTY_LEADER, motionActions);
  Actor_Jump(ACTOR_BUNZA, 2, 20);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Event_SetMessageId(MsgKalayWeveArrivedHammet);
  FieldEvent_ShowMessageThenWait(ACTOR_BUNZA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_GARET, 378, 310);
  Actor_WalkTo(ACTOR_IVAN, 400, 288);
  Actor_WalkToAndWait(ACTOR_MIA, 410, 308);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 40);
  leaderPhase = (u8 *)Actor_Get(ACTOR_GARET);
  random = Random_Next();
  leaderPhase += offsetof(struct Actor, userValue);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  leaderPhase = (u8 *)Actor_Get(ACTOR_IVAN);
  random = Random_Next();
  leaderPhase += offsetof(struct Actor, userValue);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  leaderPhase = (u8 *)Actor_Get(ACTOR_MIA);
  random = Random_Next();
  leaderPhase += offsetof(struct Actor, userValue);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  Actor_SetScript(ACTOR_GARET, motionActions);
  Actor_SetScript(ACTOR_IVAN, motionActions);
  Actor_SetScript(ACTOR_MIA, motionActions);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 2, 0, 10);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTHEAST + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMMET, 0, 10);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_IVAN);
  Actor_Stop(ACTOR_MIA);
  wait(1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMMET, 0, 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMMET, 0, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Actor_RunRepeatedMotion(ACTOR_BUNZA, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_BUNZA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 10);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH + FACING_STEP, 10);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMMET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 2, 0, 10);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTHEAST + FACING_STEP, 10);
  Import_ActorSetAnimation(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_BUNZA, 0, 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMMET, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 10);
  Import_ActorSetAnimation(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_BUNZA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_BUNZA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMMET, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTHEAST + FACING_STEP, 10);
  Import_ActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
  Event_OpenMessage(ACTOR_HAMMET, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(20);
    gFieldWork->message += 3;
  } else {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_BUNZA, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_BUNZA, 0, 40);
    FieldEvent_ShowMessageThenWait(ACTOR_BUNZA, 0, 10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  }
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_TUNNEL_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(200), 1);
  Actor_WalkToAndWait(ACTOR_HAMMET, 333, 222);
  Actor_WalkToAndWait(ACTOR_HAMMET, 284, 198);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_WEST, 10);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_TUNNEL_GUARD, FACING_EAST, 10);
  Actor_SetAnimationAndWait(ACTOR_TUNNEL_GUARD, ANIM_NOD);
  Actor_SetSpeed(ACTOR_TUNNEL_GUARD, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_TUNNEL_GUARD, 268, 198);
  Actor_FaceDirection(ACTOR_TUNNEL_GUARD, FACING_NORTH, 10);
  Actor_RunRepeatedMotion(ACTOR_TUNNEL_GUARD, 2);
  // The guard opens the passage and Hammet goes in.
  Event_PlaySound(SE_CHEST_CLOSE);
  MapGrid_CopyCellsTo(71, 60, 76, 11, 2, 1);
  MapGrid_CopyCellAttributes(71, 60, 2, 1, 16, 11);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_TUNNEL_GUARD, 246, 198);
  Actor_FaceDirection(ACTOR_TUNNEL_GUARD, FACING_EAST, 20);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(170), 1);
  Actor_WalkToAndWait(ACTOR_HAMMET, 270, 198);
  Actor_WalkToAndWait(ACTOR_HAMMET, 270, 174);
  Actor_WalkToAndWait(ACTOR_HAMMET, 224, 170);
  Actor_WalkToAndWait(ACTOR_HAMMET, 210, 158);
  Actor_WalkToAndWait(ACTOR_HAMMET, 246, 148);
  Actor_WalkToAndWait(ACTOR_HAMMET, 246, 142);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPosition(ACTOR_WAGON, PIXELS(440), PIXELS(340));
  actor = Actor_Get(ACTOR_WAGON);
  southFacing = FACING_SOUTH;
  actor->facing = southFacing;
  FieldCamera_MoveTo(PIXELS(380), -1, PIXELS(280), 0);
  MapGrid_CenterOnFollowedPoint();
  wait(10);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_BUNZA, 420, 282);
  Actor_WalkToAndWait(ACTOR_BUNZA, 420, 312);
  Actor_WalkToAndWait(ACTOR_BUNZA, 439, 312);
  Actor_WalkToAndWait(ACTOR_BUNZA, 439, 306);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);
  // The party climbs back into the wagon.
  exitActions = gKalay_PartyGatherScript;
  Actor_SetScript(ACTOR_GARET, exitActions);
  Actor_SetScript(ACTOR_IVAN, exitActions);
  Actor_RunScriptAndWait(ACTOR_MIA, exitActions);
  FieldCamera_MoveTo(PIXELS(410), -1, PIXELS(300), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 416, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 306);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Event_PlaySound(SE_RUMBLE);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(4) / 5, PIXELS(2) / 5);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(420), 1);
  Actor_WalkToAndWait(ACTOR_WAGON, 440, 420);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(600), 1);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_WAGON, 440, 500);
  Actor_WalkTo(ACTOR_WAGON, 440, 600);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  work = gFieldWork;
  work->transitionFrames = 24;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH);
  Event_End();
}

// Shared source, included in link order.
#include "shared/actor_findcell.h"

/// In the Kalay Tunnel (KalayTunnelTreasureRoom), walking into a statue pushes
/// it one cell when there is room.
static void Kalay_PushTunnelStatue(void) {
  struct FieldPosition destination;
  struct Actor *leader;
  struct Actor *statue;
  s32 direction;
  s32 probeStep;
  s32 moveStep;
  s32 leaderX;
  struct FieldPosition *target;
  s32 stoppedVelocity;
  s32 pushSpeed;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  direction = (leader->facing >> 12);
  leaderX = leader->x.part.pixel;
  probeStep = gKalay_TileStepByFacing[direction];
  statue = Actor_FindAtCell((leaderX + (probeStep >> 16)) >> 4,
                            (leader->z.part.pixel + (s16)probeStep) >> 4);
  if (statue != NULL) {
    stoppedVelocity = 0;
    statue->layer = 2;
    target = &destination;
    moveStep = gKalay_TileStepByFacing[direction];
    SET_POSITION(target, statue->x.fixed + (moveStep & -0x10000),
                 statue->y.fixed, statue->z.fixed + (moveStep << 16));
    if (FieldActor_CheckMovementCollision(statue, target) <= 0) {
      Actor_SelectAnimation(leader, ANIM_PUSH);
      pushSpeed = PIXELS(1) / 5;
      wait(15);
      Sound_Play(SE_PUSH);
      statue->speedLimit = pushSpeed;
      statue->acceleration = pushSpeed;
      FieldActor_SetMoveTarget(statue, target->x, target->y, target->z);
      leader->speedLimit = pushSpeed;
      leader->acceleration = pushSpeed;
      FieldActor_SetMoveTarget(leader, target->x, target->y, target->z);
      Actor_WaitForMove(statue);
      Scene_PlayQueuedMusic();
      statue->x.fixed = target->x;
      statue->z.fixed = target->z;
      statue->velocityX = stoppedVelocity;
      statue->velocityZ = stoppedVelocity;
      Actor_SelectAnimation(leader, ANIM_STAND);
      Kalay_UpdateTunnelPassages();
    }
  }
}

/// Each statue's passage opens once the statue is pushed far enough west. In
/// the Kalay Tunnel (KalayTunnelTreasureRoom) the passage opens step by step,
/// and the first one raises the puddle for Frost.
static void Kalay_UpdateTunnelPassages(void) {
  struct Actor *firstStatue;
  struct Actor *secondStatue;
  s32 firstPassageOpen;
  struct Actor *frostEffect;
  u32 i;
  s32 span;
  s32 height;
  s32 width;

  // Open each passage only after its corresponding statue reaches the trigger.
  firstStatue = Actor_GetById(ACTOR_FIRST_STATUE);
  secondStatue = Actor_GetById(ACTOR_SECOND_STATUE);
  firstPassageOpen = GameFlag_Get(FLAG_TUNNEL_FIRST_PASSAGE_OPEN);
  if (firstPassageOpen != 0) {
  } else if ((firstStatue->x.fixed >> 19) > 29) {
  } else {
    frostEffect = Actor_GetById(ACTOR_PUDDLE);
    Event_Start();
    MapGrid_CopyCellAttributes(7, 44, 1, 1, firstPassageOpen, 1);
    i = 67;
    width = 1;
    height = 5;
    do {
      MapGrid_CopyCellsTo(i, 58, 78, 41, width, height);
      Event_Wait(4);
      if (i == 70) {
        GameFlag_Set(FLAG_TUNNEL_FIRST_PASSAGE_OPEN);
      }
      i++;
    } while (i <= 74);
    span = 2;
    MapGrid_CopyCellsTo(16, 109, 13, 109, 3, span);
    Event_Wait(40);
    frostEffect->scaleX = Q16_ONE / 10;
    frostEffect->scaleY = Q16_ONE / 10;
    Actor_SetPosition(ACTOR_PUDDLE, PIXELS(150), PIXELS(728));
    Actor_SetScript(ACTOR_PUDDLE, gKalay_FrostGrowScript);
    height = 1;
    MapGrid_CopyCellsTo(67, 64, 71, 44, height, span);
    MapGrid_CopyCellsTo(67, 64, 72, 44, height, span);
    MapGrid_CopyCellsTo(67, 68, 73, 43, height, span);
    MapGrid_CopyCellsTo(67, 68, 74, 43, height, span);
    MapGrid_CopyCellsTo(67, 64, 75, 44, height, span);
    MapGrid_CopyCellsTo(67, 66, 76, 44, height, span);
    MapGrid_CopyCellsTo(67, 64, 77, 44, height, span);
    MapGrid_CopyCellsTo(67, 64, 78, 44, height, span);
    MapGrid_CopyCellsTo(67, 64, 79, 44, height, span);
    MapGrid_CopyCellsTo(67, 66, 80, 44, height, span);
    MapGrid_CopyCellsTo(2, 0, 9, 42, span, span);
    Event_Wait(40);
    MapGrid_CopyCellsTo(68, 64, 71, 44, height, span);
    MapGrid_CopyCellsTo(68, 64, 72, 44, height, span);
    MapGrid_CopyCellsTo(68, 68, 73, 43, height, span);
    MapGrid_CopyCellsTo(68, 68, 74, 43, height, span);
    MapGrid_CopyCellsTo(68, 64, 75, 44, height, span);
    MapGrid_CopyCellsTo(68, 66, 76, 44, height, span);
    MapGrid_CopyCellsTo(68, 64, 77, 44, height, span);
    MapGrid_CopyCellsTo(68, 64, 78, 44, height, span);
    MapGrid_CopyCellsTo(68, 64, 79, 44, height, span);
    MapGrid_CopyCellsTo(68, 66, 80, 44, height, span);
    MapGrid_CopyCellsTo(4, 0, 9, 42, span, span);
    Event_Wait(40);
    span = 10;
    MapGrid_CopyCellsTo(7, 11, 7, 42, span, 8);
    MapGrid_CopyCellsTo(71, 12, 71, 43, span, 13);
    span = 44;
    MapGrid_CopyCellAttributes(6, 13, 12, 12, 6, span);
    Event_Wait(40);
    Kalay_StartTrackingLeader();
    MapGrid_CopyCellAttributes(0, 1, 1, 1, 7, span);
    Event_End();
  }
  if (GameFlag_Get(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) != 0) {
  } else if ((secondStatue->x.fixed >> 19) > 87) {
  } else {
    Event_Start();
    i = 67;
    width = 1;
    height = 5;
    do {
      MapGrid_CopyCellsTo(i, 58, 107, 41, width, height);
      Event_Wait(4);
      if (i == 70) {
        GameFlag_Set(FLAG_TUNNEL_SECOND_PASSAGE_OPEN);
      }
      i++;
    } while (i <= 74);
    height = 2;
    MapGrid_CopyCellsTo(45, 109, 42, 109, 3, height);
    Event_Wait(40);
    span = 1;
    MapGrid_CopyCellsTo(67, 64, 102, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 103, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 104, 44, span, height);
    MapGrid_CopyCellsTo(67, 66, 105, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 106, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 107, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 108, 44, span, height);
    MapGrid_CopyCellsTo(67, 66, 109, 44, span, height);
    Event_Wait(40);
    MapGrid_CopyCellsTo(68, 64, 102, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 103, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 104, 44, span, height);
    MapGrid_CopyCellsTo(68, 66, 105, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 106, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 107, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 108, 44, span, height);
    MapGrid_CopyCellsTo(68, 66, 109, 44, span, height);
    Event_Wait(40);
    span = 8;
    MapGrid_CopyCellsTo(38, 14, 38, 44, span, 4);
    MapGrid_CopyCellsTo(102, 14, 102, 44, span, 12);
    MapGrid_CopyCellAttributes(37, 13, 10, 12, 37, 43);
    Event_Wait(40);
    Kalay_StartTrackingLeader();
    Event_End();
  }
}

// UB: the native void epilogue returns through r0, leaving the nonzero return
// address as SC_CALL's status. Repeat once per frame until the actor is
// released, with an explicit status in the modern build.
// Advance a particle, damp its velocity, and expire its lifetime.
#ifdef UBFIX
static s32 Kalay_UpdateTunnelParticle(struct Actor *interpreter) {
#else
static void Kalay_UpdateTunnelParticle(struct Actor *interpreter) {
#endif
  struct Actor *particle = interpreter;
  s16 *lifetime;
  s32 remainingFrames;
  s32 velocityX;
  s32 velocityY;

  lifetime = &particle->timer;
  remainingFrames = *lifetime;
  if (remainingFrames == 0) {
    Actor_Release(particle);
  } else if (remainingFrames == 1) {
    particle->velocityX = 0;
    particle->velocityY = 0;
    particle->x.fixed = 0;
    particle->y.fixed = 0;
  } else {
    particle->scaleX += Q16_ONE / 32;
    particle->scaleY += Q16_ONE / 32;
  }
  particle->x.fixed += particle->velocityX;
  particle->y.fixed += particle->velocityY;
  velocityX = particle->velocityX;
  velocityY = particle->velocityY;
  particle->velocityX = velocityX - velocityX / 256;
  particle->velocityY = velocityY - velocityY / 16;
  {
    s32 nextLifetime = *(u16 *)lifetime;
    nextLifetime -= 1;
    *(u16 *)lifetime = nextLifetime;
  }
#ifdef UBFIX
  return 1;
#endif
}

/// Particles stream from each statue as it nears its place. That's in the Kalay
/// Tunnel (KalayTunnelTreasureRoom), while the statue's passage is closed.
static void Kalay_UpdateTunnelEmitters(void) {
  struct Actor *firstEmitter;
  struct Actor *secondEmitter;

  firstEmitter = Actor_Get(ACTOR_FIRST_STATUE);
  secondEmitter = Actor_Get(ACTOR_SECOND_STATUE);
  if ((u32)(firstEmitter->x.part.pixel - 381) <= 12 &&
      firstEmitter->z.part.pixel > 777) {
    firstEmitter->sprite->image.priority =
        Actor_Get(ACTOR_PARTY_LEADER)->sprite->image.priority;
  } else if (!GameFlag_Get(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) &&
             firstEmitter->x.part.pixel <= 245 && (gFrameCount & 1) == 0) {
    if (!Event_GetFlag(FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED)) {
      Scene_QueueMusic(-1);
      Event_PlaySound(SE_WATER_DRAIN);
      GameFlag_Set(FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED);
    }
    Kalay_SpawnTunnelParticle(firstEmitter->x.fixed, firstEmitter->y.fixed,
                              firstEmitter->z.fixed);
  }
  if (!GameFlag_Get(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) &&
      secondEmitter->x.part.pixel <= 709 && (gFrameCount & 1) == 0) {
    if (!Event_GetFlag(FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED)) {
      Scene_QueueMusic(-1);
      Event_PlaySound(SE_WATER_DRAIN);
      GameFlag_Set(FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED);
    }
    Kalay_SpawnTunnelParticle(secondEmitter->x.fixed, secondEmitter->y.fixed,
                              secondEmitter->z.fixed);
  }
}

/// Creates a short-lived particle with random position and velocity offsets: a
/// puff of dust (SPRITE_DUST_PUFF) with its blue raised to full.
static void Kalay_SpawnTunnelParticle(s32 x, s32 height, s32 z) {
  s32 depth;
  struct Actor *particle;
  s32 random;
  s32 attributeMask;
  u8 *spriteBytes;

  depth = z;
  random = Import_RandomNext();
  particle = FieldActor_Create(
      SPRITE_DUST_PUFF, (x - PIXELS(8)),
      (((((u32)(random << 3) >> 16) << 16) + height) + PIXELS(16)), depth);
  if (particle != 0) {
    ((u8 *)particle)[offsetof(struct Actor, motionFlags)] = (attributeMask = 0);
    spriteBytes = (u8 *)particle->sprite;
    attributeMask -= 13;
    spriteBytes[9] = ((spriteBytes[9] & attributeMask) | 8);
    Actor_SetSpriteColor(particle, SPRITE_COLOR_BLUE_TINT);
    Actor_SetSpriteFlags(particle, 0);
    random = Random_Next();
    particle->velocityX = ((((u32)(random << 1) >> 16) - 1) << 16);
    random = Random_Next();
    particle->velocityY =
        ((((u32)(((random << 1) + random) << 1) >> 16) - 3) << 16);
    {
      u16 *target = (u16 *)&particle->timer;
      u16 shown = 20;

      *target = shown;
      particle->frozen = 1;
    }
    Actor_SelectAnimation(particle, 1);
    Actor_StartScript(particle, gKalay_TunnelParticleScript);
  }
}

/// The Nut in Kalay circles its spot and rocks as it goes.
static void Kalay_UpdateNutOrbit(void) {
  OrbitingMapActor *effect = (OrbitingMapActor *)Actor_GetById(ACTOR_TOWN_NUT);
  struct Sprite *sprite = (struct Sprite *)effect->sprite;
  s32 offset = Trig_Sin(effect->orbitAngle) * 2;
  s32 first;

  if (offset > 0) {
    offset = -offset;
  }
  effect->x = effect->orbitCenterX + Trig_Cos(effect->orbitAngle) * 2;
  effect->y = effect->orbitCenterY + offset;
  sprite->rotation = Trig_Cos(effect->orbitAngle + 0x8000) >> 3;
  first = Import_RandomNext();
  effect->orbitAngle += ((u32)(first << 9) >> 16) +
                        ((u32)(Import_RandomNext() << 9) >> 16) + 0x400;
}

/// Frost on the puddle in the Kalay Tunnel sinks it into the floor. The puddle
/// is in KalayTunnelTreasureRoom; sinking it clears its cells, and it stays
/// sunk until the scene is entered again.
static void Kalay_FreezePuddle(void) {
  struct Actor *effect = Actor_GetById(ACTOR_PUDDLE);

  MapGrid_CopyCellAttributes(0, 0, 1, 1, 9, 14);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 9, 45);
  if (effect != 0) {
    Actor_SetSpriteFlags(effect, 0);
    effect->y.fixed -= PIXELS(32);
    effect->priorityFlags = 2;
  }
  GameFlag_Set(FLAG_TUNNEL_PUDDLE_FROZEN);
}

// Shared source, included in link order.
#include "shared/actor_orbit.h"

// One cell in each facing direction: x in the high 16 bits, z in the low 16.
// UB: negative x coordinates cannot be left-shifted as signed integers.
#ifdef UBFIX
#define TILE_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))
#else
#define TILE_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

s32 gKalay_TileStepByFacing[] = {
    TILE_STEP(16, 0),  TILE_STEP(16, 0),  TILE_STEP(16, 0),  TILE_STEP(0, 16),
    TILE_STEP(0, 16),  TILE_STEP(0, 16),  TILE_STEP(0, 16),  TILE_STEP(-16, 0),
    TILE_STEP(-16, 0), TILE_STEP(-16, 0), TILE_STEP(-16, 0), TILE_STEP(0, -16),
    TILE_STEP(0, -16), TILE_STEP(0, -16), TILE_STEP(0, -16), TILE_STEP(16, 0),
};

s32 gKalay_TourGroupWalkScript[] = {
    SC_ANIMATION(2), SC_MOVE_BY(-PIXELS(80), 0, 0), SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_TIMER, 1),           SC_WAIT(1),
    SC_END,
};

s32 gKalay_TourGroupLeaveScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(-PIXELS(72), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(-PIXELS(100), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(-PIXELS(100), 0, 0),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_TIMER, 1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(222)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_TouristFidgetScript[] = {
    SC_CALL(Kalay_UpdateTouristFidget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKalay_GuideCountScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(228), 0, PIXELS(296)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(296)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(296)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_SET(SW_TIMER, 1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(178), 0, PIXELS(272)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_GuidePaceScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(164), 0, PIXELS(278)),
    SC_SYNC,
    SC_MOVE(PIXELS(182), 0, PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(238), 0, PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(246), 0, PIXELS(296)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(40),
    SC_SET(SW_TIMER, 1),
    SC_END,
};

s32 gKalay_HeadGuideStopScript[] = {
    SC_SET(SW_TARGET_X, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Z, ACTOR_NO_TARGET),
    SC_SET(SW_VELOCITY_X, 0),
    SC_SET(SW_VELOCITY_Y, 0),
    SC_SET(SW_VELOCITY_Z, 0),
    SC_WAIT(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(184), 0, PIXELS(294)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(SW_TIMER, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_SecondGuideStopScript[] = {
    SC_SET(SW_TARGET_X, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Z, ACTOR_NO_TARGET),
    SC_SET(SW_VELOCITY_X, 0),
    SC_SET(SW_VELOCITY_Y, 0),
    SC_SET(SW_VELOCITY_Z, 0),
    SC_WAIT(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(198), 0, PIXELS(300)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(SW_TIMER, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_HeadGuideReturnScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(178), 0, PIXELS(272)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(SW_TIMER, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_SecondGuideReturnScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(255), 0, PIXELS(289)),
    SC_SYNC,
    SC_MOVE(PIXELS(265), 0, PIXELS(280)),
    SC_SYNC,
    SC_MOVE(PIXELS(284), 0, PIXELS(272)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_SET(SW_TIMER, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_PartyFidgetScript[] = {
    SC_CALL(Scene_LookAboutRandomly),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKalay_PartyGatherScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(388), 0, PIXELS(300)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gKalay_FrostGrowScript[] = {
    SC_ADD(SW_SCALE_X, 546),
    SC_ADD(SW_SCALE_Y, 546),
    SC_WAIT(1),
    SC_LOOP(108, SC_TOP),
    SC_END,
};

struct SceneEntrance gKalay_NoEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_TownEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 224, 0, 280, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_FROM_HOUSE_NORTHWEST, CONDITION_ALWAYS, 72, 0, 200, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {KALAY_FROM_HOUSE_NORTHEAST_UPPER, CONDITION_ALWAYS, 408, 0, 184,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_FROM_HOUSE_NORTHEAST_LOWER, CONDITION_ALWAYS, 408, 0, 216,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_FROM_HOUSE_SOUTHEAST, CONDITION_ALWAYS, 408, 0, 408, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {KALAY_FROM_WEAPON_SHOP, CONDITION_ALWAYS, 328, 0, 440, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {KALAY_FROM_ARMOR_SHOP, CONDITION_ALWAYS, 120, 0, 264, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {KALAY_FROM_INN, CONDITION_ALWAYS, 312, 0, 264, FACING_SOUTH, 0, -1, -1, -1,
     -1, 0},
    {KALAY_FROM_KALAY_SANCTUM, CONDITION_ALWAYS, 152, 0, 440, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {KALAY_FROM_WORLD_MAP, CONDITION_ALWAYS, 224, 0, 464, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {KALAY_FROM_WORLD_MAP_WEST_UNUSED, CONDITION_ALWAYS, 56, 0, 272,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {KALAY_FROM_HAMMET_PALACE_GROUNDS, CONDITION_ALWAYS, 224, 0, 88,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_FROM_WEAPON_SHOP_COUNTER, CONDITION_ALWAYS, 296, 0, 408,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_FROM_ARMOR_SHOP_COUNTER, CONDITION_ALWAYS, 152, 0, 232, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM, CONDITION_ALWAYS, 440, 0, 88,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_HammetPalaceEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {HAMMET_PALACE_GROUNDS_FROM_KALAY, CONDITION_ALWAYS, 192, 0, 280,
     FACING_NORTH, 0, 0, 0, 400, 352, 0},
    {HAMMET_PALACE_GROUNDS_FROM_HAMMET_PALACE, CONDITION_ALWAYS, 192, 0, 232,
     FACING_SOUTH, 0, 0, 0, 400, 352, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_TunnelEntranceEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_TUNNEL_ENTRANCE_FROM_KALAY_TUNNEL_CAVE, CONDITION_ALWAYS, 248, 0,
     152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 424, 0, 520,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 440, 0, 540,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_TunnelCaveEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_PASSAGE, CONDITION_ALWAYS, 296, 0, 104,
     FACING_SOUTH, 0, 100, 50, 440, 445, 0},
    {KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_ENTRANCE, CONDITION_ALWAYS, 280, 0,
     392, FACING_NORTH, 0, 100, 50, 440, 445, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_TunnelPassageEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_WEST,
     CONDITION_ALWAYS, 296, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_EAST,
     CONDITION_ALWAYS, 632, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM, CONDITION_ALWAYS, 152,
     0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_FROM_LONG_PATH, CONDITION_ALWAYS, 568,
     0, 152, FACING_SOUTH, 0, 390, 0, 630, 220, 0},
    {KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_CAVE, CONDITION_ALWAYS, 472, 0, 72,
     FACING_SOUTH, 0, 390, 0, 630, 220, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_TunnelEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE, CONDITION_ALWAYS, 408, 0,
     632, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
    {KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST,
     CONDITION_ALWAYS, 296, 0, 488, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
    {KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST,
     CONDITION_ALWAYS, 744, 0, 488, FACING_SOUTH, 0, 595, 400, 930, 880, 0},
    {KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY, CONDITION_ALWAYS, 856, 0, 856,
     FACING_NORTH, 0, 595, 400, 930, 880, 0},
    {SCENE_TABLE_END},
};

u32 gKalay_Exits[] = {
    SCENE_EXITS(Kalay),
    SCENE_EXIT(KALAY_TO_HOUSE_NORTHWEST, StuccoRoomsA,
               STUCCO_ROOMS_A_HOUSE_NORTHWEST),
    SCENE_EXIT(KALAY_TO_HOUSE_NORTHEAST_UPPER, StuccoRoomsA,
               STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER),
    SCENE_EXIT(KALAY_TO_HOUSE_NORTHEAST_LOWER, StuccoRoomsA,
               STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER),
    SCENE_EXIT(KALAY_TO_HOUSE_SOUTHEAST, StuccoRoomsA,
               STUCCO_ROOMS_A_HOUSE_SOUTHEAST),
    SCENE_EXIT(KALAY_TO_WEAPON_SHOP, StuccoRoomsA, STUCCO_ROOMS_A_WEAPON_SHOP),
    SCENE_EXIT(KALAY_TO_ARMOR_SHOP, StuccoRoomsA, STUCCO_ROOMS_A_ARMOR_SHOP),
    SCENE_EXIT(KALAY_TO_INN, StuccoRoomsA, STUCCO_ROOMS_A_INN),
    SCENE_EXIT(KALAY_TO_KALAY_SANCTUM, KalaySanctum, KALAY_SANCTUM_FROM_KALAY),
    SCENE_EXIT(KALAY_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KALAY_EAST),
    SCENE_EXIT(KALAY_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_KALAY_WEST),
    SCENE_EXIT(KALAY_TO_HAMMET_PALACE_GROUNDS, HammetPalaceGrounds,
               HAMMET_PALACE_GROUNDS_FROM_KALAY),
    SCENE_EXIT(KALAY_TO_WEAPON_SHOP_COUNTER, StuccoRoomsA,
               STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER),
    SCENE_EXIT(KALAY_TO_ARMOR_SHOP_COUNTER, StuccoRoomsA,
               STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER),
    SCENE_EXIT(KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM, KalayTunnelTreasureRoom,
               KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY),
    SCENE_EXITS(HammetPalaceGrounds),
    SCENE_EXIT(HAMMET_PALACE_GROUNDS_TO_KALAY, Kalay,
               KALAY_FROM_HAMMET_PALACE_GROUNDS),
    SCENE_EXIT(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE, HammetPalace,
               HAMMET_PALACE_FROM_HAMMET_PALACE_GROUNDS),
    SCENE_EXIT(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM,
               HammetPalace,
               HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS),
    SCENE_EXITS(KalayTunnelEntrance),
    SCENE_EXIT(KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_CAVE, KalayTunnelCave,
               KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_ENTRANCE),
    SCENE_EXIT(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_NORTH, WorldMap,
               WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_NORTH),
    SCENE_EXIT(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH, WorldMap,
               WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH),
    SCENE_EXITS(KalayTunnelCave),
    SCENE_EXIT(KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_PASSAGE, KalayTunnelPassage,
               KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_CAVE),
    SCENE_EXIT(KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_ENTRANCE, KalayTunnelEntrance,
               KALAY_TUNNEL_ENTRANCE_FROM_KALAY_TUNNEL_CAVE),
    SCENE_EXITS(KalayTunnelPassage),
    SCENE_EXIT(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_WEST,
               KalayTunnelTreasureRoom,
               KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST),
    SCENE_EXIT(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_EAST,
               KalayTunnelTreasureRoom,
               KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST),
    SCENE_EXIT(KALAY_TUNNEL_PASSAGE_LONG_PATH_TO_NORTHEAST_ROOM,
               KalayTunnelPassage,
               KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_FROM_LONG_PATH),
    SCENE_EXIT(KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_TO_LONG_PATH,
               KalayTunnelPassage,
               KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM),
    SCENE_EXIT(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_CAVE, KalayTunnelCave,
               KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_PASSAGE),
    SCENE_EXITS(KalayTunnelTreasureRoom),
    SCENE_EXIT(KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE, HammetPalace,
               HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM),
    SCENE_EXIT(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_WEST,
               KalayTunnelPassage,
               KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_WEST),
    SCENE_EXIT(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_EAST,
               KalayTunnelPassage,
               KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_EAST),
    SCENE_EXIT(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY, Kalay,
               KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM),
    SCENE_EXITS_END,
};

struct ActorPlacement gKalay_NoActorPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gKalay_TownActorPlacements[] = {
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(188), 0, PIXELS(476), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(260), 0, PIXELS(476), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(222), 0, PIXELS(382), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(134), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(115), 0, PIXELS(453), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(484), 0, PIXELS(258), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(359), 0, PIXELS(314), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(404), 0, PIXELS(268), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(154), 0, PIXELS(119), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(198), 0, PIXELS(84), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(199), 0, PIXELS(233), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(46), 0, PIXELS(217), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(468), 0, PIXELS(434), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(464), 0, PIXELS(416), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(408), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(278), 0, PIXELS(318), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(56), 0, PIXELS(332), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gKalay_TourGroupActorPlacements[] = {
    { SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gKalay_HammetPalaceActorPlacements[] = {
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(212), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gKalay_TunnelEntranceActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(246), 0, PIXELS(198), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_LARGE_WAGON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gKalay_TunnelActorPlacements[] = {
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(826), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(732), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_SCORCH), MOVEMENT_TYPE_STAND,
        PIXELS(632), 0, PIXELS(712), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gKalay_NoTriggers[] = {
    { TRIGGERS_END },
};

struct Trigger gKalay_TownTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Kalay_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Kalay_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Kalay_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Kalay_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Kalay_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Kalay_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Kalay_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Kalay_OpenDoorAndExit) },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, KALAY_TO_HAMMET_PALACE_GROUNDS },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, KALAY_TO_WEAPON_SHOP_COUNTER },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, KALAY_TO_ARMOR_SHOP_COUNTER },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM },
    { TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Kalay_WatchTourGroupDeparture) },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Kalay_WatchTourGroupDeparture) },
    { TRIGGER_TALK, ACTOR_WELCOMING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgKalayWelcomeInPeace) },
    { TRIGGER_TALK, ACTOR_SOLDIER_PROTECTING_THE_PEACE, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersProtectPeace) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_HAMMET_THE_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayHammetGenerousMerchant) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_HAMMETS_CARAVANS, CONDITION_ALWAYS, MSGNO(MsgKalayHammetOnJourney) },
    { TRIGGER_TALK, ACTOR_MAN_ON_THE_REBUILT_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgKalayNorthernBridgeRebuilt) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_LAMAKAN_DESERT, CONDITION_ALWAYS, MSGNO(MsgKalayLamakanTrouble) },
    { TRIGGER_TALK, ACTOR_LATE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayElderTownNews) },
    { TRIGGER_TALK, ACTOR_WOULD_BE_TOURIST, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInnFestivalTourists) },
    { TRIGGER_TALK, ACTOR_WOULD_BE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayFormerInnFestivalTourists) },
    { TRIGGER_TALK, ACTOR_BOY_DREAMING_OF_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayChildWantsFestival) },
    { TRIGGER_TALK, ACTOR_GIRL_ON_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgKalayTraderTownNews) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_ON_HAMMETS_KNACK, CONDITION_ALWAYS, MSGNO(MsgKalayHammetBusinessSkill) },
    { TRIGGER_TALK, ACTOR_MAN_WHO_LEFT_TOLBI, CONDITION_ALWAYS, EVENT(Kalay_TalkAboutLeavingHome) },
    { TRIGGER_TALK, ACTOR_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCapturedRumor) },
    { TRIGGER_TALK, ACTOR_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayHammetWentNorth) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(Kalay_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(Kalay_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(Kalay_TalkToItemMerchant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorHonestEyes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_PROTECTING_THE_PEACE, CONDITION_ALWAYS, MSGNO(MsgKalayGuardsJudgeVisitors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_HAMMET_THE_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayHammetPeaceAndLiberty) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_HAMMETS_CARAVANS, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCanBeRash) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_REBUILT_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersWentToVault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_LAMAKAN_DESERT, CONDITION_ALWAYS, MSGNO(MsgKalayStrangeEvents) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LATE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayElderThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOULD_BE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayWishSavedForFestival) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_DREAMING_OF_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayChildFestivalDream) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgKalayTraderThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_HAMMETS_KNACK, CONDITION_ALWAYS, MSGNO(MsgKalayHammetValuesTrust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_LEFT_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTrustAgainstForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayLunpaNorthOfVault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayRumorsHideTruth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayWishBetterWeapons) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayWeaponsHaveSoul) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayPalaceWaterSupply) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TOWN_STATUE, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_TOWN_NUT, FLAG_KALAY_TOWN_NUT_CAUGHT, EVENT(Kalay_CatchTownNut) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_KALAY_SLEEP_BOMB_JAR, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_KALAY_VIAL_JAR, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 90, CONDITION_ALWAYS, EVENT(Kalay_SearchWell) },
    { TRIGGERS_END },
};

struct Trigger gKalay_HammetPalaceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, HAMMET_PALACE_GROUNDS_TO_KALAY },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Kalay_EnterPalace) },
    { TRIGGER_TALK, ACTOR_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersHandleAffairs) },
    { TRIGGER_TALK, ACTOR_SECOND_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersRescueHammet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayPrayForHammetSafety) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorsWillSaveHammet) },
    { TRIGGERS_END },
};

struct Trigger gKalay_TunnelEntranceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_CAVE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_NORTH },
    { TRIGGER_TALK, ACTOR_TUNNEL_GUARD, CONDITION_ALWAYS, EVENT(Kalay_AskAboutCaveAccess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TUNNEL_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayPassageClosedByOrders) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_TUNNEL_ENTRANCE_NUT, FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT, EVENT(Kalay_CatchTunnelEntranceNut) },
    { TRIGGERS_END },
};

struct Trigger gKalay_TunnelCaveTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_PASSAGE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_ENTRANCE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_KALAY_TUNNEL_APPLE_CHEST, FIND_ITEM(ITEM_APPLE) },
    { TRIGGERS_END },
};

struct Trigger gKalay_TunnelPassageTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_LONG_PATH_TO_NORTHEAST_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_TO_LONG_PATH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_CAVE },
    { TRIGGERS_END },
};

struct Trigger gKalay_TunnelTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY },
    { TRIGGER_TALK, ACTOR_WAITING_MAN, CONDITION_ALWAYS, EVENT(Kalay_TalkAboutLayana) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTreasureBeyondDoors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_STATUE, FLAG_TUNNEL_FIRST_PASSAGE_OPEN, EVENT(Kalay_UpdateTunnelPassages) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_STATUE, FLAG_TUNNEL_SECOND_PASSAGE_OPEN, EVENT(Kalay_UpdateTunnelPassages) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT(Kalay_PushTunnelStatue) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 11, CONDITION_ALWAYS, EVENT(Kalay_PushTunnelStatue) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT(Kalay_PushTunnelStatue) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT(Kalay_PushTunnelStatue) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 15, CONDITION_ALWAYS, EVENT(Kalay_PushTunnelStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_TUNNEL_PUDDLE_FROZEN, EVENT(Kalay_FreezePuddle) },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Kalay_StopTrackingLeader) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Kalay_StartTrackingLeader) },
    { TRIGGER_STEP, 18, CONDITION_ALWAYS, EVENT(Kalay_DisableLeaderAutomaticPriority) },
    { TRIGGER_STEP, 19, CONDITION_ALWAYS, EVENT(Kalay_EnableLeaderAutomaticPriority) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_KALAY_TUNNEL_SPIRIT_GLOVES_CHEST, FIND_ITEM(ITEM_SPIRIT_GLOVES) },
    { TRIGGER_TALK, ACTOR_SCORCH, CONDITION_ALWAYS, EVENT(Kalay_ScorchJoins) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_KALAY_TUNNEL_SLEEP_BOMB_CHEST, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_KALAY_TUNNEL_LUCKY_MEDAL_CHEST, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_KALAY_TUNNEL_COINS_CHEST, FIND_COINS(200) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 105, FLAG_KALAY_TUNNEL_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
    { TRIGGERS_END },
};
// clang-format on

u16 gKalay_DoorCellSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 33, 1, 2, 5),
    MAPGRID_ANIMATION_STEP(37, 33, 1, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

struct MapGridCellAnimation gKalay_DoorCells[] = {
    {gKalay_DoorCellSteps, 36, 10}, {gKalay_DoorCellSteps, 57, 7},
    {gKalay_DoorCellSteps, 57, 11}, {gKalay_DoorCellSteps, 57, 23},
    {gKalay_DoorCellSteps, 52, 25}, {gKalay_DoorCellSteps, 39, 14},
    {gKalay_DoorCellSteps, 51, 14}, {gKalay_DoorCellSteps, 41, 25},
};

s32 gKalay_TunnelParticleScript[] = {
    SC_CALL(Kalay_UpdateTunnelParticle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};
