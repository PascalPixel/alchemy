// Measured 2026-11-05 (tla-en): differ 10 of 2239. The spill slots agree: gcse
// numbers its reaching registers by hash bucket, (plus shots -8) buckets by
// shots' pseudo number, so shots, user and k are declared in the block where
// the volley starts. Left: sites of order.

// Draft, differ 130 of 2239, skeleton gap 48 (tla-en, 2026-11-01)
// BattleEffect_RunProjectileVolleyB, the body behind the 22 wrappers
// of projectile_volley_b.c (23 effect kinds, modes 0-24).

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

// What is left: the allocation now agrees (mode r11, i r9, target r10, the
// frame 252); the skeleton gap is scheduling ties in the launch, the mode 9,
// 19 and 20 draws and the BG_PLTT copies, plus the final clears, where 0x3c00
// stays in a register that the native rematerializes (a local function
// pointer does not change it). Most of the differ is spill slots: draw sits
// at sp+52 where the native has sp+36, after the loop's hoisted pseudos, so
// draw looks to have had a hard register before reload evicted it.
// Forms that decided the allocation: i is one counter for every r9 loop;
// user is a pointer spilled to sp+60; targets and aim are arrays; widths and
// heights are set inside the glint loop, their NULL defaults keeping loop.c
// from hoisting them; height 12 is hoisted only as a local; a launch with
// x += dx << 12 sends the whole allocation away.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_action.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "motobj.h"
#include "actor.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

static __inline__ s32 CopyWords_(void *destination, const void *source, s32 size) {
  return Iwram_CopyWords(destination, source, size);
}


static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}


// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Palette_StepFadeTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *pair);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
s32 Func_08196404(s32 slot, s32 width, s32 height, s32 flags,
                   s32 mode); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void Func_081180e8(s32 phase);       // BattlePlayback_StartFar
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void Func_0815585c(struct BattleEffectArgument *effect, s32 djinni, s32 *origin,
                   s32 *target);  // BattleEffect_RunDjinniEntrance
void Func_0815f0a0(s32 resource); // BattleEffect_StepPaletteToResource
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);
void SummonEffect_RunCanvasMode1(struct BattleEffectArgument *effect);
void SummonEffect_RunCanvasMode3(struct BattleEffectArgument *effect);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 actor);

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus The
// Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// Func_0815585c's packed argument: the Djinni's element, the side, and
// where the Djinni stands.
enum { DJINNI_VENUS, DJINNI_MERCURY, DJINNI_MARS, DJINNI_JUPITER };
#define DJINNI_SIDE(side) ((side) << 4)
#define DJINNI_BEHIND 64

// Places in the sheet.
enum VolleyBSheet {
  VOLLEY_B_SHOTS = 0x0000,   // the mode's own shot pictures
  VOLLEY_B_BEAM = 0x1000,    // the beam, doubled to 48 wide
  VOLLEY_B_GLOW = 0x1492,    // mode 20's glows
  VOLLEY_B_PICTURE = 0x0c56, // the mode's picture
  VOLLEY_B_DJINNI = 0x65c0   // the 20x40 Djinni
};

#define MOTES ((struct EffectStep *)Ram_MapCellBuffer)
#define HI(v) (((s16 *)&(v))[1]) // the i half of a 16.16 value

