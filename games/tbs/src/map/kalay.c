/// @file
/// Kalay, the merchants' town Lord Hammet founded, the front of his palace,
/// and the tunnel beneath them: four map sections in six maps.
///
/// - Kalay (MapId_Kalay), the town. Eight doors (coord events 1 to 8) open into
/// the houses, the
///   weapon and armor shops, the inn and the Kalay Sanctum (kalay_interiors.c),
///   and KALAY_TO_WEAPON_SHOP_COUNTER and KALAY_TO_ARMOR_SHOP_COUNTER into the
///   same two shops from behind their counters; KALAY_TO_HAMMET_PALACE_GROUNDS
///   leads to Hammet Palace, and coord events 9 and 10 out to the world map
///   (KALAY_TO_WORLD_MAP and KALAY_TO_WORLD_MAP_WEST, to its
///   WORLD_MAP_FROM_KALAY_EAST and WORLD_MAP_FROM_KALAY_WEST). The weapon,
///   armor and item sellers open their shops only when spoken to across the
///   counter. A Nut circles in the air until Catch pulls it in
///   (FLAG_KALAY_TOWN_NUT_CAUGHT), and a statue at the north edge
///   (LOCALID_TOWN_STATUE), which Move pushes aside, covers the way down into
///   the Kalay Tunnel (KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM); coming up from it
///   (KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM) finds the statue moved.
///
///   Once the party has heard of the Colosso tour at the inn
///   (FLAG_KALAY_HEARD_OF_COLOSSO_TOUR), leaving town plays the tour group's
///   departure, once (FLAG_KALAY_COLOSSO_TOUR_LEFT): the guides count the
///   group, find a man missing and lead it off without him, and he runs after
///   it. The second guide is the head guide's sprite with its green and blue
///   swapped (SPRITE_COLOR_RBG), here and in kalay_interiors.c and
///   kalay_tolbi_docks.c.
/// - Hammet Palace (MapId_HammetPalaceGrounds), before the door of the
///   palace, where two soldiers of Kalay stand guard; the palace's rooms are
///   MapId_HammetPalace (hammet_palace.c). The first time the party comes,
///   before Ivan has reported to Layana (FLAG_IVAN_REPORTED_TO_LAYANA), the
///   guards recognize Ivan and take everyone in to Lady Layana
///   (HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM, to
///   HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS); afterwards they
///   have other lines.
///   Walking into the door (coord event 2) opens it into the palace
///   (HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE); HAMMET_PALACE_GROUNDS_TO_KALAY
///   leads back to town.
/// - Kalay Tunnel Entrance (MapId_KalayTunnelEntrance), outside the town
///   (the world map's WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE leads in at
///   KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH). A guard
///   (LOCALID_TUNNEL_GUARD) bars `the cave up ahead` by Hammet and Layana's
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
/// - Kalay Tunnel (MapId_KalayTunnelCave to MapId_KalayTunnelTreasureRoom),
///   the passage to Hammet Palace and his treasure, which Layana opens to
///   the party in thanks once Hammet is home (FLAG_KALAY_TUNNEL_OPEN). The cave
///   (KalayTunnelCave) joins the Kalay Tunnel Entrance to the passage
///   (KalayTunnelPassage), and a chest holds an Apple. The passage opens with a
///   window transition, its way through drawn open once FLAG_KALAY_TUNNEL_OPEN
///   is set. The treasure room (KalayTunnelTreasureRoom) holds the puzzle: two
///   statues, pushed by walking into them or moved with Move, each open a
///   passage step by step once far enough west (FLAG_TUNNEL_FIRST_PASSAGE_OPEN
///   and FLAG_TUNNEL_SECOND_PASSAGE_OPEN, map-section temp flags, so the
///   statues are back once the party leaves the tunnel's maps); while a passage
///   is closed, particles stream from its statue as it nears its place. The
///   first passage raises a puddle (LOCALID_PUDDLE) that Frost freezes and
///   sinks into the floor, freeing its cells; that is a temporary flag, so the
///   puddle stands again on each entry. Scorch, a Mars Djinni, joins the party
///   when spoken to, without a battle, and five chests hold Spirit Gloves, a
///   Sleep Bomb, a Lucky Medal, 200 coins and a Potion. Once
///   FLAG_KALAY_TUNNEL_OPEN is set a man (LOCALID_WAITING_MAN) waits here,
///   thinking `The things beyond these doors belong to you all.` The fade
///   overlay (overworld/overworld/fade_overlay.c) follows the leader through
///   the treasure room unless he comes down from the town
///   (KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY); coord events 10 and 11 stop and
///   restart it, and 18 and 19 fix and free his depth priority.
///   KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE leads into Hammet Palace
///   (HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM),
///   KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY up into the town
///   (KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM).
///
/// Every map sets FLAG_KALAY_REACHED on entry. Kalay and Hammet Palace's
/// front also catch the palace's story up with Lunpa's: hearing in Lunpa that
/// Hammet is held there (FLAG_LUNPA_HEARD_OF_PRISONER) sets map-section
/// temp flag FLAG_MAPSEC_TEMP_21, which changes what Layana says, and once
/// Hammet is free (FLAG_DODONPA_LOCKED_UP, set as Donpa judges Dodonpa in
/// Lunpa) Ivan's report and Layana's scolding count as seen and
/// FLAG_KALAY_TUNNEL_OPEN opens the passage. The getters choose each map's
/// tables; the cave and the passage have no object event templates, and an
/// empty set serves any other map number.
///
/// Open questions:
///
/// - XXX (no evidence): whether the man waiting in the treasure room
///   (LOCALID_WAITING_MAN) is Bunza; likely, because he is drawn with
///   SPRITE_BUNZA, which every other map places beside Hammet as Bunza, but
///   his lines never name him.

#include "game_state.h"
#include "unit/field/field.h"
#include "engine_call.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/kalay.h"
#include "unit/map/common/object_orbit.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
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
#include "types.h"
#include "edition.h"
#include "task.h"
#include "script_layer.h"
#include "map_tables.h"
#include "resource_3a8_effect.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "action_script.h"
#include "macros.h"

#include "object_event.h"
#include "object_effect.h"
#include "orbit.h"
#include "fixed_math.h"
#include "map_header.h"
#include "heap.h"

#include "flags.h"
#include "djinn_ids.h"
#include "songs.h"
#include "audio_engine_symbols.h"

/// Kalay's object events (Kalay), numbering the rows of
/// gKalay_TownObjectEventTemplates from LOCALID_FIRST_PLACED, then the tour
/// group, which Kalay_WatchTourGroupDeparture places after them
/// (gKalay_TourGroupObjectEventTemplates).
enum KalayTownLocalId {
  /// The two soldiers of Kalay at the town's entrance from the world map.
  /// ★ Named from the game: "Welcome to Kalay! You may enter, so long as you
  /// do not disrupt the peace." (MsgKalayWelcomeInPeace)
  LOCALID_WELCOMING_SOLDIER = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "We soldiers protect the peace in Kalay with our
  /// lives." (MsgKalaySoldiersProtectPeace)
  LOCALID_SOLDIER_PROTECTING_THE_PEACE,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "Lord Hammet, the founder of Kalay, is a great
  /// merchant." (MsgKalayHammetGenerousMerchant)
  LOCALID_YOUNG_MAN_ON_HAMMET_THE_MERCHANT,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Lord Hammet is still out with his caravans on
  /// business." (MsgKalayHammetOnJourney)
  LOCALID_WOMAN_ON_HAMMETS_CARAVANS,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  /// ★ Named from the game: "Thankfully, it has been rebuilt since then."
  /// (MsgKalayNorthernBridgeRebuilt)
  LOCALID_MAN_ON_THE_REBUILT_BRIDGE,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "I heard a rumor that there's trouble in Lamakan
  /// Desert." (MsgKalayLamakanTrouble)
  LOCALID_WOMAN_ON_LAMAKAN_DESERT,
  /// The tourist the guides are missing, an old man with a cane
  /// (SPRITE_OLD_MAN_WITH_CANE), who stands in town until then and takes his
  /// place again after.
  /// ★ Named from the game: "Uh-oh... We're short one person... Now where
  /// could he be?" (MsgKalayShortOnePerson)
  LOCALID_LATE_TOURIST,
  /// An old woman (SPRITE_OLD_WOMAN). "The guests at the inn are tourists on
  /// their way to see Colosso."
  /// ★ Named from the game: "I should have saved my money. Then I'd be on
  /// that tour, too." (MsgKalayWishSavedForFestival)
  LOCALID_WOULD_BE_TOURIST,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR). "I hear Colosso
  /// is incredible. I'd love to see such a huge carnival."
  /// ★ Named from the game: "Maybe when I'm older, I'll even get to go fight
  /// in Colosso!" (MsgKalayChildFestivalDream)
  LOCALID_BOY_DREAMING_OF_COLOSSO,
  /// A little girl with pigtails (SPRITE_GIRL_WITH_PIGTAILS).
  /// ★ Named from the game: "I think it was called Mt. Aleph."
  /// (MsgKalayTraderTownNews)
  LOCALID_GIRL_ON_MT_ALEPH,
  /// A strongman (SPRITE_STRONGMAN).
  /// ★ Named from the game: "Lord Hammet's knack for business is the reason
  /// Kalay has flourished." (MsgKalayHammetBusinessSkill)
  LOCALID_STRONGMAN_ON_HAMMETS_KNACK,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN) who came to Kalay from
  /// Tolbi, afraid of Babi (Kalay_TalkAboutLeavingHome).
  /// ★ Named from the game: "I came to Kalay because I didn't like Tolbi..."
  /// (MsgKalayCameKalayBecauseDidntLike)
  LOCALID_MAN_WHO_LEFT_TOLBI,
  /// The Colosso tour's guides, men carrying packs
  /// (SPRITE_MAN_CARRYING_A_PACK), who stand in town and lead the tour off
  /// when it leaves.
  /// ★ Named from the game: "Are we all listening? Let's stop right here."
  /// (MsgKalayOkListeningLets)
  LOCALID_HEAD_GUIDE,
  /// ★ Named from the game: "We're just checking to make sure no one is
  /// missing!" (MsgKalayWhyWeStoppingAtPlace)
  LOCALID_SECOND_GUIDE,
  /// ★ Named from the game: "These weapons are good, but they're not the best
  /// Kalay makes." (MsgKalayLordHammetSellsHisBest)
  LOCALID_WEAPON_MERCHANT,
  /// ★ Named from the game: "Ask me anything about armor."
  /// (MsgArmorShopWelcome)
  LOCALID_ARMOR_MERCHANT,
  /// ★ Named from the game: "Welcome! We have just the gear you need."
  /// (MsgItemShopWelcome)
  LOCALID_ITEM_MERCHANT,
  /// The statue (with the Kalay Tunnel's sprite) that Move pushes aside from
  /// the way down into the Kalay Tunnel.
  /// ★ Named from what the player sees: a stone statue on a pedestal at the
  /// town's north edge (its render)
  LOCALID_TOWN_STATUE,
  /// Circles in the air until Catch pulls it in.
  /// ★ Named from the game: "Nut" (MsgItemName)
  LOCALID_TOWN_NUT,
  /// The tour group, LOCALID_FIRST_TOURIST and the six after it, then
  /// LOCALID_OURANOS: in row order a chef (SPRITE_CHEF), a young woman in
  /// green, a young man with brown hair, a woman with fair hair, a woman with a
  /// sword, a bald, muscular man, a young man with blue hair, and Ouranos, a
  /// young man with red hair.
  /// ★ Named from the game: "Why are we stopping at a place like this?"
  /// (MsgKalayWhyWeStoppingAtPlace)
  LOCALID_FIRST_TOURIST,
  /// The tourist the one beside him calls Ouranos.
  /// ★ Named from the game: "It's just a shame a man like you can't ride on a
  /// boat, Ouranos." (MsgKalayDecideEnteringColosso)
  LOCALID_OURANOS = LOCALID_FIRST_TOURIST + 7
};

/// Hammet Palace's front (HammetPalaceGrounds), numbering
/// gKalay_HammetPalaceObjectEventTemplates, whose first row places the party's
/// Ivan.
enum KalayPalaceLocalId {
  /// The soldiers of Kalay before the palace door. "This is Lord Hammet's
  /// palace. The lord is away."
  /// ★ Named from what the player sees: two soldiers of Kalay standing
  /// guard at the palace door (its render)
  LOCALID_PALACE_GUARD = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: the second soldier at the palace
  /// door (its render)
  LOCALID_SECOND_PALACE_GUARD
};

/// The Kalay Tunnel Entrance (KalayTunnelEntrance), numbering
/// gKalay_TunnelEntranceObjectEventTemplates after its rows for Garet, Ivan and
/// Mia.
enum KalayTunnelEntranceLocalId {
  /// A soldier of Kalay who keeps the passage closed by Hammet and Layana's
  /// command.
  /// ★ Named from the game: "Do you want to go to the cave up ahead?"
  /// (MsgKalayDoWantGoCaveUp)
  LOCALID_TUNNEL_GUARD = LOCALID_FIRST_PLACED,
  /// Bunza's.
  /// ★ Named from what the player sees: a covered wagon drawn by a horse
  /// (its render)
  LOCALID_WAGON,
  /// ★ Named from the game: "We've arrived, Hammet."
  /// (MsgKalayWeveArrivedHammet)
  LOCALID_HAMMET,
  /// ★ Named from the game: "Is that you, Bunza?" (MsgLunpaMoment)
  LOCALID_BUNZA,
  /// Floats here until Catch takes it.
  /// ★ Named from the game: "Nut" (MsgItemName)
  LOCALID_TUNNEL_ENTRANCE_NUT
};

