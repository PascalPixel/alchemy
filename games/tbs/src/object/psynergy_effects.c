#include "vram_tab.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "scene.h"
#include "battle_type.h"
#include "game_state.h"
#include "field_runtime.h"
#include "battle_system.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "event_work.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "field_obj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "owner_value.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "item_gift.h"
#include "battle_calc.h"
#include "display_transition.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "battle_party.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "field_coordinate.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_effect_scripts.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/scroll_initialize_hblank_dma.h"

// Shows "Isaac got <item>." for an item the party has just received. The
// German release has its own sentence for the Black Orb. flags are the
// message window flags of UiText_ShowPositionedMessageAndWait.
static inline void ItemGift_ShowGotItem(s32 item, s32 flags) {
#if defined(TBS_EDITION_DE)
  if (item == ITEM_BLACK_ORB) {
    UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, flags);
    return;
  }
#endif
  UiWork_PushValueSlotFar(item, 2);
  UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem, flags);
}

// This file's view of the event work (gEventWork). It also reaches the fields
// from 0xcb6 on, which the shared EventWork does not describe.
struct FieldWork {
  u8 unk000[0x10];
  s32 view;
  struct FieldActor *actors[86];
  s16 action;
  s16 input;
  s16 exit;
  s16 menu;
  s16 examine;
  s16 start;
  u16 descriptor;
  s16 tile;
  s16 scene;
  s16 command;
  s16 item;
  s16 event;
  s16 count;
  s16 survived;
  s16 quantities[9];
  s16 itemArgument;
  u8 unk19c[2];
  s16 mode;
  u8 unk1a0[8];
  s32 unk1a8;
  u8 unk1ac[4];
  s32 unk1b0;
  s32 unk1b4;
  u8 unk1b8[4];
  u8 *cells;
  s32 transition;
  u16 unk1c4;
  u16 transitionActive;
  s32 transitionFrames;
  u8 unk1cc[0xaea];
  u16 busy;
  s16 effectActive;  // nonzero while a Psynergy effect runs
  s16 effectFrames;  // frames the effect has left
  s16 effectCenterX; // centre of the effect, in pixels
  s16 effectCenterY;
  s16 effectTimerRunning; // the timed effect (kind 8) counts down while set
  s16 choiceNameMessage;  // the yes/no menu's name window (EventPromptWork)
  s16 choiceNameWidth;
  s8 resolvingAction; // set while a Psynergy effect is being shown
  s8 castStarted;     // set once a cast has been announced
  s16 queuedSound;
};

// Debug only: while debug mode is on, R makes every event wait
// (Battle_WaitMode0) end at once and L restores normal waits. Runs as a
// scheduler callback for the length of an event (Battle_Reset).
void Battle_UpdateModeFromShoulderButtons(void) {
  struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

  if (gDebugMode != 0) {
    if (gKeyState & KEY_L) {
      runtime->mode1cc = 0;
    }
    if (gKeyState & KEY_R) {
      runtime->mode1cc = -1;
    }
  }
}

// The event command "wait": waits frames frames, unless the debug fast
// forward (Battle_UpdateModeFromShoulderButtons) is on.
void Battle_WaitMode0(s32 frames) {
  if (((struct BattleRuntime *)gEventWork)->mode1cc == 0 && frames != 0) {
    WaitFrames(frames);
  }
}

// Stops the leader where it stands: no horizontal velocity, the normal walking
// speed (1.0) and acceleration (0.5), no destination, and the standing pose,
// or the climbing pose (12) while the leader is on a climbable wall.
void Battle_InitializeRenderObject(void) {
  struct BattleRenderObject *object;

  object = ObjectTable_Get(gGameState.selectedActor);
  object->unk30 = 0x10000;
  object->unk34 = 0x8000;
  object->unk38 = 0x80000000;
  object->unk40 = 0x80000000;
  object->unk24 = 0;
  object->unk2c = 0;
  if (gGameState.movementMode == 1) {
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 0xC);
    return;
  }
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

// The event command "begin event": closes any timed notice, stops the leader,
// clears the event's message and camera state, starts the debug fast-forward
// callback and makes the leader the camera's subject.
void Battle_Reset(void) {
  struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

  UiTimedNotice_CloseIfActiveFar();
  Battle_InitializeRenderObject();
  if (runtime->unkcb6 != 0) {
    Battle_ResetEffectCounter();
  }
  runtime->unkcc2 = 0;
  runtime->unkcc4 = 0;
  runtime->unk1c8 = 0x10;
  runtime->mode1cc = 0;
  runtime->unk1da = 0xFFFF;
  runtime->unk1dc = -1;
  runtime->unk1de = -1;
  Scheduler_AddOrUpdateCallback(
      (const void *)Battle_UpdateModeFromShoulderButtons, 0xC80);
  GameFlag_ClearBitFar(0x132);
  runtime->objectId = gGameState.selectedActor;
  runtime->unk1f8 = 0;
}

// Ends an event: stops the debug fast-forward callback, gives the camera back
// to the leader and refreshes the lure cap.
void BattleFx_FinishAction(void) {
  Scheduler_RemoveCallback(Battle_UpdateModeFromShoulderButtons);
  Object_AttachWorkTargetToObject(gGameState.selectedActor, 1);
  GameFlag_RefreshLureCapFar();
}

// Empty hooks that the event layer calls; they do nothing in the released
// game.
void Event_NoOpHookA(void) {}

void Event_NoOpHook(void) {}

// Respawns the scene's actors from a placement table and waits one frame for
// them to appear.
void Event_RunObjectHookAndWait(struct ScenePlacement *table) {
  Event_NoOpHook();
  ObjectTable_ResetForObject(table);
  WaitFrames(1);
  ObjectTable_Get(gGameState.selectedActor);
}

// Spawns the actors of a placement table after the last actor in use.
void Event_CallWithLastActiveObjectId(struct ScenePlacement *table) {
  Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

// Sets the scene's event table, which the field checks as the leader moves.
void Event_SetSceneEvents(s32 events) {
  struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

  work->events = (const struct SceneEvent *)events;
}

// Adds unit to the party. With announce nonzero it also shows "<name> joined
// your party." (Party_ShowJoinedMessage, which receives announce too).
void Event_PrepareObjectAndApplyValue(s32 unit, s32 announce) {
  UnitGetFar(unit);
  Party_AddActiveOwnerFar(unit);
  if (announce != 0) {
    Party_ShowJoinedMessageFar(unit, announce);
  }
}

// Adds two units to the party and announces them together.
void Event_PrepareTwoObjectsAndApply(s32 first, s32 second) {
  Party_AddActiveOwnerFar(first);
  Party_AddActiveOwnerFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

// Checks an L or R button shortcut, packed as the member in bits 10 to 13 and
// the Psynergy in bits 0 to 9. Returns 0 if it can still be cast, -1 for no
// such party member, -2 when that member is not in the party and -3 when the
// member no longer knows the Psynergy.
s32 Event_ValidatePackedId(u32 shortcut) {
  s32 member = (shortcut >> 10) & 0xf;
  s32 ability = shortcut & 0x3ff;

  if (member > 7)
    return -1;
  if (GameFlag_TestFar(member) == 0)
    return -2;
  if (Equipment_HasValueFar(member, ability) == 0)
    return -3;
  return 0;
}

// Clears each button shortcut whose Psynergy can no longer be cast.
void Event_ClearInvalidPackedValues(void) {
  if (Event_ValidatePackedId(gGameState.firstShortcut) != 0)
    gGameState.firstShortcut = 0;
  if (Event_ValidatePackedId(gGameState.secondShortcut) != 0)
    gGameState.secondShortcut = 0;
}

// Takes unit out of the party, fully restoring its HP and PP and curing its
// poison. If no member left standing has HP, the leader is given 1 HP so the
// party is never left all down.
void Party_RemoveOwnerRestored(s32 unit) {
  struct BattleUnit *vitals;
  s32 i;
  s32 count;
  s32 standing;

  Party_RemoveActiveOwnerFar(unit);
  Event_ClearInvalidPackedValues();
  vitals = UnitGetFar(unit);
  vitals->hp = vitals->maxHp;
  vitals->pp = vitals->maxPp;
  Unit_UpdateGauges(vitals);
  vitals->poison = 0;
  standing = 0;
  count = Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++) {
    vitals = UnitGetFar(gGameState.activeOwners[i]);
    if (vitals->hp != 0)
      standing++;
  }
  if (standing == 0) {
    vitals = UnitGetFar(gGameState.selectedActor);
    vitals->hp = 1;
    Unit_UpdateGauges(vitals);
  }
}

// Checks whether the party can carry one more of item: each member carries at
// most 30 of one item. When they are all full, says "Isaac's party cannot
// carry any more <item>s..." and "Isaac threw the <item> as far away as
// possible." and returns -1; otherwise returns 0.
s32 Party_CheckMemberValueTotal(s32 item) {
  s32 count;
  s32 sum = 0;
  s32 i;

  count = Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++)
    sum += Inventory_CountItemFar(gGameState.activeOwners[i], item);

  if (sum >= count * 30) {
    UiWork_PushValueSlotFar(item, 2);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore, 1);
    UiWork_PushValueSlotFar(item, 2);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore + 1, 1);
    return -1;
  }
  return 0;
}

// The event command "give item": gives item to the first party member with
// room for it and says so. When every bag is full, the player picks an item
// to drop to make room, or drops the new item instead (not allowed for an
// indispensable one). Returns the member who received the item, or -1 when
// it was left behind.
s32 PartyInventory_GiveItem(s32 item, s32 unused) {
  struct EventRuntime *work;
  s32 savedMessage;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  work = (struct EventRuntime *)gEventWork;
  savedMessage = work->message;
  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    // Every bag is full: "But you can't carry any more items."
    ItemGift_ShowGotItem(item, 1);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryAnyMore, 1);
  retry:
    UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop, 1);
    result = Shop_PickUnitItemFar(&member, &slot);
    if (result == -1) {
      // Cancelled: drop the new item instead, unless it is indispensable
      // ("That <item>'s valuable! You must get it!").
      if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE) {
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 4, 1);
        goto retry;
      }
      // "Drop the <item>?"
      UiWork_PushValueSlotFar(item, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 1, 5);
      result = Object_CallSpawnRoutineAtOrigin(1);
      UiWork_FinalizePendingCoreFar();
      if (result != 0)
        goto retry;
      // "Isaac drops the <item>."
      Item_AdjustCounterFar(item, 1);
      UiWork_PushValueSlotFar(item, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 2, 1);
      work->message = savedMessage;
      return owner;
    } else {
      // A carried item was picked: drop it (the whole stack) and take the new
      // one.
      UnitGetFar(member);
      {
        /* FAKEMATCH: the ROM loads the slot argument before the member. Only
         * the English build, without the thirty-item check below, needs the
         * copy; the direct call matches the rest. */
        s32 s = slot;

        count = Shop_GetSelectionState(member, s);
      }
#if !defined(TBS_EDITION_EN)
      if (Inventory_CountItemFar(member, item) > 29) {
        UiWork_PushValueSlotFar(member, 1);
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 7, 1);
        goto retry;
      }
#endif
      for (i = 0; i < count; i++)
        Inventory_DiscardFar(member, slot);
      owner = PartyInventory_AddFar(item);
      AudioCommand_PlayFar(83);
      if (owner == gGameState.selectedActor) {
        ItemGift_ShowGotItem(item, 3);
      } else {
        UiWork_PushValueSlotFar(item, 2);
        UiWork_PushValueSlotFar(owner, 1);
        UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 3);
      }
      work->message = savedMessage;
      return owner;
    }
  } else {
    // There was room: "Isaac got <item>.", and "Isaac gave <item> to
    // <member>." when someone other than the leader carries it.
    AudioCommand_PlayFar(83);
    ItemGift_ShowGotItem(item, 3);
    if (owner != gGameState.selectedActor) {
      UiWork_PushValueSlotFar(item, 2);
      UiWork_PushValueSlotFar(owner, 1);
#if defined(TBS_EDITION_DE)
      UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 1);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                    \
    defined(TBS_EDITION_IT)
      UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem + 1, 1);
#else
      UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem + 1, 3);
#endif
    }
    work->message = savedMessage;
  }
  return owner;
}

// Gives item to one party member. Returns unit, or -1 if its bag is full.
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 unit) {
  if (Inventory_AddItemFar(unit, item) >= 0)
    return unit;
  return -1;
}

// Does nothing.
void Inventory_NoOpCallback(void) {}

// The event work as Inventory_PromptAndSetObjectMode reads it: the yes/no
// menu's name window, which Battle_Reset clears at the start of each event.
struct EventPromptWork {
  struct EventRuntime event;
  u8 unk200[0xac2];
  s16 nameMessage; // first message of the menu's name window (0: none)
  s16 nameWidth;   // width of the name window (0: the default)
};

// Does nothing.
void Inventory_ReservedNoOp(void) {}

// Waits, at most 90 frames, for an actor's animation to stop being animation:
// the actor has finished a one-off animation such as a nod.
void Object_WaitUntilChildValueDiffers(s32 actor, s32 animation) {
  struct ObjectRuntime *obj = ObjectTable_Get(actor);

  if (obj != 0 && obj->animationKind == 1) {
    s32 frames = 0;
    u8 *current = &((struct AnimationObject *)obj->animation)->lastNo;

    while (frames <= 89) {
      WaitFrames(1);
      if (animation != *current) {
        break;
      }
      frames++;
    }
  }
}

// The event command "choose yes or no": once the message on screen has been
// read, asks yes or no below it, or above it when the message windows reach
// past row 15 or forceBottom is set. The speaker nods (animation 3) at yes
// and shakes its head (animation 4) at no. Returns 0 for yes, 1 for no.
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 forceBottom) {
  struct EventPromptWork *work = (struct EventPromptWork *)gEventWork;
  s32 sprite = ((struct ScenePlacement *)BattleAction_FindDescriptor(
                    work->event.speaker))
                   ->sprite;
  struct UiWindow *window = (struct UiWindow *)work->event.messageWindow;
  struct UiWindow *side = (struct UiWindow *)work->event.sideWindow;
  s32 useBottom = 1;
  s32 answer;

  // Wait for every button to be released and the message to finish.
  while (gKeyState != 0)
    WaitFrames(1);

  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);

  WaitFrames(3);

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

  answer = Menu_RunConfirmSelectionFar(useBottom, work->nameMessage,
                                       work->nameWidth, 0);
  if (answer != 0) {
    Object_SetModeById(actor, 4);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
    UiWork_FinalizePendingCoreFar();
    Object_WaitUntilChildValueDiffers(actor, 4);
  } else {
    Object_SetModeById(actor, 3);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
    UiWork_FinalizePendingCoreFar();
    Object_WaitUntilChildValueDiffers(actor, 3);
  }

  return answer;
}

// Asks yes or no at the top of the screen, or at the bottom with atBottom
// set. Returns 0 for yes, 1 for no.
s32 Object_CallSpawnRoutineAtOrigin(s32 atBottom) {
  return Menu_RunConfirmSelectionFar(atBottom, 0, 0, 0);
}

// Teaches unit the Psynergy ability and says "<unit> learned <ability>." with
// its jingle, waiting for the player to read it.
void UiText_DrawQuantityPairWithCue(s32 unit, s32 ability) {
  // Declared without parameters: unit and ability pass through in r0 and r1.
  OwnerAction_AddFar();
  AudioCommand_PlayFar(83);
  UiWork_PushValueSlotFar(unit, 1);
  UiWork_PushValueSlotFar(ability, 4);
  UiText_ShowPositionedMessageAndWaitFar(MsgAbilityLearned, 3);
}

// The event command "open screen": starts the event's screen transition.
void Event_StartScreenTransition(void) {
  struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

  DisplayTransition_Start(runtime->value1c0, runtime->value1c8);
  runtime->status1c6 = 1;
}

// The event command "close screen": finishes the event's screen transition.
void Event_FinishScreenTransition(void) {
  struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

  DisplayTransition_Finish(runtime->value1c0, runtime->value1c8);
  runtime->status1c6 = 0;
}

// The event command "wait for screen": waits as long as the transition runs.
void Event_WaitTransitionFrames(void) {
  WaitFrames(((struct EventRuntime *)gEventWork)->value1c8);
}

// Makes the event leave for entrance entrance of scene scene.
void Event_SetDestination(s32 scene, s32 entrance) {
  ((struct EventRuntime *)gEventWork)->value170 = 999;
  gGameState.scene = scene;
  gGameState.entrance = entrance;
}

// Makes the event leave and records scene and entrance as the saved
// destination.
void Event_SetSavedDestination(u16 scene, u16 entrance) {
  struct EventRuntime *state = (struct EventRuntime *)gEventWork;
  state->value170 = 999;
  gGameState.savedScene = scene;
  gGameState.savedEntrance = entrance;
}

// The event command "request exit": sets the field's exit request (999 leaves
// for gGameState's scene and entrance).
void Event_SetExitRequest(u16 request) {
  struct EventRuntime *state = (struct EventRuntime *)gEventWork;
  state->value170 = request;
}

//--------------------------------------------------------------------------
// Battles that an event starts with a chosen enemy group.
//--------------------------------------------------------------------------

#define AERIE 98           // encounter table fought before the Aerie's storm
#define DJINN_NO_HOP 0x800 // do not make the Djinni hop first
#define DJINNI_0 300       // pendingDjinnEvent of Djinni 0

// Sets up a battle with enemy group slot of encounter table zone. On the
// world map (mode 3) the terrain under the leader picks the battle backdrop.
void BattleFx_SetWeightedResult(s32 zoneNo, s32 slotNo) {
  s32 zone = zoneNo;
  s32 slot = slotNo;
  struct EventRuntime *ev = (struct EventRuntime *)gEventWork;

  ev->value17c = BattleFx_GetWeightedResult(zone, slot); // enemy group
  if (zone == AERIE && slot == 0)
    gGameState.special = (s32)&ResourceId_VenusAerieStormBackdrop;
  if (ev->mode19e == 3)
    BattleFx_LookupResult((u8 *)ObjectTable_Get(gGameState.selectedActor) +
                          8); // position
  BattleFx_SelectBattleCue(zone, slot);
}

// Sets up the battle with Djinni no. The low 8 bits of flag are the Djinni's
// actor, which hops first unless DJINN_NO_HOP is set. Djinn are numbered 20
// to an element (Djinn_ResolvePendingEvent).
void BattleFx_SetPhaseRequest(s32 flag, s32 no) {
  struct EventRuntime *ev;
  struct GameState *gs;
  s32 skip;

  ev = (struct EventRuntime *)gEventWork;
  skip = flag & DJINN_NO_HOP;
  flag &= 0xff;

  if (skip == 0)
    EventActor_RunHopAndLeapSequence(flag);

  gs = &gGameState;
  gs->pendingDjinnEvent = (no + DJINNI_0) | skip;
  ev->value17c = BattleFx_GetPhaseResult(no); // the Djinni's enemy group
  if (ev->mode19e == 3) {
    void *obj = ObjectTable_Get(gs->selectedActor);

    BattleFx_LookupResult((u8 *)obj + 8); // position
  }
  BattleFx_SelectBattleCue(0, 0);
}

// Sets the scene and entrance that the field goes to next.
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance) {
  gGameState.nextScene = scene;
  gGameState.nextEntrance = entrance;
}

// Sets where the party wakes up after losing a battle.
void Event_SetDefeatDestination(s32 scene, s32 entrance) {
  gGameState.defeatScene = scene;
  gGameState.defeatEntrance = entrance;
}

// Makes the event leave for entrance entrance of scene scene.
void Event_RequestDestination(u16 scene, u16 entrance) {
  struct EventRuntime *work = (struct EventRuntime *)gEventWork;
  gGameState.scene = scene;
  gGameState.entrance = entrance;
  work->value170 = 999;
}

// Plays sound effect 0x12a, then soundId (0x121 for -1), and remembers
// soundId for BattleFx_PlayQueuedSound.
void BattleFx_SetQueuedSoundAndPlay(s32 soundId) {
  ((struct FieldWork *)gEventWork)->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = 0x121;
  }
  AudioCommand_PlayFar(0x12a);
  AudioCommand_PlayFar(soundId);
}

// Plays the sound BattleFx_SetQueuedSoundAndPlay remembered, if any.
void BattleFx_PlayQueuedSound(void) {
  s16 soundId = ((struct FieldWork *)gEventWork)->queuedSound;

  if (soundId != -1)
    AudioCommand_PlayFar(soundId);
}

// Returns an actor, or NULL if it does not exist.
struct FieldActor *Object_GetById(s32 actor) {
  struct FieldActor *object = ObjectTable_Get(actor);

  if (object == NULL)
    return NULL;
  return object;
}

// The event command "set actor speed": its top speed and acceleration, 16.16
// pixels per frame.
void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
  }
}

// Gives an actor an action script (ObjectMotion_SetActionCallback) and lets
// it run.
void ObjectMotion_EnableActionAndSetCallback(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    ObjectMotion_SetActionCallback(object, action);
  }
}

// The event command "stop actor": stops its action and puts it back under
// event control (dispatch script 6).
void ObjectMotion_EnableActionAndResetMotion(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  u8 enabled;

  if (object != NULL) {
    enabled = 1;
    object->actionFlags = enabled | object->actionFlags;
    ObjectDispatch_InitFromTable6Far(object);
  }
}

// Makes an actor follow another (the linked action script).
void Object_LinkObjectAndSetCallback(u32 objectId, u32 linkedObjectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->linkedObject =
        (struct ObjectRuntime *)Object_GetById(linkedObjectId);
    ObjectMotion_SetActionCallback(object,
                                   (s32)gObjectMotion_LinkedActionScript);
  }
}

// Waits, at most 300 frames, for an actor's script to reach its end
// (command 0x10).
void Object_RefreshSelectorById(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_WaitForValue16Far(object);
}

// Gives an actor an action script and waits for the script to end.
void Object_SetActionCallbackAndRefreshById(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    ObjectMotion_SetActionCallback(object, action);
    ObjectDispatch_WaitForValue16Far(object);
  }
}

// The event command "set actor destination": the actor heads for (x, z), in
// pixels, keeping its current animation.
void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

// The event command "move actor and wait": as ObjectMotion_ResetAndSetPosition,
// then waits (at most 600 frames) for the actor to arrive.
void ObjectMotion_SetPositionAndCommit(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    Script_WaitForEventTimeoutFar(object);
  }
}

// The event command "walk actor to": the actor walks (animation 2) toward
// (x, z), in pixels.
void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

// The event command "walk actor to and wait": walks the actor to (x, z), in
// pixels, waits for it to arrive and stands it still (animation 1).
void ObjectMotion_SetPositionAndReset(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    Script_WaitForEventTimeoutFar(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  }
}

// The event command "center actor and walk": walks the actor sideways to the
// middle of its 16-pixel column, sets its sprite priority to priority
// (ObjectMotion_SetActionVariant), then sends it zOffset pixels along z.
void ObjectMotion_CenterOnColumn(u32 objectId, s32 priority, s32 zOffset) {
  struct ObjectRuntime *object;
  s16 pixelX;
  s32 columnX;
  s16 offsetInColumn;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    pixelX = ((union FieldCoordinate *)&object->x)->part.pixel;
    columnX = pixelX;
    if (pixelX < 0) {
      columnX += 15;
    }
    columnX >>= 4;
    columnX *= 16;
    offsetInColumn = pixelX - columnX;
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    Object_SetMoveTargetFar(object, object->x + ((8 - offsetInColumn) << 16),
                            object->y, object->z);
    Script_WaitForEventTimeoutFar(object);
    ObjectMotion_SetActionVariant(objectId, priority);
    Object_SetMoveTargetFar(object, object->x, object->y,
                            object->z + (zOffset << 16));
  }
}

// The event command "set actor destination offset": the actor heads
// (xOffset, zOffset) pixels from where it stands.
void ObjectMotion_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

// Walks the actor (animation 2) (xOffset, zOffset) pixels from where it
// stands.
void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 xOffset,
                                         s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

// Walks the actor (xOffset, zOffset) pixels, waits for it to arrive and
// stands it still.
void ObjectMotion_CommitPositionAndActivate(u32 objectId, s32 xOffset,
                                            s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  ObjectMotion_OffsetPositionAndReset(objectId, xOffset, zOffset);
  if (object != NULL) {
    Script_WaitForEventTimeoutFar(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  }
}

// Places an actor on the leader and sends it walking (xOffset, zOffset)
// pixels away at speed 0.6 and acceleration 0.3, through the launch script;
// the actor then takes action action.
void Motion_LaunchFromFocusedObject(u32 objectId, s32 xOffset, s32 zOffset,
                                    s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    struct ObjectRuntime *leader;

    ObjectMotion_SetSpeedParameters(objectId, 0x9999, 0x4CCC);
    leader = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
    if (leader != NULL)
      ObjectMotion_SetHorizontalPositionWithTerrain(objectId, leader->x,
                                                    leader->z);
    object->movementState = 0;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    ObjectMotion_OffsetPositionAndResetMotion(objectId, xOffset, zOffset);
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (const s32 *)&gObjectMotion_LaunchScript);
    object->action = action;
  }
}

// The event command "wait for actor": waits (at most 600 frames) for the actor
// to arrive and stands it still.
void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    Script_WaitForEventTimeoutFar(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  }
}

// The event command "set actor position": puts the actor at (x, z), 16.16
// pixels, at rest. An actor that follows the ground (flag 1) keeps its height
// above the terrain at the new place.
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object;
  s32 terrainHeight;
  s32 tileX;
  s32 tileZ;
  s32 terrainId;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    ObjectDispatch_InitFromTable6Far(object);
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->targetY = ACTOR_NO_TARGET;
    object->targetX = ACTOR_NO_TARGET;
    object->x = x;
    object->z = z;
    if (1 & object->flags) {
      terrainId = object->terrainId;
      tileX = x;
      if (tileX < 0) {
        tileX += 0xFFFF;
      }
      tileX >>= 0x10;
      tileZ = z;
      if (tileZ < 0) {
        tileZ += 0xFFFF;
      }
      tileZ >>= 0x10;
      terrainHeight = Map_GetTerrainHeightFar(terrainId, tileX, tileZ) << 0x10;
      object->y = (object->y - object->terrainHeight) + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

// Puts the actor at (x, y, z), 16.16 pixels, at rest; an actor that follows
// the ground (flag 1) is raised by the terrain height there.
void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z) {
  struct ObjectRuntime *object;
  s32 terrainHeight;
  s32 tileX;
  s32 tileZ;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    ObjectDispatch_InitFromTable6Far(object);
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->targetY = ACTOR_NO_TARGET;
    object->targetX = ACTOR_NO_TARGET;
    object->x = x;
    object->y = y;
    object->z = z;
    if (1 & object->flags) {
      s32 terrainId = object->terrainId;
      tileX = x;
      if (tileX < 0) {
        tileX += 0xFFFF;
      }
      tileX >>= 0x10;
      tileZ = z;
      if (tileZ < 0) {
        tileZ += 0xFFFF;
      }
      terrainHeight = Map_GetTerrainHeightFar(terrainId, tileX, tileZ >> 0x10)
                      << 0x10;
      object->y = (object->y - object->terrainHeight) + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

// The event command "set actor animation" (1 stands, 2 walks, 3 nods, 4 shakes
// the head and so on).
void Object_SetModeById(u32 objectId, s32 animation) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_ApplyArgumentToChildrenFar(object, animation);
}

// Sets the animation timer steps of each of an actor's sprites
// (AnimationObjects_SetActiveTimerSteps).
void Object_SetActionById(u32 objectId, s32 steps) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object,
                                           steps);
}

// Waits, at most 90 frames, for an actor's animation to change.
void ObjectMotion_WaitForAnimationChange(u32 objectId) {
  struct ObjectRuntime *object;
  struct AnimationObject *sprite;
  /* FAKEMATCH: volatile restores the form the bytes prove: saved lives in a
   * 4-byte frame, stored once and loaded again for each compare. Without it
   * differ 29, 26 instructions for 31 and no frame. */
  volatile s32 saved;
  s32 i;

  object = ObjectTable_Get(objectId);
  if (object != NULL && object->animationKind == 1) {
    sprite = object->animation;
    saved = sprite->lastNo;
    for (i = 0; i <= 89; i++) {
      WaitFrames(1);
      if (saved != sprite->lastNo) {
        break;
      }
    }
  }
}

// The event command "set actor animation and wait": plays the animation and
// waits for it to end.
void Motion_SetModeAndWaitAnimation(u32 objectId, s32 animation) {
  Object_SetModeById(objectId, animation);
  ObjectMotion_WaitForAnimationChange(objectId);
}

// Does nothing.
void ObjectMotion_NoOp(void) {}

// The event command "actor jump": throws the actor upward at speed pixels per
// frame with a jump sound (a higher one above 5), then waits frames frames.
void ObjectMotion_Launch(u32 objectId, s32 speed, s32 frames) {
  struct ObjectRuntime *object;
  u8 *objectFlags;
  s32 updatedFlags;
  s32 verticalVelocity;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    objectFlags = &object->flags;
    updatedFlags = *objectFlags | 2;
    verticalVelocity = speed << 16;
    *objectFlags = updatedFlags;
    object->velocityY = verticalVelocity;
    if (speed > 5) {
      AudioCommand_PlayFar(0x99);
    } else {
      AudioCommand_PlayFar(0x98);
    }
    Battle_WaitMode0(frames);
  }
}

// The event command "start repeated motion": the actor squashes and springs
// back count times (1 to 3), through gObjectMotion_VariantScripts.
void ObjectMotion_SetVariantCallback(u32 objectId, s32 count) {
  struct ObjectRuntime *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL && count > 0) {
    if (count > 3) {
      count = 3;
    }
    // Each count is 32 words of the script; start that many from its end.
    ObjectDispatch_InitializeFar(
        (struct DispatchObject *)object,
        (const s32 *)(gObjectMotion_VariantScripts + ((3 - count) << 7)));
  }
}

// The event command "run repeated motion": as ObjectMotion_SetVariantCallback,
// then waits for it to end.
void Motion_SetVarCbAndRefresh(u32 objectId, s32 count) {
  ObjectMotion_SetVariantCallback(objectId, count);
  Object_RefreshSelectorById(objectId);
}

// Moves a dust puff of BattleFx_SpawnBurstParticle: it drifts with its x and
// z velocity (kept in speed and acceleration), which drag slows by 1/18 and
// 1/16 each frame, and rises 1/64 pixel a frame.
void BattleFx_UpdateParticleLinearMotion(struct FieldActor *particle) {
  s32 velocityX;
  s32 x;
  s32 velocityZ;
  s32 z;
  s32 y;
  s32 roundedZ;
  velocityX = particle->speed;
  x = particle->x.fixed + velocityX;
  particle->x.fixed = x;
  particle->targetX = x;
  velocityZ = particle->acceleration;
  z = particle->z.fixed + velocityZ;
  particle->z.fixed = z;
  particle->targetZ = z;
  y = particle->y.fixed + 0x400;
  particle->y.fixed = y;
  particle->targetY = y;
  particle->speed = (velocityX - velocityX / 0x12);

  roundedZ = velocityZ;
  if (velocityZ < 0) {
    roundedZ += 0xF;
  }
  particle->acceleration = (velocityZ - (roundedZ >> 4));
}

// Kicks up a dust puff (object 222) where source stands: it flies off toward
// the back at 0.5 to 1.4 pixels a frame and sideways at up to 1.4, with one
// of two animations. A nonzero param is given to its sprite parts
// (ObjectGroup_SetChildValue).
void BattleFx_SpawnBurstParticle(struct FieldActor *source, s32 param) {
  struct FieldActor *object;
  struct FieldSprite *child;
  s32 value;

  object = (struct FieldActor *)Object_CreateFar(
      222, source->x.fixed, source->y.fixed, source->z.fixed);
  if (object != 0) {
    child = object->sprite;
    switch (Random16() & 1) {
    case 1:
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
      ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                   (const s32 *)gBattleFx_BurstParticleScriptA);
      break;
    default:
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
      ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                   (const s32 *)gBattleFx_BurstParticleScriptB);
      break;
    }

    if (param != 0)
      ObjectGroup_SetChildValue(object, param);

    // speed and acceleration hold the puff's x and z velocity.
    object->motionFlags = 0;
    value = Random16() % 10 + 5;
    object->acceleration = -0x1999 * value;
    value = Random16() % 15 - 7;
    value <<= 1;
    object->speed = 0x1999 * value;
    object->timer = 0;
    object->update = BattleFx_UpdateParticleLinearMotion;
    child->flags = 0;
    child->priority = source->sprite->priority;
  }
}

// Makes an actor jump down off a ledge in front of it (the field runs it for
// the leader when the player presses down at a ledge): it shows pose for 10
// frames, hops, spins round while sinking 28 pixels with a dust puff every
// other frame (none when param is -1), then jumps 16 pixels forward and lands.
// The landing plays the queued sound (BattleFx_PlayQueuedSound).
void BattleFx_RunRisingObjectSequence(s32 actor, s32 pose, s32 param) {
  s32 nextY;
  s32 startZ;
  struct ObjectRuntime *object;
  u8 *objectFlags;

  object = (struct ObjectRuntime *)Object_GetById(actor);
  startZ = object->z;
  if (object != 0) {
    AudioCommand_PlayFar(0x121);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, pose);
    WaitFrames(10);

    // Hop up and forward 12 pixels.
    objectFlags = &object->flags;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    {
      u8 flags = 2;
      flags |= *objectFlags;
      *objectFlags = flags;
    }
    object->velocityY = 0x40000;
    Object_SetMoveTargetFar(object, object->x, object->y, startZ + 0xC0000);
    WaitFrames(6);

    // Spin and sink 2 pixels a frame for 14 frames, raising dust.
    AudioCommand_PlayFar(0xD9);
    pose = 0; // from here a frame counter
    ObjectMotion_ArmCallback(actor, 0x5000, 0);
    *objectFlags = 0;
    do {
      nextY = object->y + 0xFFFE0000;
      object->y = nextY;
      object->targetY = nextY;
      WaitFrames(1);
      if ((param != -1) && (pose & 1)) {
        BattleFx_SpawnBurstParticle((struct FieldActor *)object, param);
      }
      pose++;
    } while ((u32)pose <= 0xD);

    // Jump 16 pixels forward and wait (at most 180 frames) to land.
    *objectFlags = 3;
    object->velocityY = 0x30000;
    Object_SetMoveTargetFar(object, object->x, object->y, startZ + 0x100000);
    Script_WaitForEventTimeoutFar(object);
    for (pose = 0; (u32)pose <= 0xB3 && object->y > object->terrainHeight;
         pose++) {
      WaitFrames(1);
    }
    WaitFrames(2);
    BattleFx_PlayQueuedSound();
  }
}

// The event command "actor face actor": turns the first actor at once to face
// the second, then waits wait frames.
void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait) {
  struct ObjectRuntime *first;
  struct ObjectRuntime *second;

  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);
  if (first != 0 && second != 0) {
    first->angle = ArcTan2(second->z - first->z, second->x - first->x);
    Battle_WaitMode0(wait);
  }
}

// The event command "actors face each other": turns two actors toward each
// other, then waits wait frames.
void Object_LinkPair(s32 firstId, s32 secondId, s32 wait) {
  void *first = ObjectTable_Get(firstId);
  void *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    FacingObject_TurnPairToFaceEachOther(first, second);
    Battle_WaitMode0(wait);
  }
}

// Turns two actors to face each other, at most 1/16 turn a frame each, until
// both do (at most 60 frames).
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *raw_first,
                                          struct FacingObject *raw_second) {
  struct ObjectRuntime *first = (struct ObjectRuntime *)raw_first;
  struct ObjectRuntime *second = (struct ObjectRuntime *)raw_second;
  s32 toward;
  s32 away;
  s32 frame;
  s32 turning;
  s32 delta;

  if (first == NULL || second == NULL)
    return;
  toward = (u16)ArcTan2(second->z - first->z, second->x - first->x);
  away = toward + 0x8000;
  for (frame = 0; frame < 60; frame++) {
    turning = 2;
    delta = (s16)(toward - first->angle);
    if (delta != 0) {
      if (delta > 0x1000)
        delta = 0x1000;
      if (delta < -0x1000)
        delta = -0x1000;
      first->angle += delta;
    } else {
      turning = 1;
    }
    delta = (s16)(away - second->angle);
    if (delta != 0) {
      if (delta > 0x1000)
        delta = 0x1000;
      if (delta < -0x1000)
        delta = -0x1000;
      second->angle += delta;
    } else {
      turning--;
    }
    if (turning == 0)
      break;
    WaitFrames(1);
  }
}

// The event command "destroy actor": releases the actor and frees its slot.
void ObjectTable_DestroyById(s32 index) {
  void *object = ObjectTable_Get(index);
  struct ObjectSlotTable *table = (struct ObjectSlotTable *)gEventWork;

  if (object != 0) {
    ObjectDispatch_ReleaseFar(object);
    table->slots[index] = NULL;
  }
}

// Does nothing.
void ObjectTable_DestroyNoOp(void) {}

