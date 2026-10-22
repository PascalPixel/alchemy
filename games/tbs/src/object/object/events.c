/// @file
/// Finding and running a scene's events: talking, touching, searching.
// Part of object.c.

#include "../object_internal.h"

/// Returns the placement of actor id in the scene's placement tables, or NULL:
/// for a party character (0 to 7) the row with its sprite, and for a placed
/// actor (8 on) the row it was created from, counting the placed rows in order.
///
/// Called by Scene_TalkToActor for the talk facing, and by the field Psynergy
/// effects that read an actor's placement (Mind Read, the Djinni effect, the
/// event messages and the view).
struct ScenePlacement *Scene_FindActorPlacement(s32 id) {
  struct ScenePlacement *placement;
  s32 tableIndex;
  s32 placedActor = ACTOR_FIRST_PLACED;

  for (tableIndex = 0; tableIndex < 4; tableIndex++) {
    placement = ((struct FieldWork *)gEventWork)->tables[tableIndex];
    if (placement == NULL)
      continue;
    if (id <= 7) {
      for (; (s16)placement->sprite != -1; placement++) {
        if ((s16)placement->sprite == id) {
#ifdef UBFIX
          return placement;
#else
          goto found;
#endif
        }
      }
    } else {
      for (; (s16)placement->sprite != -1; placement++) {
        if ((s16)placement->sprite > 7) {
          if (placedActor == id) {
#ifdef UBFIX
            return placement;
#else
            goto found;
#endif
          }
          placedActor++;
        }
      }
    }
  }
#ifdef UBFIX
  return NULL;
#else
found:
  // BUG: a missing actor leaves entry NULL if the final table is absent.
  if ((s16)placement->sprite == -1)
    return NULL;
  return placement;
#endif
}

/// Returns whether an event condition holds: -1 always holds; a condition with
/// bit 0x1000 holds while that flag is set, any other while its flag is clear.
/// The flag is the low 12 bits, since GameFlag_Test ignores bit 12.
///
/// Used for every condition column of a scene's tables: events
/// (Scene_FindEvent, Field_SearchTile), placements (Event_SpawnObjectTable),
/// exit regions (Field_CheckExitRegions) and item events.
s32 GameFlag_IsConditionActive(s32 condition) {
  s32 flagSet;

  if (condition == -1) {
    return 1;
  }
  if (0x1000 & condition) {
    return GameFlag_TestFar(condition);
  }
  flagSet = GameFlag_TestFar(condition);
  return flagSet == 0;
}

/// Returns 1 for a search event (EVENT_SEARCH) that holds whatever its
/// condition: one with more flags than its kind, unless its result is a mimic
/// battle (type 0x005 in its top 12 bits).
///
/// Called by Scene_FindEvent. Field_SearchTile tests such an event's condition
/// itself (a chest's flag is set once it is opened), so the event is found
/// either way and the leader still checks the spot; a mimic is found only
/// while its condition holds.
u32 Event_SearchIgnoresCondition(s32 flags, s32 result) {
  u32 mimicDifference;
  s32 searchFlags;
  u32 ignoresCondition;

  if ((flags & 0xf) != EVENT_SEARCH)
    return 0;
  searchFlags = flags & 0x1ff;
  ignoresCondition = 0;
  if (searchFlags != EVENT_SEARCH) {
    mimicDifference = (result & 0xfff00000) ^ FIND_MIMIC;
    ignoresCondition = mimicDifference | (0U - mimicDifference);
    // 1 when mimicDifference is not 0.
    ignoresCondition >>= 31;
  }
  return ignoresCondition;
}

