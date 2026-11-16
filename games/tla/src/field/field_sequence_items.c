/// @file
/// Using an item on the field.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "flags.h"
#include "item_ids.h"
#include "fixed_math.h"
#include "gamepak/far_call/window.h"
#include "field/event_commands.h"
#include "field/field_sequence.h"

/// Runs message as an event of its own, shown at actor.
void Event_RunMessage(s32 actor, s32 message) {
  Event_Start();
  Event_SetMessageId(message);
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

/// Ends every lasting Psynergy (Event_ResolvePendingActions with none skipped).
void Event_ResolveAllPendingActions(void) { Event_ResolvePendingActions(0); }

/// Ends the lasting Psynergy and runs their end events: Reveal unless skip bit
/// 0 is set, field Psynergy 28's ring unless bit 1, and the one
/// pendingThird marks unless bit 2.
void Event_ResolvePendingActions(s32 skip) {
  struct FieldWork *state = Ram_HeapSlots->fieldWork;

  state->busy = 0;
  if (state->revealActive != 0 && !(skip & 1))
    Field_RunPsynergyEndEvents(ABILITY_REVEAL | ACT_PACKED);
  if (state->ringActive != 0 && !(skip & 2))
    Field_RunPsynergyEndEvents(ABILITY_RING | ACT_PACKED);
  if (gMainRoutineWork.pendingThird != 0 && !(skip & 4))
    Field_RunPsynergyEndEvents(ABILITY_SAND | ACT_PACKED);
}

/// Returns the leader's actor id, or 8 while gMainRoutineWork.ownerOverride is
/// set.
s32 Party_GetLeaderActor(void) {
  struct MainRoutineWork *state = &gMainRoutineWork;
  s32 actor;

  // While the party sails, the first placed actor, the ship, stands for it.
  actor = ACTOR_FIRST_PLACED;
  if (state->ownerOverride == 0)
    actor = state->leaderActorId;
  return actor;
}

/// Returns 1 when itemId answers filter: the item itself, ITEM_FILTER_ANY any
/// item, or ITEM_FILTER_EQUIPMENT any item of types 1 to 4; else 0.
s32 Item_MatchesFilter(s32 filter, s32 itemId) {
  if (filter == itemId)
    return 1;
  if (filter == ITEM_FILTER_ANY)
    return 1;
  if (filter == ITEM_FILTER_EQUIPMENT &&
      (u32)(Item_GetDirectFar(itemId)->type - 1) <= 3)
    return 1;
  return 0;
}

/// Finds the scene's first TRIGGER_ITEM trigger whose condition holds, whose
/// facing range (when it has one) takes the controlled actor's angle, whose
/// item filter takes source (when given), and whose target is the actor the
/// leader can talk to (TRIGGER_ITEM_ON_ACTOR) or the object it is touching; 0
/// when none does. The Broken Seal's routine, asking Party_GetLeaderActor for
/// the actor and Item_MatchesFilter for the trigger's item.
const struct Trigger *Field_FindItemTrigger(s32 source) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  const struct Trigger *trigger = work->triggers;
  s32 facing = Actor_Lookup(Party_GetLeaderActor())->facing;
  s32 selected = Field_FindNearbyTalkTarget(Party_GetLeaderActor());
  s32 collision;

  source &= ITEM_FILTER_ANY;
  collision = Field_GetCollisionAheadOfLeader();

  while (trigger->control != -1) {
    s32 highValue = (s16)(u16)trigger->subject & TRIGGER_SUBJECT_FACING;
    s16 hasFacing = (u16)trigger->subject & TRIGGER_SUBJECT_NEEDS_FACING;
    s32 lowValue = (u16)trigger->subject & TRIGGER_SUBJECT_TARGET;

    if ((trigger->control & TRIGGER_KIND_MASK) == TRIGGER_ITEM &&
        GameFlag_IsConditionActive(trigger->condition) != 0 &&
        (hasFacing == 0 ||
         (u16)(highValue - facing + (TRIG_TURN * 3 / 32 - 1)) <=
             TRIG_TURN * 3 / 16 - 2)) {
      s32 item = (u16)((s32)trigger->control / 256);

      if (source == 0 || Item_MatchesFilter(item, source)) {
        if ((trigger->control & TRIGGER_ITEM_ON_ACTOR) != 0) {
          if (lowValue == selected)
            return trigger;
        } else if (lowValue == collision) {
          return trigger;
        }
      }
    }
    trigger++;
  }
  return NULL;
}

