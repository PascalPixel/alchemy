/// @file
/// Finding and running a scene's triggers: talking, step triggers, examine
/// triggers.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "battle_effect/battle_effect.h"
#include "field/event_commands.h"
#include "field/field_sequence.h"
#include "field/actor_script.h"
#include "field/object.h"
#include "shop/shop.h"
#include "window/screens.h"

/// What an examine trigger is on (enum ExamineTarget), the control bits 8..4
/// EXAMINE_TRIGGER() packs it into.
#define EXAMINE_TARGET(kind) (((kind) >> 4) & 0x1f)
/// The PP a Psynergy Stone gives each party member, enough to fill it: "The
/// party's PP is fully restored!"
#define PSYNERGY_STONE_PP 1999
/// The Psynergy Stone sprite's animation once it is used: it breaks into
/// pieces that scatter as sparkles, then is gone (psynergy_stone.png's
/// pictures 7 to 10).
#define ANIM_PSYNERGY_STONE_SHATTER 2

/// The encounter zone whose slots hold the Mimics' formations; Mimics past its
/// ENCOUNTER_ZONE_SLOTS go on in the next zone.
#define ENCOUNTER_ZONE_MIMIC 104
#define ENCOUNTER_ZONE_SLOTS 8

/// Finds actor id's row in the scene's four actor placement tables, or NULL:
/// an id below ACTOR_FIRST_PLACED is the row of that sprite, a later one the
/// row of that place among the placed actors' rows, counted in order.
struct ActionDescriptor *Scene_FindActorPlacement(s32 id) {
  struct ActionDescriptor *entry;
  s32 i;
  s32 group = ACTOR_FIRST_PLACED;

  for (i = 0; i < 4; i++) {
    entry = gFieldState->tables[i];
    if (entry == NULL)
      continue;
    if (id < ACTOR_FIRST_PLACED) {
      for (; (s16)entry->sprite != -1; entry++) {
        if ((s16)entry->sprite == id)
          goto found;
      }
    } else {
      for (; (s16)entry->sprite != -1; entry++) {
        if ((s16)entry->sprite >= ACTOR_FIRST_PLACED) {
          if (group == id)
            goto found;
          group++;
        }
      }
    }
  }
found:
  if ((s16)entry->sprite == -1)
    return NULL;
  return entry;
}

/// Returns whether an event condition holds: -1 always holds; a condition with
/// CONDITION_FLAG_SET holds while that flag is set, any other while its flag is
/// clear.
s32 GameFlag_IsConditionActive(s32 condition) {
  s32 flagSet;

  if (condition == -1) {
    return 1;
  }
  if (CONDITION_FLAG_SET & condition) {
    return GameFlag_Get(condition);
  }
  flagSet = GameFlag_Get(condition);
  return flagSet == 0;
}