/// Returns the first of the scene's events of kind for target whose condition
/// holds, or NULL. EVENT_TOUCH_WALKING also needs the leader to have pushed
/// against it for more than 12 frames (2 with control bit 11);
/// EVENT_TOUCH_FACING needs the leader to face within an eighth of a turn of
/// the direction in the top four bits.
///
/// Called by every lookup of a scene's events in this part and by
/// Object_GetTriggerTileAheadOfCurrent. Changes nothing.
struct EffectDescriptor *Scene_FindEvent(s32 kind, s32 target) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct EffectDescriptor *event = (struct EffectDescriptor *)work->events;
  s32 flags;
  u32 facing =
      ((struct FieldActor *)ObjectTable_Get(gGameState.selectedActor))->facing;

  flags = event->flags;
  // Preserve the full control word: direction and push-delay bits refine kind.
  while (flags != -1) {
    if ((flags & 0xf) == kind && event->value == target &&
        (Event_SearchIgnoresCondition(flags, event->result) != 0 ||
         (GameFlag_IsConditionActive(event->condition) != 0 &&
          (flags = event->flags, 1)))) {
      s32 matches = 0;
      s32 minimumPushFrames = 12;
      s32 touchRequirements;

      if ((flags & EVENT_TOUCH_QUICK) != 0)
        minimumPushFrames = 2;
      touchRequirements = flags & (EVENT_TOUCH_WALKING | EVENT_TOUCH_FACING);

      switch (touchRequirements) {
      case 0:
        matches = 1;
        break;
      case EVENT_TOUCH_WALKING:
        if (work->blockedSteps > minimumPushFrames) {
          matches = 1;
        }
        break;
      case EVENT_TOUCH_FACING:
        // Bits 15 to 12 are a facing in sixteenths of a turn; the leader's
        // facing must be within 0x1fff (an eighth of a turn) of it.
        flags &= 0xf000;
        matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        break;
      case EVENT_TOUCH_WALKING | EVENT_TOUCH_FACING:
        if (work->blockedSteps > minimumPushFrames) {
          flags &= 0xf000;
          matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        }
        break;
      }
      if (matches) {
        return event;
      }
    }
    event++;
    if (event->flags == -1)
      break;
    flags = event->flags;
  }
  return 0;
}

/// Unused: runs the loaded overlay's initialize entry; nothing calls it.
UNUSED void Runtime_CallWorkBlockEntry(void) { gOverlayArea.initialize(); }

/// Returns the talk event of actor id, or 0: for the actor stopped with Halt
/// (gGameState.pausedObject), its EVENT_TALK_HALTED event if it has one, the
/// event for talking to it while it is stopped (Crossbone Isle's Bane has one).
///
/// Called by Field_RunScene for the A button, to choose between talking and
/// searching, and by the field Psynergy dispatch, which Halts only an actor
/// with a talk event; both test the result only for non-zero.
s32 Scene_FindActorTalkEvent(s32 id) {
  struct EffectDescriptor *talkEvent = Scene_FindEvent(EVENT_TALK, id);
  s32 pausedActor = gGameState.pausedObject;

  if (pausedActor == id) {
    struct EffectDescriptor *pausedEvent =
        Scene_FindEvent(EVENT_TALK_HALTED, pausedActor);

    if (pausedEvent != 0) {
      return (s32)pausedEvent;
    }
  }
  return (s32)talkEvent;
}