// The event command "set actor sprite param": gives every sprite part of the
// actor param (ObjectGroup_SetChildValue). With bit 0x100 set the actor
// blinks instead (ObjectGroup_ApplyIndexedChildValue, every frame).
void ObjectGroup_ConfigureChildValue(s32 objectId, s32 param) {
  s32 blink;
  struct DispatchObject *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    blink = 0x100 & param;
    if (blink != 0) {
      ((struct ScriptMotionObject *)object)->hook = (void (*)(
          struct ScriptMotionObject *))ObjectGroup_ApplyIndexedChildValue;
      return;
    }
    ((struct ScriptMotionObject *)object)->hook = NULL;
    ObjectGroup_SetChildValue((struct FieldActor *)object, param);
  }
}

// Per-frame hook of a blinking actor: sets its sprite parts' param from
// gObjectGroup_BlinkChildValues (7 for two frames out of eight, otherwise 0).
void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object) {
  if ((object->kind & 0xf) == 1) {
    u8 param;
    struct AnimationObject *container;
    u8 partCount;

    param = gObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
    container = object->target.child;
    partCount = container->count;
    if (partCount != 0) {
      struct AnimationEntry **entries = container->entries;
      s32 remaining = partCount;
      do {
        struct AnimationEntry *entry = *entries++;
        if (entry != 0 && entry->script != 0) {
          entry->param = param;
        }
        remaining--;
      } while (remaining != 0);
    }
    container->dirty = 1;
  }
}

// Sets the param byte of every animated part of an actor's sprite. Scenes set
// 0 or 15, and alternate the two to make an actor flash.
void ObjectGroup_SetChildValue(struct FieldActor *actor, s32 param) {
  struct DispatchObject *object = (struct DispatchObject *)actor;

  if ((object->kind & 0xf) == 1) {
    struct AnimationObject *container = object->target.child;
    u8 partCount = container->count;

    if (partCount != 0) {
      struct AnimationEntry **entry = container->entries;
      u32 count = partCount;
      do {
        struct AnimationEntry *part = *entry++;
        if (part != NULL && part->script != 0) {
          part->param = param;
        }
        count--;
      } while (count != 0);
    }
    container->dirty = 1;
  }
}

// Links an actor to the actor in the low 8 bits of targetId and starts script
// on it. Unless bit 0x10000 is set it also takes action 40 and the target's
// top speed with twice its acceleration, to keep up with it.
void Object_SetTargetAndCallback(u32 objectId, s32 targetId,
                                 const void *script) {
  struct ObjectRuntime *follower = ObjectTable_Get(objectId);
  struct ObjectRuntime *target = ObjectTable_Get(targetId & 0xff);

  if (follower != NULL && target != NULL) {
    follower->linkedObject = target;
    if (!(targetId & 0x10000)) {
      follower->action = 40;
      follower->acceleration = target->acceleration * 2;
      follower->speedLimit = target->speedLimit;
      follower->unk59 = 0;
    }
    ObjectDispatch_InitializeFar((struct DispatchObject *)follower, script);
  }
}

// Script step that turns an actor toward the angle kept in its action: the
// whole way when within 1/16 turn, otherwise 1/32 turn. Returns the turn
// made, 0 once the actor faces that way.
s32 ObjectMotion_StepAngle(struct ObjectRuntime *object) {
  s32 delta = 0;

  if (object != NULL) {
    s32 targetAngle = (u16)object->action;
    s32 currentAngle = object->angle;
    delta = (s16)(targetAngle - currentAngle);
    if (delta != 0) {
      if (delta > 4096)
        delta = 2048;
      if (delta < -4096)
        delta = -2048;
      object->angle = currentAngle + delta;
    }
  }
  return delta;
}

// Clears an actor's destination, puts it under event control and stands it
// still.
void Object_ResetTargetAndSetMode1(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->targetX = ACTOR_NO_TARGET;
    object->targetY = ACTOR_NO_TARGET;
    object->targetZ = ACTOR_NO_TARGET;
    ObjectDispatch_InitFromTable6Far(object);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  }
}

// The event command "actor face direction": the actor turns toward angle
// (0x10000 a full turn; ObjectMotion_StepAngle), then the event waits wait
// frames.
void ObjectMotion_ArmCallback(s32 objectId, s32 angle, s32 wait) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->action = angle;
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (const s32 *)gObjectMotion_StepAngleScript);
    Battle_WaitMode0(wait);
  }
}

// The event command "set actor sprite priority": fixes the priority of the
// actor's sprite, which then no longer follows its depth.
void ObjectMotion_SetActionVariant(s32 objectId, s32 priority) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL && (object->animationKind & 0xF) == 1) {
    struct FieldSprite *sprite = object->animation;

    sprite->priority = priority;
    sprite->secondPriority = priority;
    object->unk23 &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

// Makes the target actor's sprite show the source actor's graphics (its VRAM
// block and tile).
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId) {
  void *p;
  u8 vramBlock;
  u32 tile;

  p = Object_GetById(sourceId);
  p = ((struct ObjectRuntime *)p)->animation;
  vramBlock = ((struct FieldSprite *)p)->vramBlock;
  tile = ((struct FieldSprite *)p)->tile;

  p = Object_GetById(targetId);
  p = ((struct ObjectRuntime *)p)->animation;
  ((struct FieldSprite *)p)->vramBlock = vramBlock;
  ((struct FieldSprite *)p)->tile = tile;
}

// Does nothing.
void ObjectVisual_ReservedNoOp(void) {}

// The event command "set message": the number of the event's next message.
void Event_SetMessageId(s32 message) {
  ((struct EventRuntime *)gEventWork)->message = message;
}

// Returns the sprite (the first animation's id) of the actor in the low 12
// bits of actor, or -1 if there is no such animated actor.
s32 ObjectTable_ReadActiveValue(s32 actor) {
  struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
  s32 sprite = -1;
  struct ObjectRuntime *entry = state->actors[actor & 0x0fff];

  if (entry != 0 && entry->animationKind == 1)
    sprite = ((struct AnimationObject *)entry->animation)->entries[0]->animId;
  return sprite;
}

// Returns the first scene actor (8 to 65) showing sprite, or -1.
s32 ObjectTable_FindActiveByValue(s32 sprite) {
  struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
  s32 result = -1;
  s32 index;
  struct ObjectRuntime *object;

  for (index = 8; index <= 65; index++) {
    object = state->actors[index];
    if (object != 0 && object->animationKind == 1 &&
        ((struct AnimationObject *)object->animation)->entries[0]->animId ==
            sprite) {
      result = index;
      break;
    }
  }
  return result;
}

// The event command "open message": opens the event's current message in a
// speech window next to the speaking actor, with the speaker's portrait
// beside it when it has one, and moves on to the next message. The low 12
// bits of actor are the speaker (0 to 7, a party member, speaks from the
// leader); its high bits place the windows: 0x1000 portrait on the left,
// 0x2000 on the right, 0x4000 message above the speaker, 0x8000 never above.
// Returns the message window.
struct UiWindow *UiText_OpenMessageAtObject(s32 actor, s32 unused) {
  struct UiRenderWork *win;
  struct EventRuntime *work;
  s32 sprite;
  s32 extra;
  s32 portraitTop;
  s32 margin;
  s32 height;
  s32 width;
  s32 top;
  s32 left;
  s32 pos[3];
  struct UiWindow *handle;
  s32 portrait;
  s32 flags;
  s32 message;
  struct ObjectRuntime *object;
  s32 x;
  s32 y;
  s32 found;
  s32 column;
#if !EDITION_INTERNATIONAL
  s32 none;
#endif

  win = (struct UiRenderWork *)gWindowWork[0];
  work = ((struct WorkPointers *)gWindowWork)->event;
  handle = 0;
  portrait = 0;
  sprite = ObjectTable_ReadActiveValue(actor);
  extra = 0;
  portraitTop = 0;
  margin = 4;
  flags = actor & 0xf000;
  message = work->message;
  actor &= 0xfff;
  x = 0;
  object = ObjectTable_Get(actor);
  work->speaker = actor;
  y = 0;
  found = 0;
  if (work->messageBusy == 0) {
    // Find the speaker on screen, in 8-pixel tiles.
    if (object != 0) {
      if (work->mode19e == 3) {
        Render_ProjectPoint(&object->x, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
        y -= 2;
      } else {
        if (Object_GetScreenPosition(actor, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    } else if (actor <= 7) {
      sprite = actor;
      object = ObjectTable_Get(gGameState.selectedActor);
      if (work->mode19e == 3) {
        Render_ProjectPoint(&object->x, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
      } else {
        if (Object_GetScreenPosition(gGameState.selectedActor, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    }

    // Place the message above or below the speaker, or mid-screen when the
    // speaker is not on screen.
    if (!found) {
      left = 15;
      top = 10;
    } else {
      left = 0;
      top = 0;
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
                                         (u32 *)&height);
      left = x - width / 2;
      if (flags & 0x4000)
        top = y - height - 1;
      else if (!(flags & 0x8000) && y <= 8)
        top = y - height - 1;
      else
        top = y + 4;
    }

    // Place the portrait beside the speaker, on the side with more room.
    if (win->mode != 0)
      margin = 5;
    column = x;
    if (flags & 0x1000) {
      column = x - margin - 2;
      if (column < 0)
        column = 0;
    } else if (flags & 0x2000) {
      column += 2;
      if (column + margin > 29)
        column = 29 - margin;
    } else if (column <= 15) {
      column = column - margin - 2;
      if (column < 0)
        column = x + 2;
    } else {
      column += 2;
      if (column + margin > 29)
        column = x - margin - 2;
    }

    // A speaker with a portrait: put the portrait's 5 rows just above or
    // below the message, and grow the message to the portrait's height.
#if EDITION_INTERNATIONAL
    found = Localization_LookupEntryIdFar(sprite);
    if (found != -1) {
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
                                         (u32 *)&height);
      message = -1;
      if (top > y)
        portraitTop = top - 5;
      else
        portraitTop = top + height;
      if (portraitTop < 0)
        portraitTop = top + height;
      else if (portraitTop + 5 > 19)
        portraitTop = top - 5;
      if (top < portraitTop) {
        s32 lines = height;

        UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
                                        (u32 *)&height);
        message = -1;
        extra = lines - height + 1;
      }
    } else if (top < y) {
      s32 lines = height;

      UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
                                      (u32 *)&height);
      extra = lines - height + 1;
      message = found;
    }
#else
    none = Localization_LookupEntryIdFar(sprite);
    found = -1;
    if (none != found) {
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
                                         (u32 *)&height);
      message = found;
      if (top > y)
        portraitTop = top - 5;
      else
        portraitTop = top + height;
      if (portraitTop < 0)
        portraitTop = top + height;
      else if (portraitTop + 5 > 19)
        portraitTop = top - 5;
    }
    if (top < y) {
      s32 lines = height;

      UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
                                      (u32 *)&height);
      message = -1;
      extra = lines - height + 1;
    }
#endif
    if (column < 0)
      column = 0;
    else if (column + margin > 29)
      column = 29 - margin;

    // Open the message (with the speaker's voice outside mode 1) and the
    // portrait, and wait for both to be drawn.
    if (win->mode != 0) {
      WaitFrames(8);
      if (extra != 0)
        handle =
            UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
      else
        handle = UiText_OpenMessageWindowFar(message, left, top, 2);
    } else {
      s32 voice = BattleFx_GetResourceId(sprite);

      if (extra != 0)
        handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                             (voice << 16) | 17);
      else
        handle =
            UiText_OpenMessageWindowFar(message, left, top, (voice << 16) | 1);
    }
    if (win->mode != 0)
      portrait =
          UiWindow_CreateWithSideObjectFar(sprite, 0, column, portraitTop);
    else
      portrait =
          UiWindow_CreateWithSideObjectFar(sprite, 0, column, portraitTop);
    while (!UiWork_IsCompleteFar())
      WaitFrames(1);
  }
  work->messageWindow = (s32)handle;
  work->sideWindow = portrait;
  work->message++;
  return handle;
}

// The event command "show message": opens the current message at actor
// (UiText_OpenMessageAtObject) and waits until it has been read. The message
// closes by itself after 600 frames, or at once while Select, L, R and A are
// all held.
void BattleEv_RunWait(s32 actor, s32 flags) {
  s32 member;
  struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;
  struct UiWindow *window = UiText_OpenMessageAtObject(actor, flags);
  s32 sprite;
  u32 frames;

  frames = 0;
  WaitFrames(1);
  sprite = ObjectTable_ReadActiveValue(actor);
  if (actor <= 7) {
    member = actor & 0x0fff;

    if (BattleAction_FindDescriptor(member) == 0) {
      sprite = member;
    }
  }
  UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);

  if (runtime->messageBusy == 0) {
    while (UiWork_IsIdleFar(window) == 0) {
      WaitFrames(1);
      frames++;
      if (frames > 600 || ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) &&
                           (gKeysHeld & KEY_L) && (gKeysHeld & KEY_A))) {
        UiWork_FinalizePendingCoreFar();
      }
    }
  }

  WaitFrames(1);
}

// The event command "show message and wait": shows the message, then waits
// wait frames.
void BattlePres_RunActionThenWaitIfModeZero(s32 actor, s32 flags, s32 wait) {
  BattleEv_RunWait(actor, flags);
  Battle_WaitMode0(wait);
}

// The event command "ask yes or no": opens the question at actor, lets the
// leader answer, then shows the next message for yes or the one after it for
// no. Returns 0 for yes, 1 for no.
s32 BattleEventRuntime_ProcessAction(s32 actor, s32 flags) {
  s32 answer;
  struct EventRuntime *runtime;
  struct GameState *save;

  UiText_OpenMessageAtObject(actor, flags);
  save = &gGameState;
  answer = Inventory_PromptAndSetObjectMode(save->selectedActor, 0);
  if (answer == 0) {
    BattleEv_RunWait(actor, flags);
    runtime = (struct EventRuntime *)gEventWork;
    runtime->message += 1;
  } else {
    runtime = (struct EventRuntime *)gEventWork;
    runtime->message += 1;
    BattleEv_RunWait(actor, flags);
  }
  return answer;
}

// Does nothing.
void BattleEventRuntime_EmptyCallback(void) {}

// Opens the event's current message for speaker, kept 32 pixels above or
// below a point and on screen, waits until it has been read and moves on to
// the next message.
void Event_ShowSpeakerMessage(s32 speaker) {
  // BUG: pos is never set, so the window goes wherever the stack's old
  // contents put it.
  struct {
    s32 x, y;
  } pos;
  struct EventRuntime *state = (struct EventRuntime *)gEventWork;

  speaker &= 0xfff;
  ObjectTable_Get(speaker);
  state->speaker = speaker;
  if (state->messageBusy == 0) {
    s32 px = pos.x;
    s32 py = pos.y;
    s32 win;

    if (py > 119)
      py += 32;
    else
      py -= 32;
    px = px < 8 ? 8 : px;
    if (px > 312)
      px = 312;
    py = py < 20 ? 20 : py;
    if (py > 220)
      py = 220;
    win = UiText_OpenMessageWindowFar(state->message, px, py, 1);
    state->messageWindow = win;
    while (UiWork_IsIdleFar((struct UiWindow *)win) == 0)
      WaitFrames(1);
  }
  state->message++;
}

// Shows the current message near the screen point (x, y)
// (Event_ShowMessageAtPosition).
void ObjectTable_CallRefreshHook(s32 unused0, s32 unused1, s32 x, s32 y) {
  Event_ShowMessageAtPosition(unused0, unused1, x, y);
}

// Opens the event's current message 32 pixels above or below the screen
// point (x, y), kept on screen, waits until it has been read and moves on to
// the next message.
void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y) {
  struct EventRuntime *state = (struct EventRuntime *)gEventWork;
  s32 py = y;
  s32 px = x;
  s32 minX = 8;
  s32 minY = 20;
  s32 window;

  if (py > 119)
    py += 32;
  else
    py -= 32;

  if (x < minX)
    px = minX;
  if (px > 312)
    px = 312;
  if (py < minY)
    py = minY;
  if (py > 220)
    py = 220;

  window = UiText_OpenMessageWindowFar(state->message, px, py, 1);
  while (UiWork_IsIdleFar((struct UiWindow *)window) == 0)
    WaitFrames(1);
  state->message++;
}

// Closes the portrait of an actor's sprite, if it has one.
void ObjectTable_RunIfActive(s32 actor) {
  s32 sprite;

  sprite = ObjectTable_ReadActiveValue(actor);
  if (sprite != -1)
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
}

// The event command "show two messages and wait": two actors speak at once,
// each message at its own (x, y) with its portrait at (portraitX, portraitY),
// all in tiles. Waits for A, B, L or R, then closes both.
void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 firstX, s32 firstY,
                                      s32 firstPortraitX, s32 firstPortraitY,
                                      s32 second, s32 secondX, s32 secondY,
                                      s32 secondPortraitX,
                                      s32 secondPortraitY) {
  struct EventRuntime *rt = (struct EventRuntime *)gEventWork;
  s32 firstSprite = ObjectTable_ReadActiveValue(first);
  s32 secondSprite = ObjectTable_ReadActiveValue(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow = UiText_OpenMessageWindowFar(
      rt->message++, firstX, firstY, BattleFx_GetResourceId(firstSprite) << 16);
  UiWindow_CreateWithSideObjectFar(firstSprite, 0, firstPortraitX,
                                   firstPortraitY);

  secondWindow =
      UiText_OpenMessageWindowFar(rt->message++, secondX, secondY,
                                  BattleFx_GetResourceId(secondSprite) << 16);
  UiWindow_CreateWithSideObjectFar(secondSprite, 0, secondPortraitX,
                                   secondPortraitY);

  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);

  WaitFrames(1);
  while ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
    WaitFrames(1);

  WaitFrames(1);
  UiWork_FinalizeEntityMatchingLocalizedIdFar(firstSprite);
  UiWork_FinalizeEntityMatchingLocalizedIdFar(secondSprite);
  UiWork_FinalizePendingCoreFar();
  WaitFrames(1);

  while (UiWork_IsIdleFar((struct UiWindow *)firstWindow) == 0)
    WaitFrames(1);
  while (UiWork_IsIdleFar((struct UiWindow *)secondWindow) == 0)
    WaitFrames(1);

  WaitFrames(1);
}

// Sets the voice of the messages that follow to that of actor's sprite, at
// the rate the message speed option picks (gUi_RenderResultValues). With
// 0x80000000 the messages are silent.
void Ui_SetRenderResultFromObject(s32 actor) {
  struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
  u8 rate;
  s32 voice;

  if (actor == 0x80000000) {
    *(s16 *)&state->result[0] = (rate = 0);
    *(s16 *)&state->result[1] = rate;
  } else {
    voice = BattleFx_GetResourceId(ObjectTable_ReadActiveValue(actor));
    rate = gUi_RenderResultValues[gGameState.messageSpeed];
    *(s16 *)&state->result[0] = voice;
    *(s16 *)&state->result[1] = rate;
  }
}

// The event command "camera follow actor": the camera (the event's view
// centre) follows actor. Unless keepPosition is set it jumps there at once
// and, off the world map, the map is redrawn around it.
void Object_AttachWorkTargetToObject(s32 actor, s32 keepPosition) {
  struct ObjectRuntime *obj;
  struct ObjectRuntime *camera;
  struct EventWork *work;
  struct MapScrollWork *map;

  obj = ObjectTable_Get(actor);
  work = Runtime_AllocateBlock(0x1B, 0xCCC);
  camera = (struct ObjectRuntime *)work->viewCenter;
  map = gMapWork[0];
  if (obj != 0) {
    map->origin = &camera->x;
    ObjectDispatch_InitFromTable4WithArgumentFar(camera, obj);
    if (keepPosition == 0) {
      camera->x = obj->x;
      camera->y = obj->y;
      camera->z = obj->z;
      WaitFrames(1);
      if (((struct EventRuntime *)work)->mode19e != 3) {
        Map_ApplyWorkOriginAndSpanFar();
      }
    }
  }
}

// The event command "camera set speed": the camera's top speed and
// acceleration, 16.16 pixels per frame.
void ObjectTable_AllocateAndSetObjectSpeed(s32 speedLimit, s32 acceleration) {
  struct EventWork *owner = Runtime_AllocateBlock(0x1b, 0xccc);
  ((struct ObjectRuntime *)owner->viewCenter)->speedLimit = speedLimit;
  ((struct ObjectRuntime *)owner->viewCenter)->acceleration = acceleration;
}

// The event command "camera move to": sends the camera toward (x, y, z),
// 16.16 pixels (-1 keeps that axis), kept far enough inside the map that the
// screen never shows past its edge. With glide 0 the camera jumps there at
// once; otherwise it travels at its own speed.
void Motion_CamBounds(s32 requestedX, s32 requestedY, s32 requestedZ,
                      s32 glide) {
  s32 shouldGlide;
  struct EventWork *runtimeBlock;
  s32 minimumX;
  s32 minimumZ;
  s32 maximumX;
  s32 cameraY;
  s32 maximumZ;
  s32 positionX;
  s32 positionZ;
  s32 positionY;
  struct MapScrollWork *map;
  struct ObjectRuntime *camera;

  positionX = requestedX;
  positionY = requestedY;
  shouldGlide = glide;
  positionZ = requestedZ;
  runtimeBlock = Runtime_AllocateBlock(0x1B, 0xCCC);
  camera = (struct ObjectRuntime *)runtimeBlock->viewCenter;
  map = gMapWork[0];

  // Half a screen (120 by 64 to 96 pixels) inside the map's bounds.
  minimumX = map->minX + 0x780000;
  cameraY = camera->y;
  minimumZ = map->minY + cameraY + 0x600000;
  maximumX = map->maxX + 0xFF880000;
  maximumZ = map->maxY + cameraY + 0xFFC00000;
  map->origin = &camera->x;
  ObjectDispatch_InitFromTable6Far(camera);
  if (positionX == -1) {
    positionX = camera->x;
  }
  if (positionY == -1) {
    positionY = camera->y;
  }
  if (positionZ == -1) {
    positionZ = camera->z;
  }
  if (positionX < minimumX) {
    positionX = minimumX;
  }
  if (positionZ < minimumZ) {
    positionZ = minimumZ;
  }
  if (positionX > maximumX) {
    positionX = maximumX;
  }
  if (positionZ > maximumZ) {
    positionZ = maximumZ;
  }
  if (shouldGlide == 0) {
    camera->x = positionX;
    camera->y = positionY;
    camera->z = positionZ;
    WaitFrames(1U);
    if (((struct EventRuntime *)runtimeBlock)->mode19e != 3) {
      Map_ApplyWorkOriginAndSpanFar();
    }
  } else {
    Object_SetMoveTargetFar(camera, positionX, positionY, positionZ);
  }
}

// The event command "camera move to actor": Motion_CamBounds to the actor's
// x and z.
void Object_PlaceCurrentWithinCameraBounds(s32 actor, s32 glide) {
  struct ObjectRuntime *obj;

  obj = ObjectTable_Get(actor);
  Runtime_AllocateBlock(0x1B, 0xCCC);
  if (obj != NULL) {
    Motion_CamBounds(obj->x, -1, obj->z, glide);
  }
}

// The event command "camera wait for move": waits (at most 600 frames) for
// the camera to arrive, then 2 frames more.
void BattleFx_CommitObjectPositionAndWait(void) {
  Script_WaitForEventTimeoutFar(
      ((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->viewCenter);
  Battle_WaitMode0(2);
}

// The event command "get view centre": returns the camera object.
struct FieldActor *Battle_GetWorkObject1e0(void) {
  return (s32)((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))
      ->viewCenter;
}

// Makes the camera follow target; unless keepCurrentPosition is set it jumps
// there at once.
void BattleFx_LinkObjectToTarget(struct ObjectRuntime *target,
                                 s32 keepCurrentPosition) {
  struct ObjectRuntime *camera;

  camera = (struct ObjectRuntime *)((struct EventWork *)Runtime_AllocateBlock(
                                        0x1B, 0xCCC))
               ->viewCenter;
  if (target != NULL) {
    ObjectDispatch_InitFromTable4WithArgumentFar(camera, NULL);
    camera->linkedObject = target;
    if (keepCurrentPosition == 0) {
      camera->x = target->x;
      camera->y = target->y;
      camera->z = target->z;
    }
  }
}

//--------------------------------------------------------------------------
// The map's view bounds and zoom transitions, and objects that ride along
// with an actor.
//--------------------------------------------------------------------------

// The high byte of a sprite's OAM attribute 2 (in a FieldSprite or an
// AnimationObject's first part). Bits 2 and 3 are its priority.
#define SPRITE_ATTR2_HIGH(sprite) (((u8 *)(sprite))[9])

// The event command "set view bounds": the camera keeps the screen inside
// these map coordinates (Motion_CamBounds).
void Map_SetViewBounds(s32 minX, s32 minY, s32 maxX, s32 maxY) {
  struct MapScrollWork *work = gMapWork[0];

  work->minX = minX;
  work->minY = minY;
  work->maxX = maxX;
  work->maxY = maxY;
}

// Scheduler callback: moves the zoom one step from its previous value toward
// its target, and removes itself on the last step. It waits while the camera
// is paused.
s32 BattleFx_StepRatioTransition(void) {
  struct PerspectiveWork *work;
  s16 *frames;
  s32 *from;
  s16 *step;
  s32 zoom;
  s32 delta;

  work = gMapWork[0];
  if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))
          ->viewCenter->paused != 0)
    return;
  frames = (s16 *)&work->zoomTransitionFrames;
  if (*frames == 0)
    return;
  from = &work->previousZoom;
  delta = work->zoom - *from;
  step = &work->zoomTransitionStep;
  (*step)++;
  /* FAKEMATCH: an explicit libgcc call, an ordinary call where the operator
   * makes a libcall block. Plain form measured 2026-10-13: the same sum with
   * the / operator differ 93 (84 instructions instead of 87). */
  zoom = *from + __divsi3(delta * *step, *frames);
  work->distance = Iwram_MulQ16(work->farPlane, zoom);
  gScanlinePitch = work->pitch + 1;
  if (*step == *frames) {
    *frames = 0;
    Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StepRatioTransition);
  }
}

// Starts a zoom to zoom (16.16) over frames frames, on the world map (event
// mode 3) only.
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames) {
  struct PerspectiveWork *work = gMapWork[0];
  s32 inverse;

  if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))
          ->mode != 3)
    return;
  {
    inverse = Iwram_RatioMulQ14(zoom, 0x10000); // 1 / zoom
  }
  work->previousZoom = work->zoom;
  work->zoom = inverse;
  work->zoomTransitionFrames = frames;
  work->zoomTransitionStep = 0;
  Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_StepRatioTransition,
                                0xc94);
}

// Waits for a zoom transition to end (on the world map only, at most 300
// frames).
void Event_WaitForZoomTransition(void) {
  s32 frames;
  struct PerspectiveWork *work = gMapWork[0];

  if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))
          ->mode == 3) {
    frames = 0;
    if (*(s16 *)&work->zoomTransitionFrames != 0) {
      do {
        WaitFrames(1);
        frames++;
      } while (frames < 300 && *(s16 *)&work->zoomTransitionFrames != 0);
    }
  }
}

// Per-frame update of a riding object: keeps it on the actor it rides (its
// linked object), raised by the actor's sprite height (its metadata's boxX) and
// 8 pixels. Its work holds the actor's id.
s32 BattleFx_CopyLinkedObjectPosition(void *self) {
  struct FieldActor *obj = self;
  struct FieldActor *host;

  host = obj->linkedObject;
  if (host != NULL) {
    obj->motionFlags = 0;
    obj->x.fixed = host->x.fixed;
    obj->y.fixed =
        host->y.fixed +
        ((s8)Resource_GetMetadataRecordFar(
             (s16)BattleAction_FindDescriptor(*(s16 *)&obj->work)->sprite)
             ->boxX
         << 16) +
        (8 << 16);
    obj->terrainHeight = host->terrainHeight;
    obj->z.fixed = host->z.fixed;
  }
  return 0;
}

// The event command "actor show emote": shows an emote bubble (object 21)
// over an actor, then waits wait frames. The low 4 bits of emote pick the
// bubble's animation (6 also plays sound 110); with 0x100 the bubble takes
// sprite priority 1, otherwise the actor's.
void BattleFx_SpawnLinked(s32 actor, s32 emote, s32 wait) {
  struct FieldActor *target;

  if ((emote & 0xff) == 6) {
    AudioCommand_PlayFar(110);
  }

  target = ObjectTable_Get(actor);
  if (target != 0) {
    struct FieldActor *bubble =
        Object_CreateFar(21, target->x.fixed, target->y.fixed, target->z.fixed);

    if (bubble != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)bubble,
                                   gBattleFx_LinkedObjectScript);
      ObjectDispatch_ApplyArgumentToChildrenFar(bubble, emote & 15);
      bubble->motionFlags = 0;
      bubble->timer = 0;
      bubble->work = actor;
      bubble->update =
          (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
      bubble->sprite->flags = 0;
      bubble->linkedObject = target;

      // Replace the bubble's priority bits (2 and 3) with 1 or the actor's.
      if ((emote & 0x100) != 0) {
        s32 mask = 13;
        u8 attr = SPRITE_ATTR2_HIGH(bubble->sprite);

        mask = -mask;
        mask &= attr;
        mask |= 4;
        SPRITE_ATTR2_HIGH(bubble->sprite) = mask;
      } else {
        s32 priority = 12;
        u8 src = SPRITE_ATTR2_HIGH(target->sprite);
        u8 dst;
        s32 mask = 13;

        priority &= src;
        dst = SPRITE_ATTR2_HIGH(bubble->sprite);
        mask = -mask;
        mask &= dst;
        mask |= priority;
        SPRITE_ATTR2_HIGH(bubble->sprite) = mask;
      }
    }
    Battle_WaitMode0(wait);
  }
}

// The event command "actor set attached effect": with mode (the low 2 bits)
// 0 removes the object attached to an actor; 1 attaches one (object 209,
// animation 1) that stays until removed; 2 shows a one-off one (animation 2)
// that ends by itself. With 0x100 the object takes sprite priority 1,
// otherwise the actor's.
void BattleFx_ConfigureLinkedObject(s32 actor, s32 flags) {
  struct FieldActor *object = ObjectTable_Get(actor);
  struct FieldActor *effect;
  struct FieldSprite *visual;
  s32 mode;

  effect = 0;
  visual = 0;

  if (object == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == 2 || object->linkedObject == 0) {
      effect = Object_CreateFar(209, object->x.fixed, object->y.fixed,
                                object->z.fixed);
    }
  } else {
    effect = object->linkedObject;
    if (effect == 0)
      return;
    ObjectDispatch_ReleaseFar(effect);
    object->linkedObject = 0;
    return;
  }

  if (effect == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case 1:
    ObjectDispatch_ApplyArgumentToChildrenFar(effect, 1);
    object->linkedObject = effect;
    effect->timer = 1;
    break;
  case 2:
    ObjectDispatch_ApplyArgumentToChildrenFar(effect, 2);
    ObjectDispatch_InitializeFar((struct DispatchObject *)effect,
                                 (u32)gObjectMotion_WaitScript);
    effect->timer = 1;
    break;
  }

  effect->work = actor;
  effect->motionFlags = 0;
  effect->update = BattleFx_CopyLinkedObjectPosition;
  visual = effect->sprite;
  visual->flags = 0;
  effect->linkedObject = object;

  // Replace the effect's priority bits (2 and 3) with 1 or the actor's.
  if (flags & 0x100) {
    s32 mask = 13;
    u8 attr = SPRITE_ATTR2_HIGH(visual);

    mask = -mask;
    mask &= attr;
    mask |= 4;
    SPRITE_ATTR2_HIGH(visual) = mask;
  } else {
    s32 priority = 12;
    u8 src = SPRITE_ATTR2_HIGH(object->sprite);
    u8 dst;
    s32 mask = 13;

    priority &= src;
    dst = SPRITE_ATTR2_HIGH(visual);
    mask = -mask;
    mask &= dst;
    mask |= priority;
    SPRITE_ATTR2_HIGH(visual) = mask;
  }
}

// Script step: clears an object's sprite flags and its collision flags.
s32 Object_ClearChildAndCollisionFlags(void *obj) {
  ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
  ((struct ScriptObjectRuntime *)obj)->flags59 = 0;
  return 0;
}

// Script step of action 5: follows the linked object. While at least action
// pixels away the object walks toward it (a step of 16 / action of the
// distance, in 16-pixel units); closer, it stands.
s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object) {
  s32 step;
  struct ObjectRuntime *target;
  s32 deltaX;
  s32 deltaZ;
  s32 pixelsX;
  s32 pixelsZ;
  s32 newX;
  s32 newZ;
  s32 distance;

  target = object->linkedObject;
  if (target != 0) {
    deltaX = target->x - object->x;
    if (deltaX < 0)
      deltaX += 0xffff;
    pixelsX = deltaX >> 16;
    deltaZ = target->z - object->z;
    if (deltaZ < 0)
      deltaZ += 0xffff;
    pixelsZ = deltaZ >> 16;
    distance = Iwram_Sqrt(pixelsX * pixelsX + pixelsZ * pixelsZ);
    step = object->action;
    if (distance >= step) {
      newX = object->x + (pixelsX << 20) / step;
      newZ = object->z + (pixelsZ << 20) / step;
      Object_SetMoveTargetFar(object, newX, object->y, newZ);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    } else {
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    }
  }
  return 1;
}

// Script step of action 6: turns the object toward its linked object, at most
// 1/16 turn a frame.
s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object) {
  s16 turnStep;
  u16 currentAngle;
  s32 targetAngle;
  struct ObjectRuntime *target;

  target = object->linkedObject;
  if (target != NULL) {
    object->actionFlags &= 0xfe;
    targetAngle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
    currentAngle = object->angle;
    turnStep = targetAngle - currentAngle;
    if (turnStep != 0) {
      if (turnStep > 0x1000)
        turnStep = 0x1000;
      if (turnStep < -0x1000)
        turnStep = -0x1000;
      object->angle = (currentAngle + turnStep);
    }
  }
  return 1;
}

// Starts an actor's action script: 1 to 4 the fixed action scripts, 5 follow
// the linked object, 6 face the leader, 7 reset. Any other value is itself
// the script to run.
void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 action) {
  switch ((action - 1)) {
  case 0:
    action = (s32)gObjectMotion_ActionKind1Script;
    break;
  case 1:
    action = (s32)gObjectMotion_ActionKind2Script;
    break;
  case 2:
    action = (s32)gObjectMotion_ActionKind3Script;
    break;
  case 3:
    action = (s32)gObjectMotion_ActionKind4Script;
    break;
  case 4:
    action = (s32)gObjectMotion_MoveTowardTargetScript;
    break;
  case 5:
    object->linkedObject = Object_GetById(gGameState.selectedActor);
    action = (s32)gObjectMotion_TurnTowardLinkedScript;
    break;
  case 6:
    action = (s32)gObjectMotion_ResetActionScript;
    break;
  default:
    break;
  }
  ObjectDispatch_InitializeFar((struct DispatchObject *)object, action);
}

// Finds the nearest animated object to self within 40 pixels and 48 pixels of
// height, ignoring objects more than 3/16 turn to either side once they are
// over 23 pixels away. Returns it if it shows sprite, otherwise NULL.
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self,
                                                     s32 sprite) {
  struct ObjectRuntime *entry;
  struct ObjectRuntime *found;
  s32 cnt;
  s32 best;
  s32 dy;
  s32 dx;
  s32 dz;
  s32 dist;
  s32 angle;
  s32 turn;

  found = NULL;
  best = 40;
  entry = gObjectSlots;
  for (cnt = 0; cnt < 64; cnt++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->animationKind != 1)
      continue;
    dy = entry->y - self->y;
    if (dy >= 0) {
      if (dy > 0x2fffff)
        continue;
    } else {
      if (self->y - entry->y > 0x2fffff)
        continue;
    }
    dx = (entry->x - self->x) / 0x10000;
    dz = (entry->z - self->z) / 0x10000;
    dist = Iwram_Sqrt(dx * dx + dz * dz);
    if (dist >= best)
      continue;
    angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
    if (dist > 23) {
      turn = (s16)(angle - self->angle);
      if (turn < -0x2fff)
        continue;
      if (turn > 0x2fff)
        continue;
    }
    found = entry;
    best = dist;
  }
  if (found == NULL)
    return NULL;
  if (((struct AnimationObject *)found->animation)->entries[0]->animId !=
      sprite)
    return NULL;
  return found;
}

// The map's cells (128 to a row), and the same cells seen one row south and
// one row north: TILE_CELLS_SOUTH[i] is the cell below TILE_CELLS[i].
#define TILE_CELLS ((struct MapCell *)Ram_MapCellBuffer)
#define TILE_CELLS_SOUTH ((struct MapCell *)(Ram_MapCellBuffer + 0x200))
#define TILE_CELLS_NORTH ((struct MapCell *)(Ram_MapCellBuffer - 0x200))

