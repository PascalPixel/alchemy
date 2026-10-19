#include "game_state.h"
#include "unit/scene/sol_sanctum_luna_sol_rooms.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "sol_sanctum_luna_sol_rooms.h"
#include "call.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "types.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "iowrite.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

// Scene temporary flags 00, 01 and 02 select the plain, first-tint and
// second-tint palettes, respectively.

// Forward declarations.
static void SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms(void);
static void SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna(void);
static s32 SolSanctumLunaSolRooms_AreAllStatueLightsLit(void);
static void SolSanctumLunaSolRooms_ShowMessageAndWait(s32 speaker,
                                                      s32 waitFrames);
static void SolSanctumLunaSolRooms_ShowFloatingEye(void);

void InitializeSceneRecordBuffer(void);

struct EventGlobals {
  struct EventWork *event;
  u8 unk04[0x10];
  u8 *field;
};

// Return the arrival positions for the Sol and Luna rooms.
const struct SceneEntrance *SolSanctumLunaSolRooms_GetEntrances(void) {
  return gSolSanctumLunaSolRoomsEntrances;
}

// The Sol and Luna rooms have no region table.
const struct SceneRegion *SolSanctumLunaSolRooms_GetRegions(void) { return 0; }

// Return the connections between the sanctum's puzzle rooms.
const u32 *SolSanctumLunaSolRooms_GetExits(void) {
  return gSolSanctumLunaSolRoomsExits;
}

// Return the statues and party actors used by these rooms.
const struct ScenePlacement *SolSanctumLunaSolRooms_GetPlacements(void) {
  return gSolSanctumLunaSolRoomsPlacements;
}

// Return the room triggers, statue interactions and Kraden's dialogue.
const struct SceneEvent *SolSanctumLunaSolRooms_GetEvents(void) {
  return gSolSanctumLunaSolRoomsEvents;
}

// Crossing trigger 13: the room fades to the tint that matches the seals'
// lights. The first time, Kraden sees the Luna and Sol rooms; with all four
// seals open, he also sees the floor symbol change.
static void Event_UpdateRoomTint(void) {
  s32 paletteActive;

  // Test all four statue-light flags without skipping any flag reads.
  if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
    paletteActive = GameFlag_IsSet(FLAG_SCENE_TEMP_01);
    if (paletteActive == 0) {
      Engine_EventBegin();
      ColorBuffer_ApplyTarget(0x2051cc, 1);
      Engine_ColorBufferInterpolate(20);
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_00);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
        SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms();
      }
      if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
        if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) == 0) {
          SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna();
        }
      }
      Engine_EventEnd();
    }
  } else {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
      Engine_EventBegin();
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
      Engine_ColorBufferInterpolate(20);
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      GameFlag_Clear(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
      Engine_EventEnd();
    }
  }
}

// Crossing trigger 15: the room fades back to its plain colours.
static void Event_RestorePlainTint(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
    Engine_EventBegin();
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
    Engine_ColorBufferInterpolate(20);
    GameFlag_Set(FLAG_SCENE_TEMP_00);
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
    GameFlag_Clear(FLAG_SCENE_TEMP_02);
    Engine_EventEnd();
  }
}

// Crossing trigger 14: as trigger 13, with the tints the other way round.
static void Event_UpdateRoomTintInverse(void) {
  s32 paletteActive;

  if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
    paletteActive = GameFlag_IsSet(FLAG_SCENE_TEMP_00);
    if (paletteActive == 0) {
      Engine_EventBegin();
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
      Engine_ColorBufferInterpolate(20);
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      GameFlag_Clear(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
      Engine_EventEnd();
    }
  } else {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) == 0) {
      Engine_EventBegin();
      ColorBuffer_ApplyTarget(0x2051cc, 1);
      Engine_ColorBufferInterpolate(20);
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_00);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
        SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms();
      }
      Engine_EventEnd();
    }
  }
}

// Crossing trigger 12: the room fades to its second tint.
static void Event_ApplySecondRoomTint(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_02) == 0) {
    ColorBuffer_ApplyTarget(0x202db1, 1);
    Engine_ColorBufferInterpolate(20);
    GameFlag_Set(FLAG_SCENE_TEMP_02);
    GameFlag_Clear(FLAG_SCENE_TEMP_00);
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
  }
}

// Crossing trigger 19: the backdrop turns black.
static void Event_ClearBackdropColor(void) {
  s32 black = 0;
  u16 *backdropColor = (u16 *)&PLTT_BACKDROP;
  *backdropColor = black;
}

// Draw the light of each open seal, from four consecutive flags, with the
// tiles of the given source row.
static inline void SolSanctumLunaSolRooms_DrawStatueLightTiles(s32 firstFlag,
                                                               s32 sourceRow) {
  if (Engine_GameFlagIsSet(firstFlag) != 0)
    Map_CopyCellsTo(45, sourceRow, 34, 10, 2, 1);
  if (Engine_GameFlagIsSet(firstFlag + 1) != 0)
    Map_CopyCellsTo(47, sourceRow, 36, 10, 2, 1);
  if (Engine_GameFlagIsSet(firstFlag + 2) != 0)
    Map_CopyCellsTo(45, sourceRow + 1, 34, 11, 2, 1);
  if (Engine_GameFlagIsSet(firstFlag + 3) != 0)
    Map_CopyCellsTo(47, sourceRow + 1, 36, 11, 2, 1);
}

