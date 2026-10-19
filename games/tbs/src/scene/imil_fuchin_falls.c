/// @file
/// The Djinni's ice room by Imil and Fuchin Falls, in seven maps: the ice room,
/// then Fuchin Falls' four platform rooms, the room of the Ki inscription and
/// the dragon statue's room.
///
/// Flags 0x301 to 0x315 belong to the cave and hold the platforms' stops: 0x301
/// to 0x303 the first room's, 0x304 to 0x307 the second's, 0x308 to 0x312 the
/// third's and 0x313 to 0x315 the fourth's.

#include "script.h"
#include "game_state.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/imil_fuchin_falls.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "battle.h"
#include "field_service.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "imil_fuchin_falls.h"
#include "types.h"
#include "glyph.h"
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "call.h"
#include "battle_party.h"
#include "field_effect.h"
#include "scene.h"
#include "field_runtime.h"
#include "display_transition.h"
#include "fx_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "object_effect.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

enum ImilFuchinFallsActor {
  ACTOR_IMIL_DJINNI = 8, ///< in the ice room
  /// The platforms, in each platform room
  ACTOR_FIRST_PLATFORM = 8,
  ACTOR_SECOND_PLATFORM = 9,
  ACTOR_THIRD_PLATFORM = 10,
  ACTOR_FOURTH_PLATFORM = 11,
  ACTOR_FUCHIN_DJINNI = 11 ///< in the second platform room
};

// Forward declarations.
static void Fuchin_RebuildFirstRoomPlatforms(void);
static void Fuchin_RebuildSecondRoomPlatforms(void);
static void Fuchin_RebuildThirdRoomPlatforms(void);
static void Fuchin_RebuildFourthRoomPlatforms(void);
static void Imil_WalkTowardDjinni(void);
static void ImilFuchinFalls_PrepareRoom(void);
static void ImilFuchinFalls_RebuildPlatformRoom(void);
static void ImilFuchinFalls_HopBy(s32 dx, s32 dz);
static void ImilFuchinFalls_PlaceDragonsEye(void);
static void Imil_SlideActorAcrossIce(void);

s32 gImilFuchinFalls_TrackLeader;

struct CaveEffectScripts {
  const s32 *script[3];
};

/// EffectOptions cut after its scales: the caller's frame holds these 24 bytes
/// only
struct CaveDustOptions {
  s32 priority;
  s32 palette;
  s32 startScaleX;
  s32 startScaleY;
  s32 targetScaleX;
  s32 targetScaleY;
};

STATIC_ASSERT(offsetof(struct CaveDustOptions, priority) ==
              offsetof(struct EffectOptions, priority));
STATIC_ASSERT(offsetof(struct CaveDustOptions, palette) ==
              offsetof(struct EffectOptions, palette));
STATIC_ASSERT(offsetof(struct CaveDustOptions, startScaleX) ==
              offsetof(struct EffectOptions, startScaleX));
STATIC_ASSERT(offsetof(struct CaveDustOptions, startScaleY) ==
              offsetof(struct EffectOptions, startScaleY));
STATIC_ASSERT(offsetof(struct CaveDustOptions, targetScaleX) ==
              offsetof(struct EffectOptions, targetScaleX));
STATIC_ASSERT(offsetof(struct CaveDustOptions, targetScaleY) ==
              offsetof(struct EffectOptions, targetScaleY));

/// Talking to the Djinni in the ice room adds it to the party.
static void Imil_RecruitDjinni(void) {
  Djinn_JoinParty(ACTOR_IMIL_DJINNI, ELEMENT_MARS, 1);
}

/// Talking to the Djinni in the second platform room starts its battle.
static void Fuchin_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(11, DJINNI_ZEPHYR);
}

/// Selects the entrances for the current Imil or Fuchin room.
const struct SceneEntrance *ImilFuchinFalls_GetEntrances(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_ImilFuchinFalls1) {
    return gImilFuchinFalls_DjinniRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls2) {
    return gImilFuchinFalls_FirstRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls3) {
    return gImilFuchinFalls_SecondRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls4) {
    return gImilFuchinFalls_ThirdRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls5) {
    return gImilFuchinFalls_FourthRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls6) {
    return gImilFuchinFalls_KiRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls7) {
    return gImilFuchinFalls_DragonRoomEntrances;
  }
  return gImilFuchinFalls_NoEntrances;
}

/// The cave has no scene regions.
const struct SceneRegion *ImilFuchinFalls_GetRegions(void) { return NULL; }

/// Returns the exit connections shared by these rooms.
u32 *ImilFuchinFalls_GetExits(void) { return gImilFuchinFalls_Exits; }

/// Selects actor placements for the current room.
const struct ScenePlacement *ImilFuchinFalls_GetPlacements(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_ImilFuchinFalls1) {
    return gImilFuchinFalls_DjinniRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls2) {
    return gImilFuchinFalls_FirstRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls3) {
    return gImilFuchinFalls_SecondRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls4) {
    return gImilFuchinFalls_ThirdRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls5) {
    return gImilFuchinFalls_FourthRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls7) {
    return gImilFuchinFalls_DragonRoomPlacements;
  }
  return gImilFuchinFalls_NoPlacements;
}

// Shared source is included here in ROM order.
#include "common/object_pairmove.h"

/// Lets the platform settle before rebuilding its room's collision cells.
static inline void Fuchin_FinishPlatformMove(void (*rebuildPlatforms)(void)) {
  Engine_AudioPlayCue(SE_289);
  Engine_TaskWait(2);
  rebuildPlatforms();
}

/// Moves the first platform east in the first puzzle room.
static void Fuchin_FirstRoom_MoveFirstPlatformEast(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 112, 0);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 112, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x301);
  Engine_TaskWait(2);
  Fuchin_RebuildFirstRoomPlatforms();
}

/// Moves the first platform west in the first puzzle room.
static void Fuchin_FirstRoom_MoveFirstPlatformWest(void) {
  s32 offset = 112;

  offset = -offset;
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, offset, 0);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, offset, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x301);
  Engine_TaskWait(2);
  Fuchin_RebuildFirstRoomPlatforms();
}

/// Moves the second platform south in the first puzzle room.
static void Fuchin_FirstRoom_MoveSecondPlatformSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_PLATFORM, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x302);
  Engine_TaskWait(2);
  Fuchin_RebuildFirstRoomPlatforms();
}

/// Moves the second platform north in the first puzzle room.
static void Fuchin_FirstRoom_MoveSecondPlatformNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_PLATFORM, 0, -64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x302);
  Engine_TaskWait(2);
  Fuchin_RebuildFirstRoomPlatforms();
}

/// Moves the third platform south in the first puzzle room.
static void Fuchin_FirstRoom_MoveThirdPlatformSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x303);
  Engine_TaskWait(2);
  Fuchin_RebuildFirstRoomPlatforms();
}

/// Moves the third platform north in the first puzzle room.
static void Fuchin_FirstRoom_MoveThirdPlatformNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, -64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x303);
  Engine_TaskWait(2);
  Fuchin_RebuildFirstRoomPlatforms();
}

/// Moves the first platform to east end in the second puzzle room.
static void Fuchin_SecondRoom_MoveFirstPlatformToEastEnd(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 144, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x304);
  Engine_TaskWait(2);
  Fuchin_RebuildSecondRoomPlatforms();
}

/// Moves the first platform to west end in the second puzzle room.
static void Fuchin_SecondRoom_MoveFirstPlatformToWestEnd(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, -144, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x304);
  Engine_GameFlagSet(0x305);
  Engine_TaskWait(2);
  Fuchin_RebuildSecondRoomPlatforms();
}

