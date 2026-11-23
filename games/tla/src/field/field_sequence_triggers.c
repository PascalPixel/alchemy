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

/// The encounter zone whose slots hold the Mimics' formations; slots from 8
/// go on in the next zone.
#define ENCOUNTER_ZONE_MIMIC 104

/// The Broken Seal's Scene_FindActorPlacement: finds actor id's placement in
/// the event work's four tables (ids up to 7 by sprite, later ones counted
/// in order among the rows past 7), or NULL.
struct ActionDescriptor *Scene_FindActorPlacement(s32 id) {
  struct ActionDescriptor *entry;
  s32 i;
  s32 group = ACTOR_FIRST_PLACED;

  for (i = 0; i < 4; i++) {
    entry = gFieldWork->tables[i];
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

// The Lost Age's form of the field's trigger search (the Broken Seal's
// Scene_FindTrigger).

/// Returns the first of the scene's triggers of kind for target whose
/// condition holds and whose push (STEP_TRIGGER_FLAG_WALKING) and facing
/// (TRIGGER_FLAG_FACING) requirements are met, or NULL. TRIGGER_PUSH and
/// TRIGGER_PUSHED, the triggers Field_RunPushCell runs for an actor pushed and
/// for the cell pushed against, answer any target; the facing test alone is
/// skipped while gMainRoutineWork.movementMode is LEADER_MOVEMENT_KEYS.
struct TriggerView *Scene_FindTrigger(s32 kind, s32 target) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct TriggerView *trigger = work->triggers;
  s32 flags;
  u32 facing = Actor_Lookup(gMainRoutineWork.leaderActorId)->facing;

  flags = trigger->flags;
  while (flags != -1) {
    // Holds one address for its whole life.
    u8 *mode = &gMainRoutineWork.movementMode;

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
        if (work->blockedSteps > minimumPushFrames)
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
        if (work->blockedSteps > minimumPushFrames) {
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

/// Returns the talk trigger of object id, or 0: for the object field Psynergy
/// has stopped (gMainRoutineWork.haltedActor), its TRIGGER_TALK_HALTED trigger
/// if it has one, the event for talking to it while it is stopped.
void *Scene_FindActorTalkTrigger(s32 id) {
  void *found = Scene_FindTrigger(TRIGGER_TALK, id);
  struct MainRoutineWork *party = &gMainRoutineWork;
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
  struct Actor *object;
  s32 savedAngle;
  s32 paused;
  s32 busyFlags;
  s32 silent;
  struct FieldWork *event;
  struct Actor *leader;
  struct Actor *shown;
  struct ContainerRow *row;
  struct TriggerView *talk;
  s32 point;
  u8 *busy;
  s32 found;
  s32 type;
  s32 flag;
  s32 contents;
  s32 index;
  s32 slot;
  struct FieldWork *work;

  placement = Scene_FindActorPlacement(id);
  result = -1;
  savedAngle = 0;
  paused = 0;
  busyFlags = 0;
  silent = id & FIELD_TALK_SILENT;
  event = Ram_HeapSlots->fieldWork;
  id &= FIELD_TALK_ACTOR_MASK;
  found = 0;
  object = Actor_GetById(id);
  if (id >= FIELD_TREASURE_ACTOR) {
    if (id != SCOOPED_ITEM_ACTOR) {
      row = SceneItem_GetRows();
      if (row != NULL) {
        /* FAKEMATCH(temporary): p, each field's address in turn, read
         * through; as row[id - FIELD_TREASURE_ACTOR].type and the rest. */
        s16 *p;

        p = &row[id - FIELD_TREASURE_ACTOR].type;
        type = *p;
        index = id;
        found = 1;
        p = &row[id - FIELD_TREASURE_ACTOR].point;
        point = *p;
        p = &row[id - FIELD_TREASURE_ACTOR].flag;
        flag = *p;
        p = &row[id - FIELD_TREASURE_ACTOR].contents;
        contents = *p;
      }
    } else {
      type = CONTAINER_GROUND;
      point = result;
      flag = result;
      contents = event->scoopedItem;
      index = SCOOPED_ITEM_ACTOR;
      found = 1;
    }
    if (found) {
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
      busy = &object->paused;
      *busy = 1;
      if (silent == 0)
        FieldMessage_ShowFar(found + (s32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
      if (gMainRoutineWork.next == SCENE_VENUS_LIGHTHOUSE && point == 0 &&
          !GameFlag_Get(flag)) {
        MessageArgument_PushFar(contents, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar((s32)&MsgChestAlreadyOpen, FIELD_MESSAGE_ZOOM);
      }
      if (!GameFlag_IsConditionActive(flag))
        goto empty;
      if (type == CONTAINER_CHEST) {
        if (contents == 0)
          ObjectEvent_SetMode6WithCue(index);
        else
          TreasureSpot_OpenChest(index);
      }
      if (type == CONTAINER_MIMIC)
        TreasureSpot_OpenChest(index);
      if (type == CONTAINER_PSYNERGY_STONE) {
        struct Actor *trap = TreasureSpot_GetActor(index);

        trap->update = ItemDisplay_EmitSparkles;
        ItemDisplay_RaiseAboveLeader(trap);
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
        trap->update = NULL;
        Actor_SelectAnimationFar(trap, 2);
        Sound_PlayFar(SE_GLITTER);
        Event_Wait(30);
        FieldMessage_ShowFar((s32)&MsgPsynergyStoneDisappeared,
                             FIELD_MESSAGE_ZOOM);
        ObjectEvent_Remove(index);
        if (flag != -1)
          GameFlag_Set(flag);
      } else if (type == CONTAINER_STONE_TABLET) {
        Event_Start();
        Event_ResolvePendingActions(0);
        SummonTablet_PlayRunes(index);
        Summon_ShowLearnedMessageFar(contents);
        Summon_LearnFar(contents);
        ObjectEvent_Remove(index);
        if (flag != -1)
          GameFlag_Set(flag);
        Event_End();
      } else if (type == CONTAINER_MIMIC) {
        work = Ram_HeapSlots->fieldWork;
        TreasureSpot_LaunchMimic(index);
        if (flag != -1)
          gMainRoutineWork.pendingDjinni = flag | CONDITION_FLAG_SET;
        if (contents > 7)
          work->encounter = Encounter_GetFormationAtSlot(
              ENCOUNTER_ZONE_MIMIC + 1, contents - 8);
        else
          work->encounter =
              Encounter_GetFormationAtSlot(ENCOUNTER_ZONE_MIMIC, contents);
        gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
        Encounter_SelectBattleMusic(ENCOUNTER_ZONE_MIMIC, contents);
        Sound_PlayFar(gMainRoutineWork.battleMusic);
        FieldMessage_ShowFar((s32)&MsgChestWasMimic, FIELD_MESSAGE_ZOOM);
      } else {
        if (type == CONTAINER_POND)
          FieldMessage_ShowFar((s32)&MsgSomethingInTheShallows,
                               FIELD_MESSAGE_ZOOM);
        if (Actor_GetSpriteShape(index)) {
          FieldMessage_ShowFar((s32)&MsgSomethingBuriedHere,
                               FIELD_MESSAGE_ZOOM);
          if (Party_FindOwnerWithActionFar(ABILITY_SCOOP) != -1)
            gFieldWork->fieldMenu = 1;
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
            if (slot == gMainRoutineWork.leaderActorId) {
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
              ObjectEvent_Remove(index);
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

  if (gMainRoutineWork.haltedActor == id) {
    paused = 1;
    talk = Scene_FindTrigger(TRIGGER_TALK_HALTED, id);
    if (talk == NULL) {
      talk = Scene_FindTrigger(TRIGGER_TALK, id);
      busyFlags = 1;
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
      object->paused = 1;
      Actor_SetAnimationSpeedFar(object, 0);
      savedAngle = object->facing;
      if ((u8)placement->talkFacing <= TALK_FACING_PARTY_AND_BACK ||
          (s8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
        Actor_SetPositionAndResetMotionFar(leader, leader->x.fixed,
                                           leader->y.fixed, leader->z.fixed);
        Actor_TurnToFaceEachOther(id, gMainRoutineWork.leaderActorId, 0);
      }
    }
    if (talk->result < TRIGGER_RESULT_MESSAGE_END) {
      Message_SetDecodeFlagsFar(busyFlags);
      Event_RunMessage(id, talk->result);
    } else {
      Leader_StopMoving();
      ((void (*)(s32))talk->result)(id);
    }
    if (!paused) {
      if (object->script[object->scriptCursor] == SCRIPT_COMMAND_END) {
        if ((s8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
          object->watchedActor = Actor_GetById(gMainRoutineWork.leaderActorId);
          object->steeringFlags |= ACTOR_FACES_MOTION;
          Actor_SetMovementType(object, (s32)gActor_FaceWatchedScript);
        } else if ((s8)placement->talkFacing == TALK_FACING_PARTY_AND_BACK) {
          object->timer = savedAngle;
          Actor_StartScriptFar(object, gActor_StepAngleScript);
        }
      }
      object->paused = 0;
      Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
    }
    result = 0;
  }
resume:
  if (paused) {
    FieldPsynergy_ResumeHaltedActor(gMainRoutineWork.haltedActor);
    gMainRoutineWork.haltedActor = -1;
  }
  return result;
}

/// Runs the scene's raised trigger (TRIGGER_RAISED) for value: its message or
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
  struct FieldWork *work;

  event = Scene_FindTrigger(TRIGGER_STEP, value);
  handled = -1;
  work = gFieldWork;
  if (event != NULL && event->result != 0) {
    if (event->flags & STEP_TRIGGER_FLAG_WALKING)
      work->blockedSteps = 0;
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
      work->exit = event->result;
      handled = 0;
    }
  }
  return handled;
}

/// Runs the push cell the leader pushes against (attributes, its cell's
/// attribute bits) once it has pushed for more than FIELD_PUSH_FRAMES, idle
/// while gMainRoutineWork.sandActive is set: a MAP_CELL_LEDGE with KEY_DOWN
/// held jumps down it, a MAP_CELL_JUMP jumps forward, a MAP_CELL_CLIMBABLE
/// climbs down or up by the key held, and a MAP_CELL_PUSH pushes the actor
/// ahead, running the
/// scene's TRIGGER_PUSH trigger for it and TRIGGER_PUSHED for the cell. Always
/// returns 0.
s32 Field_RunPushCell(s32 attributes) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 owner = gMainRoutineWork.leaderActorId;
  struct TriggerView *event;
  s32 target;
  s32 result;

  if (gMainRoutineWork.sandActive != 0)
    return 0;
  if ((attributes & MAP_CELL_LEDGE) && work->blockedSteps > FIELD_PUSH_FRAMES &&
      (work->heldKeys & KEY_DOWN)) {
    Actor_JumpDownLedge(owner, ANIM_LEADER_SPRING, 0);
    work->blockedSteps = 0;
  }
  if ((attributes & MAP_CELL_JUMP) && work->blockedSteps > FIELD_PUSH_FRAMES &&
      !Field_TryJumpForward())
    work->blockedSteps = 0;
  if ((attributes & MAP_CELL_CLIMBABLE) &&
      work->blockedSteps > FIELD_PUSH_FRAMES) {
    s32 kind = work->heldKeys & (KEY_RIGHT | KEY_LEFT | KEY_UP | KEY_DOWN);
    s32 done = -1;

    if (kind == KEY_DOWN)
      done = Field_ClimbDown();
    else if (kind == KEY_UP)
      done = Field_ClimbUp();
    if (done == 0) {
      work->blockedSteps = 0;
      work->fieldMenu = 0;
      work->pauseMenu = 0;
      work->aButton = 0;
      work->encounter = 0;
      work->psynergy = 0;
    }
  }
  if ((attributes & MAP_CELL_PUSH) && work->blockedSteps > FIELD_PUSH_FRAMES &&
      Leader_CanPushObjectAhead()) {
    target = Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId);
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
      work->blockedSteps = 0;
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
          ((void (*)(s32))event->value)(gMainRoutineWork.leaderActorId);
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
