#include "unit/object/battle_cue.h"
#include "unit/object/zonedata.h"
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
#include "flags.h"
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
#include "field_sprite.h"
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

// The scene overlay's exit table: a header word holding a scene id, then that
// scene's exits, one packed word each (and a flag word after it when
// EXIT_HAS_FLAG is set). Scene id EXIT_TABLE_END ends the exits of a scene and
// the whole table.
#define EXIT_SCENE_MASK 0xfff
#define EXIT_ENTRANCE(word) (((word) & 0xff000) >> 12)
#define EXIT_KIND(word) (((word) & 0x0ff00000) >> 20)
#define EXIT_HAS_FLAG 0x10000000
#define EXIT_TABLE_END 0x1ff
#define EXIT_KIND_ANY 0xff
#define EXIT_NONE 999 // Field_RunScene's result when the scene was not left

// Chooses the scene and entrance to go to when the party leaves the current
// scene. exitKind is the kind of exit Field_RunScene ended on; the scene's exit
// table gives the first exit of that kind (or of any kind) whose flag, if it
// has one, is not set. An exit of kind 0 or the end of the scene's exits falls
// back to the scene of its first exit.
void Scene_TakeExit(s32 exitKind) {
  s16 currentScene;
  const u32 *exitTable;
  const u32 *firstExit;
  u32 word;
  s32 scene;
  s32 nextScene;
  s32 nextEntrance;
  s32 entrance;
  s32 kind;
  s32 flag;

  currentScene = gGameState.scene;
  exitTable = gOverlayArea.exits();
  nextScene = EXIT_NONE;
  nextEntrance = 0;
  if (exitKind == EXIT_NONE)
    return;

  for (;;) {
    // Find the header of the current scene: exit words have high bits set.
    scene = *exitTable++;
    if (scene & ~EXIT_SCENE_MASK)
      continue;
    scene &= EXIT_SCENE_MASK;
    if (scene == EXIT_TABLE_END)
      break;
    if (scene != currentScene)
      continue;

    firstExit = exitTable;
    for (;;) {
      word = *exitTable++;
      scene = word & EXIT_SCENE_MASK;
      entrance = EXIT_ENTRANCE(word);
      kind = EXIT_KIND(word);
      flag = word & EXIT_HAS_FLAG;
      if (flag)
        flag = *exitTable++;
      if (scene == EXIT_TABLE_END || kind == 0) {
        nextScene = *firstExit & 0x1ff; // only 9 bits, unlike EXIT_SCENE_MASK
        break;
      }
      if (kind == EXIT_KIND_ANY || kind == exitKind) {
        if (flag == 0 || GameFlag_TestFar(flag) == 0) {
          nextScene = scene;
          nextEntrance = entrance;
          break;
        }
      }
    }
    break;
  }

  // Commit the chosen exit only after a matching scene header was found.
  if (nextScene != EXIT_NONE) {
    gGameState.scene = nextScene;
    gGameState.entrance = nextEntrance;
  }
}

// Chooses where the game continues after a battle or another scene that is
// not a field map (see Game_ResetForNewGame). result is that scene's result:
// -1 when the party was defeated, which revives the leader with 1 HP (and, with
// FLAG_CHILDHOOD, fully heals the first two characters) and returns to the
// defeat point, or to the last saved point when none is set. Any other result
// goes to the next scene the event set, or to the default scene with the party
// keeping its position.
void Scene_ReturnFromBattle(s32 result) {
  struct BattleUnit *unit;
  s32 i;
  s32 scene;
  s32 entrance;

  gGameState.sceneChangeReason = result;
  if (result == -1) {
    // Revive the leader before choosing the defeat return point.
    unit = UnitGetFar(gGameState.selectedActor);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    if (GameFlag_TestFar(FLAG_CHILDHOOD)) {
      for (i = 0; i < 2; i++) {
        unit = UnitGetFar(i);
        unit->hp = unit->maxHp;
        unit->pp = unit->maxPp;
        Unit_UpdateGauges(unit);
      }
    }
    scene = gGameState.defeatScene;
    entrance = gGameState.defeatEntrance;
    if (scene == -1 && entrance == -1) {
      gGameState.scene = gGameState.savedScene;
      gGameState.entrance = gGameState.savedEntrance;
    } else {
      if (scene != -1)
        gGameState.scene = scene;
      else
        gGameState.scene = gGameState.defaultScene;
      if (entrance != -1)
        gGameState.entrance = entrance;
      else
        gGameState.entrance = gGameState.defaultEntrance;
    }
  } else {
    // An explicit event destination overrides the scene's default return.
    scene = gGameState.nextScene;
    entrance = gGameState.nextEntrance;
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gGameState.scene = scene;
      else
        gGameState.scene = gGameState.defaultScene;
      if (entrance != -1)
        gGameState.entrance = entrance;
      else
        gGameState.entrance = gGameState.defaultEntrance;
    } else {
      gGameState.scene = gGameState.defaultScene;
      gGameState.entrance = gGameState.defaultEntrance;
      GameFlag_SetBitFar(FLAG_KEEP_PARTY_POSITION);
    }
  }
}

// Returns the group scene belongs to. The scenes of one place share a group,
// which selects their music, area name and the flags kept while inside it.
s32 BattleFx_GetResourceGroup(s32 scene) {
  return gField_SceneTable[scene].group;
}

// Scene ids above the field maps, which Game_ResetForNewGame runs without a
// map.
enum {
  SCENE_BLANK = 0x1fb,
  SCENE_REEL_GAME = 0x1fc,
  SCENE_LUCKY_DICE = 0x1fd,
  SCENE_BATTLE = 0x1fe
};

// The minigames borrow the final two 16-colour background palettes.
enum { MINIGAME_PALETTE_FIRST = 14, MINIGAME_PALETTE_BYTES = 64 };

// Runs the Lucky Dice minigame, preserving BG palettes 14 and 15.
static __inline__ s32 RunLuckyDice(s32 entrance) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = MemAlloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          DMA_ENABLE32 | DMA_32BIT | (MINIGAME_PALETTE_BYTES / 4), REG_DMA3);
  result = Runtime_BlankDisplayAndRunFar(entrance);
  Dma_Set(saved, palette,
          DMA_ENABLE32 | DMA_32BIT | (MINIGAME_PALETTE_BYTES / 4), REG_DMA3);
  MemFree(saved);
  return result;
}

// Runs the slot machine, preserving BG palettes 14 and 15.
static __inline__ s32 RunReelGame(s32 entrance) {
  void *palette = (void *)&BG_PLTT_COLOR(MINIGAME_PALETTE_FIRST, 0);
  void *saved = MemAlloc(MINIGAME_PALETTE_BYTES);
  s32 result;

  Dma_Set(palette, saved,
          DMA_ENABLE32 | DMA_32BIT | (MINIGAME_PALETTE_BYTES / 4), REG_DMA3);
  result = ReelGame_StartFar(entrance);
  Dma_Set(saved, palette,
          DMA_ENABLE32 | DMA_32BIT | (MINIGAME_PALETTE_BYTES / 4), REG_DMA3);
  MemFree(saved);
  return result;
}

// The game's main loop, which never returns. mode chooses the first scene: 0
// starts at the title screen with a fresh game state; in debug mode, 1 starts
// in Isaac's home in Vale and 2 at the clear screen. Each pass resets the
// runtime, then either runs a battle or minigame and picks the scene to return
// to, or loads a field scene's code, starts its music and runs it until the
// party takes an exit.
void Game_ResetForNewGame(s32 mode) {
  const struct SceneRecord *table;
  const struct SceneRecord *record;
  s32 entrance;

  table = gField_SceneTable;
  // Choose the first scene before initializing the per-scene runtime.
  if (gDebugMode && mode == 1) {
    gGameState.scene = (s32)&SceneId_ValeHome;
    gGameState.entrance = mode;
  } else if (gDebugMode && mode == 2) {
    gGameState.scene = (s32)&SceneId_Clear;
    gGameState.entrance = 1;
  } else {
    GameState_InitDefaultsFar();
    gGameState.scene = (s32)&SceneId_Title;
    gGameState.entrance = 2;
  }
  PaletteGlow_UpdateFar(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  VramCache_Reset();
  Scheduler_ResetTaskTable();
  Scheduler_ResetTaskTable();
  for (;;) {
    // Flag 0x101 skips audio command 0x120 once.
    if (GameFlag_TestFar(0x101))
      GameFlag_ClearBitFar(0x101);
    else
      AudioCommand_PlayFar(0x120);
    record = &table[gGameState.scene];
    entrance = gGameState.entrance;

    // Stop DMA 0, which a scene may have left running for an HBlank effect.
    {
      volatile u16 *dma0 = REG_DMA0;

      dma0[5] &= 0xc5ff;
      dma0[5] &= 0x7fff;
      dma0[5];
    }
    Scheduler_ResetTaskTable();
    Runtime_SetIrqHandler(1, 0, 0);
    Runtime_SetIrqHandler(2, 0, 0);
    Runtime_InitializeHeap();
    DebugText_Clear();
    VramCache_Reset();
    if (gGameState.scene >= SCENE_BLANK) {
      switch (gGameState.scene) {
      case SCENE_BATTLE:
        entrance = Battle_RunEncounterFar(entrance);
        break;
      case SCENE_LUCKY_DICE:
        entrance = RunLuckyDice(entrance);
        break;
      case SCENE_REEL_GAME:
        entrance = RunReelGame(entrance);
        break;
      case SCENE_BLANK:
        entrance = 0;
        break;
      }
      Scene_ReturnFromBattle(entrance);
      continue;
    }

    // A field scene: set its flags and music, load its code and run it.
    Scene_ResetFlagsOnEnter(gGameState.scene,
                            GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION));
    Scene_ResolveInteractionResult();
    if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
      // Flags 0x11a and 0x11b keep the music that is playing.
      if (GameFlag_TestFar(0x11a) || GameFlag_TestFar(0x11b))
        GameFlag_ClearBitFar(0x11a);
      else
        Audio_PlayCueFromEventWork();
    } else if (gGameState.returnCue != -1) {
      AudioCommand_PlayFar(gGameState.returnCue);
    } else {
      Audio_PlayCueFromEventWork();
    }
    gGameState.map = record->mapIndex;
    Scene_LoadOverlay(0);
    Scene_TakeExit(Field_RunScene(entrance));
  }
}

// Loads the current scene's code into the overlay area.
void Scene_LoadOverlay(s32 unused) {
  const struct SceneRecord *table;

  table = gField_SceneTable;
  Resource_LoadCode(table[gGameState.scene].resourceId, &gOverlayArea);
}

#define SCENE_RECORD(id) (&gField_SceneTable[id])

// Updates the flags and state that last for one scene or one group of scenes,
// on entering the current scene. Unless keep is set (the party returns to
// where it was, as after a battle), the flags of the last scene are cleared,
// and on entering a new group so are the group's flags and step counters, the
// location name is shown and the entrance becomes the point Retreat returns
// to. Then the group's visited flag (0x180 + group) is set.
void Scene_ResetFlagsOnEnter(s32 unused, s32 keep) {
  s32 group = SCENE_RECORD(gGameState.scene)->group;
  s32 flag;

  if (keep == 0) {
    // Local puzzle flags expire on every normal scene entrance.
    for (flag = 0x200; flag <= 0x2ff; flag++)
      GameFlag_ClearBitFar(flag);
    if (group != gGameState.sceneGroup) {
      // Group state and Retreat's destination survive moves within a place.
      for (flag = 0x300; flag <= 0x3ff; flag++)
        GameFlag_ClearBitFar(flag);
      GameFlag_SetBitFar(FLAG_SHOW_LOCATION_NAME);
      gGameState.encounterSteps = 0;
      gGameState.hazardSteps = 0;
      GameFlag_ClearBitFar(FLAG_VENUS_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_MERCURY_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_MARS_LIGHTHOUSE_LIT);
      GameFlag_ClearBitFar(FLAG_JUPITER_LIGHTHOUSE_LIT);
      gGameState.retreatScene = gGameState.scene;
      gGameState.retreatEntrance = gGameState.entrance;
    }
    for (flag = 0x80; flag <= 0xdf; flag++)
      GameFlag_ClearBitFar(flag);
    GameFlag_ClearBitFar(FLAG_LINK_BATTLE);
    GameFlag_ClearBitFar(0x144);
    GameFlag_ClearBitFar(0x161);
    GameFlag_ClearBitFar(0x123);
    GameFlag_ClearBitFar(0x11c);
    gGameState.returnCue = -1;
  }
  gGameState.sceneGroup = group;
  GameFlag_SetBitFar((group & 0x7f) + 0x180);
  // Flag 0x123 marks scenes of variant 2.
  gGameState.sceneVariant = SCENE_RECORD(gGameState.scene)->variant;
  if (gGameState.sceneVariant == 2)
    GameFlag_SetBitFar(0x123);
  GameFlag_RefreshLureCapFar();
}

// Starts the current scene's field music, chosen by
// Scene_ResolveInteractionResult.
void Audio_PlayCueFromEventWork(void) { AudioCommand_PlayFar(gGameState.cue); }

//--------------------------------------------------------------------------
// Random encounters: choosing the encounter zones for where the party is, and
// drawing an enemy group from a zone.
//--------------------------------------------------------------------------

// A field scene's encounter zones (gBattle_LocationRules, location_rules.s),
// ended by scene -1.
struct BattleLocationRule {
  s16 scene;
  s16 entrance;              // -1 for any entrance
  s16 flag : 15;             // -1, or a flag that turns the rule off once set
  s16 firstZoneOnGround : 1; // the first zone is for ground 0, not 1
  s16 firstZone;             // the first of up to seven consecutive zones
};

extern const struct BattleLocationRule gBattle_LocationRules[];

// The world map's encounter zone for each kind of terrain
// (gWorldMap_EncounterRules, zonedata.c), ended by kind -1.
typedef struct TerrainRule {
  s16 kind;     // the terrain kind of the map block
  s16 behavior; // the terrain's behavior, or -1 for any
  s16 flag;     // -1, or a flag that turns the rule off once set
  s16 zone;
} TerrainRule;

// An encounter zone (encounter.tsv).
enum { ENCOUNTER_GROUP_COUNT = 8 };

struct EncounterZone {
  u16 rate;                           // the base encounter rate
  u16 level;                          // the party level the zone is made for
  u16 enemies[ENCOUNTER_GROUP_COUNT]; // the enemy groups that can appear
  u8 weights[ENCOUNTER_GROUP_COUNT];  // each group's relative chance
};

// Chooses the current scene's encounter zones, one for each kind of ground
// the party can stand on (gEventWork->encounterGroups, read by
// EffectRuntime_LookupByTableEntry), from the first rule matching the scene
// and entrance. Ground 0 has none, and grounds 1 to 7 take consecutive zones
// from the rule's first one. With useRules 0, or no matching rule, no ground
// has encounters. Also resets the encounter counters and chooses the battle
// backdrop.
void Encounter_SelectSceneZones(s32 useRules) {
  struct EventWork *work;
  s32 scene;
  s32 entrance;
  u8 *groundZones;
  const struct BattleLocationRule *rule;
  s32 zone;
  s16 firstZoneOnGround;
  u32 i;

  work = gEventWork;
  scene = gGameState.scene;
  entrance = gGameState.entrance;
  groundZones = work->encounterGroups;
  rule = gBattle_LocationRules;
  zone = 0;
  firstZoneOnGround = 0;
  if (useRules != 0) {
    while (rule->scene != -1) {
      if (rule->scene == scene &&
          (rule->entrance == -1 || rule->entrance == entrance)) {
        if (rule->flag == -1 || GameFlag_TestFar(rule->flag) == 0) {
          zone = rule->firstZone;
          firstZoneOnGround = rule->firstZoneOnGround;
          break;
        }
      }
      rule++;
    }
  }

  // Ground zero is normally safe; the rule can move the first zone onto it.
  *groundZones++ = 0;
  for (i = 0; i <= 6; i++) {
    *groundZones++ = zone;
    if (zone != 0)
      zone++;
  }
  if (firstZoneOnGround != 0) {
    work->encounterGroups[0] = work->encounterGroups[1];
    work->encounterGroups[1] = 0;
  }
  work->encounterBias = 0;
  work->encounterThreshold = 0x100000;
  Battle_SelectSceneBackdrop();
}

// Returns the world map encounter zone for the terrain at pos (0 for none),
// and chooses the battle backdrop for that terrain.
s32 BattleFx_LookupResult(void *pos) {
  s32 kind;
  TerrainRule *rule = gWorldMap_EncounterRules;
  s32 behavior = GetWorldMapTerrainBehaviorFar(pos, &kind);
  s32 zone = 0;

  while (rule->kind != -1) {
    if (rule->kind == kind &&
        (rule->behavior == -1 || rule->behavior == behavior) &&
        (rule->flag == -1 || GameFlag_TestFar(rule->flag) == 0)) {
      zone = rule->zone;
      break;
    }
    rule++;
  }
  Battle_SelectTerrainBackdrop(behavior);
  return zone;
}

// Advances the encounter counter for zone and returns the enemy group to
// fight, or 0 for no battle. steps (16.16) scales how far the counter moves;
// a battle starts once it reaches encounterThreshold. Flags 352 and 353 and
// gGameState.cloaked prevent battles. Flag 351 skips the checks and the
// counting and draws a group at once, from an entry this function never sets.
s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps) {
  struct EventWork *work;
  struct EncounterZone *entry;
  s32 rate;
  s32 level;
  s32 bias;
  s32 step;
  s32 total;
  s32 sum;
  s32 pick;
  s32 i;

  work = gEventWork;
  if (GameFlag_TestFar(351) == 0) {
    if (GameFlag_TestFar(352) != 0 || GameFlag_TestFar(353) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gGameState.cloaked != 0)
      return 0;
    entry = (struct EncounterZone *)gEncounter_EnemyGroupTable + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;
    // With flag 5 set, there are no encounters while unit 5 has more than 130
    // experience points.
    if (GameFlag_TestFar(5) != 0) {
      level = UnitGetFar(5)->experience;
      if (level > 130)
        return 0;
    }

    // Each level (up to five) the party's average is above the zone's raises
    // the rate by 5; with Avoid active such a zone has no encounters at all.
    level = Party_GetAverageLevelFar() - entry->level;
    if (level < 0)
      level = 0;
    if (level > 5)
      level = 5;
    if (level > 0 && gGameState.avoidDistance != 0)
      return 0;
    rate += level * 5;

    // A random bias, the sum of four random numbers, varies the distance
    // between battles; it is drawn again after each battle.
    bias = work->encounterBias;
    if (bias == 0) {
      s32 a = Random16();
      s32 b = Random16();
      s32 c = Random16();

      bias = (a - b + c - (s32)Random16()) / 2;
      work->encounterBias = bias;
    }
    step =
        Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
    total = gGameState.encounterSteps += Iwram_MulQ16(step, steps);
    if (total < work->encounterThreshold)
      return 0;
  }

  // Draw one of the eight groups by weight.
  work->encounterBias = 0;
  sum = 0;
  for (i = 0; i < ENCOUNTER_GROUP_COUNT; i++)
    sum += entry->weights[i];
  if (sum == 0)
    return 0;
  pick = (Random16() * sum) >> 16;
  pick -= entry->weights[0];
  i = 0;
  if (pick >= 0) {
    do {
      i++;
      if (i >= ENCOUNTER_GROUP_COUNT)
        break;
      pick -= entry->weights[i];
    } while (pick >= 0);
  }
  sum = entry->enemies[i];
  BattleFx_SelectBattleCue(zone, i);
  return sum;
}

// Rolls a random battle on the encounter zone of the ground underfoot.
// terrain: the terrain number, 0 for ordinary ground.
// steps: the step weight, passed on to Encounter_SelectEnemyGroup.
// Returns the enemy group to fight, or 0 for no battle.
s32 Encounter_RollTerrain(u32 terrain, s32 steps) {
  u8 *zone;

  zone = gEventWork->encounterGroups;
  return Encounter_SelectEnemyGroup(zone[terrain], steps);
}

// Rolls a random battle on the world map, taking the zone from the tile at pos.
// Returns the enemy group to fight, or 0 for no battle.
s32 Encounter_RollWorldMap(s32 pos, s32 steps) {
  return Encounter_SelectEnemyGroup(BattleFx_LookupResult((void *)pos), steps);
}

// Returns the enemy group in slot (0 to 7) of zone; 14 halfwords make a zone.
u16 BattleFx_GetWeightedResult(s32 zone, s32 slot) {
  return gEncounter_EnemyGroupTable[zone * 14 + slot + 2];
}

// Returns the enemy group of Djinni battle djinni, which the table gives as
// a zone and slot.
s16 BattleFx_GetPhaseResult(s32 djinni) {
  u16 *zoneAndSlot;

  zoneAndSlot = (u16 *)&gEncounter_AreaEntryTable[djinni * 2];
#ifdef UBFIX
  return BattleFx_GetWeightedResult(zoneAndSlot[0], zoneAndSlot[1]);
#else
  // UB: no return statement; the result of the call is left in r0.
  BattleFx_GetWeightedResult(zoneAndSlot[0], zoneAndSlot[1]);
#endif
}

// A rule choosing a scene's field music (gScene_InteractionRuleTable,
// scene_rules.s), ended by id -1.
struct SceneInteractionEntry {
  s16 id;            // a scene, or a scene group
  s16 entrance : 15; // -1 for any entrance
  u16 idIsScene : 1; // 1 when id is a scene, 0 when it is a group
  s16 flag;          // -1, or a flag that must be set
  s16 music;
};

extern const struct SceneInteractionEntry gScene_InteractionRuleTable[];

// Chooses the current scene's field music (gGameState.cue) from the first rule
// matching the scene or its group and the entrance; music 18 when none does.
void Scene_ResolveInteractionResult(void) {
  s16 music = 18;
  s16 scene = gGameState.scene;
  s16 entrance = gGameState.entrance;
  s16 group = gGameState.sceneGroup;
  const struct SceneInteractionEntry *rule = gScene_InteractionRuleTable;

  for (; rule->id != -1; rule++) {
    if (rule->idIsScene) {
      if (rule->id != scene)
        continue;
      if (rule->entrance != -1 && rule->entrance != entrance)
        continue;
      if (rule->flag != -1 && GameFlag_TestFar(rule->flag) == 0)
        continue;
      music = rule->music;
      break;
    }
    if (rule->id != group)
      continue;
    if (rule->entrance != -1 && rule->entrance != entrance)
      continue;
    if (rule->flag != -1 && GameFlag_TestFar(rule->flag) == 0)
      continue;
    music = rule->music;
    break;
  }

  gGameState.cue = music;
}