/// Nudges the first platform west in the second puzzle room.
static void Fuchin_SecondRoom_NudgeFirstPlatformWest(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, -14, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x305);
  Engine_TaskWait(2);
  Fuchin_RebuildSecondRoomPlatforms();
}

/// Moves the first platform east in the second puzzle room.
static void Fuchin_SecondRoom_MoveFirstPlatformEast(void) {
  Engine_AudioPlayCue(SE_241);

  if (Engine_GameFlagIsSet(0x306) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 16, 0);
    Engine_GameFlagClear(0x305);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 144, 0);

    Engine_GameFlagSet(0x304);
  }

  Fuchin_FinishPlatformMove(Fuchin_RebuildSecondRoomPlatforms);
}

/// Moves the second platform south in the second puzzle room.
static void Fuchin_SecondRoom_MoveSecondPlatformSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_PLATFORM, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x306);
  Engine_TaskWait(2);
  Fuchin_RebuildSecondRoomPlatforms();
}

/// Moves the second platform north in the second puzzle room.
static void Fuchin_SecondRoom_MoveSecondPlatformNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_PLATFORM, 0, -64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x306);
  Engine_TaskWait(2);
  Fuchin_RebuildSecondRoomPlatforms();
}

/// Moves the third platform south in the second puzzle room.
static void Fuchin_SecondRoom_MoveThirdPlatformSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 144);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 128);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x307);
  Engine_TaskWait(2);
  Fuchin_RebuildSecondRoomPlatforms();
}

/// Moves the third platform north in the second puzzle room.
static void Fuchin_SecondRoom_MoveThirdPlatformNorth(void) {
  s32 depth = -96;

  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, depth);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, depth);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, -80);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x307);
  Engine_TaskWait(2);
  Fuchin_RebuildSecondRoomPlatforms();
}

/// Moves the first platform west in the third puzzle room.
static void Fuchin_ThirdRoom_MoveFirstPlatformWest(void) {
  Engine_AudioPlayCue(SE_241);

  if (Engine_GameFlagIsSet(0x310) != 0 || Engine_GameFlagIsSet(0x30D) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, -48, 0);
    Engine_GameFlagClear(0x308);
    Engine_GameFlagSet(0x309);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, -96, 0);
    Engine_GameFlagSet(0x308);
    Engine_GameFlagClear(0x309);
  }

  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Moves the first platform east from the middle in the third puzzle room.
static void Fuchin_ThirdRoom_MoveFirstPlatformEastFromMiddle(void) {
  Engine_AudioPlayCue(SE_241);
  Engine_GameFlagClear(0x308);
  Engine_GameFlagClear(0x309);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 48, 0);
  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Moves the first platform west if clear in the third puzzle room.
static void Fuchin_ThirdRoom_MoveFirstPlatformWestIfClear(void) {
  if (Engine_GameFlagIsSet(0x310) != 0) {
    return;
  }
  if (Engine_GameFlagIsSet(0x30D) != 0) {
    return;
  }

  Engine_AudioPlayCue(SE_241);

  Engine_GameFlagSet(0x308);
  Engine_GameFlagClear(0x309);

  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, -48, 0);
  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Moves the first platform east from the west end in the third puzzle room.
static void Fuchin_ThirdRoom_MoveFirstPlatformEastFromWestEnd(void) {
  Engine_AudioPlayCue(SE_241);
  Engine_GameFlagClear(0x308);
  Engine_GameFlagClear(0x309);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 96, 0);
  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Moves the second platform west in the third puzzle room.
static void Fuchin_ThirdRoom_MoveSecondPlatformWest(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_PLATFORM, -32, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x30A);
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the second platform east in the third puzzle room.
static void Fuchin_ThirdRoom_MoveSecondPlatformEast(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_PLATFORM, 32, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x30A);
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the third platform north from the south end in the third puzzle room.
static void Fuchin_ThirdRoom_MoveThirdPlatformNorthFromSouthEnd(void) {
  Engine_AudioPlayCue(SE_241);
  if (Engine_GameFlagIsSet(0x308) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, -64);
    Engine_GameFlagClear(0x30b);
    Engine_GameFlagSet(0x30c);
    Engine_GameFlagClear(0x30d);
    Engine_GameFlagClear(0x30e);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, -128);
    Engine_GameFlagSet(0x30b);
    Engine_GameFlagClear(0x30c);
    Engine_GameFlagClear(0x30d);
    Engine_GameFlagClear(0x30e);
  }
  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Moves the third platform south from the north end in the third puzzle room.
static void Fuchin_ThirdRoom_MoveThirdPlatformSouthFromNorthEnd(void) {
  Engine_AudioPlayCue(SE_241);

  if (Engine_GameFlagIsSet(0x308) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 16);
    Engine_GameFlagClear(0x30b);
    Engine_GameFlagSet(0x30c);
    Engine_GameFlagClear(0x30d);
    Engine_GameFlagClear(0x30e);
  } else if (Engine_GameFlagIsSet(0x310) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 16);
    Engine_GameFlagClear(0x30b);
    Engine_GameFlagSet(0x30c);
    Engine_GameFlagClear(0x30d);
    Engine_GameFlagClear(0x30e);
  } else if (Engine_GameFlagIsSet(0x311) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 64);
    Engine_GameFlagClear(0x30b);
    Engine_GameFlagClear(0x30c);
    Engine_GameFlagSet(0x30d);
    Engine_GameFlagClear(0x30e);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 128);
    Engine_GameFlagClear(0x30b);
    Engine_GameFlagClear(0x30c);
    Engine_GameFlagClear(0x30d);
    Engine_GameFlagClear(0x30e);
  }
  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Nudges the third platform north in the third puzzle room.
static void Fuchin_ThirdRoom_NudgeThirdPlatformNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, -16);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x30b);
  Engine_GameFlagClear(0x30c);
  Engine_GameFlagClear(0x30d);
  Engine_GameFlagClear(0x30e);
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the third platform south from the upper stop in the third puzzle room.
static void Fuchin_ThirdRoom_MoveThirdPlatformSouthFromUpperStop(void) {
  Engine_AudioPlayCue(SE_241);
  if (Engine_GameFlagIsSet(0x311) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 48);
    Engine_GameFlagClear(0x30b);

    Engine_GameFlagClear(0x30c);
    Engine_GameFlagSet(0x30d);
    Engine_GameFlagClear(0x30e);
  } else if (Engine_GameFlagIsSet(0x310) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 32);
    Engine_GameFlagClear(0x30b);
    Engine_GameFlagSet(0x30c);
    Engine_GameFlagClear(0x30d);
    Engine_GameFlagClear(0x30e);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 112);
    Engine_GameFlagClear(0x30b);
    Engine_GameFlagClear(0x30c);
    Engine_GameFlagClear(0x30d);
    Engine_GameFlagClear(0x30e);
  }
  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Moves the third platform north from the middle in the third puzzle room.
