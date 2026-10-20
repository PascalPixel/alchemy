/// @file
/// The south-west of Vale around Isaac's house, through the stormy night, the
/// party's departure and its return; entrance 19 is instead a room of Babi's
/// scholars, and entrance 22 shows the Mythril Bag.

#include "game_state.h"
#include "vram_tab.h"
#include "unit/scene/vale_babi.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "facing_object.h"
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
#include "map_scroll.h"
#include "field_event.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "glyph.h"
#include "animspr.h"
#include "vram_block.h"
#include "b5_context.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "heap_state.h"
#include "ability_ids.h"
#include "songs.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "field_obj.h"
#include "flags.h"
#include "fixed_math.h"
#include "fx_scene.h"

// The arrival scenes are defined after the event handlers.
static void Event_DoraWakesIsaac(void);
static void Event_HurryToThePlaza(void);
static void ValeBabi_RestoreRoomPatch(void);
static void Event_ShowMythrilBag(void);
static void ValeBabi_SpawnEffectPair(union FieldObject *object);
static void ValeBabi_DemonstrateCatch(void);

// Shared facing code retains its position before the overlay entry points.
#include "common/object_facing.h"

/// Vale's placed actors, numbered in the order of gValeBabi_Placements (the
/// scholars of entrance 19 are numbered from 8 in their own table).
enum ValeBabiActor {
  /// Isaac's mother. "Isaac, wake up!"
  ACTOR_DORA = 8,
  /// Isaac's father (sprite 0x35, Kyle in vale_storm.c). "Isaac, Dora, hurry!
  /// The Boulder could fall at any second!"
  ACTOR_KYLE = 9,
  ACTOR_INNKEEPER = 13,
  /// A neighbour who tells Isaac to be off, and who knows of Dora's illness
  /// once the party is back from Kalay. "Hey... Boy..."
  ACTOR_NEIGHBOR = 16,
  /// Kraden's sprite (0x1e, as in Sol Sanctum's scenes), seen only from
  /// entrance 22.
  ACTOR_KRADEN = 18
};

/// Vale's place flags this scene keeps while the party is in Vale.
enum ValeBabiPlaceFlag {
  /// Back from Kalay, the innkeeper welcomed Isaac home.
  FLAG_VALE_INNKEEPER_WELCOMED_ISAAC = 0x300,
  /// Back from Kalay, the neighbour remarked how much Isaac has grown.
  FLAG_VALE_NEIGHBOR_SAW_ISAAC_GROWN = 0x301
};

// FLAG_SCENE_TEMP_03: Dora got out of her sickbed on this visit.

/// Returns the entrances, among them the scenes played on arrival: 19 from
/// Babi's palace (its exit 11), 20 where a new game begins (menu/clear.c, and
/// ValeHome's exit 50), 21 from this scene's own exit 20, and 22 from Kraden's
/// cottage (ValeKraden's exit 13).
const struct SceneEntrance *ValeBabi_GetEntrances(void) {
  return gValeBabi_Entrances;
}

const struct SceneRegion *ValeBabi_GetRegions(void) { return NULL; }

/// On the stormy night the exits lead into the storm.
const u32 *ValeBabi_GetExits(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeBabi_StormExits;
  }
  return gValeBabi_Exits;
}

/// Places Babi's scholars, or Vale's villagers as the story has left them.
const struct ScenePlacement *ValeBabi_GetPlacements(void) {
  struct ScenePlacement *table;

  if (gGameState.entrance == 19)
    return gValeBabi_ScholarsPlacements;
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    table = gValeBabi_PlacementsAfterKalay;
  else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    table = gValeBabi_PlacementsAfterLeaving;
  else
    table = gValeBabi_Placements;
  FieldScene_PrepareActors(table);
  return table;
}

/// A scholar asks whether the party knows Kraden.
/// Reached by talking to scholar 9 in Babi's scholars' room (entrance 19) once
/// FLAG_COLOSSO_OVER is set; speakerId is the actor spoken to.
static void Event_ScholarAsksAboutKraden(s32 speakerId) {
  s32 firstMessage = MsgValeFolksSeemKnow;

  Engine_EventSetMessage(firstMessage);
  Engine_EventOpenMessage(speakerId, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(firstMessage + 1);
  } else {
    Engine_EventSetMessage(firstMessage + 2);
  }
  Engine_EventShowMessage(speakerId, 0);
}

/// Chooses the events: Babi's scholars talk differently once the Colosso
/// finals are over; Vale's villagers follow the storm and the story.
const struct SceneEvent *ValeBabi_GetEvents(void) {
  if (gGameState.entrance == 19) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gValeBabi_ScholarsLaterEvents;
    }
    return gValeBabi_ScholarsEvents;
  }

  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeBabi_StormEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    return gValeBabi_EventsAfterKalay;
  }
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    return gValeBabi_EventsAfterLeaving;
  }
  return gValeBabi_Events;
}

/// The neighbour asks whether Isaac has business with him, and sends him off
/// either way. "Hey... Boy..."
/// Reached by talking to the neighbour (actor 16) before the party leaves
/// Vale (gValeBabi_Events).
static void Event_AskIsaacHisBusiness(void) {
  Engine_EventBegin();
  Engine_ActorStartRepeatedMotion(ACTOR_NEIGHBOR, 2);
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgValeHeyBoy);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_NEIGHBOR, 10);
  Event_ShowMessageAndWait(ACTOR_NEIGHBOR, 0, 6);
  Actor_ShowEmote(ACTOR_NEIGHBOR, EMOTE_IN_FRONT | 2, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_NEIGHBOR, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_NEIGHBOR, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_OpenMessage(ACTOR_NEIGHBOR, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Engine_ActorStartRepeatedMotion(ACTOR_NEIGHBOR, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_NEIGHBOR, 0, 4);
  Engine_EventEnd();
}

/// A villager asks whether the masked man was Felix, and wonders whether
/// Jenna can be trusted.
/// Reached by talking to actor 14 once the party has left Vale and until it
/// reaches Kalay (gValeBabi_EventsAfterLeaving).
static void Event_AskWhetherMaskedManWasFelix(void) {
  struct EventWork *work;

  Engine_EventBegin();
  Engine_ActorFaceActor(0xE, ACTOR_PARTY_LEADER, 0xA);
  Engine_EventSetMessage(MsgValeTheMaskedManWasFelix);
  Engine_EventOpenMessage(0xE, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventShowMessage(0xE, 0);
  } else {
    work = gEventWork;
    work->message = (work->message + 1);
    Engine_EventAskYesNo(0xE, 0);
  }
  Engine_EventEnd();
}

/// Stops storm lightning before fading through the requested exit.
/// Called by the exit events below.
static void ValeBabi_CloseScreenAndExit(s32 exitId) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    FieldFx_StopLightning();
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(exitId);
}

