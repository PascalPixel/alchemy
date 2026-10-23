/// @file
/// Script commands: waits, received items, questions and destinations.
// Part of field_services.c.

#include "../field_services_internal.h"

// When a gift goes to another member, `<leader> gave <item> to <member>.`
// follows `<leader> got <item>.`; it is MsgGaveItemToMember, the next
// message, in every edition. Japanese and English show it with window flags
// 3 (zoom, busy while it shows), the European releases with 1 (zoom only).
#if (LANGUAGE == LANGUAGE_GERMAN)
#define GIFT_RECEIVED_MESSAGE MsgGaveItemToMember
#define GIFT_RECEIVED_MESSAGE_MODE 1
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||       \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define GIFT_RECEIVED_MESSAGE (MsgColossoIsaacGotItem + 1)
#define GIFT_RECEIVED_MESSAGE_MODE 1
#else
#define GIFT_RECEIVED_MESSAGE (MsgColossoIsaacGotItem + 1)
#define GIFT_RECEIVED_MESSAGE_MODE 3
#endif

/// Shows the message for an item the party has just received. The message is
/// `Isaac got <item>.`; the German release has its own sentence for the Black
/// Orb. flags are the message window flags of
/// FieldMessage_Show.
static inline void ItemGift_ShowGotItem(s32 item, s32 flags) {
#if (LANGUAGE == LANGUAGE_GERMAN)
  if (item == ITEM_BLACK_ORB) {
    FieldMessage_ShowFar(MsgGotBlackOrb, flags);
    return;
  }
#endif
  MessageArgument_PushFar(item, 2);
  FieldMessage_ShowFar(MsgColossoIsaacGotItem, flags);
}

/// Debug only: while debug mode is on, R makes every script wait
/// (FieldScript_WaitFrames) end at once and L restores normal waits. Runs as a
/// task for the length of a script (FieldScript_Start). R also
/// skips the object events' messages, which open nothing while fastForward is
/// set (FieldMessage_ShowAtObject).
void FieldScript_UpdateDebugSkip(void) {
  struct FieldWork *runtime = (struct FieldWork *)gFieldWork;

  if (gDebugMode != 0) {
    if (gKeysPressed & KEY_L) {
      runtime->fastForward = 0;
    }
    if (gKeysPressed & KEY_R) {
      runtime->fastForward = -1;
    }
  }
}

/// The script command "wait": waits the requested frames, unless debug fast
/// forward (FieldScript_UpdateDebugSkip) is on. Map scripts reach it (the WAIT
/// command); this object's object event, message, camera and climbing commands
/// wait through it too.
void FieldScript_WaitFrames(s32 frames) {
  if (((struct FieldWork *)gFieldWork)->fastForward == 0 && frames != 0) {
    WaitFrames(frames);
  }
}

/// Stops the leader where it stands: no horizontal velocity, the normal walking
/// speed (1.0) and acceleration (0.5), no destination, and the standing pose,
/// or the climbing pose (12) while the leader is on a climbable wall. Called by
/// FieldScript_Start, by Map_Run before it handles several requests, by
/// Field_UsePsynergy, Map_TalkToObjectEvent, Field_CheckWarpRegions and the
/// world map's zoom (FieldEffect_RunVisibilityTransition).
void Leader_StopMoving(void) {
  struct Object *object;

  object = ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = OBJ_EVENT_NO_TARGET;
  object->targetZ = OBJ_EVENT_NO_TARGET;
  object->velocityX = 0;
  object->velocityZ = 0;
  if (gGameState.movementMode == LEADER_MOVEMENT_CLIMB) {
    ObjectDispatch_SelectAnimationFar(object, 0xC);
    return;
  }
  ObjectDispatch_SelectAnimationFar(object, 1);
}

/// The script command "begin script": closes any map name popup, stops the
/// leader, resets the message and timing state, and starts the debug
/// fast-forward callback with the leader as the message speaker. Map scripts
/// reach it (SCRIPT_START); the field's item, Psynergy,
/// Djinni, Retreat and climbing scripts open with it too. Clears the field
/// work's name window, fastForward and messageWindow, sets transitionFrames to
/// 16 and the speaker to the leader.
void FieldScript_Start(void) {
  struct FieldWork *runtime = (struct FieldWork *)gFieldWork;

  MapNamePopup_HideFar();
  Leader_StopMoving();
  // Keep the script reset's signed halfword read.
  if ((s16)runtime->busy != 0) {
    Field_EndBusyAndReveal();
  }
  // Every script starts with no name window and normal waits.
  runtime->nameMessage = 0;
  runtime->nameWidth = 0;
  runtime->transitionFrames = 0x10;
  runtime->fastForward = 0;
  runtime->unk1da = 0xFFFF;
  runtime->unk1dc = -1;
  runtime->unk1de = -1;
  Task_Add(FieldScript_UpdateDebugSkip, 0xC80);
  // XXX: flag 0x132 is cleared at every script and nothing in ☀️ names it; the
  // map scripts or overlays that set or test 0x132 would settle its meaning.
  GameFlag_ClearBitFar(0x132);
  runtime->speaker = gGameState.leaderLocalId;
  runtime->messageWindow = 0;
}

