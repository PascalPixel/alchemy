/// @file
/// Using an item on the field.
// Part of object.c.

#include "../object_internal.h"

/// The function a scene's item event may name instead of a message, called
/// with the item, its user and its inventory slot; its result is not read.
typedef s32 (*BattleItemCallback)(s32 item, s32 actor, s32 slot);

/// Returns the scene's item event (EVENT_ITEM) for item that applies to what
/// the leader faces, or 0 (item 0 matches any item). The event's trigger names
/// an actor (EVENT_ITEM_ON_ACTOR) or a tile code, and may also require the
/// leader to face within 0x17ff (about 34 degrees) of a direction. Called by
/// Field_UseItem, and by FieldEffect_HasTrigger (psynergy.c) to test whether an
/// item would do anything here.
s32 Field_FindItemEvent(s32 item) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  const struct SceneEvent *event = work->events;
  s32 facing =
      ((struct MotionObject *)ObjectTable_Get(gGameState.selectedActor))->angle;
  s32 actorAhead = Field_FindNearbyTalkTarget(gGameState.selectedActor);
  s32 tileAhead;

  item &= 0x1ff;
  // Query both target forms once; matching events choose actor or tile below.
  tileAhead = Field_GetCollisionAheadOfLeader();

  // trigger bits: 15..12 facing | 11 facing required | 7..0 actor or tile
  // code; control bits: 15..8 item | 4 on an actor | 3..0 kind.
  while (event->control != -1) {
    s32 eventFacing = event->trigger & 0xf000;
    s16 requiresFacing = event->trigger & 0x0800;
    s32 eventTarget = event->trigger & 0xff;

    if ((event->control & 0x0f) == EVENT_ITEM &&
        GameFlag_IsConditionActive(event->condition) != 0 &&
        (requiresFacing == 0 ||
         (u16)(eventFacing - facing + 0x17ff) <= 0x2ffe)) {
      u32 flags = event->control;
      s32 eventItem = ((const u8 *)&event->control)[1];

      if (item == 0 || eventItem == item) {
        if ((flags & EVENT_ITEM_ON_ACTOR) != 0) {
          if (eventTarget == actorAhead)
            return (s32)event;
        } else if (eventTarget == tileAhead) {
          return (s32)event;
        }
      }
    }
    event++;
  }
  return 0;
}

