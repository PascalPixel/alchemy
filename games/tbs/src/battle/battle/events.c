/// @file
/// Battle playback: a downed unit's reset, waiting for the player, and the
/// playback queue, filled by BattlePlayback_Push and played at once by
/// BattlePlayback_RunAll.
// Part of battle.c.

#include "../battle_internal.h"

/// Clears every status and stat change of a unit that goes down, recalculates
/// its stats and rebuilds its status icons.
/// Reached through the PLAYBACK_DEFEAT event, from
/// BattlePlayback_RunAll and BattlePlayback_Run. Clears the unit's
/// element levels, poison, stat stages and their turns, every status, its
/// equipment bonuses, ready, immobile and challenge; Unit_CalculateFar then
/// recomputes its stats, and BattleUnit_BuildStatusFlags its battle object's
/// status flags.
#ifdef UBFIX
void BattleObject_ResetRuntimeFields(s32 unitId) {
#else
s32 BattleObject_ResetRuntimeFields(s32 unitId) {
#endif
  struct Unit *unit;
  s32 element;

  unit = Unit_GetFar(unitId);
  for (element = ELEMENT_COUNT - 1; element >= 0; element--)
    unit->summonBoost[element] = 0;

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
  unit->regenerate = 0;
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

  Unit_CalculateFar(unitId);
  BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
}

// XXX: likely only a second spelling of struct AdvanceArrowSprite, because
// BattleMessage_WaitForAdvance written through that struct as
// playback.c's PLAY_ADVANCE writes it (both words through a u32 cast, then
// tile, x and y) shows no difference in an alchemy-devtools diff of any
// edition; set field by field, 101 of 138 instructions differ in tbs-en. The
// twelve-edition comparison would settle whether this view can go.

/// The advance arrow's OAM entry, as plain halfwords: a second view of struct
/// AdvanceArrowSprite. BattleMessage_WaitForAdvance keeps it on its
/// stack and rebuilds it each frame before Oam_AddSpriteAtDepth queues it.
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
/// Called after a message by BattleStaging_DispatchAction and the
/// staging runners, BattleTurn_Build (a flight),
/// BattleIntro_AnnounceEncounter, the round's end (turn_end.c), and
/// BattlePlayback_RunAll for each PLAYBACK_MESSAGE. Waits for the
/// message window first. Takes a 128-byte VRAM cache entry for the arrow's
/// tiles and frees it; each frame queues the WINOUT writes, sets REG_BLDALPHA
/// and pushes the arrow sprite.
#ifdef UBFIX
void BattleMessage_WaitForAdvance(void) {
#else
s32 BattleMessage_WaitForAdvance(void) {
#endif
  struct AdvanceSprite *prompt;
  struct AdvanceSprite promptStorage;
  s32 frame;
  s32 tileSlot;
  s32 frameTiles;
  struct Window *window;
  struct BattleMessageOffset *offset;
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
        ((gFrameCount >> 2) & 7) * 128 + (s32)gBattleMessage_AdvanceArrowTiles;
    window = gBattleMessageWork->window;
    offset = gBattleMessageWork->offset;
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
    Oam_AddSpriteAtDepth(prompt, 240);
    // A held advance key counts only after 16 frames, so one still held from
    // the previous message does not skip this one; B held goes on at once.
    if ((gKeysHeld & KEY_B) || (gKeysPressed & KEY_ADVANCE) ||
        (frame > 15 && (gKeysHeld & KEY_ADVANCE)))
      break;
    WaitFrames(1);
  }
  Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
  VramCache_FreeEntry(tileSlot);
  WaitFrames(1);
}

/// Shows the cursor bobbing at (x, y), in screen pixels, and waits for one of
/// KEY_ADVANCE. Called by Battle_Run under the Djinn tutorial's
/// standby message, and by BattleCommandRow_Run through
/// BattleMessage_WaitForAdvanceAtFar. Waits for the message window
/// first; takes a 128-byte VRAM cache entry for the cursor's tiles and frees
/// it, and each frame queues the WINOUT writes, sets REG_BLDALPHA and pushes
/// the cursor sprite.
#ifdef UBFIX
void BattleMessage_WaitForAdvanceAt(s32 x, s32 y) {
#else
s32 BattleMessage_WaitForAdvanceAt(s32 x, s32 y) {
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
    Oam_AddSpriteAtDepth(prompt, 240);
    if (gKeysPressed & KEY_ADVANCE)
      break;
    WaitFrames(1);
  }
  VramCache_FreeEntry(tileSlot);
  WaitFrames(1);
}

/// Sets gBattleMessageWork->marked, which the next message reads, as
/// PLAYBACK_MARK does.
/// Called by BattleTurn_Build between a weapon's two unleash messages.
void BattlePlayback_MarkMessage(void) { gBattleMessageWork->marked = 1; }

/// Takes an enemy out of the battle at once (one that leaves rather than
/// falls): its HP to 0, out of the lists, and its object released.
/// Reached through the PLAYBACK_REMOVE_UNIT event, which
/// BattleAbility_ApplyToTarget queues for a self-destructing actor, from
/// BattlePlayback_RunAll and BattlePlayback_Run. A party unit is left as
/// it is. Clears the battle object's object and active and releases its
/// dispatch object.
#ifdef UBFIX
void BattleObject_DestroyTemporaryObject(s32 unitId) {
#else
s32 BattleObject_DestroyTemporaryObject(s32 unitId) {
#endif
  struct Unit *enemy;
  struct BattleObject *slot;

  enemy = Unit_GetFar(unitId);
  if (enemy->side == BATTLE_SIDE_ENEMY) {
    Unit_SetHpFar(enemy, 0);
    BattleObject_RemoveFromLists(unitId);
    BattleObject_MarkForRelease(unitId);
    slot = BattleObject_Get(unitId);
    ObjectDispatch_ReleaseFar((struct DispatchObject *)slot->object);
    slot->object = 0;
    slot->active = 0;
  }
}

/// BattlePlaybackWork.flags bit 0: a Djinni struck a weakness, so enemies that
/// fall pay the Djinn bonus (BattleEnemy_RecordDefeat).
#define PLAYBACK_FLAG_DJINN_BONUS 1

/// The PLAYBACK_DJINN_BONUS event: grants the Djinn bonus for the rest of
/// the action. Its operand is ignored.
/// Called for the event by BattlePlayback_RunAll and BattlePlayback_Run;
/// BattleAbility_ApplyToTarget queues it. Sets PLAYBACK_FLAG_DJINN_BONUS in
/// runtime->flags, which BattlePlayback_Reset clears.
void BattlePlayback_GrantDjinnBonus(struct BattlePlaybackWork *runtime,
                                    u32 operand) {
  runtime->flags |= PLAYBACK_FLAG_DJINN_BONUS;
}

/// Plays the queued playback at once, in order (BattlePlayback_Run
/// plays them over frames instead): the messages with their arguments, the
/// hit flashes, the downed units, the redraws. Then empties the queue.
/// Called where an outcome is shown outside an animation: by the staging
/// runners (a blocked cast, a dodge, a broken item, a turn's targets one by
/// one), BattleTurn_Build's debug defeat, and the turn's end (turn_end.c). Each
/// message, flash and fall waits its frames here. Ends with
/// BattlePlayback_Reset.
#ifdef UBFIX
void BattlePlayback_RunAll(void) {
#else
u32 BattlePlayback_RunAll(void) {
#endif
  struct BattlePlaybackWork *runtime = &gBattleWork->playback;
  struct BattlePlaybackQueue *queue = &runtime->queue;
  s32 eventIndex;

  for (eventIndex = 0; eventIndex < queue->count; eventIndex++) {
    u8 opcode = queue->opcodes[eventIndex];
    // PLAYBACK_SOUND, the last opcode, is skipped: played at once, the
    // queue makes no sounds but a hit's.
    if (opcode <= PLAYBACK_DJINN_BONUS)
      switch (opcode) {
      case PLAYBACK_DJINN_BONUS:
        BattlePlayback_GrantDjinnBonus(runtime, queue->operands[eventIndex]);
        break;
      case PLAYBACK_REMOVE_UNIT:
        BattleObject_DestroyTemporaryObject(queue->operands[eventIndex]);
        break;
      case PLAYBACK_UNIT_NAME:
        MessageArgument_PushFar(queue->operands[eventIndex],
                                MESSAGE_ARGUMENT_ACTOR);
        break;
      case PLAYBACK_NUMBER:
        MessageArgument_PushFar(queue->operands[eventIndex],
                                MESSAGE_ARGUMENT_NUMBER);
        break;
      case PLAYBACK_ITEM_NAME:
        MessageArgument_PushFar(queue->operands[eventIndex] &
                                    INVENTORY_ITEM_ID_MASK,
                                MESSAGE_ARGUMENT_ITEM);
        break;
      case PLAYBACK_ABILITY_NAME:
        MessageArgument_PushFar(queue->operands[eventIndex] &
                                    PSYNERGY_ABILITY_MASK,
                                MESSAGE_ARGUMENT_ABILITY);
        break;
      case PLAYBACK_MARK:
        gBattleMessageWork->marked = 1;
        break;
      case PLAYBACK_CLEAR_ARGUMENTS:
        MessageArgument_ClearAllFar();
        break;
      // A negative message (MESSAGE_NONE in target_exec.c) shows nothing:
      // the event only waits for the player.
      case PLAYBACK_MESSAGE:
        if ((s32)queue->operands[eventIndex] >= 0)
          BattleMessage_ShowAndWaitFar(queue->operands[eventIndex]);
        BattleMessage_WaitForAdvance();
        MessageArgument_ClearAllFar();
        break;
      case PLAYBACK_MESSAGE_NO_WAIT:
        if ((s32)queue->operands[eventIndex] >= 0)
          BattleMessage_ShowAndWaitFar(queue->operands[eventIndex]);
        MessageArgument_ClearAllFar();
        break;
      case PLAYBACK_HIT:
        if (runtime->pendingSong > 0)
          Sound_PlaySongFar(runtime->pendingSong);
        BattleMotion_FlashHit(queue->operands[eventIndex], 0, 0);
        break;
      case PLAYBACK_DEFEAT:
        BattleEnemy_RecordDefeat(queue->operands[eventIndex], runtime->flags);
        BattleObject_ResetRuntimeFields(queue->operands[eventIndex]);
        BattleMotion_ShowDowned(queue->operands[eventIndex]);
        break;
      case PLAYBACK_DRAW_STATUS:
        PartyStatus_DrawFar(gBattleWork->partyStatus);
        break;
      case PLAYBACK_REFRESH_UNIT:
        BattleUnit_BuildStatusFlags(
            queue->operands[eventIndex],
            BattleObject_Get(queue->operands[eventIndex]));
        BattleObject_UpdatePose(queue->operands[eventIndex]);
        break;
      }
  }
  BattlePlayback_Reset();
}

/// Queues one playback command (enum BattlePlaybackCommand) with its operand,
/// and returns the opcode. Called, directly or through PUSH_MESSAGE, by
/// BattleAbility_ApplyToTarget for each step of an outcome, and by staging.c,
/// turn.c and turn_end.c. Appends to gBattleWork->playback.queue with no test
/// against BATTLE_PLAYBACK_CAPACITY: a caller must keep one target's commands,
/// or one queue played at once, within it. The queue is emptied by
/// BattlePlayback_Reset, by BattlePlayback_SchedulePhase and before each target
/// BattlePlayback_Run resolves.
u32 BattlePlayback_Push(u32 opcode, u32 operand) {
  struct BattlePlaybackQueue *queue = &gBattleWork->playback.queue;
  u32 index = queue->count;

  queue->opcodes[index] = opcode;
  queue->operands[index] = operand;
  queue->count = index + 1;
  return opcode;
}

#undef ADVANCE_ARROW_ATTR0