// Draw the active lights from the current statue flags.
static void SolSanctumLunaSolRooms_DrawCurrentStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, 28);
}

// Draw the active lights from the saved statue flags.
static void SolSanctumLunaSolRooms_DrawSavedStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, 28);
}

// Draw the current lights using their alternate tile rows.
static void SolSanctumLunaSolRooms_DrawAlternateCurrentStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, 30);
}

// Draw the saved lights using their alternate tile rows.
static void SolSanctumLunaSolRooms_DrawAlternateSavedStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, 30);
}

// Flash both tile sets of the current statue lights at this pace.
static inline void SolSanctumLunaSolRooms_FlashCurrentStatueLights(s32 frames) {
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawCurrentStatueLights();
  Engine_EventWait(frames);
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawAlternateCurrentStatueLights();
  Engine_EventWait(frames);
}

// Flash both tile sets of the saved statue lights at this pace.
static inline void SolSanctumLunaSolRooms_FlashSavedStatueLights(s32 frames) {
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawSavedStatueLights();
  Engine_EventWait(frames);
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawAlternateSavedStatueLights();
  Engine_EventWait(frames);
}

// Walking into the hall (trigger 11) the first time: "This is it! We've found
// it!" Kraden studies the symbols and asks the party to keep exploring.
static void Event_KradenFindsHall(void) {
  struct FieldActor *leader;

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Event_UpdateRoomTint();
  }
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FOUND) == 0) {
    Engine_EventBegin();
    // Bring the party into the chamber while Kraden surveys its symbols.
    Engine_EventSetMessage(MsgSolSanctumFound);
    Engine_AudioPlayCue(MUS_017);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_AudioPlayCue(MUS_021);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(7) / 5, PIXELS(7) / 10);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 206);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT, 0);
    Engine_ActorJump(ACTOR_KRADEN, 4, 60);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetPosition(GARET, leader->x.fixed, leader->z.fixed);
    }
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetPosition(JENNA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkTo(GARET, 280, 248);
    Actor_WalkToAndWait(JENNA, 296, 248);
    Engine_ActorSetAnimation(GARET, ANIM_STAND);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 30);
    Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    Engine_CameraMoveTo(PIXELS(288), -1, PIXELS(213), 1);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2) / 5, PIXELS(1) / 5);
    Engine_ActorWalkToAndWait(ACTOR_KRADEN, 288, 176);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 60);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(JENNA, 2);
    Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 10);
    SolSanctumLunaSolRooms_ShowMessageAndWait(JENNA, 10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Actor_FaceDirection(GARET, FACING_NORTHEAST + FACING_STEP, 10);
    SolSanctumLunaSolRooms_ShowMessageAndWait(GARET, 6);
    Actor_SetAttachedEffect(JENNA, 0x102);
    Engine_EventWait(40);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 10);
    SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA,
                                              10);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 20);
    Actor_SetAttachedEffect(ACTOR_KRADEN, 0x102);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 40);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
    Engine_EventWait(40);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 40);
    SolSanctumLunaSolRooms_ShowMessageAndWait(JENNA, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 5, 60);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(GARET, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 2, 80);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 5, 80);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 60);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
    Engine_EventWait(6);
    Engine_EventOpenMessage(ACTOR_KRADEN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventSetMessage(MsgSolSanctumThank);
    } else {
      Engine_EventSetMessage(MsgSolSanctumGoBackVillage);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 7, 20);
    }
    Engine_ActorJump(ACTOR_KRADEN, 4, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Engine_EventSetMessage(MsgSolSanctumPutWayDont);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(JENNA, 4);
    SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA,
                                              6);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(GARET, 20);
    Engine_ActorJump(ACTOR_KRADEN, 6, 20);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 4, 20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(JENNA, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    // End the discussion and return the followers to Isaac.
    Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
    Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_KRADEN);
    Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    Engine_ActorSetAnimation(GARET, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    Engine_ActorSetPosition(GARET, 0, 0);
    Engine_ActorSetAnimation(JENNA, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(JENNA, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(JENNA);
    Engine_ActorSetPosition(JENNA, 0, 0);
    // Retreat does not work here (flag 0x144).
    Engine_GameFlagSet(0x144);
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_FOUND);
    Engine_EventEnd();
  }
}

// Introduce the linked Sol and Luna rooms and Kraden's search request.
static void SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms(void) {
  struct FieldActor *leader;
  s32 request;

  // Stage Kraden beside Isaac, then show the adjoining room.
  Engine_EventSetMessage(MsgSolSanctumWhRoom);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 488, 176);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 472, 168);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
  Engine_ActorJump(ACTOR_KRADEN, 4, 40);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(575), -1, PIXELS(181), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(120);
  SolSanctumLunaSolRooms_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 80);
  Camera_MoveTo(PIXELS(492), -1, PIXELS(168), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgSolSanctumLunaSolRooms);
  } else {
    Engine_EventSetMessage(MsgSolSanctumRoomLunaOne);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            10);
  // Ask Isaac to search ahead while Kraden examines the room.
  request = MsgSolSanctumMeanLookFarther;
  Engine_EventSetMessage(request);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 5, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 4);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage((request + 1));
    GameFlag_Set(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN);
  } else {
    Engine_EventSetMessage((request + 2));
  }
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            4);
  Engine_CameraFollowActor(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 486, 131);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 120);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 2);
  // Leave Kraden by the northern wall for the search interactions.
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
}