/// Ends a script: stops the debug fast-forward callback, gives the camera back
/// to the leader and refreshes FLAG_LURE_CAP (GameFlag_RefreshLureCap). Map
/// scripts reach it (SCRIPT_END), and every script that
/// opens with FieldScript_Start closes with it.
void FieldScript_End(void) {
  Task_Remove(FieldScript_UpdateDebugSkip);
  FieldCamera_FollowObjectEvent(gGameState.leaderLocalId, 1);
  GameFlag_RefreshLureCapFar();
}

/// Unused: an empty function nothing calls.
UNUSED void FieldScript_NoOpHookA(void) {}

/// An empty hook; it does nothing in the released game. Only
/// FieldScript_RunObjectHookAndWait calls it.
void FieldScript_NoOpHook(void) {}

/// Respawns the map's object events from an object event template table and
/// waits one frame for them to appear. Reached only through an unnamed veneer
/// in the overworld bank's far-call table; nothing in ☀️ names it.
void FieldScript_RunObjectHookAndWait(struct ObjectEventTemplate *table) {
  FieldScript_NoOpHook();
  ObjectEvent_ResetForObject(table);
  WaitFrames(1);
  // XXX: the leader's object is fetched and not used; a caller in an overlay
  // that reads the result would show whether this returned it once.
  ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
}

/// Spawns the object events of an object event template table after the last
/// object event in use. Called by the Kolima, Kalay and Tolbi ship map scripts
/// to bring object events in during a scene.
void FieldScript_CallWithLastActiveObjectId(struct ObjectEventTemplate *table) {
  FieldScript_SpawnObjectEventTemplates(table, ObjectEvent_FindLastActiveId());
}

/// Sets the map's event table, which the field checks as the leader moves:
/// writes work->events. Altin Peak's map script calls it.
void FieldScript_SetMapEvents(const struct MapEvent *events) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;

  work->events = events;
}

/// Adds unit to the party. With announce nonzero it also shows
/// `<name> joined your party.` (Party_ShowJoinedMessage, which receives
/// announce too). Called by six map scripts as a character joins (Vale,
/// the Vault, Goma Cave and Mercury Lighthouse among them).
void FieldScript_PrepareObjectAndApplyValue(s32 unit, s32 announce) {
  Unit_GetFar(unit);
  Party_AddMemberFar(unit);
  if (announce != 0) {
    Party_ShowJoinedMessageFar(unit, announce);
  }
}

/// Adds two units to the party and announces them together. Vale's map
/// script calls it twice.
void FieldScript_PrepareTwoObjectsAndApply(s32 first, s32 second) {
  Party_AddMemberFar(first);
  Party_AddMemberFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

/// Checks an L or R button shortcut, packed as the member in bits 10 to 13 and
/// the Psynergy in bits 0 to 9. Returns 0 if it can still be cast, -1 for no
/// such party member, -2 when that member is not in the party and -3 when the
/// member no longer knows the Psynergy. Only
/// FieldScript_ClearInvalidPackedValues calls it.
s32 FieldScript_ValidatePackedId(u32 shortcut) {
  s32 member = (shortcut >> 10) & 0xf;
  s32 ability = shortcut & 0x3ff;

  if (member > 7)
    return -1;
  // Flags 0 to 7 say who is in the party (FLAG_IN_PARTY).
  if (GameFlag_TestFar(member) == 0)
    return -2;
  if (Unit_KnowsPsynergyFar(member, ability) == 0)
    return -3;
  return 0;
}

/// Clears each button shortcut whose Psynergy can no longer be cast: zeroes
/// gGameState.firstShortcut or secondShortcut. Called whenever the party or
/// what it knows may have changed: by Party_RemoveMemberRestored,
/// Inventory_Discard, and the field menu's item, Psynergy and Djinn screens.
void FieldScript_ClearInvalidPackedValues(void) {
  if (FieldScript_ValidatePackedId(gGameState.firstShortcut) != 0)
    gGameState.firstShortcut = 0;
  if (FieldScript_ValidatePackedId(gGameState.secondShortcut) != 0)
    gGameState.secondShortcut = 0;
}

/// Takes unit out of the party, fully restoring its HP and PP and curing its
/// poison. If no member left standing has HP, the leader is given 1 HP so the
/// party is never left all down. Called by the map scripts where a member
/// leaves: Vale in the storm, the Vault, and Sol Sanctum.
void Party_RemoveMemberRestored(s32 unit) {
  struct Unit *vitals;
  s32 partyIndex;
  s32 partyCount;
  s32 survivingMembers;

  Party_RemoveMemberFar(unit);
  FieldScript_ClearInvalidPackedValues();
  vitals = Unit_GetFar(unit);
  vitals->hp = vitals->hpMax;
  vitals->pp = vitals->ppMax;
  Unit_UpdateGauges(vitals);
  vitals->poison = 0;
  // Count the members who remain and still have HP.
  survivingMembers = 0;
  partyCount = Party_CountMembersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    vitals = Unit_GetFar(gGameState.party[partyIndex]);
    if (vitals->hp != 0)
      survivingMembers++;
  }
  if (survivingMembers == 0) {
    vitals = Unit_GetFar(gGameState.leaderLocalId);
    vitals->hp = 1;
    Unit_UpdateGauges(vitals);
  }
}