// A rule naming the place a scene is in (gScene_AreaNameRules, scene_rules.s),
// ended by id -1.
struct SceneAreaNameRule {
  s16 id;             // a scene, or a scene group
  s16 entrance : 15;  // -1 for any entrance
  u16 useSceneId : 1; // 1 when id is a scene, 0 when it is a group
  s32 areaNameIndex;  // the place name
};

extern const struct SceneAreaNameRule gScene_AreaNameRules[];

// Returns the place name of scene and entrance, from the first rule matching
// the scene or its group; 0 when none does.
s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance) {
  const struct SceneAreaNameRule *rule = gScene_AreaNameRules;
  s32 name = 0;
  s32 group = BattleFx_GetResourceGroup(scene);

  while (rule->id != -1) {
    if (rule->useSceneId) {
      if (rule->id == scene) {
        if (rule->entrance == -1 || rule->entrance == entrance) {
          name = rule->areaNameIndex;
          break;
        }
      }
    } else {
      if (rule->id == group) {
        if (rule->entrance == -1 || rule->entrance == entrance) {
          name = rule->areaNameIndex;
          break;
        }
      }
    }
    rule++;
  }
  return name;
}

// Sets the scene the party returns to after a defeat (gGameState.savedScene
// and savedEntrance), from the first rule of gParty_PairResolveRules that
// matches the current scene and entrance: {scene, entrance (-1 for any),
// return scene, return entrance}, ended by scene -1. Without a match the
// return point stays as it was.
void Scene_SetDefeatReturnPoint(void) {
  s16 scene = gGameState.scene;
  s16 entrance = gGameState.entrance;
  const s16 *rule = gParty_PairResolveRules;

  while (rule[0] != -1) {
    if (rule[0] == scene && (rule[1] == -1 || rule[1] == entrance)) {
      gGameState.savedScene = rule[2];
      gGameState.savedEntrance = rule[3];
      return;
    }
    rule += 4;
  }
}

// Returns the battle backdrop chosen for the current scene.
s16 Event_GetSpecialValue(void) { return gGameState.special; }

// Chooses the battle backdrop for the current scene. gBattle_BackdropScenes
// lists each backdrop (a word with bit 31 set) followed by the scenes that
// use it, ended by 0; a scene takes the backdrop listed last before it, and a
// scene not listed takes the last backdrop.
void Battle_SelectSceneBackdrop(void) {
  s32 target = gGameState.scene;
  s32 *table = gBattle_BackdropScenes;
  s32 entry = *table++;
  s32 backdrop; // unset only for a table that starts with the scene

  if (entry != 0 && entry != target) {
    do {
      if (entry & 0x80000000) {
        backdrop = entry & 0xFFFF;
      }
      entry = *table++;
    } while (entry != 0 && entry != target);
  }
  gGameState.special = backdrop;
}

// Chooses the battle backdrop for a world map terrain behavior.
void Battle_SelectTerrainBackdrop(s32 behavior) {
  u16 backdrop;

  switch (behavior - 1) {
  case 0:
    backdrop = (u32)&ResourceId_GrasslandBackdrop;
    break;
  case 1:
    backdrop = (u32)&ResourceId_ForestBackdrop;
    break;
  case 2:
    backdrop = (u32)&ResourceId_DesertBackdrop;
    break;
  case 3:
  case 6:
    backdrop = (u32)&ResourceId_BeachBackdrop;
    break;
  case 4:
  case 5:
    backdrop = (u32)&ResourceId_SnowfieldBackdrop;
    break;
  default:
    backdrop = (u32)&ResourceId_SolSanctumBackdrop;
    break;
  }
  gGameState.special = backdrop;
}

// Chooses the battle music for slot of zone (gGameState.sceneCue).
// gEncounter_BattleMusic lists each song (with bit 15 set) followed by the
// (zone << 4) + slot keys that use it, ended by 0; a key takes the song listed
// last before it. Link battles always use music 18.
void BattleFx_SelectBattleCue(s32 zone, s32 slot) {
  u16 key = (zone << 4) + slot;
  const u16 *table = gEncounter_BattleMusic;
  u16 music;
  u16 value;

  if (GameFlag_TestFar(FLAG_LINK_BATTLE)) {
    music = 18;
  } else {
    for (;;) {
      value = *table++;
      if (value == 0)
        break;
      if (value == key)
        break;
      if (value & 0x8000)
        music = value & 0xfff;
    }
  }
  gGameState.sceneCue = music;
}

// Returns the sprite of character, which is the character itself except that
// FLAG_CHILDHOOD gives characters 0 and 1 sprites 0x12 and 0x13, and flag 0x21
// gives character 0 sprite 0x11.
s32 Party_RemapCharacterIdByFlags(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= 8) {
    if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0) {
      if (sprite == 0) {
        sprite = 0x12;
      }
      if (sprite == 1) {
        sprite = 0x13;
      }
    } else if ((GameFlag_TestFar(0x21) != 0) && (sprite == 0)) {
      sprite = 0x11;
    }
  }
  return sprite;
}

// Returns the sprite of character as Party_RemapCharacterIdByFlags would with
// FLAG_CHILDHOOD set to childhood (as recorded in a save file's summary).
s32 Party_RemapCharacterIdIfEnabled(s32 character, s32 childhood) {
  s32 sprite;

  sprite = character;
  if ((sprite <= 8) && (childhood != 0)) {
    if (sprite == 0) {
      sprite = 0x12;
    }
    if (sprite == 1) {
      sprite = 0x13;
    }
  }
  return sprite;
}

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

extern const struct ScenePlacement gField_DefaultPlacements[2];

// Creates the actors of a scene's placement table (ended by sprite -1), and
// records the table among the scene's tables. Sprites 0 to 7 are the party's
// characters and take their own actor slot; other placements take the actor
// slots from slot on, one each, up to slot 65. A placement whose condition
// does not hold is skipped, and so, outside the world map, is one with a
// condition from 48 to 127 whose condition + 80 does not hold. An actor that
// already exists is only moved back to its place, unless the party keeps its
// position.
void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot) {
  struct FieldStepWork *work;
  struct ObjectRuntime *object;
  struct ObjectRuntime *previous;
  struct AnimationObject *sprite;
  s32 i;
  s32 index;
  s32 character;
  s32 condition;
  u8 resource;
  u16 id;

  work = (struct FieldStepWork *)gEventWork;
  for (i = 0; i < 4; i++) {
    if (work->tables[i] == entry)
      break;
    if (work->tables[i] == 0) {
      work->tables[i] = entry;
      break;
    }
  }
  for (id = entry->sprite; (s16)id != -1 && slot <= 65;
       entry++, id = entry->sprite) {
    if (entry->sprite <= 7)
      index = entry->sprite;
    else if (entry->sprite <= 0x2705)
      index = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    if ((u32)(condition - 48) <= 79 && work->mode != MODE_WORLD_MAP &&
        !GameFlag_IsConditionActive(condition + 80))
      continue;
    character = Party_RemapCharacterIdByFlags(entry->sprite);
    object = ObjectTable_Get(index);
    if (object == 0) {
      object = Object_CreateFar(character, entry->x, entry->y, entry->z);
      // Placement flag 1: share the previous actor's sprite resource.
      if (entry->flags & 1) {
        previous = ObjectTable_Get(index - 1);
        if (previous->animationKind == 1 && object->animationKind == 1) {
          sprite = previous->animation;
          sprite->displayFlags |= 1;
          resource = sprite->slot;
          sprite = object->animation;
          sprite->displayFlags |= 1;
          Resource_ResetEntry(sprite->slot);
          sprite->slot = resource;
        }
      }
      if (GameFlag_TestFar(0x21) && (u32)(character - 0x12) <= 1)
        ObjectDispatch_RegisterChildMetadataFar((struct DispatchObject *)object,
                                                226);
    } else if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
      Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
    }

    if (object) {
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
      // Start each idle animation at a random frame.
      if (object->animationKind == 1 && (sprite = object->animation) != 0)
        sprite->lastNo = ((u32)(Random16()) % (u32)30);
      object->angle = entry->facing;
      object->unk59 = 1;
      ObjectMotion_SetActionCallback(object, entry->behavior);
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
      ((struct ScriptObjectRuntime *)object)->homeX = object->x / 0x10000;
      ((struct ScriptObjectRuntime *)object)->homeZ = object->z / 0x10000;
      if (object->y != 0) {
        object->flags = 4;
        object->y += 0x8000;
      }
      if (work->mode == MODE_WORLD_MAP) {
        // On the world map actors are drawn at three quarters size.
        object->flags &= 0xfe;
        if (!GameFlag_TestFar(0x21))
          sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
      } else {
        object->terrainHeight =
            Map_GetTerrainHeightFar(0, object->x, object->z);
        object->y += object->terrainHeight;
      }
      object->unk23 = 1;
    }
    work->actors[index] = object;
  }
}

// Destroys the actor in slot index, if there is one.
void ObjectTable_DestroyAtIndex(s32 index) {
#ifdef UBFIX
  struct FieldWork *work = (struct FieldWork *)gEventWork;
#else
  struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;
#endif
  void *object = ObjectTable_Get(index);

  if (object != NULL) {
    ObjectDispatch_ReleaseFar(object);
#ifdef UBFIX
    {
      void *empty = NULL;

      // Get also accepts words past the actor array, within the field work.
      memcpy((u8 *)work + 0x14 + (u32)index * sizeof(empty), &empty,
             sizeof(empty));
    }
#else
    work->actors[index] = NULL;
#endif
  }
}

// Creates a scene's actors on entering it: the party leader where the game
// state puts it, then the scene's placement table from actor slot 8, and the
// camera that follows the leader. A leader entering above ground where its cell
// and the cell north of it have collision code 0xfd is raised by the ground
// height of the cell north of it less 32 pixels, and takes movement mode 1.
void ObjectTable_ResetForObject(struct ScenePlacement *table) {
  struct FieldStepWork *work;
  struct ScenePlacement *entry;
  struct ObjectRuntime *object;
  struct ObjectRuntime *camera;
  struct MapCell *cell;
  struct MapCell *above;
  s32 leader;
  s32 pos;
  s32 height;

  work = (struct FieldStepWork *)gEventWork;
  leader = gGameState.selectedActor;
  // A two-placement table for the leader, built in the event work at 0x200.
  entry = (struct ScenePlacement *)((u8 *)work + 0x200);
  entry[0] = gField_DefaultPlacements[0];
  entry[1] = gField_DefaultPlacements[1];
  for (pos = 0; pos < 4; pos++)
    work->tables[pos] = 0;
  ObjectTable_ClearAll();
  entry->condition = -1;
  entry->sprite = leader;
  entry->x = gGameState.x;
  entry->y = 0;
  entry->z = gGameState.z;
  entry->facing = gGameState.heading;
  Event_SpawnObjectTable(entry, leader);
  Event_SpawnObjectTable(table, ACTOR_FIRST_PLACED);

  object = work->actors[leader];
  object->terrainId = gGameState.turn;
  pos = (object->x / 0x100000) + (object->z / 0x100000) * 128;
  cell = &MAP_CELLS[pos];
  above = &MAP_CELLS[pos - 128];
  if (gGameState.y != 0 && cell->collisionCode == 0xfd &&
      above->collisionCode == 0xfd) {
    gGameState.movementMode = 1;
    height =
        Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) - 0x200000;
    object->y += height;
    object->terrainHeight = object->y;
    object->flags = 0;
    ObjectDispatch_SetSingleChildFlagsFar(object, 0);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 12);
  } else {
    gGameState.movementMode = 0;
  }

  camera = Object_CreateFar(0x8000, object->x, object->y, object->z);
  camera->terrainHeight = object->terrainHeight;
  ObjectDispatch_InitFromTable4WithArgumentFar((struct DispatchObject *)camera,
                                               (s32)object);
  if (work->mode == MODE_WORLD_MAP) {
    struct AnimationEntry *meta =
        (struct AnimationEntry *)ResourceMetadata_RegisterFar(object->animation,
                                                              23);
    meta->param = 15;
    meta->priority = 9;
  }
  ((struct MapScrollWork *)gMapWork[0])->origin = &camera->x;
  ((struct EventWork *)work)->viewCenter = (struct FieldActor *)camera;
}

// Returns the actor slot after the last placed actor (8 when there is none),
// or -1 when every slot up to 65 is taken.
s32 ObjectTable_FindLastActiveId(void) {
  struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;
  s32 result = ACTOR_FIRST_PLACED - 1;
  s32 index = ACTOR_FIRST_PLACED;
  struct ObjectRuntime **actor = &work->actors[ACTOR_FIRST_PLACED];

  do {
    struct ObjectRuntime *current = *actor++;

    if (current != 0) {
      result = index;
    }
    index++;
  } while (index <= 65);
  result++;
  if (result == 66) {
    result = -1;
  }
  return result;
}

// Returns the address of actor slot index in the event work.
void *ObjectTable_GetSlotAddress(u32 index) {
  return (u8 *)gEventWork + 0x14 + index * sizeof(void *);
}

//--------------------------------------------------------------------------
// Actors and the view: placement conditions from the map's view bounds, and
// clearing placed actors.
//--------------------------------------------------------------------------

#define FLAG_IN_VIEW 0x164     // kept clear, so a placement using it appears
#define FLAG_OUT_OF_VIEW 0x165 // kept set, so a placement using it does not

#define PLACED_MAX 58 // actor slots 8 to 65

// Gives each placement with condition 0 a condition saying whether it lies
// within the map's view bounds, so that only those inside appear.
void Scene_AssignViewFlags(struct ScenePlacement *entry) {
  struct MapScrollWork *view = gMapWork[0];

  GameFlag_ClearBitFar(FLAG_IN_VIEW);
  GameFlag_SetBitFar(FLAG_OUT_OF_VIEW);
  while (entry->sprite != -1) {
    /* FAKEMATCH: the condition read through a u16 keeps loop.c to one hoisted
     * bound address (view + 236; view + 244 is formed in the loop, as native).
     * 2026-10-13: entry->condition == 0, directly or through an s16 local,
     * hoists both bound addresses (differ 18); (u16)entry->condition == 0
     * loads with ldrh (differ 23). */
    u16 raw = entry->condition;
    s16 condition = raw;

    if (condition == 0) {
      s32 x = entry->x;
      s32 z = entry->z;

      if (view->minX <= x && x <= view->maxX && view->minY <= z &&
          z <= view->maxY)
        entry->condition = FLAG_IN_VIEW;
      else
        entry->condition = FLAG_OUT_OF_VIEW;
    }
    entry++;
  }
}

// Destroys the placed actors far from the view's centre: more than 160 pixels
// to either side, 200 pixels north or 100 pixels south. Actors at (0, 0) stay.
void BattleEffect_ClearOutOfBoundsObjects(void) {
  struct EventWork *work = gEventWork;
  struct FieldActor *center;
  s32 x;
  s32 z;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  struct FieldActor **slot;
  s32 mask;
  s32 i;

  center = work->viewCenter;
  x = center->x.fixed;
  left = x - 0xa00000;
  right = x + 0xa00000;
  z = center->z.fixed;
  top = z - 0xc80000;
  bottom = z + 0x640000;
  slot = work->placedActors;
  mask = ~1;

  for (i = PLACED_MAX - 1; i >= 0; i--) {
    struct FieldActor *actor = *slot;

    if (actor != NULL) {
      s32 actorX = actor->x.fixed;
      s32 actorZ = actor->z.fixed;

      if (actorX != 0 || actorZ != 0) {
        if (actorX < left || actorX > right || actorZ < top ||
            actorZ > bottom) {
          u8 *sprite;
          u8 *removed = &actor->active;
          u32 flags;

          // Hide the sprite, then destroy the actor.
          *removed = 1;
          sprite = (u8 *)actor->sprite;
          flags = sprite[29];
          sprite[29] = flags & mask;
          ObjectDispatch_ReleaseFar(actor);
          sprite = NULL;
          *slot = (struct FieldActor *)sprite;
        }
      }
    }

    slot++;
  }
}

// Destroys every placed actor and places the scene's actors again from its
// first placement table after the leader's.
void BattleEffect_ClearAllObjects(void) {
  struct EventWork *work = gEventWork;
  struct ScenePlacement *table;
  s32 i;

  for (i = 0; i < PLACED_MAX; i++) {
    struct FieldActor *actor = work->placedActors[i];

    if (actor != 0) {
      actor->active = 1;
      actor->sprite->unk1d0 = 0;
      ObjectDispatch_ReleaseFar(actor);
      work->placedActors[i] = 0;
    }
  }
  table = work->tables[1];
  work->tables[1] = 0;
  work->tables[2] = 0;
  work->tables[3] = 0;
  if (table != 0)
    Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

// Empties all 66 actor slots, without destroying the actors.
void ObjectTable_ClearAll(void) {
  struct FieldWork *work;
  struct FieldActor *zero;
  s32 index;

  zero = NULL;
  index = 65;
  work = (struct FieldWork *)gEventWork;
  do {
    work->actors[index] = zero;
    index--;
  } while (index >= 0);
}

// Returns the actor in slot index (0 to 65), or NULL. Indices up to 191 read
// past the actor slots into the rest of the event work; larger ones give NULL.
void *ObjectTable_Get(u32 index) {
  u8 *base = (u8 *)gEventWork;
  u32 offset;
#ifdef UBFIX
  void *object;
#endif
  if (index > 0xbf)
    return 0;
  offset = (index * 4) + 0x14;
#ifdef UBFIX
  // The legacy range includes non-pointer fields. Copy their representation
  // without reading them through an incompatible pointer lvalue.
  memcpy(&object, base + offset, sizeof(object));
  return object;
#else
  return *(void **)(base + offset);
#endif
}

// Saves up to 32 actors (only the party's 8 on the world map) into
// gObjectSnapshots, with each one's animation, sprite flags and priority, so
// that ObjectTable_Restore can put them back. The 32 bytes before
// gObjectSnapshots list the saved slots, ended by 255.
void ObjectTable_Snapshot(void) {
  struct ObjectRuntime *copy = gObjectSnapshots;
  u8 *animations = (u8 *)&gObjectSnapshots[32];
  u8 *spriteFlags = animations + 32;
  u8 *priorities = animations + 64;
  u8 *indices = (u8 *)gObjectSnapshots - 32;
  u32 count = 0;
  s32 limit = 66;
  struct ObjectRuntime *object;
  s32 i;
  u32 animation;
  u32 flags;
  u32 priority;

  if (((struct FieldWork *)gEventWork)->mode == MODE_WORLD_MAP)
    limit = 8;
  for (i = 0; i < limit; i++) {
    object = ObjectTable_Get(i);
    if (object == NULL)
      continue;
    *indices++ = i;
    Dma_Set(object, copy, 0x84000000 | (sizeof(struct ObjectRuntime) / 4),
            REG_DMA3);
    if (object->animationKind == 1) {
      animation = ((struct AnimationObject *)object->animation)->lastNo;
      flags = ((struct FieldSprite *)object->animation)->flags;
      priority = ((struct FieldSprite *)object->animation)->priority;
    } else {
      animation = 0;
      flags = 0;
      priority = 0;
    }
    *animations++ = animation;
    *spriteFlags++ = flags;
    *priorities++ = priority;
    count++;
    copy++;
    if (count > 31)
      break;
  }
  for (i = count; i < 32; i++)
    *indices++ = 255;
}

// Puts back the actors ObjectTable_Snapshot saved, and moves the camera to the
// party leader's height.
void ObjectTable_Restore(void) {
  struct ObjectRuntime *copy = gObjectSnapshots;
  u8 *indices = (u8 *)gObjectSnapshots - 32;
  u8 *animations = (u8 *)&gObjectSnapshots[32];
  u8 *spriteFlags = animations + 32;
  u8 *priorities = animations + 64;
  s32 count;
  struct ObjectRuntime *object;
  struct FieldSprite *sprite;
  struct ObjectRuntime *camera;
  s32 *view;
  s32 index;
  s32 animation;
  s32 priority;
  s32 y;
  s32 *selected = &gGameState.selectedActor;
  struct EventWork **work = &gEventWork;
  struct MapScrollWork **map = (struct MapScrollWork **)gMapWork;

  count = 0;
  index = *indices++;
  while (index != 255) {
    object = ObjectTable_Get(index);
    if (object != NULL) {
      sprite = object->animation;
      Dma_Set(copy, object, 0x84000000 | (sizeof(struct ObjectRuntime) / 4),
              REG_DMA3);
      animation = *animations;
      if (animation != 0)
        ObjectDispatch_ApplyArgumentToChildrenFar(object, animation);
      ObjectDispatch_SetSingleChildFlagsFar(object, *spriteFlags);
      priority = *priorities;
      sprite->priority = priority;
      sprite->secondPriority = priority;
      object->animation = sprite;
      if (index == *selected) {
        camera = (struct ObjectRuntime *)(*work)->viewCenter;
        view = (*map)->origin;
        y = object->y;
        camera->terrainHeight = y;
        camera->y = y;
        view[1] = y;
        ObjectDispatch_InitFromTable6Far(object);
      }
    }
    copy++;
    animations++;
    spriteFlags++;
    priorities++;
    count++;
    if (count > 31)
      break;
    index = *indices++;
  }
}

// A scene region's bounds, in pixels, as 16.16 fixed point.
#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

// The fields of struct BattleUnit that Field_ProcessStep reads (see its tag).
struct StepOwner {
  u8 unitFieldsBeforeHp[0x38];
  s16 hp;
  u8 unitFieldsBeforePoison[0x131 - 0x3a];
  u8 poison;
};

// The collision codes of map cells that mean something when stepped on.
enum StepTileEvent {
  STEP_EVENT_FIRST = 1, // 1 to 239: the scene's step events
  STEP_EVENT_LAST = 239,
  STEP_TERRAIN_FIRST = 240, // 240 and 241: ground with its own encounters
  STEP_TERRAIN_LAST = 241,
  STEP_EVENT_HAZARD = 250, // ground that hurts the party over time
  STEP_WARP_FIRST = 252,   // 252 to 254: warps
  STEP_WARP_LAST = 254
};

#define STEP_FLAG_DOUBLE_SPEED 0x167

#define STEP_CUE_EFFECT 139

// Clears the field's twelve pending requests (see struct FieldWork).
void Field_ClearRequests(void) {
  short zero;
  struct FieldWork *state;
  zero = 0;
  state = (struct FieldWork *)gEventWork;
  state->action = zero;
  state->input = 0;
  state->exit = 0;
  state->menu = 0;
  state->examine = 0;
  state->start = 0;
  state->descriptor = 0;
  state->tile = 0;
  state->battle = 0;
  state->command = 0;
  state->item = 0;
  state->event = 0;
}

// Returns the sum of the field's twelve pending requests (counting the actor
// event request twice): non-zero while any request is pending.
s32 Field_CountRequests(void) {
  struct FieldWork *state = (struct FieldWork *)gEventWork;

  return state->action + state->input + state->exit + state->menu +
         state->examine + state->start + (s16)state->descriptor + state->tile +
         (s16)state->descriptor + state->battle + state->command + state->item +
         state->event;
}

// Returns the collision code of the map cell 16 pixels ahead of the party
// leader (a world map block on the world map), or 0 without a leader.
s32 Field_GetCollisionAheadOfLeader(void) {
  struct FieldWork *runtime;
  struct ObjectRuntime *object;
  struct MapState *map;
  struct FieldPosition position;
  u8 *tile;
  s32 x;
  s32 y;

  runtime = WORK_SLOT(HEAP_SLOT_EVENT);
  object = ((struct FieldStepWork *)runtime)->actors[gGameState.selectedActor];

  map = WORK_SLOT(8);

  if (object == 0)
    return 0;

  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x100000, object->angle, &position);

  if (runtime->mode == MODE_WORLD_MAP) {
    // World map blocks are 32 pixels square, 32 to a row.
    u32 tileX;
    u32 tileY;

    x = position.x;
    if (x < 0)
      x += 0x1fffff;
    tileX = (x >> 21) & 31;

    y = position.z;
    if (y < 0)
      y += 0x1fffff;
    tileY = (y >> 21) & 31;

    tile = (u8 *)gMapBlocks + ((tileX + (tileY << 5)) << 2);
  } else {
    // Map cells are 16 pixels square, 128 to a row.
    tile = (u8 *)map->layers[0].cells;

    x = position.x;
    if (x < 0)
      x += 0xfffff;
    {
      u32 tileX = x >> 20;

      y = position.z;
      if (y < 0)
        y += 0xfffff;

      tile = (u8 *)((u32)tile + ((tileX + ((y >> 20) << 7)) << 2));
    }
  }

  return tile[2];
}