s32 FieldYesNo_Ask(s32 mode);

/// Uses an item on the field. packedItem holds the item (bits 0 to 9) and the
/// character using it (bits 10 to 13; 15 for the character carrying most of
/// it); slot is its inventory slot. The scene's item trigger for what the
/// leader faces runs if there is one; otherwise an item with a field action
/// uses it (Retreat asks first and leaves the scene). FLAG_NOTHING_HAPPENS
/// still set afterwards says "But nothing happens."; FLAG_ITEM_USED_UP removes
/// the item. Returns 0 when the scene's event ran or Retreat was declined, else
/// -1, the item's own action included; FieldSequence_Run ignores it. Called by
/// FieldSequence_Run for the item request (work->item and work->itemSlot).
/// Retreat sets gMainRoutineWork.next and .entrance to the Retreat point
/// and raises work->exit.
s32 Field_UseItem(s32 packedItem, s32 slot) {
  s32 result;
  s32 itemId;
  s32 actor;
  s32 actorMask;
  struct Unit *unit;
  const struct Trigger *trigger;
  s32 j;
  s32 i;
  s32 best;
  s32 matches;
  s32 count;
  struct FieldWork *runtime;

  result = -1;
  itemId = 0x3ff;
  actorMask = 0xf;
  itemId &= packedItem;
  actor = (packedItem >> 10) & actorMask;
  {
    best = 0;
    count = Party_CountMembersFar();

    if (actor == UNIT_NONE) {
      actor = 0;
      for (i = 0; i < count; i++) {
        unit = Unit_Get(gMainRoutineWork.party[i]);
        matches = 0;
        for (j = 0; j < INVENTORY_SLOTS; j++) {
          if ((unit->inventory[j] & INVENTORY_ITEM_ID_MASK) == itemId)
            matches++;
        }

        if (best < matches) {
          best = matches;
          actor = gMainRoutineWork.party[i];
        }
      }
    } else {
      unit = Unit_Get(actor);
      for (j = 0; j < INVENTORY_SLOTS; j++) {
        if ((unit->inventory[j] & INVENTORY_ITEM_ID_MASK) == itemId)
          best++;
      }
    }

    if (best == 0) {
      FieldMessage_ShowFar((s32)&MsgNothingHappens, FIELD_MESSAGE_ZOOM);
      return -1;
    }
  }

  trigger = Field_FindItemTrigger(itemId);
  if (trigger != NULL && trigger->value != 0) {
    GameFlag_Clear(FLAG_ITEM_USED_UP);
    GameFlag_Clear(FLAG_NOTHING_HAPPENS);
    if (!(trigger->subject & TRIGGER_SUBJECT_OWN_EFFECT)) {
      MessageArgument_PushFar(actor, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar((s32)&MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
    }
    if ((s32)trigger->value < TRIGGER_RESULT_MESSAGE_END) {
      s32 objref = Field_FindNearbyTalkTarget(Party_GetLeaderActor());

      Event_Start();
      Event_SetMessageId(trigger->value);
      Event_ShowMessageAndWait(objref, 0);
      Event_End();
    } else {
      ((ItemTriggerCallback)trigger->value)(itemId, actor, slot);
    }
    result = 0;
  } else {
    s32 actionId;

    GameFlag_Clear(FLAG_ITEM_USED_UP);
    GameFlag_Set(FLAG_NOTHING_HAPPENS);
    actionId = Item_GetDirectFar(itemId)->abilityId;
    runtime = Ram_HeapSlots->fieldWork;

    if (actionId != 0) {
      GameFlag_Set(FLAG_ITEM_USE);
      GameFlag_Clear(FLAG_NOTHING_HAPPENS);

      if (actionId == ABILITY_RETREAT && !GameFlag_Get(FLAG_RETREAT_BLOCKED)) {
        MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar((s32)&MsgUseItemQuestion,
                             FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
                                 FIELD_MESSAGE_LOWER);
        i = FieldYesNo_Ask(1);
        Message_CloseFinishedWindowsFar();
        if (i != 0)
          return 0;

        gMainRoutineWork.next = gMainRoutineWork.retreatScene;
        gMainRoutineWork.entrance = gMainRoutineWork.retreatEntrance;
        runtime->exit = EXIT_KEEP_DESTINATION;
      }

      MessageArgument_PushFar(actor, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar((s32)&MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
      FieldPsynergy_LoadResources(actionId, 0);
      runtime->psynergyActing = 1;
      FieldPsynergy_Run();
      runtime->psynergyActing = 0;
      FieldPsynergy_End();

      if (Item_GetDirectFar(itemId)->useType & 1)
        GameFlag_Set(FLAG_ITEM_USED_UP);
    }
  }

  if (GameFlag_Get(FLAG_NOTHING_HAPPENS))
    FieldMessage_ShowFar((s32)&MsgNothingHappens, FIELD_MESSAGE_ZOOM);
  if (GameFlag_Get(FLAG_ITEM_USED_UP))
    Inventory_RemoveFar(actor, slot);
  return result;
}

/// Chooses what a Psynergy of the given kind acts on, for
/// Scene_FindPsynergyTrigger: among the scene's Psynergy triggers for that
/// kind whose condition holds (and whose direction, if any, the leader faces),
/// the cell the leader faces, or for Teleport the cell under it; else the
/// nearest target in reach (with PSYNERGY_TARGET_ACTOR); else
/// PSYNERGY_TARGET_ANYWHERE for a trigger that applies anywhere, or -1.
s32 Scene_SelectPsynergyTarget(s32 kind) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct TriggerEvent *event = work->triggers;
  u32 facing = Actor_Lookup(Party_GetLeaderActor())->facing;
  s32 focused = Field_GetCollisionAheadOfLeader();
  s32 special = Field_GetCollisionUnderLeader();
  s32 best = 9999;
  // Default: -1, though every path below sets it.
  s32 result = -1;
  s32 selected = -1;
  s32 nearby = -1;
  s32 fallback = -1;
  s32 flags;

  flags = event->control;
  while (flags != -1) {
    s32 high = event->trigger & TRIGGER_SUBJECT_FACING;
    s16 hasFacing = (u16)event->trigger & TRIGGER_SUBJECT_NEEDS_FACING;
    s32 target = (u16)event->trigger & TRIGGER_SUBJECT_TARGET;
    s32 eventKind = Ability_Get((u16)flags >> 8)->fieldPsynergy;

    if ((event->control & TRIGGER_KIND_MASK) == TRIGGER_PSYNERGY &&
        eventKind == kind && GameFlag_IsConditionActive(event->condition)) {
      if (hasFacing) {
        s32 distance = (s16)(high - facing);

        if (distance < 0)
          distance = -distance;
        if (distance > TRIG_TURN * 3 / 32 - 1)
          goto next;
      }
      if (event->control & TRIGGER_PSYNERGY_ANYWHERE) {
        fallback = PSYNERGY_TARGET_ANYWHERE;
      } else if (event->control & TRIGGER_PSYNERGY_ON_ACTOR) {
        s32 distance =
            Field_MeasurePsynergyReach(kind, Party_GetLeaderActor(), target);

        if (distance != -1 && best > distance) {
          nearby = target | PSYNERGY_TARGET_ACTOR;
          best = distance;
        }
      } else if (kind == FIELD_PSYNERGY_TELEPORT) {
        if (target == special) {
          selected = target;
          break;
        }
      } else if (target == focused) {
        selected = target;
        break;
      }
    }
  next:
    event++;
    if (event->control == -1)
      break;
    flags = event->control;
  }
  if (selected != -1)
    result = selected;
  else if (nearby != -1)
    result = nearby;
  else
    result = fallback;
  return result;
}

/// Returns the scene's first Psynergy trigger (TRIGGER_PSYNERGY) for a field
/// Psynergy kind whose condition holds, whose facing (when it has one) the
/// leader faces, whose stage matches filter (PSYNERGY_TRIGGER_ANY takes any)
/// and whose target is what selector names (Scene_SelectPsynergyTarget: a cell,
/// an actor, or anything); NULL when there is none.
struct TriggerEvent *Scene_FindPsynergyTrigger(s32 filter, s32 kind,
                                               s32 selector) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct TriggerEvent *event = work->triggers;
  u32 facing = Actor_Lookup(Party_GetLeaderActor())->facing;
  s32 flags;

  if (selector == -1)
    return NULL;
  flags = event->control;
  while (flags != -1) {
    s32 high = event->trigger & TRIGGER_SUBJECT_FACING;
    s16 hasFacing = (u16)event->trigger & TRIGGER_SUBJECT_NEEDS_FACING;
    s32 target = (u16)event->trigger & TRIGGER_SUBJECT_TARGET;
    s32 eventKind = Ability_Get((u16)flags >> 8)->fieldPsynergy;

    if ((event->control & TRIGGER_KIND_MASK) == TRIGGER_PSYNERGY &&
        eventKind == kind && GameFlag_IsConditionActive(event->condition)) {
      if (hasFacing) {
        s32 distance = (s16)(high - facing);

        if (distance < 0)
          distance = -distance;
        if (distance > TRIG_TURN * 3 / 32 - 1)
          goto next;
      }
      if (filter != PSYNERGY_TRIGGER_ANY &&
          (event->control & (PSYNERGY_PHASE_MASK | TRIGGER_KIND_MASK)) !=
              filter)
        goto next;
      flags = event->control;
      {
        if (flags & TRIGGER_PSYNERGY_ANYWHERE) {
          if ((selector & PSYNERGY_TARGET_KIND) == PSYNERGY_TARGET_ANYWHERE)
            return event;
        } else if (flags & TRIGGER_PSYNERGY_ON_ACTOR) {
          if ((selector & PSYNERGY_TARGET_KIND) == PSYNERGY_TARGET_ACTOR &&
              (selector & PSYNERGY_TARGET_ID) == target)
            return event;
        } else if ((selector & PSYNERGY_TARGET_KIND) == 0 &&
                   (selector & PSYNERGY_TARGET_ID) == target) {
          return event;
        }
      }
    }
  next:
    event++;
    if (event->control == -1)
      break;
    flags = event->control;
  }
  return NULL;
}

/// Runs the scene's events for Psynergy without paying its PP or showing its
/// usual effect (PSYNERGY_PHASE_BEFORE_END and PSYNERGY_PHASE_AFTER_END, around
/// the kind's ending), as for a Psynergy that ends by itself: packed holds the
/// Psynergy (bits 0 to 9) and its user (bits 10 to 13).
s32 Field_RunPsynergyEndEvents(u32 packed) {
  s32 action, mode, kind, selector, target;
  struct PendingEvent *first, *second;

  action = ABILITY_ID(packed);
  mode = ACT_UNIT(packed);
  kind = Ability_Get(action)->fieldPsynergy;
  selector = Scene_SelectPsynergyTarget(kind);
  first = (struct PendingEvent *)Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_END), kind, selector);
  second = (struct PendingEvent *)Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END), kind, selector);
  target = -1;
  if (first != NULL || second != NULL) {
    if (selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
      target = selector & PSYNERGY_TARGET_ID;
  }
  FieldPsynergy_LoadResources(action, 0);
  FieldPsynergy_SetupActorPair(Party_GetLeaderActor(), target);
  Field_RunPsynergyEvent(first, mode, target);
  FieldPsynergy_RunExtraHandler();
  BattleEffect_StopMainActor();
  Field_RunPsynergyEvent(second, mode, target);
  FieldPsynergy_End();
  return 0;
}