/// The Kalay Tunnel's treasure room (KalayTunnelTreasureRoom), numbering
/// gKalay_TunnelObjectEventTemplates.
enum KalayTunnelLocalId {
  /// The statues that open the two passages.
  /// ★ Named from what the player sees: two stone statues on pedestals that
  /// Move slides west (its render)
  LOCALID_FIRST_STATUE = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: the second stone statue, east of the
  /// first (its render)
  LOCALID_SECOND_STATUE,
  /// A man drawn with SPRITE_BUNZA who waits here once FLAG_KALAY_TUNNEL_OPEN
  /// is set.
  /// ★ Named from the game: "Layana was very hard on all of you, wasn't she?"
  /// (MsgKalayLayanaWasVeryHardOn)
  LOCALID_WAITING_MAN,
  /// The sprite of the puddle that Frost freezes into an ice pillar in
  /// bilibin_cave.c.
  /// ★ Named from what the player sees: a puddle of water, SPRITE_PUDDLE
  /// (its render)
  LOCALID_PUDDLE,
  /// A Mars Djinni.
  /// ★ Named from the game: "Scorch" (MsgDjinnName)
  LOCALID_SCORCH
};

/// SPRITE_PUDDLE's animation once Frost has frozen it into an ice pillar.
enum { PUDDLE_ANIMATION_FROZEN = 5 };

/// The Kalay Tunnel's own flags: the temporary ones last one entry, the
/// passages (map-section temp flags) while the party stays in the tunnel's
/// maps.
enum KalayTunnelFlag {
  /// The fade overlay follows the leader.
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER = FLAG_MAP_TEMP_00,
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_PUDDLE_FROZEN = FLAG_MAP_TEMP_01,
  /// Each statue's particles have started their sound.
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED = FLAG_MAP_TEMP_02,
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED = FLAG_MAP_TEMP_03,
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_FIRST_PASSAGE_OPEN = FLAG_MAPSEC_TEMP_02,
  /// ★ Named from its area name: Kalay Tunnel
  FLAG_TUNNEL_SECOND_PASSAGE_OPEN = FLAG_MAPSEC_TEMP_03
};

static void Kalay_PrepareTown(void);
static void Kalay_PrepareHammetPalace(void);
static void Kalay_PrepareTunnelEntrance(void);
static void Kalay_StartWindowTransition(void);
static void Kalay_PrepareTunnel(void);
static void Kalay_StartTouristsFidgeting(void);
static void Kalay_StartTouristFidgeting(s32 localId);
static void Kalay_StartGuidesPacing(void);
static void Kalay_AlternateGuidesPacing(void);
static void Kalay_IvanMeetsPalaceGuards(void);
static void Kalay_HammetReturnsToPalace(void);
static void Kalay_UpdateTunnelPassages(void);
static void Kalay_UpdateTunnelEmitters(void);
static void Kalay_SpawnTunnelParticle(s32 x, s32 height, s32 z);
static void Kalay_UpdateNutOrbit(void);

/// Clears the flag a guide's script sets when it is done.
static __inline__ void Kalay_ClearScriptDone(struct ObjectEvent *objectEvent) {
  s32 completed = 0;

  objectEvent->timer = completed;
}

/// Talking to Scorch, a Mars Djinni, in the Kalay Tunnel
/// (KalayTunnelTreasureRoom) adds it to the party, without a battle.
static void Kalay_ScorchJoins(void) {
  Djinn_JoinParty(LOCALID_SCORCH, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH));
}

/// In Kalay, while the tour waits, a tourist now and then nods, shakes his head
/// or turns.
static s32 Kalay_UpdateTouristFidget(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  u32 action;
  u32 nextTimer;

  if ((s16)objectEvent->work == 0) {
    action = (u32)(Engine_RandomNext() * 8) >> 0x10;
    switch (action) {
    case 0:
      ObjectDispatch_SelectAnimation(objectEvent, 3);
      break;
    case 1:
      ObjectDispatch_SelectAnimation(objectEvent, 4);
      break;
    case 3:
    case 4:
      objectEvent->facing += (u32)(Engine_RandomNext() << 0xF) >> 0x10;
      break;
    }
    nextTimer = (u32)(Engine_RandomNext() * 0x50) >> 0x10;
    objectEvent->work = nextTimer;
  }
  if ((s16)objectEvent->work != 0) {
    objectEvent->work = (s16)objectEvent->work - 1;
  }
  return 1;
}

// Shared source, included in link order.
#include "common/object_heading.h"

const struct MapWarpDest *Kalay_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Kalay) {
    return gKalay_TownWarpDests;
  }
  if (mapId == (s32)&MapId_HammetPalaceGrounds) {
    return gKalay_HammetPalaceWarpDests;
  }
  if (mapId == (s32)&MapId_KalayTunnelEntrance) {
    return gKalay_TunnelEntranceWarpDests;
  }
  if (mapId == (s32)&MapId_KalayTunnelCave) {
    return gKalay_TunnelCaveWarpDests;
  }
  if (mapId == (s32)&MapId_KalayTunnelPassage) {
    return gKalay_TunnelPassageWarpDests;
  }
  if (mapId == (s32)&MapId_KalayTunnelTreasureRoom) {
    return gKalay_TunnelWarpDests;
  }
  return gKalay_NoWarpDests;
}

const struct MapWarpRegion *Kalay_GetRegions(void) { return NULL; }

/// The numbered warps of all six maps.
const u32 *Kalay_GetWarps(void) { return gKalay_Warps; }

/// The people of Kalay, Hammet Palace's front, the Kalay Tunnel Entrance and
/// the Kalay Tunnel's treasure room.
const struct ObjectEventTemplate *Kalay_GetObjectEventTemplates(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Kalay) {
    return gKalay_TownObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_HammetPalaceGrounds) {
    return gKalay_HammetPalaceObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_KalayTunnelEntrance) {
    return gKalay_TunnelEntranceObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_KalayTunnelTreasureRoom) {
    return gKalay_TunnelObjectEventTemplates;
  }
  return gKalay_NoObjectEventTemplates;
}

/// Searching the well in Kalay: the party peers in, and it is very clean.
static void Kalay_SearchWell(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgKalayVeryCleanMaintained, 1);
  FieldScript_End();
}

/// Catch at the Kalay Tunnel Entrance pulls in the Nut floating there, until it
/// is caught (FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT).
static void Kalay_CatchTunnelEntranceNut(void) {
  FieldScript_Start();
  Engine_ObjectEventSetPosition(LOCALID_TUNNEL_ENTRANCE_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT);
  FieldEffect_StartEffectObject22(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  FieldScript_End();
}

/// The conversations, warps and Psynergy targets of each map.
const struct MapEvent *Kalay_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Kalay) {
    return gKalay_TownEvents;
  }
  if (mapId == (s32)&MapId_HammetPalaceGrounds) {
    return gKalay_HammetPalaceEvents;
  }
  if (mapId == (s32)&MapId_KalayTunnelEntrance) {
    return gKalay_TunnelEntranceEvents;
  }
  if (mapId == (s32)&MapId_KalayTunnelCave) {
    return gKalay_TunnelCaveEvents;
  }
  if (mapId == (s32)&MapId_KalayTunnelPassage) {
    return gKalay_TunnelPassageEvents;
  }
  if (mapId == (s32)&MapId_KalayTunnelTreasureRoom) {
    return gKalay_TunnelEvents;
  }
  return gKalay_NoEvents;
}

/// A man in Kalay tells why he moved there: he left Tolbi, afraid of Babi, who
/// has ruled it for more than a hundred years.
static void Kalay_TalkAboutLeavingHome(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgKalayCameKalayBecauseDidntLike);
  FieldScript_AskYesNoAndShowResponse(LOCALID_MAN_WHO_LEFT_TOLBI, 0);
  FieldScript_End();
}

/// Spoken to from any side but the north, the guard at the Kalay Tunnel
/// Entrance asks `Do you want to go to the cave up ahead?`, and turns the
/// party away either way.
static void Kalay_AskAboutCaveAccess(void) {
  struct ObjectEvent *leader;
  u32 relativeFacing;
  u16 *message;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  relativeFacing = (leader->facing + 0xfffff000) << 16;
  if (relativeFacing > 0x60000000) {
    FieldScript_Start();
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_TUNNEL_GUARD, 0);
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(MsgKalayDoWantGoCaveUp);
    FieldMessage_ShowAtObject(LOCALID_TUNNEL_GUARD, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      ObjectMotion_SetAnimationAndWait(LOCALID_TUNNEL_GUARD, ANIM_SHAKE_HEAD);
      FieldScript_ShowMessageAndWait(LOCALID_TUNNEL_GUARD, 0);
    } else {
      message = &gFieldWork->message;
      *message = *message + 1;
      ObjectMotion_SetAnimationAndWait(LOCALID_TUNNEL_GUARD, ANIM_NOD);
      FieldScript_ShowMessageAndWait(LOCALID_TUNNEL_GUARD, 0);
    }
    FieldScript_End();
  }
}

/// A man asks whether Layana was too hard on the party. He's in the Kalay
/// Tunnel (KalayTunnelTreasureRoom) once Hammet is home, and means the time she
/// told the party to leave.
static void Kalay_TalkAboutLayana(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgKalayLayanaWasVeryHardOn);
  FieldScript_AskYesNoAndShowResponse(LOCALID_WAITING_MAN, 0);
  FieldScript_End();
}

/// In Kalay, across the counter, the leader facing north, the weapon shop
/// opens; from elsewhere the merchant grumbles that Hammet sells his best
/// weapons away.
static void Kalay_TalkToWeaponMerchant(void) {
  struct ObjectEvent *leader;
  u32 relativeFacing;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x2000) << 16;
  if (relativeFacing > 0x80000000) {
    Shop_Run(22, LOCALID_WEAPON_MERCHANT);
  } else {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgKalayLordHammetSellsHisBest);
    FieldScript_ShowMessageAndWait(LOCALID_WEAPON_MERCHANT, 0);
    FieldScript_End();
  }
}

/// In Kalay, across the counter, the leader facing west to north, the armor
/// shop opens; from elsewhere the merchant asks whether the party found its
/// weapons.
static void Kalay_TalkToArmorMerchant(void) {
  struct ObjectEvent *leader;
  u32 relativeFacing;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x6001) << 16;
  if (relativeFacing <= 0x7ffe0000) {
    Shop_Run(23, LOCALID_ARMOR_MERCHANT);
  } else {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgKalayDidFindNeededInWeapon);
    FieldScript_AskYesNoAndShowResponse(LOCALID_ARMOR_MERCHANT, 0);
    FieldScript_End();
  }
}

/// In Kalay, across the counter, the leader facing east, the item shop opens;
/// from elsewhere the merchant praises Lady Layana.
static void Kalay_TalkToItemMerchant(void) {
  struct ObjectEvent *leader;
  u32 relativeFacing;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x2000) << 16;
  if (relativeFacing > 0xC0000000) {
    Shop_Run(24, LOCALID_ITEM_MERCHANT);
  } else {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgKalayLadyLayanaSharedInLord);
    FieldScript_ShowMessageAndWait(LOCALID_ITEM_MERCHANT, 0);
    FieldScript_End();
  }
}

/// Walking north into one of Kalay's eight doors (coord events 1 to 8), into
/// the houses, the inn or the Sanctum: everyone stops, the door opens and the
/// leader walks in.
static void Kalay_OpenDoorAndExit(void) {
  struct EventWork *work;
  struct ObjectEvent *objectEvent;
  s16 *trigger;
  u32 i;
  s32 door;

  work = gFieldWork;
  FieldScript_Start();
  for (i = LOCALID_FIRST_PLACED; i <= LOCALID_LAST_SLOT; i++) {
    objectEvent = ObjectEvent_GetById(i);
    if (objectEvent != NULL) {
      objectEvent->motionFlags = 0;
    }
  }
  trigger = &work->coordEvent;
  door = *trigger - 1;
  Sound_PlaySong(SE_DOOR_OPEN);
  {
    s32 x = gKalay_DoorCells[door].x;
    s32 y = gKalay_DoorCells[door].y;

    MapGrid_AnimateCells(gKalay_DoorCells[door].steps, x, y);
  }
  ObjectMotion_SetSpeedParameters(LOCALID_PARTY_LEADER, PIXELS(1) / 2,
                                  PIXELS(1) / 4);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -8);
  FieldScript_WaitFrames(10);
  Engine_ScriptRequestWarp(*trigger);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
}

/// The palace door opens in two frames.
static __inline__ void Kalay_AnimatePalaceDoor(void) {
  Script_PlaySong(SE_188);
  MapGrid_CopyCellsTo(36, 23, 43, 12, 2, 2);
  WaitFrames(5);
  MapGrid_CopyCellsTo(39, 23, 43, 12, 2, 2);
  WaitFrames(5);
}

