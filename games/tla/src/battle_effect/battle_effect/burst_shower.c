/// @file
/// The blessings' burst showers.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

/// Restarts DMA channel 0 as an H-blank transfer of the effect work's line
/// table into BG2X, one word a line.
// The Broken Seal's BattleEffect_ArmBg2AffineHBlankDma.
void BattleEffect_ArmBg2AffineHBlankDma(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[5];
  u32 io;

  channel[5] = control & 0xc5ff;
  channel[5] &= 0x7fff;
  io = 0x80;
  (void)channel[5];
  Dma_Set(work->bg2xLines, (void *)((io << 19) + 0x28),
          DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)channel);
}

/// BattleEffect_ArmBg2AffineHBlankDma for BG1HOFS and BG1VOFS: the line table
/// moves layer 1.
void BattleEffect_ArmBg1HBlankDma(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[5];
  u32 io;

  channel[5] = control & 0xc5ff;
  channel[5] &= 0x7fff;
  io = 0x80;
  (void)channel[5];
  Dma_Set(work->bg2xLines, (void *)((io << 19) + 0x14),
          DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)channel);
}

#include "field_event.h"

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode0,
                   BattleEffect_RunProjectileVolleyB, 0)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode12,
                   BattleEffect_RunProjectileVolleyB, 12)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode2,
                   BattleEffect_RunProjectileVolleyB, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode6,
                   BattleEffect_RunProjectileVolleyB, 6)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode3,
                   BattleEffect_RunProjectileVolleyB, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode5,
                   BattleEffect_RunProjectileVolleyB, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode23,
                   BattleEffect_RunProjectileVolleyB, 23)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode7,
                   BattleEffect_RunProjectileVolleyB, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode21,
                   BattleEffect_RunProjectileVolleyB, 21)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode4,
                   BattleEffect_RunProjectileVolleyB, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode8,
                   BattleEffect_RunProjectileVolleyB, 8)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode24,
                   BattleEffect_RunProjectileVolleyB, 24)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode22,
                   BattleEffect_RunProjectileVolleyB, 22)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode9,
                   BattleEffect_RunProjectileVolleyB, 9)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode13,
                   BattleEffect_RunProjectileVolleyB, 13)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode14,
                   BattleEffect_RunProjectileVolleyB, 14)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode15,
                   BattleEffect_RunProjectileVolleyB, 15)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode16,
                   BattleEffect_RunProjectileVolleyB, 16)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode17,
                   BattleEffect_RunProjectileVolleyB, 17)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode18,
                   BattleEffect_RunProjectileVolleyB, 18)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode19,
                   BattleEffect_RunProjectileVolleyB, 19)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode20,
                   BattleEffect_RunProjectileVolleyB, 20)

void BattleEffect_RunVolleyBMode10WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolleyB(effect, 10);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunVolleyBMode11WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(effect->actor)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolleyB(effect, 11);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

// BattleEffect_RunProjectileVolleyB, the body behind the 22 wrappers
// above (23 effect kinds, modes 0-24).

// New in The Lost Age. Its nearest relative is The Broken Seal's
// BattleEffect_RunProjectileVolley (effect.c; shape 0.44, structure 0.53): the
// same frame: a shot launched each frame from the user toward one target
// after another, every live shot drawn and moved, the targets hit four
// frames apart, and a fade over the last eight frames. The mode chooses
// the picture and how it is prepared (holes punched at random, a level
// remap, a doubled beam, a grey or violet ramp), the palette, the blending,
// how many shots and where they aim; modes 5 and 23 draw one big picture at
// the user instead, mode 10 also draws motes into the user, mode 24 adds
// glints around the targets, and modes 11, 14 and 18 hand over to
// SummonEffect_RunCanvasEffect at the end. Resources not yet named in
// directory.s, by row and asset: 0x151 BattleEffect_MarsDjinnSmallSheet,
// 0x157 BattleEffect_VenusDjinnSmallSheet, 0x11c
// BattleEffect_VioletLightningSheetA, 0xd7 Resource_Data0D7, 0x14d
// BattleEffect_BoulderSheet, 0x116 BattleEffect_DemonFaceSheet, 0x16e
// BattleEffect_HeartSheet, 0x161 BattleEffect_SmokeSheet, 0x13e
// BattleEffect_FlashBurstSheet, 0x12f BattleEffect_WaterSpraySheet, 0x164
// BattleEffect_PinkPalette, 0x187 BattleEffect_RockSpireSheet, 0x150
// BattleEffect_MarsDjinnSheet, 0x148 BattleEffect_OrangePaletteB, 0x163
// BattleEffect_LimePalette, 0x166 BattleEffect_CyanPalette, 0x184
// BattleEffect_BlastSheet, 0x190 BattleEffect_BlueBeamSheet and 0xb9
// BattleEffect_EffectPalette01.