/// Checks whether the party can carry one more of item: each member carries at
/// most 30 of one item. When they are all full, says
/// `Isaac's party cannot carry any more <item>s...` and
/// `Isaac threw the <item> as far away as possible.` and returns -1; otherwise
/// returns 0. Map_Run calls it before it gives gGameState.pendingItem
/// with PartyInventory_GiveItem.
s32 Party_CheckItemLimit(s32 item) {
  s32 partyCount;
  s32 heldQuantity = 0;
  s32 partyIndex;

  partyCount = Party_CountMembersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++)
    heldQuantity += Inventory_CountItemFar(gGameState.party[partyIndex], item);

  if (heldQuantity >= partyCount * 30) {
    MessageArgument_PushFar(item, 2);
    FieldMessage_ShowFar(MsgCannotCarryMore, 1);
    MessageArgument_PushFar(item, 2);
    FieldMessage_ShowFar(MsgCannotCarryMore + 1, 1);
    return -1;
  }
  return 0;
}

/// The script command "give item": gives item to the first party member with
/// room for it and says so. When every bag is full, the player picks an item
/// to drop to make room, or drops the new item instead (not allowed for an
/// indispensable one). Returns the member who received the item, or -1 when
/// it was left behind. Map scripts reach it (about
/// 24 of them); Map_Run gives the pending item and the debug test menu
/// gives items through it. The field work's message is restored afterwards.
s32 PartyInventory_GiveItem(s32 item, s32 unused) {
  // A for/continue retry loop shrinks native English 0x1c4 -> 0x1a8;
  // retain the original backedges until a plain form matches.
  struct FieldWork *work;
  s32 savedMessage;
  s32 owner;
  s32 result;
  s32 discardQuantity;
  s32 discardIndex;
  s32 member;
  s32 slot;

  work = (struct FieldWork *)gFieldWork;
  // The messages below move the script's message on; every return puts it
  // back, so the map script continues where it was.
  savedMessage = work->message;
  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    // Every bag is full: "But you can't carry any more items."
    ItemGift_ShowGotItem(item, 1);
    FieldMessage_ShowFar(MsgCannotCarryAnyMore, 1);
  retry:
    FieldMessage_ShowFar(MsgWhatWillYouDrop, 1);
    result = ItemPicker_RunFar(&member, &slot);
    if (result == -1) {
      // Cancelled: drop the new item instead, unless it is indispensable
      // ("That <item>'s valuable! You must get it!").
      if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE) {
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar(MsgWhatWillYouDrop + 4, 1);
        goto retry;
      }
      // "Drop the <item>?"
      MessageArgument_PushFar(item, 2);
      FieldMessage_ShowFar(MsgWhatWillYouDrop + 1, 5);
      result = FieldYesNo_Ask(1);
      UiWork_FinalizePendingCoreFar();
      if (result != 0)
        goto retry;
      // "Isaac drops the <item>."
      Item_AdjustCounterFar(item, 1);
      MessageArgument_PushFar(item, 2);
      FieldMessage_ShowFar(MsgWhatWillYouDrop + 2, 1);
      work->message = savedMessage;
      return owner;
    } else {
      // A carried item was picked: drop it (the whole stack) and take the new
      // one.
      Unit_GetFar(member);
      {
        /* FAKEMATCH(temporary): the slot copied to a block local loads before
         * the member; as Shop_GetSelectionState(member, slot), 2 of 181
         * instructions differ in tbs-en. */
        s32 selectedSlot = slot;

        discardQuantity = Shop_GetSelectionState(member, selectedSlot);
      }
      // A member who already carries 30 of the new item (the cap
      // Party_CheckItemLimit counts) cannot take it: say so and ask again.
      // Editions at TBS_PATCH_1 alone skip the check.
#if !(TBS_PATCH_1 && !TBS_PATCH_2)
      if (Inventory_CountItemFar(member, item) > 29) {
        MessageArgument_PushFar(member, 1);
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar(MsgWhatWillYouDrop + 7, 1);
        goto retry;
      }
#endif
      for (discardIndex = 0; discardIndex < discardQuantity; discardIndex++)
        Inventory_DiscardFar(member, slot);
      owner = PartyInventory_AddFar(item);
      Sound_PlaySongFar(MUS_TREASURE_FOUND);
      if (owner == gGameState.leaderLocalId) {
        ItemGift_ShowGotItem(item, 3);
      } else {
        MessageArgument_PushFar(item, 2);
        MessageArgument_PushFar(owner, 1);
        FieldMessage_ShowFar(MsgGaveItemToMember, 3);
      }
      work->message = savedMessage;
      return owner;
    }
  } else {
    // There was room: "Isaac got <item>.", and "Isaac gave <item> to
    // <member>." when someone other than the leader carries it.
    Sound_PlaySongFar(MUS_TREASURE_FOUND);
    ItemGift_ShowGotItem(item, 3);
    if (owner != gGameState.leaderLocalId) {
      MessageArgument_PushFar(item, 2);
      MessageArgument_PushFar(owner, 1);
      FieldMessage_ShowFar(GIFT_RECEIVED_MESSAGE, GIFT_RECEIVED_MESSAGE_MODE);
    }
    work->message = savedMessage;
  }
  return owner;
}