/// Returns the first of the scene's triggers of kind for target whose
/// condition holds and whose push (STEP_TRIGGER_FLAG_WALKING) and facing
/// (TRIGGER_FLAG_FACING) requirements are met, or NULL. TRIGGER_PUSH and
/// TRIGGER_PUSHED, the triggers Field_RunPushCell runs for an actor pushed and
/// for the cell pushed against, answer any target; the facing test alone is
/// skipped while gMainRoutineState.movementMode is LEADER_MOVEMENT_KEYS.
struct TriggerView *Scene_FindTrigger(s32 kind, s32 target) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct TriggerView *trigger = state->triggers;
  s32 flags;
  u32 facing = Actor_Lookup(gMainRoutineState.leaderActorId)->facing;

  flags = trigger->flags;
  while (flags != -1) {
    u8 *mode = &gMainRoutineState.movementMode;

    if ((flags & TRIGGER_KIND_MASK) == kind &&
        (trigger->value == target || kind == TRIGGER_PUSH ||
         kind == TRIGGER_PUSHED) &&
        GameFlag_IsConditionActive(trigger->condition)) {
      s32 matches = 0;
      s32 minimumPushFrames = FIELD_PUSH_FRAMES;
      s32 touchRequirements;

      flags = trigger->flags;
      if (flags & STEP_TRIGGER_FLAG_QUICK)
        minimumPushFrames = 2;
      touchRequirements =
          flags & (STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING);

      switch (touchRequirements) {
      case 0:
        matches = 1;
        break;
      case STEP_TRIGGER_FLAG_WALKING:
        if (state->blockedSteps > minimumPushFrames)
          matches = 1;
        break;
      case TRIGGER_FLAG_FACING:
        if (*mode != LEADER_MOVEMENT_KEYS) {
          // Bits 15 to 12 are a facing in sixteenths of a turn; the leader's
          // facing must be within an eighth of a turn of it.
          flags &= TRIGGER_FACING_MASK;
          matches = (u16)((flags - facing) + (TRIG_TURN / 8 - 1)) <=
                    TRIG_TURN / 4 - 2;
        }
        break;
      case STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING:
        if (state->blockedSteps > minimumPushFrames) {
          flags &= TRIGGER_FACING_MASK;
          matches = (u16)((flags - facing) + (TRIG_TURN / 8 - 1)) <=
                    TRIG_TURN / 4 - 2;
        }
        break;
      }
      if (matches)
        return trigger;
    }
    trigger++;
    if (trigger->flags == -1)
      break;
    flags = trigger->flags;
  }
  return NULL;
}

/// Unused: runs the loaded overlay's initialize entry; nothing calls it.
UNUSED void Overlay_CallInitialize(void) { SCENE_ENTRIES->initialize(); }

/// Returns the talk trigger of actor id, or NULL: for the actor Halt has
/// stopped (gMainRoutineState.haltedActor), its TRIGGER_TALK_HALTED trigger if
/// it has one, the event for talking to it while it is stopped.
void *Scene_FindActorTalkTrigger(s32 id) {
  void *found = Scene_FindTrigger(TRIGGER_TALK, id);
  struct MainRoutineState *party = &gMainRoutineState;
  void *event;
  s32 stopped;

  stopped = party->haltedActor;
  event = found;
  if (stopped == id) {
    void *alt = Scene_FindTrigger(TRIGGER_TALK_HALTED, stopped);

    if (alt != NULL)
      return alt;
  }
  return event;
}

s32 Speaker_GetHaltedLines(s32 id);
void Actor_TurnToFaceEachOther(s32 first, s32 second, s32 mode);