/// Talks to actor id: shows its event's message or calls its event function,
/// with the actor stopped and turned as its placement says. For the actor
/// stopped with Halt, its EVENT_TALK_HALTED event comes first; without one, a
/// talk event that is a function gives one of two lines at random instead, from
/// MsgPonderEllipsis on (dots spoken with pauses). Returns 0 when an event ran,
/// -1 when there was none or such a line stood in for it; Field_RunScene
/// ignores the result.
///
/// Called by Field_RunScene for its talk request (work->talk). Resumes
/// the stopped actor and sets gGameState.pausedObject back to -1 afterwards.
s32 Scene_TalkToActor(s32 id) {
  struct ScenePlacement *placement = Scene_FindActorPlacement(id);
  s32 result = -1;
  struct ObjectRuntime *actor = (struct ObjectRuntime *)Object_GetById(id);
  struct EffectDescriptor *event;
  s32 actorPaused = 0;
  u32 savedAngle;
  u32 talkFacing;
  s32 usePausedDialogue = 0;

  if (gGameState.pausedObject == id) {
    actorPaused = 1;
    event = Scene_FindEvent(7, id);
    if (event == 0) {
      event = Scene_FindEvent(EVENT_TALK, id);
      usePausedDialogue = 1;
      if (event == 0) {
        return -1;
      }
      // A function talk event is not run for a stopped actor: it says one of
      // two lines picked at random from the pair its definition flags choose.
      if (event->result >= 0x10000) {
        s32 definitionFlags = FieldFx_GetFlags(id);
        u32 random = Random16();
        s32 message =
            MsgPonderEllipsis + definitionFlags * 2 + (random * 2 >> 16);
        FieldEvent_Begin();
        Event_SetMessageId(message);
        FieldEvent_ShowMessageAndWait(id, 0);
        FieldEvent_End();
        goto finish;
      }
    }
  } else {
    event = Scene_FindEvent(EVENT_TALK, id);
  }

  if (event != 0 && event->result != 0) {
    if (!actorPaused) {
      // Stop the actor; with talk facing 0, 1 or 3, also stop the leader and
      // link the two.
      u8 *movementState = &actor->movementState;

      *movementState = 1;
      ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)actor, 0);
      savedAngle = actor->angle;
      talkFacing = (u8)placement->talkFacing << 24;
      if (talkFacing <= (1 << 24) || talkFacing == (3 << 24)) {
        struct ObjectRuntime *leader =
            (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);

        leader->targetX = leader->x;
        leader->targetY = leader->y;
        leader->targetZ = leader->z;
        SET_VELOCITY(leader, 0, 0, 0);
        Object_LinkPair(id, gGameState.selectedActor, 0);
      }
    }
    // A result below 0x10000 is a message; above, a function to call.
    if (event->result < 0x10000) {
      UiWork_SetBusyFlagsFar(usePausedDialogue);
      FieldEvent_Begin();
      Event_SetMessageId(event->result);
      FieldEvent_ShowMessageAndWait(id, 0);
      FieldEvent_End();
    } else {
      Leader_StopMoving();
      ((void (*)(s32))event->result)(id);
    }
    if (!actorPaused) {
      // An actor left idle (script command 16) goes back to what its talk
      // facing says: following the leader (3) or its old angle (1).
      if (actor->script[actor->step] == 16) {
        if ((s8)placement->talkFacing == 3) {
          actor->linkedObject =
              (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
          actor->actionFlags |= 1;
          ObjectMotion_SetActionCallback(actor,
                                         (s32)gObjectMotion_LinkedActionScript);
        } else if ((s8)placement->talkFacing == 1) {
          actor->action = savedAngle;
          ObjectDispatch_InitializeFar(
              (struct DispatchObject *)actor,
              (const s32 *)gObjectMotion_StepAngleScript);
        }
      }
      actor->movementState = 0;
      ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)actor, 16);
    }
    result = 0;
  }
finish:
  // Both a normal event and the paused actor's fallback line release the pause.
  if (actorPaused) {
    FieldFx_ResumeObject(gGameState.pausedObject);
    gGameState.pausedObject = -1;
  }
  return result;
}

/// Runs the scene's raised event (EVENT_RAISED) for value: its message or its
/// function. Returns 0 when an event ran, -1 when there was none.
///
/// Called by Field_RunScene for a positive event request (work->event).
s32 Scene_RunRaisedEvent(s32 value) {
  s32 result;
  s32 handled;
  struct EffectDescriptor *event;

  event = Scene_FindEvent(EVENT_RAISED, value);
  handled = -1;
  if (event != NULL) {
    result = event->result;
    if (result != 0) {
      if (result < 0x10000) {
        FieldEvent_Begin();
        Event_SetMessageId(event->result);
        FieldEvent_ShowMessageAndWait(-1, 0);
        handled = 0;
        FieldEvent_End();
      } else {
        ((void (*)(s32))result)(value);
        handled = 0;
      }
    }
  }
  return handled;
}