// The event command "leader check ahead": makes the leader jump two tiles
// forward, over the tile in front, when the map allows it
// (Map_CheckObjectStep). On a platform object (sprite 207 or 205) the platform
// plays its animation 7 and the leader bobs down 2 pixels and back. Each jump
// counts toward random encounters, and the leader keeps jumping while it lands
// on another jump spot (collision 0xf9). Returns 0 after a jump, -1 when the
// leader cannot jump.
s32 Field_TryJumpForward(void) {
  struct ObjectRuntime *leader = Object_GetById(gGameState.selectedActor);
  s32 result = -1;
  s32 angle = (leader->angle + 0x2000) & 0xc000; // to the nearest quarter
  u8 *flags = &leader->flags;
  u8 saved = *flags;
  struct FieldStepWork *work = gEventWork;
  s32 spriteFlags = 1;
  struct FieldPosition position;
  struct FieldPosition *pos;
  struct ObjectRuntime *platform;

  pos = &position;
again:
  // The middle of the tile in front, then of the tile beyond.
  pos->x = (leader->x & 0xfff00000) + 0x80000;
  pos->y = leader->y;
  pos->z = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x100000, angle, pos);
  if (Map_CheckObjectStepFar(leader, pos) == 1)
    return -1;
  pos->x = (leader->x & 0xfff00000) + 0x80000;
  pos->y = leader->y;
  pos->z = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x200000, angle, pos);
  if (Map_CheckObjectStepFar(leader, pos) == 0) {
    // Crouch, then jump there.
    if (leader->animationKind == 1)
      spriteFlags = ((struct FieldSprite *)leader->animation)->flags;
    Battle_Reset();
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
    WaitFrames(6);
    AudioCommand_PlayFar(152);
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildFlagsFar(leader, spriteFlags & 0xfe);
    ObjectMotion_SetPositionAndCommit(
        gGameState.selectedActor,
        ((union FieldCoordinate *)&pos->x)->part.pixel,
        ((union FieldCoordinate *)&pos->z)->part.pixel);
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
    ObjectDispatch_SetSingleChildFlagsFar(leader, spriteFlags);

    // A platform underfoot dips 2 pixels with the leader and comes back up.
    if ((platform = Object_FindNearestFacingTarget(leader, 207)) != NULL ||
        (platform = Object_FindNearestFacingTarget(leader, 205)) != NULL) {
      ObjectDispatch_ApplyArgumentToChildrenFar(platform, 7);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      WaitFrames(2);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      WaitFrames(10);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
      WaitFrames(4);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
    } else {
      WaitFrames(6);
    }
    *flags = saved;
    BattleFx_FinishAction();
    if (work != NULL)
      work->encounterSteps += Iwram_MulQ16(work->encounterRate, 0x200000);
    if (GetMapCellCollisionFar(leader->terrainId, pos->x, pos->z) == 0xf9) {
      ObjectDispatch_ApplyArgumentToChildrenFar(leader, 1);
      WaitFrames(6);
      goto again;
    }
    result = 0;
  }
  return result;
}

// Climbing down (the player presses down at a climbable wall). On the ground,
// when the cell south of the leader's is the same climbable cell, the leader
// turns round, grabs the wall and climbs down onto it (climbing pose 13). While
// climbing, the leader lets go and drops to the ground. Returns 0, or -1 when
// there is nothing to climb or the way is blocked.
s32 FieldEffect_UpdateGridPlacement(void) {
  struct ObjectRuntime *object = Object_GetById(gGameState.selectedActor);
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  Battle_Reset();

  if (gGameState.movementMode == 0) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    if (TILE_CELLS[index].collisionCode ==
        TILE_CELLS_SOUTH[index].collisionCode) {
      /* FAKEMATCH: frame local: the six-word probe keeps the native 24-byte
       * frame though only its first three words are the position. Plain forms
       * measured 2026-10-13: one struct FieldPosition or s32[3] differ 4 (a
       * 12-byte frame); two FieldPositions match as well. */
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      position->x = object->x;
      position->y = object->y - 0x100000;
      position->z = object->z;
      result = CheckMapPositionCellOccupiedFar((const s32 *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the tile, face north and climb down a tile.
      ObjectMotion_SetPositionAndCommit(gGameState.selectedActor, gridX, gridZ);
      object->speedLimit = 0x10000;
      ObjectMotion_ArmCallback(gGameState.selectedActor, 0xc000, 0);
      Object_RefreshSelectorById(gGameState.selectedActor);
      object->actionFlags = 1;
      object->flags = 0;
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 13);
      Object_SetMoveTargetFar(object, gridX << 16, object->y - 0x100000,
                              (gridZ << 16) + 0x100000);
      ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selectedActor);
      gGameState.movementMode = 1;
    } else {
      goto failure;
    }
  } else {
    // Let go of the wall and fall.
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 10);
    object->flags = 3;
    object->velocityY = 0x40000;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar(object, 1);
    Battle_WaitMode0(6);
    gGameState.movementMode = 0;
    object->actionFlags = 1;
    object->angle = 0xc000;
  }

  BattleFx_FinishAction();
  return 0;

failure:
  BattleFx_FinishAction();
  return -1;
}

// Climbing up (the player presses up at a climbable wall). On the ground, when
// the cell north of the leader's is the same climbable cell, the leader jumps
// onto the wall and holds on (climbing pose 12). While climbing, the leader
// climbs over the top onto the ground. Returns 0, or -1 when there is nothing
// to climb or the way is blocked.
s32 battle_owner_69(void) {
  struct ObjectRuntime *object = Object_GetById(gGameState.selectedActor);
  s16 spriteFlags = 1;
  s32 gridX =
      8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
  s32 gridZ =
      8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
  s32 result;

  Battle_Reset();

  if (object->animationKind == 1)
    spriteFlags = ((struct FieldSprite *)object->animation)->flags;

  if (gGameState.movementMode == 0) {
    s32 index = gridX / 16 + (gridZ / 16) * 128;

    if (TILE_CELLS[index].collisionCode ==
        TILE_CELLS_NORTH[index].collisionCode) {
      /* FAKEMATCH: frame local: the six-word probe keeps the native 24-byte
       * frame though only its first three words are the position. Plain forms
       * measured 2026-10-13: one struct FieldPosition or s32[3] differ 4 (a
       * 12-byte frame); two FieldPositions match as well. */
      s32 vector[6];
      struct FieldPosition *position = (struct FieldPosition *)vector;

      position->x = object->x;
      position->y = object->y;
      position->z = object->z;
      result = CheckMapPositionCellOccupiedFar((const s32 *)position);
      if (result != 0)
        goto failure;

      // Step to the middle of the tile, crouch, jump and grab the wall.
      object->actionFlags = 0;
      ObjectMotion_SetPositionAndCommit(gGameState.selectedActor, gridX, gridZ);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
      WaitFrames(4);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
      object->velocityY = 0x40000;
      WaitFrames(4);
      object->flags = 0;
      spriteFlags &= 0xfe;
      ObjectDispatch_SetSingleChildFlagsFar(object, spriteFlags);
      object->speedLimit = 0x10000;
      object->velocityY = 0;
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 12);
      WaitFrames(4);
      gGameState.movementMode = 1;
      object->actionFlags = 1;
      WaitFrames(8);
    } else {
      goto failure;
    }
  } else {
    // Climb over the top: up 8 pixels and a tile north.
    object->flags = 0;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 11);
    Object_SetMoveTargetFar(object, gridX << 16, object->y + 0x80000,
                            (gridZ << 16) - 0x100000);
    ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selectedActor);
    object->flags = 3;
    spriteFlags |= 1;
    object->terrainHeight = object->y;
    ObjectDispatch_SetSingleChildFlagsFar(object, spriteFlags);
    Battle_WaitMode0(4);
    gGameState.movementMode = 0;
    object->actionFlags = 1;
  }

  BattleFx_FinishAction();
  return 0;

failure:
  BattleFx_FinishAction();
  return -1;
}

// Finds where an actor is on screen: position[0] and position[1] receive its
// x and y in pixels, y at the top of its sprite. Returns 0, or -1 if there is
// no such actor.
s32 Object_GetScreenPosition(s32 objectId, s32 *position) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  s32 *camera;
  s32 cameraX;
  s32 cameraZ;
  s32 x;
  s32 z;

  if (object == 0)
    return -1;
  /* FAKEMATCH: the view position read as two adjacent words through one
   * pointer, so both loads share one base. Plain forms measured 2026-10-13:
   * viewX and viewY read through gMapWork[0], or through a struct MapScrollWork
   * pointer, differ 28 (61 instructions instead of 59). */
  camera = &((struct MapScrollWork *)gMapWork[0])->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / 0x10000;
  *position = z / 0x10000;
  if ((object->animationKind & 15) == 1)
    *position -=
        (s8)Resource_GetMetadataRecordFar(
            ((struct AnimationObject *)object->animation)->entries[0]->animId)
            ->boxX;
  return 0;
}

// Does nothing.
void ObjectEffect_ReservedNoOp941DC(void) {}

// Starts the transition into a random battle: plays the scene's battle cue
// and the encounter sounds, closes the screen and flashes the backdrop white,
// fading it to black over 16 frames.
s32 Scene_FadeColorFromWhite(s32 unused) {
  struct EventRuntime *work = gEventWork;
  s32 i;
  s32 level;

  AudioCommand_PlayFar(gGameState.sceneCue);
  AudioCommand_PlayFar(288);
  AudioCommand_PlayFar(147);
  if (work->mode19e == 3) {
    // On the world map the flash colours BG palette colour 243 instead.
    Io_Write16(0x7fff, (u16 *)0x050001e6);
    DisplayTransition_Finish(0x401, 16);
    work->status1c6 = 0;
    WaitFrames(16);
    for (i = 0; i < 16; i++) {
      level = 30 - i * 2;
      Io_Write16((level << 10) | (level << 5) | level, (u16 *)0x050001e6);
      WaitFrames(1);
    }
  } else {
    Io_Write16(0x7fff, &PLTT_BACKDROP);
    DisplayTransition_Finish(0x207, 16);
    work->status1c6 = 0;
    WaitFrames(16);
    for (i = 0; i < 16; i++) {
      level = 30 - i * 2;
      Io_Write16((level << 10) | (level << 5) | level, &PLTT_BACKDROP);
      WaitFrames(1);
    }
  }
}

// Starts the leader's animation for leaving through an exit: centres it on
// its tile, at the tile's south edge, stops it and plays animation over 18
// frames. The leader's sprite gains a second part (metadata 27) for it.
void ObjectEffect_PrepareContextEffect(s32 animation) {
  u32 zero;
  u8 param;
  struct ObjectRuntime *object;
  struct AnimationObject *context;
  struct AnimationEntry *effect;

  object = ObjectTable_Get(gGameState.selectedActor);
  context = object->animation;
  effect = (struct AnimationEntry *)ResourceMetadata_RegisterFar(context, 27);
  zero = 0;
  param = 15;

  context->flags = zero;
  effect->param = param;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = ACTOR_NO_TARGET;
  object->targetZ = ACTOR_NO_TARGET;
  ObjectDispatch_ApplyArgumentToChildrenFar(object, animation);
  WaitFrames(18);
}

// Leaves through an exit of kind 0x20 (animation 26); flag 0x120 makes the
// next scene play the matching arrival (ObjectEffect_RunPendingFlagEvent).
void ObjectEffect_BeginContextEffect26(void) {
  ObjectEffect_PrepareContextEffect(0x1A);
  GameFlag_SetBitFar(0x120);
}

// Leaves through an exit of kind 0x30 (animation 25), recorded in flag 0x121.
void ObjectEffect_BeginContextEffect25(void) {
  ObjectEffect_PrepareContextEffect(0x19);
  GameFlag_SetBitFar(0x121);
}

// Plays the leader's arrival animation through an exit: centred on its tile
// at the tile's north edge, the leader plays animation for 30 frames, loses
// the extra sprite part and walks 8 pixels south.
void ObjectEffect_EndContextEffect(s32 animation) {
  s32 zero;
  s32 mask;
  struct ObjectRuntime *obj = ObjectTable_Get(gGameState.selectedActor);
  struct AnimationObject *ctx = obj->animation;
  struct AnimationEntry *eff =
      (struct AnimationEntry *)ResourceMetadata_RegisterFar(ctx, 27);

  zero = 0;
  mask = 0xfff00000;
  ctx->flags = zero;
  eff->param = 15;
  obj->x = (obj->x & mask) + 0x80000;
  obj->z &= mask;
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, animation);
  WaitFrames(30);
  ctx->count = 1;
  ResourceMetadata_ClearRecordFar(ctx->entries[1]);
  ctx->entries[1] = (void *)zero;
  ctx->flags = 1;
  obj->acceleration = 0x10000;
  obj->speedLimit = 0x10000;
  Object_SetMoveTargetFar(obj, obj->x, obj->y, obj->z + 0x80000);
  Script_WaitForEventTimeoutFar(obj);
}

// Plays the leader's arrival for the way the last scene was left: through an
// exit of kind 0x20 (flag 0x120, animation 24, returns 1) or 0x30 (flag
// 0x121, animation 23, returns 2), or falling in from 160 pixels above
// (flag 0x122, returns 3). Returns 0 when there is nothing to play.
s32 ObjectEffect_RunPendingFlagEvent(void) {
  s32 result = 0;
  s32 flag = 0x120;

  if (GameFlag_TestFar(flag) != 0) {
    ObjectEffect_EndContextEffect(24);
    GameFlag_ClearBitFar(flag);
    result = 1;
  } else {
    flag = 0x121;
    if (GameFlag_TestFar(flag) != 0) {
      ObjectEffect_EndContextEffect(23);
      GameFlag_ClearBitFar(flag);
      result = 2;
    } else {
      flag = 0x122;
      if (GameFlag_TestFar(flag) != 0) {
        s32 id;
        struct ObjectRuntime *obj;

        GameFlag_ClearBitFar(flag);
        id = gGameState.selectedActor;
        obj = ObjectTable_Get(id);
        obj->y += 0x00a00000;
        Motion_CamBounds(-1, -1, -1, 0);
        while (obj->y + obj->velocityY > obj->terrainHeight) {
          WaitFrames(1);
        }
        AudioCommand_PlayFar(159);
        obj->y = obj->terrainHeight;
        ObjectDispatch_ApplyArgumentToChildrenFar(obj, 22);
        Battle_WaitMode0(15);
        Object_AttachWorkTargetToObject(id, 1);
        result = 3;
      }
    }
  }
  return result;
}

// Scheduler callback for the wavy background: restarts DMA 0 each frame to
// copy the current page's BG1 to BG3 scroll offsets into the scroll registers
// one scanline at a time. The first line is written directly; the DMA writes
// three words at each HBlank from then on.
void DisplayScroll_ArmHBlankDma(void) {
  volatile u16 *dma;
  struct DisplayScrollWork *state;
  u32 *source;
  u16 status;
  u32 control;
  volatile u32 *destination;

  state = gHBlankScrollWork;
  source = (u32 *)state->rows[state->page];

  dma = REG_DMA0;
  destination = (volatile u32 *)0x04000014; // REG_BG1HOFS
  status = dma[5];
  control = status & (u16)~DMA_START_HBLANK_REPEAT;
  dma[5] = control;
  status = dma[5];
  control = status & (u16)~DMA_ENABLE;
  dma[5] = control;
  (void)dma[5];

  *destination = *source++;
  *destination = *source++;
  *destination = *source++;

  // Enabled, at HBlank, repeating, 32-bit, destination reloaded: 3 words.
  control = 0xa6600003;
  Dma_Set(source, destination, control, (volatile u32 *)dma);
}

// Scheduler callback for the wavy background: fills the hidden page with each
// scanline's BG3, BG2 and BG1 offsets, the scroll plus a sine wave for x and
// for y (an axis with no step stays flat), then shows that page.
void DisplayScroll_BuildAndSwapHBlankPage(void) {
  struct DisplayScrollWork *work = gHBlankScrollWork;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  u16 *row;
  s32 step;
  s32 phase;
  s32 amp;
  s32 i;
  u16 wave;

  row = &work->rows[work->page ^ 1][0][0].x;
  step = work->stepX;
  phase = (work->frame + (u16)y3) * work->freqX;
  if (step == 0) {
    for (i = 0; i != 160; i++) {
      row[i * 6] = x3;
      row[i * 6 + 2] = x2;
      row[i * 6 + 4] = x1;
    }
  } else {
    u16 a = x3;
    u16 b = x2;
    u16 c = x1;

    amp = work->ampX;
    for (i = 0; i != 160; i++) {
      wave =
          Iwram_MulQ16(gDisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
      *row = a + wave;
      row += 2;
      *row = b + wave;
      row += 2;
      *row = c + wave;
      row += 2;
      phase += step;
    }
  }
  row = &work->rows[work->page ^ 1][0][0].y;
  step = work->stepY;
  phase = (work->frame + (u16)y3) * work->freqY;
  if (step == 0) {
    for (i = 0; i != 160; i++) {
      row[i * 6] = y3;
      row[i * 6 + 2] = y2;
      row[i * 6 + 4] = y1;
    }
  } else {
    u16 a = y3;
    u16 b = y2;
    u16 c = y1;

    amp = work->ampY;
    for (i = 0; i != 160; i++) {
      wave =
          Iwram_MulQ16(gDisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
      *row = a + wave;
      row += 2;
      *row = b + wave;
      row += 2;
      *row = c + wave;
      row += 2;
      phase += step;
    }
  }
  work->frame++;
  work->page ^= 1;
}

// The shared source that starts the wavy background, at its place in the ROM.
#include "../scene/common/scroll_initialize_hblank_dma.h"

// Stops the wavy background: removes its two callbacks and stops DMA 0.
void DisplayScroll_DisarmHBlankDma(void) {
  volatile u16 *dma0;

  Scheduler_RemoveCallback(DisplayScroll_ArmHBlankDma);
  Scheduler_RemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
  dma0 = REG_DMA0;
  dma0[5] = (u16)~DMA_START_HBLANK_REPEAT & dma0[5];
  dma0[5] = (u16)~DMA_ENABLE & dma0[5];
  (void)dma0[5];
}

// One small sprite of a field weather effect (ground particles, motes or
// sparkles): its OAM attributes, then its place in the world.
struct GroundParticle {
  u8 unk00[4]; // cleared with the attributes; never read here
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 attr3[2]; // OAM attribute 3, cleared with the others
  s32 posX;    // world position, 16.16 pixels
  s32 posY;    // the ground's height there
  s32 posZ;
  s32 delay; // motes: how far it has risen; sparkles: its place in a burst
  u16 timer; // frames left; 0 frees the particle
};

LAYOUT_OFFSET_GUARD(GroundParticle_PosX, struct GroundParticle, posX, 0x0c);
LAYOUT_SIZE_GUARD(GroundParticle_Size, struct GroundParticle, 0x20);

// Work slot 29 while a field weather effect runs.
struct GroundParticleWork {
  s32 entry;    // the resource entry holding the effect's tiles
  s32 tileBase; // their first tile in VRAM
  struct GroundParticle particles[32];
  s32 unk408;  // never used here
  s32 stopped; // sparkles: no new bursts while set
};

LAYOUT_SIZE_GUARD(GroundParticleWork_Size, struct GroundParticleWork, 0x410);

// Scheduler callback: animates the ground particles. Each plays a 16-frame
// animation (gFieldFx_GroundParticleFrames, 5 halfwords a frame: x and y
// offsets, tile, shape and size) on a random spot of ground within 128 pixels
// of the leader, then starts again elsewhere. They freeze while flag 0x166 is
// set.
void FieldFx_UpdateGroundParticles(void) {
  struct MapScrollWork *view = gMapWork[0];
  struct GroundParticleWork *work = gMapWork[21];
  s32 *camera = &view->viewX;
  s32 cameraX = camera[0];
  s32 cameraZ = camera[1];
  struct GroundParticle *particle = work->particles;
  u32 i;

  for (i = 0; i < 32; i++, particle++) {
    s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_TestFar(0x166))
      particle->timer++;
    frame = &gFieldFx_GroundParticleFrames[5 * particle->timer];
    x = (particle->posX - cameraX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - cameraZ) / 0x10000 + *frame++;
    if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tileBase + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Runtime_PushSlotEntry(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *leader = view->origin;

      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
      particle->timer = 16;
    }
  }
}

// Scheduler callback for the storm: every 150 to 550 frames lightning flashes
// the palette three times (5 frames apart) with a thunderclap, louder with
// loud set. Flag 358 holds the next flash back.
void BattleFx_UpdateStormFlash(void) {
  struct FieldBlendWork *work = gFieldBlendWork[0];
  struct BattleEffectBuffers *target = gFieldBlendWork[2];
  s16 *timer = &work->timer;

  if (*timer < 0)
    return;
  if (GameFlag_TestFar(358))
    work->timer = 128;
  switch ((*timer)--) {
  case 0:
    if (work->enabled != 0) {
      u32 a = Random16();
      u32 b = Random16();
      work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
      if (work->loud != 0)
        AudioCommand_PlayFar(172);
      else
        AudioCommand_PlayFar(171);
    }
    // fall through
  case 5:
  case 10:
    // Flash: jump to the lit palette and fade back over 12 frames.
    BattleFx_ApplyColorToTargetBuffer(work, 1);
    Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    BattleFx_ApplyColorToTargetBuffer(work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}

// Starts the ground particles (FieldFx_UpdateGroundParticles) with sound 170:
// loads their tiles and puts 32 of them under the leader with staggered
// timers, drawn semi-transparent.
void FieldFx_StartGroundParticles(void) {
  struct GroundParticleWork *work;
  s32 *leader;
  struct GroundParticle *particle;
  u32 *words;
  u32 i;

  /* FAKEMATCH: one pointer, words, holds the fill's zero, the decode buffer and
   * each particle's cell cursor, all three in r6 as the native keeps them. A
   * plain fill = 0 takes r3 (differ 4); a cell cursor of its own reallocates
   * the loop and drops a stack slot (differ 58). 2026-10-13: an inline
   * allocate-then-clear helper (Runtime_AllocateBlock, then Dma_Fill32(0,
   * block, size)) differs 22: the fill moves before the sound call. */
  work = Runtime_AllocateBlock(29, sizeof(struct GroundParticleWork));
  leader = ((struct MapScrollWork *)gMapWork[0])->origin;
  BattleFx_SetQueuedSoundAndPlay(170);
  words = 0;
  particle = work->particles;
  Dma_Fill32((u32)words, work, sizeof(struct GroundParticleWork));
  words = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_GroundParticleTiles, words);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramBlock_LoadCached(work->entry, 0x300, words);
  Runtime_ReleaseHeapBlock(14);
  for (i = 0; i < 32; i++) {
    s32 x;
    s32 z;

    // Semi-transparent, 16 by 16, priority 1, palette 13.
    words = (u32 *)particle;
    *words++ = 0;
    *words++ = 0x40000400;
    *words = 0xd400;
    x = leader[0];
    z = leader[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
    particle->timer = (i & 15) + 1;
    particle++;
  }
  Io_Write16(0x3f00, &REG_BLDCNT);
  Io_Write16(0x1008, &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Scheduler_AddOrUpdateCallback(FieldFx_UpdateGroundParticles, 0xc80);
}

// A particle's first word, seen as a value or a link.
union DustWord {
  u32 value;
  void *link;
};

// Scheduler callback: the floating motes. Each mote appears on the ground
// within 128 pixels of the leader (at most 8 new ones a frame), rises a pixel
// a frame, ever faster for its last 90 frames, jittering sideways and
// changing tiles as it ages, and lasts 120 frames or until it leaves the
// screen. They freeze while flag 0x166 is set.
void FieldMotes_Update(void) {
  struct GroundParticleWork *work = WORK_SLOT(29);
  struct MapScrollWork *map = WORK_SLOT(8);
  u32 spawned = 0;
  u32 i;
  struct GroundParticle *mote = work->particles;

  for (i = 0; i < 32; i++, mote++) {
    s32 x;
    s32 y;

    if (--mote->timer != 0xffff) {
      s32 *camera = &map->viewX;
      s32 cameraX = camera[0];
      s32 cameraZ = camera[1];
      u16 age = mote->timer;
      u32 a;
      u32 b;

      if (GameFlag_TestFar(0x166)) {
        mote->timer++;
        mote->delay--;
      }
      a = Random16();
      b = Random16();
      x = ((mote->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1) - 1;
      y = ((mote->posZ - mote->posY - cameraZ) >> 16) - age;
      if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159) {
        if (mote->timer < 60) {
          mote->tile = work->tileBase + 16;
          mote->delay += 3;
        } else if (mote->timer < 90) {
          mote->tile = work->tileBase + 8;
          mote->delay++;
        } else {
          mote->tile = work->tileBase;
        }
        if ((gFrameCount >> 3) & 1)
          mote->tile += 4;
        mote->x = x;
        mote->y = y - (mote->delay >> 2);
        mote->shape = 0;
        mote->size = 1;
        Runtime_PushSlotEntry(mote, 240);
      } else {
        mote->timer = 0;
      }
    }
    if (spawned < 8 && mote->timer == 0) {
      s32 *leader = map->origin;

      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      mote->posZ = y;
      mote->posX = x;
      mote->posY = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
      mote->timer = 120;
      mote->delay = 0;
      spawned++;
    }
  }
}

// Starts the floating motes (FieldMotes_Update): loads their tiles and puts
// 32 of them under the leader with staggered timers, drawn semi-transparent.
void FieldMotes_Start(void) {
  struct GroundParticleWork *work;
  struct GroundParticle *mote;
  u8 *tiles;
  u32 i;
  u32 size;

  size = 0x410;
  work = Runtime_AllocateBlock(29, size);
  mote = work->particles;
  /* FAKEMATCH: reusing the allocation size for the cleared word keeps the zero
   * in r1, the register size was passed in; a plain zero = 0 takes r3 (differ
   * 4). 2026-10-13: an allocate-then-clear helper, as an inline function or a
   * statement macro (Runtime_AllocateBlock, then Dma_Fill32(0, block, size)),
   * differs 10: the zero takes r3 and mote is formed after the fill. */
  size = 0;
  Dma_Fill32(size, work, sizeof(*work));
  tiles = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_MoteTiles, tiles);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramBlock_LoadCached(work->entry, 0x300, tiles);
  Runtime_ReleaseHeapBlock(14);
  for (i = 0; i < 32; i++) {
    s32 *pos = ((struct MapScrollWork *)gMapWork[0])->origin;
    /* FAKEMATCH: reusing the integer field cursor for the height x argument
     * keeps the stores in r1. A plain s32 * cursor with x of its own puts the
     * cursor in r2 and the leader's base in r0 (differ 16). */
    u32 p = (u32)mote;
    s32 z;
    *(s32 *)p = 0;
    p += sizeof(s32);
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = pos[0];
    z = pos[2];
    mote->posX = (s32)p;
    mote->posZ = z;
    mote->posY = Map_GetTerrainHeightFar(0, (s32)p >> 16, z >> 16) << 16;
    mote->timer = (i & 15) + 1;
    mote++;
  }
  Io_Write16(0x3f00, &REG_BLDCNT);
  Io_Write16(0x1008, &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Scheduler_AddOrUpdateCallback((s32)(FieldMotes_Update), 0xc80);
}

// Scheduler callback: the sparkles. Now and then (1 frame in 256) a sparkle
// appears on the ground within 128 pixels of the leader, and up to three more
// follow on the same spot, each 4 frames shorter. Each plays
// gFieldFx_SparkleFrames, flipping every other frame. No new bursts start
// while the work's stopped is set; flag 0x166 freezes them.
void FieldEffect_UpdateSparkles(void) {
  /* FAKEMATCH: needed to match: an unsigned-long frame read keeps size and flip
   * stores together. The plain volatile read differs by 3: it splits the two
   * bit-field stores. */
  struct GroundParticleWork *work;
  u32 spawned;
  u32 i;
  struct MapScrollWork *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct GroundParticle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 sx;
  s32 sy;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  work = WORK_SLOT(29);
  spawned = 0;
  map = WORK_SLOT(8);
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = work->particles;
  delay = 0;
  sx = 0;
  sy = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_TestFar(0x166) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gFieldFx_SparkleFrames[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = ((sparkle->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1);
      sx = x - 1;
      sy = (sparkle->posZ - sparkle->posY - cameraZ) / 0x10000 +
           *(const s16 *)frame;
      frame++;
      if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
        sparkle->tile = work->tileBase + *frame;
        sparkle->x = sx;
        sparkle->shape = 0;
        sparkle->size = 1;
        frames = *(unsigned long *)&gFrameCount;
        sparkle->y = sy;
        sparkle->affineIndex = ((frames >> 1) & 1) << 3;
        Runtime_PushSlotEntry(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        // The rest of a burst, on the first sparkle's spot.
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->posY = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        // A burst starts at a random spot.
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - 0x800000;
        spawnZ = leader[2] + (Random16() << 8) - 0x800000;
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->posY = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i <= 31);
}

// Starts the sparkles (FieldEffect_UpdateSparkles): loads their tiles and
// clears the 32 sparkles.
void FieldEffect_InitSparkles(void) {
  u32 size = 0x410;
  struct GroundParticleWork *work = Runtime_AllocateBlock(29, size);
  struct GroundParticle *p = work->particles;
  u8 *buf;
  u32 i;
  s32 clear;

  /* FAKEMATCH: needed to match: reusing the allocation size for the fill word
   * keeps zero in r1; a separate helper value differs by four instructions.
   * 2026-10-13: an allocate-then-clear helper, as an inline function or a
   * statement macro (Runtime_AllocateBlock, then Dma_Fill32(0, block, size)),
   * differs 10: the zero takes r3 and p is formed after the fill. */
  size = 0;
  Dma_Fill32(size, work, sizeof(*work));
  buf = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_SparkleTiles, buf);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramBlock_LoadCached(work->entry, 0x200, buf);
  Runtime_ReleaseHeapBlock(14);
  i = 0;
  clear = 0;
  do {
    struct MapScrollWork *view = gMapWork[0];
    s32 *leader;
    /* FAKEMATCH: needed to match: an integer field cursor keeps the view load
     * order; reusing it for the height x argument keeps the leader in r2.
     * Separate temporaries differ by 14 instructions. */
    u32 attr;
    s32 z;

    attr = (u32)p;
    *(u32 *)attr = clear;
    attr += sizeof(union DustWord);
    leader = view->origin;
    *(u32 *)attr = 0x40000400;
    attr += sizeof(union DustWord);
    *(u32 *)attr = 0xd400;
    attr = leader[0];
    z = leader[2];
    p->posX = clear;
    p->posZ = clear;
    p->posY = Map_GetTerrainHeightFar(0, (s32)attr >> 16, z >> 16) << 16;
    p->timer = (i & 15) + 1;
    i++;
    p++;
  } while (i < 32);
  Scheduler_AddOrUpdateCallback((s32)(FieldEffect_UpdateSparkles), 0xc80);
}

// Starts the storm's lightning (BattleFx_UpdateStormFlash): builds the dark
// (0x10003) and lit (0x10005) palettes and their 12-step fade, and arms the
// first flash in 600 frames, repeating.
void BattleFx_StartTwelveFrameBlend(void) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;

  work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  buffers = gPaletteBufferWork;
  Dma_Fill32(0, work, sizeof(struct FieldBlendWork));
  BattleFx_BuildBuffer(0x10003, buffers, work, 1);
  BattleFx_BuildBuffer(0x10005, buffers, work->to, 1);
  BattleFx_InterpolateBuffers(work->to, work->from, work->delta, 12);
  BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 600;
  work->enabled = 1;
  Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

// Stops the lightning: no flash for 0x7fff frames, and none after.
void BattleFx_SetBlock30ValuesMaxZero(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 0x7FFF;
  state->enabled = 0;
}

// One last flash of lightning in 12 frames, then no more.
void BattleFx_SetBlock30Values12Zero(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

// Restarts the lightning: the next flash in 128 frames, repeating.
void BattleFx_SetBlock30Values128One(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

// Flashes the palette once, 120 frames from now, from palette from to palette
// to (BattleFx_BuildBuffer modes), with the storm's flash callback.
void BattleFx_StartBufferBlend(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  s16 *target;

  work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  buffers = gPaletteBufferWork;
  Dma_Fill32(0, work, sizeof(struct FieldBlendWork));
  BattleFx_BuildBuffer(from, buffers, work, 1);
  target = work->to;
  BattleFx_BuildBuffer(to, buffers, target, 1);
  BattleFx_InterpolateBuffers(target, work->from, work->delta, 12);
  BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 120;
  work->enabled = 0;
  Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

//--------------------------------------------------------------------------
// Objects that follow the object they are linked to.
//--------------------------------------------------------------------------

// A FieldActor seen with its position as one vector, to pass it whole. The
// gaps are FieldActor fields this view does not use.
struct Object {
  u8 pad0[6];
  u16 angle;
  struct EffectVector pos;
  u8 pad1[14];
  u8 kind;
  u8 pad2[0x2d];
  struct AnimationObject *sprite;
};

// Per-frame update of an afterimage: closes half the distance to the object
// it is linked to, on all three axes.
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *obj) {
  struct ObjectRuntime *target = obj->linkedObject;

  obj->x += (target->x - obj->x) / 2;
  obj->y += (target->y - obj->y) / 2;
  obj->z += (target->z - obj->z) / 2;
}

//--------------------------------------------------------------------------
// A Djinni's hop before its battle, a Djinni joining the party, and putting
// a Djinni's actor back after its battle.
//--------------------------------------------------------------------------

#define CLONE_MAX 8      // afterimages of the leaping Djinni
#define FLAG_DJINNI_0 48 // the flag of Djinni 0, set once it is caught

// A Djinni's battle start: the Djinni (actor id) bounces twice, turns to face
// the leader, hops back 16 pixels three times, then leaps 64 pixels at the
// party trailing eight afterimages tinted for its element.
void EventActor_RunHopAndLeapSequence(s32 id) {
  struct FieldActor *actor;
  struct FieldActor *copy;
  struct FieldActor *prev;
  void *res;
  s32 dir;
  s32 back;
  s32 dist;
  s32 sprite;
  s32 tint;
  s32 i;
  struct FieldActor *copies[CLONE_MAX];

  // The way the leader faces, to the nearest quarter turn.
  actor = Object_GetById(id);
  dir = (Object_GetById(gGameState.selectedActor)->facing + 0x2000) & 0xc000;
  Battle_Reset();
  WaitFrames(10);
  AudioCommand_PlayFar(173);
  Motion_SetVarCbAndRefresh(id, 1);
  AudioCommand_PlayFar(175);
  Motion_SetVarCbAndRefresh(id, 1);
  back = dir + 0x8000;
  WaitFrames(20);
  ObjectMotion_ArmCallback(id, back, 0);
  WaitFrames(10);
  actor->sprite->priority = 0;
  actor->facing = back;
  Object_GetById(id)->steeringFlags &= ~1;
  actor->motionFlags = 2;

  // Hop 16 pixels away from the leader, three times.
  dist = 16 << 16;
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor,
                                             dist, dir);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 4, 0);
  Script_WaitForEventTimeoutFar(actor);
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor,
                                             dist, dir);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 4, 0);
  Script_WaitForEventTimeoutFar(actor);
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor,
                                             dist, dir);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 4, 0);
  Script_WaitForEventTimeoutFar(actor);
  WaitFrames(20);

  // Afterimages of the same sprite, each chasing the one before it, tinted
  // by the Djinni's sprite (90, 91 and 92).
  sprite = ((struct AnimationObject *)actor->sprite)->entries[0]->animId;
  tint = 9;
  if (sprite == 90)
    tint = 2;
  if (sprite == 92)
    tint = 10;
  if (sprite == 91)
    tint = 9;
  res = NULL;
  prev = actor;
  for (i = 0; i < CLONE_MAX; i++) {
    copy = (struct FieldActor *)Object_CreateFar(
        sprite, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->steeringFlags |= 1;
      copy->update = (void *)ObjectMotion_MoveHalfwayTowardTarget;
      copy->facing = actor->facing;
      copy->sprite->priority = 0;
      Animation_ApplyChildValuesFar(copy, tint);
      ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
      ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
      res = Object_ReplaceResourceEntry(copy->sprite, res);
      copy->linkedObject = prev;
      prev = copy;
    }
  }

  // Leap 64 pixels at the leader and stop there.
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor,
                                             64 << 16, dir + 0x8000);
  AudioCommand_PlayFar(136);
  ObjectMotion_Launch(id, 12, 0);
  WaitFrames(24);
  actor->motionFlags = 0;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  actor->velocityY = 0;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  ObjectDispatch_ApplyArgumentToChildrenFar(actor, 0);
  actor->sprite->priority = 2;
  BattleFx_FinishAction();
}

// A Djinni joins the party: djinn (0 to 19) of element goes to the member with
// the fewest Djinn, or to no one and only its flag while FLAG_DJINN_TUTORIAL
// is set. Unless slot is -1, the Djinni's actor slot first plays its
// element's capture effect and is freed. Ends with "<Djinni> joined
// <member>.".
void BattleFx_RunPageEffectForSlot(s32 slot, s32 element, void *djinn) {
  struct FieldWork *work;
  s32 owner;

  work = (struct FieldWork *)gEventWork;
  if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0) {
    owner = 0;
    GameFlag_SetBitFar((u8 *)djinn + element * 20 + FLAG_DJINNI_0);
  } else {
    owner = Djinn_AddToLeastLoadedOwnerFar(element, djinn);
  }

  if (owner >= 0) {
    Battle_Reset();
    Battle_SetObjectFlag5bWhenMode3();
    if (slot != -1) {
      if (work->mode == 3)
        BattleEffect_ClearOutOfBoundsObjects();

      if (element == 0) // Venus
        BattleFx_RunVenusDjinnCapture(slot);
      else if (element == 1) // Mercury
        BattleFx_RunMercuryDjinnCapture(slot);
      else if (element == 2) // Mars
        BattleEffect_RunPhasedRadialParticleSequence(slot);
      else if (element == 3) // Jupiter
        BattleFx_FinishHeavyImpact(slot);

      work->actors[slot] = 0;
      if (work->mode == 3)
        BattleEffect_ClearAllObjects();
    }

    Djinn_ShowJoinedMessageFar(owner, element, djinn);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_FinishAction();
  }
}

