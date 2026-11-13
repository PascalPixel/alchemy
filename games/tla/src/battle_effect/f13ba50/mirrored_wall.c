/// @file
/// The canvas's usual pair of blitters, the member beam and the shell shot.
// Part of f13ba50.c.

#include "../f13ba50_internal.h"

/// Builds the two usual routines that draw onto the 128x128 canvas, clipped,
/// mirrored when flip is nonzero. out[0] keeps the higher colour index (heap
/// slot 26), out[1] adds the colours (slot 47). BattleEffect_LoadBlitter takes
/// each slot's heap offset and the blitter flags.
// The Broken Seal's BattleEffect_FetchRectangleBlitters.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out) {
  // Unused: gives the stack frame its 4 bytes.
  UNUSED u8 unused[4];

  if (flip == 0) {
    u32 heap = 0xc0;
    s32 flags = 19;
    void **slots;

    /* FAKEMATCH(barrier): an empty asm on heap and flags, so both are set
     * before the heap base is shifted and the slot offset built; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 3);
    out[1] = slots[47];
  } else {
    u32 heap = 0xc0;
    s32 flags = 23;
    void **slots;

    /* FAKEMATCH(barrier): the same asm for the mirrored blitters; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 7);
    out[1] = slots[47];
  }
}

void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Revive, Water of Life, Lich (kind 25); Quartz (kind 201).
DEFINE_EFFECT_KIND(BattleEffect_RunRevive, Func_08144b14, 0)

/// Abilities: Dew (kind 209).
DEFINE_EFFECT_KIND(BattleEffect_RunDew, Func_08144b14, 1)

/// Abilities: Spark (kind 255).
DEFINE_EFFECT_KIND(BattleEffect_RunSpark, Func_08144b14, 2)


extern char ResourceId_EarthWallSheet;
extern char ResourceId_CrescentMoonSheet;
extern char ResourceId_VenusDjinnSheet;

extern u8 Data_0819786c[];  // the mote's picture for each step of its sway
extern u16 Data_08197874[]; // the beam's head: three pictures, offsets
extern u8 Data_0819787a[];  // ... their widths
extern u8 Data_0819787d[];  // ... their heights

// The pictures in the work sheet, after the beam (EarthWallSheet).
#define PIC_MOTE 0x2710  // the crescent motes, 24 x 24
#define PIC_DJINN 0x65c0 // the Djinn, 40 x 40
#define MOTES 8          // per target

// Member beam (kinds through three wrappers; the mode picks the Djinn:
// 0 Venus, 1 Mercury, 2 Mars): with a prelude, the Djinn appears, sways and
// rises away; for each target in turn, sixteen frames apart, eight motes
// drift down swaying; from frame 72 a beam of light falls on each target,
// flashing it, and narrows from frame 88.
void Func_08144b14(struct BattleEffectArgument *effect, s32 mode) {
  s32 djinn[3];
  s32 djinnSize[3];
  struct EffectPosition one;
  struct EffectPosition pos;
  s32 ground[3];
  s32 point[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  s32 i;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 102
  // of 652 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the canvas for the Djinn when there is a prelude, then the beam,
  // the motes and the mode's Djinn, each with its palette.
  BattleEffect_StartCanvasLayer(0);
  if (effect->prelude == 1)
    Func_0815585c(effect, (effect->side ^ effect->prelude) << 4 | mode, djinn,
                  djinnSize);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentMoonSheet,
                             work->sheet + PIC_MOTE, 1, 1);
  if (mode == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else if (mode == 1)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);

  // The motes start 80 pixels up, each at its own phase of the sway.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = 0;
    PARTICLES[i].y = 80 << 16;
    PARTICLES[i].z = (Random16() | -32) << 14;
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, BG2 is shifted so that it stands at x = 64.
  REG_BG2PA = 0x100;
  if (effect->count == 1) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &one);
    offsetX = 64 - one.x;
  } else {
    if (effect->side == 1)
      offsetX = -112;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 16 + 116; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 64)
      Sound_PlayFar(212);
    if (frame == 80)
      Func_081180e8(0);

    // The Djinn sways 20 pixels either way and rises from frame 32.
    prelude = effect->prelude;
    if (prelude == 1) {
      s32 angle = frame << 11;
      s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinn[0] + offsetX - 20;
      s32 y = (Trig_Cos(angle) * 4 >> 16) + djinn[1] - 24;

      BattleEffect_FetchRectangleBlitters(effect->side ^ prelude, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];

    // A mote every eight frames drifts down a pixel a frame, swaying 16
    // pixels, until it is 8 pixels from the ground.
    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 start = k * 16;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = target->x;
      point[1] = 0;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > start) {
        for (i = 0; i != MOTES; i++) {
          struct EffectStep *mote = &PARTICLES[k * 64 + i];

          if (frame > i * 8 + start && mote->y > 8 << 16) {
            s32 sway;

            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x += offsetX;
            sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
            if (i & 1)
              pos.x -= sway;
            else
              pos.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            Data_0819786c[mote->variant / 8 & 7] * 576 +
                            PIC_MOTE,
                        pos.x - 12, pos.y - 12, 24, 24);
            mote->y -= 1 << 16;
            mote->variant++;
          }
        }
      }
    }

    // The beam: from the top of the screen to the target's feet, drawn in
    // two mirrored halves, six pixels wide until frame 88, then narrowing a
    // pixel every three frames; its head flickers at the feet.
    for (i = 0; i != effect->count; i++) {
      s32 start = i * 16;

      if (frame >= start + 72) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        Render_ResetTransformState();
        Transform_SetLookAt(camera->eye, camera->pos);
        if (frame == start + 72)
          Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
        if (frame == start + 88)
          Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
        ground[0] = target->x;
        ground[1] = 0;
        ground[2] = target->z;
        EffectPosition_ProjectPoint(ground, &pos);
        pos.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          if (pic > 2)
            pic = (pic & 1) + 1;
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + Data_08197874[pic],
                    pos.x - Data_0819787a[pic], pos.y - Data_0819787d[pic] + 8,
                    Data_0819787a[pic], Data_0819787d[pic]);
            draw[1](canvas, work->sheet + Data_08197874[pic], pos.x,
                    pos.y - Data_0819787d[pic] + 8, Data_0819787a[pic],
                    Data_0819787d[pic]);
          }
          for (k = 0; k != pos.y; k++) {
            draw[0](canvas, work->sheet + 5, pos.x - width, k, width, 1);
            draw[1](canvas, work->sheet + 5, pos.x, k, width, 1);
          }
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void Func_0814512c(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Mad Spatter (kind 347).
DEFINE_EFFECT_KIND(BattleEffect_RunMadSpatter, Func_0814512c, 4)

/// Abilities: Mud (kind 237).
DEFINE_EFFECT_KIND(BattleEffect_RunMud, Func_0814512c, 7)

/// Abilities: Decompose (kind 350).
DEFINE_EFFECT_KIND(BattleEffect_RunDecompose, Func_0814512c, 5)

/// Abilities: Bone Chiller (kind 309).
DEFINE_EFFECT_KIND(BattleEffect_RunBoneChiller, Func_0814512c, 0)

/// Abilities: Sticky Goo (kind 315).
DEFINE_EFFECT_KIND(BattleEffect_RunStickyGoo, Func_0814512c, 1)

/// Abilities: Poison Ink (kind 340).
DEFINE_EFFECT_KIND(BattleEffect_RunPoisonInk, Func_0814512c, 6)

/// Abilities: Sticky Poison (kind 322).
DEFINE_EFFECT_KIND(BattleEffect_RunStickyPoison, Func_0814512c, 2)

/// Abilities: Rotten Blood (kind 330).
DEFINE_EFFECT_KIND(BattleEffect_RunRottenBlood, Func_0814512c, 3)

/// Abilities: Rising Venom (kind 430).
DEFINE_EFFECT_KIND(BattleEffect_RunRisingVenom, Func_0814512c, 8)

/// Abilities: Formic Acid (kind 453).
DEFINE_EFFECT_KIND(BattleEffect_RunFormicAcid, Func_0814512c, 9)

// The shots live in the map buffer, 16 for each target, and the bursts
// after them, 32 for each target.

#define BURSTS ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

void SummonEffect_RunCanvasMode1(void *action);
void SummonEffect_RunCanvasMode3(void *action);

extern char ResourceId_GoldShellSheet;
extern char ResourceId_SmokeSheet; // directory row 0x161
extern char ResourceId_BluePalette;
extern char ResourceId_SpiderWebSheet;
extern char ResourceId_LimePalette; // directory row 0x163
extern char ResourceId_GreenPalette;

// Each mode's record: whether its shots burst into shards (else into smoke),
// the shots' blitter, shots per target, and frames before the targets'
// share.
extern s8 Data_08197880[];
#define RECORD_SHARDS 0
#define RECORD_SHOT_BLITTER 1
#define RECORD_SHOTS 2
#define RECORD_FRAMES 3
extern u8 Data_081978a8[];  // shard widths
extern u8 Data_081978ba[];  // shard heights
extern u16 Data_081978cc[]; // shard cell offsets

#define SMOKE_SHEET 0x3e80 // the smoke, after the gold shells
#define SHARD_SHEET 0x1e00 // the shards, among the gold shells

// Shell shot (the body of ten kinds; the mode picks the colours and the
// record above the rest): shots arc from the caster to each target, one or
// four per target eight frames apart, turning as they fly; each bursts where
// it lands into smoke or shards, flashing the target. Mode 7 prepares a
// canvas for its prelude first; modes 3 and 5 hand the canvas to a summon
// canvas effect at the end.
void Func_0814512c(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  s32 preludeSize[3];
  s32 preludeOrigin[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 i;
  s32 k;
  s32 total;
  struct MotionObject *source;
  s32 *camera;
  u8 *palette;
  s32 height;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the canvas (mode 7 with its prelude), the gold shells and the
  // smoke with their palettes, then the mode's palette.
  if (mode == 7) {
    BattleEffect_StartCanvasLayer(0);
    Func_0815585c(effect, effect->side << 4 | 0x44, preludeOrigin, preludeSize);
    REG_BG2CNT = 0x785;
  } else {
    BattleEffect_StartCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_GoldShellSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SMOKE_SHEET, 1, 1);
  if (mode == 0)
    palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
  else if (mode == 1)
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
  else if (mode == 2)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 8)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 3)
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  else if (mode == 4 || mode == 7)
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  else if (mode == 6)
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  else if (mode == 9)
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // The shots leave the caster at the mode's height, each aimed at its
  // target with a little spread in depth.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  source = GetBattleObjectSlotFar(effect->actor)->object;
  if (mode == 7)
    height = 0x280000;
  else if (mode == 8)
    height = 0x140000;
  else
    height = 0x40000;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *target =
        GetBattleObjectSlotFar(effect->actors[k % effect->count])->object;

    for (i = 0; i != 16; i++) {
      struct EffectStep *shot = &PARTICLES[k * 16 + i];
      s32 aimZ;

      shot->x = source->x;
      shot->y = height;
      shot->z = source->z;
      aimZ = target->z + (((Random16() & 31) - 16) << 17);
      shot->velocityX = (target->x - source->x) >> 4;
      shot->velocityY = 0x40000;
      shot->velocityZ = (aimZ - source->z) >> 4;
      shot->variant = 0;
    }
  }
  for (i = 0; i != 256; i++)
    BURSTS[i].variant = -1;

  // The blitter pair for the caster's side; modes 4 and 7 clear the canvas
  // each frame and turn blending off, the others fade it to three quarters.
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  if (mode == 4 || mode == 7) {
    work->transferMode = 1;
    work->transferValue = 0;
    REG_BLDCNT = 0;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  total = Data_08197880[mode * 4 + RECORD_FRAMES] + effect->count * 8;
  Sound_PlayFar(103);

  for (frame = 0; frame != total; frame++) {
    struct EffectStep *burst;
    struct EffectStep *piece;

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // The shots: each starts four frames after the last, flies under
    // gravity and turns through ten pictures; at the ground it bursts.
    for (k = 0; k != effect->count; k++) {
      for (i = 0; i != Data_08197880[mode * 4 + RECORD_SHOTS]; i++) {
        s32 start = k * 8 + i * 4;

        if (frame >= start) {
          struct EffectStep *shot = &PARTICLES[k * 16 + i];

          if (frame == start + 17) {
            Func_0814cd48(effect->actors[k % effect->count], 7, 5, k, 16);
            Func_081180e8(133);
          }
          if (shot->variant >= 0) {
            s32 step = (frame - start) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ProjectPoint((s32 *)shot, &position);
            position.x >>= 1;
            position.y -= 8;
            if (step > 4)
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 16,
                  position.y - 12, 32, 24);
            else
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 12,
                  position.y - 16, 24, 32);
            if (shot->variant == 0)
              BattleEffectKernels_IntegrateVector3(shot, 63, -0x8000);
            if (shot->y < 0) {
              s32 count;
              s32 n;

              shot->y = 0;
              shot->variant = 1;
              if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
                count = 4;
              else
                count = 16;
              for (n = 0; n != count; n++) {
                piece = &BURSTS[(k * 4 + i) * 8 + n];
                piece->x = shot->x;
                piece->y = shot->y;
                piece->z = shot->z;
                if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
                  piece->velocityX = ((Random16() & 63) - 32) << 11;
                  piece->velocityY = 0;
                  piece->velocityZ = ((Random16() & 63) - 32) << 11;
                } else {
                  piece->velocityX = ((Random16() & 63) - 32) << 13;
                  piece->velocityY = ((Random16() & 31) + 32) << 12;
                  piece->velocityZ = ((Random16() & 63) - 32) << 13;
                }
                piece->variant = 0;
              }
            }
          }
        }
      }
    }

    // The bursts: smoke rises slowly and spreads; shards fly up, fall and
    // tumble, facing the way they move. Each lives 45 frames.
    for (i = 0, burst = BURSTS; i != 256; burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ProjectPoint((s32 *)burst, &position);
        position.x >>= 1;
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
          draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + SMOKE_SHEET,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = effect->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas, work->sheet + Data_081978cc[picture] + SHARD_SHEET,
              position.x - Data_081978a8[picture] / 2,
              position.y - Data_081978ba[picture] / 2, Data_081978a8[picture],
              Data_081978ba[picture]);
        }
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
          BattleEffectKernels_IntegrateVector3(burst, 62, 0x800);
        else
          BattleEffectKernels_IntegrateVector3(burst, 62, -0x8000);
        burst->variant++;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);

  // Modes 3 and 5 clear both canvases and go on into a summon canvas effect.
  if (mode == 3 || mode == 5) {
    /* FAKEMATCH(wrapper): both clears through ClearWords_, which builds the
     * size anew for each; as direct Iwram_ClearWords calls, 16 of 718
     * instructions differ in tla-en. */
    ClearWords_((void *)0x06004000, 0x3c00);
    ClearWords_(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    if (mode == 3)
      SummonEffect_RunCanvasMode1(effect);
    else
      SummonEffect_RunCanvasMode3(effect);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}