// Takes the exit of the first of the scene's exit regions that contains the
// position (x, y, z) and whose condition holds: records the region's exit
// value for Field_RunScene, plays the exit sound and starts leaving the scene.
// A do/while or while scan changes register allocation (51 text bytes in
// tbs-en, 2026-10-16); keep the sentinel backedge until that form matches.
void Field_CheckExitRegions(s32 positionX, s32 positionY, s32 positionZ) {
  s32 exitValue;
  struct FieldWork *runtime;
  s32 z;
  s32 y;
  s32 x;
  s16 condition;
  s16 minZ;
  s16 maxY;
  s16 minY;
  s32 maxZ;
  s16 maxX;
  s16 minX;
  const struct SceneRegion *region;

  x = positionX;
  y = positionY;
  z = positionZ;
  region = gOverlayArea.regions();
  if (region != 0 && (runtime = (struct FieldWork *)gEventWork,
                      minX = region->minX, minX != -1)) {
  loop:
    minY = region->minY;
    minZ = region->minZ;
    maxX = region->maxX;
    maxY = region->maxY;
    maxZ = region->maxZ;
    condition = region->condition;
    exitValue = region->value;
    if (GameFlag_IsConditionActive(condition) != 0 && y >= REGION_FIXED(minY) &&
        y < REGION_FIXED(maxY) && x >= REGION_FIXED(minX) &&
        x < REGION_FIXED(maxX) && z >= REGION_FIXED(minZ) &&
        z < REGION_FIXED(maxZ)) {
      runtime->exit = exitValue;
      AudioCommand_PlayFar(SOUND_MAP_EXIT);
      Leader_StopMoving();
      return;
    }
    runtime = (struct FieldWork *)gEventWork;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

// Handles one step of the party leader onto the cell at (x, y, z) of map
// layer: records step events and warps, takes exits, and while walking
// advances the encounter counter, regenerates PP, hurts the party with poison
// and hazardous ground, and wears Avoid off. When this hurts the party, the
// leader flinches and characters who fall are listed in the step work.
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z) {
  /* FAKEMATCH: needed to match: the StepOwner read prefix preserves the
   * party-load order and the 44-byte frame. Plain struct BattleUnit differs by
   * 2: its alias set ties the poison store to the active-owner read. */
  struct FieldStepWork *work = WORK_SLOT(HEAP_SLOT_EVENT);
  struct MapState *map = WORK_SLOT(8);
  s32 selected = gGameState.selectedActor;
  struct ObjectRuntime *actor = work->actors[selected];
  s16 hp[8];
  struct MapCell *tile;
  struct StepOwner *owner;
  u32 count;
  s32 hurt;
  u32 i;
  u32 event;
  s32 speed;
  s32 steps;
  s32 n;
  s32 sum;
  s32 full;

  hurt = 0;
  if (work == NULL)
    return;

  // Note each character's HP, to see who falls.
  count = Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++)
    hp[i] = UnitGetFar(gGameState.activeOwners[i])->hp;

  if (work->mode == MODE_WORLD_MAP) {
    tile = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) +
                                              (((z / 0x200000) & 31) << 5)];
  } else {
    if ((u32)layer <= 2)
      tile = map->layers[layer].cells;
    else
      tile = (struct MapCell *)gMapCellBuffer;
    tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
  }
  event = tile->collisionCode;
  work->previousTile = work->tile;
  work->tile = tile;
  if (event != 0)
    Field_CheckExitRegions(x, y, z);
  if (event >= STEP_EVENT_FIRST && event <= STEP_EVENT_LAST)
    work->stepEvent = event;
  if (event >= STEP_WARP_FIRST && event <= STEP_WARP_LAST)
    work->warpEvent = event;

  if (gGameState.movementMode == 0 && actor != NULL &&
      actor->targetX != ACTOR_NO_TARGET) {
    // Encounters: the zone comes from the world map terrain, or from the
    // ground the leader stands on.
    speed = actor->speedLimit;
    if (GameFlag_TestFar(STEP_FLAG_DOUBLE_SPEED))
      speed <<= 1;
    if (work->mode == MODE_WORLD_MAP) {
      full = 1;
      work->encounterEnemyGroup = Encounter_RollWorldMap((s32)&actor->x, speed);
    } else if (event == STEP_TERRAIN_FIRST || event == STEP_TERRAIN_LAST) {
      full = work->encounterFull[event - (STEP_TERRAIN_FIRST - 1)];
      work->encounterEnemyGroup =
          Encounter_RollTerrain(event - (STEP_TERRAIN_FIRST - 1), speed);
    } else {
      full = work->encounterFull[0];
      work->encounterEnemyGroup = Encounter_RollTerrain(0, speed);
    }

    // Every 0x10000 of distance walked (half as fast where full is 0), the
    // party regains PP and takes poison damage.
    steps = Iwram_MulQ16(work->encounterRate, actor->speedLimit);
    if (!full)
      steps /= 2;
    sum = work->encounterSteps + steps;
    work->encounterSteps = sum;
    if (sum > 0xffff) {
      n = sum / 0x10000;
      work->encounterSteps = sum & 0xffff;
      BattleParty_ApplyDrain(n, 0);
      if (BattlePlacement_UpdateTimedEntriesFar())
        AudioCommand_PlayFar(STEP_CUE_EFFECT);
      Event_ClearInvalidPackedValues();
      hurt = BattleParty_ApplyStatusDamage();
    }

    // Hazardous ground: entering it starts the count half way.
    if (gGameState.hazardMode == 0 && event == STEP_EVENT_HAZARD) {
      if (work->previousTile->collisionCode == STEP_EVENT_HAZARD)
        gGameState.hazardSteps += actor->speedLimit / 0x10000;
      else
        gGameState.hazardSteps = gGameState.hazardLimit / 2;
    }

    // Avoid wears off with distance, outside scenes of variant 2.
    if (gGameState.avoidDistance != 0 && gGameState.sceneVariant != 2) {
      gGameState.avoidDistance -= actor->speedLimit;
      if (gGameState.avoidDistance <= 0) {
        gGameState.avoidDistance = 1;
        if (work->psynergyRequest == 0)
          work->psynergyRequest = 0x2096;
      }
    }
  }

  // In hazard mode 1 the count rises each step, with warnings half way and at
  // the limit; at the limit the party takes the hazard's damage.
  if (gGameState.hazardMode == 1) {
    gGameState.hazardSteps++;
    if (gGameState.hazardSteps == gGameState.hazardLimit / 2)
      BattleFx_ConfigureLinkedObject(selected, 0x101);
    if (gGameState.hazardSteps == gGameState.hazardLimit)
      BattleFx_ConfigureLinkedObject(selected, 0x100);
  }
  if (gGameState.hazardSteps >= gGameState.hazardLimit) {
    n = gGameState.hazardDamage;
    gGameState.hazardSteps = 0;
    BattleParty_ApplyHealthDelta(-(n & 255), n & 0x100);
    hurt++;
  }

  if (hurt) {
    work->fallenCount = 0;
    work->standingCount = 0;
    actor->velocityY = 0x40000;
    BattleFx_ConfigureLinkedObject(selected, 0x102);
    for (i = 0; i < count; i++) {
      owner = (struct StepOwner *)UnitGetFar(gGameState.activeOwners[i]);
      if (owner->hp > 0) {
        work->standingCount++;
      } else if (hp[i] != 0) {
        work->fallen[work->fallenCount++] = gGameState.activeOwners[i];
        work->raisedTrigger = 0xffff;
        owner->poison = 0;
      }
    }
  }
}

// Gives each character in the party amount PP (PP regenerates as the party
// walks).
void BattleParty_ApplyDrain(s32 amount, s32 unused) {
  s32 count = Party_CountActiveOwnersFar();
  s32 i;

  for (i = 0; i < count; i++)
    Unit_AddPpFar(gGameState.activeOwners[i], amount);
}

// Changes the HP of each character in the party by amount, or by amount
// percent of their maximum HP (at least 1) when percent is set, with a flash
// and a sound for damage or the recovery sound for healing.
void BattleParty_ApplyHealthDelta(s32 amount, s32 percent) {
  s32 count;
  s32 i;
  s32 value;
  struct BattleUnit *unit;

  if (amount < 0) {
    BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
    BattleFx_StartBufferInterpolation(4);
    if (amount < -10)
      AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
    else
      AudioCommand_PlayFar(133);
  } else {
    AudioCommand_PlayFar(SOUND_RECOVERY);
  }

  count = Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++) {
    unit = UnitGetFar(gGameState.activeOwners[i]);
    if (!percent) {
      value = amount;
    } else {
      value = unit->maxHp * amount / 100;
      if (value == 0) {
        value = amount;
        if (value < 0)
          value = -value;
      }
    }
    Unit_AddHpFar(gGameState.activeOwners[i], value);
  }
}

// Hurts each poisoned character in the party: poison takes a twentieth of
// their maximum HP and venom a tenth (at least 1). Returns 0 when nobody was
// hurt, else the worst status found (1 poison, 2 venom), after a flash.
s32 BattleParty_ApplyStatusDamage(void) {
  s32 worst = 0;
  s32 count = Party_CountActiveOwnersFar();
  s32 i;

  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = UnitGetFar(gGameState.activeOwners[i]);
    s32 amount;

    switch (unit->poison) {
    case 1:
      amount = -((unit->maxHp + 10) / 20);
      if (amount == 0)
        amount = -1;
      if (worst <= 0)
        worst = 1;
      break;
    case 2:
      amount = -((unit->maxHp + 5) / 10);
      if (amount == 0)
        amount = -1;
      if (worst <= 1)
        worst = 2;
      break;
    default:
      amount = 0;
      break;
    }
    Unit_AddHpFar(gGameState.activeOwners[i], amount);
  }

  if (worst != 0) {
    BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
    BattleFx_StartBufferInterpolation(4);
    AudioCommand_PlayFar(133);
  }

  return worst;
}

// Starts an event or menu on the world map: ends any colour ratio transition
// in progress, sets the camera's movement state to 1 and resumes the map's
// animation. Elsewhere it does nothing.
void Battle_SetObjectFlag5bWhenMode3(void) {
  struct FieldWork *work;
  struct DisplayTransitionState *transition;

  work = Runtime_AllocateBlock(0x1B, 0xCCC);
  if (work->mode == MODE_WORLD_MAP) {
    transition = Runtime_AllocateBlock(0x1F, 0x540);
    if ((transition != NULL) && (transition->step != 0)) {
      transition->start = 0;
      transition->end = 0;
      transition->duration = 1;
      transition->step = 0;
    }
    ((struct ObjectRuntime *)((struct EventWork *)work)->viewCenter)
        ->movementState = 1;
    Map_ResumeAnimationFar();
  }
}

// Ends what Battle_SetObjectFlag5bWhenMode3 started: on the world map, reloads
// the area's graphics and sets the camera's movement state back to 0.
void Battle_ClearObjectFlag5bWhenMode3(void) {
  struct FieldWork *work;

  work = Runtime_AllocateBlock(0x1B, 0xCCC);
  if (work->mode == MODE_WORLD_MAP) {
    Map_LoadAreaGraphicsFar();
    ((struct ObjectRuntime *)((struct EventWork *)work)->viewCenter)
        ->movementState = 0;
  }
}

struct MenuCtrl {
  u8 unk00[4];
  u16 wait;
};

// Runs the current field scene, which the party entered by entrance, until it
// leaves; returns the exit kind it left by (for Scene_TakeExit), or 999 when
// the party fell outside battle. Sets the scene up (map, actors, encounters,
// return points), runs its initialization, fades it in, then runs the field
// loop: the party walks (Field_ProcessStep each frame) until a request is
// made, and then the requests are handled until none is left.
s32 Field_RunScene(s32 entrance) {
  struct FieldWork *work;
  struct FieldActor *actor;
  s32 mode;
  s32 result = 0;
  s32 code;

  // Set up the scene.
  work = Runtime_AllocateBlock(27, sizeof(struct FieldWork));
  Dma_Fill32(0, work, sizeof(struct FieldWork));
  GameFlag_ClearBitFar(0x103);
  gGameState.defaultScene = gGameState.scene;
  gGameState.defaultEntrance = gGameState.entrance;
  gGameState.nextScene = -1;
  gGameState.nextEntrance = -1;
  gGameState.defeatScene = -1;
  gGameState.defeatEntrance = -1;
  Scheduler_ResetTaskTable();
  Djinn_ResolvePendingEvent(0);
  if (gGameState.map == (s32)&SceneId_Clear) {
    Map_InitializePerspectiveSceneFar();
    gGameState.perspectiveScene = 1;
    mode = MODE_WORLD_MAP;
    Encounter_SelectSceneZones(0);
  } else {
    Map_LoadLayeredSceneFar(gGameState.map);
    mode = 2;
    Encounter_SelectSceneZones(1);
  }
  work->mode = mode;
  ObjectSystem_InitializeFar(mode);
  UiWork_InitializeWithResourceCountersFar();
  Field_ClearRequests();
  work->events = gOverlayArea.events();
  Scene_ApplyEntrance();
  ObjectTable_ResetForObject(
      (struct ScenePlacement *)gOverlayArea.placements());
  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) != 0)
    ObjectTable_Restore();
  if (gGameState.pendingDjinnEvent != 0)
    FieldObject_PlaceSceneActors();
  if (mode == MODE_WORLD_MAP) {
    Map_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gGameState.x, gGameState.z);
  } else {
    Map_ApplyWorkOriginAndSpanFar();
  }
  Field_PlaceMapMarkers();
  BattleEffect_InitializeBuffers();
  BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
  work->transition = 0x100;
  work->transitionFrames = 16;
  work->transitionActive = 0;
  work->unk1b0 = 0x199;
  work->unk1b4 = 0;
  if (gGameState.sceneVariant == 2) {
    work->unk1b0 = 0;
    GameFlag_SetBitFar(0x144);
  }
  work->cells = Ram_MapCellBuffer;
  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) == 0) {
    Scene_SetDefeatReturnPoint();
    gGameState.hazardLimit = 16;
    gGameState.hazardMode = 0;
    gGameState.hazardDamage = 1;
    gGameState.pausedObject = -1;
    gGameState.cloaked = 0;
  }
  FieldFx_ResumeCloakCheck();
  work->queuedSound = -1;
  gOverlayArea.initialize();

  // The scene's initialization may already have taken an exit.
  code = work->exit;
  if (code != 0) {
    result = code;
    work->exit = 0;
  } else {
    GameFlag_ClearBitFar(FLAG_KEEP_PARTY_POSITION);
    // Fade in, show the place name and give what was left pending.
    if (Field_CountRequests() == 0) {
      if (work->transitionActive == 0) {
        DisplayTransition_Start(work->transition, work->transitionFrames);
        work->transitionActive = 1;
        PLTT_BACKDROP = 0;
        if (ObjectEffect_RunPendingFlagEvent() == 0)
          WaitFrames((work->transitionFrames + 1) / 2);
      }
      if (GameFlag_TestFar(FLAG_SHOW_LOCATION_NAME) != 0) {
        GameFlag_ClearBitFar(FLAG_SHOW_LOCATION_NAME);
        UiTimedNotice_CreateFar(gGameState.scene);
      }
      if (gGameState.pendingDjinnEvent != 0) {
        Djinn_ResolvePendingEvent(gGameState.pendingDjinnEvent, 1);
        gGameState.pendingDjinnEvent = 0;
      }
      if (gGameState.pendingItem != 0) {
        Battle_SetObjectFlag5bWhenMode3();
        if (Party_CheckItemLimit(gGameState.pendingItem) == 0)
          PartyInventory_GiveItem(gGameState.pendingItem, 0);
        Battle_ClearObjectFlag5bWhenMode3();
        gGameState.pendingItem = 0;
      }
    }

    for (;;) {
      // Note where the leader is, for the requests to use.
      GameFlag_SetBitFar(0x104);
      actor = work->actors[gGameState.selectedActor];
      ObjectDispatch_InitFromTable6Far(actor);
      gGameState.x = actor->x.fixed;
      gGameState.y = 0;
      gGameState.z = actor->z.fixed;
      gGameState.heading = actor->facing;
      gGameState.turn = actor->terrainId;

      // Handle the pending requests, most urgent first.
      while (Field_CountRequests() != 0) {
        s32 next = work->exit;

        if (next != 0) {
          result = next;
          work->exit = 0;
          goto done;
        }
        if (work->event != 0) {
          work->busy = 1;
          if (work->event == -1) {
            // Characters fell to poison or hazardous ground.
            u32 i;

            Leader_StopMoving();
            Battle_SetObjectFlag5bWhenMode3();
            for (i = 0; i < work->count; i++) {
              ObjectDispatch_ApplyArgumentToChildrenFar(actor, 22);
              UiWork_PushValueSlotFar(work->quantities[i], 1);
              UiText_ShowPositionedMessageAndWaitFar(MsgFieldStrengthExhausted,
                                                     1);
            }
            if (work->survived == 0) {
              // The whole party fell: back to the last sanctum with 1 HP.
              struct BattleUnit *vitals;

              if (gGameState.selectedActor == 0) {
                if (GameFlag_TestFar(0x20) != 0)
                  ObjectDispatch_ApplyArgumentToChildrenFar(actor, 21);
                else
                  ObjectDispatch_ApplyArgumentToChildrenFar(actor, 37);
              } else {
                ObjectDispatch_ApplyArgumentToChildrenFar(actor, 19);
              }
              AudioCommand_PlayFar(59);
              UiText_ShowPositionedMessageAndWaitFar(MsgFieldPartyAnnihilated,
                                                     1);
              vitals = UnitGetFar(gGameState.selectedActor);
              vitals->hp = 1;
              Unit_UpdateGauges(vitals);
              gGameState.scene = gGameState.savedScene;
              gGameState.entrance = gGameState.savedEntrance;
              Battle_ClearObjectFlag5bWhenMode3();
              result = EXIT_NONE;
              goto done;
            }
            Battle_ClearObjectFlag5bWhenMode3();
          } else if (work->event == -888) {
            Leader_StopMoving();
            Map_ShowWorldMap((s32)&ResourceId_WorldMapPicture);
          } else if (work->event == -889) {
            Leader_StopMoving();
            BattleFx_RunVisibilityTransition();
          } else {
            Scene_RunRaisedEvent(work->event);
          }
          work->busy = 0;
          work->event = 0;
        } else if (work->battle != 0) {
          // A random encounter: save the actors and go to the battle scene.
          Leader_StopMoving();
          ObjectTable_Snapshot();
          gGameState.returnCue = -1;
          gGameState.scene = SCENE_BATTLE;
          gGameState.entrance = work->battle;
          work->exit = EXIT_NONE;
          Field_StartEncounterTransition(work->battle);
          work->unk1a8 = 0;
          work->battle = 0;
          gGameState.encounterSteps = 0;
        } else if (work->action != 0) {
          work->busy = 1;
          Scene_RunTouchOrExit(work->action);
          work->busy = 0;
          work->action = 0;
        } else if (work->input != 0) {
          Field_RunPushTileEvent(work->input);
          work->input = 0;
        } else if (work->examine != 0) {
          // Examine the actor or tile ahead, or open the menu if neither has an
          // event.
          s32 object;

          UiTimedNotice_CloseIfActiveFar();
          object = BattleEffect_SelectNearbyObject(gGameState.selectedActor);
          mode = 0;
          if (object != -1) {
            mode = BattleFx_FindDescriptorWithOverride(object) != 0;
          }
          if (mode != 0) {
            work->descriptor = object | 0x1000;
            work->menu = 0;
          } else {
            mode = Object_GetTriggerTileAheadOfCurrent();
            if (mode != 0) {
              work->tile = mode;
              work->menu = 0;
            } else {
              work->menu = 1;
            }
          }
          work->examine = 0;
        } else if (work->menu != 0) {
          // The main menu (B + Select in debug mode opens the debug menu).
          UiTimedNotice_CloseIfActiveFar();
          Leader_StopMoving();
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
          Battle_SetObjectFlag5bWhenMode3();
          GameFlag_SetBitFar(0x106);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0 &&
              (gKeysHeld & KEY_SELECT) != 0) {
            Menu_RunSelectionWithCursorObjectFar();
          } else if (GameFlag_TestFar(0x107) != 0) {
            work->event = 250;
          } else {
            work->effectTimerRunning = 0;
            Menu_RunTopSelectionFar();
            work->effectTimerRunning = 1;
          }
          Battle_ClearObjectFlag5bWhenMode3();
          GameFlag_ClearBitFar(0x106);
          GameFlag_RefreshLureCapFar();
          work->menu = 0;
        } else if ((s16)work->descriptor != 0) {
          Battle_SetObjectFlag5bWhenMode3();
          Scene_TalkToActor(work->descriptor & 0xfff);
          Battle_ClearObjectFlag5bWhenMode3();
          work->descriptor = 0;
        } else if (work->tile != 0) {
          Battle_SetObjectFlag5bWhenMode3();
          Field_SearchTile(work->tile);
          Battle_ClearObjectFlag5bWhenMode3();
          work->tile = 0;
        } else if (work->command != 0) {
          UiTimedNotice_CloseIfActiveFar();
          Battle_SetObjectFlag5bWhenMode3();
          Field_UsePsynergy(work->command);
          Battle_ClearObjectFlag5bWhenMode3();
          work->command = 0;
        } else if (work->item != 0) {
          Battle_SetObjectFlag5bWhenMode3();
          Field_UseItem(work->item, work->itemArgument);
          Battle_ClearObjectFlag5bWhenMode3();
          work->item = 0;
        } else if (work->start != 0) {
          // The pause menu (in debug mode, B opens the debug menu and L the
          // palette editor).
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
          Leader_StopMoving();
          Battle_SetObjectFlag5bWhenMode3();
          GameFlag_SetBitFar(0x106);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0) {
            Menu_RunSelectionFar();
          }
          /* FAKEMATCH: the volatile read loads gDebugMode again for this second
             test, as the bytes do; read plainly in both tests, CSE reuses the
             first load and Field_RunScene differs 48, 961 instructions for 965.
             The other three reads of gDebugMode in this function need no
             qualifier. */
          else if (*(volatile u8 *)&gDebugMode != 0 &&
                   (gKeysHeld & KEY_L) != 0) {
            Debug_RunPaletteEditor();
          } else if (GameFlag_TestFar(0x107) != 0) {
            work->event = 250;
          } else {
            Battle_ResetEffectCounter();
            UiTimedNotice_CloseIfActiveFar();
            gGameState.returnCue = AudioCommand_GetSecondaryStateByteFar();
            if (GameFlag_TestFar(0x17e) == 0) {
              // Wait a frame first when fewer than 136 of the 512 resource
              // blocks are free.
              struct MenuCtrl *ctrl = (struct MenuCtrl *)gMenuCtrlWork;
              u8 *owner = gResourceBlockOwners;
              s32 free = 0;
              s32 n = 512;

              do {
                if (*owner++ == 0xff)
                  free++;
              } while (--n != 0);
              if (free - 136 < 0) {
                ctrl->wait = 1;
                WaitFrames(1);
              }
              Menu_RunWorkspaceResultLoopFar(0);
              ctrl->wait = 0;
            } else {
              UiText_ShowPositionedMessageAndWaitFar(MsgFieldPauseUnavailable,
                                                     1);
            }
          }
          Battle_ClearObjectFlag5bWhenMode3();
          GameFlag_ClearBitFar(0x106);
          work->start = 0;
        }
      }

      // Hand the leader back to the player, as climbing (mode 1), mode 2, on
      // the world map or walking, and step until the next request.
      GameFlag_ClearBitFar(0x104);
      actor = work->actors[gGameState.selectedActor];
      if (actor != NULL) {
        if (gGameState.movementMode == 2)
          ObjectDispatch_InitFromTable0Far(actor);
        else if (gGameState.movementMode == 1)
          ObjectDispatch_InitFromTable1Far(actor);
        else if (work->mode == MODE_WORLD_MAP)
          ObjectDispatch_InitFromTable3Far(actor);
        else
          ObjectDispatch_InitFromTable2Far(actor);
      }
      do {
        WaitFrames(1);
        actor = work->actors[gGameState.selectedActor];
        // In debug mode, flag 0x163 turns step processing off.
        if (gDebugMode == 0 || GameFlag_TestFar(0x163) == 0)
          Field_ProcessStep(actor->terrainId, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
      } while (Field_CountRequests() == 0);
    }
  }