// Heading out of the hall (trigger 9) before the passage is found: Kraden
// hurries over: "That way leads out of the sanctum." After the eruption the
// room only fades back to its plain colours.
static void Event_KradenStopsPartyLeaving(void) {
  struct FieldActor *kraden;
  struct FieldActor *leader;
  struct FieldActor *destinationActor;

  kraden = Object_GetById(ACTOR_KRADEN);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FOUND) == 0) {
    return;
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Event_RestorePlainTint();
    return;
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
    return;
  }
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_EventSetMessage(MsgSolSanctumWayLeadsOutSanctum);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
    }
    Engine_EventWait(4);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
             kraden->x.fixed > PIXELS(340)) {
    Actor_SetPosition(ACTOR_KRADEN, PIXELS(392), PIXELS(168));
    Engine_EventWait(4);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      kraden->x.fixed > PIXELS(340)) {
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 232);
  } else {
    // The result is unused.
    GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
  }
  Actor_WalkToAndWait(ACTOR_KRADEN, 288, 232);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
    Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
    destinationActor = Object_GetById(ACTOR_PARTY_LEADER);
    if (destinationActor != 0) {
      Actor_SetDestination(ACTOR_KRADEN, destinationActor->x.part.pixel,
                           destinationActor->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
  } else {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 248);
  }
  Engine_EventEnd();
}

