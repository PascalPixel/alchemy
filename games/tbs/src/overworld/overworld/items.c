/// @file
/// Using an item on the field.
// Part of overworld.c.

#include "../overworld_internal.h"

/// The function a scene's item trigger may name instead of a message, called
/// with the item, its user and its inventory slot; its result is not read.
typedef s32 (*BattleItemCallback)(s32 item, s32 actorId, s32 slot);

/// Returns the scene's item trigger (TRIGGER_ITEM) for item that applies to
/// what the leader faces, or 0 (item 0 matches any item). The event's trigger
/// names an actor (TRIGGER_ITEM_ON_ACTOR) or a cell behavior,
/// and may also require the leader to face within 0x17ff (about 34 degrees) of
/// a direction. Called by Field_UseItem, and by Scene_HasPsynergyTrigger
/// (psynergy.c) to test whether an item would do anything here.
s32 Field_FindItemTrigger(s32 item) {
  struct FieldWork *work = gFieldWork;
  const struct Trigger *trigger = work->triggers;
  s32 facing =
      ((struct Actor *)Actor_Lookup(gMainRoutineWork.leaderActorId))->facing;
  s32 actorAhead = Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId);
  s32 tileAhead;

  item &= 0x1ff;
  // Query both target forms once; matching events choose actor or cell
  // below.
  tileAhead = Field_GetCollisionAheadOfLeader();

  // subject bits: 15..12 facing | 11 facing required | 7..0 actor or
  // cell behavior; control bits: 15..8 item | 4 on an actor | 3..0
  // kind.
  while (trigger->kind != -1) {
    s32 eventFacing = trigger->subject & 0xf000;
    s16 requiresFacing = trigger->subject & 0x0800;
    s32 eventTarget = trigger->subject & 0xff;

    if ((trigger->kind & 0x0f) == TRIGGER_ITEM &&
        GameFlag_IsConditionActive(trigger->condition) != 0 &&
        (requiresFacing == 0 ||
         (u16)(eventFacing - facing + 0x17ff) <= 0x2ffe)) {
      u32 flags = trigger->kind;
      s32 eventItem = ((const u8 *)&trigger->kind)[1];

      if (item == 0 || eventItem == item) {
        if ((flags & TRIGGER_ITEM_ON_ACTOR) != 0) {
          if (eventTarget == actorAhead)
            return (s32)trigger;
        } else if (eventTarget == tileAhead) {
          return (s32)trigger;
        }
      }
    }
    trigger++;
  }
  return 0;
}