/// Walking north into the door of Hammet Palace (HammetPalaceGrounds, coord
/// event 2): it opens and the leader walks in.
static void Kalay_EnterPalace(void) {
  FieldScript_Start();
  Kalay_AnimatePalaceDoor();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -8);
  FieldScript_WaitFrames(10);
  Engine_ScriptRequestWarp(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
}

/// Leaving Kalay (coord events 9 and 10) after hearing of the Colosso tour at
/// the inn, until the tour has left: the party watches the tour group stop. The
/// guides count everyone, pace back and forth while the tourists grumble, give
/// up on the missing man and lead the group off; the late tourist then runs
/// after it. Otherwise the party leaves.
static void Kalay_WatchTourGroupDeparture(void) {
  s32 message;
  struct ObjectEvent *headWait;
  struct ObjectEvent *lastWait;
  struct ObjectEvent *closing;
  struct ObjectEvent *late;
  struct ObjectEvent *headEnd;
  struct ObjectEvent *lastEnd;
  struct ObjectEvent *leader;
  struct ObjectEvent *last;
  struct ObjectEvent *head;
  struct ObjectEvent *headBack;
  struct ObjectEvent *headAgain;
  struct ObjectEvent *headHome;
  struct EventWork *work;

  work = gFieldWork;
  FieldScript_Start();
  if (GameFlag_IsSet(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR) != 0 &&
      GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0) {
    FieldScript_CallWithLastActiveObjectId(
        gKalay_TourGroupObjectEventTemplates);
    ObjectEvent_SetPosition(LOCALID_HEAD_GUIDE, PIXELS(252), PIXELS(272));
    ObjectEvent_SetPosition(LOCALID_FIRST_TOURIST, PIXELS(284), PIXELS(264));
    ObjectEvent_SetPosition(LOCALID_FIRST_TOURIST + 1, PIXELS(284),
                            PIXELS(280));
    ObjectEvent_SetPosition(LOCALID_FIRST_TOURIST + 2, PIXELS(300),
                            PIXELS(264));
    ObjectEvent_SetPosition(LOCALID_FIRST_TOURIST + 3, PIXELS(300),
                            PIXELS(280));
    ObjectEvent_SetPosition(LOCALID_FIRST_TOURIST + 5, PIXELS(316),
                            PIXELS(264));
    ObjectEvent_SetPosition(LOCALID_FIRST_TOURIST + 4, PIXELS(316),
                            PIXELS(280));
    ObjectEvent_SetPosition(LOCALID_FIRST_TOURIST + 6, PIXELS(332),
                            PIXELS(264));
    ObjectEvent_SetPosition(LOCALID_OURANOS, PIXELS(332), PIXELS(280));
    ObjectEvent_SetPosition(LOCALID_SECOND_GUIDE, PIXELS(364), PIXELS(272));
    Script_PlaySong(SONG_FADE_OUT_BGM);
    Message_SetVoiceFromObjectEvent(LOCALID_HEAD_GUIDE);
    FieldMessage_ShowCentered(MsgKalayOkListeningLets, 1, 0);
    Script_PlaySong(MUS_009);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
    if (work->coordEvent == 9) {
      FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 20);
    } else {
      FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 20);
    }
    // Stagger the speeds so the group arrives in rows.
    ObjectEvent_SetSpeed(LOCALID_HEAD_GUIDE, PIXELS(11) / 10, PIXELS(11) / 20);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 1, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 2, PIXELS(9) / 10,
                         PIXELS(9) / 20);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 3, PIXELS(9) / 10,
                         PIXELS(9) / 20);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 5, PIXELS(4) / 5,
                         PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 4, PIXELS(4) / 5,
                         PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 6, PIXELS(7) / 10,
                         PIXELS(7) / 20);
    ObjectEvent_SetSpeed(LOCALID_OURANOS, PIXELS(7) / 10, PIXELS(7) / 20);
    ObjectEvent_SetSpeed(LOCALID_SECOND_GUIDE, PIXELS(3) / 5, PIXELS(3) / 10);
    ObjectEvent_SetActionScript(LOCALID_HEAD_GUIDE, gKalay_TourGroupWalkScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST,
                                gKalay_TourGroupWalkScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 1,
                                gKalay_TourGroupWalkScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 2,
                                gKalay_TourGroupWalkScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 3,
                                gKalay_TourGroupWalkScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 5,
                                gKalay_TourGroupWalkScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 4,
                                gKalay_TourGroupWalkScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 6,
                                gKalay_TourGroupWalkScript);
    ObjectEvent_SetActionScript(LOCALID_OURANOS, gKalay_TourGroupWalkScript);
    last = ObjectEvent_Get(LOCALID_SECOND_GUIDE);
    last->timer = 0;
    ObjectEvent_SetActionScript(LOCALID_SECOND_GUIDE,
                                gKalay_TourGroupWalkScript);
    FieldCamera_MoveTo(PIXELS(186), -1, PIXELS(272), 1);
    ObjectEvent_RefreshSelectorById(LOCALID_HEAD_GUIDE);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE, FACING_EAST, 0);
    do {
      WaitFrames(1);
    } while (last->timer == 0);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST, 2);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST, FACING_SOUTH + FACING_STEP,
                              20);
    FieldScript_SetMessageId(MsgKalayWhyWeStoppingAtPlace);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST, 0, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST + 1, 2);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 1,
                              FACING_NORTHWEST + FACING_STEP, 10);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST + 1, ANIM_NOD);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 1, 0, 10);
    ObjectEvent_SetAttachedEffect(LOCALID_FIRST_TOURIST + 5,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 5, 0, 10);
    ObjectEvent_ShowEmote(LOCALID_FIRST_TOURIST + 4,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 4,
                              FACING_NORTHWEST + FACING_STEP, 10);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 4, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 4, FACING_WEST, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST + 4, 2);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST + 4, ANIM_SHAKE_HEAD);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 4, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 4,
                              FACING_NORTHWEST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 5,
                              FACING_SOUTH + FACING_STEP, 20);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST + 4, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TOURIST + 5, ANIM_NOD);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEAD_GUIDE, 2);
    ObjectEvent_SetAttachedEffect(LOCALID_HEAD_GUIDE, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEAD_GUIDE, 2);
    head = ObjectEvent_Get(LOCALID_HEAD_GUIDE);
    head->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
    ObjectEvent_WalkToAndWait(LOCALID_HEAD_GUIDE, 172, 264);
    FieldScript_WaitFrames(1);
    headBack = ObjectEvent_Get(LOCALID_HEAD_GUIDE);
    headBack->steeringFlags |= OBJ_EVENT_FACES_MOTION;
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 1, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 5, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 4, FACING_WEST, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEAD_GUIDE, ANIM_NOD);
    FieldScript_WaitFrames(20);
    headAgain = ObjectEvent_Get(LOCALID_HEAD_GUIDE);
    headAgain->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
    ObjectEvent_WalkToAndWait(LOCALID_HEAD_GUIDE, 172, 272);
    FieldScript_WaitFrames(1);
    headHome = ObjectEvent_Get(LOCALID_HEAD_GUIDE);
    headHome->steeringFlags |= OBJ_EVENT_FACES_MOTION;
    ObjectEvent_WalkToAndWait(LOCALID_HEAD_GUIDE, 180, 272);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE, FACING_EAST, 0);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 10);
    ObjectEvent_ShowEmote(LOCALID_OURANOS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_OURANOS, 1);
    ObjectMotion_SetAnimationAndWait(LOCALID_OURANOS, ANIM_NOD);
    Script_ShowMessageAndWait(LOCALID_OURANOS, 0, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST + 6, 1);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 6, 0, 10);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST + 6, ANIM_SHAKE_HEAD);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 6, 0, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_GUIDE, 2);
    ObjectEvent_ShowEmote(LOCALID_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    Script_ShowMessageAndWait(LOCALID_SECOND_GUIDE, 0, 10);
    ObjectMotion_Launch(LOCALID_HEAD_GUIDE, 2, 20);
    ObjectMotion_Launch(LOCALID_HEAD_GUIDE, 4, 40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEAD_GUIDE, 2);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 10);
    ObjectEvent_SetSpeed(LOCALID_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_SECOND_GUIDE, 265, 282);
    ObjectEvent_WalkToAndWait(LOCALID_SECOND_GUIDE, 251, 284);
    ObjectEvent_WalkToAndWait(LOCALID_SECOND_GUIDE, 246, 296);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE, FACING_NORTH, 0);
    // The tourists fidget while the guides count them.
    Kalay_StartTouristsFidgeting();
    FieldScript_WaitFrames(40);
    ObjectEvent_SetSpeed(LOCALID_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_SECOND_GUIDE, 228, 296);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE, FACING_NORTH, 40);
    ObjectEvent_WalkToAndWait(LOCALID_SECOND_GUIDE, 212, 296);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE, FACING_NORTH, 40);
    ObjectEvent_WalkToAndWait(LOCALID_SECOND_GUIDE, 192, 296);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE, FACING_NORTH, 40);
    ObjectMotion_SetVariantCallback(LOCALID_SECOND_GUIDE, 2);
    ObjectEvent_ShowEmote(LOCALID_SECOND_GUIDE,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE,
                              FACING_SOUTHEAST + FACING_STEP, 0);
    ObjectEvent_WalkToAndWait(LOCALID_SECOND_GUIDE, 184, 286);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE,
                              FACING_NORTHWEST + FACING_STEP, 10);
    ObjectEvent_SetAttachedEffect(
        LOCALID_SECOND_GUIDE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_GUIDE, ANIM_SHAKE_HEAD);
    ObjectEvent_ShowEmote(LOCALID_HEAD_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          40);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEAD_GUIDE, ANIM_NOD);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEAD_GUIDE, 2);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE, FACING_EAST, 0);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE, FACING_EAST, 60);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE,
                              FACING_SOUTHEAST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE,
                              FACING_NORTHWEST + FACING_STEP, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEAD_GUIDE, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_GUIDE, ANIM_NOD);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE, FACING_EAST, 0);
    ObjectEvent_SetSpeed(LOCALID_HEAD_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_RunActionScriptAndWait(LOCALID_HEAD_GUIDE,
                                       gKalay_GuidePaceScript);
    ObjectEvent_WalkToAndWait(LOCALID_HEAD_GUIDE, 228, 296);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE, FACING_NORTH, 40);
    ObjectEvent_WalkToAndWait(LOCALID_HEAD_GUIDE, 212, 296);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE, FACING_NORTH, 40);
    ObjectEvent_WalkToAndWait(LOCALID_HEAD_GUIDE, 192, 296);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE, FACING_NORTH, 40);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE,
                              FACING_NORTHWEST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_SECOND_GUIDE,
                              FACING_SOUTHEAST + FACING_STEP, 10);
    ObjectEvent_SetAttachedEffect(LOCALID_HEAD_GUIDE, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEAD_GUIDE, ANIM_SHAKE_HEAD);
    message = MsgKalayShortOnePerson;
    FieldScript_SetMessageId(message);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 40);
    // The guides pace while the tourists complain one by one.
    Kalay_StartGuidesPacing();
    ObjectEvent_WalkToAndWait(LOCALID_HEAD_GUIDE, 178, 272);
    ObjectEvent_FaceDirection(LOCALID_HEAD_GUIDE, FACING_EAST, 0);
    FieldScript_WaitFrames(240);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST);
    WaitFrames(1);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST, FACING_WEST, 10);
    ObjectEvent_ShowEmote(LOCALID_FIRST_TOURIST,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST, 0, 10);
    Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST);
    FieldScript_WaitFrames(80);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 1);
    WaitFrames(1);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 1,
                              FACING_NORTH + FACING_STEP, 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST + 1, 2);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 1, 0, 10);
    Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 1);
    FieldScript_WaitFrames(160);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 5);
    WaitFrames(1);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 5,
                              FACING_SOUTH + FACING_STEP, 10);
    ObjectEvent_ShowEmote(LOCALID_FIRST_TOURIST + 5,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 5, 0, 10);
    Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 5);
    FieldScript_WaitFrames(80);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 3);
    WaitFrames(1);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 3,
                              FACING_NORTHWEST + FACING_STEP, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST + 3, 1);
    FieldScript_SetMessageId(message + 6);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 3, 0, 10);
    Task_Remove(Kalay_AlternateGuidesPacing);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_HEAD_GUIDE);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_SECOND_GUIDE);
    WaitFrames(1);
    headWait = ObjectEvent_Get(LOCALID_HEAD_GUIDE);
    headWait->timer = 0;
    lastWait = ObjectEvent_Get(LOCALID_SECOND_GUIDE);
    lastWait->timer = 0;
    ObjectEvent_SetSpeed(LOCALID_HEAD_GUIDE, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_SECOND_GUIDE, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetActionScript(LOCALID_HEAD_GUIDE, gKalay_HeadGuideStopScript);
    ObjectEvent_SetActionScript(LOCALID_SECOND_GUIDE,
                                gKalay_SecondGuideStopScript);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 2);
    WaitFrames(1);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 2,
                              FACING_SOUTH + FACING_STEP, 10);
    message += 5;
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST + 2, 2);
    FieldScript_SetMessageId(message);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 2, 0, 20);
    Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 2);
    Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 3);
    do {
      WaitFrames(1);
    } while (ObjectEvent_Get(LOCALID_HEAD_GUIDE)->timer == 0 ||
             ObjectEvent_Get(LOCALID_SECOND_GUIDE)->timer != 1);
    ObjectEvent_SetActionScript(LOCALID_HEAD_GUIDE,
                                gKalay_HeadGuideReturnScript);
    ObjectEvent_SetActionScript(LOCALID_SECOND_GUIDE,
                                gKalay_SecondGuideReturnScript);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 4);
    WaitFrames(1);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 4,
                              FACING_SOUTH + FACING_STEP, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST + 4, 1);
    ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TOURIST + 4,
                                     ANIM_SHAKE_HEAD);
    message = MsgKalayDecideEnteringColosso;
    FieldScript_SetMessageId(message);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 4, 0, 10);
    Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 4);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_OURANOS);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 6);
    WaitFrames(1);
    ObjectEvent_ShowEmote(LOCALID_OURANOS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    ObjectEvent_ShowEmote(LOCALID_FIRST_TOURIST + 6,
                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    ObjectEvent_FaceDirection(LOCALID_OURANOS, FACING_NORTHWEST + FACING_STEP,
                              10);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 6,
                              FACING_SOUTH + FACING_STEP, 10);
    message += 3;
    ObjectMotion_SetAnimationAndWait(LOCALID_OURANOS, ANIM_SHAKE_HEAD);
    FieldScript_SetMessageId(message);
    Script_ShowMessageAndWait(LOCALID_OURANOS, 0, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_TOURIST + 6, 1);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST + 6, ANIM_SHAKE_HEAD);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 6, 0, 10);
    ObjectEvent_ShowEmote(LOCALID_OURANOS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    ObjectEvent_ShowEmote(LOCALID_HEAD_GUIDE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                          0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEAD_GUIDE, 2);
    FieldScript_SetMessageId(MsgKalayThatsWeCantWaitAny);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 10);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 1);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 2);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 3);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 5);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 4);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 6);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_OURANOS);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_HEAD_GUIDE);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_SECOND_GUIDE);
    WaitFrames(1);
    ObjectMotion_Launch(LOCALID_FIRST_TOURIST, 2, 0);
    ObjectMotion_Launch(LOCALID_FIRST_TOURIST + 1, 2, 0);
    ObjectMotion_Launch(LOCALID_FIRST_TOURIST + 2, 2, 0);
    ObjectMotion_Launch(LOCALID_FIRST_TOURIST + 3, 2, 0);
    ObjectMotion_Launch(LOCALID_FIRST_TOURIST + 5, 2, 0);
    ObjectMotion_Launch(LOCALID_FIRST_TOURIST + 4, 2, 0);
    ObjectMotion_Launch(LOCALID_FIRST_TOURIST + 6, 2, 0);
    ObjectMotion_Launch(LOCALID_OURANOS, 2, 0);
    ObjectMotion_Launch(LOCALID_SECOND_GUIDE, 2, 40);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 1, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 2, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 3, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 5, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 4, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 6, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_OURANOS, FACING_WEST, 40);
    ObjectMotion_Launch(LOCALID_SECOND_GUIDE, 4, 40);
    Script_ShowMessageAndWait(LOCALID_SECOND_GUIDE, 0, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEAD_GUIDE, 1);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_GUIDE, ANIM_NOD);
    Script_ShowMessageAndWait(LOCALID_SECOND_GUIDE, 0, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEAD_GUIDE, ANIM_NOD);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 10);
    ObjectEvent_ShowEmote(LOCALID_FIRST_TOURIST, EMOTE_IN_FRONT | EMOTE_WINCE,
                          40);
    ObjectMotion_SetVariantCallback(LOCALID_FIRST_TOURIST, 1);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST, 0, 10);
    ObjectEvent_ShowEmote(LOCALID_FIRST_TOURIST + 1,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Script_ShowMessageAndWait(LOCALID_FIRST_TOURIST + 1, 0, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_GUIDE, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_GUIDE, ANIM_NOD);
    Script_ShowMessageAndWait(LOCALID_SECOND_GUIDE, 0, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEAD_GUIDE, ANIM_NOD);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST, FACING_SOUTH + FACING_STEP,
                              0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 1,
                              FACING_NORTHWEST + FACING_STEP, 4);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 2,
                              FACING_SOUTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 3,
                              FACING_NORTHWEST + FACING_STEP, 4);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 5,
                              FACING_SOUTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 4,
                              FACING_NORTHWEST + FACING_STEP, 4);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 6,
                              FACING_SOUTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_OURANOS, FACING_NORTHWEST + FACING_STEP,
                              4);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TOURIST + 1, ANIM_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST + 2, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TOURIST + 3, ANIM_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST + 5, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_TOURIST + 4, ANIM_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_TOURIST + 6, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_OURANOS, ANIM_NOD);
    ObjectMotion_Launch(LOCALID_HEAD_GUIDE, 2, 40);
    Script_ShowMessageAndWait(LOCALID_HEAD_GUIDE, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 1, FACING_WEST, 4);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 2, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 3, FACING_WEST, 4);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 5, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 4, FACING_WEST, 4);
    ObjectEvent_FaceDirection(LOCALID_FIRST_TOURIST + 6, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_OURANOS, FACING_WEST, 4);
    ObjectEvent_SetSpeed(LOCALID_HEAD_GUIDE, PIXELS(11) / 10, PIXELS(11) / 20);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST, PIXELS(21) / 20,
                         PIXELS(21) / 40);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 1, PIXELS(21) / 20,
                         PIXELS(21) / 40);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 2, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 3, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 5, PIXELS(19) / 20,
                         PIXELS(19) / 40);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 4, PIXELS(19) / 20,
                         PIXELS(19) / 40);
    ObjectEvent_SetSpeed(LOCALID_FIRST_TOURIST + 6, PIXELS(9) / 10,
                         PIXELS(9) / 20);
    ObjectEvent_SetSpeed(LOCALID_OURANOS, PIXELS(9) / 10, PIXELS(9) / 20);
    ObjectEvent_SetSpeed(LOCALID_SECOND_GUIDE, PIXELS(17) / 20,
                         PIXELS(17) / 40);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_TOURIST, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_TOURIST + 1, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_TOURIST + 2, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_TOURIST + 3, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_TOURIST + 5, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_TOURIST + 4, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_TOURIST + 6, 1);
    ObjectMotion_SetActionVariant(LOCALID_OURANOS, 1);
    ObjectMotion_SetActionVariant(LOCALID_HEAD_GUIDE, 1);
    ObjectMotion_SetActionVariant(LOCALID_SECOND_GUIDE, 1);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 1);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 2);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 3);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 5);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 4);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_TOURIST + 6);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_OURANOS);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_HEAD_GUIDE);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_SECOND_GUIDE);
    WaitFrames(1);
    // Send the group onward, then bring in the missing tourist.
    ObjectEvent_SetActionScript(LOCALID_HEAD_GUIDE,
                                gKalay_TourGroupLeaveScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST,
                                gKalay_TourGroupLeaveScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 1,
                                gKalay_TourGroupLeaveScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 2,
                                gKalay_TourGroupLeaveScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 3,
                                gKalay_TourGroupLeaveScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 5,
                                gKalay_TourGroupLeaveScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 4,
                                gKalay_TourGroupLeaveScript);
    ObjectEvent_SetActionScript(LOCALID_FIRST_TOURIST + 6,
                                gKalay_TourGroupLeaveScript);
    ObjectEvent_SetActionScript(LOCALID_OURANOS, gKalay_TourGroupLeaveScript);
    closing = ObjectEvent_Get(LOCALID_SECOND_GUIDE);
    closing->timer = 0;
    ObjectEvent_SetActionScript(LOCALID_SECOND_GUIDE,
                                gKalay_TourGroupLeaveScript);
    do {
      WaitFrames(1);
    } while (ObjectEvent_Get(LOCALID_SECOND_GUIDE)->timer != 1);
    FieldScript_WaitFrames(80);
    // The late tourist finds that the rest of the group has already left.
    ObjectEvent_SetPosition(LOCALID_LATE_TOURIST, PIXELS(340), PIXELS(274));
    WaitFrames(1);
    ObjectEvent_SetSpeed(LOCALID_LATE_TOURIST, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_LATE_TOURIST, 224, 274);
    ObjectEvent_FaceDirection(LOCALID_LATE_TOURIST, FACING_EAST, 40);
    ObjectEvent_FaceDirection(LOCALID_LATE_TOURIST, FACING_WEST, 40);
    ObjectEvent_FaceDirection(LOCALID_LATE_TOURIST, FACING_NORTH, 40);
    ObjectEvent_FaceDirection(LOCALID_LATE_TOURIST, FACING_SOUTH + FACING_STEP,
                              40);
    ObjectEvent_ShowEmote(LOCALID_LATE_TOURIST, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          60);
    Script_ShowMessageAndWait(LOCALID_LATE_TOURIST, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_LATE_TOURIST, FACING_EAST, 40);
    ObjectEvent_FaceDirection(LOCALID_LATE_TOURIST, FACING_NORTH, 40);
    ObjectEvent_FaceDirection(LOCALID_LATE_TOURIST, FACING_WEST, 40);
    ObjectEvent_SetAttachedEffect(
        LOCALID_LATE_TOURIST, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectMotion_Launch(LOCALID_LATE_TOURIST, 4, 40);
    Script_ShowMessageAndWait(LOCALID_LATE_TOURIST, 0, 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_LATE_TOURIST, 2);
    Script_ShowMessageAndWait(LOCALID_LATE_TOURIST, 0, 10);
    ObjectMotion_Launch(LOCALID_LATE_TOURIST, 4, 40);
    ObjectEvent_SetSpeed(LOCALID_LATE_TOURIST, PIXELS(6) / 5, PIXELS(3) / 5);
    closing = ObjectEvent_Get(LOCALID_LATE_TOURIST);
    closing->timer = 0;
    ObjectEvent_SetActionScript(LOCALID_LATE_TOURIST,
                                gKalay_TourGroupLeaveScript);
    do {
      WaitFrames(1);
    } while (ObjectEvent_Get(LOCALID_LATE_TOURIST)->timer != 1);
    ObjectEvent_SetPosition(LOCALID_LATE_TOURIST, PIXELS(359), PIXELS(314));
    late = ObjectEvent_Get(LOCALID_LATE_TOURIST);
    late->facing = FACING_NORTH + FACING_STEP;
    ObjectEvent_SetPosition(LOCALID_HEAD_GUIDE, PIXELS(455), PIXELS(434));
    headEnd = ObjectEvent_Get(LOCALID_HEAD_GUIDE);
    headEnd->facing = FACING_NORTH + FACING_STEP;
    ObjectEvent_SetPosition(LOCALID_SECOND_GUIDE, PIXELS(464), PIXELS(416));
    lastEnd = ObjectEvent_Get(LOCALID_SECOND_GUIDE);
    lastEnd->facing = FACING_SOUTH + FACING_STEP;
    ObjectEvent_DestroyById(LOCALID_FIRST_TOURIST);
    ObjectEvent_DestroyById(LOCALID_FIRST_TOURIST + 1);
    ObjectEvent_DestroyById(LOCALID_FIRST_TOURIST + 2);
    ObjectEvent_DestroyById(LOCALID_FIRST_TOURIST + 3);
    ObjectEvent_DestroyById(LOCALID_FIRST_TOURIST + 4);
    ObjectEvent_DestroyById(LOCALID_FIRST_TOURIST + 5);
    ObjectEvent_DestroyById(LOCALID_FIRST_TOURIST + 6);
    ObjectEvent_DestroyById(LOCALID_OURANOS);
    Script_PlaySong(SONG_FADE_OUT_BGM);
    if (work->coordEvent == 9) {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 224, 458);
      leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
      leader->facing = FACING_NORTH;
    } else {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 40, 248);
      leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
      leader->facing = FACING_SOUTH;
    }
    Map_PlayMusic();
    GameFlag_Set(FLAG_KALAY_COLOSSO_TOUR_LEFT);
  } else {
    Script_PlaySong(SE_MAP_EXIT);
    Engine_ScriptRequestWarp(work->coordEvent);
    FieldScript_FinishScreenTransition();
    FieldScript_WaitTransitionFrames();
  }
  FieldScript_End();
}