done:
  if (work->transitionActive != 0) {
    DisplayTransition_Finish(work->transition, work->transitionFrames);
    work->transitionActive = 0;
    WaitFrames(work->transitionFrames);
  }
  Runtime_ReleaseHeapBlock(27);
  return result;
}

// A field actor (struct FieldActor), seen with its position as one vector.
struct Object {
  u8 pad0[6];
  u16 angle;
  struct EffectVector pos;
  u8 pad1[14];
  u8 kind;
  u8 pad2[0x2d];
  struct AnimationObject *sprite;
};

// One of the scene's entrances (gOverlayArea.entrances), ended by entrance -1:
// where the party appears, and the scroll bounds to use. -1 keeps a bound.
struct EntranceView {
  s16 entrance;
  s16 flag; // -1, or a flag that must be set
  s16 x;
  s16 y;
  s16 z;
  u16 heading;
  s16 unk0c;
  s16 left;
  s16 top;
  s16 right;
  s16 bottom;
  s16 unk16;
};

#define PALETTE ((u16 *)0x05000000)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define HEX_DIGIT 0xf0e0 // the tile of digit 0, in BG palette 15

// Returns the collision code of the map cell 16 pixels ahead of the party
// leader when there is something to search there: a tile code from 242 to 247
// whose ground is no more than 64 pixels above the leader, or a code the
// scene has a search event for. Returns 0 otherwise.
s32 Object_GetTriggerTileAheadOfCurrent(void) {
  u8 *state;
  u8 *map;
  struct Object *leader;
  struct EffectVector pos;
  u8 *cell;
  u8 *base;
  s32 code;
  s32 height;
  s32 result;

  result = 0;
  leader = ObjectTable_Get(gGameState.selectedActor);
  state = *(u8 **)Ram_EventWork;
  map = *(u8 **)Ram_MapWork;
  if (leader != 0) {
    pos.x = leader->pos.x;
    pos.y = leader->pos.y;
    pos.z = leader->pos.z;
    Vector_AddPolarOffset(0x100000, leader->angle, &pos);
    if (((struct FieldWork *)state)->mode == MODE_WORLD_MAP) {
      cell =
          Ram_MapBlocks +
          ((((pos.x / 0x200000) & 31) + (((pos.z / 0x200000) & 31) << 5)) << 2);
    } else {
      base = *(u8 **)(map + 0x130);
      cell = base + (((pos.x / 0x100000) + ((pos.z / 0x100000) << 7)) << 2);
    }
    code = cell[2];
    if (code >= 242 && code <= 247) {
      height = Map_GetTerrainHeightFar(leader->kind, pos.x, pos.z);
      if (height >= leader->pos.y && height <= leader->pos.y + 0x400000) {
        result = code;
      }
    } else if (Scene_FindEvent(EVENT_SEARCH, code) != 0) {
      result = code;
    }
  }
  return result;
}

// Places the party at the entrance it came in by (the first matching entry, or
// the scene's first one) unless it keeps its position, and sets the map's
// scroll bounds, at least a screen (240 x 160 pixels) wide and tall.
void Scene_ApplyEntrance(void) {
  struct MapScrollWork *work = gMapWork[0];
  s32 entrance = gGameState.entrance;
  struct EntranceView *view = (struct EntranceView *)gOverlayArea.entrances();
  s32 found = 0;

  while (view->entrance != -1) {
    if (view->entrance == entrance &&
        (view->flag == -1 || GameFlag_TestFar(view->flag))) {
      found = 1;
      break;
    }
    view++;
  }
  if (!found)
    view = (struct EntranceView *)gOverlayArea.entrances();

  if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
    gGameState.x = view->x << 16;
    gGameState.y = view->y << 16;
    gGameState.z = view->z << 16;
    gGameState.heading = view->heading;
    gGameState.turn = 0;
  }

  if (view->left != -1)
    work->minX = view->left << 16;
  if (view->top != -1)
    work->minY = view->top << 16;
  if (view->right != -1)
    work->maxX = view->right << 16;
  if (view->bottom != -1)
    work->maxY = view->bottom << 16;

  if (work->minX + (240 << 16) > work->maxX)
    work->minX = work->maxX - (240 << 16);
  if (work->minY + (160 << 16) > work->maxY)
    work->minY = work->maxY - (160 << 16);
}

// The debug palette editor (L on the pause menu in debug mode). Shows colours
// 1 to 15 of a palette with their red, green and blue values in hex. Left and
// right choose the colour, up and down the channel, L and R the palette (0 to
// 13); A raises the channel and B lowers it, Start makes the colour blink
// while held, and Select leaves.
void Debug_RunPaletteEditor(void) {
  s32 palette = 0;
  s32 channel = 1;
  s32 color = 1;
  u16 *map;
  u16 *p;
  u16 *colors;
  u32 tile;
  u32 value;
  u32 red;
  u32 green;
  u32 blue;
  u32 i;

  Resource_DecompressHalfwords(gDebug_PaletteSwatchTiles, (void *)0x06001a00);
redraw:
  // Column 0 shows the palette number and R, G, B; columns 1 to 15 show each
  // colour's swatch and its three channels.
  tile = (palette << 12) + 0xd1;
  colors = &PALETTE[palette * 16 + 1];
  map = EDITOR_MAP;
  p = map;
  *p = HEX_DIGIT + palette;
  p += 32;
  *p = 0xf000 | 'R';
  p += 32;
  *p = 0xf000 | 'G';
  p += 32;
  *p = 0xf000 | 'B';
  map++;
  for (i = 1; i < 16; i++) {
    p = map;
    *p = tile++;
    value = *colors++;
    p += 32;
    *p = (value & 31) + HEX_DIGIT;
    p += 32;
    *p = ((value >> 5) & 31) + HEX_DIGIT;
    p += 32;
    *p = ((value >> 10) & 31) + HEX_DIGIT;
    map++;
  }
  WaitFrames(1);
  for (;;) {
    if (gKeysRepeat & KEY_UP) {
      channel--;
      if (channel <= 0)
        channel = 3;
    }
    if (gKeysRepeat & KEY_DOWN) {
      channel++;
      if (channel > 3)
        channel = 1;
    }
    if (gKeysRepeat & KEY_LEFT) {
      color--;
      if (color <= 0)
        color = 15;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      color++;
      if (color > 15)
        color = 1;
    }
    if (gKeysRepeat & KEY_L) {
      palette--;
      if (palette < 0)
        palette = 13;
      goto redraw;
    }
    if (gKeysRepeat & KEY_R) {
      palette++;
      if (palette > 13)
        palette = 0;
      goto redraw;
    }
    if (gKeysRepeat & KEY_A) {
      colors = &PALETTE[palette * 16 + color];
      value = *colors;
      red = value & 31;
      green = (value >> 5) & 31;
      blue = (value >> 10) & 31;
      if (channel == 1 && red < 31)
        red++;
      if (channel == 2 && green < 31)
        green++;
      if (channel == 3 && blue < 31)
        blue++;
      *colors = (blue << 10) | (green << 5) | red;
      goto redraw;
    }
    if (gKeysRepeat & KEY_B) {
      colors = &PALETTE[palette * 16 + color];
      value = *colors;
      red = value & 31;
      green = (value >> 5) & 31;
      blue = (value >> 10) & 31;
      if (channel == 1 && red != 0)
        red--;
      if (channel == 2 && green != 0)
        green--;
      if (channel == 3 && blue != 0)
        blue--;
      *colors = (blue << 10) | (green << 5) | red;
      goto redraw;
    }
    if (gKeysRepeat & KEY_START) {
      // Blink white, the colour, black, the colour, every 10 frames; red
      // counts the frames here.
      colors = &PALETTE[palette * 16 + color];
      value = *colors;
      red = 0;
      for (;;) {
        WaitFrames(1);
        if (!(gKeysHeld & KEY_START))
          break;
        if (red == 0)
          *colors = 0x7fff;
        if (red == 10)
          *colors = value;
        if (red == 20)
          *colors = 0;
        if (red == 30)
          *colors = value;
        if (++red >= 40)
          red = 0;
      }
      *colors = value;
    }
    if (gKeysRepeat & KEY_SELECT)
      break;
    gFrameCount; // a read whose value is not used
    WaitFrames(1);
  }
  Ui_LoadWindowGraphics();
  DebugText_Clear();
}

// Returns the placement of actor id in the scene's placement tables, or NULL:
// for a party character (0 to 7) the row with its sprite, and for a placed
// actor (8 on) the row it was created from, counting the placed rows in order.
struct ScenePlacement *BattleAction_FindDescriptor(s32 id) {
  struct ScenePlacement *entry;
  s32 i;
  s32 placed = ACTOR_FIRST_PLACED;

  for (i = 0; i < 4; i++) {
    entry = ((struct FieldWork *)gEventWork)->tables[i];
    if (entry == NULL)
      continue;
    if (id <= 7) {
      for (; (s16)entry->sprite != -1; entry++) {
        if ((s16)entry->sprite == id) {
#ifdef UBFIX
          return entry;
#else
          goto found;
#endif
        }
      }
    } else {
      for (; (s16)entry->sprite != -1; entry++) {
        if ((s16)entry->sprite > 7) {
          if (placed == id) {
#ifdef UBFIX
            return entry;
#else
            goto found;
#endif
          }
          placed++;
        }
      }
    }
  }
#ifdef UBFIX
  return NULL;
#else
found:
  // BUG: a missing actor leaves entry NULL if the final table is absent.
  if ((s16)entry->sprite == -1)
    return NULL;
  return entry;
#endif
}

// Returns whether an event condition holds: -1 always holds; a condition with
// bit 0x1000 holds while that flag is set, any other while its flag is clear.
s32 GameFlag_IsConditionActive(s32 condition) {
  s32 on;

  if (condition == -1) {
    return 1;
  }
  if (0x1000 & condition) {
    return GameFlag_TestFar(condition);
  }
  on = GameFlag_TestFar(condition);
  return on == 0;
}

// Returns 1 for a search event (kind 3) that holds whatever its condition: one
// with more flags than its kind, unless its result is a mimic battle (type
// 0x005 in its top 12 bits).
u32 Event_SearchIgnoresCondition(s32 flags, s32 result) {
  u32 tmp;
  s32 kind;
  u32 ret;

  if ((flags & 0xf) != EVENT_SEARCH)
    return 0;
  kind = flags & 0x1ff;
  ret = 0;
  if (kind != EVENT_SEARCH) {
    tmp = (result & 0xfff00000) ^ 0x500000;
    ret = tmp | (0U - tmp);
    ret >>= 31; // 1 when tmp is not 0
  }
  return ret;
}

// Returns the first of the scene's events of kind for target whose condition
// holds, or NULL. Flag 0x200 also needs the leader to have pushed against it
// for more than 12 frames (2 with flag 0x800); flag 0x400 needs the leader to
// face within an eighth of a turn of the direction in the top four bits.
struct EffectDescriptor *Scene_FindEvent(s32 kind, s32 target) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct EffectDescriptor *ev = (struct EffectDescriptor *)work->events;
  s32 flags;
  u32 facing =
      ((struct FieldActor *)ObjectTable_Get(gGameState.selectedActor))->facing;

  flags = ev->flags;
  while (flags != -1) {
    if ((flags & 0xf) == kind && ev->value == target &&
        (Event_SearchIgnoresCondition(flags, ev->result) != 0 ||
         (GameFlag_IsConditionActive(ev->condition) != 0 &&
          (flags = ev->flags, 1)))) {
      s32 ok = 0;
      s32 steps = 12;
      s32 need;

      if ((flags & 0x800) != 0)
        steps = 2;
      need = flags & (EVENT_TOUCH_WALKING | EVENT_TOUCH_FACING);

      switch (need) {
      case 0:
        ok = 1;
        break;
      case EVENT_TOUCH_WALKING:
        if (work->blockedSteps > steps) {
          ok = 1;
        }
        break;
      case EVENT_TOUCH_FACING:
        flags &= 0xf000;
        ok = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        break;
      case EVENT_TOUCH_WALKING | EVENT_TOUCH_FACING:
        if (work->blockedSteps > steps) {
          flags &= 0xf000;
          ok = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        }
        break;
      }
      if (ok) {
        return ev;
      }
    }
    ev++;
    if (ev->flags == -1)
      break;
    flags = ev->flags;
  }
  return 0;
}

// Runs the scene's initialization again. Nothing calls this.
void Runtime_CallWorkBlockEntry(void) { gOverlayArea.initialize(); }

// Returns the talk event of actor id: for the actor the party has stopped, its
// kind 7 event if it has one.
s32 BattleFx_FindDescriptorWithOverride(s32 id) {
  struct EffectDescriptor *ev = Scene_FindEvent(EVENT_TALK, id);
  s32 paused = gGameState.pausedObject;

  if (paused == id) {
    struct EffectDescriptor *alt = Scene_FindEvent(7, paused);

    if (alt != 0) {
      return (s32)alt;
    }
  }
  return (s32)ev;
}

// Talks to actor id: shows its event's message or calls its event function,
// with the actor stopped and turned as its placement says. For the actor the
// party has stopped, its kind 7 event comes first; without one, a talk event
// that is a function gives a random line from MsgPonderEllipsis ("...")
// instead. Returns 0 when an event ran, -1 when there was none.
s32 Scene_TalkToActor(s32 id) {
  struct ScenePlacement *placement = BattleAction_FindDescriptor(id);
  s32 ret = -1;
  struct ObjectRuntime *obj = (struct ObjectRuntime *)Object_GetById(id);
  struct EffectDescriptor *ev;
  s32 paused = 0;
  u32 savedAngle;
  u32 talkFacing;
  s32 talk = 0;

  if (gGameState.pausedObject == id) {
    paused = 1;
    ev = Scene_FindEvent(7, id);
    if (ev == 0) {
      ev = Scene_FindEvent(EVENT_TALK, id);
      talk = 1;
      if (ev == 0) {
        return -1;
      }
      if (ev->result >= 0x10000) {
        s32 no = BattleFx_GetFlags(id);
        u32 random = Random16();
        s32 message = MsgPonderEllipsis + no * 2 + (random * 2 >> 16);
        Battle_Reset();
        Event_SetMessageId(message);
        BattleEv_RunWait(id, 0);
        BattleFx_FinishAction();
        goto finish;
      }
    }
  } else {
    ev = Scene_FindEvent(EVENT_TALK, id);
  }

  if (ev != 0 && ev->result != 0) {
    if (!paused) {
      // Stop the actor; with talk facing 0, 1 or 3, also stop the leader and
      // link the two.
      u8 *busy = &obj->movementState;

      *busy = 1;
      ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)obj, 0);
      savedAngle = obj->angle;
      talkFacing = (u8)placement->talkFacing << 24;
      if (talkFacing <= (1 << 24) || talkFacing == (3 << 24)) {
        struct ObjectRuntime *leader =
            (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);

        leader->targetX = leader->x;
        leader->targetY = leader->y;
        leader->targetZ = leader->z;
        leader->velocityX = 0;
        leader->velocityY = 0;
        leader->velocityZ = 0;
        Object_LinkPair(id, gGameState.selectedActor, 0);
      }
    }
    // A result below 0x10000 is a message; above, a function to call.
    if (ev->result < 0x10000) {
      UiWork_SetBusyFlagsFar(talk);
      Battle_Reset();
      Event_SetMessageId(ev->result);
      BattleEv_RunWait(id, 0);
      BattleFx_FinishAction();
    } else {
      Leader_StopMoving();
      ((void (*)(s32))ev->result)(id);
    }
    if (!paused) {
      // An actor left idle (script command 16) goes back to what its talk
      // facing says: following the leader (3) or its old angle (1).
      if (obj->script[obj->step] == (void *)16) {
        if ((s8)placement->talkFacing == 3) {
          obj->linkedObject = Object_GetById(gGameState.selectedActor);
          obj->actionFlags |= 1;
          ObjectMotion_SetActionCallback(obj, gObjectMotion_LinkedActionScript);
        } else if ((s8)placement->talkFacing == 1) {
          obj->action = savedAngle;
          ObjectDispatch_InitializeFar(
              (struct DispatchObject *)obj,
              (const s32 *)gObjectMotion_StepAngleScript);
        }
      }
      obj->movementState = 0;
      ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)obj, 16);
    }
    ret = 0;
  }
finish:
  if (paused) {
    BattleFx_ResumeObject(gGameState.pausedObject);
    gGameState.pausedObject = -1;
  }
  return ret;
}

// Runs the scene's raised event (kind 6) for value: its message or its
// function. Returns 0 when an event ran, -1 when there was none.
s32 Scene_RunRaisedEvent(s32 value) {
  s32 result;
  s32 ret;
  struct EffectDescriptor *ev;

  ev = Scene_FindEvent(EVENT_RAISED, value);
  ret = -1;
  if (ev != NULL) {
    result = ev->result;
    if (result != 0) {
      if (result < 0x10000) {
        Battle_Reset();
        Event_SetMessageId(ev->result);
        BattleEv_RunWait(-1, 0);
        ret = 0;
        BattleFx_FinishAction();
      } else {
        ((void (*)(s32))result)(value);
        ret = 0;
      }
    }
  }
  return ret;
}

// Runs the touch event (kind 2) of what the leader touched, or else takes its
// exit (kind 1): plays the exit's sound (and effect) and records where it
// leads. Returns 0 when something ran, -1 when there was nothing.
s32 Scene_RunTouchOrExit(s32 value) {
  struct EffectDescriptor *touch;
  struct EffectDescriptor *exit;
  s32 kind;
  s32 ret;
  struct FieldWork *work;

  touch = Scene_FindEvent(EVENT_TOUCH, value);
  ret = -1;
  work = (struct FieldWork *)gEventWork;
  if ((touch != 0) && (touch->result != 0)) {
    if (touch->flags & EVENT_TOUCH_WALKING) {
      work->blockedSteps = 0;
    }
    if (touch->result < 0x10000) {
      Battle_Reset();
      Event_SetMessageId(touch->result);
      BattleEv_RunWait(-1, 0);
      ret = 0;
      BattleFx_FinishAction();
    } else {
      ((void (*)(s32))touch->result)(value);
      ret = 0;
    }
  } else {
    exit = Scene_FindEvent(EVENT_EXIT, value);
    if (exit != 0) {
      kind = exit->flags & 0x30;
      switch (kind) {
      case 0:
        AudioCommand_PlayFar(SOUND_MAP_EXIT);
        break;
      case 32:
        AudioCommand_PlayFar(128);
        ObjectEffect_BeginContextEffect26();
        break;
      case 48:
        AudioCommand_PlayFar(129);
        ObjectEffect_BeginContextEffect25();
        break;
      }
      work->exit = exit->result;
      ret = 0;
    }
  }
  return ret;
}

// Runs the event of a tile the leader pushes against (codes 0xf9 and 0xfc to
// 0xfe), once it has pushed for more than 12 frames; the count then restarts.
s32 Field_RunPushTileEvent(s32 event) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 leader = gGameState.selectedActor;

  switch (event) {
  case 0xfc: // with Down held
    if (work->blockedSteps > 12 && (gKeysHeld & KEY_DOWN)) {
      Actor_JumpDownLedge(leader, 6, 0);
      work->blockedSteps = 0;
    }
    break;
  case 0xf9:
  case 0xfe: // jump forward
    if (work->blockedSteps > 12) {
      Field_TryJumpForward();
      work->blockedSteps = 0;
    }
    break;
  case 0xfd: // one thing with Down held, another with Up
    if (work->blockedSteps > 12) {
      if (gKeysHeld & KEY_DOWN) {
        Field_ClimbDown();
      } else if (gKeysHeld & KEY_UP) {
        Field_ClimbUp();
      }
      work->blockedSteps = 0;
    }
    break;
  }
  return 0;
}