// Puts a Djinni's actor back where its battle started from: for a pending
// Djinni battle (pendingDjinnEvent 300 to 380, FLAG_KEEP_PARTY_POSITION set,
// the battle over) the scene actor placed for that Djinni (condition = its
// flag) goes back to its placement, or 32 pixels north of the leader when the
// event has 0x800 set.
void FieldObject_PlaceSceneActors(void) {
  s32 flags = gGameState.pendingDjinnEvent & 0xf000;
  u32 event = gGameState.pendingDjinnEvent & 0xfff;
  s32 id;

  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) != 0 && flags == 0) {
    flags = event & 0x800;
    event &= 0x7ff;
    if (event - DJINNI_0 <= 80 && gGameState.sceneChangeReason > 0) {
      for (id = 8; id <= 65; id++) {
        struct ActionDescriptor *desc = BattleAction_FindDescriptor(id);
        struct ObjectRuntime *obj;

        if (desc == NULL || desc->condition - FLAG_DJINNI_0 != event - DJINNI_0)
          continue;
        obj = ObjectTable_Get(id);
        if (obj == NULL)
          continue;
        if (flags == 0) {
          obj->terrainHeight = flags;
          obj->flags = 3;
          Object_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
        } else {
          struct ObjectRuntime *leader =
              ObjectTable_Get(gGameState.selectedActor);

          Object_SetPositionAndResetMotionFar(obj, leader->x, leader->y,
                                              leader->z - (32 << 16));
        }
        ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
      }
    }
  }
}

// Settles a pending Djinni battle as a scene loads. The field calls it with
// shown 0 while it builds the scene and with shown nonzero once the scene is
// on screen. After a won battle (sceneChangeReason above 0) the second call
// makes the Djinni join the party (BattleFx_RunPageEffectForSlot); otherwise
// the first call sets flag 128 + the Djinni's number and drops the event. An
// event of kind 0x1000 instead sets its flag when sceneChangeReason is 1.
void Djinn_ResolvePendingEvent(s32 shown) {
  struct GameState *st = &gGameState;
  s16 *pending = &st->pendingDjinnEvent;
  s32 kind = *pending & 0xf000;
  s32 flag = *pending & 0xfff;

  if (shown == 0) {
    if (kind == 0) {
      flag &= 0x7ff;
      if (flag < DJINNI_0 || flag > DJINNI_0 + 80)
        return;
      if (st->sceneChangeReason > 0 && st->sceneChangeReason != 999)
        return;
      GameFlag_SetBitFar(flag - 172);
      *pending = kind;
    } else if (kind == 0x1000) {
      if (st->sceneChangeReason == 1)
        GameFlag_SetBitFar(flag);
      *pending = shown;
    }
    return;
  }
  if (kind == 0) {
    flag &= 0x7ff;
    if (flag >= DJINNI_0 && flag <= DJINNI_0 + 80) {
      flag &= 0x7ff;
      if (st->sceneChangeReason > 0) {
        s32 djinni = flag - DJINNI_0;
        s32 element = djinni / 20;
        s32 index = djinni % 20;
        s32 actor;

        for (actor = 8; actor <= 65; actor++) {
          struct ActionDescriptor *found = BattleAction_FindDescriptor(actor);
          if (found != 0 &&
              found->condition - FLAG_DJINNI_0 == flag - DJINNI_0) {
            WaitFrames(40);
            BattleFx_RunPageEffectForSlot(actor, element, (void *)index);
            break;
          }
        }
      }
    }
  }
  gGameState.pendingDjinnEvent = 0;
}

// Runs the update of each of the 24 effect slots.
static __inline__ void UpdateAllEffectSlots(void) {
  struct EffectSlot *slot;
  s32 n;

  slot = ((struct BattleFxScene *)gEffectWork)->slots;
  n = 23;
  do {
    n -= 1;
    EffectSlot_Update(slot);
    slot++;
  } while (n >= 0);
}

// Scheduler callback: updates the 24 effect slots.
void BattleFx_UpdateAllSlots(void) { UpdateAllEffectSlots(); }

// Allocates and clears the effect slots (heap block 56) and starts their
// update callback.
void BattleFx_InitializeSlots(void) {
  void *work;

  work = Runtime_AllocateHeapBlock(56, sizeof(struct BattleFxScene));
  Dma_Fill32(0, work, sizeof(struct BattleFxScene));
  Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllSlots, 3200);
}

// Stops the effect slots: removes their callback, frees every slot still in
// use and releases their heap block.
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct EffectSlot *slot;
  s32 i;

  Scheduler_RemoveCallback(BattleFx_UpdateAllSlots);
  for (i = 0; i < 24; i++) {
    slot = &scene->slots[i];
    if (slot->active != 0)
      BattleFx_ClearOwnedSlot(slot);
  }
  Runtime_ReleaseHeapBlock(56);
  WaitFrames(1);
}

// Per-frame update: spins an object 1/8 turn a frame.
void BattleFx_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}

// Effect slot callback of a Jupiter Djinni's light particles: each circles
// its centre while it widens for 60 frames, circles 40, drifts toward a point
// 20 pixels over the leader for 40, closes in for 60 and is freed.
void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor) {
  struct MotionObject *target;
  struct EffectVector pos;

  target = (struct MotionObject *)Object_GetById(gGameState.selectedActor);
  if (actor->state == 0) {
    actor->orbitRadius += 1;
    actor->orbitAngle += 1;
    if ((s16)actor->age == 60) {
      actor->age = 0;
      actor->state += 1;
    }
  } else if (actor->state == 1) {
    actor->orbitAngle += 1;
    if ((s16)actor->age == 40) {
      actor->age = 0;
      actor->state += 1;
    }
  } else if (actor->state == 2) {
    actor->orbitAngle += 1;
    pos.x = target->x;
    pos.y = target->y + 0x140000;
    pos.z = target->z;
    Camera_WorldToScreen(&pos);
    actor->originX += (pos.x - actor->originX) / 8;
    actor->originZ += (pos.z - actor->originZ) / 8;
    if ((s16)actor->age == 40) {
      actor->age = 0;
      actor->state += 1;
    }
  } else if (actor->state == 3) {
    actor->orbitRadius -= 1;
    actor->orbitAngle += 1;
    if ((s16)actor->age == 60) {
      actor->age = 0;
      actor->state += 1;
    }
  } else if (actor->state == 4) {
    BattleFx_ClearOwnedSlot(actor);
  }
  pos.x = actor->originX;
  pos.z = actor->originZ;
  Vector_AddPolarOffset(actor->orbitRadius << 16, actor->orbitAngle << 11,
                        &pos);
  actor->x = pos.x;
  actor->z = pos.z;
}

// A Jupiter Djinni joins the party: it bounces three times, spins, leaps out
// of sight and bursts into 24 light particles that circle and close in on
// the leader, who turns to the camera and plays animation 28.
void BattleFx_FinishHeavyImpact(s32 djinniId) {
  struct EffectVector pos;
  struct MotionObject *djinni;
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  s32 scale;
  s32 cnt;

  djinni = (struct MotionObject *)Object_GetById(djinniId);
  if (djinni == NULL)
    return;
  BattleFx_InitializeSlots();
  scene = gEffectWork;
  Shop_RestoreSceneTilesFar(0x20118c);
  AudioCommand_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  WaitFrames(20);

  // Spin, then leap high.
  AudioCommand_PlayFar(140);
  djinni->update = BattleFx_AdvanceSpinAngle;
  WaitFrames(40);
  AudioCommand_PlayFar(153);
  ObjectMotion_Launch(djinniId, 12, 22);

  // 24 light particles from where the Djinni is on screen.
  pos.x = djinni->x;
  pos.y = djinni->y;
  pos.z = djinni->z;
  Camera_WorldToScreen(&pos);
  ObjectDispatch_ReleaseFar(djinni);
  AudioCommand_PlayFar(164);
  slot = scene->slots;
  cnt = 23;
  do {
    EffectSlot_Initialize(slot, 284, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_RunAngledApproachPhases);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object,
                                              (Random16() * 7) >> 16);
    scale = Random16() / 3 + 0x10000; // 1.0 to 1.33
    slot->scaleY = scale;
    slot->scaleX = scale;
    cnt--;
    WaitFrames(1);
    slot++;
  } while (cnt >= 0);

  // The leader turns to face south (0x4000) and plays animation 28.
  WaitFrames(60);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  WaitFrames(40);
  AudioCommand_PlayFar(164);
  WaitFrames(100);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

// A FieldActor seen with the halfword at 0x64 unsigned and the one at 0x66
// signed, as the bytes read them (FieldActor's own types change the code).
struct DescendingOrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[80];
  u16 timer;
  s16 angle;
};

// A FieldActor seen with the halfword at 0x5e (lifetime), which FieldActor
// does not name.
struct CaptureObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 baseY;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[8];
  s32 gravity;
  u8 unk2c[4];
  s32 speed;
  u8 unk34[33];
  u8 mode;
  u8 unk56[5];
  u8 visible;
  u8 unk5c[2];
  u16 lifetime;
  u8 unk60[4];
  u16 timer;
  u16 angle;
  struct FieldActor *linkedObject;
  void *callback;
};

// A spinning object: its angle, and at 0x64 the frames its spin has been
// speeding up for (BattleFx_AdvanceAngleWithRamp), read signed.
struct RampedSpinObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x5c];
  s16 rampFrames;
};

// Per-frame update: makes an object pulse, switching its scale between the
// two values of gBattleFx_PulseScales every 4 frames.
void BattleFx_SetObjectAlternatingWords(u8 *object) {
  u32 *table = gBattleFx_PulseScales;
  u32 index = ((u32)gFrameTick >> 2) & 1;
  u32 value = index[table];
  ((struct MotionObject *)object)->scaleX = value;
  ((struct MotionObject *)object)->scaleY = value;
}

// Per-frame update: spins an object ever faster, from 1/16 turn a frame up to
// 1/8.
void BattleFx_AdvanceAngleWithRamp(void *obj) {
  struct RampedSpinObject *spin = obj;
  u32 step;

  step = spin->rampFrames * 0x50;
  spin->angle = spin->angle + step + 0x1000;
  if (step < 0x1000U) {
    spin->rampFrames = spin->rampFrames + 1;
  }
}

// Defines a per-frame update that spins an object 1/8 turn a frame while its
// scale changes by step, releasing the object once the scale falls below
// 0x3000.
#define DEFINE_SHRINK_AND_DESTROY(name, step)                                  \
  void name(void *obj) {                                                       \
    s32 scale;                                                                 \
                                                                               \
    scale = ((struct MotionObject *)obj)->scaleX + step;                       \
    ((struct MotionObject *)obj)->scaleY =                                     \
        ((struct MotionObject *)obj)->scaleY + step;                           \
    ((struct MotionObject *)obj)->angle =                                      \
        ((struct MotionObject *)obj)->angle + 0x2000;                          \
    ((struct MotionObject *)obj)->scaleX = scale;                              \
    if (scale < 0x3000) {                                                      \
      ObjectDispatch_ReleaseFar(obj);                                          \
    }                                                                          \
  }

DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroySlow, 0xFFFFFE40)

// Effect slot callback of a Mars Djinni's sparks. States 0 and 1 repeat: the
// spark flies 120 pixels up the screen (give or take 3/32 turn) from its
// origin and starts over. Set to 2, it stops where it arrives; 40 frames
// later, states 3 and 4 swarm it round a point 20 pixels over the leader.
// Set to 5, it is freed once it arrives.
void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect) {
  struct Object *owner;
  struct EffectVector position;
  s32 state;

  owner = (struct Object *)Object_GetById(gGameState.selectedActor);
  state = effect->state;

  if (state == 0) {
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = effect->x;
    position.z = effect->z;
    Vector_AddPolarOffset(0x780000,
                          ((Random16() * 3 << 11) >> 16) -
                              ((Random16() * 3 << 11) >> 16) + 0xc000,
                          &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x50000;
    effect->maxSpeed = 0x50000;
    effect->flag42 = state;
    effect->state++;

    ((struct AnimationObject *)effect->object)->part[0].priority =
        owner->sprite->part[0].priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(134);
  } else if (state == 1) {
    if ((s16)effect->age == 3) {
      ((struct AnimationObject *)effect->object)->part[0].priority = 0;
      effect->flags = 4;
    }
    if (BattleFx_HasReachedTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (BattleFx_HasReachedTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct AnimationObject *)effect->object)->part[0].priority = 0;
      effect->flags = 4;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == 3) {
    effect->render = 1;
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = owner->pos.x;
    position.y = owner->pos.y + 0x140000;
    position.z = owner->pos.z;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(145);
  } else if (state == 4) {
    if (BattleFx_HasReachedTarget(effect) == 0)
      effect->state--;
  } else if (state == 5) {
    if (BattleFx_HasReachedTarget(effect) == 0)
      BattleFx_ClearOwnedSlot(effect);
  }
}

// A Mars Djinni joins the party: it turns to the camera, bounces three times,
// spins ever faster and shrinks away in a fountain of 24 sparks
// (BattleEffect_UpdatePhasedRadialParticle). The leader turns to the camera
// and plays animation 28, the sparks swarm the leader, who pulses for 100
// frames, and the sparks vanish.
void BattleEffect_RunPhasedRadialParticleSequence(s32 arg) {
  struct EffectVector position;
  s32 value;
  s32 remaining;
  struct FieldActor *sourceObject;
  void *effectSlots;
  struct FieldActor *targetObject;
  struct EffectSlot *slot;
  s8 *slotState;

  sourceObject = Object_GetById(arg);
  targetObject = Object_GetById(gGameState.selectedActor);
  if (sourceObject == NULL)
    return;

  BattleFx_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_RestoreSceneTilesFar(0x201090);
  WaitFrames(30);
  ObjectMotion_ArmCallback(arg, 0x4000, 0);
  WaitFrames(20);
  AudioCommand_PlayFar(173);
  Motion_SetVarCbAndRefresh(arg, 1);
  AudioCommand_PlayFar(174);
  Motion_SetVarCbAndRefresh(arg, 1);
  AudioCommand_PlayFar(175);
  Motion_SetVarCbAndRefresh(arg, 1);
  WaitFrames(20);
  AudioCommand_PlayFar(140);
  sourceObject->update = (void *)BattleFx_AdvanceAngleWithRamp;
  sourceObject->timer = 0;
  WaitFrames(80);
  sourceObject->update = (void *)BattleFx_ShrinkObjectAndDestroySlow;
  ObjectDispatch_ApplyArgumentToChildrenFar(sourceObject, 3);

  // The sparks start where the Djinni is on screen.
  position.x = sourceObject->x.fixed;
  position.y = sourceObject->y.fixed;
  position.z = sourceObject->z.fixed;
  Camera_WorldToScreen(&position);

  slot = ((struct BattleFxScene *)effectSlots)->slots;
  remaining = 23;
  do {
    EffectSlot_Initialize(slot, 284, position.x, position.z);
    EffectSlot_SetCallback(slot,
                           (void *)BattleEffect_UpdatePhasedRadialParticle);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 10);
    value = Random16() / 3 + 0x10000; // 1.0 to 1.33
    slot->scaleY = value;
    slot->scaleX = value;
    remaining--;
    WaitFrames(1);
    slot++;
  } while (remaining >= 0);

  WaitFrames(60);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);

  // Every spark still in use stops its fountain (state 2). The cursor walks
  // the slots' state bytes; a slot's active flag is 5 bytes after its state.
  {
    s32 nextState;

    WaitFrames(20);
    nextState = 2;
    slotState = &((struct BattleFxScene *)effectSlots)->slots[0].state;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct EffectSlot);
    } while (remaining >= 0);
  }

  WaitFrames(60);
  targetObject->update = (void *)BattleFx_SetObjectAlternatingWords;
  WaitFrames(100);

  // Every spark still in use goes (state 5). This loop's cursor starts as
  // the scene plus 152, the first slot's state: written as above, the
  // cursor's two loads swap (differ 22).
  {
    s32 nextState;

    slotState = effectSlots;
    nextState = 5;
    slotState += 152;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct EffectSlot);
    } while (remaining >= 0);
  }

  // The leader stops pulsing. Storing NULL through the cleared slot cursor,
  // as the bytes do: a plain NULL store differs by 9.
  WaitFrames(10);
  slot = NULL;
  targetObject->update = (void *)slot;
  targetObject->scaleX = 0x10000;
  targetObject->scaleY = 0x10000;
  WaitFrames(30);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroyFast, 0xFFFFFC00)

// Per-frame update of a Venus Djinni's falling motes: each circles the
// leader on a shrinking spiral (radius timer * 0.4 pixels) while it sinks a
// pixel a frame, and is released 20 pixels above the leader's feet.
void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg) {
  struct EffectVector local;
  struct DescendingOrbitObject *leader;
  s32 raw;
  s16 value;
  s32 y;

  leader =
      (struct DescendingOrbitObject *)Object_GetById(gGameState.selectedActor);
  raw = arg->timer - 1;
  arg->timer = raw;
  value = arg->timer;
  local.x = leader->x;
  local.z = leader->z;
  Vector_AddPolarOffset(value * 0x6666, (value << 11) + arg->angle, &local);
  arg->x = local.x;
  arg->z = local.z;
  y = arg->y + 0xFFFF0000;
  arg->y = y;
  if (y < leader->y + 0x140000)
    ObjectDispatch_ReleaseFar(arg);
}

// Effect slot callback of a Venus Djinni's particles: each starts on screen
// at a random point 10 to 20 pixels round the leader and flies 120 pixels up
// the screen, over and over (states 0 and 1); set to 2, it is freed once it
// arrives.
void BattleFx_UpdateRadialSpread(struct EffectSlot *effect) {
  struct EffectVector position;
  struct ObjectRuntime *source;
  s32 state;
  u32 random;

  source = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
  state = effect->state;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y;
    position.z = source->z;

    random = Random16() * 10 + 0xa0000;
    Vector_AddPolarOffset(random, Random16(), &position);
    Camera_WorldToScreen(&position);

    effect->originX = position.x;
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset(0x780000, 0xc000, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x10000;
    effect->maxSpeed = 0x50000;
    effect->flag42 = state;
    effect->state++;

    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(0x90);
  } else if (state == 1) {
    if (BattleFx_HasReachedTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (BattleFx_HasReachedTarget(effect) == 0)
      BattleFx_ClearOwnedSlot(effect);
  }
}

// A Venus Djinni joins the party: it hops twice, spins and leaps, then
// shrinks away; the leader turns to the camera and plays animation 28 while
// 24 particles rise round it (BattleFx_UpdateRadialSpread). Then 24 motes
// spiral down onto the leader from 120 pixels above
// (BattleFx_UpdateDescendingOrbitObject) and 8 sparks burst from the leader.
void BattleFx_RunVenusDjinnCapture(s32 arg) {
  struct CaptureObject *leader;
  struct CaptureObject *djinni;
  void *effectSlots;
  u8 *slot;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  s32 position[3];

  leader = (struct CaptureObject *)Object_GetById(gGameState.selectedActor);
  djinni = (struct CaptureObject *)Object_GetById(arg);
  if (djinni == NULL)
    return;

  BattleFx_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_RestoreSceneTilesFar(0x201204);
  WaitFrames(30);
  djinni->visible = 0;
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(arg, 4, 15);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(arg, 4, 15);
  WaitFrames(30);
  djinni->callback = (void *)BattleFx_AdvanceSpinAngle;
  AudioCommand_PlayFar(153);
  ObjectMotion_Launch(arg, 8, 22);
  AudioCommand_PlayFar(140);
  Runtime_SetWorkTripleIfNonNegativeFar(0x14ccc, 0x14ccc, 0x10000);
  djinni->callback = (void *)BattleFx_ShrinkObjectAndDestroyFast;
  ObjectDispatch_ApplyArgumentToChildrenFar(djinni, 3);
  WaitFrames(90);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  WaitFrames(30);
  Runtime_SetWorkTripleIfNonNegativeFar(0x19999, 0x19999, 0x10000);

  position[0] = djinni->x;
  position[1] = djinni->y;
  position[2] = djinni->z;
  Camera_WorldToScreen((struct EffectVector *)position);

  slot = (u8 *)effectSlots + 88;
  remaining = 23;
  do {
    EffectSlot_Initialize((struct EffectSlot *)slot, 284, position[0],
                          position[2]);
    EffectSlot_SetCallback((struct EffectSlot *)slot,
                           (void *)BattleFx_UpdateRadialSpread);
    EffectSlot_SetObjectMode((struct EffectSlot *)slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(*(void **)slot, 11);
    ((struct EffectSlot *)slot)->scaleX = 0x8000;
    ((struct EffectSlot *)slot)->scaleY = Random16() + 0x18000;
    remaining--;
    WaitFrames(1);
    slot += 72;
  } while (remaining >= 0);

  WaitFrames(140);
  {
    s32 nextState;

    nextState = 2;
    slotState = effectSlots;
    slotState += 152;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  WaitFrames(20);
  Runtime_SetWorkTripleIfNonNegativeFar(1, 1, 1);
  WaitFrames(30);

  for (remaining = 0; remaining <= 23; remaining++) {
    position[0] = leader->x;
    position[1] = leader->y + 0x780000;
    position[2] = leader->z;
    djinni = (struct CaptureObject *)Object_Spawn(284, position[0], position[1],
                                                  position[2]);
    if (djinni != NULL) {
      scale = Random16() / 3 + 0x10000;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->timer = 100;
      djinni->angle = (remaining << 16) / 24;
      djinni->callback = (void *)BattleFx_UpdateDescendingOrbitObject;
      djinni->mode = 0;
      ObjectDispatch_ApplyArgumentToChildrenFar(djinni, 7);
      Animation_ApplyChildValuesFar(djinni, 11);
    }
  }

  WaitFrames(100);
  AudioCommand_PlayFar(288);
  WaitFrames(1);
  AudioCommand_PlayFar(151);
  position[0] = leader->x;
  position[1] = leader->y + 0x120000;
  position[2] = leader->z;
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = (struct CaptureObject *)Object_Spawn(284, position[0], position[1],
                                                  position[2]);
    if (djinni == NULL)
      break;
    djinni->scaleY = 0x9999;
    djinni->scaleX = 0x9999;
    djinni->mode = 2;
    djinni->gravity = 0x50000;
    djinni->baseY = djinni->y;
    djinni->speed = Random16() + 0x16666;
    Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)djinni,
                                               0x200000, Random16());
    Animation_ApplyChildValuesFar(djinni, 11);
    djinni->lifetime = 8;
    ObjectDispatch_InitializeFar((struct DispatchObject *)djinni,
                                 gBattleFx_CommonParticleScript);
  }

  WaitFrames(15);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

// Effect slot callback of a Mercury Djinni's droplets: each falls from 100
// pixels above to a random point near the leader (state 0), then darts
// between random points 12 pixels apart (states 1 and 2, a sound each time)
// until the sequence sets state 3, which frees it.
void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect) {
  struct ObjectRuntime *source;
  struct EffectVector position;
  s8 *statePointer;
  s32 state;

  source = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y + Random16() * 5 + 0xf0000;
    position.z = source->z;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(Random16() * 6 + 0x20000, Random16(), &position);

    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->x = position.x;
    effect->z = position.z - 0x640000;
    effect->acceleration = 0x30000;
    effect->maxSpeed = Random16() * 3 + 0x30000;
    effect->scaleX = 0x10000;
    effect->scaleY = 0x10000;
    effect->flag42 = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - 1) <= 1) {
    if (BattleFx_HasReachedTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(0xc0000, Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = 0x10000;
      effect->acceleration = 0;
      effect->maxSpeed = Random16() + 0x23333;
      effect->scaleX = 0x8000;
      effect->scaleY = 0x8000;
      AudioCommand_PlayFar(143);

      if (*statePointer == 1)
        (*statePointer)--;
      else
        (*statePointer)++;
      effect->callbackDelay = 6;
    }
  } else if (state == 3) {
    BattleFx_ClearOwnedSlot(effect);
  }
}

// Per-frame update of an afterimage: closes half the distance to the object
// it is linked to.
void BattleFx_HalveDistanceToTarget(struct FieldActor *object) {
  struct FieldActor *target = object->linkedObject;

  object->x.fixed += (target->x.fixed - object->x.fixed) / 2;
  object->y.fixed += (target->y.fixed - object->y.fixed) / 2;
  object->z.fixed += (target->z.fixed - object->z.fixed) / 2;
}

//--------------------------------------------------------------------------
// A Mercury Djinni joining the party.
//--------------------------------------------------------------------------

#define AFTERIMAGE_MAX 8 // afterimages of the rising Djinni
#define PARTICLE_MAX 24  // droplets (one an effect slot)

// A Mercury Djinni joins the party: the screen is tinted blue, the Djinni
// bounces twice and hops three times ever higher, then rises 120 pixels out
// of sight trailing eight afterimages. The leader turns to the camera and
// plays animation 28 while 24 droplets rain round it, one a frame
// (BattleFx_UpdateRandomTargetParticle); then the colours return.
void BattleFx_RunMercuryDjinnCapture(s32 id) {
  struct FieldActor *djinni;
  struct FieldActor *copy;
  struct FieldActor *previous;
  struct FieldSprite *resource;
  void *scene;
  struct FieldActor **cursor;
  struct FieldActor **list;
  u8 *slot;
  s8 *slotState;
  s32 kind;
  s32 remaining;
  s32 resourceId;
  struct FieldActor *copies[AFTERIMAGE_MAX];
  struct EffectVector position;

  djinni = Object_GetById(id);
  if (djinni == NULL)
    return;

  // Tint the screen blue (red 4, green 4, blue 16).
  BattleFx_InitializeSlots();
  scene = gEffectWork;
  Shop_RestoreSceneTilesFar(PALETTE_TRANSFORM_TINT | (16 << 10) | (4 << 5) | 4);
  WaitFrames(30);
  djinni->paused = 0;
  AudioCommand_PlayFar(173);
  Motion_SetVarCbAndRefresh(id, 1);
  AudioCommand_PlayFar(175);
  Motion_SetVarCbAndRefresh(id, 1);
  WaitFrames(20);

  // Three hops, at 3, 5 and 7 pixels a frame.
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 3, 14);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 5, 16);
  AudioCommand_PlayFar(152);
  ObjectMotion_Launch(id, 7, 18);
  WaitFrames(20);

  // Eight afterimages of the same sprite, each closing half the distance to
  // the one before it every frame.
  kind = ((struct AnimationObject *)djinni->sprite)->entries[0]->animId;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = AFTERIMAGE_MAX - 1; remaining >= 0; remaining--) {
    copy = (struct FieldActor *)Object_CreateFar(
        kind, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->steeringFlags |= 1;
      copy->update = (void *)BattleFx_HalveDistanceToTarget;
      copy->facing = djinni->facing;
      Animation_ApplyChildValuesFar(copy, 9);
      ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
      ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
      resource = Object_ReplaceResourceEntry(copy->sprite, resource);
      copy->linkedObject = previous;
      previous = copy;
    }
  }

  // Rise 8 pixels a frame for 15 frames, then remove the Djinni and its
  // afterimages (and their VRAM block, unless it is the shared block 96).
  resourceId = resource->vramBlock;
  AudioCommand_PlayFar(153);
  djinni->motionFlags = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y.fixed += 8 << 16;
    WaitFrames(1);
  }
  ObjectDispatch_ReleaseFar(djinni);
  list = copies;
  for (remaining = AFTERIMAGE_MAX - 1; remaining >= 0; remaining--)
    ObjectDispatch_ReleaseFar(*list++);
  if (resourceId != 96)
    Resource_ResetEntry(resourceId);
  WaitFrames(10);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  WaitFrames(20);

  // The droplets (object 240), one a frame, from where the Djinni was on
  // screen (BattleFx_UpdateRandomTargetParticle moves them).
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  slot = (u8 *)scene + 88; // scene->slots[0]
  remaining = PARTICLE_MAX - 1;
  do {
    EffectSlot_Initialize((struct EffectSlot *)slot, 240, position.x,
                          position.z);
    EffectSlot_SetCallback((struct EffectSlot *)slot,
                           BattleFx_UpdateRandomTargetParticle);
    EffectSlot_SetObjectMode((struct EffectSlot *)slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(*(void **)slot, 9);
    remaining--;
    WaitFrames(1);
    slot += 72; // sizeof(struct EffectSlot)
  } while (remaining >= 0);

  // After 120 frames every droplet still in use goes on to state 2.
  WaitFrames(120);
  {
    s32 nextState;

    nextState = 2;
    slotState = scene;
    slotState += 152; // scene->slots[0].state
    remaining = PARTICLE_MAX - 1;
    do {
      if (slotState[5] != 0) // active
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  // Restore the colours.
  WaitFrames(50);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

//--------------------------------------------------------------------------
// Running a Psynergy effect on the field, by its kind.
//--------------------------------------------------------------------------
// Each effect passes its objects and positions in a structure of its own,
// so the handlers take void *.

// Runs one step of the current Psynergy effect: 0 casts it with no target,
// 1 casts it on its target (secondObjectId), 2 finishes it and 3 runs its
// kind's extra handler.
void Field_DispatchTypeHandler(s32 kind) {
  if (kind == 0)
    BattleFx_Run();
  else if (kind == 1)
    BattleFx_DispatchRequestKind();
  else if (kind == 2)
    BattleFx_ClearChildValueOnMismatch();
  else if (kind == 3)
    FieldEvent_RunTypeHandler();
}

//--------------------------------------------------------------------------
// Psynergy effects on the field: the handlers for each kind, and the events
// an effect sets off.
//--------------------------------------------------------------------------

#define HEAP_SLOT_FX                                                           \
  56 // the Psynergy effect's work (gWorkSlot[56], gEffectWork)

void _call_via_r3(s32, s32);

// Casts the current Psynergy effect with no target: runs the body of its
// kind (scene->animation).
void BattleFx_Run(void) {
  struct BattleFxScene *scene;
  struct BattleRuntime *runtime;
  s32 kind;
  s32 targetId;
  s32 objId;

  scene = *(gWorkSlot + HEAP_SLOT_FX);
  runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
  kind = scene->animation;
  targetId = scene->secondObjectId;

  switch (kind) {
  case 1:
    BattleFx_RunItemBreakSequence();
    return;
  case 7:
    RunBattleEffect07();
    return;
  case 11:
    RunBattleEffect11();
    return;
  case 4:
    RunBattleEffect04();
    return;
  case 5:
    RunBattleEffect05();
    return;
  case 14:
    RunBattleEffect14();
    return;
  case 6:
    BattleFx_RunOrbitingParticles();
    return;
  case 3:
    RunBattleEffect03();
    return;
  case 12:
    BattleFx_RunBurstParticles();
    return;
  case 13:
    RunBattleEffect13();
    return;
  case 9: // free the object stopped before, then stop the nearest one
    if (gGameState.pausedObject != -1) {
      BattleFx_ResumeObject(gGameState.pausedObject);
      gGameState.pausedObject = -1;
    }

    objId =
        BattleEffect_SelectNearbyTargetObject(gGameState.selectedActor, kind);
    objId = BattleFx_FilterObjectIdByFlags(objId);
    if (BattleFx_FindDescriptorWithOverride(objId) != 0) {
      BattleFx_SetupObjectPair(gGameState.selectedActor, objId);
      BattleFx_MarkChildAndRunFallbackTransition(objId);
      BattleEffect_PauseObject(objId);
      gGameState.pausedObject = objId;
    } else {
      BattleEffect_RunFallbackObjectTransition();
    }
    return;
  case 2: // the wave and colour effect (stopping a running kind 8 first)
    if (*(s16 *)&runtime->unkcb8[0] != 0)
      ResetSceneTransitionEffect();
    RunSceneTransitionEffect(scene->firstObjectId, targetId);
    return;
  case 8:
    RunBattleEffect08();
    return;
  case 10:
    BattleFx_RunFlashingCallbackSequence();
    return;
  case 15:
    BattleFx_RunEffect15();
    return;
  case 16:
    RunBattleEffect16();
    return;
  }
}

// Casts the current Psynergy effect on its target (secondObjectId): runs the
// handler of its kind.
void BattleFx_DispatchRequestKind(void) {
  struct BattleFxScene *scene = *(gWorkSlot + HEAP_SLOT_FX);
  struct BattleRuntime *runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
  s32 kind = scene->animation;
  s32 targetId = scene->secondObjectId;

  scene->enabled = 0;
  switch (kind) {
  case 2:
    if (*(s16 *)&runtime->unkcb8[0] != 0)
      ResetSceneTransitionEffect();
    if (gGameState.pausedObject != scene->secondObjectId)
      ((struct ObjectRuntime *)scene->child)->movementState = 1;
    RunSceneTransitionEffect(scene->firstObjectId, targetId);
    break;
  case 1: // push the target with the direction keys
    BattleFx_SteerPushedTarget(targetId);
    break;
  case 7:
    BattleEffect_RunTargetedItemBreak(targetId);
    break;
  case 11:
    BattleFx_SteerLiftedTarget(targetId);
    break;
  case 4:
    BattleFx_CallEffect04(targetId);
    break;
  case 5:
    BattleFx_CallEffect05(targetId);
    break;
  case 6:
    BattleFx_StartOrbitingParticles(targetId);
    break;
  case 12:
    BattleFx_RunBurstParticleMainObject(targetId);
    break;
  case 9:
    if (gGameState.pausedObject != -1) {
      BattleFx_ResumeObject(gGameState.pausedObject);
      gGameState.pausedObject = -1;
    }
    BattleEffect_PauseObject(targetId);
    gGameState.pausedObject = targetId;
    BattleFx_MarkChildAndRunFallbackTransition(targetId);
    break;
  case 3:
    BattleFx_CallEffect03AndStop(targetId);
    break;
  case 14:
    BattleFx_CallEffect14(targetId);
    break;
  case 13:
    BattleFx_RunEffect13Hook(targetId);
    break;
  case 8:
    RunBattleEffect08();
    break;
  case 10:
    BattleFx_RunFlashingCallbackSequence();
    break;
  case 15:
    BattleFx_RunEffect15();
    break;
  case 16:
    RunBattleEffect16();
    break;
  }
}

// Finishes an effect of kind 2: ends the effect and lets its target move
// again, unless the target is the object kept stopped.
void BattleFx_ClearChildValueOnMismatch(void) {
  struct BattleFxScene *scene = gEffectWork;

  if (scene->animation == 2) {
    BattleFx_FinishSceneAndReleaseHeapBlock();
    if (gGameState.pausedObject != scene->secondObjectId) {
      ((struct ObjectRuntime *)scene->child)->movementState = 0;
    }
  }
}

// The extra handler of effect kinds 8, 10 and 16.
void FieldEvent_RunTypeHandler(void) {
  u32 kind;

  kind = ((struct BattleFxScene *)gEffectWork)->animation;
  switch (kind) {
  case 8:
    ResetSceneTransitionEffect();
    return;
  case 10:
    MapEvent_RunTileTriggerSequence();
    return;
  case 16:
    FieldEvent_ShowStatusMessage();
    return;
  }
}

// Runs the event tied to an effect: a result below 0x10000 is a message,
// shown at target; a larger one is a routine, called with param and target
// (through r3). Afterwards, with flag 322 set, says "But nothing happens.".
// Returns 0.
s32 BattleFx_RunEventAction(void *event, s32 param, s32 target) {
  s32 script;

  if (event != NULL) {
    script = ((struct EffectDescriptor *)event)->result;
    if (script != 0) {
      if (script < 0x10000) {
        Battle_Reset();
        Event_SetMessageId(((struct EffectDescriptor *)event)->result);
        BattleEv_RunWait(target, 0);
        BattleFx_FinishAction();
      } else {
        _call_via_r3(param, target);
      }
    }
    if (GameFlag_TestFar(322) != 0) {
      Battle_Reset();
      UiText_ShowPositionedMessageAndWaitFar(MsgNothingHappens, 1);
      BattleFx_FinishAction();
    }
  }
  return 0;
}

// Per-frame update: sets every part of an object's sprite to param
// frame % 6, which cycles its colours.
void ObjectGroup_ApplyRandomChildValues(void *owner) {
  s32 kind;
  void *anim;
  s32 initialCount;
  s32 cnt;
  void **entry;
  void *part;
  /* FAKEMATCH: volatile, as gFrameCount is declared (frame.h): each *frameCount
   * is a load of its own through the pointer local. A plain pointer differs 16;
   * gFrameCount read directly in the loop differs 6; 46 instructions every way.
   */
  volatile u32 *frameCount;
  s32 value;

  kind = ((struct ObjectRuntime *)owner)->animationKind;
  if (kind == 1) {
    anim = ((struct ObjectRuntime *)owner)->animation;
    if (anim != 0 &&
        (((struct AnimationObject *)anim)->displayFlags & kind) == 0) {
      initialCount = ((struct AnimationObject *)anim)->count;
      if (initialCount != 0) {
        frameCount = &gFrameCount;
        entry = (void **)((struct AnimationObject *)anim)->entries;
        cnt = initialCount;
        do {
          part = *entry++;
          value = *frameCount % 6;
          cnt--;
          ((struct AnimationEntry *)part)->param = value;
        } while (cnt != 0);
      }
      ((struct AnimationObject *)anim)->dirty = 1;
    }
  }
}

// Sends an object toward the point magnitude (16.16 pixels) away from it in
// direction angle.
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle) {
  s32 pos[3];

  if (object != 0) {
    pos[0] = object->x;
    pos[1] = object->y;
    pos[2] = object->z;
    Vector_AddPolarOffset(magnitude, angle, pos);
    Object_SetMoveTargetFar(object, pos[0], pos[1], pos[2]);
  }
}

// From here on objects are passed in each function's own view of them.

// Counts the free blocks (owner 0xff) of the 512 resource blocks.
s32 ResourceTable_CountFreeBlocks(void) {
  u8 *marker = gResourceBlockOwners;
  s32 freeCount = 0;
  s32 remaining = 0x200;

  do {
    if (*marker++ == 0xff) {
      freeCount++;
    }
    remaining--;
  } while (remaining != 0);
  return freeCount;
}

// Makes the sprite src share the VRAM block of alt, releasing its own; with
// no alt it keeps its block. Marks the sprite for redrawing and returns the
// sprite whose block is shared from now on (alt, or src when there was none),
// or NULL for no src. Chains of afterimages pass one block along this way.
void *Object_ReplaceResourceEntry(void *src, void *alt) {
  void *ret;
  struct FieldSprite *obj;

  obj = src;
  ret = NULL;
  if (obj != NULL) {
    if (alt == NULL) {
      obj->unk1d0 = 1;
    } else {
      Resource_ResetEntry(obj->vramBlock);
      obj->vramBlock = ((struct FieldSprite *)alt)->vramBlock;
      obj->unk1d0 = 1;
      obj = alt;
    }
    ret = obj;
  }
  return ret;
}

// Creates an object for an effect, at the caster's ground height, with
// sprite priority 0 and animation 1. An object without animation is released
// and NULL returned.
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct BattleFxScene *work = gEffectWork;
  struct ObjectRuntime *obj;

  obj = Object_CreateFar(kind, x, y, z);
  if (obj != NULL) {
    if (obj->animationKind == 0) {
      ObjectDispatch_ReleaseFar(obj);
      return NULL;
    }
    obj->terrainHeight =
        ((struct ObjectRuntime *)work->mainObject)->terrainHeight;
    obj->flags = 4;
    obj->unk23 = 4;
    ((struct AnimationObject *)obj->animation)->part[0].priority = 0;
    ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
  }
  return obj;
}

// Sets the movement state and animation timer steps of every object but the
// player's and except.
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state,
                                    s32 value) {
  s16 *player;
  struct ObjectRuntime *obj;
  s32 id;

  id = 0;
  player = (s16 *)gPlayerObjectId;
  do {
    obj = ObjectTable_Get(id);
    if (id != *player && obj != NULL && obj != except) {
      obj->movementState = state;
      ObjectDispatch_ApplyValueToChildrenFar(obj, value);
    }
    id++;
  } while (id <= OBJ_LAST);
}

