/// @file
/// The body of Heavy Divide and Strong Hit, then Cool, Wyrd Curse, Radiant Fire
/// and Hurricane.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

struct FxPartyList;
struct Model;
struct StripQuad;

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyUniformScale(s32 scale);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyYaw(s32 angle);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Transform_ApplyRoll(s32 angle);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattlePlayback_StartFar(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_GreatSwordSheet;
extern char ResourceId_AxeSheet;
extern u8 gMapCellBuffer[];
extern const u8 Data_0819928c[];  // the sword's mesh
extern const u8 Data_081991c0[];  // its corners
extern const u8 gData_08199340[]; // the axe's mesh
extern const u8 Data_081991e0[];  // its corners
extern u8 Data_08198b96[];        // per mode: the strike's frame
extern u8 Data_08198b98[];        // per mode: the chips
extern const u8 Data_08198b9a[];  // a chip's two pixels

// The body of Heavy Divide (mode 0) and Strong Hit (mode 1). Heavy Divide:
// a great sword (with four fading after-images, each a dimmer copy of the
// sheet) swings down from over the caster, striking at frame
// Data_08198b96[0]. Strong Hit: an axe is thrown, spinning, from the caster
// to the target. Either way chips of stone fly from the target and bounce
// on the ground.
void BattleEffect_RunHeavyBlow(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  DrawRectangle blit[2];
  s32 frame;
  void *caster;
  s32 frames;
  struct EffectPosition hit;
  struct EffectPosition source;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  s32 i;
  s32 k;
  s32 zero; // One constant, rebuilt at each store.

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x410;
  BattleEffect_LoadBlitter(
      SLOT_BLITTER,
      BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 1;
  work->transferValue = 0;
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  caster = BattleObject_GetFar(effect->actor)->object;
  if (mode == 0) {
    frames = 80;
    Resource_LoadAndDecompress((s32)&ResourceId_GreatSwordSheet, work->sheet, 1,
                               1);

    // Four after-images, each in its own dimmer colour.
    for (k = 1; k != 5; k++) {
      ((u16 *)BG_PLTT)[59 + k] = (4 - k) * 0x1000 + 0x3c00;
      for (i = 0; i != 0x1000; i++) {
        if (work->sheet[i] == 0) {
          work->sheet[k * 0x1000 + i] = 0;
        } else {
          work->sheet[k * 0x1000 + i] = k + 59;
        }
      }
    }
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_AxeSheet, work->sheet, 1, 1);
    frames = 64;
  }
  Sound_PlayFar(212);
  zero = 0;

  for (frame = 0; frame != frames; frame++) {
    if (frame == 0) {
      s32 actor = effect->actors[0];
      struct EffectPosition *at = &hit;
      struct EffectStep *chip;

      // The store in a once-only block, so the buffer's address loads before
      // at is passed; as the plain store, 2 of 671 instructions differ in
      // tla-en.
      do {
        chip = (struct EffectStep *)gMapCellBuffer;
      } while (0);
      EffectPosition_ProjectBattler(actor, at);
      for (i = 0; i != 256; i++, chip++) {
        s32 angle = Random16();
        s32 speed = 255;

        speed &= Random16();
        chip->x = (hit.x / 2) << 16;
        chip->y = (hit.y - 16) << 16;
        chip->velocityX = (Trig_Sin(angle) * (speed + 128)) >> 6;
        chip->velocityY = (Trig_Cos(angle) * (speed + 128)) >> 6;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (mode == 0) {
      if (frame == 8) {
        BattleEffect_TintBattler(effect->actor, -1, 2, 0, -1);
      }
    } else if (frame == 0) {
      BattleEffect_TintBattler(effect->actor, -1, 2, 0, -1);
      Actor_SetAnimationSpeedFar(caster, 32);
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    EffectPosition_ProjectBattler(effect->actor, &source);
    EffectPosition_ProjectBattler(effect->actors[0], &target);
    model->kind = 9;
    model->texture = &texture;
    model->vertices = vertices;
    model->shade = zero;
    if (mode == 0) {
      s32 size;
      s32 angle;

      texture.logWidth = 5;
      texture.logHeight = 7;
      model->mesh = Data_0819928c;
      if (frame > 59) {
        size = 68 - frame;
      } else {
        size = frame + 2;
      }
      if (size > 8) {
        size = 8;
      }
      if (size > 0) {
        if (frame > 38) {
          angle = 0x4000 - Trig_Sin((frame << 9) - 0x4e00) / 16;
        } else if (frame > 15) {
          angle = (0x10000 - Trig_Cos((frame << 9) - 0x2000)) / 2;
        } else {
          angle = 0;
        }

        // The after-images keep the sword's last four angles.
        for (i = 4; i != 0; i--) {
          work->particles[i].variant = work->particles[i - 1].variant;
        }
        work->particles[0].variant = angle;
        for (i = 4; i != -1; i--) {
          struct EffectStep *step = &work->particles[i];

          if (i != 0 && frame > i + 38) {
            continue;
          }
          if (frame < i * 2) {
            continue;
          }
          texture.pixels = work->sheet + i * 0x1000;
          Transform_Reset();
          SceneTransform_ApplyPosition3((source.x / 2 - 64) << 16,
                                        (source.y - 88) << 16, 0);
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          if (effect->side == 0) {
            Transform_ApplyRoll(0x8000 - step->variant);
          } else {
            Transform_ApplyRoll(step->variant + 0x8000);
          }
          SceneTransform_ApplyPosition3(0, 0xfff00000, 0);
          SceneTransform_ApplyScale3(size << 15, size * 13192, size << 13);
          if (effect->side == 0) {
            Transform_ApplyYaw(0x8000);
          }
          Render_TransformVertices(Data_081991c0, vertices, 4);
          Render_DrawModelList(model);
        }
      }
    } else {
      model->mesh = gData_08199340;
      texture.logWidth = 6;
      texture.logHeight = 6;
      if (frame == 0) {
        for (i = 0; i != 5; i++) {
          work->particles[i].x = (source.x / 2) << 16;
          work->particles[i].y = (source.y - 32) << 16;
          work->particles[i].velocityX = (target.x / 2 - source.x / 2) << 11;
          work->particles[i].velocityY = (target.y - source.y) << 11;
          work->particles[i].z = zero;
          if (effect->side == 0) {
            work->particles[i].velocityZ = -0x1000;
          } else {
            work->particles[i].velocityZ = 0x1000;
          }
        }
      }
      if (frame > 7) {
        s32 scale = 80000;

        texture.pixels = work->sheet;
        Transform_Reset();
        SceneTransform_ApplyPosition3(work->particles[0].x - 0x400000,
                                      work->particles[0].y - 0x400000, 0);
        SceneTransform_ApplyScale3(scale >> 1, scale, 0x8000);
        Transform_ApplyRoll(work->particles[0].z);
        if (effect->side == 0) {
          Transform_ApplyYaw(0x8000);
        }
        Render_TransformVertices(Data_081991e0, vertices, 4);
        Render_DrawModelList(model);
        BattleEffectKernels_IntegrateVector3(work->particles, 64, 0);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);

    if (frame == Data_08198b96[mode]) {
      BattlePlayback_StartFar(144);
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
      work->shakeFrames = 8;
    }
    if (frame >= Data_08198b96[mode]) {
      i = 0;
      if (i != Data_08198b98[mode]) {
        DrawRectangle blitter = blit[0];
        struct EffectStep *chip = (struct EffectStep *)gMapCellBuffer;

        do {
          if (chip->variant >= 0) {
            blitter(canvas, Data_08198b9a, HI(chip->x), HI(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, 0x1000);
            blitter(canvas, Data_08198b9a, HI(chip->x), HI(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, 0x1000);
            blitter(canvas, Data_08198b9a, HI(chip->x), HI(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, 0x1000);
            if (HI(chip->y) > 104) {
              chip->velocityY = -chip->velocityY;
              chip->variant -= 8;
            }
            chip->variant--;
          }
          i++;
          chip++;
        } while (i != Data_08198b98[mode]);
      }
    }
    if (mode == 0) {
      BattleCamera_Shake(4, 16);
    } else {
      BattleCamera_Shake(8, 8);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Actor_SetAnimationSpeedFar(caster, 16);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// The column pictures: 36 pixels wide, and a slim 17-pixel one after them.
#define SLIM_COLUMN_OFFSET 0x870
#define SPARK_OFFSET 0x3060

// How a column leans as it grows.
enum { LEAN_LEFT, LEAN_RIGHT, SLIM_LEAN_LEFT, SLIM_LEAN_RIGHT };

extern char ResourceId_Data0CE;
extern char ResourceId_BlastSheet;
extern char ResourceId_BlueFlameSheet;

// The columns, by number: x, start frame, full height and lean.
extern s16 Data_08198b9c[];
extern u8 Data_08198bb0[];
extern u8 Data_08198bba[];
extern u8 Data_08198bc4[];
// The Broken Seal's gEruption_PuffCells and gCybele_EmberSizes:
// the spark pictures by stage.
extern u16 Data_08198bce[];
extern u16 Data_08198bdc[];

s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags,
                              u32 mode); // BattleEffect_BuildBlitter

// Cool: ice columns burst up around the targets one after another, leaning
// as they grow, and stand until they sink back; sparks of frost fly up
// from the ground and drift upward.
//
// Setup: canvas layer 1 at half blend and full width, centred between the
// first and last target; the column sheet, the spark sheet after it, and
// the blue palette.
// Frame loop (54 frames and 4 more per level): the sparks start at frame
// 0; column i rises from its start frame 10 pixels a frame, strikes a
// target two frames in, and sinks 10 pixels a frame from frame 32 + 4 per
// level. The columns are drawn with max blitters until frame 24 on the
// higher levels, with additive ones otherwise.
/// Cool: leaning ice columns and sparks of frost.
void BattleEffect_RunCool(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 sinkFrame;
  DrawRectangle draw[2];
  struct EffectPosition ends[2];
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CE, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + SPARK_OFFSET, 1, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                       128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  {
    s32 stagger;

    stagger = effect->variant * 4;
    sinkFrame = stagger + 32;

    // Centre the canvas between the first and the last target.
    EffectPosition_ProjectBattler(effect->actors[0], &ends[0]);
    EffectPosition_ProjectBattler(effect->actors[effect->count - 1], &ends[1]);
    ends[0].x += (ends[1].x - ends[0].x) / 2;
    REG_BG2X = (64 - ends[0].x) << 8;

    frame = 0;
    if (stagger != -54) {
    // No loop notes: the heap slot reads below stay in the frame loop.
    top:
      // Frame 0: 16 sparks leave the ground in random directions.
      if (frame == 0) {
        struct EffectStep *spark;

        for (i = 0, spark = work->particles; i != 16; i++, spark++) {
          s32 speed = (Random16() & 0x1ff) + 128;
          s32 angle = Random16() & 0xffff;

          spark->x = ((Random16() & 63) + 32) << 16;
          spark->y = 96 << 16;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 8;
          spark->variant = 8;
        }
      }
      if (frame == 32)
        BattlePlayback_StartFar(134);

      if (frame >= 0) {
        u32 mode = BLIT_MAX;

        if (frame <= 23 && effect->variant > 0)
          mode = BLIT_ADDITIVE;
        BattleEffect_BuildBlitter(104, 7, 7,
                                  COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
        draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
        BattleEffect_BuildBlitter(
            188, 7, 7, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
            mode);
        // At its address: through the slots' base it would share the base
        // above.
        draw[1] = *(DrawRectangle *)&Ram_HeapSlots->unkb8[1];

        for (i = 0; i != effect->variant * 3 + 4; i++) {
          s32 age = frame - Data_08198bb0[i];
          s32 x = Data_08198b9c[i];
          s32 scale;
          s32 height;

          // Two frames after it starts, a column strikes the next target.
          if (age == 2) {
            s32 member = i % effect->count;

            BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 3);
            BattleMotion_ApplyKnockbackFar(effect->actors[member], 0);
            Sound_PlayFar(212);
            work->shakeFrames = 3;
          }

          // The slim columns stand a little over half as high.
          scale = 60;
          if (Data_08198bc4[i] > LEAN_RIGHT)
            scale = 35;
          scale = Data_08198bba[i] * scale / 64;
          if (frame < sinkFrame)
            height = age * 10;
          else
            height = scale - (frame - sinkFrame) * 10;
          if (height > scale)
            height = scale;

          switch (Data_08198bc4[i]) {
          case LEAN_LEFT:
            x -= height / 2;
            break;
          case LEAN_RIGHT:
            x += height / 2;
            break;
          case SLIM_LEAN_LEFT:
            x -= height / 4;
            break;
          case SLIM_LEAN_RIGHT:
            x += height / 4;
            break;
          }
          if (Data_08198bc4[i] <= LEAN_RIGHT)
            draw[Data_08198bc4[i] & 1](canvas, work->sheet, x - 18,
                                       112 - height, 36, height);
          else
            draw[Data_08198bc4[i] & 1](canvas, work->sheet + SLIM_COLUMN_OFFSET,
                                       x - 9, 108 - height, 17, height);
        }
        HeapSlot_Release(104);
        HeapSlot_Release(188);
      }

      // Spark i rises from frame 8 + 2i through seven pictures, three frames
      // apiece; past the first 16 they wait until the columns sink.
      BattleEffect_FetchRectangleBlitters(effect->side, draw);
      {
        for (i = 0; i != effect->variant * 4 + 8; i++) {
          struct EffectStep *spark = &work->particles[i];

          if ((i <= 15 || frame >= sinkFrame) && frame >= i * 2 + 8 &&
              spark->variant <= 28) {
            s32 cell = spark->variant / 3;
            s32 x = HI(spark->x);
            s32 y = HI(spark->y);
            u32 size;
            u8 *src;

            if (cell > 6)
              cell = 6;
            src = work->sheet + Data_08198bce[cell] + SPARK_OFFSET;
            size = Data_08198bdc[cell];
            draw[0](canvas, src, x - size / 2, y - size / 2, size, size);
            spark->variant++;
            BattleEffectKernels_IntegrateVector2(spark, 62, -0x2000);
          }
        }
      }
      HeapSlot_Release(188);
      HeapSlot_Release(104);

      BattleCamera_Shake(4, 4);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      if (++frame != stagger + 54)
        goto top;
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef BLITTER

// Wyrd Curse: drops fall on the target and bounce away, feeding a glow that
// grows over it, then splashes rise and the target is struck six times.

extern char ResourceId_Data134;
extern char ResourceId_Data0D8;

extern u8 gData_02014000[];
extern u16 gBattleEffect_MoteCells[];
extern const s8 Data_08198bea[];  // the drops' bounce speeds
extern const u8 Data_08199244[];  // the glow's and the splashes' mesh
extern const u8 gData_08199210[]; // its four corners

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])

#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define WYRD_FRAMES 120
#define WYRD_DROPS 16

/// Abilities: Wyrd Curse.
///
/// A 32x32 glow is mirrored from a shaded disc, and BG palette 0 becomes a
/// dark ramp. Sixteen drops form over the target; from frame 0 one falls
/// every eight frames, and on reaching the ground it bounces away and adds
/// its weight to the glow (the last particle), which grows over the target
/// and fades from frame 64. From frame 64 six splashes, four frames apart,
/// rise from the drops, and from frame 80 the target is struck six times.
void BattleEffect_RunWyrdCurse(struct BattleEffectArgument *effect) {
  struct ModelTexture texture;
  struct EffectPosition screen;
  void *canvas;
  // Unused: only draw[0] is read; draw[1] gives the stack frame its size.
  DrawRectangle draw[2];
  s32 frame;
  s32 zero;
  u8 *sprites;
  void *caster;
  struct BattleEffectWork *work;
  s32 i;
  struct EffectStep *drop;
  struct EffectStep *last;
  u16 *palette;
  s32 size;
  s32 width;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
  } else {
    BattleEffect_LoadBlitter(104,
                             BLIT_MODE(BLIT_MAX, BLIT_CLIP | COMPOSITE_FLIP_X));
  }
  draw[0] = BLITTER;
  work->transferMode = 2;
  work->transferValue = 50;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  width = 32;
  palette = (u16 *)BG_PLTT + 1;
  caster = BattleObject_GetFar(effect->actor)->object;
  Iwram_ClearWordsEntry((void *)BG_PLTT, 128);
  for (i = 0; i != 63; i++) {
    *palette++ = i / 3 + 8;
  }

  // The glow: a 16-pixel disc mirrored into the four quarters of 32x32.
  {
    s32 y;
    s32 x;
    s32 half;
    s32 offset;
    // The clear through a local: the native loads its entry before the sheet.
    s32 (*clear)(void *destination, s32 size) = Iwram_ClearWordsEntry;

    size = width / 2;
    half = size / 2;
    clear(work->sheet + 0x1000, 0x800);
    BattleEffect_DrawShadedCircles(work->sheet, size, 0x10000);
    y = 0;
    offset = 0;
  nextRow:
    for (x = 0; x != half; x++) {
      u8 v = work->sheet[offset / 2 + x];

      if (v != 0) {
        s32 mirrorRow;
        s32 row;

        row = y * 32;
        work->sheet[0x1000 + row + x * 2] = v;
        // x + x: the same byte as x * 2 + 1, spelled so that CSE cannot share
        // row + x * 2 between the two stores, which gives row the register
        // order the listing has.
        work->sheet[0x1000 + row + x + x + 1] = v;
        work->sheet[0x1000 + (offset + (size - x - 1)) * 2] = v;
        work->sheet[0x1000 + (offset + (size - x - 1)) * 2 + 1] = v;
        mirrorRow = (32 - y) * 32;
        mirrorRow -= 32;
        work->sheet[0x1000 + mirrorRow + x * 2] = v;
        work->sheet[0x1000 + mirrorRow + x * 2 + 1] = v;
        work->sheet[0x1000 + mirrorRow + (size - x - 1) * 2] = v;
        work->sheet[0x1000 + mirrorRow + (size - x - 1) * 2 + 1] = v;
      }
    }
    y++;
    offset += size;
    if (y != size) {
      goto nextRow;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0D8, work->sheet + 0x4000, 0,
                             0);

  // A local holding 0 for the whole effect clears the glow's variant and both
  // models' shade (with literal zeros, 151 of 568 differ in tla-en).
  zero = 0;
  for (frame = 0; frame != WYRD_FRAMES; frame++) {
    struct RenderModel *model;
    void *vertices;

    s32 level;
    s32 splash;

    if (frame == 0) {
      s32 still;

      EffectPosition_ProjectBattler(effect->actors[0], &screen);
      // A local holding 0 for the drops' rest values: the listing builds the 0
      // between i and the drop pointer (with literal zeros, 2 of 568 differ).
      still = 0;
      for (i = 0, drop = work->particles; i != WYRD_DROPS; i++, drop++) {
        drop->x = (screen.x / 2 + (Random16() & 15) - 8) << 16;
        drop->y = -8;
        drop->velocityX = still;
        drop->velocityY = still;
        drop->z = (Random16() & 3) + 4;
        drop->velocityZ = 160 - i * 16;
        drop->variant = still;
      }
      {
        struct EffectStep *glow = &work->particles[63];

        glow->x = (screen.x / 2) << 16;
        glow->y = 0x700000;
        glow->variant = zero;
      }
    }

    // The falling drops, one more every eight frames.
    if (frame <= 63) {
      for (i = 0, drop = work->particles; i != 6; i++, drop++) {
        if (frame > i * 8 && drop->variant == 0) {
          s32 dropSize = drop->z;

          draw[0](canvas, sprites + gBattleEffect_MoteCells[dropSize - 1],
                  HI(drop->x) - dropSize / 2, HI(drop->y) - dropSize, dropSize,
                  dropSize * 2);
          BattleEffectKernels_IntegrateVector2(drop, 64, 0x8000);
          if (HI(drop->y) > 111) {
            drop->variant = 1;
            // DEAD: overwritten by the next store; the random number is still
            // drawn.
            drop->velocityX = ((Random16() & 31) - 16) << 12;
            drop->velocityX = Data_08198bea[i % 4] << 13;
            drop->velocityY = 0;
          }
        }
      }
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    texture.logHeight = 5;
    texture.pixels = work->sheet + 0x1000;
    model->kind = 6;
    model->mesh = Data_08199244;
    model->texture = &texture;
    model->vertices = vertices;
    model->shade = zero;

    // The glow gathers what the landed drops bring.
    {
      struct EffectStep *glow;
      struct EffectStep *landed;

      i = 0;
      glow = &work->particles[63];
      landed = work->particles;
      for (; i != 8; i++, landed++) {
        if (landed->variant > 0) {
          glow->variant += landed->velocityZ;
          landed->velocityZ /= 2;
        }
      }
    }
    last = &work->particles[63];
    level = last->variant;
    if (level > 0) {
      s32 scale = (level << 13) / 824;

      if (frame > 63) {
        last->variant = level - 24;
      }
      Transform_Reset();
      SceneTransform_ApplyPosition3(last->x - 0x400000, last->y - 0x400000, 0);
      SceneTransform_ApplyScale3(scale * 2, scale, scale * 2);
      Transform_ApplyPitch(-0x2000);
      Render_TransformVertices(gData_08199210, vertices, 4);
      Render_DrawModelList(model);
    }

    // The splashes, from frame 64.
    texture.logWidth = 5;
    texture.logHeight = 5;
    model->kind = 7;
    model->mesh = Data_08199244;
    splash = 25000;
    /* FAKEMATCH(barrier): an empty asm through splash hides its constant, so it
     * stays in a register through the loop; without it, 137 of 568
     * instructions differ in tla-en. */
    asm("" : "+r"(splash));
    // The loop is written with goto: as for (i = 0; i != 6; i++, drop++),
    // loop.c hoists the halving of splash out of it, and 150 of 568
    // instructions differ in tla-en.
    i = 0;
    drop = work->particles;
  nextSplash:
    if (frame > i * 4 + 64) {
      model->shade = zero;
      texture.pixels = work->sheet + 0x4000 + ((i % 2) << 10);
      Transform_Reset();
      SceneTransform_ApplyPosition3(drop->x - 0x400000, drop->y - 0x300000, 0);
      SceneTransform_ApplyScale3(splash >> 1, splash, splash >> 1);
      Transform_ApplyPitch(-0x4000);
      Render_TransformVertices(gData_08199210, vertices, 4);
      Render_DrawModelList(model);
      BattleEffectKernels_IntegrateVector2(drop, 60, -0x8000);
    }
    i++;
    drop++;
    if (i != 6) {
      goto nextSplash;
    }
    Heap_Release(model);
    Heap_Release(vertices);

    if (frame == 64) {
      Sound_PlayFar(147);
    }
    for (i = 0; i != 6; i++) {
      if (frame == i * 4 + 80) {
        work->shakeFrames = 2;
        BattleMotion_ApplyKnockbackFar(effect->actors[0], 0);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 2);
        if (i == 0) {
          BattlePlayback_StartFar(134);
        } else {
          Sound_PlayFar(133);
        }
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Actor_SetAnimationSpeedFar(caster, 16);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef BLITTER

struct Model;

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define FLAMES 32

extern char ResourceId_Data0C2;
extern char ResourceId_OrangePaletteB;

extern const u8 gData_08199364[]; // a textured square

void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);

// Radiant Fire: eight bursts of fire bloom one after another around the
// target, each a shaded disc that swells and shrinks under a spinning
// square of flame, with a white flash of the canvas as the first lands.
//
// Setup: canvas layer 0 at half blend; one max blitter; the flame texture
// (resource 0x0c2) and the orange palette; 32 shaded discs of 2 to 64
// pixels drawn into the map cell buffer, their offsets kept in the work
// sheet.
// Frame loop (66 frames): at frame 0 the bursts are placed at random
// around the target and the user closes in; from frame 8 burst i lands at
// frame 16 + 4i and burns for 24 frames.
void BattleEffect_RunRadiantFire(struct BattleEffectArgument *effect) {
  struct EffectPosition screen;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  s32 savedYaw;
  struct MotionObject *target;
  s16 *discs;
  struct EffectStep *flame;
  struct EffectStep *burst;
  struct EffectStep *square;
  struct RenderModel *model;
  void *vertices;
  s32 offset;
  s32 size;
  s32 cells;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      104, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet + 0x1000, 0,
                             0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       128);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  camera = Ram_HeapSlots->cameraWork;
  offset = 0;
  savedYaw = (s16)camera->yaw;
  size = 2;
  target = BattleObject_GetFar(effect->actors[0])->object;

  // The discs, smallest first, one after another in the map cell buffer.
  cells = (s32)Ram_MapCellBuffer;
  discs = (s16 *)work->sheet;
  for (i = 0; i != FLAMES; i++) {
    BattleEffect_DrawShadedCircles((void *)(offset + cells), size, 0x14e20);
    discs[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }

  for (frame = 0; frame != 66; frame++) {
    // Frame 0: each burst takes a random place on a ring about the target
    // that widens with its number, 48 pixels up, and is projected to the
    // screen; it waits 4 frames more than the one before it.
    if (frame == 0) {
      camera->yaw = savedYaw;
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      flame = work->particles;
      for (i = 0; i != FLAMES; i++, flame++) {
        s32 angle;
        s32 radius;

        Random16();
        angle = Random16() & 0xffff;
        radius = (Random16() & 7) + i * 4;
        flame->x = target->x + Trig_Sin(angle) * radius * 2;
        flame->y = target->y + Trig_Cos(angle) * radius + (48 << 16);
        flame->z = target->z;
        EffectPosition_ProjectPoint(&flame->x, &screen);
        flame->x = screen.x / 2;
        flame->y = screen.y;
        Random16();
        flame->velocityX = 32 - i;
        flame->velocityY = Random16();
        flame->velocityZ = Random16();
        flame->variant = -i * 4;
      }
    }
    if (frame == 0)
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 8, 80);

    // Frame 8: the canvas flashes white and the target is struck.
    if (frame == 8) {
      Iwram_FillWordsEntry(canvas, 0x3c00, 0x3f3f3f3f);
      work->shakeFrames = 16;
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 0);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
      BattlePlayback_StartFar(145);
    }

    if (frame >= 8) {
      // Each burst strikes the target as it lands, then its disc swells
      // and shrinks with a sine over 32 frames.
      i = 0;
      burst = work->particles;
      for (; i != 8; i++, burst++) {
        burst->variant++;
        if (burst->variant == 8) {
          Sound_PlayFar(144);
          work->shakeFrames = 8;
          BattleMotion_ApplyKnockbackFar(effect->actors[0], 0);
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
        }
        if (burst->variant >= 0 && burst->variant < 32) {
          s32 diameter;
          s32 index;

          diameter =
              ((Trig_Sin(burst->variant << 10) * burst->velocityX) >> 16) * 2;
          index = diameter - 2;
          if ((u32)index <= 61)
            Canvas_BlitAtFoot((void *)(*(s16 *)(index + (s32)discs) + cells),
                              burst->x, burst->y, diameter);
        }
      }

      // The square of flame turns over each burst for 24 frames, fading
      // over the last eight.
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet + 0x1000;
      model->kind = 6;
      model->vertices = vertices;
      model->mesh = gData_08199364;
      model->texture = &texture;
      i = 0;
      square = work->particles;
      for (; i != 8; i++, square++) {
        if (square->variant >= 0 && square->variant < 24) {
          s32 brightness = 0;
          s32 roll = square->variant * 3 << 11;

          if (square->variant > 15)
            brightness = (16 - square->variant) * 8;
          model->shade = brightness;
          Transform_Reset();
          SceneTransform_ApplyPosition3((square->x - 64) << 16,
                                        (square->y - 64) << 16, 0);
          Transform_ApplyRoll(square->velocityZ);
          Transform_ApplyPitch(square->velocityY);
          SceneTransform_ApplyUniformScale(roll);
          Render_TransformVertices(Data_081991e0, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

// The map cell buffer holds 1,024 more particles while an effect runs.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define STREAKS 64

// The splash: six 24x48 pictures in the work sheet after the streak.
#define SPLASH_OFFSET 0x3200
#define SPLASH_SIZE (24 * 48)

extern char ResourceId_CyanPalette;
extern char ResourceId_SmokeSheet;

void SummonEffect_RunCanvasMode3(void *action);

// Hurricane: driving rain slants down over the target, each drop bursting
// into a splash where it lands, while the target is struck eight times; the
// user rushes in at frame 32. The canvas then hands over to the summon
// canvas effect in mode 3.
//
// Setup: canvas layer 1 at half blend and full width, centred on the
// target; one additive blitter; the cyan palette; a 4x64 streak drawn into
// the work sheet as a slanted line, leaning with the wind; the splash
// pictures after it.
// Frame loop (68 frames): 64 streaks start above the screen at frame 0 and
// fall in turn, one more each frame; a streak that falls below line 48
// starts its splash.
void BattleEffect_RunHurricane(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit[2];
  struct EffectPosition position;
  struct EffectStep *streak;
  struct EffectStep *splash;
  s32 size;
  s32 frame;
  s32 i;

  size = 0x3c00;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      104, BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_CyanPalette),
                       128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EffectPosition_ProjectBattler(effect->actors[0], &position);
  {
    /* FAKEMATCH(temporary): x computed before the register's address is built;
     * as REG_BG2X = (64 - position.x) << 8, 10 of 376 instructions differ in
     * tla-en. */
    s32 x = 64 - position.x;

    REG_BG2X = x << 8;
  }

  // The streak: one pixel of colour 24 a row, stepping a pixel sideways
  // every 16 rows against the wind.
  Iwram_ClearWordsEntry(work->sheet, 256);
  for (i = 0; i != STREAKS; i++) {
    if (effect->side == 0)
      work->sheet[i * 4 - i / 16 + 3] = 24;
    else
      work->sheet[i * 4 + i / 16] = 24;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SPLASH_OFFSET, 1, 0);
  REG_BG2PA = 0x100;

  for (frame = 0; frame != 68; frame++) {
    // Frame 0: the streaks wait above the screen, drifting with the wind.
    if (frame == 0) {
      Sound_PlayFar(170);
      for (i = 0; i != 256; i++)
        PARTICLES[i].variant = -1;
      for (i = 0; i != STREAKS; i++) {
        streak = &work->particles[i];
        streak->y = ((Random16() & 31) - 64) << 16;
        if (effect->side == 0) {
          streak->x = ((Random16() & 63) + 40) << 16;
          streak->velocityX = -(1 << 16);
        } else {
          streak->x = ((Random16() & 63) + 24) << 16;
          streak->velocityX = 1 << 16;
        }
        streak->velocityY = 16 << 16;
        streak->variant = -1;
      }
    }
    if (frame == 32)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8,
                                     3 << 16);
    if (frame == 46) {
      BattlePlayback_StartFar(134);
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
    }

    // Each splash rises and spreads through its six pictures, four frames
    // apiece.
    splash = PARTICLES;
    for (i = 0; i != 256; i++, splash++) {
      if (splash->variant >= 0 && splash->variant < 24) {
        blit[0](canvas,
                work->sheet + SPLASH_OFFSET + splash->variant / 4 * SPLASH_SIZE,
                HI(splash->x) - 12, HI(splash->y) - 32, 24, 48);
        BattleEffectKernels_IntegrateVector2(splash, 56, -0x4000);
        splash->variant++;
      }
    }

    // The target is struck every four frames from frame 6.
    for (i = 0; i != 8; i++) {
      if (frame == i * 4 + 6) {
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 3);
        Sound_PlayFar(132);
      }
    }

    // Streak i falls from frame i, shortened as it reaches the ground at
    // line 112; below line 48 it starts splash i.
    for (i = 0; i != 32; i++) {
      s32 y;
      s32 height;
      s32 x;

      streak = &work->particles[i];
      if (frame < i)
        continue;
      y = HI(streak->y);
      if (y > 111)
        continue;
      height = 64;
      if (y > 48)
        height = 112 - y;
      blit[0](canvas, work->sheet, HI(streak->x), y, 4, height);
      x = streak->x + streak->velocityX;
      streak->x = x;
      streak->y += streak->velocityY;
      if (streak->y >> 16 > 48) {
        /* FAKEMATCH(temporary): landing set to the buffer, then advanced; as
         * &PARTICLES[i], 162 of 376 instructions differ in tla-en. */
        struct EffectStep *landing = PARTICLES;

        landing += i;
        if (landing->variant == -1) {
          landing->y = 112 << 16;
          landing->variant = 0;
          landing->x = x;
          landing->velocityX = (Random16() % 12 - 6) << 15;
          landing->velocityY = (-Random16() & 7) << 10;
        }
      }
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Clear the canvas and its characters and hand the layer over.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  /* FAKEMATCH(barrier): an empty asm that keeps the clear routine's load
   * after HeapSlot_Release; without it, 2 of 376 instructions differ in
   * tla-en. */
  __asm__("");
  Iwram_ClearWordsEntry((void *)0x06004000, size);
  Iwram_ClearWordsEntry(canvas, size);
  Task_Remove(Palette_StepFadeTransfer);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  SummonEffect_RunCanvasMode3(effect);
}

void BattleEffect_RunBoltTrail(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Earth Force (kind 459).
void BattleEffect_RunEarthForce(s32 a0) { BattleEffect_RunBoltTrail(a0, 1); }

/// Abilities: Thunder Mine (kind 86).
void BattleEffect_RunThunderMine(s32 a0) { BattleEffect_RunBoltTrail(a0, 0); }
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef SLIM_COLUMN_OFFSET
#undef SPARK_OFFSET
#undef BLIT_MODE
#undef BLIT_CLIP
#undef WYRD_FRAMES
#undef WYRD_DROPS
#undef FLAMES
#undef PARTICLES
#undef STREAKS
#undef SPLASH_OFFSET
#undef SPLASH_SIZE