// Searches the tile ahead with the given collision code. Codes 242 to 247 show
// their two messages. Otherwise the scene's search event for the tile runs: a
// function, a message, or a chest holding a Psynergy stone, a mimic, coins or
// an item; with no event, "checked the ground" and "found nothing".
s32 Field_SearchTile(s32 tile) {
  const struct SceneEvent *event;
  s32 leader;
  u32 n;
  s16 condition;
  u32 value;
  s32 type;
  s32 allowed;

  leader = gGameState.selectedActor;
  n = tile - 242;
  if (n <= 5) {
    Leader_StopMoving();
    tile = gField_TileMessageOffsets[n];
    UiText_ShowPositionedMessageAndWaitFar(MsgTileChecked + tile, 1);
    UiText_ShowPositionedMessageAndWaitFar(MsgTileResult + tile, 1);
  } else {
    event = (const struct SceneEvent *)Scene_FindEvent(EVENT_SEARCH, tile);
    if (event != NULL) {
      s32 flags = event->control;

      // Bits 4 to 8 choose what the leader says on checking (flag 0x142 marks
      // that it was said).
      condition = event->condition;
      n = (flags >> 4) & 31;
      if ((event->trigger & 0x400) == 0 && n != 0) {
        Leader_StopMoving();
        UiText_ShowPositionedMessageAndWaitFar(MsgTileChecked + n, 1);
        GameFlag_SetBitFar(0x142);
      } else {
        GameFlag_ClearBitFar(0x142);
      }
      value = event->value;
      if ((value & 0x0f000000) != 0 || (value & 0xfff00000) == 0x400000) {
        if ((value & 0x0f000000) != 0) {
          // A function.
          if (GameFlag_IsConditionActive(condition) != 0)
            ((void (*)(s32))event->value)(gGameState.selectedActor);
          if (GameFlag_TestFar(0x142) != 0)
            UiText_ShowPositionedMessageAndWaitFar(MsgTileResult + n, 1);
        } else {
          // A message (type 0x004).
          if (GameFlag_IsConditionActive(condition) != 0)
            UiText_ShowPositionedMessageAndWaitFar(
                (*(const u16 *)&event->value), 1);
          else
            UiText_ShowPositionedMessageAndWaitFar(MsgButFoundNothing, 1);
        }
      } else {
        // A chest; its condition flag is set once it has been opened.
        Battle_Reset();
        if (GameFlag_IsConditionActive(condition) != 0) {
          value = event->value;
          allowed = 1;
          if ((value & 0xf0000) == 0x10000 && leader <= 7)
            allowed = 0;
          if (allowed != 0) {
            if ((event->control & 0x1ff) == 19)
              Marker_OpenChest(tile);
            type = event->value & 0xfff00000;
            if (type == 0x300000) {
              // A Psynergy stone: restores all PP.
              void *object;

              if ((event->control & 0x1ff) == 19)
                Marker_ShowOpened(tile);
              object = Marker_GetActor(tile);
              ItemDisplay_RaiseAboveLeader(object);
              AudioCommand_PlayFar(SOUND_TREASURE_FOUND);
              UiWork_PushValueSlotFar((*(const u16 *)&event->value), 5);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotPsynergyStone, 3);
              BattleParty_ApplyDrain(999, 0);
              UiWindow_CreateWithLayoutBoundsFar(1);
              AudioCommand_PlayFar(SOUND_RECOVERY);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotPsynergyStone + 1,
                                                     1);
              UiWork_FinalizeAndReleaseBlock16Far();
              ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
              AudioCommand_PlayFar(246);
              Battle_WaitMode0(30);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotPsynergyStone + 2,
                                                     1);
              Marker_Hide(tile);
              if (condition != -1)
                GameFlag_SetBitFar(condition);
            } else if (type == 0x500000) {
              // A mimic: a battle with enemy group slot of zone 99.
              struct FieldWork *work = (struct FieldWork *)gEventWork;

              if ((event->control & 0x1ff) == 19)
                Marker_LaunchMimic(tile);
              if (condition != -1) {
                condition = gGameState.pendingDjinnEvent = condition | 0x1000;
              }
              work->battle =
                  BattleFx_GetWeightedResult(99, (*(const u16 *)&event->value));
              Battle_SetStart(2);
              BattleFx_SelectBattleCue(99, (*(const u16 *)&event->value));
              AudioCommand_PlayFar(gGameState.sceneCue);
              UiText_ShowPositionedMessageAndWaitFar(MsgChestWasMimic, 1);
            } else if (type == 0x200000) {
              // Coins.
              void *object;

              object = BattleFx_StartRandomParticleEmitter(
                  gGameState.selectedActor, 0);
              WaitFrames(30);
              if ((event->control & 0x1ff) == 19)
                Marker_ShowOpened(tile);
              ItemDisplay_RaiseAboveLeader(object);
              AudioCommand_PlayFar(SOUND_TREASURE_FOUND);
              UiWork_PushValueSlotFar((*(const u16 *)&event->value), 5);
              UiText_ShowPositionedMessageAndWaitFar(MsgGotCoins, 3);
              Party_AdjustCoinsFar((*(const u16 *)&event->value));
              if (condition != -1)
                GameFlag_SetBitFar(condition);
              ObjectDispatch_ReleaseFar(object);
            } else {
              // An item, which stays in the chest when nobody can carry it.
              void *object;
              s32 owner;

              object = BattleFx_StartRandomParticleEmitter(
                  gGameState.selectedActor, event->value & 0xfff);
              WaitFrames(30);
              owner = PartyInventory_AddFar((*(const u16 *)&event->value));
              n = 0xffff;
              if (owner == -1) {
                UiWork_PushValueSlotFar(event->value & 0xfff, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgFoundItem, 1);
                UiText_ShowPositionedMessageAndWaitFar(MsgFoundItem + 4, 1);
                Object_DestroyIfPresent(object);
                if ((event->control & 0x1ff) == 19)
                  Marker_CloseChest(tile);
              } else {
                if ((event->control & 0x1ff) == 19)
                  Marker_ShowOpened(tile);
                ItemDisplay_RaiseAboveLeader(object);
                AudioCommand_PlayFar(SOUND_TREASURE_FOUND);
                UiWork_PushValueSlotFar(event->value & n, 2);
                if (owner == gGameState.selectedActor) {
                  UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem,
                                                         3);
                } else {
                  UiWork_PushValueSlotFar(owner, 1);
                  UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember,
                                                         3);
                }
                if (condition != -1)
                  GameFlag_SetBitFar(condition);
                ObjectDispatch_ReleaseFar(object);
              }
            }
          } else {
            UiText_ShowPositionedMessageAndWaitFar(MsgCantOpenIt, 1);
          }
        } else {
          UiText_ShowPositionedMessageAndWaitFar(MsgTileResult + n, 1);
        }
        BattleFx_FinishAction();
        BattleFx_PlayQueuedSound();
      }
    } else {
      UiText_ShowPositionedMessageAndWaitFar(MsgCheckedGround, 1);
      UiText_ShowPositionedMessageAndWaitFar(MsgGroundDidntFindAnything, 1);
    }
    GameFlag_ClearBitFar(0x142);
  }
  return 0;
}

//--------------------------------------------------------------------------
// Choosing the actor nearest to where an actor faces.
//--------------------------------------------------------------------------

// Returns how far, in pixels, a field Psynergy of the given kind reaches:
// gBattleFx_TargetRangeByMode pairs kinds with ranges, ended by -1, and a kind
// not listed reaches 16 pixels.
s32 BattleFx_MapKeyThroughTable(s32 kind) {
  s16 *p = gBattleFx_TargetRangeByMode;
  s32 range = 16;
  s32 key = *p;

  while (key != -1) {
    ++p;
    if (kind == key) {
      range = *p;
      break;
    }
    ++p;
    key = *p;
  }
  return range;
}

// Returns the actor nearest to actor id that it could talk to, or -1: within
// 48 pixels of height and 32 pixels of distance and, beyond 11 pixels, within
// 0x2fff of the direction it faces. Actors with flag 8 are skipped, and those
// with flag 4 count as 10/13 as far.
s32 BattleEffect_SelectNearbyObject(s32 id) {
  struct ObjectRuntime *src;
  struct ObjectRuntime *obj;
  s32 best;
  s32 bestDist;
  s32 i;
  s32 dx;
  s32 dy;
  s32 dz;
  s32 xx;
  s32 yy;
  s32 zz;
  s32 sq;
  s32 dist;
  s32 angle;

  best = -1;
  bestDist = 32;
  src = ObjectTable_Get(id);
  if (src != 0) {
    for (i = 0; i <= OBJ_LAST; i++) {
      if (i == id)
        continue;

      obj = ObjectTable_Get(i);
      if (obj == 0 || (obj->unk59 & 8) != 0)
        continue;

      if (obj->y - src->y >= 0) {
        if (obj->y - src->y >= 48 << 16)
          continue;
      } else if (src->y - obj->y >= 48 << 16) {
        continue;
      }

      // The distance in pixels.
      dx = (obj->x - src->x) / 0x10000;
      dy = (obj->y - src->y) / 0x10000;
      dz = (obj->z - src->z) / 0x10000;

      xx = dx * dx;
      yy = dy * dy;
      zz = dz * dz;
      sq = xx + yy + zz;
      dist = Iwram_Sqrt(sq);
      if ((obj->unk59 & 4) != 0)
        dist = dist * 10 / 13;
      if (dist >= bestDist)
        continue;

      angle = (u16)ArcTan2(obj->z - src->z, obj->x - src->x);
      if (dist > 11) {
        s32 diff = (s16)(angle - src->angle);

        if (diff < -0x2fff)
          continue;
        if (diff > 0x2fff)
          continue;
      }

      best = i;
      bestDist = dist;
    }
  }
  return best;
}

// Returns the actor nearest in front of actor id that a field Psynergy of the
// given kind can reach, or -1. The direction is rounded to a quarter turn and
// the distance measured on the ground; kind sets the range, the height
// allowed and the width of the cone.
s32 BattleEffect_SelectNearbyTargetObject(s32 id, s32 kind) {
  struct ObjectRuntime *src;
  struct ObjectRuntime *obj;
  s32 best;
  s32 bestDist;
  s32 dir;
  s32 i;
  s32 dyMax;
  s32 dx;
  s32 dz;
  s32 cz;
  s32 dist;
  s32 angle;
  s32 width;

  best = -1;
  bestDist = BattleFx_MapKeyThroughTable(kind);
  src = (struct ObjectRuntime *)ObjectTable_Get(id);
  if (src == 0)
    return best;

  dir = (src->angle + 0x2000) & 0xc000;
  for (i = 0; i <= OBJ_LAST; i++) {
    if (i == id)
      continue;

    obj = (struct ObjectRuntime *)ObjectTable_Get(i);
    if (obj == 0 || (obj->unk59 & 8) != 0)
      continue;

    // The height allowed: 8 pixels, or 48 for kind 13, 64 for 5, 16 for 2.
    dyMax = 8 << 16;
    if (kind == 13)
      dyMax = 48 << 16;
    if (kind == 5)
      dyMax = 64 << 16;
    if (kind == 2)
      dyMax = 16 << 16;

    {
      s32 dy = obj->y - src->y;
      if (dy >= 0) {
        if (dy > dyMax)
          continue;
      } else if (src->y - obj->y > dyMax) {
        continue;
      }
    }

    // The distance in whole pixels, rounded toward 0.
    dx = obj->x - src->x;
    if (dx < 0)
      dx += 0xffff;
    dx >>= 16;

    dz = obj->z - src->z;
    if (dz < 0)
      dz += 0xffff;
    cz = dz >> 16;

    dist = Iwram_Sqrt(dx * dx + cz * cz);
    if ((obj->unk59 & 0x10) != 0)
      dist = dist * 2 / 3;
    if (dist >= bestDist)
      continue;

    // The cone: 0x1800 either side up to 19 pixels, 0x1000 beyond; 0x2000
    // for kind 2.
    angle = (u16)ArcTan2(obj->z - src->z, obj->x - src->x);
    width = 0x1800;
    if (dist > 19)
      width = 0x1000;
    if (kind == 2)
      width = 0x2000;

    if (dist > 11) {
      s32 diff = (s16)(angle - dir);
      if (diff < 0)
        diff = -diff;
      if (diff >= width)
        continue;
    }

    best = i;
    bestDist = dist;
  }

  return best;
}

// If actor target exists, plays sound 124, gives it animation 4 and, 12 frames
// later, starts a particle emitter on actor (mode as for
// BattleFx_StartRandomParticleEmitter); returns the emitter, or 0.
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 actor, s32 target, s32 mode) {
  void *obj;
  s32 ret;

  obj = Object_GetById(target);
  ret = 0;
  if (obj != 0) {
    AudioCommand_PlayFar(124);
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 4);
    WaitFrames(12);
    ret = (s32)BattleFx_StartRandomParticleEmitter(actor, mode);
  }
  return ret;
}

typedef s32 (*BattleItemCallback)(s32 item, s32 actor, s32 slot);

// Gives every part of a motion object's sprite (but those at palette 15)
// palette - 1, or for palette 0 the next palette of a cycle that changes every
// 2 frames.
void Animation_ApplyChildPalette(struct MotionObject *obj, s32 palette) {
  struct AnimationObject *state;
  struct AnimationEntry **p;
  struct AnimationEntry *current;
  s32 cnt;
  s32 replacement;
  u8 n;

  if ((obj->recordStorageKind & 15) == 1) {
    state = obj->records;
    replacement = palette - 1;
    if (palette == 0)
      replacement = gAnimation_ChildPaletteCycle[(gFrameCount >> 1) & 7];

    n = state->count;
    if (n != 0) {
      p = state->entries;
      cnt = n;
      do {
        current = *p++;
        if (current != 0 && current->script != 0 && current->param != 15)
          current->param = replacement;
        cnt--;
      } while (cnt != 0);
    }
    state->dirty = 1;
  }
}

// Clears the field's busy flag and, while Reveal is active, ends it: 0x2090
// is Reveal (144) with ACT_PACKED, which runs its ending events.
void Battle_ResetEffectCounter(void) {
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;

  runtime->busy = 0;
  if (runtime->revealActive != 0) {
    Field_RunPsynergyEvents(0x2090);
  }
}

// A field Psynergy or item use, packed into one value.
#define ABILITY_ID(c) ((c) & 0x3ff)    // the Psynergy
#define ACT_UNIT(c) (((c) >> 10) & 15) // who uses it
#define ACT_PACKED 0x2000              // use it without paying PP
#define UNIT_NONE 15
#define ALLY_MAX 7 // the last party member

#define PSYNERGY_REVEAL 144
#define PSYNERGY_RETREAT 149

#define EXIT_RETREAT 999 // the exit Retreat takes

#define FLAG_PSYNERGY_NONE 0x17e   // Psynergy has no effect here
#define FLAG_RETREAT_BLOCKED 0x144 // Retreat does not work here
#define FLAG_ITEM_USE 0x145        // the Psynergy comes from an item
#define FLAG_PSYNERGY_EFFECT 0x140 // play the usual effect
#define FLAG_PSYNERGY_TARGET 0x141 // aim the effect at the target

// The flags of a Psynergy event (kind 5) in the given phase.
#define PSYNERGY_EV(phase) ((phase) << 28 | EVENT_PSYNERGY)
#define PSYNERGY_EV_ANY PSYNERGY_EV(7) // any phase

// Shows message m with the user's and the Psynergy's names, and waits.
#define PSYNERGY_MESSAGE(who, act, m)                                          \
  (UiWork_PushValueSlotFar((who), 1), UiWork_PushValueSlotFar((act), 4),       \
   UiText_ShowPositionedMessageAndWaitFar((m), 1))

// Returns the scene's item event (kind 4) for item that applies to what the
// leader faces, or 0 (item 0 matches any item). The event's trigger names an
// actor (control flag 0x10) or a tile code, and may also require the leader
// to face within 0x17ff of a direction.
s32 Field_FindItemEvent(s32 item) {
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;
  const struct SceneEvent *event = runtime->events;
  s32 facing =
      ((struct MotionObject *)ObjectTable_Get(gGameState.selectedActor))->angle;
  s32 actorAhead = BattleEffect_SelectNearbyObject(gGameState.selectedActor);
  s32 tileAhead;

  item &= 0x1ff;
  tileAhead = Field_GetCollisionAheadOfLeader();

  while (event->control != -1) {
    s32 direction = event->trigger & 0xf000;
    s16 hasDirection = event->trigger & 0x0800;
    s32 target = event->trigger & 0xff;

    if ((event->control & 0x0f) == 4 &&
        GameFlag_IsConditionActive(event->condition) != 0 &&
        (hasDirection == 0 || (u16)(direction - facing + 0x17ff) <= 0x2ffe)) {
      u32 flags = event->control;
      s32 eventItem = ((const u8 *)&event->control)[1];

      if (item == 0 || eventItem == item) {
        if ((flags & 0x10) != 0) {
          if (target == actorAhead)
            return (s32)event;
        } else if (target == tileAhead) {
          return (s32)event;
        }
      }
    }
    event++;
  }
  return 0;
}

// Uses an item on the field. packedItem holds the item (bits 0 to 9) and the
// character using it (bits 10 to 13; 15 for the character carrying most of
// it); slot is its inventory slot. The scene's item event for what the leader
// faces runs if there is one; otherwise an item with a field action uses it
// (Retreat asks first and leaves the scene). Flag 0x142 means nothing happened
// and flag 0x143 that the item is used up.
s32 Field_UseItem(s32 packedItem, s32 slot) {
  s32 result;
  s32 itemId;
  s32 actor;
  struct BattleUnit *obj;
  const struct SceneEvent *event;
  s32 j;
  s32 i;
  s32 best;
  s32 matches;
  s32 count;
  struct FieldWork *runtime;

  result = -1;
  itemId = packedItem & 0x3ff;
  actor = (packedItem >> 10) & 0xf;
  {
    best = 0;
    count = Party_CountActiveOwnersFar();

    if (actor == 15) {
      actor = 0;
      for (i = 0; i < count; i++) {
        obj = UnitGetFar(gGameState.activeOwners[i]);
        matches = 0;
        for (j = 0; j < 15; j++) {
          if ((obj->inventory[j] & 0x1ff) == itemId)
            matches++;
        }

        if (best < matches) {
          best = matches;
          actor = gGameState.activeOwners[i];
        }
      }
    } else {
      obj = UnitGetFar(actor);
      for (j = 0; j < 15; j++) {
        if ((obj->inventory[j] & 0x1ff) == itemId)
          best++;
      }
    }

    if (best == 0) {
      UiText_ShowPositionedMessageAndWaitFar(MsgNothingHappens, 1);
      return -1;
    }
  }

  event = (const struct SceneEvent *)Field_FindItemEvent(itemId);
  if (event != 0 && event->value != 0) {
    // The scene's event: a message, or a function.
    GameFlag_ClearBitFar(0x143);
    GameFlag_ClearBitFar(0x142);
    if (!(event->trigger & 0x400)) {
      UiWork_PushValueSlotFar(actor, 1);
      UiWork_PushValueSlotFar(itemId, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgActorUsesItem, 1);
    }
    if ((s32)event->value < 0x10000) {
      s32 objref = BattleEffect_SelectNearbyObject(gGameState.selectedActor);
      Battle_Reset();
      Event_SetMessageId(event->value);
      BattleEv_RunWait(objref, 0);
      BattleFx_FinishAction();
    } else {
      ((BattleItemCallback)event->value)(itemId, actor, slot);
    }
    result = 0;
  } else {
    // The item's own field action, if it has one.
    s32 actionId;

    GameFlag_ClearBitFar(0x143);
    GameFlag_SetBitFar(0x142);
    actionId = Item_GetDirectFar(itemId)->actionId;
    runtime = (struct FieldWork *)gEventWork;

    if (actionId != 0) {
      GameFlag_SetBitFar(FLAG_ITEM_USE);
      GameFlag_ClearBitFar(0x142);

      if (actionId == PSYNERGY_RETREAT && !GameFlag_TestFar(0x144)) {
        UiWork_PushValueSlotFar(itemId, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgUseItemQuestion, 13);
        /* FAKEMATCH: needed to match: the owner index doubles as the declined
         * flag, which ranks it above best in global allocation (r6, best r7). A
         * declined local of its own: differ 109. */
        i = Menu_AskYesNo(1);
        UiWork_FinalizePendingCoreFar();
        if (i != 0)
          return 0;

        gGameState.scene = gGameState.retreatScene;
        gGameState.entrance = gGameState.retreatEntrance;
        runtime->exit = EXIT_RETREAT;
      }

      UiWork_PushValueSlotFar(actor, 1);
      UiWork_PushValueSlotFar(itemId, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgActorUsesItem, 1);
      // UB: this routine returns void; call it through its declared type.
#ifdef UBFIX
      BattleFx_LoadActionEffectResources(actionId, 0);
#else
      /* FAKEMATCH: called through a cast to the s32 call the bytes show: called
       * as void by its prototype, the 0 is loaded before the action id (differ
       * 2), also with actionId declared at function scope; with actionId read
       * after runtime: differ 8. */
      ((s32 (*)(s32, s32))BattleFx_LoadActionEffectResources)(actionId, 0);
#endif
      runtime->resolvingAction = 1;
      BattleFx_Run();
      runtime->resolvingAction = 0;
      BattleEffect_CleanupSceneObjects();

      if (Item_GetDirectFar(itemId)->useType & 1)
        GameFlag_SetBitFar(0x143);
    }
  }

  if (GameFlag_TestFar(0x142))
    UiText_ShowPositionedMessageAndWaitFar(MsgNothingHappens, 1);
  if (GameFlag_TestFar(0x143))
    Inventory_RemoveFar(actor, slot);
  return result;
}

