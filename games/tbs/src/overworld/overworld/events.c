/// @file
/// Finding and running a map's events: talking, coord events, bg events.
// Part of overworld.c.

#include "../overworld_internal.h"

/// Returns the actor template of actor id in the map's
/// actor template tables, or NULL: for a party character (0 to 7) the row with
/// its sprite, and for a placed actor (8 on) the row it was created
/// from, counting the placed rows in order.
///
/// Called by Scene_TalkToActor for the talk facing, and by the field
/// Psynergy effects that read an actor's template (Mind Read, the
/// Djinni effect, the script messages and the view).
struct ActorTemplate *Scene_FindActorTemplate(s32 id) {
  struct ActorTemplate *template;
  s32 tableIndex;
  s32 placedActorId = ACTOR_FIRST_PLACED;

  for (tableIndex = 0; tableIndex < 4; tableIndex++) {
    template = gFieldWork->tables[tableIndex];
    if (template == NULL)
      continue;
    if (id <= 7) {
      for (; (s16)template->sprite != -1; template++) {
        if ((s16)template->sprite == id) {
#ifdef UBFIX
          return template;
#else
          goto found;
#endif
        }
      }
    } else {
      for (; (s16)template->sprite != -1; template++) {
        if ((s16)template->sprite > 7) {
          if (placedActorId == id) {
#ifdef UBFIX
            return template;
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
  if ((s16)template->sprite == -1)
    return NULL;
  return template;
#endif
}

/// Returns whether an event condition holds: -1 always holds; a condition with
/// bit 0x1000 holds while that flag is set, any other while its flag is clear.
/// The flag is the low 12 bits, since GameFlag_Get ignores bit 12.
///
/// Used for every condition column of a map's tables: events
/// (Scene_FindEvent, Field_SearchCell), actor templates
/// (FieldScript_SpawnActorTemplates), exit regions
/// (Field_CheckExitRegions) and item events.
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

/// Returns 1 for a bg event (MAP_EVENT_BG) that holds whatever its
/// condition: one with more flags than its kind, unless its result is a mimic
/// battle (type 0x005 in its top 12 bits).
///
/// Called by Scene_FindEvent. Field_SearchCell tests such an event's condition
/// itself (a chest's flag is set once it is opened), so the event is found
/// either way and the leader still checks the spot; a mimic is found only
/// while its condition holds.
u32 Scene_BgEventIgnoresCondition(s32 flags, s32 result) {
  u32 mimicDifference;
  s32 searchFlags;
  u32 ignoresCondition;

  if ((flags & 0xf) != MAP_EVENT_BG)
    return 0;
  searchFlags = flags & 0x1ff;
  ignoresCondition = 0;
  if (searchFlags != MAP_EVENT_BG) {
    mimicDifference = (result & 0xfff00000) ^ FIND_MIMIC;
    ignoresCondition = mimicDifference | (0U - mimicDifference);
    // 1 when mimicDifference is not 0.
    ignoresCondition >>= 31;
  }
  return ignoresCondition;
}

/// Returns the first of the map's events of kind for target whose condition
/// holds, or NULL. COORD_EVENT_FLAG_WALKING also needs the leader to have
/// pushed against it for more than 12 frames (2 with control bit 11);
/// MAP_EVENT_FLAG_FACING needs the leader to face within an eighth of a turn of
/// the direction in the top four bits.
///
/// Called by every lookup of a map's events in this part and by
/// Actor_GetTriggerTileAheadOfCurrent. Changes nothing.
struct MapEventView *Scene_FindEvent(s32 kind, s32 target) {
  struct FieldWork *work = gFieldWork;
  struct MapEventView *event = (struct MapEventView *)work->events;
  s32 flags;
  u32 facing =
      ((struct Actor *)Actor_GetByActorId(gSaveBlock.leaderActorId))->facing;

  flags = event->flags;
  // Preserve the full control word: direction and push-delay bits refine kind.
  while (flags != -1) {
    if ((flags & 0xf) == kind && event->value == target &&
        (Scene_BgEventIgnoresCondition(flags, event->result) != 0 ||
         (GameFlag_IsConditionActive(event->condition) != 0 &&
          (flags = event->flags, 1)))) {
      s32 matches = 0;
      s32 minimumPushFrames = 12;
      s32 touchRequirements;

      if ((flags & COORD_EVENT_FLAG_QUICK) != 0)
        minimumPushFrames = 2;
      touchRequirements =
          flags & (COORD_EVENT_FLAG_WALKING | MAP_EVENT_FLAG_FACING);

      switch (touchRequirements) {
      case 0:
        matches = 1;
        break;
      case COORD_EVENT_FLAG_WALKING:
        if (work->blockedSteps > minimumPushFrames) {
          matches = 1;
        }
        break;
      case MAP_EVENT_FLAG_FACING:
        // Bits 15 to 12 are a facing in sixteenths of a turn; the leader's
        // facing must be within 0x1fff (an eighth of a turn) of it.
        flags &= 0xf000;
        matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        break;
      case COORD_EVENT_FLAG_WALKING | MAP_EVENT_FLAG_FACING:
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
UNUSED void Overlay_CallInitialize(void) { gOverlayArea.initialize(); }

/// Returns the talk event of actor id, or 0: for the actor
/// stopped with Halt
/// (gSaveBlock.pausedObject), its MAP_EVENT_TALK_HALTED event if it has one,
/// the event for talking to it while it is stopped (Crossbone Isle's Bane has
/// one).
///
/// Called by SceneManager_Run for the A button, to choose between talking and
/// searching, and by the field Psynergy dispatch, which Halts only an
/// actor with a talk event; both test the result only for non-zero.
s32 Scene_FindActorTalkEvent(s32 id) {
  struct MapEventView *talkEvent = Scene_FindEvent(MAP_EVENT_TALK, id);
  s32 pausedActorId = gSaveBlock.pausedObject;

  if (pausedActorId == id) {
    struct MapEventView *pausedEvent =
        Scene_FindEvent(MAP_EVENT_TALK_HALTED, pausedActorId);

    if (pausedEvent != 0) {
      return (s32)pausedEvent;
    }
  }
  return (s32)talkEvent;
}

/// Talks to actor id: shows its event's message or calls its event
/// function, with the actor stopped and turned as its template says.
/// For the actor stopped with Halt, its MAP_EVENT_TALK_HALTED event
/// comes first; without one, a talk event that is a function gives one of two
/// lines at random instead, from MsgPonderEllipsis on (dots spoken with
/// pauses). Returns 0 when an event ran, -1 when there was none or such a line
/// stood in for it; SceneManager_Run ignores the result.
///
/// Called by SceneManager_Run for its talk request (work->talk). Resumes
/// the stopped actor and sets gSaveBlock.pausedObject back to -1
/// afterwards.
s32 Scene_TalkToActor(s32 id) {
  struct ActorTemplate *template = Scene_FindActorTemplate(id);
  s32 result = -1;
  struct Object *actor = (struct Object *)Actor_GetById(id);
  struct MapEventView *event;
  s32 actorPaused = 0;
  u32 savedAngle;
  u32 talkFacing;
  s32 usePausedDialogue = 0;

  if (gSaveBlock.pausedObject == id) {
    actorPaused = 1;
    event = Scene_FindEvent(7, id);
    if (event == 0) {
      event = Scene_FindEvent(MAP_EVENT_TALK, id);
      usePausedDialogue = 1;
      if (event == 0) {
        return -1;
      }
      // A function talk event is not run for a stopped actor: it says
      // one of two lines picked at random from the pair its definition flags
      // choose.
      if (event->result >= 0x10000) {
        s32 definitionFlags = Speaker_GetIdleFlags(id);
        u32 random = Random16();
        s32 message =
            MsgPonderEllipsis + definitionFlags * 2 + (random * 2 >> 16);
        FieldScript_Start();
        FieldScript_SetMessageId(message);
        FieldScript_ShowMessageAndWait(id, 0);
        FieldScript_End();
        goto finish;
      }
    }
  } else {
    event = Scene_FindEvent(MAP_EVENT_TALK, id);
  }

  if (event != 0 && event->result != 0) {
    if (!actorPaused) {
      // Stop the actor; with talk facing 0, 1 or 3, also stop the leader
      // and link the two.
      u8 *movementState = &actor->paused;

      *movementState = 1;
      Object_SetAnimationSpeedFar((struct Object *)actor, 0);
      savedAngle = actor->facing;
      talkFacing = (u8)template->talkFacing << 24;
      if (talkFacing <= (1 << 24) || talkFacing == (3 << 24)) {
        struct Object *leader =
            (struct Object *)Actor_GetById(gSaveBlock.leaderActorId);

        leader->targetX = leader->x;
        leader->targetY = leader->y;
        leader->targetZ = leader->z;
        SET_VELOCITY(leader, 0, 0, 0);
        Actor_LinkPair(id, gSaveBlock.leaderActorId, 0);
      }
    }
    // A result below 0x10000 is a message; above, a function to call.
    if (event->result < 0x10000) {
      Message_SetDecodeFlagsFar(usePausedDialogue);
      FieldScript_Start();
      FieldScript_SetMessageId(event->result);
      FieldScript_ShowMessageAndWait(id, 0);
      FieldScript_End();
    } else {
      Leader_StopMoving();
      ((void (*)(s32))event->result)(id);
    }
    if (!actorPaused) {
      // An actor left idle (script command 16) goes back to what its
      // talk facing says: following the leader (3) or its old angle (1).
      if (actor->script[actor->scriptCursor] == 16) {
        if ((s8)template->talkFacing == 3) {
          actor->linkedObject =
              (struct Object *)Actor_GetById(gSaveBlock.leaderActorId);
          actor->steeringFlags |= 1;
          Actor_SetMovementType(actor, (s32)gObjectMotion_LinkedActionScript);
        } else if ((s8)template->talkFacing == 1) {
          actor->action = savedAngle;
          Object_InitializeFar((struct Object *)actor,
                               (const s32 *)gObjectMotion_StepAngleScript);
        }
      }
      actor->paused = 0;
      Object_SetAnimationSpeedFar((struct Object *)actor, 16);
    }
    result = 0;
  }
finish:
  // Both a normal event and the paused actor's fallback line release
  // the pause.
  if (actorPaused) {
    FieldEffect_ResumeObject(gSaveBlock.pausedObject);
    gSaveBlock.pausedObject = -1;
  }
  return result;
}

/// Runs the map's raised event (MAP_EVENT_RAISED) for value: its message or
/// its function. Returns 0 when an event ran, -1 when there was none.
///
/// Called by SceneManager_Run for a positive event request (work->event).
s32 Scene_RunRaisedEvent(s32 value) {
  s32 result;
  s32 handled;
  struct MapEventView *event;

  event = Scene_FindEvent(MAP_EVENT_RAISED, value);
  handled = -1;
  if (event != NULL) {
    result = event->result;
    if (result != 0) {
      if (result < 0x10000) {
        FieldScript_Start();
        FieldScript_SetMessageId(event->result);
        FieldScript_ShowMessageAndWait(-1, 0);
        handled = 0;
        FieldScript_End();
      } else {
        ((void (*)(s32))result)(value);
        handled = 0;
      }
    }
  }
  return handled;
}

/// Runs the coord event (MAP_EVENT_COORD) of what the leader touched, or else
/// takes its exit (MAP_EVENT_EXIT): plays the exit's sound (and effect) and
/// records where it leads. Returns 0 when something ran, -1 when there was
/// nothing.
///
/// Called by SceneManager_Run for its step request (work->action), which
/// Field_ProcessStep raises for MB_EVENT_FIRST to MB_EVENT_LAST.
/// Sets work->exit for an exit, and clears work->blockedSteps after a coord
/// event that needs pushing.
s32 Scene_RunCoordEventOrExit(s32 value) {
  struct MapEventView *touch;
  struct MapEventView *exit;
  s32 exitId;
  s32 handled;
  struct FieldWork *work;

  touch = Scene_FindEvent(MAP_EVENT_COORD, value);
  handled = -1;
  work = gFieldWork;
  if ((touch != 0) && (touch->result != 0)) {
    if (touch->flags & COORD_EVENT_FLAG_WALKING) {
      work->blockedSteps = 0;
    }
    if (touch->result < 0x10000) {
      FieldScript_Start();
      FieldScript_SetMessageId(touch->result);
      FieldScript_ShowMessageAndWait(-1, 0);
      handled = 0;
      FieldScript_End();
    } else {
      ((void (*)(s32))touch->result)(value);
      handled = 0;
    }
  } else {
    exit = Scene_FindEvent(MAP_EVENT_EXIT, value);
    if (exit != 0) {
      // An enum ExitStyle chooses how the leader leaves; the animated styles'
      // flags make the next map play the matching arrival.
      exitId = exit->flags & EXIT_STYLE_MASK;
      switch (exitId) {
      case EXIT_STYLE_NORMAL:
        Sound_PlaySongFar(SE_MAP_EXIT);
        break;
      case EXIT_STYLE_STAIRS_UP:
        Sound_PlaySongFar(SE_STAIRS_UP);
        Exit_StartStairsUp();
        break;
      case EXIT_STYLE_STAIRS_DOWN:
        Sound_PlaySongFar(SE_STAIRS_DOWN);
        Exit_StartStairsDown();
        break;
      }
      work->exit = exit->result;
      handled = 0;
    }
  }
  return handled;
}

/// Runs the event of a cell the leader pushes against (MB_LEDGE to
/// MB_JUMP_FORWARD, and MB_JUMP_FORWARD_UNREACHED), once it has
/// pushed for more than 12 frames; the count then restarts.
///
/// Called by SceneManager_Run for its push request (work->pushCell), which
/// Field_ProcessStep raises for MB_LEDGE to MB_JUMP_FORWARD.
/// Clears work->blockedSteps when it acts.
s32 Field_RunPushCellEvent(s32 behavior) {
  struct FieldWork *work = gFieldWork;
  s32 leader = gSaveBlock.leaderActorId;

  switch (behavior) {
  case MB_LEDGE:
    if (work->blockedSteps > 12 && (gKeysHeld & KEY_DOWN)) {
      Actor_JumpDownLedge(leader, 6, 0);
      work->blockedSteps = 0;
    }
    break;
  case MB_JUMP_FORWARD_UNREACHED:
  case MB_JUMP_FORWARD:
    if (work->blockedSteps > 12) {
      Field_TryJumpForward();
      work->blockedSteps = 0;
    }
    break;
  case MB_CLIMB:
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

/// Searches the cell ahead with the given metatile behavior. The fixed search
/// spots, MB_JAR to MB_WELL, show their two messages. Otherwise the
/// map's bg event for the cell runs: a function, a message, or a chest
/// holding a Psynergy Stone, a mimic, coins or an item; with no event, "checked
/// the ground" and "found nothing".
///
/// Called by SceneManager_Run for its search request (work->tile). Opening a
/// chest sets its condition flag, and it may raise a mimic battle
/// (work->battle), give coins or an item, or restore all PP.
s32 Field_SearchCell(s32 tile) {
  const struct MapEvent *event;
  s32 leader;
  u32 messageOffset;
  s16 condition;
  u32 searchResult;
  s32 rewardType;
  s32 canOpen;

  leader = gSaveBlock.leaderActorId;
  // Each fixed spot has its own pair of lines, chosen through
  // gField_BgEventMessageOffsets; the unsigned compare also rejects codes below
  // MB_JAR.
  messageOffset = tile - MB_JAR;
  if (messageOffset <= MB_WELL - MB_JAR) {
    Leader_StopMoving();
    tile = gField_BgEventMessageOffsets[messageOffset];
    FieldMessage_ShowFar(MsgTileChecked + tile, FIELD_MESSAGE_ZOOM);
    FieldMessage_ShowFar(MsgTileResult + tile, FIELD_MESSAGE_ZOOM);
  } else {
    event = (const struct MapEvent *)Scene_FindEvent(MAP_EVENT_BG, tile);
    if (event != NULL) {
      s32 flags = event->kind;

      // Bits 8 to 4, the search target (enum BgEventTarget), choose what the
      // leader checks ("checked the chest...") unless
      // MAP_EVENT_TRIGGER_OWN_EFFECT says the event has its own message; if the
      // event leaves FLAG_NOTHING_HAPPENS set, the matching result line follows
      // ("but the chest was empty.").
      condition = event->condition;
      messageOffset = (flags >> 4) & 31;
      if ((event->trigger & MAP_EVENT_TRIGGER_OWN_EFFECT) == 0 &&
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
      searchResult = event->value;
      if ((searchResult & 0x0f000000) != 0 ||
          (searchResult & 0xfff00000) == FIND_MESSAGE(0)) {
        if ((searchResult & 0x0f000000) != 0) {
          // A function.
          if (GameFlag_IsConditionActive(condition) != 0)
            ((void (*)(s32))event->value)(gSaveBlock.leaderActorId);
          if (GameFlag_GetFar(FLAG_NOTHING_HAPPENS) != 0)
            FieldMessage_ShowFar(MsgTileResult + messageOffset,
                                 FIELD_MESSAGE_ZOOM);
        } else {
          // A message.
          if (GameFlag_IsConditionActive(condition) != 0)
            FieldMessage_ShowFar((u16)event->value, FIELD_MESSAGE_ZOOM);
          else
            FieldMessage_ShowFar(MsgButFoundNothing, FIELD_MESSAGE_ZOOM);
        }
      } else {
        // A chest; its condition flag is set once it has been opened.
        FieldScript_Start();
        if (GameFlag_IsConditionActive(condition) != 0) {
          // Bit 16 of the value marks a chest the party's own characters
          // cannot open ("Can't open it").
          searchResult = event->value;
          canOpen = 1;
          if ((searchResult & 0xf0000) == 0x10000 && leader <= 7)
            canOpen = 0;
          if (canOpen != 0) {
            // A BG_EVENT(BG_EVENT_CHEST) event is a chest the map shows as a
            // treasure spot.
            if ((event->kind & 0x1ff) == BG_EVENT(BG_EVENT_CHEST))
              TreasureSpot_OpenChest(tile);
            rewardType = event->value & 0xfff00000;
            if (rewardType == FIND_PSYNERGY_STONE) {
              // A Psynergy Stone: restores all PP.
              void *object;

              if ((event->kind & 0x1ff) == BG_EVENT(BG_EVENT_CHEST))
                TreasureSpot_ShowOpened(tile);
              object = TreasureSpot_GetActor(tile);
              ItemDisplay_RaiseAboveLeader(object);
              Sound_PlaySongFar(MUS_TREASURE_FOUND);
              MessageArgument_PushFar((u16)event->value,
                                      MESSAGE_ARGUMENT_NUMBER);
              FieldMessage_ShowFar(MsgGotPsynergyStone,
                                   FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_WAITING_FOR_SOUND);
              Party_AddPp(999, 0);
              PartyStatus_CreateWindowFar(PARTY_STATUS_SHOW_PP);
              Sound_PlaySongFar(SE_RECOVERY);
              FieldMessage_ShowFar(MsgGotPsynergyStone + 1, FIELD_MESSAGE_ZOOM);
              PartyStatus_CloseFar();
              Object_SelectAnimationFar(object, 2);
              Sound_PlaySongFar(SE_GLITTER);
              FieldScript_WaitFrames(30);
              FieldMessage_ShowFar(MsgGotPsynergyStone + 2, FIELD_MESSAGE_ZOOM);
              TreasureSpot_Hide(tile);
              if (condition != -1)
                GameFlag_SetFar(condition);
            } else if (rewardType == FIND_MIMIC) {
              // A mimic: a battle with its slot of ENCOUNTER_ZONE_MIMIC.
              struct FieldWork *work = gFieldWork;

              if ((event->kind & 0x1ff) == BG_EVENT(BG_EVENT_CHEST))
                TreasureSpot_LaunchMimic(tile);
              // The chest's flag (with 0x1000) waits in pendingDjinnEvent,
              // which SceneManager_Run resolves when the map is entered again
              // after the battle.
              if (condition != -1) {
                condition = gSaveBlock.pendingDjinnEvent = condition | 0x1000;
              }
              // BATTLE_TYPE_NO_ESCAPE (battle/battle/round.c): no first strike
              // or surprise, and every attempt to run fails; the command menu
              // still offers it, refusing outright only for
              // BATTLE_TYPE_SCRIPTED and BATTLE_TYPE_LINK.
              work->battle = Encounter_GetFormationAtSlot(ENCOUNTER_ZONE_MIMIC,
                                                          (u16)event->value);
              Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
              Encounter_SelectBattleMusic(ENCOUNTER_ZONE_MIMIC,
                                          (u16)event->value);
              Sound_PlaySongFar(gSaveBlock.battleMusic);
              FieldMessage_ShowFar(MsgChestWasMimic, FIELD_MESSAGE_ZOOM);
            } else if (rewardType == FIND_COINS(0)) {
              // Coins.
              void *object;

              object = FieldEffect_StartRandomParticleEmitter(
                  gSaveBlock.leaderActorId, 0);
              WaitFrames(30);
              if ((event->kind & 0x1ff) == BG_EVENT(BG_EVENT_CHEST))
                TreasureSpot_ShowOpened(tile);
              ItemDisplay_RaiseAboveLeader(object);
              Sound_PlaySongFar(MUS_TREASURE_FOUND);
              MessageArgument_PushFar((u16)event->value,
                                      MESSAGE_ARGUMENT_NUMBER);
              FieldMessage_ShowFar(MsgGotCoins,
                                   FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_WAITING_FOR_SOUND);
              Party_AdjustCoinsFar((u16)event->value);
              if (condition != -1)
                GameFlag_SetFar(condition);
              Object_ReleaseFar(object);
            } else {
              // An item, which stays in the chest when nobody can carry it.
              void *object;
              s32 recipient;

              object = FieldEffect_StartRandomParticleEmitter(
                  gSaveBlock.leaderActorId, event->value & 0xfff);
              WaitFrames(30);
              recipient = PartyInventory_AddFar((u16)event->value);
              // messageOffset is reused here as the item id's 0xffff mask.
              messageOffset = 0xffff;
              if (recipient == -1) {
                MessageArgument_PushFar(event->value & 0xfff,
                                        MESSAGE_ARGUMENT_ITEM);
                FieldMessage_ShowFar(MsgFoundItem, FIELD_MESSAGE_ZOOM);
                FieldMessage_ShowFar(MsgFoundItem + 4, FIELD_MESSAGE_ZOOM);
                Object_DestroyIfPresent(object);
                if ((event->kind & 0x1ff) == BG_EVENT(BG_EVENT_CHEST))
                  TreasureSpot_CloseChest(tile);
              } else {
                if ((event->kind & 0x1ff) == BG_EVENT(BG_EVENT_CHEST))
                  TreasureSpot_ShowOpened(tile);
                ItemDisplay_RaiseAboveLeader(object);
                Sound_PlaySongFar(MUS_TREASURE_FOUND);
                MessageArgument_PushFar(event->value & messageOffset,
                                        MESSAGE_ARGUMENT_ITEM);
                if (recipient == gSaveBlock.leaderActorId) {
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
                Object_ReleaseFar(object);
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
        FieldScript_End();
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
