/// @file
/// Running summon and table effects, and shared target helpers.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Calls the empty BattleEffect_DoNothing count times, without waiting for a
/// frame: a busy delay that changes no state.
///
/// Called only by BattleEffect_RunAnnihilation (annihilation.c), with 10000,
/// before it releases the blitter it has just drawn with.
void BattleEffect_CallDoNothing(s32 count) {
  s32 callIndex;

  callIndex = 0;
  if (count != 0) {
    do {
      callIndex += 1;
      BattleEffect_DoNothing();
    } while (callIndex != count);
  }
}

// Two of the battle's three entry points, BattleEffect_RunSummon and
// BattleEffect_RunFromTable; README.md describes all three and the heap
// blocks they lend an effect. Both size the sheet slot for one set of the
// ten mote cells (MOTE_SET_BYTES), where most effects decode
// ParticleSpritesA; an animation's own pictures go in work->sheet. The shared
// helpers after them manage the scanline window and the targets' sprites.

/// Runs one of the large battle effects, the ones whose effect word
/// has bit 15 set: takes the effect's sheet, work and canvas, runs the
/// effect chosen by its kind to the end, and frees them again.
///
/// Called through BattleEffect_RunSummonFar by
/// BattleStaging_HideField (battle/staging.c), when the turn's
/// effect word has EFFECT_SUMMON, and by
/// AutoBattle_RunPsynergy (battle/auto_battle.c).
///
/// Each effect is named for the summon whose gAbilityBattleEffects entry
/// selects its kind, as the handlers of gBattleEffect_Handlers are;
/// SUMMON_ANIM_NONE, which no ability selects, runs Meteor's effect too.
/// Takes heap slots HEAP_SLOT_BATTLE_SHEET, HEAP_SLOT_BATTLE_EFFECT and
/// HEAP_SLOT_BATTLE_CANVAS and releases them in reverse, so the effect must
/// have removed every task that reads them. Unlike
/// BattleEffect_RunFromTable it does not set work->args: each effect does
/// that itself.
void BattleEffect_RunSummon(struct BattleEffectParams *args) {
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);

  if (args->kind == SUMMON_ANIM_NONE) {
    BattleEffect_RunMeteor(args);
  } else {
    switch (args->kind) {
    case SUMMON_ANIM_RAMSES:
      BattleEffect_RunRamses(args);
      break;
    case SUMMON_ANIM_NEREID:
      BattleEffect_RunNereid(args);
      break;
    case SUMMON_ANIM_KIRIN:
      BattleEffect_RunKirin(args);
      break;
    case SUMMON_ANIM_ATALANTA:
      BattleEffect_RunAtalanta(args);
      break;
    case SUMMON_ANIM_CYBELE:
      BattleEffect_RunCybele(args);
      break;
    case SUMMON_ANIM_NEPTUNE:
      BattleEffect_RunNeptune(args);
      break;
    case SUMMON_ANIM_TIAMAT:
      BattleEffect_RunTiamat(args);
      break;
    case SUMMON_ANIM_PROCNE:
      BattleEffect_RunProcne(args);
      break;
    case SUMMON_ANIM_JUDGMENT:
      BattleEffect_RunJudgment(args);
      break;
    case SUMMON_ANIM_BOREAS:
      BattleEffect_RunBoreas(args);
      break;
    case SUMMON_ANIM_METEOR:
      BattleEffect_RunMeteor(args);
      break;
    case SUMMON_ANIM_THOR:
      BattleEffect_RunThor(args);
      break;
    }
  }

  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