extern char ResourceId_Data134; // the particle sheet
extern char ResourceId_TornadoSheet;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_VioletLightningSheetA;
extern char ResourceId_Data0D7;
extern char ResourceId_BoulderSheet;
extern char ResourceId_DemonFaceSheet;
extern char ResourceId_HeartSheet;
extern char ResourceId_SmokeSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_WaterSpraySheet;
extern char ResourceId_PinkPalette;
extern char ResourceId_RockSpireSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_LimePalette;
extern char ResourceId_CyanPalette;
extern char ResourceId_BlastSheet;
extern char ResourceId_BlueBeamSheet;
extern char ResourceId_IceBlockSheet;
extern char ResourceId_EffectPalette01;
extern u16 gData_08197410[]; // gBattleEffect_MoteCells
extern u16 gData_08197486[]; // gBattleEffect_GlintCells
extern u8 gData_08197492[];  // gBattleEffect_GlintCellWidths
extern u8 gData_08197498[];  // gBattleEffect_GlintCellHeights
extern u8 gData_0819749e[];  // gSpire_ShardWidths
extern u8 gData_081974ad[];  // the shard heights
extern u16 gData_081974bc[]; // gSpire_ShardCells
extern u8 Data_081983c8[];   // gTargetStrike_GlintDrawFlags

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
      Func_0815585c(effect,
                    DJINNI_MARS | DJINNI_SIDE(effect->side) | DJINNI_BEHIND,
                    origin, djinniTarget);
      Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                                 work->sheet + VOLLEY_B_DJINNI, 1, 0);
    } else {
      Func_0815585c(effect,
                    DJINNI_VENUS | DJINNI_SIDE(effect->side) | DJINNI_BEHIND,
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
      BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
      pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));
      
      pair[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    }
  } else if (mode == 20) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_TRANSPARENT, CLIP));
    pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
    
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
      CopyWords_((void *)BG_PLTT,
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
    CopyWords_((void *)BG_PLTT,
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
    CopyWords_((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 19) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 0);
    CopyWords_((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (mode == 24) {
    // Six smoke pictures, each with more holes punched at random.
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 0);
    for (i = 0; i != 6; i++) {
      for (j = 0; j != i * 256 + 512; j++)
        work->sheet[VOLLEY_B_PICTURE + i * 1152 +
                    Random16() % 1152] = 0;
    }
    CopyWords_((void *)BG_PLTT,
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
      CopyWords_((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_LimePalette),
                      128);
    else
      CopyWords_((void *)BG_PLTT,
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
            work->sheet[VOLLEY_B_PICTURE + (i << 11) +
                        (Random16() & 0x7ff)] = 0;
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
      if (palette != NULL)
        /* FAKEMATCH(temporary): the routine's address named first, so its pool load leads the arguments; as a direct call, 2 of 2239 instructions differ in tla-en. */
        {
        __typeof__(Iwram_CopyWords) routine = Iwram_CopyWords;
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
  {s32 shots; struct EffectPosition *user; s32 k;
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
    EffectPosition_ProjectScaledBattler(
        effect->actors[i], (struct EffectPosition *)targets[i]);
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

    object = GetBattleObjectSlotFar(effect->actors[0])->object;
    half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
    for (i = 0; i != 128; i++) {
      struct EffectStep *mote = &MOTES[i];

      mote->x = object->x;
      mote->y = object->y + half;
      mote->z = object->z;
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
      Func_0815f0a0((s32)&ResourceId_CyanPalette);
    if (mode == 10 && frame > 31)
      Func_0815f0a0((s32)&ResourceId_BlastSheet);
    if (frame == 4) {
      if (mode == 9 || mode == 17)
        Sound_PlayFar(92);
      else
        Sound_PlayFar(136);
    }
    if (mode == 8) {
    } else if (mode == 6) {
      if (frame == 60)
        Func_081180e8(134);
    } else if (mode == 24) {
      if (frame == 48)
        Func_081180e8(134);
    } else if (frame == 24) {
      Func_081180e8(134);
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
        pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                user->x - 2, user->y - 32, 72, 62);
      else
        pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
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
        aim[1] = targets[target][1] + ((Trig_Sin(frame << 12) << 8) >> 16) - 128;
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
      s32 c2;

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
        c2 = i / 2 % 3;

        // Every other shot, wobbling up and down.
        if (i & 1) {
          s32 y = shot->y + (Trig_Sin((i * 2 + frame) << 11) << 5);

          pair[1](canvas, work->sheet + c2 * 1024, HI(shot->x) - 16,
                  (y >> 16) - 16, 32, 32);
        }
      } else if (mode == 20) {
        s32 cel = i % 5 * 3 + age % 3;
        u32 width = gData_0819749e[cel];
        s32 x = HI(shot->x);
        u32 half = width >> 1;
        const u8 *hs = gData_081974ad;
        u32 height;
        s32 y;
        x -= half;
        height = hs[cel];
        y = HI(shot->y) - (height >> 1);

        pair[0](canvas, work->sheet + gData_081974bc[cel] + VOLLEY_B_GLOW, x, y,
                width, height);
      } else if (mode == 19) {
        s32 wobble = Trig_Sin((i * 2 + frame) << 11);
        s32 y;
        c2 = age >> 2;
        y = (shot->y + (wobble << 5)) >> 16;

        pair[1](canvas, work->sheet + c2 * 1152, HI(shot->x) - 12, y - 24,
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
        shot->velocityY -= 0x4000;
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
            Func_08196404(188, 7, 7, Data_081983c8[Random16() & 3] | CLIP,
                          BLIT_ADDITIVE);
            ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
                canvas, work->sheet + gData_08197486[cel], x, y,
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
          Func_0814cd48(effect->actors[i], 7, 5, i, 4);
        }
      }
    } else if (mode == 10) {
      if (frame == 18) {
        work->shakeFrames = 4;
        Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
      }
      if (frame == 46)
        Func_0814cd48(effect->actor, 7, -1, -1, 0);
      if (frame == 54)
        Func_0814cd48(effect->actor, 0, -1, -1, 0);
    } else if (mode == 8) {
      if (frame == 20) {
        for (i = 0; i != effect->count; i++)
          Func_0814cd48(effect->actors[i], 6, 5, i, 60);
      }
    } else {
      for (i = 0; i != effect->count; i++) {
        if (frame >= i * 4 + 16 && frame < shots - 8 &&
            (frame & 3) == i) {
          work->shakeFrames = 8;
          if (mode == 6)
            Func_0814cd48(effect->actors[i], 14, 5, i, 3);
          else
            Func_0814cd48(effect->actors[i], 7, 5, i, 3);
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

      object = GetBattleObjectSlotFar(effect->actor)->object;
      half = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
      camera = Ram_HeapSlots->cameraWork;
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      for (i = 0; i != 24; i++) {
        struct EffectStep *mote = &MOTES[i];
        s32 height = 12;

        if (mote->variant >= 0) {
          EffectPosition_ProjectPoint((s32 *)mote, (struct EffectPosition *)screen);
          screen[0] >>= 1;
          pair[0](canvas, sheet + gData_08197410[5], screen[0] - 3,
                  screen[1] - height / 2, 6, height);
          BattleEffectKernels_IntegrateVector3(mote, 62, 0);
          if (frame > i + 16) {
            s32 dx;
            s32 dy;
            s32 dz;

dx = object->x - mote->x;
dy = object->y + half - mote->y;
dz = object->z - mote->z;
            mote->velocityY += dy >> 8;
            mote->velocityX += dx >> 7;
            mote->velocityZ += dz >> 8;
            if ((object->x < 0 ? -object->x : object->x) <
                (mote->x < 0 ? -mote->x : mote->x))
              mote->variant = -1;
          }
        }
      }
    }
    Func_08158ce0(4, 4);
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
    ClearWords_((void *)0x06004000, 0x3c00);
    ClearWords_(canvas, 0x3c00);
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