static void Fuchin_ThirdRoom_MoveThirdPlatformNorthFromMiddle(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, -64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x30B);
  Engine_GameFlagClear(0x30D);
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the third platform south from the middle in the third puzzle room.
static void Fuchin_ThirdRoom_MoveThirdPlatformSouthFromMiddle(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x30b);
  Engine_GameFlagClear(0x30c);
  Engine_GameFlagClear(0x30d);
  Engine_GameFlagClear(0x30e);
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the third platform north from the lower stop in the third puzzle room.
static void Fuchin_ThirdRoom_MoveThirdPlatformNorthFromLowerStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, -80);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x30b);
  Engine_GameFlagClear(0x30c);
  Engine_GameFlagClear(0x30d);
  Engine_GameFlagClear(0x30e);
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the third platform south from the lower stop in the third puzzle room.
static void Fuchin_ThirdRoom_MoveThirdPlatformSouthFromLowerStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x30B);
  Engine_GameFlagClear(0x30C);
  Engine_GameFlagClear(0x30D);
  Engine_GameFlagClear(0x30E);
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the fourth platform north in the third puzzle room.
static void Fuchin_ThirdRoom_MoveFourthPlatformNorth(void) {
  Engine_AudioPlayCue(SE_241);

  if (Engine_GameFlagIsSet(0x308) != 0 || Engine_GameFlagIsSet(0x30d) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, -64);
    Engine_GameFlagClear(0x30f);
    Engine_GameFlagClear(0x310);
    Engine_GameFlagSet(0x311);
    Engine_GameFlagClear(0x312);
  } else if (Engine_GameFlagIsSet(0x30c) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, -112);
    Engine_GameFlagClear(0x30f);
    Engine_GameFlagSet(0x310);
    Engine_GameFlagClear(0x311);
    Engine_GameFlagClear(0x312);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, -128);
    Engine_GameFlagSet(0x30f);
    Engine_GameFlagClear(0x310);
    Engine_GameFlagClear(0x311);
    Engine_GameFlagClear(0x312);
  }
  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Clears all saved intermediate stops for the fourth platform.
static void Fuchin_ClearFourthPlatformStops(void) {
  Engine_GameFlagClear(0x30F);
  Engine_GameFlagClear(0x310);
  Engine_GameFlagClear(0x311);
  Engine_GameFlagClear(0x312);
}

/// Moves the fourth platform south from the north end in the third puzzle room.
static void Fuchin_ThirdRoom_MoveFourthPlatformSouthFromNorthEnd(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, 128);
  Fuchin_ClearFourthPlatformStops();
  Fuchin_FinishPlatformMove(Fuchin_RebuildThirdRoomPlatforms);
}

/// Nudges the fourth platform north in the third puzzle room.
static void Fuchin_ThirdRoom_NudgeFourthPlatformNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, -16);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x30F);
  Engine_GameFlagClear(0x310);
  Engine_GameFlagClear(0x311);
  Engine_GameFlagClear(0x312);
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the fourth platform south from the upper stop in the third puzzle
/// room.
static void Fuchin_ThirdRoom_MoveFourthPlatformSouthFromUpperStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, 112);
  Engine_AudioPlayCue(SE_289);
  Fuchin_ClearFourthPlatformStops();
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the fourth platform south from the middle in the third puzzle room.
static void Fuchin_ThirdRoom_MoveFourthPlatformSouthFromMiddle(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Fuchin_ClearFourthPlatformStops();
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the fourth platform south from the lower stop in the third puzzle
/// room.
static void Fuchin_ThirdRoom_MoveFourthPlatformSouthFromLowerStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, 80);
  Engine_AudioPlayCue(SE_289);
  Fuchin_ClearFourthPlatformStops();
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the fourth platform south from the last stop in the third puzzle room.
static void Fuchin_ThirdRoom_MoveFourthPlatformSouthFromLastStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_PLATFORM, 0, 48);
  Engine_AudioPlayCue(SE_289);
  Fuchin_ClearFourthPlatformStops();
  Engine_TaskWait(2);
  Fuchin_RebuildThirdRoomPlatforms();
}

/// Moves the first platform south in the fourth puzzle room.
static void Fuchin_FourthRoom_MoveFirstPlatformSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 0, 112);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x313);
  Engine_TaskWait(2);
  Fuchin_RebuildFourthRoomPlatforms();
}

/// Moves the first platform north in the fourth puzzle room.
static void Fuchin_FourthRoom_MoveFirstPlatformNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_PLATFORM, 0, -112);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x313);
  Engine_TaskWait(2);
  Fuchin_RebuildFourthRoomPlatforms();
}

/// Moves the second platform west in the fourth puzzle room.
static void Fuchin_FourthRoom_MoveSecondPlatformWest(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_PLATFORM, -128, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x314);
  Engine_TaskWait(2);
  Fuchin_RebuildFourthRoomPlatforms();
}

/// Moves the second platform east in the fourth puzzle room.
static void Fuchin_FourthRoom_MoveSecondPlatformEast(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_PLATFORM, 128, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x314);
  Engine_TaskWait(2);
  Fuchin_RebuildFourthRoomPlatforms();
}

/// Moves the third platform east in the fourth puzzle room.
static void Fuchin_FourthRoom_MoveThirdPlatformEast(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, 160, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(0x315);
  Engine_TaskWait(2);
  Fuchin_RebuildFourthRoomPlatforms();
}

/// Moves the third platform west in the fourth puzzle room.
static void Fuchin_FourthRoom_MoveThirdPlatformWest(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_PLATFORM, -160, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(0x315);
  Engine_TaskWait(2);
  Fuchin_RebuildFourthRoomPlatforms();
}

/// Advances the first platform through its two rolling animations.
static void Fuchin_AnimateFirstPlatform(void) {
  Engine_ActorSetAnimation(ACTOR_FIRST_PLATFORM, 1);
  Engine_ActorSetAnimation(ACTOR_FIRST_PLATFORM, 2);
}

/// Advances the second platform through its two rolling animations.
static void Fuchin_AnimateSecondPlatform(void) {
  Engine_ActorSetAnimation(ACTOR_SECOND_PLATFORM, 1);
  Engine_ActorSetAnimation(ACTOR_SECOND_PLATFORM, 2);
}

/// Advances the third platform through its two rolling animations.
static void Fuchin_AnimateThirdPlatform(void) {
  Engine_ActorSetAnimation(ACTOR_THIRD_PLATFORM, 1);
  Engine_ActorSetAnimation(ACTOR_THIRD_PLATFORM, 2);
}

/// Advances the fourth platform through its two rolling animations.
static void Fuchin_AnimateFourthPlatform(void) {
  Engine_ActorSetAnimation(ACTOR_FOURTH_PLATFORM, 1);
  Engine_ActorSetAnimation(ACTOR_FOURTH_PLATFORM, 2);
}

/// Creates a stationary foreground actor with its palette and priority set.
static struct FieldActor *Fuchin_CreateForegroundActor(s32 x, s32 y, s32 z,
                                                       s32 resourceId) {
  struct FieldActor *actor = Engine_ObjectCreate(resourceId, x, y, z);

  if (actor != 0) {
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    Engine_ActorSetSpriteFlags(actor, 0);
    ObjectGroup_SetChildValue(actor, 15);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return 0;
}

/// Selects interactions for the current room.
const struct SceneEvent *ImilFuchinFalls_GetEvents(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_ImilFuchinFalls1) {
    return gImilFuchinFalls_DjinniRoomEvents;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls2) {
    return gImilFuchinFalls_FirstRoomEvents;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls3) {
    return gImilFuchinFalls_SecondRoomEvents;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls4) {
    return gImilFuchinFalls_ThirdRoomEvents;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls5) {
    return gImilFuchinFalls_FourthRoomEvents;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls6) {
    return gImilFuchinFalls_KiRoomEvents;
  }
  if (sceneId == (s32)&SceneId_ImilFuchinFalls7) {
    return gImilFuchinFalls_DragonRoomEvents;
  }
  return gImilFuchinFalls_NoEvents;
}

/// Faces the leader south and leaves the Imil Djinn room.
static void Imil_LeaveDjinnRoom(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  leader->facing = FACING_SOUTH;
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(1);
}

/// Starts sliding the selected actor in the held direction.
static void Imil_MoveAcrossIce(void) { Imil_SlideActorAcrossIce(); }