#define DJINNI_BEHIND 64

// A volley of shots flies from the user at the targets, one launched each
// frame toward the next target.

// Setup: an optional Djinni appears behind the user (the prelude); the
// mode prepares its picture and palette and moves the launch point. Frame
// loop: a shot leaves the user each frame for 48 frames (16 in mode 10),
// aimed near the next target; every live shot is drawn and moved and goes
// out after 24 frames (32 in modes 9 and 17); from frame 16 the targets
// are hit four frames apart and recoil; the blend fades out over the last
// eight frames.
void BattleEffect_RunProjectileVolleyB(struct BattleEffectArgument *effect,
                                       u32 mode) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 target;
  s32 i;
  s32 j;
  u8 *widths;
  u8 *heights;
  s32 limit;
  s32 row;
  s32 column;

  u8 *palette;
  s32 origin[3];
  s32 djinniTarget[3];
  struct EffectPosition userPosition;
  s32 targets[8][3];
  s32 aim[3];
  s32 screen[3];
  DrawRectangle pair[2];

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];
  if (mode == 10 || mode == 11 || mode == 13 || mode == 14 || mode == 15 ||
      mode == 17 || mode == 18 || mode == 19)
    BattleEffect_StartCanvasLayer(1);
  else
    BattleEffect_StartCanvasLayer(0);

  // The prelude: a Djinni behind the user, Mars for modes 9, 22 and 24 and
  // Venus otherwise.
  if (effect->prelude == 1) {
    if (mode == 9 || mode == 22 || mode == 24) {
      BattleEffect_RunDjinniEntrance(
          effect, DJINNI_MARS | DJINNI_SIDE(effect->side) | DJINNI_BEHIND,
          origin, djinniTarget);
      Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                                 work->sheet + VOLLEY_B_DJINNI, 1, 0);
    } else {
      BattleEffect_RunDjinniEntrance(
          effect, DJINNI_VENUS | DJINNI_SIDE(effect->side) | DJINNI_BEHIND,
          origin, djinniTarget);
      Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                                 work->sheet + VOLLEY_B_DJINNI, 1, 0);
    }
    REG_BG2CNT = 0x785;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  REG_BLDALPHA = 0x1010;

  // The blitters: [0] MAX and [1] ADDITIVE, mirrored for the far side.
  if (effect->prelude == 1) {
    if (effect->side == 0) {
      BattleEffect_FetchRectangleBlitters(0, pair);
    } else {
      BattleEffect_LoadBlitter(104,
                               BLIT_MODE(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
      pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(
          188, BLIT_MODE(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));

      pair[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    }
  } else if (mode == 20) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_TRANSPARENT, CLIP));
    pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(
        188, BLIT_MODE(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));

    pair[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    REG_BLDALPHA = 0x310;
  } else if (mode == 7 || mode == 21) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP));
    pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));

    pair[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  } else {
    BattleEffect_FetchRectangleBlitters(effect->side, pair);
  }

  // The pictures and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (mode == 5 || mode == 23) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == 9 || mode == 17) {
    // Remap the levels: 0 stays, 1-16 become 1, the rest drop by 16.
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    for (i = 0; i != 0xc00; i++) {
      u8 *cell = work->sheet + i;
      u32 value = *cell;
      u8 level = value;

      if (level == 0)
        *cell = 0;
      else if (level <= 16)
        *cell = 1;
      else
        *cell = value - 16;
    }
    if (mode == 17)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
  } else if (mode == 20) {
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == 7) {
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == 22) {
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 21) {
    // A violet ramp from color 1, blended 16:4.
    for (i = 1; i != 64; i++) {
      s32 low = 15 - i / 4;
      s32 value = ((31 - i / 2) << 10) | low;

      BG_PLTT[i] = value;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 0);
    REG_BLDALPHA = 0x410;
  } else if (mode == 18) {
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 19) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 24) {
    // Six smoke pictures, each with more holes punched at random.
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 0);
    for (i = 0; i != 6; i++) {
      for (j = 0; j != i * 256 + 512; j++)
        work->sheet[VOLLEY_B_PICTURE + i * 1152 + Random16() % 1152] = 0;
    }
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 12 || mode == 16) {
    // The beam, each 24-pixel row drawn twice side by side.
    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet, work->sheet, 1,
                               1);
    for (row = 0; row != 24; row++) {
      for (column = 0; column != 24; column++) {
        work->sheet[VOLLEY_B_BEAM + row * 48 + column] =
            work->sheet[row * 24 + column];
        work->sheet[VOLLEY_B_BEAM + row * 48 + 24 + column] =
            work->sheet[row * 24 + column];
      }
    }
    if (mode == 12)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_LimePalette),
                      128);
    else
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet),
                      128);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
    if (mode == 6) {
      // A grey ramp, unblended.
      for (i = 0; i != 64; i++) {
        s32 level = i / 4;

        BG_PLTT[i] = (level << 10) | (level << 5) | level;
      }
      REG_BLDCNT = 0;
    } else {
      switch (mode) {
      case 13:
      case 14:
      case 15:
        // Six flash pictures, each with more holes punched at random.
        for (i = 0; i != 6; i++) {
          for (j = 0; j != i * 512 + 1024; j++)
            work->sheet[VOLLEY_B_PICTURE + (i << 11) + (Random16() & 0x7ff)] =
                0;
        }
        if (mode == 15) {
          REG_BLDCNT = 0;
          for (i = 0; i != 64; i++) {
            s32 level = i / 4;

            BG_PLTT[i] = (level << 10) | (level << 5) | level;
          }
          palette = NULL;
        } else if (mode == 13) {
          REG_BLDCNT = 0;
          palette = Resource_GetTableEntry((s32)&ResourceId_RockSpireSheet);
        } else {
          palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        }
        break;
      case 0:
        palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
        break;
      case 1:
        palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
        break;
      case 2:
      case 16:
        palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
        break;
      case 3:
        palette = Resource_GetTableEntry((s32)&ResourceId_PinkPalette);
        break;
      case 8:
        palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
        break;
      case 10:
        palette = Resource_GetTableEntry((s32)&ResourceId_EffectPalette01);
        break;
      case 11:
        palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        break;
      default:
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
        break;
      }
      if (palette != NULL) {
        __typeof__(Iwram_CopyWordsEntry) routine = Iwram_CopyWordsEntry;
        routine((void *)BG_PLTT, palette, 128);
      }
    }
  }

  // The trail: none for the rock modes, else halved or to 3/4.
  if (mode == 13 || mode == 15 || mode == 20) {
    work->transferMode = 1;
    work->transferValue = 0;
  } else if (mode == 7 || mode == 9 || mode == 17 || mode == 18 || mode == 19 ||
             mode == 24) {
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  for (i = 0; i != 64; i++)
    work->particles[i].variant = -1;
  {
    s32 shots;
    struct EffectPosition *user;
    s32 k;
    if (mode == 8 || mode == 11 || mode == 14)
      shots = 32;
    else if (mode == 20)
      shots = 64;
    else if (mode == 18)
      shots = 48;
    else
      shots = 56;

    // Where the shots leave from, and where the targets are.
    EffectPosition_ProjectSprite(effect->actor, user = &userPosition);
    for (i = 0; i != effect->count; i++)
      EffectPosition_ProjectScaledBattler(effect->actors[i],
                                          (struct EffectPosition *)targets[i]);
    if (mode == 12 || mode == 15) {
      if (effect->side == 0)
        user->x -= 24;
      else
        user->x += 24;
    }
    if (mode == 20) {
      if (effect->side == 0)
        user->x -= 48;
      else
        user->x += 48;
    }
    if (mode == 21) {
      if (effect->side == 0)
        user->x -= 16;
      else
        user->x += 16;
      user->y -= 40;
    }
    if (mode == 16) {
      if (effect->side == 0)
        user->x -= 32;
      else
        user->x += 32;
    }
    if (mode == 3)
      user->y -= 16;
    if (mode == 8)
      user->y -= 40;
    if (mode == 24)
      user->y -= 48;
    if (mode == 22)
      user->y -= 40;
    if (mode == 9)
      user->y -= 40;
    if (mode == 17)
      user->y -= 16;
    if (mode == 19)
      user->y -= 16;
    if (mode == 10) {
      user->y -= 24;
      for (i = 0; i != effect->count; i++)
        targets[i][1] += 80;
    }
    if (mode == 4) {
      if (effect->side == 1)
        user->x += 28;
      else
        user->x -= 28;
    }
    if (mode == 7) {
      if (effect->side == 1)
        user->x += 16;
      else
        user->x -= 16;
    }
    if (mode == 5 || mode == 23) {
      user->x /= 3;
      if (mode == 23)
        user->y -= 16;
      REG_BG2PA = 0x55; // three times as wide
    }

    // Mode 10: 128 motes burst from the middle of the first target.
    if (mode == 10) {
      struct MotionObject *object;
      s32 half;

      object = BattleObject_GetFar(effect->actors[0])->object;
      half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
      for (i = 0; i != 128; i++) {
        struct EffectStep *mote = &MOTES[i];

        SET_POSITION(mote, object->x, object->y + half, object->z);
        mote->velocityX = ((Random16() & 0xff) - 128) << 10;
        mote->velocityY = ((Random16() & 0xff) - 128) << 10;
        mote->velocityZ = ((Random16() & 0xff) - 128) << 10;
        mote->variant = 0;
      }
    }

    for (frame = 0; frame != shots; frame++) {
      target = frame % effect->count;

      // Fade out over the last eight frames.
      if (frame >= shots - 8)
        REG_BLDALPHA = -((frame - shots) * 2) | 0x1000;
      if (mode == 8 && frame > 15)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);
      if (mode == 10 && frame > 31)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
      if (frame == 4) {
        if (mode == 9 || mode == 17)
          Sound_PlayFar(MUS_092);
        else
          Sound_PlayFar(SE_WHOOSH);
      }
      if (mode == 8) {
      } else if (mode == 6) {
        if (frame == 60)
          BattlePlayback_StartFar(134);
      } else if (mode == 24) {
        if (frame == 48)
          BattlePlayback_StartFar(134);
      } else if (frame == 24) {
        BattlePlayback_StartFar(134);
      }

      // The Djinni sways, and after frame 56 rises two pixels a frame.
      if (effect->prelude == 1) {
        s32 x = ((-Trig_Sin(frame << 11) * 4) >> 16) + origin[0] / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 2) >> 16) + origin[1] - 24;

        if (frame > 56)
          y = y - frame * 2 + 112;
        if (frame <= 3)
          pair[1](canvas, work->sheet + VOLLEY_B_DJINNI, x, y, 20, 40);
        pair[1](canvas, work->sheet + VOLLEY_B_DJINNI, x, y, 20, 40);
      }

      if (mode == 5 || mode == 23) {
        // One big picture at the user, three frames a cel.
        if (effect->side == 1)
          pair[0](canvas,
                  work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                  user->x - 2, user->y - 32, 72, 62);
        else
          pair[0](canvas,
                  work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                  user->x - 70, user->y - 32, 72, 62);
      } else {
        // Launch a shot at a point near this frame's target.
        aim[0] = targets[target][0] + (Random16() & 31) - 16;
        if (mode == 9 || mode == 17 || mode == 19)
          aim[1] = targets[target][1] + (Random16() & 127) - 32;
        else if (mode == 12 || mode == 15 || mode == 18 || mode == 20 ||
                 mode == 21 || mode == 22 || mode == 16)
          aim[1] = targets[target][1] + (Random16() & 127) - 64;
        else if (mode == 24)
          aim[1] = targets[target][1] + Random16() % 96 - 32;
        else if (mode == 11) {
          aim[1] = targets[target][1] + (Random16() & 255) - 192;
          aim[1] =
              targets[target][1] + ((Trig_Sin(frame << 12) << 8) >> 16) - 128;
        } else
          aim[1] = targets[target][1] + (Random16() & 63) - 16;
        limit = 16;
        if (mode != 10)
          limit = 48;
        if (frame < limit) {
          struct EffectStep *shot = &work->particles[frame];
          s32 x = user->x << 15;
          s32 y;
          s32 dx;
          s32 dy;

          shot->x = x;
          y = user->y << 16;
          shot->y = y;
          dx = aim[0] - user->x;
          shot->velocityX = dx << 11;
          dy = aim[1] - user->y;
          shot->velocityY = dy << 11;
          shot->x = x + (dx << 12);
          shot->y = y + (dy << 12);
          shot->variant = 0;
        }
      }

      // Draw and move the shots in flight.
      for (i = 0; i != 64; i++) {
        struct EffectStep *shot = &work->particles[i];
        s32 age = shot->variant;
        s32 cel;

        if (age < 0)
          continue;
        if (mode == 7 || mode == 21) {
          if (mode == 22 && (i & 1))
            continue;
          if (age > 7)
            pair[effect->side](canvas, work->sheet + VOLLEY_B_PICTURE,
                               HI(shot->x) - 16, HI(shot->y) - 32, 32, 64);
        } else if (mode == 22) {
          if (i & 1)
            continue;
          if (age > 2)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE, HI(shot->x) - 16,
                    HI(shot->y) - 32, 32, 64);
        } else if (mode == 12 || mode == 16) {
          if (age > 1)
            pair[0](canvas, work->sheet + VOLLEY_B_BEAM, HI(shot->x) - 12,
                    HI(shot->y) - 24, 24, 48);
        } else if (mode == 9 || mode == 17) {
          cel = i / 2 % 3;

          // Every other shot, wobbling up and down.
          if (i & 1) {
            s32 y = shot->y + (Trig_Sin((i * 2 + frame) << 11) << 5);

            pair[1](canvas, work->sheet + cel * 1024, HI(shot->x) - 16,
                    (y >> 16) - 16, 32, 32);
          }
        } else if (mode == 20) {
          s32 cel = i % 5 * 3 + age % 3;
          u32 width = gSpire_ShardWidths[cel];
          s32 x = HI(shot->x);
          u32 half = width >> 1;
          const u8 *shardHeights = gData_081974ad;
          u32 height;
          s32 y;
          x -= half;
          height = shardHeights[cel];
          y = HI(shot->y) - (height >> 1);

          pair[0](canvas, work->sheet + gSpire_ShardCells[cel] + VOLLEY_B_GLOW,
                  x, y, width, height);
        } else if (mode == 19) {
          s32 wobble = Trig_Sin((i * 2 + frame) << 11);
          s32 y;
          cel = age >> 2;
          y = (shot->y + (wobble << 5)) >> 16;

          pair[1](canvas, work->sheet + cel * 1152, HI(shot->x) - 12, y - 24,
                  24, 48);
        } else if (mode == 18) {
          if (age > 1)
            pair[0](canvas, work->sheet, HI(shot->x) - 11, HI(shot->y) - 21, 22,
                    42);
        } else if (mode == 4) {
          if (age > 5)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 2048,
                    HI(shot->x) - 16, HI(shot->y) - 32, 32, 64);
        } else if (mode == 24) {
          if (age > 1)
            pair[1](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 1152,
                    HI(shot->x) - 12, HI(shot->y) - 24, 24, 48);
        } else if (mode != 5) {
          if (age > 1)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 2048,
                    HI(shot->x) - 16, HI(shot->y) - 32, 32, 64);
        }
        shot->x += shot->velocityX;
        shot->y += shot->velocityY;
        if (mode == 10)
          shot->velocityY -= px(1) / 4;
        shot->variant++;
        if (mode == 9 || mode == 17) {
          if (shot->variant == 32)
            shot->variant = -1;
        } else if (shot->variant == 24) {
          shot->variant = -1;
        }
      }

      // Mode 24: three glints a frame around each target being hit.
      if (mode == 24) {
        for (i = 0; i != effect->count; i++) {
          if (frame >= i * 4 + 16 && frame < shots - 8) {
            // Default: set for real below.
            widths = NULL;
            heights = NULL;
            for (k = 0; k != 3; k++) {
              s32 cel = i & 3;
              s32 angle;
              s32 x;
              s32 y;

              // Default: set for real below.
              widths = NULL;
              heights = NULL;
              angle = Random16() & 0xffff;
              x = (Trig_Sin(angle) << 4) >> 16;
              x += targets[i][0] / 2;
              widths = gData_08197492;
              x -= widths[cel] >> 1;
              y = (Trig_Cos(angle) << 4) >> 16;
              y += targets[i][1];
              heights = gData_08197498;
              y -= heights[cel] >> 1;

              HeapSlot_Release(188);
              HeapSlot_Release(104);
              BattleEffect_BuildBlitter(
                  188, 7, 7,
                  gTargetStrike_GlintDrawFlags[Random16() & 3] | CLIP,
                  BLIT_ADDITIVE);
              ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
                  canvas, work->sheet + gBattleEffect_GlintCells[cel], x, y,
                  widths[cel], heights[cel]);
              HeapSlot_Release(188);
              BattleEffect_FetchRectangleBlitters(effect->side, pair);
            }
          }
        }
      }

      // The hits.
      if (mode == 9 || mode == 17) {
      } else if (mode == 5 || mode == 23) {
        for (i = 0; i != effect->count; i++) {
          if (frame >= i * 4 + 2 && (frame & 7) == i) {
            work->shakeFrames = 8;
            BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 4);
          }
        }
      } else if (mode == 10) {
        if (frame == 18) {
          work->shakeFrames = 4;
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
        }
        if (frame == 46)
          BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
        if (frame == 54)
          BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);
      } else if (mode == 8) {
        if (frame == 20) {
          for (i = 0; i != effect->count; i++)
            BattleEffect_TintBattler(effect->actors[i], 6, 5, i, 60);
        }
      } else {
        for (i = 0; i != effect->count; i++) {
          if (frame >= i * 4 + 16 && frame < shots - 8 && (frame & 3) == i) {
            work->shakeFrames = 8;
            if (mode == 6)
              BattleEffect_TintBattler(effect->actors[i], 14, 5, i, 3);
            else
              BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 3);
            if (mode != 11 && mode != 14)
              BattleEffect_StartActorMotion(effect->actors[i], 1, 0x10000,
                                            0x40000, 0x20000, frame + 100);
          }
        }
      }

      // Mode 10: from frame 16 the motes are drawn into the user, and go out
      // once past it.
      if (mode == 10 && frame > 15) {
        struct MotionObject *object;
        s32 half;
        s32 *camera;

        object = BattleObject_GetFar(effect->actor)->object;
        half = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
        camera = Ram_HeapSlots->cameraWork;
        Transform_Reset();
        Transform_SetLookAt(camera, camera + 3);
        for (i = 0; i != 24; i++) {
          struct EffectStep *mote = &MOTES[i];
          s32 height = 12;

          if (mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote,
                                        (struct EffectPosition *)screen);
            screen[0] >>= 1;
            pair[0](canvas, sheet + gBattleEffect_MoteCells[5], screen[0] - 3,
                    screen[1] - height / 2, 6, height);
            BattleEffectKernels_IntegrateVector3(mote, 62, 0);
            if (frame > i + 16) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = object->x - mote->x;
              dy = object->y + half - mote->y;
              dz = object->z - mote->z;
              /* FAKEMATCH(barrier): an empty asm through dy adds the references
               * that rank it above dz and dx. */
              __asm__("" : "=r"(dy) : "0"(dy));
              /* FAKEMATCH(barrier): an empty asm through dz ranks it above dx.
               */
              __asm__("" : "=r"(dz) : "0"(dz));
              mote->velocityX += dx >> 7;
              mote->velocityY += dy >> 8;
              mote->velocityZ += dz >> 8;
              if ((object->x < 0 ? -object->x : object->x) <
                  (mote->x < 0 ? -mote->x : mote->x))
                mote->variant = -1;
            }
          }
        }
      }
      BattleCamera_Shake(4, 4);
      if (mode != 6)
        BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    if (mode == 11 || mode == 14 || mode == 18) {
      // Hand over to the canvas effect: clear both canvases first.
      Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), 0x3c00);
      Iwram_ClearWords(canvas, 0x3c00);
      effect->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      if (mode == 14)
        SummonEffect_RunCanvasMode1(effect);
      else
        SummonEffect_RunCanvasMode3(effect);
    } else {
      BattleEffect_EndCanvasLayer();
    }
  }
}