/// Runs an ordinary battle effect: takes the effect's sheet, work and
/// canvas, runs the entry of gBattleEffect_Handlers for its kind,
/// and frees them again.
///
/// Called through BattleEffect_RunFromTableFar by
/// BattleStaging_RunAttack, BattleStaging_RunItem,
/// BattleStaging_RunPsynergyCast and BattleStaging_HideField
/// (battle/staging.c, the last when the turn sets neither EFFECT_SUMMON nor
/// EFFECT_BY_ID_RANGE), and by AutoBattle_RunAttackOrItem
/// (battle/auto_battle.c).
///
/// Sets work->args before the handler runs, which every handler and
/// task reads the effect through. BATTLE_EFFECT_NONE is no effect and
/// only clears args->variant.
void BattleEffect_RunFromTable(struct BattleEffectParams *args) {
  s32 kind;
  struct BattleEffectWork *work;

  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  work->args = args;
  kind = args->kind;
  if (kind == BATTLE_EFFECT_NONE)
    args->variant = EFFECT_VARIANT_BASIC;
  else
    gBattleEffect_Handlers[kind - 1](args);

  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

/// Builds window 0's right edge for every scanline and restarts the HBlank
/// DMA that writes it to WIN0H each line.
///
/// Runs each VBlank as a task in the VBlank pass, added by
/// BattleEffect_RunNeptune (neptune.c) and BattleEffect_RunBoreas (boreas.c),
/// which fill the edge offsets and remove the task again. Writes the WIN0H
/// table and stops and re-arms DMA channel 0.
///
/// Ram_MapGridCellBuffer + 0    : the base right edge (u16)
/// Ram_MapGridCellBuffer + 2    : how far each of lines 8 to 135 pulls the edge
/// in
///                            (u8 x 128)
/// Ram_MapGridCellBuffer + 0x82 : the WIN0H table built here (u16 x 160)
/// Lines outside 8 to 135 get 0xfff1, a left edge (255) past the right (241);
/// window 0's vertical range, lines 16 to 135 since
/// BattleEffect_ConfigureEffectDisplay, leaves them out anyway. Edges are
/// clamped to 0 to 240.
void BattleEffect_BuildWindowEdgeTable(void) {
  u16 *baseRightEdge;
  u16 *scanlineEdges;
  u8 *edgeOffsets;
  s32 scanline;
  s32 rightEdge;

  baseRightEdge = (u16 *)Ram_MapGridCellBuffer;
  scanlineEdges = (u16 *)(Ram_MapGridCellBuffer + 0x82);
  edgeOffsets = Ram_MapGridCellBuffer + 2;
  for (scanline = 0; scanline != 160; scanline++) {
    if (scanline >= 8 && scanline <= 135) {
      rightEdge = *baseRightEdge - edgeOffsets[scanline - 8];
      CLAMP(rightEdge, 0, 240);
      scanlineEdges[scanline] = rightEdge;
    } else {
      scanlineEdges[scanline] = 0xfff1;
    }
  }
  {
    volatile u16 *channel = REG_DMA0;

    Dma_Stop(channel);
    Dma_Set(Ram_MapGridCellBuffer + 0x82, (void *)&REG_WIN0H,
            DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
  }
}

/// Shows the battle objects of every unit with HP left on the targets' side:
/// the enemies (units 128 to 133) when the first target is one, otherwise
/// the party (units 0 to 7).
///
/// Called by the summon effects (Ramses, Nereid, Kirin, Atalanta, Cybele,
/// Neptune, Tiamat, Procne, Judgment, Boreas, Thor) and the particle streams
/// as they open. Spawns the
/// actors through BattleObject_SpawnObjectsForListFar.
void BattleEffect_SelectLivingTargets(struct BattleEffectParams *args) {
  s16 list[14];
  s32 count;
  s32 i;

  count = 0;
  if (args->targets[0] >= FIRST_ENEMY_UNIT) {
    for (i = 0; i != 6; i++) {
      s32 id = i + 128;

      if (Unit_GetFar(id)->hp > 0)
        list[count++] = id;
    }
  } else {
    for (i = 0; i != 8; i++) {
      if (Unit_GetFar(i)->hp > 0)
        list[count++] = i;
    }
  }
  list[count] = 0xff; // the end of the list
  BattleObject_SpawnObjectsForListFar(list, 0);
}

/// Shows the BG1 battle background behind a cleared canvas: clears the canvas
/// buffer and the 16 KiB of VRAM it is copied to, then adds 14/16 of BG1 and
/// BG2 to all of what lies below them and moves the projection centre down to
/// line 120.
///
/// Called by the summon effects (Ramses, Kirin, Atalanta, Neptune, Procne,
/// Judgment, Boreas, Thor) and the particle streams as they close, to give
/// the battle back its own background. Sets gBgScroll[1].y to 32, reloads
/// BG1 with the battle's background darkened to level 24, waits one frame
/// with every layer off, then writes REG_BLDALPHA, REG_BLDCNT and
/// REG_DISPCNT and gProjection.centerY.
void BattleEffect_SetupBlendedDisplay(void) {
  void **slots = &gHeapSlots[HEAP_SLOT_BATTLE];
  u8 *canvas = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
  struct BattleWork *battle = slots[0];
  volatile u32 fill;

  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = 32;
  // Level 24 is 60% brightness: each level takes 1092 from the 16.16
  // brightness (BattleBackground_Load).
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
  Dma_Fill32Cell(fill, 0, canvas, CANVAS_BYTES);
  Dma_Fill32Cell(fill, 0, BG_CHAR_BLOCK(1), CANVAS_BYTES);
  WaitFrames(1);
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = 120;
}

/// Updates every effect group of one unit's battle object.
///
/// unitId: the unit whose battle object is updated.
/// palette: the colour mode each sprite part takes (enum SpriteColorMode), 0
///          for the unit's own, or -1 to leave the parts alone. A part
///          that changes colours also restarts its frame.
/// animation: the pose each group switches to (enum BattlePose), or -1 for
///            none.
/// timerSlot: the actor timer (gBattleEffectWork's actorTimers) to set, or -1;
///            BattleEffect_CountDownBattlerTints restores the unit's palette
///            when it runs out.
/// timerFrames: the frames that timer counts.
/// The parts and effects of a unit that is fading out stay as they are,
/// but its timer is still set.
///
/// Called throughout the effects, most often through
/// EFFECT_STRIKE_TARGET when a hit lands. Writes work->actorTimers[timerSlot]
/// and each part's param and frame (0xff restarts it).
void BattleEffect_TintBattler(s32 unitId, s32 palette, s32 animation,
                              s32 timerSlot, s32 timerFrames) {
  struct BattleObject *slot;
  struct Sprite *group;
  struct BattleEffectWork *work;
  s32 groupIndex;

  slot = GetBattleObjectSlotFar(unitId);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  groupIndex = 0;

  while ((group = GetMotionRecordFar(slot->actor, groupIndex)) != NULL) {
    // The timer's low byte, read through memory as the original does.
    if (timerSlot != -1) {
      work->actorTimers[timerSlot] = ((u8 *)&timerFrames)[0];
    }
    if (slot->fading == 0) {
      if (palette != -1) {
        s32 partIndex;

        partIndex = 0;
        if (group->count != 0) {
          struct SpriteAnimation **parts;

          parts = group->entries;
          do {
            struct SpriteAnimation *part;

            // Every part but the unit's shadowEntry and animationEntry.
            part = *parts++;
            if (part != NULL &&
                part != (struct SpriteAnimation *)slot->shadowEntry &&
                part != (struct SpriteAnimation *)slot->animationEntry) {
              if (palette == 0)
                part->colorMode = BattleObject_GetPaletteFar(unitId);
              else
                part->colorMode = palette;
              part->picture = 0xff;
            }
            partIndex++;
          } while (partIndex != group->count);
        }
      }
      if (animation != -1)
        Sprite_StartAnimationFar(group, animation);
    }
    groupIndex++;
  }
}

/// Writes the tiled canvas tilemap for BG2 (priority 1), then closes the
/// canvas layer again, which fades the background palette back in.
///
/// Nothing in The Broken Seal calls it; The Lost Age keeps it too.
UNUSED void BattleEffect_InitializeTilemapAndFlushQueue(void) {
  BattleEffect_StartTiledCanvas(1);
  BattleEffect_EndCanvasLayer();
}