/// Centers a platform on its cell and gives it underfoot priority.
static void Fuchin_PlacePlatformAtCell(s32 id, s32 column, s32 row) {
  struct FieldActor *actor = Object_GetById(id);

  if (actor != NULL) {
    Engine_ActorSetSpritePriority(id, 3);
    actor->terrainId = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = PIXELS(column * 16) + PIXELS(8);
    actor->z.fixed = PIXELS(row * 16) + PIXELS(8);
  }
}

/// Restores platform positions and collision cells in the first puzzle room.
static void Fuchin_RebuildFirstRoomPlatforms(void) {
  {
    s32 column = 8;
    s32 row = 29;
    Map_CopyCellAttributes(8, 42, 15, 5, column, row);
  }

  if (Engine_GameFlagIsSet(0x301) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 22, 31);
    {
      s32 column = 8;
      s32 row = 30;
      Map_CopyCellAttributes(9, 30, 1, 3, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 8, 31);
    {
      s32 column = 22;
      s32 row = 30;
      Map_CopyCellAttributes(9, 30, 1, 3, column, row);
    }
  }

  if (Engine_GameFlagIsSet(0x302) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_SECOND_PLATFORM, 12, 29);
    {
      s32 column = 11;
      s32 row = 33;
      Map_CopyCellAttributes(14, 33, 3, 1, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_SECOND_PLATFORM, 12, 33);
    {
      s32 column = 11;
      s32 row = 29;
      Map_CopyCellAttributes(14, 29, 3, 1, column, row);
    }
  }

  if (Engine_GameFlagIsSet(0x303) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 18, 29);
    {
      s32 column = 17;
      s32 row = 33;
      Map_CopyCellAttributes(14, 33, 3, 1, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 18, 33);
    {
      s32 column = 17;
      s32 row = 29;
      Map_CopyCellAttributes(14, 29, 3, 1, column, row);
    }
  }
}

/// Restores platform positions and collision cells in the second puzzle room.
static void Fuchin_RebuildSecondRoomPlatforms(void) {
  {
    s32 column = 12;
    s32 row = 8;
    Map_CopyCellAttributes(0, 28, 10, 18, column, row);
  }

  if (Engine_GameFlagIsSet(0x304) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 21, 20);
    {
      s32 column = 13;
      s32 row = 19;
      Map_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 13, 20);
    {
      s32 column = 21;
      s32 row = 19;
      Map_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
  }

  if (Engine_GameFlagIsSet(0x305) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 12, 20);
    {
      s32 column = 12;
      s32 row = 19;
      Map_CopyCellAttributes(5, 19, 1, 3, column, row);
    }
    {
      s32 column = 13;
      s32 row = 19;
      Map_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
    if (Engine_GameFlagIsSet(0x304) != 0) {
      Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 21, 20);
      {
        s32 column = 13;
        s32 row = 19;
        Map_CopyCellAttributes(20, 19, 1, 3, column, row);
      }
      {
        s32 column = 12;
        s32 row = 19;
        Map_CopyCellAttributes(20, 19, 1, 3, column, row);
      }
    }
  }

  if (Engine_GameFlagIsSet(0x306) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_SECOND_PLATFORM, 15, 21);
    {
      s32 column = 14;
      s32 row = 17;
      Map_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_SECOND_PLATFORM, 15, 17);
    {
      s32 column = 14;
      s32 row = 21;
      Map_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  }

  if (Engine_GameFlagIsSet(0x307) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 19, 8);
    {
      s32 column = 18;
      s32 row = 25;
      Map_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 19, 25);
    {
      s32 column = 18;
      s32 row = 8;
      Map_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  }
}

/// Restores the interacting platform stops and collision cells in the third
/// room.
static void Fuchin_RebuildThirdRoomPlatforms(void) {
  {
    Map_CopyCellAttributes(12, 3, 9, 16, 12, 21);
  }

  if (Engine_GameFlagIsSet(0x308) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 14, 25);
    {
      Map_CopyCellAttributes(16, 24, 1, 3, 20, 24);
    }
  } else if (Engine_GameFlagIsSet(0x309) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 17, 25);
    {
      s32 row = 24;
      Map_CopyCellAttributes(18, 24, 1, 3, 20, row);
      Map_CopyCellAttributes(18, 24, 1, 3, 14, row);
      Map_CopyCellAttributes(8, 41, 1, 3, 17, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 20, 25);
    {
      Map_CopyCellAttributes(16, 24, 1, 3, 14, 24);
    }
  }

  if (Engine_GameFlagIsSet(0x30a) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_SECOND_PLATFORM, 13, 35);
    {
      Map_CopyCellAttributes(14, 34, 1, 3, 15, 34);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_SECOND_PLATFORM, 15, 35);
    {
      Map_CopyCellAttributes(14, 34, 1, 3, 13, 34);
    }
  }

  if (Engine_GameFlagIsSet(0x30b) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 15, 22);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 30);
      Map_CopyCellAttributes(5, 41, 3, 1, column, 22);
    }
  } else if (Engine_GameFlagIsSet(0x30c) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 15, 23);
    {
      s32 column = 14;
      Map_CopyCellAttributes(5, 42, 3, 1, column, 23);
      Map_CopyCellAttributes(14, 29, 3, 1, column, 30);
      Map_CopyCellAttributes(10, 44, 3, 1, column, 21);
    }
  } else if (Engine_GameFlagIsSet(0x30d) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 15, 26);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 22);
      Map_CopyCellAttributes(5, 43, 3, 1, column, 26);
      Map_CopyCellAttributes(14, 29, 3, 1, column, 30);
    }
  } else if (Engine_GameFlagIsSet(0x30e) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 15, 27);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 22);
      Map_CopyCellAttributes(14, 29, 3, 1, column, 30);
      Map_CopyCellAttributes(5, 44, 3, 1, column, 27);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 15, 30);
  }

  if (Engine_GameFlagIsSet(0x30f) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FOURTH_PLATFORM, 15, 23);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 31);
      Map_CopyCellAttributes(10, 40, 3, 1, column, 23);
    }
  } else if (Engine_GameFlagIsSet(0x310) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FOURTH_PLATFORM, 15, 24);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 31);
      Map_CopyCellAttributes(10, 41, 3, 1, column, 24);
    }
  } else if (Engine_GameFlagIsSet(0x311) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FOURTH_PLATFORM, 15, 27);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 31);
      Map_CopyCellAttributes(10, 42, 3, 1, column, 27);
    }
  } else if (Engine_GameFlagIsSet(0x312) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FOURTH_PLATFORM, 15, 28);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 31);
      Map_CopyCellAttributes(10, 43, 3, 1, column, 28);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_FOURTH_PLATFORM, 15, 31);
  }
}

/// Restores both halves of the fourth puzzle room from its saved platform
/// stops.
static void Fuchin_RebuildFourthRoomPlatforms(void) {
  if (((u32)((u16)gGameState.entrance - 1) << 16) <= 0x10000) {
    {
      s32 column = 14;
      s32 row = 10;
      Map_CopyCellAttributes(22, 20, 9, 8, column, row);
    }
  } else {
    {
      s32 column = 7;
      s32 row = 45;
      Map_CopyCellAttributes(20, 45, 11, 4, column, row);
    }
  }

  if (Engine_GameFlagIsSet(0x313) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 20, 17);
    {
      s32 column = 19;
      s32 row = 10;
      Map_CopyCellAttributes(19, 11, 3, 1, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_FIRST_PLATFORM, 20, 10);
    {
      s32 column = 19;
      s32 row = 17;
      Map_CopyCellAttributes(19, 11, 3, 1, column, row);
    }
  }

  if (Engine_GameFlagIsSet(0x314) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_SECOND_PLATFORM, 14, 16);
    {
      s32 column = 22;
      s32 row = 15;
      Map_CopyCellAttributes(16, 15, 1, 3, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_SECOND_PLATFORM, 22, 16);
    {
      s32 column = 14;
      s32 row = 15;
      Map_CopyCellAttributes(16, 15, 1, 3, column, row);
    }
  }

  if (Engine_GameFlagIsSet(0x315) != 0) {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 17, 46);
    {
      s32 column = 7;
      s32 row = 45;
      Map_CopyCellAttributes(15, 15, 1, 3, column, row);
    }
  } else {
    Fuchin_PlacePlatformAtCell(ACTOR_THIRD_PLATFORM, 7, 46);
    {
      s32 column = 17;
      s32 row = 45;
      Map_CopyCellAttributes(15, 15, 1, 3, column, row);
    }
  }
}