/// Touching coord event 10 in the Kalay Tunnel (KalayTunnelTreasureRoom) stops
/// the fade overlay following the leader.
static void Kalay_StopTrackingLeader(void) {
  if (GameFlag_IsSet(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER) != 0) {
    gObjectEventEffectWork->objectEvent = NULL;
    GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
}

/// Touching coord event 11 in the Kalay Tunnel (KalayTunnelTreasureRoom) makes
/// the fade overlay follow the leader.
static void Kalay_StartTrackingLeader(void) {
  struct FadeOverlayWork *work;

  if (GameFlag_IsSet(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER) == 0) {
    work = gObjectEventEffectWork;
    work->objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    GameFlag_Set(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
}

/// Coord event 19 in the Kalay Tunnel (KalayTunnelTreasureRoom): the leader's
/// depth priority follows his position.
static void Kalay_EnableLeaderAutomaticPriority(void) {
  u8 *priorityFlags;
  s32 flags;

  priorityFlags = &ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags;
  flags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  flags |= *priorityFlags;
  *priorityFlags = flags;
}

/// Coord event 18 in the Kalay Tunnel (KalayTunnelTreasureRoom): the leader's
/// depth priority stays fixed.
static void Kalay_DisableLeaderAutomaticPriority(void) {
  u8 *priorityFlags;
  s32 flags;

  priorityFlags = &ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags;
  flags = (u8)~OBJ_EVENT_PRIORITY_AUTOMATIC;
  flags &= *priorityFlags;
  *priorityFlags = flags;
}

/// Catch in Kalay pulls in the Nut circling there, until it is caught.
static void Kalay_CatchTownNut(void) {
  Task_Remove(Kalay_UpdateNutOrbit);
  WaitFrames(1);
  Engine_ObjectEventSetPosition(LOCALID_TOWN_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_KALAY_TOWN_NUT_CAUGHT);
  FieldEffect_StartEffectObject22(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
}

/// Every map of the overlay sets FLAG_KALAY_REACHED, then prepares its own
/// people and cells.
s32 Kalay_Initialize(void) {
  s32 mapId;

  Engine_GameFlagSet(FLAG_KALAY_REACHED);
  mapId = gGameState.mapId;
  if (mapId == (s32)&MapId_Kalay) {
    Kalay_PrepareTown();
  } else if (mapId == (s32)&MapId_HammetPalaceGrounds) {
    Kalay_PrepareHammetPalace();
  } else if (mapId == (s32)&MapId_KalayTunnelEntrance) {
    Kalay_PrepareTunnelEntrance();
  } else if (mapId == (s32)&MapId_KalayTunnelPassage) {
    Kalay_StartWindowTransition();
  } else if (mapId == (s32)&MapId_KalayTunnelTreasureRoom) {
    Kalay_PrepareTunnel();
  }
  return 0;
}

/// Sets up Kalay. Having heard in Lunpa that Hammet is held there sets
/// map-section temp flag FLAG_MAPSEC_TEMP_21; once Hammet is free
/// (FLAG_DODONPA_LOCKED_UP) the palace's FLAG_IVAN_REPORTED_TO_LAYANA,
/// FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS and FLAG_KALAY_TUNNEL_OPEN are
/// set too, so its earlier scenes count as seen and the passage is open. Coming
/// up from the Kalay Tunnel (KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM) finds its
/// statue moved aside, and until it is caught the Nut circles in the air.
static void Kalay_PrepareTown(void) {
  OrbitingMapObject *nut;
  s32 found;
  u8 *heap;
  OrbitingMapObjectSprite *sprite;

  if (GameFlag_Test(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_21);
    Engine_GameFlagSet(FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS);
    Engine_GameFlagSet(FLAG_IVAN_REPORTED_TO_LAYANA);
    Engine_GameFlagSet(FLAG_KALAY_TUNNEL_OPEN);
  }
  if (GameFlag_Test(FLAG_LUNPA_HEARD_OF_PRISONER) != 0) {
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_21);
  }
  if (gGameState.warpDest == KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM) {
    ObjectEvent_SetPosition(LOCALID_TOWN_STATUE, PIXELS(424), PIXELS(88));
  }
  // The second guide shares the first's sprite, drawn with its green and blue
  // swapped to tell them apart.
  ObjectGroup_ConfigureChildValue(LOCALID_SECOND_GUIDE, SPRITE_COLOR_RBG);
  found = GameFlag_Test(FLAG_KALAY_TOWN_NUT_CAUGHT);
  if (found != 0) {
    Engine_ObjectEventSetPosition(LOCALID_TOWN_NUT, 0, 0);
  } else {
    nut = (OrbitingMapObject *)ObjectEvent_GetById(LOCALID_TOWN_NUT);
    sprite = nut->sprite;
    sprite->priority = 1;
    sprite->fullColor = 0;
    sprite->paletteNumber = 0;
    sprite->state = found;
    nut->active = 1;
    nut->mode = found;
    nut->y = PIXELS(10);
    nut->visible = 1;
    heap = AllocHeapSlot(17, 0x608);
    Ui_PrepareTransferForItem(ITEM_NUT);
    VramCache_Load(sprite->palette, 128, heap + 0x400);
    FreeHeapSlot(17);
    nut->orbitAngle = found;
    nut->orbitCenterX = nut->x;
    nut->orbitCenterY = nut->y;
    nut->orbitCenterZ = nut->z;
    FieldScheduler_AddCallback(Kalay_UpdateNutOrbit, TASK_PRIORITY_MAP);
  }
}

/// Sets up Hammet Palace's front (HammetPalaceGrounds): the palace flags as in
/// the town, and, the first time the party comes before Ivan has reported to
/// Layana, the guards scene.
static void Kalay_PrepareHammetPalace(void) {
  if (GameFlag_Test(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_21);
    Engine_GameFlagSet(FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS);
    Engine_GameFlagSet(FLAG_IVAN_REPORTED_TO_LAYANA);
    Engine_GameFlagSet(FLAG_KALAY_TUNNEL_OPEN);
  }
  if (GameFlag_Test(FLAG_LUNPA_HEARD_OF_PRISONER) != 0) {
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_21);
  }
  if (gGameState.warpDest != WARP_DEST_DEFAULT) {
    if (GameFlag_Test(FLAG_IVAN_REPORTED_TO_LAYANA) == 0) {
      Kalay_IvanMeetsPalaceGuards();
    }
  }
}

/// Sets up the Kalay Tunnel Entrance (KalayTunnelEntrance): the Nut floats
/// until caught, the guard turns east once FLAG_KALAY_TUNNEL_OPEN opens the
/// passage, and arriving by KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH brings
/// Hammet home in Bunza's wagon.
static void Kalay_PrepareTunnelEntrance(void) {
  struct ObjectEvent *guard;

  if (GameFlag_Test(FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT) == 0) {
    Map_FloatNut(LOCALID_TUNNEL_ENTRANCE_NUT);
  }
  if (GameFlag_Test(FLAG_KALAY_TUNNEL_OPEN) != 0) {
    guard = ObjectEvent_GetById(LOCALID_TUNNEL_GUARD);
    guard->facing = 0;
  }
  if (gGameState.warpDest == KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH) {
    Kalay_HammetReturnsToPalace();
  }
}

/// Sets up the Kalay Tunnel's passage (KalayTunnelPassage): it opens with a
/// window transition, its way through is drawn open once FLAG_KALAY_TUNNEL_OPEN
/// is set, and arriving from the treasure room
/// (KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_WEST and
/// KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_EAST) or by the stairs
/// from the north-east room
/// (KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM) starts SE_170.
static void Kalay_StartWindowTransition(void) {
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Test(FLAG_KALAY_TUNNEL_OPEN) != 0) {
    MapGrid_CopyCellsTo(58, 5, 58, 8, 2, 3);
    {
      MapGrid_CopyCellAttributes(8, 11, 2, 1, 8, 10);
    }
    MapGrid_CopyCellsTo(8, 12, 8, 11, 2, 1);
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
  }
  if (gGameState.warpDest <=
      KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM) {
    Map_QueueMusic(SE_170);
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
  struct ObjectEvent *objectEvent;
  s32 trackingValue;
  s32 column;
  struct FadeOverlayWork *effectWork;
  // HEAP_SLOT_FADE_OVERLAY is nine heap slots after HEAP_SLOT_FIELD; index
  // the actual heap slot array, not adjacent storage reached past the scalar
  // gFieldWork declaration.
  void **workSlots = gHeapSlots + HEAP_SLOT_FIELD;

  ((struct EventWork *)workSlots[0])->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  FieldEffect_StartFadeOverlay(0);
#if TBS_PATCH_1
  resumedMap = GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION);
  if (resumedMap != 0) {
    effectWork = workSlots[HEAP_SLOT_FADE_OVERLAY - HEAP_SLOT_FIELD];
    trackingValue = GameFlag_IsSet(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
#ifdef UBFIX
    if (trackingValue != 0) {
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    } else {
      objectEvent = NULL;
    }
    effectWork->objectEvent = objectEvent;
#else
    // The original integer temporary carries an object event pointer too. The
    // typed branch adds a jump and a zero load in the native English trial.
    if (trackingValue != 0) {
      trackingValue = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    }
    effectWork->objectEvent = (struct ObjectEvent *)trackingValue;
#endif
  } else {
    GameFlag_Set(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
    if (gGameState.warpDest == KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY) {
      ((struct FadeOverlayWork *)
           workSlots[HEAP_SLOT_FADE_OVERLAY - HEAP_SLOT_FIELD])
          ->objectEvent = NULL;
      GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
    }
  }
#else
  GameFlag_Set(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    effectWork = workSlots[HEAP_SLOT_FADE_OVERLAY - HEAP_SLOT_FIELD];
    trackingValue = GameFlag_IsSet(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
#ifdef UBFIX
    if (trackingValue != 0) {
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    } else {
      objectEvent = NULL;
    }
    effectWork->objectEvent = objectEvent;
#else
    // As in the earlier-edition branch, the integer also carries a pointer.
    if (trackingValue != 0) {
      trackingValue = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    }
    effectWork->objectEvent = (struct ObjectEvent *)trackingValue;
#endif
  } else if (gGameState.warpDest == KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY) {
    ((struct FadeOverlayWork *)
         workSlots[HEAP_SLOT_FADE_OVERLAY - HEAP_SLOT_FIELD])
        ->objectEvent = NULL;
    GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
#endif
  if (GameFlag_IsSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) != 0) {
    ObjectEvent_SetPosition(LOCALID_PUDDLE, PIXELS(150), PIXELS(728));
    if (GameFlag_IsSet(FLAG_TUNNEL_PUDDLE_FROZEN) != 0) {
      ObjectEvent_Get(LOCALID_PUDDLE);
      column = 9;
      Engine_ObjectEventSetAnimation(LOCALID_PUDDLE, PUDDLE_ANIMATION_FROZEN);
      MapGrid_CopyCellAttributes(0, 0, 1, 1, column, 14);
      MapGrid_CopyCellAttributes(0, 0, 1, 1, column, 45);
      {
        u8 *p = &ObjectEvent_GetById(LOCALID_PUDDLE)->priorityFlags;
        s32 v = OBJ_EVENT_PRIORITY_UNDERFOOT;

        v |= *p;
        *p = v;
      }
    }
  }
  objectEvent = ObjectEvent_Get(LOCALID_FIRST_STATUE);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_Get(LOCALID_SECOND_STATUE);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  FieldScheduler_AddCallback(Kalay_UpdateTunnelEmitters, TASK_PRIORITY_MAP);
  if (GameFlag_IsSet(FLAG_KALAY_TUNNEL_OPEN) != 0) {
    ObjectEvent_SetPosition(LOCALID_WAITING_MAN, PIXELS(426), PIXELS(730));
    objectEvent = ObjectEvent_Get(LOCALID_WAITING_MAN);
    {
      u16 shown = 0x5000;

      objectEvent->facing = shown;
    }
    MapGrid_CopyCellsTo(88, 48, 88, 45, 2, 3);
    MapGrid_CopyCellsTo(24, 49, 24, 48, 2, 1);
    MapGrid_CopyCellsTo(25, 42, 25, 47, 1, 1);
    MapGrid_CopyCellAttributes(22, 50, 2, 1, 24, 49);
  }
  if (GameFlag_IsSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) != 0) {
    ObjectEvent_SetPosition(LOCALID_FIRST_STATUE, PIXELS(232), PIXELS(732));
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
    switch (gGameState.warpDest) {
    case KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE:
    case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST:
      Map_QueueMusic(SE_170);
      break;
    }
  }
  if (GameFlag_IsSet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) != 0) {
    ObjectEvent_SetPosition(LOCALID_SECOND_STATUE, PIXELS(696), PIXELS(732));
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
    switch (gGameState.warpDest) {
    case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST:
    case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY:
      Map_QueueMusic(SE_170);
      break;
    }
  }
}

/// All eight tourists in Kalay start fidgeting.
static void Kalay_StartTouristsFidgeting(void) {
  Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST);
  Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 1);
  Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 2);
  Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 3);
  Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 5);
  Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 4);
  Kalay_StartTouristFidgeting(LOCALID_FIRST_TOURIST + 6);
  Kalay_StartTouristFidgeting(LOCALID_OURANOS);
}