// Animate the first falling light and manage the shared shake pulse.
static void SolSanctumLunaSolRooms_UpdateFirstStatueLight(void) {
  if ((Random_Next() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light1Timer) {
    case 0:
      Event_PlayCue(SE_187);
      Map_CopyCellsTo(46, 59, 30, 33, 1, 5);
      break;
    case 1:
      Map_CopyCellsTo(46, 59, 30, 33, 1, 1);
      Map_CopyCellsTo(46, 59, 30, 34, 1, 5);
      break;
    case 2:
      Map_CopyCellsTo(46, 59, 30, 34, 1, 1);
      Map_CopyCellsTo(46, 59, 30, 35, 1, 5);
      break;
    case 3:
      Map_CopyCellsTo(46, 59, 30, 35, 1, 1);
      Map_CopyCellsTo(46, 59, 30, 36, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      Map_CopyCellsTo(46, 59, 30, 36, 1, 1);
      Map_CopyCellsTo(46, 59, 30, 37, 1, 5);
      break;
    case 0x50:
      Map_CopyCellsTo(46, 49, 30, 33, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light1Timer++;
    if (gSolSanctumLunaSolRooms_Light1Timer >
        ((u32)(Random_Next() * 40) >> 16) + 90) {
      gSolSanctumLunaSolRooms_Light1Timer = 0;
    }
  }
  if (gSolSanctumLunaSolRooms_FlashState != 0) {
    if (gSolSanctumLunaSolRooms_FlashState == 2) {
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    } else if (gSolSanctumLunaSolRooms_FlashState == 1) {
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
    gSolSanctumLunaSolRooms_FlashState--;
  }
}

// Animate the second falling light with its own random restart delay.
static void SolSanctumLunaSolRooms_UpdateSecondStatueLight(void) {
  if ((Engine_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light2Timer) {
    case 0:
      Engine_AudioPlayCue(SE_187);
      Map_CopyCellsTo(47, 59, 42, 33, 1, 5);
      break;
    case 1:
      Map_CopyCellsTo(47, 59, 42, 33, 1, 1);
      Map_CopyCellsTo(47, 59, 42, 34, 1, 5);
      break;
    case 2:
      Map_CopyCellsTo(47, 59, 42, 34, 1, 1);
      Map_CopyCellsTo(47, 59, 42, 35, 1, 5);
      break;
    case 3:
      Map_CopyCellsTo(47, 59, 42, 35, 1, 1);
      Map_CopyCellsTo(47, 59, 42, 36, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      Map_CopyCellsTo(47, 59, 42, 36, 1, 1);
      Map_CopyCellsTo(47, 59, 42, 37, 1, 5);
      break;
    case 0x5a:
      Map_CopyCellsTo(47, 49, 42, 33, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light2Timer++;
    if (gSolSanctumLunaSolRooms_Light2Timer >
        ((u32)(Engine_RandomNext() * 40) >> 16) + 100) {
      gSolSanctumLunaSolRooms_Light2Timer = 0;
    }
  }
}

// Animate the third falling light with its own random restart delay.
static void SolSanctumLunaSolRooms_UpdateThirdStatueLight(void) {
  if ((Engine_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light3Timer) {
    case 0:
      Engine_AudioPlayCue(SE_187);
      Map_CopyCellsTo(48, 59, 31, 36, 1, 5);
      break;
    case 1:
      Map_CopyCellsTo(48, 59, 31, 36, 1, 1);
      Map_CopyCellsTo(48, 59, 31, 37, 1, 5);
      break;
    case 2:
      Map_CopyCellsTo(48, 59, 31, 37, 1, 1);
      Map_CopyCellsTo(48, 59, 31, 38, 1, 5);
      break;
    case 3:
      Map_CopyCellsTo(48, 59, 31, 38, 1, 1);
      Map_CopyCellsTo(48, 59, 31, 39, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      Map_CopyCellsTo(48, 59, 31, 39, 1, 1);
      Map_CopyCellsTo(48, 59, 31, 40, 1, 5);
      break;
    case 0x5f:
      Map_CopyCellsTo(48, 49, 31, 36, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light3Timer++;
    if (gSolSanctumLunaSolRooms_Light3Timer >
        ((u32)(Engine_RandomNext() * 40) >> 16) + 105) {
      gSolSanctumLunaSolRooms_Light3Timer = 0;
    }
  }
}

// Animate the fourth falling light with its own random restart delay.
static void SolSanctumLunaSolRooms_UpdateFourthStatueLight(void) {
  if ((Engine_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light4Timer) {
    case 0:
      Engine_AudioPlayCue(SE_187);
      Map_CopyCellsTo(46, 59, 41, 36, 1, 5);
      break;
    case 1:
      Map_CopyCellsTo(46, 59, 41, 36, 1, 1);
      Map_CopyCellsTo(46, 59, 41, 37, 1, 5);
      break;
    case 2:
      Map_CopyCellsTo(46, 59, 41, 37, 1, 1);
      Map_CopyCellsTo(46, 59, 41, 38, 1, 5);
      break;
    case 3:
      Map_CopyCellsTo(46, 59, 41, 38, 1, 1);
      Map_CopyCellsTo(46, 59, 41, 39, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      Map_CopyCellsTo(46, 59, 41, 39, 1, 1);
      Map_CopyCellsTo(46, 59, 41, 40, 1, 5);
      break;
    case 0x55:
      Map_CopyCellsTo(46, 49, 41, 36, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light4Timer++;
    if (gSolSanctumLunaSolRooms_Light4Timer >
        ((u32)(Engine_RandomNext() * 40) >> 16) + 95) {
      gSolSanctumLunaSolRooms_Light4Timer = 0;
    }
  }
}

// Prepare the statue-room tiles, camera and opening palette flash.
static void SolSanctumLunaSolRooms_PrepareStatueTransition(void) {
  Camera_MoveTo(-1, -1, -1, 0);
  Map_CopyCellsTo(30, 43, 32, 40, 8, 3);
  Map_CopyCellsTo(30, 43, 33, 39, 8, 1);
  Map_CopyCellsTo(30, 43, 36, 38, 3, 3);
  Map_CopyCellsTo(14, 41, 32, 41, 8, 4);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(158), 0);
  Engine_MapRedraw();
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(574), PIXELS(120));
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  ColorBuffer_ApplyTarget(0x2051cc, 1);
  Engine_ColorBufferInterpolate(20);
  GameFlag_Set(FLAG_SCENE_TEMP_01);
  GameFlag_Clear(FLAG_SCENE_TEMP_00);
  GameFlag_Clear(FLAG_SCENE_TEMP_02);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Event_PlayCue(SE_171);
  ColorBuffer_ApplyTarget(0x10005, 1);
  Engine_ColorBufferInterpolate(8);
  Engine_EventWait(32);
  Engine_ColorBufferApplyTarget(0x2051cc, 1);
  Engine_ColorBufferInterpolate(24);
}

// Accelerate the statue lights, then let Kraden retreat from the trap.
static void SolSanctumLunaSolRooms_SpringStatueTrap(void) {
  s32 twelveFrameFlashes;
  s32 eightFrameFlashes;
  s32 sixFrameFlashes;
  s32 fourFrameFlashes;
  s32 twoFrameFlashes;

  Engine_EventBegin();
  SolSanctumLunaSolRooms_PrepareStatueTransition();
  Engine_EventSetMessage(MsgSolSanctumWhatsHappeningAtTheTrap);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT, 0);
  Engine_ActorJump(ACTOR_KRADEN, 6, 30);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            20);
  // Shorten each flash interval as the statue-light sequence accelerates.
  for (twelveFrameFlashes = 0; twelveFrameFlashes != 4; twelveFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(12);
  }
  for (eightFrameFlashes = 0; eightFrameFlashes != 6; eightFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(8);
  }
  for (sixFrameFlashes = 0; sixFrameFlashes != 8; sixFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(6);
  }
  for (fourFrameFlashes = 0; fourFrameFlashes != 10; fourFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(4);
  }
  for (twoFrameFlashes = 0; twoFrameFlashes != 12; twoFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(2);
  }
  SolSanctumLunaSolRooms_DrawCurrentStatueLights();
  Engine_EventWait(6);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 280);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG);
  Engine_EventRequestExit(3);
}

// Run the falling lights while Kraden retreats from the changed room.
static void SolSanctumLunaSolRooms_RunStatueLightSequence(void) {
  s32 flash;
  Engine_EventBegin();
  SolSanctumLunaSolRooms_PrepareStatueTransition();
  gSolSanctumLunaSolRooms_Light1Timer = 0;
  gSolSanctumLunaSolRooms_Light2Timer = 0;
  gSolSanctumLunaSolRooms_Light3Timer = 0;
  gSolSanctumLunaSolRooms_Light4Timer = 0;
  Engine_EventSetMessage(MsgSolSanctumWhatsHappening);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT, 0);
  Engine_ActorJump(ACTOR_KRADEN, 6, 30);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            20);
  for (flash = 0; flash != 4; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(12);
  }
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  gSolSanctumLunaSolRooms_Light1Timer =
      (((u32)Engine_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light2Timer =
      (((u32)Engine_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light3Timer =
      (((u32)Engine_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light4Timer =
      (((u32)Engine_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_FlashState = 0;
  // Run the falling lights independently while the statues keep flashing.
  Task_AddCallback(SolSanctumLunaSolRooms_UpdateFirstStatueLight, 3200);
  Task_AddCallback(SolSanctumLunaSolRooms_UpdateSecondStatueLight, 3200);
  Task_AddCallback(SolSanctumLunaSolRooms_UpdateThirdStatueLight, 3200);
  FieldScheduler_AddCallback(SolSanctumLunaSolRooms_UpdateFourthStatueLight,
                             3200);
  for (flash = 0; flash != 6; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(5);
  }
  for (flash = 0; flash != 8; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(4);
  }
  for (flash = 0; flash != 10; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(3);
  }
  for (flash = 0; flash != 12; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(2);
  }
  Map_CopyCellsTo(45, 30, 34, 10, 4, 2);
  Engine_ActorJump(ACTOR_KRADEN, 6, 40);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 280);
  // Stop all light callbacks before leaving this room.
  Engine_TaskRemoveCallback(SolSanctumLunaSolRooms_UpdateFirstStatueLight);
  Engine_TaskRemoveCallback(SolSanctumLunaSolRooms_UpdateSecondStatueLight);
  Engine_TaskRemoveCallback(SolSanctumLunaSolRooms_UpdateThirdStatueLight);
  Engine_TaskRemoveCallback(SolSanctumLunaSolRooms_UpdateFourthStatueLight);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(4);
}

// Reconcile the saved light flags and replay their accelerating flashes.
static void SolSanctumLunaSolRooms_AnimateSavedStatueLights(void) {
  s32 speaker;
  s32 twelveFrameFlashes;
  s32 eightFrameFlashes;
  s32 sixFrameFlashes;
  s32 fourFrameFlashes;
  s32 twoFrameFlashes;

  Engine_EventBegin();
  // Toggle the saved lights only where the current statues are lit.
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
    Map_CopyCellsTo(45, 28, 34, 10, 2, 1);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
    Map_CopyCellsTo(47, 28, 36, 10, 2, 1);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
    Map_CopyCellsTo(45, 29, 34, 11, 2, 1);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
    Map_CopyCellsTo(47, 29, 36, 11, 2, 1);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
  }
  SolSanctumLunaSolRooms_PrepareStatueTransition();
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Engine_ActorJump(ACTOR_KRADEN, 6, 30);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  for (twelveFrameFlashes = 0; twelveFrameFlashes != 4; twelveFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(12);
  }
  for (eightFrameFlashes = 0; eightFrameFlashes != 6; eightFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(8);
  }
  for (sixFrameFlashes = 0; sixFrameFlashes != 8; sixFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(6);
  }
  for (fourFrameFlashes = 0; fourFrameFlashes != 10; fourFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(4);
  }
  for (twoFrameFlashes = 0; twoFrameFlashes != 12; twoFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(2);
  }
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawSavedStatueLights();
  Engine_EventWait(6);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT) == 0) {
    speaker = SPEAKER_WINDOW_BELOW | ACTOR_KRADEN;
    Engine_EventSetMessage(MsgSolSanctumYouFoundIt);
    SolSanctumLunaSolRooms_ShowMessageAndWait(speaker, 6);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    SolSanctumLunaSolRooms_ShowMessageAndWait(speaker, 6);
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(5);
}

// Walking up to Kraden (trigger 8) once all four seals are open: "Look, Isaac!
// The picture of Luna..." has changed to Sol.
static void Event_KradenSeesLunaPictureChange(void) {
  struct FieldActor *leader;

  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0) {
  } else {
    if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() == 0) {
    } else {
      Engine_EventBegin();
      Actor_SetPosition(ACTOR_KRADEN, PIXELS(577), PIXELS(147));
      Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 1);
      Camera_MoveTo(PIXELS(574), -1, PIXELS(184), 1);
      // Reunite Isaac and Kraden to discuss the changed Luna picture.
      Engine_EventSetMessage(MsgSolSanctumThePictureOfLunaChanged);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 576, 232);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
      Engine_CameraWaitForMove();
      Engine_EventWait(10);
      Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 152);
      Engine_EventWait(6);
      Engine_ActorJump(ACTOR_KRADEN, 6, 30);
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_EventWait(2);
      Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
      Engine_EventWait(30);
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
      Engine_EventWait(6);
      Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
      Engine_EventWait(40);
      SolSanctumLunaSolRooms_ShowMessageAndWait(
          SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 6);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 208);
      Engine_EventWait(40);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_EventWait(6);
      Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 2, PIXELS(1) / 4);
      Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                             leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(ACTOR_KRADEN);
      Actor_SetPosition(ACTOR_KRADEN, 0, 0);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
      Engine_EventEnd();
    }
  }
}

// Show Kraden noticing that the floor symbol changed to Luna.
static void SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna(void) {
  Engine_AudioPlayCue(MUS_021);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 184);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(376), PIXELS(184));
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 392, 168);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_EventSetMessage(MsgSolSanctumLookSymbolFloor);
  Engine_ActorJump(ACTOR_KRADEN, 4, 30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(6);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Actor_WalkToAndWait(ACTOR_KRADEN, 376, 184);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(1608), PIXELS(1608));
  Engine_EventWait(4);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
}