// The exit events: stepping on the cell code of the same number in any of the
// Vale tables plays a step sound and takes that exit (gValeBabi_Exits, or
// gValeBabi_StormExits on the stormy night).

/// Reached by cell code 1; takes exit 1, to ValeHome's entrance 6.
static void Event_ExitNorthwestArea(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(1);
}

/// Reached by cell code 2; takes exit 2, to Kraden's cottage (ValeKraden's
/// entrance 3).
static void Event_ExitNorthArea(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(2);
}

/// Reached by cell code 3; takes exit 3, to ValeHome's entrance 2.
static void Event_ExitNortheastArea(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(3);
}

/// Reached by cell code 4; takes exit 4, to ValeHome's entrance 7.
static void Event_ExitWestAreaSouth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(4);
}

/// Reached by cell code 5; takes exit 5, to this scene's entrance 8.
static void Event_ExitWestAreaNorth(void) {
  Engine_AudioPlayCue(SE_128);
  ValeBabi_CloseScreenAndExit(5);
}

/// Reached by cell code 6; takes exit 6, to Vale's entrance 8, or on the
/// stormy night to ValeStorm's.
static void Event_ExitCenterAreaSouth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(6);
}

/// Reached by cell code 7; takes exit 7, to this scene's entrance 9.
static void Event_ExitCenterAreaNorth(void) {
  Engine_AudioPlayCue(SE_128);
  ValeBabi_CloseScreenAndExit(7);
}

/// Reached by cell code 8; takes exit 8, to this scene's entrance 5.
static void Event_ExitSouthwestArea(void) {
  Engine_AudioPlayCue(SE_129);
  ValeBabi_CloseScreenAndExit(8);
}

/// Reached by cell code 9; takes exit 9, to this scene's entrance 7.
static void Event_ExitSouthAreaWest(void) {
  Engine_AudioPlayCue(SE_129);
  ValeBabi_CloseScreenAndExit(9);
}

/// Reached by cell code 10; takes exit 10, to ValeStorm's entrance 12.
static void Event_ExitSouthAreaSouth(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(10);
}

/// Reached by cell code 11; takes exit 11, to ValeStorm's entrance 13.
static void Event_ExitSoutheastArea(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(11);
}

/// Sets up the stormy night or the villagers of the story, and plays the
/// scenes of entrances 20 to 22.
/// Called by Field_RunScene once the actors are placed. Entrance 20 sets
/// FLAG_VALE_STORM before Event_DoraWakesIsaac; back from Kalay, Dora is put
/// back in her sickbed unless FLAG_SCENE_TEMP_03 says she got up.
s32 ValeBabi_Initialize(void) {
  struct FieldActor *actor;

  if (gGameState.entrance == 19) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
      Engine_ActorSetPosition(11, 0, 0);
      Engine_ActorSetPosition(12, 0, 0);
      Engine_ActorSetPosition(13, 0, 0);
      Engine_ActorSetPosition(14, 0, 0);
      Engine_ActorSetPosition(15, 0, 0);
      Engine_ActorSetPosition(ACTOR_NEIGHBOR, 0, 0);
    } else {
      ValeBabi_RestoreRoomPatch();
    }
    Engine_ActorSetSpritePriority(13, 1);
    if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
      actor = Object_GetById(17);
      Engine_ActorSetSpriteFlags(actor, 0);
      if (gGameState.entrance == 6 || gGameState.entrance == 7) {
        if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
          if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_03) != 0) {
            Map_ClearLayerEntryFlag(12);
          }
        } else {
          Map_ClearLayerEntryFlag(11);
          actor = Object_GetById(ACTOR_DORA);
          Engine_ActorSetSpriteFlags(actor, 0);
          Engine_ActorSetAnimation(ACTOR_DORA, 10);
        }
      }
    } else {
      if (gGameState.entrance == 21) {
        Event_HurryToThePlaza();
      } else {
        if (gGameState.entrance == 20) {
          Engine_GameFlagSet(FLAG_VALE_STORM);
          Event_DoraWakesIsaac();
        } else {
          if (gGameState.entrance == 22) {
            Event_ShowMythrilBag();
          } else {
            ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
                SCENE_TRANSITION(TRANSITION_WINDOW, 9);
            if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
              FieldFx_StartLightning();
              ((struct FieldBlendWork *)WORK_SLOT(30))->loud = 1;
              FieldFx_LastLightning();
              Engine_TaskWait(30);
              Engine_EventOpenScreen();
              Engine_EventWaitForScreen();
              FieldFx_ResumeLightning();
            } else {
              Engine_MapRedraw();
              Engine_TaskWait(1);
            }
          }
        }
      }
    }
  }
  return 0;
}