// Per-frame update of arc A: over 32 frames it swells and shrinks again
// (scale sin(frame / 64 turn)) while it rises a pixel a frame on the object
// it follows, 9 pixels in front of it and up to 5 pixels further while
// small.
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj) {
  struct FieldActor *link;
  s32 v;

  link = obj->linkedObject;
  v = ++obj->timer;
  if (v > 31) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
    return;
  }
  v = Trig_Sin(v << 10);
  obj->scaleX = v;
  obj->scaleY = v;
  obj->x.fixed = link->x.fixed;
  obj->y.fixed += 0x10000;
  obj->z.fixed = link->z.fixed + (0x10000 - v) * 5 + 0x90000;
}

// Per-frame update of arc B: arc A upside down, 16 pixels in front of the
// object and up to 5 pixels nearer while small.
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj) {
  struct FieldActor *link;
  s32 v;

  link = obj->linkedObject;
  v = ++obj->timer;
  if (v > 31) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
    return;
  }
  v = Trig_Sin(v << 10);
  obj->scaleX = v;
  obj->scaleY = -v;
  obj->x.fixed = link->x.fixed;
  obj->y.fixed += 0x10000;
  obj->z.fixed = link->z.fixed - (0x10000 - v) * 5 + 0x100000;
}

//--------------------------------------------------------------------------
// Psynergy effects: preparing the work, recording caster and target, and
// spawning the swelling arcs.
//--------------------------------------------------------------------------
// A sprite as the arcs set it up: its three OAM attributes and its
// bookkeeping.
struct ArcSprite {
  u8 pad00[4];
  u16 y : 8;
  u16 affine : 1;
  u16 doubleSize : 1;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 color : 1; // 1 = 256 colours
  u16 shape : 2;
  u16 x : 9;
  u16 unused : 3;
  u16 hflip : 1;
  u16 vflip : 1;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 pad0a[18];
  u8 slot; // VRAM block
  u8 active : 1;
  u8 displayFlags : 7;
  u8 pad1e[8];
  u8 flags;
  u8 pad27;
  struct AnimationEntry *child;
};

// Spawns the two swelling arcs (object 26) on an object, A and B (B upside
// down); they follow it for 32 frames.
void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *source = scene->mainObject;
  struct FieldActor *objs[2];
  struct FieldActor *obj;
  struct ArcSprite *spr;
  s32 i;

  for (i = 0; i <= 1; i++) {
    obj = (struct FieldActor *)Object_CreateFar(
        26, linked->x.fixed, linked->y.fixed, linked->z.fixed);
    objs[i] = obj;
    if (obj == 0)
      continue;
    obj->terrainHeight = linked->terrainHeight;
    spr = obj->sprite;
    obj->motionFlags = 0;
    obj->timer = 0;
    obj->linkedObject = linked;
    obj->scaleX = obj->scaleY = 0x1999; // 0.1
    if (spr == 0)
      continue;
    // A wide 16-colour sprite drawn from the effect's tiles.
    AnimationObjects_SelectAnimationFar((struct AnimationObject *)spr, 0);
    spr->flags = 0;
    Resource_ResetEntry(spr->slot);
    spr->slot = scene->tileSlot;
    spr->active = 1;
    spr->tile = (gVramBlockCache[spr->slot].offset >> 5) & 0x3ff;
    spr->color = 0;
    spr->shape = 1;
    spr->size = 2;
    spr->child->frame = 0;
  }
  // Arc B in front of everything, arc A at the caster's priority.
  objs[0]->update = BattleFx_UpdateScaledArcObjectB;
  objs[0]->sprite->priority = 0;
  objs[1]->update = BattleFx_UpdateScaledArcObjectA;
  objs[1]->sprite->priority = source->sprite->priority;
  objs[1]->priorityFlags = 2;
}

// Per-frame update: flashes an object between sprite param 7 and 0 every 2
// frames and spawns the swelling arcs on it every 16.
void BattleFx_FlickerObjectAndTick(s32 obj) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValuesFar((struct DispatchObject *)obj, 7);
  } else {
    Animation_ApplyChildValuesFar((struct DispatchObject *)obj, 0);
  }
  if ((gFrameCount & 15) == 0) {
    BattleFx_SpawnScaledArcObjects(obj);
  }
}

// Per-frame update: cycles an object through sprite params 0 to 5, one step
// every 2 frames, and spawns the swelling arcs on it every 16.
void BattleFx_CycleObjectValueByCounter(s32 obj) {
  if ((gFrameCount & 1) != 0) {
    s32 col = (gFrameCount >> 1) % 6;

    Animation_ApplyChildValuesFar((struct DispatchObject *)obj, col);
  }
  if ((gFrameCount & 15) == 0) {
    BattleFx_SpawnScaledArcObjects(obj);
  }
}

// Scheduler callback: updates the 24 effect slots.
void BattleFx_UpdateAllEffectSlots(void) { UpdateAllEffectSlots(); }

// Prepares the work of the Psynergy effect for ability (heap slot 56) and
// loads its spark tiles. Called again while an effect resolves, it only
// switches the ability.
void BattleFx_LoadActionEffectResources(s32 abilityId, s32 mode) {
  struct FieldWork *ev = *(gWorkSlot + HEAP_SLOT_EVENT);
  struct MapScrollWork *map = *(gWorkSlot + 8);
  struct BattleFxScene *work;
  s32 running;

  running = ev->resolvingAction;
  if (running == 0) {
    work = Runtime_AllocateHeapBlock(HEAP_SLOT_FX, sizeof *work);
    Dma_Fill32(running, work, sizeof *work);
  } else {
    work = *(gWorkSlot + HEAP_SLOT_FX);
  }
  work->action = abilityId;
  work->animation = BattleActGet(abilityId)->type0c;
  running = ev->resolvingAction;
  if (running != 0)
    return;
  work->freeBlocks = 512 - ResourceTable_CountFreeBlocks();
  work->mode = mode;
  work->visible = 1;
  work->enabled = 1;
  work->active = 1;
  work->messageMode = 1;
  // Keep the map's shake and its decay, to restore after the effect.
  work->savedX = map->shakeX;
  work->savedY = map->shakeY;
  work->savedZ = map->shakeDecay;
  if (gGameState.map == (s32)&SceneId_MercuryEntrance)
    work->lit = 1;
  if (gGameState.map == (s32)&SceneId_MercuryLighthouse2)
    work->lit = 1;
  BattleFx_SetupObjectPair(gGameState.selectedActor, -1);
  if (work->animation != 8)
    ev->effectTimerRunning = running;
  work->tileSlot = Resource_FindFreeEntry();
  VramBlock_LoadCached((s16)work->tileSlot, 256, gBattleFx_SparkTiles);
  Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllEffectSlots, 0xc80);
}

// Records the caster and the target of an effect. With no target, the effect
// takes place 16 pixels in front of the caster (facing to the nearest
// quarter turn).
void BattleFx_SetupObjectPair(s32 userId, s32 targetId) {
  struct ScriptMotionObject *user;
  struct ScriptMotionObject *target;
  s32 dir;
  struct BattleFxScene *work;

  work = gEffectWork;
  work->firstObjectId = userId;
  user = ObjectTable_Get((s16)userId);
  work->secondObjectId = targetId;
  work->mainObject = user;
  target = ObjectTable_Get((s16)targetId);
  dir = (user->facing + 0x2000) & 0xc000; // to the nearest quarter turn
  work->child = target;
  work->angle = dir;
  if (target != NULL) {
    // Keep the target's motion and colours, to restore them later.
    work->savedCallback = (void *)target->hook;
    work->savedScript = (const s32 *)target->script;
    {
      struct AnimationObject *anim =
          ((struct ObjectRuntime *)target)->animation;
      u8 pal = anim->entries[0]->param;

      work->savedPalette = pal;
    }
    work->x = target->x;
    work->z = target->z;
    work->y = target->y;
    return;
  }
  work->x = user->x;
  work->z = user->z;
  work->y = user->y;
  Vector_AddPolarOffset(0x100000, dir, &work->x);
}

// Stops the caster's per-frame hook and restores its colours.
void EffectRuntime_StopCurrentObject(void) {
  struct BattleFxScene *work = gEffectWork;
  struct ScriptMotionObject *user = work->mainObject;

  user->hook = NULL;
  Animation_ApplyChildValuesFar(user, 0);
  WaitFrames(1);
}

struct BattleEffectPosition {
  u8 reserved00[4];
  s32 x;
  s32 y;
  s32 z;
};

#define BATTLE_OBJECT_SLOTS ((struct ScriptMotionObject *)gObjectSlots)

// Ends a Psynergy effect: frees the effect slots still in use and, unless
// another effect is being resolved, waits (at most 30 frames) for the
// swelling arcs to end, stops the slot callback, releases the effect's
// tiles, restores the map's shake, plays the queued sound and releases the
// effect's work.
void BattleEffect_CleanupSceneObjects(void) {
  struct BattleFxScene **sceneCell;
  struct BattleFxScene *scene;
  struct FieldWork *runtime;
  struct BattleEffectPosition *position;
  struct EffectSlot *sceneObject;
  s32 remaining;

  sceneCell = (struct BattleFxScene **)&gEffectWork;
  scene = *sceneCell;
  // The event work (slot 27) and the map work (slot 8), reached from the
  // effect's slot 56 of gWorkSlot.
  runtime = *(struct FieldWork **)((u8 *)sceneCell - 116);
  position = *(struct BattleEffectPosition **)((u8 *)sceneCell - 192);
  for (remaining = 0; remaining < 24; remaining++) {
    sceneObject = &scene->slots[remaining];
    if (sceneObject->active != 0)
      BattleFx_ClearOwnedSlot(sceneObject);
  }

  if (runtime->resolvingAction == 0) {
    s32 waited = 0;
    void (*firstActiveUpdate)(void) =
        (void (*)(void))BattleFx_UpdateScaledArcObjectB;
    void (*secondActiveUpdate)(void) =
        (void (*)(void))BattleFx_UpdateScaledArcObjectA;
    s32 active;

    do {
      struct ScriptMotionObject *slot = BATTLE_OBJECT_SLOTS;

      active = 0;
      remaining = 0;
      while (remaining <= 63) {
        void (*update)(void) = (void (*)(void))slot->hook;

        if (update == firstActiveUpdate || update == secondActiveUpdate) {
          active = 1;
          break;
        }
        remaining++;
        slot++;
      }

      if (active != 0) {
        waited++;
        WaitFrames(1);
      }
    } while (active != 0 && waited <= 29);

    runtime->castStarted = 0;
    Scheduler_RemoveCallback(BattleFx_UpdateAllEffectSlots);
    Resource_ResetEntry((s16)scene->tileSlot);
    position->x = scene->savedX;
    position->y = scene->savedY;
    position->z = scene->savedZ;
    if (scene->animation != 8)
      runtime->effectTimerRunning = 1;
    BattleFx_PlayQueuedSound();
    Runtime_ReleaseHeapBlock(0x38);
  }
}

// Starts casting: the caster takes its casting pose (animation 20) and
// stops. Unless the effect is silent the caster flashes with a sound; unless
// it is quiet, the ability's name is shown while every other object stands
// still. With flag 0x140 the caster's colours keep cycling and it
// takes pose 21; otherwise the caster's hook stops.
void EventObject_Initialize(void) {
  s32 eventValue;
  s32 eventIndex;
  struct FieldActor *eventObject;
  struct BattleFxScene *eventState;
  void *renderState;

  eventState = *(void **)Ram_EffectWork;
  renderState = *(void **)Ram_EventWork;
  eventObject = eventState->mainObject;
  eventValue = (s16)eventState->action;
  eventIndex = eventValue;
  ObjectDispatch_ApplyArgumentToChildrenFar(eventObject, 0x14);
  eventObject->targetX = eventObject->x.fixed;
  eventObject->targetY = eventObject->y.fixed;
  eventObject->targetZ = eventObject->z.fixed;
  eventObject->velocityX = 0;
  eventObject->velocityY = 0;
  eventObject->velocityZ = 0;
  if (eventState->visible != 0) {
    AudioCommand_PlayFar(212);
    eventObject->update = (void *)BattleFx_FlickerObjectAndTick;
  }
  if (eventState->active != 0) {
    ObjectGroup_SetActionForOthers(eventObject, 1, 0);
    UiWork_PushValueSlotFar(eventIndex, 4);
    if ((s8)eventState->mode != 0) {
      UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce,
                                             eventState->messageMode);
    } else {
      UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce,
                                             eventState->messageMode);
    }
    ObjectGroup_SetActionForOthers(eventObject, 0, 0x10);
  }
  if (GameFlag_TestFar(0x140) != 0) {
    if (eventState->visible != 0) {
      eventObject->update = (void *)BattleFx_CycleObjectValueByCounter;
    }
    ObjectDispatch_ApplyArgumentToChildrenFar(eventObject, 0x15);
  } else {
    EffectRuntime_StopCurrentObject();
  }
  ((struct FieldWork *)renderState)->castStarted = 1;
}

// Saves the palettes for the effect (once, unless an effect is already
// shown) and fades them toward one of the eight effect colours
// (gBattleFx_TargetColors), picked by the frame counter or forced by flags
// 0x148 to 0x14f, over 8 frames.
void BattleEffect_InitializeSharedScene(void) {
  u8 *scene;
  struct BattleEffectBuffers *work;
  s32 no;

  scene = *(gWorkSlot + HEAP_SLOT_EVENT);
  work = *(gWorkSlot + 32);
  Dma_Set(&work->target[0x2a0], scene + 0x776, 0x84000150, REG_DMA3);
  if (((struct FieldWork *)scene)->effectActive == 0)
    Dma_Set(work->target, scene + 0x236, 0x84000150, REG_DMA3);
  Dma_Set(work->target, work->current, 0x84000000 | (sizeof work->current / 4),
          REG_DMA3);

  no = gFrameCount & 7;
  if (GameFlag_TestFar(0x148))
    no = 0;
  if (GameFlag_TestFar(0x149))
    no = 1;
  if (GameFlag_TestFar(0x14a))
    no = 2;
  if (GameFlag_TestFar(0x14b))
    no = 3;
  if (GameFlag_TestFar(0x14c))
    no = 4;
  if (GameFlag_TestFar(0x14d))
    no = 5;
  if (GameFlag_TestFar(0x14e))
    no = 6;
  if (GameFlag_TestFar(0x14f))
    no = 7;
  BattleFx_ApplyColorToTargetBuffer(gBattleFx_TargetColors[no], 1);
  BattleFx_StartBufferInterpolation(8);
}

// Fades the palettes back from an effect's colours to the saved ones (or to
// palette 0x10001 while an effect is shown) over 8 frames.
void BattleFx_PrepareBufferInterpolation(void) {
  s32 value;
  u8 *state;

  state = (u8 *)gEventWork;
  value = (s32)(state + 0x236);
  BattleFx_ApplyColorToTargetBuffer(value, 2);
  if (((struct FieldWork *)state)->effectActive != 0) {
    BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
  } else {
    BattleFx_ApplyColorToTargetBuffer(value, 1);
  }
  BattleFx_StartBufferInterpolation(8);
}

// Converts a world position (16.16 pixels) to the screen: x and z receive
// the screen's x and y and y becomes 0. The world map uses its perspective
// projection.
void Camera_WorldToScreen(struct EffectVector *position) {
  struct EventRuntime **data = (struct EventRuntime **)&gEventWork;

  if ((*data)->mode19e == 3) {
    struct EffectVector result;

    Render_ProjectPoint(position, &result);
    position->x = result.x << 16;
    position->z = result.y << 16;
    position->y = 0;
  } else {
    // The map work, gWorkSlot[8], reached from the event work's slot 27.
    struct MapScrollWork *state = *(struct MapScrollWork **)((u8 *)data - 76);
    s32 x = state->viewX & 0xffff0000;
    s32 z = state->viewY & 0xffff0000;

    position->x -= x;
    position->z -= position->y;
    position->z -= z;
    position->y = 0;
  }
}

// From here on positions are passed in each function's own structures.

// Work slot 22 of the wave effect, 0x298 bytes (RunSceneTransitionEffect).
struct WaveFxWork {
  u16 lines[2][162]; // each scanline's offset, two pages written in turn
  u16 phase;         // the wave's phase
  u8 page;           // the page being written
  s8 red;            // the hue as a colour (red, green, blue, 4 to 11 each)
  s8 green;
  s8 blue;
  u16 hue;      // hue, 0 to 359 degrees
  u16 sourceId; // the caster
  u16 targetId; // the target
  u8 delay;     // frames before it starts
  u8 counter;   // particle spawn cycle (0 to 60)
};

// Starts the wave effect (kind 2) of caster sourceId on target targetId:
// fades the palettes toward a hue that shifts with the frame counter, and
// starts its per-frame update (BattleFx_UpdateWaveScene) after 8 frames.
void RunSceneTransitionEffect(s32 x, s32 y) {
  struct WaveFxWork *work;

  work = Runtime_AllocateBlock(22, 0x298);
  BattleEffect_InitializeSharedScene();
  Dma_Fill32(0, work, 0x298);
  work->hue = ((u32)(gFrameCount << 1) % (u32)360);
  BattleFx_AdvanceHueCycle();
  BattleFx_ApplyColorToTargetBuffer(
      (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
  BattleFx_StartBufferInterpolation(8);
  work->sourceId = x;
  work->targetId = y;
  work->delay = 8;
  Ui_FillBank15PaletteGrey();
  Scheduler_AddOrUpdateCallback((void (*)(void))BattleFx_UpdateWaveScene,
                                0xc80);
}

//--------------------------------------------------------------------------
// The wave effect: scanline waves, shifting colours, and particles flowing
// from the target to the caster.
//--------------------------------------------------------------------------

struct WaveFxSprite {
  u32 unk00[2];
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2;
  u32 unk09_4 : 4;
  u8 unk0a[0x1c];
  u8 palette;
};

// Stops the wave effect: restores the caster's colours and the palettes and
// releases the effect's work.
void BattleFx_FinishSceneAndReleaseHeapBlock(void) {
  void *work;

  work = *(void **)((u32)&gBattleBgFxWork);
  Ui_SetBank15PaletteAndClearRenderMode();
  Scheduler_RemoveCallback(BattleFx_UpdateWaveScene);
  Animation_ApplyChildPalette(
      Object_GetById(((struct WaveFxWork *)work)->sourceId), 1);
  BattleFx_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(22);
}

// Scheduler callback of the wave effect: once its delay has run out, writes
// the next page of scanline offsets (a sine wave of 4 pixels either way),
// steps the colour fade and the hue, keeps the caster's colours lit, and
// every 60 frames sends three particles (object 281) from the target to the
// caster, with sound 130.
void BattleFx_UpdateWaveScene(void) {
  struct WaveFxWork *work = gBattleBgFxWork;
  u16 *line;
  u32 i;
  struct FieldActor *src;
  struct FieldActor *dst;
  struct FieldActor *obj;
  struct WaveFxSprite *sprite;
  struct EffectVector from;
  struct EffectVector to;
  struct AnimationMetadata *meta;

  if (work->delay != 0) {
    work->delay--;
    return;
  }

  // The 160 scanlines' offsets, a sine wave of 4 pixels either way, on the
  // hidden page; then show that page.
  line = work->lines[work->page ^ 1];
  for (i = 0; i < 160; i++)
    *line++ = Trig_Sin(((u32)((work->phase + i * 8) << 16) / (u32)160)) >> 14;
  work->phase += 4;
  work->page ^= 1;
  if (work->page != 0) {
    BattleFx_ApplyColorToTargetBuffer(
        (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
    BattleFx_StartBufferInterpolation(1);
    BattleFx_AdvanceHueCycle();
  }
  Animation_ApplyChildPalette(Object_GetById(work->sourceId), 0);

  // At steps 0, 8 and 16 of the cycle, a particle from the target to the
  // caster.
  if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
    src = Object_GetById(work->sourceId);
    dst = Object_GetById(work->targetId);
    if (src != 0 && dst != 0) {
      // From 2 pixels below each one's sprite height (its metadata's boxX).
      from.x = src->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)BattleAction_FindDescriptor(work->sourceId)->sprite);
      from.y = src->y.fixed + ((s8)meta->boxX << 16) - (2 << 16);
      from.z = src->z.fixed;
      to.x = dst->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)BattleAction_FindDescriptor(work->targetId)->sprite);
      to.y = dst->y.fixed + ((s8)meta->boxX << 16) - (2 << 16);
      to.z = dst->z.fixed;
      obj = Object_CreateFar(281, to.x, to.y, to.z);
      if (obj != 0) {
        sprite = obj->sprite;
        obj->motionFlags = 0;
        obj->speed = 0xa3d7; // 0.64 pixels a frame
        obj->acceleration = 0xa3d7;
        obj->facing = ArcTan2(from.z - to.z, from.x - to.x);
        obj->update = BattleFx_SetCallbackWhenTargetUnset;
        sprite->palette = 0;
        sprite->mode = 1;
        Object_SetMoveTargetFar(obj, from.x, from.y, from.z);
      }
    }
  }

  if (work->counter == 0)
    AudioCommand_PlayFar(130);
  if (++work->counter > 60)
    work->counter = 0;
}

//--------------------------------------------------------------------------
// The background's scanline scroll DMA, the hue cycle, and BG palette 15.
//--------------------------------------------------------------------------

#define GREY28 0x739c // grey: red, green and blue 28

// Scheduler callback: restarts the HBlank DMA that writes BG0's horizontal
// scroll for each scanline from the wave's shown page.
void BattleFx_ArmBg0HBlankDma(void) {
  struct WaveFxWork *work = gBattleBgFxWork;
  if (!work->delay) {
    u16 *line = work->lines[work->page];
    volatile u16 *dma = REG_DMA0;
    dma[5] &= 0xc5ff; // clear the start timing and repeat
    dma[5] &= 0x7fff; // stop
    (void)dma[5];
    // One halfword at each HBlank (repeating, destination reloaded).
    Dma_Set(line, REG_BG0HOFS, 0xa2600001, (volatile u32 *)dma);
  }
}

// Moves the wave's hue on 4 degrees and works out its colour, each channel
// 4 to 11.
void BattleFx_AdvanceHueCycle(void) {
  struct WaveFxWork *work = gBattleBgFxWork;
  s32 r = 0;
  s32 g = 0;
  s32 b = 0;
  s32 offset;
  u8 *pr;
  u8 *pg;
  u8 *pb;

  BattleFx_ComputeHueChannels(work->hue << 16, &r, &g, &b);

  // Squeeze 0 to 31 into 4 to 11; offset walks red, green and blue (0x28b).
  offset = 0x28b;
  pr = (u8 *)work + offset;
  *pr = (r >> 18) + 4;
  offset += 1;
  pg = (u8 *)work + offset;
  *pg = (g >> 18) + 4;
  offset += 1;
  pb = (u8 *)work + offset;
  *pb = (b >> 18) + 4;

  work->hue += 4;

  *pr &= 0x1f;
  *pg &= 0x1f;
  *pb &= 0x1f;

  if (work->hue >= 360) {
    work->hue = 0;
  }
}

// Works out red, green and blue (0 to 31 each, 16.16) for a hue in degrees
// (16.16).
void BattleFx_ComputeHueChannels(s32 hue, s32 *r, s32 *g, s32 *b) {
  *r = BattleFx_HueChannelRamp(hue + (120 << 16), 0, 31 << 16);
  *g = BattleFx_HueChannelRamp(hue, 0, 31 << 16);
  *b = BattleFx_HueChannelRamp(hue - (120 << 16), 0, 31 << 16);
}

// One colour channel's strength at a hue angle: rising from 0 to 60 degrees,
// high from 60 to 180, falling from 180 to 240, low elsewhere.
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high) {
  angle = Fixed_Remainder(angle, 360 << 16);
  if (angle < 60 << 16)
    return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, angle));
  if (angle >= 60 << 16 && angle < 180 << 16)
    return high;
  if (angle >= 180 << 16 && angle < 240 << 16)
    return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, (240 << 16) - angle));
  return low;
}

// The 16.16 remainder of value divided by divisor (0 for a zero divisor).
s32 Fixed_Remainder(s32 value, s32 divisor) {
  s32 quot;

  if (divisor == 0)
    return 0;
  if (divisor & 0xf0000000)
    divisor = -divisor;
  quot = Iwram_RatioMulQ14(divisor, value);
  return value -
         Iwram_MulQ16(quot & 0xffff0000,
                      divisor); // less the quotient's whole part times divisor
}

// Per-frame update: once the object has arrived (no destination left), runs
// the script for untargeted objects.
void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *obj) {
  s32 ty;
  s32 tx;

  tx = obj->targetX;
  if (tx == 0x80000000) // no destination
  {
    ty = obj->targetY;
    if ((ty == tx) && (obj->targetZ == ty)) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_UntargetedObjectScript);
    }
  }
}

// Turns colours 1 and 3 to 12 of BG palette 15 grey and starts the BG0
// scanline scroll (BattleFx_ArmBg0HBlankDma).
void Ui_FillBank15PaletteGrey(void) {
  volatile u16 *p;

  ((struct UiRenderWork *)gWindowWork[0])->mode = 1;
  p = &BG_PLTT_COLOR(15, 1);
  *p = GREY28;
  p += 2;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg0HBlankDma, 0x480);
}

// Stops the BG0 scanline scroll and restores BG palette 15.
void Ui_SetBank15PaletteAndClearRenderMode(void) {
  void *work;

  work = gWindowWork[0];
  Scheduler_RemoveCallback(BattleFx_ArmBg0HBlankDma);
  BG_PLTT_COLOR(15, 1) = 0x7fff;  // white
  BG_PLTT_COLOR(15, 3) = 0;       // black
  BG_PLTT_COLOR(15, 11) = 0x294a; // grey (10)
  BG_PLTT_COLOR(15, 12) = 0x5294; // grey (20)
  PaletteGlow_UpdateFar(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  ((struct UiRenderWork *)work)->mode = 0;
}

// Returns the direction of the d-pad (0x10000 a full turn): right 0, down
// 0x4000, left 0x8000, up 0xc000; 0xffff when none or a diagonal is held.
s32 BattleFx_GetCycledTableWord(u32 unused) {
  return gBattleFx_CyclePatternWords[(gKeysHeld >> 4) &
                                     15]; // the right, left, up and down bits
}

//--------------------------------------------------------------------------
// The item break effect: the main fragment's motion.
//--------------------------------------------------------------------------
// A FieldActor seen with the halfword at 0x5e (its lifetime), which
// FieldActor does not name.
struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 gravity;
  u8 reserved4c[9];
  u8 motionFlags;
  u8 reserved56[8];
  u16 lifetime;
};

// Per-frame update of a broken item's main fragment: turns it toward the
// object it is linked to, at most 1/16 turn a frame, and sheds a particle
// (object 0x11d) round it.
void BattleFx_UpdateItemBreakFragment(struct FieldActor *src) {
  struct FieldActor *target;
  struct EffectVector pos;
  struct ItemBreakFragmentObject *obj;
  s32 turn;
  s32 dist;

  target = src->linkedObject;
  if (target != 0) {
    s32 dx = target->x.fixed - src->x.fixed;
    s32 dz = target->z.fixed - src->z.fixed;

    if (dx != 0 || dz != 0) {
      turn = (s16)(ArcTan2(dz, dx) - src->facing);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      src->facing += turn;
    }
    src->steeringFlags = 0;
  }

  // The particle starts 8 to 24 pixels lower and up to 48 pixels away in a
  // random direction.
  pos.x = src->x.fixed;
  pos.y = src->y.fixed - (Random16() << 4) - 0x80000;
  pos.z = src->z.fixed;
  dist = Random16() * 3;
  dist <<= 4;
  Vector_AddPolarOffset(dist, Random16(), &pos);

  obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->motionFlags = 2;
    obj->gravity = 0x1999;
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
    obj->lifetime = 12;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }
}

//--------------------------------------------------------------------------
// Field effects: pushing a target with the d-pad, appearing and vanishing
// particles, and watching the leader's range.
//--------------------------------------------------------------------------

#define CELL_MASK 0xfff00000 // a 16-pixel tile

// Move (kind 1): puts the pushing hand beside the target and pushes the
// target 16 pixels the way the d-pad points, for as long as the player
// pushes; A, B, L or R end it. Where the target cannot go, an error sound
// plays every 16 frames. When the push would run into the caster, the caster
// blocks it or, when the tile beyond is free, is pushed along too.
void BattleFx_SteerPushedTarget(s32 targetId) {
  struct EffectVector position;
  struct EffectVector beyond;
  struct BattleFxScene *work;
  struct FieldActor *user;
  struct FieldActor *target;
  struct FieldActor *pusher;
  struct FieldActor *hit;
  s32 x;
  s32 z;
  s32 angle;
  s32 moved;
  s32 dir;
  s32 keys;
  s32 cellX;
  s32 cellZ;
  struct EffectVector *p;

  work = gEffectWork;
  user = work->mainObject;
  target = work->child;
  angle = work->angle + 0x8000; // opposite work->angle
  moved = 0;
  if (target == NULL)
    return;
  BattleEffect_InitializeSharedScene();
  user->linkedObject = target;
  ObjectDispatch_InitializeFar((struct DispatchObject *)user,
                               gBattleFx_SourceHoldScript);
  pusher = BattleFx_StartItemBreak(user);
  if (pusher == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }

  // The hand goes 16 pixels above the target and 16 pixels toward angle.
  pusher->linkedObject = target;
  p = &position;
  p->x = target->x.fixed;
  p->y = target->y.fixed + (16 << 16);
  p->z = target->z.fixed;
  Vector_AddPolarOffset(16 << 16, angle, p);
  Object_SetMoveTargetFar(pusher, p->x, p->y, p->z);
  BattleFx_SnapScaleToFull((struct MotionObject *)pusher);
  pusher->speed = 4 << 16;
  pusher->acceleration = 0x8000;
  pusher->motionFlags = 4;
  target->update = ObjectGroup_ApplyRandomChildValues; // cycle its colours
  target->speed = 0x6666;
  target->acceleration = 0x3333;
  target->steeringFlags = moved;
  target->terrainId = 2;
  for (;;) {
    WaitFrames(1);
    keys = gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS);
    if (keys != 0)
      break;

    // With the d-pad released, the hand goes back to its place.
    dir = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
    if (dir == 0xffff) {
      position.x = target->x.fixed;
      position.y = target->y.fixed + (16 << 16);
      position.z = target->z.fixed;
      Vector_AddPolarOffset(16 << 16, angle, &position);
      Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 1);
      pusher->velocityX = 0;
      pusher->velocityY = 0;
      pusher->velocityZ = 0;
      continue;
    }

    // The hand leans 2 pixels the way it pushes; check the tiles 16 and 32
    // pixels beyond the target.
    position.x = target->x.fixed;
    position.y = target->y.fixed + (16 << 16);
    position.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, angle, &position);
    Vector_AddPolarOffset(2 << 16, dir, &position);
    Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    Script_WaitForEventTimeoutFar(pusher);
    position.x = target->x.fixed;
    position.y = target->y.fixed;
    position.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, dir, &position);
    beyond.x = target->x.fixed;
    beyond.y = target->y.fixed;
    beyond.z = target->z.fixed;
    Vector_AddPolarOffset(32 << 16, dir, &beyond);
    if (Map_CheckObjectStepFar(target, &position) > 0) {
      ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
      if ((gFrameCount & 15) == 0)
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      continue;
    }
    hit = ScriptObject_FindOverlappingEntryFar(target, p);
    if (hit != NULL) {
      if (hit != user) {
        ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
        continue;
      }

      // The push runs into the caster: if the caster stands on the tile the
      // target would move to, it cannot be pushed; if the caster stands on the
      // tile 32 pixels on, the caster moves along when the tile beyond is free.
      cellX = user->x.fixed & CELL_MASK;
      cellZ = user->z.fixed & CELL_MASK;
      if (cellX == (p->x & CELL_MASK) && cellZ == (p->z & CELL_MASK)) {
        ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
        continue;
      }
      if (cellX == (beyond.x & CELL_MASK) && cellZ == (beyond.z & CELL_MASK)) {
        if (Map_GetCellAttributeLowNibbleFar(user->terrainId, beyond.x,
                                             beyond.z) != 0) {
          ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
          if ((gFrameCount & 15) == 0)
            AudioCommand_PlayFar(SOUND_MENU_ERROR);
          continue;
        }
        moved = 1;
      }
    }

    // Push: the hand's animation comes from the turn between angle and the
    // push (in quarter turns).
    AudioCommand_PlayFar(175);
    x = position.x;
    z = position.z;
    ObjectDispatch_ApplyArgumentToChildrenFar(
        pusher, gBattleFx_QuadrantModes[(u16)(angle - dir) >> 14]);
    WaitFrames(15);
    target->paused = 0;
    target->speed = 0x3333;
    target->acceleration = 0x3333;
    Object_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speed = 0x3333;
    pusher->acceleration = 0x3333;
    Vector_AddPolarOffset(16 << 16, dir, &position);
    Object_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16),
                            position.z);
    if (moved == 1) {
      Object_GetById(work->firstObjectId)->steeringFlags &= 0xfe;
      user->speed = 0x3333;
      user->acceleration = 0x3333;
      Object_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
    }
    Script_WaitForEventTimeoutFar(target);
    target->x.fixed = x;
    target->z.fixed = z;
    target->velocityX = 0;
    target->velocityZ = 0;
    break;
  }

  // Restore the target's colours, script and update, and remove the hand.
  Animation_ApplyChildValuesFar(target, work->savedPalette);
  ObjectDispatch_InitializeFar((struct DispatchObject *)target,
                               work->savedScript);
  target->update = work->savedCallback;
  EffectRuntime_StopCurrentObject();
  if (moved == 1)
    (Object_GetById(work->firstObjectId))->steeringFlags |= 1;
  BattleFx_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(pusher);
}