// Searching the floor symbol (trigger 20) after Kraden has seen it change:
// the floor opens, and Isaac then Kraden rise into the Elemental Star room.
static void Event_RiseToElementalStarRoom(void) {
  struct FieldActor *actor;
  s32 frame;
  struct AnimationObject *sprite;

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) == 0) {
  } else {
    Engine_EventBegin();
    Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
    Camera_MoveTo(PIXELS(287), -1, PIXELS(148), 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 30);
    Actor_SetPosition(ACTOR_KRADEN, PIXELS(288), PIXELS(120));
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_KRADEN, 276, 136);
    Actor_WalkTo(ACTOR_KRADEN, 264, 136);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 136);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 20);
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
      Engine_AudioPlayCue(SE_220);
    }
    Engine_EventWait(40);
    // Reveal the passage only once; later visits reuse its open state.
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
    } else {
      Map_CopyCellsTo(36, 62, 17, 36, 2, 3);
      Map_CopyCellsTo(44, 59, 17, 38, 2, 1);
      Engine_EventWait(10);
      Map_CopyCellsTo(38, 62, 17, 36, 2, 3);
      Map_CopyCellsTo(44, 59, 17, 39, 2, 1);
      Engine_EventWait(10);
      Map_CopyCellsTo(40, 62, 17, 36, 2, 3);
      Map_CopyCellsTo(0, 32, 17, 39, 2, 1);
      Map_CopyCellsTo(44, 59, 17, 40, 2, 1);
      Engine_EventWait(10);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT, 0);
      Map_CopyCellsTo(42, 62, 17, 36, 2, 3);
      Map_CopyCellsTo(0, 32, 17, 40, 2, 1);
      Map_CopyCellsTo(44, 59, 17, 41, 2, 1);
      Engine_EventWait(10);
      Map_CopyCellsTo(0, 32, 17, 41, 2, 1);
      Map_CopyCellsTo(44, 59, 17, 42, 2, 1);
      Engine_EventWait(10);
      Map_CopyCellsTo(0, 32, 17, 42, 2, 3);
      Engine_EventWait(80);
      Map_ClearLayerEntryFlag(9);
      Map_ClearLayerEntryFlag(10);
      Engine_GameFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
    }
    Object_LinkPair(16, 0, 30);
    Engine_ActorSetAnimation(ACTOR_KRADEN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_EventSetMessage(MsgSolSanctumHa);
    SolSanctumLunaSolRooms_ShowMessageAndWait(
        SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 6);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
    Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(60);
    // Raise Isaac first, then shrink his sprite into the passage.
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    Engine_CameraMoveTo(PIXELS(287), -1, PIXELS(114), 1);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    /* FAKEMATCH: register choice only: the zero goes through the sprite
       variable, whose pseudo has r7, as native. Plain forms measured
       (2026-10-13): motionFlags = 0 puts the zero in r2 (differ 4), also
       with the actor held as union FieldObject and with FieldActor's
       coordinates as s32. */
    sprite = 0;
    actor->motionFlags = (u32)sprite;
    Engine_AudioPlayCue(SE_201);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0x100);
    sprite = actor->sprite;
    frame = 0;
    sprite->flags = 0;
    do {
      actor->y.fixed += PIXELS(1) / 5;
      Engine_TaskWait(1);
      frame++;
    } while (frame != 120);
    Engine_AudioPlayCue(SE_190);
    frame = 0;
    do {
      actor->y.fixed += PIXELS(1) / 10;
      sprite->scale += -Q16_ONE / 64;
      Engine_TaskWait(1);
      frame++;
    } while (frame != 60);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_ActorJump(ACTOR_KRADEN, 4, 20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 120);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
    // Repeat the ascent for Kraden before closing the room transition.
    Engine_ActorSetSpeed(ACTOR_KRADEN, PIXELS(3) / 10, PIXELS(3) / 20);
    actor = Object_GetById(ACTOR_KRADEN);
    /* FAKEMATCH: register choice and order only: the mask temporary
       gives the cleared flags r2 and the motion address r3, so both
       byte stores wait past the zero, as native. Plain forms measured
       (2026-10-13): &= ~ACTOR_FACES_MOTION with frame = 0 and the
       store before, after or chained puts the flags in r3, stored at
       once (differ 10); motionFlags = 0 before frame = 0, 23. */
    {
      s32 steeringFlags = 254;

      steeringFlags &= actor->steeringFlags;
      frame = 0;
      actor->steeringFlags = steeringFlags;
      actor->motionFlags = frame;
    }
    Engine_AudioPlayCue(SE_201);
    Actor_SetChildValue(ACTOR_KRADEN, 0x100);
    sprite = actor->sprite;
    sprite->flags = 0;
    do {
      actor->y.fixed += PIXELS(1) / 5;
      Engine_TaskWait(1);
      frame++;
    } while (frame != 120);
    Engine_AudioPlayCue(SE_190);
    frame = 0;
    do {
      actor->y.fixed += PIXELS(1) / 10;
      sprite->scale += -Q16_ONE / 64;
      Engine_TaskWait(1);
      frame++;
    } while (frame != 60);
    Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    Engine_EventWait(80);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    gEventWork->transitionFrames = 24;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    Engine_EventRequestExit(7);
    Engine_EventEnd();
  }
}

