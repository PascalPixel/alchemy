/// @file
/// Part of object.c, the field object, compiled only through it.
/// Finding and running a scene's events: talking, touching, searching.

#include "../object_internal.h"

/// Returns the placement of actor id in the scene's placement tables, or NULL:
/// for a party character (0 to 7) the row with its sprite, and for a placed
/// actor (8 on) the row it was created from, counting the placed rows in order.
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

/// Returns 1 for a search event (kind 3) that holds whatever its condition: one
/// with more flags than its kind, unless its result is a mimic battle (type
/// 0x005 in its top 12 bits).
u32 Event_SearchIgnoresCondition(s32 flags, s32 result) {
  u32 mimicDifference;
  s32 searchFlags;
  u32 ignoresCondition;

  if ((flags & 0xf) != EVENT_SEARCH)
    return 0;
  searchFlags = flags & 0x1ff;
  ignoresCondition = 0;
  if (searchFlags != EVENT_SEARCH) {
    mimicDifference = (result & 0xfff00000) ^ 0x500000;
    ignoresCondition = mimicDifference | (0U - mimicDifference);
    // 1 when mimicDifference is not 0.
    ignoresCondition >>= 31;
  }
  return ignoresCondition;
}

/// Returns the first of the scene's events of kind for target whose condition
/// holds, or NULL. Flag 0x200 also needs the leader to have pushed against it
/// for more than 12 frames (2 with flag 0x800); flag 0x400 needs the leader to
/// face within an eighth of a turn of the direction in the top four bits.
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

      if ((flags & 0x800) != 0)
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

/// Runs the scene's initialization again. Nothing calls this.
UNUSED void Runtime_CallWorkBlockEntry(void) { gOverlayArea.initialize(); }

/// Returns the talk event of actor id: for the actor the party has stopped, its
/// kind 7 event if it has one.
s32 Scene_FindActorTalkEvent(s32 id) {
  struct EffectDescriptor *talkEvent = Scene_FindEvent(EVENT_TALK, id);
  s32 pausedActor = gGameState.pausedObject;

  if (pausedActor == id) {
    struct EffectDescriptor *pausedEvent = Scene_FindEvent(7, pausedActor);

    if (pausedEvent != 0) {
      return (s32)pausedEvent;
    }
  }
  return (s32)talkEvent;
}

/// Talks to actor id: shows its event's message or calls its event function,
/// with the actor stopped and turned as its placement says. For the actor the
/// party has stopped, its kind 7 event comes first; without one, a talk event
/// that is a function gives a random line from MsgPonderEllipsis ("...")
/// instead. Returns 0 when an event ran, -1 when there was none.
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
      if (event->result >= 0x10000) {
        s32 definitionFlags = BattleFx_GetFlags(id);
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
          actor->linkedObject = Object_GetById(gGameState.selectedActor);
          actor->actionFlags |= 1;
          ObjectMotion_SetActionCallback(actor,
                                         gObjectMotion_LinkedActionScript);
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
    BattleFx_ResumeObject(gGameState.pausedObject);
    gGameState.pausedObject = -1;
  }
  return result;
}

/// Runs the scene's raised event (kind 6) for value: its message or its
/// function. Returns 0 when an event ran, -1 when there was none.
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

/// Runs the touch event (kind 2) of what the leader touched, or else takes its
/// exit (kind 1): plays the exit's sound (and effect) and records where it
/// leads. Returns 0 when something ran, -1 when there was nothing.
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
      exitKind = exit->flags & 0x30;
      switch (exitKind) {
      case 0:
        AudioCommand_PlayFar(SE_MAP_EXIT);
        break;
      case 32:
        AudioCommand_PlayFar(SE_128);
        ObjectEffect_BeginContextEffect26();
        break;
      case 48:
        AudioCommand_PlayFar(SE_129);
        ObjectEffect_BeginContextEffect25();
        break;
      }
      work->exit = exit->result;
      handled = 0;
    }
  }
  return handled;
}