/// A tourist starts fidgeting after a random pause.
static void Kalay_StartTouristFidgeting(s32 localId) {
  struct ObjectEvent *objectEvent;
  u32 random;

  objectEvent = ObjectEvent_GetById(localId);
  objectEvent->timer = localId;
  random = Engine_RandomNext();
  objectEvent->work = (random * 5) >> 12;
  ObjectEvent_SetScript(objectEvent, gKalay_TouristFidgetScript);
}

/// The guides speed up and start pacing in turn.
static void Kalay_StartGuidesPacing(void) {
  gKalay_DanceStep = 0;
  ObjectEvent_SetSpeed(LOCALID_HEAD_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(LOCALID_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
  FieldScheduler_AddCallback(Kalay_AlternateGuidesPacing, TASK_PRIORITY_MAP);
}

/// Each guide paces in turn, starting when the other finishes.
static void Kalay_AlternateGuidesPacing(void) {
  switch (gKalay_DanceStep) {
  case 0:
    Kalay_ClearScriptDone(ObjectEvent_GetById(LOCALID_SECOND_GUIDE));
    ObjectEvent_SetActionScript(LOCALID_SECOND_GUIDE, gKalay_GuideCountScript);
    gKalay_DanceStep++;
    break;
  case 1:
    if (ObjectEvent_GetById(LOCALID_SECOND_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(ObjectEvent_GetById(LOCALID_HEAD_GUIDE));
      ObjectEvent_SetActionScript(LOCALID_HEAD_GUIDE, gKalay_GuidePaceScript);
      gKalay_DanceStep++;
    }
    break;
  case 2:
    if (ObjectEvent_GetById(LOCALID_HEAD_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(ObjectEvent_GetById(LOCALID_HEAD_GUIDE));
      ObjectEvent_SetActionScript(LOCALID_HEAD_GUIDE, gKalay_GuideCountScript);
      gKalay_DanceStep++;
    }
    break;
  case 3:
    if (ObjectEvent_GetById(LOCALID_HEAD_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(ObjectEvent_GetById(LOCALID_SECOND_GUIDE));
      ObjectEvent_SetActionScript(LOCALID_SECOND_GUIDE, gKalay_GuidePaceScript);
      gKalay_DanceStep++;
    }
    break;
  case 4:
    if (ObjectEvent_GetById(LOCALID_SECOND_GUIDE)->timer != 0) {
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
  struct ObjectEvent *objectEvent;

  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(192), PIXELS(342));
  WaitFrames(1);
  FieldCamera_SetSpeed(PIXELS(1) / 5, PIXELS(1) / 40);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(252), 1);
  gFieldWork->transitionFrames = 40;
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 192, 278);
  ObjectEvent_SetSpeed(LOCALID_PALACE_GUARD, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_SECOND_PALACE_GUARD, PIXELS(1), PIXELS(1) / 2);
  objectEvent = ObjectEvent_Get(LOCALID_PALACE_GUARD);
  objectEvent->facing = FACING_SOUTHEAST + FACING_STEP;
  objectEvent = ObjectEvent_Get(LOCALID_SECOND_PALACE_GUARD);
  objectEvent->facing = FACING_SOUTH + FACING_STEP;
  WaitFrames(1);
  ObjectEvent_Get(LOCALID_PALACE_GUARD)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_SECOND_PALACE_GUARD)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkTo(LOCALID_PALACE_GUARD, 184, 232);
  ObjectEvent_WalkToAndWait(LOCALID_SECOND_PALACE_GUARD, 198, 232);
  Engine_ObjectEventSetAnimation(LOCALID_PALACE_GUARD, ANIM_STAND);
  FieldScript_WaitFrames(20);
  ObjectEvent_Get(LOCALID_PALACE_GUARD)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_SECOND_PALACE_GUARD)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_PALACE_GUARD, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(MsgKalayLordHammetsPalaceLordAway);
  Script_ShowMessageAndWait(LOCALID_PALACE_GUARD, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_SECOND_PALACE_GUARD,
                            FACING_SOUTH + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_PALACE_GUARD, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_SECOND_PALACE_GUARD, 0, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(IVAN, 212, 268);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_WalkToAndWait(IVAN, 202, 254);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(10);
  Script_ShowMessageAndWait(IVAN, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_SECOND_PALACE_GUARD,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_FaceDirection(LOCALID_PALACE_GUARD, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_PALACE_GUARD, FACING_WEST, 40);
  ObjectEvent_FaceDirection(LOCALID_PALACE_GUARD,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_PALACE_GUARD,
                            FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  Script_ShowMessageAndWait(IVAN, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PALACE_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_PALACE_GUARD, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  ObjectMotion_SetVariantCallback(LOCALID_PALACE_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_PALACE_GUARD, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_PALACE_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_SECOND_PALACE_GUARD, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 10);
  Script_AskYesNo(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PALACE_GUARD, 1);
  ObjectMotion_SetAnimationAndWait(LOCALID_PALACE_GUARD, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_PALACE_GUARD, 0, 10);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_PALACE_GUARD, 1);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_PALACE_GUARD, ANIM_NOD);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Script_ShowMessageAndWait(LOCALID_SECOND_PALACE_GUARD, 0, 10);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 60);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 10);
  Script_ShowMessageAndWait(IVAN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PALACE_GUARD, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_PALACE_GUARD, FACING_WEST, 40);
  ObjectEvent_FaceDirection(LOCALID_PALACE_GUARD,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_PALACE_GUARD,
                            FACING_SOUTHEAST + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PALACE_GUARD, 1);
  Script_OpenMessage(LOCALID_PALACE_GUARD, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
    Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  }
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_PALACE_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_SECOND_PALACE_GUARD, 0, 10);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(IVAN, 0, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_PALACE_GUARD, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Script_ShowMessageAndWait(LOCALID_PALACE_GUARD, 0, 10);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  Script_ShowMessageAndWait(IVAN, 0, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_PALACE_GUARD, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_SECOND_PALACE_GUARD,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PALACE_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_PALACE_GUARD, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_PALACE_GUARD, 2);
  ObjectEvent_FaceDirection(LOCALID_SECOND_PALACE_GUARD, FACING_WEST, 10);
  Script_ShowMessageAndWait(LOCALID_SECOND_PALACE_GUARD, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PALACE_GUARD, FACING_EAST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PALACE_GUARD, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_PALACE_GUARD,
                            FACING_SOUTHEAST + FACING_STEP, 10);
  Script_ShowMessage(LOCALID_PALACE_GUARD, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_PALACE_GUARD,
                            FACING_SOUTHEAST + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 10);
  Script_AskYesNo(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  ObjectEvent_SetSpeed(LOCALID_PALACE_GUARD, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectMotion_SetSpeedParameters(LOCALID_SECOND_PALACE_GUARD, PIXELS(4) / 5,
                                  PIXELS(2) / 5);
  objectEvent = ObjectEvent_Get(LOCALID_PALACE_GUARD);
  objectEvent->facing = 0;
  objectEvent = ObjectEvent_Get(LOCALID_SECOND_PALACE_GUARD);
  objectEvent->facing = FACING_WEST;
  ObjectEvent_Get(LOCALID_PALACE_GUARD)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_SECOND_PALACE_GUARD)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkTo(LOCALID_PALACE_GUARD, 168, 232);
  ObjectEvent_WalkToAndWait(LOCALID_SECOND_PALACE_GUARD, 212, 232);
  Engine_ObjectEventSetAnimation(LOCALID_PALACE_GUARD, ANIM_STAND);
  FieldScript_WaitFrames(20);
  ObjectEvent_Get(LOCALID_PALACE_GUARD)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_SECOND_PALACE_GUARD)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(IVAN, 192, 232);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Kalay_AnimatePalaceDoor();
  ObjectEvent_WalkToAndWait(IVAN, 192, 222);
  // Lead the party and guards through the opened palace door.
  ObjectEvent_SetPosition(IVAN, 0, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 192, 222);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PALACE_GUARD, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_PALACE_GUARD, ANIM_NOD);
  ObjectEvent_WalkTo(LOCALID_PALACE_GUARD, 184, 232);
  ObjectEvent_WalkToAndWait(LOCALID_SECOND_PALACE_GUARD, 198, 232);
  ObjectEvent_WalkTo(LOCALID_PALACE_GUARD, 188, 212);
  ObjectEvent_WalkToAndWait(LOCALID_SECOND_PALACE_GUARD, 194, 212);
  ObjectEvent_SetPosition(LOCALID_PALACE_GUARD, 0, 0);
  ObjectEvent_SetPosition(LOCALID_SECOND_PALACE_GUARD, 0, 0);
  gFieldWork->transitionFrames = 24;
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Engine_ScriptRequestWarp(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM);
  FieldScript_End();
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
  struct ObjectEvent *objectEvent;
  struct EventWork *work;
  const s32 *motionActions;
  s32 southFacing;
  const s32 *exitActions;

  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_WAGON, PIXELS(440), PIXELS(522));
  objectEvent = ObjectEvent_Get(LOCALID_WAGON);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(522), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(400), 1);
  Script_PlaySong(SE_RUMBLE);
  ObjectEvent_SetSpeed(LOCALID_WAGON, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_WAGON, 440, 400);
  FieldCamera_SetSpeed(PIXELS(3) / 4, PIXELS(3) / 32);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(300), 1);
  ObjectEvent_SetSpeed(LOCALID_WAGON, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_WAGON, 440, 300);
  Engine_ObjectEventSetAnimation(LOCALID_WAGON, 0);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetSpeed(LOCALID_BUNZA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_BUNZA, PIXELS(439), PIXELS(306));
  ObjectMotion_Launch(LOCALID_BUNZA, 4, 0);
  ObjectEvent_WalkToAndWait(LOCALID_BUNZA, 439, 312);
  ObjectEvent_WalkToAndWait(LOCALID_BUNZA, 416, 312);
  ObjectEvent_WalkToAndWait(LOCALID_BUNZA, 400, 256);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTHEAST + FACING_STEP, 40);
  FieldCamera_MoveTo(PIXELS(410), -1, PIXELS(280), 1);
  ObjectEvent_SetSpeed(LOCALID_HAMMET, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetPosition(LOCALID_HAMMET, PIXELS(439), PIXELS(306));
  ObjectMotion_Launch(LOCALID_HAMMET, 4, 0);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 439, 312);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 416, 312);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 388, 270);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_NORTH + FACING_STEP, 10);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH + FACING_STEP, 10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(439), PIXELS(306));
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 439, 312);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 416, 312);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 388, 300);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 40);
  // Seed the party idle phases for the arrival conversation.
  leaderPhase = (u8 *)ObjectEvent_Get(LOCALID_PARTY_LEADER);
  random = Random_Next();
  motionPhase = random * 5;
  motionActions = gKalay_PartyFidgetScript;
  leaderPhase += offsetof(struct ObjectEvent, work);
  *(u16 *)leaderPhase = motionPhase >> 12;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, motionActions);
  ObjectMotion_Launch(LOCALID_BUNZA, 2, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  FieldScript_SetMessageId(MsgKalayWeveArrivedHammet);
  Script_ShowMessageAndWait(LOCALID_BUNZA, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(GARET, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(MIA, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(GARET, 378, 310);
  ObjectEvent_WalkTo(IVAN, 400, 288);
  ObjectEvent_WalkToAndWait(MIA, 410, 308);
  Engine_ObjectEventSetAnimation(GARET, ANIM_STAND);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_STAND);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 40);
  leaderPhase = (u8 *)ObjectEvent_Get(GARET);
  random = Random_Next();
  leaderPhase += offsetof(struct ObjectEvent, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  leaderPhase = (u8 *)ObjectEvent_Get(IVAN);
  random = Random_Next();
  leaderPhase += offsetof(struct ObjectEvent, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  leaderPhase = (u8 *)ObjectEvent_Get(MIA);
  random = Random_Next();
  leaderPhase += offsetof(struct ObjectEvent, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  ObjectEvent_SetActionScript(GARET, motionActions);
  ObjectEvent_SetActionScript(IVAN, motionActions);
  ObjectEvent_SetActionScript(MIA, motionActions);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 2, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_HAMMET, 0, 10);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(GARET);
  ObjectMotion_EnableActionAndResetMotion(IVAN);
  ObjectMotion_EnableActionAndResetMotion(MIA);
  WaitFrames(1);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_HAMMET, 0, 10);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_HAMMET, 0, 10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(GARET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BUNZA, 2);
  Script_ShowMessageAndWait(LOCALID_BUNZA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 10);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_NORTH + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_HAMMET, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(IVAN, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 2, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ObjectEventSetAnimation(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_BUNZA, 0, 10);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTHEAST + FACING_STEP, 10);
  ObjectEvent_ShowEmote(LOCALID_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Script_ShowMessageAndWait(LOCALID_HAMMET, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  Script_ShowMessageAndWait(GARET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTH + FACING_STEP, 10);
  Engine_ObjectEventSetAnimation(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_BUNZA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_BUNZA, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_HAMMET, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ObjectEventSetAnimation(LOCALID_HAMMET, ANIM_NOD);
  Script_OpenMessage(LOCALID_HAMMET, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    gFieldWork->message += 3;
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_BUNZA, 2);
    Script_ShowMessageAndWait(LOCALID_BUNZA, 0, 40);
    Script_ShowMessageAndWait(LOCALID_BUNZA, 0, 10);
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
    ObjectEvent_FaceDirection(MIA, FACING_NORTH, 20);
    Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  }
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 10);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_TUNNEL_GUARD,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(200), 1);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 333, 222);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 284, 198);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_WEST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_TUNNEL_GUARD, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_TUNNEL_GUARD, ANIM_NOD);
  ObjectEvent_SetSpeed(LOCALID_TUNNEL_GUARD, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_TUNNEL_GUARD, 268, 198);
  ObjectEvent_FaceDirection(LOCALID_TUNNEL_GUARD, FACING_NORTH, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_TUNNEL_GUARD, 2);
  // The guard opens the passage and Hammet goes in.
  Script_PlaySong(SE_125);
  MapGrid_CopyCellsTo(71, 60, 76, 11, 2, 1);
  MapGrid_CopyCellAttributes(71, 60, 2, 1, 16, 11);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkToAndWait(LOCALID_TUNNEL_GUARD, 246, 198);
  ObjectEvent_FaceDirection(LOCALID_TUNNEL_GUARD, 0, 20);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(170), 1);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 270, 198);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 270, 174);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 224, 170);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 210, 158);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 246, 148);
  ObjectEvent_WalkToAndWait(LOCALID_HAMMET, 246, 142);
  ObjectEvent_SetPosition(LOCALID_HAMMET, 0, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  ObjectEvent_SetPosition(LOCALID_WAGON, PIXELS(440), PIXELS(340));
  objectEvent = ObjectEvent_Get(LOCALID_WAGON);
  southFacing = FACING_SOUTH;
  objectEvent->facing = southFacing;
  FieldCamera_MoveTo(PIXELS(380), -1, PIXELS(280), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(10);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  ObjectEvent_WalkToAndWait(LOCALID_BUNZA, 420, 282);
  ObjectEvent_WalkToAndWait(LOCALID_BUNZA, 420, 312);
  ObjectEvent_WalkToAndWait(LOCALID_BUNZA, 439, 312);
  ObjectEvent_WalkToAndWait(LOCALID_BUNZA, 439, 306);
  ObjectEvent_SetPosition(LOCALID_BUNZA, 0, 0);
  // The party climbs back into the wagon.
  exitActions = gKalay_PartyGatherScript;
  ObjectEvent_SetActionScript(GARET, exitActions);
  ObjectEvent_SetActionScript(IVAN, exitActions);
  ObjectEvent_RunActionScriptAndWait(MIA, exitActions);
  FieldCamera_MoveTo(PIXELS(410), -1, PIXELS(300), 1);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 416, 312);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 439, 312);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 439, 306);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  Script_PlaySong(SE_RUMBLE);
  ObjectEvent_SetSpeed(LOCALID_WAGON, PIXELS(4) / 5, PIXELS(2) / 5);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(420), 1);
  ObjectEvent_WalkToAndWait(LOCALID_WAGON, 440, 420);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(600), 1);
  ObjectEvent_SetSpeed(LOCALID_WAGON, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_WAGON, 440, 500);
  ObjectEvent_WalkTo(LOCALID_WAGON, 440, 600);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  work = gFieldWork;
  work->transitionFrames = 24;
  work->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Engine_ScriptRequestWarp(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH);
  FieldScript_End();
}