/// Attaches the active cave light to the party leader.
static void Fuchin_FollowLeaderWithLight(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gWorkSlot + 36);

    work->actor = Object_GetById(ACTOR_PARTY_LEADER);
  }
}

/// Detaches the active cave light from the party leader.
static void Fuchin_StopLightFollowingLeader(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gWorkSlot + 36);

    work->actor = NULL;
  }
}

/// Entering the Djinni's ice room the first time, the leader walks toward the
/// Djinni; otherwise each room restores its platforms, eye and lighting.
s32 ImilFuchinFalls_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gGameState.scene == (s32)&SceneId_ImilFuchinFalls1) {
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
    Imil_WalkTowardDjinni();
  } else {
    ImilFuchinFalls_PrepareRoom();
  }
  return 0;
}

/// Shows the Imil Djinni and walks the leader toward it.
static void Imil_WalkTowardDjinni(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  actor = Object_GetById(ACTOR_IMIL_DJINNI);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 264, 196);
  Engine_EventEnd();
}

/// Restores room props and the cave light for this entrance.
static void ImilFuchinFalls_PrepareRoom(void) {
  struct FieldActor *actor;
  /* FAKEMATCH: needed to match: the shared zero/address-word lifetime avoids
   * swapping registers and adding a heap-cell add. Literal zeros and the slot
   * read in place: differ 51. */
  s32 value;

  ImilFuchinFalls_RebuildPlatformRoom();
  if (gGameState.scene == (s32)&SceneId_ImilFuchinFalls4) {
    if (!GameFlag_IsSet(0xf13) && gGameState.entrance == 1) {
      ImilFuchinFalls_PlaceDragonsEye();
    }
    if ((u16)(gGameState.entrance - 2) <= 3) {
      Fuchin_CreateForegroundActor(PIXELS(156), 0, PIXELS(452), 223);
      Fuchin_CreateForegroundActor(PIXELS(188), 0, PIXELS(452), 223);
    }
  } else if (gGameState.scene == (s32)&SceneId_ImilFuchinFalls7) {
    value = 0;
    actor = Actor_Get(8);
    gImilFuchinFalls_TrackLeader = value;
    actor->motionFlags = value;
    actor->y.fixed = value;
    Engine_ActorSetSpritePriority(8, 1);
    Actor_SetChildValue(8, 15);
    switch (gGameState.entrance) {
    case 1:
    case 2:
      BattleFx_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      break;
    case 5:
      BattleFx_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      value = *(s32 *)(gWorkSlot + 36);
      ((struct FadeOverlayWork *)value)->actor = NULL;
      break;
    }
    if (gGameState.entrance <= 6) {
      if (GameFlag_IsSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET)) {
        Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
        Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
      } else {
        gEventWork->startTransition =
            SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        ColorBuffer_ApplySource(0x203108, 1);
        ColorBuffer_ApplyTarget(0x203108, 1);
        Engine_ColorBufferInterpolate(1);
        Engine_TaskWait(1);
      }
    }
  }
}

/// Restores the collision templates for the current platform room.
static void ImilFuchinFalls_RebuildPlatformRoom(void) {
  if (gGameState.scene == (s32)&SceneId_ImilFuchinFalls2) {
    Map_CopyCellAttributes(8, 29, 15, 5, 8, 42);
    Fuchin_RebuildFirstRoomPlatforms();
  } else if (gGameState.scene == (s32)&SceneId_ImilFuchinFalls3) {
    Map_CopyCellAttributes(12, 8, 10, 18, 0, 28);
    Fuchin_RebuildSecondRoomPlatforms();
  } else if (gGameState.scene == (s32)&SceneId_ImilFuchinFalls4 &&
             gGameState.entrance != 1) {
    Map_CopyCellAttributes(12, 21, 9, 16, 12, 3);
    Fuchin_RebuildThirdRoomPlatforms();
  } else if (gGameState.scene == (s32)&SceneId_ImilFuchinFalls5) {
    if (gGameState.entrance == 1 || gGameState.entrance == 2) {
      Map_CopyCellAttributes(14, 10, 9, 8, 22, 20);
    } else {
      Map_CopyCellAttributes(7, 45, 11, 4, 20, 45);
    }
    Fuchin_RebuildFourthRoomPlatforms();
  }
}

/// Hops across a platform edge when its current stop permits it.
static void ImilFuchinFalls_HopOnTrigger(void) {
  s32 trigger = gEventWork->touchedTrigger;

  if (gGameState.scene == (s32)&SceneId_ImilFuchinFalls3) {
    if (trigger == 17) {
      ImilFuchinFalls_HopBy(0, -32);
    } else {
      ImilFuchinFalls_HopBy(-32, 0);
    }
  }
  if (gGameState.scene == (s32)&SceneId_ImilFuchinFalls4 && trigger == 25 &&
      Engine_GameFlagIsSet(0x309)) {
    ImilFuchinFalls_HopBy(0, 32);
  }
}

/// Hops the leader one gap north.
static void Fuchin_HopNorth(void) { ImilFuchinFalls_HopBy(0, -32); }

/// Hops the leader one gap west.
static void Fuchin_HopWest(void) { ImilFuchinFalls_HopBy(-32, 0); }

/// Jumps the leader by a pixel offset while using the hopping animations.
static void ImilFuchinFalls_HopBy(s32 dx, s32 dz) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(5) / 2, PIXELS(5) / 4);
  Engine_ActorSetDestinationOffset(0, dx, dz);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 7);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
  Engine_EventEnd();
}

/// Creates the collectible eye and upload its item icon.
static void ImilFuchinFalls_PlaceDragonsEye(void) {
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  GlyphTransfer *glyph;
  u8 clear = 0;

  actor = Object_Create(22, PIXELS(248), PIXELS(8), PIXELS(152));
  if (actor != NULL) {
    sprite = actor->sprite;
    sprite->flags = clear;
    sprite->count = clear;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    actor->motionFlags = clear;
    actor->unk5c = 1;
    glyph = Engine_HeapAllocate(17, sizeof(*glyph));
    Engine_ItemLoadIcon(ITEM_DRAGONS_EYE);
    Engine_VramLoad(sprite->slot, 128, glyph->tiles);
    Engine_HeapRelease(17);
    gImilFuchinFalls_DragonsEye = actor;
  }
}

/// Gives the eye to the party and removes its displayed actor.
static void Fuchin_CollectDragonsEye(void) {
  Engine_EventBegin();

  if (gImilFuchinFalls_DragonsEye != NULL) {
    Engine_RunRisingObjectSequence(gImilFuchinFalls_DragonsEye, 3);
  }

  Engine_PartyGiveItem(ITEM_DRAGONS_EYE, 0);
  Engine_GameFlagSet(0xF13);

  if (gImilFuchinFalls_DragonsEye != NULL) {
    Engine_ObjectDispatchRelease(gImilFuchinFalls_DragonsEye);
  }

  Engine_EventEnd();
}

