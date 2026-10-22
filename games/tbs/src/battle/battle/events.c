/// @file
/// Battle events: a downed unit's reset, waiting for the player, and the
/// event queue, filled by BattleEvPush and played at once by
/// BattleEvent_DispatchQueued.
// Part of battle.c.

#include "../battle_internal.h"

/// Clears every ailment and stat change of a unit that goes down, recalculates
/// its stats and rebuilds its status icons.
/// Reached through the BATTLE_EVENT_DEFEAT event, from
/// BattleEvent_DispatchQueued and BattleEvent_Playback. Clears the unit's
/// element levels, poison, stat stages and their turns, every status, its
/// equipment bonuses, ready, immobile and challenge; UnitCalcFar then
/// recomputes its stats, and BattleUnit_BuildStatusFlags its battle object's
/// status flags.
#ifdef UBFIX
void BattleObject_ResetRuntimeFields(s32 unitId) {
#else
s32 BattleObject_ResetRuntimeFields(s32 unitId) {
#endif
  struct BattleUnit *unit;
  s32 element;

  unit = UnitGetFar(unitId);
  for (element = ELEMENT_COUNT - 1; element >= 0; element--)
    unit->elementLevels[element] = 0;

  unit->poison = 0;
  unit->attackTurns = 0;
  unit->attackStage = 0;
  unit->defenseTurns = 0;
  unit->defenseStage = 0;
  unit->resistanceTurns = 0;
  unit->resistanceStage = 0;
  unit->delusion = 0;
  unit->confusion = 0;
  unit->charm = 0;
  unit->stun = 0;
  unit->sleep = 0;
  unit->psySeal = 0;
  unit->refrain = 0;
  unit->reflect = 0;
  unit->haunt = 0;
  unit->grimReaper = 0;
  unit->equipBonus[0] = 0;
  unit->equipBonus[1] = 0;
  unit->ready = 0;
  unit->immobile = 0;
  unit->agilityTurns = 0;
  unit->agilityStage = 0;
  unit->challenge = 0;

  UnitCalcFar(unitId);
  BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
}

// XXX: likely only a second spelling of struct BattlePromptSprite, because
// BattlePresentation_WaitForAdvance written through that struct as
// playback.c's PLAY_ADVANCE writes it (both words through a u32 cast, then
// tile, x and y) shows no difference in an alchemy-devtools diff of any
// edition; set field by field, 101 of 138 instructions differ in tbs-en. The
// twelve-edition comparison would settle whether this view can go.

/// The advance arrow's OAM entry, as plain halfwords: a second view of struct
/// BattlePromptSprite. BattlePresentation_WaitForAdvance keeps it on its
/// stack and rebuilds it each frame before Runtime_PushSlotEntry queues it.
struct AdvanceSprite {
  struct AdvanceSprite *next; ///< never set here
  union {
    struct {
      u16 attr0; ///< OAM attribute 0: y in the low byte
      u16 attr1; ///< OAM attribute 1: x in the low nine bits
      u16 attr2; ///< OAM attribute 2: tile in the low ten bits
      u16 attr3; ///< cleared with attr2 each frame
    } fields;
    u32 words[2]; ///< paired attributes, initialized together
    u8 bytes[8];  ///< the y byte can change without replacing attribute 0
  } attributes;
};

/// The advance arrow's attribute 0: a 256-colour, semi-transparent tall
/// sprite; with attribute 1's size 0 it is 8 by 16 pixels.
#define ADVANCE_ARROW_ATTR0 0xa400

/// Shows the bobbing arrow at the message window and waits for the player to
/// go on (B, or one of KEY_ADVANCE), then plays the cursor sound.
/// Called after a message by BattlePresentation_DispatchAction and the
/// staging runners, BattleCommand_BuildPlan (a flight),
/// BattleIntro_AnnounceEncounter, the round's end (turn_end.c), and
/// BattleEvent_DispatchQueued for each BATTLE_EVENT_MESSAGE. Waits for the
/// message window first. Takes a 128-byte VRAM cache entry for the arrow's
/// tiles and frees it; each frame queues the WINOUT writes, sets REG_BLDALPHA
/// and pushes the arrow sprite.
#ifdef UBFIX
void BattlePresentation_WaitForAdvance(void) {
#else
s32 BattlePresentation_WaitForAdvance(void) {
#endif
  struct AdvanceSprite *prompt;
  struct AdvanceSprite promptStorage;
  s32 frame;
  s32 tileSlot;
  s32 frameTiles;
  struct UiWindow *window;
  struct BattleDisplayOffset *offset;
  s32 attr2;
  s32 attr1;
  s32 x;
  s16 tile;

  while (!UiWork_IsCompleteFar())
    WaitFrames(1);
  prompt = &promptStorage;
  tileSlot = VramCache_AllocEntry(128);
  for (frame = 0;; frame++) {
    // Cycle the eight frames, one every four frames.
    frameTiles = ((gFrameCount >> 2) & 7) * 128 +
                 (s32)gBattlePresentation_AdvanceArrowTiles;
    window = gBattleDisplayWork->window;
    offset = gBattleDisplayWork->offset;
    // Outside windows 0 and 1, sprites show (WINOUT bit 4) and BG2 does not
    // (bit 2); the semi-transparent arrow blends at full weight.
    IoWriteQueue_ClearBits16((u32)&REG_WINOUT, 4);
    IoWriteQueue_SetBits16((u32)&REG_WINOUT, 16);
    Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
    prompt->attributes.words[0] = ADVANCE_ARROW_ATTR0;
    prompt->attributes.words[1] = 0;
    tile = VramCache_Reload(tileSlot, frameTiles) & 0x3ff;
    attr2 = prompt->attributes.fields.attr2;
    prompt->attributes.fields.attr2 = (attr2 & ~0x3ff) | tile;
    // The window is placed in tiles, its offset in 1/256 pixels.
    x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
    attr1 = prompt->attributes.fields.attr1;
    prompt->attributes.fields.attr1 = (attr1 & ~0x1ff) | x;
    // Bob up and down by up to 2 pixels, once every 16 frames (a sixteenth
    // of a turn a frame).
    prompt->attributes.bytes[0] = Trig_Sin(gFrameCount << 12) / 32768 +
                                  window->y * 8 + (offset->y >> 8) + 6;
    Runtime_PushSlotEntry(prompt, 240);
    // A held advance key counts only after 16 frames, so one still held from
    // the previous message does not skip this one; B held goes on at once.
    if ((gKeysHeld & KEY_B) || (gKeysPressed & KEY_ADVANCE) ||
        (frame > 15 && (gKeysHeld & KEY_ADVANCE)))
      break;
    WaitFrames(1);
  }
  AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
  Resource_ResetEntry(tileSlot);
  WaitFrames(1);
}

/// Shows the cursor bobbing at (x, y), in screen pixels, and waits for one of
/// KEY_ADVANCE. Called by Battle_RunEncounter under the Djinn tutorial's
/// standby message, and by Menu_RunCommandRow through
/// BattlePresentation_WaitForPromptAtFar. Waits for the message window
/// first; takes a 128-byte VRAM cache entry for the cursor's tiles and frees
/// it, and each frame queues the WINOUT writes, sets REG_BLDALPHA and pushes
/// the cursor sprite.
#ifdef UBFIX
void BattlePresentation_WaitForPromptAt(s32 x, s32 y) {
#else
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y) {
#endif
  struct MenuPoint position;
  s32 frameTiles;
  struct CursorSprite promptStorage;
  struct CursorSprite *prompt;
  s32 tileSlot;

  frameTiles = Ui_GetTableWordZeroFar(0);
  position.x = x;
  position.y = y;
  while (!UiWork_IsCompleteFar())
    WaitFrames(1);
  prompt = &promptStorage;
  tileSlot = VramCache_AllocEntry(128);
  while (1) {
    IoWriteQueue_ClearBits16((u32)&REG_WINOUT, 4);
    IoWriteQueue_SetBits16((u32)&REG_WINOUT, 16);
    Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
    ((s32 *)prompt)[1] = MENU_SPRITE_SIZE_16;
    ((s32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, frameTiles);
    // Nudge it up and right every four frames.
    prompt->x = position.x + ((gFrameCount & 4) >> 1) - 4;
    prompt->y = position.y - ((gFrameCount & 4) >> 2) - 8;
    Runtime_PushSlotEntry(prompt, 240);
    if (gKeysPressed & KEY_ADVANCE)
      break;
    WaitFrames(1);
  }
  Resource_ResetEntry(tileSlot);
  WaitFrames(1);
}

/// Sets gBattleDisplayWork->marked, which the next message reads, as
/// BATTLE_EVENT_MARK does.
/// Called by BattleCommand_BuildPlan between a weapon's two unleash messages.
void BattleEvent_MarkDisplay(void) { gBattleDisplayWork->marked = 1; }

/// Takes an enemy out of the battle at once (one that leaves rather than
/// falls): its HP to 0, out of the lists, and its object released.
/// Reached through the BATTLE_EVENT_REMOVE_UNIT event, which BattleTargetExec
/// queues for a self-destructing actor, from BattleEvent_DispatchQueued and
/// BattleEvent_Playback. A party unit is left as it is. Clears the battle
/// object's object and active and releases its dispatch object.
#ifdef UBFIX
void BattleObject_DestroyTemporaryObject(s32 unitId) {
#else
s32 BattleObject_DestroyTemporaryObject(s32 unitId) {
#endif
  struct BattleUnit *enemy;
  struct BattleObj *slot;

  enemy = UnitGetFar(unitId);
  if (enemy->side == BATTLE_UNIT_ENEMY) {
    Unit_SetHpFar(enemy, 0);
    BattleObject_RemoveFromLists(unitId);
    BattleObject_MarkForRelease(unitId);
    slot = BattleObjGet(unitId);
    ObjectDispatch_ReleaseFar((struct DispatchObject *)slot->object);
    slot->object = 0;
    slot->active = 0;
  }
}

/// BattleEventState.flags bit 0: a Djinni struck a weakness, so enemies that
/// fall pay the Djinn bonus (BattleEnemy_RecordDefeat).
#define EVENT_FLAG_DJINN_BONUS 1

/// The BATTLE_EVENT_DJINN_BONUS event: grants the Djinn bonus for the rest of
/// the action. Its operand is ignored.
/// Called for the event by BattleEvent_DispatchQueued and BattleEvent_Playback;
/// BattleTargetExec queues it. Sets EVENT_FLAG_DJINN_BONUS in runtime->flags,
/// which BattleEvent_Reset clears.
void BattleEvent_GrantDjinnBonus(struct BattleEventState *runtime,
                                 u32 operand) {
  runtime->flags |= EVENT_FLAG_DJINN_BONUS;
}

/// Plays the queued battle events at once, in order (BattleEvent_Playback
/// plays them over frames instead): the messages with their arguments, the
/// hit flashes, the downed units, the redraws. Then empties the queue.
/// Called where an outcome is shown outside an effect: by the staging runners
/// (a blocked cast, a dodge, a broken item, a plan's targets one by one),
/// BattleCommand_BuildPlan's debug defeat, and the turn's end (turn_end.c).
/// Each message, flash and fall waits its frames here. Ends with
/// BattleEvent_Reset.
#ifdef UBFIX
void BattleEvent_DispatchQueued(void) {
#else
u32 BattleEvent_DispatchQueued(void) {
#endif
  struct BattleEventState *runtime = &gBattleWork->events;
  struct BattleEventQueue *queue = &runtime->queue;
  s32 eventIndex;

  for (eventIndex = 0; eventIndex < queue->count; eventIndex++) {
    u8 opcode = queue->opcodes[eventIndex];
    // BATTLE_EVENT_SOUND, the last opcode, is skipped: played at once, the
    // queue makes no sounds but a hit's.
    if (opcode <= BATTLE_EVENT_DJINN_BONUS)
      switch (opcode) {
      case BATTLE_EVENT_DJINN_BONUS:
        BattleEvent_GrantDjinnBonus(runtime, queue->operands[eventIndex]);
        break;
      case BATTLE_EVENT_REMOVE_UNIT:
        BattleObject_DestroyTemporaryObject(queue->operands[eventIndex]);
        break;
      case BATTLE_EVENT_UNIT_NAME:
        UiWork_PushValueSlotFar(queue->operands[eventIndex], VALUE_UNIT_NAME);
        break;
      case BATTLE_EVENT_NUMBER:
        UiWork_PushValueSlotFar(queue->operands[eventIndex], VALUE_NUMBER);
        break;
      case BATTLE_EVENT_ITEM_NAME:
        UiWork_PushValueSlotFar(queue->operands[eventIndex] & ITEM_ID_MASK,
                                VALUE_ITEM_NAME);
        break;
      case BATTLE_EVENT_ABILITY_NAME:
        UiWork_PushValueSlotFar(queue->operands[eventIndex] &
                                    OWNER_ACTION_ID_MASK,
                                VALUE_ABILITY_NAME);
        break;
      case BATTLE_EVENT_MARK:
        gBattleDisplayWork->marked = 1;
        break;
      case BATTLE_EVENT_CLEAR_ARGUMENTS:
        UiWork_ClearValueNameTablesFar();
        break;
      // A negative message (MESSAGE_NONE in target_exec.c) shows nothing:
      // the event only waits for the player.
      case BATTLE_EVENT_MESSAGE:
        if ((s32)queue->operands[eventIndex] >= 0)
          UiText_ShowMessageAndWaitCoreFar(queue->operands[eventIndex]);
        BattlePresentation_WaitForAdvance();
        UiWork_ClearValueNameTablesFar();
        break;
      case BATTLE_EVENT_MESSAGE_NO_WAIT:
        if ((s32)queue->operands[eventIndex] >= 0)
          UiText_ShowMessageAndWaitCoreFar(queue->operands[eventIndex]);
        UiWork_ClearValueNameTablesFar();
        break;
      case BATTLE_EVENT_HIT:
        if (runtime->pendingCue > 0)
          AudioCommand_PlayFar(runtime->pendingCue);
        BattleMotion_FlashHit(queue->operands[eventIndex], 0, 0);
        break;
      case BATTLE_EVENT_DEFEAT:
        BattleEnemy_RecordDefeat(queue->operands[eventIndex], runtime->flags);
        BattleObject_ResetRuntimeFields(queue->operands[eventIndex]);
        BattleMotion_ShowDowned(queue->operands[eventIndex]);
        break;
      case BATTLE_EVENT_DRAW_STATUS:
        PartyStatusDrawFar(gBattleWork->partyStatus);
        break;
      case BATTLE_EVENT_REFRESH_UNIT:
        BattleUnit_BuildStatusFlags(queue->operands[eventIndex],
                                    BattleObjGet(queue->operands[eventIndex]));
        BattlePresentation_SetActorModeAndAction(queue->operands[eventIndex]);
        break;
      }
  }
  BattleEvent_Reset();
}

/// Queues one battle event (enum BattleEventOpcode) with its operand, and
/// returns the opcode.
/// Called, directly or through PUSH_MESSAGE, by BattleTargetExec for each
/// step of an outcome, and by staging.c, plan.c and turn_end.c. Appends to
/// gBattleWork->events.queue with no test against BATTLE_EVENT_CAPACITY: a
/// caller must keep one target's events, or one queue played at once, within
/// it. The queue is emptied by BattleEvent_Reset, by
/// BattleEvent_SchedulePhase and before each target
/// BattleEvent_Playback resolves.
u32 BattleEvPush(u32 opcode, u32 operand) {
  struct BattleEventQueue *queue = &gBattleWork->events.queue;
  u32 index = queue->count;

  queue->opcodes[index] = opcode;
  queue->operands[index] = operand;
  queue->count = index + 1;
  return opcode;
}

#undef ADVANCE_ARROW_ATTR0