/// Gives item to one party member. Returns unit, or -1 if its bag is full.
/// Reached only through an unnamed veneer in the overworld bank's far-call
/// table; nothing in ☀️ names it.
s32 Inventory_TryAddAndReturnMember(s32 item, s32 unused, s32 unit) {
  if (Inventory_AddItemFar(unit, item) >= 0)
    return unit;
  return -1;
}

/// An empty function; nothing in ☀️ calls it.
void Inventory_NoOpCallback(void) {}

/// An empty function; nothing in ☀️ calls it.
void Inventory_ReservedNoOp(void) {}

/// Waits, at most 90 frames, for an object event's animation to stop being
/// animation: the object event has finished a one-off animation such as a nod.
/// Only object events drawn as a sprite (OBJECT_SPRITE_ONE) are
/// waited for. Called by FieldScript_AskYesNoWithObjectEventAnimation.
void ObjectEvent_WaitForAnimationEnd(s32 localId, s32 animation) {
  struct Object *objectEventObject = ObjectEvent_GetByLocalId(localId);

  if (objectEventObject != 0 &&
      objectEventObject->animationKind == OBJECT_SPRITE_ONE) {
    s32 frames = 0;
    u8 *animationNumber =
        &((struct Sprite *)objectEventObject->animation)->lastNo;

    while (frames <= 89) {
      WaitFrames(1);
      if (animation != *animationNumber) {
        break;
      }
      frames++;
    }
  }
}