/// Applies the dust velocity and scale change for one frame.
static void Fuchin_UpdateDustMotion(union FieldObject *raw) {
  struct FieldEffect *object = &raw->effect;

  object->x += object->velocity.x;
  object->y += object->velocity.y;
  object->z += object->velocity.z;
  object->scaleX += object->scaleRateX;
  object->scaleY += object->scaleRateY;
}

/// Selects the dust palette when its object script finishes looping.
static s32 Fuchin_SetDustPalette(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, 15);
  return 0;
}

/// Creates scripted cave dust with the requested velocity, palette and scale.
void ImilFuchinFalls_SpawnDust(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra) {
  struct FieldEffect *effect;
  struct AnimationObject *sprite;
  const s32 *script;
  /* FAKEMATCH: needed to match: copying three scripts keeps the 12-byte frame
   * and prevents an early scale-script load. Reading
   * gImilFuchinFalls_DustScripts in place: differ 107. */
  struct CaveEffectScripts scripts =
      *(const struct CaveEffectScripts *)gImilFuchinFalls_DustScripts;

  effect = (struct FieldEffect *)Engine_ObjectCreate(222, x, y, z);
  if (effect == 0)
    return;
  sprite = effect->sprite;
  Object_SetMode((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Engine_ObjectSetScript((struct FieldActor *)effect,
                         scripts.script[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->flags = 0;
  effect->update = Fuchin_UpdateDustMotion;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  sprite->image.priority = 1;
  if ((flags & 0xffff0000) == 0 || extra == 0)
    return;
  if (flags & EFFECT_USE_PALETTE)
    ObjectGroup_SetChildValue((struct FieldActor *)effect, extra->palette);
  if (flags & EFFECT_USE_PRIORITY) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority = extra->priority;
  }
  if (flags & EFFECT_USE_START_SCALE) {
    effect->scaleX = extra->startScaleX;
    effect->scaleY = extra->startScaleY;
  }
  if (flags & EFFECT_SCALE_TO_TARGET) {
    script = scripts.script[flags & EFFECT_SCRIPT_MASK];
    if (flags & EFFECT_USE_START_SCALE) {
      effect->scaleRateX = (extra->targetScaleX - effect->scaleX) / script[3];
      effect->scaleRateY = (extra->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (extra->targetScaleX - Q16_ONE) / script[3];
      effect->scaleRateY = (extra->targetScaleY - Q16_ONE) / script[3];
    }
  }
}

/// Emits cave-mouth dust every fourth frame while the dragon awakens.
static void ImilFuchinFalls_BlowCaveMouthDust(void) {
  struct CaveDustOptions options;
  s32 framePhase;
  s32 velocityX;
  s32 velocityY;

  framePhase = gFrameCount & 3;
  if (framePhase != 0)
    return;
  options.palette = 10;
  options.startScaleX = Q16_ONE / 2;
  options.startScaleY = Q16_ONE / 2;
  options.targetScaleX = 9 * Q16_ONE / 5;
  options.targetScaleY = 9 * Q16_ONE / 5;
  if ((gFrameCount & 7) == 0)
    Engine_AudioPlayCue(SE_136);
  velocityX = -PIXELS(1) - ((((u32)Engine_RandomNext() << 1) >> 16) << 16);
  velocityY = -((((u32)Engine_RandomNext() * 3) >> 16) * (PIXELS(1) / 5));
  ImilFuchinFalls_SpawnDust(
      PIXELS(308), PIXELS(64), PIXELS(222), velocityX, velocityY, framePhase,
      1 | EFFECT_USE_PALETTE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
      (const struct EffectOptions *)&options);
}

/// Inserts the eye, light the dragon room and reveal its hidden path.
static void Fuchin_LightDragonWithEye(void) {
  u32 pass;

  // Flash the room as the eye opens the dragon's path.
  Engine_AudioPlayCue(MUS_019);
  Engine_AudioPlayCue(SE_182);
  Engine_EventBegin();
  Battle_ResetEffectCounter();

  for (pass = 0; pass < 4; pass++) {
    Engine_ColorBufferApplyTarget(0x204318, 1);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(2);
    if (pass == 0) {
      Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
      Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
    }
    Engine_ColorBufferApplyTarget(0x203108, 1);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(2);
  }

  Engine_TaskWait(30);

  // Blow dust from the cave mouth while the room brightens.
  FieldScheduler_AddCallback(ImilFuchinFalls_BlowCaveMouthDust,
                             TASK_PRIORITY_SCENE);
  Engine_TaskWait(40);
  Engine_ColorBufferApplyTarget(0x201090, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_TaskWait(80);
  Engine_TaskRemoveCallback(ImilFuchinFalls_BlowCaveMouthDust);
  Engine_TaskWait(20);

  // Settle the lighting, consume the eye and remember the revealed path.
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(80);

  Engine_TaskWait(80);

  Engine_GameFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET);
  PartyInventory_Discard(ITEM_DRAGONS_EYE);
  Audio_PlayCueFromEventWork();

  Engine_EventEnd();
}

/// Reads the inscription describing the dragon flame.
static void Fuchin_ReadDragonFlameInscription(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFuchinDragonFlameShowsPath, 1);
  Engine_EventEnd();
}

/// Reads the inscription about the secret power of Ki.
static void Fuchin_ReadKiInscription(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFuchinSecretKiRevealed, 1);
  Engine_EventEnd();
}

/// Reads the inscription about light revealing shadows.
static void Fuchin_ReadShadowInscription(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFuchinLightRevealsShadows, 1);
  Engine_EventEnd();
}

/// Describes the dragon eyes or offers to insert the carried eye.
static void Fuchin_SearchDragonStatue(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET) != 0) {
    Engine_MessageShowCentered(MsgFuchinDragonRedEyes, 1);
  } else {
    Engine_MessageShowCentered(MsgFuchinEyelessDragon, 1);
    if (PartyInventory_FindOwner(ITEM_DRAGONS_EYE) != -1) {
      gEventWork->unk172 = 1;
    }
  }
  Engine_EventEnd();
}

/// Restores the dark cave lighting when Reveal expires.
static void Fuchin_EndRevealLighting(void) {
  struct BattleFxScene *scene;
  struct DisplayTransitionState *transition;

  if (gGameState.entrance <= 6) {
    scene = *(gWorkSlot + 56);
    transition = *(gWorkSlot + 31);
    scene->childOption = 1;
    transition->nextMode = 0;
    transition->duration = 1;
    transition->step = 1;
    Engine_ColorBufferApplySource(0, 1);
    Engine_ColorBufferApplyTarget(0x203108, 1);
    Engine_ColorBufferInterpolate(16);
    Engine_TaskWait(16);
  }
}

/// Turns the sliding actor gradually toward the held direction.
static void Imil_TurnSkaterTowardHeldDirection(union FieldObject *object) {
  struct FieldActor *actor = &object->actor;
  s32 timer;
  s32 direction;
  u16 facing;

  timer = actor->timer;
  if (timer != 0) {
    actor->timer--;
    return;
  }
  actor->steeringFlags = 0;
  direction = gImilFuchinFalls_KeyHeadings[(gKeysHeld >> 4) & 15];
  if (direction == -1) {
    Object_SetMode(actor, 9);
    return;
  }
  facing = actor->facing;
  direction = (s16)(direction - facing);
  if (direction > 0x1000)
    direction = 0x1000;
  if (direction < -0x1000)
    direction = -0x1000;
  actor->facing = facing + direction;
  Object_SetMode(actor, 2);
  ObjectDispatch_SetAnimationSpeed(actor, 0x30);
}