/// Runs the touch event (EVENT_TOUCH) of what the leader touched, or else
/// takes its exit (EVENT_EXIT): plays the exit's sound (and effect) and records
/// where it leads. Returns 0 when something ran, -1 when there was nothing.
///
/// Called by Field_RunScene for its step request (work->action), which
/// Field_ProcessStep raises for MAP_CELL_EVENT_FIRST to MAP_CELL_EVENT_LAST.
/// Sets work->exit for an exit, and clears work->blockedSteps after a touch
/// that needs pushing.
s32 Scene_RunTouchOrExit(s32 value) {
  struct EffectDescriptor *touch;
  struct EffectDescriptor *exit;
  s32 exitKind;
  s32 handled;
  struct FieldWork *work;

  touch = Scene_FindEvent(EVENT_TOUCH, value);
  handled = -1;
  work = (struct FieldWork *)gEventWork;
  if ((touch != 0) && (touch->result != 0)) {
    if (touch->flags & EVENT_TOUCH_WALKING) {
      work->blockedSteps = 0;
    }
    if (touch->result < 0x10000) {
      FieldEvent_Begin();
      Event_SetMessageId(touch->result);
      FieldEvent_ShowMessageAndWait(-1, 0);
      handled = 0;
      FieldEvent_End();
    } else {
      ((void (*)(s32))touch->result)(value);
      handled = 0;
    }
  } else {
    exit = Scene_FindEvent(EVENT_EXIT, value);
    if (exit != 0) {
      // An enum ExitStyle chooses how the leader leaves; the animated styles'
      // flags make the next scene play the matching arrival.
      exitKind = exit->flags & EXIT_STYLE_MASK;
      switch (exitKind) {
      case EXIT_STYLE_NORMAL:
        AudioCommand_PlayFar(SE_MAP_EXIT);
        break;
      case EXIT_STYLE_ANIMATED:
        AudioCommand_PlayFar(SE_STAIRS_UP);
        ObjectEffect_BeginContextEffect26();
        break;
      case EXIT_STYLE_ANIMATED_ALTERNATE:
        AudioCommand_PlayFar(SE_STAIRS_DOWN);
        ObjectEffect_BeginContextEffect25();
        break;
      }
      work->exit = exit->result;
      handled = 0;
    }
  }
  return handled;
}

/// Runs the event of a tile the leader pushes against (MAP_CELL_LEDGE to
/// MAP_CELL_JUMP_FORWARD, and MAP_CELL_JUMP_FORWARD_UNREACHED), once it has
/// pushed for more than 12 frames; the count then restarts.
///
/// Called by Field_RunScene for its push request (work->pushTile), which
/// Field_ProcessStep raises for MAP_CELL_LEDGE to MAP_CELL_JUMP_FORWARD.
/// Clears work->blockedSteps when it acts.
s32 Field_RunPushTileEvent(s32 collisionCode) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 leader = gGameState.selectedActor;

  switch (collisionCode) {
  case MAP_CELL_LEDGE:
    if (work->blockedSteps > 12 && (gKeysHeld & KEY_DOWN)) {
      Actor_JumpDownLedge(leader, 6, 0);
      work->blockedSteps = 0;
    }
    break;
  case MAP_CELL_JUMP_FORWARD_UNREACHED:
  case MAP_CELL_JUMP_FORWARD:
    if (work->blockedSteps > 12) {
      Field_TryJumpForward();
      work->blockedSteps = 0;
    }
    break;
  case MAP_CELL_CLIMB:
    if (work->blockedSteps > 12) {
      if (gKeysHeld & KEY_DOWN) {
        Field_ClimbDown();
      } else if (gKeysHeld & KEY_UP) {
        Field_ClimbUp();
      }
      work->blockedSteps = 0;
    }
    break;
  }
  return 0;
}