/// Dora wakes Isaac on the stormy night, makes him take his tunic, and leads
/// him out into the storm. "The Mt. Aleph Boulder is going to fall!"
/// Called by ValeBabi_Initialize on arrival by entrance 20, where a new game
/// begins. Sets FLAG_ISAAC_WEARS_TUNIC and takes exit 20, back into this
/// scene by entrance 21, where Event_HurryToThePlaza follows.
static void Event_DoraWakesIsaac(void) {
  s32 firstMessage;
  struct AnimationObject *sprite;
  struct MapScrollWork *mapWork;
  struct FieldActor *viewCenter;
  struct EventWork **eventSlot;

  eventSlot = (struct EventWork **)Ram_EventWork;
  /* FAKEMATCH(temporary): the event record read into a block-local before
     the map work and view centre; as reads in place, 11 of 430 instructions
     differ in tbs-en (13 with the view centre first). */
  {
    struct EventWork *event = *eventSlot;

    mapWork = *(struct MapScrollWork **)Ram_MapWork;
    viewCenter = event->viewCenter;
  }
  sprite = Object_GetById(17)->sprite;
  // Present the bedside conversation before revealing the wider storm scene.
  Engine_EventBegin();
  Engine_ActorSetPosition(11, 0, 0);
  Engine_ActorSetPosition(12, 0, 0);
  Engine_ActorSetPosition(13, 0, 0);
  Engine_ActorSetPosition(14, 0, 0);
  Engine_ActorSetPosition(15, 0, 0);
  Engine_ActorSetPosition(ACTOR_NEIGHBOR, 0, 0);
  // Isaac lies asleep in bed (animation 18); actor 17 is set beside him.
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 18);
  sprite->rotation = 1365;
  Object_GetById(17)->motionFlags = 0;
  Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
  Engine_ActorSetPosition(17, PIXELS(576), PIXELS(650));
  Map_ClearLayerEntryFlag(7);
  Engine_ActorSetPosition(ACTOR_DORA, PIXELS(534), PIXELS(688));
  ObjectSystem_SuspendProjectionAndShowSprites();
  // On a dark screen, Dora's voice: "Isaac, wake up!" "Please, dear, wake up!"
  Ui_SetMessageVoiceFromActor(ACTOR_DORA);
  firstMessage = MsgValeWake;
  UiText_ShowCenteredMessage(firstMessage, 1, 0);
  Engine_EventWait(40);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Ui_SetMessageVoiceFromActor(ACTOR_DORA);
  UiText_ShowCenteredMessage(firstMessage + 1, 1, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();
  Engine_EventWait(40);
  // Widen the camera bounds and reveal the lightning outside.
  mapWork->minX = PIXELS(328);
  mapWork->minY = PIXELS(600);
  mapWork->maxX = PIXELS(624);
  mapWork->maxY = PIXELS(816);
  viewCenter->x.fixed = PIXELS(564);
  viewCenter->y.fixed = 0;
  viewCenter->z.fixed = PIXELS(691);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  (*eventSlot)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  (*eventSlot)->transitionFrames = 64;
  FieldFx_StartLightning();
  (*(struct FieldBlendWork **)((u8 *)eventSlot + Ram_SceneMapStateOffset))
      ->loud = 1;
  FieldFx_LastLightning();
  Engine_TaskWait(30);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FieldFx_ResumeLightning();
  // "The Mt. Aleph Boulder is going to fall!" "Come on, Isaac. We have to go
  // now!"
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(firstMessage + 2);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  // Isaac gets out of bed (animation 19) and jumps down beside Dora.
  Map_SetLayerEntryFlag(7);
  Engine_EventWait(20);
  Map_ClearLayerEntryFlag(8);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, Q16_ONE, 0x8000);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 557, 679);
  Map_SetLayerEntryFlag(8);
  Map_ClearLayerEntryFlag(9);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 555, 680);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 543, 674);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
  // "Isaac! You forgot something!" Catch's effect plays on Dora.
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_DORA,
                    0);
  ValeBabi_DemonstrateCatch();
  // Resolve the Psynergy demonstration before the pair walks away together.
  // "It's pouring outside! Don't go out without your tunic!" She steps up to
  // him and he puts it on, with a flash and SE_RECOVERY.
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 20);
  Object_GetById(ACTOR_DORA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_DORA, 542, 680);
  Engine_EventWait(1);
  Object_GetById(ACTOR_DORA)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  ObjectDispatch_RegisterChildMetadata(
      (struct DispatchObject *)Object_GetById(ACTOR_PARTY_LEADER), 226);
  Engine_GameFlagSet(FLAG_ISAAC_WEARS_TUNIC);
  Engine_AudioPlayCue(SE_RECOVERY);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 7);
  Engine_EventWait(10);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Object_GetById(ACTOR_DORA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_DORA, 534, 688);
  Engine_EventWait(1);
  Object_GetById(ACTOR_DORA)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_DORA, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
  Engine_CameraFollowActor(ACTOR_DORA, 1);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= 0x1;
  Actor_SetActionScript(ACTOR_DORA, gValeBabi_LeaveHouseScript);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeBabi_LeaveHouseScript);
  Object_RefreshSelectorById(ACTOR_DORA);
  Actor_WalkToAndWait(ACTOR_DORA, 419, 661);
  Actor_WalkToAndWait(ACTOR_DORA, 408, 661);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 10);
  // "Have you got everything you need?" Yes: "Good for you, Isaac."; no: "I'm
  // sorry, but we don't have time..."
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    (*eventSlot)->message++;
  }
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_DORA, gValeBabi_DoraLeavesScript);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeBabi_IsaacLeavesScript);
  Engine_EventWait(20);
  (*eventSlot)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  (*eventSlot)->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(20);
}

/// Selects the hardware blend targets for the storm sequence.
static __inline__ void SetBlendTarget(u32 value) { REG_BLDCNT = value; }

static __inline__ void SetBlendAlpha(u32 value) { REG_BLDALPHA = value; }