// Shared source, included in link order.
#include "common/object_findcell.h"

/// In the Kalay Tunnel (KalayTunnelTreasureRoom), walking into a statue pushes
/// it one cell when there is room.
static void Kalay_PushTunnelStatue(void) {
  struct FieldPosition destination;
  struct ObjectEvent *leader;
  struct ObjectEvent *statue;
  s32 direction;
  s32 probeStep;
  s32 moveStep;
  s32 leaderX;
  struct FieldPosition *target;
  s32 stoppedVelocity;
  s32 pushSpeed;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  direction = (leader->facing >> 12);
  leaderX = leader->x.part.pixel;
  probeStep = gKalay_TileStepByFacing[direction];
  statue = ObjectEvent_FindAtCell((leaderX + (probeStep >> 16)) >> 4,
                                  (leader->z.part.pixel + (s16)probeStep) >> 4);
  if (statue != NULL) {
    stoppedVelocity = 0;
    statue->layer = 2;
    target = &destination;
    moveStep = gKalay_TileStepByFacing[direction];
    SET_POSITION(target, statue->x.fixed + (moveStep & -0x10000),
                 statue->y.fixed, statue->z.fixed + (moveStep << 16));
    if (FieldObject_CheckMovementCollision(statue, target) <= 0) {
      ObjectDispatch_SelectAnimation(leader, ANIM_PUSH);
      pushSpeed = PIXELS(1) / 5;
      WaitFrames(15);
      Sound_PlaySong(SE_PUSH);
      statue->speed = pushSpeed;
      statue->acceleration = pushSpeed;
      Object_SetPosition(statue, target->x, target->y, target->z);
      leader->speed = pushSpeed;
      leader->acceleration = pushSpeed;
      Object_SetPosition(leader, target->x, target->y, target->z);
      Object_WaitForMove(statue);
      Map_PlayQueuedMusic();
      statue->x.fixed = target->x;
      statue->z.fixed = target->z;
      statue->velocityX = stoppedVelocity;
      statue->velocityZ = stoppedVelocity;
      ObjectDispatch_SelectAnimation(leader, ANIM_STAND);
      Kalay_UpdateTunnelPassages();
    }
  }
}

