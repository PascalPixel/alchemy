/// @file
/// Running summon and table effects, and shared target helpers.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Calls the empty BattleEffect_DoNothing count times: a busy delay.
void BattleEffect_CallDoNothing(s32 count) {
  s32 i;

  for (i = 0; i != count; i++)
    BattleEffect_DoNothing();
}

// Two of the battle's three entry points; README.md describes all three and the
// heap blocks they lend an effect. The shared helpers after them manage the
// scanline window and the targets' sprites.

/// Runs one of the large battle effects, the summons: takes the effect's sheet,
/// work and canvas, runs it and frees them again.
void BattleEffect_RunSummon(struct BattleEffectParams *args) {
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);

  if (args->kind == SUMMON_EFFECT_NONE) {
    BattleEffect_RunMeteor(args);
  } else {
    switch (args->kind) {
    case SUMMON_EFFECT_RAMSES:
      BattleEffect_RunRamses(args);
      break;
    case SUMMON_EFFECT_NEREID:
      BattleEffect_RunNereid(args);
      break;
    case SUMMON_EFFECT_KIRIN:
      BattleEffect_RunKirin(args);
      break;
    case SUMMON_EFFECT_ATALANTA:
      BattleEffect_RunAtalanta(args);
      break;
    case SUMMON_EFFECT_CYBELE:
      BattleEffect_RunCybele(args);
      break;
    case SUMMON_EFFECT_NEPTUNE:
      BattleEffect_RunNeptune(args);
      break;
    case SUMMON_EFFECT_TIAMAT:
      BattleEffect_RunTiamat(args);
      break;
    case SUMMON_EFFECT_PROCNE:
      BattleEffect_RunProcne(args);
      break;
    case SUMMON_EFFECT_JUDGMENT:
      BattleEffect_RunJudgment(args);
      break;
    case SUMMON_EFFECT_BOREAS:
      BattleEffect_RunBoreas(args);
      break;
    case SUMMON_EFFECT_METEOR:
      BattleEffect_RunMeteor(args);
      break;
    case SUMMON_EFFECT_THOR:
      BattleEffect_RunThor(args);
      break;
    }
  }

  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

/// Runs an ordinary battle effect from gBattleEffect_Handlers, with the sheet,
/// work and canvas it lends.
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

/// Builds window 0's right edge for every line and restarts the DMA that writes
/// it.
void BattleEffect_BuildWindowEdgeTable(void) {
  u16 *baseRightEdge;
  u16 *scanlineEdges;
  u8 *edgeOffsets;
  s32 scanline;
  s32 rightEdge;

  baseRightEdge = (u16 *)Ram_MapGridCellBuffer;
  scanlineEdges = (u16 *)(Ram_MapGridCellBuffer + 130);
  edgeOffsets = Ram_MapGridCellBuffer + 2;
  for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
    if (scanline >= 8 && scanline <= 135) {
      rightEdge = *baseRightEdge - edgeOffsets[scanline - 8];
      CLAMP(rightEdge, 0, DISPLAY_WIDTH);
      scanlineEdges[scanline] = rightEdge;
    } else {
      scanlineEdges[scanline] = WIN_RANGE(255, 241);
    }
  }
  {
    volatile u16 *channel = REG_DMA0;

    Dma_Stop(channel);
    Dma_Set(Ram_MapGridCellBuffer + 130, (void *)&REG_WIN0H,
            DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
  }
}

/// Shows the battle objects of every unit with HP left on the targets' side.
void BattleEffect_SelectLivingTargets(struct BattleEffectParams *args) {
  s16 list[UNIT_MAX];
  s32 count;
  s32 i;

  count = 0;
  if (args->targets[0] >= FIRST_ENEMY_UNIT) {
    for (i = 0; i != ENEMY_BATTLE_SIZE; i++) {
      s32 id = i + FIRST_ENEMY_UNIT;

      if (Unit_GetFar(id)->hp > 0)
        list[count++] = id;
    }
  } else {
    for (i = 0; i != PARTY_UNIT_COUNT; i++) {
      if (Unit_GetFar(i)->hp > 0)
        list[count++] = i;
    }
  }
  list[count] = BATTLE_UNIT_LIST_END;
  BattleObject_SpawnObjectsForListFar(list, 0);
}

/// Shows the battle background behind a cleared canvas.
void BattleEffect_SetupBlendedDisplay(void) {
  void **slots = &gHeapSlots[HEAP_SLOT_BATTLE];
  u8 *canvas = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE, HEAP_SLOT_BATTLE_CANVAS);
  struct BattleWork *battle = slots[0];
  volatile u32 fill;

  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
  Dma_Fill32Cell(fill, 0, canvas, CANVAS_BYTES);
  Dma_Fill32Cell(fill, 0, BG_CHAR_BLOCK(1), CANVAS_BYTES);
  wait(1);
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = 120;
}

/// Updates every effect group of one unit's battle object.
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
    if (timerSlot != TINT_KEEP) {
      work->actorTimers[timerSlot] = timerFrames;
    }
    if (slot->fading == 0) {
      if (palette != TINT_KEEP) {
        s32 partIndex;

        for (partIndex = 0; partIndex != group->count; partIndex++) {
          struct SpriteAnimation *part = group->entries[partIndex];

          if (part != NULL &&
              part != (struct SpriteAnimation *)slot->shadowEntry &&
              part != (struct SpriteAnimation *)slot->animationEntry) {
            if (palette == SPRITE_COLOR_NORMAL)
              part->colorMode = BattleObject_GetPaletteFar(unitId);
            else
              part->colorMode = palette;
            part->picture = 0xff;
          }
        }
      }
      if (animation != TINT_KEEP)
        Sprite_StartAnimationFar(group, animation);
    }
    groupIndex++;
  }
}

/// Writes the canvas's tile map for BG2, then closes the canvas layer.
UNUSED void BattleEffect_InitializeTilemapAndFlushQueue(void) {
  BattleEffect_StartTiledCanvas(1);
  BattleEffect_EndCanvasLayer();
}