/// Kyle calls to Isaac and Dora to hurry; actor 10 shakes, falls and
/// fades as they run to the plaza.
/// Called by ValeBabi_Initialize on arrival by entrance 21, after
/// Event_DoraWakesIsaac. Takes exit 21, to ValeStorm's entrance 15, where
/// Event_DoraSendsIsaacToThePlaza follows.
static void Event_HurryToThePlaza(void) {
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  u32 i;
  volatile u16 *alpha;

  actor = Object_GetById(10);
  sprite = actor->sprite;
  Engine_EventBegin();
  // The villagers stay out of sight; actor 10 starts shaking on the slope.
  Engine_ActorSetPosition(11, 0, 0);
  Engine_ActorSetPosition(12, 0, 0);
  Engine_ActorSetPosition(13, 0, 0);
  Engine_ActorSetPosition(14, 0, 0);
  Engine_ActorSetPosition(15, 0, 0);
  Engine_ActorSetPosition(ACTOR_NEIGHBOR, 0, 0);
  Actor_SetPosition(ACTOR_DORA, PIXELS(431), PIXELS(391));
  Engine_ActorSetPosition(10, PIXELS(463), PIXELS(404));
  Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->motionFlags = 0;
  sprite->image.priority = 1;
  Actor_SetActionScript(10, gValeBabi_Actor10ShakesScript);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Map_CopyCellsTo(83, 15, 83, 19, 5, 4);
  Map_CopyCellsTo(90, 16, 90, 20, 5, 4);
  Map_CopyCellsTo(77, 23, 82, 23, 5, 7);
  Map_CopyCellsTo(83, 33, 85, 33, 2, 2);
  Map_CopyCellsTo(91, 28, 90, 28, 1, 1);
  Map_CopyCellsTo(91, 28, 88, 30, 1, 1);
  Map_CopyCellsTo(94, 27, 94, 23, 6, 4);
  Map_CopyCellsTo(92, 28, 87, 23, 4, 4);
  Map_CopyCellsTo(65, 53, 88, 24, 2, 2);
  DisplayBlend_SuspendScript();
  SetBlendTarget(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  Io_Write16(BLEND_ALPHA(12, 16), &REG_BLDALPHA);
  FieldFx_StartLightning();
  ((struct FieldBlendWork *)WORK_SLOT(30))->loud = 1;
  FieldFx_LastLightning();
  Engine_TaskWait(30);
  // Dora and Isaac run out of the house (SE_DOOR_OPEN).
  Engine_CameraFollowActor(ACTOR_DORA, 1);
  Actor_SetSpeed(ACTOR_DORA, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_KYLE, 0x18000, 0xc000);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeBabi_IsaacRunsAheadScript);
  Actor_SetActionScript(ACTOR_DORA, gValeBabi_DoraRunsAheadScript);
  Engine_EventOpenScreen();
  Object_RefreshSelectorById(ACTOR_DORA);
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 0, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 10);
  // The camera finds Kyle coming up the path: "Isaac, Dora, hurry! The
  // Boulder could fall at any second!"
  Camera_SetSpeed(PIXELS(4), 0x8000);
  Camera_MoveTo(PIXELS(414), -1, PIXELS(530), 1);
  Actor_SetPosition(ACTOR_KYLE, PIXELS(414), PIXELS(530));
  Engine_ActorWalkToAndWait(ACTOR_KYLE, 427, 483);
  Engine_CameraWaitForMove();
  Engine_EventSetMessage(MsgValeDoraHurryBoulder);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
  Engine_CameraSetSpeed(0x18000, 0x3000);
  Engine_CameraMoveTo(PIXELS(480), -1, PIXELS(444), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  // They nod, Kyle runs back down, and the two turn back (SE_234).
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 20);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_KYLE, ANIM_NOD);
  Engine_ActorWalkTo(ACTOR_KYLE, 415, 589);
  Engine_EventWait(10);
  Actor_SetActionScript(ACTOR_DORA, gValeBabi_DoraRunsBackScript);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeBabi_IsaacRunsBackScript);
  Engine_AudioPlayCue(SE_234);
  Engine_EventWait(20);
  // Actor 10 drops (gValeBabi_Actor10FallsScript) and lands with SE_202.
  Actor_SetActionScript(10, gValeBabi_Actor10FallsScript);
  alpha = &REG_BLDALPHA;

  // Bring the boulder fully into view before fading it away.
  i = 0;
ramp:
  *alpha = 0x100e + i;
  Engine_TaskWait(1);
  if (++i <= 3)
    goto ramp;
  Engine_AudioPlayCue(SE_202);
  Engine_TaskWait(10);
  for (i = 0; i <= 15; i++) {
    SetBlendAlpha(0x100f - i);
    Engine_TaskWait(1);
  }
  // Dora and Isaac look back, startled, then nod to each other.
  Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_STAND);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
  Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_EventWait(80);
  Engine_ActorSetPosition(ACTOR_KYLE, 0, 0);
  Engine_ActorFaceEachOther(ACTOR_DORA, ACTOR_PARTY_LEADER, 20);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  Engine_CameraSetSpeed(0xcccc, 0x1999);
  Engine_CameraFollowActor(ACTOR_DORA, 1);
  Actor_SetActionScript(ACTOR_DORA, gValeBabi_RunToPlazaScript);
  Actor_RunActionScriptAndWait(ACTOR_PARTY_LEADER, gValeBabi_RunToPlazaScript);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
      SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->transitionFrames = 32;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(21);
}

/// The innkeeper offers a room from behind the counter; elsewhere talks of the
/// travelers, the eruption, or, back from Kalay, welcomes Isaac home.
/// Reached by talking to the innkeeper (actor 13) in every Vale table but the
/// storm's, and back from Kalay by Mind Read on him until he has welcomed
/// Isaac. Sets FLAG_VALE_INNKEEPER_WELCOMED_ISAAC on that welcome.
static void Event_TalkToInnkeeper(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if ((u32)(actor->facing + -0x2000) > 0x9000) {
    Engine_InnOpen(0, ACTOR_INNKEEPER);
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_REACHED_KALAY) != 0) {
      Engine_ActorRunRepeatedMotion(ACTOR_INNKEEPER, 2);
      Actor_FaceActor(ACTOR_INNKEEPER, ACTOR_PARTY_LEADER, 10);
      if (GameFlag_IsSet(FLAG_VALE_INNKEEPER_WELCOMED_ISAAC) == 0) {
        Engine_EventSetMessage(MsgValeCameBack2);
        Event_ShowMessage(ACTOR_INNKEEPER, 0);
        GameFlag_Set(FLAG_VALE_INNKEEPER_WELCOMED_ISAAC);
      }
      Engine_EventSetMessage(MsgValeHomeJustToStay);
      Event_AskYesNo(ACTOR_INNKEEPER, 0);
      Actor_FaceDirection(ACTOR_INNKEEPER, FACING_WEST + FACING_STEP, 10);
    } else {
      if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage(MsgValeTheVisitorsCausedTheEruption);
      } else {
        Engine_EventSetMessage(MsgValeTheThreeTravelersSeemOdd);
      }
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    }
    Engine_EventEnd();
  }
}