// Restore puzzle tiles and select the scene associated with this entrance.
s32 SolSanctumLunaSolRooms_Initialize(void) {
  s32 playedEntrySequence;
  s32 anyLightLit;

  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  playedEntrySequence = 0;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FOUND) != 0 &&
      Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) == 0 &&
      Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
    // Retreat does not work here (flag 0x144).
    Engine_GameFlagSet(0x144);
  }
  Engine_ColorBufferApplySource(PALETTE_TRANSFORM_PLAIN, 0);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
      Engine_ColorBufferApplyTarget(0x2051cc, 1);
      Engine_ColorBufferInterpolate(1);
      Engine_TaskWait(1);
    } else if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
      Engine_ColorBufferApplyTarget(0x202db1, 1);
      Engine_ColorBufferInterpolate(1);
      Engine_TaskWait(1);
    }
  } else {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) != 0) {
      Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(120));
    }
  }
  // Play the one-time trap or light sequence selected by this entrance.
  if (gGameState.entrance == 4) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG) == 0) {
      SolSanctumLunaSolRooms_SpringStatueTrap();
      Engine_GameFlagSet(FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG);
      playedEntrySequence = 1;
    }
  } else if (gGameState.entrance == 5) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0) {
      SolSanctumLunaSolRooms_RunStatueLightSequence();
      Engine_GameFlagSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN);
      Engine_GameFlagClear(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN);
      Engine_GameFlagClear(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN);
      Engine_GameFlagClear(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN);
      Engine_GameFlagClear(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN);
      playedEntrySequence = 1;
    }
  } else if (gGameState.entrance == 6) {
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) != 0) {
      SolSanctumLunaSolRooms_AnimateSavedStatueLights();
      Engine_GameFlagSet(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT);
    }
    playedEntrySequence = 1;
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
  }
  Engine_TaskWait(4);
  // Restore completed puzzle tiles without replaying the entrance scene.
  if (playedEntrySequence == 0) {
    if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
      Map_CopyCellsTo(30, 44, 30, 38, 12, 5);
      Map_CopyCellsTo(30, 44, 34, 37, 4, 1);
      Map_CopyCellsTo(14, 41, 32, 41, 8, 4);
      Map_CopyCellsTo(45, 28, 34, 10, 4, 2);
      Map_CopyCellsTo(45, 30, 16, 10, 4, 2);
      Map_CopyCellsTo(14, 45, 14, 41, 8, 4);
      if (gGameState.entrance != 8) {
        if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) == 0) {
          if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) !=
              0) {
            Map_CopyCellsTo(0, 32, 17, 39, 2, 1);
            Map_CopyCellsTo(42, 62, 17, 36, 2, 3);
            Map_CopyCellsTo(0, 32, 17, 40, 2, 1);
            Map_CopyCellsTo(0, 32, 17, 41, 2, 1);
            Map_CopyCellsTo(0, 32, 17, 42, 2, 3);
          } else {
            Map_CopyCellsTo(44, 59, 17, 37, 2, 6);
          }
          Map_ClearLayerEntryFlag(9);
          Map_ClearLayerEntryFlag(10);
        }
      }
      Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    } else {
      anyLightLit = 0;
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
        Map_CopyCellsTo(45, 28, 34, 10, 2, 1);
        Map_CopyCellsTo(45, 30, 16, 10, 2, 1);
        anyLightLit = 1;
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
        Map_CopyCellsTo(47, 28, 36, 10, 2, 1);
        Map_CopyCellsTo(47, 30, 18, 10, 2, 1);
        anyLightLit = 1;
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
        Map_CopyCellsTo(45, 29, 34, 11, 2, 1);
        Map_CopyCellsTo(45, 31, 16, 11, 2, 1);
        anyLightLit = 1;
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
        Map_CopyCellsTo(47, 29, 36, 11, 2, 1);
        Map_CopyCellsTo(47, 31, 18, 11, 2, 1);
        anyLightLit = 1;
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) != 0 ||
          anyLightLit != 0) {
        Map_CopyCellsTo(30, 43, 32, 40, 8, 3);
        Map_CopyCellsTo(30, 43, 33, 39, 8, 1);
        Map_CopyCellsTo(30, 43, 36, 38, 3, 3);
        Map_CopyCellsTo(36, 58, 32, 41, 8, 4);
      }
      Map_CopyCellAttributes(15, 6, 2, 1, 17, 6);
    }
  }
  // The eruption visit begins with the floating-eye scene only once.
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FLOATING_EYE_SEEN) == 0 &&
      gGameState.entrance == 8) {
    SolSanctumLunaSolRooms_ShowFloatingEye();
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_FLOATING_EYE_SEEN);
    Map_CopyCellAttributes(15, 6, 2, 1, 17, 6);
  } else {
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
      BattleFx_SetQueuedSoundAndPlay(SE_141);
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
      InitializeSceneRecordBuffer();
      Map_CopyCellAttributes(15, 6, 2, 1, 17, 6);
    }
  }
  return 0;
}

