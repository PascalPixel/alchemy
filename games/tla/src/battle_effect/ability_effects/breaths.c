/// @file
/// The breaths, and the crescent slash of Human Hunt and Echo Cut.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104
#define HEAP_SLOT_BLITTER_ALTERNATE 188

// The transfer task runs at this priority, where The Broken Seal's runs at
// 0x480.
#define TASK_PASS_FRAME 0xc80

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// The jaws' texture, unpacked into the map cell buffer's second 16 KB.
#define JAW_TEXTURE (Ram_MapCellBuffer + 0x4000)

// The four breaths, as the wrappers pass them.
enum {
  BREATH_BEAM,          // Beam, kind 73: a beam of fading rings.
  BREATH_ROLLING_FLAME, // Rolling Flame, kind 434: a ball rolls to the target.
  BREATH_PIKE,          // Ghost Soldier and Pike Assault, kind 449.
  BREATH_BLAST,         // Blast Breath, kind 414: a blast and a jet.
};

// A texture for the model kernels: its size as powers of two, then its
// pixels.
/* VIEW(f18bc98): tag struct ModelTexture as struct ModelTexture__f18bc98;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunBreaths moves: 1 of 1241 instructions in
 * tla-ja. */
struct ModelTexture__f18bc98 {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

// One entry of a model list, as Render_AllocateModelList allocates them and
// Render_DrawModelList draws them.
/* VIEW(f18bc98): tag struct Model as struct Model__f18bc98; ability_effects.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:40081: structure has no member
 * named `vertices'. */
struct Model__f18bc98 {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture__f18bc98 *texture;
  s32 alpha;
  s32 tail;
};

extern char ResourceId_Data134;
extern char ResourceId_BlueFlameSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0C1;
extern char ResourceId_Data0C9;
extern char ResourceId_DustCloudSheet;

// Each breath's length, the frame its burst starts, and the glow's widest
// size by variant.
extern u8 Data_08199d97[];
extern u8 Data_08199d9b[];
extern u8 Data_08199d9f[];

// The four jaws of variant 2, by breath: their start frames, their growth,
// and their x offsets from the mouth.
extern u8 Data_08199da3[];
extern u8 Data_08199daf[];
extern s8 Data_08199dbb[];

// The frame each breath hits the targets.
extern u8 Data_08199dc7[];

// The quad and jaw meshes and corners.
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];
extern const u8 Data_08198ec4[];
extern const u8 Data_08198cac[];
extern const u8 Data_08199220[];
extern const u8 Data_081991c0[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gBattleEffect_MoteCells[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
s32 BattleEffect_LoadBlitter(s32 slot, s32 packed);

void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void Func_0815b510(u8 *map, s32 size, s32 scale);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
struct PoolEntry *Render_AllocateModelList(s32 count);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyYaw(s32 angle);
void Transform_ApplyRoll(s32 angle);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
/* VIEW(f18bc98): routine Render_DrawModelList as void
 * Render_DrawModelList(struct Model__f18bc98 *model); f18ad40 has void
 * Render_DrawModelList(struct Model__f18ad40 *model). */
extern void Render_DrawModelList__f18bc98(struct Model__f18bc98 *model) __asm__(
    "Render_DrawModelList");
void BattlePlayback_StartFar(s32 phase);
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 group, s32 timerSlot,
                              s32 frames);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void BattleMotion_ApplyKnockbackFar(s32 actor, s32 motion);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_EndCanvasLayer(void);

// The breaths (Beam, Rolling Flame, Ghost Soldier and Pike Assault, Blast
// Breath): 32 motes gather at the user's mouth, then burst out of it as
// sparks while a flash grows there. The Beam glows and fires a beam of
// fading rings; Rolling Flame rolls a ball of dust to the target; Blast
// Breath glows, blasts and jets. With variant 2, four jaws open one after
// another in front of the user. Each breath hits every target at its frame
// (Blast Breath strikes them in turn, four frames apart).
/// The breaths (Beam, Rolling Flame, Ghost Soldier, Pike Assault and Blast
/// Breath): motes gather at the user's mouth and burst out of it, then each
/// breath fires its own way.
void BattleEffect_RunBreaths(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *graphics;
  struct EffectPosition user;
  struct EffectPosition target;
  DrawRectangle draw[2];
  struct ModelTexture__f18bc98 texture;
  struct EffectStep *step;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, the sparks, the colours (blue for the pikes, orange
  // otherwise) and the jaws' texture, then each breath's own sheet: the Beam
  // computes 16 fading rings of 32 x 32.
  if (mode != BREATH_BEAM)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  if (mode == BREATH_PIKE) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet);
    s32 (*copyWords)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copyWords((void *)BG_PLTT, palette, 128);
  } else {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copyWords)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copyWords((void *)BG_PLTT, palette, 128);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, JAW_TEXTURE, 0, 0);
  if (mode == BREATH_BEAM) {
    for (i = 0; i != 16; i++) {
      u8 *pixel = work->sheet + i * 0x400;

      for (j = 0; j != 32; j++) {
        s32 shade;
        s32 k;

        if (effect->variant == 0)
          shade = ((Trig_Sin(j << 10) << 7) >> 16) - i * 4 - 64;
        else
          shade = ((Trig_Sin(j << 10) * 63) >> 16) - i * 4;
        if (shade > 63)
          shade = 63;
        if (shade < 0)
          shade = 0;
        for (k = 0; k != 32; k++)
          *pixel++ = shade;
      }
    }
  } else if (mode == BREATH_BLAST) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet, 0, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_DustCloudSheet, work->sheet, 1,
                               0);
  }

  // A clipped MAX blitter facing the targets, and a clipped additive one.
  REG_BLDALPHA = 0x1010;
  if (effect->side == 0)
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                BLIT_MAX));
  else
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_MAX));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(
      HEAP_SLOT_BLITTER_ALTERNATE,
      BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_ADDITIVE));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The pikes lunge; Blast Breath comes from 32 pixels in front of the user.
  if (mode == BREATH_PIKE)
    BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 10, 80);
  EffectPosition_ProjectBattler(effect->actor, &user);
  if (mode == BREATH_BLAST) {
    if (effect->side == 0)
      user.x -= 32;
    else
      user.x += 32;
    user.y -= 8;
  }
  EffectPosition_ProjectBattler(effect->actors[0], &target);

  for (frame = 0; frame != Data_08199d97[mode]; frame++) {
    // Frame 0: 32 motes wait around the mouth, each a frame later; the
    // rolling ball sets out from the mouth toward the target.
    if (frame == 0) {
      for (i = 0; i != 32; i++) {
        step = &work->particles[32 + i];
        step->variant = -i;
        step->x = Random16();
        step->y = (Random16() & 31) + 32;
      }
      step = &work->particles[0];
      step->x = (user.x / 2) << 16;
      step->y = (user.y - 32) << 16;
      step->velocityX = 0;
      step->velocityY = 0;
      step->z = (target.x / 2 - user.x / 2) << 10;
      step->velocityZ = (target.y - user.y) << 10;
      step->variant = 0;
    }
    {
      s32 mouthX;
      s32 mouthY;

      mouthY = user.y - 32;
      if (effect->side == 0)
        mouthX = user.x / 2 - 8;
      else
        mouthX = user.x / 2 + 8;

      // The Beam's glow swells and shrinks at the mouth, frames 16-79.
      if (mode == BREATH_BEAM && (frame >= 16 && frame <= 79)) {
        s32 size = frame - 16;

        if (frame > 63)
          size = Data_08199d9f[effect->variant] - frame + 64;
        size = size / 2 * 2;
        if (size > Data_08199d9f[effect->variant])
          size = Data_08199d9f[effect->variant];
        if (size > 0) {
          Func_0815b510(work->sheet + 0x4000, size, 0x10000);
          draw[1](canvas, work->sheet + 0x4000, mouthX - size / 2,
                  mouthY - size, size, size * 2);
        }
      }

      // Blast Breath's glow, frames 0-47, shrinking from frame 40.
      if (mode == BREATH_BLAST && (frame >= 0 && frame <= 47)) {
        s32 size = 40;
        s32 half;

        if (frame > 39)
          size = 104 - frame * 2;
        half = size >> 1;
        size = half * 2;
        Func_0815b510(work->sheet + 0x4000, size, 0x10000);
        draw[1](canvas, work->sheet + 0x4000, mouthX - (size >> 1),
                mouthY - size, size, half * 4);
      }

      if (frame >= Data_08199d9b[mode]) {
        // The burst: the motes fly out of the mouth, away from the targets.
        if (frame == Data_08199d9b[mode]) {
          for (i = 0; i != 32; i++) {
            step = &work->particles[32 + i];
            step->x = mouthX << 16;
            step->y = mouthY << 16;
            step->velocityY = ((Random16() & 63) - 32) << 14;
            if (effect->side == 0)
              step->velocityX = -(Random16() & 63) << 13;
            else
              step->velocityX = (Random16() & 63) << 13;
            step->variant = (Random16() & 15) + 16;
          }
        }
        for (i = 0; i != 32; i++) {
          step = &work->particles[32 + i];
          if (step->variant >= 0) {
            s32 size = (i & 3) + 1;

            if (effect->variant == 1)
              size++;
            else if (effect->variant == 2)
              size = (i & 3) + 3;
            draw[i & 1](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                        HI(step->x) - size / 2, HI(step->y) - size, size,
                        size * 2);
            BattleEffectKernels_IntegrateVector2(step, 60, 0);
            step->variant--;
          }
        }
      } else {
        // Before it: the motes spiral into the mouth.
        for (i = 0; i != 32; i++) {
          step = &work->particles[32 + i];
          if (++step->variant >= 0) {
            s32 x = mouthX + ((Trig_Sin(step->x) * step->y) >> 16);
            s32 y = mouthY + ((Trig_Cos(step->x) * step->y) >> 16);
            s32 size = (i & 3) + 3;

            if (step->y > 1) {
              step->y -= 2;
              draw[i & 1](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                          x - size / 2, y - size, size, size * 2);
            }
          }
        }
      }

      if (mode == BREATH_BEAM) {
        // From frame 56 the beam: four rings in a row, fading from frame 72.
        if (frame > 55) {
          s32 x = user.x / 2;
          s32 y = user.y;
          s32 ring = frame - 72;

          if (ring < 0)
            ring = 0;
          if (ring > 15)
            ring = 15;
          for (i = 0; i != 4; i++) {
            if (effect->side == 0) {
              draw[0](canvas, work->sheet + (ring << 10), x - 40, y - 48, 32,
                      32);
              x -= 32;
            } else {
              draw[0](canvas, work->sheet + (ring << 10), x + 8, y - 48, 32,
                      32);
              x += 32;
            }
          }
        }
      } else if (mode == BREATH_ROLLING_FLAME && frame >= Data_08199d9b[mode]) {
        // The ball of dust rolls to the target.
        step = &work->particles[0];
        draw[0](canvas, work->sheet, HI(step->x) - 16, HI(step->y) - 24, 32,
                48);
        BattleEffectKernels_IntegrateVector2(step, 56, 0);
        step->velocityX += step->z;
        step->velocityY += step->velocityZ;
        step->variant++;
      }

      if (frame >= Data_08199d9b[mode]) {
        void *vertices;
        struct Model__f18bc98 *model;

        vertices = Heap_AllocEwram(1024);
        model = Render_AllocateModelList(1);
        texture.logWidth = 7;
        texture.logHeight = 7;
        model->kind = 6;
        model->texture = &texture;
        model->mesh = gData_08199364;
        model->vertices = vertices;
        texture.pixels = Ram_MapCellBuffer;

        // The flash at the mouth grows and fades over 16 frames; Blast
        // Breath's is two, crossed.
        if (mode != BREATH_ROLLING_FLAME) {
          s32 growth = 10192;
          s32 age = frame - Data_08199d9b[mode];
          s32 scale = age * growth + 0x8000;
          s32 alpha = 128 - age * 8;

          if (effect->variant == 0)
            scale /= 2;
          else if (effect->variant == 1)
            scale = scale * 3 / 2;
          if (alpha > 0)
            alpha = 0;
          if (alpha > -64) {
            s32 count = 2 - (mode != BREATH_BLAST);

            model->alpha = alpha;
            for (i = 0; i != count; i++) {
              Transform_Reset();
              if (mode == BREATH_BLAST) {
                SceneTransform_ApplyPosition3((mouthX - 64) << 16,
                                              (mouthY - 64) << 16, 0);
                SceneTransform_ApplyScale3(0x10000, 0x20000, 0x10000);
                Transform_ApplyRoll((i << 14) + 0x2000);
                Transform_ApplyPitch(-0x1000);
              } else {
                if (effect->side == 0)
                  SceneTransform_ApplyPosition3((mouthX - 72) << 16,
                                                (mouthY - 64) << 16, 0);
                else
                  SceneTransform_ApplyPosition3((mouthX - 56) << 16,
                                                (mouthY - 64) << 16, 0);
                Transform_ApplyYaw(0x32c8);
                Transform_ApplyPitch(0x4000);
              }
              if (mode == BREATH_PIKE)
                scale /= 2;
              SceneTransform_ApplyScale3(scale / 2, scale, scale / 2);
              Render_TransformVertices(gData_08199210, vertices, 4);
              Render_DrawModelList__f18bc98(model);
            }
          }
        }

        // Variant 2: four jaws open one after another.
        if (effect->variant == 2) {
          texture.pixels = JAW_TEXTURE;
          model->kind = 7;
          model->mesh = Data_08198ec4;
          for (i = 0; i != 4; i++) {
            s32 start = Data_08199da3[mode * 4 + i];

            if (frame > start) {
              s32 scale =
                  (frame - start) * Data_08199daf[mode * 4 + i] * 1500 + 0x4180;
              s32 alpha = (start - frame) * 8 + 56;

              if (alpha > 0)
                alpha = 0;
              if (alpha > -64) {
                model->alpha = alpha;
                Transform_Reset();
                if (mode == BREATH_PIKE) {
                  scale /= 2;
                  if (effect->side == 0)
                    SceneTransform_ApplyPosition3(
                        (mouthX + Data_08199dbb[mode * 4 + i] - 48) << 16,
                        (mouthY - 64) << 16, 0);
                  else
                    SceneTransform_ApplyPosition3(
                        (mouthX - Data_08199dbb[mode * 4 + i] - 80) << 16,
                        (mouthY - 64) << 16, 0);
                } else {
                  if (effect->side == 0)
                    SceneTransform_ApplyPosition3(
                        (mouthX + Data_08199dbb[mode * 4 + i] - 64) << 16,
                        (mouthY - 64) << 16, 0);
                  else
                    SceneTransform_ApplyPosition3(
                        (mouthX - Data_08199dbb[mode * 4 + i] - 64) << 16,
                        (mouthY - 64) << 16, 0);
                }
                SceneTransform_ApplyScale3(scale, scale, scale);
                Transform_ApplyYaw(0x36b0);
                Transform_ApplyPitch(0x4000);
                Transform_ApplyYaw(frame << 10);
                Render_TransformVertices(Data_08198cac, vertices, 32);
                Render_DrawModelList__f18bc98(model);
              }
            }
          }
        }

        // Blast Breath's jet, sweeping round from frame 8 and fading from
        // frame 32.
        if (mode == BREATH_BLAST) {
          s32 alpha = 0;
          s32 angle = 0x3060;

          if (frame > 31)
            alpha = (32 - frame) * 4;
          model->alpha = alpha;
          texture.logWidth = 5;
          texture.logHeight = 2;
          model->kind = 7;
          model->mesh = Data_08199220;
          texture.pixels = work->sheet;
          Transform_Reset();
          SceneTransform_ApplyPosition3((mouthX - 64) << 16,
                                        (mouthY - 64) << 16, 0);
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x10000);
          if (frame > 7)
            angle = (frame << 7) + 0x2c60;
          if (effect->side == 1)
            angle = -angle;
          Transform_ApplyRoll(angle);
          SceneTransform_ApplyScale3(0x40000, 0x20000, 0x10000);
          Render_TransformVertices(Data_081991c0, vertices, 4);
          Render_DrawModelList__f18bc98(model);
        }
        Heap_Release(model);
        Heap_Release(vertices);
      }

      if (mode == BREATH_BEAM) {
        if (frame == 0)
          Sound_PlayFar(140);
      } else if (mode == BREATH_PIKE) {
        if (frame == 2)
          Sound_PlayFar(212);
      }

      if (mode == BREATH_BLAST) {
        // Blast Breath strikes each target in turn, every fourth frame.
        if (frame == 0) {
          work->shakeFrames = 8;
          Sound_PlayFar(221);
        }
        if (frame == 16) {
          Sound_PlayFar(134);
          BattlePlayback_StartFar(134);
        }
        for (i = 0; i != effect->count; i++) {
          if (frame >= i * 4 + Data_08199d9b[mode] &&
              frame < Data_08199d97[mode] - 8 && (frame & 3) == i) {
            work->shakeFrames = 8;
            // DEAD: never true inside Blast Breath; the source keeps the test.
            if (mode == 6)
              BattleEffect_TintBattler(effect->actors[i], 14, 5, i, 3);
            else
              BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 3);
            BattleEffect_StartActorMotion(effect->actors[i], 1, 0x10000,
                                          0x40000, 0x20000, frame + 100);
          }
        }
      } else if (frame == Data_08199dc7[mode]) {
        // The others hit every target at once, and the screen flashes.
        for (i = 0; i != effect->count; i++) {
          BattleMotion_ApplyKnockbackFar(effect->actors[i], 4);
          BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 8;
        BattlePlayback_StartFar(144);
        Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      }
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void BattleEffect_RunCrescentSlash(struct BattleEffectArgument *effect,
                                   s32 mode);

/// Abilities: Human Hunt (kind 437).
DEFINE_EFFECT_KIND(BattleEffect_RunHumanHunt, BattleEffect_RunCrescentSlash, 0)

/// Abilities: Echo Cut (kind 390).
DEFINE_EFFECT_KIND(BattleEffect_RunEchoCut, BattleEffect_RunCrescentSlash, 1)

// Does nothing and gives no value: inlined with its value ignored, it leaves
// only a clobber of the return register, an insn with no code.
static inline s32 Nothing(void) {}

extern char ResourceId_GreyCrescentSheet;
extern char ResourceId_StarCrescentSheet;
extern char ResourceId_MarsDjinnSheet;

void BattleEffect_StartTiledCanvas(s32 bgControl);
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);