/// Back from Kalay, the neighbour remarks how much Isaac has grown.
/// Reached by talking to the neighbour (actor 16) while
/// FLAG_VALE_INNKEEPER_WELCOMED_ISAAC is clear, or by Mind Read on him until
/// FLAG_VALE_NEIGHBOR_SAW_ISAAC_GROWN is set, which this sets. XXX: the talk
/// row tests the innkeeper's flag, not this one; likely a slip, because the
/// Mind Read row tests this one, but both show the same line.
static void Event_NeighborSeesIsaacHasGrown(void) {
  Engine_EventBegin();
  Engine_ActorFaceActor(ACTOR_NEIGHBOR, ACTOR_PARTY_LEADER, 10);
  Engine_EventSetMessage(MsgValeYouveGrownSoMuch);
  Engine_EventShowMessage(ACTOR_NEIGHBOR, 0);
  Engine_ActorFaceDirection(ACTOR_NEIGHBOR, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_GameFlagSet(FLAG_VALE_NEIGHBOR_SAW_ISAAC_GROWN);
  Engine_EventEnd();
}

/// Mind Read on the innkeeper: Dora would not let Isaac stay in Vale.
/// Reached back from Kalay once he has welcomed Isaac. Sets
/// FLAG_READ_THOUGHTS_ABOUT_DORA.
static void Event_ReadInnkeepersThoughts(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDoraWouldntLetHimStay);
  Engine_EventShowMessage(ACTOR_INNKEEPER, 0);
  Engine_GameFlagSet(FLAG_READ_THOUGHTS_ABOUT_DORA);
  Engine_EventEnd();
}

/// Mind Read on the neighbour: Dora was struck with illness.
/// Reached back from Kalay once he has remarked on Isaac. Sets
/// FLAG_READ_THOUGHTS_ABOUT_DORA.
static void Event_ReadThoughtsAboutDorasIllness(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDoraWasStruckWithIllness);
  Engine_EventShowMessage(ACTOR_NEIGHBOR, 0);
  Engine_GameFlagSet(FLAG_READ_THOUGHTS_ABOUT_DORA);
  Engine_EventEnd();
}

/// Back from Kalay, Isaac visits Dora in her sickbed; she asks who told him she
/// was ill, then gets up to show she feels well.
/// Reached by talking to Dora (actor 8), or by Mind Read on her, until
/// FLAG_VISITED_SICK_DORA is set. Sets it and FLAG_SCENE_TEMP_03.
static void Event_VisitSickDora(void) {
  struct FieldActor *leader;
  s32 firstMessage;

  Engine_EventBegin();
  // Isaac steps up to the bed. "Cough, cough! Who is it? Is it the innkeeper?"
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 569, 393);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventSetMessage(MsgValeCoughCoughWho);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 1);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(80);
  Actor_SetSpeed(ACTOR_DORA, 0xcccc, 0x6666);
  // Bring Dora out of bed before asking about her illness.
  Engine_ActorSetDestination(ACTOR_DORA, 584, 406);
  Map_SetLayerEntryFlag(11);
  Map_ClearLayerEntryFlag(12);
  Engine_ActorSetAnimation(ACTOR_DORA, 12);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(40);
  // "Isaac..." "Are you finally home? Have you completed your mission!?"
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 40);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 8, 40);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 13);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 3, 0);
  Engine_ActorSetAnimation(ACTOR_DORA, 11);
  Engine_EventWait(40);
  // "You haven't, have you?" "Have you forgotten your promise, Isaac?"
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 12);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorSetAnimation(ACTOR_DORA, 13);
  // "You promised you wouldn't come back until your quest was complete,
  // right?" Either reply has its own answer; she is less surprised when Mind
  // Read has told Isaac of her illness.
  Engine_EventOpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  if (GameFlag_IsSet(FLAG_READ_THOUGHTS_ABOUT_DORA) != 0) {
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 2, 60);
  }
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_DORA, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 7, 60);
  // Branch on Isaac's replies and whether Mind Read revealed her illness.
  firstMessage = MsgValeWhoToldIll;
  Engine_EventSetMessage(firstMessage);
  Engine_EventOpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  if (Engine_GameFlagIsSet(FLAG_READ_THOUGHTS_ABOUT_DORA) != 0) {
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 2, 60);
  }
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_DORA, 0);
  Engine_ActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 7, 60);
  // "You can't get out of your promises just because you're worried about
  // me." "I just caught a cold." "But I'm fine now. Look!"
  Engine_EventSetMessage(firstMessage + 3);
  Engine_EventShowMessage(ACTOR_DORA, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 13);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(40);
  // Make room for Dora to walk and demonstrate that she feels well.
  // Isaac steps back as Dora gets up, turns about and jumps three times.
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  leader->facing = 0;
  Engine_TaskWait(1);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 558, 388);
  Actor_SetSpeed(ACTOR_DORA, 0x13333, 0x9999);
  Engine_ActorSetAnimation(ACTOR_DORA, 14);
  Engine_ActorMoveToAndWait(ACTOR_DORA, 586, 400);
  Engine_EventWait(40);
  Actor_WalkToAndWait(ACTOR_DORA, 580, 382);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_NORTH, 8);
  Engine_ActorFaceDirection(ACTOR_DORA, FACING_EAST, 8);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 8);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 10);
  Engine_ActorJump(ACTOR_DORA, 4, 20);
  Engine_ActorJump(ACTOR_DORA, 6, 40);
  Engine_ActorJump(ACTOR_DORA, 4, 20);
  // "See? I'm fine!" "Don't worry about me. Go on."
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 40);
  Actor_SetSpeed(ACTOR_DORA, 0x6666, 0x3333);
  Actor_WalkToAndWait(ACTOR_DORA, 572, 384);
  Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_GameFlagSet(FLAG_VISITED_SICK_DORA);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_03);
  Engine_EventEnd();
}

/// Talking to Dora: in her sickbed she murmurs Kyle's name; once up, she tells
/// Isaac not to worry.
/// Reached by talking to her back from Kalay once FLAG_VISITED_SICK_DORA is
/// set; she is up while FLAG_SCENE_TEMP_03 is set.
static void Event_TalkToDora(void) {
  s32 message;

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_03) != 0) {
    Object_SetTargetAndCallback(ACTOR_DORA,
                                ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                gValeBabi_DoraFacesIsaacScript);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeDontWorryBest);
    Engine_EventShowMessage(ACTOR_DORA, 0);
  } else {
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(40);
    message = MsgValeUnnOhhKyle;
    Engine_EventSetMessage(message);
    Engine_EventShowMessageAndWait(ACTOR_DORA, 0, 40);
    Engine_MessageShowCentered((message + 1), 1);
  }
  Engine_EventEnd();
}

/// Restores the room's tiles and collision patch outside the storm sequence.
/// Called by ValeBabi_Initialize when FLAG_VALE_STORM is clear.
static void ValeBabi_RestoreRoomPatch(void) {
  Map_CopyCellsTo(22, 85, 25, 85, 2, 2);
  Map_CopyCellAttributes(25, 15, 2, 2, 25, 25);
}