/// Uses an item on the field. packedItem holds the item (bits 0 to 9) and the
/// character using it (bits 10 to 13; 15 for the character carrying most of
/// it); slot is its inventory slot. The scene's item trigger for what the
/// leader faces runs if there is one; otherwise an item with a field action
/// uses it (Retreat asks first and leaves the scene). FLAG_NOTHING_HAPPENS
/// still set afterwards says "But nothing happens."; FLAG_ITEM_USED_UP removes
/// the item. Returns 0 when the scene's event ran or Retreat was declined, else
/// -1, the item's own action included; FieldSequence_Run ignores it. Called by
/// FieldSequence_Run for the item request (work->item and work->itemSlot).
/// Retreat sets gMainRoutineWork.sceneId and .entrance to the Retreat point
/// and raises work->exit.
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
  struct FieldWork *work;

  result = -1;
  itemId = packedItem & 0x3ff;
  actorId = (packedItem >> 10) & 0xf;
  // Count the user's slots holding the item; with no user given, the user is
  // whoever holds it in the most slots, the earlier on a tie. Nobody holding
  // it ends the use at once.
  {
    mostMatchingSlots = 0;
    partyCount = Party_CountMembersFar();

    if (actorId == 15) {
      actorId = 0;
      for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
        unit = Unit_GetFar(gMainRoutineWork.party[partyIndex]);
        matchingSlots = 0;
        for (inventorySlot = 0; inventorySlot < 15; inventorySlot++) {
          if ((unit->inventory[inventorySlot] & 0x1ff) == itemId)
            matchingSlots++;
        }

        if (mostMatchingSlots < matchingSlots) {
          mostMatchingSlots = matchingSlots;
          actorId = gMainRoutineWork.party[partyIndex];
        }
      }
    } else {
      unit = Unit_GetFar(actorId);
      for (inventorySlot = 0; inventorySlot < 15; inventorySlot++) {
        if ((unit->inventory[inventorySlot] & 0x1ff) == itemId)
          mostMatchingSlots++;
      }
    }

    if (mostMatchingSlots == 0) {
      FieldMessage_ShowFar(MsgNothingHappens, FIELD_MESSAGE_ZOOM);
      return -1;
    }
  }

  trigger = (const struct Trigger *)Field_FindItemTrigger(itemId);
  if (trigger != 0 && trigger->value != 0) {
    // The scene's event: a message, or a function. A value below 0x10000 is
    // a message number; anything else is a code address. Trigger bit 0x400
    // skips the "uses the item" message.
    GameFlag_ClearFar(FLAG_ITEM_USED_UP);
    GameFlag_ClearFar(FLAG_NOTHING_HAPPENS);
    if (!(trigger->subject & 0x400)) {
      MessageArgument_PushFar(actorId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar(MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
    }
    if ((s32)trigger->value < 0x10000) {
      s32 nearbyActorId =
          Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId);
      Event_Start();
      Event_SetMessageId(trigger->value);
      Event_ShowMessageAndWait(nearbyActorId, 0);
      Event_End();
    } else {
      ((BattleItemCallback)trigger->value)(itemId, actorId, slot);
    }
    result = 0;
  } else {
    // The item's own field action, if it has one: the ability abilityId names
    // plays as if cast. Items with useType bit 0 are used up.
    s32 abilityId;

    GameFlag_ClearFar(FLAG_ITEM_USED_UP);
    GameFlag_SetFar(FLAG_NOTHING_HAPPENS);
    abilityId = Item_GetDirectFar(itemId)->abilityId;
    work = gFieldWork;

    if (abilityId != 0) {
      GameFlag_SetFar(FLAG_ITEM_USE);
      GameFlag_ClearFar(FLAG_NOTHING_HAPPENS);

      if (abilityId == ABILITY_RETREAT &&
          !GameFlag_GetFar(FLAG_RETREAT_BLOCKED)) {
        MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar(MsgUseItemQuestion, FIELD_MESSAGE_ZOOM |
                                                     FIELD_MESSAGE_KEEP_OPEN |
                                                     FIELD_MESSAGE_LOWER);
        /* FAKEMATCH(temporary): the answer kept in partyIndex, which ranks it
         * higher in global allocation (r6, as the bytes have it); as a local
         * of its own, 109 of 253 instructions differ in tbs-en. */
        partyIndex = FieldYesNo_Ask(1);
        Message_CloseFinishedWindowsFar();
        if (partyIndex != 0)
          return 0;

        gMainRoutineWork.sceneId = gMainRoutineWork.retreatScene;
        gMainRoutineWork.entrance = gMainRoutineWork.retreatEntrance;
        work->exit = EXIT_KEEP_DESTINATION;
      }

      MessageArgument_PushFar(actorId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar(MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
      // UB: this routine returns void; call it through its declared type.
#ifdef UBFIX
      FieldPsynergy_LoadResources(abilityId, 0);
#else
      /* FAKEMATCH(wrapper): called through a cast to the s32 function the
       * bytes show, which loads the action id before the 0; as a call by its
       * void prototype, 2 of 253 instructions differ in tbs-en. */
      ((s32 (*)(s32, s32))FieldPsynergy_LoadResources)(abilityId, 0);
#endif
      work->psynergyActing = 1;
      FieldPsynergy_Run();
      work->psynergyActing = 0;
      FieldPsynergy_End();

      if (Item_GetDirectFar(itemId)->useType & 1)
        GameFlag_SetFar(FLAG_ITEM_USED_UP);
    }
  }

  if (GameFlag_GetFar(FLAG_NOTHING_HAPPENS))
    FieldMessage_ShowFar(MsgNothingHappens, FIELD_MESSAGE_ZOOM);
  if (GameFlag_GetFar(FLAG_ITEM_USED_UP))
    Inventory_RemoveFar(actorId, slot);
  return result;
}

/// Returns the scene's Psynergy trigger (TRIGGER_PSYNERGY) for Psynergy of the
/// given group that applies to what the leader faces, or NULL; stores the
/// actor the Psynergy would target in *target. eventFlags gives the
/// event's phase and kind (PSYNERGY_TRIGGER_ANY for any phase). The event may
/// apply to any target (TRIGGER_PSYNERGY_ANYWHERE), to an actor
/// (TRIGGER_PSYNERGY_ON_ACTOR) or to a cell behavior. Called by
/// Field_UsePsynergy for each phase, by Field_RunPsynergyEndEvents, and by the
/// Force, Frost and Reveal (event_commands/) for their own phases.
void *Scene_FindPsynergyTrigger(s32 eventFlags, s32 group, void *target) {
  struct FieldWork *work = gFieldWork;
  const struct Trigger *trigger = work->triggers;
  s32 facing =
      ((struct Actor *)Actor_Lookup(gMainRoutineWork.leaderActorId))->facing;
  s32 actorAhead =
      Field_FindNearbyPsynergyTarget(gMainRoutineWork.leaderActorId, group);
  s32 tileAhead;
  s32 anyPhase = 0;

  *(s32 *)target = actorAhead;
  // The target output is filled even when no event ultimately matches.
  tileAhead = Field_GetCollisionAheadOfLeader();
  if (eventFlags == PSYNERGY_TRIGGER_ANY)
    anyPhase = 1;

  while (trigger->kind != -1) {
    s32 eventFacing = trigger->subject & 0xf000;
    s16 requiresFacing = trigger->subject & 0x0800;
    s32 eventTarget = trigger->subject & 0xff;

    if ((trigger->kind & 0x0f) == TRIGGER_PSYNERGY &&
        GameFlag_IsConditionActive(trigger->condition) != 0 &&
        (requiresFacing == 0 ||
         (u16)(eventFacing - facing + 0x17ff) <= 0x2ffe) &&
        Ability_Get(((const u8 *)&trigger->kind)[1])->fieldPsynergy == group &&
        (anyPhase || (trigger->kind & 0x7000000f) == eventFlags)) {
      // Any Psynergy of the same field kind (fieldPsynergy) as the event's
      // matches.
      if ((trigger->kind & TRIGGER_PSYNERGY_ANYWHERE) != 0)
        return (void *)trigger;
      if ((trigger->kind & TRIGGER_PSYNERGY_ON_ACTOR) != 0) {
        if (eventTarget == actorAhead)
          return (void *)trigger;
      } else if (eventTarget == tileAhead) {
        return (void *)trigger;
      }
    }
    trigger++;
  }
  return NULL;
}

/// Runs the scene's events for Psynergy without paying its PP or showing its
/// usual effect (PSYNERGY_PHASE_BEFORE_END and PSYNERGY_PHASE_AFTER_END, around
/// the kind's ending), as for a
/// Psynergy that ends by itself: packed holds the Psynergy (bits 0 to 9) and
/// its user (bits 10 to 13). Called by Field_UsePsynergy for a use with
/// ACT_PACKED, and by Field_EndBusyAndReveal to end Reveal.
s32 Field_RunPsynergyEndEvents(s32 packed) {
  s32 target;
  s32 abilityId;
  s32 userId;
  u8 group;
  void *beforeEvent;
  void *afterEvent;

  abilityId = packed & 0x3FF;
  userId = ((u32)packed >> 10) & 0xF;
  group = Ability_Get(abilityId)->fieldPsynergy;
  // The leader is looked up and the result is not used.
  Actor_Lookup(gMainRoutineWork.leaderActorId);
  beforeEvent = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_END), group, &target);
  afterEvent = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END), group, &target);
  FieldPsynergy_LoadResources(abilityId, 0);
  FieldPsynergy_SetupActorPair(gMainRoutineWork.leaderActorId, target);
  Field_RunPsynergyEvent(beforeEvent, userId, target);
  Event_RunFieldPsynergy();
  FieldPsynergy_StopCasterUpdate();
  Field_RunPsynergyEvent(afterEvent, userId, target);
  FieldPsynergy_End();
  return 0;
}