static s32 SolSanctumLunaSolRooms_AreAllStatueLightsLit(void) {
  s32 allLit = 1;

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0)
    allLit = 0;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) == 0)
    allLit = 0;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) == 0)
    allLit = 0;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) == 0)
    allLit = 0;

  return allLit;
}

// Advance the active dialogue and pause for the requested frames.
static void SolSanctumLunaSolRooms_ShowMessageAndWait(s32 speaker,
                                                      s32 waitFrames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(waitFrames);
}

// Entering after the eruption (entrance 8) the first time: in a white flash a
// floating eye shows itself to Isaac and Garet, and the hall shakes.
static void SolSanctumLunaSolRooms_ShowFloatingEye(void) {
  struct FieldActor *leader;
  struct BattleEffectBuffers *palette;

  Engine_EventBegin();
  Camera_MoveTo(PIXELS(286), -1, PIXELS(134), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  ColorBuffer_ApplySource(0x7fff, 0);
  Engine_ColorBufferApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  Actor_SetPosition(GARET, PIXELS(280), PIXELS(134));
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  gEventWork->transitionFrames = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ColorBufferApplyTarget(0x2051cc, 1);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(120);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_02);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(100);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_ActorSetAttachedEffect(GARET, 0x102);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST + FACING_STEP, 40);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 60);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 20);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  // Let Isaac and Garet react to the eye before the stronger tremor.
  Engine_EventSetMessage(MsgSolSanctumFloatingEyeThing);
  Engine_EventOpenMessage(1, 0);
  Engine_EventChooseYesNo(0, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_107);
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  Engine_EventWait(40);
  InitializeSceneRecordBuffer();
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorJump(GARET, 4, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 300, 130);
  Engine_ActorSetDestination(GARET, 266, 144);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(40);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  // Recover from the tremor and bring Garet back into the party.
  BattleFx_SetQueuedSoundAndPlay(SE_141);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  SolSanctumLunaSolRooms_ShowMessageAndWait(GARET, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetPosition(GARET, 0, 0);
  // UB: EventGlobals traverses beyond scalar gEventWork to slot 32. Its
  // named load keeps native EN size 0x310 but changes address setup.
#ifdef UBFIX
  palette = gPaletteBufferWork;
#else
  palette =
      (struct BattleEffectBuffers *)((struct EventGlobals *)&gEventWork)->field;
#endif
  palette->target[0] = 0;
  palette->target[1] = 0;
  palette->target[2] = 0;
  palette->page = 0;
  palette->duration = 1;
  palette->step = 1;
  palette->padding2a03 = 1;
  Io_Write16(0, &PLTT_BACKDROP);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gEventWork->transitionFrames = 16;
  Engine_EventEnd();
}