// Effect slot callback of a particle that flies 30 pixels out from where it
// started, comes back and is freed.
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot) {
  struct EffectVector position;
  s8 *statePtr = &slot->state;
  s32 state;

next_state:
  state = *statePtr;
  if (state == 0) {
    u32 angle;

    position.x = slot->originX;
    position.z = slot->originZ;
    angle = Random16();
    Vector_AddPolarOffset(30 << 16, (u16)angle, &position);
    slot->targetX = position.x;
    slot->targetZ = position.z;
    slot->acceleration = 4 << 16;
    slot->maxSpeed = 4 << 16;
    slot->flag42 = state;
    (*statePtr)++;
    return;
  }

  if (state == 1) {
    if (BattleFx_HasReachedTarget(slot) == 0) {
      (*statePtr)++;
      goto next_state;
    }
    return;
  }

  if (state == 2) {
    slot->targetX = slot->originX;
    slot->targetZ = slot->originZ;
    slot->maxTurnStep = 0x400;
    slot->flag42 = 1;
    (*statePtr)++;
    return;
  }

  if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
    BattleFx_ClearOwnedSlot(slot);
}

// The item break effect cast with no target: shows the broken item beside
// the caster for 30 frames, then turns it into particles.
void BattleFx_RunItemBreakSequence(void) {
  s32 work[3];
  struct BattleFxScene *scene;
  void *object;

  scene = gEffectWork;
  object = scene->mainObject;

  /* FAKEMATCH: the one-pass block keeps the mainObject load below the
   * frame's sub sp. Measured: plain, the scheduler lifts the load
   * above it (differ 2). */
  do {
    BattleEffect_InitializeSharedScene();
  } while (0);
  object = BattleFx_StartItemBreak(object);
  BattleFx_SnapScaleToFull(object);
  if (object != 0) {
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 4);
    WaitFrames(30);
  }
  BattleFx_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(object);
}

// Spawns the broken item (object 215) 16 pixels above object and moves it 16
// pixels the way object faces (to the nearest quarter turn), with 8
// particles round it. Returns the item, or 0.
void *BattleFx_StartItemBreak(void *object) {
  struct MotionObject *src = object;
  struct MotionObject *parent;
  struct MotionObject *child;
  s32 angle;
  s32 dist;
  s32 speed;
  s32 i;
  u32 spread;
  u32 jitter;

  angle = (src->angle + 0x2000) & 0xc000; // to the nearest quarter turn
  parent = Object_Spawn(215, src->x, src->y + (16 << 16), src->z);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000; // from 0.25
  parent->scaleX = 0x4000;
  parent->update = BattleFx_UpdateItemBreakFragment;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  ObjectDispatch_ApplyArgumentToChildrenFar(parent, 3);
  Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  // Particles: 8 to 32 pixels within 1/8 turn of the way it faces, at 1 to 2
  // pixels a frame.
  for (i = 7; i >= 0; i--) {
    child = Object_Spawn(285, src->x, src->y + (16 << 16), src->z);
    if (child != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)child,
                                   gBattleFx_FragmentScript);
      speed = Random16() + 0x10000;
      child->acceleration = 1 << 16;
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->verticalMotionStrength = 0x51e;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24 + (8 << 16);
      jitter = Random16();
      Motion_SetTargetPositionFromMagnitudeAngle(
          child, dist, ((jitter - Random16()) >> 3) + src->angle);
    }
  }
  AudioCommand_PlayFar(SOUND_ITEM_BREAK);
  return parent;
}

// Per-frame update: grows an object's scale by 1/16 a frame up to full size
// and fixes its position.
void BattleFx_SnapScaleToFull(struct MotionObject *obj) {
  s32 next;
  s32 scale;

  if (obj != NULL) {
    scale = obj->scaleX;
    if (scale <= 0xffff) {
      do {
        next = scale + 0x1000;
        scale = next;
      } while (next <= 0xffff);
      obj->scaleX = next;
      obj->scaleY = next;
    }
    Script_WaitForEventTimeoutFar(obj);
  }
}

// Per-frame update: the object spins as it rises for 31 frames, shrinks, and
// bursts into 8 particles.
void UpdateRisingParticleBurst(void *object) {
  struct MotionObject *src = object;
  struct MotionObject *child;
  s32 i;
  s32 dist;
  u32 spread;

  AudioCommand_PlayFar(154);
  for (i = 30; i >= 0; i--) {
    src->y += 1 << 16;
    src->angle += 0x2000; // 1/8 turn
    src->scaleX += -0x800;
    src->scaleY += -0x800;
    WaitFrames(1);
  }
  for (i = 7; i >= 0; i--) {
    child = Object_Spawn(285, src->x, src->y, src->z);
    if (child != 0) {
      s32 speed;
      /* FAKEMATCH: a block-local 1.0, set before the block's calls, keeps
       * 0x10000 in one register that loop.c hoists for both uses.
       * Measured: written as the constant at both uses, or with speed
       * holding the call plus the constant, it stays unhoisted (89
       * instructions for 93, differ 24); the acceleration stored before
       * Random16() written inline hoists it but moves that store above
       * the call (differ 4). */
      s32 unit = 0x10000;

      ObjectDispatch_InitializeFar((struct DispatchObject *)child,
                                   gBattleFx_FragmentScript);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->verticalMotionStrength = 0xa3d;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += 8 << 16;
      Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  AudioCommand_PlayFar(131);
  ObjectDispatch_ReleaseFar(src);
}

// Passes value (as animation timer steps) to every one of the 64 objects
// whose first sprite part shows sprite 200.
void ObjectDispatch_ApplyValueToKind200Children(s32 value) {
  struct ObjectRuntime *object = gObjectSlots;
  s32 cnt = 63;

  do {
    if (object->script != NULL) {
      u8 kind = object->animationKind;

      if (kind == 1) {
        struct AnimationObject *anim = object->animation;
        struct AnimationEntry *entry = anim->entries[0];

        if (entry->animId == 200) {
          entry->param = value;
          anim->dirty = kind;
        }
      }
    }
    cnt--;
    object++;
  } while (cnt >= 0);
}

// Scheduler callback of the timed effect (kind 8): asks for it to end once
// the leader is 60 pixels from its centre or its time has run out. The
// centre and the time are in the event work from 0xcb8 on
// (RunBattleEffect08).
void FieldEffect_WatchLeaderDistance(void) {
  struct FieldWork *work;
  struct ObjectRuntime *actor;
  s16 *timer;
  s32 dx;
  s32 dz;

  work = (struct FieldWork *)gEventWork;
  actor = ObjectTable_Get(gGameState.selectedActor);
  if (work->effectTimerRunning != 0) {
    timer = &work->effectFrames;
    if (*timer != 0)
      (*timer)--;
  }

  // x is scaled by 0xd105 (about 0.82) for the comparison.
  dx = Iwram_MulQ16(work->effectCenterX - actor->x / 0x10000, 0xd105);
  dz = work->effectCenterY - (actor->z - actor->y) / 0x10000;
  if (dx * dx + dz * dz >= 3600 || work->effectFrames == 0) {
    u16 *slot = (u16 *)&work->command; // psynergyRequest
    u32 request = 0x2090;

    *slot = request;
  }
}

//--------------------------------------------------------------------------
// Psynergy effect 8: a screen transition centred on the caster, and markers.
//--------------------------------------------------------------------------

// Psynergy effect 8: starts a screen transition centred on the leader, runs
// the matching event if there is one, and puts markers round the leader.
void RunBattleEffect08(void) {
  struct FieldWork *scene;
  struct DisplayTransitionState *work;
  struct FieldActor *target;
  s32 arg;
  void *ev;
  s32 frame;

  scene = WORK_SLOT(HEAP_SLOT_EVENT);
  ObjectDispatch_ApplyValueToKind200Children(6);
  DisplayTransition_InitializeBattleEffectState(8);
  work = WORK_SLOT(HEAP_SLOT_TRANSITION);
  target = ObjectTable_Get(gGameState.selectedActor);
  work->centreX = target->x.fixed;
  work->centreY = target->z.fixed - target->y.fixed;
  BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
  BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
  BattleFx_StartBufferInterpolation(1);
#if !defined(TBS_EDITION_EN)
  scene->effectActive = 0; // 英語版にはない
#endif
  WaitFrames(1);
  ev = BattleFx_FindMatchingEvent(0x50000005, 8, &arg);
  if (ev != NULL)
    BattleFx_RunEventAction(ev, gGameState.selectedActor, arg);
  AudioCommand_PlayFar(131);

  // イベントのワークに中心 (ドット) と時間を入れ、近くに印を出す
  scene->effectActive = 1;
  scene->effectCenterX = work->centreX / 0x10000;
  scene->effectCenterY = work->centreY / 0x10000;
  scene->effectFrames = 600;
  scene->effectTimerRunning = 1;
  FieldEffect_SpawnNearbyMarkers();
  frame = 0;
  do {
    WaitFrames(1);
    work->value = frame;
    frame++;
  } while (frame <= 18);
  Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}

//--------------------------------------------------------------------------
// burstobj.c
// エナジー演出：イマジンの打ち切り、演出 12 の粒、演出 7 で対象を持ち上げる
//--------------------------------------------------------------------------

#define PSYNERGY_EV_4 0x40000005 // エナジーのイベント、段階 4
#define FX_REVEAL 8              // イマジンの演出
#define PARTICLE_OBJ 284         // 粒の物体
#define FALL_OBJ 217             // 落ちる物体
#define FALL_LAST 4              // 落ちる物体は 0-4 の 5 個

struct BurstParticleVector {
  s32 values[3]; // x, y, z
};

// 粒のスプライトの属性
struct BurstParticleVisual {
  u8 reserved00[5];
  u8 flagsALow : 5;
  u8 flip : 1;
  u8 flagsAHigh : 2;
  u8 reserved06;
  u8 flagsBLow : 6;
  u8 flagsBHigh : 2;
  union {
    struct {
      u16 shape : 10;
      u16 unused : 6;
    } bits;
    struct {
      u8 lo;
      u8 highLow : 4;
      u8 palette : 4;
    } bytes;
  } attributes;
};

struct BurstParticleVisualGroup {
  struct BurstParticleVisual primary;
  struct BurstParticleVisual child;
};

//--------------------------------------------------------------------------
// イマジンの効果中なら打ち切る
// 見張りをやめ、色を黒から 30 フレームで戻し、イマジンの段階 4
// のイベントを実行する
//--------------------------------------------------------------------------
void ResetSceneTransitionEffect(void) {
  // gEffectWork はスロット 56。0x74 前がイベントワーク (27)、0x64 前がスロット
  // 31
  struct BattleFxScene **cell = &gEffectWork;
  struct BattleFxScene *work = *cell;
  struct DisplayTransitionState *scene =
      *(struct DisplayTransitionState **)((u8 *)cell - 0x64);
  struct FieldWork *state = *(struct FieldWork **)((u8 *)cell - 0x74);
  s16 zero;
  s32 tid;
  void *ev;

  if (state->effectActive != 0) {
    AudioCommand_PlayFar(SOUND_SCENE_TRANSITION);
    Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);

    zero = 0;
    state->effectActive = zero;
    state->effectFrames = zero;
    ObjectDispatch_ApplyValueToKind200Children(0);

    BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 1);
    BattleFx_StartBufferInterpolation(1);
    BattleFx_ApplyColorToSourceBuffer(0, 0);
    BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 0);
    BattleFx_StartBufferInterpolation(30);
    WaitFrames(1);

    ev = BattleFx_FindMatchingEvent(PSYNERGY_EV_4, FX_REVEAL, &tid);
    if (ev != NULL)
      BattleFx_RunEventAction(ev, gGameState.selectedActor, tid);

    if (work->childOption == 0) {
      scene->nextMode = 0;
      scene->duration = 1;
      scene->step = 1;
      WaitFrames(10);
    }
  }
}

//--------------------------------------------------------------------------
// 対象のある演出 12：対象のまわりに粒を出し、対象を足元の優先度にする
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticleMainObject(s32 unused) {
  struct FieldActor *target;

  target = ((struct BattleFxScene *)gEffectWork)->child;
  if (target != NULL) {
    BattleEffect_SpawnBurstParticleField();
    ObjectDispatch_ApplyArgumentToChildrenFar(target, 2);
    target->collisionFlags = 0;
    ObjectDispatch_SetSingleChildFlagsFar(target, 0);
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    WaitFrames(10);
    AudioCommand_PlayFar(SOUND_RECOVERY);
    WaitFrames(40);
    BattleFx_PrepareBufferInterpolation();
  }
}

//--------------------------------------------------------------------------
// 対象のない演出 12：粒を出したあと、まわり 4-10 ドットに落ちる物体を 5 個出す
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticles(void) {
  struct BattleFxScene *work = gEffectWork;
  struct BurstParticleVector pos;
  struct BurstParticleVector *p;
  s32 n;

  BattleEffect_SpawnBurstParticleField();
  AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
  p = &pos;
  n = FALL_LAST;
  do {
    struct FieldActor *obj;
    s32 dist;

    p->values[0] = work->x;
    p->values[2] = work->z;
    dist = (Random16() * 6) + 0x40000;
    Vector_AddPolarOffset(dist, Random16(), p);
    p->values[1] = work->y;
    obj = Object_Spawn(FALL_OBJ, p->values[0], p->values[1], p->values[2]);
    if (obj != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_BurstParticleObjectScript);
      obj->motionFlags = ACTOR_FALLS;
    }
    WaitFrames(((Random16() * 2) >> 16) + 2);
    n--;
  } while (n >= 0);
  WaitFrames(30);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 使う者の前から粒を 24 個、2 フレームおきに出し、演出の位置のまわりへ飛ばす
// 出る位置：向き 0x4000 は 10 ドット上、0xc000 は 24 ドット上、ほかは 10
// ドット上の 10 ドット前
//--------------------------------------------------------------------------
void BattleEffect_SpawnBurstParticleField(void) {
  struct BattleFxScene *work;
  struct FieldActor *user;
  s32 pos[3];
  s32 n;

  work = gEffectWork;
  user = work->mainObject;
  BattleEffect_InitializeSharedScene();

  for (n = 0; n < PARTICLE_MAX; n++) {
    struct FieldActor *obj;
    struct BurstParticleVisual *visual;
    struct BurstParticleVisual *child;
    s32 dist;

    if (work->angle == 0x4000) {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + 0xa0000;
      pos[2] = user->z.fixed;
    } else if (work->angle == 0xc000) {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + 0x180000;
      pos[2] = user->z.fixed;
    } else {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + 0xa0000;
      pos[2] = user->z.fixed;
      Vector_AddPolarOffset(0xa0000, work->angle, pos);
    }

    // 子のスプライトに、向きと形と色を写す
    obj = Object_Spawn(PARTICLE_OBJ, pos[0], pos[1], pos[2]);
    visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
    child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
    child->flip = visual->flip;
    child->flagsAHigh = visual->flagsAHigh;
    child->flagsBHigh = visual->flagsBHigh;
    child->attributes.bits.shape = visual->attributes.bits.shape;
    child->attributes.bytes.palette = visual->attributes.bytes.palette;

    if (obj != 0) {
      obj->scaleY = 0xb333; // 0.7 倍
      obj->scaleX = 0xb333;
      obj->acceleration = 0x18000;
      obj->speed = 0x18000;
      obj->motionFlags = 0;
      Animation_ApplyChildValuesFar(obj, 11);
      ObjectDispatch_ApplyArgumentToChildrenFar(obj, 7);
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_CommonParticleScript + 1);
      ObjectDispatch_SetSingleChildFlagsFar(obj, 1);

      // 行き先：演出の位置から 4-10 ドット
      pos[0] = work->x;
      pos[1] = work->y;
      pos[2] = work->z;
      if (work->angle == 0xc000)
        Vector_AddPolarOffset(0xe0000, work->angle, pos);
      dist = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset(dist, Random16(), pos);
      Object_SetMoveTargetFar(obj, pos[0], pos[1], pos[2]);
    }
    AudioCommand_PlayFar(131);
    WaitFrames(2);
  }
  WaitFrames(8);
}

//--------------------------------------------------------------------------
// 対象のある演出 7
// 左右 32 ドットに物体を出して 14 ドット動かし、対象といっしょに 32
// ドット持ち上げる
//--------------------------------------------------------------------------
void BattleEffect_RunTargetedItemBreak(s32 unused) {
  struct BattleFxScene *work;
  struct DispatchObject *user;
  struct FieldActor *target;
  struct MotionObject *pair[2];
  s32 pos[3];
  s32 x, y, z;
  s32 n;
  s32 startY;

  work = gEffectWork;
  target = work->child;
  user = work->mainObject;
  if (target == NULL)
    return;

  BattleEffect_InitializeSharedScene();
  user->argument = (s32)target;
  ObjectDispatch_InitializeFar(user, gBattleFx_CommonParticleScript + 3);

  x = work->x;
  pos[0] = x;
  y = work->y + 0x100000; // 16 ドット上
  pos[1] = y;
  z = work->z;
  pos[2] = z;
  pair[0] = BattleFx_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
  pair[1] = BattleFx_SpawnItemBreakMode3(pos[0] - 0x200000, pos[1], pos[2], 0);

  WaitFrames(15);
  for (n = 0; n < 2; n++) {
    struct MotionObject *obj = pair[n];
    if (obj != NULL)
      Motion_SetTargetPositionFromMagnitudeAngle(obj, 0xe0000, obj->angle);
  }

  Script_WaitForEventTimeoutFar(pair[0]);
  target->update =
      (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
  AudioCommand_PlayFar(130);
  target->motionFlags = 4;

  // 1/4 ドットずつ 32 ドット上げる
  startY = target->y.fixed;
  if (pair[0] != NULL && pair[1] != NULL && startY <= startY + 0x200000) {
    do {
      pair[0]->y += 0x4000;
      pair[1]->y += 0x4000;
      target->y.fixed += 0x4000;
      WaitFrames(1);
    } while (target->y.fixed <= startY + 0x200000);
  }

  UpdateRisingParticleBurst(pair[0]);
  UpdateRisingParticleBurst(pair[1]);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// anchor.c
// エナジー演出 7：左右の二つの物体と、飛び散る破片
//--------------------------------------------------------------------------
// 破片の数
#define ANCHOR_SHARD_MAX 24

//--------------------------------------------------------------------------
// 対象のない演出の本体：演出の位置の 16 ドット上、左右 32 ドットに spawn で
// 物体を出し、それぞれの向きへ 24 ドット動かしてから、破片を 24 個、
// 8-32 ドット先へ散らす
//--------------------------------------------------------------------------
#define DEFINE_ITEM_BREAK_EFFECT(name, spawn)                                  \
  void name(void) {                                                            \
    struct BattleFxScene *work;                                                \
    struct MotionObject *obj;                                                  \
    struct MotionObject *pair[2];                                              \
    struct ScatterPosition pos;                                                \
    struct MotionObject **walk;                                                \
    s32 n;                                                                     \
    s32 dist;                                                                  \
                                                                               \
    work = gEffectWork;                                                        \
    BattleEffect_InitializeSharedScene();                                      \
    pos.x = work->x;                                                           \
    pos.y = work->y + 0x100000;                                                \
    pos.z = work->z;                                                           \
    pair[0] = spawn(pos.x + 0x200000, pos.y, pos.z, 0x8000);                   \
    pair[1] = spawn(pos.x - 0x200000, pos.y, pos.z, 0);                        \
                                                                               \
    WaitFrames(15);                                                            \
    walk = pair;                                                               \
    for (n = 0; n < 2; n++) {                                                  \
      obj = *walk++;                                                           \
      if (obj != 0)                                                            \
        Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle); \
    }                                                                          \
                                                                               \
    Script_WaitForEventTimeoutFar(pair[0]);                                    \
    AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);                                  \
    for (n = 0; n < ANCHOR_SHARD_MAX; n++) {                                   \
      pos.x = work->x;                                                         \
      pos.y = work->y + 0x100000;                                              \
      pos.z = work->z;                                                         \
      obj = Object_Spawn(285, pos.x, pos.y, pos.z);                            \
      if (obj != 0) {                                                          \
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj,             \
                                     gBattleFx_FragmentScript);                \
        obj->speedLimit = Random16() + 0x20000;                                \
        obj->acceleration = 0x20000;                                           \
        obj->motionFlags = 0;                                                  \
        dist = Random16() * 24 + 0x80000;                                      \
        Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());     \
      }                                                                        \
    }                                                                          \
                                                                               \
    ObjectDispatch_ReleaseFar(pair[0]);                                        \
    ObjectDispatch_ReleaseFar(pair[1]);                                        \
    BattleFx_PrepareBufferInterpolation();                                     \
  }

// 対象のない演出 7
DEFINE_ITEM_BREAK_EFFECT(RunBattleEffect07, BattleFx_SpawnItemBreakMode3)

//--------------------------------------------------------------------------
// 物体 215 を出し、回しながら 1/4 から等倍まで 24 フレームで大きくする
// angle : 大きくなったあとの向き
//--------------------------------------------------------------------------
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z,
                                                  s32 angle) {
  struct MotionObject *obj;
  s32 scale;

  AudioCommand_PlayFar(SOUND_ITEM_BREAK);
  obj = Object_Spawn(215, x, y, z);
  if (obj != NULL) {
    obj->scaleX = obj->scaleY = 0x4000;
    obj->acceleration = obj->speedLimit = 0x30000; // 3 ドット/フレーム
    ((struct FieldSprite *)obj->records)->priority = 0;
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
    scale = obj->scaleX;
    if (scale < 0x10000) {
      do {
        scale += 0x800;
        obj->scaleY = scale;
        obj->scaleX = scale;
        obj->angle += 0x2000; // 1/8 回転
        WaitFrames(1);
        scale = obj->scaleX;
      } while (scale <= 0xffff);
    }
    obj->angle = angle;
  }
  return obj;
}

//--------------------------------------------------------------------------
// impact.c
// 散って集まる粒の動きと、重い衝撃の破片
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 粒の動き : 原点のまわりへ散ってから、主オブジェクトのそばへ集まって消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot) {
  struct BattleFxScene *scene = gEffectWork;
  struct EffectVector pos;
  s32 state;

again:
  state = slot->state;
  if (state == 0) {
    // 原点から 25 ドット、でたらめな方向へ
    pos.x = slot->originX;
    pos.z = slot->originZ;
    Vector_AddPolarOffset(25 << 16, (u16)Random16(), &pos);
    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 3 << 16;
    slot->maxSpeed = 3 << 16;
    slot->flag42 = 0;
    slot->state++;
    return;
  }

  if (state == 1) {
    // 着いたらすぐ次へ
    if (BattleFx_HasReachedTarget(slot) == 0) {
      slot->state++;
      goto again;
    }
    return;
  }

  if (state == 2) {
    // 主オブジェクトの 16 ドット上から scene->angle へ 8 ドット、
    // 画面座標にして 4 ドットの揺らぎを足す
    struct MotionObject *obj = scene->mainObject;

    pos.x = obj->x;
    pos.y = obj->y + (16 << 16);
    pos.z = obj->z;
    Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->maxTurnStep = 0x800;
    slot->flag42 = 1;
    slot->state++;
    return;
  }

  if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
    BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 重い衝撃 : y を 32 ドット下げた位置に 0x11b を置き、破片を 12 個まき散らす
// src : 衝撃の位置になるオブジェクト
//--------------------------------------------------------------------------
s32 SpawnHeavyImpactEffect(struct MotionObject *src) {
  struct EffectVector pos;
  struct ObjectRuntime *obj;
  struct ObjectRuntime *frag;
  s32 i;

  AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
  pos.x = src->x;
  pos.y = src->y;
  pos.z = src->z;
  obj = Object_Spawn(0x11b, pos.x, pos.y - (32 << 16), pos.z);
  if (obj != NULL) {
    obj->flags = 0;
    obj->unk5e = 20;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }

  // 破片 : 速さ 1〜2 ドット、8〜32 ドット先のでたらめな方向へ
  for (i = 0; i < 12; i++) {
    frag = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
    if (frag != NULL) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)frag,
                                   gBattleFx_FragmentScript);
      frag->speedLimit = Random16() + (1 << 16);
      frag->acceleration = 1 << 16;
      frag->flags = 0;
      Motion_SetTargetPositionFromMagnitudeAngle(
          frag, Random16() * 24 + (8 << 16), Random16());
    }
  }
  return 0;
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 4 番を呼ぶ
//--------------------------------------------------------------------------
void BattleFx_CallEffect04(s32 unused) { RunBattleEffect04(); }

//--------------------------------------------------------------------------
// orbit.c
// フィールドのエナジー演出：4 番（押し出す弾）、6 番（渦を巻いて集まる粒）
//--------------------------------------------------------------------------

#define ABOVE (16 << 16) // 16 ドット上

//--------------------------------------------------------------------------
// obj の 16 ドット上の位置
//--------------------------------------------------------------------------
static __inline__ void PosAbove(struct FieldActor *obj,
                                struct EffectVector *pos) {
  pos->x = obj->x.fixed;
  pos->y = obj->y.fixed + ABOVE;
  pos->z = obj->z.fixed;
}

// FieldActor と同じ物体。FieldActor に欄のない 0x5e を使う
struct SparkObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[24];
  s32 anchorX; // ゆれの中心（FieldActor の targetX の位置）
  u8 unk3c[12];
  s32 gravity; // FieldActor と同じ位置
  u8 unk4c[9];
  u8 motionFlags; // FieldActor と同じ位置
  u8 unk56[8];
  u16 lifetime;
  u8 unk60[4];
  s16 phase; // ゆれの位相（128 フレームで 1 周）
  u8 unk66[6];
  void (*update)(struct SparkObject *);
};

//--------------------------------------------------------------------------
// エナジー 4 番 : 使う者の前に弾を出して大きくし、残像 3 つと前へ飛ばす
// 当たった物（scene->child）は 16 ドット押し出す
//--------------------------------------------------------------------------
void RunBattleEffect04(void) {
  struct FieldActor *target;
  s32 ctx;
  struct FieldActor *copies[4];
  struct EffectVector pos;
  struct FieldActor *obj;
  struct FieldActor *copy;
  struct FieldActor *src;
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  struct AnimationObject *res;
  void *ev;
  s32 scale;
  s32 i;
  u8 resSlot;

  scene = gEffectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(130);
  slot = scene->slots;

  // 使う者の頭上から、光を 2 フレームおきに 12 個はじけさせる
  for (i = 0; i <= 11; i++) {
    src = scene->mainObject;
    PosAbove(src, &pos);
    Camera_WorldToScreen(&pos);
    EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 9);
    slot->scaleY = 0xb333; // 0.7 倍
    slot->scaleX = 0xb333;
    WaitFrames(2);
    slot++;
  }

  // 弾：使う者の向きに 8 ドット先で、0.25 倍から等倍まで大きくする
  src = scene->mainObject;
  pos.x = src->x.fixed;
  pos.y = src->y.fixed + ABOVE;
  pos.z = src->z.fixed;
  Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
  obj = Object_Spawn(0xd7, pos.x, pos.y, pos.z);
  if (obj == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }
  obj->scaleY = 0x4000;
  obj->scaleX = 0x4000;
  obj->facing = scene->angle;
  obj->speed = 4 << 16;
  obj->acceleration = 4 << 16;
  obj->motionFlags = 0;
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 5);
  Animation_ApplyChildValuesFar(obj, 3);
  scale = obj->scaleX;
  if (scale < 0x10000) {
    do {
      scale += 0x500;
      obj->scaleY = scale;
      obj->scaleX = scale;
      WaitFrames(1);
      scale = obj->scaleX;
    } while (scale <= 0xffff);
  }
  WaitFrames(3);

  // 残像 3 つ。絵の資源はひとつを分けあう
  res = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] =
        Object_Spawn(0xd7, obj->x.fixed, obj->y.fixed, obj->z.fixed);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->facing = scene->angle;
      copy->speed = 4 << 16;
      copy->acceleration = 4 << 16;
      copy->motionFlags = 0;
      ObjectDispatch_ApplyArgumentToChildrenFar(copy, 5);
      Animation_ApplyChildValuesFar(copy, 2);
      res = Object_ReplaceResourceEntry((struct AnimationObject *)copy->sprite,
                                        res);
    }
  }
  resSlot = res->slot;

  // 行き先：scene->enabled なら使う者の 56 ドット先、ほかは scene の位置
  if (scene->enabled != 0) {
    src = scene->mainObject;
    pos.x = src->x.fixed;
    pos.y = src->y.fixed + ABOVE;
    pos.z = src->z.fixed;
    Vector_AddPolarOffset(56 << 16, scene->angle, &pos);
  } else {
    pos.x = scene->x;
    pos.y = scene->y + ABOVE;
    pos.z = scene->z;
  }
  Object_SetMoveTargetFar(obj, pos.x, pos.y, pos.z);
  ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                               gBattleFx_BurstParticleObjectScript + 4);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      WaitFrames(3);
      Object_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
      ObjectDispatch_InitializeFar((struct DispatchObject *)copy,
                                   gBattleFx_CommonParticleScript + 1);
    }
  }

  // 弾のスクリプトが終わるまで、60 フレームまで待つ
  for (i = 0; i <= 59 && ((struct ScriptMotionObject *)obj)->script != NULL;
       i++)
    WaitFrames(1);

  // 当たった物を、ぶつからなければ 16 ドット押し出す（childOption なら跳ねる）
  if (target != NULL && scene->childMode == 0) {
    if (scene->childOption != 0)
      target->velocityY = 8 << 16;
    pos.x = target->x.fixed;
    pos.y = target->y.fixed;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, scene->angle, &pos);
    if (Map_CheckObjectStepFar(target, &pos) == 0 &&
        ScriptObject_CheckOverlapFar(target, &pos) == 0) {
      target->acceleration = 1 << 16;
      target->speed = 1 << 16;
      Object_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
    }
  }

  // 応じるマップのイベントがあれば動かす
  ev = BattleFx_FindMatchingEvent(0x50000005, 4, &ctx);
  if (ev != NULL)
    BattleFx_RunEventAction(ev, gGameState.selectedActor, ctx);
  WaitFrames(10);
  BattleFx_PrepareBufferInterpolation();
  WaitFrames(20);
  if (resSlot != 96)
    Resource_ResetEntry(resSlot);
}