/// Uses an item on the field. packedItem holds the item (bits 0 to 9) and the
/// character using it (bits 10 to 13; 15 for the character carrying most of
/// it); slot is its inventory slot. The scene's item event for what the leader
/// faces runs if there is one; otherwise an item with a field action uses it
/// (Retreat asks first and leaves the scene). FLAG_NOTHING_HAPPENS still set
/// afterwards says "But nothing happens."; FLAG_ITEM_USED_UP removes the item.
/// Returns 0 when the scene's event ran or Retreat was declined, else -1, the
/// item's own action included; Field_RunScene ignores it.
/// Called by Field_RunScene for the item request (work->item and
/// work->itemArgument). Retreat sets gGameState.scene and .entrance to the
/// Retreat point and raises work->exit.
s32 Field_UseItem(s32 packedItem, s32 slot) {
  s32 result;
  s32 itemId;
  s32 actor;
  struct BattleUnit *unit;
  const struct SceneEvent *event;
  s32 inventorySlot;
  s32 partyIndex;
  s32 mostMatchingSlots;
  s32 matchingSlots;
  s32 partyCount;
  struct FieldWork *work;

  result = -1;
  itemId = packedItem & 0x3ff;
  actor = (packedItem >> 10) & 0xf;
  // Count the user's slots holding the item; with no user given, the user is
  // whoever holds it in the most slots, the earlier on a tie. Nobody holding
  // it ends the use at once.
  {
    mostMatchingSlots = 0;
    partyCount = Party_CountActiveOwnersFar();

    if (actor == 15) {
      actor = 0;
      for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
        unit = UnitGetFar(gGameState.activeOwners[partyIndex]);
        matchingSlots = 0;
        for (inventorySlot = 0; inventorySlot < 15; inventorySlot++) {
          if ((unit->inventory[inventorySlot] & 0x1ff) == itemId)
            matchingSlots++;
        }

        if (mostMatchingSlots < matchingSlots) {
          mostMatchingSlots = matchingSlots;
          actor = gGameState.activeOwners[partyIndex];
        }
      }
    } else {
      unit = UnitGetFar(actor);
      for (inventorySlot = 0; inventorySlot < 15; inventorySlot++) {
        if ((unit->inventory[inventorySlot] & 0x1ff) == itemId)
          mostMatchingSlots++;
      }
    }

    if (mostMatchingSlots == 0) {
      UiText_ShowPositionedMessageAndWaitFar(MsgNothingHappens, 1);
      return -1;
    }
  }

  event = (const struct SceneEvent *)Field_FindItemEvent(itemId);
  if (event != 0 && event->value != 0) {
    // The scene's event: a message, or a function. A value below 0x10000 is
    // a message number; anything else is a code address. Trigger bit 0x400
    // skips the "uses the item" message.
    GameFlag_ClearBitFar(FLAG_ITEM_USED_UP);
    GameFlag_ClearBitFar(FLAG_NOTHING_HAPPENS);
    if (!(event->trigger & 0x400)) {
      UiWork_PushValueSlotFar(actor, 1);
      UiWork_PushValueSlotFar(itemId, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgActorUsesItem, 1);
    }
    if ((s32)event->value < 0x10000) {
      s32 nearbyActor = Field_FindNearbyTalkTarget(gGameState.selectedActor);
      FieldEvent_Begin();
      Event_SetMessageId(event->value);
      FieldEvent_ShowMessageAndWait(nearbyActor, 0);
      FieldEvent_End();
    } else {
      ((BattleItemCallback)event->value)(itemId, actor, slot);
    }
    result = 0;
  } else {
    // The item's own field action, if it has one: the ability actionId names
    // plays as if cast. Items with useType bit 0 are used up.
    s32 actionId;

    GameFlag_ClearBitFar(FLAG_ITEM_USED_UP);
    GameFlag_SetBitFar(FLAG_NOTHING_HAPPENS);
    actionId = Item_GetDirectFar(itemId)->actionId;
    work = (struct FieldWork *)gEventWork;

    if (actionId != 0) {
      GameFlag_SetBitFar(FLAG_ITEM_USE);
      GameFlag_ClearBitFar(FLAG_NOTHING_HAPPENS);

      if (actionId == ABILITY_RETREAT &&
          !GameFlag_TestFar(FLAG_RETREAT_BLOCKED)) {
        UiWork_PushValueSlotFar(itemId, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgUseItemQuestion, 13);
        /* FAKEMATCH(temporary): the answer kept in partyIndex, which ranks it
         * higher in global allocation (r6, as the bytes have it); as a local
         * of its own, 109 of 253 instructions differ in tbs-en. */
        partyIndex = Menu_AskYesNo(1);
        UiWork_FinalizePendingCoreFar();
        if (partyIndex != 0)
          return 0;

        gGameState.scene = gGameState.retreatScene;
        gGameState.entrance = gGameState.retreatEntrance;
        work->exit = EXIT_RETREAT;
      }

      UiWork_PushValueSlotFar(actor, 1);
      UiWork_PushValueSlotFar(itemId, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgActorUsesItem, 1);
      // UB: this routine returns void; call it through its declared type.
#ifdef UBFIX
      FieldEffect_LoadActionEffectResources(actionId, 0);
#else
      /* FAKEMATCH(wrapper): called through a cast to the s32 function the
       * bytes show, which loads the action id before the 0; as a call by its
       * void prototype, 2 of 253 instructions differ in tbs-en. */
      ((s32 (*)(s32, s32))FieldEffect_LoadActionEffectResources)(actionId, 0);
#endif
      work->resolvingAction = 1;
      FieldEffect_Run();
      work->resolvingAction = 0;
      FieldEffect_End();

      if (Item_GetDirectFar(itemId)->useType & 1)
        GameFlag_SetBitFar(FLAG_ITEM_USED_UP);
    }
  }

  if (GameFlag_TestFar(FLAG_NOTHING_HAPPENS))
    UiText_ShowPositionedMessageAndWaitFar(MsgNothingHappens, 1);
  if (GameFlag_TestFar(FLAG_ITEM_USED_UP))
    Inventory_RemoveFar(actor, slot);
  return result;
}