/// Each statue's passage opens once the statue is pushed far enough west. In
/// the Kalay Tunnel (KalayTunnelTreasureRoom) the passage opens step by step,
/// and the first one raises the puddle for Frost.
static void Kalay_UpdateTunnelPassages(void) {
  struct ObjectEvent *firstStatue;
  struct ObjectEvent *secondStatue;
  s32 firstPassageOpen;
  struct ObjectEvent *frostEffect;
  u32 i;
  s32 span;
  s32 height;
  s32 width;

  // Open each passage only after its corresponding statue reaches the trigger.
  firstStatue = ObjectEvent_GetById(LOCALID_FIRST_STATUE);
  secondStatue = ObjectEvent_GetById(LOCALID_SECOND_STATUE);
  firstPassageOpen = GameFlag_IsSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN);
  if (firstPassageOpen != 0) {
  } else if ((firstStatue->x.fixed >> 19) > 29) {
  } else {
    frostEffect = ObjectEvent_GetById(LOCALID_PUDDLE);
    FieldScript_Start();
    MapGrid_CopyCellAttributes(7, 44, 1, 1, firstPassageOpen, 1);
    i = 67;
    width = 1;
    height = 5;
    do {
      MapGrid_CopyCellsTo(i, 58, 78, 41, width, height);
      FieldScript_WaitFrames(4);
      if (i == 70) {
        GameFlag_Set(FLAG_TUNNEL_FIRST_PASSAGE_OPEN);
      }
      i++;
    } while (i <= 74);
    span = 2;
    MapGrid_CopyCellsTo(16, 109, 13, 109, 3, span);
    FieldScript_WaitFrames(40);
    frostEffect->scaleX = Q16_ONE / 10;
    frostEffect->scaleY = Q16_ONE / 10;
    ObjectEvent_SetPosition(LOCALID_PUDDLE, PIXELS(150), PIXELS(728));
    ObjectEvent_SetActionScript(LOCALID_PUDDLE, gKalay_FrostGrowScript);
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
    FieldScript_WaitFrames(40);
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
    FieldScript_WaitFrames(40);
    span = 10;
    MapGrid_CopyCellsTo(7, 11, 7, 42, span, 8);
    MapGrid_CopyCellsTo(71, 12, 71, 43, span, 13);
    span = 44;
    MapGrid_CopyCellAttributes(6, 13, 12, 12, 6, span);
    FieldScript_WaitFrames(40);
    Kalay_StartTrackingLeader();
    MapGrid_CopyCellAttributes(0, 1, 1, 1, 7, span);
    FieldScript_End();
  }
  if (GameFlag_IsSet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) != 0) {
  } else if ((secondStatue->x.fixed >> 19) > 87) {
  } else {
    FieldScript_Start();
    i = 67;
    width = 1;
    height = 5;
    do {
      MapGrid_CopyCellsTo(i, 58, 107, 41, width, height);
      FieldScript_WaitFrames(4);
      if (i == 70) {
        GameFlag_Set(FLAG_TUNNEL_SECOND_PASSAGE_OPEN);
      }
      i++;
    } while (i <= 74);
    height = 2;
    MapGrid_CopyCellsTo(45, 109, 42, 109, 3, height);
    FieldScript_WaitFrames(40);
    span = 1;
    MapGrid_CopyCellsTo(67, 64, 102, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 103, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 104, 44, span, height);
    MapGrid_CopyCellsTo(67, 66, 105, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 106, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 107, 44, span, height);
    MapGrid_CopyCellsTo(67, 64, 108, 44, span, height);
    MapGrid_CopyCellsTo(67, 66, 109, 44, span, height);
    FieldScript_WaitFrames(40);
    MapGrid_CopyCellsTo(68, 64, 102, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 103, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 104, 44, span, height);
    MapGrid_CopyCellsTo(68, 66, 105, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 106, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 107, 44, span, height);
    MapGrid_CopyCellsTo(68, 64, 108, 44, span, height);
    MapGrid_CopyCellsTo(68, 66, 109, 44, span, height);
    FieldScript_WaitFrames(40);
    span = 8;
    MapGrid_CopyCellsTo(38, 14, 38, 44, span, 4);
    MapGrid_CopyCellsTo(102, 14, 102, 44, span, 12);
    MapGrid_CopyCellAttributes(37, 13, 10, 12, 37, 43);
    FieldScript_WaitFrames(40);
    Kalay_StartTrackingLeader();
    FieldScript_End();
  }
}