//--------------------------------------------------------------------------
// 毎フレームの処理：8 フレームごとに 2 フレームだけアニメ 2 にして光らせる
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleMain(struct FieldActor *particle) {
  s32 frame = gFrameCount & 7;
  if (frame == 0) {
    Animation_ApplyChildValuesFar(particle, 2);
  } else if (frame == 2) {
    Animation_ApplyChildValuesFar(particle, 0);
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理：1/16 ずつ縮み、1/16 以下で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleFade(struct FieldActor *particle) {
  s32 scale;

  if (particle != NULL) {
    scale = particle->scaleX - 0x1000;
    particle->scaleY -= 0x1000;
    particle->scaleX = scale;
    if (scale <= 0x1000)
      ObjectDispatch_InitializeFar((struct DispatchObject *)particle,
                                   gBattleFx_CommonParticleScript);
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理：target のまわりを左まわりに渦を巻いて寄る
// 半径は残りフレーム × 2 ドット、1 フレームに 0x800 まわる
// work は始めの向き、timer は残りフレーム。0 で縮んで消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle) {
  struct EffectVector pos;
  s16 life;
  s32 cnt;

  if (particle != NULL) {
    cnt = (u16)particle->timer - 1;
    particle->timer = cnt;
    life = cnt;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset(life << 17, (s16)particle->work + (life << 11),
                            &pos);
      particle->x.fixed = pos.x;
      particle->y.fixed = pos.y;
      particle->z.fixed = pos.z;
    } else {
      particle->update =
          (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
    }
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理：左まわりと同じで、右まわり
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle) {
  struct EffectVector pos;
  s16 life;
  s32 cnt;

  if (particle != NULL) {
    cnt = (u16)particle->timer - 1;
    particle->timer = cnt;
    life = cnt;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset(life << 17, (s16)particle->work - (life << 11),
                            &pos);
      particle->x.fixed = pos.x;
      particle->y.fixed = pos.y;
      particle->z.fixed = pos.z;
    } else {
      particle->update =
          (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
    }
  }
}

//--------------------------------------------------------------------------
// エナジー 6 番をかけられた物（scene->child）から始める
// childMode があれば、物は変えずに粒だけ出す
//--------------------------------------------------------------------------
void BattleFx_StartOrbitingParticles(s32 unused) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *target = scene->child;

  if (target != 0) {
    if (scene->childMode != 0) {
      scene->enabled = 1;
    }
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    BattleFx_RunOrbitingParticles();
  }
}

//--------------------------------------------------------------------------
// エナジー 6 番 : 粒 16 個が scene の位置へ渦を巻いて集まり、
// 物を 16 回光らせる。childOption がなければ物を mode 2 にする
//--------------------------------------------------------------------------
void BattleFx_RunOrbitingParticles(void) {
  s32 ctx;
  struct EffectVector pos;
  struct EffectVector *p;
  struct BattleFxScene *scene;
  struct FieldActor *target;
  struct FieldActor *particle;
  void *ev;
  s32 cnt;

  scene = gEffectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(115);

  // 3 フレームおきに粒を出す。大きさ 0.5〜1 倍、まわる向きはでたらめ
  // 中心は scene の位置から 2〜6 ドットずらす
  p = &pos;
  cnt = 15;
  do {
    particle = Object_Spawn(0xe8, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 dist;

      scale = (Random16() >> 1) + 0x8000;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update =
            (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleLeft;
      else
        particle->update =
            (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->work = Random16();
      Animation_ApplyChildValuesFar(particle, 9);

      p->x = scene->x;
      p->y = scene->y;
      p->z = scene->z;
      dist = (Random16() << 2) + (2 << 16);
      Vector_AddPolarOffset(dist, Random16(), p);
      particle->targetX = p->x;
      particle->targetY = p->y;
      particle->targetZ = p->z;
    }

    WaitFrames(3);
    cnt--;
  } while (cnt >= 0);

  WaitFrames(10);
  AudioCommand_PlayFar(115);
  WaitFrames(50);

  if (target != NULL && scene->enabled == 0) {
    AudioCommand_PlayFar(212);

    // 5 フレームごとに 1 フレーム光る
    cnt = 15;
    do {
      Animation_ApplyChildValuesFar(target, 7);
      WaitFrames(1);
      Animation_ApplyChildValuesFar(target, 0);
      WaitFrames(4);
      cnt--;
    } while (cnt >= 0);

    if (scene->childOption == 0) {
      AudioCommand_PlayFar(220);
      ObjectDispatch_ApplyArgumentToChildrenFar(target, 2);
    }

    target->update =
        (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleMain;
    ev = BattleFx_FindMatchingEvent(0x50000005, 6, &ctx);
    if (ev != NULL) {
      BattleFx_RunEventAction(ev, gGameState.selectedActor, ctx);
    }
    WaitFrames(20);
  }

  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 毎フレームの処理：anchorX を中心に左右 4 ドット、128 フレームで 1 往復ゆれる
//--------------------------------------------------------------------------
void BattleFx_SwayParticle(struct SparkObject *obj) {
  s16 *phase;

  phase = &obj->phase;
  obj->x = obj->anchorX + Iwram_MulQ16(4 << 16, Trig_Sin(*phase << 9));
  (*phase)++;
  *phase = (*phase + 128) % 128;
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 10 ドット上を中心に、渦を巻いて寄る
// 半径は残りフレームと同じドット数。timer が 0 で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *obj) {
  struct BattleFxScene *scene = gEffectWork;
  struct EffectVector pos;
  s16 life;
  s32 cnt;

  if (obj != NULL) {
    cnt = (u16)obj->timer - 1;
    obj->timer = cnt;
    life = cnt;
    if (life != 0) {
      pos.x = scene->x;
      pos.y = scene->y + (10 << 16);
      pos.z = scene->z;
      Vector_AddPolarOffset(life << 16, (s16)obj->work + (life << 11), &pos);
      obj->x.fixed = pos.x;
      obj->y.fixed = pos.y;
      obj->z.fixed = pos.z;
    } else {
      ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                   gBattleFx_CommonParticleScript);
    }
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 16 ドット上で左右 6 ドット（64
// フレーム）ゆれ、 3 フレームごとに、ゆれながらゆっくり落ちる粒を出す phase が
// -1 なら、その場から動かない
//--------------------------------------------------------------------------
void BattleFx_RunSparkEmitter(struct SparkObject *obj) {
  struct BattleFxScene *scene;
  s16 *phase;
  struct SparkObject *spark;
  s32 pos[3];

  phase = &obj->phase;
  scene = gEffectWork;
  if (*phase != -1) {
    obj->x = scene->x + Iwram_MulQ16(6 << 16, Trig_Sin(*phase << 10));
    obj->y = scene->y + ABOVE;
    obj->z = scene->z;
    (*phase)++;
    *phase = (*phase + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    // 2 ドット上から、でたらめな向きに 0〜6 ドットずらす
    pos[0] = obj->x;
    pos[1] = obj->y + (2 << 16);
    pos[2] = obj->z;
    Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
    spark = Object_Spawn(0x11d, pos[0], pos[1], pos[2]);
    if (spark != 0) {
      spark->update = BattleFx_SwayParticle;
      spark->scaleX = spark->scaleY = 0x9999; // 0.6 倍
      spark->motionFlags = ACTOR_FALLS;
      spark->gravity = 458;
      spark->phase = Random16() >> 9;
      spark->anchorX = spark->x;
      Animation_ApplyChildValuesFar(spark, 9);
      spark->lifetime = 72;
      ObjectDispatch_InitializeFar((struct DispatchObject *)spark,
                                   gBattleFx_CommonParticleScript);
    }
  }
}

//--------------------------------------------------------------------------
// call_effect_03_and_stop.c
// 演出 03：渦を巻いて集まる 8 個の粒と、火花を出して飛び去る 1 個
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 演出 03 を流し、いまのオブジェクトを止める
//--------------------------------------------------------------------------
void BattleFx_CallEffect03AndStop(s32 unused) {
  RunBattleEffect03();
  EffectRuntime_StopCurrentObject();
}

//--------------------------------------------------------------------------
// 演出 03
// 8 個の粒が渦を巻いて中心へ集まり、中心に現れた 1 個が火花を出してから
// 遠くへ飛び去る
//--------------------------------------------------------------------------
void RunBattleEffect03(void) {
  struct BattleFxScene *scn = gEffectWork;
  struct FieldActor *target = scn->mainObject;
  struct FieldActor *object;
  struct FieldSprite *last;
  u8 marker;
  s32 i;

  BattleEffect_InitializeSharedScene();

  // 渦の粒：1 フレームに 1 個ずつ、1/8 周ずらして出す
  last = 0;
  for (i = 0; i < 8; i++) {
    object = Object_Spawn(233, target->x.fixed, target->y.fixed + 0x200000,
                          target->z.fixed);
    if (object != 0) {
      object->scaleY = 0xb333; // 0.7 倍
      object->scaleX = 0xb333;
      object->update = BattleFx_UpdateShrinkingOrbitObject;
      object->timer = 120;
      object->work = i << 13;
      object->motionFlags = 4;
      last = Object_ReplaceResourceEntry(object->sprite, last);
    }
    WaitFrames(1);
  }

  marker = last->vramBlock;
  AudioCommand_PlayFar(130);
  WaitFrames(110);

  // 中心の 1 個
  target = Object_Spawn(233, 0, 0, 0);
  object = target;
  if (target != 0) {
    target->scaleY = 0xb333;
    target->scaleX = 0xb333;
    target->x.fixed = scn->x;
    target->y.fixed = scn->y + 0x100000;
    target->z.fixed = scn->z;
    target->motionFlags = 4;
    Animation_ApplyChildValuesFar(target, 7);
  }

  AudioCommand_PlayFar(131);
  WaitFrames(12);
  if (target != 0) {
    // 2 フレームごとに子の値を切り替える（4 回に 1 回は 10）
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH: needed to match: the ROM keeps object as a second register
       * copy of the spark (r5 beside r6); this redundant reassignment stops
       * gcse copy-propagating object = target away, and loop then drops it
       * (differ 1 without it). */
      target = object;
      if (i & 3)
        Animation_ApplyChildValuesFar(object, 9);
      else
        Animation_ApplyChildValuesFar(object, 10);
      WaitFrames(2);
    }
  }

  // 火花
  Animation_ApplyChildValuesFar(object, 0);
  AudioCommand_PlayFar(84);
  if (object != 0) {
    target->update = BattleFx_RunSparkEmitter;
    target->timer = 0;
    if (scn->enabled != 0)
      WaitFrames(128);
    else
      WaitFrames(192);
  }

  // 飛び去る
  if (target != 0) {
    target->timer = -1;
    target->speed = 0x50000;
    target->acceleration = 0x6666;
    target->steeringFlags = 0;
    Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
    Script_WaitForEventTimeoutFar(target);
    ObjectDispatch_ReleaseFar(target);
  }
  if (marker != 0x60)
    Resource_ResetEntry(marker);
  if (scn->finishCallback != 0)
    scn->finishCallback();
  BattleFx_PrepareBufferInterpolation();
}

#define CLOAK_TILE 0xfb  // 隠れていられるタイルの属性
#define CLOAK_END 0x2092 // 隠れていられないときの psynergyRequest

//--------------------------------------------------------------------------
// 毎フレームの処理 : 選択中のオブジェクトの足もとのタイルが
// CLOAK_TILE でなければ、CLOAK_END を要求する
// イベントの mode が 3 なら 32 ドット四方の gMapBlocks、
// ほかは 16 ドット四方の地形のマップを見る
//--------------------------------------------------------------------------
void CheckObjectMapTile(void) {
  u32 slot;
  struct EventRuntime *ev;
  struct ObjectRuntime *obj;
  u8 *tile;
  s32 x;
  s32 y;

  slot = (u32)&gEventWork;
  ev = gEventWork;
  obj = ObjectTable_Get(gGameState.selectedActor);
  tile = (u8 *)*(struct MapState **)(slot - 76); // gMapWork[0]

  if (ev->mode19e == 3) {
    u32 tx;
    u32 ty;

    x = obj->x;
    if (x < 0)
      x += 0x1fffff;
    tx = (x >> 21) & 31;

    y = obj->z;
    if (y < 0)
      y += 0x1fffff;
    ty = (y >> 21) & 31;

    tile = (u8 *)gMapBlocks + ((tx + (ty << 5)) << 2);
  } else {
    if (obj->terrainId <= 2) {
      tile = (u8 *)((struct MapState *)tile)->layers[obj->terrainId].cells;
    } else
      tile = (u8 *)gMapCellBuffer;

    x = obj->x;
    if (x < 0)
      x += 0xfffff;
    {
      u32 tx = x >> 20;

      y = obj->z;
      if (y < 0)
        y += 0xfffff;

      tile = (u8 *)(tile + ((tx + ((y >> 20) << 7)) << 2));
    }
  }

  if (tile[2] != CLOAK_TILE)
    ((struct EventWork *)ev)->psynergyRequest = CLOAK_END;
}

//--------------------------------------------------------------------------
// 姿を戻す : 確認を止め、点滅させて cloaked を消す
//--------------------------------------------------------------------------
void MapEvent_RunTileTriggerSequence(void) {
  struct ObjectRuntime *obj;
  struct AnimationObject *anim;
  struct AnimationEntry *entry;
  u32 i;

  obj = ObjectTable_Get(gGameState.selectedActor);
  anim = obj->animation;
  entry = anim->entries[0];

  AudioCommand_PlayFar(154);
  Scheduler_RemoveCallback(CheckObjectMapTile);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
  ((struct ScriptMotionObject *)obj)->hook = 0;

  for (i = 0; i < 5; ++i) {
    entry->param = 7;
    anim->dirty = 1;
    anim->flags = 2;
    WaitFrames(2);
    anim->dirty = 1;
    anim->flags = 0;
    WaitFrames(2);
  }

  for (i = 0; i < 5; ++i) {
    entry->param = 7;
    anim->dirty = 1;
    anim->flags = 0;
    WaitFrames(2);
    entry->param = 0;
    anim->dirty = 1;
    WaitFrames(2);
  }

  anim->flags = 1;
  gGameState.cloaked = 0;
}

//--------------------------------------------------------------------------
// 隠れているなら、足もとの確認を始める
//--------------------------------------------------------------------------
void BattleFx_ScheduleCloakedMapCheck(void) {
  if (gGameState.cloaked != 0) {
    Scheduler_AddOrUpdateCallback((SchedulerCallback)CheckObjectMapTile, 0xc80);
  }
}

//--------------------------------------------------------------------------
// 姿を隠す : 10 回点滅させて cloaked を立て、足もとを確かめる
// その場で隠れられなければ、すぐに姿を戻す
//--------------------------------------------------------------------------
void BattleFx_RunFlashingCallbackSequence(void) {
  struct EventWork *ev = gEventWork;
  struct ObjectRuntime *obj = ObjectTable_Get(gGameState.selectedActor);
  struct AnimationObject *anim = obj->animation;
  struct AnimationEntry *entry = anim->entries[0];
  u32 n;
  void (*check)(void);

  AudioCommand_PlayFar(130);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
  ((struct ScriptMotionObject *)obj)->hook = 0;
  n = 0;
  do {
    entry->param = 7;
    anim->dirty = 1;
    anim->flags = 2;
    WaitFrames(2);
    anim->dirty = 1;
    anim->flags = 0;
    WaitFrames(2);
    n++;
  } while (n <= 9);
  n = 0;
  entry->param = n;
  anim->flags = 2;
  anim->dirty = 1;
  check = CheckObjectMapTile;
  Scheduler_AddOrUpdateCallback(check, 0xc80);
  gGameState.cloaked = 1;
  check();
  if (ev->psynergyRequest == CLOAK_END) {
    MapEvent_RunTileTriggerSequence();
    ev->psynergyRequest = n;
  }
}

// FieldActor と同じ物体。FieldActor に欄のない 0x5e (phase) を使う
struct Effect05Object {
  u8 reserved00[8];
  s32 x, y, z;
  s32 altitude; // 落ちて止まる高さ
  s32 scaleX, scaleY;
  u8 reserved20[8];
  s32 velocityY;
  u8 reserved2c[4];
  s32 speed;
  u8 reserved34[0x1c];
  struct WaveFxSprite *sprite;
  u8 reserved54;
  u8 flag;
  u8 reserved56[8];
  s16 phase;
  u8 reserved60[0xc];
  void *callback; // 毎フレームの処理
};

struct Effect05State {
  s32 angle;
  s32 x, y, z; // 玉の行き先の足もと
  struct Object *target;
  s32 initialized;
  u8 reserved18[8];
  u8 highArc;
  u8 reserved21[0x13];
  u8 variant;
  u8 reserved35[0x10];
  u8 alternate;
};

//--------------------------------------------------------------------------
// 毎フレームの処理 : 落ちる粒が altitude に着いたら、小さな粒を 3 つ散らす
//--------------------------------------------------------------------------
void BattleFx_SpawnRandomAngleTriplet(struct Effect05Object *obj) {
  s32 i;
  struct Effect05Object *p;
  s32 phase = 2;
  s32 phase2;
  s16 *pp;

  if (obj->y <= obj->altitude) {
    obj->phase = phase;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
    p = NULL;
    obj->callback = p;

    // 半分の大きさで、上へ 1 ドットの速さで跳ね、32 ドット先のでたらめな方向へ
    for (i = 0; i <= 2; i++) {
      p = Object_Spawn(0xf0, obj->x, obj->y, obj->z);
      if (p == NULL) {
        break;
      }
      p->scaleY = 0x8000;
      p->scaleX = 0x8000;
      p->flag = 2;
      p->velocityY = 1 << 16;
      p->speed = Random16() + 0x13333;
      Motion_SetTargetPositionFromMagnitudeAngle(p, 32 << 16, Random16());
      pp = &p->phase;
      phase2 = 6;
      *pp = phase2;
      ObjectDispatch_InitializeFar((struct DispatchObject *)p,
                                   gBattleFx_CommonParticleScript);
    }
  }
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 左右にゆれながらゆっくり落ち、地面に着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *obj) {
  s32 r;

  obj->y -= 0x4ccc;
  r = Random16();
  obj->x += r - Random16();
  if (obj->y <= obj->terrainHeight) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }
}

void BattleFx_CallEffect05(s32 unused) { RunBattleEffect05(); }

//--------------------------------------------------------------------------
// 0〜10 段で start から end へ
//--------------------------------------------------------------------------
static __inline__ s32 Lerp10(s32 start, s32 end, s32 step) {
  return start + step * (end - start) / 10;
}

//--------------------------------------------------------------------------
// 光の玉を対象の 16 ドット上から行き先の 32 ドット上 (variant なら 80 ドット上)
// へ 10 フレームで運びながら大きくし、粒を降らせてから戻す
// 行き先は、まだ決まっていなければ対象から angle の向きへ 16 ドット
//--------------------------------------------------------------------------
void RunBattleEffect05(void) {
  struct EffectVector pos;
  struct EffectVector start;
  struct EffectVector end;
  struct Effect05State *work = gEffectWork;
  struct Object *target = work->target;
  struct Effect05Object *orb;
  struct EffectVector *from;
  struct EffectVector *to;
  s32 i;
  s32 steps = 11;
  s32 count;
  s32 y;
  struct Effect05Object *mote;

  orb = Object_Spawn(0xef, i = 0, 0, 0);
  if (orb == 0)
    return;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(138);
  if (work->initialized == 0) {
    work->x = target->pos.x;
    work->z = target->pos.z;
    Vector_AddPolarOffset(16 << 16, work->angle,
                          (struct EffectVector *)&work->x);
    work->y = Map_GetTerrainHeightFar(0, work->x, work->z);
  }
  from = &start;
  from->x = target->pos.x;
  from->y = target->pos.y + (16 << 16);
  from->z = target->pos.z;
  to = &end;
  to->x = work->x;
  y = work->y;
  to->y = y + (32 << 16);
  to->z = work->z;
  if ((s8)work->variant != 0)
    to->y = y + (80 << 16);

  // 運びながら 1/4 から等倍まで大きくする
  for (; i < steps; i++) {
    s32 scale;

    {
      struct EffectVector *a = &start;
      struct EffectVector *b = &end;
      s32 e, t;

      e = b->x;
      t = a->x;
      t += i * (e - t) / 10;
      orb->x = t;
      e = b->y;
      t = a->y;
      t += i * (e - t) / 10;
      orb->y = t;
      e = b->z;
      t = a->z;
      t += i * (e - t) / 10;
      orb->z = t;
    }
    scale = Lerp10(0x4000, 0x10000, i);
    orb->scaleX = scale;
    orb->scaleY = scale;
    WaitFrames(1);
  }
  WaitFrames(10);

  if ((s8)work->alternate == 0) {
    // 玉のまわり 3〜8 ドットから粒を 6 フレームおきに落とす (最後は玉の真下)
    count = 10;
    if ((s8)work->highArc == 0)
      count = 24;
    for (i = 0; i < count; i++) {
      pos.x = orb->x;
      pos.y = orb->y;
      pos.z = orb->z;
      Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
      if (i == count - 1) {
        WaitFrames(25);
        pos.x = orb->x;
        pos.y = orb->y;
        pos.z = orb->z;
      }
      mote = Object_Spawn(0xf0, pos.x, pos.y, pos.z);
      if (mote != 0) {
        mote->altitude = pos.y - (32 << 16);
        mote->callback = (void *)BattleFx_SpawnRandomAngleTriplet;
        mote->flag = 2;
      }
      AudioCommand_PlayFar(132);
      WaitFrames(6);
    }
    WaitFrames(10);
  } else {
    // 玉のまわり 3〜8 ドットから、ゆれて落ちる粒を 6 フレームおきに出す
    if ((s8)work->highArc != 0)
      count = 10;
    else
      count = 30;
    for (i = 0; i < count; i++) {
      pos.x = orb->x;
      pos.y = orb->y;
      pos.z = orb->z;
      Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
      mote = Object_Spawn(0x11c, pos.x, pos.y, pos.z);
      if (mote != 0) {
        mote->callback = (void *)BattleFx_UpdateDriftingFallObject;
        mote->flag = 0;
        mote->sprite->mode = 2;
        ObjectDispatch_ApplyArgumentToChildrenFar(mote, 8);
        Animation_ApplyChildValuesFar(mote, 7);
      }
      WaitFrames(6);
    }
    WaitFrames(70);
  }

  // 等倍から 1/4 まで縮めながら、対象の上へ戻す
  i = 0;
  do {
    s32 scale;

    {
      struct EffectVector *a = &end;
      struct EffectVector *b = &start;
      s32 e, t;

      e = b->x;
      t = a->x;
      t += i * (e - t) / 10;
      orb->x = t;
      e = b->y;
      t = a->y;
      t += i * (e - t) / 10;
      orb->y = t;
      e = b->z;
      t = a->z;
      t += i * (e - t) / 10;
      orb->z = t;
    }
    scale = Lerp10(0x10000, 0x4000, i);
    orb->scaleX = scale;
    orb->scaleY = scale;
    WaitFrames(1);
    i++;
  } while (i < steps);
  ObjectDispatch_ReleaseFar(orb);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// particles.c
// バトル効果：持ち上げた物のまわりに落ちる粒
//--------------------------------------------------------------------------

struct FallScene {
  u8 reserved00[20];
  struct FieldActor *child; // 持ち上げている物
};

LAYOUT_SIZE_GUARD(FallPos_Size, struct EffectVector, 0x0c);
LAYOUT_OFFSET_GUARD(FallScene_Child, struct FallScene, child, 0x14);

//--------------------------------------------------------------------------
// 毎フレームの処理：持ち上げている物のまわりに、落ちる粒をひとつ出す
// 高さは物から 8〜24 ドット上、横はでたらめな向きに 0〜48 ドット
//--------------------------------------------------------------------------
void BattleFx_SpawnFallingParticles(void) {
  struct FieldActor *src;
  struct EffectVector pos;
  struct ItemBreakFragmentObject *obj;
  u32 dist;

  src = ((struct FallScene *)gEffectWork)->child;
  pos.x = src->x.fixed;
  pos.y = src->y.fixed - Random16() * 16 + (24 << 16);
  pos.z = src->z.fixed;

  dist = Random16() * 3;
  Vector_AddPolarOffset(dist * 16, Random16(), &pos);

  obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->motionFlags = ACTOR_FALLS;
    obj->gravity = 0x1999; // 重力 0.1 ドット
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
    obj->lifetime = 12;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }
}

//--------------------------------------------------------------------------
// lift_drop.c
// 対象を 2 つの持ち手で持ち上げ、方向キーで動かして落とす
//--------------------------------------------------------------------------

#define SHARD_MAX 20 // 破片の数

//--------------------------------------------------------------------------
// 足もとを 16 ドット上げて、pos へ動いたときにぶつかるかを調べる
//--------------------------------------------------------------------------
static __inline__ s32 ObjProbeUp(struct FieldActor *obj,
                                 struct EffectVector *pos) {
  s32 hit;

  obj->terrainHeight += 16 << 16;
  hit = Map_CheckObjectStepFar(obj, pos);
  obj->terrainHeight -= 16 << 16;
  return hit;
}

//--------------------------------------------------------------------------
// 対象を左右の持ち手で 24 ドットの高さまで持ち上げ、方向キーの向きへ
// 16 ドット動かして落とす (A・B・L・R ならその場で落とす)
// 行き先がふさがっていれば、ブザーを鳴らして待つ
//--------------------------------------------------------------------------
void BattleFx_SteerLiftedTarget(s32 id) {
  struct BattleFxScene *work;
  struct FieldActor *src;
  struct FieldActor *target;
  struct FieldActor *shards[SHARD_MAX];
  struct EffectVector pos;
  struct FieldActor *hands[2];
  struct FieldActor *obj;
  struct FieldActor *right;
  s32 i;
  s32 angle;
  s32 dist;
  struct EffectVector *p;

  work = gEffectWork;
  src = work->mainObject;
  target = work->child;
  if (target == NULL)
    return;
  BattleEffect_InitializeSharedScene();
  src->linkedObject = target;
  ObjectDispatch_InitializeFar((struct DispatchObject *)src,
                               gBattleFx_SourceHoldScript);

  // 持ち手を左右 32 ドットに出し、対象の左右 16 ドットへ動かす
  /* FAKEMATCH: the position is reached through this pointer everywhere
   * but in the steering loop's first part, which names the position
   * itself. Measured: the position named everywhere differs 10; the
   * pointer everywhere differs 561 (514 instructions for 531). */
  p = &pos;
  p->x = work->x;
  p->y = work->y + (16 << 16);
  p->z = work->z;
  obj = BattleFx_SpawnItemBreakMode1(p->x + (32 << 16), p->y, p->z, 0x8000);
  hands[0] = obj;
  right = BattleFx_SpawnItemBreakMode1(p->x - (32 << 16), p->y, p->z, 0);
  hands[1] = right;
  if (hands[0] == NULL || right == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }
  WaitFrames(15);
  p->x = target->x.fixed;
  p->y = target->y.fixed + (16 << 16);
  p->z = target->z.fixed;
  Object_SetMoveTargetFar(obj, p->x + (16 << 16), p->y, p->z);
  Object_SetMoveTargetFar(right, p->x - (16 << 16), p->y, p->z);
  Script_WaitForEventTimeoutFar(obj);
  Script_WaitForEventTimeoutFar(right);
  obj->x.fixed = p->x + (16 << 16);
  obj->velocityX = 0;
  right->x.fixed = p->x - (16 << 16);
  right->velocityX = 0;

  // 地面から 24 ドットの高さまで持ち上げる
  target->update = ObjectGroup_ApplyRandomChildValues;
  Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
  AudioCommand_PlayFar(130);
  target->motionFlags = 4;
  ObjectDispatch_SetSingleChildFlagsFar(target, 0);
  if (hands[0] != NULL && hands[1] != NULL) {
    while (target->y.fixed - target->terrainHeight <= 24 << 16) {
      obj->y.fixed += 0x6000;
      right->y.fixed += 0x6000;
      target->y.fixed += 0x6000;
      WaitFrames(1);
    }
  }
  hands[0]->speed = 4 << 16;
  hands[0]->acceleration = 0x8000;
  hands[1]->speed = 4 << 16;
  hands[1]->acceleration = 0x8000;
  target->speed = 0x6666;
  target->acceleration = 0x3333;
  target->steeringFlags = 0;
  target->terrainId = 2;

  //------------------------------------------------------------------
  // 方向キーで動かす
  //------------------------------------------------------------------
  for (;;) {
    WaitFrames(1);
    if ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
      break;

    // 方向キーなし : 持ち手を対象の左右に戻す
    angle = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
    if (angle == 0xffff) {
      pos.x = target->x.fixed;
      pos.y = target->y.fixed + (16 << 16);
      pos.z = target->z.fixed;
      Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
      Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 1);
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 1);
      continue;
    }

    // 持ち手をその向きへ 2 ドット寄せ、16 ドット先の地面を調べる
    pos.x = target->x.fixed;
    pos.y = target->y.fixed + (16 << 16);
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(2 << 16, angle, &pos);
    Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
    Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
    Script_WaitForEventTimeoutFar(hands[0]);
    Script_WaitForEventTimeoutFar(hands[1]);
    pos.x = target->x.fixed;
    pos.y = target->terrainHeight;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(16 << 16, angle, &pos);
    i = ScriptObject_CheckOverlapFar(target, &pos);
    /* FAKEMATCH: entering the blocked path from both tests keeps it
     * ahead of the drop path, as the ROM lays them out. Measured: the two
     * tests as one || differ 407 (536 instructions for 531, the
     * allocation changing from the prologue on), under for (;;) or
     * while (1). */
    if (i != 0)
      goto blocked;
    if (ObjProbeUp(target, p) > 0) {
    blocked: // ふさがっている : 16 フレームごとにブザー
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
      if ((gFrameCount & 15) == 0)
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      continue;
    }
    // 空いている : 対象をそこへ動かす
    {
      struct FieldActor *second;
      s32 x;
      s32 z;

      AudioCommand_PlayFar(175);
      x = p->x;
      z = p->z;
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
      ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
      WaitFrames(15);
      target->paused = i;
      target->speed = 0x3333;
      target->acceleration = 0x3333;
      Object_SetMoveTargetFar(target, p->x, p->y, p->z);
      hands[0]->speed = 0x3333;
      hands[0]->acceleration = 0x3333;
      second = hands[1];
      second->speed = 0x3333;
      second->acceleration = 0x3333;
      Object_SetMoveTargetFar(hands[0], p->x + (16 << 16), p->y, p->z);
      Object_SetMoveTargetFar(second, p->x - (16 << 16), p->y, p->z);
      Script_WaitForEventTimeoutFar(target);
      target->x.fixed = x;
      target->z.fixed = z;
      target->velocityX = i;
      target->velocityZ = i;
      WaitFrames(10);
      break;
    }
  }

  //------------------------------------------------------------------
  // 対象の位置から破片を飛ばして持ち手を消し、対象を跳ねさせて落とす
  //------------------------------------------------------------------
  ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
  ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
  Scheduler_RemoveCallback(BattleFx_SpawnFallingParticles);
  AudioCommand_PlayFar(135);
  WaitFrames(15);
  AudioCommand_PlayFar(135);
  WaitFrames(15);
  p->x = target->x.fixed;
  p->y = target->y.fixed + (16 << 16);
  p->z = target->z.fixed;
  {
    struct FieldActor **walk = shards;

    // 破片 : 速さ 2〜3 ドット、8〜32 ドット先のでたらめな方向へ
    for (i = 0; i < SHARD_MAX; i++) {
      obj = Object_Spawn(0x11d, p->x, p->y, p->z);
      *walk++ = obj;
      if (obj != NULL) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                     gBattleFx_FragmentScript);
        obj->speed = Random16() + (2 << 16);
        obj->acceleration = 2 << 16;
        obj->motionFlags = 0;
        dist = Random16() * 24 + (8 << 16);
        Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
      }
    }
  }
  AudioCommand_PlayFar(131);
  ObjectDispatch_ReleaseFar(hands[0]);
  ObjectDispatch_ReleaseFar(hands[1]);

  // 対象の見た目と動きを戻し、上へ 10 ドットの速さで跳ねさせる
  Animation_ApplyChildValuesFar(target, work->savedPalette);
  ObjectDispatch_InitializeFar((struct DispatchObject *)target,
                               work->savedScript);
  target->update = work->savedCallback;
  target->motionFlags = 3;
  target->velocityY = 10 << 16;
  target->bounce = 0x3333;
  target->terrainId = 0;
  src->update = 0;
  Animation_ApplyChildValuesFar(src, 0);
  if (work->childOption != 0) {
    // 上がりきって落ちきるまで (それぞれ最長 90 フレーム) 待ち、輪を出す
    for (i = 0; i < 90 && target->velocityY >= 0; i++)
      WaitFrames(1);
    WaitFrames(1);
    for (i = 0; i < 90 && target->velocityY < 0; i++)
      WaitFrames(1);
    BattleFx_SpawnRadialParticleRing(target);
    BattleFx_PrepareBufferInterpolation();
    WaitFrames(30);
  } else {
    BattleFx_PrepareBufferInterpolation();
  }
}

DEFINE_ITEM_BREAK_EFFECT(RunBattleEffect11, BattleFx_SpawnItemBreakMode1)

struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z,
                                                  s32 angle) {
  struct MotionObject *obj;
  s32 scale;

  AudioCommand_PlayFar(SOUND_ITEM_BREAK);
  obj = Object_Spawn(215, x, y, z);
  if (obj != NULL) {
    obj->scaleY = 0x4000;
    obj->scaleX = 0x4000;
    obj->speedLimit = 0x30000;
    obj->acceleration = 0x30000;
    obj->autoFaceMotion = 0;
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
    scale = obj->scaleX;
    if (scale < 0x10000) {
      do {
        scale += 0x800;
        obj->scaleY = scale;
        obj->scaleX = scale;
        obj->angle += 0x2000;
        WaitFrames(1);
        scale = obj->scaleX;
      } while (scale <= 0xffff);
    }
    obj->angle = angle;
  }
  return obj;
}

// FieldActor と同じ物体。0x64 を符号なし、0x66 を符号つきで読む (FieldActor
// の型ではバイトが変わる)
struct OrbitEffect {
  u8 unk00[6];
  u16 angle; // 進む向き・回る角度
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;                  // 経過フレーム
  s16 pause;                  // 向きを大きく変える残りフレーム
  struct OrbitEffect *anchor; // 回る中心
  void (*update)(struct OrbitEffect *);
};

// ここに共通ソースが入る（ROM の並びのとおり）
#define EFFECT_SPAWN_RESIDENT
#include "../scene/common/effect_spawn.h"
#undef EFFECT_SPAWN_RESIDENT

//--------------------------------------------------------------------------
// 粒子の毎フレーム処理
// 速度で進み、x と z の速度を少しずつ落とし、拡大率と回転を変える
//--------------------------------------------------------------------------
void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;

  effect->x += effect->velocityX;
  effect->y += effect->velocityY;
  effect->z += effect->velocityZ;
  effect->velocityX -= effect->velocityX / 18;
  effect->velocityZ -= effect->velocityZ / 16;
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}

//--------------------------------------------------------------------------
// origin の位置から、水平に輪を描いて広がる粒子を 17 個出す
// x 方向の速度は z 方向の 1.5 倍
//--------------------------------------------------------------------------
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin) {
  struct EffectOptions opt;
  s32 vel[3];
  s32 vz;
  s32 angle;
  u32 i;

  opt.palette = 0;
  opt.update = BattleFx_UpdateParticleMotionAndScale;
  opt.startScaleX = 0xcccc; // 0.8 倍
  opt.startScaleY = 0xcccc;
  i = 0;
  do {
    angle = i << 12; // 1/16 周ずつ
    vel[0] = Trig_Cos(angle) * 3 / 2;
    vel[1] = 0;
    vz = Trig_Sin(angle);
    vel[2] = vz;
    Effect_SpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1], vz,
                         0x01090001, &opt);
    i++;
  } while (i <= 16);
}

//--------------------------------------------------------------------------
// 中心を離れてさまよう（毎フレーム）
// 1 フレームに 2〜3 ドット進み、ときどき向きを大きく変える
// 101 フレームで共通の粒子スクリプトへ移る
//--------------------------------------------------------------------------
void BattleFx_WanderAroundAnchor(struct OrbitEffect *effect) {
  s32 len;
  s32 angle;
  s32 dx;
  s32 dz;

  len = Random16() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(len, Trig_Cos(angle));
  dz = Iwram_MulQ16(len, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) // 1/32 の確率
  {
    effect->pause = ((Random16() << 4) >> 16) + 8; // 8〜23 フレーム
  }
  if (++effect->timer == 101)
    ObjectDispatch_InitializeFar((struct DispatchObject *)effect,
                                 gBattleFx_CommonParticleScript);
}

//--------------------------------------------------------------------------
// 中心のまわりを半径 8 ドットで回る（毎フレーム）
// 121 フレーム回ったら、さまよう動きに切り替える
//--------------------------------------------------------------------------
void BattleFx_CircleAnchor(struct OrbitEffect *effect) {
  struct OrbitEffect *anchor;
  s32 angle;
  s32 r;
  s32 dx;
  s32 dz;

  anchor = effect->anchor;
  r = 0x80000;
  angle = effect->angle;
  dx = Iwram_MulQ16(r, Trig_Cos(angle));
  dz = Iwram_MulQ16(r, Trig_Sin(angle));
  effect->x = anchor->x + dx;
  effect->z = anchor->z + dz;
  effect->angle += 0x800;
  if (++effect->timer == 121) {
    effect->update = BattleFx_WanderAroundAnchor;
    effect->timer = 0;
    effect->pause = 0;
    effect->speed = 0x1999;
    effect->scale = 0x30000;
    effect->angle = Random16();
  }
}

//--------------------------------------------------------------------------
// 毎フレーム縮め、0.5 倍を切ったら位置を戻して処理を外す
//--------------------------------------------------------------------------
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj) {
  s32 scale;

  scale = obj->scaleX - 0x80;
  obj->scaleY = scale;
  obj->scaleX = scale;
  if (scale < 0x8000) {
    Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
    ((struct ScriptMotionObject *)obj)->hook = 0;
  }
}

void BattleFx_CallEffect14(s32 unused) { RunBattleEffect14(); }

//--------------------------------------------------------------------------
// radial.c
// フィールドのエナジー演出：14 番（運ばれる玉と、まわりを回る粒）ほか
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// エナジー 14 番 : 玉を使う者から scene の位置（4 ドット上）まで 10
// フレームで運び、 0.25 倍から大きくする。scene->enabled
// がなければ、かけられた物を縮め、 玉のまわりに 6 フレームおきに粒を 16
// 個出して回らせる
//--------------------------------------------------------------------------
void RunBattleEffect14(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *src = scene->mainObject;
  struct FieldActor *target = scene->child;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  struct FieldActor *ball;
  s32 step;
  struct FieldActor *particle;
  s32 steps = 11;

  step = 0;
  from.x = src->x.fixed;
  from.y = src->y.fixed;
  from.z = src->z.fixed;
  to.x = scene->x;
  to.y = scene->y - (4 << 16);
  to.z = scene->z;
  ball = Object_Spawn(0xda, 0, 0, 0);
  if (ball == 0) {
    return;
  }
  BattleEffect_InitializeSharedScene();
  ObjectDispatch_ApplyArgumentToChildrenFar(ball, 2);
  do {
    s32 scale;
    ball->x.fixed = from.x + step * (to.x - from.x) / 10;
    ball->y.fixed = from.y + step * (to.y - from.y) / 10;
    ball->z.fixed = from.z + step * (to.z - from.z) / 10;
    scale = 0x4000 + step * 0x10ccc / 10; // 0.25 倍から 1.3 倍へ
    ball->scaleX = scale;
    ball->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  ball->scaleX = 0x1b333; // 横 1.7 倍
  ball->scaleY = 0x14ccc; // 縦 1.3 倍
  AudioCommand_PlayFar(163);
  WaitFrames(20);
  if (scene->enabled == 0) {
    if (target != 0)
      target->update = BattleFx_ShrinkObjectScaleUntilHalf;

    // 粒は玉の 4 ドット上から 0.8 ドットずつ高く、3〜8 ドット離して出す
    step = 0;
    do {
      s32 dist;

      pos.x = ball->x.fixed;
      pos.y = ball->y.fixed + step * 0xcccc + (4 << 16);
      pos.z = ball->z.fixed;
      dist = Random16() * 5 + (3 << 16);
      Vector_AddPolarOffset(dist, Random16(), &pos);
      particle = Object_Spawn(0xf9, pos.x, pos.y, pos.z);
      if (particle != 0) {
        particle->update = BattleFx_CircleAnchor;
        particle->linkedObject = ball;
        particle->timer = 0;
        particle->work = 0;
        particle->facing = Random16();
      }
      WaitFrames(6);
      step++;
    } while (step <= 15);
    WaitFrames(20);
    WaitFrames(120);
  }
  ObjectDispatch_ApplyArgumentToChildrenFar(ball, 1);
  WaitFrames(30);
  AudioCommand_PlayFar(136);
  WaitFrames(20);
  ObjectDispatch_ReleaseFar(ball);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// スロットの粒の動き（毎フレーム）
// state 0 : 出た所から 40〜70 ドット離れた所へ向かう
// state 1 : 着くまで待ち、着いたら state 2 へ
// state 2 : scene の位置の 8 ドット上（画面の位置）のまわり 4 ドットへ向かう
// state 3 : 着いたらスロットを空ける
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect) {
  struct BattleFxScene *scene;
  struct EffectVector pos;
  s8 *st;
  s16 angle;
  s32 state;

  scene = gEffectWork;
  st = &effect->state;
top:
  state = *st;
  if (state == 0) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 4 << 16;
    effect->maxSpeed = 4 << 16;
    effect->flag42 = state;
    *st = *st + 1;
    return;
  }
  if (state == 1) {
    if (BattleFx_HasReachedTarget(effect) != 0)
      return;
    *st = *st + 1;
    goto top;
  }
  if (state == 2) {
    pos.x = scene->x;
    pos.y = scene->y + (8 << 16);
    pos.z = scene->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->flag42 = 1;
    *st = *st + 1;
    return;
  }
  if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
    BattleFx_ClearOwnedSlot(effect);
}

//--------------------------------------------------------------------------
// かけられた物（scene->child）に paused を立て、
// BattleEffect_RunFallbackObjectTransition の演出を動かす
//--------------------------------------------------------------------------
void BattleFx_MarkChildAndRunFallbackTransition(s32 unused) {
  struct FieldActor *target = ((struct BattleFxScene *)gEffectWork)->child;

  target->paused = 1;
  BattleEffect_RunFallbackObjectTransition();
}

//--------------------------------------------------------------------------
// fallback.c
// フィールド効果：対象のない場所へ飛ばして散らす
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 使い手から効果の位置へオブジェクトを 10 フレームで飛ばし、16
// 個の粒にして散らす
//--------------------------------------------------------------------------
void BattleEffect_RunFallbackObjectTransition(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct MotionObject *user = scene->mainObject;
  struct MotionObject *obj;
  struct EffectSlot *slot;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  s32 step;
  s32 i;

  scene->y = user->y;
  obj = Object_Spawn(250, 0, 0, 0);
  step = 0;
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
  if (obj == 0)
    return;

  // 使い手の 16 ドット上から、効果の位置の 8 ドット上へ
  BattleEffect_InitializeSharedScene();
  from.x = user->x;
  from.y = user->y + (16 << 16);
  from.z = user->z;
  to.x = scene->x;
  to.y = scene->y + (8 << 16);
  to.z = scene->z;
  for (;;) {
    s32 steps = 11;
    s32 scale;

    obj->x = from.x + (to.x - from.x) * step / 10;
    obj->y = from.y + (to.y - from.y) * step / 10;
    obj->z = from.z + (to.z - from.z) * step / 10;
    scale = BattleFx_Interpolate(0x10000, 0x4000, step); // 0.25 倍から等倍へ
    obj->scaleX = scale;
    obj->scaleY = scale;
    WaitFrames(1);
    step++;
    if (step >= steps)
      break;
  }

  WaitFrames(5);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
  AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SOUND_TRIPLE_TONE_HIGH);

  // その周り 4 ドットに粒を置き、外へ飛ばしてから効果の位置へ戻す
  slot = &scene->slots[0];
  for (i = 0; i < 16; i++) {
    pos.x = obj->x;
    pos.y = obj->y + (8 << 16);
    pos.z = obj->z;
    Camera_WorldToScreen(&pos.x);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos.x);
    EffectSlot_Initialize(slot, 285, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialCamera);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 7);
    slot++;
  }

  pos.x = obj->x;
  pos.y = obj->y + (8 << 16);
  pos.z = obj->z;
  WaitFrames(8);
  ObjectDispatch_ReleaseFar(obj);
  WaitFrames(4);
  WaitFrames(30);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// object_pause.c