// Talking to Kraden: "Let me know what you find" once Isaac agreed to search,
// otherwise "Just wait over there."
static void Event_TalkToKraden(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN) != 0) {
    Engine_EventSetMessage(MsgSolSanctumKradenLetMeKnowWhat);
  } else {
    Engine_EventSetMessage(MsgSolSanctumKradenJustWaitOverThere);
  }
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
  Engine_EventEnd();
}

struct SceneEntrance gSolSanctumLunaSolRoomsEntrances[] = {
    {0, CONDITION_ALWAYS, 288, 0, 157, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {1, CONDITION_ALWAYS, 55, 0, 477, FACING_NORTH, 0, 0, 0, 752, 496, 0},
    {2, CONDITION_ALWAYS, 647, 0, 327, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {3, CONDITION_ALWAYS, 287, 0, 218, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {4, CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {5, CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {6, CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {7, CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {8, CONDITION_ALWAYS, 297, 0, 119, FACING_NORTHWEST, 0, 0, 0, 752, 496, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumLunaSolRoomsExits[] = {
    SCENE_EXITS(11),      SCENE_EXIT(1, 16, 16), SCENE_EXIT(2, 12, 1),
    SCENE_EXIT(3, 12, 3), SCENE_EXIT(4, 12, 4),  SCENE_EXIT(5, 12, 5),
    SCENE_EXIT(6, 12, 6), SCENE_EXIT(7, 17, 10), SCENE_EXIT(10, 510, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSolSanctumLunaSolRoomsPlacements[] = {
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gSolSanctumLunaSolRoomsEvents[] = {
    {EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToKraden)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_UpdateRoomTintInverse)},
    {EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UpdateRoomTint)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RestorePlainTint)},
    {EVENT_TOUCH, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ApplySecondRoomTint)},
    {EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearBackdropColor)},
    {SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_RiseToElementalStarRoom)},
    {EVENT_TOUCH, 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_KradenSeesLunaPictureChange)},
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_KradenFindsHall)},
    {EVENT_TOUCH, 9, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_KradenStopsPartyLeaving)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_SOL_SANCTUM_HERB_FOUND,
     FIND_ITEM(ITEM_HERB)},
    {SCENE_EVENTS_END},
};