/// Runs the event of a tile the leader pushes against (codes 0xf9 and 0xfc to
/// 0xfe), once it has pushed for more than 12 frames; the count then restarts.
s32 Field_RunPushTileEvent(s32 collisionCode) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 leader = gGameState.selectedActor;

  switch (collisionCode) {
  case 0xfc: // with Down held
    if (work->blockedSteps > 12 && (gKeysHeld & KEY_DOWN)) {
      Actor_JumpDownLedge(leader, 6, 0);
      work->blockedSteps = 0;
    }
    break;
  case 0xf9:
  case 0xfe: // jump forward
    if (work->blockedSteps > 12) {
      Field_TryJumpForward();
      work->blockedSteps = 0;
    }
    break;
  case 0xfd: // one thing with Down held, another with Up
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

/// Searches the tile ahead with the given collision code. Codes 242 to 247 show
/// their two messages. Otherwise the scene's search event for the tile runs: a
/// function, a message, or a chest holding a Psynergy stone, a mimic, coins or
/// an item; with no event, "checked the ground" and "found nothing".
s32 Field_SearchTile(s32 tile) {
  const struct SceneEvent *event;
  s32 leader;
  u32 messageOffset;
  s16 condition;
  u32 searchResult;
  s32 rewardType;
  s32 canOpen;

  leader = gGameState.selectedActor;
  messageOffset = tile - 242;
  if (messageOffset <= 5) {
    Leader_StopMoving();
    tile = gField_TileMessageOffsets[messageOffset];
    UiText_ShowPositionedMessageAndWaitFar(MsgTileChecked + tile, 1);
    UiText_ShowPositionedMessageAndWaitFar(MsgTileResult + tile, 1);
  } else {
    event = (const struct SceneEvent *)Scene_FindEvent(EVENT_SEARCH, tile);
    if (event != NULL) {
      s32 flags = event->control;

      // Bits 4 to 8 choose what the leader says on checking; if the event
      // leaves FLAG_NOTHING_HAPPENS set, the matching result line follows.
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
      searchResult = event->value;
      if ((searchResult & 0x0f000000) != 0 ||
          (searchResult & 0xfff00000) == 0x400000) {
        if ((searchResult & 0x0f000000) != 0) {
          // A function.
          if (GameFlag_IsConditionActive(condition) != 0)
            ((void (*)(s32))event->value)(gGameState.selectedActor);
          if (GameFlag_TestFar(FLAG_NOTHING_HAPPENS) != 0)
            UiText_ShowPositionedMessageAndWaitFar(
                MsgTileResult + messageOffset, 1);
        } else {
          // A message (type 0x004).
          if (GameFlag_IsConditionActive(condition) != 0)
            UiText_ShowPositionedMessageAndWaitFar((u16)event->value, 1);
          else
            UiText_ShowPositionedMessageAndWaitFar(MsgButFoundNothing, 1);
        }
      } else {
        // A chest; its condition flag is set once it has been opened.
        FieldEvent_Begin();
        if (GameFlag_IsConditionActive(condition) != 0) {
          searchResult = event->value;
          canOpen = 1;
          if ((searchResult & 0xf0000) == 0x10000 && leader <= 7)
            canOpen = 0;
          if (canOpen != 0) {
            if ((event->control & 0x1ff) == 19)
              Marker_OpenChest(tile);
            rewardType = event->value & 0xfff00000;
            if (rewardType == 0x300000) {
              // A Psynergy stone: restores all PP.
              void *object;

              if ((event->control & 0x1ff) == 19)
                Marker_ShowOpened(tile);
              object = Marker_GetActor(tile);
              ItemDisplay_RaiseAboveLeader(object);
              AudioCommand_PlayFar(MUS_TREASURE_FOUND);
              UiWork_PushValueSlotFar((u16)event->value, 5);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotPsynergyStone, 3);
              BattleParty_ApplyDrain(999, 0);
              UiWindow_CreateWithLayoutBoundsFar(1);
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
            } else if (rewardType == 0x500000) {
              // A mimic: a battle with enemy group slot of zone 99.
              struct FieldWork *work = (struct FieldWork *)gEventWork;

              if ((event->control & 0x1ff) == 19)
                Marker_LaunchMimic(tile);
              if (condition != -1) {
                condition = gGameState.pendingDjinnEvent = condition | 0x1000;
              }
              work->battle =
                  Encounter_GetEnemyGroupAtSlot(99, (u16)event->value);
              Battle_SetStart(2);
              BattleFx_SelectBattleCue(99, (u16)event->value);
              AudioCommand_PlayFar(gGameState.sceneCue);
              UiText_ShowPositionedMessageAndWaitFar(MsgChestWasMimic, 1);
            } else if (rewardType == 0x200000) {
              // Coins.
              void *object;

              object = BattleFx_StartRandomParticleEmitter(
                  gGameState.selectedActor, 0);
              WaitFrames(30);
              if ((event->control & 0x1ff) == 19)
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

              object = BattleFx_StartRandomParticleEmitter(
                  gGameState.selectedActor, event->value & 0xfff);
              WaitFrames(30);
              recipient = PartyInventory_AddFar((u16)event->value);
              messageOffset = 0xffff;
              if (recipient == -1) {
                UiWork_PushValueSlotFar(event->value & 0xfff, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgFoundItem, 1);
                UiText_ShowPositionedMessageAndWaitFar(MsgFoundItem + 4, 1);
                Object_DestroyIfPresent(object);
                if ((event->control & 0x1ff) == 19)
                  Marker_CloseChest(tile);
              } else {
                if ((event->control & 0x1ff) == 19)
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
          UiText_ShowPositionedMessageAndWaitFar(MsgTileResult + messageOffset,
                                                 1);
        }
        FieldEvent_End();
        BattleFx_PlayQueuedSound();
      }
    } else {
      UiText_ShowPositionedMessageAndWaitFar(MsgCheckedGround, 1);
      UiText_ShowPositionedMessageAndWaitFar(MsgGroundDidntFindAnything, 1);
    }
    GameFlag_ClearBitFar(FLAG_NOTHING_HAPPENS);
  }
  return 0;
}