// Returns the scene's Psynergy event (kind 5) for a Psynergy of the given
// group that applies to what the leader faces, or NULL; stores the actor the
// Psynergy would target in *target. eventFlags gives the event's phase and
// kind (0x70000005 for any phase). The event may apply to any target
// (control flag 0x80), to an actor (0x10) or to a tile code.
void *BattleFx_FindMatchingEvent(s32 eventFlags, s32 group, void *target) {
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;
  const struct SceneEvent *event = runtime->events;
  s32 facing =
      ((struct MotionObject *)ObjectTable_Get(gGameState.selectedActor))->angle;
  s32 actorAhead =
      BattleEffect_SelectNearbyTargetObject(gGameState.selectedActor, group);
  s32 tileAhead;
  s32 anyPhase = 0;

  *(s32 *)target = actorAhead;
  tileAhead = Field_GetCollisionAheadOfLeader();
  if (eventFlags == PSYNERGY_EV_ANY)
    anyPhase = 1;

  while (event->control != -1) {
    s32 direction = event->trigger & 0xf000;
    s16 hasDirection = event->trigger & 0x0800;
    s32 eventTarget = event->trigger & 0xff;

    if ((event->control & 0x0f) == EVENT_PSYNERGY &&
        GameFlag_IsConditionActive(event->condition) != 0 &&
        (hasDirection == 0 || (u16)(direction - facing + 0x17ff) <= 0x2ffe) &&
        BattleActGet(((const u8 *)&event->control)[1])->type0c == group &&
        (anyPhase || (event->control & 0x7000000f) == eventFlags)) {
      if ((event->control & 0x80) != 0)
        return (void *)event;
      if ((event->control & 0x10) != 0) {
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

// Runs the scene's events for a Psynergy without paying its PP or showing its
// usual effect (phases 3 and 2, around the event's own effect), as for a
// Psynergy that ends by itself: packed holds the Psynergy (bits 0 to 9) and
// its user (bits 10 to 13).
s32 Field_RunPsynergyEvents(s32 packed) {
  s32 target;
  s32 ability;
  s32 user;
  u8 group;
  void *first;
  void *second;

  ability = packed & 0x3FF;
  user = ((u32)packed >> 10) & 0xF;
  group = BattleActGet(ability)->type0c;
  // The leader is looked up and the result is not used.
  ObjectTable_Get(gGameState.selectedActor);
  first = BattleFx_FindMatchingEvent(PSYNERGY_EV(3), group, &target);
  second = BattleFx_FindMatchingEvent(PSYNERGY_EV(2), group, &target);
  BattleFx_LoadActionEffectResources(ability, 0);
  BattleFx_SetupObjectPair(gGameState.selectedActor, target);
  BattleFx_RunEventAction(first, user, target);
  FieldEvent_RunTypeHandler();
  EffectRuntime_StopCurrentObject();
  BattleFx_RunEventAction(second, user, target);
  BattleEffect_CleanupSceneObjects();
  return 0;
}

//--------------------------------------------------------------------------
// Using Psynergy on the field, and the map's markers.
//--------------------------------------------------------------------------

// Uses a Psynergy on the field. code holds the Psynergy (bits 0 to 9), its
// user (bits 10 to 13; 15 for character 0) and ACT_PACKED to use it without
// paying PP. Checks it can be used here (Reveal not on the world map, Retreat
// where it is not blocked, after asking), pays its PP, then plays its effect
// between the scene's Psynergy events for what the leader faces: phase 1
// before, phase 0 after and phase 5 while it plays. An event whose trigger
// has flag 0x400 plays the effect itself.
s32 Field_UsePsynergy(u32 code) {
  s32 abilityId = ABILITY_ID(code);
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  const struct SceneEvent *before;
  const struct SceneEvent *after;
  const struct SceneEvent *during;
  s32 user;
  s32 target;
  s32 retreat;
  s32 group;
  s32 cost;
  s32 answer;

  group = BattleActGet(abilityId)->type0c;
  user = ACT_UNIT(code);
  ObjectTable_Get(gGameState.selectedActor);
  retreat = 0;
  Leader_StopMoving();
  GameFlag_ClearBitFar(FLAG_ITEM_USE);
  if (user == UNIT_NONE)
    user = 0;

  // Where Psynergy has no effect.
  if (GameFlag_TestFar(FLAG_PSYNERGY_NONE)) {
    PSYNERGY_MESSAGE(user, abilityId, MsgNothingHappened);
    return 0;
  }
  if (work->mode == MODE_WORLD_MAP && abilityId == PSYNERGY_REVEAL) {
    PSYNERGY_MESSAGE(user, PSYNERGY_REVEAL, MsgNothingHappened);
    return 0;
  }

  // Retreat: ask, then go back to where the party entered this place.
  if (abilityId == PSYNERGY_RETREAT) {
    if (GameFlag_TestFar(FLAG_RETREAT_BLOCKED)) {
      PSYNERGY_MESSAGE(user, PSYNERGY_RETREAT, MsgDoesNotWorkHere);
      return 0;
    }
    UiWork_PushValueSlotFar(PSYNERGY_RETREAT, 4);
    UiText_ShowPositionedMessageAndWaitFar(MsgUseAbilityConfirm, 13);
    answer = Menu_AskYesNo(1); // 0 is yes
    UiWork_FinalizePendingCoreFar();
    if (answer != 0)
      return 0;
    {
      struct GameState *st = &gGameState;
      s32 scene, ent;
      scene = (u16)st->retreatScene;
      st->scene = scene;
      ent = (u16)st->retreatEntrance;
      st->entrance = ent;
    }
    work->exit = EXIT_RETREAT;
    retreat = 1;
  }
  if (code & ACT_PACKED)
    return Field_RunPsynergyEvents(code);

  // A party member pays the PP.
  if (user <= ALLY_MAX) {
    cost = BattleActGet(abilityId)->ppCost;
    if (UnitGetFar(user)->pp < cost) {
      PSYNERGY_MESSAGE(user, abilityId, MsgNotEnoughPp);
      if (retreat)
        work->exit = 0;
      return 0;
    }
    Unit_AddPpFar(user, -cost);
  }

  // Find the scene's events, and choose how the effect plays.
  before = BattleFx_FindMatchingEvent(PSYNERGY_EV(1), group, &target);
  after = BattleFx_FindMatchingEvent(PSYNERGY_EV(0), group, &target);
  during = BattleFx_FindMatchingEvent(PSYNERGY_EV(5), group, &target);
  target = -1;
  GameFlag_SetBitFar(FLAG_PSYNERGY_EFFECT);
  GameFlag_SetBitFar(FLAG_PSYNERGY_TARGET);
  if (before || after || during) {
    target =
        BattleEffect_SelectNearbyTargetObject(gGameState.selectedActor, group);
    if (after && (after->trigger & 0x400)) // the event plays the effect
    {
      GameFlag_ClearBitFar(FLAG_PSYNERGY_EFFECT);
      GameFlag_ClearBitFar(FLAG_PSYNERGY_TARGET);
    }
  } else
    GameFlag_ClearBitFar(FLAG_PSYNERGY_TARGET);

  // Play the effect.
  if (work->mode == MODE_WORLD_MAP)
    BattleEffect_ClearOutOfBoundsObjects();
  BattleFx_LoadActionEffectResources(abilityId, 0);
  work->resolvingAction = 1;
  BattleFx_SetupObjectPair(gGameState.selectedActor, target);
  EventObject_Initialize();

  BattleFx_RunEventAction(before, user, target);
  if (GameFlag_TestFar(FLAG_PSYNERGY_EFFECT)) {
    if (GameFlag_TestFar(FLAG_PSYNERGY_TARGET))
      BattleFx_DispatchRequestKind();
    else
      BattleFx_Run();
  }
  EffectRuntime_StopCurrentObject();
  BattleFx_RunEventAction(after, user, target);
  if (GameFlag_TestFar(FLAG_PSYNERGY_EFFECT))
    BattleFx_ClearChildValueOnMismatch();
  GameFlag_ClearBitFar(FLAG_PSYNERGY_EFFECT);
  GameFlag_ClearBitFar(FLAG_PSYNERGY_TARGET);
  work->resolvingAction = 0;
  BattleEffect_CleanupSceneObjects();
  if (work->mode == MODE_WORLD_MAP)
    BattleEffect_ClearAllObjects();
  return 0;
}

// Returns whether the scene has a Psynergy event, in any phase, for a
// Psynergy of group and what the leader faces.
s32 BattleFx_HasMatchingEvent5(s32 group) {
  s32 target;
  s32 ev =
      (s32)BattleFx_FindMatchingEvent(PSYNERGY_EV_ANY, (u16)group, &target);
  return (u32)((-ev) | ev) >> 31;
}

// Returns whether the scene has an event for item and what the leader faces.
u32 BattleFx_HasTrigger(u16 item) {
  s32 ev;

  ev = Field_FindItemEvent(item);
  return (u32)((0 - ev) | ev) >> 31;
}

// Puts an actor on the ground under it.
void ObjectMotion_SnapToTerrain(void *object) {
  struct ObjectRuntime *obj = object;
  s32 height;

  height = Map_GetTerrainHeightFar(0, obj->x, obj->z);
  obj->y = height;
  obj->terrainHeight = height;
}

// The start of the map's work.
struct MarkerMap {
  u8 unk00[0x10];
  u8 *markers; // (column, row, id) triples, ended by 255, 255
};

// Places an actor at each of the map's markers (ids 100 to 239) that the
// scene has an event for, up to ten: a chest (actor 20) for a chest event,
// shown open once its flag is set and gone once its mimic was beaten, and a
// Psynergy stone (actor 28) for a search event giving one, until it is taken.
void Field_PlaceMapMarkers(void) {
  u32 id;
  struct MarkerMap *map = gMapWork[0];
  struct FieldWork *work = gMapWork[HEAP_SLOT_EVENT - 8];
  s32 count = 0;
  u8 *list = map->markers;
  struct MarkerSlot *slot = work->markers;
  const struct SceneEvent *event;
  struct ScriptObjectRuntime *obj;
  u32 column;
  u32 row;

  Dma_Fill32(0, slot, sizeof(struct MarkerSlot) * MARKER_MAX);
  if (list == NULL)
    return;

  column = *list++;
  row = *list++;
  while (column != 255 || row != 255) {
    id = *list++;
    if (id >= 100 && id <= 239) {
      event = gOverlayArea.events();
      for (; event->control != -1; event++) {
        if (event->trigger != id)
          continue;
        if ((event->control & 0x1ff) == 19) {
          // A chest, at the centre of its cell.
          obj = (struct ScriptObjectRuntime *)Object_CreateFar(
              20, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
          if (obj == NULL)
            continue;
          ObjectMotion_SnapToTerrain(obj);
          ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
          if (GameFlag_TestFar(event->condition)) {
            if ((event->value & 0xfff00000) == 0x500000) {
              ObjectDispatch_ReleaseFar(obj);
              continue;
            }
            ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
          }
          ObjectDispatch_InitFromTable6Far(obj);
          obj->homeX = obj->x / 0x10000;
          obj->homeZ = obj->z / 0x10000;
          obj->unk23 = 1;
          obj->flags59 = 1;
          slot->id = event->trigger;
          slot->object = (struct ObjectRuntime *)obj;
          slot->column = obj->x / 0x100000;
          slot->row = obj->z / 0x100000;
          slot++;
          count++;
          if (count >= MARKER_MAX)
            return;
        } else if ((event->control & 0x1ff) == EVENT_SEARCH) {
          // A Psynergy stone not yet taken.
          if ((event->value & 0xfff00000) != 0x300000)
            continue;
          if (GameFlag_TestFar(event->condition))
            continue;
          obj = (struct ScriptObjectRuntime *)Object_CreateFar(
              28, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
          if (obj == NULL)
            continue;
          ObjectMotion_SnapToTerrain(obj);
          ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
          ObjectDispatch_InitFromTable6Far(obj);
          ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
          obj->homeX = obj->x / 0x10000;
          obj->homeZ = obj->z / 0x10000;
          obj->flags59 = 1;
          obj->unk23 = 1;
          slot->object = (struct ObjectRuntime *)obj;
          slot->id = event->trigger;
          slot->column = obj->x / 0x100000;
          slot->row = obj->z / 0x100000;
          slot++;
          count++;
          if (count >= MARKER_MAX)
            return;
        } else {
          continue;
        }
        break;
      }
    }
    column = *list++;
    row = *list++;
  }
}

// Returns the marker slot (0 to 9) whose event trigger is id, or -1.
s32 Marker_FindSlot(s32 id) {
  s32 index;
  s32 found;
  struct MarkerSlot *slot;

  slot = ((struct FieldWork *)gEventWork)->markers;
  found = -1;
  for (index = 0; index <= 9; index++, slot++) {
    if (slot->id == id) {
      found = index;
      break;
    }
  }
  return found;
}

// Does nothing.
void EffectRuntime_ReservedNoOp(void) {}

// Gives the actor of marker id animation, plays sound and waits 12 frames.
static __inline__ void SetMarkerAnimationAndPlaySound(s32 id, s32 animation,
                                                      s32 sound) {
  s32 slot;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slot = Marker_FindSlot(id);
  if (slot != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slot];
    object = marker->object;
    if (object != NULL)
      ObjectDispatch_ApplyArgumentToChildrenFar(object, animation);
    AudioCommand_PlayFar(sound);
    WaitFrames(0xc);
  }
}

// Closes the chest of marker id again (its item could not be carried).
void Marker_CloseChest(s32 id) { SetMarkerAnimationAndPlaySound(id, 5, 0x7d); }

// Makes the chest of marker id jump up as a mimic.
void Marker_LaunchMimic(s32 id) {
  s32 slot;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slot = Marker_FindSlot(id);
  if (slot != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slot];
    object = marker->object;
    WaitFrames(0x12);
    if (object != NULL)
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
    AudioCommand_PlayFar(0x92);
    if (object != NULL) {
      object->velocityY = 0x80000;
      ObjectDispatch_SetSingleChildFlagsFar(object, 1);
    }
  }
}

// Opens the chest of marker id.
void Marker_OpenChest(s32 id) { SetMarkerAnimationAndPlaySound(id, 4, 0x7c); }

// Shows the chest of marker id open (animation 2).
void Marker_ShowOpened(s32 id) {
  s32 slot;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slot = Marker_FindSlot(id);
  if (slot != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slot];
    object = marker->object;
    if (object != NULL)
      ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
  }
}

// Returns the actor of marker id, or NULL.
struct ObjectRuntime *Marker_GetActor(s32 id) {
  s32 slot = Marker_FindSlot(id);
  struct MarkerSlot *marker;

  if (slot == -1)
    return NULL;
  marker = &((struct FieldWork *)gEventWork)->markers[slot];
  return marker->object;
}

// Hides the actor of marker id.
void Marker_Hide(s32 id) {
  s32 slot;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slot = Marker_FindSlot(id);
  if (slot != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slot];
    object = marker->object;
    if (object != NULL)
      object->animationKind = 0;
  }
}

// Moves the actor of marker id to (x, z) on the ground; -1 keeps the centre
// of its cell on that axis.
void EffectRuntime_SetCurrentPosition(s32 id, s32 x, s32 z) {
  s32 slot;
  s32 height;
  s32 objectX;
  s32 objectZ;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  objectZ = z;
  objectX = x;
  slot = Marker_FindSlot(id);
  if (slot != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slot];
    object = marker->object;
    if (object != NULL) {
      if (objectX == -1)
        objectX = (marker->column << 20) + 0x80000;
      if (objectZ == -1)
        objectZ = (marker->row << 20) + 0x80000;
      object->x = objectX;
      object->z = objectZ;
      height = Map_GetTerrainHeightFar(0, objectX, objectZ);
      object->terrainHeight = height;
      object->y = height;
    }
  }
}

//--------------------------------------------------------------------------
// Field effects: showing a found item with its sparkles, Reveal's sparkles on
// hidden things, and the windows' HBlank DMA.
//--------------------------------------------------------------------------

LAYOUT_OFFSET_GUARD(ParticleEmitter_travel_offset_offset, struct ObjectRuntime,
                    velocityY, 0x28);
LAYOUT_OFFSET_GUARD(ParticleEmitter_active_offset, struct ObjectRuntime, flags,
                    0x55);
LAYOUT_OFFSET_GUARD(ParticleEffectObject_child_offset, struct ObjectRuntime,
                    animation, 0x50);

// An actor as these effects see it.
struct EffectObj {
  s32 data; // its script
  u8 pad0[0x24];
  u32 travel;
  u8 pad1[0x1c];
  u32 dy;
  u8 pad2[4];
  struct AnimationObject *vis;
  u8 pad3[24];
  void (*proc)(void); // called each frame
};

// The table of 64 actors, 88 bytes before gEventWork.
#define EffectPool (*(struct EffectObj **)((u8 *)&gEventWork - 88))

// One of the scene's events (ended by flags -1).
struct MapEventEntry {
  s32 flags; // the low 4 bits are the kind
  s16 id;
  s16 flag;
  s32 position;
};

// Keeps the party leader off the markers: a leader within 16 pixels of a
// marker cell's centre is moved 20 pixels from that centre, in the direction
// it stood, and loses its move target.
void BattleFx_EmitRandomParticle(void) {
  struct ObjectRuntime *actor = ObjectTable_Get(gGameState.selectedActor);
  struct MarkerSlot *cell = ((struct FieldWork *)gEventWork)->markers;
  s32 i = 0;
  s32 x;
  s32 z;
  s32 cx;
  s32 cz;
  s32 dx;
  s32 dz;

  if (cell->id != 0) {
    x = actor->x;
    z = actor->z;
    for (;;) {
      cx = cell->column << 20;
      dx = x - cx - (8 << 16);
      cz = cell->row << 20;
      dz = z - cz - (8 << 16);
      if ((dx >= -0xfffff && dx <= 0xfffff) &&
          (dz >= -0xfffff && dz <= 0xfffff)) {
        actor->x = cx + (8 << 16);
        actor->z = cz + (8 << 16);
        Vector_AddPolarOffset(20 << 16, (u16)ArcTan2(dz, dx),
                              (struct EffectVector *)&actor->x);
        actor->targetX = ACTOR_NO_TARGET;
        actor->targetY = ACTOR_NO_TARGET;
        actor->targetZ = ACTOR_NO_TARGET;
        return;
      }
      i++;
      cell++;
      if (i > 9 || cell->id == 0)
        break;
    }
  }
}

// Each frame of a found item's picture: one time in ten, a sparkle appears
// within 16 pixels of it.
void ItemDisplay_EmitSparkles(struct ObjectRuntime *emitter) {
  struct EffectVector pos;
  struct ObjectRuntime *obj;
  u32 randomAngle;

  if (emitter->velocityY >= -255 && emitter->velocityY <= 255)
    emitter->flags = 0;

  if ((100 * Random16() >> 16) > 9)
    return;

  pos.x = emitter->x;
  pos.y = emitter->y;
  pos.z = emitter->z;
  randomAngle = Random16();
  Vector_AddPolarOffset(randomAngle << 4, Random16(), &pos);
  obj = Object_Spawn(285, pos.x, pos.y, pos.z);
  if (obj != 0) {
    // Draw the sparkle at priority 1.
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_ParticleScript);
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
    ((struct AnimationObject *)obj->animation)->part[0].priority = 1;
  }
}

// Shows the picture of item (0 for none) at the centre of the cell 16 pixels
// in front of character actor, 16 pixels up, with sparkles. While Reveal is
// active, its sparkles are cleared first. Returns the picture's actor, or 0.
struct EffectObj *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item) {
  struct EffectObj *obj;

  s32 work = (s32)gEventWork;
  u32 off = (actor * 4) + 0x14; // EventWork's actors[actor]

  s32 srcZ = (s32)*(struct FieldActor **)(work + off);
  struct EffectVector pos;
  s32 x;

  if (srcZ == 0)
    return 0;

  pos.x = ((struct FieldActor *)srcZ)->x.fixed;
  pos.y = ((struct FieldActor *)srcZ)->y.fixed;
  pos.z = ((struct FieldActor *)srcZ)->z.fixed;
  Vector_AddPolarOffset(16 << 16, ((struct FieldActor *)srcZ)->facing, &pos);
  x = (pos.x & 0xfff00000) + (8 << 16);
  srcZ = (pos.z & 0xfff00000) + (8 << 16); // srcZ now holds the cell's z

  if (((struct FieldWork *)work)->revealActive != 0) {
    struct EffectObj *ent = EffectPool;

    work = 63; // work now counts the actors left

    do {
      if (ent->data != 0) {
        if (ent->proc == (void (*)(void))RevealMarker_EmitSparkles) {
          ObjectDispatch_ReleaseFar(ent);
        }
        if (ent->data == (s32)gBattleFx_ParticleScript) {
          ObjectDispatch_ReleaseFar(ent);
        }
      }
      work--;
      ent++;
    } while (work >= 0);
  }

  WaitFrames(3);
  obj = (struct EffectObj *)Object_CreateFar(22, x, 16 << 16, srcZ);
  if (obj == 0)
    return 0;

  ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                               gBattleFx_ParticleEmitterScript);
  {
    struct AnimationObject *vis = obj->vis;
    void *buf;

    vis->flags = 0;
    vis->count = 0;
    vis->part[0].fullColor = 0;
    vis->part[0].palette = 0;
    vis->part[0].priority = 1;

    obj->travel = 0x20000;
    obj->dy = 0x4000;

    // Draw the item's icon into heap block 17, then send it to the sprite's
    // tiles.
    buf = Runtime_AllocateHeapBlock(17, 0x608);
    Ui_PrepareTransferForItemFar(item);

    vis->part[0].tile = VramCache_Load(vis->slot, 128, (u8 *)buf + 0x400);

    Runtime_ReleaseHeapBlock(17);
    obj->proc = (void (*)(void))ItemDisplay_EmitSparkles;
  }

  return obj;
}

// Destroys object if there is one.
void Object_DestroyIfPresent(void *object) {
  if (object != 0)
    ObjectDispatch_ReleaseFar(object);
}

// Raises a found item's picture to 36 pixels above the party leader, who
// holds it up (animation 28) facing the screen.
void ItemDisplay_RaiseAboveLeader(struct ObjectRuntime *object) {
  struct ObjectRuntime *actor;

  if (object == 0)
    return;

  actor = ObjectTable_Get(gGameState.selectedActor);
  object->acceleration = 1 << 16;
  object->speedLimit = 2 << 16;
  object->flags = 0;
  Object_SetMoveTargetFar(object, actor->x, actor->y + (36 << 16), actor->z);
  WaitFrames(3);
  ObjectDispatch_ApplyArgumentToChildrenFar(actor, 28);
  ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                               gBattleFx_RisingPulseScript);
  actor->angle = 0x4000;
}

// Shows a found item's picture: flags bit 0 adds sparkles and bit 1 raises it
// above the leader, with waits of 60 and 80 frames around that when both are
// set. The leader then returns to animation 1.
void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags) {
  void *actor;

  if (object != NULL) {
    actor = ObjectTable_Get(gGameState.selectedActor);
    if (flags & 1) {
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      ObjectDispatch_InitializeFar(object, gBattleFx_ParticleEmitterScript);
      ((struct EffectObj *)object)->travel = 0x20000;
      ((struct EffectObj *)object)->dy = 0x4000;
      ((struct EffectObj *)object)->proc =
          (void (*)(void))ItemDisplay_EmitSparkles;
    }
    if (flags == 3) {
      WaitFrames(60);
    }
    if (flags & 2) {
      ItemDisplay_RaiseAboveLeader(object);
    }
    if (flags == 3) {
      WaitFrames(80);
    }
    ObjectDispatch_ApplyArgumentToChildrenFar(actor, 1);
  }
}