/// The Mythril Bag rises and wobbles before Kraden, with no text.
/// Called by ValeBabi_Initialize on arrival by entrance 22, from Kraden's
/// cottage, while he prepares for the first Sol Sanctum visit
/// (Event_TalkToKraden). Takes exit 22 back outside (ValeKraden's entrance 10),
/// where Event_KradenSetsOff gathers the party.
static void Event_ShowMythrilBag(void) {
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  GlyphTransfer *glyph;
  u8 clear;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(480), PIXELS(202));
  Engine_TaskWait(1);
  Engine_CameraFollowActor(ACTOR_KRADEN, 1);
  clear = 0;
  actor = Engine_ObjectCreate(22, PIXELS(328), PIXELS(2), PIXELS(195));
  actor->motionFlags = clear;
  sprite = actor->sprite;
  actor->y.fixed = PIXELS(5);
  sprite->count = clear;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  // Load the item icon into the temporary actor before animating the bag.
  glyph = Engine_HeapAllocate(17, sizeof(*glyph));
  Engine_ItemLoadIcon(ITEM_MYTHRIL_BAG);
  Engine_VramLoad(sprite->slot, 128, glyph->tiles);
  Engine_HeapRelease(17);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  // Walk the presenter into view, then wait for both bag scripts to finish.
  Actor_SetSpeed(ACTOR_KRADEN, Q16_ONE, 0x8000);
  Actor_WalkToAndWait(ACTOR_KRADEN, 480, 176);
  Actor_WalkToAndWait(ACTOR_KRADEN, 420, 164);
  Actor_WalkToAndWait(ACTOR_KRADEN, 326, 185);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
  Engine_ObjectSetScript(actor, gValeBabi_MythrilBagRisesScript);
  ObjectDispatch_WaitForValue16(actor);
  Object_SetScript(actor, gValeBabi_MythrilBagWobblesScript);
  ObjectDispatch_WaitForValue16(actor);
  Engine_EventWait(20);
  Engine_ObjectDispatchRelease(actor);
  Engine_ActorJump(ACTOR_KRADEN, 2, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(22);
}

/// Flashes between two palettes and emits a paired effect every sixteen frames.
UNUSED static void
ValeBabi_FlashPaletteAndSpawnPair(union FieldObject *object) {
  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(&object->actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(&object->actor, 0);
  }
  if ((gFrameCount & 15) == 0) {
    ValeBabi_SpawnEffectPair(object);
  }
}

/// Cycles six palettes on odd frames and periodically emit paired effects.
static void ValeBabi_CyclePaletteAndSpawnPair(union FieldObject *object) {
  if ((gFrameCount & 1) != 0) {
    Engine_ObjectSetPartPalettes(&object->actor, (gFrameCount >> 1) % 6);
  }
  if ((gFrameCount & 15) == 0) {
    ValeBabi_SpawnEffectPair(object);
  }
}

/// Advances deterministically through six palettes on odd frames.
static void ValeBabi_CyclePaletteOnOddFrames(union FieldObject *object) {
  if ((gFrameCount & 1) != 0) {
    s32 slot = (gFrameCount >> 1) % 6;

    Engine_ObjectSetPartPalettes(&object->actor, slot);
  }
}

// Shared vertical-effect callbacks precede their paired-effect creator.
#include "common/effect_vertical.h"

/// Copies the parent's terrain height into two oppositely moving vertical
/// effects.
static void ValeBabi_SpawnEffectPair(union FieldObject *object) {
  struct FieldActor *parent = &object->actor;
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *part;
  struct AnimationObject *sprite;
  struct BattleFxScene *work = gEffectWork;
  s32 effectIndex;

  for (effectIndex = 0; effectIndex < 2; ++effectIndex) {
    child = Engine_ObjectCreate(26, parent->x.fixed, parent->y.fixed,
                                parent->z.fixed);
    pair[effectIndex] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      part = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (part != NULL) {
        sprite = part;
        Object_InitializeMode(part, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = work->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramBlockCache[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        part->image.size = 2;
        part->entries[0]->frame = 0;
      }
    }
  }
  {
    struct FieldActor *effectActor = pair[0];
    struct AnimationObject *effectSprite = effectActor->sprite;

    effectActor->update = Effect_AnimateVerticalNegative;
    effectSprite->image.priority = 2;
  }
  {
    struct FieldActor *effectActor = pair[1];
    struct AnimationObject *effectSprite = effectActor->sprite;

    effectSprite->image.priority = 2;
    effectActor->update = Effect_AnimateVerticalPositive;
    effectActor->priorityFlags = 2;
  }
}

// Shared Psynergy setup precedes the scene's effect wrappers.
#include "common/effect_psynergy.h"

UNUSED static void ValeBabi_CyclePaletteWithParticles(void) {
  ValeBabi_CyclePaletteAndSpawnPair((union FieldObject *)Object_GetById(8));
}

UNUSED static void ValeBabi_CyclePaletteWithoutParticles(void) {
  ValeBabi_CyclePaletteOnOddFrames((union FieldObject *)Object_GetById(17));
}

/// Catch's effect plays on Dora, then its objects are released.
/// Called by Event_DoraWakesIsaac as Dora hands Isaac his tunic.
static void ValeBabi_DemonstrateCatch(void) {
  Engine_PsynergyBegin(ABILITY_CATCH, 1);
  Engine_PsynergySetTarget(ACTOR_DORA, 0x11);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_PsynergyLowerHands();
  BattleEffect_CleanupSceneObjects();
}

s32 gValeBabi_LeaveHouseScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(522), 0, PIXELS(748)),
    SC_SYNC,   SC_MOVE(PIXELS(419), 0, PIXELS(748)),
    SC_SYNC,   SC_MOVE(PIXELS(419), 0, PIXELS(691)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabi_DoraLeavesScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(387), 0, PIXELS(661)), SC_SYNC, SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeBabi_IsaacLeavesScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(408), 0, PIXELS(661)),
    SC_SYNC,   SC_MOVE(PIXELS(387), 0, PIXELS(661)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeBabi_DoraRunsAheadScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(468), 0, PIXELS(390)),
    SC_SYNC,   SC_MOVE(PIXELS(508), 0, PIXELS(402)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabi_IsaacRunsAheadScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(431), 0, PIXELS(391)),
    SC_SYNC,   SC_MOVE(PIXELS(468), 0, PIXELS(390)),
    SC_SYNC,   SC_MOVE(PIXELS(486), 0, PIXELS(393)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabi_DoraRunsBackScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(525), 0, PIXELS(414)),
    SC_SYNC,   SC_MOVE(PIXELS(509), 0, PIXELS(442)),
    SC_SYNC,   SC_MOVE(PIXELS(455), 0, PIXELS(450)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabi_IsaacRunsBackScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(525), 0, PIXELS(414)),
    SC_SYNC,   SC_MOVE(PIXELS(509), 0, PIXELS(442)),
    SC_SYNC,   SC_MOVE(PIXELS(481), 0, PIXELS(443)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabi_RunToPlazaScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(428), 0, PIXELS(460)),
    SC_SYNC,   SC_MOVE(PIXELS(415), 0, PIXELS(513)),
    SC_SYNC,   SC_MOVE(PIXELS(415), 0, PIXELS(589)),
    SC_SYNC,   SC_END,
};

