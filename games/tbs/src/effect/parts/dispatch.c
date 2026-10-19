/// @file
/// Running scene and table effects, and shared target helpers.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

/// Calls the empty BattleFx_DoNothing count times, without waiting for a
/// frame.
/// count : how many calls
void BattleFx_CallDoNothing(s32 count) {
  s32 callIndex;

  callIndex = 0;
  if (count != 0) {
    do {
      callIndex += 1;
      BattleFx_DoNothing();
    } while (callIndex != count);
  }
}

// Two of the battle's three entry points, BattleFx_RunSceneEffect and
// BattleFx_RunTableEffect; README.md describes all three and the heap blocks
// they lend an effect. The shared helpers after them manage the scanline
// window and the targets' animation objects.

/// Runs one of the large battle effects, the ones whose presentation flags
/// have bit 15 set: takes the effect's sheet, work and canvas, runs the
/// effect chosen by its kind to the end, and frees them again.
/// Each scene is named for the summon whose gBattle_ActionFlags entry selects
/// its kind, as the handlers of gBattleFx_EffectHandlers are; kind 0, which no
/// ability selects, runs Meteor's scene too.
void BattleFx_RunSceneEffect(struct BattleEffectArgument *effect) {
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT,
                            sizeof(struct BattleEffectWork));
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);

  if (effect->kind == 0) {
    BattleFx_RunMeteor(effect);
  } else {
    switch (effect->kind) {
    case SCENE_EFFECT_RAMSES:
      BattleFx_RunRamses(effect);
      break;
    case SCENE_EFFECT_NEREID:
      BattleFx_RunNereid(effect);
      break;
    case SCENE_EFFECT_KIRIN:
      BattleFx_RunKirin(effect);
      break;
    case SCENE_EFFECT_ATALANTA:
      BattleFx_RunAtalanta(effect);
      break;
    case SCENE_EFFECT_CYBELE:
      BattleFx_RunCybele(effect);
      break;
    case SCENE_EFFECT_NEPTUNE:
      BattleFx_RunNeptune(effect);
      break;
    case SCENE_EFFECT_TIAMAT:
      BattleFx_RunTiamat(effect);
      break;
    case SCENE_EFFECT_PROCNE:
      BattleFx_RunProcne(effect);
      break;
    case SCENE_EFFECT_JUDGMENT:
      BattleFx_RunJudgment(effect);
      break;
    case SCENE_EFFECT_BOREAS:
      BattleFx_RunBoreas(effect);
      break;
    case SCENE_EFFECT_METEOR:
      BattleFx_RunMeteor(effect);
      break;
    case SCENE_EFFECT_THOR:
      BattleFx_RunThor(effect);
      break;
    }
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

/// Runs an ordinary battle effect: takes the effect's sheet, work and
/// canvas, runs the entry of gBattleFx_EffectHandlers for its kind,
/// and frees them again. Kind 0 is no effect and only clears the variant
/// in the shared command.
void BattleFx_RunTableEffect(struct BattleEffectArgument *effect) {
  s32 kind;
  struct BattleEffectWork *work;

  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT,
                            sizeof(struct BattleEffectWork));
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  work->effect = effect;
  kind = effect->kind;
  if (kind == 0)
    effect->variant = 0;
  else
    gBattleFx_EffectHandlers[kind - 1](effect);

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

/// Scheduler callback: builds window 0's right edge for every scanline and
/// restarts the HBlank DMA that writes it to WIN0H each line.
/// Ram_MapCellBuffer + 0    : the base right edge (u16)
/// Ram_MapCellBuffer + 2    : how far each of lines 8 to 135 pulls the edge in
///                            (u8 x 128)
/// Ram_MapCellBuffer + 0x82 : the WIN0H table built here (u16 x 160)
/// Lines outside 8 to 135 get 0xfff1, a left edge past the right, so nothing
/// of them is in the window.
void BattleFx_BuildWindowEdgeTable(void) {
  u16 *baseRightEdge;
  u16 *scanlineEdges;
  u8 *edgeOffsets;
  s32 scanline;
  s32 rightEdge;

  baseRightEdge = (u16 *)Ram_MapCellBuffer;
  scanlineEdges = (u16 *)(Ram_MapCellBuffer + 0x82);
  edgeOffsets = Ram_MapCellBuffer + 2;
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
    Dma_Set(Ram_MapCellBuffer + 0x82, (void *)&REG_WIN0H,
            DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
  }
}

/// Shows the battle objects of every unit with HP left on the targets' side:
/// the enemies (units 128 to 133) when the first target is one, otherwise
/// the party (units 0 to 7).
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect) {
  s16 list[14];
  s32 count;
  s32 i;

  count = 0;
  if (effect->actors[0] > 127) {
    for (i = 0; i != 6; i++) {
      s32 id = i + 128;

      if (UnitGetFar(id)->hp > 0)
        list[count++] = id;
    }
  } else {
    for (i = 0; i != 8; i++) {
      if (UnitGetFar(i)->hp > 0)
        list[count++] = i;
    }
  }
  list[count] = 0xff; // the end of the list
  BattleActor_SpawnObjectsForListFar(list, 0);
}

