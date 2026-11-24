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

/// What Event_ResolvePendingActions leaves running, as bits of its skip.
enum PsynergyEndSkip {
  END_SKIP_REVEAL = 1 << 0,
  END_SKIP_RING = 1 << 1, ///< the ring of light (ABILITY_MAGNET)
  END_SKIP_SAND = 1 << 2,
};

/// Runs message as an event of its own, shown at actor.
void Event_RunMessage(s32 actor, s32 message) {
  Event_Start();
  Event_SetMessageId(message);
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

/// Ends every lasting Psynergy (Event_ResolvePendingActions with none skipped).
void Event_ResolveAllPendingActions(void) { Event_ResolvePendingActions(0); }

/// Ends the lasting Psynergy and runs their end events: Reveal, the ring of
/// light and Sand (while the party is sand, gMainRoutineState.sandActive), each
/// unless skip holds its bit (PsynergyEndSkip).
void Event_ResolvePendingActions(s32 skip) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->busy = 0;
  if (state->revealActive != 0 && !(skip & END_SKIP_REVEAL))
    Field_RunPsynergyEndEvents(ABILITY_REVEAL | ACT_PACKED);
  if (state->ringActive != 0 && !(skip & END_SKIP_RING))
    Field_RunPsynergyEndEvents(ABILITY_MAGNET | ACT_PACKED);
  if (gMainRoutineState.sandActive != 0 && !(skip & END_SKIP_SAND))
    Field_RunPsynergyEndEvents(ABILITY_SAND | ACT_PACKED);
}

/// Returns the leader's actor id, or ACTOR_FIRST_PLACED while
/// gMainRoutineState.ownerOverride is set.
s32 Party_GetLeaderActor(void) {
  struct MainRoutineState *state = &gMainRoutineState;
  s32 actor;

  // While the party sails, the first placed actor, the ship, stands for it.
  actor = ACTOR_FIRST_PLACED;
  if (state->ownerOverride == 0)
    actor = state->leaderActorId;
  return actor;
}

/// Returns 1 when itemId answers filter: the item itself, ITEM_FILTER_ANY any
/// item, or ITEM_FILTER_HEAVY any weapon, armor, shield or helm; else 0.
s32 Item_MatchesFilter(s32 filter, s32 itemId) {
  if (filter == itemId)
    return 1;
  if (filter == ITEM_FILTER_ANY)
    return 1;
  if (filter == ITEM_FILTER_HEAVY &&
      (u32)(Item_GetDirectFar(itemId)->type - ITEM_TYPE_WEAPON) <=
          ITEM_TYPE_HELM - ITEM_TYPE_WEAPON)
    return 1;
  return 0;
}

/// Returns the scene's item trigger (TRIGGER_ITEM) for item that applies to
/// what the leader faces, or NULL (item 0 matches any): its item or item filter
/// takes item (Item_MatchesFilter), and its target is the actor the leader can
/// talk to (TRIGGER_ITEM_ON_ACTOR) or the cell ahead.
const struct Trigger *Field_FindItemTrigger(s32 item) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  const struct Trigger *trigger = state->triggers;
  s32 facing = Actor_Lookup(Party_GetLeaderActor())->facing;
  s32 actorAhead = Field_FindNearbyTalkTarget(Party_GetLeaderActor());
  s32 tileAhead;

  item &= INVENTORY_ITEM_ID_MASK;
  tileAhead = Field_GetCollisionAheadOfLeader();

  for (; trigger->control != -1; trigger++) {
    s32 eventFacing = (s16)(u16)trigger->subject & TRIGGER_SUBJECT_FACING;
    s16 requiresFacing = (u16)trigger->subject & TRIGGER_SUBJECT_NEEDS_FACING;
    s32 eventTarget = (u16)trigger->subject & TRIGGER_SUBJECT_TARGET;

    if ((trigger->control & TRIGGER_KIND_MASK) == TRIGGER_ITEM &&
        GameFlag_IsConditionActive(trigger->condition) != 0 &&
        (requiresFacing == 0 ||
         (u16)(eventFacing - facing + (TRIG_TURN * 3 / 32 - 1)) <=
             TRIG_TURN * 3 / 16 - 2)) {
      // The item, as USE_ITEM packs it above the trigger's kind.
      s32 eventItem = (u16)((s32)trigger->control / 256);

      if (item == 0 || Item_MatchesFilter(eventItem, item)) {
        if ((trigger->control & TRIGGER_ITEM_ON_ACTOR) != 0) {
          if (eventTarget == actorAhead)
            return trigger;
        } else if (eventTarget == tileAhead) {
          return trigger;
        }
      }
    }
  }
  return NULL;
}