s32 gValeBabi_Actor10ShakesScript[] = {
    SC_SET(SW_SCALE_X, 0xe666),  SC_SET(SW_SCALE_Y, 0x10000), SC_WAIT(4),
    SC_SET(SW_SCALE_X, 0xcccc),  SC_SET(SW_SCALE_Y, 0xcccc),  SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeBabi_Actor10FallsScript[] = {
    SC_ARG(2),
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
    SC_POS(0, 0, 0),
    SC_ARG(1),
    SC_END,
};

s32 gValeBabi_DoraFacesIsaacScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_MOVE(PIXELS(567), 0, PIXELS(690)),
    SC_SYNC,
    SC_MOVE(PIXELS(543), 0, PIXELS(674)),
    SC_SYNC,
    SC_END,
};

s32 gValeBabi_MythrilBagRisesScript[] = {
    SC_ADD(SW_Y, 0x8000),
    SC_WAIT(1),
    SC_LOOP(16, SC_TOP),
};

s32 gValeBabi_MythrilBagWobblesScript[] = {
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

struct SceneEntrance gValeBabi_Entrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 160, 0, 228, FACING_NORTH, 0, 24, 0, 248, 256, 0},
    {2, CONDITION_ALWAYS, 480, 0, 176, FACING_NORTH, 0, 264, 8, 560, 240, 0},
    {3, CONDITION_ALWAYS, 721, 0, 192, FACING_NORTH, 0, 584, 0, 848, 224, 0},
    {4, CONDITION_ALWAYS, 144, 0, 494, FACING_NORTH, 0, 8, 256, 320, 528, 0},
    {5, CONDITION_ALWAYS, 119, 0, 340, FACING_SOUTH, 0, 8, 256, 320, 528, 0},
    {6, CONDITION_ALWAYS, 416, 0, 528, FACING_NORTH, 0, 344, 272, 640, 560, 0},
    {7, CONDITION_ALWAYS, 408, 0, 371, FACING_SOUTH, 0, 344, 272, 640, 560, 0},
    {8, CONDITION_ALWAYS, 184, 0, 633, FACING_EAST, 0, 8, 576, 320, 736, 0},
    {9, CONDITION_ALWAYS, 408, 0, 665, FACING_EAST, 0, 328, 600, 624, 816, 0},
    {10, CONDITION_ALWAYS, 528, 0, 683, FACING_SOUTH, 0, 328, 600, 624, 816, 0},
    {11, CONDITION_ALWAYS, 705, 0, 817, FACING_NORTH, 0, 640, 552, 952, 864, 0},
    {12, CONDITION_ALWAYS, 879, 0, 765, FACING_NORTH, 0, 640, 552, 952, 864, 0},
    {16, CONDITION_ALWAYS, 538, 0, 683, FACING_SOUTH, 0, 328, 600, 624, 816, 0},
    {19, CONDITION_ALWAYS, 760, 0, 328, FACING_SOUTH, 0, 672, 272, 960, 488, 0},
    {20, CONDITION_ALWAYS, 567, 0, 676, FACING_EAST, 0, 1024, 576, 1024, 576,
     0},
    {21, CONDITION_ALWAYS, 408, 0, 371, FACING_SOUTH, 0, 344, 272, 640, 560, 0},
    {22, CONDITION_ALWAYS, 480, 0, 176, FACING_NORTH, 0, 264, 8, 560, 240, 0},
    {SCENE_TABLE_END},
};

u32 gValeBabi_Exits[] = {
    SCENE_EXITS(8),          SCENE_EXIT(1, 5, 6),   SCENE_EXIT(2, 6, 3),
    SCENE_EXIT(3, 5, 2),     SCENE_EXIT(4, 5, 7),   SCENE_EXIT(5, 8, 8),
    SCENE_EXIT(6, 4, 8),     SCENE_EXIT(7, 8, 9),   SCENE_EXIT(8, 8, 5),
    SCENE_EXIT(9, 8, 7),     SCENE_EXIT(10, 3, 12), SCENE_EXIT(11, 3, 13),
    SCENE_EXIT(19, 138, 11), SCENE_EXIT(20, 8, 21), SCENE_EXIT(21, 3, 15),
    SCENE_EXIT(22, 6, 10),   SCENE_EXITS_END,
};