/// Shows the BG1 battle background behind a cleared canvas: clears the canvas
/// buffer and the 16 KiB of VRAM it is copied to, then blends BG2 over the
/// rest at 14/16 and moves the projection centre down to line 120.
void BattleFx_SetupBlendedDisplay(void) {
  void **slots = &gWorkSlot[HEAP_SLOT_BATTLE];
  u8 *canvas = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
  struct BattleWork *battle = slots[0];
  volatile u32 fill;

  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, battle->background, 24);
  Dma_Fill32Cell(fill, 0, canvas, CANVAS_BYTES);
  Dma_Fill32Cell(fill, 0, BG_CHAR_BLOCK(1), CANVAS_BYTES);
  WaitFrames(1);
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = 120;
}

/// Updates every animation group of one battler's object.
///
/// actorId: the battler whose object is updated.
/// palette: the colour mode each sprite part takes (enum SpriteColorMode), 0
///          for the battler's own, or -1 to leave the parts alone. A part
///          that changes colours also restarts its frame.
/// animation: the pose each group switches to (enum BattlePose), or -1 for
///            none.
/// timerSlot: the actor timer (gBattleFxWork's actorTimers) to set, or -1;
///            ObjectGroup_TickMemberTimers restores the battler's palette
///            when it runs out.
/// timerFrames: the frames that timer counts.
/// The parts and animations of a battler that is fading out stay as they are,
/// but its timer is still set.
void ObjectGroup_UpdateMembers(s32 actorId, s32 palette, s32 animation,
                               s32 timerSlot, s32 timerFrames) {
  struct BattleObj *slot;
  struct AnimationObject *group;
  struct BattleEffectWork *work;
  s32 groupIndex;

  slot = GetBattleObjectSlotFar(actorId);
  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  groupIndex = 0;

  while ((group = GetMotionRecordFar(slot->object, groupIndex)) != NULL) {
    // The timer's low byte, read through memory as the original does.
    if (timerSlot != -1) {
      work->actorTimers[timerSlot] = ((u8 *)&timerFrames)[0];
    }
    if (slot->fading == 0) {
      if (palette != -1) {
        s32 partIndex;

        partIndex = 0;
        if (group->count != 0) {
          struct AnimationEntry **parts;

          parts = group->entries;
          do {
            struct AnimationEntry *part;

            // Every part but the battler's effect and animation entries.
            part = *parts++;
            if (part != NULL &&
                part != (struct AnimationEntry *)slot->effectEntry &&
                part != (struct AnimationEntry *)slot->animationEntry) {
              if (palette == 0)
                part->param = BattleMotion_GetSlotPaletteFar(actorId);
              else
                part->param = palette;
              part->frame = 0xff;
            }
            partIndex++;
          } while (partIndex != group->count);
        }
      }
      if (animation != -1)
        AnimationObjects_SelectAnimationFar(group, animation);
    }
    groupIndex++;
  }
}

/// Writes the tiled canvas tilemap for BG2 (priority 1), then closes the
/// canvas layer again, which fades the background palette back in.
void BattleFx_InitTilemapAndFlushQueue(void) {
  BattleFx_BeginTiledCanvas(1);
  BattleFx_EndCanvasLayer();
}