/// Searches the tile ahead with the given collision code. The fixed search
/// spots, MAP_CELL_JAR to MAP_CELL_WELL, show their two messages. Otherwise the
/// scene's search event for the tile runs: a function, a message, or a chest
/// holding a Psynergy Stone, a mimic, coins or an item; with no event, "checked
/// the ground" and "found nothing".
///
/// Called by Field_RunScene for its search request (work->tile). Opening a
/// chest sets its condition flag, and it may raise a mimic battle
/// (work->battle), give coins or an item, or restore all PP.
s32 Field_SearchTile(s32 tile) {
  const struct SceneEvent *event;
  s32 leader;
  u32 messageOffset;
  s16 condition;
  u32 searchResult;
  s32 rewardType;
  s32 canOpen;

  leader = gGameState.selectedActor;
  // Each fixed spot has its own pair of lines, chosen through
  // gField_TileMessageOffsets; the unsigned compare also rejects codes below
  // MAP_CELL_JAR.
  messageOffset = tile - MAP_CELL_JAR;
  if (messageOffset <= MAP_CELL_WELL - MAP_CELL_JAR) {
    Leader_StopMoving();
    tile = gField_TileMessageOffsets[messageOffset];
    UiText_ShowPositionedMessageAndWaitFar(MsgTileChecked + tile, 1);
    UiText_ShowPositionedMessageAndWaitFar(MsgTileResult + tile, 1);
  } else {
    event = (const struct SceneEvent *)Scene_FindEvent(EVENT_SEARCH, tile);
    if (event != NULL) {
      s32 flags = event->control;

      // Bits 8 to 4, the search target (enum SearchTarget), choose what the
      // leader checks ("checked the chest...") unless trigger bit 10 says the
      // event has its own message; if the event leaves FLAG_NOTHING_HAPPENS
      // set, the matching result line follows ("but the chest was empty.").
      condition = event->condition;
      messageOffset = (flags >> 4) & 31;
      if ((event->trigger & 0x400) == 0 && messageOffset != 0) {
        Leader_StopMoving();
        UiText_ShowPositionedMessageAndWaitFar(MsgTileChecked + messageOffset,
                                               1);
        GameFlag_SetBitFar(FLAG_NOTHING_HAPPENS);
      } else {
        GameFlag_ClearBitFar(FLAG_NOTHING_HAPPENS);
      }
      // The value is a function address (bits 27 to 24 set) or a FIND_ value
      // whose top 12 bits give its type: an item, coins, a Psynergy Stone, a
      // message or a mimic.
      searchResult = event->value;
      if ((searchResult & 0x0f000000) != 0 ||
          (searchResult & 0xfff00000) == FIND_MESSAGE(0)) {
        if ((searchResult & 0x0f000000) != 0) {
          // A function.
          if (GameFlag_IsConditionActive(condition) != 0)
            ((void (*)(s32))event->value)(gGameState.selectedActor);
          if (GameFlag_TestFar(FLAG_NOTHING_HAPPENS) != 0)
            UiText_ShowPositionedMessageAndWaitFar(
                MsgTileResult + messageOffset, 1);
        } else {
          // A message.
          if (GameFlag_IsConditionActive(condition) != 0)
            UiText_ShowPositionedMessageAndWaitFar((u16)event->value, 1);
          else
            UiText_ShowPositionedMessageAndWaitFar(MsgButFoundNothing, 1);
        }
      } else {
        // A chest; its condition flag is set once it has been opened.
        FieldEvent_Begin();
        if (GameFlag_IsConditionActive(condition) != 0) {
          // Bit 16 of the value marks a chest the party's own characters
          // cannot open ("Can't open it").
          searchResult = event->value;
          canOpen = 1;
          if ((searchResult & 0xf0000) == 0x10000 && leader <= 7)
            canOpen = 0;
          if (canOpen != 0) {
            // A SEARCH(SEARCH_CHEST) event is a chest the map shows as a
            // marker.
            if ((event->control & 0x1ff) == SEARCH(SEARCH_CHEST))
              Marker_OpenChest(tile);
            rewardType = event->value & 0xfff00000;
            if (rewardType == FIND_PSYNERGY_STONE) {
              // A Psynergy Stone: restores all PP.
              void *object;

              if ((event->control & 0x1ff) == SEARCH(SEARCH_CHEST))
                Marker_ShowOpened(tile);
              object = Marker_GetActor(tile);
              ItemDisplay_RaiseAboveLeader(object);
              AudioCommand_PlayFar(MUS_TREASURE_FOUND);
              UiWork_PushValueSlotFar((u16)event->value, 5);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotPsynergyStone, 3);
              Party_AddPp(999, 0);
              UiWindow_CreateWithLayoutBoundsFar(PARTY_STATUS_SHOW_PP);
              AudioCommand_PlayFar(SE_RECOVERY);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotPsynergyStone + 1,
                                                     1);
              PartyStatus_CloseFar();
              ObjectDispatch_SelectAnimationFar(object, 2);
              AudioCommand_PlayFar(SE_246);
              FieldEvent_WaitFrames(30);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotPsynergyStone + 2,
                                                     1);
              Marker_Hide(tile);
              if (condition != -1)
                GameFlag_SetBitFar(condition);
            } else if (rewardType == FIND_MIMIC) {
              // A mimic: a battle with its slot of ENCOUNTER_ZONE_MIMIC.
              struct FieldWork *work = (struct FieldWork *)gEventWork;

              if ((event->control & 0x1ff) == SEARCH(SEARCH_CHEST))
                Marker_LaunchMimic(tile);
              // The chest's flag (with 0x1000) waits in pendingDjinnEvent,
              // which Field_RunScene resolves when the scene is entered again
              // after the battle.
              if (condition != -1) {
                condition = gGameState.pendingDjinnEvent = condition | 0x1000;
              }
              // BATTLE_START_NO_ESCAPE (battle/battle/round.c): no first strike
              // or surprise, and every attempt to run fails; the command menu
              // still offers it, refusing outright only for BATTLE_START_SCENE
              // and BATTLE_START_LINK.
              work->battle = Encounter_GetEnemyGroupAtSlot(ENCOUNTER_ZONE_MIMIC,
                                                           (u16)event->value);
              Battle_SetStart(BATTLE_START_NO_ESCAPE);
              FieldFx_SelectBattleCue(ENCOUNTER_ZONE_MIMIC, (u16)event->value);
              AudioCommand_PlayFar(gGameState.sceneCue);
              UiText_ShowPositionedMessageAndWaitFar(MsgChestWasMimic, 1);
            } else if (rewardType == FIND_COINS(0)) {
              // Coins.
              void *object;

              object = FieldFx_StartRandomParticleEmitter(
                  gGameState.selectedActor, 0);
              WaitFrames(30);
              if ((event->control & 0x1ff) == SEARCH(SEARCH_CHEST))
                Marker_ShowOpened(tile);
              ItemDisplay_RaiseAboveLeader(object);
              AudioCommand_PlayFar(MUS_TREASURE_FOUND);
              UiWork_PushValueSlotFar((u16)event->value, 5);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotCoins, 3);
              Party_AdjustCoinsFar((u16)event->value);
              if (condition != -1)
                GameFlag_SetBitFar(condition);
              ObjectDispatch_ReleaseFar(object);
            } else {
              // An item, which stays in the chest when nobody can carry it.
              void *object;
              s32 recipient;

              object = FieldFx_StartRandomParticleEmitter(
                  gGameState.selectedActor, event->value & 0xfff);
              WaitFrames(30);
              recipient = PartyInventory_AddFar((u16)event->value);
              // messageOffset is reused here as the item id's 0xffff mask.
              messageOffset = 0xffff;
              if (recipient == -1) {
                UiWork_PushValueSlotFar(event->value & 0xfff, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgFoundItem, 1);
                UiText_ShowPositionedMessageAndWaitFar(MsgFoundItem + 4, 1);
                Object_DestroyIfPresent(object);
                if ((event->control & 0x1ff) == SEARCH(SEARCH_CHEST))
                  Marker_CloseChest(tile);
              } else {
                if ((event->control & 0x1ff) == SEARCH(SEARCH_CHEST))
                  Marker_ShowOpened(tile);
                ItemDisplay_RaiseAboveLeader(object);
                AudioCommand_PlayFar(MUS_TREASURE_FOUND);
                UiWork_PushValueSlotFar(event->value & messageOffset, 2);
                if (recipient == gGameState.selectedActor) {
                  UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem,
                                                         3);
                } else {
                  UiWork_PushValueSlotFar(recipient, 1);
                  UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember,
                                                         3);
                }
                if (condition != -1)
                  GameFlag_SetBitFar(condition);
                ObjectDispatch_ReleaseFar(object);
              }
            }
          } else {
            UiText_ShowPositionedMessageAndWaitFar(MsgCantOpenIt, 1);
          }
        } else {
          // Already opened: the result line for the check message.
          UiText_ShowPositionedMessageAndWaitFar(MsgTileResult + messageOffset,
                                                 1);
        }
        FieldEvent_End();
        FieldFx_PlayQueuedSound();
      }
    } else {
      UiText_ShowPositionedMessageAndWaitFar(MsgCheckedGround, 1);
      UiText_ShowPositionedMessageAndWaitFar(MsgGroundDidntFindAnything, 1);
    }
    GameFlag_ClearBitFar(FLAG_NOTHING_HAPPENS);
  }
  return 0;
}