/// Talks to actor id. A container (actors from FIELD_TREASURE_ACTOR, and the
/// Scoop spot, SCOOPED_ITEM_ACTOR) is checked instead: a chest gives what it
/// holds, a Mimic starts its battle, a Psynergy Stone restores the party's PP
/// and a stone tablet teaches its summon. Any other actor shows its event's
/// message or calls its event function, stopped and turned as its placement
/// says; the actor Halt has stopped answers its TRIGGER_TALK_HALTED trigger
/// first.
s32 Scene_TalkToActor(s32 id) {
  struct ActorPlacement *placement;
  s32 result;
  struct Actor *talker;
  s32 savedAngle;
  s32 paused;
  s32 decodeFlags;
  s32 silent;
  struct FieldState *field;
  struct Actor *leader;
  struct Actor *shown;
  s16 *row;
  struct TriggerView *talk;
  s32 point;
  u8 *busy;
  s32 found;
  s32 type;
  s32 flag;
  s32 contents;
  s32 index;
  s32 slot;
  struct FieldState *state;

  placement = Scene_FindActorPlacement(id);
  result = -1;
  savedAngle = 0;
  paused = 0;
  decodeFlags = 0;
  silent = id & FIELD_TALK_SILENT;
  field = Ram_HeapSlots->fieldState;
  id &= FIELD_TALK_ACTOR_MASK;
  found = 0;
  talker = Actor_GetById(id);
  if (id >= FIELD_TREASURE_ACTOR) {
    if (id != SCOOPED_ITEM_ACTOR) {
      row = SceneItem_GetRows();
      if (row != NULL) {
        // The actor's row (struct ContainerRow): its type, point, flag and
        // contents.
        row += (id - FIELD_TREASURE_ACTOR) * 4;
        type = *row++;
        index = id;
        found = 1;
        point = *row++;
        flag = *row++;
        contents = *row;
      }
    } else {
      type = CONTAINER_GROUND;
      point = result;
      flag = result;
      contents = field->scoopedItem;
      index = SCOOPED_ITEM_ACTOR;
      found = 1;
    }
    if (found) {
      // found now holds what the search reports (enum ExamineTarget).
      found = type;
      if ((u16)(type - CONTAINER_CHEST) <= CONTAINER_MIMIC - CONTAINER_CHEST)
        found = EXAMINE_CHEST;
      if ((u16)(type - CONTAINER_PSYNERGY_STONE) <=
          CONTAINER_STONE_TABLET - CONTAINER_PSYNERGY_STONE)
        found = EXAMINE_GROUND;
      if (type == CONTAINER_STONE_TABLET)
        found = EXAMINE_STONE_TABLET;
      if (type == CONTAINER_POND)
        found = EXAMINE_POND;
      Event_Start();
      busy = &talker->paused;
      *busy = 1;
      if (silent == 0)
        FieldMessage_ShowFar(found + (s32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
      if (gMainRoutineState.next == SCENE_VENUS_LIGHTHOUSE && point == 0 &&
          !GameFlag_Get(flag)) {
        MessageArgument_PushFar(contents, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar((s32)&MsgChestAlreadyOpen, FIELD_MESSAGE_ZOOM);
      }
      if (!GameFlag_IsConditionActive(flag))
        goto empty;
      if (type == CONTAINER_CHEST) {
        if (contents == 0)
          TreasureSpot_OpenEmptyChest(index);
        else
          TreasureSpot_OpenChest(index);
      }
      if (type == CONTAINER_MIMIC)
        TreasureSpot_OpenChest(index);
      if (type == CONTAINER_PSYNERGY_STONE) {
        struct Actor *stone = TreasureSpot_GetActor(index);

        stone->update = ItemDisplay_EmitSparkles;
        ItemDisplay_RaiseAboveLeader(stone);
        Sound_PlayFar(MUS_TREASURE_FOUND);
        MessageArgument_PushFar(contents, MESSAGE_ARGUMENT_NUMBER);
        FieldMessage_ShowFar((s32)&MsgGotPsynergyStone,
                             FIELD_MESSAGE_ZOOM |
                                 FIELD_MESSAGE_WAITING_FOR_SOUND);
        Party_AddPp(PSYNERGY_STONE_PP, 0);
        PartyStatus_CreateWindowFar(PARTY_STATUS_SHOW_PP);
        Sound_PlayFar(SE_RECOVERY);
        // "<leader>'s PP are maxed out!" alone, else "The party's PP is
        // fully restored!"
        if (Party_CountMembersFar() == 1)
          FieldMessage_ShowFar((s32)&MsgGotPsynergyStone + 6,
                               FIELD_MESSAGE_ZOOM);
        else
          FieldMessage_ShowFar((s32)&MsgGotPsynergyStone + 1,
                               FIELD_MESSAGE_ZOOM);
        PartyStatus_CloseFar();
        stone->update = NULL;
        Actor_SelectAnimationFar(stone, ANIM_PSYNERGY_STONE_SHATTER);
        Sound_PlayFar(SE_GLITTER);
        Event_Wait(30);
        FieldMessage_ShowFar((s32)&MsgPsynergyStoneDisappeared,
                             FIELD_MESSAGE_ZOOM);
        TreasureSpot_Remove(index);
        if (flag != -1)
          GameFlag_Set(flag);
      } else if (type == CONTAINER_STONE_TABLET) {
        Event_Start();
        Event_EndLastingPsynergy(0);
        SummonTablet_PlayRunes(index);
        Summon_ShowLearnedMessageFar(contents);
        Summon_LearnFar(contents);
        TreasureSpot_Remove(index);
        if (flag != -1)
          GameFlag_Set(flag);
        Event_End();
      } else if (type == CONTAINER_MIMIC) {
        state = Ram_HeapSlots->fieldState;
        TreasureSpot_LaunchMimic(index);
        if (flag != -1)
          gMainRoutineState.pendingDjinni = flag | CONDITION_FLAG_SET;
        if (contents > ENCOUNTER_ZONE_SLOTS - 1)
          state->encounter = Encounter_GetFormationAtSlot(
              ENCOUNTER_ZONE_MIMIC + 1, contents - ENCOUNTER_ZONE_SLOTS);
        else
          state->encounter =
              Encounter_GetFormationAtSlot(ENCOUNTER_ZONE_MIMIC, contents);
        gMainRoutineState.battleType = BATTLE_TYPE_NO_ESCAPE;
        Encounter_SelectBattleMusic(ENCOUNTER_ZONE_MIMIC, contents);
        Sound_PlayFar(gMainRoutineState.battleMusic);
        FieldMessage_ShowFar((s32)&MsgChestWasMimic, FIELD_MESSAGE_ZOOM);
      } else {
        if (type == CONTAINER_POND)
          FieldMessage_ShowFar((s32)&MsgSomethingInTheShallows,
                               FIELD_MESSAGE_ZOOM);
        if (Actor_GetSpriteShape(index)) {
          FieldMessage_ShowFar((s32)&MsgSomethingBuriedHere,
                               FIELD_MESSAGE_ZOOM);
          if (Party_FindOwnerWithActionFar(ABILITY_SCOOP) != -1)
            gFieldState->fieldMenu = 1;
        } else if (contents & CONTAINER_COINS) {
          if (type == CONTAINER_GROUND) {
            shown = TreasureSpot_GetActor(index);
            shown->update = ItemDisplay_EmitSparkles;
          } else {
            shown = ItemDisplay_ShowItemInFront(index, 0);
            wait(30);
            if (type == CONTAINER_CHEST)
              TreasureSpot_ShowOpened(index);
          }
          ItemDisplay_RaiseAboveLeader(shown);
          Sound_PlayFar(MUS_TREASURE_FOUND);
          contents &= CONTAINER_COINS - 1;
          MessageArgument_PushFar(contents, MESSAGE_ARGUMENT_NUMBER);
          FieldMessage_ShowFar((s32)&MsgCoinsReceived,
                               FIELD_MESSAGE_ZOOM |
                                   FIELD_MESSAGE_WAITING_FOR_SOUND);
          Party_AdjustCoinsFar(contents);
          if (flag != -1)
            GameFlag_Set(flag);
          if (type == CONTAINER_GROUND) {
            Actor_SetPositionAndResetMotionFar(shown, 0, 0, 0);
            shown->update = NULL;
          } else {
            Actor_ReleaseFar(shown);
          }
        } else if (contents == 0) {
          if (flag != -1)
            GameFlag_Set(flag);
          goto empty;
        } else {
          if (type == CONTAINER_GROUND) {
            shown = TreasureSpot_GetActor(index);
            shown->update = ItemDisplay_EmitSparkles;
          } else {
            shown = ItemDisplay_ShowItemInFront(index, contents);
            wait(30);
          }
          slot = PartyInventory_AddFar(contents);
          if (slot == -1) {
            MessageArgument_PushFar(contents, MESSAGE_ARGUMENT_ITEM);
            FieldMessage_ShowFar((s32)&MsgFoundItem, FIELD_MESSAGE_ZOOM);
            // "But <leader>'s party can't carry any more, so they left it
            // behind."
            FieldMessage_ShowFar((s32)&MsgFoundItem + 4, FIELD_MESSAGE_ZOOM);
            if (type != CONTAINER_GROUND)
              Actor_ReleaseIfPresent(shown);
            if (type == CONTAINER_CHEST)
              TreasureSpot_CloseChest(index);
          } else {
            if (type == CONTAINER_CHEST)
              TreasureSpot_ShowOpened(index);
            ItemDisplay_RaiseAboveLeader(shown);
            Sound_PlayFar(MUS_TREASURE_FOUND);
            MessageArgument_PushFar(contents, MESSAGE_ARGUMENT_ITEM);
            if (slot == gMainRoutineState.leaderActorId) {
              FieldMessage_ShowFar((s32)&MsgItemReceived,
                                   FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_WAITING_FOR_SOUND);
            } else {
              MessageArgument_PushFar(slot, MESSAGE_ARGUMENT_ACTOR);
              FieldMessage_ShowFar((s32)&MsgItemGiven,
                                   FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_WAITING_FOR_SOUND);
            }
            if (flag != -1)
              GameFlag_Set(flag);
            if (type == CONTAINER_GROUND) {
              Actor_SetPositionAndResetMotionFar(shown, 0, 0, 0);
              shown->update = NULL;
            } else {
              Actor_ReleaseFar(shown);
            }
            if (type == CONTAINER_POND)
              TreasureSpot_Remove(index);
            result = 0;
          }
        }
      }
      goto done;
    empty:
      FieldMessage_ShowFar(found + (s32)&MsgTileResult, FIELD_MESSAGE_ZOOM);
    done:
      *busy = 0;
      Event_End();
      if (result != 0)
        return result;
    }
  }

  if (gMainRoutineState.haltedActor == id) {
    paused = 1;
    talk = Scene_FindTrigger(TRIGGER_TALK_HALTED, id);
    if (talk == NULL) {
      // With no halted line of its own, a halted actor's plain talk comes out
      // as halting speech, broken off after its first word.
      talk = Scene_FindTrigger(TRIGGER_TALK, id);
      decodeFlags = MESSAGE_DECODE_HALTING_SPEECH;
      if (talk == NULL)
        return result;
      // A function talk trigger is not run for a stopped actor: it says one
      // of the two lines of its sprite's Halted pair, at random.
      if (talk->result >= TRIGGER_RESULT_MESSAGE_END) {
        Event_RunMessage(id, Speaker_GetHaltedLines(id) * 2 +
                                 ((Random16() << 1) >> 16) +
                                 (s32)&MsgPonderEllipsis);
        goto resume;
      }
    }
  } else {
    talk = Scene_FindTrigger(TRIGGER_TALK, id);
  }
  if (talk != NULL && talk->result != 0) {
    if (!paused) {
      talker->paused = 1;
      Actor_SetAnimationSpeedFar(talker, 0);
      savedAngle = talker->facing;
      if ((u8)placement->talkFacing <= TALK_FACING_PARTY_AND_BACK ||
          (s8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
        leader = Actor_GetById(gMainRoutineState.leaderActorId);
        Actor_SetPositionAndResetMotionFar(leader, leader->x.fixed,
                                           leader->y.fixed, leader->z.fixed);
        Actor_TurnToFaceEachOther(id, gMainRoutineState.leaderActorId, 0);
      }
    }
    if (talk->result < TRIGGER_RESULT_MESSAGE_END) {
      Message_SetDecodeFlagsFar(decodeFlags);
      Event_RunMessage(id, talk->result);
    } else {
      Leader_StopMoving();
      ((void (*)(s32))talk->result)(id);
    }
    if (!paused) {
      if (talker->script[talker->scriptCursor] == SCRIPT_COMMAND_END) {
        if ((s8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
          talker->watchedActor = (union FieldObject *)Actor_GetById(
              gMainRoutineState.leaderActorId);
          talker->steeringFlags |= ACTOR_FACES_MOTION;
          Actor_SetMovementType(talker, (s32)gActor_FaceWatchedScript);
        } else if ((s8)placement->talkFacing == TALK_FACING_PARTY_AND_BACK) {
          talker->timer = savedAngle;
          Actor_StartScriptFar(talker, gActor_StepAngleScript);
        }
      }
      talker->paused = 0;
      Actor_SetAnimationSpeedFar(talker, ANIM_STEP_NORMAL);
    }
    result = 0;
  }
resume:
  if (paused) {
    FieldPsynergy_ResumeHaltedActor(gMainRoutineState.haltedActor);
    gMainRoutineState.haltedActor = -1;
  }
  return result;
}

/// Runs the scene's raised trigger (TRIGGER_RAISED) for id: its message or
/// its function. Returns 0 when an event ran, -1 when there was none.
s32 Scene_RunRaisedTrigger(s32 id) {
  struct TriggerView *trigger = Scene_FindTrigger(TRIGGER_RAISED, id);
  s32 result = -1;

  if (trigger != NULL) {
    s32 handler = trigger->result;

    if (handler != 0) {
      if (handler < TRIGGER_RESULT_MESSAGE_END)
        Event_RunMessage(-1, handler);
      else
        ((void (*)(s32))handler)(id);
      result = 0;
    }
  }
  return result;
}

/// Runs the step trigger of what the leader touched, or else takes its exit:
/// plays the exit's sound (and effect) and records where it leads. Returns 0
/// when something ran, -1 when there was nothing.
s32 Scene_RunStepTriggerOrExit(s32 value) {
  struct TriggerView *event;
  s32 handled;
  struct FieldState *state;

  event = Scene_FindTrigger(TRIGGER_STEP, value);
  handled = -1;
  state = gFieldState;
  if (event != NULL && event->result != 0) {
    if (event->flags & STEP_TRIGGER_FLAG_WALKING)
      state->blockedSteps = 0;
    if (event->result < TRIGGER_RESULT_MESSAGE_END) {
      Event_RunMessage(handled, event->result);
      handled = 0;
    } else {
      ((void (*)(s32))event->result)(value);
      handled = 0;
    }
  } else {
    event = Scene_FindTrigger(TRIGGER_EXIT, value);
    if (event != NULL) {
      switch (event->flags & EXIT_STYLE_MASK) {
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
      case EXIT_STYLE_HIDDEN_LAYER:
        Sound_PlayFar(SE_MAP_EXIT);
        Exit_AddHiddenLeaderLayer();
        break;
      }
      state->exit = event->result;
      handled = 0;
    }
  }
  return handled;
}

/// Runs the push cell the leader pushes against (attributes, its cell's
/// attribute bits) once it has pushed for more than FIELD_PUSH_FRAMES, idle
/// while gMainRoutineState.sandActive is set: a MAP_CELL_LEDGE with KEY_DOWN
/// held jumps down it, a MAP_CELL_JUMP jumps forward, a MAP_CELL_CLIMBABLE
/// climbs down or up by the key held, and a MAP_CELL_PUSH pushes the actor
/// ahead, running the scene's TRIGGER_PUSH trigger for it and TRIGGER_PUSHED
/// for the cell. Always returns 0.
s32 Field_RunPushCell(s32 attributes) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 owner = gMainRoutineState.leaderActorId;
  struct TriggerView *event;
  s32 target;
  s32 result;

  if (gMainRoutineState.sandActive != 0)
    return 0;
  if ((attributes & MAP_CELL_LEDGE) &&
      state->blockedSteps > FIELD_PUSH_FRAMES && (state->heldKeys & KEY_DOWN)) {
    Actor_JumpDownLedge(owner, ANIM_LEADER_SPRING, 0);
    state->blockedSteps = 0;
  }
  if ((attributes & MAP_CELL_JUMP) && state->blockedSteps > FIELD_PUSH_FRAMES &&
      !Field_TryJumpForward())
    state->blockedSteps = 0;
  if ((attributes & MAP_CELL_CLIMBABLE) &&
      state->blockedSteps > FIELD_PUSH_FRAMES) {
    s32 kind = state->heldKeys & (KEY_RIGHT | KEY_LEFT | KEY_UP | KEY_DOWN);
    s32 done = -1;

    if (kind == KEY_DOWN)
      done = Field_ClimbDown();
    else if (kind == KEY_UP)
      done = Field_ClimbUp();
    if (done == 0) {
      state->blockedSteps = 0;
      state->fieldMenu = 0;
      state->pauseMenu = 0;
      state->aButton = 0;
      state->encounter = 0;
      state->psynergy = 0;
    }
  }
  if ((attributes & MAP_CELL_PUSH) && state->blockedSteps > FIELD_PUSH_FRAMES &&
      Leader_CanPushObjectAhead()) {
    target = Field_FindNearbyTalkTarget(gMainRoutineState.leaderActorId);
    if (target != -1) {
      event = Scene_FindTrigger(TRIGGER_PUSH, target);
      if (event != NULL && (result = event->result) != 0) {
        if (result < TRIGGER_RESULT_MESSAGE_END)
          Event_RunMessage(-1, result);
        else
          ((void (*)(s32, s32))result)(attributes, target);
      }
      Leader_PushObjectAhead();
      event = Scene_FindTrigger(TRIGGER_PUSHED, attributes);
      if (event != NULL && (result = event->result) != 0) {
        if (result < TRIGGER_RESULT_MESSAGE_END)
          Event_RunMessage(-1, result);
        else
          ((void (*)(s32, s32))result)(attributes, target);
      }
      state->blockedSteps = 0;
    }
  }
  return 0;
}

/// The search command. CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL show their pair
/// of messages from gField_ExamineTriggerMessageOffsets; otherwise the examine
/// trigger of the cell runs: its "checked the ..." message, then its message or
/// callback, or the matching "found nothing" message. MsgTileChecked is the
/// base of the "X checked the Y..." lines, MsgTileResult the base of the "found
/// nothing" lines.
s32 Field_ExamineCell(s32 id) {
  struct Trigger *event;
  s32 index;
  s32 kind;
  s32 condition;
  u32 result;

  // The unsigned compare also rejects codes below CELL_BEHAVIOR_JAR.
  index = id - CELL_BEHAVIOR_JAR;
  if ((u32)index <= CELL_BEHAVIOR_WELL - CELL_BEHAVIOR_JAR) {
    Leader_StopMoving();
    // id, no longer needed, holds the pair's message number.
    id = gField_ExamineTriggerMessageOffsets[index];
    FieldMessage_ShowFar(id + (s32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
    FieldMessage_ShowFar(id + (s32)&MsgTileResult, FIELD_MESSAGE_ZOOM);
    return 0;
  }
  event = Scene_FindTrigger(TRIGGER_EXAMINE, id);
  if (event != NULL) {
    s32 flags = event->control;

    condition = event->condition;
    kind = EXAMINE_TARGET(flags);
    if (!(event->subject & TRIGGER_SUBJECT_OWN_EFFECT) && kind != 0) {
      Leader_StopMoving();
      FieldMessage_ShowFar(kind + (s32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
      GameFlag_Set(FLAG_NOTHING_HAPPENS);
    } else {
      GameFlag_Clear(FLAG_NOTHING_HAPPENS);
    }
    // The value is a function's address (FIND_FUNCTION_BITS set), which is
    // called, or a FIND_ value: a FIND_MESSAGE is shown and any other type
    // does nothing here.
    result = event->value;
    if ((result & FIND_FUNCTION_BITS) != 0 ||
        (result & FIND_TYPE_MASK) == FIND_MESSAGE(0)) {
      if ((result & FIND_FUNCTION_BITS) != 0) {
        if (GameFlag_IsConditionActive(condition))
          ((void (*)(s32))event->value)(gMainRoutineState.leaderActorId);
        if (GameFlag_Get(FLAG_NOTHING_HAPPENS))
          FieldMessage_ShowFar(kind + (s32)&MsgTileResult, FIELD_MESSAGE_ZOOM);
      } else {
        if (GameFlag_IsConditionActive(condition))
          FieldMessage_ShowFar((u16)event->value, FIELD_MESSAGE_ZOOM);
        else
          FieldMessage_ShowFar((s32)&MsgButFoundNothing, FIELD_MESSAGE_ZOOM);
      }
    }
  } else {
    FieldMessage_ShowFar((s32)&MsgTileChecked + EXAMINE_GROUND,
                         FIELD_MESSAGE_ZOOM);
    FieldMessage_ShowFar((s32)&MsgTileResult + EXAMINE_GROUND,
                         FIELD_MESSAGE_ZOOM);
  }
  GameFlag_Clear(FLAG_NOTHING_HAPPENS);
  return 0;
}