// Shows the picture of item 36 pixels above the party leader for 80 frames;
// flags bit 0 adds sparkles and bit 1 raises it as a found item.
void BattleFx_StartEffectObject22(s32 item, s32 flags) {
  struct FieldActor *actor = ObjectTable_Get(gGameState.selectedActor);
  void *handle = Runtime_AllocateHeapBlock(17, 0x608);
  struct EffectObj *object = (struct EffectObj *)Object_CreateFar(
      22, actor->x.fixed, actor->y.fixed + (36 << 16), actor->z.fixed);

  if (object != 0) {
    struct AnimationObject *visual = object->vis;

    visual->flags = 0;
    visual->count = 0;
    visual->part[0].fullColor = 0;
    visual->part[0].palette = 0;
    visual->part[0].priority = 1;

    Ui_PrepareTransferForItemFar(item);
    VramCache_Load(visual->slot, 128, (u8 *)handle + 0x400);
    Runtime_ReleaseHeapBlock(17);

    if (flags & 1)
      object->proc = (void (*)(void))ItemDisplay_EmitSparkles;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader((struct ObjectRuntime *)object);

    WaitFrames(80);
    ObjectDispatch_ApplyArgumentToChildrenFar(actor, 1);
    ObjectDispatch_ReleaseFar(object);
  }
}

// The German, Spanish, French and Italian editions stop Reveal's sparkles
// while a menu is open.
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)

#define PARTICLES_PAUSE_FOR_MENU 1
#endif

#if defined(PARTICLES_PAUSE_FOR_MENU)
#define MENU_OPEN_OFFSET 0xcca // in gEventWork: non-zero while a menu is open

// While Reveal is active, destroys all its sparkles.
void BattleFx_ClearRandomParticles(void) {
  if (((struct FieldWork *)gEventWork)->revealActive != 0) {
    struct EffectObj *ent = EffectPool;
    s32 n = 63;

    do {
      if (ent->data != 0) {
        if (ent->data == (s32)gBattleFx_ParticleScript)
          ObjectDispatch_ReleaseFar(ent);
      }
      n--;
      ent++;
    } while (n >= 0);
  }
}

#endif

// Each frame of a Reveal marker: one time in ten, a sparkle appears within 16
// pixels of it.
void RevealMarker_EmitSparkles(const struct FieldActor *source) {
  struct EffectVector pos;
  struct ObjectRuntime *obj;
  u32 random;

#if defined(PARTICLES_PAUSE_FOR_MENU)
  if (*(s8 *)((u8 *)gEventWork + MENU_OPEN_OFFSET) != 0)
    return;
#endif
  if ((100 * Random16() >> 16) > 9)
    return;

  pos.x = source->x.fixed;
  pos.y = source->y.fixed;
  pos.z = source->z.fixed;
  random = Random16();
  Vector_AddPolarOffset(random << 4, Random16(), &pos);
  obj = Object_Spawn(285, pos.x, pos.y, pos.z);
  if (obj != 0) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_ParticleScript);
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
    ((struct AnimationObject *)obj->animation)->part[0].priority = 1;
  }
}

// Returns 1 while Reveal is active, else 0.
s32 Field_IsRevealActive(struct ScriptInterpreter *interpreter) {
  s16 count;
  u32 active = 0;
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;

  if (runtime != NULL) {
    count = runtime->revealActive;
    active = (0 - count) | count;
    active >>= 31;
  }
  return active;
}

// Reveal: marks with sparkles the hidden things within 8 cells across and 5
// down of the leader: map places 100 to 239 with no marker actor, whose search
// event (kind 3) gives an item, coins, a Psynergy stone or a mimic (types 0
// to 3 and 5) and has not been found yet.
void FieldEffect_SpawnNearbyMarkers(void) {
  u8 *list;
  struct ObjectRuntime *actor;
  struct MapEventEntry *entry;
  s32 actorX;
  s32 actorZ;
  s32 x;
  s32 z;
  s32 id;

  // The map's places: (x, z, id) triples, ended by x and z 0xff.
  list = *(u8 **)(((u8 *)gMapWork[0]) + 16);
  actor = ObjectTable_Get(gGameState.selectedActor);
  actorX = actor->x >> 20;
  actorZ = actor->z >> 20;
  if (list != 0) {
    while (x = *list++, z = *list++, x != 0xff || z != 0xff) {
      id = *list++;
      if (Marker_GetActor(id) != NULL)
        continue;
      if (id < 100 || id > 239)
        continue;
      if (actorX - x >= 0) {
        if (actorX - x > 8)
          continue;
      } else if (x - actorX > 8)
        continue;
      if (actorZ - z >= 0) {
        if (actorZ - z > 5)
          continue;
      } else if (z - actorZ > 5)
        continue;
      entry = (struct MapEventEntry *)gOverlayArea.events();
      if (entry->flags == -1)
        continue;
      do {
        if (entry->id == id && (entry->flags & 15) == 3) {
          switch (entry->position & 0xfff00000) {
          case 0:
          case 0x100000:
          case 0x200000:
          case 0x300000:
          case 0x500000:
            if (entry->flag != -1 && GameFlag_TestFar(entry->flag) == 0) {
              /* FAKEMATCH: the spawned object reuses the actor variable, so the
               * one pseudo keeps ObjectTable_Get's result in r5 (adds r5, r0,
               * #0) before the actorX and actorZ loads. 2026-10-13: a separate
               * object variable, block-local or function-scope in any
               * declaration position, differs 9 of 163: the copy into r5 is
               * gone, the loads read r0 and three pool offsets move. */
              actor = (struct ObjectRuntime *)Object_CreateFar(
                  22, (x << 20) + (8 << 16), 0, (z << 20) + (8 << 16));
              if (actor != 0) {
                ObjectDispatch_InitializeFar((struct DispatchObject *)actor,
                                             gBattleFx_MarkerParticleScript);
                ObjectDispatch_SetSingleChildFlagsFar(actor, 0);
                ((struct EffectObj *)actor)->proc =
                    (void (*)(void))RevealMarker_EmitSparkles;
              }
            }
            break;
          }
        }
        entry++;
      } while (entry->flags != -1);
    }
  }
}

// Each frame, before drawing: turns windows 0 and 1 on, writes the first four
// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
// copy the rest into WIN0H and WIN1H one word each HBlank, so that every line
// of the screen gets its own window edges.
void BattleFx_StartWindowHBlankDma(void) {
  struct DisplayTransitionState *work = gDisplayTransition;
  u16 *src = work->pages[work->page];
  volatile u16 *channel = REG_DMA0;

  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  (void)channel[5];
  REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  Io_Write16(*src++, &REG_WININ);
  Io_Write16(*src++, &REG_WINOUT);
  Io_Write16(*src++, &REG_WIN0H);
  Io_Write16(*src++, &REG_WIN1H);
  Io_Write16(160, &REG_WIN0V);
  Io_Write16(160, &REG_WIN1V);
  Dma_Set(src, (void *)&REG_WIN0H, 0xa6600001, (volatile u32 *)channel);
}

struct TransitionSpan {
  u16 cnt;
  u16 val;
  u8 x0;
  u8 x1;
  u8 y0;
  u8 y1;
};

struct TransitionShape {
  u16 first;
  u16 second;
  struct TransitionSpan spans[3];
};

extern struct TransitionShape gDisplayTransition_Shapes[];

// Each frame of a window transition: advances its value toward its end (and
// at the end turns the windows off, or moves to the next mode), then builds
// the next page of window edges, WININ and WINOUT first and then a
// (left << 8 | right) pair for each of the 160 lines. Bit 5 of the value says
// whether the transition opens or closes; its low 5 bits are its progress.
// Modes: 0 random bars, 1 a shape from gDisplayTransition_Shapes, 2 random
// horizontal noise, 3 a circle at the screen's centre, 4 vertical blinds,
// 7 and 9 an ellipse around the party leader (7 flickers), 8 a wobbling
// ellipse around a point, 77 a flatter ellipse around the leader.
void DisplayTransition_UpdateScanlineTable(void) {
  struct DisplayTransitionState *state;
  struct TransitionSpan *span;
  struct TransitionShape *shape;
  struct MapScrollWork *map;
  u16 *dst;
  s32 *view7;
  s32 *view8;
  s32 *view9;
  s32 *view77;
  struct ObjectRuntime *obj;
  u32 i;
  s32 k;
  s32 cnt;
  s32 v;
  s32 rr;
  s32 d;
  u32 h;
  s32 lo;
  s32 hi;
  s32 cx;
  s32 cy;

  s32 ay;
  s32 bx;
  s32 by;
  u32 w;
  s32 e;

  map = gMapWork[0];
  state = gMapWork[23];

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      state->duration = 0;
      if (state->nextMode == 0) {
        if (state->end == 64) {
          REG_DISPCNT &= 0x81ff;
        } else {
          REG_DISPCNT &= 0x9fff;
        }
        Scheduler_RemoveCallback(
            (SchedulerCallback)BattleFx_StartWindowHBlankDma);
        Scheduler_RemoveCallback(
            (SchedulerCallback)DisplayTransition_UpdateScanlineTable);
        {
          volatile u16 *dma0 = REG_DMA0;

          dma0[5] &= 0xc5ff;
          dma0[5] &= 0x7fff;
          dma0[5];
        }
        return;
      }
      state->mode = state->nextMode;
    } else {
      state->value =
          state->start +
          Iwram_SignedDivide((state->end - state->start) * ++state->step,
                             state->duration);
    }
  }

  dst = state->pages[1 ^ state->page];
  UiWindow_FillFromSceneFar(dst + 2);

  switch (state->mode) {
  case 0:
    *dst++ = 0x7f7f;
    *dst++ = 1;
    v = state->value;
    if (v & 32) {
      v = 32 - (v & 31);
    } else {
      v &= 31;
    }
    v = gDisplayTransition_NoiseWidths[v];
    for (i = 0; i < 160; i++) {
      w = (Random16() * (241 - v)) >> 16;
      dst[0] = ((w << 8) | (w + v));
      dst += 2;
    }
    break;

  case 1:
    v = state->value;
    shape = &gDisplayTransition_Shapes[v & 31];
    if (v & 32) {
      *dst++ = shape->first;
      *dst++ = shape->second;
    } else {
      *dst++ = shape->second;
      *dst++ = shape->first;
    }
    for (i = 0; i < 3; i++) {
      span = &shape->spans[i];
      cnt = span->cnt;
      hi = span->val;
      if (cnt != 0) {
        if (hi != 0) {
          for (k = 0; k < cnt; k++) {
            dst[0] = hi;
            dst += 2;
          }
        } else {
          rr = span->x0;
          lo = span->y0;
          d = span->x1;
          hi = span->y1;
          for (k = 0; k < cnt; k++) {
            dst[0] =
                (((rr + (d - rr) * k / cnt) << 8) + (lo + (hi - lo) * k / cnt));
            dst += 2;
          }
        }
      }
    }
    break;

  case 2:
    v = state->value - 1;
    if (v & 32) {
      *dst++ = 1;
      *dst++ = 0x7f7f;
    } else {
      *dst++ = 0x7f7f;
      *dst++ = 1;
    }
    v &= 31;
    for (i = 0; i < 160; i++) {
      h = (v << 4) + ((16 * Random16()) >> 16);
      if (h > 255) {
        h = 255;
      }
      dst[0] = h;
      dst += 2;
    }
    break;

  case 3:
    v = state->value;
    *dst++ = 0x7f7f;
    *dst++ = 1;
    if ((u32)v > 32) {
      *dst++ = 1;
      *dst++ = 0x7f7f;
      v = 64 - v;
    }
    v *= 5;
    rr = (v * v) << 16;
    for (i = 0; i < 160; i++) {
      v = i - 80;
      h = Iwram_Sqrt(rr - ((v * v) << 16)) >> 8;
      lo = 120 - h;
      hi = h + 120;
      if (lo < 0) {
        lo = 0;
      }
      if (hi < 0) {
        hi = 0;
      }
      if (lo > 240) {
        lo = 240;
      }
      if (hi > 240) {
        hi = 240;
      }
      dst[0] = ((lo << 8) + hi);
      dst += 2;
    }
    break;

  case 4:
    v = state->value;
    if (v & 32) {
      *dst++ = 1;
      *dst++ = 0x7f7f;
    } else {
      *dst++ = 0x7f7f;
      *dst++ = 1;
    }
    v = (u32)(240 * (v & 31)) >> 5;
    for (i = 0; i < 160; i += 2) {
      e = ((240 - v) << 8) + 240;
      dst[0] = v;
      dst[2] = e;
      dst += 4;
    }
    break;

  case 7: {
    s32 ax;

    view7 = &map->viewX;
    ax = view7[0] & 0xffff0000;
    ay = view7[1] & 0xffff0000;
    obj = ObjectTable_Get(gGameState.selectedActor);
    cx = (obj->x - ax) / 0x10000;
    cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
    // the frame counter is read here, where mode 8 forms its index, and the
    // value is not used
    gFrameCount;
    *dst++ = state->active;
    *dst++ = state->mask;
    v = state->value;
    if ((v & 32) == 0) {
      v = 32 - (v & 31);
    } else {
      v &= 31;
    }
    if (gFrameCount & 1) {
      v = 0;
    }
    v *= 5;
    rr = (v * v) << 16;
    for (i = 0; i < 160; i++) {
      v = i - cy;
      h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
      lo = cx - h;
      hi = cx + h;
      if (lo < 0) {
        lo = 0;
      }
      if (hi < 0) {
        hi = 0;
      }
      if (lo > 240) {
        lo = 240;
      }
      if (hi > 240) {
        hi = 240;
      }
      dst[0] = ((lo << 8) + hi);
      dst += 2;
    }
    break;
  }
  case 8:
    view8 = &map->viewX;
    bx = view8[0] & 0xffff0000;
    by = view8[1] & 0xffff0000;
    cx = (state->centreX - bx) / 0x10000;
    cy = (state->centreY - by) / 0x10000 - 16;
    k = -cy * 2 + gFrameCount;
    *dst++ = state->mask;
    *dst++ = state->active;
    v = state->value;
    if (v & 32) {
      v = 32 - (v & 31);
    } else {
      v &= 31;
    }
    v *= 5;
    rr = (v * v) << 16;
    for (i = 0; i < 160; i++) {
      v = i - cy;
      h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
      lo = cx - h;
      hi = cx + h;
      if (lo < hi) {
        d = gDisplayTransition_EdgeOffsets[k & 31];
        lo -= d;
        hi += d;
        if (lo >= hi) {
          lo = 240;
          hi = 240;
        }
      }
      if (lo < 0) {
        lo = 0;
      }
      if (hi < 0) {
        hi = 0;
      }
      if (lo > 240) {
        lo = 240;
      }
      if (hi > 240) {
        hi = 240;
      }
      dst[0] = ((lo << 8) + hi);
      dst += 2;
      k += 2;
    }
    break;

  case 9: {
    s32 ax;

    view9 = &map->viewX;
    ax = view9[0] & 0xffff0000;
    ay = view9[1] & 0xffff0000;
    obj = ObjectTable_Get(gGameState.selectedActor);
    cx = (obj->x - ax) / 0x10000;
    cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
    // the frame counter is read here, where mode 8 forms its index, and the
    // value is not used
    gFrameCount;
    *dst++ = state->mask;
    *dst++ = state->active;
    v = state->value;
    if (v & 32) {
      v = 32 - (v & 31);
    } else {
      v &= 31;
    }
    v *= 5;
    rr = (v * v) << 16;
    for (i = 0; i < 160; i++) {
      v = i - cy;
      h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
      lo = cx - h;
      hi = cx + h;
      if (lo < 0) {
        lo = 0;
      }
      if (hi < 0) {
        hi = 0;
      }
      if (lo > 240) {
        lo = 240;
      }
      if (hi > 240) {
        hi = 240;
      }
      dst[0] = ((lo << 8) + hi);
      dst += 2;
    }
    break;
  }
  case 77: {
    s32 ax;

    view77 = &map->viewX;
    ax = view77[0] & 0xffff0000;
    ay = view77[1] & 0xffff0000;
    obj = ObjectTable_Get(gGameState.selectedActor);
    cx = (obj->x - ax) / 0x10000;
    cy = ((obj->z - obj->y) - ay) / 0x10000 - 8;
    // the frame counter is read here, where mode 8 forms its index, and the
    // value is not used
    gFrameCount;
    *dst++ = state->mask;
    *dst++ = state->active;
    v = state->value;
    if (v & 32) {
      v = 32 - (v & 31);
    } else {
      v &= 31;
    }
    v *= 5;
    rr = (v * v) << 16;
    for (i = 0; i < 160; i++) {
      v = i - cy;
      h = Iwram_Sqrt(rr - ((v * v * 3) << 14)) >> 8;
      lo = cx - h;
      hi = cx + h;
      if (lo < 0) {
        lo = 0;
      }
      if (hi < 0) {
        hi = 0;
      }
      if (lo > 240) {
        lo = 240;
      }
      if (hi > 240) {
        hi = 240;
      }
      dst[0] = ((lo << 8) + hi);
      dst += 2;
    }
    break;
  }
  }

  state->page ^= 1;
}

// Starts window transition mode at value 0, with no duration: the caller
// moves it.
void DisplayTransition_InitializeBattleEffectState(s32 mode) {
  struct DisplayTransitionState *state =
      Runtime_AllocateBlock(31, sizeof(*state));

  Dma_Fill32(0, state, sizeof(*state));
  state->mode = mode;
  state->value = 0;
  state->mask = 0x3f3f;
  state->active = 1;
  Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
  Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
}

// Suspends the window transition's two callbacks.
void BattleFx_EnableTwoCallbacks(void) {
  Scheduler_SuspendTasks(&DisplayTransition_UpdateScanlineTable);
  Scheduler_SuspendTasks(&BattleFx_StartWindowHBlankDma);
}

// Allocates the display transition state (heap block 31) and clears it.
struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void) {
  struct DisplayTransitionState *state;

  state = Runtime_AllocateBlock(31, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  return state;
}

// Fades the screen in over frames frames. mode's high byte chooses how: 0
// from black, 1 from the backdrop colour, 2 a window transition (its low
// byte the mode), 3 a dissolve, 4 opening from the centre line (low byte 1
// flickers).
void DisplayTransition_Start(s32 mode, s32 frames) {
  struct DisplayTransitionWindow *display;
  s32 value;
  s32 kind;

  kind = (mode >> 8) & 0xff;
  display = gMapWork[0];
  value = mode & 0xff;
  switch (kind) {
  case 0:
    Fade_ToBlack(0);
    Fade_FromBlack(frames);
    WaitFrames(1);
    break;
  case 1:
    BattleFx_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
    BattleFx_StartBufferInterpolation(frames);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    BattleFx_SetPrimaryBufferValue(0);
    return;
  case 2: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = value;
    state->value = 0;
    state->mask = 63;
    state->active = 1;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 3: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = value;
    state->value = 32;
    DisplayTransition_FillTilemapAndSolidTile(15);
    WaitFrames(1);
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 4: {
    struct DisplayTransitionState *state;

    display = gMapWork[0];
    state = DisplayTransition_AllocateAndClearState();
    display->firstLine = 80;
    display->secondLine = 80;
    WaitFrames(1);
    if (value == 0) {
      Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                      0x20000);
}

// Fades the screen out over frames frames, as DisplayTransition_Start fades
// it in.
void DisplayTransition_Finish(s32 mode, s32 frames) {
  s32 value;
  s32 kind;

  kind = (mode >> 8) & 0xff;
  value = mode & 0xff;
  switch (kind) {
  case 0:
    Fade_FromBlack(0);
    Fade_ToBlack(frames);
    break;
  case 1:
    BattleFx_ApplyColorToTargetBuffer(0x8000, 0);
    BattleFx_StartBufferInterpolation(frames);
    break;
  case 2: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = value;
    state->value = 32;
    state->mask = 63;
    state->active = 1;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
    WaitFrames(1);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 3: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = value;
    state->value = 32;
    DisplayTransition_FillTilemapAndSolidTile(0);
    WaitFrames(1);
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 4: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    if (value == 0) {
      Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 0;
      state->end = 80;
      state->duration = frames;
      state->step = 0;
    } else {
      Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 0;
      state->end = 80;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
}

// Clears the map's display-control bits for backgrounds 3, 2 and 1
// (0x0800, 0x0400 and 0x0200) for each argument that is set.
void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200) {
  struct DisplayTransitionWindow *state;

  state = gMapWork[0];
  if (state != NULL) {
    if (clear_0200 != 0) {
      state->dispcnt &= 0xFDFF;
    }
    if (clear_0400 != 0) {
      state->dispcnt &= 0xFBFF;
    }
    if (clear_0800 != 0) {
      state->dispcnt &= 0xF7FF;
    }
  }
}

// Each frame of the centre-line transition: moves the value toward its end
// and sets the two lines where the screen switches between shown and blank.
void DisplayTransition_Update(void) {
  struct DisplayTransitionFrame *state = *Ram_DisplayWork;
  struct DisplayTransitionWindow *display =
      *(struct DisplayTransitionWindow **)Ram_MapWork;
  s8 *duration = &state->duration;
  u32 displayValue;

  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Scheduler_RemoveCallback(DisplayTransition_Update);
      Runtime_SetIrqHandler(1, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 value;

      (*step)++;
      value = Iwram_SignedDivide(delta * *step, *duration);
      state->value = state->start + value;
    }
  }

  displayValue = state->value;
  if (displayValue > 79) {
    display->firstLine = 200;
    display->secondLine = 250;
  } else {
    display->firstLine = displayValue;
    display->secondLine = 159 - displayValue;
  }
}

// As DisplayTransition_Update, but the lines flicker every other frame.
void DisplayTransition_UpdateFromCentre(void) {
  struct DisplayTransitionFrame *state = *Ram_DisplayWork;
  struct DisplayTransitionWindow *window =
      *(struct DisplayTransitionWindow **)Ram_MapWork;
  s8 *duration = &state->duration;
  u16 value;

  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Scheduler_RemoveCallback(DisplayTransition_UpdateFromCentre);
      Runtime_SetIrqHandler(1, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 offset;

      (*step)++;
      offset = Iwram_SignedDivide(delta * *step, *duration);
      state->value = state->start + offset;
    }
  }

  value = state->value;
  if (value > 79) {
    window->firstLine = 200;
    window->secondLine = 250;
  } else if (value != 0 && (gFrameCount & 1)) {
    window->firstLine = value + 80;
    window->secondLine = 80 - value;
  } else {
    window->firstLine = 0;
    window->secondLine = 159;
  }
}

// The VCount interrupt of the centre-line transition: switches the display
// to mode 2 (nothing shown) at the first line and back to mode 0 at the
// second.
void DisplayTransition_UpdateScanline(void) {
  u32 line;
  struct DisplayTransitionWindow *state;

  line = REG_VCOUNT;
  state = gMapWork[0];

again:
  switch (state->irqPhase) {
  case 3:
    if (line >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xFFF8) | 2;
      state->irqPhase = 9;
    }
    break;
  case 2:
    if (line >= state->irqSecondLine) {
      REG_DISPCNT &= 0xFFF8;
      state->irqPhase = 9;
    }
    break;
  case 1:
    if (line >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xFFF8) | 2;
      state->irqPhase++;
      goto again;
    }
    if (line >= state->irqSecondLine) {
      REG_DISPCNT &= 0xFFF8;
      state->irqPhase = 3;
      goto again;
    }
    break;
  case 0:
    if (line <= 158) {
      state->irqPhase = 1;
      goto again;
    }
    break;
  }
}

