/// @file
/// Finding and running a scene's triggers: talking, step triggers, examine
/// triggers.
// Part of overworld.c.

#include "../overworld_internal.h"

/// Returns the actor placement of actor id in the scene's
/// actor placement tables, or NULL: for a party character (0 to 7) the row with
/// its sprite, and for a placed actor (8 on) the row it was created
/// from, counting the placed rows in order.
///
/// Called by Scene_TalkToActor for the talk facing, and by the field
/// Psynergy and event commands that read an actor's placement (Mind Read,
/// a Djinni's joining, the event messages and the view).
struct ActorPlacement *Scene_FindActorPlacement(s32 id) {
  struct ActorPlacement *placement;
  s32 tableIndex;
  s32 placedActorId = ACTOR_FIRST_PLACED;

  for (tableIndex = 0; tableIndex < 4; tableIndex++) {
    placement = gFieldWork->tables[tableIndex];
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
          if (placedActorId == id) {
#ifdef UBFIX
            return placement;
#else
            goto found;
#endif
          }
          placedActorId++;
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
/// The flag is the low 12 bits, since GameFlag_Get ignores bit 12.
///
/// Used for every condition column of a scene's tables: triggers
/// (Scene_FindTrigger, Field_ExamineCell), actor placements
/// (Event_SpawnActorPlacements), exit regions
/// (Field_CheckExitRegions) and item triggers.
s32 GameFlag_IsConditionActive(s32 condition) {
  s32 flagSet;

  if (condition == -1) {
    return 1;
  }
  if (0x1000 & condition) {
    return GameFlag_GetFar(condition);
  }
  flagSet = GameFlag_GetFar(condition);
  return flagSet == 0;
}

/// Returns 1 for an examine trigger (TRIGGER_EXAMINE) that holds whatever its
/// condition: one with more flags than its kind, unless its result is a mimic
/// battle (type 0x005 in its top 12 bits).
///
/// Called by Scene_FindTrigger. Field_ExamineCell tests such an event's
/// condition itself (a chest's flag is set once it is opened), so the event is
/// found either way and the leader still checks the spot; a mimic is found only
/// while its condition holds.
u32 Scene_ExamineTriggerIgnoresCondition(s32 flags, s32 result) {
  u32 mimicDifference;
  s32 searchFlags;
  u32 ignoresCondition;

  if ((flags & 0xf) != TRIGGER_EXAMINE)
    return 0;
  searchFlags = flags & 0x1ff;
  ignoresCondition = 0;
  if (searchFlags != TRIGGER_EXAMINE) {
    mimicDifference = (result & 0xfff00000) ^ FIND_MIMIC;
    ignoresCondition = mimicDifference | (0U - mimicDifference);
    // 1 when mimicDifference is not 0.
    ignoresCondition >>= 31;
  }
  return ignoresCondition;
}

/// Returns the first of the scene's triggers of kind for target whose condition
/// holds, or NULL. STEP_TRIGGER_FLAG_WALKING also needs the leader to have
/// pushed against it for more than 12 frames (2 with control bit 11);
/// TRIGGER_FLAG_FACING needs the leader to face within an eighth of a turn of
/// the direction in the top four bits.
///
/// Called by every lookup of a scene's triggers in this part and by
/// Actor_GetTriggerCellAheadOfCurrent. Changes nothing.
struct TriggerView *Scene_FindTrigger(s32 kind, s32 target) {
  struct FieldWork *work = gFieldWork;
  struct TriggerView *trigger = (struct TriggerView *)work->triggers;
  s32 flags;
  u32 facing = ((struct Actor *)Actor_Lookup(gAdventure.leaderActorId))->facing;

  flags = trigger->flags;
  // Preserve the full control word: direction and push-delay bits refine kind.
  while (flags != -1) {
    if ((flags & 0xf) == kind && trigger->value == target &&
        (Scene_ExamineTriggerIgnoresCondition(flags, trigger->result) != 0 ||
         (GameFlag_IsConditionActive(trigger->condition) != 0 &&
          (flags = trigger->flags, 1)))) {
      s32 matches = 0;
      s32 minimumPushFrames = 12;
      s32 touchRequirements;

      if ((flags & STEP_TRIGGER_FLAG_QUICK) != 0)
        minimumPushFrames = 2;
      touchRequirements =
          flags & (STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING);

      switch (touchRequirements) {
      case 0:
        matches = 1;
        break;
      case STEP_TRIGGER_FLAG_WALKING:
        if (work->blockedSteps > minimumPushFrames) {
          matches = 1;
        }
        break;
      case TRIGGER_FLAG_FACING:
        // Bits 15 to 12 are a facing in sixteenths of a turn; the leader's
        // facing must be within 0x1fff (an eighth of a turn) of it.
        flags &= 0xf000;
        matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        break;
      case STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING:
        if (work->blockedSteps > minimumPushFrames) {
          flags &= 0xf000;
          matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        }
        break;
      }
      if (matches) {
        return trigger;
      }
    }
    trigger++;
    if (trigger->flags == -1)
      break;
    flags = trigger->flags;
  }
  return 0;
}

/// Unused: runs the loaded overlay's initialize entry; nothing calls it.
UNUSED void Overlay_CallInitialize(void) { gOverlayArea.initialize(); }

/// Returns the talk trigger of actor id, or 0: for the actor
/// stopped with Halt
/// (gAdventure.haltedActor), its TRIGGER_TALK_HALTED event if it has one,
/// the event for talking to it while it is stopped (Crossbone Isle's Bane has
/// one).
///
/// Called by SceneSequence_Run for the A button, to choose between talking and
/// searching, and by the field Psynergy dispatch, which Halts only an
/// actor with a talk trigger; both test the result only for non-zero.
s32 Scene_FindActorTalkTrigger(s32 id) {
  struct TriggerView *talkTrigger = Scene_FindTrigger(TRIGGER_TALK, id);
  s32 pausedActorId = gAdventure.haltedActor;

  if (pausedActorId == id) {
    struct TriggerView *pausedTrigger =
        Scene_FindTrigger(TRIGGER_TALK_HALTED, pausedActorId);

    if (pausedTrigger != 0) {
      return (s32)pausedTrigger;
    }
  }
  return (s32)talkTrigger;
}

/// Talks to actor id: shows its event's message or calls its event
/// function, with the actor stopped and turned as its placement says.
/// For the actor stopped with Halt, its TRIGGER_TALK_HALTED event
/// comes first; without one, a talk trigger that is a function gives one of two
/// lines at random instead, from MsgPonderEllipsis on (dots spoken with
/// pauses). Returns 0 when an event ran, -1 when there was none or such a line
/// stood in for it; SceneSequence_Run ignores the result.
///
/// Called by SceneSequence_Run for its talk request (work->talk). Resumes
/// the stopped actor and sets gAdventure.haltedActor back to -1
/// afterwards.
s32 Scene_TalkToActor(s32 id) {
  struct ActorPlacement *placement = Scene_FindActorPlacement(id);
  s32 result = -1;
  struct Actor *actor = Actor_GetById(id);
  struct TriggerView *trigger;
  s32 actorPaused = 0;
  u32 savedAngle;
  u32 talkFacing;
  s32 usePausedDialogue = 0;

  if (gAdventure.haltedActor == id) {
    actorPaused = 1;
    trigger = Scene_FindTrigger(7, id);
    if (trigger == 0) {
      trigger = Scene_FindTrigger(TRIGGER_TALK, id);
      usePausedDialogue = 1;
      if (trigger == 0) {
        return -1;
      }
      // A function talk trigger is not run for a stopped actor: it says
      // one of two lines picked at random from the pair its definition flags
      // choose.
      if (trigger->result >= 0x10000) {
        s32 definitionFlags = Speaker_GetIdleFlags(id);
        u32 random = Random16();
        s32 message =
            MsgPonderEllipsis + definitionFlags * 2 + (random * 2 >> 16);
        Event_Start();
        Event_SetMessageId(message);
        Event_ShowMessageAndWait(id, 0);
        Event_End();
        goto finish;
      }
    }
  } else {
    trigger = Scene_FindTrigger(TRIGGER_TALK, id);
  }

  if (trigger != 0 && trigger->result != 0) {
    if (!actorPaused) {
      // Stop the actor; with talk facing 0, 1 or 3, also stop the leader
      // and make each watch the other.
      u8 *movementState = &actor->paused;

      *movementState = 1;
      Actor_SetAnimationSpeedFar(actor, 0);
      savedAngle = actor->facing;
      talkFacing = (u8)placement->talkFacing << 24;
      if (talkFacing <= (1 << 24) || talkFacing == (3 << 24)) {
        struct Actor *leader = Actor_GetById(gAdventure.leaderActorId);

        leader->targetX = leader->x.fixed;
        leader->targetY = leader->y.fixed;
        leader->targetZ = leader->z.fixed;
        SET_VELOCITY(leader, 0, 0, 0);
        Actor_TurnToFaceEachOther(id, gAdventure.leaderActorId, 0);
      }
    }
    // A result below 0x10000 is a message; above, a function to call.
    if (trigger->result < 0x10000) {
      Message_SetDecodeFlagsFar(usePausedDialogue);
      Event_Start();
      Event_SetMessageId(trigger->result);
      Event_ShowMessageAndWait(id, 0);
      Event_End();
    } else {
      Leader_StopMoving();
      ((void (*)(s32))trigger->result)(id);
    }
    if (!actorPaused) {
      // An actor left idle (script command 16) goes back to what its
      // talk facing says: following the leader (3) or its old angle (1).
      if (actor->script[actor->scriptCursor] == 16) {
        if ((s8)placement->talkFacing == 3) {
          actor->watchedActor = Actor_GetById(gAdventure.leaderActorId);
          actor->steeringFlags |= 1;
          Actor_SetMovementType(actor, (s32)gActor_FaceWatchedScript);
        } else if ((s8)placement->talkFacing == 1) {
          actor->timer = savedAngle;
          Actor_StartScriptFar(actor, (const s32 *)gActor_StepAngleScript);
        }
      }
      actor->paused = 0;
      Actor_SetAnimationSpeedFar(actor, 16);
    }
    result = 0;
  }
finish:
  // Both a normal event and the paused actor's fallback line release
  // the pause.
  if (actorPaused) {
    FieldPsynergy_ResumeHaltedActor(gAdventure.haltedActor);
    gAdventure.haltedActor = -1;
  }
  return result;
}

/// Runs the scene's raised trigger (TRIGGER_RAISED) for value: its message or
/// its function. Returns 0 when an event ran, -1 when there was none.
///
/// Called by SceneSequence_Run for a positive event request (work->event).
s32 Scene_RunRaisedTrigger(s32 value) {
  s32 result;
  s32 handled;
  struct TriggerView *trigger;

  trigger = Scene_FindTrigger(TRIGGER_RAISED, value);
  handled = -1;
  if (trigger != NULL) {
    result = trigger->result;
    if (result != 0) {
      if (result < 0x10000) {
        Event_Start();
        Event_SetMessageId(trigger->result);
        Event_ShowMessageAndWait(-1, 0);
        handled = 0;
        Event_End();
      } else {
        ((void (*)(s32))result)(value);
        handled = 0;
      }
    }
  }
  return handled;
}

/// Runs the step trigger (TRIGGER_STEP) of what the leader touched, or else
/// takes its exit (TRIGGER_EXIT): plays the exit's sound (and effect) and
/// records where it leads. Returns 0 when something ran, -1 when there was
/// nothing.
///
/// Called by SceneSequence_Run for its step request (work->stepTrigger), which
/// Field_UpdateWalking raises for CELL_BEHAVIOR_EVENT_FIRST to
/// CELL_BEHAVIOR_EVENT_LAST. Sets work->exit for an exit, and clears
/// work->blockedSteps after a step trigger that needs pushing.
s32 Scene_RunStepTriggerOrExit(s32 value) {
  struct TriggerView *touch;
  struct TriggerView *exit;
  s32 exitId;
  s32 handled;
  struct FieldWork *work;

  touch = Scene_FindTrigger(TRIGGER_STEP, value);
  handled = -1;
  work = gFieldWork;
  if ((touch != 0) && (touch->result != 0)) {
    if (touch->flags & STEP_TRIGGER_FLAG_WALKING) {
      work->blockedSteps = 0;
    }
    if (touch->result < 0x10000) {
      Event_Start();
      Event_SetMessageId(touch->result);
      Event_ShowMessageAndWait(-1, 0);
      handled = 0;
      Event_End();
    } else {
      ((void (*)(s32))touch->result)(value);
      handled = 0;
    }
  } else {
    exit = Scene_FindTrigger(TRIGGER_EXIT, value);
    if (exit != 0) {
      // An enum ExitStyle chooses how the leader leaves; the animated styles'
      // flags make the next scene play the matching arrival.
      exitId = exit->flags & EXIT_STYLE_MASK;
      switch (exitId) {
      case EXIT_STYLE_NORMAL:
        Sound_PlayFar(SE_MAP_EXIT);
        break;
      case EXIT_STYLE_STAIRS_UP:
        Sound_PlayFar(SE_STAIRS_UP);
        Exit_StartStairsUp();
        break;
      case EXIT_STYLE_STAIRS_DOWN:
        Sound_PlayFar(SE_STAIRS_DOWN);
        Exit_StartStairsDown();
        break;
      }
      work->exit = exit->result;
      handled = 0;
    }
  }
  return handled;
}

/// Runs the event of a cell the leader pushes against (CELL_BEHAVIOR_LEDGE to
/// CELL_BEHAVIOR_JUMP_FORWARD, and CELL_BEHAVIOR_JUMP_FORWARD_UNREACHED), once
/// it has pushed for more than 12 frames; the count then restarts.
///
/// Called by SceneSequence_Run for its push request (work->pushCell), which
/// Field_UpdateWalking raises for CELL_BEHAVIOR_LEDGE to
/// CELL_BEHAVIOR_JUMP_FORWARD. Clears work->blockedSteps when it acts.
s32 Field_RunPushCell(s32 behavior) {
  struct FieldWork *work = gFieldWork;
  s32 leader = gAdventure.leaderActorId;

  switch (behavior) {
  case CELL_BEHAVIOR_LEDGE:
    if (work->blockedSteps > 12 && (gKeysHeld & KEY_DOWN)) {
      Actor_JumpDownLedge(leader, 6, 0);
      work->blockedSteps = 0;
    }
    break;
  case CELL_BEHAVIOR_JUMP_FORWARD_UNREACHED:
  case CELL_BEHAVIOR_JUMP_FORWARD:
    if (work->blockedSteps > 12) {
      Field_TryJumpForward();
      work->blockedSteps = 0;
    }
    break;
  case CELL_BEHAVIOR_CLIMB:
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

/// Searches the cell ahead with the given cell behavior. The fixed search
/// spots, CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL, show their two messages.
/// Otherwise the scene's examine trigger for the cell runs: a function, a
/// message, or a chest holding a Psynergy Stone, a mimic, coins or an item;
/// with no event, "checked the ground" and "found nothing".
///
/// Called by SceneSequence_Run for its search request (work->tile). Opening a
/// chest sets its condition flag, and it may raise a mimic battle
/// (work->encounter), give coins or an item, or restore all PP.
s32 Field_ExamineCell(s32 tile) {
  const struct Trigger *trigger;
  s32 leader;
  u32 messageOffset;
  s16 condition;
  u32 searchResult;
  s32 rewardType;
  s32 canOpen;

  leader = gAdventure.leaderActorId;
  // Each fixed spot has its own pair of lines, chosen through
  // gField_ExamineTriggerMessageOffsets; the unsigned compare also rejects
  // codes below CELL_BEHAVIOR_JAR.
  messageOffset = tile - CELL_BEHAVIOR_JAR;
  if (messageOffset <= CELL_BEHAVIOR_WELL - CELL_BEHAVIOR_JAR) {
    Leader_StopMoving();
    tile = gField_ExamineTriggerMessageOffsets[messageOffset];
    FieldMessage_ShowFar(MsgTileChecked + tile, FIELD_MESSAGE_ZOOM);
    FieldMessage_ShowFar(MsgTileResult + tile, FIELD_MESSAGE_ZOOM);
  } else {
    trigger = (const struct Trigger *)Scene_FindTrigger(TRIGGER_EXAMINE, tile);
    if (trigger != NULL) {
      s32 flags = trigger->kind;

      // Bits 8 to 4, the search target (enum ExamineTarget), choose what the
      // leader checks ("checked the chest...") unless
      // TRIGGER_SUBJECT_OWN_EFFECT says the event has its own message; if the
      // event leaves FLAG_NOTHING_HAPPENS set, the matching result line follows
      // ("but the chest was empty.").
      condition = trigger->condition;
      messageOffset = (flags >> 4) & 31;
      if ((trigger->subject & TRIGGER_SUBJECT_OWN_EFFECT) == 0 &&
          messageOffset != 0) {
        Leader_StopMoving();
        FieldMessage_ShowFar(MsgTileChecked + messageOffset,
                             FIELD_MESSAGE_ZOOM);
        GameFlag_SetFar(FLAG_NOTHING_HAPPENS);
      } else {
        GameFlag_ClearFar(FLAG_NOTHING_HAPPENS);
      }
      // The value is a function address (bits 27 to 24 set) or a FIND_ value
      // whose top 12 bits give its type: an item, coins, a Psynergy Stone, a
      // message or a mimic.
      searchResult = trigger->value;
      if ((searchResult & 0x0f000000) != 0 ||
          (searchResult & 0xfff00000) == FIND_MESSAGE(0)) {
        if ((searchResult & 0x0f000000) != 0) {
          // A function.
          if (GameFlag_IsConditionActive(condition) != 0)
            ((void (*)(s32))trigger->value)(gAdventure.leaderActorId);
          if (GameFlag_GetFar(FLAG_NOTHING_HAPPENS) != 0)
            FieldMessage_ShowFar(MsgTileResult + messageOffset,
                                 FIELD_MESSAGE_ZOOM);
        } else {
          // A message.
          if (GameFlag_IsConditionActive(condition) != 0)
            FieldMessage_ShowFar((u16)trigger->value, FIELD_MESSAGE_ZOOM);
          else
            FieldMessage_ShowFar(MsgButFoundNothing, FIELD_MESSAGE_ZOOM);
        }
      } else {
        // A chest; its condition flag is set once it has been opened.
        Event_Start();
        if (GameFlag_IsConditionActive(condition) != 0) {
          // Bit 16 of the value marks a chest the party's own characters
          // cannot open ("Can't open it").
          searchResult = trigger->value;
          canOpen = 1;
          if ((searchResult & 0xf0000) == 0x10000 && leader <= 7)
            canOpen = 0;
          if (canOpen != 0) {
            // A EXAMINE_TRIGGER(EXAMINE_CHEST) event is a chest the map shows
            // as a treasure spot.
            if ((trigger->kind & 0x1ff) == EXAMINE_TRIGGER(EXAMINE_CHEST))
              TreasureSpot_OpenChest(tile);
            rewardType = trigger->value & 0xfff00000;
            if (rewardType == FIND_PSYNERGY_STONE) {
              // A Psynergy Stone: restores all PP.
              void *object;

              if ((trigger->kind & 0x1ff) == EXAMINE_TRIGGER(EXAMINE_CHEST))
                TreasureSpot_ShowOpened(tile);
              object = TreasureSpot_GetActor(tile);
              ItemDisplay_RaiseAboveLeader(object);
              Sound_PlayFar(MUS_TREASURE_FOUND);
              MessageArgument_PushFar((u16)trigger->value,
                                      MESSAGE_ARGUMENT_NUMBER);
              FieldMessage_ShowFar(MsgGotPsynergyStone,
                                   FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_WAITING_FOR_SOUND);
              Party_AddPp(999, 0);
              PartyStatus_CreateWindowFar(PARTY_STATUS_SHOW_PP);
              Sound_PlayFar(SE_RECOVERY);
              FieldMessage_ShowFar(MsgGotPsynergyStone + 1, FIELD_MESSAGE_ZOOM);
              PartyStatus_CloseFar();
              Actor_SelectAnimationFar(object, 2);
              Sound_PlayFar(SE_GLITTER);
              Event_Wait(30);
              FieldMessage_ShowFar(MsgGotPsynergyStone + 2, FIELD_MESSAGE_ZOOM);
              TreasureSpot_Hide(tile);
              if (condition != -1)
                GameFlag_SetFar(condition);
            } else if (rewardType == FIND_MIMIC) {
              // A mimic: a battle with its slot of ENCOUNTER_ZONE_MIMIC.
              struct FieldWork *work = gFieldWork;

              if ((trigger->kind & 0x1ff) == EXAMINE_TRIGGER(EXAMINE_CHEST))
                TreasureSpot_LaunchMimic(tile);
              // The chest's flag (with 0x1000) waits in pendingDjinni,
              // which SceneSequence_Run resolves when the scene is entered
              // again after the battle.
              if (condition != -1) {
                condition = gAdventure.pendingDjinni = condition | 0x1000;
              }
              // BATTLE_TYPE_NO_ESCAPE (battle/battle/turn.c): no first strike
              // or surprise, and every attempt to run fails; the command menu
              // still offers it, refusing outright only for
              // BATTLE_TYPE_SCRIPTED and BATTLE_TYPE_LINK.
              work->encounter = Encounter_GetFormationAtSlot(
                  ENCOUNTER_ZONE_MIMIC, (u16)trigger->value);
              Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
              Encounter_SelectBattleMusic(ENCOUNTER_ZONE_MIMIC,
                                          (u16)trigger->value);
              Sound_PlayFar(gAdventure.battleMusic);
              FieldMessage_ShowFar(MsgChestWasMimic, FIELD_MESSAGE_ZOOM);
            } else if (rewardType == FIND_COINS(0)) {
              // Coins.
              void *object;

              object = FieldEffect_StartRandomParticleEmitter(
                  gAdventure.leaderActorId, 0);
              wait(30);
              if ((trigger->kind & 0x1ff) == EXAMINE_TRIGGER(EXAMINE_CHEST))
                TreasureSpot_ShowOpened(tile);
              ItemDisplay_RaiseAboveLeader(object);
              Sound_PlayFar(MUS_TREASURE_FOUND);
              MessageArgument_PushFar((u16)trigger->value,
                                      MESSAGE_ARGUMENT_NUMBER);
              FieldMessage_ShowFar(MsgGotCoins,
                                   FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_WAITING_FOR_SOUND);
              Party_AdjustCoinsFar((u16)trigger->value);
              if (condition != -1)
                GameFlag_SetFar(condition);
              Actor_ReleaseFar(object);
            } else {
              // An item, which stays in the chest when nobody can carry it.
              void *object;
              s32 recipient;

              object = FieldEffect_StartRandomParticleEmitter(
                  gAdventure.leaderActorId, trigger->value & 0xfff);
              wait(30);
              recipient = PartyInventory_AddFar((u16)trigger->value);
              // messageOffset is reused here as the item id's 0xffff mask.
              messageOffset = 0xffff;
              if (recipient == -1) {
                MessageArgument_PushFar(trigger->value & 0xfff,
                                        MESSAGE_ARGUMENT_ITEM);
                FieldMessage_ShowFar(MsgFoundItem, FIELD_MESSAGE_ZOOM);
                FieldMessage_ShowFar(MsgFoundItem + 4, FIELD_MESSAGE_ZOOM);
                Actor_ReleaseIfPresent(object);
                if ((trigger->kind & 0x1ff) == EXAMINE_TRIGGER(EXAMINE_CHEST))
                  TreasureSpot_CloseChest(tile);
              } else {
                if ((trigger->kind & 0x1ff) == EXAMINE_TRIGGER(EXAMINE_CHEST))
                  TreasureSpot_ShowOpened(tile);
                ItemDisplay_RaiseAboveLeader(object);
                Sound_PlayFar(MUS_TREASURE_FOUND);
                MessageArgument_PushFar(trigger->value & messageOffset,
                                        MESSAGE_ARGUMENT_ITEM);
                if (recipient == gAdventure.leaderActorId) {
                  FieldMessage_ShowFar(MsgColossoIsaacGotItem,
                                       FIELD_MESSAGE_ZOOM |
                                           FIELD_MESSAGE_WAITING_FOR_SOUND);
                } else {
                  MessageArgument_PushFar(recipient, MESSAGE_ARGUMENT_ACTOR);
                  FieldMessage_ShowFar(MsgGaveItemToMember,
                                       FIELD_MESSAGE_ZOOM |
                                           FIELD_MESSAGE_WAITING_FOR_SOUND);
                }
                if (condition != -1)
                  GameFlag_SetFar(condition);
                Actor_ReleaseFar(object);
              }
            }
          } else {
            FieldMessage_ShowFar(MsgCantOpenIt, FIELD_MESSAGE_ZOOM);
          }
        } else {
          // Already opened: the result line for the check message.
          FieldMessage_ShowFar(MsgTileResult + messageOffset,
                               FIELD_MESSAGE_ZOOM);
        }
        Event_End();
        Scene_PlayQueuedMusic();
      }
    } else {
      FieldMessage_ShowFar(MsgCheckedGround, FIELD_MESSAGE_ZOOM);
      FieldMessage_ShowFar(MsgGroundDidntFindAnything, FIELD_MESSAGE_ZOOM);
    }
    GameFlag_ClearFar(FLAG_NOTHING_HAPPENS);
  }
  return 0;
}
