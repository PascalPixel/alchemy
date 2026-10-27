// Draft of BattleEffect_RunProjectileVolleyB, the body behind the 22 wrappers
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

// Draft: differ 2442 of 2239 (draft 2215), a first transcription of the
// listing block by block. The frame is 244 bytes where the native's is 252
// (two fewer spill slots: the mode sits in r9 here, r11 there), so every
// stack offset and most allocation differs; 24 instructions are missing,
// spread over the mode tests.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void Palette_StepFadeTransfer(void); // Palette_StepFadeTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void BattleEffect_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *pair); // BattleEffect_FetchRectangleBlitters
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_08196404(s32 slot, s32 width, s32 height, s32 flags,
                   s32 mode); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_StartPhaseFar
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void Func_0815585c(struct BattleEffectArgument *effect, s32 djinni, s32 *origin,
                   s32 *target);  // BattleEffect_RunDjinniEntrance
void Func_0815f0a0(s32 resource); // BattleEffect_StepPaletteToResource
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void EffectPosition_ProjectSprite(s32 id,
                                             struct EffectPosition *position);
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
#define HI(v) (((s16 *)&(v))[1]) // the pixel half of a 16.16 value

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
  u8 *sheet;
  s32 frame;
  s32 shots;
  s32 target;
  s32 i;
  s32 column;
  s32 smokeHole;
  s32 color;
  s32 pixel;
  s32 k;
  s32 flashPicture;
  s32 row;
  s32 smokePicture;
  s32 member;
  s32 glowMember;
  s32 flashMember;
  DrawRectangle *draw;
  u8 *palette;
  s32 origin[3];
  s32 djinniTarget[3];
  struct EffectPosition user;
  struct EffectPosition targets[8];
  struct EffectPosition aim;
  struct EffectPosition screen;
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
      BattleEffect_FetchRectangleBlitters(0, draw = pair);
    } else {
      BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
      pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));
      draw = pair;
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    }
  } else if (mode == 20) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_TRANSPARENT, CLIP));
    pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
    draw = pair;
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    REG_BLDALPHA = 0x310;
  } else if (mode == 7 || mode == 21) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP));
    pair[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw = pair;
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  } else {
    BattleEffect_FetchRectangleBlitters(effect->side, draw = pair);
  }

  // The pictures and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (mode == 5 || mode == 23) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == 9 || mode == 17) {
    // Remap the levels: 0 stays, 1-16 become 1, the rest drop by 16.
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    for (pixel = 0; pixel != 0xc00; pixel++) {
      u8 level = work->sheet[pixel];

      if (level == 0)
        work->sheet[pixel] = 0;
      else if (level <= 16)
        work->sheet[pixel] = 1;
      else
        work->sheet[pixel] = level - 16;
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
    for (color = 1; color != 64; color++)
      BG_PLTT[color] = ((31 - color / 2) << 10) | (15 - color / 4);
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
    for (smokePicture = 0; smokePicture != 6; smokePicture++) {
      for (smokeHole = 0; smokeHole != smokePicture * 256 + 512; smokeHole++)
        work->sheet[VOLLEY_B_PICTURE + smokePicture * 1152 +
                    Random16() % 1152] = 0;
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
      case 13:
      case 14:
      case 15:
        // Six flash pictures, each with more holes punched at random.
        for (flashPicture = 0; flashPicture != 6; flashPicture++) {
          for (i = 0; i != flashPicture * 512 + 1024; i++)
            work->sheet[VOLLEY_B_PICTURE + (flashPicture << 11) +
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
      default:
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
        break;
      }
      if (palette != NULL)
        Iwram_CopyWords((void *)BG_PLTT, palette, 128);
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
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  for (i = 0; i != 64; i++)
    work->particles[i].variant = -1;
  if (mode == 8 || mode == 11 || mode == 14)
    shots = 32;
  else if (mode == 20)
    shots = 64;
  else if (mode == 18)
    shots = 48;
  else
    shots = 56;

  // Where the shots leave from, and where the targets are.
  EffectPosition_ProjectSprite(effect->actor, &user);
  for (member = 0; member != effect->count; member++)
    EffectPosition_ProjectScaledBattler(effect->actors[member],
                                       &targets[member]);
  if (mode == 12 || mode == 15) {
    if (effect->side == 0)
      user.x -= 24;
    else
      user.x += 24;
  }
  if (mode == 20) {
    if (effect->side == 0)
      user.x -= 48;
    else
      user.x += 48;
  }
  if (mode == 21) {
    if (effect->side == 0)
      user.x -= 16;
    else
      user.x += 16;
    user.y -= 40;
  }
  if (mode == 16) {
    if (effect->side == 0)
      user.x -= 32;
    else
      user.x += 32;
  }
  if (mode == 3)
    user.y -= 16;
  if (mode == 8)
    user.y -= 40;
  if (mode == 24)
    user.y -= 48;
  if (mode == 22)
    user.y -= 40;
  if (mode == 9)
    user.y -= 40;
  if (mode == 17)
    user.y -= 16;
  if (mode == 19)
    user.y -= 16;
  if (mode == 10) {
    user.y -= 24;
    for (member = 0; member != effect->count; member++)
      targets[member].y += 80;
  }
  if (mode == 4) {
    if (effect->side == 1)
      user.x += 28;
    else
      user.x -= 28;
  }
  if (mode == 7) {
    if (effect->side == 1)
      user.x += 16;
    else
      user.x -= 16;
  }
  if (mode == 5 || mode == 23) {
    user.x /= 3;
    if (mode == 23)
      user.y -= 16;
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
        Audio_PlayCue(92);
      else
        Audio_PlayCue(136);
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
        draw[1](canvas, work->sheet + VOLLEY_B_DJINNI, x, y, 20, 40);
      draw[1](canvas, work->sheet + VOLLEY_B_DJINNI, x, y, 20, 40);
    }

    if (mode == 5 || mode == 23) {
      // One big picture at the user, three frames a cel.
      if (effect->side == 1)
        pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                user.x - 2, user.y - 32, 72, 62);
      else
        pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                user.x - 70, user.y - 32, 72, 62);
    } else {
      // Launch a shot at a point near this frame's target.
      aim.x = targets[target].x + (Random16() & 31) - 16;
      if (mode == 9 || mode == 17 || mode == 19)
        aim.y = targets[target].y + (Random16() & 127) - 32;
      else if (mode == 12 || mode == 15 || mode == 18 || mode == 20 ||
               mode == 21 || mode == 22 || mode == 16)
        aim.y = targets[target].y + (Random16() & 127) - 64;
      else if (mode == 24)
        aim.y = targets[target].y + Random16() % 96 - 32;
      else if (mode == 11) {
        aim.y = targets[target].y + (Random16() & 255) - 192;
        aim.y = targets[target].y + ((Trig_Sin(frame << 12) << 8) >> 16) - 128;
      } else
        aim.y = targets[target].y + (Random16() & 63) - 16;
      if (frame < (mode == 10 ? 16 : 48)) {
        struct EffectStep *shot = &work->particles[frame];

        shot->x = user.x << 15;
        shot->y = user.y << 16;
        shot->velocityX = (aim.x - user.x) << 11;
        shot->velocityY = (aim.y - user.y) << 11;
        shot->x += (aim.x - user.x) << 12;
        shot->y += (aim.y - user.y) << 12;
        shot->variant = 0;
      }
    }

    // Draw and move the shots in flight.
    for (i = 0; i != 64; i++) {
      struct EffectStep *shot = &work->particles[i];
      s32 age = shot->variant;

      if (age < 0)
        continue;
      if (mode == 7 || mode == 21) {
        if (mode == 22 && (i & 1))
          continue;
        if (age > 7)
          draw[effect->side](canvas, work->sheet + VOLLEY_B_PICTURE,
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
        s32 cel = i / 2 % 3;

        // Every other shot, wobbling up and down.
        if (i & 1)
          draw[1](canvas, work->sheet + cel * 1024, HI(shot->x) - 16,
                  ((shot->y + (Trig_Sin((i * 2 + frame) << 11) << 5)) >> 16) -
                      16,
                  32, 32);
      } else if (mode == 20) {
        s32 cel = i % 5 * 3 + age % 3;
        u32 width = gData_0819749e[cel];
        u32 height = gData_081974ad[cel];

        pair[0](canvas, work->sheet + gData_081974bc[cel] + VOLLEY_B_GLOW,
                HI(shot->x) - (width >> 1), HI(shot->y) - (height >> 1), width,
                height);
      } else if (mode == 19) {
        s32 wobble = Trig_Sin((i * 2 + frame) << 11);

        draw[1](canvas, work->sheet + (age >> 2) * 1152, HI(shot->x) - 12,
                ((shot->y + (wobble << 5)) >> 16) - 24, 24, 48);
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
          draw[1](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 1152,
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
      for (member = 0; member != effect->count; member++) {
        if (frame >= member * 4 + 16 && frame < shots - 8) {
          s32 cel = member & 3;

          for (k = 0; k != 3; k++) {
            s32 angle = Random16() & 0xffff;
            s32 x = ((Trig_Sin(angle) << 4) >> 16) + targets[member].x / 2 -
                    (gData_08197492[cel] >> 1);
            s32 y = ((Trig_Cos(angle) << 4) >> 16) + targets[member].y -
                    (gData_08197498[cel] >> 1);

            Runtime_ReleaseHeapBlock(188);
            Runtime_ReleaseHeapBlock(104);
            Func_08196404(188, 7, 7, Data_081983c8[Random16() & 3] | CLIP,
                          BLIT_ADDITIVE);
            ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
                canvas, work->sheet + gData_08197486[cel], x, y,
                gData_08197492[cel], gData_08197498[cel]);
            Runtime_ReleaseHeapBlock(188);
            BattleEffect_FetchRectangleBlitters(effect->side, draw);
          }
        }
      }
    }

    // The hits.
    if (mode == 9 || mode == 17) {
    } else if (mode == 5 || mode == 23) {
      for (flashMember = 0; flashMember != effect->count; flashMember++) {
        if (frame >= flashMember * 4 + 2 && (frame & 7) == flashMember) {
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[flashMember], 7, 5, flashMember, 4);
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
        for (glowMember = 0; glowMember != effect->count; glowMember++)
          Func_0814cd48(effect->actors[glowMember], 6, 5, glowMember, 60);
      }
    } else {
      for (member = 0; member != effect->count; member++) {
        if (frame >= member * 4 + 16 && frame < shots - 8 &&
            (frame & 3) == member) {
          work->shakeFrames = 8;
          if (mode == 6)
            Func_0814cd48(effect->actors[member], 14, 5, member, 3);
          else
            Func_0814cd48(effect->actors[member], 7, 5, member, 3);
          if (mode != 11 && mode != 14)
            BattleEffect_StartActorMotion(effect->actors[member], 1, 0x10000,
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

        if (mote->variant >= 0) {
          s32 x;

          EffectPosition_ProjectPoint((s32 *)mote, &screen);
          screen.x >>= 1;
          pair[0](canvas, sheet + gData_08197410[5], screen.x - 3, screen.y - 6,
                  6, 12);
          BattleEffectKernels_IntegrateVector3(mote, 62, 0);
          if (frame > i + 16) {
            x = mote->x;
            mote->velocityX += (object->x - x) >> 7;
            mote->velocityY += (object->y + half - mote->y) >> 8;
            mote->velocityZ += (object->z - mote->z) >> 8;
            if ((object->x < 0 ? -object->x : object->x) < (x < 0 ? -x : x))
              mote->variant = -1;
          }
        }
      }
    }
    Func_08158ce0(4, 4);
    if (mode != 6)
      ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitTicks(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  if (mode == 11 || mode == 14 || mode == 18) {
    // Hand over to the canvas effect: clear both canvases first.
    Iwram_ClearWords((void *)0x06004000, 0x3c00);
    Iwram_ClearWords(canvas, 0x3c00);
    effect->prelude = 0;
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    if (mode == 14)
      SummonEffect_RunCanvasMode1(effect);
    else
      SummonEffect_RunCanvasMode3(effect);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}