/// Slides along the ice until terrain or a collision boundary ends the move.
static void Imil_SlideActorAcrossIce(void) {
  struct FieldActor *actor;
  struct FieldPosition probe;
  s32 heading;
  s32 startCollision;
  s32 nextCollision;
  s32 z;
  s32 x;

  actor = Engine_ActorLookup(gGameState.selectedActor);

  for (;;) {
    heading = gImilFuchinFalls_SlideHeadings[(gKeysHeld >> 4) & 15];
    if (((u32)heading << 16) == 0xffff0000) {
      return;
    }

    Engine_EventBegin();

    // Start at the cell center and reject a blocked or raised next cell.
    probe.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
    probe.y = actor->y.fixed;
    probe.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);
    z = probe.z;
    x = probe.x;
    startCollision = GetMapCellCollision(actor->terrainId, x, z);
    FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);

    nextCollision = GetMapCellCollision(actor->terrainId, probe.x, probe.z);
    if (nextCollision == 255 ||
        Map_GetTerrainHeight(actor->terrainId, probe.x, probe.z) -
                actor->y.fixed >
            PIXELS(8)) {
      actor->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    actor->speed = PIXELS(2);
    actor->acceleration = PIXELS(1) / 10;
    actor->timer = 0;
    Engine_ObjectSetPosition(actor, x, actor->y.fixed, z);

    Object_SetMode(actor, 2);
    ObjectDispatch_SetAnimationSpeed(actor, 48);
    FieldObject_WaitForMove(actor);
    actor->update = Imil_TurnSkaterTowardHeldDirection;

    // Keep sliding through cells with the same collision kind.
    for (;;) {
      FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
      nextCollision = GetMapCellCollision(actor->terrainId, probe.x, probe.z);
      if (nextCollision == 255) {
        break;
      }
      if (Map_GetTerrainHeight(actor->terrainId, probe.x, probe.z) -
              actor->y.fixed >
          PIXELS(8)) {
        break;
      }
      x = probe.x;
      z = probe.z;
      actor->speed = PIXELS(2);
      actor->acceleration = PIXELS(1) / 10;
      Engine_ObjectSetPosition(actor, probe.x, probe.y, probe.z);
      FieldObject_WaitForMove(actor);
      if (nextCollision != startCollision) {
        goto blocked;
      }
    }

    actor->speed = PIXELS(2);
    actor->acceleration = PIXELS(1);
    Engine_ObjectSetPosition(actor, x, actor->y.fixed, z);
    FieldObject_WaitForMove(actor);
    Engine_TaskWait(2);
  }

blocked:
  // A collision-kind change ends the slide and restores normal steering.
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;

  actor->acceleration = PIXELS(1) / 4;

tail:
  Engine_TaskWait(10);
  Engine_EventEnd();
}

s32 *gImilFuchinFalls_DustScripts[] = {
    gImilFuchinFalls_DustScript0,
    gImilFuchinFalls_DustScript1,
    gImilFuchinFalls_DustScript2,
};