void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId,
                                    s32 travelDivisor, s32 initialVelocityY);

// The crescent slash of Human Hunt (kind 437, mode 0) and Echo Cut (kind 390,
// mode 1). The caster closes in on the target; from frame 8 a crescent is cut
// in five steps of three frames: the star crescent, a blank frame, the two
// halves of the grey crescent and the star crescent's last cell. Echo Cut
// cuts it upside down and higher, and throws the target into the air at
// frame 12. Human Hunt loads the Mars Djinn's colours into the background
// palette and blends its slash more faintly.
void BattleEffect_RunCrescentSlash(struct BattleEffectArgument *effect,
                                   s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *cells = Ram_MapCellBuffer;
  // The blitter pair, as a two-word array one DImode pseudo: it takes r6 and
  // r7, the second unused, which is why r7 is saved and effect spilled.
  DrawRectangle draw[2];
  s32 flags;
  s32 frame;
  s32 position[3];

  // Set up the canvas and a clipped MAX blitter, mirrored to face the target
  // and flipped upside down for Echo Cut.
  BattleEffect_StartTiledCanvas(0);
  if (mode == 0) {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;
    if (effect->side != 0)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  } else {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_Y;
    if (effect->side != 0)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP;
  }
  /* FAKEMATCH(signature): Nothing() typed s32 though it returns nothing; its
   * return-register clobber issues in the mode store's second cycle, so the
   * flags move is picked before r1 and r2; as a void function, or without the
   * call, 2 of 303 instructions differ in tla-en. */
  Nothing();
  BattleEffect_BuildBlitter(HEAP_SLOT_BLITTER, 7, 7, flags, BLIT_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // Load both crescents and set the blend.
  Resource_LoadAndDecompress((s32)&ResourceId_GreyCrescentSheet, cells, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrescentSheet, work->sheet, 1,
                             0);
  if (mode == 0) {
    Iwram_CopyWordsEntry(
        (void *)0x05000000,
        Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
    REG_BLDALPHA = 0x810;
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    REG_BLDALPHA = 0x1010;
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The caster closes in on the target.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8, 0x40000);

  for (frame = 0; frame != 32; frame++) {
    if (frame == 0)
      EffectPosition_ProjectBattler(effect->actors[0],
                                    (struct EffectPosition *)position);
    if (frame == 8)
      Sound_PlayFar(212);

    // Frame 12: the hit. Echo Cut throws the target up.
    if (frame == 12) {
      if (mode == 1)
        BattlePlayback_StartFar(144);
      else
        BattlePlayback_StartFar(134);
      BattleEffect_StartActorMotion(effect->actors[0], 1, 0x40000, 0x100000,
                                    0x40000, 250);
      work->shakeFrames = 4;
    }

    // From frame 8: the slash, one step every three frames.
    if (frame > 7) {
      s32 step = (frame - 8) / 3;
      s32 y = mode != 0 ? 16 : 0;
      s32 x;

      if (effect->side == 0)
        x = position[0] / 2 - 48;
      else
        x = position[0] / 2 - 80;
      if (mode == 0) {
        switch (step) {
        case 0:
          draw[0](canvas, work->sheet, x, y, 120, 120);
          break;
        case 1:
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          break;
        case 2:
          draw[0](canvas, cells, x - 4, y, 128, 128);
          break;
        case 3:
          draw[0](canvas, cells + 0x4000, x - 4, y, 128, 128);
          break;
        case 4:
          draw[0](canvas, work->sheet + 0x3840, x, y, 120, 120);
          break;
        }
      } else {
        switch (step) {
        case 1:
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          break;
        case 0:
          draw[0](canvas, work->sheet, x, y, 120, 120);
          break;
        case 2:
          draw[0](canvas, cells, x - 4, y - 8, 128, 128);
          break;
        case 3:
          draw[0](canvas, cells + 0x4000, x - 4, y - 8, 128, 128);
          break;
        case 4:
          draw[0](canvas, work->sheet + 0x3840, x, y, 120, 120);
          break;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

typedef s32 (*CanvasBlitFunc)(u8 *canvas, void *source, s32 x, s32 y, s32 width,
                              s32 height);

void Canvas_BlitAtFoot(void *source, s32 x, s32 y, s32 size) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 half;

  BattleEffect_LoadBlitter(188, 19);
  half = size / 2;
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x - half, y - size,
                                            half, size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 23);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x, y - size, half,
                                            size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 27);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x - half, y, half,
                                            size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 31);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x, y, half, size);
  HeapSlot_Release(188);
}
#undef HEAP_SLOT_BLITTER
#undef HEAP_SLOT_BLITTER_ALTERNATE
#undef BLITTER
#undef HI
#undef JAW_TEXTURE
