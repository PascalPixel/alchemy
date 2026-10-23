/// @file
/// Using an item on the field.
// Part of overworld.c.

#include "../overworld_internal.h"

/// The function a map's item event may name instead of a message, called
/// with the item, its user and its inventory slot; its result is not read.
typedef s32 (*BattleItemCallback)(s32 item, s32 localId, s32 slot);

/// Returns the map's item event (MAP_EVENT_ITEM) for item that applies to
/// what the leader faces, or 0 (item 0 matches any item). The event's trigger
/// names an object event (MAP_EVENT_ITEM_ON_OBJ_EVENT) or a metatile behavior,
/// and may also require the leader to face within 0x17ff (about 34 degrees) of
/// a direction. Called by Field_UseItem, and by Map_HasPsynergyEvent
/// (psynergy.c) to test whether an item would do anything here.
s32 Field_FindItemEvent(s32 item) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;
  const struct MapEvent *event = work->events;
  s32 facing = ((struct MotionObject *)ObjectEvent_GetByLocalId(
                    gGameState.leaderLocalId))
                   ->angle;
  s32 objectEventAhead = Field_FindNearbyTalkTarget(gGameState.leaderLocalId);
  s32 tileAhead;

  item &= 0x1ff;
  // Query both target forms once; matching events choose object event or cell
  // below.
  tileAhead = Field_GetCollisionAheadOfLeader();

  // trigger bits: 15..12 facing | 11 facing required | 7..0 object event or
  // metatile behavior; control bits: 15..8 item | 4 on an object event | 3..0
  // kind.
  while (event->kind != -1) {
    s32 eventFacing = event->trigger & 0xf000;
    s16 requiresFacing = event->trigger & 0x0800;
    s32 eventTarget = event->trigger & 0xff;

    if ((event->kind & 0x0f) == MAP_EVENT_ITEM &&
        GameFlag_IsConditionActive(event->condition) != 0 &&
        (requiresFacing == 0 ||
         (u16)(eventFacing - facing + 0x17ff) <= 0x2ffe)) {
      u32 flags = event->kind;
      s32 eventItem = ((const u8 *)&event->kind)[1];

      if (item == 0 || eventItem == item) {
        if ((flags & MAP_EVENT_ITEM_ON_OBJ_EVENT) != 0) {
          if (eventTarget == objectEventAhead)
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
/// it); slot is its inventory slot. The map's item event for what the leader
/// faces runs if there is one; otherwise an item with a field action uses it
/// (Retreat asks first and leaves the map). FLAG_NOTHING_HAPPENS still set
/// afterwards says "But nothing happens."; FLAG_ITEM_USED_UP removes the item.
/// Returns 0 when the map's event ran or Retreat was declined, else -1, the
/// item's own action included; Map_Run ignores it.
/// Called by Map_Run for the item request (work->item and
/// work->itemArgument). Retreat sets gGameState.mapId and .warpDest to the
/// Retreat point and raises work->warp.
s32 Field_UseItem(s32 packedItem, s32 slot) {
  s32 result;
  s32 itemId;
  s32 localId;
  struct Unit *unit;
  const struct MapEvent *event;
  s32 inventorySlot;
  s32 partyIndex;
  s32 mostMatchingSlots;
  s32 matchingSlots;
  s32 partyCount;
  struct FieldWork *work;

  result = -1;
  itemId = packedItem & 0x3ff;
  localId = (packedItem >> 10) & 0xf;
  // Count the user's slots holding the item; with no user given, the user is
  // whoever holds it in the most slots, the earlier on a tie. Nobody holding
  // it ends the use at once.
  {
    mostMatchingSlots = 0;
    partyCount = Party_CountMembersFar();

    if (localId == 15) {
      localId = 0;
      for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
        unit = Unit_GetFar(gGameState.party[partyIndex]);
        matchingSlots = 0;
        for (inventorySlot = 0; inventorySlot < 15; inventorySlot++) {
          if ((unit->inventory[inventorySlot] & 0x1ff) == itemId)
            matchingSlots++;
        }

        if (mostMatchingSlots < matchingSlots) {
          mostMatchingSlots = matchingSlots;
          localId = gGameState.party[partyIndex];
        }
      }
    } else {
      unit = Unit_GetFar(localId);
      for (inventorySlot = 0; inventorySlot < 15; inventorySlot++) {
        if ((unit->inventory[inventorySlot] & 0x1ff) == itemId)
          mostMatchingSlots++;
      }
    }

    if (mostMatchingSlots == 0) {
      FieldMessage_ShowFar(MsgNothingHappens, 1);
      return -1;
    }
  }

  event = (const struct MapEvent *)Field_FindItemEvent(itemId);
  if (event != 0 && event->value != 0) {
    // The map's event: a message, or a function. A value below 0x10000 is
    // a message number; anything else is a code address. Trigger bit 0x400
    // skips the "uses the item" message.
    GameFlag_ClearBitFar(FLAG_ITEM_USED_UP);
    GameFlag_ClearBitFar(FLAG_NOTHING_HAPPENS);
    if (!(event->trigger & 0x400)) {
      MessageArgument_PushFar(localId, 1);
      MessageArgument_PushFar(itemId, 2);
      FieldMessage_ShowFar(MsgActorUsesItem, 1);
    }
    if ((s32)event->value < 0x10000) {
      s32 nearbyLocalId = Field_FindNearbyTalkTarget(gGameState.leaderLocalId);
      FieldScript_Start();
      FieldScript_SetMessageId(event->value);
      FieldScript_ShowMessageAndWait(nearbyLocalId, 0);
      FieldScript_End();
    } else {
      ((BattleItemCallback)event->value)(itemId, localId, slot);
    }
    result = 0;
  } else {
    // The item's own field action, if it has one: the ability actionId names
    // plays as if cast. Items with useType bit 0 are used up.
    s32 actionId;

    GameFlag_ClearBitFar(FLAG_ITEM_USED_UP);
    GameFlag_SetBitFar(FLAG_NOTHING_HAPPENS);
    actionId = Item_GetDirectFar(itemId)->abilityId;
    work = (struct FieldWork *)gFieldWork;

    if (actionId != 0) {
      GameFlag_SetBitFar(FLAG_ITEM_USE);
      GameFlag_ClearBitFar(FLAG_NOTHING_HAPPENS);

      if (actionId == ABILITY_RETREAT &&
          !GameFlag_TestFar(FLAG_RETREAT_BLOCKED)) {
        MessageArgument_PushFar(itemId, 2);
        FieldMessage_ShowFar(MsgUseItemQuestion, 13);
        /* FAKEMATCH(temporary): the answer kept in partyIndex, which ranks it
         * higher in global allocation (r6, as the bytes have it); as a local
         * of its own, 109 of 253 instructions differ in tbs-en. */
        partyIndex = FieldYesNo_Ask(1);
        UiWork_FinalizePendingCoreFar();
        if (partyIndex != 0)
          return 0;

        gGameState.mapId = gGameState.retreatMap;
        gGameState.warpDest = gGameState.retreatWarpDest;
        work->warp = WARP_RETREAT;
      }

      MessageArgument_PushFar(localId, 1);
      MessageArgument_PushFar(itemId, 2);
      FieldMessage_ShowFar(MsgActorUsesItem, 1);
      // UB: this routine returns void; call it through its declared type.
#ifdef UBFIX
      FieldEffect_LoadActionEffectResources(actionId, 0);
#else
      /* FAKEMATCH(wrapper): called through a cast to the s32 function the
       * bytes show, which loads the action id before the 0; as a call by its
       * void prototype, 2 of 253 instructions differ in tbs-en. */
      ((s32 (*)(s32, s32))FieldEffect_LoadActionEffectResources)(actionId, 0);
#endif
      work->psynergyActing = 1;
      FieldEffect_Run();
      work->psynergyActing = 0;
      FieldEffect_End();

      if (Item_GetDirectFar(itemId)->useType & 1)
        GameFlag_SetBitFar(FLAG_ITEM_USED_UP);
    }
  }

  if (GameFlag_TestFar(FLAG_NOTHING_HAPPENS))
    FieldMessage_ShowFar(MsgNothingHappens, 1);
  if (GameFlag_TestFar(FLAG_ITEM_USED_UP))
    Inventory_RemoveFar(localId, slot);
  return result;
}

/// Returns the map's Psynergy event (MAP_EVENT_PSYNERGY) for Psynergy of the
/// given group that applies to what the leader faces, or NULL; stores the
/// object event the Psynergy would target in *target. eventFlags gives the
/// event's phase and kind (PSYNERGY_EV_ANY for any phase). The event may apply
/// to any target (MAP_EVENT_PSYNERGY_IN_AREA), to an object event
/// (MAP_EVENT_PSYNERGY_ON_OBJ_EVENT) or to a metatile behavior. Called by
/// Field_UsePsynergy for each phase, by Field_RunPsynergyEvents, and by the
/// Force, Frost and Reveal effects (psynergy_effects/) for their own phases.
void *Map_FindPsynergyEvent(s32 eventFlags, s32 group, void *target) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;
  const struct MapEvent *event = work->events;
  s32 facing = ((struct MotionObject *)ObjectEvent_GetByLocalId(
                    gGameState.leaderLocalId))
                   ->angle;
  s32 objectEventAhead =
      Field_FindNearbyPsynergyTarget(gGameState.leaderLocalId, group);
  s32 tileAhead;
  s32 anyPhase = 0;

  *(s32 *)target = objectEventAhead;
  // The target output is filled even when no event ultimately matches.
  tileAhead = Field_GetCollisionAheadOfLeader();
  if (eventFlags == PSYNERGY_EV_ANY)
    anyPhase = 1;

  while (event->kind != -1) {
    s32 eventFacing = event->trigger & 0xf000;
    s16 requiresFacing = event->trigger & 0x0800;
    s32 eventTarget = event->trigger & 0xff;

    if ((event->kind & 0x0f) == MAP_EVENT_PSYNERGY &&
        GameFlag_IsConditionActive(event->condition) != 0 &&
        (requiresFacing == 0 ||
         (u16)(eventFacing - facing + 0x17ff) <= 0x2ffe) &&
        Ability_Get(((const u8 *)&event->kind)[1])->fieldPsynergy == group &&
        (anyPhase || (event->kind & 0x7000000f) == eventFlags)) {
      // Any Psynergy of the same field kind (fieldPsynergy) as the event's
      // matches.
      if ((event->kind & MAP_EVENT_PSYNERGY_IN_AREA) != 0)
        return (void *)event;
      if ((event->kind & MAP_EVENT_PSYNERGY_ON_OBJ_EVENT) != 0) {
        if (eventTarget == objectEventAhead)
          return (void *)event;
      } else if (eventTarget == tileAhead) {
        return (void *)event;
      }
    }
    event++;
  }
  return NULL;
}

/// Runs the map's events for Psynergy without paying its PP or showing its
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
  group = Ability_Get(abilityId)->fieldPsynergy;
  // The leader is looked up and the result is not used.
  ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
  beforeEvent = Map_FindPsynergyEvent(PSYNERGY_EVENT(PSYNERGY_PHASE_BEFORE_END),
                                      group, &target);
  afterEvent = Map_FindPsynergyEvent(PSYNERGY_EVENT(PSYNERGY_PHASE_AFTER_END),
                                     group, &target);
  FieldEffect_LoadActionEffectResources(abilityId, 0);
  FieldEffect_SetupObjectPair(gGameState.leaderLocalId, target);
  Field_RunEventAction(beforeEvent, userId, target);
  FieldScript_RunTypeHandler();
  FieldEffect_StopCasterHook();
  Field_RunEventAction(afterEvent, userId, target);
  FieldEffect_End();
  return 0;
}