s32 FieldYesNo_Ask(s32 atBottom);

/// Uses an item on the field. packedItem holds the item (bits 0 to 9) and the
/// character using it (bits 10 to 13; UNIT_NONE for whoever holds it in the
/// most inventory slots); slot is its inventory slot. The scene's item trigger
/// for what the leader faces runs if there is one; otherwise an item with a
/// field action casts it, Retreat asking first. Returns 0 when the scene's
/// event ran or Retreat was declined, else -1.
s32 Field_UseItem(s32 packedItem, s32 slot) {
  s32 result;
  s32 itemId;
  s32 actorId;
  struct Unit *unit;
  const struct Trigger *trigger;
  s32 inventorySlot;
  s32 partyIndex;
  s32 mostMatchingSlots;
  s32 matchingSlots;
  s32 partyCount;
  struct FieldState *state;

  result = -1;
  itemId = ACT_ITEM_ID(packedItem);
  actorId = ACT_UNIT(packedItem);
  // Count the user's slots holding the item; with no user given, the user is
  // whoever holds it in the most slots, the earlier on a tie.
  {
    mostMatchingSlots = 0;
    partyCount = Party_CountMembersFar();

    if (actorId == UNIT_NONE) {
      actorId = 0;
      for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
        unit = Unit_Get(gMainRoutineState.party[partyIndex]);
        matchingSlots = 0;
        for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS;
             inventorySlot++) {
          if ((unit->inventory[inventorySlot] & INVENTORY_ITEM_ID_MASK) ==
              itemId)
            matchingSlots++;
        }

        if (mostMatchingSlots < matchingSlots) {
          mostMatchingSlots = matchingSlots;
          actorId = gMainRoutineState.party[partyIndex];
        }
      }
    } else {
      unit = Unit_Get(actorId);
      for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS;
           inventorySlot++) {
        if ((unit->inventory[inventorySlot] & INVENTORY_ITEM_ID_MASK) == itemId)
          mostMatchingSlots++;
      }
    }

    if (mostMatchingSlots == 0) {
      // "But nothing happens."
      FieldMessage_ShowFar((s32)&MsgNothingHappens, FIELD_MESSAGE_ZOOM);
      return -1;
    }
  }

  trigger = Field_FindItemTrigger(itemId);
  if (trigger != NULL && trigger->value != 0) {
    // The scene's event: a message, or a function.
    GameFlag_Clear(FLAG_ITEM_USED_UP);
    GameFlag_Clear(FLAG_NOTHING_HAPPENS);
    if (!(trigger->subject & TRIGGER_SUBJECT_OWN_EFFECT)) {
      MessageArgument_PushFar(actorId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar((s32)&MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
    }
    if ((s32)trigger->value < TRIGGER_RESULT_MESSAGE_END) {
      s32 nearbyActorId = Field_FindNearbyTalkTarget(Party_GetLeaderActor());

      Event_Start();
      Event_SetMessageId(trigger->value);
      Event_ShowMessageAndWait(nearbyActorId, 0);
      Event_End();
    } else {
      ((ItemTriggerCallback)trigger->value)(itemId, actorId, slot);
    }
    result = 0;
  } else {
    // The item's own field action, if it has one: the ability abilityId names
    // plays as if cast.
    s32 abilityId;

    GameFlag_Clear(FLAG_ITEM_USED_UP);
    GameFlag_Set(FLAG_NOTHING_HAPPENS);
    abilityId = Item_GetDirectFar(itemId)->abilityId;
    state = Ram_HeapSlots->fieldState;

    if (abilityId != 0) {
      GameFlag_Set(FLAG_PSYNERGY_FROM_ITEM);
      GameFlag_Clear(FLAG_NOTHING_HAPPENS);

      if (abilityId == ABILITY_RETREAT && !GameFlag_Get(FLAG_RETREAT_BLOCKED)) {
        MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar((s32)&MsgUseItemQuestion,
                             FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
                                 FIELD_MESSAGE_LOWER);
        // partyIndex now holds the answer: 0 is yes.
        partyIndex = FieldYesNo_Ask(TRUE);
        Message_CloseFinishedWindowsFar();
        if (partyIndex != 0)
          return 0;

        gMainRoutineState.next = gMainRoutineState.retreatScene;
        gMainRoutineState.entrance = gMainRoutineState.retreatEntrance;
        state->exit = EXIT_KEEP_DESTINATION;
      }

      MessageArgument_PushFar(actorId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar((s32)&MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
      FieldPsynergy_LoadResources(abilityId, 0);
      state->psynergyActing = 1;
      FieldPsynergy_Run();
      state->psynergyActing = 0;
      FieldPsynergy_End();

      // ITEM_USE_TEACHES has ITEM_USE_CONSUMED's bit too: both are used up.
      if (Item_GetDirectFar(itemId)->useType & ITEM_USE_CONSUMED)
        GameFlag_Set(FLAG_ITEM_USED_UP);
    }
  }

  if (GameFlag_Get(FLAG_NOTHING_HAPPENS))
    // "But nothing happens."
    FieldMessage_ShowFar((s32)&MsgNothingHappens, FIELD_MESSAGE_ZOOM);
  if (GameFlag_Get(FLAG_ITEM_USED_UP))
    Inventory_RemoveFar(actorId, slot);
  return result;
}

/// Chooses what a Psynergy of the given kind acts on, for
/// Scene_FindPsynergyTrigger: among the scene's Psynergy triggers for that
/// kind whose condition holds (and whose direction, if any, the leader faces),
/// the cell the leader faces, or for Teleport the cell under it; else the
/// nearest target in reach (with PSYNERGY_TARGET_ACTOR); else
/// PSYNERGY_TARGET_ANYWHERE for a trigger that applies anywhere, or -1.
s32 Scene_SelectPsynergyTarget(s32 kind) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct TriggerEvent *event = state->triggers;
  u32 facing = Actor_Lookup(Party_GetLeaderActor())->facing;
  s32 tileAhead = Field_GetCollisionAheadOfLeader();
  s32 tileUnder = Field_GetCollisionUnderLeader();
  s32 best = BEYOND_REACH;
  // Default: -1, though every path below sets it.
  s32 result = -1;
  s32 selected = -1;
  s32 nearby = -1;
  s32 fallback = -1;

  for (; event->control != -1; event++) {
    s32 eventFacing = event->trigger & TRIGGER_SUBJECT_FACING;
    s16 requiresFacing = (u16)event->trigger & TRIGGER_SUBJECT_NEEDS_FACING;
    s32 eventTarget = (u16)event->trigger & TRIGGER_SUBJECT_TARGET;
    s32 eventKind = Ability_Get((u16)event->control >> 8)->fieldPsynergy;

    if ((event->control & TRIGGER_KIND_MASK) == TRIGGER_PSYNERGY &&
        eventKind == kind && GameFlag_IsConditionActive(event->condition)) {
      if (requiresFacing) {
        s32 distance = (s16)(eventFacing - facing);

        if (distance < 0)
          distance = -distance;
        if (distance > TRIG_TURN * 3 / 32 - 1)
          continue;
      }
      if (event->control & TRIGGER_PSYNERGY_ANYWHERE) {
        fallback = PSYNERGY_TARGET_ANYWHERE;
      } else if (event->control & TRIGGER_PSYNERGY_ON_ACTOR) {
        s32 distance = Field_MeasurePsynergyReach(kind, Party_GetLeaderActor(),
                                                  eventTarget);

        if (distance != -1 && best > distance) {
          nearby = eventTarget | PSYNERGY_TARGET_ACTOR;
          best = distance;
        }
      } else if (kind == FIELD_PSYNERGY_TELEPORT) {
        if (eventTarget == tileUnder) {
          selected = eventTarget;
          break;
        }
      } else if (eventTarget == tileAhead) {
        selected = eventTarget;
        break;
      }
    }
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
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct TriggerEvent *event = state->triggers;
  u32 facing = Actor_Lookup(Party_GetLeaderActor())->facing;

  if (selector == -1)
    return NULL;
  for (; event->control != -1; event++) {
    s32 eventFacing = event->trigger & TRIGGER_SUBJECT_FACING;
    s16 requiresFacing = (u16)event->trigger & TRIGGER_SUBJECT_NEEDS_FACING;
    s32 eventTarget = (u16)event->trigger & TRIGGER_SUBJECT_TARGET;
    s32 eventKind = Ability_Get((u16)event->control >> 8)->fieldPsynergy;

    if ((event->control & TRIGGER_KIND_MASK) == TRIGGER_PSYNERGY &&
        eventKind == kind && GameFlag_IsConditionActive(event->condition)) {
      if (requiresFacing) {
        s32 distance = (s16)(eventFacing - facing);

        if (distance < 0)
          distance = -distance;
        if (distance > TRIG_TURN * 3 / 32 - 1)
          continue;
      }
      if (filter != PSYNERGY_TRIGGER_ANY &&
          (event->control & (PSYNERGY_PHASE_MASK | TRIGGER_KIND_MASK)) !=
              filter)
        continue;
      if (event->control & TRIGGER_PSYNERGY_ANYWHERE) {
        if ((selector & PSYNERGY_TARGET_KIND) == PSYNERGY_TARGET_ANYWHERE)
          return event;
      } else if (event->control & TRIGGER_PSYNERGY_ON_ACTOR) {
        if ((selector & PSYNERGY_TARGET_KIND) == PSYNERGY_TARGET_ACTOR &&
            (selector & PSYNERGY_TARGET_ID) == eventTarget)
          return event;
      } else if ((selector & PSYNERGY_TARGET_KIND) == 0 &&
                 (selector & PSYNERGY_TARGET_ID) == eventTarget) {
        return event;
      }
    }
  }
  return NULL;
}

/// Runs the scene's events for Psynergy without paying its PP or showing its
/// usual effect (PSYNERGY_PHASE_BEFORE_END and PSYNERGY_PHASE_AFTER_END, around
/// the kind's ending), as for a Psynergy that ends by itself: packed holds the
/// Psynergy (bits 0 to 9) and its user (bits 10 to 13).
s32 Field_RunPsynergyEndEvents(u32 packed) {
  s32 action, user, kind, selector, target;
  struct PendingEvent *beforeEnd, *afterEnd;

  action = ABILITY_ID(packed);
  user = ACT_UNIT(packed);
  kind = Ability_Get(action)->fieldPsynergy;
  selector = Scene_SelectPsynergyTarget(kind);
  beforeEnd = (struct PendingEvent *)Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_END), kind, selector);
  afterEnd = (struct PendingEvent *)Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END), kind, selector);
  target = -1;
  if (beforeEnd != NULL || afterEnd != NULL) {
    if (selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
      target = selector & PSYNERGY_TARGET_ID;
  }
  FieldPsynergy_LoadResources(action, 0);
  FieldPsynergy_SetupActorPair(Party_GetLeaderActor(), target);
  Field_RunPsynergyEvent(beforeEnd, user, target);
  FieldPsynergy_RunExtraHandler();
  BattleEffect_StopMainActor();
  Field_RunPsynergyEvent(afterEnd, user, target);
  FieldPsynergy_End();
  return 0;
}