/// The script command "choose yes or no": once the message on screen has been
/// read, asks yes or no below it, or above it when the message windows reach
/// past row 15 or forceBottom is set. The speaker nods (animation 3) at yes
/// and shakes its head (animation 4) at no. Returns 0 for yes, 1 for no.
/// Map scripts reach it (about
/// 54 of them); the Battle Arena and FieldScript_AskYesNoAndShowResponse call
/// it too.
s32 FieldScript_AskYesNoWithObjectEventAnimation(s32 localId, s32 forceBottom) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;
  s32 sprite = Map_FindObjectEventTemplate(work->speaker)->sprite;
  struct Window *window = (struct Window *)work->messageWindow;
  struct Window *side = (struct Window *)work->sideWindow;
  s32 useBottom = 1;
  s32 answer;

  // Wait for every button to be released and the message to finish.
  while (gKeysPressed != 0)
    WaitFrames(1);

  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);

  WaitFrames(3);

  // The question goes above the message windows when they reach past row 15,
  // so it does not cover them.
  if (forceBottom == 0) {
    s32 bottom = window->y + window->height;

    if (side != 0) {
      s32 sideBottom = side->y + side->height;
      if (bottom < sideBottom)
        bottom = sideBottom;
    }

    if (bottom > 15)
      useBottom = 0;
  }

  answer = YesNoMenu_RunFar(useBottom, work->nameMessage, work->nameWidth, 0);
  if (answer != 0) {
    ObjectEvent_SetModeById(localId, 4);
    Portrait_CloseWindowFar(sprite);
    UiWork_FinalizePendingCoreFar();
    ObjectEvent_WaitForAnimationEnd(localId, 4);
  } else {
    ObjectEvent_SetModeById(localId, 3);
    Portrait_CloseWindowFar(sprite);
    UiWork_FinalizePendingCoreFar();
    ObjectEvent_WaitForAnimationEnd(localId, 3);
  }

  return answer;
}

/// Asks yes or no at the top of the screen, or at the bottom with atBottom
/// set. Returns 0 for yes, 1 for no. Called by PartyInventory_GiveItem,
/// Field_UseItem, Field_UsePsynergy and the cleared game's menu.
s32 FieldYesNo_Ask(s32 atBottom) { return YesNoMenu_RunFar(atBottom, 0, 0, 0); }

/// Teaches unit the Psynergy ability and says so with its fanfare, waiting for
/// the player to read it. The message is `<unit> learned <ability>.` Fuchin
/// Temple's map script calls it.
void Party_LearnAbility(s32 unit, s32 ability) {
  // Declared without parameters: unit and ability pass through in r0 and r1.
  Unit_LearnPsynergyFar();
  Sound_PlaySongFar(MUS_TREASURE_FOUND);
  MessageArgument_PushFar(unit, 1);
  MessageArgument_PushFar(ability, 4);
  FieldMessage_ShowFar(MsgAbilityLearned, 3);
}

/// The script command "open screen": starts the script's screen transition and
/// sets work->transitionActive. Map scripts reach it; the inn, the title, the
/// main menu, the Battle Arena and RegionMap_Show call it too.
void FieldScript_StartScreenTransition(void) {
  struct FieldWork *runtime = (struct FieldWork *)gFieldWork;

  ScreenTransition_Start(runtime->transition, runtime->transitionFrames);
  runtime->transitionActive = 1;
}

/// The script command "close screen": finishes the script's screen transition
/// and clears work->transitionActive. Map scripts reach it; the inn, the main
/// menu and RegionMap_Show call it too.
void FieldScript_FinishScreenTransition(void) {
  struct FieldWork *runtime = (struct FieldWork *)gFieldWork;

  ScreenTransition_Finish(runtime->transition, runtime->transitionFrames);
  runtime->transitionActive = 0;
}

/// The script command "wait for screen": waits as long as the transition runs
/// (work->transitionFrames). Map scripts reach it.
void FieldScript_WaitTransitionFrames(void) {
  WaitFrames(((struct FieldWork *)gFieldWork)->transitionFrames);
}

/// Makes the script leave for the given warp destination of the given map:
/// raises the warp request 999 and writes gGameState.mapId and warpDest. Called
/// by the title, the main menu, the Battle Arena, the debug battle command
/// and five map scripts (the Lemurian ship and the world map among them).
void FieldScript_SetDestination(s32 mapId, s32 warpDest) {
  ((struct FieldWork *)gFieldWork)->warp = 999;
  gGameState.mapId = mapId;
  gGameState.warpDest = warpDest;
}

/// Makes the script leave and records map and warp destination as the saved
/// destination. Reached only through an unnamed veneer in the overworld bank's
/// far-call table; nothing in ☀️ names it.
void FieldScript_SetSavedDestination(u16 mapId, u16 warpDest) {
  struct FieldWork *state = (struct FieldWork *)gFieldWork;
  state->warp = 999;
  gGameState.savedMap = mapId;
  gGameState.savedWarpDest = warpDest;
}

/// The script command "request warp": sets the field's warp request (999 leaves
/// for gGameState's mapId and warpDest). Map scripts reach it as
/// Engine_ScriptRequestWarp (about 61 of them).
void FieldScript_SetWarpRequest(u16 request) {
  struct FieldWork *state = (struct FieldWork *)gFieldWork;
  state->warp = request;
}

#undef GIFT_RECEIVED_MESSAGE
#undef GIFT_RECEIVED_MESSAGE_MODE