// UB: the native void epilogue returns through r0, leaving the nonzero return
// address as SC_CALL's status. Repeat once per frame until the object is
// destroyed, with an explicit status in the modern build.
// Advance a particle, damp its velocity, and expire its lifetime.
#ifdef UBFIX
static s32 Kalay_UpdateTunnelParticle(struct ObjectScriptContext *interpreter) {
#else
static void
Kalay_UpdateTunnelParticle(struct ObjectScriptContext *interpreter) {
#endif
  struct ObjectEvent *particle = (struct ObjectEvent *)interpreter;
  s16 *lifetime;
  s32 remainingFrames;
  s32 velocityX;
  s32 velocityY;

  lifetime = &particle->timer;
  remainingFrames = *lifetime;
  if (remainingFrames == 0) {
    ObjectDispatch_Release((struct DispatchObject *)particle);
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
  struct ObjectEvent *firstEmitter;
  struct ObjectEvent *secondEmitter;

  firstEmitter = ObjectEvent_Get(LOCALID_FIRST_STATUE);
  secondEmitter = ObjectEvent_Get(LOCALID_SECOND_STATUE);
  if ((u32)(firstEmitter->x.part.pixel - 381) <= 12 &&
      firstEmitter->z.part.pixel > 777) {
    firstEmitter->sprite->image.priority =
        ObjectEvent_Get(LOCALID_PARTY_LEADER)->sprite->image.priority;
  } else if (!GameFlag_IsSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) &&
             firstEmitter->x.part.pixel <= 245 && (gFrameCount & 1) == 0) {
    if (!GameFlag_IsSet(FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED)) {
      Map_QueueMusic(-1);
      Script_PlaySong(SE_230);
      GameFlag_Set(FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED);
    }
    Kalay_SpawnTunnelParticle(firstEmitter->x.fixed, firstEmitter->y.fixed,
                              firstEmitter->z.fixed);
  }
  if (!GameFlag_IsSet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) &&
      secondEmitter->x.part.pixel <= 709 && (gFrameCount & 1) == 0) {
    if (!GameFlag_IsSet(FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED)) {
      Map_QueueMusic(-1);
      Script_PlaySong(SE_230);
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
  struct ObjectEvent *particle;
  s32 random;
  s32 attributeMask;
  u8 *spriteBytes;

  depth = z;
  random = Engine_RandomNext();
  particle = ObjectEvent_Create(
      SPRITE_DUST_PUFF, (x - PIXELS(8)),
      (((((u32)(random << 3) >> 16) << 16) + height) + PIXELS(16)), depth);
  if (particle != 0) {
    ((u8 *)particle)[offsetof(struct ObjectEvent, motionFlags)] =
        (attributeMask = 0);
    spriteBytes = (u8 *)particle->sprite;
    attributeMask -= 13;
    spriteBytes[9] = ((spriteBytes[9] & attributeMask) | 8);
    ObjectGroup_SetChildValue(particle, SPRITE_COLOR_BLUE_TINT);
    ObjectEvent_SetSpriteFlags(particle, 0);
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
    ObjectDispatch_SelectAnimation(particle, 1);
    ObjectEvent_SetScript(particle, gKalay_TunnelParticleScript);
  }
}

/// The Nut in Kalay circles its spot and rocks as it goes.
static void Kalay_UpdateNutOrbit(void) {
  OrbitingMapObject *effect =
      (OrbitingMapObject *)ObjectEvent_GetById(LOCALID_TOWN_NUT);
  struct Sprite *sprite = (struct Sprite *)effect->sprite;
  s32 offset = Trig_Sin(effect->orbitAngle) * 2;
  s32 first;

  if (offset > 0) {
    offset = -offset;
  }
  effect->x = effect->orbitCenterX + Trig_Cos(effect->orbitAngle) * 2;
  effect->y = effect->orbitCenterY + offset;
  sprite->rotation = Trig_Cos(effect->orbitAngle + 0x8000) >> 3;
  first = Engine_RandomNext();
  effect->orbitAngle += ((u32)(first << 9) >> 16) +
                        ((u32)(Engine_RandomNext() << 9) >> 16) + 0x400;
}

/// Frost on the puddle in the Kalay Tunnel sinks it into the floor. The puddle
/// is in KalayTunnelTreasureRoom; sinking it clears its cells, and it stays
/// sunk until the map is entered again.
static void Kalay_FreezePuddle(void) {
  struct ObjectEvent *effect = ObjectEvent_GetById(LOCALID_PUDDLE);

  MapGrid_CopyCellAttributes(0, 0, 1, 1, 9, 14);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 9, 45);
  if (effect != 0) {
    ObjectEvent_SetSpriteFlags(effect, 0);
    effect->y.fixed -= PIXELS(32);
    effect->priorityFlags = 2;
  }
  Engine_GameFlagSet(FLAG_TUNNEL_PUDDLE_FROZEN);
}

// Shared source, included in link order.
#include "common/object_orbit.h"

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
    SC_SET(SW_TARGET_X, OBJ_EVENT_NO_TARGET),
    SC_SET(SW_TARGET_Y, OBJ_EVENT_NO_TARGET),
    SC_SET(SW_TARGET_Z, OBJ_EVENT_NO_TARGET),
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
    SC_SET(SW_TARGET_X, OBJ_EVENT_NO_TARGET),
    SC_SET(SW_TARGET_Y, OBJ_EVENT_NO_TARGET),
    SC_SET(SW_TARGET_Z, OBJ_EVENT_NO_TARGET),
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
    SC_CALL(Map_LookAboutRandomly),
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

struct MapWarpDest gKalay_NoWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gKalay_TownWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 224, 0, 280, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

struct MapWarpDest gKalay_HammetPalaceWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {HAMMET_PALACE_GROUNDS_FROM_KALAY, CONDITION_ALWAYS, 192, 0, 280,
     FACING_NORTH, 0, 0, 0, 400, 352, 0},
    {HAMMET_PALACE_GROUNDS_FROM_HAMMET_PALACE, CONDITION_ALWAYS, 192, 0, 232,
     FACING_SOUTH, 0, 0, 0, 400, 352, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gKalay_TunnelEntranceWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_TUNNEL_ENTRANCE_FROM_KALAY_TUNNEL_CAVE, CONDITION_ALWAYS, 248, 0,
     152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 424, 0, 520,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 440, 0, 540,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gKalay_TunnelCaveWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_PASSAGE, CONDITION_ALWAYS, 296, 0, 104,
     FACING_SOUTH, 0, 100, 50, 440, 445, 0},
    {KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_ENTRANCE, CONDITION_ALWAYS, 280, 0,
     392, FACING_NORTH, 0, 100, 50, 440, 445, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gKalay_TunnelPassageWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

struct MapWarpDest gKalay_TunnelWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE, CONDITION_ALWAYS, 408, 0,
     632, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
    {KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST,
     CONDITION_ALWAYS, 296, 0, 488, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
    {KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST,
     CONDITION_ALWAYS, 744, 0, 488, FACING_SOUTH, 0, 595, 400, 930, 880, 0},
    {KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY, CONDITION_ALWAYS, 856, 0, 856,
     FACING_NORTH, 0, 595, 400, 930, 880, 0},
    {MAP_TABLE_END},
};

u32 gKalay_Warps[] = {
    MAP_WARPS(Kalay),
    MAP_WARP(KALAY_TO_HOUSE_NORTHWEST, KalayInteriors,
             KALAY_INTERIORS_HOUSE_NORTHWEST),
    MAP_WARP(KALAY_TO_HOUSE_NORTHEAST_UPPER, KalayInteriors,
             KALAY_INTERIORS_HOUSE_NORTHEAST_UPPER),
    MAP_WARP(KALAY_TO_HOUSE_NORTHEAST_LOWER, KalayInteriors,
             KALAY_INTERIORS_HOUSE_NORTHEAST_LOWER),
    MAP_WARP(KALAY_TO_HOUSE_SOUTHEAST, KalayInteriors,
             KALAY_INTERIORS_HOUSE_SOUTHEAST),
    MAP_WARP(KALAY_TO_WEAPON_SHOP, KalayInteriors, KALAY_INTERIORS_WEAPON_SHOP),
    MAP_WARP(KALAY_TO_ARMOR_SHOP, KalayInteriors, KALAY_INTERIORS_ARMOR_SHOP),
    MAP_WARP(KALAY_TO_INN, KalayInteriors, KALAY_INTERIORS_INN),
    MAP_WARP(KALAY_TO_KALAY_SANCTUM, KalaySanctum, KALAY_SANCTUM_FROM_KALAY),
    MAP_WARP(KALAY_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KALAY_EAST),
    MAP_WARP(KALAY_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_KALAY_WEST),
    MAP_WARP(KALAY_TO_HAMMET_PALACE_GROUNDS, HammetPalaceGrounds,
             HAMMET_PALACE_GROUNDS_FROM_KALAY),
    MAP_WARP(KALAY_TO_WEAPON_SHOP_COUNTER, KalayInteriors,
             KALAY_INTERIORS_WEAPON_SHOP_COUNTER),
    MAP_WARP(KALAY_TO_ARMOR_SHOP_COUNTER, KalayInteriors,
             KALAY_INTERIORS_ARMOR_SHOP_COUNTER),
    MAP_WARP(KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM, KalayTunnelTreasureRoom,
             KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY),
    MAP_WARPS(HammetPalaceGrounds),
    MAP_WARP(HAMMET_PALACE_GROUNDS_TO_KALAY, Kalay,
             KALAY_FROM_HAMMET_PALACE_GROUNDS),
    MAP_WARP(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE, HammetPalace,
             HAMMET_PALACE_FROM_HAMMET_PALACE_GROUNDS),
    MAP_WARP(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM, HammetPalace,
             HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS),
    MAP_WARPS(KalayTunnelEntrance),
    MAP_WARP(KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_CAVE, KalayTunnelCave,
             KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_ENTRANCE),
    MAP_WARP(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_NORTH, WorldMap,
             WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_NORTH),
    MAP_WARP(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH, WorldMap,
             WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH),
    MAP_WARPS(KalayTunnelCave),
    MAP_WARP(KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_PASSAGE, KalayTunnelPassage,
             KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_CAVE),
    MAP_WARP(KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_ENTRANCE, KalayTunnelEntrance,
             KALAY_TUNNEL_ENTRANCE_FROM_KALAY_TUNNEL_CAVE),
    MAP_WARPS(KalayTunnelPassage),
    MAP_WARP(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_WEST,
             KalayTunnelTreasureRoom,
             KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST),
    MAP_WARP(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_EAST,
             KalayTunnelTreasureRoom,
             KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST),
    MAP_WARP(KALAY_TUNNEL_PASSAGE_LONG_PATH_TO_NORTHEAST_ROOM,
             KalayTunnelPassage,
             KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_FROM_LONG_PATH),
    MAP_WARP(KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_TO_LONG_PATH,
             KalayTunnelPassage,
             KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM),
    MAP_WARP(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_CAVE, KalayTunnelCave,
             KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_PASSAGE),
    MAP_WARPS(KalayTunnelTreasureRoom),
    MAP_WARP(KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE, HammetPalace,
             HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM),
    MAP_WARP(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_WEST,
             KalayTunnelPassage,
             KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_WEST),
    MAP_WARP(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_EAST,
             KalayTunnelPassage,
             KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_EAST),
    MAP_WARP(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY, Kalay,
             KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gKalay_NoObjectEventTemplates[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gKalay_TownObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalay_TourGroupObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalay_HammetPalaceObjectEventTemplates[] = {
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(212), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalay_TunnelEntranceObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gKalay_TunnelObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct MapEvent gKalay_NoEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gKalay_TownEvents[] = {
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, KALAY_TO_HAMMET_PALACE_GROUNDS },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, KALAY_TO_WEAPON_SHOP_COUNTER },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, KALAY_TO_ARMOR_SHOP_COUNTER },
    { MAP_EVENT_WARP, 14, CONDITION_ALWAYS, KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_WatchTourGroupDeparture) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_WatchTourGroupDeparture) },
    { MAP_EVENT_TALK, LOCALID_WELCOMING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgKalayWelcomeInPeace) },
    { MAP_EVENT_TALK, LOCALID_SOLDIER_PROTECTING_THE_PEACE, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersProtectPeace) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_ON_HAMMET_THE_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayHammetGenerousMerchant) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_HAMMETS_CARAVANS, CONDITION_ALWAYS, MSGNO(MsgKalayHammetOnJourney) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_THE_REBUILT_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgKalayNorthernBridgeRebuilt) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_LAMAKAN_DESERT, CONDITION_ALWAYS, MSGNO(MsgKalayLamakanTrouble) },
    { MAP_EVENT_TALK, LOCALID_LATE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayElderTownNews) },
    { MAP_EVENT_TALK, LOCALID_WOULD_BE_TOURIST, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInnFestivalTourists) },
    { MAP_EVENT_TALK, LOCALID_WOULD_BE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayFormerInnFestivalTourists) },
    { MAP_EVENT_TALK, LOCALID_BOY_DREAMING_OF_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayChildWantsFestival) },
    { MAP_EVENT_TALK, LOCALID_GIRL_ON_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgKalayTraderTownNews) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_ON_HAMMETS_KNACK, CONDITION_ALWAYS, MSGNO(MsgKalayHammetBusinessSkill) },
    { MAP_EVENT_TALK, LOCALID_MAN_WHO_LEFT_TOLBI, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkAboutLeavingHome) },
    { MAP_EVENT_TALK, LOCALID_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCapturedRumor) },
    { MAP_EVENT_TALK, LOCALID_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayHammetWentNorth) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToItemMerchant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WELCOMING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorHonestEyes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SOLDIER_PROTECTING_THE_PEACE, CONDITION_ALWAYS, MSGNO(MsgKalayGuardsJudgeVisitors) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_ON_HAMMET_THE_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayHammetPeaceAndLiberty) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_HAMMETS_CARAVANS, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCanBeRash) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_THE_REBUILT_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersWentToVault) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_LAMAKAN_DESERT, CONDITION_ALWAYS, MSGNO(MsgKalayStrangeEvents) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LATE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayElderThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOULD_BE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayWishSavedForFestival) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_DREAMING_OF_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayChildFestivalDream) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_ON_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgKalayTraderThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_ON_HAMMETS_KNACK, CONDITION_ALWAYS, MSGNO(MsgKalayHammetValuesTrust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_WHO_LEFT_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTrustAgainstForce) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayLunpaNorthOfVault) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayRumorsHideTruth) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayWishBetterWeapons) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayWeaponsHaveSoul) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayPalaceWaterSupply) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_TOWN_STATUE, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_TOWN_NUT, FLAG_KALAY_TOWN_NUT_CAUGHT, EVENT_SCRIPT(Kalay_CatchTownNut) },
    { BG_EVENT(BG_EVENT_JAR), 100, FLAG_KALAY_SLEEP_BOMB_JAR, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_KALAY_VIAL_JAR, FIND_ITEM(ITEM_VIAL) },
    { BG_EVENT(BG_EVENT_SILENT), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_SearchWell) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalay_HammetPalaceEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, HAMMET_PALACE_GROUNDS_TO_KALAY },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_EnterPalace) },
    { MAP_EVENT_TALK, LOCALID_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersHandleAffairs) },
    { MAP_EVENT_TALK, LOCALID_SECOND_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersRescueHammet) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayPrayForHammetSafety) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorsWillSaveHammet) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalay_TunnelEntranceEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_CAVE },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_TALK, LOCALID_TUNNEL_GUARD, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_AskAboutCaveAccess) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TUNNEL_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayPassageClosedByOrders) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_TUNNEL_ENTRANCE_NUT, FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT, EVENT_SCRIPT(Kalay_CatchTunnelEntranceNut) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalay_TunnelCaveEvents[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_PASSAGE },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_ENTRANCE },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_KALAY_TUNNEL_APPLE_CHEST, FIND_ITEM(ITEM_APPLE) },
    { MAP_EVENTS_END },
};

struct MapEvent gKalay_TunnelPassageEvents[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_EAST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_LONG_PATH_TO_NORTHEAST_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_TO_LONG_PATH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_CAVE },
    { MAP_EVENTS_END },
};

struct MapEvent gKalay_TunnelEvents[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_EAST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY },
    { MAP_EVENT_TALK, LOCALID_WAITING_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkAboutLayana) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WAITING_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTreasureBeyondDoors) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_STATUE, FLAG_TUNNEL_FIRST_PASSAGE_OPEN, EVENT_SCRIPT(Kalay_UpdateTunnelPassages) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SECOND_STATUE, FLAG_TUNNEL_SECOND_PASSAGE_OPEN, EVENT_SCRIPT(Kalay_UpdateTunnelPassages) },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelStatue) },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelStatue) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelStatue) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelStatue) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelStatue) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_PUDDLE, FLAG_TUNNEL_PUDDLE_FROZEN, EVENT_SCRIPT(Kalay_FreezePuddle) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_StopTrackingLeader) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_StartTrackingLeader) },
    { MAP_EVENT_COORD, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_DisableLeaderAutomaticPriority) },
    { MAP_EVENT_COORD, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_EnableLeaderAutomaticPriority) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_KALAY_TUNNEL_SPIRIT_GLOVES_CHEST, FIND_ITEM(ITEM_SPIRIT_GLOVES) },
    { MAP_EVENT_TALK, LOCALID_SCORCH, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_ScorchJoins) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_KALAY_TUNNEL_SLEEP_BOMB_CHEST, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_KALAY_TUNNEL_LUCKY_MEDAL_CHEST, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_CHEST), 104, FLAG_KALAY_TUNNEL_COINS_CHEST, FIND_COINS(200) },
    { BG_EVENT(BG_EVENT_CHEST), 105, FLAG_KALAY_TUNNEL_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
    { MAP_EVENTS_END },
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
