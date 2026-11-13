/// @file
/// Using an item on the field.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"

void Event_RunMessage(s32 actor, s32 message) {
  Event_Start();
  Event_SetMessageId(message);
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

void Event_ResolveAllPendingActions(void) {
  Event_ResolvePendingActions(0);
}

void Event_ResolvePendingActions(s32 skip) {
  struct EventWork *state = Ram_HeapSlots->eventWork;

  state->actionCounter = 0;
  if (state->pendingFirst != 0 && !(skip & 1))
    Event_ExecutePackedAction(0x2090);
  if (state->pendingSecond != 0 && !(skip & 2))
    Event_ExecutePackedAction(0x209b);
  if (gMainRoutineWork.pendingThird != 0 && !(skip & 4))
    Event_ExecutePackedAction(0x208b);
}

s32 Party_GetLeaderActor(void) {
  struct MainRoutineWork *state = &gMainRoutineWork;
  s32 actor;

  actor = 8;
  if (state->ownerOverride == 0)
    actor = state->currentOwner;
  return actor;
}

s32 Item_MatchesFilter(s32 filter, s32 item_id) {
  if (filter == item_id)
    return 1;
  if (filter == 0x1ff)
    return 1;
  if (filter == 0x1fe && (u32)(Item_Get(item_id)->type - 1) <= 3)
    return 1;
  return 0;
}

// The field's event work as this function reads it: the scene's trigger table.
/// Finds the scene's first kind-4 trigger whose condition holds, whose facing
/// range (when it has one) takes the controlled actor's angle, whose item
/// filter takes source (when given), and whose target is the actor the leader
/// can talk to (flag 0x10) or the object it is touching; 0 when none does.
/// Rewritten from The Broken Seal's form: The Lost Age asks
/// Party_GetLeaderActor for the actor and Item_MatchesFilter for
/// the trigger's item.
s32 Event_FindFacingTrigger(s32 source) {
  struct FieldStepWork *work = Ram_HeapSlots->eventWork;
  const struct Trigger *trigger = work->triggers;
  s32 facing = ObjectTable_Get(Party_GetLeaderActor())->facing;
  s32 selected = Field_FindNearbyTalkTarget(Party_GetLeaderActor());
  s32 collision;

  source &= 0x1ff;
  collision = Func_080cb09c();

  while (trigger->control != -1) {
    s32 highValue = (s16)(u16)trigger->subject & 0xf000;
    s16 hasFacing = (u16)trigger->subject & 0x0800;
    s32 lowValue = (u16)trigger->subject & 0xff;

    if ((trigger->control & 0x0f) == 4 &&
        GameFlag_IsConditionActive(trigger->condition) != 0 &&
        (hasFacing == 0 || (u16)(highValue - facing + 0x17ff) <= 0x2ffe)) {
      s32 item = (u16)((s32)trigger->control / 256);

      if (source == 0 || Item_MatchesFilter(item, source)) {
        if ((trigger->control & 0x10) != 0) {
          if (lowValue == selected)
            return (s32)trigger;
        } else if (lowValue == collision) {
          return (s32)trigger;
        }
      }
    }
    trigger++;
  }
  return 0;
}

struct ItemUnit {
  u8 unk000[0xd8];
  u16 inventory[15];
};

struct ItemTrigger {
  u8 unk00[4];
  u16 trigger;
  u8 unk06[2];
  u32 value;
};

struct ItemDefinitionView {
  u8 unk00[12];
  u8 useType;
  u8 unk0d[27];
  u16 actionId;
};

struct ItemFieldWork {
  u8 unk000[0x158];
  s16 exit;
  u8 unk15a[0xcb6 - 0x15a];
  u8 resolvingAction;
};

typedef void (*ItemTriggerCallback)(s32 item, s32 actor, s32 slot);

s32 Func_080d295c(s32 mode);
void Message_CloseFinishedWindowsFar(void);
void BattleEffect_CleanupSceneObjects(void);

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
  struct ItemUnit *obj;
  struct ItemTrigger *event;
  s32 j;
  s32 i;
  s32 best;
  s32 matches;
  s32 count;
  struct ItemFieldWork *runtime;

  result = -1;
  // The mask is assigned first, as games/tbs/src/MATCHING.md describes.
  itemId = 0x3ff;
  actorMask = 0xf;
  itemId &= packedItem;
  actor = (packedItem >> 10) & actorMask;
  {
    best = 0;
    count = Party_CountMembersFar();

    if (actor == 15) {
      actor = 0;
      for (i = 0; i < count; i++) {
        obj = Unit_Get(gMainRoutineWork.activeOwners[i]);
        matches = 0;
        for (j = 0; j < 15; j++) {
          if ((obj->inventory[j] & 0x1ff) == itemId)
            matches++;
        }

        if (best < matches) {
          best = matches;
          actor = gMainRoutineWork.activeOwners[i];
        }
      }
    } else {
      obj = Unit_Get(actor);
      for (j = 0; j < 15; j++) {
        if ((obj->inventory[j] & 0x1ff) == itemId)
          best++;
      }
    }

    if (best == 0) {
      FieldMessage_ShowFar((s32)&MsgNothingHappens, 1);
      return -1;
    }
  }

  event = (struct ItemTrigger *)Event_FindFacingTrigger(itemId);
  if (event != 0 && event->value != 0) {
    GameFlag_ClearBit(0x143);
    GameFlag_ClearBit(0x142);
    if (!(event->trigger & 0x400)) {
      MessageArgument_PushFar(actor, 1);
      MessageArgument_PushFar(itemId, 2);
      FieldMessage_ShowFar((s32)&MsgActorUsesItem, 1);
    }
    if ((s32)event->value < 0x10000) {
      s32 objref = Field_FindNearbyTalkTarget(Party_GetLeaderActor());

      Event_Start();
      Event_SetMessageId(event->value);
      Event_ShowMessageAndWait(objref, 0);
      Event_End();
    } else {
      ((ItemTriggerCallback)event->value)(itemId, actor, slot);
    }
    result = 0;
  } else {
    s32 actionId;

    GameFlag_ClearBit(0x143);
    GameFlag_SetBit(0x142);
    actionId = ((struct ItemDefinitionView *)Item_Get(itemId))->actionId;
    runtime = Ram_HeapSlots->eventWork;

    if (actionId != 0) {
      GameFlag_SetBit(0x145);
      GameFlag_ClearBit(0x142);

      if (actionId == 149 && !GameFlag_Test(0x144)) {
        MessageArgument_PushFar(itemId, 2);
        FieldMessage_ShowFar((s32)&MsgUseItemQuestion, 13);
        i = Func_080d295c(1);
        Message_CloseFinishedWindowsFar();
        if (i != 0)
          return 0;

        gMainRoutineWork.scene = *(u16 *)&gMainRoutineWork.retreatScene;
        gMainRoutineWork.entrance = *(u16 *)&gMainRoutineWork.retreatEntrance;
        runtime->exit = 999;
      }

      MessageArgument_PushFar(actor, 1);
      MessageArgument_PushFar(itemId, 2);
      FieldMessage_ShowFar((s32)&MsgActorUsesItem, 1);
      BattleEffect_LoadActionEffectResources(actionId, 0);
      runtime->resolvingAction = 1;
      BattleEffect_Run();
      runtime->resolvingAction = 0;
      BattleEffect_CleanupSceneObjects();

      if (((struct ItemDefinitionView *)Item_Get(itemId))->useType & 1)
        GameFlag_SetBit(0x143);
    }
  }

  if (GameFlag_Test(0x142))
    FieldMessage_ShowFar((s32)&MsgNothingHappens, 1);
  if (GameFlag_Test(0x143))
    Inventory_RemoveFar(actor, slot);
  return result;
}

