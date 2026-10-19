// Part of battle.c, the battle object, compiled only through it.
// Battle events: waiting for the player, and the queued events played at once.

#include "../battle_internal.h"

// Clears every ailment and stat change of unit id, recalculates its stats
// and rebuilds its status icons; called when the unit goes down.
s32 BattleActor_ResetRuntimeFields(s32 unitId) {
  struct BattleUnit *unit;
  s32 element;

  unit = UnitGetFar(unitId);
  for (element = NUM_ELEMENTS - 1; element >= 0; element--)
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
  return BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
}

// One OAM entry.
// A second view of struct BattlePromptSprite, kept: plain halfwords, where its
// bit-fields change BattlePresentation_WaitForAdvance (differ 50).
struct AdvanceSprite {
  struct AdvanceSprite *next;
  u16 attr0;
  u16 attr1;
  u16 attr2;
  u16 attr3;
};

// Shows the bobbing arrow at the message window and waits for the player:
// B held, or any of KEY_ADVANCE pressed, goes on; after 16 frames holding one
// of them is enough. Plays the cursor sound.
s32 BattlePresentation_WaitForAdvance(void) {
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
    frameTiles =
        ((gFrameCount >> 2) & 7) * 128 + (s32)gBattlePres_AdvanceArrowTiles;
    window = gBattleDisplayWork->window;
    offset = gBattleDisplayWork->offset;
    QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
    QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
    Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
    *(u32 *)&prompt->attr0 = 0xa400;
    *(u32 *)&prompt->attr2 = 0;
    tile = VramCache_Reload(tileSlot, frameTiles) & 0x3ff;
    attr2 = prompt->attr2;
    prompt->attr2 = (attr2 & ~0x3ff) | tile;
    // The window is placed in tiles, its offset in 1/256 pixels.
    x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
    attr1 = prompt->attr1;
    prompt->attr1 = (attr1 & ~0x1ff) | x;
    // Bob up and down by up to 2 pixels.
    *(u8 *)&prompt->attr0 = Trig_Sin(gFrameCount << 12) / 32768 +
                            window->y * 8 + (offset->y >> 8) + 6;
    Runtime_PushSlotEntry(prompt, 240);
    if ((gKeysHeld & KEY_B) || (gKeysPressed & KEY_ADVANCE) ||
        (frame > 15 && (gKeysHeld & KEY_ADVANCE)))
      break;
    WaitFrames(1);
  }
  AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
  Resource_ResetEntry(tileSlot);
  WaitFrames(1);
}

// Shows the cursor bobbing at (x, y) and waits for one of KEY_ADVANCE.
// x, y : in pixels
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y) {
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
    QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
    QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
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

// Marks the battle display for redrawing.
void BattleEv_MarkDisplay(void) { gBattleDisplayWork->marked = 1; }

// Takes an enemy out of the battle at once (one that leaves rather than
// falls): its HP to 0, out of the lists, and its object released.
// Declared to return s32, but returns nothing.
s32 BattleActor_DestroyTemporaryObject(s32 unitId) {
  struct BattleUnit *enemy;
  struct BattleObj *slot;

  enemy = UnitGetFar(unitId);
  if (enemy->side == BATTLE_UNIT_ENEMY) {
    Unit_SetHpFar(enemy, 0);
    BattleActor_RemoveFromLists(unitId);
    BattleObj_MarkForRelease(unitId);
    slot = BattleObjGet(unitId);
    ObjectDispatch_ReleaseFar((struct DispatchObject *)slot->object);
    slot->object = 0;
    slot->active = 0;
  }
}

// BattleEventState.flags bit 0: a Djinni struck a weakness, so enemies that
// fall pay the Djinn bonus (BattleEnemy_RecordDefeat).
#define EVENT_FLAG_DJINN_BONUS 1

// The BATTLE_EVENT_DJINN_BONUS event: grants the Djinn bonus for the rest of
// the action. Its operand is ignored.
void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand) {
  runtime->flags |= EVENT_FLAG_DJINN_BONUS;
}

// Plays the queued battle events at once, in order (BattleEvent_Playback
// plays them over frames instead): the messages with their arguments, the
// hit flashes, the downed units, the redraws. Then empties the queue.
u32 BattleEv_DispatchQueued(void) {
  struct BattleEventState *runtime = &gBattleWork->events;
  struct BattleEventQueue *queue = &runtime->queue;
  s32 eventIndex;

  for (eventIndex = 0; eventIndex < queue->count; eventIndex++) {
    u8 opcode = queue->opcodes[eventIndex];
    if (opcode <= BATTLE_EVENT_DJINN_BONUS)
      switch (opcode) {
      case BATTLE_EVENT_DJINN_BONUS:
        Battle_SetRuntimeFlagBit0(runtime, queue->operands[eventIndex]);
        break;
      case BATTLE_EVENT_REMOVE_UNIT:
        BattleActor_DestroyTemporaryObject(queue->operands[eventIndex]);
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
        BattleMotion_RunValueSequence(queue->operands[eventIndex], 0, 0);
        break;
      case BATTLE_EVENT_DEFEAT:
        BattleEnemy_RecordDefeat(queue->operands[eventIndex], runtime->flags);
        BattleActor_ResetRuntimeFields(queue->operands[eventIndex]);
        BattleMotion_InitializeActorRecords(queue->operands[eventIndex]);
        break;
      case BATTLE_EVENT_DRAW_STATUS:
        PartyStatusDrawFar(gBattleWork->partyStatus);
        break;
      case BATTLE_EVENT_REFRESH_UNIT:
        BattleUnit_BuildStatusFlags(queue->operands[eventIndex],
                                    BattleObjGet(queue->operands[eventIndex]));
        BattlePres_SetActorModeAndAction(queue->operands[eventIndex]);
        break;
      }
  }
  return BattleEventRuntime_Reset();
}

// Queues one battle event (enum BattleEventOpcode) with its operand, and
// returns the opcode.
u32 BattleEvPush(u32 opcode, u32 operand) {
  struct BattleEventQueue *queue = &gBattleWork->events.queue;
  u32 index = queue->count;

  queue->opcodes[index] = opcode;
  queue->operands[index] = operand;
  queue->count = index + 1;
  return opcode;
}