/// Returns the scene's Psynergy event (EVENT_PSYNERGY) for Psynergy of the
/// given group that applies to what the leader faces, or NULL; stores the actor
/// the Psynergy would target in *target. eventFlags gives the event's phase and
/// kind (PSYNERGY_EV_ANY for any phase). The event may apply to any target
/// (EVENT_PSYNERGY_IN_AREA), to an actor (EVENT_PSYNERGY_ON_ACTOR) or to a
/// tile code. Called by
/// Field_UsePsynergy for each phase, by Field_RunPsynergyEvents, and by the
/// Force, Frost and Reveal effects (psynergy_effects/) for their own phases.
void *FieldEffect_FindMatchingEvent(s32 eventFlags, s32 group, void *target) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  const struct SceneEvent *event = work->events;
  s32 facing =
      ((struct MotionObject *)ObjectTable_Get(gGameState.selectedActor))->angle;
  s32 actorAhead =
      Field_FindNearbyPsynergyTarget(gGameState.selectedActor, group);
  s32 tileAhead;
  s32 anyPhase = 0;

  *(s32 *)target = actorAhead;
  // The target output is filled even when no event ultimately matches.
  tileAhead = Field_GetCollisionAheadOfLeader();
  if (eventFlags == PSYNERGY_EV_ANY)
    anyPhase = 1;

  while (event->control != -1) {
    s32 eventFacing = event->trigger & 0xf000;
    s16 requiresFacing = event->trigger & 0x0800;
    s32 eventTarget = event->trigger & 0xff;

    if ((event->control & 0x0f) == EVENT_PSYNERGY &&
        GameFlag_IsConditionActive(event->condition) != 0 &&
        (requiresFacing == 0 ||
         (u16)(eventFacing - facing + 0x17ff) <= 0x2ffe) &&
        BattleActGet(((const u8 *)&event->control)[1])->type0c == group &&
        (anyPhase || (event->control & 0x7000000f) == eventFlags)) {
      // Any Psynergy of the same field kind (type0c) as the event's matches.
      if ((event->control & EVENT_PSYNERGY_IN_AREA) != 0)
        return (void *)event;
      if ((event->control & EVENT_PSYNERGY_ON_ACTOR) != 0) {
        if (eventTarget == actorAhead)
          return (void *)event;
      } else if (eventTarget == tileAhead) {
        return (void *)event;
      }
    }
    event++;
  }
  return NULL;
}

/// Runs the scene's events for Psynergy without paying its PP or showing its
/// usual effect (PSYNERGY_PHASE_BEFORE_END and PSYNERGY_PHASE_AFTER_END, around
/// the kind's ending), as for a
/// Psynergy that ends by itself: packed holds the Psynergy (bits 0 to 9) and
/// its user (bits 10 to 13). Called by Field_UsePsynergy for a use with
/// ACT_PACKED, and by Field_EndBusyAndReveal to end Reveal.
s32 Field_RunPsynergyEvents(s32 packed) {
  s32 target;
  s32 abilityId;
  s32 userId;
  u8 group;
  void *beforeEvent;
  void *afterEvent;

  abilityId = packed & 0x3FF;
  userId = ((u32)packed >> 10) & 0xF;
  group = BattleActGet(abilityId)->type0c;
  // The leader is looked up and the result is not used.
  ObjectTable_Get(gGameState.selectedActor);
  beforeEvent = FieldEffect_FindMatchingEvent(
      PSYNERGY_EV(PSYNERGY_PHASE_BEFORE_END), group, &target);
  afterEvent = FieldEffect_FindMatchingEvent(
      PSYNERGY_EV(PSYNERGY_PHASE_AFTER_END), group, &target);
  FieldEffect_LoadActionEffectResources(abilityId, 0);
  FieldEffect_SetupObjectPair(gGameState.selectedActor, target);
  Field_RunEventAction(beforeEvent, userId, target);
  FieldEvent_RunTypeHandler();
  FieldEffect_StopCasterHook();
  Field_RunEventAction(afterEvent, userId, target);
  FieldEffect_End();
  return 0;
}