struct TriggerEvent {
  s32 control;
  s16 trigger;
  s16 condition;
  u32 value;
};

struct TriggerWork {
  u8 unk000[0x10];
  struct TriggerEvent *events;
};

struct PendingAction {
  u8 unk00[6];
  u8 eventKind;
};

s32 Func_080ce31c(s32 kind) {
  struct TriggerWork *work = Ram_HeapSlots->eventWork;
  struct TriggerEvent *event = work->events;
  u32 facing = ObjectTable_Get(Party_GetLeaderActor())->facing;
  s32 focused = Func_080cb09c();
  s32 special = Func_080cb144();
  s32 best = 9999;
  // Default: -1, though every path below sets it.
  s32 result = -1;
  s32 selected = -1;
  s32 nearby = -1;
  s32 fallback = -1;
  s32 flags;

  flags = event->control;
  while (flags != -1) {
    s32 high = event->trigger & 0xf000;
    s16 hasFacing = (u16)event->trigger & 0x800;
    s32 target = (u16)event->trigger & 0xff;
    s32 eventKind = ((const struct PendingAction *)Ability_Get((u16)flags >> 8))->eventKind;

    if ((event->control & 15) == 5 && eventKind == kind &&
        GameFlag_IsConditionActive(event->condition)) {
      if (hasFacing) {
        s32 distance = (s16)(high - facing);

        if (distance < 0)
          distance = -distance;
        if (distance > 0x17ff)
          goto next;
      }
      if (event->control & 0x80) {
        fallback = 0x200;
      } else if (event->control & 0x10) {
        s32 distance = Func_080cdac0(kind, Party_GetLeaderActor(), target);

        if (distance != -1 && best > distance) {
          nearby = target | 0x100;
          best = distance;
        }
      } else if (kind == 30) {
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

struct TriggerEvent *Func_080ce458(s32 filter, s32 kind, s32 selector) {
  struct TriggerWork *work = Ram_HeapSlots->eventWork;
  struct TriggerEvent *event = work->events;
  u32 facing = ObjectTable_Get(Party_GetLeaderActor())->facing;
  s32 flags;

  if (selector == -1)
    return NULL;
  flags = event->control;
  while (flags != -1) {
    s32 high = event->trigger & 0xf000;
    s16 hasFacing = (u16)event->trigger & 0x800;
    s32 target = (u16)event->trigger & 0xff;
    s32 eventKind = ((const struct PendingAction *)Ability_Get((u16)flags >> 8))->eventKind;

    if ((event->control & 15) == 5 && eventKind == kind &&
        GameFlag_IsConditionActive(event->condition)) {
      if (hasFacing) {
        s32 distance = (s16)(high - facing);

        if (distance < 0)
          distance = -distance;
        if (distance > 0x17ff)
          goto next;
      }
      if (filter != 0xf0000005 && (event->control & 0xf000000f) != filter)
        goto next;
      flags = event->control;
      {
        if (flags & 0x80) {
          if ((selector & 0xff00) == 0x200)
            return event;
        } else if (flags & 0x10) {
          if ((selector & 0xff00) == 0x100 && (selector & 0xff) == target)
            return event;
        } else if ((selector & 0xff00) == 0 && (selector & 0xff) == target) {
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

s32 Event_ExecutePackedAction(u32 packed) {
  s32 action, mode, kind, selector, target;
  struct PendingEvent *first, *second;

  action = packed & 0x3ff;
  mode = (packed >> 10) & 15;
  kind = Ability_Get(action)->unk04[2];
  selector = Func_080ce31c(kind);
  first = (struct PendingEvent *)Func_080ce458(0x30000005, kind, selector);
  second = (struct PendingEvent *)Func_080ce458(0x20000005, kind, selector);
  target = -1;
  if (first != NULL || second != NULL) {
    if (selector != -1 && (selector & 0x100))
      target = selector & 255;
  }
  BattleEffect_LoadActionEffectResources(action, 0);
  BattleEffect_SetupObjectPair(Party_GetLeaderActor(), target);
  Field_RunPsynergyEvent(first, mode, target);
  FieldEvent_RunTypeHandler();
  BattleEffect_StopMainActor();
  Field_RunPsynergyEvent(second, mode, target);
  BattleEffect_CleanupSceneObjects();
  return 0;
}