// Each frame of the dissolve: moves the value toward its end and sets two
// more of the solid tile's 64 pixels, in the order of
// gDisplayTransition_DitherTable, to colour 15 (or 0 when closing).
void DisplayTransition_UpdateFrame(void) {
  struct DisplayTransitionFrame *state = *Ram_DisplayWork;
  s32 value;
  s32 blend;
  u32 i;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      volatile u16 *dma0;
      state->duration = 0;
      Scheduler_RemoveCallback(DisplayTransition_UpdateFrame);
      dma0 = REG_DMA0;
      dma0[5] &= 0xc5ff;
      dma0[5] &= 0x7fff;
      (void)dma0[5];
      return;
    } else {
      s32 delta = state->end - state->start;
      state->step++;
      state->value = state->start +
                     Iwram_SignedDivide(delta * state->step, state->duration);
    }
  }

  value = state->value - 1;
  state->ditherToggle ^= 1;
  blend = 0;
  if (value & 32)
    blend = 15;
  value = (value & 31) * 2;
  for (i = 0; i <= 1; i++) {
    u8 entry = gDisplayTransition_DitherTable[value & 63];
    u8 *p = &state->paletteNibbles[entry >> 1];
    if (entry & 1)
      *p = (*p & 0x0f) | (blend << 4);
    else
      *p = (*p & 0xf0) | blend;
    value++;
  }

  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->paletteNibbles,
                      0x84000008);
}

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

// Fills background screen block 4 with tile 0 in palette 15 and, unless color
// is -1, makes tile 0 solid color.
void DisplayTransition_FillTilemapAndSolidTile(s32 color) {
  struct DisplayTransitionState *work = gDisplayTransition;
  Dma_Fill32(0xf000f000, BG_SCREEN_BLOCK(4), 0x500);
  if (color != -1) {
    u32 pattern = 0;
    s32 cnt;
    u32 *tile;
    for (cnt = 7; cnt >= 0; --cnt)
      pattern = (pattern << 4) | color;
    tile = work->solidTile;
    for (cnt = 7; cnt >= 0; --cnt)
      *tile++ = pattern;
    Dma_Set(work->solidTile, BG_CHAR_BLOCK(0), 0x84000008, REG_DMA3);
  }
}

// Starts a dissolve of the given mode and waits 120 frames.
void DisplayTransition_InitializeState(s32 value) {
  struct DisplayTransitionState *state;

  state = Runtime_AllocateBlock(0x1f, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  DisplayTransition_FillTilemapAndSolidTile(0);
  state->mode = value;
  state->value = 0;
  Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
  WaitFrames(0x78);
}

// Sets each of the 0x540 colour channels of step to the amount to add each
// frame for source to reach target in frames frames.
void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                 s32 frames) {
  s32 index;
  s32 first;
  s32 second;

  if (frames > 0) {
    index = 0x53F;
    do {
      first = *source;
      second = *target;
      *step = Iwram_SignedDivide(second - first, frames);
      index--;
      source++;
      target++;
      step++;
    } while (index >= 0);
  }
}

// Each frame (unless flag 0x152 is set): moves the colour buffer one step
// toward its target, packs it into RGB555 and queues it for the BG and OBJ
// palettes.
void BattlePalette_UpdateBlend(void) {
  struct BattleEffectBuffers *work = gPaletteBufferWork;
  u16 *add = work->delta;
  s32 i;

  if (GameFlag_TestFar(0x152) != 0) {
    return;
  }
  if (work->duration == 0) {
    return;
  }
  if (++work->step < work->duration) {
    u16 *sum = work->current;
    for (i = 0; i < 0x540; i++) {
      *sum++ += *add++;
    }
  } else {
    Iwram_CopyWords(work->current, work->target, sizeof(work->current));
    work->duration = 0;
  }

  {
    u16 *packed = work->packed[1 ^ work->page];
    s32 blue = 0x7c00;
    u16 *current;

    i = 0x1c0;
    current = work->current;
    for (; i != 0; i--) {
      *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) |
                  (((s16)current[2] >> 10) & 0x1f);
      current += 3;
    }
  }

  work->page ^= 1;
  {
    u16 *packed = work->packed[work->page];

    IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)packed, 0x84000070);
    IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(packed + 0xe0), 0x84000070);
  }
}

//--------------------------------------------------------------------------
// The colour buffer: palette colour changes for Psynergy and transitions,
// building target colours and moving the palettes toward them.
//--------------------------------------------------------------------------

#define HEAP_SLOT_PALBUF 32 // the colour buffer (gPaletteBufferWork)

#define COLOR_MAX 448 // 224 BG colours and 224 OBJ colours
#define COLOR_HALF 224

// Curves mapping a 5-bit channel to the buffer's channel shifted up 10 bits.
extern u16 gBattlePalette_ChannelCurve[];
extern u16 gBattlePalette_ChannelCurveLight[];
extern u16 gBattlePalette_ChannelCurveContrast[];

// Builds colours for the colour buffer from the palette src into dst, which
// holds each colour as B, G and R channels shifted up 10 bits. mode below
// 0x8000 is one colour for all; PALETTE_TRANSFORM_FROM_SOURCE the backdrop
// colour; the other PALETTE_TRANSFORM_* values transform each colour; any
// other value is the address of a ready buffer to copy. half: 0 for all 448
// colours, 1 for the BG's 224, 2 for the OBJ's 224.
void BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half) {
  u32 i;
  u32 cnt = COLOR_MAX;
  s32 r;
  s32 b;
  u32 c;
  s32 g;

  if (mode == PALETTE_TRANSFORM_FROM_SOURCE)
    mode = *(u16 *)BG_PLTT;
  if (half == 1) {
    cnt = COLOR_HALF;
  } else if (half == 2) {
    dst += COLOR_HALF * 3;
    src += COLOR_HALF;
    cnt = COLOR_HALF;
  }

  // One colour: write it once, then let DMA copy it over the rest.
  if (mode < PALETTE_TRANSFORM_FROM_SOURCE) {
    *dst++ = mode & 0x7c00;
    *dst++ = (mode & 0x03e0) << 5;
    *dst++ = (mode & 0x001f) << 10;
    Dma_Set(dst - 3, dst, (((cnt - 1) * 6) >> 1) | DMA_ENABLE32, REG_DMA3);
  } else if (mode < PALETTE_TRANSFORM_FLAGS) {
    switch (mode) {
    case PALETTE_TRANSFORM_GREY: // grey of brightness (2R + 4G + B) / 7
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = ((c << 11) & 0xf800);
        g = ((c << 7) & 0x1f000);
        b = (c & 0x7c00);
        c = Iwram_SignedDivide(r + g + b, 7);
        *dst++ = c;
        *dst++ = c;
        *dst++ = c;
      }
      break;
    case PALETTE_TRANSFORM_SEPIA: // the brightness, red stronger, within 8 to
                                  // 28
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        c = Iwram_SignedDivide(r + ((c >> 5) & 31) + ((c >> 10) & 31), 10);
        r = c * 4 + 5;
        b = c * 3 + 5;
        g = b;
        if (r < 8)
          r = 8;
        if (g < 8)
          g = 8;
        if (b < 8)
          b = 8;
        if (r > 28)
          r = 28;
        if (g > 28)
          g = 28;
        if (b > 28)
          b = 28;
        *dst++ = gBattlePalette_ChannelCurveLight[b];
        *dst++ = gBattlePalette_ChannelCurveLight[g];
        *dst++ = gBattlePalette_ChannelCurveLight[r];
      }
      break;
    case PALETTE_TRANSFORM_WARM: // half R + 10, two thirds G + 8, B - 7 (black
                                 // stays black)
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        if ((r | g | b) != 0) {
          r -= (u32)r >> 1;
          g -= g / 3;
          r += 10;
          g += 8;
          b -= 7;
          r = BattleFx_ClampRgb555Channel(r);
          g = BattleFx_ClampRgb555Channel(g);
          b = BattleFx_ClampRgb555Channel(b);
        }
        *dst++ = gBattlePalette_ChannelCurveContrast[b];
        *dst++ = gBattlePalette_ChannelCurveLight[g];
        *dst++ = gBattlePalette_ChannelCurve[r];
      }
      break;
    case PALETTE_TRANSFORM_MUTED: // narrow the range, G and B + 2
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        if (r < 10)
          r = 10;
        if (g < 16)
          g = 16;
        if (b < 16)
          b = 16;
        if (r > 28)
          r = 28;
        if (g > 24)
          g = 24;
        if (b > 26)
          b = 26;
        g += 2;
        b += 2;
        r = BattleFx_ClampRgb555Channel(r);
        g = BattleFx_ClampRgb555Channel(g);
        b = BattleFx_ClampRgb555Channel(b);
        *dst++ = gBattlePalette_ChannelCurve[b];
        *dst++ = gBattlePalette_ChannelCurve[g];
        *dst++ = gBattlePalette_ChannelCurve[r];
      }
      break;
    case PALETTE_TRANSFORM_PALE: // half of each, plus the mean
      for (i = 0; i < cnt; i++) {
        s32 mean;

        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        mean = BattleFx_ClampRgb555Channel((r + g + b) / 3);
        r = (r >> 1) + mean;
        g = (g >> 1) + mean;
        b = (b >> 1) + mean;
        r = BattleFx_ClampRgb555Channel(r);
        g = BattleFx_ClampRgb555Channel(g);
        b = BattleFx_ClampRgb555Channel(b);
        dst[0] = gBattlePalette_ChannelCurveContrast[b];
        dst[1] = gBattlePalette_ChannelCurveContrast[g];
        dst[2] = gBattlePalette_ChannelCurveContrast[r];
        dst += 3;
      }
      break;
    case PALETTE_TRANSFORM_RED: // an eighth of G and B added to R; G and B two
                                // thirds
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        r += ((g >> 3) + (b >> 3));
        r = BattleFx_ClampRgb555Channel(r);
        g -= g / 3;
        b -= b / 3;
        dst[0] = gBattlePalette_ChannelCurve[b];
        dst[1] = gBattlePalette_ChannelCurve[g];
        dst[2] = gBattlePalette_ChannelCurveLight[r];
        dst += 3;
      }
      break;
    case PALETTE_TRANSFORM_WARM2: // half R + 6, two thirds G + 4, B - 6
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        r -= (u32)r >> 1;
        g -= g / 3;
        r += 6;
        g += 4;
        b -= 6;
        r = BattleFx_ClampRgb555Channel(r);
        g = BattleFx_ClampRgb555Channel(g);
        b = BattleFx_ClampRgb555Channel(b);
        dst[0] = gBattlePalette_ChannelCurveContrast[b];
        dst[1] = gBattlePalette_ChannelCurveLight[g];
        dst[2] = gBattlePalette_ChannelCurve[r];
        dst += 3;
      }
      break;
    default: // unchanged
      for (i = 0; i < cnt; i++) {
        c = *src++;
        dst[i * 3 + 0] = c & 0x7c00;
        dst[i * 3 + 1] = (c & 0x03e0) << 5;
        dst[i * 3 + 2] = (c & 0x001f) << 10;
      }
      break;
    }
  } else if (mode & PALETTE_TRANSFORM_TINT) {
    // The brightness (2R + 4G + B) times the colour in mode.
    u32 tr;
    u32 tg;
    u32 tb;
    s32 r;
    s32 g;

    tr = mode & 31;
    tg = (mode >> 5) & 31;
    tb = (mode >> 10) & 31;
    for (i = 0; i < cnt; i++) {
      c = *src++;
      r = ((c << 11) & 0xf800);
      g = ((c << 7) & 0x1f000);
      b = (c & 0x7c00);
      c = Iwram_SignedDivide(r + g + b, 96);
      r = c * tr;
      g = c * tg;
      b = c * tb;
      r = BattleFx_ClampRgb555Component(r);
      g = BattleFx_ClampRgb555Component(g);
      b = BattleFx_ClampRgb555Component(b);
      dst[0] = b;
      dst[1] = g;
      dst[2] = r;
      dst += 3;
    }
  } else if (mode & PALETTE_TRANSFORM_BLEND) {
    // The brightness R + G + B shared out in the proportions of mode's colour.
    u32 tr;
    u32 tg;
    u32 tb;
    s32 nr;
    s32 ng;
    s32 nb;

    tr = mode & 31;
    tg = (mode >> 5) & 31;
    tb = (mode >> 10) & 31;
    for (i = 0; i < cnt; i++) {
      c = *src++;
      r = c & 31;
      c = Iwram_SignedDivide((r + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4,
                             tr + tg + tb);
      nr = Iwram_MulQ16(((c * tr) >> 4) << 16, (s32)(tr << 16) >> 4);
      ng = Iwram_MulQ16(((c * tg) >> 4) << 16, (s32)(tg << 16) >> 4);
      nb = Iwram_MulQ16(((c * tb) >> 4) << 16, (s32)(tb << 16) >> 4);
      nr = BattleFx_ClampRgb555Channel((u32)nr >> 16);
      ng = BattleFx_ClampRgb555Channel((u32)ng >> 16);
      nb = BattleFx_ClampRgb555Channel((u32)nb >> 16);
      *dst++ = gBattlePalette_ChannelCurve[nb];
      *dst++ = gBattlePalette_ChannelCurve[ng];
      *dst++ = gBattlePalette_ChannelCurve[nr];
    }
  } else if (mode & PALETTE_TRANSFORM_SPLIT) {
    for (i = 0; i < cnt; i++) {
      c = *src++;
      dst[i * 3 + 0] = c & 0x7c00;
      dst[i * 3 + 1] = (c & 0x03e0) << 5;
      dst[i * 3 + 2] = (c & 0x001f) << 10;
    }
  } else {
    // Copy a ready buffer.
    if (half == 2)
      mode += COLOR_HALF * 3 * sizeof(u16);
    Dma_Set((const void *)mode, dst,
            ((cnt * 6) >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
  }
}

// Sets up the colour buffer (heap block 32) from the BG and OBJ palettes and
// starts updating the palettes from it each frame.
void BattleEffect_InitializeBuffers(void) {
  struct BattleEffectBuffers *buf;

  buf = Runtime_AllocateBlock(HEAP_SLOT_PALBUF, sizeof(*buf));
  Dma_Fill32(0, buf, sizeof(*buf));
  Dma_Set((void *)BG_PLTT, &buf->palette[0],
          DMA_ENABLE32 | DMA_32BIT | (COLOR_HALF / 2), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, &buf->palette[COLOR_HALF],
          DMA_ENABLE32 | DMA_32BIT | (COLOR_HALF / 2), REG_DMA3);
  BattleFx_BuildBuffer(PALETTE_TRANSFORM_PLAIN, buf->palette, buf->target, 0);
  Scheduler_AddOrUpdateCallback(BattlePalette_UpdateBlend, 0xc8f);
}

// Stops the colour buffer and frees it.
void Runtime_ScheduleCallbackAndReleaseBlock32B(void) {
  Scheduler_RemoveCallback(&BattlePalette_UpdateBlend);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_PALBUF);
}

// Sets the colours the buffer moves toward (see BattleFx_BuildBuffer).
void BattleFx_ApplyColorToTargetBuffer(s32 mode, s32 half) {
  struct BattleEffectBuffers *buf = gPaletteBufferWork;

  if (buf != NULL) {
    BattleFx_BuildBuffer(mode, buf->palette, buf->target, half);
  }
}

// Sets the colours the buffer shows now (see BattleFx_BuildBuffer).
void BattleFx_ApplyColorToSourceBuffer(s32 mode, s32 half) {
  struct BattleEffectBuffers *buf = gPaletteBufferWork;

  if (buf != NULL) {
    BattleFx_BuildBuffer(mode, buf->palette, buf->current, half);
  }
}

// Sets the buffer's source backdrop colour.
void BattleFx_SetPrimaryBufferValue(u32 col) {
  struct BattleEffectBuffers *buf = gPaletteBufferWork;

  if (buf != NULL) {
    buf->palette[0] = col;
  }
}

// Starts moving the shown colours to the target colours over frames frames.
void BattleFx_StartBufferInterpolation(s32 frames) {
  struct BattleEffectBuffers *buf = gPaletteBufferWork;

  if (buf != NULL) {
    buf->duration = frames;
    buf->step = 0;
    BattleFx_InterpolateBuffers(buf->current, buf->target, buf->delta, frames);
  }
}

// Clamps a 5-bit colour channel to 0 to 31.
s32 BattleFx_ClampRgb555Channel(s32 value) {
  if (value > 31)
    return 31;
  if (value < 0)
    value = 0;
  return value;
}

// Clamps a colour channel shifted up 10 bits to at most 31 << 10.
s32 BattleFx_ClampRgb555Component(s32 value) {
  if (value > 31744)
    value = 31744;
  return value;
}

struct FadeActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[2];
  s16 floor;
  u8 unk18[10];
  u8 layer;
};

struct FadeWork {
  struct AnimationSpritePart marks[2];
  struct FadeActor *actor;
};

struct FadeView {
  u8 unk00[228];
  s16 camera[4];
};

// Each frame of the fade overlay: on each of the two cells beside the actor
// (left and right of it), where the ground one or two cells south rises above
// the actor's floor, draws a semi-transparent 16-pixel-wide sprite of colour 1
// at the height of that ground.
void FadeOverlay_DrawMarks(void) {
  struct FadeWork *work = WORK_SLOT(36);
  s16 *camera = ((struct FadeView *)WORK_SLOT(8))->camera;
  s32 cameraX = camera[1];
  s32 cameraZ = camera[3];
  struct AnimationSpritePart *mark = work->marks;
  struct FadeActor *actor = work->actor;
  s32 x;
  s32 z;
  s32 floor;
  s32 layer;
  u32 tile;
  s32 height;
  s32 other;
  s32 row;
  /* FAKEMATCH: never used; nothing reads or writes these four bytes of the
   * native frame. Without them the frame is 12 bytes, not 16 (differ 4: the sp
   * adjustments), measured 2026-10-13. */
  s16 unused[2];

  if (actor == 0)
    return;
  x = actor->x;
  z = actor->z;
  floor = actor->floor;
  layer = actor->layer;
  tile = gVramBlockCache[94].offset >> 5;
  x -= 0x80000;
  height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
  other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16;
  if (other > height)
    height = other;
  if (height > 0 && height > floor) {
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0x400;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    row = (z >> 16) & 0xf0;
    mark->y = row - cameraZ - height + 16;
    Runtime_PushSlotEntry(mark, 0);
  }
  x += 0x100000;
  height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
  mark = &work->marks[1];
  other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16;
  if (other > height)
    height = other;
  if (height > 0 && height > floor) {
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    row = (z >> 16) & 0xf0;
    mark->y = row - cameraZ - height + 16;
    Runtime_PushSlotEntry(mark, 0);
  }
}

// Starts the fade overlay on object (the party leader when NULL): loads its
// solid tiles, sets the blending and draws it each frame.
void BattleFx_StartFadeOverlay(void *object) {
  u32 *work;
  u8 *tiles;
  volatile u32 fill;

  work = Runtime_AllocateBlock(36, 28);
  tiles = Runtime_AllocateHeapBlock(14, 0x400);
  Dma_Fill32Cell(fill, 0x11111111, tiles, 0x200);
  VramCache_Load(94, 0x200, tiles);
  Runtime_ReleaseHeapBlock(14);
  Scheduler_AddOrUpdateCallback(FadeOverlay_DrawMarks, 0xc80);
  Io_Write16(0x3f9e, &REG_BLDCNT);
  Io_Write16(16, &REG_BLDALPHA);
  Io_Write16(31, &REG_BLDY);
  Dma_Fill32Cell(fill, 0, work, 28);
  if (object == NULL)
    object = ObjectTable_Get(gGameState.selectedActor);
  work[6] = (u32)object;
}

// Suspends the fade overlay.
void Object_EnableEffectSpawnCallback(void) {
  Scheduler_SuspendTasks(FadeOverlay_DrawMarks);
}

// Resumes the fade overlay.
void Object_DisableEffectSpawnCallback(void) {
  Scheduler_ResumeTasks(FadeOverlay_DrawMarks);
}

// Returns the speaker definition of sprite id from gBattleFx_DefinitionTable,
// or its last entry when there is none.
struct BattleEffectEntry *BattleFx_FindDefinition(u32 id) {
  struct BattleEffectEntry *entry = gBattleFx_DefinitionTable;
  u32 index = 0;

  if (entry->id != id) {
    do {
      index += 1;
      entry++;
      if (index > 0x81)
        break;
    } while (entry->id != id);
  }
  return entry;
}

// Returns the sprite of actor id, or 0 when it has no animated sprite.
s32 BattleFx_GetAnimationValue(s32 id) {
  struct BattleRenderObject *object = ObjectTable_Get(id);

  if (object->kind != 1 || object->animation == NULL ||
      object->animation->value28 == NULL) {
    return 0;
  }
  return *object->animation->value28;
}

// Returns the portrait shown beside the messages of sprite id (0x100 plus
// its definition's value), or 0 with no portrait or with gGameState.speech
// off.
s32 BattleFx_GetResourceId(u32 id) {
  u8 value;

  if (gGameState.speech == 0 ||
      (value = BattleFx_FindDefinition(id)->value) == 0xFF) {
    return 0;
  }
  return value + 0x100;
}

// Returns the definition flags of actor id's sprite, which choose its line
// when it has nothing to say.
s32 BattleFx_GetFlags(s32 id) {
  return BattleFx_FindDefinition(BattleFx_GetAnimationValue(id))->flags;
}