u32 gValeBabi_StormExits[] = {
    SCENE_EXITS(8),        SCENE_EXIT(1, 5, 6),   SCENE_EXIT(2, 6, 3),
    SCENE_EXIT(3, 5, 2),   SCENE_EXIT(4, 5, 7),   SCENE_EXIT(5, 8, 8),
    SCENE_EXIT(6, 3, 8),   SCENE_EXIT(7, 8, 9),   SCENE_EXIT(8, 8, 5),
    SCENE_EXIT(9, 8, 7),   SCENE_EXIT(10, 3, 12), SCENE_EXIT(11, 3, 13),
    SCENE_EXIT(20, 8, 21), SCENE_EXIT(21, 3, 15), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeBabi_Placements[] = {
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x35, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xd0, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x78, 0x0, ACTOR_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x80, 0x0, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x22, 0x0, ACTOR_STAND,
        PIXELS(54), 0, PIXELS(669), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xe2, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHEAST, TALK_KEEP_FACING, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeBabi_PlacementsAfterLeaving[] = {
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x35, 0x0, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xd0, 0x0, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x78, 0x0, ACTOR_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x80, 0x0, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeBabi_PlacementsAfterKalay[] = {
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(586), 0, PIXELS(406), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x35, 0x0, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xd0, 0x0, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x78, 0x0, ACTOR_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x80, 0x0, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(373), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, 0x0, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(378), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeBabi_ScholarsPlacements[] = {
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(336), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x71, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(864), 0, PIXELS(360), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(904), 0, PIXELS(376), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x71, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(832), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeBabi_StormEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNorthwestArea) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNorthArea) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNortheastArea) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitWestAreaSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitWestAreaNorth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitCenterAreaSouth) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitCenterAreaNorth) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthwestArea) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthAreaWest) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthAreaSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSoutheastArea) },
    { SEARCH(SEARCH_WOODEN_BOX), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeWoodenBoxEmpty)) },
    { SEARCH(SEARCH_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_BARREL), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabi_Events[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeMyFatherAlsoDiedInThat) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeMySonIsStillYoungSo) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeOneOfTheMenFromThat) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeTwoOfThoseThreeGuestsStaying) },
    { EVENT_TALK, ACTOR_NEIGHBOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskIsaacHisBusiness) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNorthwestArea) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNorthArea) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNortheastArea) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitWestAreaSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitWestAreaNorth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitCenterAreaSouth) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitCenterAreaNorth) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthwestArea) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthAreaWest) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthAreaSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSoutheastArea) },
    { SEARCH(SEARCH_WOODEN_BOX), 100, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { SEARCH(SEARCH_JAR), 101, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabi_EventsAfterLeaving[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeComeBackAsAStrongWarrior) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeIWantedJennaToBeMy) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhetherMaskedManWasFelix) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeWereASmallHotelAndWe) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeIWillThinkOfJennaAs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeNowThatJennaIsGoneWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeIWonderIfFelixChangedMuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeFelixAndHisGangMustHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeWhoCouldHaveKnownThatMasked) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNorthwestArea) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNorthArea) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNortheastArea) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitWestAreaSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitWestAreaNorth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitCenterAreaSouth) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitCenterAreaNorth) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthwestArea) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthAreaWest) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthAreaSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSoutheastArea) },
    { SEARCH(SEARCH_WOODEN_BOX), 100, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { SEARCH(SEARCH_JAR), 101, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabi_EventsAfterKalay[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeOwItsGonnaBeYearsBefore) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHomeIsaacOhWhatHappened) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeYouTraveledAroundTheEasternEdge) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeArentYouGladToSeeThe) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_NEIGHBOR, FLAG_VALE_INNKEEPER_WELCOMED_ISAAC, EVENT_SCRIPT(Event_NeighborSeesIsaacHasGrown) },
    { EVENT_TALK, ACTOR_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgValeYouveGrownSoMuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeIPromisedIdBecomeAGreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeThatShyGirlWithIsaacSure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeIWonderIfIsaacWillContinue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeValeWillNeverChangeSoYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_INNKEEPER, FLAG_VALE_INNKEEPER_WELCOMED_ISAAC, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadInnkeepersThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_NEIGHBOR, FLAG_VALE_NEIGHBOR_SAW_ISAAC_GROWN, EVENT_SCRIPT(Event_NeighborSeesIsaacHasGrown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NEIGHBOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadThoughtsAboutDorasIllness) },
    { EVENT_TALK, ACTOR_DORA, FLAG_VISITED_SICK_DORA, EVENT_SCRIPT(Event_VisitSickDora) },
    { EVENT_TALK, ACTOR_DORA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDora) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_DORA, FLAG_VISITED_SICK_DORA, EVENT_SCRIPT(Event_VisitSickDora) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DORA, FLAG_SCENE_TEMP_03, MSGNO(MsgValeKyleWatchOutTheBoulderThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgValeImSorryISeemedSoHarsh) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNorthwestArea) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNorthArea) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitNortheastArea) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitWestAreaSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitWestAreaNorth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitCenterAreaSouth) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitCenterAreaNorth) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthwestArea) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthAreaWest) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSouthAreaSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExitSoutheastArea) },
    { SEARCH(SEARCH_WOODEN_BOX), 100, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { SEARCH(SEARCH_JAR), 101, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { SEARCH(SEARCH_JAR), 108, FLAG_VALE_JAR_VIAL, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_BARREL), 107, FLAG_VALE_BARREL_COINS, FIND_COINS(123) },
    { SEARCH(SEARCH_UNNAMED), 109, FLAG_VALE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabi_ScholarsEvents[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiOriginallyThereWereAncientRuinsAt) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBabiNoMatterHowManyTimesI) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBabiItIsSaidThatLongAgo) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBabiOriginallyTheAncientVenusRuinsLay) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiBabiMakesHisScholarsPerformAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiNoOneHasEverReachedThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiStillMyExperimentsInAlchemyAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiKradensJourneyWasMeantToRevive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiTheVenusRuinsAreTheRemnants) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiLordBabiExpectedGreatResultsFrom) },
    { 0x21, 20, CONDITION_ALWAYS, 19 },
    { SEARCH(SEARCH_JAR), 103, FLAG_BABI_PALACE_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresABookEntitledEncyclopediaOf)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledExperimentsIn)) },
    { SEARCH(SEARCH_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledStudyOf)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabi_ScholarsLaterEvents[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiOnceBabiLighthouseIsCompleteId) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ScholarAsksAboutKraden) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBabiAlchemyTeachesThatTheFourElements) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBabiIfTheRiddleOfTheVenus) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiImSureKradenWouldStopBy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiTheRelationshipBetweenAlchemyAndThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiIWantToSeeKradenAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiTheresGotToBeAGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiWhatSortOfUnfathomableRiddlesAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiThereMustBeAReasonWhy) },
    { 0x21, 20, CONDITION_ALWAYS, 19 },
    { SEARCH(SEARCH_JAR), 103, FLAG_BABI_PALACE_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresABookEntitledEncyclopediaOf)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledExperimentsIn)) },
    { SEARCH(SEARCH_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledStudyOf)) },
    { SCENE_EVENTS_END },
};
// clang-format on
