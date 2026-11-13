/// @file
/// Finding and running a scene's triggers: talking, step triggers, examine
/// triggers.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"

struct ActionDescriptor {
  u16 sprite;
  s16 condition;
  s32 behavior;
  s32 x;
  s32 y;
  s32 z;
  u16 facing;
  u8 talkFacing;
  u8 flags;
};

struct ActionDescriptorTables {
  struct ActionDescriptor *tables[4];
};

/// The Broken Seal's Scene_FindActorPlacement: finds actor id's placement in
/// the event work's four tables (ids up to 7 by sprite, later ones counted
/// in order among the rows past 7), or NULL.
struct ActionDescriptor *Scene_FindActorPlacement(s32 id) {
  struct ActionDescriptor *entry;
  s32 i;
  s32 group = 8;

  for (i = 0; i < 4; i++) {
    entry =
        ((struct ActionDescriptorTables *)Ram_HeapSlots->eventWork)->tables[i];
    if (entry == NULL)
      continue;
    if (id <= 7) {
      for (; (s16)entry->sprite != -1; entry++) {
        if ((s16)entry->sprite == id)
          goto found;
      }
    } else {
      for (; (s16)entry->sprite != -1; entry++) {
        if ((s16)entry->sprite > 7) {
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

s32 GameFlag_IsConditionActive(s32 condition) {
  s32 flag_state;

  if (condition == -1) {
    return 1;
  }
  if (0x1000 & condition) {
    return GameFlag_Test(condition);
  }
  flag_state = GameFlag_Test(condition);
  return 1 - ((u32)((0 - flag_state) | flag_state) >> 31);
}

// The Lost Age's form of the field's trigger search (the Broken Seal's
// Scene_FindTrigger).

/// Returns the first of the scene's triggers of kind for target whose
/// condition holds and whose push (STEP_TRIGGER_FLAG_WALKING) and facing
/// (TRIGGER_FLAG_FACING) requirements are met, or NULL. Kinds 8 and 9 answer
/// any target, and the facing test alone is skipped while
/// gMainRoutineWork.renderMode is 2.
// XXX: what kinds 8 and 9 are; the triggers of the scenes that use them would
// settle it.
struct TriggerView *Func_080ccd78(s32 kind, s32 target) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TriggerView *trigger = work->triggers;
  s32 flags;
  u32 facing = ObjectTable_Get(gMainRoutineWork.currentOwner)->facing;

  flags = trigger->flags;
  while (flags != -1) {
    // Holds one address for its whole life.
    u8 *mode = &gMainRoutineWork.renderMode;

    if ((flags & TRIGGER_KIND_MASK) == kind &&
        (trigger->value == target || kind == 8 || kind == 9) &&
        GameFlag_IsConditionActive(trigger->condition)) {
      s32 matches = 0;
      s32 minimumPushFrames = 12;
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
        if (*mode != 2) {
          flags &= TRIGGER_FACING_MASK;
          matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        }
        break;
      case STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING:
        if (work->blockedSteps > minimumPushFrames) {
          flags &= TRIGGER_FACING_MASK;
          matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
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

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

extern struct DescriptorEntry gData_080eff28[];

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void Func_0814ea58(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Shop_InitializeWork(void);
s32 Inn_RoomPrice(s32 innId);
void Shop_ReleaseWork(void);

/// Unused: runs the loaded overlay's initialize entry; nothing calls it.
UNUSED void Overlay_CallInitialize(void) { ((s32 (*)(void))gOverlayArea[1])(); }

/// Returns the talk trigger of object id, or 0: for the object field Psynergy
/// has stopped (gMainRoutineWork.pausedObject), its kind 7 trigger if it has
/// one, the event for talking to it while it is stopped.
void *Scene_FindActorTalkTrigger(s32 id) {
  void *found = Func_080ccd78(0, id);
  struct MainRoutineWork *party = &gMainRoutineWork;
  void *event;
  s32 stopped;

  stopped = party->pausedObject;
  event = found;
  if (stopped == id) {
    void *alt = Func_080ccd78(7, stopped);

    if (alt != NULL)
      return alt;
  }
  return event;
}

struct ContainerRow {
  s16 type;
  s16 noHint;
  s16 flag;
  s16 contents;
};

struct TalkEvent {
  u8 unk00[8];
  s32 script;
};

void PartyStatus_CreateWindowFar(s32 layout);
void PartyStatus_CloseFar(void);
void Func_080ea14c(s32 targetId);
void Func_08038340(s32 value);
void Func_080ad2f8(s32 value);
s32 PartyInventory_AddFar(s32 item);
s32 BattleEffect_GetFlags(s32 id);
void Actor_TurnToFaceEachOther(s32 first, s32 second, s32 mode);
void Window_SetBusyFlagsFar(s32 flags);
void BattleEffect_ResumeObject(s32 id);

s32 Func_080ccec8(s32 id) {
  struct ActorPlacement *placement;
  s32 result;
  struct Actor *object;
  s32 savedAngle;
  s32 paused;
  s32 busyFlags;
  s32 silent;
  u8 *event;
  struct Actor *leader;
  struct Actor *shown;
  struct ContainerRow *row;
  struct TalkEvent *talk;
  s32 noHint;
  u8 *busy;
  s32 found;
  s32 type;
  s32 flag;
  s32 contents;
  s32 index;
  s32 slot;
  u8 *work;

  placement = Scene_FindActorPlacement(id);
  result = -1;
  savedAngle = 0;
  paused = 0;
  busyFlags = 0;
  silent = id & 0x1000;
  event = Ram_HeapSlots->eventWork;
  id &= 0xfff;
  found = 0;
  object = Actor_GetById(id);
  if (id > 63) {
    if (id != 79) {
      row = Func_080ceba8();
      if (row != NULL) {
        u8 *end = (u8 *)row + id * 8;
        s16 *p;
        p = (s16 *)(end - 0x200);
        type = *p;
        index = id;
        found = 1;
        p = (s16 *)(end - 0x1fe);
        noHint = *p;
        p = (s16 *)(end - 0x1fc);
        flag = *p;
        p = (s16 *)(end - 0x1fa);
        contents = *p;
      }
    } else {
      type = 131;
      noHint = result;
      flag = result;
      contents = *(s16 *)(event + 0xd34);
      index = 79;
      found = 1;
    }
    if (found) {
      found = type;
      if ((u16)(type - 128) <= 1)
        found = 1;
      if ((u16)(type - 130) <= 2)
        found = 5;
      if (type == 132)
        found = 26;
      if (type == 133)
        found = 32;
      Event_Start();
      busy = &((u8 *)object)[91];
      *busy = 1;
      if (silent == 0)
        FieldMessage_ShowFar(
            found + (s32)&MsgKey0dc4, 1);
      if (gMainRoutineWork.scene == (s32)&SceneId_VenusLighthouse && noHint == 0 &&
          !GameFlag_Test(flag)) {
        MessageArgument_PushFar(contents, 2);
        FieldMessage_ShowFar((s32)&MsgKey15b5, 1);
      }
      if (!GameFlag_IsConditionActive(flag))
        goto empty;
      if (type == 128) {
        if (contents == 0)
          ObjectEvent_SetMode6WithCue(index);
        else
          ObjectEvent_SetMode4WithCue(index);
      }
      if (type == 129)
        ObjectEvent_SetMode4WithCue(index);
      if (type == 130) {
        struct Actor *trap = ObjectEvent_GetObject(index);

        trap->update = Marker_MoveLeaderAwayFromEmitter;
        ItemDisplay_RaiseAboveLeader(trap);
        Sound_PlayFar(83);
        MessageArgument_PushFar(contents, 5);
        FieldMessage_ShowFar((s32)&MsgKey0e17, 3);
        BattleParty_ApplyDrain(1999, 0);
        PartyStatus_CreateWindowFar(1);
        Sound_PlayFar(126);
        if (Party_CountActiveOwnersFar() == 1)
          FieldMessage_ShowFar((s32)&MsgKey0e17 + 6, 1);
        else
          FieldMessage_ShowFar((s32)&MsgKey0e17 + 1, 1);
        PartyStatus_CloseFar();
        trap->update = NULL;
        Actor_SetMode(trap, 2);
        Sound_PlayFar(246);
        Event_Wait(30);
        FieldMessage_ShowFar((s32)&MsgKey0e19, 1);
        ObjectEvent_Remove(index);
        if (flag != -1)
          GameFlag_SetBit(flag);
        goto done;
      }
      if (type == 132) {
        Event_Start();
        Event_ResolvePendingActions(0);
        Func_080ea14c(index);
        Func_08038340(contents);
        Func_080ad2f8(contents);
        ObjectEvent_Remove(index);
        if (flag != -1)
          GameFlag_SetBit(flag);
        Event_End();
        goto done;
      }
      if (type == 129) {
        work = Ram_HeapSlots->eventWork;
        ObjectEvent_Spring(index);
        if (flag != -1)
          gMainRoutineWork.pendingDjinnEvent = flag | 0x1000;
        if (contents > 7)
          *(u16 *)(work + 356) =
              Encounter_GetEnemyGroupAtSlot(105, contents - 8);
        else
          *(u16 *)(work + 356) = Encounter_GetEnemyGroupAtSlot(104, contents);
        gMainRoutineWork.battleRule24b = 2;
        BattleEffect_SelectBattleCue(104, contents);
        Sound_PlayFar(gMainRoutineWork.sceneCue);
        FieldMessage_ShowFar((s32)&MsgKey0e1a, 1);
        goto done;
      }
      if (type == 133)
        FieldMessage_ShowFar((s32)&MsgKey0de5, 1);
      if (Actor_GetSpriteShape(index)) {
        FieldMessage_ShowFar((s32)&MsgKey0e1c, 1);
        if (Party_FindOwnerWithActionFar(136) != -1)
          ((struct EventWork *)Ram_HeapSlots->eventWork)->unk15a = 1;
        goto done;
      }
      if (contents & 0x8000) {
        if (type == 131) {
          shown = ObjectEvent_GetObject(index);
          shown->update = Marker_MoveLeaderAwayFromEmitter;
        } else {
          shown = BattleEffect_StartRandomParticleEmitter(index, 0);
          wait(30);
          if (type == 128)
            ObjectEvent_SetMode2(index);
        }
        ItemDisplay_RaiseAboveLeader(shown);
        Sound_PlayFar(83);
        contents &= 0x7fff;
        MessageArgument_PushFar(contents, 5);
        FieldMessage_ShowFar((s32)&MsgKey0e10, 3);
        Party_AdjustCoinsFar(contents);
        if (flag != -1)
          GameFlag_SetBit(flag);
        if (type == 131) {
          Actor_SetPositionAndResetMotionFar(shown, 0, 0, 0);
          shown->update = NULL;
        } else {
          Actor_Destroy(shown);
        }
        goto done;
      }
      if (contents == 0) {
        if (flag != -1)
          GameFlag_SetBit(flag);
        goto empty;
      }
      if (type == 131) {
        shown = ObjectEvent_GetObject(index);
        shown->update = Marker_MoveLeaderAwayFromEmitter;
      } else {
        shown = BattleEffect_StartRandomParticleEmitter(index, contents);
        wait(30);
      }
      slot = PartyInventory_AddFar(contents);
      if (slot == -1) {
        MessageArgument_PushFar(contents, 2);
        FieldMessage_ShowFar((s32)&MsgFoundItem, 1);
        FieldMessage_ShowFar((s32)&MsgFoundItem + 4, 1);
        if (type != 131)
          Actor_ReleaseIfPresent(shown);
        if (type == 128)
          ObjectEvent_SetMode5WithCue(index);
        goto done;
      }
      if (type == 128)
        ObjectEvent_SetMode2(index);
      ItemDisplay_RaiseAboveLeader(shown);
      Sound_PlayFar(83);
      MessageArgument_PushFar(contents, 2);
      if (slot == gMainRoutineWork.currentOwner) {
        FieldMessage_ShowFar((s32)&MsgItemReceived, 3);
      } else {
        MessageArgument_PushFar(slot, 1);
        FieldMessage_ShowFar((s32)&MsgItemGiven, 3);
      }
      if (flag != -1)
        GameFlag_SetBit(flag);
      if (type == 131) {
        Actor_SetPositionAndResetMotionFar(shown, 0, 0, 0);
        shown->update = NULL;
      } else {
        Actor_Destroy(shown);
      }
      if (type == 133)
        ObjectEvent_Remove(index);
      result = 0;
      goto done;
    empty:
      FieldMessage_ShowFar(found + (s32)&MsgKey0def,
                                             1);
    done:
      *busy = 0;
      Event_End();
      if (result != 0)
        return result;
    }
  }

  if (gMainRoutineWork.pausedObject == id) {
    paused = 1;
    talk = Func_080ccd78(7, id);
    if (talk == NULL) {
      talk = Func_080ccd78(0, id);
      busyFlags = 1;
      if (talk == NULL)
        return result;
      if (talk->script >= 0x10000) {
        Event_RunMessage(id, BattleEffect_GetFlags(id) * 2 +
                                        ((Random16() << 1) >> 16) + (s32)&MsgKey137f);
        goto resume;
      }
      goto check;
    }
  } else {
    talk = Func_080ccd78(0, id);
  check:
    if (talk == NULL)
      goto resume;
  }
  if (talk->script == 0)
    goto resume;
  if (!paused) {
    ((u8 *)object)[91] = 1;
    Actor_SetAnimationSpeedFar(object, 0);
    savedAngle = object->facing;
    if ((u8)placement->talkFacing <= 1 || (s8)placement->talkFacing == 3) {
      leader = Actor_GetById(gMainRoutineWork.currentOwner);
      Actor_SetPositionAndResetMotionFar(leader, leader->x, leader->y,
                                          leader->z);
      Actor_TurnToFaceEachOther(id, gMainRoutineWork.currentOwner, 0);
    }
  }
  if (talk->script < 0x10000) {
    Window_SetBusyFlagsFar(busyFlags);
    Event_RunMessage(id, talk->script);
  } else {
    Leader_StopMoving();
    ((void (*)(s32))talk->script)(id);
  }
  if (!paused) {
    if (object->script[object->step] == 17) {
      if ((s8)placement->talkFacing == 3) {
        object->watchedObject = Actor_GetById(gMainRoutineWork.currentOwner);
        object->actionFlags |= 1;
        Actor_SetActionCallback(object, (s32)gActor_FaceWatchedScript);
      } else if ((s8)placement->talkFacing == 1) {
        object->timer = savedAngle;
        Actor_StartScriptFar(object, gActor_StepAngleScript);
      }
    }
    ((u8 *)object)[91] = 0;
    Actor_SetAnimationSpeedFar(object, 16);
  }
  result = 0;
resume:
  if (paused) {
    BattleEffect_ResumeObject(gMainRoutineWork.pausedObject);
    gMainRoutineWork.pausedObject = -1;
  }
  return result;
}

/// A scene event as the event finder (Func_080ccd78) returns it.
struct TouchEvent {
  s32 flags;
  u8 target;
  u8 unk05;
  s16 condition;
  /// A message number below 0x10000, else a function; read as a union
  /// member, so a store to the field work does not let it stay in a
  /// register.
  union {
    s32 result;
  } u;
};

struct TouchWork {
  u8 unk000[0x158];
  s16 exit;
  u8 unk15a[0x184 - 0x15a];
  s16 blockedSteps;
};

void ObjectEffect_StartContextEffect25(void);
void ObjectEffect_StartContextEffect26(void);
void Func_080d5fd4(void);

s32 Scene_RunRaisedEvent(s32 id) {
  u8 *descriptor = (u8 *)Func_080ccd78(6, id);
  s32 result = -1;

  if (descriptor != NULL) {
    s32 handler = *(s32 *)(descriptor + 8);

    if (handler != 0) {
      if (handler < 0x10000)
        Event_RunMessage(-1, handler);
      else
        ((void (*)(s32))handler)(id);
      result = 0;
    }
  }
  return result;
}

/// Runs the touch event (kind 2) of what the leader touched, or else takes its
/// exit (kind 1): plays the exit's sound (and effect) and records where it
/// leads. Returns 0 when something ran, -1 when there was nothing.
// As ☀️'s Scene_RunTouchOrExit.
s32 Scene_RunTouchOrExit(s32 value) {
  struct TouchEvent *event;
  s32 handled;
  struct TouchWork *work;

  event = Func_080ccd78(2, value);
  handled = -1;
  work = Ram_HeapSlots->eventWork;
  if (event != NULL && event->u.result != 0) {
    if (event->flags & 0x200)
      work->blockedSteps = 0;
    if (event->u.result < 0x10000) {
      Event_RunMessage(handled, event->u.result);
      handled = 0;
    } else {
      ((void (*)(s32))event->u.result)(value);
      handled = 0;
    }
  } else {
    event = Func_080ccd78(1, value);
    if (event != NULL) {
      switch (event->flags & 0x70) {
      case 0:
        Sound_PlayFar(123);
        break;
      case 32:
        Sound_PlayFar(128);
        ObjectEffect_StartContextEffect26();
        break;
      case 48:
        Sound_PlayFar(129);
        ObjectEffect_StartContextEffect25();
        break;
      case 80:
        Sound_PlayFar(123);
        Func_080d5fd4();
        break;
      }
      work->exit = event->u.result;
      handled = 0;
    }
  }
  return handled;
}

struct SceneEventEntry {
  s32 flags;
  u8 value;
  u8 unk05;
  s16 condition;
  s32 result;
};

void Actor_JumpDownLedge(s32 owner, s32 a, s32 b);
s32 Func_080d4d08(void);
s32 Func_080d50f8(void);
s32 Func_080d53b8(void);

/// The field's push and button handler for keys, idle while
/// gMainRoutineWork.pendingThird is set. After the leader has pushed for more than
/// 12 frames: key 0x20 on a 0x80 push makes the leader jump
/// (Actor_JumpDownLedge(owner, 6, 0)); key 0x80 runs
/// Func_080d4d08; key 0x40 runs Func_080d50f8 or Func_080d53b8 by the push kind
/// and clears the push state when it returns 0; key 2 with Func_080cdc74 runs
/// the nearby talk target's kind 8 event and then the kind 9 event for keys
/// (messages under 0x10000, otherwise a routine called with keys and the
/// target). Always returns 0.
s32 Func_080cd680(s32 keys) {
  struct EventWorkView *work = Ram_HeapSlots->eventWork;
  s32 owner = gMainRoutineWork.currentOwner;
  struct SceneEventEntry *event;
  s32 target;
  s32 result;

  if (gMainRoutineWork.pendingThird != 0)
    return 0;
  if ((keys & 0x20) && work->blockedSteps > 12 && (work->pushKind & 0x80)) {
    Actor_JumpDownLedge(owner, 6, 0);
    work->blockedSteps = 0;
  }
  if ((keys & 0x80) && work->blockedSteps > 12 && !Func_080d4d08())
    work->blockedSteps = 0;
  if ((keys & 0x40) && work->blockedSteps > 12) {
    s32 kind = work->pushKind & 0xf0;
    s32 done = -1;

    if (kind == 0x80)
      done = Func_080d50f8();
    else if (kind == 0x40)
      done = Func_080d53b8();
    if (done == 0) {
      work->blockedSteps = 0;
      work->unk15a = 0;
      work->unk15e = 0;
      work->unk15c = 0;
      work->unk164 = 0;
      work->unk166 = 0;
    }
  }
  if ((keys & 2) && work->blockedSteps > 12 && Func_080cdc74()) {
    target = Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner);
    if (target != -1) {
      event = Func_080ccd78(8, target);
      if (event != NULL && (result = event->result) != 0) {
        if (result < 0x10000)
          Event_RunMessage(-1, result);
        else
          ((void (*)(s32, s32))result)(keys, target);
      }
      Func_080cdd80();
      event = Func_080ccd78(9, keys);
      if (event != NULL && (result = event->result) != 0) {
        if (result < 0x10000)
          Event_RunMessage(-1, result);
        else
          ((void (*)(s32, s32))result)(keys, target);
      }
      work->blockedSteps = 0;
    }
  }
  return 0;
}

// The search command. Collision codes 242 to 247 show their pair of messages
// from Data_080eff22; otherwise the search event (kind 3) of the cell runs:
// its "checked the ..." message, then its message or callback, or the
// matching "found nothing" message. MsgKey0dc4 is the base of the "X
// checked the Y..." lines, MsgKey0def the base of the "found nothing"
// lines.

extern const u8 Data_080eff22[];

/// The search command. Collision codes 242 to 247 show their pair of messages
/// from Data_080eff22; otherwise the search event (kind 3) of the cell runs:
/// its "checked the ..." message, then its message or callback, or the matching
/// "found nothing" message. MsgKey0dc4 is the base of the "X checked the Y..."
/// lines, MsgKey0def the base of the "found nothing" lines.
s32 Func_080cd808(s32 id) {
  struct Trigger *event;
  s32 index;
  s32 kind;
  s32 condition;
  u32 result;

  index = id - 242;
  // The gotos lay the callback out before the message, both after the two
  // tests, as the original does.
  if ((u32)index <= 5) {
    Leader_StopMoving();
    // id, no longer needed, holds the pair's message number.
    id = Data_080eff22[index];
    FieldMessage_ShowFar(id + (s32)&MsgKey0dc4, 1);
    FieldMessage_ShowFar(id + (s32)&MsgKey0def, 1);
    return 0;
  }
  event = Func_080ccd78(3, id);
  if (event != NULL) {
    s32 flags = event->control;

    condition = event->condition;
    kind = (flags >> 4) & 31;
    if (!(event->subject & 0x400) && kind != 0) {
      Leader_StopMoving();
      FieldMessage_ShowFar(kind + (s32)&MsgKey0dc4, 1);
      GameFlag_SetBit(0x142);
    } else {
      GameFlag_ClearBit(0x142);
    }
    result = event->value;
    if ((result & 0xf000000) == 0) {
      if ((result & 0xfff00000) == 0x400000)
        goto message;
      goto done;
    }
    if (GameFlag_IsConditionActive(condition))
      ((void (*)(s32))event->value)(gMainRoutineWork.currentOwner);
    if (GameFlag_Test(0x142))
      FieldMessage_ShowFar(kind + (s32)&MsgKey0def, 1);
    goto done;
  message:
    if (GameFlag_IsConditionActive(condition))
      FieldMessage_ShowFar((u16)event->value, 1);
    else
      FieldMessage_ShowFar((s32)&MsgButFoundNothing, 1);
  } else {
    FieldMessage_ShowFar((s32)&MsgKey0dc4 + 5, 1);
    FieldMessage_ShowFar((s32)&MsgKey0def + 5, 1);
  }
done:
  GameFlag_ClearBit(0x142);
  return 0;
}