// clang-format off
s16 gImilFuchinFalls_KeyHeadings[] = {
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
    FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
    FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

s16 gImilFuchinFalls_SlideHeadings[] = {
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
    FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
    FACING_EAST, FACING_EAST,
};

struct FieldActor *gImilFuchinFalls_DragonsEye = NULL;

struct SceneEntrance gImilFuchinFalls_NoEntrances[] = {
    { 0, CONDITION_ALWAYS, 256, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_DjinniRoomEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 264, 0, 289, FACING_NORTH, 0, 135, 62, 376, 328, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_FirstRoomEntrances[] = {
    { 0, CONDITION_ALWAYS, 248, 0, 600, FACING_SOUTH, 0, 64, 64, 416, 656, 0 },
    { 1, CONDITION_ALWAYS, 248, 0, 632, FACING_NORTH, 0, 64, 368, 416, 656, 0 },
    { 2, CONDITION_ALWAYS, 376, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { 3, CONDITION_ALWAYS, 120, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { 4, CONDITION_ALWAYS, 232, 0, 408, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { 5, CONDITION_ALWAYS, 232, 0, 344, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { 6, CONDITION_ALWAYS, 376, 0, 152, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { 7, CONDITION_ALWAYS, 120, 0, 200, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { 8, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_SecondRoomEntrances[] = {
    { 1, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 64, 416, 480, 0 },
    { 2, CONDITION_ALWAYS, 152, 0, 200, FACING_NORTH, 0, 96, 64, 416, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ThirdRoomEntrances[] = {
    { 1, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 128, 64, 368, 256, 0 },
    { 2, CONDITION_ALWAYS, 184, 0, 328, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { 3, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { 4, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { 5, CONDITION_ALWAYS, 280, 0, 616, FACING_NORTH, 0, 96, 256, 416, 656, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_FourthRoomEntrances[] = {
    { 1, CONDITION_ALWAYS, 328, 0, 72, FACING_SOUTH, 0, 64, 16, 416, 416, 0 },
    { 2, CONDITION_ALWAYS, 248, 0, 392, FACING_NORTH, 0, 64, 16, 416, 416, 0 },
    { 3, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 416, 416, 800, 0 },
    { 4, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 64, 416, 416, 800, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_KiRoomEntrances[] = {
    { 1, CONDITION_ALWAYS, 264, 0, 232, FACING_NORTH, 0, 128, 16, 384, 256, 0 },
    { 2, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 256, 416, 720, 0 },
    { 3, CONDITION_ALWAYS, 216, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { 4, CONDITION_ALWAYS, 280, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_DragonRoomEntrances[] = {
    { 1, CONDITION_ALWAYS, 280, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { 2, CONDITION_ALWAYS, 216, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { 3, CONDITION_ALWAYS, 104, 0, 280, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { 4, CONDITION_ALWAYS, 200, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { 5, CONDITION_ALWAYS, 248, 0, 312, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { 6, CONDITION_ALWAYS, 296, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { 7, CONDITION_ALWAYS, 200, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { 8, CONDITION_ALWAYS, 248, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { 9, CONDITION_ALWAYS, 296, 0, 424, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { 10, CONDITION_ALWAYS, 376, 0, 472, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { 11, CONDITION_ALWAYS, 248, 0, 536, FACING_NORTH, 0, 32, 344, 432, 736, 0 },
    { 12, CONDITION_ALWAYS, 120, 0, 696, FACING_SOUTH, 0, 32, 344, 432, 752, 0 },
    { SCENE_TABLE_END },
};

u32 gImilFuchinFalls_Exits[] = {
    SCENE_EXITS(52),
    SCENE_EXIT(1, 50, 9),
    SCENE_EXIT(0, 62, 0),
    SCENE_EXIT(1, 60, 3),
    SCENE_EXIT(2, 63, 2),
    SCENE_EXIT(3, 65, 4),
    SCENE_EXIT(4, 62, 5),
    SCENE_EXIT(5, 62, 4),
    SCENE_EXIT(6, 64, 4),
    SCENE_EXIT(7, 67, 12),
    SCENE_EXIT(8, 67, 11),
    SCENE_EXIT(0, 63, 0),
    SCENE_EXIT(1, 64, 5),
    SCENE_EXIT(2, 62, 2),
    SCENE_EXIT(0, 64, 0),
    SCENE_EXIT(1, 64, 3),
    SCENE_EXIT(2, 67, 10),
    SCENE_EXIT(3, 64, 1),
    SCENE_EXIT(4, 62, 6),
    SCENE_EXIT(5, 63, 1),
    SCENE_EXIT(0, 65, 0),
    SCENE_EXIT(1, 67, 3),
    SCENE_EXIT(2, 65, 3),
    SCENE_EXIT(3, 65, 2),
    SCENE_EXIT(4, 62, 3),
    SCENE_EXIT(0, 66, 0),
    SCENE_EXIT(1, 66, 2),
    SCENE_EXIT(2, 66, 1),
    SCENE_EXIT(3, 67, 2),
    SCENE_EXIT(4, 67, 1),
    SCENE_EXIT(0, 67, 0),
    SCENE_EXIT(1, 66, 3),
    SCENE_EXIT(2, 66, 4),
    SCENE_EXIT(3, 65, 1),
    SCENE_EXIT(4, 67, 7),
    SCENE_EXIT(5, 67, 8),
    SCENE_EXIT(6, 67, 9),
    SCENE_EXIT(7, 67, 4),
    SCENE_EXIT(8, 67, 5),
    SCENE_EXIT(9, 67, 6),
    SCENE_EXIT(10, 64, 2),
    SCENE_EXIT(11, 62, 8),
    SCENE_EXIT(12, 62, 7),
    SCENE_EXITS_END,
};

struct ScenePlacement gImilFuchinFalls_NoPlacements[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_DjinniRoomPlacements[] = {
    { 0x5c, 0x59, ACTOR_STAND,
        PIXELS(263), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_FirstRoomPlacements[] = {
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(200), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(296), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_SecondRoomPlacements[] = {
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5d, 0x6e, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_ThirdRoomPlacements[] = {
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(328), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(568), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_FourthRoomPlacements[] = {
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(328), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(120), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_DragonRoomPlacements[] = {
    { 0xdf, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gImilFuchinFalls_NoEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilFuchinFalls_DjinniRoomEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_LeaveDjinnRoom) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_MoveAcrossIce) },
    { EVENT_TALK, ACTOR_IMIL_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RecruitDjinni) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilFuchinFalls_FirstRoomEvents[] = {
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveFirstPlatformEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveFirstPlatformWest) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstPlatform) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstPlatform) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveSecondPlatformSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveSecondPlatformNorth) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondPlatform) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondPlatform) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveThirdPlatformSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveThirdPlatformNorth) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, 6 },
    { 0x21, 27, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 8 },
    { SEARCH(SEARCH_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ReadDragonFlameInscription) },
    { SEARCH(SEARCH_CHEST), 100, 0xef1, 0x500001 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilFuchinFalls_SecondRoomEvents[] = {
    { TOUCH_WALKING_INTO(FACING_EAST), 11, 0x306, EVENT_SCRIPT(Fuchin_SecondRoom_MoveFirstPlatformToEastEnd) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_NudgeFirstPlatformWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { TOUCH_WALKING_INTO(FACING_EAST), 17, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveFirstPlatformEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveFirstPlatformToWestEnd) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstPlatform) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstPlatform) },
    { EVENT_TOUCH, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstPlatform) },
    { TOUCH_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { TOUCH_WALKING_INTO(FACING_WEST), 14, 0x304, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveSecondPlatformSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveSecondPlatformNorth) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondPlatform) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondPlatform) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveThirdPlatformSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveThirdPlatformNorth) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, ACTOR_FUCHIN_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_StartDjinnBattle) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilFuchinFalls_ThirdRoomEvents[] = {
    { TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFirstPlatformWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFirstPlatformEastFromMiddle) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFirstPlatformWestIfClear) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFirstPlatformEastFromWestEnd) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, IF_FLAG_SET(0x30b), EVENT_SCRIPT(Fuchin_HopNorth) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstPlatform) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstPlatform) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstPlatform) },
    { TOUCH_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveSecondPlatformWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveSecondPlatformEast) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondPlatform) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondPlatform) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdPlatformNorthFromSouthEnd) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdPlatformSouthFromNorthEnd) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_NudgeThirdPlatformNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 17, 0x308, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdPlatformSouthFromUpperStop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdPlatformNorthFromMiddle) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 18, 0x310, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdPlatformSouthFromMiddle) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdPlatformNorthFromLowerStop) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdPlatformSouthFromLowerStop) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { EVENT_TOUCH, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { EVENT_TOUCH, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdPlatform) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthPlatformNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, 0x308, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthPlatformSouthFromNorthEnd) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_NudgeFourthPlatformNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthPlatformSouthFromUpperStop) },
    { TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_HopWest) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthPlatformSouthFromMiddle) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthPlatformSouthFromLowerStop) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthPlatformSouthFromLastStop) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthPlatform) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthPlatform) },
    { EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthPlatform) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthPlatform) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthPlatform) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x21, 2, CONDITION_ALWAYS, 2 },
    { 0x31, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { SEARCH(SEARCH_UNNAMED), 30, 0xf13, EVENT_SCRIPT(Fuchin_CollectDragonsEye) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gImilFuchinFalls_FourthRoomEvents[] = {
    {TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveFirstPlatformSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveFirstPlatformNorth)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_AnimateFirstPlatform)},
    {EVENT_TOUCH, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_AnimateFirstPlatform)},
    {TOUCH_WALKING_INTO(FACING_WEST), 13, 0x313,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveSecondPlatformWest)},
    {TOUCH_WALKING_INTO(FACING_EAST), 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveSecondPlatformEast)},
    {EVENT_TOUCH, 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_AnimateSecondPlatform)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_AnimateSecondPlatform)},
    {TOUCH_WALKING_INTO(FACING_EAST), 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveThirdPlatformEast)},
    {TOUCH_WALKING_INTO(FACING_WEST), 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveThirdPlatformWest)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_AnimateThirdPlatform)},
    {EVENT_TOUCH, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_AnimateThirdPlatform)},
    {0x21, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {SEARCH(SEARCH_CHEST), 100, 0xf6b, FIND_ITEM(ITEM_ARCTIC_BLADE)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gImilFuchinFalls_KiRoomEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {SEARCH(SEARCH_UNNAMED), 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_ReadKiInscription)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND,
     FIND_ITEM(ITEM_ORB_OF_FORCE)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gImilFuchinFalls_DragonRoomEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
    {EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x31, 10, CONDITION_ALWAYS, 10},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x31, 12, CONDITION_ALWAYS, 12},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FollowLeaderWithLight)},
    {EVENT_TOUCH, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_StopLightFollowingLeader)},
    {SEARCH(SEARCH_UNNAMED), 19, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_ReadShadowInscription)},
    {USE_ITEM(ITEM_DRAGONS_EYE), 20, FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT_SCRIPT(Fuchin_LightDragonWithEye)},
    {SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_SearchDragonStatue)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT_SCRIPT(Fuchin_EndRevealLighting)},
    {SCENE_EVENTS_END},
};

s32 gImilFuchinFalls_DustScript0[] = {
    SC_WAIT(1), SC_LOOP(22, SC_TOP), SC_CALL(Fuchin_SetDustPalette),
    SC_WAIT(1), SC_POS(0, 0, 0),     SC_WAIT(1),
    SC_FREE,
};

s32 gImilFuchinFalls_DustScript1[] = {
    SC_WAIT(1), SC_LOOP(44, SC_TOP), SC_CALL(Fuchin_SetDustPalette),
    SC_WAIT(1), SC_POS(0, 0, 0),     SC_WAIT(1),
    SC_FREE,
};

s32 gImilFuchinFalls_DustScript2[] = {
    SC_WAIT(1), SC_LOOP(126, SC_TOP), SC_CALL(Fuchin_SetDustPalette),
    SC_WAIT(1), SC_POS(0, 0, 0),      SC_WAIT(1),
    SC_FREE,
};