// オブジェクトを止めて表の値をでたらめに当て続け、もとの処理に戻す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 止めているあいだの毎フレームの処理 : 表の 8 つからでたらめに選んで当てる
//--------------------------------------------------------------------------
void BattleEffect_SetRandomTableValueOnObject(void *obj) {
  s8 *table = gBattleFx_RandomChildValues;
  s32 r = Random16();
  Animation_ApplyChildValuesFar(obj, table[(u32)(r * 8) >> 16]);
}

//--------------------------------------------------------------------------
// id のオブジェクトを止める
// 毎フレームの処理と、動いていれば最初のアニメーションの param を
// gGameState に覚えておく
//--------------------------------------------------------------------------
void BattleEffect_PauseObject(s32 id) {
  struct FieldActor *obj;
  struct AnimationEntry *entry;

  obj = ObjectTable_Get(id);
  if (obj != NULL) {
    gGameState.pausedUpdate = (u32)obj->update;
    gGameState.pausedPose = 0;
    if (obj->active == 1) {
      entry = ((struct AnimationObject *)obj->sprite)->entries[0];
      if (entry != NULL) {
        gGameState.pausedPose = entry->param;
      }
    }
    obj->update =
        (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject;
    obj->paused = 1;
    ObjectDispatch_ApplyValueToChildrenFar(obj, 0);
  }
}

//--------------------------------------------------------------------------
// id のオブジェクトを、覚えておいた毎フレームの処理と param に戻す
//--------------------------------------------------------------------------
void BattleFx_ResumeObject(s32 id) {
  struct FieldActor *obj = ObjectTable_Get(id);
  if (obj != 0) {
    if (obj->update == (void (*)(union FieldObject *))
                           BattleEffect_SetRandomTableValueOnObject) {
      struct GameState *state = &gGameState;
      obj->update = (void (*)(union FieldObject *))state->pausedUpdate;
      state->pausedUpdate = 0;
      Animation_ApplyChildValuesFar(obj, state->pausedPose);
    }
    obj->paused = 0;
    ObjectDispatch_ApplyValueToChildrenFar(obj, 16);
  }
}

//--------------------------------------------------------------------------
// 今の演出定義の flags が 0xff なら、対象なし (-1) を返す
// id : 対象オブジェクト番号
//--------------------------------------------------------------------------
s32 BattleFx_FilterObjectIdByFlags(s32 id) {
  if (BattleFx_GetFlags(id) == 0xff)
    return -1;
  return id;
}

//--------------------------------------------------------------------------
// run_effect13_hook.c
// エナジー演出 13：オブジェクト 215 を目標まで伸ばし、押し下げて戻す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 対象への演出 (BattleFx_DispatchRequestKind から)
//--------------------------------------------------------------------------
void BattleFx_RunEffect13Hook(s32 unused) { RunBattleEffect13(); }

//--------------------------------------------------------------------------
// オブジェクト 215 を使う者の 16 ドット上から目標へ 10 段で運びながら
// 1/4 倍から等倍にし、20 ドット押し下げて child を (-9, -9) へ移し、
// 19.5 ドット上げてから縮めて戻す
// 目標は scene->enabled なら使う者の前 32 ドット、ほかは scene の位置で、
// どちらも 32 ドット上
//--------------------------------------------------------------------------
void RunBattleEffect13(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct MotionObject *actor = scene->mainObject;
  struct MotionObject *child = scene->child;
  struct MotionObject *object;
  struct FixedPointPosition spawn;
  struct FixedPointPosition origin;
  struct FixedPointPosition target;
  s32 step;
  s32 i;
  s32 steps = 11;

  origin.x = actor->x;
  origin.y = actor->y + (16 << 16);
  origin.z = actor->z;
  if (scene->enabled) {
    target.x = actor->x;
    target.y = actor->y + (32 << 16);
    target.z = actor->z;
    Vector_AddPolarOffset(32 << 16, scene->angle, &target.x);
  } else {
    target.x = scene->x;
    target.y = scene->y + (32 << 16);
    target.z = scene->z;
  }
  spawn.x = scene->x;
  spawn.y = scene->y + (32 << 16);
  spawn.z = scene->z;
  object = Object_Spawn(215, spawn.x, spawn.y, spawn.z);
  if (object == 0)
    return;
  BattleEffect_InitializeSharedScene();
  AudioCommand_PlayFar(138);
  object->angle = actor->angle;
  object->speedLimit = 0x14ccc;
  object->motionFlags = 0;
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
  Animation_ApplyChildValuesFar(object, 1);

  // 目標へ伸ばす
  step = 0;
  do {
    s32 scale;

    object->x = origin.x + (target.x - origin.x) * step / 10;
    object->y = origin.y + (target.y - origin.y) * step / 10;
    object->z = origin.z + (target.z - origin.z) * step / 10;
    scale = BattleFx_Interpolate(0x10000, 0x4000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  WaitFrames(10);
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
  WaitFrames(15);

  // 2 ドットずつ 10 フレーム押し下げる
  for (i = 0; i < 10; i++) {
    object->y -= 2 << 16;
    WaitFrames(1);
  }
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
  AudioCommand_PlayFar(132);
  if (child != 0)
    Object_SetPositionAndResetMotionFar(child, -(9 << 16), child->y,
                                        -(9 << 16));
  WaitFrames(20);

  // 1.5 ドットずつ 13 フレーム上げる
  for (i = 0; i < 13; i++) {
    object->y += 0x18000;
    WaitFrames(1);
  }
  WaitFrames(10);
  AudioCommand_PlayFar(114);

  // 使う者のところへ縮めて戻す
  step = 0;
  do {
    s32 scale;

    object->x = target.x + (origin.x - target.x) * step / 10;
    object->y = target.y + (origin.y - target.y) * step / 10;
    object->z = target.z + (origin.z - target.z) * step / 10;
    scale = BattleFx_Interpolate(0x4000, 0x10000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    WaitFrames(1);
    step++;
  } while (step < steps);
  ObjectDispatch_ReleaseFar(object);
  BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// launch.c
// 戦闘エフェクト 15 番：回って縮む主オブジェクトと、扇状に飛び出す粒
//--------------------------------------------------------------------------

#define SLOT_MAX 24 // エフェクト枠の数 (粒の数)

//--------------------------------------------------------------------------
// 毎フレームの更新 : 0x1000 + timer × 0x50 ずつ回し、timer × 0x50 が 0x1000 に
// 達するまで timer を増やして速める
//--------------------------------------------------------------------------
void BattleFx_UpdateSpinAngle(struct FieldActor *obj) {
  u32 step;

  step = obj->timer * 0x50;
  obj->facing = obj->facing + step + 0x1000;
  if (step < 0x1000)
    obj->timer = obj->timer + 1;
}

//--------------------------------------------------------------------------
// 毎フレームの更新 : 45 度ずつ回りながら縮み、y を 1 ドットずつ増やす
// 拡大率が 0x3000 を下回ったら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDescendingObject(struct FieldActor *obj) {
  u32 scale;

  obj->scaleY -= 0x280;
  scale = obj->scaleX - 0x280;
  obj->facing += 0x2000;
  obj->y.fixed += 1 << 16;
  obj->scaleX = scale;
  if ((s32)scale < 0x3000)
    obj->active = 0;
}

//--------------------------------------------------------------------------
// 粒の動き : 主オブジェクトの画面位置 (z + 8 ドット) から、0xc000 の向きの ±45
// 度へ 120 ドット飛ぶ。状態 1 は着いたら飛びなおし、状態 2 は着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot) {
  struct EffectVector pos;
  struct FieldActor *src;
  s32 state;
  u32 r;

  src = ((struct BattleFxScene *)gEffectWork)->mainObject;
  state = slot->state;

  if (state == 0) {
    pos.x = src->x.fixed;
    pos.y = src->y.fixed;
    pos.z = src->z.fixed;
    Camera_WorldToScreen(&pos);

    slot->x = pos.x;
    slot->z = pos.z + (8 << 16);
    slot->originZ = slot->z;
    slot->originX = slot->x;
    pos.z = slot->originZ;
    pos.x = slot->originX;

    r = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 5 << 16;
    slot->maxSpeed = 5 << 16;
    slot->flag42 = state;
    slot->state++;

    if ((gFrameCount & 2) != 0)
      AudioCommand_PlayFar(246);
  } else if (state == 1) {
    if (BattleFx_HasReachedTarget(slot) == 0)
      slot->state--;
  } else if (state == 2) {
    if (BattleFx_HasReachedTarget(slot) == 0)
      BattleFx_ClearOwnedSlot(slot);
  }
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 15 番
// 主オブジェクトを回して縮めながら、粒を 1 フレームに 1 つずつ 24 個飛ばす
//--------------------------------------------------------------------------
void BattleFx_RunEffect15(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *obj = scene->mainObject;
  struct EffectVector pos;
  struct EffectSlot *slot;
  u8 last;
  u32 i;

  Battle_Reset();
  Motion_CamBounds(-1, -1, -1, 0);
  BattleEffect_InitializeSharedScene();
  WaitFrames(10);
  ObjectMotion_ArmCallback(scene->firstObjectId, 0x4000, 0);
  WaitFrames(30);
  obj->update =
      (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
  AudioCommand_PlayFar(131);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 28);
  WaitFrames(40);

  // 回りはじめる
  AudioCommand_PlayFar(220);
  Animation_ApplyChildValuesFar(obj, 0);
  ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
  obj->update = (void (*)(union FieldObject *))BattleFx_UpdateSpinAngle;
  obj->timer = 0;
  WaitFrames(70);

  // 縮みはじめ、その画面位置から粒を飛ばす
  ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
  obj->motionFlags = 0;
  obj->update = (void (*)(union FieldObject *))BattleFx_UpdateDescendingObject;
  obj->targetX = ACTOR_NO_TARGET;
  pos.x = obj->x.fixed;
  pos.y = obj->y.fixed;
  pos.z = obj->z.fixed;
  Camera_WorldToScreen(&pos);
  i = 0;
  slot = scene->slots;
  do {
    s32 scale;
    EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialLaunch);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object,
                                              (Random16() * 7) >> 16);
    scale = (Random16() >> 1) + 0x13333; // 1.2〜1.7 倍
    slot->scaleY = scale;
    slot->scaleX = scale;
    i++;
    WaitFrames(1);
    slot++;
  } while (i < SLOT_MAX);
  WaitFrames(70);

  // 飛んでいる粒を、着いたら消える状態にする
  i = 0;
  last = 2;
  do {
    if (scene->slots[i].active != 0)
      scene->slots[i].state = last;
    i++;
  } while (i < SLOT_MAX);
  WaitFrames(40);
  BattleFx_PrepareBufferInterpolation();
  WaitFrames(10);
}

void BattleFx_UpdateDescendingParticlePositiveArc(struct FieldActor *particle) {
  struct FieldActor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (gGameState.map == (s32)&SceneId_Clear)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    ObjectDispatch_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > 0x10000)
    scale = 0x10000;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - 0x20000;
  particle->z.fixed = source->z.fixed + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleFx_UpdateDescendingParticleNegativeArc(struct FieldActor *particle) {
  struct FieldActor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (gGameState.map == (s32)&SceneId_Clear)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    ObjectDispatch_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > 0x10000)
    scale = 0x10000;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - 0x20000;
  particle->z.fixed = source->z.fixed - ((0x10000 - scale) * 5) + 0x100000;
}

struct DescendingArcSprite {
  u8 padding00[5];
  u8 flags05_0 : 5;
  u8 flags05_5 : 1;
  u8 layer : 2;
  u8 padding06;
  u8 flags07_0 : 6;
  u8 size : 2;
  union {
    struct {
      u16 tile : 10;
      u16 flags08_10 : 6;
    } half;
    struct {
      u8 tileLow;
      u8 flags09_0 : 2;
      u8 variant : 2;
      u8 flags09_4 : 4;
    } byte;
  } attr;
  u8 padding0c[16];
  u8 resource;
  u8 active : 1;
  u8 flags1d1 : 7;
  u8 padding1e[8];
  u8 animation;
  u8 padding27;
  struct AnimationEntry *frame;
};

void BattleFx_SpawnDescendingArcParticles(struct FieldActor *arc) {
  struct FieldActor *source = arc;
  struct BattleFxScene *scene = gEffectWork;
  struct FieldActor *leader = scene->mainObject;
  struct FieldActor *objects[2];
  struct FieldActor *object;
  struct DescendingArcSprite *sprite;
  s32 i;
  u8 zero;

  for (i = 0; i <= 1; i++) {
    object = Object_CreateFar(26, source->x.fixed, source->y.fixed + 0x400000,
                              source->z.fixed);
    objects[i] = object;
    if (object == NULL)
      continue;
    object->terrainHeight = source->terrainHeight;
    sprite = object->sprite;
    object->motionFlags = 0;
    object->timer = 0;
    zero = 0;
    object->linkedObject = source;
    object->scaleY = 0x6666;
    object->scaleX = 0x6666;
    if (sprite == NULL)
      continue;
    AnimationObjects_SelectAnimationFar(sprite, 0);
    sprite->animation = zero;
    Resource_ResetEntry(sprite->resource);
    sprite->resource = scene->arcTileSlot;
    sprite->active = 1;
    sprite->attr.half.tile = gVramBlockCache[sprite->resource].offset >> 5;
    sprite->flags05_5 = 0;
    sprite->layer = 1;
    sprite->size = 2;
    sprite->frame->frame = zero;
  }
  objects[0]->update = (void *)BattleFx_UpdateDescendingParticleNegativeArc;
  ((struct DescendingArcSprite *)objects[0]->sprite)->attr.byte.variant = 0;
  objects[1]->update = (void *)BattleFx_UpdateDescendingParticlePositiveArc;
  ((struct DescendingArcSprite *)objects[1]->sprite)->attr.byte.variant =
      ((struct DescendingArcSprite *)leader->sprite)->attr.byte.variant;
}

//--------------------------------------------------------------------------
// arcstate.c
// エナジー演出 16 (プルーフ)：使う者の動きと、効果が切れたときのメッセージ
//--------------------------------------------------------------------------

#define PSYNERGY_AVOID 150      // プルーフ
#define ITEM_SACRED_FEATHER 236 // せいなるはね

//--------------------------------------------------------------------------
// 演出の終わり：使う者の表示の flags を 5 フレームの周期で 3 と 1 に切り替える
//--------------------------------------------------------------------------
void BattleFx_UpdateEffect16State(void) {
  struct ArcEffectObject *obj;
  struct AnimationObject *rec;
  struct AnimationEntry *entry;
  s32 phase;

  obj = ObjectTable_Get(gGameState.selectedActor);
  rec = obj->records;
  entry = rec->entries[0];
  phase = gFrameCount % 5;
  if (phase == 0) {
    rec->dirty = 1;
    rec->flags = 3;
  } else if (phase == 2) {
    entry->param = 0; // 色を戻す
    rec->dirty = 1;
    rec->flags = 1;
  }
}

//--------------------------------------------------------------------------
// 演出中の使う者：5 フレームごと (ワールドマップでは 7) に降る弧の粒を出し、
// phase 1 では回る
//--------------------------------------------------------------------------
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *obj) {
  s16 phase;
  s32 cnt;

  phase = obj->phase;
  cnt = obj->step++;

  if (gGameState.map == (s32)&SceneId_Clear) {
    if (cnt % 7 == 0)
      BattleFx_SpawnDescendingArcParticles(obj);
  } else if (cnt % 5 == 0) {
    BattleFx_SpawnDescendingArcParticles(obj);
  }

  if (phase == 1)
    obj->angle += 0xc00;
}

//--------------------------------------------------------------------------
// プルーフ (せいなるはね) の効果が切れた
//--------------------------------------------------------------------------
void FieldEvent_ShowStatusMessage(void) {
  gGameState.avoidDistance = 0;
  if (gGameState.avoidByItem == 0) {
    UiWork_PushValueSlotFar(PSYNERGY_AVOID, 4);
    UiText_ShowPositionedMessageAndWaitFar(MsgAbilityWoreOff, 1);
    return;
  }
  UiWork_PushValueSlotFar(ITEM_SACRED_FEATHER, 2);
  UiText_ShowPositionedMessageAndWaitFar(MsgItemWoreOff, 1);
}

void RunBattleEffect16(void) {
  struct BattleFxScene *scene;
  struct ArcEffectObject *object;
  struct AnimationObject *group;
  struct AnimationEntry *entry;
  u32 angle;
  s32 slot;
  s32 count;

  scene = gEffectWork;
  object = scene->mainObject;
  group = object->records;
  entry = group->entries[0];
  angle = object->angle;
  slot = Resource_FindFreeEntry();
  {
    s16 *tileSlot = &scene->arcTileSlot;
    s32 zero = 0;

    *tileSlot = slot;
    VramBlock_LoadCached((s16)slot, 0x100, gBattleFx_ArcSparkTiles);
    gGameState.avoidDistance = 0x09600000;
    gGameState.avoidByItem = GameFlag_TestFar(0x145);
    Animation_ApplyChildValuesFar(object, zero);
    object->callback = BattleFx_UpdatePairedArcSpawner;
    object->phase = zero;
    object->step = zero;
  }
  AudioCommand_PlayFar(0x8c);
  WaitFrames(15);
  object->phase = 1;
  WaitFrames(10);
  for (count = 0; count < 20; count++) {
    entry->param = 7;
    group->dirty = 1;
    WaitFrames(2);
    group->dirty = 1;
    entry->param = 0;
    group->flags = 1;
    WaitFrames(3);
  }
  object->callback = NULL;
  object->angle = angle;
  Scheduler_AddOrUpdateCallback(BattleFx_UpdateEffect16State, 0xc80);
  WaitFrames(15);
  AudioCommand_PlayFar(0xae);
  WaitFrames(55);
  Scheduler_RemoveCallback(BattleFx_UpdateEffect16State);
  if (gGameState.cloaked != 0)
    ObjectDispatch_SetSingleChildFlagsFar(object, 2);
  else
    ObjectDispatch_SetSingleChildFlagsFar(object, 1);
  Animation_ApplyChildValuesFar(object, 0);
  Resource_ResetEntry(scene->arcTileSlot);
  UiText_ShowPositionedMessageAndWaitFar(MsgMonstersAttackLess, 1);
}

void EffectSlot_Update(struct EffectSlot *effect) {
  if (effect->active != 0) {
    effect->age++;
    if (effect->callbackDelay != 0)
      effect->callbackDelay--;
    else if (effect->callback != 0)
      effect->callback(effect);
    if (effect->active != 0) {
      if (effect->updateMotion != 0)
        EffectSlot_UpdateMotion(effect);
      if (effect->render != 0)
        BattleFx_DrawScaledObject(effect);
    }
  }
}

void BattleFx_DrawScaledObject(struct EffectSlot *object) {
  struct AnimationObject *sprite;
  s32 offset;
  s32 scale[2];
  s32 position[4];

  sprite = object->object;
  offset = 0;
  if (object->flags & 4)
    offset = 0x1fc0000 - object->z;
  scale[0] = Iwram_MulQ16(object->scaleX, sprite->scale);
  scale[1] = Iwram_MulQ16(object->scaleY, sprite->scale);
  position[0] = object->x;
  position[1] = offset;
  position[2] = object->z + offset;
  position[3] = 0;
  if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000 &&
      object->z < 0xe00000)
    Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

void EffectSlot_UpdateMotion(struct EffectSlot *effect) {
  s32 dx;
  s32 dz;
  s32 distance;
  s32 ix;
  s32 iz;
  s32 speed;
  s16 angle;
  s32 heading;
  s32 facing;
  s16 turn;

  if (effect->targetX == EFFECT_NO_TARGET)
    return;
  dx = effect->targetX - effect->x;
  dz = effect->targetZ - effect->z;
  if (effect->stopAtTarget != 0) {
    ix = dx / 0x10000;
    iz = dz / 0x10000;
    distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
    if (distance < 0x800000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    if (distance <= 0x80000) {
      EffectSlot_SetPosition(effect, effect->targetX, effect->targetZ);
      return;
    }
  }
  angle = ArcTan2(dz, dx);
  if (effect->flag42 != 0) {
    heading = effect->heading;
    turn = angle - heading;
    if ((turn >= 0 ? turn : -turn) >= effect->maxTurnStep) {
      if (turn < 0) {
        if (-turn > effect->maxTurnStep)
          turn = -effect->maxTurnStep;
      } else if (turn > effect->maxTurnStep) {
        turn = effect->maxTurnStep;
      }
      angle = turn + heading;
    }
  }
  facing = (u16)angle;
  effect->heading = facing;
  speed = effect->speed + effect->acceleration;
  if (speed > effect->maxSpeed)
    speed = effect->maxSpeed;
  angle = facing;
  effect->speed = speed;
  effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
  effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

u32 BattleFx_HasReachedTarget(struct EffectSlot *effect) {
  u32 value;

  if (effect->stopAtTarget == 0) {
    return 0;
  }
  value = effect->targetX ^ 0x80000000;
  return ((0u - value) | value) >> 31;
}

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z) {
  effect->targetX = EFFECT_NO_TARGET;
  effect->targetZ = EFFECT_NO_TARGET;
  effect->x = x;
  effect->z = z;
  effect->speed = 0;
}

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode) {
  AnimationObjects_SelectAnimationFar(effect->object, mode);
}

void EffectSlot_SetCallback(struct EffectSlot *effect,
                            EffectCallback callback) {
  effect->callback = callback;
  effect->callbackDelay = 0;
  effect->age = 0;
  effect->state = 0;
}

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z) {
  s32 unit;
  s8 *object;

  Dma_Fill32(0, effect, sizeof *effect);
  object = ResourceObject_CreateFar(kind);
  effect->object = object;
  if (object != NULL)
    object[9] &= ~0x0c;
  EffectSlot_SetPosition(effect, x, z);
  /* FAKEMATCH: register choice only: 1.0 is held in a local set before
     maxSpeed, so it lives beside 2.0 and takes r2, which lets sched2
     load the object ahead of the three unit stores, as native. Plain
     forms measured (2026-10-13): the chain with the constant, three
     separate stores, or the chain before maxSpeed give 1.0 r3 and the
     reload after the stores (differ 12). */
  unit = 0x10000;
  effect->maxSpeed = 0x20000;
  effect->acceleration = effect->scaleY = effect->scaleX = unit;
  effect->originX = x;
  effect->originZ = z;
  ((s8 *)effect->object)[38] = 0;
  effect->stopAtTarget = 1;
  effect->flag42 = 1;
  effect->updateMotion = 1;
  effect->render = 1;
  effect->active = 1;
  effect->randomValue = Random16();
  effect->flags = 4;
  EffectSlot_SetObjectMode(effect, 1);
}

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot) {
  if (slot->object)
    ResourceObject_ReleaseFar(slot->object);
  Dma_Fill32(0, slot, sizeof *slot);
}

// ワールドマップの作業領域（gMapCellBuffer に置く）
struct WorldMapWork {
  u16 vramBlock;           // 印の OBJ キャラクタの VRAM ブロック
  u16 windowSlot;          // もうひとつの VRAM ブロック（終わりに返す）
  union FieldCoordinate x; // カーソル（ドット）
  union FieldCoordinate z;
  u8 padding0c[6];
  s16 shown; // いま地名ウインドウに出している印（-1 はなし）
  u8 padding14[4];
  s32 speed; // カーソルの速さ
  s32 window;
  struct AnimationSpritePart markers[67];
};

void BattleFx_SetupResourcesAndWindow(void) {
  u32 *oam;
  u32 *pal;
  struct WorldMapWork *work;
  struct FieldActor *object;
  s32 attr;
  u32 i;
  u32 tile;
  u32 attr2;

  pal = Runtime_BumpAllocateAlternatePool(32);
  work = (struct WorldMapWork *)gMapCellBuffer;
  oam = (u32 *)work->markers;
  object = Object_GetById(gGameState.selectedActor);
  work->vramBlock = Resource_FindFreeEntry();
  Dma_Fill32(0, pal, 0x80);
  pal[0] = 255;
  pal[1] = 0x1ff;
  pal[2] = 272;
  pal[8] = 68;
  pal[9] = 324;
  pal[10] = 272;
  pal[16] = 119;
  pal[17] = 0x177;
  pal[18] = 272;
  pal[24] = 0xff0;
  pal[25] = 0xffff;
  pal[26] = 0x1ffff;
  pal[27] = 0x11ff0;
  pal[28] = 0x1100;
  attr = VramBlock_LoadCached(work->vramBlock, 128, pal) | 0x400;
  for (i = 0; i < 66; i++) {
    *oam++ = 0;
    *oam++ = 0;
    *oam++ = attr;
  }
  MemFree(pal);
  if (GameFlag_TestFar(284)) {
    work->x.fixed = 240 << 15;
    work->z.fixed = 160 << 15;
  } else {
    work->x.fixed = (((object->x.fixed - 0x10000000) >> 16) * 240 / 4096) << 16;
    work->z.fixed = ((object->z.part.pixel * 160) / 4096) << 16;
  }
  work->windowSlot = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gVramBlockCache[work->windowSlot].offset;
  work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
  work->shown = -1;
  work->speed = 0x10000;
  *oam++ = 0;
  *oam++ = 0x40000000;
  attr2 = 0x400;
  attr2 |= tile >> 5;
  *oam = attr2;
}

//--------------------------------------------------------------------------
// cleanup_resources_and_window.c
// ワールドマップ：地名の印・カーソル・地名ウインドウ
//--------------------------------------------------------------------------

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
// キーを離すとカーソルが近くの地名へ寄る
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64 // 地名を出す距離の 2 乗
#define WORLD_MAP_NAME_LEFT 3   // 地名ウインドウを印から左へずらす量
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

#define FLAG_NOT_ON_MAP 284 // いまいる場所が地図にのっていない

//--------------------------------------------------------------------------
// ワールドマップを閉じる：VRAM ブロックを返し、地名ウインドウを消す
//--------------------------------------------------------------------------
void BattleFx_CleanupResourcesAndWindow(void) {
  struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

  Resource_ResetEntry(work->vramBlock);
  Resource_ResetEntry(work->windowSlot);
  UiWork_FinalizeFar(work->window, 2);
}

//--------------------------------------------------------------------------
// ワールドマップの毎フレーム処理（スケジューラのコールバック）
// フラグの立った場所に印を出し、十字キーでカーソルを動かし、
// カーソルにいちばん近い印の地名をウインドウに出す
// L・R を押している間はカーソルを現在地に合わせる
//--------------------------------------------------------------------------
void Map_UpdateWorldMapMarkers(void) {
  s32 leader;
  const s32 *place;
  s32 tileBase;
  s32 blend;
  s32 best;
  s32 bestDist;
  s32 cursorX;
  s32 bestX;
  s32 bestY;
  s32 message;
  s32 bestMessage;
  s32 height;
  s32 width;
  s32 pos[3];
  struct WorldMapWork *work;
  struct FieldActor *obj;
  s32 cursorY;
  s32 tile;
  s32 mode;
  s32 flag;
  s32 i;
  struct AnimationSpritePart *marker;
  s32 x;
  s32 y;
  s32 angle;

#if WORLD_MAP_CURSOR_SNAPS
  leader = gGameState.selectedActor;
  work = (struct WorldMapWork *)Ram_MapCellBuffer;
  marker = work->markers;
  place = gWorldMap_PlaceMarkers;
  tileBase = gVramBlockCache[work->vramBlock].offset >> 5;
#else
  work = (struct WorldMapWork *)Ram_MapCellBuffer;
  leader = gGameState.selectedActor;
  place = gWorldMap_PlaceMarkers;
  tileBase = gVramBlockCache[work->vramBlock].offset >> 5;
  marker = work->markers;
#endif
  best = -1;
  bestDist = WORLD_MAP_NAME_RANGE;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

  //------------------------------------------------------------------
  // カーソル
  //------------------------------------------------------------------
  if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS)) {
    // L・R：現在地
    obj = ObjectTable_Get(gGameState.selectedActor);
    if (obj != NULL) {
      cursorX = ((obj->x.fixed - 0x10000000) >> 16) * 240 / 4096;
      cursorY = obj->z.part.pixel * 160 / 4096;
    }
  } else {
    // 十字キー：押し続けると速くなる
    angle = gWorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
    if (angle != 0xffff) {
      pos[0] = work->x.fixed;
      pos[1] = 0;
      pos[2] = work->z.fixed;
      Vector_AddPolarOffset(work->speed, angle, pos);
      if (pos[0] < 0x100000) // x は 16〜239 ドット
        pos[0] = 0x100000;
      if (pos[0] > 0xef0000)
        pos[0] = 0xef0000;
      if (pos[2] < 0) // y は 0〜141 ドット
        pos[2] = 0;
      if (pos[2] > 0x8d0000)
        pos[2] = 0x8d0000;
      work->x.fixed = pos[0];
      work->z.fixed = pos[2];
      if (work->speed < 0x60000)
        work->speed += 0x2000;
    } else {
      work->speed = 0x10000;
    }
    cursorX = work->x.part.pixel;
    cursorY = work->z.part.pixel;
  }

  //------------------------------------------------------------------
  // 印：0 番は現在地、11〜65 番は gWorldMap_PlaceMarkers の場所
  // 表は（フラグ, 地名）の組で、フラグの上位 16bit はキャラクタ番号
  //------------------------------------------------------------------
  for (i = 0; i <= 65; i++) {
    if (i != leader && i <= 10)
      continue;
    if (i == 0) {
      if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
        continue;
      tile = 0;
      flag = leader;
      mode = 0;
    } else {
      flag = *place++;
      message = *place++;
      if (flag == 0)
        continue;
#if WORLD_MAP_CURSOR_SNAPS
      if (flag == -1)
        break;
      tile = flag >> 16;
      mode = 1;
#else
      tile = flag >> 16;
      mode = 1;
      if (flag == -1)
        break;
#endif
    }
    if (!GameFlag_TestFar(flag))
      continue;
    obj = ObjectTable_Get(i);
    if (obj == NULL)
      continue;

    // ワールドの 4096 ドット四方を 240x160 に縮める
    x = ((obj->x.fixed - 0x10000000) >> 16) * 240 / 4096;
    y = obj->z.part.pixel * 160 / 4096;
    marker->objectMode = mode;
    marker->tile = tileBase + tile;
    marker->x = x - 1;
    marker->y = y - 1;
    width = x - cursorX;
    height = y - cursorY;
    if (width * width + height * height < bestDist) {
      bestMessage = message;
      best = i;
      bestDist = width * width + height * height;
      bestX = x;
      bestY = y;
    }
    if (i != 0 || (gFrameCount & 15) <= 7) // 現在地は 8 フレームごとに点滅
      Runtime_PushSlotEntry(marker++, y);
  }
#if WORLD_MAP_CURSOR_SNAPS
  // キーを離していれば、カーソルをいちばん近い印へ寄せる
  if (gKeysHeld == 0) {
    if (best != -1) {
      if (bestDist <= 1) {
        work->x.fixed = bestX << 16;
        work->z.fixed = bestY << 16;
        work->speed = 0x10000;
      } else {
        if (work->speed < 0x20000)
          work->speed = 0x20000;
        angle = ArcTan2(bestY - cursorY, bestX - cursorX);
        pos[0] = work->x.fixed;
        pos[1] = 0;
        pos[2] = work->z.fixed;
        Vector_AddPolarOffset(work->speed, (u16)angle, pos);
        work->x.fixed = pos[0];
        work->z.fixed = pos[2];
        if (work->speed < 0x60000)
          work->speed += 0x2000;
      }
      cursorX = work->x.part.pixel;
      cursorY = work->z.part.pixel;
    } else {
      work->speed = 0x10000;
    }
  }
#endif

  // いちばん近い印を囲む枠（点滅）
  if (best != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = 0;
    marker->tile = tileBase + 3;
    marker->x = bestX - 2;
    marker->y = bestY - 2;
    Runtime_PushSlotEntry(marker, bestY);
  }

  // カーソル
  marker = &work->markers[66];
  marker->affineIndex = 0;
  marker->x = cursorX - 17;
  marker->y = cursorY + 1;
  Runtime_PushSlotEntry(marker, 246);

  //------------------------------------------------------------------
  // 地名ウインドウ：印が変わったときだけ書き直す
  //------------------------------------------------------------------
  if (work->shown != best) {
    work->shown = best;
    RenderOutput_PrepareForRedrawFar(work->window);
    if (best != -1) {
      if (best == 0)
        bestMessage = MsgPresentLocation;
      else
        bestMessage =
            Scene_GetAreaNameIndex(bestMessage, 1) + MsgDebugEntryName;
      UiText_MeasureResourceEntriesFar(bestMessage, &width, &height);
      cursorX = bestX;
      cursorY = bestY;
      cursorX -= WORLD_MAP_NAME_LEFT;
      cursorY -= 11;
      if (cursorX + width > 240) {
        // 右へはみ出すなら、印の左上へ回す
        cursorX = 232 - width;
        cursorY = bestY - 20;
      }
      if (cursorY < 0)
        cursorY = 0;
      UiText_DrawResourceFar(bestMessage, work->window, cursorX, cursorY);
    }
  }

  // 印の半透明を明滅させる
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

void Map_ShowWorldMap(s32 picture) {
  struct MapState *map = WORK_SLOT(8);
  struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);
  struct ObjectSystemWork *menu = WORK_SLOT(HEAP_SLOT_MENU_CONTROL);
  s32 resource = (s32)&ResourceId_WorldMapPicture;
  struct MapAnimation *layer;
  u8 savedFlags[16];
  s32 i;
  u8 *graphics;
  s32 savedStatus;
  s16 blend;

  if (field->mode != 3)
    return;
  field->viewCenter->paused = 1;
  savedStatus = field->transitionFrames;
  field->transitionFrames = 6;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  layer = map->anim;
  for (i = 0; i < 16; i++) {
    savedFlags[i] = layer->paused;
    layer->paused = 1;
    layer++;
  }
  menu->suspended = 1;
  WaitFrames(1);
  blend = REG_BLDALPHA;
  graphics = Resource_GetTableEntry(resource);
  Dma_Set(graphics, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | (448 / 4),
          REG_DMA3);
  BG_PLTT[0] = 0;
  Resource_DecodeType01(graphics + 448, gMapCellBuffer);
  Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
          DMA_ENABLE32 | DMA_32BIT | (240 * 160 / 4), REG_DMA3);
  Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2, &REG_BG1CNT);
  Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                 DISPCNT_OBJ_1D_MAP,
             &REG_DISPCNT);
  BattleFx_SetupResourcesAndWindow();
  Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers, 0xc80);
  if (GameFlag_TestFar(284))
    UiText_ShowPositionedMessageAndWaitFar(MsgNotOnMap, 1);
  do {
    WaitFrames(1);
  } while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
  Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
  BattleFx_CleanupResourcesAndWindow();
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  Map_LoadAreaGraphicsFar();
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, 0x20000);
  layer = map->anim;
  for (i = 0; i < 16; i++) {
    layer->paused = savedFlags[i];
    layer++;
  }
  menu->suspended = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  field->transitionFrames = savedStatus;
  field->viewCenter->paused = 0;
}

void BattleFx_UpdateObjectVisibilityBounds(void) {
  struct ObjectRuntime *object;
  s32 x;
  s32 y;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  u32 id;

  // the selected actor is looked up and the result discarded, as the ROM does
  Object_GetById(gGameState.selectedActor);
  object = (struct ObjectRuntime *)gEventWork->viewCenter;
  x = object->x;
  left = x - (320 << 16);
  right = x + (320 << 16);
  y = object->z;
  top = y - (600 << 16);
  bottom = y + (400 << 16);

  id = 8;
  do {
    object = ObjectTable_Get(id);

    if (object != 0) {
      s32 ox = object->x;
      s32 oy = object->z;

      if (ox < left || ox > right || oy < top || oy > bottom) {
        object->animationKind = 0;
      } else {
        object->animationKind = 1;
      }
    }
    id++;
  } while (id <= 65);
}

void BattleFx_RunVisibilityTransition(void) {
  struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);

  if (field->mode == 3) {
    Scheduler_EnableUnmaskedOverlayCallbacks();
    BattleFx_UpdateObjectVisibilityBounds();
    Battle_InitializeRenderObject();
    BattleFx_ScheduleRatioTransition(0x9d89, 6);
    if (gKeysHeld & KEY_L) {
      do {
        WaitFrames(1);
      } while (gKeysHeld & KEY_L);
    }
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Scheduler_DisableOverlayCallbacks();
  }
}
