#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/object/object.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "edition.h"
#include "io_reg.h"
#include "battle_unit.h"
#include "game_state.h"
#include "battle_system.h"
#include "text/msg_ids.h"
#include "mem.h"
#include "resource_ids.h"
#include "callback_scheduler.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "battle_display.h"
#include "battle_message.h"
#include "battle_party.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "scene.h"
#include "battle_work.h"
#include "battle_type.h"
#include "battle_summon.h"
#include "tbs_edition.h"
#include "serial_runtime.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_command.h"
#include "resource.h"
#include "heap_state.h"
#include "flags.h"
#include "ability_ids.h"
#include "menu_list.h"
#include "audio_engine_symbols.h"
#include "ui.h"
#include "battle_calc.h"
#include "objdisp.h"
#include "motobj.h"
#include "battle_status_icon.h"
#include "animspr.h"
#include "field_event.h"
#include "metadata_lookup.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "effect_step.h"
#include "battle_escape.h"
#include "battle_target.h"
#include "battle_obj.h"
#include "battle_event.h"
#include "project.h"
#include "battle_random.h"
#include "irqlock.h"
#include "transform.h"
#include "battle_effect.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "vram_block.h"
#include "owner_value.h"
#include "sound_ids.h"
#include "ramcode.h"
#include "map_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "b5_context.h"
#include "frame.h"
#include "unit/object/psynergy_effects.h"

// The kinds of argument a message's placeholders take, for
// UiWork_PushValueSlotFar: a unit's name, an item's, a class's, an
// ability's, and a number.
#define VALUE_UNIT_NAME 1
#define VALUE_ITEM_NAME 2
#define VALUE_CLASS_NAME 3
#define VALUE_ABILITY_NAME 4
#define VALUE_NUMBER 5

// One row of the debug party presets (presets.tsv): a party member, the
// level to raise them to, the djinn of each element to give them, the items
// to equip and up to two abilities to teach. A row whose owner is -1 ends a
// preset.
struct DebugPartyEntry {
  s8 owner;
  u8 level;
  s8 djinnCounts[NUM_ELEMENTS];
  u16 items[4];
  u16 abilities[2];
  u16 unused; // 0 in every row
};

extern struct DebugPartyEntry gDebug_PartyPresets[];

#define DEBUG_PRESET_LAST_ROW 332
#define DEBUG_PRESET_END -1

//--------------------------------------------------------------------------
// Debug: replaces the party with the members of preset number preset.
// Each member is raised to the row's level, given the row's djinn (Set, in
// order of their numbers across the party), the row's abilities, and an
// emptied inventory holding the row's items, each equipped.
// Returns 1 when the table ends before that preset, 0 otherwise.
//--------------------------------------------------------------------------
s32 DebugParty_LoadPreset(s32 preset) {
  s32 nextDjinn[NUM_ELEMENTS]; // the next djinn number of each element to hand
                               // out
  struct BattleUnit *member;
  s32 owner;
  s32 notFound;
  s32 row;
  s32 djinnIndex;
  s32 currentPreset;
  s32 i;

  notFound = 0;
  // Replace the active party before scanning the requested preset's rows.
  Party_RemoveActiveOwnerFar(ISAAC);
  Party_RemoveActiveOwnerFar(GARET);
  Party_RemoveActiveOwnerFar(IVAN);
  Party_RemoveActiveOwnerFar(MIA);
  Party_RemoveActiveOwnerFar(JENNA);

  currentPreset = 0;
  for (i = 0; i < NUM_ELEMENTS; i++)
    nextDjinn[i] = 0;

  for (row = 0;; row++) {
    owner = gDebug_PartyPresets[row].owner;
    if (row < 0 || row > DEBUG_PRESET_LAST_ROW) {
      notFound = 1;
      break;
    }
    if (owner == DEBUG_PRESET_END) {
      if (currentPreset == preset)
        break;
      currentPreset++;
      continue;
    }
    if (currentPreset != preset)
      continue;

    Party_AddActiveOwnerFar(owner);
    Unit_LevelUpToFar(owner, gDebug_PartyPresets[row].level);

    // Start from no djinn and no abilities.
    member = (struct BattleUnit *)UnitGetFar(owner);
    for (i = 0; i < NUM_ELEMENTS; i++) {
      member->djinnOwnedCounts[i] = 0;
      member->djinnActiveCounts[i] = 0;
      member->djinnAvailable[i] = 0;
      member->djinnActive[i] = 0;
    }
    for (i = 0; i < 32; i++)
      /* FAKEMATCH: the halfword store through a u16 cast loads its zero from
       * the literal pool, as the reference does. 2026-10-13: the field store
       * member->actionSlots[i].encodedAction = 0, with or without a (u16) cast
       * on the zero, differs 23 of 210: movs r3, #0 takes the place of the ldr
       * of the zero and the literal pool moves to the function end; any store
       * through a dereference (this cast, *&...encodedAction, ((u16
       * *)member->actionSlots)[i * 2]) matches. */
      *(u16 *)&member->actionSlots[i] = 0;
    for (i = 0; i < 2; i++) {
      if (gDebug_PartyPresets[row].abilities[i] != 0)
        Unit_LearnPsynergyFar(owner, gDebug_PartyPresets[row].abilities[i]);
    }

    for (i = 0; i < NUM_ELEMENTS; i++) {
      for (djinnIndex = 0; djinnIndex < gDebug_PartyPresets[row].djinnCounts[i];
           djinnIndex++) {
        Djinn_AddToOwnerFar(owner, i, nextDjinn[i]);
        Djinn_ActivateFar(owner, i, nextDjinn[i]);
        nextDjinn[i]++;
      }
    }

    // Empty the inventory, then add and equip the row's items.
    for (i = 0; i < INVENTORY_SLOTS + 1; i++) // slot 0, more times than needed
      Inventory_RemoveFar(owner, 0);
    for (i = 0; i < 4; i++) {
      if (gDebug_PartyPresets[row].items[i] != 0)
        Inventory_EquipFar(
            owner,
            Inventory_AddItemFar(owner, gDebug_PartyPresets[row].items[i]));
    }
  }
  return notFound;
}

// The window work's byte that marks a message window busy; it moved when the
// international editions grew the work.
#if EDITION_INTERNATIONAL
#define WINDOW_WORK_MESSAGE_BUSY 0x12f8
#else
#define WINDOW_WORK_MESSAGE_BUSY 0x1188
#endif

// The first message of the viewer's second range: in Japanese, an earlier
// copy of the first range's opening lines; the localizations moved it
// elsewhere.
#if defined(TBS_EDITION_EN)
#define DEBUG_SECOND_FORM MsgBecameCursed
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define DEBUG_SECOND_FORM MsgEnd
#else
#define DEBUG_SECOND_FORM MsgDebugSecondForm
#endif

// The highest message index the viewer reaches.
#define DEBUG_VIEW_LIMIT                                                       \
  (MsgLaliveroHowDidSearchForSheba -                                           \
   MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely + 5)

// The length of a unit's name, its terminator excluded.
#define UNIT_NAME_LENGTH 14

#define DEBUG_VIEWER_SONG 71

//--------------------------------------------------------------------------
// Debug: a message viewer that never returns. Names the first party member
// "Ha hi fu he ho" (MsgDebugSampleName), fills the message arguments with
// sample values and shows one message at a time, counted from the Babi
// Lighthouse dialogue (MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely) or,
// in the second range, from DEBUG_SECOND_FORM. Right and Left step the
// index, R and L step it by ten, Up and Down pick the range, and B
// alternates between the ranges, moving on one message each time it leaves
// the first. A closes the window and shows the message again.
//--------------------------------------------------------------------------
void DebugBattle_ViewMessages(void) {
  u16 text[64];
  s32 secondRange = 0;
  struct BattleUnit *unit = UnitGetFar(0);
  s32 i;
  s32 index;
  struct UiWindow *messageWindow;
  s32 message;

  // Supply a readable sample name before entering the message browser.
  Ui_AdjustValueWithoutLimitFar(MsgDebugSampleName, text);
  for (i = 0; i < UNIT_NAME_LENGTH; i++) {
    unit->name[i] = text[i];
    if (text[i] == 0)
      break;
  }
  unit->name[UNIT_NAME_LENGTH] = 0;
  UiWork_InitializeWithResourceCountersFar();
  AudioCommand_PlayFar(DEBUG_VIEWER_SONG);
  for (;;) {
    index = 0;
    {
      s32 control = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                    DISPCNT_OBJ_1D_MAP | 1; // mode 1

      REG_DISPCNT = control;
    }
    gGameState.messageSpeed = 2;
    for (;;) {
      // Sample arguments for the message's placeholders.
      UiWork_ClearValueNameTablesFar();
      UiWork_PushValueSlotFar(999, VALUE_NUMBER);
      UiWork_PushValueSlotFar(0, VALUE_CLASS_NAME);
      UiWork_PushValueSlotFar(1, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(1, VALUE_ITEM_NAME);
      UiWork_PushValueSlotFar(2, VALUE_ABILITY_NAME);
      if (secondRange == 0) {
        message = MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely;
        messageWindow = UiText_OpenMessageWindowFar(index + message, 2, 10, 4);
      } else {
        message = DEBUG_SECOND_FORM;
        messageWindow = UiText_OpenMessageWindowFar(index + message, 2, 2, 4);
      }
      WaitFrames(10);

      // Wait for a key that picks another message, or for A once the
      // message has been shown in full.
      for (;;) {
        if (gKeysRepeat & KEY_B) {
          if (secondRange != 0) {
            secondRange = 0;
          } else {
            index++;
            secondRange = 1;
          }
        }
        if (gKeysRepeat & KEY_RIGHT)
          index++;
        if (gKeysRepeat & KEY_LEFT)
          index -= 2;
        if (gKeysRepeat & KEY_UP)
          secondRange = 1;
        if (gKeysRepeat & KEY_DOWN)
          secondRange = 0;
        if (gKeysRepeat & KEY_R)
          index += 10;
        if (gKeysRepeat & KEY_L)
          index -= 10;
        if (index < 0)
          index = 0;
        if (index >= (u32)DEBUG_VIEW_LIMIT)
          index = DEBUG_VIEW_LIMIT;
        if (gKeysRepeat & (KEY_B | KEYS_DPAD | KEYS_SHOULDERS))
          break;
        if (UiWork_IsCompleteFar() != 0 && (gKeysRepeat & KEY_A))
          break;
        WaitFrames(1);
      }
      UiWork_DrainPendingFar(1);
      // Finish this window before opening the newly selected message.
      UiWork_FinalizeFar(messageWindow, 1);
      (*(u8 **)&gWindowWork)[WINDOW_WORK_MESSAGE_BUSY] = 0;
    }
  }
}

// The encounter the debug battle starts with, and the Djinn tutorial's.
#define DEBUG_FIRST_ENCOUNTER 257
#define DJINN_TUTORIAL_ENCOUNTER 28

//--------------------------------------------------------------------------
// Debug: the battle test menu, which never returns. Without Down held it
// fights encounter 257 again and again with auto battle on. Holding Down at
// the start opens the chooser: Right and Left step the encounter, Up and
// Down by ten, R and L pick the party preset, Start opens the message
// viewer and A fights; B marks every later battle with start mode 5.
// Holding Down when A is pressed makes it a link battle.
//--------------------------------------------------------------------------
void DebugBattle_RunMenu(void) {
  s32 startModeSet = 0;
  s32 encounter;
  s32 preset;
  s32 loadedPreset;

  GameState_InitDefaultsFar();
restart:
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  Scheduler_ResetTaskTable();
  Runtime_InitializeHeap();
  VramCache_Reset();
  GameFlag_SetBitFar(FLAG_AUTO_BATTLE);
  encounter = DEBUG_FIRST_ENCOUNTER;
  if (gKeysHeld & KEY_DOWN) {
    loadedPreset = -1;
    GameFlag_ClearBitFar(FLAG_AUTO_BATTLE);
    preset = 0;
    for (;;) {
      GameFlag_ClearBitFar(FLAG_CHILDHOOD);
      WaitFrames(1);
      for (;;) {
        if (gKeysRepeat & KEY_RIGHT)
          encounter++;
        if (gKeysRepeat & KEY_LEFT)
          encounter--;
        if (gKeysRepeat & KEY_UP)
          encounter -= 10;
        if (gKeysRepeat & KEY_DOWN)
          encounter += 10;
        if (gKeysRepeat & KEY_R)
          preset++;
        if (gKeysRepeat & KEY_L)
          preset--;
        if (gKeysRepeat & KEY_A)
          break;
        if (gKeysRepeat & KEY_START)
          DebugBattle_ViewMessages();
        if (gKeysRepeat & KEY_SELECT)
          Battle_ReservedNoOp2A08();
        if ((gKeysRepeat & KEY_B) || startModeSet) {
          startModeSet = 1;
          Battle_SetStart(5);
        }
        if (preset != loadedPreset) {
          GameState_InitDefaultsFar();
          DebugParty_LoadPreset(preset);
          loadedPreset = preset;
        }
        WaitFrames(1);
      }
      // The chooser's current input also determines whether this is a link
      // battle.
      if (gKeysHeld & KEY_DOWN)
        GameFlag_SetBitFar(FLAG_LINK_BATTLE);
      UnitCalcFar(0);
      gGameState.special = (s32)&ResourceId_SuhallaGateBackdrop;
      if (encounter == DJINN_TUTORIAL_ENCOUNTER)
        GameFlag_SetBitFar(FLAG_DJINN_TUTORIAL);
      GameFlag_SetBitFar(354);
      Battle_RunEncounter(encounter);
      Ui_LoadWindowGraphics();
      DebugText_Clear();
      Scheduler_ResetTaskTable();
      Runtime_InitializeHeap();
      VramCache_Reset();
    }
  }
  GameFlag_SetBitFar(354);
  Battle_RunEncounter(DEBUG_FIRST_ENCOUNTER);
  goto restart;
}

// This console's number in a link session (SIOCNT bits 4 and 5, which the
// hardware sets in multi-player mode): 0 for the parent, 1 for the child.
#define SIO_MULTI_PLAYER_ID() ((REG_SIOCNT << 26) >> 30)

// gLinkStatus: both consoles are connected.
#define LINK_BOTH_CONNECTED 3

// The frames a link battle tolerates without a connection before it gives
// up.
#define LINK_MAX_MISSED_FRAMES 24

//--------------------------------------------------------------------------
// Runs each frame of a battle. In a link battle it first checks the link:
// a connection lost for more than LINK_MAX_MISSED_FRAMES frames, or a
// console whose link number changed, pauses the battle. Then it turns the
// camera a sixteenth of the way toward a transition's yaw, places the camera
// at its position (or at the one it follows) with its yaw, pitch and
// distance, and, unless a transition owns the background, refreshes the
// background's perspective.
//--------------------------------------------------------------------------
void BattlePresentation_UpdateCamera(void) {
  // UB: indexing past scalar gCameraWork reaches other work slots. Named
  // slots add four bytes in the native EN trial (0xf8 to 0xfc).
#ifdef UBFIX
  struct BattleCamera *camera = gCameraWork;
  struct BattlePresentationTransition *transition =
      WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleWork *work = gBattleWork;
#else
  void **slot = (void **)&gCameraWork;
  struct BattleCamera *camera = slot[0];
  struct BattlePresentationTransition *transition =
      slot[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
  struct BattleWork *work = slot[HEAP_SLOT_BATTLE - HEAP_SLOT_CAMERA];
#endif
  struct FixedPointPosition eyeOffset;
  s32 *focus;
  s16 delta;
  u32 linkId;

  // Treat a lost connection or changed console number as a paused battle.
  if (work->twoSided != 0) {
    if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
      work->linkMisses++;
      if (work->linkMisses > LINK_MAX_MISSED_FRAMES) {
        work->linkPaused = 1;
      }
    } else {
      linkId = SIO_MULTI_PLAYER_ID();
      if (work->linkSide != linkId) {
        work->linkPaused = 1;
      }
      work->linkMisses = 0;
    }
  }

  // Ease the yaw before building the view around its current focus.
  if (transition->frames != 0) {
    delta = transition->targetYaw - camera->yaw;
    delta /= 16;
    camera->yaw += delta;
    transition->frames--;
  }

  focus = camera->pos;
  if (camera->followPos != 0) {
    focus = camera->followPos;
  }

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(focus);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);

  // The eye sits distance behind the focus, along the turned view.
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset.x, (s32 *)camera);

  if (transition->flag == 0) {
    BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
  }
}

//--------------------------------------------------------------------------
// Announces the enemies at the start of a battle: "<enemy> appeared!" for
// each of the first enemyCount enemies, the last one's message a different
// line, then whether the party strikes first or is caught by surprise.
//--------------------------------------------------------------------------
void BattleIntro_AnnounceEncounter(s32 enemyCount) {
  s16 enemies[8];
  struct BattleWork *battle;
  s16 *enemy;
  s32 announced;

  battle = gBattleWork;
  UiWork_ClearValueNameTablesFar();
  BattleEnemyList(enemies);

  announced = 0;
  // Finish all appearance messages before announcing an opening advantage.
  if (enemyCount != 0) {
    enemy = enemies;
    do {
      UiWork_PushValueSlotFar((u16)*enemy++, VALUE_UNIT_NAME);
      if (announced == enemyCount - 1)
        UiText_ShowMessageAndWaitCoreFar(MsgLastEnemyAppeared);
      else
        UiText_ShowMessageAndWaitCoreFar(MsgEnemyAppeared);
      announced++;
      BattlePresentation_WaitForAdvance();
    } while (announced != enemyCount);
  }

  UiWork_FinalizeSharedSlotFar();
  if (battle->encounterMode == BATTLE_ENCOUNTER_PARTY_FIRST) {
    UiWork_ClearValueNameTablesFar();
    UiWork_PushValueSlotFar(0, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgPartyStrikesFirst);
    BattlePresentation_WaitForAdvance();
  } else if (battle->encounterMode == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
    UiWork_ClearValueNameTablesFar();
    UiWork_PushValueSlotFar(0, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgPartySurprised);
    BattlePresentation_WaitForAdvance();
  }
}

// BattleWork.battleType, from the enemy formation
// (BattleFormation_BuildEnemyList): the random formations stand left to right
// in the order they were drawn, any other is laid out from the centre.
#define FORMATION_SHUFFLED 0
#define FORMATION_INTERLEAVED 1

//--------------------------------------------------------------------------
// Fills the battle's two unit lists, each ended by BATTLE_UNIT_LIST_END:
// the party members in their order, and the enemies, left to right for a
// random formation or, for a fixed one, alternating out from the centre
// (gBattleParty_CenterOrderOffsets).
//--------------------------------------------------------------------------
void BattleParty_CollectUnitList(void) {
  u16 unitIds[14];
  struct BattleWork *battle;
  s32 count;
  s32 i;
  s32 position;
  s32 formation;

  battle = gBattleWork;
  // The party list follows active-owner order regardless of enemy formation.
  count = BattleParty_PrepareActiveOwners(unitIds);
  for (i = 0; i < count; i++)
    battle->partyUnits[i] = unitIds[i];
  battle->partyUnits[count] = BATTLE_UNIT_LIST_END;

  count = BattleEnemyList(unitIds);
  formation = battle->battleType;
  if (formation >= FORMATION_SHUFFLED) {
    if (formation <= FORMATION_INTERLEAVED) {
      for (i = 0; i < count; i++)
        battle->enemyIds[i] = unitIds[i];
      goto done;
    }
  }
  for (i = 0; i < count; i++) {
    position = gBattleParty_CenterOrderOffsets[i] + count / 2;
    battle->enemyIds[position] = unitIds[i];
  }
done:
  battle->enemyIds[count] = BATTLE_UNIT_LIST_END;
}

//--------------------------------------------------------------------------
// Moves the 16 bytes at VRAM 0x06000290 down to 0x06000280 and clears the
// 20 bytes from 0x0600028c.
//--------------------------------------------------------------------------
s32 BattlePresentation_InitializeTilePattern(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, 0x80000008,
          REG_DMA3); // eight halfwords
  return Iwram_ClearWords((void *)0x0600028c, 20);
}

//--------------------------------------------------------------------------
// Sets BG0's vertical scroll back to 0.
//--------------------------------------------------------------------------
void BattlePresentation_ClearBg0VerticalOffset(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

//--------------------------------------------------------------------------
// Does nothing.
//--------------------------------------------------------------------------
void Party_ReservedNoOp5B14(s32 unused) {}

//--------------------------------------------------------------------------
// Clears every battle-only state of the party members (stat stages and
// their turns, statuses, Djinn element boosts, the countdowns) and
// recalculates their stats, as a battle ends.
//--------------------------------------------------------------------------
s32 BattleParty_ResetActiveRuntimeFields(void) {
  u16 owners[10];
  s32 count;
  s32 i;

  count = BattleParty_PrepareActiveOwners(owners);

  i = 0;
  if (i < count) {
    struct BattleUnit *unit;
    u8 *level;
    s32 remaining;

    do {
      unit = UnitGetFar(owners[i]);

      // The four element levels, cleared from the last.
      level = (u8 *)&unit->elementLevels[NUM_ELEMENTS - 1];
      remaining = NUM_ELEMENTS - 1;
      do {
        remaining--;
        *level-- = 0;
      } while (remaining >= 0);

      // Clear temporary battle effects before recalculating persistent stats.
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
      unit->grimReaper = 0;
      unit->equipBonus[0] = 0;
      unit->equipBonus[1] = 0;
      unit->ready = 0;
      unit->immobile = 0;
      unit->agilityTurns = 0;
      unit->agilityStage = 0;
      unit->challenge = 0;

      UnitCalcFar(owners[i]);
      i++;
    } while (i < count);
  }
}

// The Djinn recovery list's two tables (Djinn_GetRecoveryTableFar): the
// party's and the link opponent's.
#define DJINN_RECOVERY_PARTY 0
#define DJINN_RECOVERY_OPPONENT 1

// DjinnRecoveryEntry.turns of a Djinni waiting for the battle to end.
#define DJINN_RECOVERY_AFTER_BATTLE -1

#define DJINN_PER_ELEMENT 20

//--------------------------------------------------------------------------
// Brings the Djinn recovery list up to date as a battle ends: every Djinni
// a party member has on standby that the list lacks joins it. Then, outside
// a link battle, each Djinni waiting for the battle to end whose owner is
// no longer in the battle is Set again and leaves the list.
//--------------------------------------------------------------------------
s32 BattleParty_ReconcileDjinnRecovery(void) {
  u16 owners[10];
  s32 count;
  s32 i;
  s32 owner;
  s32 element;
  s32 djinni;

  count = BattleParty_PrepareActiveOwners(owners);

  // Register standby Djinn that are missing from their side's recovery list.
  for (i = 0; i < count; i++) {
    owner = owners[i];
    for (element = 0; element < NUM_ELEMENTS; element++) {
      for (djinni = 0; djinni < DJINN_PER_ELEMENT; djinni++) {
        if (Djinn_IsReadyFar(owner, element, djinni) != 0) {
          struct DjinnRecoveryList *list =
              &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                    (owner < 0 || owner > 7) ? DJINN_RECOVERY_OPPONENT
                                             : DJINN_RECOVERY_PARTY))
                   ->list;
          s32 j;

          for (j = 0; j < list->count; j++) {
            if (element == list->entries[j].element &&
                djinni == list->entries[j].index)
              break;
          }
          if (j == list->count)
            DjinnRecovery_AddStandbyFar(owner, element, djinni);
        }
      }
    }
  }

  // Link battles keep their recovery lists; only ordinary battles reset them.
  if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0)
    return;

  {
    struct DjinnRecoveryList *list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
              DJINN_RECOVERY_PARTY))
             ->list;
    struct DjinnRecoveryEntry *entry;

    i = 0;
    if (i < list->count) {
      s32 afterBattle = DJINN_RECOVERY_AFTER_BATTLE;

      entry = list->entries;
      do {
        if (entry->turns == afterBattle && BattleObjGet(entry->unitId) == 0) {
          u8 id = entry->unitId;
          u8 element = entry->element;
          u8 index = entry->index;

          Djinn_ActivateFar(id, element, index);
          DjinnRecovery_RemoveFar(id, element, index);
        }
        i++;
        entry++;
      } while (i < list->count);
    }
  }
}

#define NUM_SUMMONS 32

//--------------------------------------------------------------------------
// Works out which summons the party can call: one whose Djinn requirement
// of every element is met by the party's Djinn of that element, all
// members together. Stores the mask of them (bit n for summon n) and
// returns the state that holds it.
//--------------------------------------------------------------------------
struct BattleSummonState *BattleSummon_UpdateAvailability(void) {
  u8 totals[NUM_ELEMENTS];
  u16 partyMembers[10];
  s32 partySize;
  s32 element;
  u32 availableMask;

  partySize = BattleParty_PrepareActiveOwners(partyMembers);
  availableMask = 0;

  // The party's Djinn of each element.
  element = 0;
  do {
    totals[element] = 0;
    {
      s32 partySlot;

      for (partySlot = 0; partySlot < partySize; partySlot++) {
        struct BattleUnit *member = UnitGetFar(partyMembers[partySlot]);

        totals[element] += member->djinnOwnedCounts[element];
      }
    }
    element++;
  } while (element < NUM_ELEMENTS);

  // A summon is available when all four elements meet its requirement.
  // The element counter is reused here to walk the summon IDs.
  element = 0;
  do {
    const struct SummonDefinition *summon = SummonDefinition_Get(element);
    s32 elementsMet;

    if (summon != 0) {
      const u8 *required = summon->djinnRequired;

      elementsMet = 0;
      if (totals[0] >= required[0]) {
        u8 *total = totals;

        do {
          elementsMet++;
          if (elementsMet >= NUM_ELEMENTS)
            break;
          total++;
          required++;
        } while (*total >= *required);
      }

      if (elementsMet == NUM_ELEMENTS)
        availableMask |= 1u << element;
    }
    element++;
  } while (element < NUM_SUMMONS);

  {
    struct BattleSummonState *state =
        Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY);
    state->availableMask = availableMask;
    return state;
  }
}

//--------------------------------------------------------------------------
// Does nothing.
//--------------------------------------------------------------------------
void BattleSummon_ReservedNoOp(void) {}

// The longest "Enemy" label the opponent's names take before them; the
// European editions also put a space after it.
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define NAME_LIMIT 7
#define APPEND_NAME_SPACE 1
#else
#define NAME_LIMIT 4
#define APPEND_NAME_SPACE 0
#endif

// What a link battle sends of each party member, and of the Djinn recovery
// list.
enum LinkPartyPacketSize {
  LINK_PARTY_UNIT_BYTES = 340,
  LINK_PARTY_OFFER_BYTES = 320
};

// Unit ids from 128 are the enemies'; in a link battle the opponent's party
// takes the first three.
#define FIRST_ENEMY_UNIT 128
#define LINK_PARTY_SIZE 3

//--------------------------------------------------------------------------
// Link battle: receives the opponent's party into the enemy units, one
// member at a time, and puts the "Enemy" label (MsgEnemyLabel) before each
// name; then receives the opponent's Djinn recovery list. Stops early if a
// transfer cannot start. Returns how many of the received units are
// members (a unit sent with side 0 is an empty place).
//--------------------------------------------------------------------------
s32 UpdateNameEntries(void) {
  u16 label[24];
  void *buffer;
  struct BattleUnit *opponent;
  s32 memberCount;
  s32 partySlot;
  s32 labelLength;
  s32 i;

  buffer = Runtime_BumpAllocateAlternatePool(LINK_PARTY_UNIT_BYTES);
  memberCount = 0;
  partySlot = 0;
  // Receive each fixed-size member record before adding the localized prefix.
  while (partySlot < LINK_PARTY_SIZE) {
    opponent = UnitGetFar(partySlot + FIRST_ENEMY_UNIT);
    if (SerialRuntime_BeginReceive(opponent) == -1) {
      break;
    }
    SerialRuntime_WaitForReceive();
    if (opponent->side != 0) {
      memberCount += 1;
    }
    WaitFrames(2);

    // Shift the name right to make room for the label, then write it in.
    Ui_AdjustValueWithoutLimitFar(MsgEnemyLabel, label);
    i = 0;
    if (label[i] != 0) {
      do {
        i += 1;
        if (i > NAME_LIMIT) {
          break;
        }
      } while (label[i] != 0);
    }
#if APPEND_NAME_SPACE
    label[i] = ' ';
    i += 1;
#endif
    labelLength = i;
    for (i = UNIT_NAME_LENGTH; i >= labelLength; i--) {
      opponent->name[i] = opponent->name[i - labelLength];
    }
    for (i = 0; i < labelLength; i++) {
      opponent->name[i] = label[i];
    }
    opponent->name[UNIT_NAME_LENGTH] = 0;
    partySlot += 1;
  }
  MemFree(buffer);

  // The recovery packet follows all three party places.
  buffer = Runtime_BumpAllocateAlternatePool(LINK_PARTY_OFFER_BYTES);
  if (SerialRuntime_BeginReceive(
          Djinn_GetRecoveryTableFar(DJINN_RECOVERY_OPPONENT)) != -1) {
    SerialRuntime_WaitForReceive();
    WaitFrames(2);
  }
  MemFree(buffer);
  return memberCount;
}

//--------------------------------------------------------------------------
// Link battle: sends the party, one member at a time and three places in
// all (an empty place has side 0), then the Djinn recovery list with each
// owner given the unit id the opponent knows them by. Records that id of
// each member in ownerSlots.
//--------------------------------------------------------------------------
s32 LinkBattle_SendParty(void) {
  u8 *buffer;
  struct BattleWork *work;
  s32 j;
  struct DjinnRecoveryList *list;
  s32 size;
  s32 count;
  s32 i;
  s32 noSlot;
  u16 owners[8];

  size = LINK_PARTY_UNIT_BYTES;
  buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
  noSlot = 0xff;
  work = gBattleWork;
  for (i = 7; i >= 0; i--)
    work->ownerSlots[i] = noSlot;

  // The members, each marked as the opponent's.
  count = BattleParty_PrepareActiveOwners(owners);
  for (i = 0; i < count; i++) {
    Iwram_CopyWords(buffer, UnitGetFar(owners[i]), size);
    ((struct BattleUnit *)buffer)->side = BATTLE_UNIT_PARTY;
    work->ownerSlots[owners[i]] =
        i - FIRST_ENEMY_UNIT; // FIRST_ENEMY_UNIT + i in a byte
    if (SerialRuntime_BeginSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    SerialRuntime_WaitForSend();
    WaitFrames(2);
  }
  // Empty places up to three.
  for (; i < LINK_PARTY_SIZE; i++) {
    ((struct BattleUnit *)buffer)->side = BATTLE_UNIT_ABSENT;
    if (SerialRuntime_BeginSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    SerialRuntime_WaitForSend();
    WaitFrames(2);
  }

  // The Djinn recovery list, its owners renumbered for the opponent.
  size = LINK_PARTY_OFFER_BYTES;
  MemFree(buffer);
  buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
  /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the list pointer's copy
   * is emitted before the count's address as in the reference. 2026-10-13: the
   * plain call differs 2 of 152 (adds r4, r6, #0 is scheduled after the count's
   * load); list set before the copy differs 6; list as buffer + 8, list set in
   * the for's initializer, and the offer state read into a local first differ
   * the same 2. */
  do {
    Iwram_CopyWords(buffer, Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY),
                    size);
  } while (0);
  list = &((struct DjinnRecoveryTable *)buffer)->list;
  for (j = 0; j < list->count; j++)
    list->entries[j].unitId = work->ownerSlots[list->entries[j].unitId];
  if (SerialRuntime_BeginSend(buffer, LINK_PARTY_OFFER_BYTES) != -1) {
    SerialRuntime_WaitForSend();
    WaitFrames(1);
    WaitFrames(2);
  }
  MemFree(buffer);
}

//--------------------------------------------------------------------------
// Unused. Copies the first four characters of a name into a buffer on its
// own stack, an underscore for each one that is missing, and returns a
// pointer past them into that buffer, which no longer exists once it
// returns. Its first two arguments are ignored.
//--------------------------------------------------------------------------
u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source) {
  u8 buffer[5];
  u8 *base = buffer;
  u32 fill = '_';
  u8 *current = base;
  u8 *dst = current;
  s32 count = 3;

  do {
    u32 value = *source;

    *dst = value;
    source++;
    dst++;
    if ((u8)value == 0)
      *current = fill;
    current++;
    count--;
  } while (count >= 0);
  base[4] = 0;
  return dst;
}

//--------------------------------------------------------------------------
// In a link battle, shows the "EXEC" signature to the other console and
// waits up to 30 frames for it to show the same. Returns 0 once both do (or
// outside a link battle), and -1 if the battle is paused, the link is lost
// for more than LINK_MAX_MISSED_FRAMES frames or the signatures never meet.
//--------------------------------------------------------------------------
s32 BattlePres_WaitSync(void) {
  struct BattleWork *work = gBattleWork;
  u16 *peer;
  u16 *signature;
  s32 missed = 0;
  s32 i;

  if (work->twoSided != 0) {
    u32 other = work->linkSide ^ 1;

    peer = (u16 *)(gSerialPeerPayloads[other] + 4);
    signature = (u16 *)gSerialTransfer.payload;
    if (work->linkPaused == 0) {
      // Publish the execution signature, then wait for the peer's final pair.
      signature[0] = 'E';
      signature[1] = 'X';
      signature[2] = 'E';
      signature[3] = 'C';

      for (i = 0; i < 30; i++) {
        if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
          missed++;
          if (missed > LINK_MAX_MISSED_FRAMES)
            return -1;
        } else {
          missed = 0;
          if (signature[2] == peer[2] && signature[3] == peer[3])
            return 0;
        }
        WaitFrames(1);
      }
    }
    return -1;
  }
  return 0;
}

//--------------------------------------------------------------------------
// In a link battle, after each action and after the end of each round,
// steps both consoles through the same handshake: "ex" "TU", then "rn",
// "EXEC", "tu" and finally "RN". Each step shows its signature and waits
// for the other console to show it too, failing if the other console
// shows a different earlier step. Returns -1 if the battle is paused, the
// link is lost for more than LINK_MAX_MISSED_FRAMES frames, or the steps
// disagree; 0 once in step, or outside a link battle.
//--------------------------------------------------------------------------
s32 BattlePres_SyncTurn(void) {
  struct BattleWork *work = gBattleWork;
  u16 *peer;
  u16 *signature;
  s32 missed = 0;
  /* FAKEMATCH: never used: the reference's frame keeps twenty bytes that no
   * instruction touches; without it 20 of 232 differ (no sub sp, #20, and the
   * branches and literal offsets shift). */
  u16 spare[10];

  if (work->twoSided != 0) {
    peer = gLinkPeerSignatures[work->linkSide ^ 1];
    signature = (u16 *)gSerialTransfer.payload;
    if (work->linkPaused != 0)
      return -1;

    // Publish "ex"/"TU" while both sides still agree on the earlier phase.
    signature[0] = 'e';
    signature[1] = 'x';
    signature[4] = 'T';
    signature[5] = 'U';
    WaitFrames(1);
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (signature[2] != peer[2] || signature[3] != peer[3])
          return -1;
        if (signature[0] == peer[0] && signature[1] == peer[1] &&
            signature[4] == peer[4] && signature[5] == peer[5])
          break;
      }
      WaitFrames(1);
    }

    // Advance to "rn" only after the peer has reached "TU".
    signature[6] = 'r';
    signature[7] = 'n';
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (signature[4] != peer[4] || signature[5] != peer[5])
          return -1;
        if (signature[6] == peer[6] && signature[7] == peer[7])
          break;
      }
      WaitFrames(1);
    }

    // Confirm "rn" before publishing the execution signature.
    signature[0] = 'E';
    signature[1] = 'X';
    signature[2] = 'E';
    signature[3] = 'C';
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (signature[6] != peer[6] || signature[7] != peer[7])
          return -1;
        if (signature[0] == peer[0] && signature[1] == peer[1] &&
            signature[2] == peer[2] && signature[3] == peer[3])
          break;
      }
      WaitFrames(1);
    }

    // Confirm the execution signature before acknowledging with "tu".
    signature[4] = 't';
    signature[5] = 'u';
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (signature[0] != peer[0] || signature[1] != peer[1] ||
            signature[2] != peer[2] || signature[3] != peer[3])
          return -1;
        if (signature[4] == peer[4] && signature[5] == peer[5])
          break;
      }
      WaitFrames(1);
    }

    // "RN": wait for the other console to move on from "rn".
    signature[6] = 'R';
    signature[7] = 'N';
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (peer[6] != 'r' || peer[7] != 'n')
          break;
      }
      WaitFrames(1);
    }
  }
  return 0;
}

//--------------------------------------------------------------------------
// Records, for each party member, the unit id the link opponent knows them
// by (FIRST_ENEMY_UNIT plus their place in the party).
//--------------------------------------------------------------------------
s32 BattleParty_AssignMemberSlots(void) {
  u16 activeMembers[8];
  struct BattleWork *battleState = gBattleWork;
  s32 partySize = BattleParty_PrepareActiveOwners(activeMembers);
  s32 memberSlot;
  s32 unitId;

  for (memberSlot = 0; memberSlot < partySize; memberSlot++) {
    unitId = activeMembers[memberSlot];
    battleState->ownerSlots[unitId] =
        memberSlot - FIRST_ENEMY_UNIT; // FIRST_ENEMY_UNIT + slot in a byte
  }
}

//--------------------------------------------------------------------------
// Clears the link transfer's payload, the signatures included.
//--------------------------------------------------------------------------
s32 Battle_ClearSerialTransferPayload(void) {
  return Iwram_ClearWords(gSerialTransfer.payload, 16);
}

/* FAKEMATCH: the one-pass block keeps the seed store in place in
 * Battle_RunEncounter: the reference stores the zero held in r5 and only then
 * sets wait's zero in r5. 2026-10-13: as a plain assignment 4 of 645 differ
 * (the seed's address and wait's zero are scheduled around the store); the
 * store in the for's initializer, after a comma or chained with wait, differs
 * the same 4; wait = 0 set before it 18. */
#define BattleRandom_ClearSeed()                                               \
  do {                                                                         \
    gBattleRandomSeed = 0;                                                     \
  } while (0)

// BattleWork.partyStatus: how the party status window draws, with the
// Summon command offered or not.
#define PARTY_STATUS_NO_SUMMONS 1
#define PARTY_STATUS_SUMMONS 3

// Battle_RunEncounter's results besides the number of enemies defeated.
#define ENCOUNTER_PARTY_LOST -1
#define ENCOUNTER_FLED 999

// BattleWork.defeatState: whether the last enemy fell alone, or with others
// before it ("has been downed!" or "'s party goes down!"); 3 when a party
// member's action ended the battle with a single defeat.
#define DEFEAT_ONE 0
#define DEFEAT_SEVERAL 1
#define DEFEAT_BY_PARTY_MEMBER 3

// The size of the block a link battle copies its field state into.
#define LINK_OWNER_STATES_SIZE 0x7c8

//--------------------------------------------------------------------------
// Fights the battle against the enemy formation enemyGroup, from the
// opening transition to the end, and returns how it ended: the number of
// enemies defeated when the party won (or the link broke), ENCOUNTER_FLED
// when the party ran, or ENCOUNTER_PARTY_LOST.
//
// Each round, the party's commands are chosen (or chosen automatically
// under FLAG_AUTO_BATTLE), every unit acts in turn order, and the round ends
// with the turn-end effects; the Djinn tutorial adds its two messages
// after each round.
//--------------------------------------------------------------------------
s32 Battle_RunEncounter(s32 enemyGroup) {
  struct BattleCamera *camera;
  s32 *focus;
  struct BattleWork *battle;
  u8 *ownerStates;
  struct BattlePresentationTransition *transition;
  struct BattleDisplayWork *display;
  s32 enemyCount;
  u8 *ownerStateDestination;
  s32 actionCount;
  s32 i;
  s32 result;
  struct UiWindow *tutorialWindow;
  s32 linkWaitFrames;
  u32 copyIndex;
  u32 actingUnit;
  s16 cue;
  u8 unused[48]; // never used: the native frame keeps these 48 bytes
  volatile u32 fill;

  // The battle's work areas.
  camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  battle = Runtime_AllocateBlock(HEAP_SLOT_BATTLE, sizeof(struct BattleWork));
  ownerStates = Runtime_AllocateBlock(54, LINK_OWNER_STATES_SIZE);
  transition = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_BACKGROUND, 32);
  Runtime_AllocateBlock(11, 0x280);
  focus = camera->pos;
  /* FAKEMATCH: needed to match: result starts at 0 and is never read before
   * each exit sets it; CSE takes the setup zeros from its register, r7 as
   * listed (without it 262). 2026-10-13: initialised at its declaration 130;
   * set after Scheduler_ResetTaskTable 262; the zero in handle 20 (setup
   * matches, handle and result swap r5/r7); in wait 120, pos 113, actor 121, i
   * 323, count 363. */
  result = 0;
  Iwram_ClearWords(ownerStates, LINK_OWNER_STATES_SIZE);
  Scheduler_ResetTaskTable();
  transition->frames = 0;
  transition->targetYaw = 0x2000;
  transition->flag = 1;
  transition->unk18 = 0;
  transition->unk1c = 0;
  REG_DISPCNT = 1;
  GameFlag_SetBitFar(0x103);
  GameFlag_SetBitFar(0x169);
  Render_ResetTransformState();

  Dma_Fill32Cell(fill, 0, camera, sizeof(struct BattleCamera));
  Dma_Fill32Cell(fill, 0, battle, sizeof(struct BattleWork));
  battle->promptSlot = -1;
  battle->enemyGroup = enemyGroup;
  display = Runtime_AllocateBlock(37, sizeof(struct BattleDisplayWork));
  Dma_Fill32Cell(fill, 0, display, sizeof(struct BattleDisplayWork));
  battle->background = Event_GetSpecialValueFar();
  Runtime_AllocateBlock(4, 0xe00);
  Runtime_AllocateBlock(3, 0x600);
  ObjectSystem_InitializeFar(4);
  if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0)
    UiWork_InitializeFar(1);
  else
    UiWork_InitializeFar(0);

  // The camera's starting view, and the enemies.
  focus[0] = 0;
  focus[1] = 0x400000;
  focus[2] = 0;
  camera->eye[0] = 0;
  camera->eye[1] = 0xb40000;
  camera->eye[2] = 0x400000;
  camera->yaw = 0x2800;
  camera->pitch = 0x5000;
  camera->distance = 0x1000000;
  enemyCount = BattleFormation_BuildEnemyList(battle->enemyGroup);

  // A link battle: wait for the connection, take this console's side, keep
  // a copy of the field state, and fight as the formation's first type.
  if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
    battle->twoSided = 1;
    Battle_SetStart(4);
  }
  if (battle->twoSided != 0) {
    BattleRandom_ClearSeed();
    for (linkWaitFrames = 0;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) == LINK_BOTH_CONNECTED)
        break;
      WaitFrames(1);
      linkWaitFrames++;
      if (linkWaitFrames > LINK_MAX_MISSED_FRAMES) {
        battle->linkPaused = 1;
        break;
      }
    }
    battle->linkSide = SIO_MULTI_PLAYER_ID();
    ownerStateDestination = gBattleOwnerStates;
    for (copyIndex = 0; copyIndex < LINK_OWNER_STATES_SIZE; copyIndex++)
      *ownerStateDestination++ = gActorSpriteSlots[copyIndex];
    enemyCount = GameFlag_GetByteFar(0x3f0);
    BattleParty_AssignMemberSlots();
    battle->battleType = FORMATION_SHUFFLED;
  }
  Scheduler_AddOrUpdateCallback(BattlePresentation_UpdateCamera, 0xc7f);

  // The battle music: the scene's own cue, or the usual battle songs.
  cue = gGameState.sceneCue;
  if (cue != 0) {
    AudioCommand_PlayFar(cue);
    if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
      AudioCommand_PlayFar(55);
      Sound_LoadPresetParameters(4);
    }
  } else {
    AudioCommand_PlayFar(51);
    AudioCommand_PlayFar(76);
  }

  // The units, the windows and the background.
  BattleParty_CollectUnitList();
  BattleUnit_RefreshPlacement();
  BattleParty_ReconcileDjinnRecovery();
  BattleSummon_UpdateAvailability();
  if (((struct BattleSummonState *)Djinn_GetRecoveryTableFar(
           DJINN_RECOVERY_PARTY))
          ->availableMask != 0)
    battle->partyStatus = PARTY_STATUS_SUMMONS;
  else
    battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
  UiWindow_CreateWithLayoutBoundsFar(9);
  Camera_InitDefaultTransform();
  BattleObjPut();
  BattlePresentation_InitializeWorkAndResetState();
  BattleBackground_Load(1, battle->background, 0);
  BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
  BattlePres_SetupTransitionScene(0, 0, 0, 190);
  Party_ReservedNoOp5B14(1);
  REG_BLDCNT = 0;
  Summon_ClearWorkFields();
  battle->promptSlot = VramCache_AllocEntry(128);

  // A random encounter may open with a first strike (1 in 16) or a
  // surprise (1 in 32 of the rest); the Djinn tutorial always strikes first.
  battle->encounterMode = BATTLE_ENCOUNTER_NORMAL;
  if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0) {
    battle->encounterMode = BATTLE_ENCOUNTER_PARTY_FIRST;
  } else if (gGameState.battleStart == 0) {
    if ((BattleRandomFar() & 15) == 0)
      battle->encounterMode = BATTLE_ENCOUNTER_PARTY_FIRST;
    else if ((BattleRandomFar() & 31) == 0)
      battle->encounterMode = BATTLE_ENCOUNTER_ENEMIES_FIRST;
  }
  BattlePres_RunEntryTransition(enemyCount, enemyGroup);
  transition->flag = 0;
  gCpuLoadDisplayEnabled = 0;
  Scheduler_AddOrUpdateCallback(BattleActor_UpdateSpritePriorities, 0xc80);

  // The rounds.
  for (;;) {
    Battle_ReservedNoOp9B2C();
    BattleSummon_UpdateAvailability();
    if (((struct BattleSummonState *)Djinn_GetRecoveryTableFar(
             DJINN_RECOVERY_PARTY))
            ->availableMask != 0)
      battle->partyStatus = PARTY_STATUS_SUMMONS;
    else
      battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
    transition->targetYaw = 0x2800;
    transition->frames = 60;
    PartyStatusDrawFar(battle->partyStatus);

    // Every unit's action for the round, in turn order.
    Iwram_ClearWords(battle->actions, sizeof(battle->actions));
    Resource_ResetEntry(battle->promptSlot);
    if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0) {
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
      actionCount = BattlePresentation_BuildActions(battle->actions);
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
    } else {
      actionCount = BattlePresentation_BuildSortedUnitEntries(battle->actions);
    }
    battle->promptSlot = VramCache_AllocEntry(128);
    PartyStatusDrawFar(battle->partyStatus);
    if (actionCount < 0)
      goto aborted;

    // Each action, until one side is down or the party runs.
    for (i = 0; i < actionCount; i++) {
      actingUnit = battle->actions[i].unitId;
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
      if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0) {
        if (BattlePresentation_DispatchAction(&battle->actions[i],
                                              i == 0 ? 10 : 0) == 1)
          goto fled;
      } else {
        if (BattlePres_RunAction(&battle->actions[i]) == 1)
          goto fled;
      }
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
      if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) == 0)
        goto party_lost;
      if (BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) == 0) {
        if (actingUnit <= 7 && battle->spoils.defeated == 1)
          battle->defeatState = DEFEAT_BY_PARTY_MEMBER;
        goto won;
      }
      if (BattlePres_SyncTurn() < 0)
        goto aborted;
    }

    // The end of the round.
    battle->encounterMode = BATTLE_ENCOUNTER_NORMAL;
    Battle_ReservedNoOpF674();
    Battle_ProcessRoundEnd();
    BattleMotion_DestroyAllSlotObjects();
    if (battle->twoSided != 0) {
      if (BattlePres_SyncTurn() < 0)
        goto aborted;
    } else {
      WaitFrames(20);
    }
    if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0)
      continue;

    // The Djinn tutorial explains unleashing and standby.
    tutorialWindow = UiText_OpenMessageWindowFar(MsgTutorialUnleashed, 0, 4, 1);
    while (UiWork_IsCompleteFar() == 0)
      WaitFrames(1);
    UiWork_FinalizeFar(tutorialWindow, 1);
    WaitFrames(1);
    tutorialWindow = UiText_OpenMessageWindowFar(MsgTutorialStandby, 10, 4, 1);
    BattlePresentation_WaitForPromptAt(92, 24);
    UiWork_FinalizeFar(tutorialWindow, 1);
    WaitFrames(1);
  }

won:
  // Name the defeat ("X has been downed!" or "X's party goes down!") after
  // the first enemy class to fall, then hand out the spoils.
  Battle_ClearSerialTransferPayload();
  if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0) {
    if (battle->twoSided != 0)
      AudioCommand_PlayFar(58);
    if (battle->spoils.defeated != 0) {
      AudioCommand_PlayFar(58);
      if (battle->defeatState <= DEFEAT_SEVERAL) {
        BattleUnitSetFar(FIRST_ENEMY_UNIT,
                         battle->enemyClasses[battle->firstDefeated], 26);
        UiWork_ClearValueNameTablesFar();
        UiWork_PushValueSlotFar(FIRST_ENEMY_UNIT, VALUE_UNIT_NAME);
        UiText_ShowMessageAndWaitCoreFar(battle->defeatState + MsgDowned);
        BattlePresentation_WaitForAdvance();
      }
    }
    Battle_AwardSpoils();
  }
  AudioCommand_PlayFar(17);
  Fade_ToBlack(30);
  Fade_Wait();
  result = battle->spoils.defeated;
  goto finished;

aborted:
  // The link broke, or no actions could be gathered.
  Battle_ClearSerialTransferPayload();
  Scheduler_SuspendTasks(0);
  REG_DISPCNT = 1;
  result = battle->spoils.defeated;
  GameFlag_SetBitFar(0x3e8);
  goto finished;

party_lost:
  // "X goes down in defeat..." for a lone member, else "X's party has been
  // defeated!".
  Battle_ClearSerialTransferPayload();
  AudioCommand_PlayFar(59);
  UiWork_ClearValueNameTablesFar();
  UiWork_PushValueSlotFar(gGameState.activeOwners[0], VALUE_UNIT_NAME);
  if (BattleParty_PrepareActiveOwners(0) == 1)
    UiText_ShowMessageAndWaitCoreFar(MsgGoesDownInDefeat);
  else
    UiText_ShowMessageAndWaitCoreFar(MsgPartyDefeated);
  BattlePresentation_WaitForAdvance();
  AudioCommand_PlayFar(17);
  Fade_ToBlack(30);
  result = ENCOUNTER_PARTY_LOST;
  Fade_Wait();
  goto finished;

fled:
  AudioCommand_PlayFar(17);
  Fade_ToBlack(30);
  Fade_Wait();
  result = ENCOUNTER_FLED;

finished:
  BattleParty_ResetActiveRuntimeFields();
  Battle_ReservedNoOpF674();
  BattlePlacement_UpdateTimedEntries();
  Battle_SetStart(0);
  Scheduler_RemoveCallback(BattleActor_UpdateSpritePriorities);
  Runtime_ReleaseHeapBlock10();
  return result;
}

// The party fights four at a time, three in a link battle; six enemies at
// most, three in a link battle.
#define PARTY_BATTLE_SIZE 4
#define ENEMY_BATTLE_SIZE 6

//--------------------------------------------------------------------------
// Lists the party members who fight (the first four of the party, three in
// a link battle) and marks each unit as a party member. owners, if not
// null, receives their ids ended by BATTLE_UNIT_LIST_END. Returns how many
// fight.
//--------------------------------------------------------------------------
s32 BattleParty_PrepareActiveOwners(u16 *owners) {
  s32 limit;
  s32 count;
  s32 index;

  limit = PARTY_BATTLE_SIZE;
  if (gBattleWork->twoSided != 0)
    limit = LINK_PARTY_SIZE;

  count = Party_CountActiveOwnersFar();
  if (count > limit)
    count = limit;

  for (index = 0; index < count; index++) {
    s32 owner = gGameState.activeOwners[index];

    if (owners != 0)
      *owners++ = owner;
    UnitGetFar(owner)->side = BATTLE_UNIT_PARTY;
  }

  if (owners != 0)
    *owners = BATTLE_UNIT_LIST_END;
  return count;
}

//--------------------------------------------------------------------------
// Lists the enemy units present, ended by BATTLE_UNIT_LIST_END, and returns
// how many there are (0 when unitIds is null).
//--------------------------------------------------------------------------
s32 BattleEnemyList(s16 *unitIds) {
  s16 *output;
  s32 count;
  s32 id;
  s32 lastId;
  s32 listed;

  output = unitIds;
  listed = 0;
  lastId = ENEMY_BATTLE_SIZE;
  count = 0;
  if (output != NULL) {
    if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
      lastId = LINK_PARTY_SIZE;
    }
    id = FIRST_ENEMY_UNIT;
    lastId += FIRST_ENEMY_UNIT;
    for (; id < lastId; id += 1) {
      if (UnitGetFar(id)->side != BATTLE_UNIT_ABSENT) {
        *output = id;
        listed += 1;
        output += 1;
      }
    }
    *output = BATTLE_UNIT_LIST_END;
    count = listed;
  }
  return count;
}

//--------------------------------------------------------------------------
// Lists the units still standing (HP above 0) on the sides in sideMask
// (BATTLE_SIDE_PARTY, BATTLE_SIDE_ENEMIES or both), party first. unitIds,
// if not null, receives them ended by BATTLE_UNIT_LIST_END. Returns how
// many there are.
//--------------------------------------------------------------------------
s32 BattleParty_ListLivingUnits(s32 sideMask, u16 *unitIds) {
  u16 activeMembers[8];
  u16 *base;
  s32 remaining;
  s32 lastEnemy;
  u16 *output;
  s32 livingCount;
  s32 enemyCapacity;
  u16 *member;
  s32 unitId;
  s16 hp;
  s32 activeCount;
  struct BattleUnit *unit;

  output = unitIds;
  livingCount = 0;
  enemyCapacity = ENEMY_BATTLE_SIZE;
  if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
    enemyCapacity = LINK_PARTY_SIZE;
  }
  // Count the living party members before scanning the enemy unit-id range.
  if (sideMask & BATTLE_SIDE_PARTY) {
    base = activeMembers;
    activeCount = BattleParty_PrepareActiveOwners(base);
    if (livingCount < activeCount) {
      member = base;
      remaining = activeCount;
      do {
        unitId = *member;
        member += 1;
        hp = UnitGetFar(unitId)->hp;
        if (hp > 0) {
          if (output != NULL) {
            *output = unitId;
            output += 1;
          }
          livingCount += 1;
        }
        remaining -= 1;
      } while (remaining != 0);
    }
  }
  if (sideMask & BATTLE_SIDE_ENEMIES) {
    // remaining walks the enemy unit ids here.
    remaining = FIRST_ENEMY_UNIT;
    lastEnemy = enemyCapacity + FIRST_ENEMY_UNIT;
    if (remaining < lastEnemy) {
      do {
        unit = UnitGetFar(remaining);
        if ((unit->side != BATTLE_UNIT_ABSENT) && (unit->hp > 0)) {
          if (output != NULL) {
            *output = remaining;
            output += 1;
          }
          livingCount += 1;
        }
        remaining += 1;
      } while (remaining < lastEnemy);
    }
  }
  if (output != NULL) {
    *output = BATTLE_UNIT_LIST_END;
  }
  return livingCount;
}

//--------------------------------------------------------------------------
// Lists the units in the battle's unit lists on the sides in sideMask,
// party first, skipping those removed from the battle. unitIds, if not
// null, receives them ended by BATTLE_UNIT_LIST_END. Returns how many
// there are.
//--------------------------------------------------------------------------
s32 BattleParty_ListActorIds(s32 sideMask, u16 *unitIds) {
  struct BattleWork *battle;
  s32 count;
  s32 i;

  battle = gBattleWork;
  count = 0;
  if (sideMask & BATTLE_SIDE_PARTY) {
    for (i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      if (battle->partyUnits[i] != BATTLE_UNIT_REMOVED) {
        if (unitIds != NULL)
          *unitIds++ = battle->partyUnits[i];
        count++;
      }
    }
  }
  if (sideMask & BATTLE_SIDE_ENEMIES) {
    for (i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
      if (battle->enemyIds[i] != BATTLE_UNIT_REMOVED) {
        if (unitIds != NULL)
          *unitIds++ = battle->enemyIds[i];
        count++;
      }
    }
  }
  if (unitIds != NULL)
    *unitIds = BATTLE_UNIT_LIST_END;
  return count;
}

//--------------------------------------------------------------------------
// actor_commit_placement.c
// Battle: placing every unit's object
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Readies the sprite of every unit in the battle (releasing those of units
// that left) and sets each one's display mode and motion afresh.
//--------------------------------------------------------------------------
void BattleObjPut(void) {
  s16 unitIds[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  BattleActor_SpawnObjectsForList(unitIds, 1);
}

//--------------------------------------------------------------------------
// list_all_units_and_submit.c
// Battle: placing every unit's object
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Readies the sprite of every unit in the battle (releasing those of units
// that left), keeping their modes and motions.
//--------------------------------------------------------------------------
void BattleParty_ListAllUnitsAndSubmit(void) {
  s16 unitIds[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  BattleActor_SpawnObjectsForList(unitIds, 0);
}

//--------------------------------------------------------------------------
// Returns the palette of unit id's battle sprite.
//--------------------------------------------------------------------------
s32 BattleMotion_GetSlotPalette(s32 id) { return BattleObjGet(id)->palette; }

// The battle has six enemy sprite slots; a large enemy takes two side by
// side, a small one (Summon_IsEntryFlagged) one.
#define ENEMY_SPRITE_SLOTS 6

//--------------------------------------------------------------------------
// Returns 1 if an enemy of class classId would find room among the enemy
// sprite slots: one free slot for a small enemy, two free side by side for
// a large one.
//--------------------------------------------------------------------------
s32 EnemyCheck(s32 classId) {
  struct BattleWork *battle;
  s32 small;
  s32 i;

  small = Summon_IsEntryFlagged(classId);
  battle = gBattleWork;
  for (i = 0; i < ENEMY_SPRITE_SLOTS; i++) {
    if (battle->spriteSlots[i] != 0)
      continue;
    if (small != 0)
      break;
    if (i < ENEMY_SPRITE_SLOTS - 1 && battle->spriteSlots[i + 1] == 0)
      break;
  }
  return i != ENEMY_SPRITE_SLOTS;
}

// The two battle sprites whose next sprite is loaded with them (into a
// second free place), the pass after them loading animation + 1.
#define SPRITE_WITH_SECOND_PART_A 476
#define SPRITE_WITH_SECOND_PART_B 483

//--------------------------------------------------------------------------
// Loads an enemy unit's battle sprite into the first free enemy sprite
// slots (one for a small enemy, two side by side for a large one) and
// records the unit as their owner. Two sprites take a second part,
// loaded the same way into further slots. Returns the first slot and the
// sprite as (slot << 12) | sprite, or 0 for a party member's unit or when
// a load fails.
//--------------------------------------------------------------------------
s32 SummonSlot_RegisterActorSprites(s32 unit) {
  s32 pass;
  struct BattleWork *battle = gBattleWork;
  struct BattleUnit *actor = UnitGetFar(unit);
  s32 small = Summon_IsEntryFlagged(actor->classId);
  /* FAKEMATCH: initializing the two halves preserves the word copy into pass,
   * mov r4, r10; one s32 zero folds that copy into movs r4, #0 (one instruction
   * differs). 2026-10-13: result as an s32 set to 0, with pass = result or pass
   * = 0, differs 2 of 114; pass = result = 0 differs 6, result = pass = 0 19.
   */
  union {
    s32 value;
    u16 halves[2];
  } result;
  s32 sprite;

  result.halves[0] = 0;
  result.halves[1] = 0;
  sprite = Summon_GetEntryValue(actor->classId);
  pass = result.value;
  do {
    s32 slot;

    if (actor->classIndex != 0)
      continue;

    // The first free slot (and, for a large enemy, the one after it).
    for (slot = 0; slot < ENEMY_SPRITE_SLOTS; slot++) {
      if (battle->spriteSlots[slot] != 0)
        continue;
      if (small)
        break;
      if (slot < ENEMY_SPRITE_SLOTS - 1 && battle->spriteSlots[slot + 1] == 0)
        break;
    }

    if (slot == ENEMY_SPRITE_SLOTS)
      break;

    {
      s32 palette = Summon_GetEntryFlag1Field(actor->classId);
      s32 buffer = (slot << 14) + (s32)Ram_ActorSpriteSlots;

      if (ResourceSlot_LoadFar(slot, (u32 *)buffer, sprite + pass, palette) ==
          0)
        return 0;
    }

    /* FAKEMATCH: cancelling updates retain sprite's allocation
     * priority; a self-assignment instead differs in 18 rows.
     * 2026-10-13: without them 18 of 114 differ; the exit test written
     * !(== || ==) or as a switch on sprite 18; the load's
     * value as pass + sprite 16. */
    sprite ^= 1;
    sprite ^= 1;
    if (pass == 0)
      result.value = (slot << 12) | sprite;

    battle->spriteSlots[slot] = unit;
    if (!small)
      battle->spriteSlots[slot + 1] = unit;

    if (sprite != SPRITE_WITH_SECOND_PART_A &&
        sprite != SPRITE_WITH_SECOND_PART_B)
      break;
  } while (++pass <= 1);

  return result.value;
}

//--------------------------------------------------------------------------
// Frees every enemy sprite slot unit unitId holds, unloading its sprite.
//--------------------------------------------------------------------------
s32 BattleMotion_ReleaseObjectSlotByValue(s32 unitId) {
  struct BattleWork *work = gBattleWork;
  s32 index;

  for (index = 0; index < ENEMY_SPRITE_SLOTS; index++) {
    if (work->spriteSlots[index] == unitId) {
      ResourceSlot_LoadFar(index, 0, 0, 0);
      work->spriteSlots[index] = 0;
    }
  }
}

// gResource_SlotAssignments: each entry is a weapon's item id in the low nine
// bits and its kind above them; the list ends with -1.
#define WEAPON_ITEM_MASK 0x1ff
#define WEAPON_KIND_SHIFT 9
#define WEAPON_KIND_NONE 6

//--------------------------------------------------------------------------
// Returns the kind of the weapon whose item id is itemId (0 to 5), which
// picks a party member's attack animation, or WEAPON_KIND_NONE for an item
// the list lacks.
//--------------------------------------------------------------------------
s32 Resource_FindFreeSlot(s32 itemId) {
  struct SlotAssignmentView {
    volatile u16 entries[52];
  };
  s32 index;
  s32 entry;
  u32 value;
  s32 kind;

  for (index = 0;; index++) {
    entry = gResource_SlotAssignments[index];
    if (itemId == (entry & WEAPON_ITEM_MASK)) {
      /* FAKEMATCH: the volatile array member preserves the native second ldrh
       * [table, index]. A scalar pointer needs an extra address add; a
       * nonvolatile member reuses the first load. */
      value = ((struct SlotAssignmentView *)gResource_SlotAssignments)
                  ->entries[index];
      kind = value >> WEAPON_KIND_SHIFT;
      break;
    }
    if ((s16)entry == -1) {
      kind = WEAPON_KIND_NONE;
      break;
    }
  }
  return kind;
}

// The inventory's weapon slot, for Inventory_FindEquippedFar.
#define EQUIP_WEAPON 1

//--------------------------------------------------------------------------
// Returns the weapon sprite a party member's attack shows for the kind of
// weapon they have equipped (gBattleUnit_WeaponAnimsClass*), or 0 for no
// weapon, for Felix, or for a unit that is no party member.
//--------------------------------------------------------------------------
s32 BattleUnit_LookupWeaponValueByClass(s32 id) {
  struct BattleUnit *unit;
  s32 slot;
  s32 weaponSprite;

  unit = UnitGetFar(id);
  slot = Inventory_FindEquippedFar(id, EQUIP_WEAPON);
  weaponSprite = 0;
  if (slot >= 0) {
    s32 kind;

    kind = Resource_FindFreeSlot(unit->inventory[slot] & WEAPON_ITEM_MASK);
    switch (unit->classId) {
    case ISAAC:
      weaponSprite = gBattleUnit_WeaponAnimsClass0[kind];
      break;
    case GARET:
      weaponSprite = gBattleUnit_WeaponAnimsClass1[kind];
      break;
    case IVAN:
      weaponSprite = gBattleUnit_WeaponAnimsClass2[kind];
      break;
    case MIA:
      weaponSprite = gBattleUnit_WeaponAnimsClass3[kind];
      break;
    case FELIX:
      break;
    case JENNA:
      weaponSprite = gBattleUnit_WeaponAnimsClass5[kind];
      break;
    }
  }
  return weaponSprite;
}

// Isaac and Garet hold a Machete with sprites of their own.
#define ITEM_MACHETE 15
#define SPRITE_ISAAC_MACHETE 480
#define SPRITE_GARET_MACHETE 482

// BattleObj.scale and MotionObject scales are 16.16 fixed point.
#define SCALE_ONE 0x10000

// BattleObj.effect while no effect is attached.
#define BATTLE_OBJ_NO_EFFECT 0x1fe

//--------------------------------------------------------------------------
// Creates the battle object of unit at (x, y) on the battlefield, in
// pixels: its sprite (an enemy's from its class, sharing the slot it
// already holds; a party member's battle sprite), palette, size and weapon
// sprite, turned to face the centre, and starts its motion script.
//--------------------------------------------------------------------------
void BattleObjMake(struct BattleObj *actor, s32 unit, s32 x, s32 y) {
  s32 fixedX;
  s32 fixedY;
  s32 palette;
  s32 sprite;
  s32 loadedSprite;
  struct BattleUnit *unitRecord;
  struct MotionObject *object;
  s32 angle;
  s32 weaponSprite;
  u8 classId;

  fixedX = x << 16;
  fixedY = y << 16;
  object = Object_CreateFar(0xf000, fixedX, 0, fixedY);
  unitRecord = UnitGetFar(unit);
  palette = 0;
  loadedSprite = SummonSlot_RegisterActorSprites(unit);

  // The sprite: an enemy's from its class, a party member's by character;
  // enemies use the second palette.
  if (unitRecord->classIndex == 0) {
    sprite = Summon_GetEntryValue(unitRecord->classId);
    if (loadedSprite == 0)
      palette = Summon_GetEntryFlag1Field(unitRecord->classId);
    else
      sprite = loadedSprite;
  } else {
    switch (unitRecord->classId) {
    case GARET:
      sprite = BATTLE_CHARACTER(GARET);
      break;
    case MIA:
      sprite = BATTLE_CHARACTER(MIA);
      break;
    case IVAN:
      sprite = BATTLE_CHARACTER(IVAN);
      break;
    case JENNA:
      sprite = BATTLE_CHARACTER(JENNA);
      break;
    case ISAAC:
    default:
      sprite = BATTLE_CHARACTER(ISAAC);
      break;
    }
    if (unit < 0 || unit > 7)
      palette = 1;
  }

  // The enemy classes drawn larger or smaller than their sprites.
  actor->scale = SCALE_ONE;
  switch (unitRecord->classId) {
  case 78:
    actor->scale = 0x19999;
    break;
  case 89:
    actor->scale = 0x18ccc;
    break;
  case 130:
    actor->scale = 0x13333;
    break;
  case 131:
    actor->scale = 0x19999;
    break;
  case 138:
    actor->scale = 0x18000;
    break;
  case 147:
    actor->scale = 0x1cccc;
    break;
  case 149:
    actor->scale = 0x1cccc;
    break;
  case 29:
    actor->scale = 0x10000;
    break;
  case 121:
    actor->scale = 0x1b333;
    break;
  case 148:
    actor->scale = 0x18000;
    break;
  case 150:
    actor->scale = 0x18000;
    break;
  case 151:
    actor->scale = 0x18000;
    break;
  case 152:
    actor->scale = 0x18000;
    break;
  case 153:
    actor->scale = 0x18000;
    break;
  case 154:
    actor->scale = 0x18000;
    break;
  case 155:
    actor->scale = 0x18000;
    break;
  case 156:
    actor->scale = 0x18000;
    break;
  case 157:
    actor->scale = 0x18000;
    break;
  case 47:
    actor->scale = 0x13333;
    break;
  case 48:
    actor->scale = 0x13333;
    break;
  case 49:
    actor->scale = 0x16666;
    break;
  case 84:
    actor->scale = 0x10000;
    break;
  case 85:
    actor->scale = 0x14000;
    break;
  case 128:
    actor->scale = 0x16666;
    break;
  case 129:
    actor->scale = 0x16666;
    break;
  case 94:
    actor->scale = 0x18000;
    break;
  case 98:
    actor->scale = 0x14ccc;
    break;
  case 110:
    actor->scale = 0x13333;
    break;
  case 132:
    actor->scale = 0x10ccc;
    break;
  case 133:
    actor->scale = 0x10ccc;
    break;
  case 134:
    actor->scale = 0x11999;
    break;
  case 135:
    actor->scale = 0x11999;
    break;
  case 136:
    actor->scale = 0x13333;
    break;
  case 137:
    actor->scale = 0x13333;
    break;
  case 141:
    actor->scale = 0x18000;
    break;
  case 144:
    actor->scale = 0x13333;
    break;
  case 145:
    actor->scale = 0x18000;
    break;
  case 146:
    actor->scale = 0x18ccc;
    break;
  case 52:
    actor->scale = 0x14000;
    break;
  case 105:
    actor->scale = 0x14000;
    break;
  case 18:
  case 19:
  case 20:
  case 21:
  case 30:
  case 68:
  case 69:
  case 70:
  case 92:
  case 122:
  case 123:
  case 124:
  case 125:
  case 126:
    actor->scale = 0xe666;
    break;
  }

  actor->object = object;
  actor->anchorX = fixedX;
  actor->anchorZ = fixedY;
  actor->palette = palette;
  actor->resource = sprite;
  weaponSprite = BattleUnit_LookupWeaponValueByClass(unit);
  actor->animation = 0;
  actor->animationEntry = 0;
  actor->effectEntry = 0;
  actor->active = 0;
  actor->fading = 0;
  actor->effect = BATTLE_OBJ_NO_EFFECT;
  classId = unitRecord->classId;
  actor->overlay = weaponSprite;

  if (classId <= GARET &&
      Inventory_GetEquippedItemFar(unitRecord, EQUIP_WEAPON) == ITEM_MACHETE) {
    if (unitRecord->classId == ISAAC) {
      sprite = SPRITE_ISAAC_MACHETE;
      actor->resource = sprite;
    } else {
      sprite = SPRITE_GARET_MACHETE;
      actor->resource = sprite;
    }
    actor->overlay = 0;
  }

  // Face the centre of the battlefield; enemies are drawn larger.
  angle = ArcTan2(y / 8, x) + 0x8000;
  object->angle = angle;
  object->unk59 = 3;
  object->motionFlags = 2;
  if (unitRecord->classIndex == 0) {
    object->scaleX = 0x14ccc;
    object->scaleY = 0x14ccc;
  } else {
    object->scaleX = SCALE_ONE;
    object->scaleY = SCALE_ONE;
  }
  ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                               gBattlePres_ActorObjectScript);
}

//--------------------------------------------------------------------------
// Reads the battlefield position of party place index from
// gBattlePlacement_StepPairs, in pixels.
//--------------------------------------------------------------------------
void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y) {
  index *= 2;
  *x = gBattlePlacement_StepPairs[index];
  *y = gBattlePlacement_StepPairs[index + 1];
}

// BattleWork.enemyIds of an enemy that left the battle, and of a place
// held for one: unit ids 254 and 255 in the list.
#define IS_PLACEHOLDER_ID(id) ((u16)((id) - BATTLE_UNIT_REMOVED) <= 1)

//--------------------------------------------------------------------------
// Lays out count enemies in a column 80 pixels left of the centre, spaced
// by their size (38 pixels around a large enemy, 27 around a small one, 25
// around an empty place, closer when there are more than four) and centred
// on the line. Two large classes stand nearer, 50 pixels left.
//--------------------------------------------------------------------------
void Summon_LayoutPositions(u16 *actorIds, s32 count, s32 *xPositions,
                            s32 *zPositions) {
  s32 totalSpacing = count <= 4 ? 30 : 27;
  s32 z = ((count - 1) * totalSpacing) / 2;
  s32 index;

  for (index = 0; index != count; index++) {
    s32 spacing = 0;

    xPositions[index] = -80;
    if (index != 0) {
      struct BattleUnit *actor;

      spacing = 25;
      if (!IS_PLACEHOLDER_ID(actorIds[index])) {
        actor = UnitGetFar(actorIds[index]);
        spacing = Summon_IsEntryFlagged(actor->classId) ? 27 : 38;
        if (actor->classId == 148 || actor->classId == 121) {
          xPositions[index] = -50;
        }
      }
    }
    z -= spacing / 2;
    zPositions[index] = z;
    spacing = 25;
    if (!IS_PLACEHOLDER_ID(actorIds[index])) {
      struct BattleUnit *actor = UnitGetFar(actorIds[index]);

      spacing = Summon_IsEntryFlagged(actor->classId) ? 27 : 38;
    }
    z -= spacing / 2;
  }
}

//--------------------------------------------------------------------------
// Returns the id of the first free enemy unit, or -1 when all six are taken.
//--------------------------------------------------------------------------
s32 EnemySlotGet(void) {
  s32 i;
  s32 id;

  for (i = 0; i < ENEMY_BATTLE_SIZE; i++) {
    id = i + FIRST_ENEMY_UNIT;
    if (UnitGetFar(id)->side == BATTLE_UNIT_ABSENT)
      break;
  }
  if (i == ENEMY_BATTLE_SIZE)
    return -1;
  return id;
}

//--------------------------------------------------------------------------
// Lays the enemies out again (after one joins or leaves) and moves each
// one's object to its new place.
//--------------------------------------------------------------------------
s32 EnemyUpdate(void) {
  struct BattleWork *work = gBattleWork;
  u16 unitIds[14];
  s32 x[6];
  s32 z[6];
  s32 count;
  s32 i;

  for (i = 0;
       i < ENEMY_BATTLE_SIZE && work->enemyIds[i] != BATTLE_UNIT_LIST_END; i++)
    unitIds[i] = work->enemyIds[i];
  count = i;
  Summon_LayoutPositions(unitIds, count, x, z);
  for (i = 0; i < count; i++) {
    s32 id = work->enemyIds[i];

    if (id != BATTLE_UNIT_REMOVED) {
      struct BattleObj *object = BattleObjGet(id);

      object->anchorX = x[i] << 16;
      object->anchorZ = z[i] << 16;
    }
  }
}

//--------------------------------------------------------------------------
// Places every unit at the start of a battle: numbers the places (party
// members 0 up in their order, enemies 8 to 13) and creates each unit's
// object at its spot.
//--------------------------------------------------------------------------
void BattleUnit_RefreshPlacement(void) {
  struct BattleWork *work;
  u16 ids[14];
  s32 x[6];
  s32 z[6];
  s32 count;
  s32 i;
  s32 place;
  s32 stand;
  s32 n;
  s32 value;
  u8 *p;

  place = 0;
  stand = 0;
  work = gBattleWork;
  count = BattleParty_PrepareActiveOwners(ids);
  value = 0xff;
  for (i = 13; i >= 0; i--)
    work->placement[i] = value;
  p = &work->placement[13];
  for (n = 5, value = 13; n >= 0; n--, value--)
    *p-- = value;

  // The party, at its fixed spots.
  for (i = 0; i < count; place++, i++) {
    s32 id = ids[i];

    work->placement[id] = place;
    BattleObjMake(BattleObjGet(id), id, gBattlePlacement_StepPairs[place * 2],
                  gBattlePlacement_StepPairs[place * 2 + 1]);
  }

  // The enemies, laid out by size.
  for (i = 0;
       i < ENEMY_BATTLE_SIZE && work->enemyIds[i] != BATTLE_UNIT_LIST_END; i++)
    ids[i] = work->enemyIds[i];
  count = i;
  Summon_LayoutPositions(ids, count, x, z);
  for (i = 0; i < count; i++, stand++) {
    s32 id = work->enemyIds[i];

    if (id != BATTLE_UNIT_REMOVED)
      BattleObjMake(BattleObjGet(id), id, x[stand], z[stand]);
  }
}

//--------------------------------------------------------------------------
// contains_id.c
// Battle: searching a list of unit ids
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Returns 1 if the unit at place no is in list.
// list : unit ids, ended by BATTLE_UNIT_LIST_END, 14 at most
// no   : the place (0 to 7 the party, 8 to 13 the enemies)
//--------------------------------------------------------------------------
s32 BattlePlacement_ContainsId(s16 *list, s32 no) {
  u32 i;

  if (no > 7)
    no += FIRST_ENEMY_UNIT - 8; // enemy unit ids start at 128
  i = 0;
  do {
    u16 id = *list++;

    if (id == BATTLE_UNIT_LIST_END)
      break;
    if (id == no)
      return 1;
  } while (++i < 14);
  return 0;
}

//--------------------------------------------------------------------------
// battle.c
// Battle: the units' drawing priority, status icons and poses
//--------------------------------------------------------------------------

#define UNIT_MAX 14 // the party's and the enemies' units together

// MotionObject.recordStorageKind's low four bits: no sprite, one sprite in
// records, or records pointing to a table of up to four.
#define RECORDS_NONE 0
#define RECORDS_SINGLE 1
#define RECORDS_TABLE 2
#define RECORDS_KIND_MASK 15
#define RECORDS_TABLE_SIZE 4

//--------------------------------------------------------------------------
// Runs each frame of a battle: advances every unit's status icon, lifts an
// airborne unit's effect sprite above the others, and draws whichever side
// is nearer the camera in front.
//--------------------------------------------------------------------------
void BattleActor_UpdateSpritePriorities(void) {
  u16 ids[UNIT_MAX];
  s32 priority[2]; // the party's, the enemies'
  s32 i;
  s32 j;
  s32 count;
  struct MotionObject *obj;
  struct AnimationObject *rec;
  struct AnimationObject **list;

  // Every unit's status icon.
  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
  i = 0;
  if (ids[i] != BATTLE_UNIT_LIST_END) {
    for (;;) {
      struct BattleObj *slot = BattleObjGet(ids[i]);

      if (slot != 0) {
        obj = slot->object;

        BattleStatusIcon_Cycle(slot);
        if (slot->effectEntry != 0) {
          rec = GetMotionRecord(obj, 0);

          if (rec != 0) {
            struct SpriteEntry *effect;
            s32 effectPriority = 0;

            if (obj->y != 0) // off the ground
              effectPriority = 9;
            effect = slot->effectEntry;
            if (effect->priority != effectPriority) {
              effect->priority = effectPriority;
              rec->dirty = 1;
            }
          }
        }
      }
      i++;
      if (i >= UNIT_MAX || ids[i] == BATTLE_UNIT_LIST_END)
        break;
    }
  }

  // The camera's turn decides which side is in front.
  if ((s16)gCameraWork->yaw >= 0) {
    priority[0] = 1;
    priority[1] = 2;
  } else {
    priority[0] = 2;
    priority[1] = 1;
  }

  // The party.
  {
    count = BattleParty_ListActorIds(BATTLE_SIDE_PARTY, ids);
    for (i = 0; i < count; i++) {
      struct BattleObj *slot = BattleObjGet(ids[i]);

      if (slot != 0) {
        obj = slot->object;

        switch (obj->recordStorageKind & RECORDS_KIND_MASK) {
        case RECORDS_SINGLE: {
          rec = obj->records;
          rec->part[0].priority = priority[0];
          break;
        }
        case RECORDS_TABLE: {
          list = (struct AnimationObject **)obj->records;
          for (j = 0; j < RECORDS_TABLE_SIZE; j++) {
            rec = *list++;
            if (rec != 0)
              rec->part[0].priority = priority[0];
          }
          break;
        }
        }
      }
    }
  }

  // The enemies.
  {
    count = BattleParty_ListActorIds(BATTLE_SIDE_ENEMIES, ids);
    for (i = 0; i < count; i++) {
      struct BattleObj *slot = BattleObjGet(ids[i]);

      if (slot != 0) {
        obj = slot->object;

        switch (obj->recordStorageKind & RECORDS_KIND_MASK) {
        case RECORDS_SINGLE: {
          rec = obj->records;
          rec->part[0].priority = priority[1];
          break;
        }
        case RECORDS_TABLE: {
          list = (struct AnimationObject **)obj->records;
          for (j = 0; j < RECORDS_TABLE_SIZE; j++) {
            rec = *list++;
            if (rec != 0)
              rec->part[0].priority = priority[1];
          }
          break;
        }
        }
      }
    }
  }
}

//--------------------------------------------------------------------------
// The status icons' bits in BattleObj.activeConditions
//--------------------------------------------------------------------------
#define ICON_POISON 0x01
#define ICON_VENOM 0x02 // deadly poison
#define ICON_STUN 0x04
#define ICON_PSY_SEAL 0x08
#define ICON_SLEEP 0x10
#define ICON_DELUSION 0x20
#define ICON_HAUNT 0x40
#define ICON_GRIM_REAPER 6 // the curse's turns left, 1 to 7, as bits 7 to 13

#define ICON_MAX 14 // the kinds of icon

//--------------------------------------------------------------------------
// Builds the status icon bits of unit id's ailments into slot.
//--------------------------------------------------------------------------
s32 BattleUnit_BuildStatusFlags(s32 id, struct BattleObj *slot) {
  struct BattleUnit *unit = UnitGetFar(id);
  s8 poison = unit->poison;
  u32 flags = 0;

  if (poison == 1) // poisoned
    flags = ICON_POISON;
  if (poison == 2)   // deadly poison
    flags |= poison; // ICON_VENOM
  if (unit->delusion != 0)
    flags |= ICON_DELUSION;
  if (unit->stun != 0) {
    s32 classId;
    flags |= ICON_STUN;
    classId = unit->classId;
    if (classId == 121 || classId == 148) // two classes never show it
      flags &= ~ICON_STUN;
  }
  if (unit->psySeal != 0)
    flags |= ICON_PSY_SEAL;
  if (unit->haunt != 0)
    flags |= ICON_HAUNT;
  if (unit->sleep != 0)
    flags |= ICON_SLEEP;
  if (unit->grimReaper != 0)
    flags |= 1 << (unit->grimReaper + ICON_GRIM_REAPER);
  slot->activeConditions = flags;
}

// The frames each status icon shows before the next one's turn.
#define ICON_CYCLE_FRAMES 80

// The icons' first animations, for units 32 pixels wide and for the rest.
#define ICON_ANIM_NARROW 340
#define ICON_ANIM_WIDE 355

//--------------------------------------------------------------------------
// Shows a unit's status icons one after another, each for
// ICON_CYCLE_FRAMES frames, switching at once when the shown status ends,
// and removes the icon when no status is left.
//--------------------------------------------------------------------------
s32 BattleStatusIcon_Cycle(struct BattleObj *slot) {
  struct AnimationEntry *shownIcon;
  struct AnimationEntry *icon;
  struct AnimationObject *rec;
  struct MotionObject *owner;
  s32 anim;
  s32 handle;
  s32 previous;
  s32 changed = 0;

  if (slot->cycleTimer >= 0)
    slot->cycleTimer--;

  // Move on when the time is up, when the shown status has ended, or when a
  // status appears with no icon shown.
  shownIcon = (struct AnimationEntry *)slot->animationEntry;
  if ((shownIcon == 0
           ? (s16)slot->activeConditions != 0
           : (((s16)slot->activeConditions >> slot->selectedCondition) & 1) ==
                 0) ||
      slot->cycleTimer == 0) {
    anim = -1;
    owner = slot->object;
    if ((s16)slot->activeConditions != 0) {
      // The next status after the shown one, wrapping around.
      previous = slot->selectedCondition;
      for (anim = previous + 1;; anim++) {
        if (anim > ICON_MAX - 1)
          anim = 0;
        if ((((s16)slot->activeConditions >> anim) & 1) != 0)
          break;
      }

      if (previous != anim || shownIcon == 0) {
        slot->selectedCondition = anim;
        changed = 1;
      }
      slot->cycleTimer = ICON_CYCLE_FRAMES;
    } else {
      changed = 1;
    }

    rec = GetMotionRecord(owner, 0);
    if (rec != 0) {
      // Units 32 pixels wide take their own set of icon animations.
      if (anim >= 0) {
        if (rec->width == 32)
          anim += ICON_ANIM_NARROW;
        else
          anim += ICON_ANIM_WIDE;
      }

      if (slot->animationEntry != 0 && changed != 0) {
        ResourceMetadata_UnregisterFar(rec, (s32)slot->animationEntry);
        slot->animationEntry = 0;
      }

      if (anim >= 0 && changed != 0) {
        // The handle is stored first and cleared again when registration fails.
        handle = ResourceMetadata_RegisterFar(rec, anim);
        slot->animationEntry = (struct SpriteEntry *)handle;
        if (handle == -1)
          slot->animationEntry = 0;
        icon = (struct AnimationEntry *)slot->animationEntry;
        if (icon != 0) {
          icon->priority = 3;
          Animation_SetWorkEntryFar(icon, 0);
        }
      }

      rec->dirty = 1;
      if (anim >= 0)
        slot->animation = anim;
      else
        slot->animation = 0;
    }
  }
}

// The battle poses BattlePres_SetActorModeAndAction picks.
#define POSE_ENEMY_HELD 0
#define POSE_STANDING 1
#define POSE_PARTY_HELD 4 // asleep, stunned or held; also a downed member
#define POSE_ENEMY_DOWN 5

//--------------------------------------------------------------------------
// Sets unit id's pose to match its state: held (asleep, stunned or unable
// to move) or down, else standing; then gives its sprites their palette
// row, 14 to 17 by unit.
//--------------------------------------------------------------------------
s32 BattlePres_SetActorModeAndAction(s32 id) {
  struct BattleUnit *unit;
  s32 pose;

  unit = UnitGetFar(id);
  pose = POSE_STANDING;
  if (unit->hp != 0) {
    if (unit->sleep != 0 || unit->stun != 0 || unit->immobile != 0)
      pose = (unit->side != BATTLE_UNIT_ENEMY) * POSE_PARTY_HELD;
  } else {
    // 4 for a party member, 5 for an enemy.
    s32 notEnemy = unit->side ^ pose;
    pose = (u32)(-notEnemy | notEnemy) >> 31;
    pose = POSE_ENEMY_DOWN - pose;
  }

  ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(id)->object, pose);
  ObjectDispatch_ApplyValueToChildrenFar(BattleObjGet(id)->object,
                                         (id & 3) + 14);
}

//--------------------------------------------------------------------------
// release_battle_object_records.c
// Battle: releasing an object's sprites
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Releases every sprite unit no's object holds and leaves it with none.
// no : the unit id
//--------------------------------------------------------------------------
void ReleaseBattleObjectRecords(s32 no) {
  struct BattleObj *slot;
  struct MotionObject *obj;
  s32 i;
  void *rec;

  slot = BattleObjGet(no);
  if (slot != NULL) {
    obj = slot->object;
    if (obj != NULL) {
      slot->animationEntry = 0;
      slot->effectEntry = 0;
      i = 0;
      while ((rec = GetMotionRecord(obj, i)) != NULL) {
        ResourceObject_ReleaseFar(rec);
        i += 1;
      }
      obj->recordStorageKind = RECORDS_NONE;
      obj->records = NULL;
    }
  }
}

// A battle sprite id's low twelve bits; the two-part sprites' second part
// is their id + 0x2001.
#define SPRITE_ID_MASK 0xfff
#define SPRITE_SECOND_PART 0x2001

// The sprite unit 32 pixels wide use for an effect.
#define NARROW_EFFECT_SPRITE 0x1ff

//--------------------------------------------------------------------------
// Readies the sprites of the units in list: releases those of every place
// list lacks, then creates each listed unit's sprites (body, weapon, status
// icon and effect; or the two parts of a two-part enemy) unless it has
// them, building its status icons and setting its pose. With refresh, sets
// every listed unit's pose again afterwards.
//--------------------------------------------------------------------------
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh) {
  s32 i;
  s32 id;
  struct BattleObj *slot;
  struct MotionObject *object;
  struct AnimationObject *resistance;
  struct SpriteEntry *entry;
  struct AnimationObject **objects;
  s32 resource;
  s32 objectId;

  // Release the sprites of the places no listed unit holds.
  for (i = 0; i < UNIT_MAX; i++) {
    if (!BattlePlacement_ContainsId(list, i)) {
      objectId = i + FIRST_ENEMY_UNIT - 8;
      if (i <= 7)
        objectId = i;
      ReleaseBattleObjectRecords(objectId);
    }
  }
  if (gSchedulerStatus == 0)
    WaitFrames(1);

  for (i = 0; i < UNIT_MAX && (id = list[i]) != BATTLE_UNIT_LIST_END; i++) {
    if (id == BATTLE_UNIT_REMOVED)
      continue;
    slot = BattleObjGet(id);
    if (slot == 0)
      continue;
    BattleUnit_BuildStatusFlags(id, slot);
    object = slot->object;
    if (object == 0)
      continue;
    if (object->recordStorageKind != RECORDS_NONE)
      continue;
    if ((slot->resource & SPRITE_ID_MASK) == SPRITE_WITH_SECOND_PART_A ||
        (slot->resource & SPRITE_ID_MASK) == SPRITE_WITH_SECOND_PART_B) {
      // A two-part enemy: a table of its two sprites, kept in the menu
      // control work.
      objects = (struct AnimationObject **)gMenuCtrlWork->unk08 +
                gMenuCtrlWork->count;
      resource = slot->resource;
      object->recordStorageKind = RECORDS_TABLE;
      object->records = objects;
      Iwram_ClearWords(objects, 16);
      resistance = ResourceObject_CreateFar(resource);
      if (resistance != 0) {
        resistance->scale = Iwram_MulQ16(resistance->scale, slot->scale);
        object->height = Resource_GetMetadataRecordFar(resource)->boxY >> 1;
        *objects = resistance;
        objects++;
      }
      resistance->flags = 0;
      resistance = ResourceObject_CreateFar(resource + SPRITE_SECOND_PART);
      if (resistance != 0) {
        resistance->scale = Iwram_MulQ16(resistance->scale, slot->scale);
        *objects = resistance;
      }
      resistance->flags = 0;
    } else {
      // The body, then the weapon, the status icon and the effect on it.
      resistance = ResourceObject_CreateFar(slot->resource);
      if (resistance != 0) {
        object->recordStorageKind = RECORDS_SINGLE;
        object->records = resistance;
        resistance->scale = Iwram_MulQ16(resistance->scale, slot->scale);
        entry = (struct SpriteEntry *)resistance->entries[0];
        entry->priority = 1;
        entry->palette = slot->palette;
        if ((resource = slot->overlay) != 0) {
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(resistance,
                                                                     resource);
          entry->priority = 1;
        }
        if ((resource = slot->animation) != 0) {
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(resistance,
                                                                     resource);
          slot->animationEntry = entry;
          Animation_SetWorkEntryFar((struct AnimationEntry *)entry, 0);
          entry->priority = 3;
        }
        if ((resource = slot->effect) != 0) {
          if (resistance->width == 32)
            resource = NARROW_EFFECT_SPRITE;
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(resistance,
                                                                     resource);
          slot->effectEntry = entry;
          entry->priority = 0;
          resistance->flags = 0;
        }
      }
    }
    BattlePres_SetActorModeAndAction(id);
  }

  if (refresh) {
    i = 0;
    if ((resource = list[0]) != BATTLE_UNIT_LIST_END) {
      for (;;) {
        if (list[i] != BATTLE_UNIT_REMOVED &&
            (slot = BattleObjGet(resource)) != 0 &&
            (object = slot->object) != 0)
          BattlePres_SetActorModeAndAction(resource);
        i++;
        if (i >= UNIT_MAX || (resource = list[i]) == BATTLE_UNIT_LIST_END)
          break;
      }
    }
  }
}

//--------------------------------------------------------------------------
// Returns unit objectId's battle object, through its place, or null for a
// unit with no place.
//--------------------------------------------------------------------------
struct BattleObj *BattleObjGet(s32 objectId) {
  struct BattleWork *work = gBattleWork;
  struct BattleObj *slots = work->slots;
  s32 place;
  if (objectId > 7) {
    objectId -= FIRST_ENEMY_UNIT - 8;
  }
  place = objectId;
  if (work->placement[place] == 0xFF) {
    return NULL;
  }
  return &slots[work->placement[place]];
}

//--------------------------------------------------------------------------
// Detaches the scripts from the four sprite entries of one sprite.
//--------------------------------------------------------------------------
void ResetMotionRecordGroup(void *owner) {
  struct AnimationObject *object = owner;
  s32 i;

  if (object != NULL) {
    for (i = 0; i < 4; i++) {
      if (object->entries[i] != NULL)
        object->entries[i]->script = NULL;
    }
  }
}

//--------------------------------------------------------------------------
// Detaches the scripts from every sprite of a battle object.
//--------------------------------------------------------------------------
void ResetBattleObjectRecordGroups(struct MotionObject *object) {
  s32 storageKind;
  s32 remaining;
  void **recordGroups;
  void *recordGroup;

  if (object != NULL) {
    storageKind = object->recordStorageKind & RECORDS_KIND_MASK;
    switch (storageKind) {
    case RECORDS_SINGLE:
      ResetMotionRecordGroup(object->records);
      return;
    case RECORDS_TABLE:
      recordGroups = object->records;
      remaining = RECORDS_TABLE_SIZE - 1;
      do {
        recordGroup = *recordGroups++;
        if (recordGroup != NULL) {
          ResetMotionRecordGroup(recordGroup);
        }
        remaining -= 1;
      } while (remaining >= 0);
      break;
    }
  }
}

//--------------------------------------------------------------------------
// Frees the enemy sprite slots unit objectId holds and marks its object
// for release at the end of the round (BattleMotion_DestroyAllSlotObjects).
//--------------------------------------------------------------------------
s32 ActivateBattleObjectSlot(s32 objectId) {
  struct BattleObj *slot;
  BattleMotion_ReleaseObjectSlotByValue(objectId);
  slot = BattleObjGet(objectId);
  slot->active = 1;
  return 0;
}

//--------------------------------------------------------------------------
// Releases the object of every unit marked by ActivateBattleObjectSlot.
// Declared to return a pointer, but it returns nothing.
//--------------------------------------------------------------------------
void *BattleMotion_DestroyAllSlotObjects(void) {
  s32 no;
  s32 i;
  struct BattleObj *slot;

  i = 0;
  do {
    no = i + FIRST_ENEMY_UNIT - 8;
    if (i <= 7) {
      no = i;
    }
    slot = BattleObjGet(no);
    if ((slot != NULL) && (slot->active != 0)) {
      ObjectDispatch_ReleaseFar((s32)slot->object);
      slot->object = NULL;
      slot->active = 0;
    }
    i += 1;
  } while (i < UNIT_MAX);
}

//--------------------------------------------------------------------------
// Writes the battlefield spot of place no (its anchor, on the ground) to
// out.
//--------------------------------------------------------------------------
s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out) {
  struct BattleWork *state = gBattleWork;

  out->x = state->slots[no].anchorX;
  out->y = 0;
  out->z = state->slots[no].anchorZ;
  return 0;
}

//--------------------------------------------------------------------------
// Sets up the camera's view for projecting points; under flag 0x16b,
// through the extra transform gCamera_FlagTransformWork first.
//--------------------------------------------------------------------------
s32 Camera_ApplyTransformByFlag(void) {
  struct BattleCamera *camera = gCameraWork;
  Render_ResetTransformState();
  if (GameFlag_TestFar(0x16B) != 0) {
    Iwram_TransformMatrix((s32 *)gCamera_FlagTransformWork);
    Transform_ApplyLookAt(camera->eye, camera->pos);
  } else {
    Transform_SetLookAt(camera->eye, camera->pos);
  }
}

//--------------------------------------------------------------------------
// Projects unit id's position onto the screen, into projected.
//--------------------------------------------------------------------------
s32 BattleMotion_ProjectPosition(s32 id, s32 *projected) {
  struct MotionObject *object = BattleObjGet(id)->object;
  struct AnimationObject *record = GetMotionRecord(object, 0);
  s32 position[3];
  s32 scaled;

  Camera_ApplyTransformByFlag();
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  scaled = Render_ProjectPoint(position, projected);
  (void)Iwram_MulQ16(scaled, record->scale);
  return 0;
}

//--------------------------------------------------------------------------
// Returns sprite recordIndex of an object (0 only, for a single sprite), or
// null when it has no such sprite.
//--------------------------------------------------------------------------
void *GetMotionRecord(struct MotionObject *object, s32 recordIndex) {
  s32 storageKind = object->recordStorageKind & RECORDS_KIND_MASK;
  if (storageKind == RECORDS_SINGLE) {
    if (recordIndex == 0) {
      return object->records;
    }
  } else if (storageKind == RECORDS_TABLE) {
    return ((void **)object->records)[recordIndex];
  }
  return NULL;
}

//--------------------------------------------------------------------------
// Puts the battle camera at its default view of the battlefield's centre.
//--------------------------------------------------------------------------
void Camera_InitDefaultTransform(void) {
  struct BattleCamera *camera = gCameraWork;
  struct FixedPointPosition eyeOffset;

  camera->yaw = 192 << 6;
  camera->pitch = 254 << 8;
  camera->distance = 255 << 17;
  camera->pos[0] = 0;
  camera->pos[1] = 0;
  camera->pos[2] = 0;
  camera->followPos = 0;
  camera->unk18 = 0;

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&camera->pos[0]);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector((s32 *)&eyeOffset, (s32 *)camera);
}

// MotionObject.targetX and targetZ while the object has no move target.
#define NO_MOVE_TARGET 0x80000000

//--------------------------------------------------------------------------
// Sends unit slotId back to its spot, landing there, and turns it to face
// the centre.
//--------------------------------------------------------------------------
void ObjMotRst(s32 slotId) {
  s32 z;
  s32 zero;
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObjGet(slotId);
  object = slot->object;
  zero = 0;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->velocityY = zero;
  object->verticalMotionPhase = zero;
  object->autoFaceMotion = zero;
  object->snapToTarget = 1;
  ObjectDispatch_InitFromTable6Far(object);
  Object_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
  z = slot->anchorZ;
  if (z < 0)
    z += 7;
  object->angle = ArcTan2(z >> 3, slot->anchorX) - 0x8000;
}

//--------------------------------------------------------------------------
// Sends unit objectId running off its side of the battlefield, three times
// as far out as its spot, with a hop.
//--------------------------------------------------------------------------
void BattleMotion_SetupEscapeObject(s32 objectId) {
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObjGet(objectId);
  object = slot->object;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->velocityY = 0x50000;
  object->verticalMotionStrength = 0x7851;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_InitFromTable6Far(object);
  Object_SetMoveTargetFar(object, slot->anchorX * 3, 0, slot->anchorZ);
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

//--------------------------------------------------------------------------
// Sends the unit of startSlot leaping progress percent of the way toward
// the unit of endSlot.
//--------------------------------------------------------------------------
void BattleMotion_InterpolatePosition(struct BattleObj *startSlot,
                                      struct BattleObj *endSlot, s32 progress) {
  s32 zStep;
  s32 startX;
  s32 x;
  s32 startZ;
  s32 endZ;
  struct MotionObject *start;
  struct MotionObject *end;

  start = startSlot->object;
  end = endSlot->object;
  startX = start->x;
  x = startX + progress * (end->x - startX) / 100;
  endZ = end->z;
  startZ = start->z;
  zStep = progress * (endZ - startZ) / 100;
  REG_BLDCNT = 0;
  start->acceleration = 0x20000;
  start->speedLimit = 0x80000;
  start->velocityY = 0x40000;
  start->verticalMotionStrength = 0xab85;
  start->verticalMotionPhase = 0;
  start->autoFaceMotion = 1;
  Object_SetMoveTargetFar(start, x, 0, startZ + zStep);
  ObjectDispatch_ApplyArgumentToChildrenFar(start, 2);
}

//--------------------------------------------------------------------------
// Sends a unit's object back to its spot.
//--------------------------------------------------------------------------
void BattleMotion_SetObjectPosition(struct BattleObj *slot) {
  struct MotionObject *object = slot->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Object_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
}

//--------------------------------------------------------------------------
// Sends unit objectId hopping out to one and a half times its spot's
// distance from the centre.
//--------------------------------------------------------------------------
void BattleMotion_ResetObjectAtScaledAnchor(s32 objectId) {
  u32 scaledX;
  struct BattleObj *slot;
  struct MotionObject *object;

  slot = BattleObjGet(objectId);
  object = slot->object;
  object->acceleration = 0x10000;
  object->speedLimit = 0x40000;
  object->velocityY = 0x30000;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_InitFromTable6Far(object);
  scaledX = slot->anchorX * 3;
  Object_SetMoveTargetFar(object, (s32)(scaledX + (scaledX >> 0x1F)) >> 1, 0,
                          slot->anchorZ);
}

//--------------------------------------------------------------------------
// Sends unit id leaping back to 1.3 times its spot's distance from the
// centre, in pose 5.
//--------------------------------------------------------------------------
void BattleMotion_InitializeObject(s32 id) {
  struct BattleObj *slot = BattleObjGet(id);
  struct MotionObject *object = slot->object;

  object->acceleration = 128 << 9;
  object->speedLimit = 128 << 11;
  object->velocityY = 128 << 11;
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  ObjectDispatch_InitFromTable6Far(object);
  Object_SetMoveTargetFar(object, Iwram_MulQ16(slot->anchorX, 0x14ccc), 0,
                          slot->anchorZ);
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

#define CLASS_NO_KNOCKBACK 0x94 // a class that never moves when hit

//--------------------------------------------------------------------------
// Knocks unit id back, by the speed, hop and distance of motion variant
// (gBattleMotion_Variant*), and puts it in pose 5. One class only takes
// the pose.
//--------------------------------------------------------------------------
void BattleMotion_ApplyVariantMotion(s32 id, s32 variant) {
  struct BattleObj *slot;
  struct MotionObject *object;
  s32 x;
  s32 anchorX;

  slot = BattleObjGet(id);
  object = slot->object;
  if (UnitGetFar(id)->classId != CLASS_NO_KNOCKBACK) {
    object->acceleration = gBattleMotion_VariantAcceleration[variant];
    object->speedLimit = gBattleMotion_VariantSpeedLimit[variant];
    if (object->y == 0 || variant > 4)
      object->velocityY = gBattleMotion_VariantVelocityY[variant];
    object->verticalMotionStrength = 0x9999;
    object->verticalMotionPhase = 0;
    object->autoFaceMotion = 0;
    ObjectDispatch_InitFromTable6Far(object);
    anchorX = slot->anchorX;
    x = (anchorX * gBattleMotion_VariantDistancePercent[variant]) / 100;
    Object_SetMoveTargetFar(object, x, 0, slot->anchorZ);
  }
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

//--------------------------------------------------------------------------
// Sends unit actorId three quarters of the way to unit targetId, arriving
// in travelDivisor steps, leaping up at initialVelocityY.
//--------------------------------------------------------------------------
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY) {
  struct BattleObj *actorSlot = BattleObjGet(actorId);
  struct BattleObj *targetSlot = BattleObjGet(targetId);
  struct MotionObject *object = actorSlot->object;
  struct MotionObject *target = targetSlot->object;
  s32 percent = 75;
  s32 dx = target->x - object->x;
  s32 startX = object->x;
  s32 stepX = percent * dx / 100;
  s32 dz = target->z - object->z;
  s32 startZ = object->z;
  s32 stepZ = percent * dz / 100;
  s32 x = startX + stepX;
  s32 z = startZ + stepZ;
  s32 cellX = stepX >> 8;
  s32 cellZ = stepZ >> 8;
  s32 speed;

  speed = Iwram_Sqrt(cellX * cellX + cellZ * cellZ);
  speed = (speed << 8) / travelDivisor;
  object->acceleration = speed;
  object->speedLimit = speed;
  object->snapToTarget = 1;

  if (object->motionFlags & 4)
    object->velocityY = initialVelocityY;
  object->velocityY = initialVelocityY;
  object->verticalMotionStrength = 0xab85;
  object->autoFaceMotion = 1;
  ObjectDispatch_InitFromTable6Far(object);
  Object_SetMoveTargetFar(object, x, 0, z);
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

//--------------------------------------------------------------------------
// Restarts unit id's motion in pose 2.
//--------------------------------------------------------------------------
void BattleMotion_ResetSlotObjectMode2(s32 id) {
  struct MotionObject *object;

  object = BattleObjGet(id)->object;
  ObjectDispatch_InitFromTable6Far(object);
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

//--------------------------------------------------------------------------
// Does nothing; BattleMotion_ApplyPairMidpoint passes it a position and a
// distance.
//--------------------------------------------------------------------------
void BattleMotion_ReservedNoOp83B0() {}

//--------------------------------------------------------------------------
// Works out the point halfway between two units, at their move targets
// where they have one, and hands it to BattleMotion_ReservedNoOp83B0,
// which does nothing.
//--------------------------------------------------------------------------
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId) {
  struct BattleObj *secondSlot;
  struct MotionObject *first;
  struct MotionObject *second;
  s32 x1;
  s32 x2;
  s32 z1;
  s32 z2;
  struct FixedPointPosition pos;

  first = BattleObjGet(firstId)->object;
  secondSlot = BattleObjGet(secondId);
  second = secondSlot->object;

  x1 = first->targetX;
  if (x1 == NO_MOVE_TARGET) {
    x1 = first->x;
  }
  z1 = first->targetZ;
  if (z1 == NO_MOVE_TARGET) {
    z1 = first->z;
  }
  x2 = second->targetX;
  if (x2 == NO_MOVE_TARGET) {
    x2 = second->x;
  }
  z2 = second->targetZ;
  if (z2 == NO_MOVE_TARGET) {
    z2 = second->z;
  }

  pos.x = (x1 + x2) / 2;
  pos.y = 0;
  pos.z = (z1 + z2) / 2;

  BattleMotion_ReservedNoOp83B0(&pos, 0x1000);
}

#define HIT_FLASH_ANIM 0x11b

//--------------------------------------------------------------------------
// Plays the hit flash on unit id's sprite and waits ten frames.
//--------------------------------------------------------------------------
void BattleMotion_SpawnSlotEffectAndWait(s32 id) {
  struct MotionObject *object;
  struct AnimationEntry *effect;
  s32 entry;

  object = BattleObjGet(id)->object;
  if ((object != NULL) &&
      ((RECORDS_KIND_MASK & object->recordStorageKind) == RECORDS_SINGLE)) {
    entry = ResourceMetadata_RegisterFar(object->records, HIT_FLASH_ANIM);
    if (entry != 0) {
      effect = (struct AnimationEntry *)entry;
      Animation_SetWorkEntryFar(effect, 1);
      effect->priority = 3;
    }
    WaitFrames(0xA);
  }
}

//--------------------------------------------------------------------------
// Projects the point above unit id where its effects play (its class's
// height, Battle_GetObjectTableValue) onto the screen, into projected.
//--------------------------------------------------------------------------
s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected) {
  struct MotionObject *object = BattleObjGet(id)->object;
  struct AnimationObject *record = GetMotionRecord(object, 0);
  s32 scaled;
  s32 factor;
  /* FAKEMATCH: never used: the input position BattleMotion_ProjectPosition
   * fills, unused here; it keeps the native's 12-byte frame. Without it 4 of 42
   * differ (no sub sp, #12 and add sp, #12). */
  struct EffectPosition input;

  Camera_ApplyTransformByFlag();
  scaled = Render_ProjectPoint(&object->x, projected);
  factor = Iwram_MulQ16(scaled, record->scale);
  scaled = Iwram_MulQ16(factor, (s32)Battle_GetObjectTableValue(id) >> 16);
  projected[1] -= scaled;
  return 0;
}

//--------------------------------------------------------------------------
// Projects the point above unit id onto the screen, into projected: 24
// pixels up for a small enemy, 48 for others, as drawn.
//--------------------------------------------------------------------------
s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected) {
  struct MotionObject *object = BattleObjGet(id)->object;
  struct AnimationObject *record = GetMotionRecord(object, 0);
  s32 scaled;
  s32 factor;

  Camera_ApplyTransformByFlag();
  scaled = Render_ProjectPoint(&object->x, projected);
  factor = Iwram_MulQ16(scaled, record->scale);
  if (Summon_IsEntryFlagged(UnitGetFar(id)->classId) != 0)
    scaled = Iwram_MulQ16(factor, 24);
  else
    scaled = Iwram_MulQ16(factor, 48);
  projected[1] -= scaled;
  return 0;
}

//--------------------------------------------------------------------------
// Returns the height above unit id where its effects play, 16.16: its
// class's own (Summon_GetEntryByte4), else 24 for a small enemy and 48 for
// others.
//--------------------------------------------------------------------------
s32 Battle_GetObjectTableValue(s32 id) {
  u32 height;
  u8 classId;

  classId = UnitGetFar(id)->classId;
  height = (u8)Summon_GetEntryByte4(classId) << 16;
  if (height == 0) {
    classId = UnitGetFar(id)->classId;
    if (Summon_IsEntryFlagged(classId) != 0) {
      height = 0x180000;
    } else {
      height = 0x300000;
    }
  }
  return height;
}

// BattleActionRecord.target of an action that needs none: a held unit's
// lost turn (the enemies' and the party's), and an enemy's attack in auto
// battle, which picks a party place.
#define TARGET_NONE_ENEMY 0x100
#define TARGET_NONE_PARTY 0x180

//--------------------------------------------------------------------------
// Auto battle: gives every living unit a plain attack and sorts them into
// turn order. The party act at their Agility and target the first enemy;
// enemies act at half their Agility plus a random part of it and target a
// random party member. Returns how many actions there are.
//--------------------------------------------------------------------------
s32 BattlePresentation_BuildSortedUnitEntries(
    struct BattleActionRecord *entries) {
  struct BattleActionRecord swap;
  u16 unitIds[14];
  s32 partyCount;
  s32 count = 0;
  s32 enemyCount;
  s32 targetRange;
  s32 index;
  struct BattleActionRecord *entry;
  struct BattleUnit *unit;

  partyCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unitIds);
  for (index = 0; index != 4; index++)
    UnitGetFar(index);
  for (index = 0; index < partyCount; index++) {
    s32 unitId = unitIds[index];

    unit = UnitGetFar(unitId);
    entry = &entries[index];
    entry->unitId = unitId;
    entry->priority = unit->agi;
    entry->command = CMD_ATTACK;
    entry->parameter = 0;
    entry->target = FIRST_ENEMY_UNIT;
    count++;
  }
  enemyCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unitIds);
  entry = &entries[partyCount];
  targetRange = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, NULL);
  for (index = 0; index < enemyCount; index++) {
    s32 unitId = unitIds[index];

    unit = UnitGetFar(unitId);
    entry->unitId = unitId;
    entry->priority = unit->agi >> 1;
    if (entry->priority != 0)
      entry->priority += (Random16() * unit->agi) >> 16;
    entry->command = CMD_ATTACK;
    entry->parameter = 0;
    entry->target = (Random16() * targetRange) >> 16;
    count++;
    entry++;
  }

  // Bubble the faster units forward until a pass swaps nothing.
  for (index = count - 2; index > 0; index--) {
    s32 swaps = 0;
    s32 pos;

    for (pos = count - 1; pos > 0; pos--) {
      if ((s16)entries[pos].priority > (s16)entries[pos - 1].priority) {
        Dma_Set(&entries[pos], &swap, 0x84000004, REG_DMA3);
        Dma_Set(&entries[pos - 1], &entries[pos], 0x84000004, REG_DMA3);
        Dma_Set(&swap, &entries[pos - 1], 0x84000004, REG_DMA3);
        swaps++;
      }
    }
    if (swaps == 0)
      break;
  }
  return count;
}

// Turns the camera while L or R is held.
DEFINE_ADJUST_CAMERA(BattlePres_AdjustCameraByShoulderKeys)

// The camera's yaw while a party member acts, and while an enemy does.
#define YAW_PARTY_TURN 0x2000
#define YAW_ENEMY_TURN -0x2000

//--------------------------------------------------------------------------
// Auto battle: plays one action. A unit already down does nothing (-1);
// a defeated target is replaced by a living unit of its side first.
// Returns 1 when the party got away, 0 otherwise.
//--------------------------------------------------------------------------
s32 BattlePres_RunAction(struct BattleActionRecord *action) {
  struct BattlePresentationTransition *transition;
  s32 actorId;
  s32 yaw;
  struct BattleUnit *actor;

  actorId = action->unitId;
  actor = UnitGetFar(actorId);
  if (actor->hp == 0)
    return -1;

  action->target = BattleTarget_ReplaceDefeated(action);
  transition = gTransitionWork;
  if (action->unitId > 4)
    yaw = YAW_ENEMY_TURN;
  else
    yaw = YAW_PARTY_TURN;
  transition->targetYaw = yaw;
  transition->frames = 60;
  UiWork_ClearValueNameTablesFar();

  switch (action->command) {
  case CMD_FLEE:
    UiText_ShowMessageAndWaitCoreFar(MsgPartyFlees);
    if (BattleEscape_PlayRun(action) != 0)
      return 1;
    break;
  case CMD_DEFEND:
    WaitFrames(45);
    BattlePres_ShowDefendMessage(action);
    break;
  case CMD_ITEM:
    WaitFrames(45);
    BattlePres_RunUnitAction(action);
    break;
  case CMD_ATTACK:
  default: {
    struct BattlePresentationTransition *tr = gTransitionWork;
    tr->flag = 0;
    BattlePres_RunUnitAction(action);
    tr->flag = 0;
    break;
  }
  case CMD_PSYNERGY:
    BattlePresentation_RunPairedUnitTransition(action);
    break;
  }

  UiWork_ResetFreeChannelFar();
  return 0;
}

//--------------------------------------------------------------------------
// Returns 0 for a party member's or an enemy's unit id, -1 for any other.
//--------------------------------------------------------------------------
s32 BattleObject_IsValidId(u32 objectId) {
  if (objectId <= 7) {
    return 0;
  }
  if (objectId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE) {
    return 0;
  }
  return -1;
}

//--------------------------------------------------------------------------
// Auto battle: the party tries to run, and gets away 15 times in 16, each
// living member running off in turn. Returns 1 when they got away; else
// shows "But there's no escape!"... and returns 0.
//--------------------------------------------------------------------------
s32 BattleEscape_PlayRun(struct BattleActionRecord *action) {
  s16 partyMembers[14];
  s32 partySize;
  s32 animated;
  s32 memberSlot;

  (void)action;
  if (((Random16() << 4) >> 16) != 0) {
    partySize = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, partyMembers);
    animated = 0;
    if (partySize != 0) {
      memberSlot = 0;
      do {
        BattleMotion_SetupEscapeObject(partyMembers[memberSlot]);
        animated++;
        WaitFrames(8);
        memberSlot++;
      } while (animated != partySize);
    }
    WaitFrames(22);
    return 1;
  }
  UiText_ShowMessageAndWaitCoreFar(MsgCannotEscape);
  return 0;
}

//--------------------------------------------------------------------------
// Auto battle: shows "<unit> is defending" for a standing unit. Returns -1
// for an invalid unit id, 0 otherwise.
//--------------------------------------------------------------------------
s32 BattlePres_ShowDefendMessage(const struct BattleActionRecord *action) {
  s32 objectId;
  s32 result;
  struct BattleUnit *unit;

  objectId = action->unitId;
  unit = UnitGetFar(objectId);
  if (BattleObject_IsValidId(objectId) < 0) {
    return -1;
  }
  result = 0;
  if (unit->hp <= 0) {
    return result;
  }
  UiWork_ClearValueNameTablesFar();
  UiWork_PushValueSlotFar(objectId, VALUE_UNIT_NAME);
  UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
  return 0;
}

// REG_BLDCNT: blend the targets' sprites over every layer, fading.
#define BLEND_SPRITES_OVER_ALL 0x3f40
#define BLEND_ALPHA(top, bottom) ((top) | ((bottom) << 8))

//--------------------------------------------------------------------------
// Auto battle: plays a Psynergy. The camera turns to the caster's side and
// the caster raises its hand; the target side fades out, its living units
// are readied alone, the Psynergy's effect (kind action->parameter) plays
// on them, and everyone fades back in. Returns -1 for an invalid actor or
// target.
//--------------------------------------------------------------------------
s32 BattlePresentation_RunPairedUnitTransition(
    struct BattleActionRecord *action) {
  u16 visibleUnits[14];
  struct BattlePresentationWork context;
  s32 actorId;
  s32 targetId;
  u32 sideStart;
  u32 sideCount;
  u32 livingCount;
  struct MotionObject *actorRecord;
  u32 index;

  livingCount = 0;
  actorId = action->unitId;
  if (BattleObject_IsValidId(actorId) < 0) {
    return -1;
  }
  targetId = action->target;
  if (BattleObject_IsValidId(targetId) < 0) {
    return -1;
  }

  {
    struct BattlePresentationTransition *transition = gTransitionWork;
    transition->targetYaw = action->unitId > 4 ? 0x5000 : YAW_PARTY_TURN;
    transition->frames = 60;
  }
  WaitFrames(10);
  Random16();
  actorRecord = BattleObjGet(actorId)->object;

  // The caster's side: its pose when the caster is among them.
  if (targetId >= 0 && targetId <= 7) {
    sideCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, visibleUnits);
    sideStart = FIRST_ENEMY_UNIT;
  } else {
    sideCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, visibleUnits);
    sideStart = 0;
  }

  for (index = 0; index != sideCount; index++) {
    if (index + sideStart == actorId) {
      ObjectDispatch_ApplyArgumentToChildrenFar(actorRecord, 3);
    }
  }

  // Fade the caster's side out.
  WaitFrames(30);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++) {
    BattlePres_SetActorRecordMode(index + sideStart, 1);
  }
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = BLEND_ALPHA(16 - index, 16);
    WaitFrames(1);
  }
  PartyStatusDrawFar(9);

  // Ready only the target side's living units, and play the effect.
  if (targetId > FIRST_ENEMY_UNIT - 1) {
    u32 count = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      u32 unit = index + FIRST_ENEMY_UNIT;
      if (UnitGetFar(unit)->hp > 0) {
        visibleUnits[livingCount++] = unit;
      }
    }
  } else {
    u32 count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      if (UnitGetFar(index)->hp > 0) {
        visibleUnits[livingCount++] = index;
      }
    }
  }
  visibleUnits[livingCount] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForList(visibleUnits, 0);

  context.kind = action->parameter;
  context.actor = actorId;
  for (index = 0; index != livingCount; index++) {
    context.actors[index] = visibleUnits[index];
  }
  context.count = livingCount;
  if (targetId >= 0 && targetId <= 7) {
    context.side = 1;
  } else {
    context.side = 0;
  }
  BattleFx_InitializeModeFar((s32 *)&context);

  // Bring every unit back and fade the caster's side in.
  WaitFrames(10);
  BattleObjPut();
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++) {
    BattlePres_SetActorRecordMode(index + sideStart, 1);
  }
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = BLEND_ALPHA(index, 16);
    WaitFrames(1);
  }
  for (index = 0; index != sideCount; index++) {
    BattlePres_SetActorRecordMode(index + sideStart, 0);
  }
  BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
  WaitFrames(3);
  return 0;
}

//--------------------------------------------------------------------------
// Auto battle: an attack. Shows "<unit> attacks!", sends the attacker up
// to its target, plays the blow and sends both back. Returns -1 for an
// invalid attacker or target.
//--------------------------------------------------------------------------
s32 BattlePres_RunApproachAction(struct BattleActionRecord *input) {
  struct BattlePresentationWork work;

  if (gTransitionWork->targetYaw == YAW_PARTY_TURN) {
    gTransitionWork->targetYaw = YAW_PARTY_TURN;
    WaitFrames(10);
  } else {
    gTransitionWork->targetYaw = YAW_PARTY_TURN;
    WaitFrames(30);
  }

  work.actor = input->unitId;
  if (BattleObject_IsValidId(work.actor) < 0)
    return -1;

  work.actors[0] = input->target;
  if (BattleObject_IsValidId(work.actors[0]) < 0)
    return -1;

  UnitGetFar(work.actor);
  UnitGetFar(work.actors[0]);
  Random16();
  UiWork_PushValueSlotFar(work.actor, VALUE_UNIT_NAME);
  UiText_ShowMessageAndWaitCoreFar(MsgActorAttacks);
  BattleMotion_ApproachTarget(work.actor, work.actors[0], 13, 0);
  ObjectDispatch_ApplyValueToChildrenFar(BattleObjGet(work.actor)->object, 16);
  BattleObjGet(work.actors[0]);

  work.count = 1;
  if ((u16)work.actors[0] <= 7)
    work.side = 1;
  else
    work.side = 0;
  work.flags = 0;

  WaitFrames(4);
  BattleFx_DispatchByIdRangeFar(&work);
  ObjMotRst(work.actors[0]);
  ObjMotRst(work.actor);
  return 0;
}

//--------------------------------------------------------------------------
// Auto battle: an attack or an item. Turns the camera to the actor's side,
// poses the actor and plays the effect (kind action->parameter) on the
// first living unit of the target's side, then sends both back. Returns -1
// for an invalid actor.
//--------------------------------------------------------------------------
s32 BattlePres_RunUnitAction(struct BattleActionRecord *action) {
  struct BattlePresentationWork work;
  struct BattlePresentationTransition *transition = gTransitionWork;
  struct MotionObject *object;
  s32 facing;
  s32 i;

  if (action->unitId > 4)
    facing = YAW_ENEMY_TURN;
  else
    facing = YAW_PARTY_TURN;
  if (transition->targetYaw == facing) {
    transition->frames = 40;
    WaitFrames(40);
  } else {
    transition->targetYaw = facing;
    transition->frames = 40;
    WaitFrames(40);
  }
  work.kind = action->parameter;
  work.initialValue = action->range;
  work.actor = action->unitId;
  work.firstTarget = action->target;
  if (BattleObject_IsValidId(work.actor) < 0)
    return -1;
  if (work.firstTarget > FIRST_ENEMY_UNIT - 1)
    work.count = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, work.actors);
  else
    work.count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, work.actors);
  PartyStatusDrawFar(gBattleWork->partyStatus & ~1);
  object = BattleObjGet(work.actor)->object;
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
  ObjectDispatch_ApplyValueToChildrenFar(object, 16);
  if ((u16)action->target <= 7) {
    work.side = 1;
    BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, work.actors);
    work.count = 1;
  } else {
    work.side = 0;
    BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, work.actors);
    work.count = 1;
  }

  // Keep each target's sprite entries' parameters for the effect.
  for (i = 0; i != work.count; i++) {
    struct AnimationObject *animation =
        BattleObjGet(work.actors[i])->object->records;
    s32 j;

    for (j = 0; j != animation->count - 1; j++)
      work.params[i][j] = animation->entries[j]->param;
  }
  /* FAKEMATCH: needed to match: the one-pass stages force fresh work pointers
   * in the reset loop and place the dispatch argument before the kind store. As
   * plain statements the allocation changes from the side choice on: differ
   * 103, also with the variant stored before the kind. */
  do {
    work.kind = 0;
    work.variant = 0;
    BattleFx_RunIceFleckBurstFar(&work);
    work.kind = 1;
    BattleFx_RunIceFleckBurstFar(&work);
    work.kind = 2;
    BattleFx_RunIceFleckBurstFar(&work);
    work.kind = 3;
    BattleFx_RunIceFleckBurstFar(&work);
    work.kind = 0;
    BattleFx_DispatchModeFar(&work);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  } while (0);
  for (i = 0; i != work.count; i++)
    ObjMotRst(work.actors[i]);
  ObjMotRst(work.actor);
  return 0;
}

//--------------------------------------------------------------------------
// Auto battle: deals damage HP to unit unitId and shows it: "A bitter
// blow!" (to the party) or "A critical hit!" (to an enemy) first when
// showMessage is set, then "<unit> takes <damage> damage!", and "<unit>
// goes down..." or "<unit> has been downed!" when the HP runs out.
// context, if null, is replaced by four cleared bytes nothing reads.
//--------------------------------------------------------------------------
void BattlePresentation_ApplyUnitDamage(u32 unitId, s32 damage, s32 showMessage,
                                        u8 *context) {
  u8 localContext[4];
  struct BattleUnit *character;
  struct BattleObj *slot;

  if (context == 0) {
    context = localContext;
    context[0] = 0;
    context[1] = 0;
    context[2] = 0;
    context[3] = 0;
  }

  character = UnitGetFar(unitId);
  character->hp -= damage;
  if (character->hp < 0)
    character->hp = 0;

  slot = BattleObjGet(unitId);
  ObjectDispatch_ApplyArgumentToChildrenFar(slot->object, 5);
  PartyStatusDrawFar(0);
  UiWork_ClearValueNameTablesFar();

  if (unitId <= 7) {
    if (showMessage != 0)
      UiText_ShowMessageAndWaitCoreFar(MsgBitterBlow);
    UiWork_PushValueSlotFar(damage, VALUE_NUMBER);
    UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgDmgP);
  } else {
    if (showMessage != 0)
      UiText_ShowMessageAndWaitCoreFar(MsgCritical);
    UiWork_PushValueSlotFar(damage, VALUE_NUMBER);
    UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgDmgE);
    UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
  }

  BattleMotion_SetMode5AndActivateSlot(unitId);
  if (unitId <= 7) {
    if (character->hp <= 0) {
      UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgGoesDown);
    }
  } else if (character->hp <= 0) {
    UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgDowned);
  }

  slot = BattleObjGet(unitId);
  ObjectDispatch_ApplyArgumentToChildrenFar(slot->object, 1);
}

//--------------------------------------------------------------------------
// For a unit whose HP has run out: plays its falling animation to the last
// frame and marks its object for release (ActivateBattleObjectSlot).
//--------------------------------------------------------------------------
void BattleMotion_SetMode5AndActivateSlot(s32 unitId) {
  struct AnimationEntry *actorData;
  struct AnimationObject *object;
  if (UnitGetFar(unitId)->hp <= 0) {
    object = BattleObjGet(unitId)->object->records;
    AnimationObjects_SelectAnimationFar(object, 5);
    actorData = object->entries[0];
    actorData->param = 6;
    actorData->frame = 0xFF;
    WaitFrames(4);
    Map_RenderAnimatedTileFramesForObjectFar(object);
    ActivateBattleObjectSlot(unitId);
  }
}

// BattleTarget_ReplaceDefeated's result when the target's side is all down.
#define TARGET_SIDE_DEFEATED 0x100

//--------------------------------------------------------------------------
// Returns the action's target, or a random living unit of its side when
// the target is down; TARGET_SIDE_DEFEATED when none is left.
//--------------------------------------------------------------------------
s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action) {
  u16 livingUnits[14];
  s32 targetId;
  s32 livingCount;

  targetId = action->target;
  if (UnitGetFar(targetId)->hp != 0) {
    return targetId;
  }

  if (targetId > FIRST_ENEMY_UNIT - 1) {
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
  } else {
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
  }

  if (livingCount == 0) {
    return TARGET_SIDE_DEFEATED;
  }

  return (s16)livingUnits[(Random16() * livingCount) >> 0x10];
}

// The sprite palettes 14 and 15 (BG palette RAM 0x05000000).
#define OBJ_PALETTE_14 ((u16 *)0x050001C0)

//--------------------------------------------------------------------------
// Pulses sprite palette 14's colours: each takes the matching colour of
// palette 15 brightened by up to six steps, rising and falling with the
// frame count.
//--------------------------------------------------------------------------
void Palette_UpdatePulseBrightness(void) {
  u16 *palette;
  s32 i;
  s32 mask;

  palette = OBJ_PALETTE_14;
  mask = 31;
  i = 15;
  do {
    u32 color;
    s32 amount;
    s32 tick;
    s32 angle;
    s32 red;
    s32 green;
    s32 blue;

    tick = gFrameCount;
    angle = (tick * 3) << 10;
    color = palette[16];
    amount = (0x10000 - Trig_Cos(angle)) / 0x2AAA;

    red = (color >> 10) & mask;
    green = (color >> 5) & mask;
    blue = color & mask;
    red += amount;
    green += amount;
    blue += amount;

    if (red < 0 || red > 31)
      red = 31;
    if (green < 0 || green > 31)
      green = 31;
    if (blue < 0 || blue > 31)
      blue = 31;

    *palette++ = (red << 10) | (green << 5) | blue;
    i--;
  } while (i >= 0);
}

//--------------------------------------------------------------------------
// Sets the camera up for a scene drawn over the battle (the summons'):
// a fixed view raised by pos pixels, with its projection and the
// background's perspective to match.
//--------------------------------------------------------------------------
void Camera_ConfigureScene(s32 pos) {
  // UB: indexing past scalar gCameraWork reaches another work slot. Named
  // slots add four bytes in the native EN trial (0xd8 to 0xdc).
#ifdef UBFIX
  struct BattleCamera *camera = gCameraWork;
  struct BattlePresentationTransition *transition =
      WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
#else
  void **slots = (void **)&gCameraWork;
  struct BattleCamera *camera = slots[0];
  struct BattlePresentationTransition *transition =
      slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
#endif
  struct FixedPointPosition eyeOffset;
  u32 result;

  camera->pos[1] = 160 << 11;
  camera->pos[0] = 0;
  camera->pos[2] = 0;
  transition->targetYaw = 128 << 7;
  camera->yaw = 128 << 7;
  camera->pitch = 244 << 8;
  camera->followPos = 0;
  camera->distance = 0x02ee0000;
  camera->unk18 = 0;

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&camera->pos[0]);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector((s32 *)&eyeOffset, (s32 *)camera);

  result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
  Projection_Set(0, result, 0x07920000);

  gProjection.centerY = pos + 120;
  transition->active = 1;
  BattleCamera_SetRange(240 << 15, (0x76 - pos) << 16, 0, 128 << 4, 128 << 10);
  transition->flag = 1;
  transition->active = 0;
}

//--------------------------------------------------------------------------
// Ends every unit's guard (Defend and the like) and recalculates its stats.
//--------------------------------------------------------------------------
void BattleUnit_ResetGuardLevels(void) {
  u16 ids[14];
  s32 count;
  s32 index;

  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
  for (index = 0; index < count; index++) {
    struct BattleUnit *actor;

    actor = UnitGetFar(ids[index]);
    actor->guardLevel = 0;
    UnitCalcFar(ids[index]);
  }
}

// gGameState.battleStart of a battle the party cannot run from.
#define BATTLE_START_NO_ESCAPE 2

//--------------------------------------------------------------------------
// Rolls whether the party gets away. The chance, in hundredths of a
// percent, is 5000 plus 2000 for each attempt that failed, plus 500 times
// the party's average level, less 500 times the enemies' average level. A
// party that struck first always gets away; no one gets away from a battle
// that forbids it. Every attempt counts toward the next. Returns 1 when
// the party gets away.
//--------------------------------------------------------------------------
s32 BattleEscape_CheckSuccess(void) {
  s32 escaped;
  u8 *failedAttempts;
  s16 livingUnits[14];
  s32 livingCount;
  s32 levelTotal;
  s32 unitIndex;
  s32 chance;
  struct BattleWork *escapeState;

  escaped = 0;
  escapeState = gBattleWork;
  if (escapeState->encounterMode == BATTLE_ENCOUNTER_PARTY_FIRST) {
    escaped = 1;
  } else {
    failedAttempts = &escapeState->escapeFailures;
    chance = 5000 + (escapeState->escapeFailures * 2000);
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
    levelTotal = 0;
    for (unitIndex = escaped; unitIndex < livingCount; unitIndex++) {
      levelTotal += UnitGetFar(livingUnits[unitIndex])->level;
    }
    chance += levelTotal * 500 / livingCount;
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
    levelTotal = 0;
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += UnitGetFar(livingUnits[unitIndex])->level;
    }
    chance -= levelTotal * 500 / livingCount;
    if ((chance > 0) && (((10000 * Random16()) >> 0x10) < chance)) {
      escaped = 1;
    }
    *failedAttempts += 1;
  }
  if (gGameState.battleStart == BATTLE_START_NO_ESCAPE) {
    escaped = 0;
  }
  return escaped;
}

//--------------------------------------------------------------------------
// The party's actions for the round, from the command menu: a member who
// is asleep, confused, charmed or stunned loses each of their turns
// (CMD_NONE); the others choose theirs (Battle_CollectPartyCommandsFar).
// Returns how many actions there are, or -1 if the menu gave up.
//--------------------------------------------------------------------------
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries) {
  u16 *choosingUnits = Runtime_BumpAllocateAlternatePool(17);
  u16 *unitIds = Runtime_BumpAllocateAlternatePool(9);
  s32 unitCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unitIds);
  s32 choosingCount = 0;
  s32 entryCount = 0;
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    struct BattleUnit *unit = UnitGetFar(unitIds[unitIndex]);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionEntryCount; copyIndex++) {
      // The word from delusion holds confusion, charm and stun above it.
      if (unit->sleep != 0 || (*(u32 *)&unit->delusion & 0xffffff00)) {
        struct BattleActionRecord *entry = &entries[entryCount];
        entry->unitId = unitIds[unitIndex];
        entry->priority = unit->agi;
        entry->command = CMD_NONE;
        entry->parameter = 0;
        entry->target = TARGET_NONE_PARTY;
        entryCount++;
      } else {
        choosingUnits[choosingCount++] = unitIds[unitIndex];
      }
    }
  }

  {
    s32 appended;

    entries += entryCount;
    appended =
        Battle_CollectPartyCommandsFar(entries, choosingUnits, choosingCount);
    if (appended < 0) {
      unitCount = -1;
    } else {
      unitCount = entryCount + appended;
    }
    MemFree(unitIds);
    MemFree(choosingUnits);
    return unitCount;
  }
}

//--------------------------------------------------------------------------
// The enemies' actions for the round, in a shuffled order, each chosen by
// BattleCommand_SelectAutomatic; a sleeping or stunned enemy loses its
// turns. An enemy acting more than once a turn acts later at half its
// Agility. None act when the party struck first; when the party is caught
// by surprise, only two to five enemies act, once each. Returns how many
// actions there are.
//--------------------------------------------------------------------------
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries) {
  u16 unitIds[14];
  s32 entryCount = 0;
  struct BattleWork *battle = gBattleWork;
  s32 unitCount;
  s32 i;

  if (battle->encounterMode == BATTLE_ENCOUNTER_PARTY_FIRST) {
    return 0;
  }

  unitCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unitIds);
  if (unitCount == 0) {
    return 0;
  }

  for (i = 31; i >= 0; i--) {
    u32 first = (unitCount * Random16()) >> 16;
    u32 second = (unitCount * Random16()) >> 16;
    s32 swap = unitIds[first];
    unitIds[first] = unitIds[second];
    unitIds[second] = swap;
  }

  if (battle->encounterMode == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
    s32 limit = ((Random16() * 5) >> 16) + 1;

    if (limit <= 1) {
      limit = 2;
    }
    if (limit < unitCount) {
      unitCount = limit;
    }
  }

  for (i = 0; i < unitCount; i++) {
    s32 unitId = unitIds[i];
    struct BattleUnit *unit = UnitGetFar(unitId);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionEntryCount; copyIndex++) {
      struct BattleActionRecord *entry = &entries[entryCount];
      s32 agility;

      entry->unitId = unitId;
      agility = unit->agi;
      entry->priority = agility;
      if (copyIndex != 0) {
        entry->priority = (s16)agility / 2;
      }

      if (unit->sleep != 0 || unit->stun != 0) {
        entry->command = CMD_NONE;
        entry->parameter = 0;
        entry->target = TARGET_NONE_ENEMY;
      } else {
        BattleCommand_SelectAutomatic(entry, 0);
      }

      entryCount++;

      if (battle->encounterMode == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        break;
      }
    }
  }

  return entryCount;
}

// A Djinni whose effect is a guard or Ground acts before everyone else.
#define DJINN_FIRST_PRIORITY 10000

//--------------------------------------------------------------------------
// Sorts the round's actions into turn order, fastest first, after moving
// the guarding and grounding Djinn to the front.
//--------------------------------------------------------------------------
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count) {
  s32 i;
  s32 j;
  s32 swapped;

  for (i = 0; i < count; i++) {
    struct BattleActionRecord *entry = &entries[i];

    if (entry->command == CMD_DJINN) {
      struct BattleAct *action;

      // parameter: the Djinni's element in bits 8 to 11, its number below.
      UnitGetFar(entry->unitId);
      action = BattleActGet(Djinn_GetAbilityIdFar(entry->parameter >> 8 & 15,
                                                  entry->parameter & 0xff));
      if (action->effect == ABILITY_EFFECT_DEFEND ||
          action->effect == ABILITY_EFFECT_TOTAL_DEFENSE ||
          action->effect == ABILITY_EFFECT_GROUND) {
        entry->priority += DJINN_FIRST_PRIORITY;
      }
    }
  }

  do {
    swapped = 0;
    for (j = count - 1; j > 0; j--) {
      if ((s16)entries[j].priority > (s16)entries[j - 1].priority) {
        struct BattleActionRecord temporary;

        Iwram_CopyWords(&temporary, &entries[j],
                        sizeof(struct BattleActionRecord));
        Iwram_CopyWords(&entries[j], &entries[j - 1],
                        sizeof(struct BattleActionRecord));
        Iwram_CopyWords(&entries[j - 1], &temporary,
                        sizeof(struct BattleActionRecord));
        swapped++;
      }
    }
  } while (swapped != 0);
}

// The header a link battle sends before its actions: how many there are,
// the next random number to check the consoles agree, and the parent's
// random seed for the round.
struct BattleLinkedActionState {
  s32 count;
  s32 check;
  s32 seed;
  u8 unused[28];
};

// The link sends in packets of 20 bytes (the header is one), and a transfer
// may take 300 frames.
#define LINK_PACKET_BYTES 20
#define LINK_TRANSFER_TIMEOUT 300
// The bytes of count actions, in whole packets.
#define LINK_ACTIONS_BYTES(count)                                              \
  ((u32)((count) * 16 + LINK_PACKET_BYTES - 1) / LINK_PACKET_BYTES *           \
   LINK_PACKET_BYTES)

//--------------------------------------------------------------------------
// Link battle: exchanges the round's actions with the other console and
// appends the opponent's after the count of this console's. Each console
// sees its own party as the party: the received actions' units take the
// ids they have here and their targets swap sides. The parent sends first
// and gives the round its random seed; the child checks both consoles
// agree on the next random number. A party member's priority is made even
// on the parent and odd on the child, so equal speeds sort alike on both.
// Returns how many actions were received, or -1 when the exchange fails.
//--------------------------------------------------------------------------
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions,
                                           s32 count) {
  struct BattleWork *battle = gBattleWork;
  s32 result = 0;
  s32 allocationSize = LINK_ACTIONS_BYTES(count);
  struct BattleLinkedActionState *state =
      (struct BattleLinkedActionState *)Runtime_BumpAllocateAlternatePool(40);
  s32 index;

  // Sends the header, then this console's actions.
  s32 BattleLink_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = SerialRuntime_BeginSend(state, LINK_PACKET_BYTES);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (SerialRuntime_GetActiveTransfers()) {
      WaitFrames(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++disconnected > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (allocationSize) {
      status = SerialRuntime_BeginSend(actions, allocationSize);
      if (status == -1)
        return status;
      while (SerialRuntime_GetActiveTransfers()) {
        WaitFrames(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
          if (++disconnected > LINK_MAX_MISSED_FRAMES)
            return -1;
        } else {
          disconnected = 0;
        }
      }
    }
    return 0;
  }

  // Receives the header, then the other console's actions after ours.
  s32 BattleLink_ReceiveActions(void) {
    /* FAKEMATCH: gSerialReceivedSize is volatile (serial_runtime.h), so every
     * read here is a load of its own. Measured on tbs-en: without the qualifier
     * differ 116, 129 instructions for 131. */
    s32 timeout;
    s32 disconnected;
    s32 status = SerialRuntime_BeginReceive(state);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (SerialRuntime_GetActiveTransfers()) {
      if (gSerialReceivedSize > LINK_PACKET_BYTES)
        return -1;
      WaitFrames(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++disconnected > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (gSerialReceivedSize != LINK_PACKET_BYTES)
      return -1;
    result = state->count;
    if (state->count) {
      status = SerialRuntime_BeginReceive(actions + count);
      if (status == -1)
        return status;
      while (SerialRuntime_GetActiveTransfers()) {
        if (gSerialReceivedSize > LINK_ACTIONS_BYTES(result))
          return -1;
        WaitFrames(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
          if (++disconnected > LINK_MAX_MISSED_FRAMES)
            return -1;
        } else {
          disconnected = 0;
        }
      }
      if (gSerialReceivedSize != LINK_ACTIONS_BYTES(result))
        return -1;
    }
    return 0;
  }

  // Each action carries its unit's id as the opponent knows it.
  for (index = 0; index < count; index++) {
    struct BattleActionRecord *action = &actions[index];

    action->slot = battle->ownerSlots[action->unitId];

    if (battle->linkSide == 0) {
      if (action->priority & 1)
        action->priority++;
    } else {
      action->priority |= 1;
    }
  }

  if (battle->linkPaused == 0) {
    if (battle->linkSide == 0) {
      u32 saved;

      state->count = count;
      state->check = BattleRandomFar();
      IRQ_LOCK(saved);
      gBattleRandomSeed = state->seed = gRandomSeed;
      IRQ_UNLOCK(saved);
      if (BattleLink_SendActions() < 0 || BattleLink_ReceiveActions() < 0)
        goto fail;
      result = state->count;
    } else {
      if (BattleLink_ReceiveActions() < 0)
        goto fail;
      result = state->count;
      state->count = count;
      if (BattleLink_SendActions() < 0 || BattleRandomFar() != state->check)
        goto fail;
      gBattleRandomSeed = state->seed;
    }

    // The received actions, seen from this side.
    for (index = 0; index < result; index++) {
      struct BattleActionRecord *action = &actions[count + index];

      action->unitId = action->slot;
      action->target ^= FIRST_ENEMY_UNIT;
    }

    MemFree(state);
    return result;
  }

fail:
  BattleLink_ResetTransferState();
  SerialRuntime_RemoveIrqHandlers();
  MemFree(state);
  return -1;
}

//--------------------------------------------------------------------------
// actions.c
// Battle: building the turn's actions, and converting list places
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Copies BG palette 15 to 14 and 5 to 4, brightened or darkened.
// offset : added to each of the red, green and blue values (held in 0 to 31)
//--------------------------------------------------------------------------
void Palette_CopyBanksWithBrightnessOffset(s32 offset) {
  s32 n;
  s32 bank;
  s32 col;
  s32 mask;
  s32 idx;

  bank = 15;
  n = 0;
  mask = 31;
  do {
    for (col = 0; col <= 15; col++) {
      u32 color;
      s32 b;
      s32 g;
      s32 r;
      s32 val;

      idx = bank * 16 + col;
      color = ((u16 *)0x05000000)[idx];
      b = (color >> 10) & mask;
      g = (color >> 5) & mask;
      r = color & mask;
      b += offset;
      g += offset;
      r += offset;
      if (b > 31)
        b = 31;
      if (g > 31)
        g = 31;
      if (r > 31)
        r = 31;
      if (b < 0)
        b = 0;
      if (g < 0)
        g = 0;
      if (r < 0)
        r = 0;
      val = b << 10;
      val |= g << 5;
      val |= r;
      ((s16 *)0x04ffffe0)[idx] = val; // into the palette before
    }
    n++;
    bank = 5;
  } while (n <= 1);
}

//--------------------------------------------------------------------------
// Builds the turn's actions and sorts them into turn order: the party's
// commands, then the link opponent's or the enemies' actions.
// Returns how many actions there are; negative to abort (a link failure).
// actions : the actions (20 at most)
//--------------------------------------------------------------------------
s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions) {
  struct BattleUnit *unit;
  struct BattleWork *work;
  u8 *encounterMode;
  s32 count;
  s32 received;
  s32 i;

  work = gBattleWork;
  {
    struct BattleActionRecord *record = work->actions;
    u32 n;

    for (n = 0; n < BATTLE_ACTION_CAPACITY; n++) {
      record[n].unitId = BATTLE_UNIT_LIST_END;
      record[n].priority = 0x8000; // the lowest priority
    }
  }

  BattleUnit_ResetGuardLevels();
  Palette_CopyBanksWithBrightnessOffset(8);
  // Set while the commands are chosen.
  GameFlag_SetBitFar(363);
  encounterMode = &work->encounterMode;
  Camera_ConfigureScene(0);
  UiWork_FinalizeSharedSlotFar();

  // The party's commands (none when the enemies strike first).
  if (*encounterMode != BATTLE_ENCOUNTER_ENEMIES_FIRST) {
    count = BattlePres_BuildUnitEntries(actions);
    if (count < 0)
      goto finish;
    // A failed escape turns the round into one the enemies began.
    if (count != 0 && actions[0].command == CMD_FLEE &&
        BattleEscape_CheckSuccess() == 0)
      *encounterMode = BATTLE_ENCOUNTER_ENEMIES_FIRST;
  } else {
    count = 0;
  }

  // The link opponent's actions, or the enemies'.
  if (gBattleWork->twoSided != 0) {
    received = BattlePresentation_AppendLinkedActions(actions, count);
    if (BattlePres_WaitSync() < 0) {
      count = -1;
      goto finish;
    }
    count += received;
    if (received < 0) {
      count = -1;
      goto finish;
    }
  } else {
    count += BattlePres_BuildOpponentEntries(actions + count);
  }

  // Sort by priority, and raise the guards of Defend and Total Defense at
  // once, before anyone actions.
  BattleQueue_SortByPriority(actions, count);
  if (count > 0) {
    struct BattleActionRecord *action = actions;

    i = count;
    do {
      if (action->command == CMD_DEFEND || action->command == CMD_DEFEND2) {
        unit = UnitGetFar(action->unitId);
        unit->guardLevel =
            action->command == CMD_DEFEND ? 1 : 2; // a half or a tenth
      }
      action++;
      i--;
    } while (i != 0);
  }

finish:
  GameFlag_ClearBitFar(363);
  Camera_InitDefaultTransform();
  gTransitionWork->targetYaw = YAW_PARTY_TURN;
  return count;
}

//--------------------------------------------------------------------------
// Returns the unit id at a list place.
// slot : the place in the low four bits; 0x80 set for the enemies' list
//--------------------------------------------------------------------------
s32 Battle_GetTaggedSlotValue(s32 slot) {
  struct BattleWork *work = gBattleWork;

  if ((slot & 0x80) != 0)
    return work->enemyIds[slot & 0xf];
  return work->partyUnits[slot & 0xf];
}

//--------------------------------------------------------------------------
// Returns the list place of unit id, or -1 if it is in neither list: the
// party's as 0x100 | place, the enemies' as 0x180 | place.
// id : the unit id (0 to 7 the party, any other an enemy)
//--------------------------------------------------------------------------
s32 Battle_FindTaggedSlotByValue(u32 id) {
  struct BattleWork *work = gBattleWork;
  s32 i;
  s16 unit;

  if (id <= 7) {
    for (i = 0;; i++) {
      unit = work->partyUnits[i];
      if (unit == BATTLE_UNIT_LIST_END)
        return -1;
      if (unit != BATTLE_UNIT_REMOVED && unit == id)
        return i | 0x100;
    }
  }
  for (i = 0;; i++) {
    unit = work->enemyIds[i];
    if (unit == BATTLE_UNIT_LIST_END)
      return -1;
    if (unit != BATTLE_UNIT_REMOVED && unit == id)
      return i | 0x180;
  }
}

// Turns the camera while L or R is held (a second copy).
DEFINE_ADJUST_CAMERA(BattlePres_AdjustCameraByShoulderKeysAlt)

//--------------------------------------------------------------------------
// Does nothing; Battle_RunEncounter calls it at the start of each round.
//--------------------------------------------------------------------------
void Battle_ReservedNoOp9B2C(void) {}

// BattlePlan.outcome: how BattlePresentation_DispatchAction shows the
// action, set by BattleCommand_BuildPlan from the ability (gBattle_ActionStatus
// where it gives one).
enum BattlePlanStaging {
  PLAN_SHOW_MESSAGES = 1,   // no animation: each target's events only
  PLAN_SHOW_ATTACK = 2,     // the actor runs up and strikes
  PLAN_SHOW_CAST = 3,       // a Psynergy, and Venus, Mercury, Mars, Jupiter
  PLAN_SHOW_ITEM = 4,       // an item
  PLAN_SHOW_SUMMON = 5,     // the larger summons, the actor hidden
  PLAN_SHOW_DJINNI = 6,     // a Djinni with no attack (Granite, Quartz...)
  PLAN_SHOW_FLEE = 7,       // a unit runs
  PLAN_SHOW_ENEMY_CAST = 8, // an enemy's Psynergy
  PLAN_SHOW_FULL_SCREEN = 9 // Gaia, Rockfall, Procne and their kin
};

//--------------------------------------------------------------------------
// Plays one action of the round, after delay frames facing the party: an
// enemy picks its ability now; the action is planned (its targets, hits
// and damage) and shown the way its staging asks, then the turn's end
// effects run and every unit returns to its spot. Returns 1 when the
// party got away, 0 otherwise, and -1 when the actor is already down.
//--------------------------------------------------------------------------
s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request,
                                      s32 delay) {
  struct BattlePresentationTransition *view;
  struct BattleWork *battle;
  struct BattleCamera *camera;
  struct BattleUnit *unit;
  u16 actor[2];
  u16 ids[14];
  s32 fled;
  s32 result;
  s32 count;
  s32 i;

  fled = 0;
  if (request->unitId == BATTLE_UNIT_LIST_END)
    return 0;
  unit = UnitGetFar(request->unitId);
  if (unit->hp == 0)
    return -1;
  if (unit->classIndex == 0)
    BattleCommand_SelectAutomatic(request, 1);
  view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  view->frames = 60;
  battle = WORK_SLOT(HEAP_SLOT_BATTLE);
  view->flag = 0;
  battle->brightness = 0x10000;
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, camera->pos);
  Projection_Set(0x01fe0000, Iwram_RatioMulQ14(0x01fe0000, 0xc000), 0x7fff0000);
  if (delay != 0) {
    view->targetYaw = YAW_PARTY_TURN;
    WaitFrames(delay);
  }
  actor[0] = request->unitId;
  actor[1] = BATTLE_UNIT_LIST_END;
  BattlePres_SetActorModes(actor, 1);
  result = BattleCommand_BuildPlan(request, &battle->plan);
  if (result == 0) {
    switch (battle->plan.outcome) {
    case PLAN_SHOW_MESSAGES:
      BattlePres_RunActorEntries(&battle->plan, 0);
      break;
    case PLAN_SHOW_ATTACK:
      RunBattlePresentation(&battle->plan, 0);
      break;
    case PLAN_SHOW_SUMMON:
      BattlePresentation_RunUnitTransition(&battle->plan, 1);
      break;
    case PLAN_SHOW_FULL_SCREEN:
      BattlePresentation_RunUnitTransition(&battle->plan, 0);
      break;
    case PLAN_SHOW_CAST:
      BattlePres_RunPsynergyCast(&battle->plan, 0);
      break;
    case PLAN_SHOW_DJINNI:
      BattlePres_RunPsynergyCast(&battle->plan, 1);
      break;
    case PLAN_SHOW_ENEMY_CAST:
      BattlePres_RunPsynergyCast(&battle->plan, 2);
      break;
    case PLAN_SHOW_ITEM:
      BattlePres_RunItem(&battle->plan, 0, request);
      break;
    case PLAN_SHOW_FLEE:
      if (BattlePresentation_RunEncounterOrUnitTrigger(&battle->plan) != 0)
        fled = 1;
      if (fled != 0)
        goto finish;
      break;
    }
  } else {
    if (result == -1) {
      BattlePresentation_WaitForAdvance();
      WaitFrames(3);
    }
    BattlePres_SetActorModes(0, 0);
  }
  BattleMotion_DestroyAllSlotObjects();
  BattleUnit_ProcessTurnEnd(&battle->plan);
  BattleObjPut();
  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
  for (i = 0; i < count; i++)
    ObjMotRst(ids[i]);
  request->unitId = BATTLE_UNIT_LIST_END; // done
finish:
  BattlePresentation_ConfigurePaletteFade(2, gBattleWork->background, 0);
  return fled;
}

// BattlePlan.presentationFlags: the effect's kind in the low twelve bits and
// its strength (a double or triple blow) above them; then how the effect is
// staged.
#define STAGING_KIND_MASK 0xfff
#define STAGING_VARIANT_MASK 0x3000
#define STAGING_VARIANT_SHIFT 12
#define STAGING_BY_ID_RANGE 0x4000 // BattleFx_DispatchByIdRangeFar
#define STAGING_SCENE 0x8000       // BattleFx_InitializeModeFar, actor's view
#define STAGING_ON_DOWNED 0x10000  // shown on downed targets too (revivals)
#define STAGING_OTHER_SIDE 0x20000 // played from the other side

// The sound as a caster raises its hand.
#define SOUND_CAST 0x9a

//--------------------------------------------------------------------------
// Fills an effect's presentation work from a plan: its kind and strength,
// the actor and the targets still standing (or every target, for an effect
// that revives), at least the first.
//--------------------------------------------------------------------------
s32 BattlePres_BuildTargetList(struct BattlePlan *plan,
                               struct BattlePresentationWork *work) {
  s16 *dst;
  u32 flags;
  s32 count;
  s32 i;
  u8 *target;

  count = 0;
  work->flags = 0;
  flags = plan->presentationFlags;
  work->kind = flags & STAGING_KIND_MASK;
  work->variant = (flags & STAGING_VARIANT_MASK) >> STAGING_VARIANT_SHIFT;
  work->actor = plan->actorId;
  i = 0;
  if (i < plan->targetCount) {
    target = plan->targetIds;
    dst = work->actors;
    do {
      if (UnitGetFar(*target)->hp != 0 ||
          (plan->presentationFlags & STAGING_ON_DOWNED)) {
        count++;
        *dst++ = *target;
      }
      i++;
      target++;
    } while (i < plan->targetCount);
  }
  if (count == 0) {
    work->actors[0] = plan->targetIds[0];
    count = 1;
  }
  work->firstTarget = plan->targetIds[0];
  work->count = count;
  work->initialValue = 1;
}

//--------------------------------------------------------------------------
// Plays a unit's attempt to run. The party gets away unless the enemies
// struck first ("But there's no escape!"), each member able to move running
// off in turn; an enemy gets away 7 times in 10 and leaves the battle.
// Returns 1 when the party got away.
//--------------------------------------------------------------------------
s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BattlePlan *plan) {
  struct BattlePresentationTransition *presentation =
      WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleWork *scene = WORK_SLOT(HEAP_SLOT_BATTLE);
  /* FAKEMATCH: the halfword initialization preserves the word copy into
   * partyMode instead of folding it into an immediate zero (one instruction
   * differs). 2026-10-13: completed as an s32 set to 0, with partyMode =
   * completed or partyMode = 0, differs 2 of 105 (movs r2, #0 for adds r2, r7,
   * #0). */
  union {
    s32 value;
    u16 halves[2];
  } completed;
  s32 partyMode;

  completed.halves[0] = 0;
  completed.halves[1] = 0;
  presentation->targetYaw = YAW_PARTY_TURN;
  presentation->active = 1;
  BattlePres_SetActorModes(0, 0);

  if (plan->actorId <= 7) {
    partyMode = completed.value;

    if (scene->encounterMode != BATTLE_ENCOUNTER_ENEMIES_FIRST) {
      partyMode = 1;
    }

    if (!partyMode) {
      UiText_ShowMessageAndWaitCoreFar(MsgNoEscape);
      BattlePresentation_WaitForAdvance();
    } else {
      s16 unitIds[14];
      s32 index =
          BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, (u16 *)unitIds) - 1;

      while (index != -1) {
        struct BattleUnit *unit = UnitGetFar(unitIds[index]);

        if (unit->stun == 0 && unit->sleep == 0) {
          BattleMotion_SetupEscapeObject(unitIds[index]);
          WaitFrames(8);
        }
        index--;
      }
      WaitFrames(22);
      completed.value = 1;
    }
  } else if (((Random16() * 10) >> 16) <= 6) {
    s16 event[2];

    event[0] = plan->actorId;
    event[1] = BATTLE_UNIT_LIST_END;
    BattleMotion_SetupEscapeObject(event[0]);
    WaitFrames(8);
    BattleActor_RemoveFromLists(plan->actorId);
    ActivateBattleObjectSlot(plan->actorId);
  } else {
    UiText_ShowMessageAndWaitCoreFar(MsgNoEscape);
    BattlePresentation_WaitForAdvance();
  }

  presentation->active = 0;
  return completed.value;
}

//--------------------------------------------------------------------------
// Plays a summon or a full-screen Psynergy. The camera turns to the target
// side (or to the actor's, for a staged scene), the actor glows in its
// element and the other side fades out; then, unless the cast failed
// ("blocked" or too little PP), the target side's units are readied alone
// and the effect plays while the events run. Everyone fades back in.
// mode : bit 0 hides the actor too (the summons)
//--------------------------------------------------------------------------
s32 BattlePresentation_RunUnitTransition(struct BattlePlan *plan, s32 mode) {
  u16 units[14];
  struct BattlePresentationWork work;
  u32 actor;
  u32 target;
  u32 count;
  s32 i;
  s32 n;
  u32 kept;
  struct MotionObject *object;
  // UB: indexing past scalar gBattleWork reaches another work slot. Named
  // slots keep the native EN size (0x3ba), but reorder address setup.
#ifndef UBFIX
  void **slots = (void **)&gBattleWork;
#endif

  BattlePres_BuildTargetList(plan, &work);
  actor = plan->actorId;
  target = plan->targetIds[0];
  if (plan->presentationFlags & STAGING_SCENE) {
    struct BattlePresentationTransition *transition;
    s32 yaw;
#ifdef UBFIX
    transition = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
#else
    transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
    yaw = 0x2000;
    if (actor > 7)
      yaw = 0x5000;
    transition->targetYaw = yaw;
    transition->frames = 60;
  } else {
    struct BattlePresentationTransition *transition;
    s32 yaw;
#ifdef UBFIX
    transition = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
#else
    transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
    yaw = -0x2000;
    if (actor <= 7)
      yaw = 0x2000;
    if (transition->targetYaw != yaw)
      transition->targetYaw = yaw;
  }
  BattlePres_SetActorModes(0, 0);
  PartyStatusDrawFar(gBattleWork->partyStatus & ~1);
  object = BattleObjGet(actor)->object;
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
  for (i = 0; i != count; i++) {
    if (units[i] == BATTLE_UNIT_REMOVED)
      continue;
    if (units[i] != actor) {
      if ((target <= 7) != (units[i] <= 7))
        BattlePres_SetActorRecordMode(units[i], 1);
    } else {
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
    }
  }
  AudioCommand_PlayFar(SOUND_CAST);
  BattleFx_PlayUnitElementEffect(work.actor, plan->element, 0, 0);
  if (mode & 1)
    BattlePres_SetActorRecordMode(actor, 1);
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = BLEND_ALPHA(16 - i, 16);
    WaitFrames(1);
  }
  if (plan->failure != 0) {
    if (plan->failure == 1) {
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, plan->actorId);
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgPsynergyBlocked);
    } else {
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgCastLacksPp);
    }
    BattleEv_DispatchQueued();
    BattlePres_RunWithZeroArguments();
  } else {
    kept = 0;
    for (i = 0; i < count; i++) {
      u32 unit = units[i];
      if (unit == actor) {
        if (!(mode & 1))
          units[kept++] = actor;
      } else if ((target <= 7) == (unit <= 7)) {
        units[kept++] = unit;
      }
    }
    units[kept] = BATTLE_UNIT_LIST_END;
    BattleActor_SpawnObjectsForList(units, 0);
    for (n = 0; n < plan->targetCount; n++)
      units[n] = plan->targetIds[n];
    units[n] = BATTLE_UNIT_LIST_END;
    for (i = 0; i != work.count; i++) {
      struct AnimationObject *animation;
      u32 child;

      animation = GetMotionRecord(BattleObjGet(work.actors[i])->object, 0);
      for (child = 0; child != animation->count - 1; child++)
        work.params[i][child] = animation->entries[child]->param;
    }
    if (plan->presentationFlags & STAGING_SCENE) {
      if (target > 7)
        work.side = 0;
      else
        work.side = 1;
    } else {
      if (plan->targetIds[0] <= 7)
        work.side = 1;
      else
        work.side = 0;
    }
    if (plan->presentationFlags & STAGING_OTHER_SIDE)
      work.side ^= 1;
    Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
    if (plan->presentationFlags & STAGING_SCENE)
      BattleFx_InitializeModeFar((s32 *)&work);
    else if (plan->presentationFlags & STAGING_BY_ID_RANGE)
      BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
    else
      BattleFx_DispatchModeFar((s32 *)&work);
    BattleEventRuntime_WaitForReady();
  }
  BattleObjPut();
  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (i = 0; i != count; i++) {
    if (units[i] == BATTLE_UNIT_REMOVED || units[i] == actor)
      continue;
    if ((target <= 7) != (units[i] <= 7))
      BattlePres_SetActorRecordMode(units[i], 1);
  }
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = BLEND_ALPHA(i, 16);
    WaitFrames(1);
  }
  for (i = 0; i != count; i++)
    BattlePres_SetActorRecordMode(units[i], 0);
  BattlePres_SetupTransitionScene(0, 0, 0, 100);
  WaitFrames(1);
  return 0;
}

//--------------------------------------------------------------------------
// act_run_entries.c
// Battle display: an action's effect on each of its targets
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Clears the highlights, then applies the action to each target in turn and
// plays its events; with no target, only waits for the player.
// plan : the action and its targets
//--------------------------------------------------------------------------
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused) {
  s32 i;

  BattlePres_SetActorModes(0, 0);
  if (plan->targetCount == 0) {
    BattlePresentation_WaitForAdvance();
  } else {
    for (i = 0; i < plan->targetCount; i++) {
      BattleTargetExec(plan, i);
      BattleEv_DispatchQueued();
    }
  }
  WaitFrames(1);
}

//--------------------------------------------------------------------------
// Plays an attack: the camera turns halfway to the attacker's view, the
// attacker runs up to its first target over its swing's frames, and the
// blow lands: a miss makes the target hop back ("nimbly dodges the
// blow"); a hit plays its effect and the events, and a critical hit
// zooms in on the pair with only those two shown. Both then return.
//--------------------------------------------------------------------------
s32 RunBattlePresentation(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  struct MotionObject *object;
  struct AnimationObject *animation;
  struct BattleUnit *unit;
  s16 position[3];
  s32 critical;
  struct BattlePresentationTransition *transition;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 x;
  s32 z;
  s32 frames;
  s32 missed;
  s32 index;
  s32 phase;

  transition = gTransitionWork;
  {
    struct BattlePlan *savedPlan = plan;

    /* FAKEMATCH: the temporary null assignment and restoration end the
     * native first CSE block, so the first plan read reloads its stack
     * carrier. Both conditional arms disappear from the generated code.
     * Without them, 288 of 289 instructions remain and 17 rows differ. */
    if (transition)
      plan = NULL;
    if (transition)
      plan = savedPlan;
  }
  object = BattleObjGet(plan->actorId)->object;
  z = object->z;
  x = object->x;
  angle = (u16)ArcTan2(x, z);
  adjusted = angle - 0x2000;
  if (plan->actorId > 7)
    adjusted = angle + 0x6000;
  adjusted &= 0x7fff;
  facingAngle = (adjusted - 0x2000) / 2 + 0x2000;

  if (transition->targetYaw == facingAngle) {
    transition->targetYaw = facingAngle;
    WaitFrames(5);
  } else {
    transition->targetYaw = facingAngle;
    WaitFrames(10);
  }

  BattlePres_SetActorModes(0, 0);
  BattlePres_BuildTargetList(plan, &work);
  if (work.kind == 0x87)
    PartyStatusDrawFar(gBattleWork->partyStatus & ~1);

  unit = UnitGetFar(work.actor);
  UnitGetFar(work.actors[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  animation = GetMotionRecord(BattleObjGet(plan->actorId)->object, 0);
  frames = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->animId,
                                                 2, 1);
  BattleMotion_ApproachTarget(work.actor, work.actors[0], frames,
                              Battle_GetSpriteFlagHighBits(unit->classId)
                                  << 16);
  ObjectDispatch_ApplyValueToChildrenFar(
      (struct DispatchObject *)BattleObjGet(work.actor)->object, 16);
  BattleObjGet(work.actors[0]);

  if ((u16)work.actors[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  REG_WIN0H = 0x00f0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0x00f0;
  REG_WIN1V = 0x1088;
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f21;
  REG_DISPCNT |= 0x6000;

  if (missed != 0) {
    WaitFrames(10);
    BattleMotion_ResetObjectAtScaledAnchor(work.actors[0]);
    WaitFrames(2);
    WaitFrames(4);
    WaitFrames(10);
    BattleEvPush(BATTLE_EVENT_UNIT_NAME, plan->targetIds[0]);
    BattleEvPush(BATTLE_EVENT_MESSAGE, MsgNimblyDodgesBlow);
    BattleEv_DispatchQueued();
    ObjMotRst(work.actors[0]);
  } else {
    phase = 0;
    work.flags = 0;
    if (plan->presentationFlags != 0)
      work.flags = 1;

    if (critical != 0) {
      work.kind += 200;
      phase = 1;
      transition->flag = 1;
      position[0] = work.actor;
      position[1] = work.firstTarget;
      position[2] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForList(position, 0);
    }

    frames -= 8;
    if (frames <= 0)
      frames = 1;
    {
      s32 first;
      s32 second;

      for (index = 0; index != frames; index++) {
        if (phase != 0) {
          first = work.actor;
          second = work.firstTarget;
          BattlePres_SetupTransitionAtPairMidpoint(first, second,
                                                   index * 30 / frames + 100);
        }
        WaitFrames(1);
      }
    }

    Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
    if (work.kind != 0) {
      if (plan->presentationFlags & STAGING_BY_ID_RANGE)
        BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
      else
        BattleFx_DispatchModeFar((s32 *)&work);
    }
    BattleEventRuntime_WaitForReady();
    if (critical != 0) {
      transition->flag = 0;
      BattleParty_ListAllUnitsAndSubmit();
      BattlePres_SetupTransitionScene(0, 0, 0, 100);
    }
    ObjMotRst(work.actors[0]);
  }
  ObjMotRst(work.actor);
  return 0;
}

//--------------------------------------------------------------------------
// Plays a plain attack run: the attacker runs up to its first target and
// the blow's effect and events play, or, with bit 1 of flags (a miss), the
// target hops back; both then return.
//--------------------------------------------------------------------------
s32 BattlePres_RunSimple(struct BattlePlan *input, s32 flags) {
  struct BattlePresentationWork work;
  struct BattlePlan *savedInput;
  struct MotionObject *object;
  struct AnimationObject *animation;
  struct BattlePresentationTransition *transition;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 x;
  s32 z;
  s32 frames;
  s32 missed;

  transition = gTransitionWork;
  savedInput = input;
  object = BattleObjGet(savedInput->actorId)->object;
  z = object->z;
  x = object->x;
  angle = (u16)ArcTan2(x, z);
  adjusted = angle - 0x2000;
  if (savedInput->actorId > 7)
    adjusted = angle + 0x6000;
  adjusted &= 0x7fff;
  facingAngle = (adjusted - 0x2000) / 2 + 0x2000;

  if (transition->targetYaw == facingAngle) {
    transition->targetYaw = facingAngle;
    WaitFrames(5);
  } else {
    transition->targetYaw = facingAngle;
    WaitFrames(20);
  }

  BattlePres_SetActorModes(0, 0);
  BattlePres_BuildTargetList(savedInput, &work);
  UnitGetFar(work.actor);
  UnitGetFar(savedInput->targetIds[0]);

  missed = flags & 2;
  animation = GetMotionRecord(BattleObjGet(savedInput->actorId)->object, 0);
  frames = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->animId,
                                                 2, 1);
  BattleMotion_ApproachTarget(work.actor, savedInput->targetIds[0], frames, 0);
  ObjectDispatch_ApplyValueToChildrenFar(
      (struct DispatchObject *)BattleObjGet(work.actor)->object, 16);
  BattleObjGet(savedInput->targetIds[0]);

  if (savedInput->targetIds[0] <= 7)
    work.side = 1;
  else
    work.side = 0;
  if (missed != 0) {
    WaitFrames(10);
    BattleMotion_ResetObjectAtScaledAnchor(savedInput->targetIds[0]);
    WaitFrames(2);
    WaitFrames(4);
    WaitFrames(10);
    ObjMotRst(savedInput->targetIds[0]);
  } else {
    BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
    BattleEv_DispatchQueued();
    ObjMotRst(savedInput->targetIds[0]);
  }
  ObjMotRst(work.actor);
  return 0;
}

// ItemDefinition.useType: what using an item in battle does to it.
#define ITEM_USE_CONSUMED 1  // one is used up
#define ITEM_USE_MAY_BREAK 2 // breaks one time in eight
#define ITEM_USE_EMPTIES 4   // Hermes' Water leaves an Empty Bottle

#define ITEM_HERMES_WATER 0xb8
#define ITEM_EMPTY_BOTTLE 0xb9

// Inventory_RemoveFar's result when the last one left its slot, and the
// slots after it moved up.
#define ITEM_REMOVED_SLOT_GONE 2

//--------------------------------------------------------------------------
// Plays an item's use: the camera turns to the user's side, the user raises
// the item and, for an item with an effect, the screen dims while it plays
// with the events. Then the item is spent as its use type says; when its
// slot empties, later item commands of the same member this round follow
// their items to their new slots (or lose theirs, -1).
//--------------------------------------------------------------------------
s32 BattlePres_RunItem(struct BattlePlan *plan, s32 unused,
                       struct BattleActionRecord *selection) {
  struct BattlePresentationWork work;
  struct MotionObject *object;
  s32 i;
  s32 item;
  struct BattleUnit *unit;
  struct ItemDefinition *def;
  struct BattlePresentationTransition *transition = gTransitionWork;
  s32 facing;

  if (plan->actorId > 4)
    facing = YAW_ENEMY_TURN;
  else
    facing = YAW_PARTY_TURN;
  if (transition->targetYaw != facing)
    transition->targetYaw = facing;
  BattlePres_BuildTargetList(plan, &work);
  BattlePres_SetActorModes(0, 0);
  object = BattleObjGet(work.actor)->object;
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
  ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
  if (plan->targetIds[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  for (i = 0; i != work.count; i++) {
    struct AnimationObject *animation =
        GetMotionRecord(BattleObjGet(work.actors[i])->object, 0);
    s32 j;

    for (j = 0; j != animation->count - 1; j++)
      work.params[i][j] = animation->entries[j]->param;
  }

  Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
  if (work.kind != 0) {
    s32 fade;

    i = 0;
    fade = 0;
    do {
      struct BattleWork *battle = gBattleWork;

      if (i < 20) {
        u16 *palette = battle->palette;
        s32 value = 0x10000 - fade;

        battle->brightness = value;
        Graphics_ScaleRgb555Clamped(palette, (u16 *)(BG_PLTT + 6 * 16), value,
                                    128);
      }
      WaitFrames(1);
      i++;
      fade += 0x444;
    } while (i < 20);
    if (plan->presentationFlags & STAGING_BY_ID_RANGE)
      BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
    else
      BattleFx_DispatchModeFar((s32 *)&work);
  } else {
    WaitFrames(60);
  }
  BattleEventRuntime_WaitForReady();
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
  for (i = 0; i != work.count; i++)
    ObjMotRst(work.actors[i]);

  unit = UnitGetFar(selection->unitId);
  item = unit->inventory[selection->parameter];
  def = Item_GetDirectFar(item);
  if (def->useType == ITEM_USE_CONSUMED) {
    s32 result = Inventory_RemoveFar(selection->unitId, selection->parameter);
    s32 index = selection->parameter;
    if (result == ITEM_REMOVED_SLOT_GONE) {
      struct BattleWork *battle = gBattleWork;
      u32 row;

      for (row = 0; row < BATTLE_ACTION_CAPACITY; row++) {
        if (battle->actions[row].command == CMD_ITEM &&
            battle->actions[row].unitId == selection->unitId) {
          if (battle->actions[row].parameter == index)
            battle->actions[row].parameter = -1;
          else if (battle->actions[row].parameter > index)
            battle->actions[row].parameter--;
        }
      }
    }
  } else if (def->useType == ITEM_USE_MAY_BREAK) {
    if ((BattleRandomFar() & 7) == 0) {
      BattleEvPush(BATTLE_EVENT_ITEM_NAME,
                   unit->inventory[selection->parameter]);
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgUsedItemBroke);
      Inventory_BreakFar(selection->unitId, selection->parameter);
      BattleEv_DispatchQueued();
    }
  } else if (def->useType == ITEM_USE_EMPTIES) {
    if ((item & ITEM_ID_MASK) == ITEM_HERMES_WATER)
      item = ITEM_EMPTY_BOTTLE;
    unit->inventory[selection->parameter] = item;
  }
  return 0;
}

//--------------------------------------------------------------------------
// Gives every sprite of an object palette value on its first entry (0 on
// the others) and restarts each entry's frame.
//--------------------------------------------------------------------------
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value) {
  s32 index = 0;
  s32 childIndex;
  s32 count;
  struct AnimationObject *record;
  struct AnimationEntry *child;
  struct AnimationEntry **children;
  /* FAKEMATCH: the mask is a value and the first frame is read into a
   * temporary: loop.c keeps 255 in sl across the call and the frame is or-ed
   * with it, as the native. 2026-10-13: a constant (child->frame = 0xff, |=
   * 0xff, |= -1, = -1) stores movs #255, 18 of 46 differ; child->frame |= mask
   * or = child->frame | mask without the temporary, with a u8, s32 or u32 mask,
   * differ 28 (registers and the order of the two stores). */
  u8 mask = 0xff;
  u8 frame;

  while ((record = GetMotionRecord(object, index)) != NULL) {
    children = &record->entries[1];
    child = record->entries[0];
    frame = child->frame;
    child->frame = frame | mask;
    count = record->count;
    child->param = value;
    for (childIndex = 1; childIndex < count; childIndex++) {
      child = *children++;
      child->param = 0;
      child->frame = 0xff;
    }
    index++;
  }
}

// More of BattlePlan.presentationFlags: the camera takes the other side's
// view (an enemy's cast a view of its own), or turns right round.
#define STAGING_VIEW_ACROSS 0x40000
#define STAGING_VIEW_REVERSED 0x80000

//--------------------------------------------------------------------------
// Plays a Psynergy, a Djinni or an enemy's cast. The camera turns toward
// the targets from the caster's side, the caster raises its hand and glows
// in its element (no glow for a Djinni); then the cast fails ("blocked" or
// too little PP) or its effect plays with the events, and the targets
// return to their spots.
// mode : 1 a Djinni, 2 an enemy's cast, 0 any other
//--------------------------------------------------------------------------
s32 BattlePres_RunPsynergyCast(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  struct BattlePresentationTransition *transition = gTransitionWork;
  struct MotionObject *object;
  s32 isDjinni;
  s32 i;

  if (plan->presentationFlags & STAGING_VIEW_ACROSS) {
    transition->targetYaw = plan->actorId <= 7 ? -0x2000 : 0x5000;
    transition->frames = 60;
  } else {
    struct MotionObject *actor = BattleObjGet(plan->actorId)->object;
    s32 angle = (u16)ArcTan2(actor->x, actor->z);
    s32 current;

    if (plan->actorId <= 7)
      current = angle - 0x1800;
    else
      current = angle + 0x1800;
    current = (s16)current;
    current += ((plan->actorId <= 7 ? 0x2000 : -0x2000) - current) * 3 / 4;
    if (!((plan->actorId <= 7) ^ (plan->targetIds[0] <= 7)))
      current = plan->actorId <= 7 ? 0x2400 : -0x2400;
    if (transition->targetYaw != current)
      transition->targetYaw = current;
  }
  if (plan->presentationFlags & STAGING_VIEW_REVERSED) {
    transition->targetYaw = plan->actorId <= 7 ? -0x2000 : 0x2000;
    transition->frames = 60;
  }

  BattlePres_BuildTargetList(plan, &work);
  isDjinni = mode & 1;
  if (isDjinni)
    work.flags = 1;
  BattlePres_SetActorModes(0, 0);
  PartyStatusDrawFar(gBattleWork->partyStatus & ~1);
  object = BattleObjGet(work.actor)->object;
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
  ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
  AudioCommand_PlayFar(SOUND_CAST);
  if (mode & 2)
    BattleFx_PlayUnitElementEffect(work.actor, plan->element, 1, 0);
  else if (!isDjinni)
    BattleFx_PlayUnitElementEffect(work.actor, plan->element, 0, 0);
  if (plan->targetIds[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  for (i = 0; i != work.count; i++) {
    struct AnimationObject *animation =
        GetMotionRecord(BattleObjGet(work.actors[i])->object, 0);
    s32 j;

    for (j = 0; j != animation->count - 1; j++)
      work.params[i][j] = animation->entries[j]->param;
  }
  if (plan->failure != 0) {
    if (plan->failure == 1) {
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, plan->actorId);
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgPsynergyBlocked);
    } else {
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgCastLacksPp);
    }
    BattleEv_DispatchQueued();
    BattlePres_RunWithZeroArguments();
  } else {
    Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
    if (work.kind) {
      if (plan->presentationFlags & STAGING_BY_ID_RANGE)
        BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
      else
        BattleFx_DispatchModeFar((s32 *)&work);
    } else {
      BattlePres_RunWithZeroArguments();
    }
    BattleEventRuntime_WaitForReady();
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    for (i = 0; i != work.count; i++)
      ObjMotRst(work.actors[i]);
  }
  return 0;
}

//--------------------------------------------------------------------------
// Flashes unit id as it is hit: puts it in pose 5 and switches its sprites
// between palette 7 and its own twice, two frames each, its status panel
// highlighted.
//--------------------------------------------------------------------------
void BattleMotion_RunValueSequence(s32 id, s32 unused0, s32 unused1) {
  u16 selection[2];
  u16 *selected;
  s32 target;
  s32 remaining;

  UnitGetFar(id);
  ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(id)->object, 5);

  selected = selection;
  remaining = 1;
  do {
    target = 0xff;
    selected[1] = target;
    selected[0] = id;
    BattleLayout_HighlightPartyPanelsFar(selected);
    BattleMotion_SetRecordChildValues(BattleObjGet(id)->object, 7);
    WaitFrames(2);

    selected[0] = id;
    BattleLayout_HighlightPartyPanelsFar(selected);
    BattleMotion_SetRecordChildValues(BattleObjGet(id)->object,
                                      BattleMotion_GetSlotPalette(id));
    WaitFrames(2);
    remaining--;
  } while (remaining >= 0);

  PartyStatusDrawFar(gBattleWork->partyStatus);
}

//--------------------------------------------------------------------------
// actor.c
// Battle: removing a downed unit's sprite and places
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Takes unit id out of the party's or the enemies' list and out of this
// round's actions, and frees its place in the enemy count.
// id : the unit id
//--------------------------------------------------------------------------
void BattleActor_RemoveFromLists(s32 id) {
  struct BattleWork *work;
  s32 i;
  u32 j;
  s32 unit;

  // The party's list.
  work = gBattleWork;
  UnitGetFar(id)->side = BATTLE_UNIT_ABSENT;
  for (i = 0;; i++) {
    if (work->partyUnits[i] == id) {
      work->partyUnits[i] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (work->partyUnits[i] == BATTLE_UNIT_LIST_END)
      break;
  }

  // The enemies' list.
  /* FAKEMATCH: the one-pass block around the enemy scan keeps the party loop's
   * shape: its REMOVED constant loaded once ahead of it and the store inside
   * it; without the block the party loop's first test is peeled ahead of it
   * (differ 26 in tbs-en). The enemy scan is a label loop: as for (;;) left by
   * return it walks a pointer and keeps 0xfe in r4 (differ 24), as a do-while
   * on the end mark differ 51. */
  do {
    j = 0;
  again:
    unit = work->enemyIds[j];
    if (unit == id) {
      work->enemyIds[j] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    j++;
    if (unit == BATTLE_UNIT_LIST_END)
      return;
    goto again;
  } while (0);

removed:
  EnemyCountRelease(id);
  for (j = 0; j < BATTLE_ACTION_CAPACITY; j++) {
    if (work->actions[j].unitId == id)
      work->actions[j].unitId = BATTLE_UNIT_LIST_END;
  }
}

//--------------------------------------------------------------------------
// Shows a downed unit: a party member's sprite in animation 4, an enemy's in
// 5; an enemy then leaves the lists after four frames and its sprite dissolves
// a dot at a time.
// id : the unit id
//--------------------------------------------------------------------------
void BattleMotion_InitializeActorRecords(s32 id) {
  void *recs[4];
  struct BattleUnit *unit;
  struct AnimationObject *rec;
  struct AnimationEntry *ent;
  s32 n;

  unit = UnitGetFar(id);
  n = 0;
  while ((rec = GetMotionRecord(BattleObjGet(id)->object, n)) != 0) {
    if (unit->side != BATTLE_UNIT_ENEMY)
      AnimationObjects_SelectAnimationFar(rec, 4);
    else
      AnimationObjects_SelectAnimationFar(rec, 5);
    n++;
  }

  if (unit->side == BATTLE_UNIT_ENEMY) {
    n = 0;
    while ((rec = GetMotionRecord(BattleObjGet(id)->object, n)) != 0) {
      ent = rec->entries[0];
      recs[n] = rec;
      ent->param = 6;
      ent->frame = 0xff;
      n++;
    }
    WaitFrames(4);
    BattleActor_RemoveFromLists(id);
    Map_RenderAllAnimatedTileFramesFar(recs, n); // a dot at a time
    ActivateBattleObjectSlot(id);
  }
}

// The battle work read as halfwords, to walk its unit lists by index.
struct SlotArray {
  s16 items[64];
};

// The list place codes the target pickers return: the party's places
// 0x100 | place, the enemies' 0x180 | place (Battle_FindTaggedSlotByValue).
#define PLACE_PARTY 0x100
#define PLACE_ENEMY 0x180

//--------------------------------------------------------------------------
// Picks a random place: with requireLivingUnit, a party member still
// standing; otherwise any enemy place in use. Returns its place code, or 0
// when there is none.
//--------------------------------------------------------------------------
s32 BattleTarget_SelectRandomPosition(s32 requireLivingUnit) {

  u16 positions[6];
  struct SlotArray *order;
  s16 *entry;
  u16 *cursor;
  s32 value;
  s32 count;
  s32 index;
  s32 slot;
  s32 tail;
  s32 offset;

  count = 0;
  order = (struct SlotArray *)gBattleWork;

  if (requireLivingUnit != 0) {
    /* FAKEMATCH: a loop that runs once, left by its closing break. Written
     * plain, without the loop, the function differs 35 instructions in EN (85
     * -> 84): the first branch's zero offset takes r2 for r1 and the else
     * branch sets up its pointers in another order. */
    for (;;) {
      index = 0;
      slot = (u32)&((struct BattleWork *)0)->partyUnits / sizeof(s16);
      if (order->items[slot] != BATTLE_UNIT_LIST_END) {
        entry = order->items;
        do {
          value = entry[slot];
          if (value != BATTLE_UNIT_REMOVED) {
            if (UnitGetFar(value)->hp != 0) {
              positions[count] = index | PLACE_PARTY;
              count++;
            }
          }
          slot++;
          index++;
        } while (entry[slot] != BATTLE_UNIT_LIST_END);
      }
      break;
    }
  } else {
    index = 0;
    slot = (u32)&((struct BattleWork *)0)->enemyIds / sizeof(s16) - 1;
    tail = (u32)&((struct BattleWork *)0)->enemyIds / sizeof(s16) - 1;
    offset = tail * 2;
    entry = (order->items + 1);
    if (*(s16 *)((char *)entry + offset) != BATTLE_UNIT_LIST_END) {
      cursor = entry;
      do {
        if ((s16)cursor[slot] != BATTLE_UNIT_REMOVED) {
          positions[count] = index | PLACE_ENEMY;
          count++;
        }
        slot++;
        tail++;
        index++;
      } while (entry[tail] != BATTLE_UNIT_LIST_END);
    }
  }

  if (count == 0)
    return 0;
  return positions[(Random16() * count) >> 16];
}

// Counts the ailments of unit that a partial cure removes (delusion,
// confusion, charm, sleep, Psynergy seal, the Grim Reaper's curse).
#define COUNT_PARTIAL_CURES(unit, count)                                       \
  {                                                                            \
    if ((unit)->delusion != 0)                                                 \
      (count)++;                                                               \
    if ((u8)(unit)->confusion != 0)                                            \
      (count)++;                                                               \
    if ((unit)->charm != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->sleep != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->psySeal != 0)                                                  \
      (count)++;                                                               \
    if ((unit)->grimReaper != 0)                                               \
      (count)++;                                                               \
  }

// Counts the ailments a full cure removes: those, haunting and poison.
#define COUNT_ALL_CURES(unit, count)                                           \
  {                                                                            \
    COUNT_PARTIAL_CURES((unit), (count));                                      \
    if ((unit)->haunt != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->poison != 0)                                                   \
      (count)++;                                                               \
  }

// Counts unit's raised stats: Attack, Defense, Resistance and each element.
#define COUNT_POSITIVE_MODIFIERS(unit, count)                                  \
  {                                                                            \
    if ((unit)->attackStage > 0)                                               \
      (count)++;                                                               \
    if ((unit)->defenseStage > 0)                                              \
      (count)++;                                                               \
    if ((unit)->resistanceStage > 0)                                           \
      (count)++;                                                               \
    if ((unit)->elementLevels[0] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[1] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[2] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[3] > 0)                                          \
      (count)++;                                                               \
  }

// How worthwhile an enemy's ability is against unit, in applies (0 for
// pointless): a stat change that can still move (one more when it is about
// to wear off), a cure or a heal the unit needs, an ailment it lacks, a
// revival of a downed unit (100). Failing its added effect, the ability's
// damage or healing must have something to work on. kindIndex receives the
// ability's kind less one.
#define CHECK_EFFECT_TARGET(unit, action, applies, kindIndex)                  \
  {                                                                            \
    (applies) = 0;                                                             \
    switch ((action)->effect) {                                                \
    case EFFECT_ATK_UP2:                                                       \
    case EFFECT_ATK_UP1:                                                       \
      if ((unit)->attackStage + 1 <= 4)                                        \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case EFFECT_ATK_DOWN2:                                                     \
    case EFFECT_ATK_DOWN1:                                                     \
      if ((unit)->attackStage - 1 >= -4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case EFFECT_DEF_UP2:                                                       \
    case EFFECT_DEF_UP1:                                                       \
      if ((unit)->defenseStage + 1 <= 4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case EFFECT_DEF_DOWN2:                                                     \
    case EFFECT_DEF_DOWN1:                                                     \
      if ((unit)->defenseStage - 1 >= -4)                                      \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case EFFECT_RES_UP2:                                                       \
    case EFFECT_RES_UP1:                                                       \
      if ((unit)->resistanceStage + 1 <= 4)                                    \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case EFFECT_RES_DOWN2:                                                     \
    case EFFECT_RES_DOWN1:                                                     \
      if ((unit)->resistanceStage - 1 >= -4)                                   \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case EFFECT_CURE_POISON:                                                   \
      if ((unit)->poison != 0)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case EFFECT_CURE_PART:                                                     \
      COUNT_PARTIAL_CURES((unit), (applies));                                  \
      break;                                                                   \
    case EFFECT_HEAL_60:                                                       \
    case EFFECT_HEAL_30:                                                       \
      if ((unit)->hp < (unit)->maxHp)                                          \
        (applies) = 1;                                                         \
      break;                                                                   \
    case EFFECT_BUFF_CLEAR:                                                    \
      COUNT_POSITIVE_MODIFIERS((unit), (applies));                             \
      break;                                                                   \
    case EFFECT_CURE_ALL:                                                      \
      COUNT_ALL_CURES((unit), (applies));                                      \
      break;                                                                   \
    case EFFECT_DEATH_CURSE:                                                   \
      if ((unit)->grimReaper == 0)                                             \
        (applies) = 1;                                                         \
      break;                                                                   \
    case EFFECT_POISON:                                                        \
      if ((unit)->poison == 0)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case EFFECT_VENOM:                                                         \
      if ((unit)->poison <= 1)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case EFFECT_STUN:                                                          \
      if ((unit)->stun == 0)                                                   \
        (applies) = 1;                                                         \
      break;                                                                   \
    case EFFECT_SLEEP:                                                         \
      if ((unit)->sleep == 0)                                                  \
        (applies) = 1;                                                         \
      break;                                                                   \
    case EFFECT_EVIL_SPIRIT:                                                   \
      if ((unit)->haunt == 0)                                                  \
        (applies) = 1;                                                         \
      break;                                                                   \
    case EFFECT_REVIVE_FULL:                                                   \
    case EFFECT_REVIVE_HALF:                                                   \
    case EFFECT_REVIVE_80:                                                     \
      if ((unit)->hp == 0)                                                     \
        (applies) = 100;                                                       \
      break;                                                                   \
    case 0:                                                                    \
    case 1:                                                                    \
    case 2:                                                                    \
      break;                                                                   \
    default:                                                                   \
      (applies) = 1;                                                           \
      break;                                                                   \
    }                                                                          \
    if ((unit)->hp == 0 && Battle_IsReviveEffectFar((action)->effect) == 0)    \
      (applies) = 0;                                                           \
    if ((applies) == 0) {                                                      \
      (kindIndex) = ((action)->flags & ABILITY_KIND_MASK) - 1;                 \
      switch (kindIndex) {                                                     \
      case ABILITY_KIND_ATTACK - 1:                                            \
      case ABILITY_KIND_ATTACK_MULTIPLIED - 1:                                 \
        if ((unit)->hp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_SUMMON - 1:                                            \
      case 9 - 1:                                                              \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_PP_DAMAGE - 1:                                         \
        if ((unit)->pp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_HEAL_HP - 1:                                           \
        if ((unit)->hp != 0 && (unit)->hp < (unit)->maxHp)                     \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_EFFECT - 1:                                            \
      case ABILITY_KIND_DAMAGE - 1:                                            \
      case ABILITY_KIND_UNLEASHED_DAMAGE - 1:                                  \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

// BattleAct.targetMode: whom an ability may target, from its user's view.
#define TARGET_MODE_NONE 0
#define TARGET_MODE_FOES 1
#define TARGET_MODE_ALLIES 2
#define TARGET_MODE_SELF 4

// EnemyDefinition.targetStrategy: a single-target attacker favours the
// target with the most HP left, the most maximum HP, or no one.
#define STRATEGY_MOST_HP 0
#define STRATEGY_MOST_MAX_HP 1
#define STRATEGY_RANDOM 2

//--------------------------------------------------------------------------
// Picks the target of an enemy's ability (or a confused unit's), as a place
// code: among the units of the side it may target, those it would affect
// (CHECK_EFFECT_TARGET), at random; a single-target attack by an enemy with
// a strategy favours the strongest, the first of the sorted targets 6 times
// in 11, 15 or 18, the next 5 times, and so on. Returns -2 when the side
// has no one, -1 when the ability would affect no one.
//--------------------------------------------------------------------------
s32 BattleTarget_SelectForAction(s32 actorId, struct BattleAct *action) {
  u16 orderPositions[6];
  s32 unitIds[6];
  s32 targetPositions[6];
  struct BattleWork *turnOrder;
  struct BattleUnit *unit;
  struct BattleUnit *nextUnit;
  struct BattleUnit *thisUnit;
  s32 candidateCount;
  s32 targetIndex;
  s32 targetCount;
  s32 innerIndex;
  s32 selected;
  s32 unitId;
  s32 value;
  s32 nextValue;
  s32 temp;
  s32 ownSide;
  s32 kindIndex;
  s32 applies;
  u32 roll;
  s16 *slot;

  turnOrder = gBattleWork;
  targetCount = 0;
  candidateCount = 0;

  if (action->targetMode != TARGET_MODE_NONE) {
    ownSide = 0;
    if (action->targetMode == TARGET_MODE_ALLIES ||
        action->targetMode == TARGET_MODE_SELF)
      ownSide = 1;

    if (actorId < 0 || actorId > 7) {
      if (ownSide != 0)
        goto scan_enemies;
    } else if (ownSide == 0) {
      goto scan_enemies;
    }

    targetIndex = 0;
    while (turnOrder->partyUnits[targetIndex] != BATTLE_UNIT_LIST_END) {
      slot = &turnOrder->partyUnits[targetIndex];
      unitId = *slot;
      if (unitId != BATTLE_UNIT_REMOVED) {
        if (action->targetMode != TARGET_MODE_SELF || unitId == actorId) {
          unitIds[candidateCount] = unitId;
          orderPositions[candidateCount] = PLACE_PARTY;
          orderPositions[candidateCount] |= targetIndex;
          candidateCount++;
        }
      }
      targetIndex++;
    }
    goto scan_complete;

  scan_enemies:
    targetIndex = 0;
    while (turnOrder->enemyIds[targetIndex] != BATTLE_UNIT_LIST_END) {
      unitId = turnOrder->enemyIds[targetIndex];
      if (unitId != BATTLE_UNIT_REMOVED) {
        if (action->targetMode != TARGET_MODE_SELF || unitId == actorId) {
          unitIds[candidateCount] = unitId;
          orderPositions[candidateCount] = PLACE_ENEMY;
          orderPositions[candidateCount] |= targetIndex;
          candidateCount++;
        }
      }
      targetIndex++;
    }
  }

scan_complete:

  if (candidateCount == 0)
    return -2;

  for (targetIndex = 0; targetIndex < candidateCount; targetIndex++) {
    unit = UnitGetFar(unitIds[targetIndex]);
    CHECK_EFFECT_TARGET(unit, action, applies, kindIndex);
    if (applies != 0) {
      unitIds[targetCount] = unitIds[targetIndex];
      targetPositions[targetCount] = orderPositions[targetIndex];
      targetCount++;
    }
  }

  if (targetCount == 0)
    return -1;

  if (action->targetMode == TARGET_MODE_FOES && action->range == 1 &&
      ((struct EnemyDefinition *)Enemy_GetDefinitionFar(
           UnitGetFar(actorId)->classId))
              ->targetStrategy != STRATEGY_RANDOM &&
      (u32)((action->flags & ABILITY_KIND_MASK) - ABILITY_KIND_ATTACK) <= 2) {
    selected = -1;
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
      for (innerIndex = targetIndex; innerIndex < targetCount - 1;
           innerIndex++) {
        thisUnit = UnitGetFar(unitIds[innerIndex]);
        nextUnit = UnitGetFar(unitIds[innerIndex + 1]);
        if (((struct EnemyDefinition *)Enemy_GetDefinitionFar(
                 UnitGetFar(actorId)->classId))
                ->targetStrategy == STRATEGY_MOST_HP) {
          value = thisUnit->hp;
          nextValue = nextUnit->hp;
        } else {
          value = thisUnit->maxHp;
          nextValue = nextUnit->maxHp;
        }
        if (value < nextValue) {
          temp = unitIds[innerIndex];
          unitIds[innerIndex] = unitIds[innerIndex + 1];
          unitIds[innerIndex + 1] = temp;
          temp = targetPositions[innerIndex];
          targetPositions[innerIndex] = targetPositions[innerIndex + 1];
          targetPositions[innerIndex + 1] = temp;
        }
      }
    }

    switch (targetCount) {
    case 1:
      selected = 0;
      break;
    case 2:
      roll = (11 * Random16()) >> 16;
      selected = 0;
      if (roll > 5)
        selected = 1;
      break;
    case 3:
      selected = (15 * Random16()) >> 16;
      if (selected <= 5)
        selected = 0;
      else if (selected <= 10)
        selected = 1;
      else
        selected = 2;
      break;
    case 4:
      selected = (18 * Random16()) >> 16;
      if (selected <= 5)
        selected = 0;
      else if (selected <= 10)
        selected = 1;
      else if (selected <= 14)
        selected = 2;
      else
        selected = 3;
      break;
    }

    if (selected >= 0)
      return targetPositions[selected];
  }
  return targetPositions[(Random16() * targetCount) >> 16];
}

//--------------------------------------------------------------------------
// actor_reset_runtime_fields.c
// Battle: clearing a unit's states, and waiting for the player
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Clears every ailment and stat change of unit id, recalculates its stats
// and rebuilds its status icons; called when the unit goes down.
//--------------------------------------------------------------------------
s32 BattleActor_ResetRuntimeFields(s32 id) {
  struct BattleUnit *unit;
  s8 *lv;
  s32 n;
  u8 zero;

  unit = UnitGetFar(id);
  n = 3;
  zero = 0;
  lv = &unit->elementLevels[3];
  do {
    n--;
    *lv-- = zero;
  } while (n >= 0);

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

  UnitCalcFar(id);
  return BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
}

//--------------------------------------------------------------------------
// The arrow that asks the player to go on
//--------------------------------------------------------------------------
#define KEY_ADVANCE (KEY_A | KEY_B | KEYS_SHOULDERS) // the keys that go on

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

//--------------------------------------------------------------------------
// Shows the bobbing arrow at the message window and waits for the player:
// B held, or any of KEY_ADVANCE pressed, goes on; after 16 frames holding one
// of them is enough. Plays the cursor sound.
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForAdvance(void) {
  struct AdvanceSprite *spr;
  struct AdvanceSprite sprite;
  s32 frame;
  s32 slot;
  s32 pat;
  struct UiWindow *window;
  struct BattleDisplayOffset *offset;
  s32 attr2;
  s32 attr1;
  s32 x;
  s16 tile;

  while (!UiWork_IsCompleteFar())
    WaitFrames(1);
  spr = &sprite;
  slot = VramCache_AllocEntry(128);
  for (frame = 0;; frame++) {
    // Cycle the eight frames, one every four frames.
    pat = ((gFrameCount >> 2) & 7) * 128 + (s32)gBattlePres_AdvanceArrowTiles;
    window = gBattleDisplayWork->window;
    offset = gBattleDisplayWork->offset;
    QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
    QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
    Io_Write16(16, &REG_BLDALPHA);
    *(u32 *)&spr->attr0 = 0xa400;
    *(u32 *)&spr->attr2 = 0;
    tile = VramCache_Reload(slot, pat) & 0x3ff;
    attr2 = spr->attr2;
    spr->attr2 = (attr2 & ~0x3ff) | tile;
    // The window is placed in tiles, its offset in 1/256 pixels.
    x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
    attr1 = spr->attr1;
    spr->attr1 = (attr1 & ~0x1ff) | x;
    // Bob up and down by up to 2 pixels.
    *(u8 *)&spr->attr0 = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                         (offset->y >> 8) + 6;
    Runtime_PushSlotEntry(spr, 240);
    if ((gKeysHeld & KEY_B) || (gKeysPressed & KEY_ADVANCE) ||
        (frame > 15 && (gKeysHeld & KEY_ADVANCE)))
      break;
    WaitFrames(1);
  }
  AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
  Resource_ResetEntry(slot);
  WaitFrames(1);
}

//--------------------------------------------------------------------------
// Shows the cursor bobbing at (x, y) and waits for one of KEY_ADVANCE.
// x, y : in pixels
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y) {
  struct MenuPoint pos;
  s32 pat;
  struct CursorSprite sprite;
  struct CursorSprite *spr;
  s32 slot;

  pat = Ui_GetTableWordZeroFar(0);
  pos.x = x;
  pos.y = y;
  while (!UiWork_IsCompleteFar())
    WaitFrames(1);
  spr = &sprite;
  slot = VramCache_AllocEntry(128);
  while (1) {
    QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
    QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
    Io_Write16(16, &REG_BLDALPHA);
    ((s32 *)spr)[1] = MENU_SPRITE_SIZE_16;
    ((s32 *)spr)[2] = 0;
    spr->tile = VramCache_Reload(slot, pat);
    // Nudge it up and right every four frames.
    spr->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
    spr->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
    Runtime_PushSlotEntry(spr, 240);
    if (gKeysPressed & KEY_ADVANCE)
      break;
    WaitFrames(1);
  }
  Resource_ResetEntry(slot);
  WaitFrames(1);
}

//--------------------------------------------------------------------------
// Marks the battle display for redrawing.
//--------------------------------------------------------------------------
void BattleEv_MarkDisplay(void) { gBattleDisplayWork->marked = 1; }

//--------------------------------------------------------------------------
// Takes an enemy out of the battle at once (one that leaves rather than
// falls): its HP to 0, out of the lists, and its object released.
// Declared to return s32, but returns nothing.
//--------------------------------------------------------------------------
s32 BattleActor_DestroyTemporaryObject(s32 unitId) {
  struct BattleUnit *enemy;
  struct BattleObj *slot;

  enemy = UnitGetFar(unitId);
  if (enemy->side == BATTLE_UNIT_ENEMY) {
    Unit_SetHpFar(enemy, 0);
    BattleActor_RemoveFromLists(unitId);
    ActivateBattleObjectSlot(unitId);
    slot = BattleObjGet(unitId);
    ObjectDispatch_ReleaseFar((struct DispatchObject *)slot->object);
    slot->object = 0;
    slot->active = 0;
  }
}

// BattleEventState.flags bit 0: a Djinni struck a weakness, so enemies that
// fall pay the Djinn bonus (BattleEnemy_RecordDefeat).
#define EVENT_FLAG_DJINN_BONUS 1

//--------------------------------------------------------------------------
// The BATTLE_EVENT_DJINN_BONUS event: grants the Djinn bonus for the rest of
// the action. Its operand is ignored.
//--------------------------------------------------------------------------
void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand) {
  runtime->flags |= EVENT_FLAG_DJINN_BONUS;
}

//--------------------------------------------------------------------------
// Plays the queued battle events at once, in order (BattleEvent_Playback
// plays them over frames instead): the messages with their arguments, the
// hit flashes, the downed units, the redraws. Then empties the queue.
//--------------------------------------------------------------------------
u32 BattleEv_DispatchQueued(void) {
  struct BattleEventState *runtime = &gBattleWork->events;
  struct BattleEventQueue *queue = &runtime->queue;
  s32 i;

  for (i = 0; i < queue->count; i++) {
    u8 opcode = queue->opcodes[i];
    if (opcode <= BATTLE_EVENT_DJINN_BONUS)
      switch (opcode) {
      case BATTLE_EVENT_DJINN_BONUS:
        Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]);
        break;
      case BATTLE_EVENT_REMOVE_UNIT:
        BattleActor_DestroyTemporaryObject(queue->operands[i]);
        break;
      case BATTLE_EVENT_UNIT_NAME:
        UiWork_PushValueSlotFar(queue->operands[i], VALUE_UNIT_NAME);
        break;
      case BATTLE_EVENT_NUMBER:
        UiWork_PushValueSlotFar(queue->operands[i], VALUE_NUMBER);
        break;
      case BATTLE_EVENT_ITEM_NAME:
        UiWork_PushValueSlotFar(queue->operands[i] & ITEM_ID_MASK,
                                VALUE_ITEM_NAME);
        break;
      case BATTLE_EVENT_ABILITY_NAME:
        UiWork_PushValueSlotFar(queue->operands[i] & OWNER_ACTION_ID_MASK,
                                VALUE_ABILITY_NAME);
        break;
      case BATTLE_EVENT_MARK:
        gBattleDisplayWork->marked = 1;
        break;
      case BATTLE_EVENT_CLEAR_ARGUMENTS:
        UiWork_ClearValueNameTablesFar();
        break;
      case BATTLE_EVENT_MESSAGE:
        if ((s32)queue->operands[i] >= 0)
          UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
        BattlePresentation_WaitForAdvance();
        UiWork_ClearValueNameTablesFar();
        break;
      case BATTLE_EVENT_MESSAGE_NO_WAIT:
        if ((s32)queue->operands[i] >= 0)
          UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
        UiWork_ClearValueNameTablesFar();
        break;
      case BATTLE_EVENT_HIT:
        if (runtime->pendingCue > 0)
          AudioCommand_PlayFar(runtime->pendingCue);
        BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
        break;
      case BATTLE_EVENT_DEFEAT:
        BattleEnemy_RecordDefeat(queue->operands[i], runtime->flags);
        BattleActor_ResetRuntimeFields(queue->operands[i]);
        BattleMotion_InitializeActorRecords(queue->operands[i]);
        break;
      case BATTLE_EVENT_DRAW_STATUS:
        PartyStatusDrawFar(gBattleWork->partyStatus);
        break;
      case BATTLE_EVENT_REFRESH_UNIT:
        BattleUnit_BuildStatusFlags(queue->operands[i],
                                    BattleObjGet(queue->operands[i]));
        BattlePres_SetActorModeAndAction(queue->operands[i]);
        break;
      }
  }
  return BattleEventRuntime_Reset();
}

//--------------------------------------------------------------------------
// Queues one battle event (enum BattleEventOpcode) with its operand, and
// returns the opcode.
//--------------------------------------------------------------------------
u32 BattleEvPush(u32 opcode, u32 operand) {
  struct BattleEventQueue *queue = &gBattleWork->events.queue;
  u32 index = queue->count;

  queue->opcodes[index] = opcode;
  queue->operands[index] = operand;
  queue->count = index + 1;
  return opcode;
}

//--------------------------------------------------------------------------
// tgtexec.c
// Battle: an ability's effect on each of its targets
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Returns 1 for an effect that still applies when the ability's damage has
// just knocked its target out: the drains, which give the damage they dealt
// back to the actor.
// effect : the ability's effect (enum AbilityEffect)
//--------------------------------------------------------------------------
s32 BattleEffectDeadCheck(s32 effect) {
  switch (effect) {
  case ABILITY_EFFECT_DRAIN_HP:
  case ABILITY_EFFECT_DRAIN_PP:
  case ABILITY_EFFECT_DRAIN_HALF_HP:
  case ABILITY_EFFECT_PSY_LEECH:
    return 1;
  default:
    return 0;
  }
}

//--------------------------------------------------------------------------
// The target's checks, effect and messages
//--------------------------------------------------------------------------
// A stat stage changes the stat by an eighth of it (UnitCalc), from half at
// the lowest to one and a half at the highest.
#define STAT_STAGE_MIN -4
#define STAT_STAGE_MAX 4
// Zephyr's Agility stage, beyond the usual highest: twice the Agility.
#define AGILITY_STAGE_DOUBLED 8
// A Resistance stage adds 20 to the resistance to every element (UnitCalc).
#define RESISTANCE_PER_STAGE 20

// The turns a stat change or a status lasts; each may end sooner, at the
// rolls the end of every turn makes (BattleUnit_RollCounterEarlyEnd).
#define STAT_CHANGE_TURNS 7
#define AGILITY_CHANGE_TURNS 5
#define STATUS_TURNS 7
// The Grim Reaper's countdown.
#define CURSE_TURNS 7

// BattleUnit.psySeal: Bind's seal counts its seven turns in the low three
// bits, and may end sooner; Luff's counts down 8 a turn, two turns that never
// end early (Battle_AdvanceCounterAndCheckChance).
#define PSY_SEAL_BIND 7
#define PSY_SEAL_LUFF 16

// BattleUnit.guardLevel: Defend and Granite halve the damage, Total Defense and
// Flash cut it to a tenth.
#define GUARD_DEFEND 1
#define GUARD_TOTAL_DEFENSE 2

// BattleUnit.poison: the poison takes a tenth of the maximum HP at the end of
// each turn, deadly poison a fifth.
#define POISON_NORMAL 1
#define POISON_DEADLY 2

// The rate BattlePowerCalc and BattleRecoveryCalc scale by: 256 is 1.
#define FULL_RATE 256

// A message operand that shows nothing: the event only waits for the player.
#define MESSAGE_NONE -1

// Unit ids 0 to 7 are the party's; the enemies' start at 8.
#define IS_PARTY_UNIT(unitId) ((unitId) >= 0 && (unitId) < 8)

// The helpers below are macros because the bytes need them so: written as
// inline functions they compile to different code.

// Holds a stat stage within STAT_STAGE_MIN to STAT_STAGE_MAX.
#define CLAMP_STAT_STAGE(stage)                                                \
  {                                                                            \
    if ((stage) < STAT_STAGE_MIN)                                              \
      (stage) = STAT_STAGE_MIN;                                                \
    if ((stage) > STAT_STAGE_MAX)                                              \
      (stage) = STAT_STAGE_MAX;                                                \
  }

// Pushes one message for a unit of the party and another for an enemy.
#define PUSH_SIDE_MESSAGE(unitId, partyMessage, enemyMessage)                  \
  {                                                                            \
    if (IS_PARTY_UNIT(unitId))                                                 \
      PUSH_MESSAGE(partyMessage);                                              \
    else                                                                       \
      PUSH_MESSAGE(enemyMessage);                                              \
  }

// A guarding unit takes half the damage, or a tenth behind Total Defense.
#define REDUCE_BY_GUARD(damage, unit)                                          \
  {                                                                            \
    s32 guard = (unit)->guardLevel;                                            \
                                                                               \
    if (guard != 0) {                                                          \
      if (guard == GUARD_DEFEND)                                               \
        (damage) /= 2;                                                         \
      else                                                                     \
        (damage) /= 10;                                                        \
    }                                                                          \
  }

//--------------------------------------------------------------------------
// Applies the planned ability to one of its targets: first its damage or
// healing, then its added effect (a status, a stat change, a revival...).
// Nothing is shown here: each step queues battle events (the names and
// numbers a message needs, the message, the target's reactions) that
// BattleEvent_Playback then plays in order. The units themselves change at
// once: UnitCalcFar recalculates a unit's stats from its stages, and
// UnitRatioFar its HP and PP gauges after either changes.
//
// plan : the action under way: the actor, the ability, its element and its
//        targets, each with its distance from the aimed position, damage
//        multiplier, critical hit and any hit roll made in advance.
// slot : which of the plan's targets (0 to plan->targetCount - 1).
//
// Declared to return s32, as the bytes require (the epilogue keeps r0), but
// it returns nothing and its callers ignore the result.
//--------------------------------------------------------------------------
s32 BattleTargetExec(struct BattlePlan *plan, s32 slot) {
  s32 distance; // steps from the aimed position, 0 to 5
  struct BattleAct *ability;
  struct BattleUnit *actor;
  s32 actorId;
  s32 abilityId;
  s32 elementBonus; // elemental power less resistance
  struct BattleWork *work;
  s32 halveDefense; // the attack counts half the target's Defense
  s32 multiplier;   // 0 on a miss, 2 or 3 for a double or triple blow
  s32 damageDealt;  // HP or PP the target lost (negative if healed)
  s32 leaveOneHp;   // the blow leaves the target 1 HP at most
  s32 hit;          // the ability's effect lands
  s32 critical;     // enum BattleCritical
  s32 skipDamage;   // no damage step: instant death replaces it
  s32 kind;         // how the power applies: enum AbilityKind
  s32 affinity;     // -1 a resisted element, 1 a weakness, else 0
  // The target as it was, to measure each stat's change.
  struct BattleUnit *savedTarget;
  s32 elementPower; // the actor's power in the ability's element
  s32 targetId;
  s32 element;
  struct BattleUnit *target;
  s32 amount; // the damage, or the HP or PP restored
  s32 unitSize;

  // Keep a copy of the target as it was, to show how much each stat changes.
  elementBonus = 0;
  work = gBattleWork;
  halveDefense = 0;
  damageDealt = 0;
  leaveOneHp = 0;
  skipDamage = 0;
  affinity = 0;
  unitSize = sizeof(struct BattleUnit);
  savedTarget = MemAlloc(unitSize);

  actorId = plan->actorId;
  targetId = plan->targetIds[slot];
  abilityId = plan->abilityId;
  element = plan->element;
  multiplier = plan->targetMultiplier[slot];
  critical = plan->targetCritical[slot];

  ability = BattleActGet(abilityId);
  actor = UnitGetFar(actorId);
  target = UnitGetFar(targetId);
  Iwram_CopyWords(savedTarget, target, unitSize);

  // The target's distance from the aimed position: every damage and
  // healing table falls off with it. An ability that reaches the whole side
  // treats every target as aimed at.
  if (ability->range != ABILITY_RANGE_ALL) {
    distance = plan->targetOffset[slot];
    if (distance < 0)
      distance = -distance;
  } else {
    distance = 0;
  }

  // The element's affinity: -1 when the target resists no element better
  // than this one, 1 when it resists none worse. It picks the damage
  // message ("damage.", "damage!" or "damage!!!").
  if (element != ELEMENT_NONE) {
    s32 resistance;
    s32 i;

    resistance = target->baseElement[element].resistance;
    for (i = 0; i < NUM_ELEMENTS; i++) {
      if (resistance < target->baseElement[i].resistance)
        break;
    }
    if (i == NUM_ELEMENTS)
      affinity = -1;

    for (i = 0; i < NUM_ELEMENTS; i++) {
      if (resistance > target->baseElement[i].resistance)
        break;
    }
    if (i == NUM_ELEMENTS)
      affinity = 1;
  }

  // The actor's power in the ability's element; items always use 100. (Here
  // and below the element is read from the plan again, as the bytes need.)
  if (plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER &&
      plan->command != CMD_ITEM)
    elementPower = actor->element[plan->element].power;
  else
    elementPower = 100;

  // A Djinni striking the target's weakness may earn the Djinn bonus: an
  // enemy it fells then gives more coins and experience and a better chance
  // of its item (BattleEnemy_RecordDefeat). The chance is the power less
  // the resistance, plus 30, in percent; the event's operand is unused.
  if (plan->command == CMD_DJINN &&
      (plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER) &&
      affinity > 0) {
    s32 chance;

    chance = elementPower - target->element[plan->element].resistance;
    chance += 30;
    chance *= 65536 / 100; // percent to a fraction of the 16-bit random range
    if (chance > (BattleRandomFar() & 0xffff))
      BattleEvPush(BATTLE_EVENT_DJINN_BONUS, 5);
  }

  // Whether the effect lands: a roll made when the action was planned, or
  // one made now, less likely the farther the target is.
  kind = ability->flags & ABILITY_KIND_MASK;
  {
    s32 plannedHit;

    plannedHit = plan->targetHit[slot];
    if (plannedHit == HIT_NOT_ROLLED)
      hit = BattleHitCheck(actorId, targetId, element, ability->effect,
                           gHitTable[distance]);
    else
      hit = plannedHit;
  }

  // Calling for help: an enemy calls another of its kind, or a random member
  // of its formation, into an empty enemy slot. Speed Surge is an enemy
  // splitting itself in two.
  if (ability->effect == ABILITY_EFFECT_CALL_ALLY ||
      ability->effect == ABILITY_EFFECT_CALL_HELP) {
    s32 classId;
    s32 newUnitId;

    classId = actor->classId;
    newUnitId = EnemySlotGet();
    if (ability->effect == ABILITY_EFFECT_CALL_HELP)
      classId = BattleGroupRandom(work->enemyGroup);
    if (hit != 0 && EnemyCheck(classId) != 0 && newUnitId >= 0) {
      s32 suffix;

      // The newcomer's name letter; on the first duplicate the enemy
      // already there takes a letter too.
      suffix = EnemyCountDecrement(classId, 1);
      if (suffix & 0x8000)
        EnemyCountClear(classId);
      BattleUnitSetFar(newUnitId, classId, suffix & 0x7fff);
      // Fill a removed enemy's place in the list, or append the newcomer.
      {
        s32 i;

        for (i = 0; i < BATTLE_ENEMY_CAPACITY; i++) {
          if (work->enemyIds[i] == BATTLE_UNIT_REMOVED) {
            work->enemyIds[i] = newUnitId;
            break;
          }
          if (work->enemyIds[i] == BATTLE_UNIT_LIST_END) {
            work->enemyIds[i] = newUnitId;
            work->enemyIds[i + 1] = BATTLE_UNIT_LIST_END;
            break;
          }
        }
      }
      // Lay the enemies out again and put the newcomer's sprite in place
      // (the anchors are 16.16 fixed point).
      EnemyUpdate();
      {
        struct BattleObj *obj;

        obj = BattleObjGet(newUnitId);
        BattleObjMake(obj, newUnitId, obj->anchorX / 0x10000,
                      obj->anchorZ / 0x10000);
      }
      BattleObjPut();
      {
        s16 enemyIds[8];
        s32 enemyCount;
        s32 i;

        enemyCount = BattleEnemyList(enemyIds);
        for (i = 0; i < enemyCount; i++)
          ObjMotRst((u16)enemyIds[i]);
      }
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, newUnitId);
      if (abilityId != ABILITY_SPEED_SURGE)
        PUSH_MESSAGE(MsgAppears);
      else
        PUSH_MESSAGE(MsgSplitOff);
    } else if (abilityId == ABILITY_SPEED_SURGE) {
      PUSH_MESSAGE(MsgSplitFail);
    } else {
      PUSH_MESSAGE(MsgNoOneCame);
    }
  }

  // Effects that change how the damage step works.
  if (hit != 0) {
    if (ability->effect == ABILITY_EFFECT_GROUND) {
      u32 i;

      // Ground holds only a target with an action in this turn's queue.
      hit = 0;
      for (i = 0; i < BATTLE_ACTION_CAPACITY; i++) {
        if (work->actions[i].unitId == targetId) {
          hit = 1;
          break;
        }
      }
    } else if (ability->effect == ABILITY_EFFECT_HALVE_DEFENSE) {
      halveDefense = 1;
    } else if (ability->effect == ABILITY_EFFECT_LEAVE_ONE_HP) {
      leaveOneHp = 1;
    } else if (ability->effect == ABILITY_EFFECT_INSTANT_DEATH) {
      skipDamage = 1;
    } else if (ability->effect == ABILITY_EFFECT_SELF_DESTRUCT) {
      // The actor leaves the battle (BattleActor_DestroyTemporaryObject).
      if (actor->hp != 0)
        BattleEvPush(BATTLE_EVENT_REMOVE_UNIT, actorId);
    } else if (ability->effect == ABILITY_EFFECT_DRAIN_PP) {
      // Psy Drain does PP damage, and misses a target with no PP.
      if (target->pp != 0)
        kind = ABILITY_KIND_PP_DAMAGE;
      else
        hit = 0;
    }
  }

  //------------------------------------------------------------------
  // Damage and healing
  //------------------------------------------------------------------
  // A downed target is skipped, unless the ability revives.
  if (skipDamage == 0 &&
      (target->hp != 0 || Battle_IsReviveEffectFar(ability->effect) != 0)) {
    s32 pp;
    s32 newHp;
    s32 power;

    switch (kind) {
    // An attack: the actor's Attack against the target's Defense.
    case ABILITY_KIND_ATTACK:
    case ABILITY_KIND_ATTACK_MULTIPLIED: {
      s32 hp;
      s32 defense;
      s32 baseDefense;
      s32 pass;

      hp = target->hp;
      baseDefense = target->def;
      defense = baseDefense;
      if (halveDefense != 0)
        defense = (u32)baseDefense / 2;
      // This loop runs once, with pass 1, so its pass-0 code never runs:
      // the element bonus always applies and the critical hit message is
      // never shown.
      for (pass = 1; pass <= 1; pass++) {
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        if (pass == 0)
          elementBonus = 0;
        power = ability->power;
        if (kind == ABILITY_KIND_ATTACK_MULTIPLIED)
          amount =
              BattleAtkCalc(actor->atk, defense, 0, elementBonus) * power / 10;
        else
          amount = BattleAtkCalc(actor->atk, defense, power, elementBonus);
        amount *= multiplier;
        // A critical hit adds a quarter (a half for an ability's), then
        // the target's level / 5 + 6.
        if (critical != CRITICAL_NONE) {
          if (critical == CRITICAL_HIT)
            amount = amount * 5 / 4;
          else
            amount = amount * 3 / 2;
          amount += (target->level / 5) + 6;
          if (pass == 0) {
            BattleEvPush(BATTLE_EVENT_MARK, 0);
            {
              s32 message;

              // "A critical hit!", or "A bitter blow!" (the next message)
              // when it lands on the party.
              message = MsgCritical;
              if (IS_PARTY_UNIT(targetId))
                message += 1;
              BattleEvPush(BATTLE_EVENT_MESSAGE_NO_WAIT, message);
            }
          }
        }
        amount += BattleRandomFar() & 3;
        REDUCE_BY_GUARD(amount, target);
        if (amount <= 0)
          amount = 1;
        if (leaveOneHp != 0) {
          if (amount < hp - 1) {
            amount = hp - 1;
            if (amount <= 0)
              amount = 1;
          }
        }
        // In the Djinn tutorial a Djinni's blow always leaves 1 HP.
        if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0 &&
            plan->command == CMD_DJINN && hp <= amount) {
          amount = hp - 1;
        }
      }
      hp -= amount;
      BattleEvPush(BATTLE_EVENT_HIT, targetId);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, amount);
      // The affinity picks "damage.", "damage!" or "damage!!!".
      PUSH_MESSAGE((IS_PARTY_UNIT(targetId) ? MsgDmgEmphP : MsgDmgEmphE) +
                   affinity);

      if (hp <= 0) {
        BattleEvPush(BATTLE_EVENT_DEFEAT, targetId);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
        hp = 0;
        PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
      } else
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, targetId);
      damageDealt = target->hp - hp;
      target->hp = hp;
      UnitRatioFar(targetId);
      break;
    }

    case ABILITY_KIND_PP_DAMAGE: {
      if (ability->power == 0)
        break;
      pp = target->pp;
      if (element != ELEMENT_NONE)
        elementBonus = elementPower - target->element[element].resistance;
      power = ability->power;
      amount = BattlePowerCalc(power, elementBonus, FULL_RATE);
      amount = amount * gPpLossTable[distance] / 100;
      amount *= multiplier;
      REDUCE_BY_GUARD(amount, target);
      // Psy Drain takes no more PP than the target has.
      if (ability->effect == ABILITY_EFFECT_DRAIN_PP && amount > pp)
        amount = pp;

      BattleEvPush(BATTLE_EVENT_HIT, targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, amount);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgPpLossP : MsgPpLossE);
      pp -= amount;
      if (pp <= 0)
        pp = 0;
      BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, targetId);
      damageDealt = target->pp - pp;
      target->pp = pp;
      UnitRatioFar(targetId);
      break;
    }

    case ABILITY_KIND_HEAL_HP: {
      if (ability->power == 0)
        break;
      newHp = target->hp;

      // An ability with no element (an item) heals as if at power 100.
      power = ability->power;
      amount = BattleRecoveryCalc(
          power, element == ELEMENT_NONE ? 100 : elementPower, FULL_RATE);
      amount = amount * gHpRecoveryTable[distance] / 100;
      amount *= multiplier;
      amount += BattleRandomFar() & 3;
      newHp += amount;
      if (newHp > target->maxHp) {
        newHp = target->maxHp;
        amount = newHp - target->hp;
      }
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      if (newHp == target->maxHp)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, amount);
        PUSH_MESSAGE(MsgHpRecover);
      }
      damageDealt = target->hp - newHp;
      target->hp = newHp;
      UnitRatioFar(targetId);
      break;
    }

    // Never reached, as the kind is 0 to 15: PP damage that, unlike
    // ABILITY_KIND_PP_DAMAGE, does not count as damage dealt.
    case -1: {
      if (ability->power == 0)
        break;
      pp = target->pp;
      if (element != ELEMENT_NONE)
        elementBonus = elementPower - target->element[element].resistance;
      power = ability->power;
      amount = BattlePowerCalc(power, elementBonus, FULL_RATE);
      amount = amount * gPpDmgTable[distance] / 100;
      amount *= multiplier;
      REDUCE_BY_GUARD(amount, target);
      BattleEvPush(BATTLE_EVENT_HIT, targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, amount);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgDmgP : MsgDmgE);
      pp -= amount;
      if (pp <= 0)
        pp = 0;
      BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, targetId);
      target->pp = pp;
      UnitRatioFar(targetId);
      break;
    }

    // HP damage from the ability's power, each kind with its own fall-off
    // by distance.
    case ABILITY_KIND_DAMAGE:
    case ABILITY_KIND_UNLEASHED_DAMAGE:
    case ABILITY_KIND_SUMMON: {
      s32 pass;

      if (ability->power == 0)
        break;
      newHp = target->hp;
      // Runs once, like the attack's loop.
      for (pass = 1; pass <= 1; pass++) {
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        if (pass == 0)
          elementBonus = 0;
        power = ability->power;
        // A summon adds 3% of the target's max HP for each Djinni it costs.
        if (plan->command == CMD_SUMMON) {
          s32 maxHpPercent;

          switch (abilityId) {
          case ABILITY_VENUS:
          case ABILITY_MERCURY:
          case ABILITY_MARS:
          case ABILITY_JUPITER:
            maxHpPercent = 3;
            break;
          case ABILITY_RAMSES:
          case ABILITY_NEREID:
          case ABILITY_KIRIN:
          case ABILITY_ATALANTA:
            maxHpPercent = 6;
            break;
          case ABILITY_CYBELE:
          case ABILITY_NEPTUNE:
          case ABILITY_TIAMAT:
          case ABILITY_PROCNE:
            maxHpPercent = 9;
            break;
          case ABILITY_JUDGMENT:
          case ABILITY_BOREAS:
          case ABILITY_METEOR:
          case ABILITY_THOR:
            maxHpPercent = 12;
            break;
          }
          power += target->maxHp * maxHpPercent / 100;
        }
        amount = BattlePowerCalc(power, elementBonus, FULL_RATE);
        amount *= multiplier;
        // The fall-off by distance. (The kind is masked again, as the bytes
        // need.)
        switch (kind & ABILITY_KIND_MASK) {
        case ABILITY_KIND_DAMAGE:
          amount = amount * gHpDmgTable5[distance] / 100;
          break;
        case ABILITY_KIND_SUMMON:
          amount = amount * gHpDmgTable8[distance] / 100;
          break;
        case ABILITY_KIND_UNLEASHED_DAMAGE:
          amount = amount * gHpDmgTable6[distance] / 100;
          break;
        }
        amount += BattleRandomFar() & 3;
        REDUCE_BY_GUARD(amount, target);
        // In the Djinn tutorial a summon always fells its target.
        if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0 &&
            plan->command == CMD_SUMMON && newHp > amount) {
          amount = newHp;
        }
      }
      BattleEvPush(BATTLE_EVENT_HIT, targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, amount);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE((IS_PARTY_UNIT(targetId) ? MsgDmgEmphP : MsgDmgEmphE) +
                   affinity);
      newHp -= amount;

      if (newHp <= 0) {
        BattleEvPush(BATTLE_EVENT_DEFEAT, targetId);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
        newHp = 0;
        PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgGoesDown : MsgFelled);
      } else
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, targetId);
      damageDealt = target->hp - newHp;
      target->hp = newHp;
      UnitRatioFar(targetId);
      break;
    }

    case ABILITY_KIND_HEAL_PP: {
      if (ability->power == 0)
        break;
      pp = target->pp;
      power = ability->power;
      amount = BattleRecoveryCalc(
          power, element == ELEMENT_NONE ? 100 : elementPower, FULL_RATE);
      amount = amount * gPpRecoveryTable[distance] / 100;
      amount *= multiplier;
      pp += amount;
      if (pp > target->maxPp) {
        pp = target->maxPp;
        amount = pp - target->pp;
      }
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      if (pp == target->maxPp)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, amount);
        PUSH_MESSAGE(MsgPpRecover);
      }
      target->pp = pp;
      UnitRatioFar(targetId);
      break;
    }

    // The kinds with no damage or healing (see enum AbilityKind).
    case 0:
    case 7:
    case 9:
      break;

    // An effect ability: its power deals damage only if the effect lands.
    case ABILITY_KIND_EFFECT:
      if (hit != 0) {
        if (ability->power == 0)
          break;
        newHp = target->hp;
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        power = ability->power;
        amount = BattlePowerCalc(power, elementBonus, FULL_RATE);
        amount *= multiplier;
        amount = amount * gHpDmgTable[distance] / 100;
        REDUCE_BY_GUARD(amount, target);
        BattleEvPush(BATTLE_EVENT_HIT, targetId);
        BattleEvPush(BATTLE_EVENT_NUMBER, amount);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgDmgP : MsgDmgE);
        newHp -= amount;

        if (newHp <= 0) {
          BattleEvPush(BATTLE_EVENT_DEFEAT, targetId);
          BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
          newHp = 0;
          PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
        } else
          BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, targetId);
        damageDealt = target->hp - newHp;
        target->hp = newHp;
        UnitRatioFar(targetId);
        break;
      }
      BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, targetId);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE(MsgNoEffect);
      break;
    }
  }

  //------------------------------------------------------------------
  // The added effect: a status, a stat change, a cure or a revival
  //------------------------------------------------------------------
  // It needs the effect to have landed on a target still standing, unless
  // it revives or drains.
  BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
  if ((Battle_IsReviveEffectFar(ability->effect) != 0 || target->hp != 0 ||
       BattleEffectDeadCheck(ability->effect) != 0) &&
      hit != 0) {
    switch (ability->effect) {
    // Tonic cures every ailment; sleep goes without a message.
    case ABILITY_EFFECT_CURE_ALL:
      if (target->delusion != 0) {
        target->delusion = 0;
        PUSH_MESSAGE(MsgCureDelusion);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureStun);
      }
      target->sleep = 0;
      if (target->psySeal != 0) {
        target->psySeal = 0;
        PUSH_MESSAGE(MsgCureSeal);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      }
      if (target->grimReaper != 0) {
        target->grimReaper = 0;
        PUSH_MESSAGE(MsgCureCurse);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      }
      if (target->haunt != 0) {
        target->haunt = 0;
        PUSH_MESSAGE(MsgCureSpirit);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      }
      if (target->poison != 0) {
        PUSH_MESSAGE(MsgCurePoison);
        target->poison = 0;
      }
      BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      break;

    // Restore cures delusion, stun, sleep, the Psynergy seal and the Grim
    // Reaper, but not poison or a haunting.
    case ABILITY_EFFECT_RESTORE:
      if (target->delusion != 0) {
        target->delusion = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureDelusion);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureStun);
      }
      target->sleep = 0;
      if (target->psySeal != 0) {
        target->psySeal = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureSeal);
      }
      if (target->grimReaper == 0)
        break;
      target->grimReaper = 0;
      BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE(MsgCureCurse);
      break;

    case ABILITY_EFFECT_HEAL_HP_60:
    case ABILITY_EFFECT_HEAL_HP_30: {
      s32 oldHp;
      s32 maxHpBits;
      s32 maxHp;
      s32 newHp;
      u16 *stat;
      s32 restored;

      /* FAKEMATCH: walking a pointer down from hp to maxHp reads each stat
       * twice, unsigned and signed, as the ROM does; plain member reads,
       * (u16)target->hp beside target->hp and the same for maxHp, share one
       * load each (differ 116 in tbs-en). */
      stat = (u16 *)&target->hp;
      oldHp = *stat--;
      newHp = *(s16 *)(stat + 1);
      stat--;
      maxHpBits = *stat--;
      maxHp = *(s16 *)(stat + 1);
      if (ability->effect == ABILITY_EFFECT_HEAL_HP_60)
        newHp += maxHp * 60 / 100;
      else
        newHp += maxHp * 30 / 100;
      if (newHp > (s16)maxHpBits)
        newHp = (s16)maxHpBits;
      restored = newHp - (s16)oldHp;
      // Nothing to show if no HP came back, unless the ability heals.
      if (restored == 0 && kind != ABILITY_KIND_HEAL_HP)
        break;
      if (newHp == (s16)maxHpBits)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, restored);
        PUSH_MESSAGE(MsgHpRecover);
      }
      target->hp = newHp;
      UnitRatioFar(targetId);
      break;
    }

    case ABILITY_EFFECT_HEAL_PP_7: {
      s32 oldPp;
      s32 maxPp;
      s32 newPp;
      s32 restored;

      newPp = target->pp;
      maxPp = target->maxPp;
      oldPp = newPp;
      newPp += maxPp * 7 / 100;
      if (newPp > maxPp)
        newPp = maxPp;
      restored = newPp - oldPp;
      if (restored == 0 && kind != ABILITY_KIND_HEAL_PP)
        break;
      if (newPp == maxPp)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, restored);
        PUSH_MESSAGE(MsgPpRecover);
      }
      target->pp = newPp;
      UnitRatioFar(targetId);
      break;
    }

    // The stat changes. The amount shown is how much the stat itself moved,
    // measured against the saved copy once UnitCalc has recalculated it.
    case ABILITY_EFFECT_AGILITY_UP:
      target->agilityStage = AGILITY_STAGE_DOUBLED;
      target->agilityTurns = AGILITY_CHANGE_TURNS;
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->agi - savedTarget->agi);
      PUSH_MESSAGE(MsgAgiUp);
      break;

    case ABILITY_EFFECT_AGILITY_DOWN:
      target->agilityStage = STAT_STAGE_MIN;
      target->agilityTurns = AGILITY_CHANGE_TURNS;
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, savedTarget->agi - target->agi);
      PUSH_MESSAGE(MsgAgiDown);
      break;

    case ABILITY_EFFECT_ATTACK_DOWN:
      target->attackStage -= 1;
      CLAMP_STAT_STAGE(target->attackStage);
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, savedTarget->atk - target->atk);
      PUSH_MESSAGE(MsgAtkDown);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_DOWN_2:
      target->attackStage -= 2;
      CLAMP_STAT_STAGE(target->attackStage);
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, savedTarget->atk - target->atk);
      PUSH_MESSAGE(MsgAtkDown);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_UP:
      target->attackStage += 1;
      CLAMP_STAT_STAGE(target->attackStage);
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->atk - savedTarget->atk);
      PUSH_MESSAGE(MsgAtkUp);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_UP_2:
      target->attackStage += 2;
      CLAMP_STAT_STAGE(target->attackStage);
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->atk - savedTarget->atk);
      PUSH_MESSAGE(MsgAtkUp);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_DOWN:
      target->defenseStage -= 1;
      CLAMP_STAT_STAGE(target->defenseStage);
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, savedTarget->def - target->def);
      PUSH_MESSAGE(MsgDefDown);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_DOWN_2:
      target->defenseStage -= 2;
      CLAMP_STAT_STAGE(target->defenseStage);
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, savedTarget->def - target->def);
      PUSH_MESSAGE(MsgDefDown);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_UP:
      target->defenseStage += 1;
      CLAMP_STAT_STAGE(target->defenseStage);
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->def - savedTarget->def);
      PUSH_MESSAGE(MsgDefUp);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_UP_2:
      target->defenseStage += 2;
      CLAMP_STAT_STAGE(target->defenseStage);
      UnitCalcFar(targetId);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->def - savedTarget->def);
      PUSH_MESSAGE(MsgDefUp);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    // The revivals bring back a downed target with all, half or 80% of its
    // HP.
    case ABILITY_EFFECT_REVIVE:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->maxHp;
      UnitRatioFar(targetId);
      break;

    case ABILITY_EFFECT_REVIVE_HALF:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->maxHp / 2;
      UnitRatioFar(targetId);
      break;

    case ABILITY_EFFECT_REVIVE_80:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->maxHp * 8 / 10;
      UnitRatioFar(targetId);
      break;

    case ABILITY_EFFECT_CURE_POISON:
      if (target->poison != 0)
        PUSH_MESSAGE(MsgCurePoison);
      target->poison = 0;
      break;

    // A Resistance change needs no recalculation to show: it is the change
    // in stages times RESISTANCE_PER_STAGE.
    case ABILITY_EFFECT_RESISTANCE_DOWN:
      target->resistanceStage -= 1;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattleEvPush(BATTLE_EVENT_NUMBER,
                   (savedTarget->resistanceStage - target->resistanceStage) *
                       RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResDown);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_RESISTANCE_DOWN_2:
      target->resistanceStage -= 2;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattleEvPush(BATTLE_EVENT_NUMBER,
                   (savedTarget->resistanceStage - target->resistanceStage) *
                       RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResDown);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_RESISTANCE_UP:
      target->resistanceStage += 1;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattleEvPush(BATTLE_EVENT_NUMBER,
                   (target->resistanceStage - savedTarget->resistanceStage) *
                       RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResUp);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_RESISTANCE_UP_2:
      target->resistanceStage += 2;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattleEvPush(BATTLE_EVENT_NUMBER,
                   (target->resistanceStage - savedTarget->resistanceStage) *
                       RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResUp);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    // Poison does nothing to a poisoned target, deadly poison nothing to one
    // that already has it.
    case ABILITY_EFFECT_POISON:
      if (target->poison != 0)
        break;
      PUSH_MESSAGE(MsgPoisoned);
      target->poison = POISON_NORMAL;
      break;

    case ABILITY_EFFECT_DEADLY_POISON:
      if (target->poison > POISON_NORMAL)
        break;
      PUSH_MESSAGE(MsgVenom);
      target->poison = POISON_DEADLY;
      break;

    case ABILITY_EFFECT_DELUSION:
      PUSH_MESSAGE(MsgDelusion);
      target->delusion = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_CONFUSION:
      PUSH_MESSAGE(MsgConfused);
      target->confusion = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_CHARM: // "X's heart has been stolen!"
      PUSH_MESSAGE(MsgCharmed);
      target->charm = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_STUN:
      PUSH_MESSAGE(MsgStunned);
      target->stun = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_SLEEP:
      PUSH_MESSAGE(MsgAsleep);
      target->sleep = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_SEAL:
      PUSH_SIDE_MESSAGE(targetId, MsgPsyBlock, MsgPsySeal);
      target->psySeal |= PSY_SEAL_BIND;
      break;

    case ABILITY_EFFECT_SEAL_2_TURNS:
      PUSH_SIDE_MESSAGE(targetId, MsgPsyBlock, MsgPsySeal);
      target->psySeal |= PSY_SEAL_LUFF;
      break;

    // Instant death: "X goes down!" on the party, Drown's "X suffocates!"
    // or "X's spirit has been drained!" on an enemy.
    case ABILITY_EFFECT_INSTANT_DEATH:
      BattleEvPush(BATTLE_EVENT_DEFEAT, targetId);
      if (target->side == BATTLE_UNIT_PARTY)
        PUSH_MESSAGE(MsgKoDown);
      else if (abilityId == ABILITY_DROWN)
        PUSH_MESSAGE(MsgSuffocate);
      else
        PUSH_MESSAGE(MsgSpiritDrain);
      target->hp = 0;
      UnitRatioFar(targetId);
      break;

    case ABILITY_EFFECT_REGENERATE:
      PUSH_MESSAGE(MsgRefrain);
      target->refrain = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_REFLECT:
      PUSH_MESSAGE(MsgReflect);
      target->reflect = STATUS_TURNS;
      break;

    // The drains give the damage dealt back to the actor: Drain all of it
    // as HP, Sap half.
    case ABILITY_EFFECT_DRAIN_HP:
    case ABILITY_EFFECT_DRAIN_HALF_HP: {
      s32 newHp;

      newHp = actor->hp;
      amount = damageDealt;
      if (ability->effect == ABILITY_EFFECT_DRAIN_HALF_HP)
        amount /= 2;
      newHp += amount;
      if (newHp > actor->maxHp) {
        newHp = actor->maxHp;
        amount = newHp - actor->hp;
      }
      BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, actorId);
      if (newHp == actor->maxHp)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, amount);
        PUSH_MESSAGE(MsgHpRecover);
      }
      actor->hp = newHp;
      UnitRatioFar(actorId);
      break;
    }

    // Psy Drain gives the PP taken.
    case ABILITY_EFFECT_DRAIN_PP: {
      s32 newPp;
      s32 drained;

      newPp = actor->pp;
      drained = damageDealt;
      newPp += drained;
      if (newPp > actor->maxPp) {
        newPp = actor->maxPp;
        drained = newPp - actor->pp;
      }
      BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, actorId);
      if (newPp == actor->maxPp)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, drained);
        PUSH_MESSAGE(MsgPpRecover);
      }
      actor->pp = newPp;
      UnitRatioFar(actorId);
      break;
    }

    // Psy Leech: a tenth of the damage as PP, as far as the target has it
    // and the actor has room.
    case ABILITY_EFFECT_PSY_LEECH:
      amount = damageDealt / 10;
      if (target->pp < amount)
        amount = target->pp;
      if (actor->pp + amount > actor->maxPp)
        amount = actor->maxPp - actor->pp;
      if (amount == 0)
        break;
      BattleEvPush(BATTLE_EVENT_NUMBER, amount);
      PUSH_SIDE_MESSAGE(targetId, MsgLeechTake, MsgLeechGain);
      Unit_AddPpFar(actorId, amount);
      break;

    // Break undoes every raised stat and the element stages.
    case ABILITY_EFFECT_BREAK:
      if (target->attackStage > 0) {
        target->attackStage = 0;
        target->attackTurns = 0;
      }
      if (target->defenseStage > 0) {
        target->defenseStage = 0;
        target->defenseTurns = 0;
      }
      if (target->resistanceStage > 0) {
        target->resistanceStage = 0;
        target->resistanceTurns = 0;
      }
      if (target->agilityStage > 0)
        target->agilityStage = 0;
      target->elementLevels[ELEMENT_VENUS] = 0;
      target->elementLevels[ELEMENT_MERCURY] = 0;
      target->elementLevels[ELEMENT_MARS] = 0;
      target->elementLevels[ELEMENT_JUPITER] = 0;
      PUSH_MESSAGE(MsgBuffsReset);
      break;

    case ABILITY_EFFECT_HAUNT:
      PUSH_MESSAGE(MsgEvilSpirit);
      target->haunt = 1;
      break;

    // The Grim Reaper counts down to the target's death; each later curse
    // brings it a turn closer.
    case ABILITY_EFFECT_CURSE:
      if (target->grimReaper == 0) {
        PUSH_MESSAGE(MsgDeathCurse);
        target->grimReaper = CURSE_TURNS;
        break;
      }
      if (target->grimReaper <= 1)
        break;
      target->grimReaper -= 1;
      BattleEvPush(BATTLE_EVENT_NUMBER, target->grimReaper);
      PUSH_MESSAGE(MsgDeathCount);
      break;

    // Kite: the target acts twice in the next turn (the count drops at each
    // turn's end).
    case ABILITY_EFFECT_ACT_TWICE:
      PUSH_MESSAGE(MsgReadies);
      target->ready = 2;
      break;

    // The target rises to the challenge and won't flee; on the party, no one
    // may run ("No time to run!").
    case ABILITY_EFFECT_CHALLENGE:
      PUSH_MESSAGE(MsgChallenge);
      target->challenge = 1;
      if (IS_PARTY_UNIT(targetId))
        work->challengeFlag |= 2;
      break;

    // Ground: the target loses its next action.
    case ABILITY_EFFECT_GROUND:
      PUSH_MESSAGE(MsgImmobile);
      target->immobile = 1;
      break;

    case ABILITY_EFFECT_DEFEND:
      PUSH_MESSAGE(MsgAura);
      if (target->guardLevel > 0)
        break;
      target->guardLevel = GUARD_DEFEND;
      break;

    case ABILITY_EFFECT_TOTAL_DEFENSE:
      PUSH_MESSAGE(MsgAura2);
      if (target->guardLevel > GUARD_DEFEND)
        break;
      target->guardLevel = GUARD_TOTAL_DEFENSE;
      break;

    // Contain, Threaten and Tremble do nothing; the player only reads their
    // message.
    case ABILITY_EFFECT_THREATEN:
      PUSH_MESSAGE(MESSAGE_NONE);
      break;

    default:
      break;
    }
  }

  //------------------------------------------------------------------
  // Waking from damage, and the recalculated stats
  //------------------------------------------------------------------
  BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);

  // Damage wakes a sleeping target one time in four, unless it fell asleep
  // this turn (its count still at STATUS_TURNS).
  if (target->hp != 0 && target->sleep != 0 && target->sleep < STATUS_TURNS &&
      damageDealt > 0 && (BattleRandomFar() & 3) == 0) {
    target->sleep = 0;
    BattleEvPush(BATTLE_EVENT_UNIT_NAME, targetId);
    PUSH_MESSAGE(MsgWakes);
  }
  MemFree(savedTarget);
  UnitCalcFar(targetId);
  PartyStatusDrawFar(gBattleWork->partyStatus);
  if (target->hp != 0)
    BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, targetId);
  // A haunted actor: the evil spirit may hurt it for a quarter of the
  // damage it dealt, at the end of its turn (BattleUnit_ProcessTurnEnd).
  if (actor->haunt != 0 && (BattleRandomFar() & 3) == 0 && damageDealt > 0) {
    s32 share;

    share = damageDealt >> 2;
    if (share == 0)
      share = 1;
    plan->hauntDamage += share;
  }
}

#define ABILITY_PSY_DRAIN 126

//--------------------------------------------------------------------------
// check_status_or_special_id.c
// Battle: whether an ability is Psynergy
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Returns 1 for an ability that costs PP, and for Psy Drain, which costs
// none.
// abilityId : the ability
//--------------------------------------------------------------------------
u32 Ability_CheckStatusOrSpecialId(s32 abilityId) {
  u8 pp;

  if (abilityId == ABILITY_PSY_DRAIN)
    return 1;
  pp = BattleActGet(abilityId)->ppCost;
  return (u32)((0 - pp) | pp) >> 31; // 1 if pp is not 0
}

//--------------------------------------------------------------------------
// cmdplay.c
// Battle: choosing the enemies' actions, and playing the battle events
//--------------------------------------------------------------------------
#define ABILITY_ATTACK 1 // the ability "Attack"
#define ALLY_MAX 7       // the last party member's unit id

// An enemy's memory of its action choice, kept in BattleUnit.rewards.
struct BattleAutomaticState {
  unsigned initialized : 1;
  unsigned selection : 3; // the place in its action list
  unsigned remaining : 28;
};

//--------------------------------------------------------------------------
// Chooses an enemy's action (a party member's is chosen in the menu): one of
// the eight in its definition, picked by its action strategy, with an item
// instead where its item mask says so, and a target worth the ability. A
// choice that cannot be made (too little PP, sealed, a forbidden escape, no
// worthwhile target) picks again; after 18 tries the enemy defends.
// retry : 0 when the round is built: only a guard is settled now (it must
//         raise before anyone acts); 1 at the enemy's turn: the rest.
//--------------------------------------------------------------------------
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *req, s32 retry) {
  u32 selection;
  struct BattleAutomaticState *memo;
  struct EnemyDefinition *enemy;
  s32 itemOk;
  s32 tries;
  s32 keepGoing;
  s8 *strategy;
  u8 *itemMask;
  s16 command;
  s32 target;
  s32 target2;
  s32 side;
  s32 side2;
  struct ItemDefinition *item;
  s32 useItem;
  s8 mode;
  struct BattleAct *itemAct;
  struct BattleAct *act;
  struct BattleUnit *unit;
  s32 abilityId;
  s32 flags;
  u8 effect;
  u8 targetMode;

  // Picks a place in the action list from a 0-255 roll and eight weights.
  s32 WeightRandom(u8 *weights) {
    s32 random;
    s32 total;
    s32 no;
    s32 n;

    random = BattleRandomFar() & 0xff;
    total = weights[0];
    no = 0;
    n = 0;
    if (random >= total) {
      while (++n <= 7) {
        total += weights[n];
        if (random < total) {
          no = n;
          break;
        }
      }
    }
    return no;
  }

  unit = UnitGetFar(req->unitId);
  keepGoing = itemOk = 1;
  tries = 0;
  selection = -1U;
  if (unit->classIndex != 0) // a party member chooses in the menu
    return;
  if (retry != 0 && req->command != CMD_ABILITY)
    return;

  enemy = Enemy_GetDefinitionFar(unit->classId);
  memo = (struct BattleAutomaticState *)&unit->rewards;
  strategy = &enemy->actionStrategy;
  itemMask = &enemy->itemActions;

  // A place in the action list, by the enemy's strategy.
select_again:
  mode = *strategy;
  switch (mode) {
  case 0: // the first weight table
    selection = WeightRandom(gBattleCommand_SelectionWeights[0]);
    break;
  case 1:
    selection = WeightRandom(gBattleCommand_SelectionWeights[1]);
    break;
  case 2:
    selection = WeightRandom(gBattleCommand_SelectionWeights[2]);
    break;
  case 3: // from a random place, then in order
    if (memo->initialized == 0) {
      memo->selection = BattleRandomFar();
      memo->initialized = 1;
    }
    // fall through
  case 4: // in order from the remembered place
    selection = memo->selection;
    if (retry != 0) {
      memo->selection = selection + 1;
    }
    break;
  case 5: // the next place at each new pick
    selection++;
    break;
  case 6: // the same place
    break;
  }

  // A place marked for an item: use the first item held, if any.
  useItem = (*itemMask >> selection) & 1;
  useItem &= itemOk;
  abilityId = enemy->actions[selection];
  req->command = CMD_ABILITY;
  if (useItem != 0 && retry != 0) {
    if (!(0x1ff & unit->inventory[0])) {
      useItem = 0;
      if (enemy->targetStrategy == 0) {
        req->command = CMD_ITEM;
        req->parameter = 0x1fd;
        return;
      }
    }
    if (useItem != 0) {
      item = Item_GetDirectFar(unit->inventory[0]);
      if (item->useType == 1) {
        itemAct = BattleActGet(item->actionId);
        abilityId = item->actionId;
        req->command = CMD_ITEM;
        flags = itemAct->flags;
        req->parameter = 0;
        switch (flags) {
        case 1:
        case 2:
          break;
        default:
          useItem = 0;
          break;
        }
      } else {
        useItem = 0;
      }
    }
    if (useItem == 0)
      itemOk = 0;
  }

  if (keepGoing != 0) {
    // A guard or an escape targets the user itself.
    act = BattleActGet(abilityId);
    effect = act->effect;
    switch (effect) {
    case 46:
      req->command = CMD_DEFEND;
      req->target = Battle_FindTaggedSlotByValue(req->unitId);
      break;
    case 47:
      req->command = CMD_DEFEND2;
      req->target = Battle_FindTaggedSlotByValue(req->unitId);
      break;
    case 49:
      req->command = CMD_FLEE;
      req->target = Battle_FindTaggedSlotByValue(req->unitId);
      break;
    }

    if (retry == 0) {
      switch (req->command) {
      case CMD_DEFEND:
      case CMD_DEFEND2:
        break;
      default:
        goto done;
      }
    }

    // Psynergy: a careful enemy (strategy not 0) avoids it without the PP,
    // and the most careful (2) also while sealed.
    if (req->command != CMD_ITEM &&
        Ability_CheckStatusOrSpecialId(abilityId) != 0) {
      req->command = CMD_PSYNERGY;
      req->parameter = abilityId;
      if (act->ppCost > unit->pp && enemy->targetStrategy != 0)
        goto next_candidate;
      if (unit->psySeal != 0 && enemy->targetStrategy == 2)
        goto next_candidate;
    }
    command = req->command;
    if (command == CMD_FLEE && (u8)unit->challenge != 0)
      goto next_candidate;
    if (retry != 0 && (command == CMD_DEFEND || command == CMD_DEFEND2))
      goto next_candidate;

    if (command == CMD_ABILITY) {
      req->parameter = abilityId;
      if (abilityId == ABILITY_ATTACK)
        req->command = CMD_ATTACK;
    }

    // The target.
    req->range = act->range;
    targetMode = act->targetMode;
    switch (targetMode) {
    case 2:
    case 4:
      target = BattleTarget_SelectForAction(req->unitId, act);
      if (target == -2) {
        side = 0;
        if (req->unitId <= (u32)ALLY_MAX)
          side = 1;
        target = BattleTarget_SelectRandomPosition(side);
      }
      if (target != -1) {
        req->target = target;
        keepGoing = 0;
      }
      break;
    case 1:
      target2 = BattleTarget_SelectForAction(req->unitId, act);
      if (target2 == -2) {
        side2 = 0;
        if (req->unitId <= (u32)ALLY_MAX)
          side2 = 1;
        target2 = BattleTarget_SelectRandomPosition(side2);
      }
      if (target2 != -1) {
        req->target = target2;
        keepGoing = 0;
      }
      break;
    case 3:
      req->target = Battle_FindTaggedSlotByValue(req->unitId);
      break;
    default:
      target2 = Battle_FindTaggedSlotByValue(req->unitId);
      req->target = target2;
      keepGoing = 0;
      break;
    }
  }
  if (retry == 0)
    keepGoing = 0;
next_candidate:
  // After 18 failed picks, defend.
  if (keepGoing != 0 && tries > 16) {
    req->command = CMD_DEFEND;
    keepGoing = 0;
  }
  tries++;
  if (keepGoing != 0)
    goto select_again;
done:
  return;
}

//--------------------------------------------------------------------------
// Sets DMA3 three times over, stopping it, then calls the IRQ return
// instruction's word as a routine.
//--------------------------------------------------------------------------
void Dma_StopAllThenRunHook(void) {
  Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
  Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
  Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
  _call_via_r0(IwramIrqModeWord);
}

//--------------------------------------------------------------------------
// Resumes paused playback from target processing.
// cue: hit sound effect; zero leaves it unchanged.
//--------------------------------------------------------------------------
void BattleEventRuntime_BeginPhase(s32 cue) {
  struct BattleEventState *ev;

  ev = &gBattleWork->events;
  if (ev->phase == BATTLE_PLAYBACK_IDLE) {
    ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    if (cue != 0) {
      ev->pendingCue = cue;
    }
  }
}

//--------------------------------------------------------------------------
// Clears the event sequence and schedules playback.
// frames: delay before the first event.
//--------------------------------------------------------------------------
s32 BattleEventRuntime_SchedulePhase(s32 frames) {
  struct BattleEventState *ev;

  ev = &gBattleWork->events;
  ev->queue.count = 0;
  ev->eventIndex = 0;
  ev->timer = frames;
  ev->phase = BATTLE_PLAYBACK_DISPATCH;
  gBattleWork->plan.targetCount = 0;
  return Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
}

//--------------------------------------------------------------------------
// Battle event playback.
//--------------------------------------------------------------------------
#define PLAY_MESSAGE 3      // Wait for message display.
#define PLAY_ADVANCE 5      // Wait for the advance key.
#define PLAY_HIT 10         // Flash the unit that was hit.
#define PLAY_FELLED 11      // Remove a defeated enemy.
#define PLAY_MESSAGE_ADD 13 // Continue the message without waiting for a key.

#define POSE_DAMAGE 5 // Pose when taking damage.
#define PAL_HIT 7     // Flash colour.

#define FELLED_FLASH 0x400 // Timer base while changing colour.

#define OBJ_VRAM 0x06010000

//--------------------------------------------------------------------------
// Scheduler callback that runs BattleTargetExec's queued events in order.
//--------------------------------------------------------------------------
void BattleEvent_Playback(void) {
  u16 selection[8];
  struct AnimationObject *recs[4];
  struct BattleWork *work = gBattleWork;
  struct BattleEventState *ev = &work->events;

  // Clear the character's tiles.
  void ClrRecTiles(struct AnimationObject * rec, s32 value) {
    u8 *tiles = (u8 *)OBJ_VRAM + gVramBlockCache[rec->slot].offset;

    Iwram_ClearWords(tiles, rec->width * rec->height);
  }

  if (work->events.phase == BATTLE_PLAYBACK_IDLE)
    return;
  for (;;) {
    if (ev->phase == BATTLE_PLAYBACK_READY)
      return;
    if (ev->phase == BATTLE_PLAYBACK_RESOLVE_TARGET) {
      // Calculate effects for the next target and enqueue its events.
      if (ev->queue.targetIndex < work->plan.targetCount) {
        ev->queue.count = 0;
        ev->eventIndex = 0;
        ev->timer = 0;
        BattleTargetExec(&work->plan, ev->queue.targetIndex);
        ev->queue.targetIndex++;
        ev->phase = BATTLE_PLAYBACK_DISPATCH;
      } else {
        ev->phase = BATTLE_PLAYBACK_READY;
      }
    } else if (ev->phase == BATTLE_PLAYBACK_DISPATCH) {
      // Run the queued events in order.
      while (ev->eventIndex < ev->queue.count) {
        {
          s32 i = ev->eventIndex;

          if (ev->phase != BATTLE_PLAYBACK_DISPATCH)
            break;
          if (ev->timer != 0) {
            ev->timer--;
            return;
          }
          switch (ev->queue.opcodes[i]) {
          case BATTLE_EVENT_SOUND: // Sound effect.
            AudioCommand_PlayFar(ev->queue.operands[i]);
            break;
          case BATTLE_EVENT_DJINN_BONUS:
            Battle_SetRuntimeFlagBit0(ev, ev->queue.operands[i]);
            break;
          case BATTLE_EVENT_UNIT_NAME: // Name inserted into the message.
            UiWork_PushValueSlotFar(ev->queue.operands[i], VALUE_UNIT_NAME);
            break;
          case BATTLE_EVENT_NUMBER: // Number inserted into the message.
            UiWork_PushValueSlotFar(ev->queue.operands[i], VALUE_NUMBER);
            break;
          case BATTLE_EVENT_ITEM_NAME: // Item name inserted into the message.
            UiWork_PushValueSlotFar(ev->queue.operands[i] & 0x1ff,
                                    VALUE_ITEM_NAME);
            break;
          case BATTLE_EVENT_ABILITY_NAME: // Ability name inserted into the
                                          // message.
            UiWork_PushValueSlotFar(ev->queue.operands[i] & 0x3fff,
                                    VALUE_ABILITY_NAME);
            break;
          case BATTLE_EVENT_MARK:
            gBattleDisplayWork->marked = 1;
            break;
          case BATTLE_EVENT_MESSAGE: // Show the message and wait for advance.
            if ((s32)ev->queue.operands[i] >= 0)
              UiText_PrepareMessageWorkFar(ev->queue.operands[i]);
            ev->phase = PLAY_MESSAGE;
            gKeysPressedLatch = 0;
            break;
          case BATTLE_EVENT_MESSAGE_NO_WAIT: // Show without waiting for
                                             // advance.
            if ((s32)ev->queue.operands[i] >= 0)
              UiText_PrepareMessageWorkFar(ev->queue.operands[i]);
            ev->phase = PLAY_MESSAGE_ADD;
            break;
          case BATTLE_EVENT_CLEAR_ARGUMENTS: // Clear message arguments.
            UiWork_ClearValueNameTablesFar();
            break;
          case BATTLE_EVENT_REMOVE_UNIT:
            BattleActor_DestroyTemporaryObject(ev->queue.operands[i]);
            break;
          case BATTLE_EVENT_HIT: // Hit.
            if (ev->pendingCue > 0)
              AudioCommand_PlayFar(ev->pendingCue);
            ev->actorId = ev->queue.operands[i];
            ObjectDispatch_ApplyArgumentToChildrenFar(
                BattleObjGet(ev->actorId)->object, POSE_DAMAGE);
            ev->phase = PLAY_HIT;
            ev->timer = 0;
            break;
          case BATTLE_EVENT_DEFEAT: // Defeated.
          {
            struct BattleUnit *unit;
            struct AnimationObject *rec;
            s32 n;

            ev->actorId = ev->queue.operands[i];
            BattleEnemy_RecordDefeat(ev->actorId, ev->flags);
            BattleActor_ResetRuntimeFields(ev->actorId);
            unit = UnitGetFar(ev->actorId);
            for (n = 0; (rec = GetMotionRecord(
                             BattleObjGet(ev->actorId)->object, n)) != 0;
                 n++) {
              if (unit->side != BATTLE_UNIT_ENEMY)
                AnimationObjects_SelectAnimationFar(rec, 4);
              else
                AnimationObjects_SelectAnimationFar(rec, 5);
            }
            if (unit->side == BATTLE_UNIT_ENEMY) {
              ev->phase = PLAY_FELLED;
              ev->timer = 0;
            }
            break;
          }
          case BATTLE_EVENT_DRAW_STATUS: // Redraw party status.
            PartyStatusDrawFar(gBattleWork->partyStatus);
            break;
          case BATTLE_EVENT_REFRESH_UNIT: // Update status icons, colour and
                                          // pose.
          {
            struct BattleObj *slot;

            BattleUnit_BuildStatusFlags(ev->queue.operands[i],
                                        BattleObjGet(ev->queue.operands[i]));
            slot = BattleObjGet(ev->queue.operands[i]);
            BattleMotion_SetRecordChildValues(
                slot->object,
                BattleMotion_GetSlotPalette(ev->queue.operands[i]));
            BattlePres_SetActorModeAndAction(ev->queue.operands[i]);
            break;
          }
          }
          ev->eventIndex++;
        }
      }
      if (ev->phase == BATTLE_PLAYBACK_DISPATCH)
        ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    } else if (ev->phase == PLAY_MESSAGE || ev->phase == PLAY_MESSAGE_ADD) {
      // Wait until the message finishes appearing.
      if (UiWork_IsCompleteFar() == 0)
        return;
      if (ev->phase == PLAY_MESSAGE_ADD) {
        ev->phase = BATTLE_PLAYBACK_DISPATCH;
        ev->timer = 0;
      } else {
        ev->phase = PLAY_ADVANCE;
        ev->displaySource = -1;
        ev->timer = gFrameTick;
      }
    } else if (ev->phase == PLAY_ADVANCE) {
      // Show the arrow and wait for advance. Holding B or pressing an advance
      // key proceeds; after ten ticks, holding an advance key also proceeds.
      struct BattlePromptSprite *spr = &ev->displayEntry;
      u8 *arrow = gBattlePres_AdvanceArrowTiles;
      s32 pat = (s32)arrow + ((gFrameCount >> 2) & 7) * 128;
      struct BattleDisplayWork *disp = gBattleDisplayWork;
      struct UiWindow *window = disp->window;
      struct BattleDisplayOffset *offset = disp->offset;
      s32 x = 0;

      if (ev->displaySource == -1)
        ev->displaySource = work->promptSlot;
      UiWork_ClearValueNameTablesFar();
      QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
      QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
      ((u32 *)spr)[1] = 0xa000;
      ((u32 *)spr)[2] = x;
      spr->tile = VramCache_Reload(ev->displaySource, pat);
      x = window->x * 8 + (offset->x >> 8) + 4;
      spr->x = x;
      spr->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
               (offset->y >> 8) + 6;
      if ((BATTLE_OPTIONS & KEY_B) || (gKeysPressedLatch & KEY_ADVANCE) ||
          ((u32)(gFrameTick - ev->timer) > 10 &&
           (BATTLE_OPTIONS & KEY_ADVANCE))) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        ev->phase = BATTLE_PLAYBACK_DISPATCH;
        ev->timer = 0;
      } else {
        Runtime_PushSlotEntry(spr, 240);
        return;
      }
    } else if (ev->phase == PLAY_HIT) {
      // Change colour every two frames and restore it after nine frames.
      if (ev->timer & 1) {
        if (ev->timer & 2) {
          struct BattleObj *slot;

          selection[0] = 0xff;
          slot = BattleObjGet(ev->actorId);
          BattleMotion_SetRecordChildValues(
              slot->object, BattleMotion_GetSlotPalette(ev->actorId));
        } else {
          selection[0] = ev->actorId;
          selection[1] = 0xff;
          BattleMotion_SetRecordChildValues(BattleObjGet(ev->actorId)->object,
                                            PAL_HIT);
        }
        BattleLayout_HighlightPartyPanelsFar(selection);
      }
      ev->timer++;
      if (ev->timer <= 8)
        return;
      ev->phase = BATTLE_PLAYBACK_DISPATCH;
      ev->timer = 0;
    } else if (ev->phase == PLAY_FELLED) {
      // With flags set, first change colour for 30 frames.
      if (ev->timer == 0 || ev->timer >= FELLED_FLASH) {
        s32 frame = 6;

        if (ev->timer == 0 && ev->flags != 0) {
          s32 kind = Summon_GetLayoutKind(UnitGetFar(ev->actorId)->classId);

          if (kind >= 0) {
            kind--;
            if (kind < 0)
              kind = 0;
            AudioCommand_PlayFar(kind + 146);
          }
          ev->timer = FELLED_FLASH;
        }
        if (ev->timer > FELLED_FLASH + 29)
          ev->timer = 0;
        if (ev->timer == 0) {
          s32 kind = Summon_GetLayoutKind(UnitGetFar(ev->actorId)->classId);

          if (kind >= 0)
            AudioCommand_PlayFar(kind + 146);
        }
        if (ev->timer >= FELLED_FLASH)
          frame = (ev->timer - FELLED_FLASH) / 8 % 5 + 1;
        if (frame == 6 || (ev->timer & 7) == 0) {
          struct AnimationObject *rec;
          s32 n;
          struct AnimationObject **slot;
          s32 none;

          for (n = 0, slot = recs, none = 0xff;
               (rec = GetMotionRecord(BattleObjGet(ev->actorId)->object, n)) !=
               0;
               n++) {
            struct AnimationEntry *child;
            u8 old;

            *slot++ = rec;
            child = rec->entries[0];
            old = child->frame;
            child->param = frame;
            child->frame = old | none;
          }
        }
        ev->timer++;
        return;
      }
      if (ev->timer == 4) {
        BattleActor_RemoveFromLists(ev->actorId);
        ev->timer++;
        return;
      }
      if (ev->timer > 4) {
        // Remove the image by replacing its tiles in sequence.
        struct BattleObj *slot;
        struct AnimationObject *rec;
        s32 count;
        s32 frame;
        s32 base;

        slot = BattleObjGet(ev->actorId);
        slot->fading = 1;
        for (count = 0; (rec = GetMotionRecord(slot->object, count)) != 0;
             count++)
          recs[count] = rec;
        base = ev->timer * 4;
        frame = base - 20;
        if (frame > 127) {
          s32 k;

          for (k = 0; k < count; k++)
            ClrRecTiles(recs[k], 0);
          ActivateBattleObjectSlot(ev->actorId);
          ev->phase = BATTLE_PLAYBACK_DISPATCH;
          ev->timer = 0;
          return;
        }
        {
          s32 k;

          for (k = 0; k < count; k++) {
            render_animated_tile_frameFar((u8 *)recs[k], frame);
            render_animated_tile_frameFar((u8 *)recs[k], base - 19);
            render_animated_tile_frameFar((u8 *)recs[k], base - 18);
            render_animated_tile_frameFar((u8 *)recs[k], base - 17);
          }
        }
        ev->timer++;
        return;
      }
      ev->timer++;
      return;
    }
  }
}

u32 BattleEventRuntime_Reset(void) {
  struct BattleEventState *events;

  events = &gBattleWork->events;
  events->phase = BATTLE_PLAYBACK_IDLE;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = 0;
  events->queue.targetIndex = 0;
  events->pendingCue = 0x86;
  events->flags = 0;
}

s32 BattleEventRuntime_WaitForReady(void) {
  s32 state;
  struct BattleEventState *events;

  events = &gBattleWork->events;
  state = events->phase;
  if (state == BATTLE_PLAYBACK_IDLE) {
    events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    state = BATTLE_PLAYBACK_RESOLVE_TARGET;
  }
  if (state != BATTLE_PLAYBACK_READY) {
    do {
      WaitFrames(1U);
    } while (events->phase != BATTLE_PLAYBACK_READY);
  }
  Scheduler_RemoveCallback(BattleEvent_Playback);
  return BattleEventRuntime_Reset();
}

//--------------------------------------------------------------------------
// is_unit_listed.c
// Battle: checks whether a unit is in the active formation.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Returns one when the unit is in its side's formation.
// id: unit id; zero through seven are party members, the rest are enemies.
//--------------------------------------------------------------------------
u32 BattleParty_IsUnitListed(u32 id) {
  u16 list[8];
  s32 side;
  s32 cnt;
  s32 i;

  side = BATTLE_SIDE_PARTY;
  if (id > 7)
    side = BATTLE_SIDE_ENEMIES;
  cnt = BattleParty_ListActorIds(side, list);

  for (i = 0; i < cnt; i++) {
    if (list[i] == id)
      break;
  }

  return i != cnt;
}

s32 BattlePlacement_CountValidEntries(u32 unitId, u8 *counts) {
  u16 unitIds[8];
  struct DjinnRecoveryList *list;
  s32 total;
  s32 count;
  s32 i;
  s32 j;
  s32 sideMask;
  s32 side;

  count = 0;
  sideMask = BATTLE_SIDE_PARTY;
  if (unitId > 7)
    sideMask = BATTLE_SIDE_ENEMIES;
  total = BattleParty_ListActorIds(sideMask, unitIds);
  side = 0;
  if (unitId > 7)
    side = 1;
  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
  if (counts != 0)
    for (j = 3; j >= 0; j--)
      counts[j] = 0;
  i = 0;
  if (list->count != 0) {
    do {
      if (list->entries[i].turns == -1) {
        for (j = 0; j < total; j++)
          if (unitIds[j] == list->entries[i].unitId)
            break;
        if (j != total) {
          if (counts != 0)
            counts[list->entries[i].element]++;
          count++;
        }
      }
      i++;
    } while (i != list->count);
  }
  return count;
}

//--------------------------------------------------------------------------
// command.c
// Battle: announces actions and plans targets and effects.
//--------------------------------------------------------------------------
// Djinni selector: element in bits 8..11, index in bits 0..7.
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)
#define TARGET_MAX 14

//--------------------------------------------------------------------------
// Announces the action and plans its targets and effects.
// Returns zero to execute, -1 on failure (already reported), or -2 for no
// action.
//--------------------------------------------------------------------------
s32 BattleCommand_BuildPlan(struct BattleActionRecord *req,
                            struct BattlePlan *plan) {
  struct BattleUnit *unit = UnitGetFar(req->unitId);
  struct BattleWork *work = gBattleWork;
  s32 tid;
  s32 abilityId;

  //----------------------------------------------------------------------
  // List targets within the ability's range; defend and return -1 if none
  // exist.
  //----------------------------------------------------------------------
  s32 TargetSet(s32 no) {
    struct BattleAct *act = BattleActGet(no);
    s32 mode = act->targetMode;
    s32 deadOk = 0;
    s32 cnt;
    s32 partyCount;
    s32 enemyCount;
    s32 center;
    s32 range;
    s32 pos;
    s32 id;

    switch (act->effect) {
    case 5: // Revive.
    case 56:
    case 57:
      deadOk = 1;
      break;
    }
    switch (mode) {
    case 0: // Single target.
      plan->targetOffset[0] = mode;
      plan->targetCount = 1;
      plan->targetIds[0] = tid;
      plan->targetMultiplier[0] = 1;
      break;
    case 4:
      plan->targetOffset[0] = 0;
      plan->targetCount = 1;
      plan->targetIds[0] = tid;
      plan->targetMultiplier[0] = 1;
      break;
    default:
      // Formation positions through range - 1 from the selected position.
      cnt = 0;
      for (pos = 0; work->partyUnits[pos] != BATTLE_UNIT_LIST_END; pos++)
        ;
      partyCount = pos;
      for (pos = 0; work->enemyIds[pos] != BATTLE_UNIT_LIST_END; pos++)
        ;
      enemyCount = pos;
      center = req->target & 15;
      range = req->range;
      {
        s32 first = center - range + 1;
        s32 last = center + range - 1;

        for (pos = first; pos <= last; pos++) {
          if (pos < 0)
            continue;
          if (req->target & 128) // Enemy side.
          {
            if (pos >= enemyCount)
              continue;
            id = work->enemyIds[pos];
            if (id == BATTLE_UNIT_REMOVED)
              continue;
            if (!deadOk && UnitGetFar(id)->hp == 0)
              continue;
            plan->targetMultiplier[cnt] = 1;
            plan->targetOffset[cnt] = pos - center;
            plan->targetIds[cnt] = id;
            cnt++;
          } else {
            if (pos >= partyCount)
              continue;
            id = work->partyUnits[pos];
            if (id == BATTLE_UNIT_REMOVED)
              continue;
            if (!deadOk && UnitGetFar(id)->hp == 0)
              continue;
            plan->targetMultiplier[cnt] = 1;
            plan->targetOffset[cnt] = pos - center;
            plan->targetIds[cnt] = id;
            cnt++;
          }
        }
      }
      plan->targetCount = cnt;
      if (cnt <= 0) {
        UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
        UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
        if (unit->guardLevel == 0)
          unit->guardLevel = 1;
        return -1;
      }
    }
  }

  tid = Battle_GetTaggedSlotValue(req->target);
  BattleEventRuntime_Reset();
  plan->hauntDamage = 0;
  plan->actorId = req->unitId;
  plan->targetCount = 0;
  plan->presentationFlags = 0;
  plan->failure = 0;
  plan->element = 4;
  UiWork_ClearValueNameTablesFar();
  if (unit->hp == 0)
    return -2;

  // Debug: hold R to defeat all enemies, or R+Select to defeat the whole party.
  if (BATTLE_AUTO_MODE && GameFlag_TestFar(0x16d) && (BATTLE_OPTIONS & KEY_R)) {
    s32 enemy = 1;
    s32 i;
    s32 id;

    if (BATTLE_OPTIONS & KEY_SELECT)
      enemy = 0;
    for (i = 0;; i++) {
      if (enemy)
        id = work->enemyIds[i];
      else
        id = work->partyUnits[i];
      if (id == BATTLE_UNIT_LIST_END)
        break;
      if (id == BATTLE_UNIT_REMOVED)
        continue;
      if (Unit_AddHpFar(id, 0xc0000000) == 0) {
        BattleEvPush(BATTLE_EVENT_HIT, id);
        BattleEvPush(BATTLE_EVENT_DEFEAT, id);
      }
    }
    BattleEv_DispatchQueued();
    return -2;
  }

  // Conditions that prevent acting.
  UiWork_ClearValueNameTablesFar();
  if (unit->immobile) {
    unit->immobile = 0;
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgUnableToMove);
    return -1;
  }
  if (unit->sleep) {
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsAsleep);
    return -1;
  }
  if (unit->stun) {
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsParalyzed);
    return -1;
  }
  // Paralysis prevents acting one quarter of the time.
  if ((unit->restraint & 1) && req->command != CMD_DEFEND &&
      (BattleRandomFar() & 3) == 0) {
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsBound);
    return -1;
  }
  if (req->command == CMD_NONE)
    return -2;
  abilityId = ABILITY_ATTACK;
  {
    s32 i;

    for (i = 0; i < TARGET_MAX; i++)
      plan->targetCritical[i] = 0;
    for (i = 0; i < TARGET_MAX; i++)
      plan->targetHit[i] = -1;
  }

  //----------------------------------------------------------------------
  // Announce the selected action.
  //----------------------------------------------------------------------
  switch (req->command) {
  case CMD_FLEE:
    if ((u16)req->unitId <= ALLY_MAX) {
      UiText_ShowMessageAndWaitCoreFar(MsgPartyFlees);
    } else {
      UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgActorRuns);
    }
    BattlePresentation_WaitForAdvance();
    plan->outcome = 7;
    return 0;

  case CMD_ATTACK:
    // A weapon unleash replaces the action with its ability.
    abilityId = RollWeaponUnleashFar(unit);
    if (TargetSet(abilityId) == -1)
      return -1;
    if (abilityId != ABILITY_ATTACK) {
      UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(Inventory_GetEquippedItemFar(unit, 1),
                              VALUE_ITEM_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgWeaponHowls);
      BattleEv_MarkDisplay();
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgWeaponHowls + 1);
    }
    break;

  case CMD_PSYNERGY: {
    struct BattleAct *act;
    s32 ok = 1;

    abilityId = req->parameter;
    act = BattleActGet(abilityId);
    if (TargetSet(abilityId) == -1)
      return -1;
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorCasts);
    // Fail if PP is insufficient or Psynergy is sealed.
    if (unit->pp < act->ppCost) {
      plan->failure = 2;
      ok = 0;
    }
    if (unit->psySeal) {
      plan->failure = 1;
      ok = 0;
    }
    if (ok) {
      plan->failure = 0;
      unit->pp -= act->ppCost;
      UnitRatioFar(req->unitId);
      if (unit->pp < 0)
        unit->pp = 0;
      if (unit->pp > unit->maxPp)
        unit->pp = unit->maxPp;
    }
  } break;

  case CMD_ITEM: {
    struct ItemDefinition *item;

    if (req->parameter < 0) {
      UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgItemAlreadyUsed);
      return -1;
    }
    item = Item_GetDirectFar(unit->inventory[req->parameter]);
    abilityId = item->actionId;
    if (abilityId == 0 || (unit->inventory[req->parameter] & 0x400)) {
      UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
      if (unit->guardLevel == 0)
        unit->guardLevel = 1;
      return -1;
    }
    if (TargetSet(abilityId) == -1)
      return -1;
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(unit->inventory[req->parameter], VALUE_ITEM_NAME);
    if (item->useType == 2 || item->useType == 0) {
      switch (item->type) {
      case 1:
      case 3:
      case 6:
      case 7:
      case 8:
        UiText_ShowMessageAndWaitCoreFar(
            MsgActorRaisesItem); // Raises the item.
        break;
      default:
        UiText_ShowMessageAndWaitCoreFar(
            MsgActorUsesBattleItem); // Uses the item.
        break;
      }
    } else {
      UiText_ShowMessageAndWaitCoreFar(MsgActorUsesBattleItem);
    }
  } break;

  case CMD_DEFEND:
  case CMD_DEFEND2:
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
    return -1;

  case CMD_NONE:
    return -2;

  case CMD_ABILITY: {
    s32 message;

    abilityId = req->parameter;
    if (TargetSet(abilityId) == -1)
      return -1;
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    if ((BattleActGet(abilityId)->flags & 15) == 6)
      message = MsgActorUnleashesAbility;
    else
      message = MsgActorUsesAbility;
    // Select the wording for each ability.
    switch (abilityId) {
    case 435: // Ultrasonic waves.
    case 436:
      message = MsgEmitsUltrasonicWaves;
      break;
    case 224: // Heat mirage.
      message = MsgActorCasts;
      break;
    case 500: // Intimidation.
      message = MsgGlowersFerociously;
      break;
    case 501: // Trembling hands.
      message = MsgIsIntimidated;
      break;
    case 499: // Threatening gaze.
      message = MsgBalefulGaze;
      break;
    case 494: // Eating worms.
      message = MsgEatsWorms;
      break;
    case 437: // Roaring and howling abilities.
    case 438:
    case 439:
    case 440:
    case 441:
      message = MsgLetsOutAbility;
      break;
    case 442: // Wing attacks.
    case 443:
    case 444:
      message = MsgActorUsesAbility;
      break;
    case 472: // Rotten bodily fluid.
      message = MsgEnemyUnleashesAbility;
      break;
    case 488: // Sorrowful gaze.
      message = MsgGlowersMiserably;
      break;
    case 492: // Stench of decay.
      message = MsgSmellOfDecay;
      break;
    case 495: // Becoming enraged.
      message = MsgFuriousRage;
      break;
    case 503: // Attempting to divide.
      message = MsgAttemptsToDivide;
      break;
    case 504: // Looking for allies.
      message = MsgLooksForAllies;
      break;
    case 508: // Looking for help.
      message = MsgLooksForHelp;
      break;
    }
    UiText_ShowMessageAndWaitCoreFar(message);
  } break;

  case CMD_DJINN:
    // If not set, return the standby Djinni to its set state.
    abilityId = Djinn_GetAbilityIdFar(DJINN_ELEMENT(req->parameter),
                                      DJINN_INDEX(req->parameter));
    if (!Djinn_IsActiveFar(req->unitId, DJINN_ELEMENT(req->parameter),
                           DJINN_INDEX(req->parameter))) {
      if (Djinn_IsReadyFar(req->unitId, DJINN_ELEMENT(req->parameter),
                           DJINN_INDEX(req->parameter))) {
        BattleActGet(abilityId);
        BattlePres_SetActorModes(0, 0);
        Djinn_ActivateFar(req->unitId, DJINN_ELEMENT(req->parameter),
                          DJINN_INDEX(req->parameter));
        DjinnRecovery_RemoveFar(req->unitId, DJINN_ELEMENT(req->parameter),
                                DJINN_INDEX(req->parameter));
        UnitCalcFar(req->unitId);
        BattleEventRuntime_Reset();
        BattleEventRuntime_SchedulePhase(30);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, req->unitId);
        BattleEvPush(BATTLE_EVENT_ABILITY_NAME,
                     DJINN_ELEMENT(req->parameter) * 20 +
                         DJINN_INDEX(req->parameter) + 300);
        BattleEvPush(BATTLE_EVENT_SOUND, 175);
        BattleEvPush(BATTLE_EVENT_DRAW_STATUS, 0);
        BattleEvPush(BATTLE_EVENT_MESSAGE, MsgDjinnSet);
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, req->unitId);
        AudioCommand_PlayFar(212);
        ObjectDispatch_ApplyArgumentToChildrenFar(
            BattleObjGet(req->unitId)->object, 3);
        ObjectDispatch_ApplyValueToChildrenFar(
            (struct DispatchObject *)BattleObjGet(req->unitId)->object, 32);
        BattleFx_PlayUnitElementEffect(req->unitId,
                                       DJINN_ELEMENT(req->parameter), 3, 0);
        BattleEventRuntime_WaitForReady();
        return -2;
      }
      // Recovering.
      UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      AudioCommand_PlayFar(SOUND_MENU_ERROR);
      UiText_ShowMessageAndWaitCoreFar(MsgDjinnInRecovery);
      WaitFrames(60);
      return -1;
    }
    // Unleash the set Djinni.
    {
      struct BattleAct *act;

      if (TargetSet(abilityId) == -1)
        return -1;
      DjinnRecovery_AddStandbyFar(req->unitId, DJINN_ELEMENT(req->parameter),
                                  DJINN_INDEX(req->parameter));
      act = BattleActGet(abilityId);
      UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgActorUnleashesDjinn);
      plan->element = act->element;
    }
    break;

  case CMD_SUMMON: {
    const struct SummonDefinition *summon;
    struct DjinnRecoveryList *list;
    s32 side;
    u8 counts[4];
    /* FAKEMATCH: never read: the native frame keeps 8 bytes below counts,
     * exactly this array; without it the frame is 40 bytes, not 48, and every
     * slot above it moves (differ 28 in tbs-en). */
    u8 unused[8];
    s32 i;

    // Check whether enough Djinn of each element are on standby.
    summon = SummonDefinition_Get(req->parameter);
    BattlePlacement_CountValidEntries(req->unitId, counts);
    side = 0;
    if ((u16)req->unitId > ALLY_MAX)
      side = 1;
    list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
    for (i = 0; i < 4 && counts[i] >= summon->djinnRequired[i]; i++)
      counts[i] = summon->djinnRequired[i];
    abilityId = summon->nameMessageId;
    if (TargetSet(abilityId) == -1)
      return -1;
    if (i != 4) {
      UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgSummonLacksDjinn);
      return -1;
    }
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorSummons);
    // Mark the standby Djinn being used with -2.
    for (i = 0; i != list->count; i++) {
      struct DjinnRecoveryEntry *entry = &list->entries[i];

      if (entry->turns == -1 && BattleParty_IsUnitListed(entry->unitId) &&
          counts[entry->element] != 0) {
        entry->turns = -2;
        counts[entry->element]--;
      }
    }
  } break;
  }

  //----------------------------------------------------------------------
  // Plan a normal attack.
  //----------------------------------------------------------------------
  if (abilityId == ABILITY_ATTACK) {
    struct BattleUnit *target = UnitGetFar(plan->targetIds[0]);

    plan->abilityId = ABILITY_ATTACK;
    plan->element = Item_GetEquippedElementFar(req->unitId);
    plan->outcome = 2;
    if (unit->classIndex == 0) // Enemy.
    {
      plan->presentationFlags =
          Battle_GetSpriteFlagNibble(unit->classId) | 0x4000;
    } else {
      plan->presentationFlags = 0;
      switch (unit->classId) {
      case ISAAC:
        plan->presentationFlags = 0x4001;
        break;
      case GARET:
        plan->presentationFlags = 0x4001;
        break;
      case IVAN:
        plan->presentationFlags = 0x4004;
        break;
      case MIA:
        plan->presentationFlags = 0x4004;
        break;
      case JENNA:
        plan->presentationFlags = 0x4001;
        break;
      }
    }
    UiWork_PushValueSlotFar(req->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorAttacks);
    // Miss chance: 153/256 when deluded, plus a separate 1/32 roll.
    // Attacks cannot miss a target that cannot move.
    /* FAKEMATCH: the one-pass block places the body ahead of its five tests,
     * reached by a backward beq from the last one, whose zero (r5) the first
     * store reuses. As a plain if, with the tests negated or not, or as a while
     * loop left by break, the tests come first (differ 105 in tbs-en). */
    do {
      if (target->hp == 0 || target->sleep != 0 || target->stun != 0 ||
          target->immobile != 0 || target->charm != 0)
        break;
      if (unit->delusion != 0 && (BattleRandomFar() & 255) <= 152)
        plan->targetMultiplier[0] = 0;
      if ((BattleRandomFar() & 31) == 0)
        plan->targetMultiplier[0] = 0;
    } while (0);
    if (GameFlag_TestFar(366))
      plan->targetMultiplier[0] = 0;
    // Critical hit: one chance in 32, or the equipment's chance.
    if (target->hp != 0) {
      if ((BattleRandomFar() & 31) == 0)
        plan->targetCritical[0] = 1;
      else if ((Equipment_GetUnleashRateBonusFar(unit) << 16) / 200 >
               (BattleRandomFar() & 0xffff))
        plan->targetCritical[0] = 1;
    }
  }

  //----------------------------------------------------------------------
  // Plan an ability.
  //----------------------------------------------------------------------
  else {
    struct BattleAct *act = BattleActGet(abilityId);

    plan->element = act->element;
    plan->presentationFlags = 0;
    plan->abilityId = abilityId;
    if (act->effect == 65 || act->effect == 41 || act->effect == 42 ||
        act->effect == 43 || act->effect == 44 || act->effect == 68) {
      // Chance to increase the damage multiplier.
      s32 chance;
      s32 amount;

      if (act->effect == 65 || act->effect == 68)
        chance = 153;
      else if (act->effect == 41 || act->effect == 43)
        chance = 32;
      else
        chance = 64;
      if (act->effect == 65 || act->effect == 41 || act->effect == 42)
        amount = 1;
      else
        amount = 2;
      if ((BattleRandomFar() & 255) < chance) {
        s32 i;

        for (i = 0; i < plan->targetCount; i++)
          plan->targetMultiplier[i] += amount;
      }
    } else if (act->effect >= 36 && act->effect <= 40) {
      // Critical-hit chance ranges from 1/64 to 1/4.
      s32 mask;

      switch (act->effect) {
      case 36:
        mask = 63;
        break;
      case 37:
        mask = 31;
        break;
      case 38:
        mask = 15;
        break;
      case 39:
        mask = 7;
        break;
      case 40:
      default:
        mask = 3;
        break;
      }
      if ((BattleRandomFar() & mask) == 0) {
        s32 i;

        for (i = 0; i < plan->targetCount; i++)
          plan->targetCritical[i] = 2;
      }
    } else if (abilityId == 178) // Lethal technique: roll the hit first.
    {
      s32 i;

      for (i = 0; i < plan->targetCount; i++)
        plan->targetHit[i] = BattleHitCheck(req->unitId, plan->targetIds[i],
                                            act->element, act->effect, 100);
    }
    if (abilityId >= 0 && abilityId <= 518) {
      plan->presentationFlags = gBattle_ActionFlags[abilityId];
      if (plan->targetMultiplier[0] > 1)
        plan->presentationFlags = gBattle_ActionFlags[abilityId] +
                                  (plan->targetMultiplier[0] << 12) - 0x1000;
    }
    if ((abilityId >= 0 && abilityId <= 517) &&
        gBattle_ActionStatus[abilityId] != 0)
      plan->outcome = gBattle_ActionStatus[abilityId];
    else if (Ability_CheckStatusOrSpecialId(abilityId))
      plan->outcome = 3;
    else if (plan->presentationFlags != 0) {
      if (unit->classIndex == 0)
        plan->outcome = 8;
      else
        plan->outcome = 3;
    } else
      plan->outcome = 1;
    if (Battle_IsReviveEffectFar(act->effect))
      plan->presentationFlags |= 0x10000;
    if (abilityId == 178 && plan->targetHit[0] != 0)
      plan->presentationFlags |= 0x1000;
  }
  if (req->command == CMD_ITEM && plan->outcome != 5 && plan->outcome != 9)
    plan->outcome = 4;
  plan->command = req->command;
  return 0;
}

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias) {
  struct BattleUnit *state = UnitGetFar(unitId);

  if (turns <= 5) {
    s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
    if (threshold >= (BattleRandomFar() & 0xffff))
      return 1;
  }
  return 0;
}

s32 BattleUnit_TickAttackModifier(s32 unitId) {
  struct BattleUnit *state = UnitGetFar(unitId);

  if (state->attackTurns != 0) {
    s32 zero;
    state->attackTurns--;
    zero = 0;
    if (state->attackTurns == 0) {
      state->attackStage = zero;
      return 1;
    }
    if (state->attackStage < 0) {
      if (BattleUnit_RollCounterEarlyEnd(unitId, state->attackTurns, 30) != 0) {
        state->attackStage = zero;
        state->attackTurns = zero;
        return 1;
      }
    }
  }
  return 0;
}

// Decrements a stat change's turn counter. Resets level and returns one when
// the counter reaches zero, or when BattleUnit_RollCounterEarlyEnd ends a
// negative-level stat reduction early.
#define DEFINE_TICK_MODIFIER(name, counter, level)                             \
  s32 name(s32 unitId) {                                                       \
    struct BattleUnit *state = UnitGetFar(unitId);                             \
                                                                               \
    if (state->counter != 0) {                                                 \
      state->counter--;                                                        \
      if (state->counter == 0) {                                               \
        state->level = 0;                                                      \
        return 1;                                                              \
      }                                                                        \
      if (state->level < 0 &&                                                  \
          BattleUnit_RollCounterEarlyEnd(unitId, state->counter, 20) != 0) {   \
        state->level = 0;                                                      \
        state->counter = 0;                                                    \
        return 1;                                                              \
      }                                                                        \
    }                                                                          \
    return 0;                                                                  \
  }

DEFINE_TICK_MODIFIER(BattleUnit_TickDefenseModifier, defenseTurns, defenseStage)
DEFINE_TICK_MODIFIER(BattleUnit_TickResistanceModifier, resistanceTurns,
                     resistanceStage)

// Decrements a status condition's turn counter. Returns one when it reaches
// zero, or when BattleUnit_RollCounterEarlyEnd(bias) ends it early.
#define DEFINE_TICK_STATUS(name, counter, bias)                                \
  s32 name(s32 unitId) {                                                       \
    struct BattleUnit *state = UnitGetFar(unitId);                             \
    if (state->counter != 0) {                                                 \
      state->counter--;                                                        \
      if (state->counter == 0)                                                 \
        return 1;                                                              \
      if (BattleUnit_RollCounterEarlyEnd(unitId, state->counter, bias) != 0) { \
        state->counter = 0;                                                    \
        return 1;                                                              \
      }                                                                        \
    }                                                                          \
    return 0;                                                                  \
  }

DEFINE_TICK_STATUS(BattleUnit_TickDelusion, delusion, 30)

s32 BattleUnit_TickConfusion(s32 unitId) {
  struct BattleUnit *state = UnitGetFar(unitId);
  if ((u8)state->confusion != 0) {
    (*(u8 *)&state->confusion)--;
    if (state->confusion == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, (u8)state->confusion, 60) != 0) {
      state->confusion = 0;
      return 1;
    }
  }
  return 0;
}

DEFINE_TICK_STATUS(BattleUnit_TickCharm, charm, 70)
DEFINE_TICK_STATUS(BattleUnit_TickStun, stun, 40)
DEFINE_TICK_STATUS(BattleUnit_TickSleep, sleep, 50)

s32 Battle_AdvanceCounterAndCheckChance(s32 id) {
  s32 next;
  u8 remaining;
  s32 turns;
  s32 packed;
  struct BattleUnit *unit;

  unit = UnitGetFar(id);
  packed = unit->psySeal;
  turns = packed & 0xFF;
  if (turns != 0) {
    if (turns > 7U) {
      turns += 0xF8;
      unit->psySeal = turns;
      packed = turns;
    }
    if (packed & 7) {
      next = packed + 0xFF;
      unit->psySeal = next;
      packed = next;
    }
    remaining = packed;
    if (remaining == 0)
      return 1;
    if (remaining <= 7U &&
        BattleUnit_RollCounterEarlyEnd(id, unit->psySeal, 0x1E) != 0) {
      unit->psySeal = 0U;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickRefrain(s32 unitId) {
  u8 *value = &UnitGetFar(unitId)->refrain;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickReflect(s32 id) {
  u8 *value = (u8 *)&UnitGetFar(id)->reflect;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickAgilityModifier(s32 id) {
  struct BattleUnit *unit = UnitGetFar(id);
  u8 *value = &unit->agilityTurns;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      unit->agilityStage = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattlePlacement_UpdateTimedEntries(void) {
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;
  s32 index;
  s32 removed;
  s32 initialCount;

  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(0))->list;
  initialCount = list->count;
  index = 0;
  removed = 0;
  if (index < initialCount) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 && UnitGetFar(timedEntry->unitId)->hp != 0) {
        timedEntry->turns--;
      }
      index++;
      timedEntry++;
    } while (index < list->count);
  }
  index = 0;
  if (index < list->count) {
    expiredEntry = list->entries;
    do {
      if (expiredEntry->turns == 0) {
        u8 id = expiredEntry->unitId;

        Djinn_ActivateFar(id, expiredEntry->element, expiredEntry->index);
        DjinnRecovery_RemoveFar(id, expiredEntry->element, expiredEntry->index);
        UnitCalcFar(id);
        removed = 1;
      } else {
        expiredEntry++;
        index++;
      }
    } while (index < list->count);
  }
  return removed;
}

s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void) {
  s32 turns;

  turns = 19;
  do {
    turns--;
    BattlePlacement_UpdateTimedEntries();
  } while (turns >= 0);
  return 0;
}

void Battle_ReservedNoOpF674(void) {}

struct BattleGroupOrder {
  s32 group[2];
};

s32 Battle_ProcessRoundEnd(void) {
  struct BattleWork *work;
  s32 side;
  s32 sides;
  s32 count;
  s32 i;
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;

  work = gBattleWork;
  sides = (work->twoSided != 0) + 1;
  for (side = 0; side < sides; side++) {
    list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
    i = 0;
    if (i < list->count) {
      timedEntry = list->entries;
      do {
        if (timedEntry->turns > 0 && BattleObjGet(timedEntry->unitId) != 0 &&
            UnitGetFar(timedEntry->unitId)->hp != 0) {
          timedEntry->turns--;
        }
        i++;
        timedEntry++;
      } while (i < list->count);
    }
    i = 0;
    if (i < list->count) {
      expiredEntry = list->entries;
      do {
        if (expiredEntry->turns == 0) {
          u8 id = expiredEntry->unitId;
          s32 element;

          if (BattleObjGet(id) != 0) {
            BattleEventRuntime_Reset();
            BattleEventRuntime_SchedulePhase(30);
            BattleEvPush(BATTLE_EVENT_UNIT_NAME, id);
            BattleEvPush(BATTLE_EVENT_ABILITY_NAME, expiredEntry->element * 20 +
                                                        expiredEntry->index +
                                                        300);
            BattleEvPush(BATTLE_EVENT_SOUND, 175);
            BattleEvPush(BATTLE_EVENT_DRAW_STATUS, 0);
            PUSH_MESSAGE(MsgDjinnSet);
            BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, id);
            AudioCommand_PlayFar(212);
            ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(id)->object,
                                                      3);
            ObjectDispatch_ApplyValueToChildrenFar(
                (struct DispatchObject *)BattleObjGet(id)->object, 32);
            element = expiredEntry->element;
            Djinn_ActivateFar(id, element, expiredEntry->index);
            DjinnRecovery_RemoveFar(id, expiredEntry->element,
                                    expiredEntry->index);
            UnitCalcFar(id);
            BattleFx_PlayUnitElementEffect(id, element, 3, 0);
            BattleEventRuntime_WaitForReady();
          }
        } else {
          expiredEntry++;
          i++;
        }
      } while (i < list->count);
    }
  }
  BattlePresentation_ConfigurePaletteFade(2, gBattleWork->background, 0);
  {
    s32 *groupList;
    struct BattleGroupOrder order = gBattleParty_RoundEndGroupOrder;
    u16 ids[14];
    s32 group;
    s32 j;

    if (work->twoSided != 0 && work->linkSide != 0) {
      order.group[0] = 2;
      order.group[1] = 1;
    }
    groupList = order.group;
    for (group = 0; group <= 1; group++) {
      count = BattleParty_ListActorIds(groupList[group], ids);
      for (j = 0; j < count; j++) {
        s32 hpAmount;
        s32 ppAmount;
        s32 id = ids[j];
        struct BattleUnit *unit = UnitGetFar(id);

        if (unit->ready != 0) {
          unit->ready--;
        }
        if (unit->hp != 0) {
          if (unit->hpRegen != 0 && unit->hp != unit->maxHp) {
            hpAmount = unit->hpRegen;
            if (unit->hp + hpAmount > unit->maxHp) {
              hpAmount = unit->maxHp - unit->hp;
            }
            Unit_AddHpFar(id, hpAmount);
            UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
            UiWork_PushValueSlotFar(hpAmount, VALUE_NUMBER);
            if (unit->hp == unit->maxHp) {
              UiText_ShowMessageAndWaitCoreFar(MsgHpFull);
            } else {
              UiText_ShowMessageAndWaitCoreFar(MsgHpRecover);
            }
            AudioCommand_PlayFar(175);
            BattlePresentation_WaitForAdvance();
          }
          if (unit->ppRegen != 0 && unit->pp != unit->maxPp) {
            ppAmount = unit->ppRegen;
            if (unit->pp + ppAmount > unit->maxPp) {
              ppAmount = unit->maxPp - unit->pp;
            }
            Unit_AddPpFar(id, ppAmount);
            UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
            UiWork_PushValueSlotFar(ppAmount, VALUE_NUMBER);
            if (unit->pp == unit->maxPp) {
              UiText_ShowMessageAndWaitCoreFar(MsgPpFull);
            } else {
              UiText_ShowMessageAndWaitCoreFar(MsgPpRecover);
            }
            AudioCommand_PlayFar(175);
            BattlePresentation_WaitForAdvance();
          }
        }
        if (BattleUnit_TickAgilityModifier(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgAgiNormal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickAttackModifier(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgAtkNormal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickDefenseModifier(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgDefNormal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickResistanceModifier(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgResNormal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickDelusion(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgCureDelusion);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickConfusion(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgConscious);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickCharm(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgConscious2);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickStun(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          BattlePres_SetActorModeAndAction(id);
          UiText_ShowMessageAndWaitCoreFar(MsgCureStun);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickSleep(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          BattlePres_SetActorModeAndAction(id);
          UiText_ShowMessageAndWaitCoreFar(MsgWakes);
          BattlePresentation_WaitForAdvance();
        }
        if (Battle_AdvanceCounterAndCheckChance(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgCureSeal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickRefrain(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgRefrainEnd);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickReflect(id) != 0) {
          BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
          UiWork_PushValueSlotFar(id, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgReflectEnd);
          BattlePresentation_WaitForAdvance();
        }
      }
    }
  }
}

s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan) {
  s32 id;
  struct BattleUnit *unit;
  s32 bothSides;
  s32 counts[4];
  struct DjinnRecoveryList *list;
  s32 best;
  s32 order;

  id = plan->actorId;
  bothSides = 0;
  unit = UnitGetFar(id);
  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
               (id < 0 || id > 7)))
              ->list;
  for (best = 0; best < list->count; best++) {
    if (list->entries[best].unitId == id && list->entries[best].turns == -1) {
      Djinn_DeactivateFar(id, list->entries[best].element,
                          list->entries[best].index);
    }
  }
  if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) != 0 &&
      BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) != 0) {
    bothSides = 1;
  }
  {
    s32 i;
    s32 owner;
    struct DjinnRecoveryList *pending;
    pending = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                    (id < 0 || id > 7)))
                   ->list;
    for (i = 0; i < 4; i++) {
      counts[i] = 0;
    }
    for (;;) {
      owner = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].turns == -2) {
          owner = pending->entries[i].unitId;
          break;
        }
      }
      if (owner == -1) {
        break;
      }
      order = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns > order) {
          order = pending->entries[i].turns;
        }
      }
      order++;
      if (order <= 1) {
        order = 2;
      }
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns == -2) {
          pending->entries[i].turns = order;
          counts[pending->entries[i].element]++;
          order++;
        }
      }
    }
  }
  if (bothSides != 0) {
    struct BattleUnit *before;
    s32 most;
    s32 i;
    most = 0;
    before = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleUnit));
    Iwram_CopyWords(before, unit, sizeof(struct BattleUnit));
    best = -1;
    for (i = 0; i <= 3; i++) {
      if (counts[i] > most) {
        most = counts[i];
        best = i;
      }
    }
    if (best >= 0 && unit->elementLevels[best] < most) {
      unit->elementLevels[best] = most;
    }
    UnitCalcFar(id);
    for (i = 0; i <= 3; i++) {
      s32 gain = unit->element[i].power - before->element[i].power;

      if (gain > 0) {
        BattleEventRuntime_Reset();
        BattleEventRuntime_SchedulePhase(25);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, id);
        BattleEvPush(BATTLE_EVENT_NUMBER, gain);
        BattleEvPush(BATTLE_EVENT_SOUND, 175);
        PUSH_MESSAGE(MsgEarthPowerUp + i);
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, id);
        AudioCommand_PlayFar(212);
        ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(id)->object, 3);
        ObjectDispatch_ApplyValueToChildrenFar(
            (struct DispatchObject *)BattleObjGet(id)->object, 32);
        BattleFx_PlayUnitElementEffect(id, i, 2, most - 1);
        BattleEventRuntime_WaitForReady();
      }
    }
    MemFree(before);
  }
  if (bothSides != 0) {
    s8 *poison;
    BattleEventRuntime_Reset();
    if (plan->hauntDamage != 0) {
      BattleEvPush(BATTLE_EVENT_HIT, id);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, id);
      BattleEvPush(BATTLE_EVENT_NUMBER, plan->hauntDamage);
      PUSH_MESSAGE(MsgCurseDamage);
      if (Unit_AddHpFar(id, -plan->hauntDamage) == 0) {
        s32 text;
        BattleEvPush(BATTLE_EVENT_DEFEAT, id);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, id);
        if (id >= 0 && id <= 7) {
          text = MsgGoesDown;
        } else {
          text = MsgStrengthExhausted;
        }
        PUSH_MESSAGE(text);
      } else {
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, id);
      }
    }
    BattleEv_DispatchQueued();
    BattleEventRuntime_Reset();
    poison = &unit->poison;
    if (*poison != 0) {
      s32 damage = unit->maxHp * *poison / 10;
      struct BattleWork *state = gBattleWork;
      BattleEvPush(BATTLE_EVENT_HIT, id);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, id);
      BattleEvPush(BATTLE_EVENT_NUMBER, damage);
      PUSH_MESSAGE(MsgPoisonDamage);
      if (*poison != 0) {
        state->events.pendingCue = SOUND_HEAVY_IMPACT;
      } else {
        state->events.pendingCue = 133;
      }
      if (Unit_AddHpFar(id, -damage) == 0) {
        s32 text;
        BattleEvPush(BATTLE_EVENT_DEFEAT, id);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, id);
        if (id >= 0 && id <= 7) {
          text = MsgGoesDown;
        } else {
          text = MsgStrengthExhausted;
        }
        PUSH_MESSAGE(text);
      } else {
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, id);
      }
    }
    BattleEv_DispatchQueued();
    BattleEventRuntime_Reset();
    if (unit->grimReaper != 0) {
      if (--unit->grimReaper == 0 && Unit_AddHpFar(id, 0xc0000000) == 0) {
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, id);
        PUSH_MESSAGE(MsgReaperCalls);
        BattleEvPush(BATTLE_EVENT_HIT, id);
        BattleEvPush(BATTLE_EVENT_DEFEAT, id);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, id);
        if (id >= 0 && id <= 7) {
          PUSH_MESSAGE(MsgReaperCalls - 3);
        } else {
          PUSH_MESSAGE(MsgReaperCalls + 3);
        }
      }
    }
    BattleEv_DispatchQueued();
  }
  UnitCalcFar(id);
}

s32 BattlePres_RunMosaicFadeOut(void) {
  /* FAKEMATCH: volatile restores the form the bytes prove: the four saved
   * BGxCNT values live at sp+6, 4, 2 and 0, each stored with strh and loaded
   * again with ldrh for the write back (an 8-byte frame). Without all four
   * differ 61, 83 instructions for 90 and no frame; without one alone, 27 to
   * 45; as a u16 saved[4] array, in either order: differ 60. */
  volatile u16 bg0cnt;
  volatile u16 bg1cnt;
  volatile u16 bg2cnt;
  volatile u16 bg3cnt;
  s32 i;
  volatile u16 *reg;

  bg0cnt = REG_BG0CNT;
  REG_BG0CNT = bg0cnt | 0x40;
  reg = &REG_BG0CNT;
  bg1cnt = *++reg;
  *reg = bg1cnt | 0x40;
  bg2cnt = *++reg;
  *reg = bg2cnt | 0x40;
  bg3cnt = *++reg;
  *reg = bg3cnt | 0x40;
  reg += 0x21;
  *reg = 0x3eee;

  Fade_ToBlack(16);
  for (i = 0; i <= 15; i++) {
    Random16();
    Random16();
    Random16();
    Random16();
    REG_MOSAIC = (i << 8) | i;
    WaitFrames(1);
  }

  REG_DISPCNT = 1;
  WaitFrames(4);
  reg = &REG_BG0CNT;
  *reg++ = bg0cnt;
  *reg++ = bg1cnt;
  *reg++ = bg2cnt;
  *reg = bg3cnt;
  return 0;
}

//--------------------------------------------------------------------------
// build_sequential_table.c
// Tiles: builds a map of sequential byte indices.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Writes byte sequences 0..255 and 0..223, then fills the remaining 544 bytes
// with 0xff, for 1024 bytes total.
//--------------------------------------------------------------------------
void Graphics_BuildSequentialTileTable(s32 *dst) {
  s32 v;
  s32 n;

  // Advance four indices per iteration because each store writes four bytes.
  v = 0x03020100;
  n = 0;
  do {
    n += 1;
    *dst++ = v;
    v += 0x04040404;
  } while (n >= 0 && n <= 63);

  v = 0x03020100;
  n = 0;
  do {
    n += 1;
    *dst++ = v;
    v += 0x04040404;
  } while (n >= 0 && n <= 55);

  Iwram_FillWords(dst, 544, -1);
}

void BattlePresentation_BuildTilemap(s32 *destination) {
  s32 entry;
  u32 index;

  Iwram_FillWords(destination, 0x100, -1);
  destination += 0x40;
  Iwram_FillWords(destination, 0x80, 0x03ff03ff);
  entry = (0x0201 << 16) | 0x0200;
  destination += 0x20;
  index = 0;
  do {
    index++;
    *destination++ = entry;
    entry += 0x00020002;
  } while (index <= 239);
  Iwram_FillWords(destination, 0x280, 0x03ff03ff);
}

void BattlePres_UpdateHBlankScroll(void) {
  struct BattleAffineHdma *records;
  u16 *record;

  if (((struct BattleBackgroundView *)WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND))
          ->mode == 2) {
    records = WORK_SLOT(10);
    record = records->lines[records->page];
    Io_Write16(record[0], &REG_BG2CNT);
    Dma_Set(record + 1, (void *)0x0400000c, 0xa2600001, &REG_DMA0SAD);
    Dma_Set(&records->affine, (void *)0x04000020, 0x84000004, REG_DMA3);
  }
}

void BattlePresentation_UploadTileVariant(void) {
  u32 variant = *gDisp - 1;
  if (variant <= 31)
    Dma_Set(gBattlePres_TileVariants + (variant >> 2) * 32, (void *)0x06005000,
            0x84000008, REG_DMA3);
}

void BattlePres_AdvanceTransitionTimer(void) {
  s32 scroll;
  struct BattleCamera *camera;
  u32 *timer;
  struct BgScroll *offset;
  u32 frame;
  u32 next;

  timer = *(u32 **)Ram_Disp;
  frame = *timer;
  camera = *(struct BattleCamera **)Ram_CameraWork;
  scroll = 0x34 - frame;
  if (scroll > 0x20)
    scroll = 0x20;
  offset = (struct BgScroll *)Ram_BgScroll;
  if (scroll < 0)
    scroll = 0;
  offset->y = scroll;
  if (frame <= 0x50U)
    camera->yaw = 45 * frame * 8 + 0xaf80;
  next = (*timer = *timer + 1);
  if (next <= 0x50U) {
    BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
    return;
  }
  BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

void BattlePres_DrawTransitionRows(void) {
  u32 i;
  s32 frame;
  s32 aligned;
  s32 tile;
  u32 row;
  u16 *tiles;

  frame = *gDisp;
  if (frame >= 0 && frame <= 79) {
    tile = (7 & frame) + 0xf081;
    if (frame >= 0) {
      aligned = frame;
    } else {
      aligned = frame + 7;
    }
    row = 13 - (aligned >> 3);
    i = 0;
    tiles = (u16 *)((row << 6) + 0x06006000);
    do {
      i++;
      *tiles = tile;
      tiles++;
    } while (i != 32);

    tile |= 0x800;
    aligned = frame;
    if (frame < 0) {
      aligned += 7;
    }
    row = (aligned >> 3) + 13;
    if (row <= 20) {
      i = 0;
      tiles = (u16 *)((row << 6) + 0x06006000);
      do {
        i++;
        *tiles = tile;
        tiles++;
      } while (i != 32);
    }
  }
}

void Graphics_ClearBg0Vofs(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

struct DisplayRegisters {
  u16 control;
  u16 greenSwap;
};

void BattlePres_RunEntryTransition(s32 enemyCount, s32 kind) {
  volatile u32 fill;
  struct BattlePresentationWork list;
  u16 party[14];
  u16 actors[14];
  struct BattleBackgroundView *work;
  s32 *timer;
  u16 *map;
  s32 tile;
  u32 i;
  u32 j;
  s32 count;
  s32 id;
  struct BattleObj *object;
  struct BattleWork *session;
  u16 *ids;
  s32 n;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  timer = Runtime_AllocateHeapBlock(42, 4);
  if (kind != 0x15b) {
    {

      volatile u32 *dma;

      Dma_Set(gBattlePres_CurtainTiles, (void *)0x06005020, 0x84000008,
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 32, (void *)0x06005040, 0x84000008,
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 64, (void *)0x06005060, 0x84000008,
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 96, (void *)0x06005080, 0x84000008,
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 128, (void *)0x060050a0, 0x84000008,
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 160, (void *)0x060050c0, 0x84000008,
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 192, (void *)0x060050e0, 0x84000008,
              REG_DMA3);
      dma = REG_DMA3;
      /* FAKEMATCH: introducing the pointer after the transfers avoids a
       * preserved-register copy; the volatile output orders its subtraction
       * before the control value, differing by two instructions without
       * volatile. 2026-10-11: the native subs r3, #212 is reload_cse_move2add
       * on a constant address that happened to land in r3; plain pointer
       * arithmetic is CSE-reused by the later Dma_Sets (differ 45), REG_DISPCNT
       * = 1 takes a pool constant (63), and a constant struct-field store gets
       * r2 from local-alloc, so no move2add (31). */
      asm volatile("" : "+l"(dma));
      ((struct DisplayRegisters *)((u8 *)dma - 212))->control = 1;
    }
    work->secondMode = 1;
    work->mode = 1;
    work->busy = 0;
    Dma_Fill32Cell(fill, 0x33333333, (void *)0x06005000, 0x20);
    Dma_Fill32Cell(fill, 0, (void *)0x06005100, 0x20);
    REG_BG1CNT = 0xc04;
    REG_BG0CNT |= 2;
    work->mode = 2;
    map = (u16 *)0x06006000;
    for (i = 0; i <= 31; i++) {
      if (i <= 20)
        tile = 0xf080;
      else
        tile = 0xf088;
      for (j = 0; j <= 31; j++)
        *map++ = tile;
    }
    {
      s32 start = 0;

      gBgScroll[0].y = 32;
      gBgScroll[1].y = 32;
      gBgScroll[1].x = 8;
      WaitFrames(1);
      REG_WIN0H = 0xf0;
      REG_WIN0V = 0x88;
      REG_WIN1H = 0xf0;
      REG_WIN1V = 0x88;
      REG_WININ = 0x3537;
      REG_WINOUT = 0x3f21;
      QueueIoWriteDelay2(0x04000000, 0x7741);
      BattlePres_SetupTransitionScene(0, 0, 0, 180);
      *timer = start;
      Scheduler_AddOrUpdateCallback(BattlePres_AdvanceTransitionTimer, 3200);
      Scheduler_AddOrUpdateCallback(BattlePres_DrawTransitionRows, 0x480);
      Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
      gBgScroll[0].y = 32;
      WaitFrames(1);
      UiWindow_CreateWithLayoutBoundsFar(gBattleWork->partyStatus);
      WaitFrames(20);
      QueueIoWriteDelay10(0x04000008, 2);
      QueueIoWriteDelay6(0x04000008, 0);
      BattleIntro_AnnounceEncounter(enemyCount);
      Scheduler_RemoveCallback(BattlePres_AdvanceTransitionTimer);
      Scheduler_RemoveCallback(BattlePres_DrawTransitionRows);
      gBgScroll[0].y = start;
      Runtime_SetIrqHandler(2, 0, 0);
    }
  } else {
    session = WORK_SLOT(HEAP_SLOT_BATTLE);
    work->secondMode = 1;
    work->busy = 0;
    count = BattleParty_ListActorIds(3, (u16 *)party);
    for (i = 0; i != count; i++) {
      id = i + 120;
      if ((s32)i <= 7)
        id = i;
      object = BattleObjGet(id);
      if (UnitGetFar(id)->classId != 148)
        object->scale = 0xb333;
    }
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
    WaitFrames(1);
    {
      s32 start = 0;

      session->background = (u32)&ResourceId_VenusAerieStormBackdrop;
      list.count = BattleParty_ListActorIds(2, list.actors);
      list.actors[list.count] = 0xff;
      BattleActor_SpawnObjectsForList(list.actors, 0);
      BattleEffect_RunTileAndPaletteAnimationFar(&list);
      BattlePres_SetupTransitionScene(0, 0, 0, 100);
      *timer = start;
    }
    Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
    WaitFrames(1);
    WaitFrames(20);
    UiWindow_CreateWithLayoutBoundsFar(gBattleWork->partyStatus);
    QueueIoWriteDelay10(0x04000008, 2);
    QueueIoWriteDelay6(0x04000008, 0);
    REG_BLDCNT = 0x3f40;
    ids = actors;
    n = BattleParty_ListActorIds(3, (u16 *)ids);
    actors[n] = 0xff;
    BattleActor_SpawnObjectsForList(ids, 0);
    n = BattleParty_ListActorIds(1, (u16 *)ids);
    for (i = 0; i != n; i++)
      BattlePres_SetActorRecordMode(ids[i], 1);
    for (i = 0; i != 16; i++) {
      REG_BLDALPHA = i | 0x1000;
      WaitFrames(1);
    }
    for (i = 0; i != n; i++)
      BattlePres_SetActorRecordMode(ids[i], 0);
    BattleIntro_AnnounceEncounter(enemyCount);
    gBgScroll[0].y = 0;
    WaitFrames(1);
    Runtime_SetIrqHandler(2, 0, 0);
  }
  Runtime_SetIrqHandler(2, 0, 0);
  REG_BG1CNT = 0x1f83;
  WaitFrames(1);
  REG_BG1CNT = 0x1f83;
  REG_BG0CNT &= 0xfffd;
  gBgScroll[1].x = 8;
  REG_DISPCNT = 0x1541;
  Runtime_ReleaseHeapBlock(42);
}

void BattlePresentation_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *screen = gBattleWork;
  u16 *palette = screen->palette;
  u32 ime;
  u32 *slot = &ime;

  /* FAKEMATCH: needed to match: the one-pass block and the loop on a zero local
   * keep the IME save through slot's register. As a plain block, also with a
   * volatile register pointer, the save is stored sp-relative, ahead of the
   * palette address: differ 8; as IRQ_LOCK(*slot) and IRQ_UNLOCK the save is
   * stored sp-relative and the restore loads the register address first: differ
   * 8; plain statements restoring REG_IME = *slot: differ 14. */
  do {
    u16 *ime_reg = (u16 *)0x04000208;
    s32 once = 0;

    do {
      *slot = *ime_reg;
      *ime_reg = (u32)ime_reg;
    } while (once);
  } while (0);

  if (level == 0) {
    Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
  } else {
    Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0,
                                screen->brightness = 0x10000 - level * 1092,
                                128);
  }
  {
    u32 saved = *slot;

    REG_IME = saved;
  }
}

void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 value, s32 fade) {
  struct BattleBackgroundView *transition =
      (struct BattleBackgroundView *)gTransitionWork;

  if (transition->mode == 0) {
    Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
  }
  transition->mode = mode;

  if (mode == 1) {
    IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f83, 0x20000);
  }

  Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
  *(u16 *)0x050000bc = *(u16 *)0x050001e8;

  if (fade == 0x80) {
    Dma_Set(gBattleWork->palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
  } else if (fade != 0) {
    u16 *source = gBattleWork->palette;
    u16 *destination = (u16 *)0x050000c0;
    s32 i;

    for (i = 0; i != 128; i++) {
      s32 red = source[i] & 31;
      s32 green = (source[i] >> 5) & 31;
      s32 blue = (source[i] >> 10) & 31;

      if (red > fade) {
        red -= fade;
      } else {
        red = 0;
      }
      if (green > fade) {
        green -= fade;
      } else {
        green = 0;
      }
      if (blue > fade) {
        blue -= fade;
      } else {
        blue = 0;
      }
      destination[i] = (blue << 10) | (green << 5) | red;
    }
  }

  Graphics_BuildSequentialTileTable((void *)0x06003800);
  BattlePresentation_BuildTilemap((void *)0x0600f800);
}

void BattlePresentation_InitializeWorkAndResetState(void) {
  struct BattleAffineHdma *work;
  struct BattleBackgroundView *state;
  work = Runtime_AllocateBlock(10, sizeof *work);
  state = (struct BattleBackgroundView *)gTransitionWork;
  Dma_Fill32(0, work, sizeof *work);
  state->mode = 0;
}

void Runtime_ReleaseHeapBlock10(void) { Runtime_ReleaseHeapBlock(10); }

typedef void (*BitDecoder)(const u8 *source, void *destination);

void BattleBackground_Load(s32 mode, s32 resource, s32 level) {
  struct BattleBackgroundView *view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  u8 *data = Resource_GetTableEntry(resource);
  struct BattleWork *session = WORK_SLOT(HEAP_SLOT_BATTLE);
  u16 *palette;

  RamCode_LoadSlot(49, BitDecoder_DecodeImage, BitDecoder_Size);
  {
    u32 offset = 0x100;
    void *vram;

    /* FAKEMATCH: conditional cancelling updates end CSE before the
     * decoder arguments, keeping the VRAM load before the data add.
     * They disappear from all 103 instructions in every edition;
     * a reassignment or XOR pair instead differs by four rows.
     * Plain, with data + 0x100 and the VRAM address as the call's
     * arguments, the data add is scheduled first: differ 2; through
     * the vram local alone: differ 4; through a decoder local:
     * differ 79. */
    if (session) {
      offset++;
      offset--;
    }
    vram = (void *)0x06008000;
    ((BitDecoder)WORK_SLOT(49))(data + offset, vram);
  }
  Runtime_ReleaseHeapBlock(49);
  palette = session->palette;
  Dma_Set(data, palette, 0x84000040, REG_DMA3);
  if (level >= 0)
    Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0,
                                session->brightness = 0x10000 - level * 1092,
                                128);
  Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
  *(u16 *)0x050000bc = *(u16 *)0x050001e8;
  Graphics_BuildSequentialTileTable((void *)0x06003800);
  BattlePresentation_BuildTilemap((void *)0x0600f800);
  Iwram_ClearWords((void *)0x0600ffc0, 64);
  if (view->mode == 0)
    Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
  view->mode = mode;
  if (mode == 1)
    REG_BG1CNT = 0x1f83;
}

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale) {
  struct BattleBackgroundView *view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleAffineHdma *work = WORK_SLOT(10);
  struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  u16 wrap;
  s32 horizon;
  u16 *line;
  struct BattleAffine *affine;
  s32 ratio;
  s32 step;
  s32 x;
  s32 y;
  u32 i;
  u32 count;

  horizon = 0x800;
  wrap = 0;
  if (scale >= 0x10000) {
    wrap = 0x2000;
    horizon = 0x6800 + -(s16)camera->yaw * 3;
  }
  if (work == NULL)
    return;
  if ((view->mode == 1 || view->secondMode == 1) && view->busy == 0)
    gBgScroll[1].x = horizon >> 8;
  if (view->mode != 2)
    return;
  line = work->lines[work->page ^ 1];
  ratio = Iwram_RatioMulQ14(scale, 0x10000);
  step = ratio >> 8;
  affine = &work->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulQ16(ratio, Iwram_MulQ16(cx, scale - 0x10000));
  y = Iwram_MulQ16(ratio, Iwram_MulQ16(cy, scale - 0x10000));
  affine->x = ((x + 0x7fff) >> 8) + ox + horizon;
  y = ((y + 0x7fff) >> 8) + oy - 0x1000;
  affine->y = y;
  count = (Iwram_RatioMulQ14((s16)step, 0x4000 - y) >> 16) + 1;
  i = 0;
  if (!GameFlag_TestFar(0x16b)) {
    for (; i < 16; i++)
      *line++ = 0x3f8e;
  }
  if (count > 136)
    count = 136;
  for (; i < count; i++)
    *line++ = wrap | 0x478a;
  for (; i < 136; i++)
    *line++ = wrap | 0x478e;
  for (; i < 160; i++)
    *line++ = 0x3f8e;
  work->page ^= 1;
}

void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y) {
  s32 span = 0x01fe0000;
  // BUG: mode is uninitialized; reads entry r4 directly (lsls r0, r4, #16).
  s32 mode;
  struct BattleCamera *scene = gCameraWork;
  s32 *pos = scene->pos;
  s32 scale = (mode << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(pos);
  SceneTransform_ApplyYaw((s16)scene->yaw);
  SceneTransform_ApplyPitch((s16)scene->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = scene->distance;
  Iwram_TransformVector(renderBounds, (s32 *)scene);
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *scene = gCameraWork;
  s32 *pos = scene->pos;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 span = 0x01fe0000;
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(pos);
  SceneTransform_ApplyYaw((s16)scene->yaw);
  SceneTransform_ApplyPitch((s16)scene->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = span;
  Iwram_TransformVector(renderBounds, (s32 *)scene);
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

//--------------------------------------------------------------------------
// setup_transition_at_pair_midpoint.c
// Battle display: centers the camera between two actors.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Centers the camera at the two objects' midpoint in x and z.
// a, b: object ids.
// zoom: scale percentage.
//--------------------------------------------------------------------------
void BattlePres_SetupTransitionAtPairMidpoint(s32 a, s32 b, s32 zoom) {
  struct MotionObject *obj_a = BattleObjGet(a)->object;
  struct MotionObject *obj_b = BattleObjGet(b)->object;
  s32 ax = obj_a->x;
  s32 bx = obj_b->x;
  s32 az = obj_a->z;
  s32 bz = obj_b->z;
  s32 x = (bx + ax) / 2;
  s32 z = (bz + az) / 2;

  BattlePres_SetupTransitionScene(x, 0, z, zoom);
}

void Graphics_FadeAlphaOut(void) {
  s32 alpha_step;

  REG_BLDCNT = 0x2044;
  for (alpha_step = 1; alpha_step <= 16; alpha_step += 2) {
    REG_BLDALPHA = 0x1010 - alpha_step;
    WaitFrames(1);
  }
}

void Graphics_FadeAlphaIn(void) {
  s32 step;

  REG_BLDCNT = 0x2044;
  for (step = 1; step <= 16; step += 2) {
    REG_BLDALPHA = step + 0x1000;
    WaitFrames(1);
  }
}

struct BattleTransitionEntry {
  u32 value;
  u32 sum;
  u32 field8;
  u32 fieldc;
};

void Graphics_SetBlendControl(void) { REG_BLDCNT = 0xbf; }

void BattlePresentation_InitializeTransitionEntries(
    struct BattleTransitionEntry *entries) {
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(entries);
  entries[0].sum = previous + entries[0].value;
}

s32 BattlePres_DivideBy16(s32 value) { return value / 16; }

#if EDITION_INTERNATIONAL
#define DEBUG_SELECT_VALUE 0x18f
#else
#define DEBUG_SELECT_VALUE 480
#endif

s32 Runtime_AdjustDebugValueWithButtons(s32 ret) {
  struct BattleWork *battle;

  if (gKeysHeld & KEY_START) {
    for (;;) {
      battle = gBattleWork;
      if (gKeysRepeat & 0x20)
        battle->debugValue -= 1;
      if (gKeysRepeat & 0x10)
        battle->debugValue += 1;
      if (gKeysRepeat & 0x40)
        battle->debugValue -= 100;
      if (gKeysRepeat & 0x80)
        battle->debugValue += 100;
      if (gKeysPressed & KEY_A)
        ret = battle->debugValue;
      else {
        WaitFrames(1);
        continue;
      }
      break;
    }
  }
  if (gKeysHeld & KEY_SELECT)
    ret = DEBUG_SELECT_VALUE;
  return ret;
}

void BattlePres_SetActorRecordMode(s32 actorId, s32 mode) {
  struct BattleObj *slot = BattleObjGet(actorId);
  struct MotionObject *object;
  struct AnimationObject *record;
  struct AnimationObject **records;
  s32 i;

  if (slot == 0)
    return;
  object = slot->object;
  if (object == 0)
    return;
  switch (object->recordStorageKind & 15) {
  case 1: {
    record = object->records;
    record->part[0].objectMode = mode;
    record->part[1].objectMode = mode;
    break;
  }
  case 2: {
    records = object->records;
    i = 0;
    do {
      record = *records++;
      if (record == 0)
        break;
      record->part[0].objectMode = mode;
      record->part[1].objectMode = mode;
      i++;
    } while (i <= 3);
    break;
  }
  }
}

//--------------------------------------------------------------------------
// present.c
// Battle display: toggles translucent unit sprites.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Makes every unit sprite except the selected one translucent (OBJ mode one).
// except: unit id left unchanged.
//--------------------------------------------------------------------------
void BattlePres_SetOtherActorRecordModes(s32 except) {
  s16 list[14];
  s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  s32 i;

  for (i = 0; i < cnt; i++) {
    if (list[i] != except)
      BattlePres_SetActorRecordMode(list[i], 1);
  }
}

//--------------------------------------------------------------------------
// Restores every unit sprite to normal rendering (OBJ mode zero).
//--------------------------------------------------------------------------
void BattlePres_ClearAllActorRecordModes(void) {
  s16 list[14];
  s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  s32 i;

  for (i = 0; i < cnt; i++)
    BattlePres_SetActorRecordMode(list[i], 0);
}

void Graphics_AdvancePaletteCycle(void) {
  struct BattleWork *battle = gBattleWork;
  s8 *table;
  u16 step;
  s32 next;

  if (battle != NULL && battle->highlightMode != 0) {
    REG_BLDCNT = 0x3f90;
    REG_BLDALPHA = 0x10;
    table = gBattleFx_HighlightBlendRamp;
    REG_BLDY = table[battle->highlightStep];
    step = battle->highlightStep;
    next = (step + 1) & 0xf;
    if (step > 14)
      next |= 0x10;
    battle->highlightStep = next;
  }
}

void BattlePres_SetActorModes(u16 *actors, s32 mode) {
  s16 activeActors[14];
  struct BattleWork *battle = gBattleWork;
  u32 count;
  u32 i;

  if (mode == 0) {
    Scheduler_RemoveCallback(Graphics_AdvancePaletteCycle);
    REG_BLDY = mode;
    BattlePres_ClearAllActorRecordModes();
    WaitFrames(1);
    QueueIoWriteDelay2(0x04000050, 0);
  }
  if (battle != 0 && mode != 0) {
    battle->highlightMode = mode;
    battle->highlightStep = 0;
    REG_BLDY = 0;
    Io_Write16(16, &REG_BLDALPHA);

    count = BattleParty_ListActorIds(3, activeActors);
    for (i = 0; i < count; i++)
      BattlePres_SetActorRecordMode(activeActors[i], mode & 1);

    if (actors != 0) {
      u32 actor = *actors;
      i = 0;
      actors++;
      if (actor != 0xff) {
        do {
          BattlePres_SetActorRecordMode(actor, (mode & 1) ^ 1);
          i++;
          if (i > 13)
            break;
          actor = *actors++;
        } while (actor != 0xff);
      }
    }
    WaitFrames(1);
    QueueIoWriteDelay2(0x04000050, 0);
    Scheduler_AddOrUpdateCallback(Graphics_AdvancePaletteCycle, 0x480);
  }
}

struct Spark {
  s32 pos[6];
  s32 life;
};

struct SparkWork {
  u8 sheet[0x11c0];
  struct Spark sparks[16];
  struct Ring rings[3];
  s32 frames;
  s32 ready;
  s32 originX;
  s32 originY;
  s32 originPending;
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width,
                         s32 height);

// UB: the scheduler invokes a void callback. Plain void changes the native
// return register from r1 to r0 in tbs-en; matching builds keep the old type.
#ifdef UBFIX
void BattleFx_UpdateStarField(void) {
#else
s32 BattleFx_UpdateStarField(void) {
#endif
  s32 j;
  s32 frame;
  u32 size;
  s32 ringX;
  s32 ringY;
  s32 phase;
  void *dest;
  s32 i;
  struct SparkWork *work;
  DrawFunc draw;
  struct Spark *spark;
  struct Ring *ring;
  s32 *pos;
  s32 k;
  s32 dist;
  s32 scale;
  s32 value;
  s32 step;
  u32 radius;
  s32 angle;
  u32 half;
  s32 x;
  s32 y;
  s32 life;

  dest = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  work->ready = 0;
  draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER];
  for (i = 0; i < 16; i++) {
    spark = &work->sparks[i];
    life = spark->life;
    if (life != 0) {
      dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8) +
                       (spark->pos[1] >> 8) * (spark->pos[1] >> 8) +
                       (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
      if (dist <= 0xfff) {
        spark->life = 0;
      } else {
        scale = Iwram_RatioMulQ14(dist, 0x10000);
        spark->life--;
        pos = spark->pos;
        for (k = 2; k >= 0; k--) {
          value = *pos;
          step = Iwram_MulQ16(Iwram_MulQ16(-value >> 8, scale), 0x13000);
          pos[3] = pos[3] - (pos[3] >> 7) + step;
          *pos = value + pos[3];
          pos++;
        }
      }
      life = spark->life;
    }
    if (life == 0 && work->frames <= 24) {
      angle = Random16();
      radius = Random16() + 0x10000;
      half = radius >> 1;
      spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
      spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
      if (spark->pos[0] & 1)
        spark->pos[0] = -spark->pos[0];
      if (spark->pos[1] & 1)
        spark->pos[1] = -spark->pos[1];
      spark->pos[2] = (Random16() + 0x8000) >> 2;
      spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
      spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
      spark->pos[5] = 0;
      life = spark->life = (radius >> 13) + 1;
    }
    if (life != 0) {
      x = (spark->pos[0] >> 10) + 64;
      y = (spark->pos[1] >> 10) + 64;
      frame = life;
      if (frame < 0)
        frame = 0;
      else if (frame > 6)
        frame = 6;
      size = gBattleFx_StarSizes[frame];
      draw(dest, (u8 *)work + gBattleFx_StarWorkOffsets[frame], x - (size >> 1),
           y - (size >> 1), size, size);
    }
  }
  draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  for (ring = work->rings, j = 2; j >= 0; j--, ring++) {
    ring->x += ring->velX;
    ring->y += ring->velY;
    ringX = ring->x >> 10;
    ringY = ring->y >> 10;
    phase = 3 - ring->age / 8;
    if (phase >= 0) {
      ring->age++;
      draw(dest, (u8 *)work + gBattleFx_RingFrameOffsets[phase], ringX + 48,
           ringY + 48, 32, 32);
    }
  }
  work->frames++;
  work->ready = 1;
}

// UB: the scheduler invokes a void callback. Plain void changes the native
// return register from r1 to r0 in tbs-en; matching builds keep the old type.
#ifdef UBFIX
void Graphics_UploadVramBlock(void) {
#else
u32 Graphics_UploadVramBlock(void) {
#endif
  void *source = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  struct SparkWork *work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];

  if (source != 0) {
    if (work->ready != 0) {
      work->ready = 0;
      ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000,
                                              0x4000);
    }
  }
}

s32 BattleFx_InitializeStarField(s32 mode) {
  struct SparkWork *work;
  struct Spark *spark;
  struct Ring *ring;
  s32 i;
  u8 *data;
  s32 zero;
  /* FAKEMATCH: needed to match: an unused Spark, the 28 bytes the native frame
   * reserves. */
  struct Spark unused;

  ((s32 *)gTransitionWork)[2] = 1;
  work = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
  /* FAKEMATCH: the empty loop ends a scheduling region, keeping the
   * multiply routine's address load ahead of the spark cursor. Measured:
   * without it the two swap (differ 2); the sparks indexed in place
   * still differ 2, the cursor walked as a pointer 4, and the two heap
   * allocations in the other order 12. */
  do {
  } while (0);
  for (i = 0; i < 16; i++) {
    s32 angle = Random16();
    u32 radius = Random16() + 0x10000;
    u32 half = radius >> 1;

    spark = &work->sparks[i];
    spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
    spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
    if (spark->pos[0] & 1)
      spark->pos[0] = -spark->pos[0];
    if (spark->pos[1] & 1)
      spark->pos[1] = -spark->pos[1];
    spark->pos[2] = (Random16() + 0x8000) >> 2;
    spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
    spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
    spark->pos[5] = 0;
    spark->life = (radius >> 13) + 1;
  }
  for (i = 0; i < 3; i++) {
    s32 radius = 0x1000;
    s32 angle;

    ring = &work->rings[i];
    angle = i * 0x5555;
    ring->x = Iwram_MulQ16(Trig_Cos(angle), radius);
    ring->y = Iwram_MulQ16(Trig_Sin(angle), radius);
    ring->velX = Iwram_MulQ16(Trig_Cos(angle), 0x200);
    ring->velY = Iwram_MulQ16(Trig_Sin(angle), 0x200);
    ring->age = 0;
  }
  work->frames = 0;
  work->ready = 0;
  work->originPending = 0;
  Iwram_ClearWords(gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], 0x4000);
  data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
  Iwram_CopyWords(BG_PLTT, data, 128);
  data += 128;
  Resource_DecodeType01(data, work);
  switch (mode) {
  case 0:
    data = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
    break;
  case 1:
    data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
    break;
  case 2:
    data = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
    break;
  default:
    data = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
    break;
  }
  Dma_Set(data, BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 32, REG_DMA3);
  /* FAKEMATCH: the one-pass block keeps the zero ahead of the BG2X
   * address, which reload derives from the DMA register's. Measured:
   * plain stores of 0, or of zero outside a block, load it after the
   * address (differ 2); REG_BG2X = REG_BG2Y = 0 differs 13. */
  do {
    zero = 0;
    REG_BG2X = zero;
    REG_BG2Y = zero;
  } while (0);
  Io_Write16(0x100, &REG_BG2PA);
  Io_Write16(0, &REG_BG2PB);
  Io_Write16(0, &REG_BG2PC);
  Io_Write16(0x100, &REG_BG2PD);
  BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
  BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(BattleFx_UpdateStarField, 0xc80);
#else
  Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_UpdateStarField,
                                0xc80);
#endif
#ifdef UBFIX
  return Scheduler_AddOrUpdateCallback(Graphics_UploadVramBlock, 0xc80);
#else
  return Scheduler_AddOrUpdateCallback(
      (SchedulerCallback)Graphics_UploadVramBlock, 0xc80);
#endif
}

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused) {
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  REG_DISPCNT = 0x1341;
#ifdef UBFIX
  Scheduler_RemoveCallback(Graphics_UploadVramBlock);
#else
  Scheduler_RemoveCallback((SchedulerCallback)Graphics_UploadVramBlock);
#endif
#ifdef UBFIX
  return Scheduler_RemoveCallback(BattleFx_UpdateStarField);
#else
  return Scheduler_RemoveCallback((SchedulerCallback)BattleFx_UpdateStarField);
#endif
}

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale,
                                s32 count) {
  s32 remaining;
  u32 mask_red;
  u32 mask_green;
  u32 mask_blue;
  u32 pixel;
  u32 red;
  u32 green;
  u32 blue;

  if (scale > 0x10000)
    scale = 0x10000;
  if (count > 0) {
    mask_red = 0x1f;
    mask_green = 0x3e0;
    mask_blue = 0x7c00;
    remaining = count;
    do {
      pixel = *source;
      red = pixel & mask_red;
      green = pixel & mask_green;
      blue = mask_blue & pixel;
      red *= scale;
      green *= scale;
      blue *= scale;
      pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
      pixel |= (blue >> 16) & mask_blue;
      *destination = pixel;
      source++;
      destination++;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant) {
  /* FAKEMATCH: needed to match: an unread ten-word launch tail, the native
   * frame's 40 extra bytes. */
  s32 spare[10];
  struct EffectPosition position;
  struct EffectPosition position2;
  struct BattlePresentationWork launch;
  struct BattlePresentationWork launch2;
  /* FAKEMATCH: one variable, assigned twice, holds slot 12's address
   * and later the IME address, as the bytes do; the session is slot 9,
   * three words below. Measured: the session read as gWorkSlot[9] with
   * ime set only in the loop differs 124 (247 instructions for 249);
   * with IO_WRITE_QUEUE_PUSH in the loop as well, 140. */
  volatile u16 *ime = (volatile u16 *)&gWorkSlot[12];
  struct BattleWork *session = *(struct BattleWork **)((u8 *)ime - 12);
  struct SparkWork *work;
  s32 frame;
  s32 *brightness;
  struct EffectPosition *pos;

  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFade(1, session->background, 0);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  QueueIoWriteDelay2(0x04000000, 0x3741);
  QueueIoWriteDelay2(0x0400000c, 0x784);
  QueueIoWriteDelay2(0x04000050, 0x3f44);
  WaitFrames(1);
  Io_Write16(240, &REG_WIN0H);
  Io_Write16(0x1088, &REG_WIN0V);
  Io_Write16(63, &REG_WININ);
  Io_Write16(17, &REG_WINOUT);
  if (mode == 0) {
    QueueIoWriteDelay2(0x04000052, 0x100e);
    BattleFx_InitializeStarField(kind);
    for (frame = 0, pos = &position; frame <= 44; frame++) {
      brightness = &session->brightness;
      ime = &REG_IME;
      work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
      if (frame <= 24)
        Graphics_ScaleRgb555Clamped(session->palette, (u16 *)0x050000c0,
                                    *brightness = 0x10000 - frame * 1092, 128);
      BattleMotion_ProjectScaledPosition(unit, (s32 *)pos);
      work->originX = (64 - pos->x) << 8;
      work->originY = (64 - pos->y) << 8;
      IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000028, (u32)&work->originX, 0x84000002);
      work->originPending = 1;
      WaitFrames(1);
    }
    Graphics_ResetVramBlockAndReleaseHeapBlocks(kind);
  } else if (mode == 1) {
    // This workspace is effect.c's BattleEffectWork, but these stores use
    // the same offsets as the star workspace.
    BattlePresentation_PrepareSceneFar(kind);
    for (frame = 0; frame < 40; frame++) {
      work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
      BattleMotion_ProjectScaledPosition(unit, (s32 *)&position2);
      work->originX = (64 - position2.x) << 8;
      work->originY = (64 - position2.y) << 8;
      IO_WRITE_QUEUE_PUSH(0x04000028, (u32)&work->originX, 0x84000002);
      work->originPending = 1;
      WaitFrames(1);
    }
    BattleFx_ScheduleCallbacksAndReleaseBlocksFar();
  } else if (mode == 2) {
    launch.flags = 0;
    launch.kind = kind;
    launch.variant = variant;
    launch.actor = unit;
    launch.actors[0] = unit;
    launch.firstTarget = unit;
    launch.count = 1;
    launch.initialValue = 1;
    BattleFx_RunIceFleckBurstFar((struct BattleEffectArgument *)&launch);
  } else {
    launch2.kind = kind;
    launch2.flags = 0;
    launch2.variant = 0;
    launch2.actor = unit;
    launch2.actors[0] = unit;
    launch2.firstTarget = unit;
    launch2.count = 1;
    launch2.initialValue = 1;
    BattleFx_RunGatheringBurstFar(&launch2);
  }
}

void BattlePres_RunWithZeroArguments(void) {
  BattlePresentation_SetPaletteLevel(0, 0);
}

void Runtime_RemoveIrqHandlerSlot2(void) { Runtime_SetIrqHandler(2, 0, 0); }

s32 Party_ComputeEligibleMemberAverage(s32 recordId) {
  struct BattleFormationRecord *record;
  struct EnemyDefinition *enemy;
  u32 i;
  s32 id;
  s32 sum;
  s32 count;
  /* FAKEMATCH: needed to match: an unread seven-word level table, the native
   * frame's 28 bytes */
  s32 levels[7];

  count = 0;
  sum = 0;
  record = &gBattleFormation_Records[recordId];
  for (i = 0; i < 5; i++) {
    if (record->minimumCounts[i] != 0)
      break;
  }
  if (i == 5)
    return -1;
  for (i = 0; i < 5; i++) {
    if (record->maximumCounts[i] == 0)
      continue;
    id = record->memberIds[i];
    enemy = Enemy_GetDefinitionFar(id + 8);
    if (enemy == 0)
      continue;
    if (enemy->level > 3 && !GameFlag_TestFar(372) &&
        !GameFlag_TestFar(id + 1544))
      return -2;
    sum += enemy->level;
    count++;
  }
  if (count == 0)
    return -3;
  return sum / count;
}

struct FormationCandidate {
  s16 recordId;
  s16 score;
};

struct BattleBaseStats {
  s16 baseHp;
  s16 basePp;
  s16 hpGauge;
  s16 ppGauge;
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 actionCount;
  u8 hpRegen;
  u8 ppRegen;
  u8 unk12[2];
  struct BattleElementStats element[4];
};

s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin) {
  s32 matchCount = 0;
  struct FormationCandidate *pool = Runtime_BumpAllocateAlternatePool(128);
  u16 party[8];
  s32 levelTotal = 0;
  s32 unitCount = BattleParty_PrepareActiveOwners(party);
  s32 i;
  s32 j;
  s32 chance;
  s32 result;

  if (unitCount > 0) {
    u16 *p = party;

    i = unitCount;
    do {
      u8 level = UnitGetFar(*p)->level;

      i--;
      p++;
      levelTotal += level;
    } while (i != 0);
  }

  chance = levelTotal / unitCount;
  chance += (s8)GameFlag_GetByteFar(1016);
  if (chance <= 0)
    chance = 1;
  if (chance > 99)
    chance = 99;

  for (i = 0; i <= 31; i++)
    pool[i].score = -1;

  for (i = 0; (u32)i <= 19; i++) {
    Enemy_GetDefinitionFar(gBattleFormation_LevelMatchedEnemies[i]);
    GameFlag_ClearBitFar(gBattleFormation_LevelMatchedEnemies[i] + 1536);
  }

  for (i = 0; (u32)i <= 379; i++) {
    s32 score = Party_ComputeEligibleMemberAverage(i);

    if (score >= 0 && score <= chance + 3) {
      s32 minIndex = -1;
      s32 minValue;

      for (j = 0, minValue = 999; j <= 31; j++) {
        if (pool[j].score < minValue) {
          minValue = pool[j].score;
          minIndex = j;
        }
      }

      if (minIndex >= 0) {
        pool[minIndex].score = score;
        pool[minIndex].recordId = i;
        matchCount++;
      }
    }
  }

  if (matchCount > 32)
    matchCount = 32;

  if (matchCount != 0) {
    struct FormationCandidate *chosen = &pool[(matchCount * Random16()) >> 16];

    result = chosen->recordId;
    *outMargin = chance - chosen->score;
  } else {
    *outMargin = matchCount;
    result = 1;
  }

  MemFree(pool);
  return result;
}

s32 Owner_ApplyLevelGains(s32 owner, s32 levels) {
  struct BattleBaseStats *base;
  struct BattleUnit *state;
  s32 value;
  s32 floor;
  s32 i;

  base = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleBaseStats));
  state = UnitGetFar(owner);
  Iwram_CopyWords(base, &state->baseHp, sizeof(struct BattleBaseStats));

  value = state->baseHp;
  value += levels * 97 / 10;
  floor = base->baseHp * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 9999)
    value = 9999;
  state->baseHp = value;

  value = state->basePp;
  value += levels * 15 / 10;
  floor = base->basePp * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 9999)
    value = 9999;
  state->basePp = value;

  floor = levels * 123 / 10;
  value = state->baseAttack;
  value += floor;
  floor = base->baseAttack * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->baseAttack = value;

  floor = levels * 33 / 10;
  value = state->baseDefense;
  value += floor;
  floor = base->baseDefense * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->baseDefense = value;

  floor = levels * 51 / 10;
  value = state->baseAgility;
  value += floor;
  floor = base->baseAgility * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->baseAgility = value;

  for (i = 0; i < 4; i++) {
    value = state->baseElement[i].power + levels * 15;
    floor = base->element[i].power * 7 / 10;
    if (value < floor)
      value = floor;
    if (value > 200)
      value = 200;
    state->baseElement[i].power = value;
  }

  state->level += levels;
  UnitCalcFar(owner);
  MemFree(base);
}

#if EDITION_INTERNATIONAL
#define CH_CNT 9
#else
#define CH_CNT 26
#endif

s32 EnemyCountDecrement(s32 no, s32 n) {
  struct BattleWork *w;
  s32 num;
  s32 i;
  s32 retry;
  s32 ch;

  w = gBattleWork;
  num = w->summonCount;
  for (i = 0; i < num; i++) {
    if (w->enemyClasses[i] == no)
      break;
  }
  if (i != num) {
    retry = 0;
    if (w->summonChannels[i] < 0) {
      w->summonChannels[i] = 1;
      w->summonMasks[i] = 3;
      return 0x8001;
    }
    for (; retry <= 31; retry++) {
      ch = (w->summonChannels[i] + 1) % CH_CNT;
      w->summonChannels[i] = ch;
      if ((w->summonMasks[i] & (1 << (s8)ch)) == 0)
        break;
    }
    w->summonMasks[i] |= 1 << w->summonChannels[i];
    return w->summonChannels[i];
  }
  if (num <= 4) {
    w->summonChannels[num] = -1;
    w->enemyClasses[num] = no;
    w->summonMasks[num] = 0;
    w->summonCount = num + 1;
    return CH_CNT;
  }
  return -1;
}

s32 EnemyCountRelease(s32 actorId) {
  struct BattleWork *state = gBattleWork;
  struct BattleUnit *actor;
  s32 count = state->summonCount;
  s32 index;
  s32 nameLength;
  s32 bit;
  s32 classId;

  actor = UnitGetFar(actorId);
  if (actor->classIndex != 0)
    return;

  classId = actor->classId;
  for (index = 0; index < count; index++) {
    if (state->enemyClasses[index] == classId)
      break;
  }
  if (index == count || state->summonMasks[index] == 0)
    return;

  for (nameLength = 0; nameLength <= 13; nameLength++) {
    if (actor->name[nameLength] == 0)
      break;
  }

  bit = 32;
  if (nameLength > 0)
    bit = actor->name[nameLength - 1] - LIST_MARKER_CHAR;
  state->summonMasks[index] &= ~(1 << bit);
}

s32 EnemyCountClear(s32 classId) {
  struct BattleUnit *summon;
  s32 objectIndex;
  s32 slot;
  s32 nextSlot;
  u8 occupied;
  u8 marker;

  objectIndex = 0;
  marker = LIST_MARKER_CHAR;
  for (; objectIndex <= 5; objectIndex++) {
    summon = UnitGetFar(objectIndex + 0x80);
    occupied = summon->side;
    if (occupied != 1)
      continue;
    if (summon->classId != classId)
      continue;
    slot = 0;
    if (summon->name[0] == 0) {
      summon->name[0] = marker;
      summon->name[occupied] = slot;
      return;
    }
  scan_slots:
    slot++;
    if (slot > 13)
      return;
    occupied = summon->name[slot];
    if (occupied != 0)
      goto scan_slots;
    nextSlot = slot + 1;
    summon->name[slot] = marker;
    summon->name[nextSlot] = occupied;
    return;
  }
}

s32 BattleGroupRandom(s32 recordId) {
  s32 choices[5];
  struct BattleFormationRecord *record;
  s32 *choicesBase;
  s32 *choiceCursor;
  u8 *maximumCount;
  u8 *memberId;
  u32 count;
  s32 remaining;

  count = 0;
  if (recordId < 0 || recordId >= 380)
    recordId = 1;

  choicesBase = choices;
  record = &gBattleFormation_Records[recordId];
  memberId = record->memberIds;
  maximumCount = record->maximumCounts;
  choiceCursor = choicesBase;
  remaining = 4;
  do {
    if (*maximumCount++ != 0) {
      *choiceCursor++ = *memberId + 8;
      count++;
    }
    memberId++;
    remaining--;
  } while (remaining >= 0);

  return choices[(Random16() * count) >> 16];
}

s32 BattleFormation_BuildEnemyList(s32 recordId) {
  u16 *list;
  struct BattleWork *work;
  struct BattleFormationRecord *record;
  s32 count;
  s32 margin;
  u16 listBuffer[14];
  s32 extra[5];
  s32 counts[5];
  s32 order[5];
  s32 budget;
  s32 changed;
  s32 room;
  s32 size;
  s32 fit;
  s32 id;
  s32 k;
  s32 n;
  s32 total;
  u32 i;
  s32 j;

  work = gBattleWork;
  margin = 0;
  work->summonCount = 0;
  list = listBuffer;
  if (GameFlag_TestFar(0x173))
    recordId = BattleFormation_SelectLevelMatchedCandidate(&margin);
  if (recordId < 0 || recordId >= 380)
    recordId = 1;
  record = &gBattleFormation_Records[recordId];
  for (j = 0; j <= 4 && record->minimumCounts[j] == 0; j++)
    ;
  if (j == 5)
    record = &gBattleFormation_Records[1];

  budget = 6;
  count = 0;
  for (i = 0; i <= 4; i++) {
    if (record->minimumCounts[i] != 0) {
      s32 size1 = 2 - (Summon_IsEntryFlagged(record->memberIds[i] + 8) != 0);

      budget -= size1 * record->minimumCounts[i];
    }
  }

  for (i = 0; i <= 4; i++) {
    s32 minimum;
    s32 maximum;

    counts[i] = minimum = record->minimumCounts[i];
    maximum = record->maximumCounts[i];
    room = maximum - minimum;
    if (room > 0) {
      s32 size2 = 2 - (Summon_IsEntryFlagged(record->memberIds[i] + 8) != 0);

      fit = budget / size2;
      if (fit < room)
        room = fit;
      extra[i] = ((room + 1) * Random16()) >> 16;
    } else {
      extra[i] = 0;
    }
  }

  do {
    changed = 0;
    for (i = 0; i <= 4; i++) {
      id = record->memberIds[i] + 8;
      if (extra[i] != 0) {
        size = 2 - (Summon_IsEntryFlagged(id) != 0);
        if (size > budget) {
          extra[i] = 0;
        } else {
          counts[i]++;
          extra[i]--;
          budget -= size;
          changed = 1;
        }
      }
    }
  } while (changed);

  work->battleType = record->battleType;
  switch (work->battleType) {
  case 0:
    for (i = 0; i <= 4; i++)
      order[i] = i;
    for (i = 0; i <= 9; i++) {
      s32 first = (Random16() * 5) >> 16;
      s32 second = (Random16() * 5) >> 16;
      s32 swap = order[first];

      order[first] = order[second];
      order[second] = swap;
    }
    for (i = 0; i <= 4; i++) {
      s32 m;
      s32 slot = order[i];

      for (m = 0; m < counts[slot]; m++)
        list[count++] = record->memberIds[slot] + 8;
    }
    break;
  case 1:
    for (;;) {
      n = 0;
      for (i = 0; i <= 4; i++) {
        if (counts[i] != 0)
          order[n++] = i;
      }
      if (n == 0)
        break;
      k = order[(Random16() * n) >> 16];
      list[count++] = record->memberIds[k] + 8;
      counts[k]--;
    }
    break;
  default:
    for (i = 0; i <= 4; i++) {
      total = counts[i];
      for (k = 0; k < total; k++)
        list[count++] = record->memberIds[i] + 8;
    }
    break;
  }
  list[count] = 0;
  work->firstDefeated = 6;
  work->defeatState = 0;
  for (i = 128; i <= 133; i++)
    Iwram_ClearWords(UnitGetFar(i), BATTLE_UNIT_SIZE);

  i = 0;
  if (list[0] == 0)
    goto assigned;
assign_enemy:
  {
    s32 charge = EnemyCountDecrement(list[i], 1);

    if (charge & 0x8000)
      EnemyCountClear(list[i]);
    BattleUnitSetFar(i + 128, list[i], charge & 0x7fff);
    UnitGetFar(i + 128);
    if (margin != 0)
      Owner_ApplyLevelGains(i + 128, margin);
  }
  i++;
  if (i > 5)
    goto assigned;
  if (list[i] != 0)
    goto assign_enemy;
assigned:
  return i;
}

u32 Summon_GetLayoutKind(s32 index) {
  u32 kind = gSummon_EntryTable[index].layoutKind;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

s32 Summon_GetEntryValue(s32 index) {
  if (index < 0 || index > 171)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].animation;
}

s32 Summon_GetEntryFlag1Field(s32 index) {
  if (index < 0 || index > 171)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].layoutBits1;
}

s32 Summon_IsEntryFlagged(s32 index) {
  s32 result;

  if (index < 0 || index > 171)
    return 0;
  result = 0;
  if (gSummon_EntryTable[index].spriteBit0)
    result = 1;
  return result;
}

u32 Battle_GetSpriteFlagNibble(u32 index) {
  u16 value;

  if (index > 171)
    return 1;
  value = gSummon_EntryTable[index].spriteBits1;
  return value ? value : 1;
}

u32 Battle_GetSpriteFlagHighBits(u32 index) {
  u16 value;

  if (index > 171)
    return 0;
  value = gSummon_EntryTable[index].spriteBits5;
  return value ? value : 0;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index) {
  if (index < 0 || index > 171)
    return 0;
  return gSummon_EntryTable[index].layoutBit0;
}

s32 Summon_GetEntryByte4(s32 index) {
  if (index < 0 || index > 171)
    return 0;
  return gSummon_EntryTable[index].unk04[0];
}

u32 Item_EncodeBankedId(u32 value) {
  u32 bank = 0;
  u32 base = value & 0x1ff;
  if (base == 0)
    return 0;
  {
    u8 flags = Item_GetDirectFar(base)->flags;

    if (flags & 8)
      bank = 1;
    bank <<= 1;
    if (flags & 4)
      bank++;
    bank <<= 9;
    bank += base;
  }
  return bank;
}

void Summon_ClearWorkFields(void) {
  struct BattleWork *work = gBattleWork;
  struct BattleSpoils *spoils = &work->spoils;
  u16 *items;
  s32 index;

  gGameState.pendingItem = 0;
  spoils->coins = 0;
  spoils->experience = 0;
  spoils->defeated = 0;
  items = work->spoils.items;
  for (index = 3; index >= 0; index--)
    items[index] = 0;
}

static __inline__ s32 BattleEnemy_CalculateCoinReward(struct BattleUnit *unit,
                                                      u16 *reward) {
  s32 i;
  s32 bonus = 0;
  s32 base;
  s32 floor;

  for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
    bonus += ((Random16() * 6) >> 16) + 1;
  base = *reward;
  floor = base * 3 / 10;
  if (bonus < floor)
    bonus = floor;
  return bonus + base;
}

static __inline__ s32
BattleEnemy_CalculateExperienceReward(struct BattleUnit *unit, u16 *reward) {
  s32 i;
  s32 bonus = 0;
  s32 base;
  s32 floor;

  for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
    bonus += ((Random16() * 4) >> 16) + 1;
  base = *reward;
  floor = base * 3 / 10;
  if (bonus < floor)
    bonus = floor;
  return bonus + base;
}

s32 BattleEnemy_RecordDefeat(s32 unitId, s32 earned) {
  struct BattleUnit *unit;
  struct EnemyDefinition *rec;
  struct BattleWork *formation;
  struct BattleSpoils *spoils;
  s32 i;
  s32 chance;
  s32 slot;
  s32 lowest;
  s32 lowestSlot;
  s32 value;

  unit = UnitGetFar(unitId);
  formation = gBattleWork;
  spoils = &formation->spoils;
  if (unitId >= 0 && unitId < 8)
    return -1;
  if (unit->classIndex != 0)
    return -2;
  slot = 0;
  i = 0;
  if (formation->enemyClasses[0] != unit->classId) {
    do {
      if (++i > 5)
        break;
    } while (formation->enemyClasses[i] != unit->classId);
  }
  if (i != 6)
    slot = i;
  if (formation->defeatState != 2) {
    if (slot < formation->firstDefeated)
      formation->firstDefeated = slot;
    if (spoils->defeated)
      formation->defeatState = 1;
  }
  spoils->defeated++;
  if (GameFlag_TestFar(0x173))
    return 0;
  GameFlag_SetBitFar(unit->classId + 0x600);
  rec = Enemy_GetDefinitionFar(unit->classId);
  if (earned != 0) {
    if (rec->coins != 0)
      spoils->coins += BattleEnemy_CalculateCoinReward(unit, &rec->coins);
    if (rec->experience != 0)
      spoils->experience +=
          BattleEnemy_CalculateExperienceReward(unit, &rec->experience);
  } else {
    spoils->coins += rec->coins;
    spoils->experience += rec->experience;
  }
  if (rec->item == 0 || rec->itemChance == 0)
    return 0;
  i = 0;
  if (spoils->items[0] != rec->item) {
    do {
      if (++i > 3)
        break;
    } while (spoils->items[i] != rec->item);
  }
  if (i != 4)
    return 0;
  chance = rec->itemChance;
  if (earned != 0)
    chance -= 2;
  if (chance < 0)
    chance = 0;
  if ((0x20000 >> chance) > (BattleRandomFar() & 0xffff)) {
    lowestSlot = -1;
    lowest = 0x40000000;
    for (i = 0; i <= 3; i++) {
      value = Item_EncodeBankedId(spoils->items[i]);
      if (value < lowest) {
        lowest = value;
        lowestSlot = i;
      }
    }
    if ((s32)Item_EncodeBankedId(rec->item) > lowest)
      spoils->items[lowestSlot] = rec->item;
  }
  return 0;
}

void Battle_AwardSpoils(void) {
  struct BattleSpoils *spoils;
  s32 count;
  u16 *list;
  s32 i;
  s32 unitId;
  struct BattleUnit *unit;
  struct BattleUnit *backup;
  s32 cnt;
  u32 learned;
  s32 j;
  s32 best;
  s32 bestSlot;
  s32 slot;
  u16 *found;
  s32 value;
  struct LevelUpResult gains; // Read stat increases as signed values.
  u16 units[8];

  spoils = &gBattleWork->spoils;
  if (spoils->experience != 0) {
    UiWork_PushValueSlotFar(spoils->experience, VALUE_NUMBER);
    UiText_ShowMessageAndWaitCoreFar(MsgExpGained);
    BattlePresentation_WaitForAdvance();
  }
  list = units;
  count = BattleParty_ListLivingUnits(1, list);
  backup = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleUnit));
  for (i = 0; i < count; i++) {
    unitId = list[i];
    unit = UnitGetFar(unitId);
    unit->experience += spoils->experience;
    while (Iwram_CopyWords(backup, unit, sizeof(struct BattleUnit)),
           Unit_TryLevelUpFar(unitId, &gains) != 0) {
      AudioCommand_PlayFar(0x59);
      UiWork_ClearValueNameTablesFar();
      UiWork_PushValueSlotFar(unit->classIndex, VALUE_CLASS_NAME);
      UiWork_PushValueSlotFar(list[i], VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(unit->level, VALUE_NUMBER);
      UiText_ShowMessageAndWaitCoreFar(MsgLevelUp);
      BattlePresentation_WaitForAdvance();
      for (cnt = 0; cnt < 32; cnt++) {
        learned = unit->actionSlots[cnt].encodedAction;
        if ((learned & OWNER_ACTION_ID_MASK) && (learned >> 15)) {
          for (j = 0; j < 32; j++) {
            if (learned == backup->actionSlots[j].encodedAction)
              break;
          }
          if (j == 32) {
            UiWork_ClearValueNameTablesFar();
            UiWork_PushValueSlotFar(unit->classIndex, VALUE_CLASS_NAME);
            UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
            UiWork_PushValueSlotFar(learned & OWNER_ACTION_ID_MASK,
                                    VALUE_ABILITY_NAME);
            AudioCommand_PlayFar(SOUND_CAST);
            UiText_ShowMessageAndWaitCoreFar(MsgAbilityMastered);
            BattlePresentation_WaitForAdvance();
          }
        }
      }
      if ((s16)gains.hp != 0) {
        UiWork_PushValueSlotFar((s16)gains.hp, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgMaxHpRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.pp != 0) {
        UiWork_PushValueSlotFar((s16)gains.pp, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgMaxPpRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.atk != 0) {
        UiWork_PushValueSlotFar((s16)gains.atk, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgAttackRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.def != 0) {
        UiWork_PushValueSlotFar((s16)gains.def, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgDefenseRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.agi != 0) {
        UiWork_PushValueSlotFar((s16)gains.agi, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgAgilityRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.luck != 0) {
        UiWork_PushValueSlotFar((s16)gains.luck, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgLuckRises);
        BattlePresentation_WaitForAdvance();
      }
    }
  }
  MemFree(backup);
  if (spoils->coins != 0) {
    UiWork_PushValueSlotFar(spoils->coins, VALUE_NUMBER);
    UiText_ShowMessageAndWaitCoreFar(MsgCoinsGained);
    Party_AdjustCoinsFar(spoils->coins);
    BattlePresentation_WaitForAdvance();
  }
  found = &gOverflowItem;
  for (;;) {
    bestSlot = -1;
    best = -1;
    for (slot = 0; slot <= 3; slot++) {
      if (spoils->items[slot] != 0) {
        value = Item_EncodeBankedId(spoils->items[slot]);
        if (value >= best) {
          best = value;
          bestSlot = slot;
        }
      }
    }
    if (bestSlot == -1)
      break;
    UiWork_PushValueSlotFar(spoils->items[bestSlot], VALUE_ITEM_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgItemGained);
    BattlePresentation_WaitForAdvance();
    if (PartyInventory_AddFar(spoils->items[bestSlot]) == -1) {
      *found = spoils->items[bestSlot];
      break;
    }
    spoils->items[bestSlot] = 0;
  }
}

void Battle_ReservedNoOp2A08(void) {}
