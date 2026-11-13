/// @file
/// The Moloch and Iris summons, and the model renderers and projections the
/// effects draw with.
// Part of summon_effects.c.

#include "../summon_effects_internal.h"

struct FxPartyList;
struct Model;

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

// Summon 16's frames: the shower of motes, the rising flames, the rings and
// the closing glow, by frame.
#define SUMMON16_FRAMES 296
#define SUMMON16_SKIP_TO 232

struct Point2D {
  s32 x;
  s32 y;
};

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C1;
extern char ResourceId_Data134;
extern u8 gMapCellBuffer[];
extern const u8 Data_08196f28[];
// The particle sheet's cell offsets by size (The Broken Seal's
// gBattleEffect_MoteCells).
extern u16 gData_08197410[];
extern const u8 Data_08198ec4[];
extern const u8 Data_08198cac[];
extern const u8 Data_08199f34[];
extern const u8 Data_08199f38[];
extern const s8 Data_08199f3c[];

void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Graphics_LoadGreyRamp(void);
void BattleEffect_SelectLivingTargets(struct FxPartyList *party);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void Func_0816442c(s32 formation, s32 x, s32 y);
void Func_0816467c(s32 formation, s32 x, s32 y);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer,
                   s32 frames); // BattleEffect_TintBattler
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void Func_08164a4c(s32 blue, s32 green, s32 red);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Sound_PlayFar(s32 cue);

void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void SceneTransform_ApplyPitch(s32 angle);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);

// Summon 16. A shower of 256 motes drifts down the screen and, from frame
// 91, gathers toward the middle; from frame 97 flames rise in twelve columns
// and from frame 136 four textured rings spread out from the summoned
// objects. At frames 212 to 268 the targets are struck in turn, and the
// scene closes with a glow of the whole palette. A or B skips to frame 232.
//
// The work's particles 0-11 are the flames (velocityZ picks the blitter, 2
// retires the column), particles 24-39 the embers of the closing glow; the
// map cell buffer holds the motes (variant counts down the frames before a
// mote appears, 0 is a live mote).
void BattleEffect_RunMoloch(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct ModelTexture texture;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;
  struct EffectStep *mote;
  struct EffectStep *flame;
  struct EffectStep *motes;
  struct RenderModel *model;
  void *projected;
  s32 i;
  s32 k;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x3000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Graphics_LoadGreyRamp();
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    s32 frame;

    focus.x = 128 << 16;
    focus.y = 64 << 16;
    BattleEffect_SelectLivingTargets(work->effect);
    wait(1);
    BattleEffect_SpawnObjects(12, 654, 2);

    frame = 0;
    do {
      if ((gInput.repeat & 3) && frame <= 231 && frame > 15) {
        Sound_PlayFar(144);
        frame = SUMMON16_SKIP_TO;
      }

      if (frame == 0) {
        for (i = 0; i != 64; i++)
          work->particles[i].variant = -1;

        // The motes start along the top, each after its own delay.
        motes = gMapCellBuffer;
        for (i = 0; i != 256; i++) {
          mote = &motes[i];
          Random16();
          mote->variant = i;
          mote->x = ((Random16() & 255) - 64) << 16;
          mote->y = 0;
          mote->velocityX = 0;
          mote->velocityY = 0;
        }

        // Twelve flame columns rise from y = 72, four pixels apart.
        for (i = 0; i != 16; i++) {
          flame = &work->particles[i];
          flame->x = (i * 3 + i + 44) << 16;
          flame->y = 72 << 16;
          flame->velocityX = 0;
          flame->velocityY = -((Random16() & 3) + 2) << 16;
          flame->velocityZ = 0;
          flame->variant = -i - (Random16() & 15);
        }

        // The embers of the closing glow start at (54, 56).
        for (i = 0; i != 16; i++) {
          flame = &work->particles[24 + i];
          flame->x = 54 << 16;
          flame->y = 56 << 16;
          flame->variant = i * -2;
          flame->velocityX = -(((Random16() & 3) << 16) + (2 << 16));
          flame->velocityY = -(Random16() & 3) << 15;
        }
      }

      if (frame <= 271) {
        motes = gMapCellBuffer;
        for (i = 0; i != 256; i++) {
          mote = &motes[i];
          if (mote->variant == 0) {
            s32 size = Data_08196f28[i & 7];

            draw[0](canvas, graphics + gData_08197410[size - 1],
                    HI(mote->x) - (size >> 1), HI(mote->y) - size, size,
                    size * 2);

            // Frames 91-119: motes still at rest head for a random point of
            // the middle, timed to arrive as they reach the ground.
            if ((u32)(frame - 91) <= 28 && mote->velocityX == 0 &&
                HI(mote->y) <= 113) {
              s32 dx;

              mote->velocityX = ((Random16() & 63) << 16) + (56 << 16);
              dx = mote->velocityX - mote->x;
              mote->velocityX = dx / (((114 << 16) - mote->y) / (size << 14));
            }

            // Frames 142-161: they swirl about the centre line.
            if ((u32)(frame - 142) <= 19) {
              s32 distance = mote->x - (96 << 16);
              s32 pull;

              if (distance < 0)
                distance = -distance;
              pull = (mote->y / 2 - distance + (s32)((Random16() & 31) << 16) +
                      (96 << 16)) /
                     32;
              if (mote->x < 96 << 16 && pull > 0)
                pull = -pull;
              if (mote->x > 96 << 16 && pull < 0)
                pull = -pull;
              mote->velocityX += pull / 8;
            }

            mote->x += mote->velocityX + Trig_Sin(mote->y >> 5) / 5;
            mote->y += mote->velocityY + (size << 14);
            if (mote->y > 114 << 16) {
              if (frame > 215) {
                // Late motes fly back in from the right.
                mote->variant = 0;
                mote->x = 120 << 16;
                mote->y = ((Random16() & 127) - 32) << 16;
                mote->velocityX = ((Random16() & 3) - 8) << 16;
                mote->velocityY = 0;
              } else {
                mote->variant = Random16() & 255;
                mote->x = ((Random16() & 255) - 64) << 16;
                mote->y = -8 << 16;
                mote->velocityX = 0;
                mote->velocityY = 0;
              }
            }
          } else {
            mote->variant--;
          }

          // Frames 217-245: the wind turns.
          if ((u32)(frame - 217) <= 28) {
            mote->velocityX -= 0x2000;
            mote->velocityY += 0x2000;
          }
        }
      }

      // Frames 91-255: the flame columns, a 32x64 cell for every four frames.
      if ((u32)(frame - 91) <= 164) {
        for (i = 0; i != 12; i++) {
          flame = &work->particles[i];
          if (flame->velocityZ != 2) {
            if ((u32)flame->variant <= 23) {
              draw[flame->velocityZ](canvas,
                                     work->sheet + (flame->variant >> 2) * 2048,
                                     HI(flame->x), HI(flame->y), 32, 64);
              flame->x += flame->velocityX;
              flame->y += flame->velocityY;
            }
            if (++flame->variant == 24) {
              flame->x = (i * 3 + i + 44) << 16;
              flame->y = 72 << 16;
              flame->variant = 0;
              flame->velocityY = -((Random16() & 3) + 2) << 16;
              flame->velocityZ = 0;
              if ((u32)(frame - 97) <= 42)
                flame->velocityZ = 1;
              else if (frame > 140)
                flame->velocityZ = 2;
            }
            if (frame == 136) {
              if (flame->x < 80 << 16)
                flame->velocityX -= 4 << 16;
              else
                flame->velocityX += 4 << 16;
            }
          }
        }
      }

      // Frames 201-271: the embers of the closing glow.
      if ((u32)(frame - 200) <= 71) {
        if (frame > 200) {
          for (i = 0; i != 16; i++) {
            flame = &work->particles[24 + i];
            if ((u32)flame->variant <= 23) {
              draw[0](canvas, work->sheet + (flame->variant >> 2) * 2048,
                      HI(flame->x), HI(flame->y), 32, 64);
              flame->x += flame->velocityX;
              flame->y += flame->velocityY;
            }
            if (++flame->variant == 24) {
              flame->x = 54 << 16;
              flame->y = 56 << 16;
              flame->velocityX = -(((Random16() & 3) << 16) + (2 << 16));
              flame->variant = 0;
            }
          }
        }
      }

      // From frame 136: four rings, each starting at its own frame, grow and
      // fade out over eight frames.
      if (frame > 135) {
        projected = Heap_AllocEwram(1024);
        model = Render_AllocateModelList(1);
        texture.logWidth = 7;
        texture.logHeight = 5;
        texture.pixels = work->sheet + 0x3000;
        model->mesh = Data_08198ec4;
        model->kind = 7;
        model->texture = &texture;
        model->vertices = projected;
        model->scrollU = 0;
        model->scrollV = 0;
        for (i = 0; i != 4; i++) {
          s32 start = Data_08199f34[i] + 136;
          s32 scale;
          s32 shade;

          if (frame <= start)
            continue;
          scale = (frame - start) * Data_08199f38[i] * 2000 + 0x8000;
          shade = (start - frame) * 8 + 56;
          if (shade > 0)
            shade = 0;
          if (shade <= -64)
            continue;
          model->shade = shade;
          Render_ResetTransformState();
          Func_08015160(16 << 16, Data_08199f3c[i] << 16, 0);
          Func_080151e4(scale, scale, scale);
          SceneTransform_ApplyPitch(0xbb8);
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Render_TransformVertices(Data_08198cac, projected, 32);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(projected);
      }

      if (frame > 139)
        Func_0816442c(5, focus.x, focus.y);

      // Frames 212-268, every eight: the targets flash and are thrown back.
      for (i = 0; i != 8; i++) {
        if (frame == i * 8 + 212) {
          for (k = 0; k != work->effect->count; k++) {
            Func_0814cd48(work->effect->actors[k], 7, 5, k, 8);
            BattleEffect_StartActorMotion(effect->actors[k], 1, 0x10000,
                                          0x40000, 0x10000, i * 16 + 100);
          }
        }
      }
      if ((u32)(frame - 201) <= 54)
        work->shakeFrames = 2;

      if (frame == 32)
        Sound_PlayFar(246);
      if (frame == 90)
        Sound_PlayFar(225);
      if (frame == 140)
        Sound_PlayFar(154);
      if (frame == 200)
        Sound_PlayFar(208);
      if (frame == 232)
        Sound_PlayFar(144);

      // Frames 256-271 brighten the whole screen to white; at 272 the battle
      // phase resumes and the canvas fades back.
      if (frame == 272) {
        Func_081180e8(134);
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Graphics_LoadGreyRamp();
      } else if ((u32)(frame - 256) <= 15) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        if (frame > 269)
          work->transferValue = 0x3f3f3f3f;
        Func_08164a4c(2, 2, 2);
      }

      BattleCamera_Shake(4, 8);
      BattleEffect_CountDownBattlerTints();
      frame++;
      work->transferPending = 1;
      wait(1);
    } while (frame != SUMMON16_FRAMES);

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    Func_0816467c(5, focus.x, focus.y);
    for (i = 0; i != 12; i++)
      ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}

extern char ResourceId_VioletSkyBackdrop;

void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleBackground_LoadFar(s32 mode, s32 background, s32 level);

// Summon 25's backdrop change: clears the targets' ground motion and drops
// each a little above the ground at a random height, empties the actor
// list, loads the violet sky and recolours its 128 BG palette entries from
// 96 on: each channel is darkened and the three are rotated (red to green,
// green to blue, blue to red).
void Func_0819253c(void) {
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  s16 list[14];
  u16 *palette;
  s32 i;

  REG_BG1HOFS = 0;
  gCameraSceneParameters[4] = 240;

  // The targets fall in from just above the ground, halfway closer in depth.
  for (i = 0; i != work->effect->count; i++) {
    struct MotionObject *object =
        GetBattleObjectSlotFar(work->effect->actors[i])->object;

    object->x = 0;
    object->y = (16 - (Random16() & 15)) << 16;
    object->z /= 2;
    object->verticalMotionStrength = 0;
  }

  // No actor objects over the new backdrop.
  list[0] = 0xff;
  BattleActor_SpawnObjectsForListFar(list, 0);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);

  palette = (u16 *)BG_PLTT + 96;
  for (i = 0; i != 128; i++) {
    s32 color = *palette;
    s32 blue = ((u16)color >> 10) & 31;
    s32 green = ((u16)color >> 5) & 31;
    s32 red = 31;

    red &= color;
    red -= 16;
    green -= 24;
    blue -= 20;

    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *palette++ = green << 10 | red << 5 | blue;
  }
  transition->active = 1;
  gBgScroll[1].x = 0;
}

// Summon 25: draws one falling drop at (x, y), large (20x40) when small is 0
// and small (12x24) otherwise, its animation picture chosen by frame / 2, and
// under it a trail of particles every half size down to row 112.
void Func_08192648(u32 frame, s32 x, s32 y, s32 small) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  u32 size;
  u32 half;
  s32 i;
  s32 trailY;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The drop: one 20x40 or 12x24 cell of its strip in the sheet.
  if (small == 0)
    draw[1](canvas, work->sheet + 0x1b12 + frame / 2 * (20 * 40), x - 10,
            y - 20, 20, 40);
  else
    draw[1](canvas, work->sheet + 0x4c12 + frame / 2 * (12 * 24), x - 6, y - 12,
            12, 24);

  // The trail: particles of size 8 (6 for a small drop), half a size apart.
  size = 8 - small * 2;
  half = size / 2;
  // A goto loop: loop.c leaves it alone, so y + i * half multiplies on every
  // pass and the table entry and frame / 2 * 0x302 are recomputed; as a for
  // loop, 111 of 120 instructions differ in tla-en. The argument x - size / 2
  // is not one GCSE carries out of the loop (as x - half, 83 differ).
  i = 1;
  trailY = y + i * half;
  if (trailY > 112)
    return;
next:
  // The offset grouped before the sheet base: ungrouped, 2 instructions
  // differ (the base is added before the product).
  draw[0](canvas, work->sheet + (gData_08197410[size - 1] + frame / 2 * 0x302),
          x - size / 2, trailY, size, size * 2);
  i++;
  trailY = y + i * half;
  if (trailY <= 112)
    goto next;
}

// One model vertex: signed byte coordinates, padded to a word.

extern const struct ModelVertex Data_08196f30[6];
extern const u16 Data_08199f4c[];

void SceneTransform_ApplyPosition(const s32 *pos);

// Summon 25: draws one rising pillar, a textured strip of six vertices whose
// two lower rows come from Data_08196f30 and whose top row is raised to
// height. x is the pillar's screen column (63 is the centre); the base sits
// height pixels below row 63, so the pillar grows upward from there.
void Func_0819273c(s32 halfWidth, s32 x, s32 height) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  void *projected;
  struct RenderModel *model;
  struct ModelVertex vertices[6];
  struct ModelTexture texture;
  struct Vector3 position;

  projected = Heap_AllocEwram(48);
  model = Render_AllocateModelList(1);
  memcpy(vertices, Data_08196f30, sizeof(vertices));

  // A 16x16 texture at 0x302 into the sheet.
  texture.logWidth = 4;
  texture.logHeight = 4;
  texture.pixels = work + 0xa02;

  // Each row is halfWidth to either side; the top row reaches height.
  vertices[0].x = -halfWidth;
  vertices[1].x = halfWidth;
  vertices[2].x = -halfWidth;
  vertices[3].x = halfWidth;
  vertices[4].x = -halfWidth;
  vertices[5].x = halfWidth;
  vertices[5].y = height;
  vertices[4].y = height;

  Render_ResetTransformState();
  position.x = (x << 16) - (63 << 16);
  position.y = (63 - height) << 16;
  position.z = 0;
  SceneTransform_ApplyPosition(&position);

  model->kind = 4;
  model->texture = &texture;
  model->mesh = Data_08199f4c;
  model->vertices = projected;
  Render_TransformVertices(vertices, projected, 6);
  Render_DrawModelList(model);
  Heap_Release(model);
  Heap_Release(projected);
}

void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

/// Draws a 45-degree canvas line from (x + w/2, y - w/2) down-left toward
/// (x, y), at most 48 pixels long, in the given colour.
void BattleEffect_DrawDiagonalCanvasLine(s32 x, s32 y, s32 w, u8 color) {
  s32 h = w >> 1;
  s32 x1;
  s32 y1;

  x += h;
  y -= h;
  if (h > 48)
    h = 48;
  y1 = y + h;
  x1 = x - h;
  Func_08143eb4(x, y, x1, y1, color);
}

struct StripQuad;

s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags);   // BattleEffect_BuildBlitter
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyYaw(s32 angle);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll

void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HALF(p, n) (((s16 *)(p))[n])

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

struct Scale {
  s32 x;
  s32 y;
};

// The spark objects' work, in the map cell buffer at 0x1f40.
struct IrisSparks {
  s16 life[36]; // high byte: frames left; 0 frees the slot
  u16 tiles[15];
  u16 frames[36];
  s32 x[36];
  s32 y[36];
  s32 vx[36];
  s32 vy[36];
};

// A point of a streak: the head moves, the other nine trail it.
struct IrisPoint {
  s32 x;
  s32 y;
  s32 angle;
  s32 speed;
  s32 unk10;
};

// The opening's circle of light and its fifteen streaks, at 0x2ee0.
struct IrisScene {
  union {
    s32 x;
    struct {
      u16 xFraction;
      s16 xPixels;
    };
  };
  union {
    s32 y;
    struct {
      u16 yFraction;
      s16 yPixels;
    };
  };
  s32 unk08;
  s32 unk0c;
  union {
    s32 radius;
    struct {
      u16 radiusFraction;
      s16 radiusPixels;
    };
  };
  struct IrisPoint streaks[15][10];
};

struct IrisSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 unk08_10 : 6;
  u8 unk0a[6];
  u8 resource;
};

struct IrisSpriteFlags {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
};

struct Widths5 {
  s32 w[5];
};

struct Widths7 {
  s32 w[7];
};

struct Spans4 {
  struct WordPair p[4];
};

extern char ResourceId_SunsetBackdrop, ResourceId_StarfieldFlareBackdrop;
extern char ResourceId_RobedGoddessTiles, ResourceId_PinkPetalsTiles;
extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 Data_02010578[];
extern struct IrisSparks Data_02011f40;
extern struct IrisScene Data_02012ee0;
#define Data_02012ef4 ((struct IrisPoint(*)[10])(Ram_MapCellBuffer + 0x2ef4))

extern const struct Scale Data_08196f48;
extern const struct Widths5 Data_08196f50;
extern const struct Widths7 Data_08196f64;
extern const struct Widths7 Data_08196f80;
extern const struct Spans4 Data_08196f9c;
extern const struct Spans4 Data_08196fbc;
extern s32 Data_08199f40[];
extern const u8 Data_08199f48[];
extern const u8 Data_08199f88[];
extern const u8 Data_08199f98[];
extern const u8 Data_08199fbc[];
extern const u16 Data_08199fc8[];
extern const u16 Data_08199fd2[];
extern const s8 Data_08199fde[];
extern const u8 Data_08199fed[];
extern const u16 Data_08199ffc[];
extern const s16 Data_0819a01a[];
extern const u8 Data_0819a038[];
extern s32 Data_0819a040[];
extern const u16 Data_0819a048[];
extern const u16 Data_0819a054[];
extern const u8 Data_0819a060[];
extern const u8 Data_0819a066[];
extern const u8 Data_0819a06c[];
extern const u8 Data_0819a072[];
extern const u8 Data_0819a078[];
extern const u8 Data_0819a07e[];
extern const u8 Data_0819a084[];
extern const u8 Data_0819a0ac[];
extern const u8 Data_0819a0d4[];
extern const u8 Data_0819a140[];
extern const u8 Data_0819a160[];

struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);
struct IrisSprite *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void BattleEffect_ConfigureEffectDisplay(void);

void Palette_LoadBasicObjColors(void);
void Func_0815b510(u8 *map, s32 size, s32 scale);
void Func_08164b2c(s32 red, s32 green, s32 blue);
void Func_08191958(u8 side, u8 speed, void (*midpoint)(void));
void Func_08191cc4(void);
void BattleEffect_BuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats,
                                          s32 scale);
void SceneTransform_ResetMatrix(void);
void Func_080151ac(s32 *scale); // SceneTransform_ApplyScale

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)
#define SCENE (&Data_02012ee0)
#define PLTT 0x05000000
#define STREAK(i, j) (((struct IrisPoint *)Data_02012ef4)[(i) * 10 + (j)])
// The 128 background colours the scene fades (entries 96 to 223).
#define FADE_PLTT ((u16 *)(PLTT + 0xc0))

#define Iwram_FillWords(dest, size, value)                                          \
  do {                                                                         \
    Iwram_FillWords(dest, size, value);                                         \
  } while (0)

#define DECODE_01(src, dest)                                                   \
  do {                                                                         \
    Resource_DecodeType01(src, dest);                                          \
  } while (0)

// A splash where a drop lands: rings spread from (x, y) for twice the
// splash's duration, then a smaller pair of rings and a column of spray
// rises while the screen shakes; the drop itself is drawn last.
static inline void Iris_DrawSplash(s32 t, s32 x, s32 y, s32 small) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  DrawRectangle drawAlt;
  DrawRectangle draw;
  u8 *sprites;
  s32 r;
  s32 cel;
  s32 cel8;
  s32 sheetOffset;
  s32 point;
  s32 segment;
  s32 column;
  s32 dead;

  sprites = Ram_HeapSlots->unk64[0];
  draw = Ram_HeapSlots->unk64[1];
  drawAlt = Ram_HeapSlots->unkb8[1];
  if (t < *(Data_08199f40 + small) * 2) {
    r = *(Data_08199f40 + small) * 2 - t;
    cel = r / 6;
    cel8 = cel * 8;
    sheetOffset = cel * 0x302;
    for (point = 0; point != 64; point++) {
      s32 px;
      s32 a;
      s32 py;
      a = point << 10;
      px = (r * Trig_Sin(a) / 2 >> 16) + x;
      py = (r * Trig_Cos(a) >> 16) + y;
      draw(canvas, sprites + cel8 + 444, px, py - 2, 2, 4);
    }
    if (small == 0) {
      s32 offset = 0;
      u8 *ring;
      DrawRectangle blit;
      s32 spoke;

      if (t <= 11)
        offset = 196;
      else if (t <= 23)
        offset = 98;
      /* FAKEMATCH(dead): t * 4 / 3 into dead, which flow deletes once PRE has
       * copied t * 4 into the join ahead of ring and blit; without it, 700 of
       * 4851 instructions of BattleEffect_RunIris differ in tla-en. */
      dead = t * 4;
      dead /= 3;
      ring = sprites + offset;
      /* FAKEMATCH(temporary): draw copied into blit for the ring's draws; as
       * draw called directly, 1126 of 4851 instructions of
       * BattleEffect_RunIris differ in tla-en. */
      blit = draw;
      for (spoke = 0; spoke != 8; spoke++) {
        s32 px;
        s32 a;
        s32 py;
        a = (spoke << 13) + (t * 5 << 9);
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        blit(canvas, ring, px - 3, py - 7, 7, 14);
        a += a >> 1;
        px = (Iwram_MulQ16(0x28000, r * Trig_Sin(a) / 2) >> 16) + x;
        py = (Iwram_MulQ16(0x50000, r * Trig_Cos(a) / 2) >> 16) + y;
        blit(canvas, ring, px - 3, py - 7, 7, 14);
      }
      draw(canvas, work->sheet + (gData_08197410[8] + sheetOffset), x - 4,
           y - 9, 9, 18);
    } else {
      s32 offset = 294;
      u8 *ring;
      DrawRectangle blit;
      s32 spoke;

      if (t <= 11)
        offset = 394;
      else if (t <= 23)
        offset = 344;
      /* FAKEMATCH(dead): t * 4 / 3 into dead, as in the large splash's arm;
       * without it, 698 of 4851 instructions of BattleEffect_RunIris differ in
       * tla-en. */
      dead = t * 4;
      dead /= 3;
      ring = sprites + offset;
      /* FAKEMATCH(temporary): draw copied into blit for the ring's draws; as
       * draw called directly, 1120 of 4851 instructions of
       * BattleEffect_RunIris differ in tla-en. */
      blit = draw;
      for (spoke = 0; spoke != 8; spoke++) {
        s32 px;
        s32 a;
        s32 py;
        a = (spoke << 13) + (t * 5 << 9);
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        blit(canvas, ring, px - 2, py - 5, 5, 10);
        a += a >> 1;
        px = (Iwram_MulQ16(0x28000, r * Trig_Sin(a) / 2) >> 16) + x;
        py = (Iwram_MulQ16(0x50000, r * Trig_Cos(a) / 2) >> 16) + y;
        blit(canvas, ring, px - 2, py - 5, 5, 10);
      }
      draw(canvas, work->sheet + (gData_08197410[6] + sheetOffset), x - 3,
           y - 7, 7, 14);
    }
  } else {
    if (t < *(Data_08199f40 + small) * 2 + *(Data_08199f40 + small)) {
      s32 ringCel;

      r = t - *(Data_08199f40 + small) * 2;
      ringCel = r / 3 * 8;
      if (ringCel == 64)
        ringCel = 56;
      ringCel += 444;
      for (point = 0; point != 64; point++) {
        s32 px;
        s32 a;
        s32 py;
        a = point << 10;
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        draw(canvas, sprites + ringCel, px - 1, py - 2, 2, 4);
      }
      for (point = 0; point != 64; point++) {
        s32 px;
        s32 a;
        s32 py;
        a = point << 10;
        px = (r * Trig_Sin(a) * 2 >> 16) + x;
        py = (r * Trig_Cos(a) >> 16) + y + Data_08199f48[small] * 2;
        draw(canvas, sprites + ringCel, px - 1, py - 2, 2, 4);
      }
    }
    for (segment = 0;
         segment < t - *(Data_08199f40 + small) * 2 && y + segment * 32 <= 112;
         segment++)
      draw(canvas, sprites + 508, x - 4, y + segment * 32, 8, 32);
    if (t < *(Data_08199f40 + small) * 2 + 8) {
      s32 shake = (*(Data_08199f40 + small) * 2 - t + 8) >> 1;
      typeof(gBgScroll[0]) *scroll = gBgScroll;
      s16 base = 32;

      // DEAD: both arms below set it again.
      scroll[1].y = base;
      if (t & 1)
        scroll[1].y = shake + 32;
      else
        scroll[1].y = base - shake;
    }
    if (small == 0)
      drawAlt(canvas, work->sheet + 0x1b12, x - 10, y - 20, 20, 40);
    else
      drawAlt(canvas, work->sheet + 0x4c12, x - 6, y - 12, 12, 24);
  }
}

// Summon 25 (Iris): a circle of light opens over a dark field while fifteen
// streaks swirl up out of it; the goddess's crystal rises in a 4 x 4 grid
// of sprites and bursts into sparks; drops fall and splash in rings; pillars
// of light climb, the scene fills with rising motes, rays turn about the
// centre, and the screen whitens as she heals the party.
void BattleEffect_RunIris(struct BattleEffectArgument *effect) {
  typeof(SCENE) scene = SCENE;
  struct IrisPoint(*streaks)[10] = Data_02012ef4;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct IrisSprite **objects = (struct IrisSprite **)gMapCellBuffer;
  u8 *sprites = Ram_HeapSlots->unk64[0];
  struct IrisSparks *sparks = &Data_02011f40;
  u8 *battle = (u8 *)Ram_HeapSlots->battleWork;
  s32 savedScroll = gBgScroll[1].x;
  s16 *camera = Ram_HeapSlots->cameraWork;
  s32 savedZoom;
  s32 frame;
  void *draw[2];

  // The first spark slot with no life left, or -1.
  s32 FindFreeSpark(void) {
    s32 i;
    s32 slot = 36;

    for (i = 0; i != 36; i++) {
      if (sparks->life[i] == 0) {
        slot = i;
        break;
      }
    }
    if (slot == 36)
      slot = -1;
    return slot;
  }

  // Each live spark: placed at its size, moved, dropped when it leaves the
  // screen, otherwise animated and aged.
  void UpdateSparks(void) {
    struct Scale scale;
    s32 place[4];
    s32 i;

    for (i = 0; i != 36; i++) {
      if (sparks->life[i] != 0) {
        scale.x = scale.y = sparks->life[i] * 2;
        place[3] = 0;
        place[1] = 0xff0000;
        place[0] = sparks->x[i];
        place[2] = sparks->y[i];
        Render_ApplyProjectedPlacementFar(objects[i], place, (s32 *)&scale, 0);
        sparks->x[i] += sparks->vx[i];
        sparks->y[i] += sparks->vy[i];
        if (sparks->x[i] < -0x100000 || sparks->x[i] > 0xf00000 ||
            sparks->y[i] < 0 || sparks->y[i] > 0xa00000) {
          sparks->life[i] = 0;
          continue;
        }
        if (!(frame & 3))
          sparks->frames[i]++;
        if (sparks->frames[i] > 14)
          sparks->frames[i] = 0;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
        sparks->life[i] -= 0x100;
      }
    }
  }

  // The first free mote of 32, or -1.
  s32 FindFreeParticle(void) {
    s32 i;
    s32 slot = 32;

    for (i = 0; i != 32; i++) {
      if (work->particles[i].variant == 0) {
        slot = i;
        break;
      }
    }
    if (slot == 32)
      slot = -1;
    return slot;
  }

  // The motes: a sheet cell, fading after 32 frames with bit 4, or a
  // textured quad spinning up with bit 8.
  void DrawParticles(void) {
    struct ModelTexture texture;
    s32 place[3];
    s32 i;

    for (i = 0; i != 32; i++) {
      if (work->particles[i].variant != 0) {
        s32 n = work->particles[i].variant & 15;
        s32 px = work->particles[i].x >> 16;
        s32 py = work->particles[i].y >> 16;
        s32 offset = 0;

        if (work->particles[i].x < 0 || work->particles[i].x > 0x7f0000 ||
            work->particles[i].y < 0 || work->particles[i].y > 0x7f0000) {
          work->particles[i].variant = 0;
          continue;
        }
        if (work->particles[i].variant & 16) {
          s32 age = frame - work->particles[i].z;

          if (age > 32)
            offset = (age - 32) * 0x302;
          if (offset == 0x1810) {
            work->particles[i].variant = 0;
            continue;
          }
        }
        if (work->particles[i].variant & 256) {
          void *verts = Heap_AllocEwram(32);
          struct RenderModel *model = Render_AllocateModelList(1);
          s32 age;

          offset = 0;
          age = frame - work->particles[i].z;
          if (age > 24)
            offset = (age - 24) * 0x800;
          texture.logWidth = 5;
          texture.logHeight = 6;
          texture.pixels = work->sheet + 0x1810 + offset;
          SceneTransform_ResetMatrix();
          place[0] = work->particles[i].x - 0x400000;
          place[1] = work->particles[i].y - 0x400000;
          place[2] = 0;
          SceneTransform_ApplyPosition(place);
          place[1] = place[0] = work->particles[i].velocityZ;
          place[2] = 0;
          Func_080151ac(place);
          work->particles[i].velocityZ += 0x2000;
          model->kind = 4;
          model->texture = &texture;
          model->mesh = Data_08199f98;
          model->vertices = verts;
          Render_TransformVertices(Data_08199f88, verts, 4);
          Render_DrawModelList(model);
          Heap_Release(model);
          Heap_Release(verts);
          if (offset == 0x3800)
            work->particles[i].variant = 0;
        } else {
          ((DrawRectangle)draw[0])(
              canvas, work->sheet + (gData_08197410[n - 1] + offset),
              px - n / 2, py - n, n, n * 2);
        }
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
      }
    }
  }

  s32 motesMask;
  u8 *tileBuffer;
  s32 pos[4];
  struct Scale scale;
  struct Scale shrink;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  s32 lift;
  s32 i;
  s32 j;

  s32 column;
  s32 m;
  s32 off;
  u8 *dst;
  u8 *tiles;
  /* FAKEMATCH(dead): 284 locals nothing uses (any count from 275 to 294
   * does), which number the later pseudos so PRE's spilled registers take
   * their frame slots in order; as no locals, 34 of 4851 instructions differ
   * in tla-en. */
  UNUSED s32 z0, z1, z2, z3, z4, z5, z6, z7, z8, z9, z10, z11, z12, z13, z14,
      z15, z16, z17, z18, z19, z20, z21, z22, z23, z24, z25, z26, z27, z28, z29,
      z30, z31, z32, z33, z34, z35, z36, z37, z38, z39, z40, z41, z42, z43, z44,
      z45, z46, z47, z48, z49, z50, z51, z52, z53, z54, z55, z56, z57, z58, z59,
      z60, z61, z62, z63, z64, z65, z66, z67, z68, z69, z70, z71, z72, z73, z74,
      z75, z76, z77, z78, z79, z80, z81, z82, z83, z84, z85, z86, z87, z88, z89,
      z90, z91, z92, z93, z94, z95, z96, z97, z98, z99, z100, z101, z102, z103,
      z104, z105, z106, z107, z108, z109, z110, z111, z112, z113, z114, z115,
      z116, z117, z118, z119, z120, z121, z122, z123, z124, z125, z126, z127,
      z128, z129, z130, z131, z132, z133, z134, z135, z136, z137, z138, z139,
      z140, z141, z142, z143, z144, z145, z146, z147, z148, z149, z150, z151,
      z152, z153, z154, z155, z156, z157, z158, z159, z160, z161, z162, z163,
      z164, z165, z166, z167, z168, z169, z170, z171, z172, z173, z174, z175,
      z176, z177, z178, z179, z180, z181, z182, z183, z184, z185, z186, z187,
      z188, z189, z190, z191, z192, z193, z194, z195, z196, z197, z198, z199,
      z200, z201, z202, z203, z204, z205, z206, z207, z208, z209, z210, z211,
      z212, z213, z214, z215, z216, z217, z218, z219, z220, z221, z222, z223,
      z224, z225, z226, z227, z228, z229, z230, z231, z232, z233, z234, z235,
      z236, z237, z238, z239, z240, z241, z242, z243, z244, z245, z246, z247,
      z248, z249, z250, z251, z252, z253, z254, z255, z256, z257, z258, z259,
      z260, z261, z262, z263, z264, z265, z266, z267, z268, z269, z270, z271,
      z272, z273, z274, z275, z276, z277, z278, z279, z280, z281, z282, z283;

  work->effect = effect;
  BattleEffect_StartCanvasLayer(0x2000);
  BattleEffect_ConfigureEffectDisplay();
  *(u16 *)PLTT = 0;
  *(u16 *)(PLTT + 2) = 0;
  work->transferMode = 0;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  REG_DISPCNT = 0x7741;
  REG_BG2CNT = 0x784;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  Func_08191958(0, 4, Func_0819253c);
  Iwram_FillWords(canvas, 0x4000, 0);
  while (((volatile struct DmaRegisters *)REG_DMA3)->control & 0x80000000) {
  }
  work->transferPending = 1;
  wait(1);
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  work->transferMode = 2;
  work->transferValue = 75;
  savedZoom = camera[27];
  if (effect->side == 0) {
    s32 yaw = 0x4000;

    camera[27] = yaw;
  } else {
    s32 yaw = 0xc000;

    camera[27] = yaw;
  }
  tileBuffer = Data_02010578;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  BattleEffect_LoadBlitter(SLOT_ALT, 3);
  draw[0] = Ram_HeapSlots->unk64[1];
  draw[1] = Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, tileBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  for (i = 0; i != 0x302; i++)
    sprites[i] >>= 1;
  Func_0815b434(work->sheet, 32, 0x10000);
  Graphics_LoadGreyRamp();
  frame = 0;
  HeapSlot_Release(SLOT_ALT);
  Render_ResetTransformState();

  for (;;) {
    motesMask = 31;
    // A or B: skips the opening, or the rest of the scene.
    if (gInput.repeat & 3) {
      if (frame <= 175) {
        frame = 176;
      } else if ((u32)(frame - 177) <= 268) {
        REG_BLDALPHA = 0x1010;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        work->transferPending = 1;
        wait(1);
        if (frame <= 386) {
          for (i = 0; i != 16; i++)
            ResourceObject_ReleaseFar(work->objects[i]);
          BattleEffect_SelectLivingTargets(work->effect);
          Palette_LoadBasicObjColors();
          for (i = 0; i != work->effect->count; i++)
            Func_0814cd48(work->effect->actors[i], -1, 5, -1, 0);
        }
        frame = 446;
      } else {
        Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
        work->transferPending = 1;
        wait(1);
        break;
      }
    }
    if (frame == 850)
      break;

    if (frame == 0)
      Sound_PlayFar(206);
    for (i = 0; i != 11; i++)
      if (frame == Data_08199fbc[i])
        Sound_PlayFar(130);
    if (frame == 175)
      Sound_PlayFar(212);
    for (i = 0; i != 5; i++)
      if (frame == Data_08199fc8[i])
        Sound_PlayFar(209);
    for (i = 0; i != 6; i++)
      if (frame == Data_08199fd2[i])
        Sound_PlayFar(212);
    if (frame == 369)
      Sound_PlayFar(206);
    for (i = 0; i != 6; i++)
      if (frame == 644 + i * 12)
        Sound_PlayFar(104);
    if (frame == 755)
      Sound_PlayFar(212);
    for (i = 0; i != 6; i++)
      if (frame == 759 + i * 9)
        Sound_PlayFar(145);
    if (frame == 820)
      Sound_PlayFar(208);

    // The opening: the circle and the streaks.
    if (frame == 0) {
      scene->x = 0x3c0000;
      scene->y = 0x800000;
      scene->radius = 0xc0000;
      for (i = 0; i != 15; i++) {
        struct IrisPoint *s = streaks[i];

        s->x = Data_08199fde[i] << 16;
        s->y = Data_08199fed[i] << 16;
        s->angle = Data_08199ffc[i];
        s->speed = Data_0819a01a[i];
        s->unk10 = 0;
        for (j = 1; j != 10; j++) {
          STREAK(i, j).x = 0x960000;
          STREAK(i, j).y = 0x960000;
        }
      }
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame <= 175) {
      BattleEffect_LoadBlitter(SLOT_ALT, 3);
      draw[1] = Ram_HeapSlots->unkb8[1];
      for (i = 0; i != 15; i++) {
        if (!(frame & 1)) {
          for (j = 9; j != 0; j--) {
            STREAK(i, j).x = STREAK(i, j - 1).x;
            STREAK(i, j).y = STREAK(i, j - 1).y;
          }
        }
        if (frame >= i * 8 + 24) {
          struct IrisPoint *s = streaks[i];
          if (HALF(s, 3) > 59) {
            s32 size;

            s->x += Trig_Sin(s->angle) * 3 / 2;
            s->y -= Trig_Cos(s->angle) * 3 / 2;
            s->angle += s->speed * 2 / 3;
            size = (i & 3) + 5;
            for (j = 0; j != 10; j++) {
              u8 *to = canvas;
              u8 *cell = sprites + gData_08197410[size - 1];
              struct IrisPoint *p = &STREAK(i, j);

              ((DrawRectangle)draw[1])(to, cell, HALF(p, 1) - (size >> 1),
                                       HALF(p, 3) - size, size, size * 2);
            }
          }
        }
      }
      if (frame <= 71) {
        scene->y -= 0x10000;
        scene->radius -= 0x1000;
        if (!(frame & 15)) {
          {
            u16 *fade = FADE_PLTT;

            for (i = 0; i != 128; i++) {
              s32 c = *fade;
              s32 b = (u16)c >> 10 & 31;
              s32 g = (u16)c >> 5 & 31;
              s32 r = c & 31;

              r += 1;
              *fade++ = (b + 1) << 10 | (g + 1) << 5 | r;
            }
          }
        }
      } else {
        scene->radius += 0x2000;
        if (!(frame & 7)) {
          {
            u16 *fade = FADE_PLTT;

            for (i = 0; i != 128; i++) {
              s32 c = *fade;
              s32 b = (u16)c >> 10 & 31;
              s32 g = (u16)c >> 5 & 31;
              s32 r = c & 31;

              if (r < 23)
                r++;
              if (g > 1)
                g--;
              if (b < 15)
                b++;
              {
                /* FAKEMATCH(temporary): the colour copied into a u16 before its
                 * store; as *fade++ = b << 10 | g << 5 | r, 171 of 4851
                 * instructions differ in tla-en. */
                u16 v = b << 10 | g << 5 | r;

                *fade++ = v;
              }
            }
          }
        }
      }
      HeapSlot_Release(SLOT_ALT);
      {
        s32 size = scene->radiusPixels * 2;

        Func_0815b434(work->sheet, size, 0x10000);
        Func_0818caa8(work->sheet, scene->xPixels, scene->yPixels, size);
      }
      if (frame == 175) {
        REG_BG2CNT = 0x785;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        Iwram_FillWords(FADE_PLTT, 0x100, 0x7fff7fff);
      }
    }

    // The crystal: sixteen sprites, the sparks and the first motes.
    if (frame == 176) {
      u8 *colors;
      struct ResourceTableEntry *table;

      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      work->transferPending = 1;
      wait(1);
      battle[0x867] = 1;
      BattleBackground_LoadFar(1, (s32)&ResourceId_SunsetBackdrop, 0);
      colors = Resource_GetTableEntry((s32)&ResourceId_RobedGoddessTiles);
      Dma_Set(colors, (void *)(PLTT + 0x200), 0x84000080, REG_DMA3);
      colors += 0x200;
      Resource_DecodeType01(colors, tileBuffer);
      tiles = Data_02010578;
      for (i = 0; i != 16; i++) {
        struct IrisSprite *obj = Func_0815b290(32, 32, 0x80002000, 0);

        work->objects[i] = obj;
        ((struct IrisSpriteFlags *)obj)->priority = 1;
        Dma_Set(tiles + i * 0x400, (void *)OBJ_TILES(obj), 0x84000100,
                REG_DMA3);
      }
      colors = Resource_GetTableEntry((s32)&ResourceId_PinkPetalsTiles);
      Iwram_FillWords((void *)(PLTT + 0x3e0), 32, -1);
      colors += 32;
      DECODE_01(colors, work->sheet);
      table = gResourceTableEntries;
      for (i = 0; i != 15; i++) {
        struct IrisSprite *obj = Func_0815b290(16, 16, 0x40000000, 0xf000);

        objects[i] = obj;
        ((struct IrisSpriteFlags *)obj)->priority = 1;
        sparks->tiles[i] = obj->tile;
        sparks->frames[i] = i;
        Dma_Set(work->sheet + i * 128,
                (void *)(0x06010000 + table[obj->resource].tile), 0x84000020,
                REG_DMA3);
      }
      for (; i != 36; i++) {
        struct IrisSprite *obj = Func_0815b3b0(16, 16, 0x40000000, 0xf000);

        objects[i] = obj;
        Dma_Set(objects[0], obj, 0x84000006, REG_DMA3);
        sparks->frames[i] = Random16() % 15;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
      }
      for (i = 0; i != 18; i++) {
        sparks->x[i] = (Random16() % 15 << 16) + 0x390000;
        sparks->y[i] = (Random16() % 15 << 16) + 0x4f0000;
        sparks->vx[i] = ((Random16() & 63) - 32) << 12;
        sparks->vy[i] = ((Random16() & 63) - 32) << 12;
      }
      for (; i != 36; i++) {
        sparks->x[i] = (Random16() % 15 << 16) + 0xa90000;
        sparks->y[i] = (Random16() % 15 << 16) + 0x4f0000;
        sparks->vx[i] = ((Random16() & 63) - 32) << 12;
        sparks->vy[i] = ((Random16() & 63) - 32) << 12;
      }
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->sheet, 0, 0);
      for (i = 0; i != 6; i++) {
        work->particles[i].x = (Random16() % 15 << 16) + 0x210000;
        work->particles[i].y = (Random16() % 15 << 16) + 0x390000;
        work->particles[i].velocityX = -(Random16() & motesMask) << 12;
      }
      for (; i != 12; i++) {
        work->particles[i].x = (Random16() % 15 << 16) + 0x510000;
        work->particles[i].y = (Random16() % 15 << 16) + 0x390000;
        work->particles[i].velocityX = (Random16() & motesMask) << 12;
      }
      work->transferMode = 2;
      work->transferValue = 70;
    }
    if ((u32)(frame - 177) <= 216) {
      scale = Data_08196f48;
      pos[3] = 0;
      pos[1] = 0xff0000;
      lift = 0;
      if (frame > 350) {
        u16 *pal = (u16 *)(PLTT + 0x200);

        for (i = 0; i != 255; i++) {
          s32 c = *pal;
          s32 r = 31;
          s32 g;
          s32 b;

          r &= c;
          g = (u16)c >> 5 & 31;
          b = (u16)c >> 10 & 31;

          if (r != 31)
            r++;
          if (g != 31)
            g++;
          if (b != 31)
            b++;
          *pal++ = b << 10 | g << 5 | r;
        }
      }
      if (frame <= 385) {
        if ((u32)(frame - 351) <= 14)
          lift = Trig_Sin((frame - 350) << 10) << 6;
        else if (frame > 365)
          lift = ((366 - frame) * 5 << 17) + 0x400000;
        if (frame <= 229)
          lift = (Trig_Sin((frame << 6) - 0x2c00) << 6) - 0x147200;
        for (j = 0; j != 4; j++) {
          for (column = 0; column != 4; column++) {
            pos[0] = 0x480000 + column * 0x200000;
            pos[2] = lift + 0x380000 + j * 0x200000;
            Render_ApplyProjectedPlacementFar(work->objects[j * 4 + column],
                                              pos, (s32 *)&scale, 0);
          }
        }
      }
      if (frame == 386) {
        for (i = 0; i != 16; i++)
          ResourceObject_ReleaseFar(work->objects[i]);
        BattleEffect_SelectLivingTargets(work->effect);
        Palette_LoadBasicObjColors();
        for (i = 0; i != work->effect->count; i++)
          Func_0814cd48(work->effect->actors[i], -1, 5, -1, 0);
      }
      if (frame <= 191) {
        for (i = 0; i != 3; i++) {
          s32 y = i + 62;
          u8 v = frame + (i & 1) * 47 + 80;
          s32 off;
          s32 x;
          s32 m;

          off = ((y & 7) << 3) + ((y >> 3) << 10);
          for (x = 0; x != 128; x += 8) {
            for (m = x; m != x + 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if ((u32)(frame - 177) <= 46) {
      for (i = 0; i != 36; i++) {
        shrink.x = shrink.y = 0x10000 - ((frame - 177) >> 1 << 11);
        pos[3] = 0;
        pos[1] = 0xff0000;
        pos[0] = sparks->x[i];
        pos[2] = sparks->y[i];
        Render_ApplyProjectedPlacementFar(objects[i], pos, (s32 *)&shrink, 0);
        sparks->x[i] += sparks->vx[i];
        sparks->y[i] += sparks->vy[i];
        if (!(frame & 3))
          sparks->frames[i]++;
        if (sparks->frames[i] > 14)
          sparks->frames[i] = 0;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
      }
    }
    if ((u32)(frame - 179) <= 38) {
      s32 radius = frame - 170;

      for (i = 0; i != 256; i++) {
        s32 a = i << 8;
        s32 px = Trig_Sin(a) * radius;
        s32 py = Trig_Cos(a) * radius;

        ((DrawRectangle)draw[0])(canvas, work->sheet + gData_08197410[3],
                                 (px >> 16) + 58, (py * 2 >> 16) + 48, 4, 8);
      }
      for (i = 0; i != 12; i++) {
        ((DrawRectangle)draw[0])(canvas, work->sheet + gData_08197410[1],
                                 (work->particles[i].x >> 16) - 2,
                                 (work->particles[i].y >> 16) - 2, 2, 4);
        work->particles[i].x += work->particles[i].velocityX;
      }
    }
    if ((u32)(frame - 179) <= 31)
      REG_BLDALPHA = (210 - frame) >> 1 | 0x1000;

    // The drops: their frames, the sparks' cells and the spray.
    if (frame == 230) {
      REG_BLDALPHA = 0x1010;
      REG_BG2CNT = 0x784;
      for (i = 1; i != 8; i++) {
        s32 next;

        for (j = 0; j != 10; j = next) {
          u8 *src = work->sheet + gData_08197410[j];
          u8 *out = work->sheet + (gData_08197410[j] + i * 0x302);
          s32 off = 0;

          next = j + 1;

          for (; off != (next << 1) * next; off++) {
            u8 v = *src - i * 8;

            if (v > 63)
              v = 0;
            *out = v;
            src++;
            out++;
          }
        }
      }
      for (i = 0; i != 8; i++) {
        Func_0815b510(work->sheet + 0x1b12 + i * 800, 20, 0x10000 - i * 0x2000);
        Func_0815b510(work->sheet + 0x4c12 + i * 288, 12, 0x10000 - i * 0x2000);
      }
      for (i = 0; i != 36; i++)
        sparks->life[i] = 0;
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
      for (i = 0; i != 32; i++)
        for (j = 0; j != 8; j++)
          sprites[508 + i * 8 + j] = Data_0819a038[j];
      dst = sprites;
      for (i = 0; i != 2; i++) {
        for (j = 0; j != 3; j++) {
          u8 *src = work->sheet + gData_08197410[Data_0819a040[i] - 1];

          for (m = 0; m != *(Data_0819a040 + i) * *(Data_0819a040 + i) * 2;
               m++) {
            s32 v = *src - j * 16;

            if (v < 0)
              v = 0;
            *dst = v;
            dst++;
            src++;
          }
        }
      }
      for (j = 0; j != 8; j++) {
        u8 *src = work->sheet + gData_08197410[1];

        for (m = 0; m != 8; m++) {
          s32 v = *src - j * 8;

          if (v < 0)
            v = 0;
          *dst = v;
          src++;
          dst++;
        }
      }
    }
    BattleEffect_LoadBlitter(SLOT_ALT, 3);
    if ((u32)(frame - 240) <= 127 && !(frame & 15)) {
      s32 v = (240 - frame) / 16;

      Func_08164b2c(v, v, v);
    }
    for (j = 0; j != 6; j++) {
      if (frame > Data_0819a048[j]) {
        if (frame < Data_0819a054[j]) {
          Iris_DrawSplash(frame - Data_0819a048[j], Data_0819a060[j],
                          Data_0819a066[j], Data_0819a06c[j]);
          if (Data_0819a048[j] - Data_0819a06c[j] * 16 + 48 == frame) {
            for (i = 0; i != 6; i++) {
              s32 s = FindFreeSpark();

              sparks->life[s] = 0x7900;
              sparks->x[s] = Data_0819a060[j] << 17;
              sparks->y[s] = (Data_0819a066[j] << 16) + 0x200000;
              sparks->vx[s] = ((Random16() & 31) - 16) << 12;
              sparks->vy[s] = -(Random16() & 31) << 12;
            }
          }
        } else if (frame < Data_0819a054[j] + Data_0819a078[j]) {
          s32 t = frame - Data_0819a054[j];

          Func_08192648(t / Data_0819a07e[j], Data_0819a060[j],
                        Data_0819a066[j] - Data_0819a072[j] * t / 2,
                        Data_0819a06c[j]);
        }
      }
    }
    if ((u32)(frame - 279) <= 132 || frame > 446)
      UpdateSparks();
    HeapSlot_Release(SLOT_ALT);

    // The pillars of light.
    if (frame == 370) {
      Func_0815b434(work->sheet + 0x3e8, 16, 0x10000);
      for (i = 0; i != 16; i++) {
        Dma_Set(work->sheet + 0x3e8 + (i << 3), work->sheet + 0x302 + (i << 4),
                0x80000008, REG_DMA3);
        for (j = 0; j != 8; j++)
          work->sheet[0x30a + i * 16 + j] = work->sheet[0x3ef + i * 8 - j];
      }
      REG_BLDALPHA = 0x1008;
    }
    if ((u32)(frame - 414) <= 7) {
      /* FAKEMATCH(temporary): the blend level copied into alpha; as
       * REG_BLDALPHA = (frame - 406) | 0x1000, 14 of 4851 instructions
       * differ in tla-en. */
      s32 alpha = frame - 406;
      REG_BLDALPHA = alpha | 0x1000;
    }
    if (frame > 385)
      gCameraSceneParameters[4] = 2556 - frame * 6;
    if (frame <= 422) {
      if (frame > 370) {
        s32 h = (frame - 370) << 3;

        if (h > 127)
          h = 127;
        Func_0819273c(3, 34, h);
      }
      if (frame > 375) {
        struct Widths5 widths = Data_08196f50;
        s32 h = (frame - 375) << 3;

        if (h > 127)
          h = 127;
        Func_0819273c(widths.w[h >> 5], (h >> 5) * 2 + 94, h);
      }
      if (frame > 380) {
        struct Widths7 widths = Data_08196f64;
        s32 h = (frame - 380) << 2;
        s32 q;

        if (h > 127)
          h = 127;
        q = h / 19;
        Func_0819273c(widths.w[q], q + 12, h);
      }
      if (frame > 382) {
        struct Widths7 widths = Data_08196f80;
        s32 h = (frame - 382) << 3;
        s32 q;

        if (h > 127)
          h = 127;
        q = h / 21;
        Func_0819273c(widths.w[q], q * 2 + 70, h);
      }
      if (frame > 385) {
        s32 h = (frame - 385) << 2;

        if (h > 127)
          h = 127;
        Func_0819273c(14, (h >> 3) + 48, h);
      }
      if (frame == 422) {
        work->transferMode = 0;
        work->transferValue = 0;
        REG_BLDALPHA = 0x1010;
      }
    }
    if ((u32)(frame - 423) <= 19) {
      s32 step = frame - 422;
      s32 first;
      s32 dead;
      s32 top;
      s32 off;
      s32 k;
      s32 m;
      u8 v;

      top = 120;
      first = top - step * 8;
      /* FAKEMATCH(dead): off set to 0, which nothing reads before the loop sets
       * it; without it, 168 of 4851 instructions differ in tla-en. */
      off = 0;
      /* FAKEMATCH(temporary): the band's first row passed to i through first;
       * as i = top - step * 8, 168 of 4851 instructions differ in tla-en. */
      i = first;
      top = i;
      for (; i != top + 40 && (v = (i - top) * 2, i <= 127); i++) {
        if (i >= 0) {
          /* FAKEMATCH(dead): i / 3 into dead, which flow deletes; its call puts
           * the band's loop over loop.c's threshold, which then keeps
           * (i - top) * 2; without it, 191 of 4851 instructions differ in
           * tla-en. */
          dead = i / 3;
          if (v > 63)
            v = 63;
          off = ((i & 7) << 3) + ((i >> 3) << 10);
          for (k = 0; k != 128; k += 8) {
            for (m = 0; m != 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if ((u32)(frame - 401) <= 30 && (frame & 1)) {
      Resource_GetTableEntry((s32)&ResourceId_VioletSkyBackdrop);
      {
        u16 *fade = FADE_PLTT;

        for (i = 0; i != 128; i++) {
          s32 c = *fade;
          s32 r = 31;
          s32 g;
          s32 b;

          r &= c;
          g = (u16)c >> 5 & 31;
          b = (u16)c >> 10 & 31;

          if (r > 3)
            r -= 2;
          if (g > 3)
            g -= 2;
          if (b > 3)
            b -= 2;
          *fade++ = b << 10 | g << 5 | r;
        }
      }
    }

    // The fountain: the second backdrop, the motes and the light shafts.
    if (frame == 446) {
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      Render_ResetTransformState();
      BattleBackground_LoadFar(1, (s32)&ResourceId_StarfieldFlareBackdrop, 0);
      off = 0x302;
      {
        u16 *fade = FADE_PLTT;
        s32 red = 31;

        for (i = 0; i != 128; i++) {
          s32 c = *fade;
          s32 r = red & c;
          s32 g = (u16)c >> 5 & 31;
          s32 b = (u16)c >> 10 & 31;

          g += 1;
          *fade++ = (b + 1) << 10 | g << 5 | (r + 1);
        }
      }
      Graphics_LoadGreyRamp();
      for (i = 0; i != 6; i += 2) {
        s32 n = i + 14;
        s32 w = n * 2;

        Func_0815b434(work->sheet + off, n, 0x10000);
        off += n * w;
      }
      Iwram_FillWords(work->sheet + 0x1810, 0x1000, 0);
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x2010, 16, 64,
                                           0x10000);
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x2810, 16, 64,
                                           0x10000);
      {
        u8 *ramp = work->sheet + 0x2010;

        for (i = 0; i != 64; i++) {
          for (j = 0; j != 32; j++) {
            if (*ramp > i)
              *ramp = i;
            ramp++;
          }
        }
      }
      for (i = 0; i != 36; i++)
        sparks->life[i] = 0;
      for (i = 0; i != 32; i++)
        work->particles[i].variant = 0;
      for (i = 1; i != 8; i++) {
        s32 next;

        for (j = 0; j != 10; j = next) {
          u8 *src = work->sheet + gData_08197410[j];
          u8 *out = work->sheet + (gData_08197410[j] + i * 0x302);
          s32 off = 0;

          next = j + 1;

          for (; off != (next << 1) * next; off++) {
            u8 v = *src - i * 8;

            if (v > 63)
              v = 0;
            *out = v;
            src++;
            out++;
          }
        }
      }
      work->transferMode = 2;
      work->transferValue = 70;
    }
    if ((u32)(frame - 447) <= 15) {
      s32 step = frame - 446;
      s32 top;
      s32 off;
      s32 x;
      s32 m;

      top = 136 - step * 8;
      for (i = top; i != 0; i--) {
        u8 v = (top - i) * 2;

        if (i <= 127) {
          if (v > 63)
            v = 63;
          if (top - 8 < i)
            v = 0;
          off = ((i & 7) << 3) + ((i >> 3) << 10);
          for (x = 0; x != 128; x += 8) {
            for (m = 0; m != 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if (frame > 446)
      DrawParticles();
    if ((u32)(frame - 447) <= 194) {
      void *verts = Heap_AllocEwram(80);
      struct RenderModel *model = Render_AllocateModelList(1);
      s32 scroll;
      struct Spans4 spans;
      struct Spans4 spans2;

      texture.logWidth = 5;
      texture.logHeight = 6;
      SceneTransform_ResetMatrix();
      model->kind = 4;
      model->mesh = Data_0819a0d4;
      model->vertices = verts;
      if (frame <= 563) {
        scroll = 0x1000;
        if (frame > 500) {
          scroll = (s16)(scroll - (frame - 500) * 64);
          if (scroll < 0)
            scroll = 0;
        }
        texture.pixels = work->sheet + 0x1810 + scroll;
        if (!(frame & 7)) {
          s32 s;

          spans = Data_08196f9c;
          s = FindFreeSpark();

          if (s != -1) {
            s32 r = Random16() & 127;

            sparks->life[s] = -0x7c00;
            sparks->x[s] =
                (spans.p[(r + 1) >> 5].a + Random16() % spans.p[(r + 1) >> 5].b)
                << 16;
            sparks->y[s] = (r << 16) + 0x100000;
            sparks->vx[s] = (Random16() & 15) * 3 << 11;
            sparks->vy[s] = -0x8000 - ((Random16() & 31) * 3 << 11);
          }
        }
        model->texture = &texture;
        Render_TransformVertices(Data_0819a084, verts, 10);
        Render_DrawModelList(model);
      }
      if (frame <= 563) {
        scroll = 0x1000;
        if (frame > 470) {
          scroll = (s16)(scroll - (frame - 470) * 64);
          if (scroll < 0)
            scroll = 0;
        }
        texture.pixels = work->sheet + 0x1810 + scroll;
        if (!(frame & 7) && frame <= 501) {
          s32 s;

          spans2 = Data_08196fbc;
          s = FindFreeSpark();

          if (s != -1) {
            s32 r = Random16() & 127;

            sparks->life[s] = -0x7c00;
            sparks->x[s] = (spans2.p[(r + 1) >> 5].a +
                            Random16() % spans2.p[(r + 1) >> 5].b)
                           << 16;
            sparks->y[s] = (r << 16) + 0x100000;
            sparks->vx[s] = (Random16() & 15) * 3 << 11;
            sparks->vy[s] = -0x8000 - ((Random16() & 31) * 3 << 11);
          }
        }
        if (!(frame & 3)) {
          s32 n = FindFreeParticle();

          if (n != -1) {
            work->particles[n].x = 0x200000;
            if (frame <= 509)
              work->particles[n].x += (Random16() & 63) << 16;
            else
              work->particles[n].x += (Random16() & motesMask) << 16;
            work->particles[n].y = 0x7f0000;
            work->particles[n].velocityX = (Random16() & 15) << 12;
            work->particles[n].velocityY =
                -0x8000 - ((Random16() & 31) * 7 << 11);
            work->particles[n].variant = (Random16() & 3) + 1;
          }
        }
        model->texture = &texture;
        Render_TransformVertices(Data_0819a0ac, verts, 10);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    // The five light bars sweeping in from the edges.
    if ((u32)(frame - 513) <= 209) {
      s16 shade = 723 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(0, 127, frame - 512, shade);
    }
    if ((u32)(frame - 522) <= 209) {
      s16 shade = 732 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(
          0, (Iwram_MulQ16((frame - 521) * 0x10000, 0x400) + 0x7b0000) >> 16,
          frame - 521, shade);
    }
    if ((u32)(frame - 531) <= 209) {
      s32 x = 0x40000;
      s16 shade = 741 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      x -= Iwram_MulQ16((frame - 530) * 0x10000, 0x400);
      BattleEffect_DrawDiagonalCanvasLine(x >> 16, 127, frame - 530, shade);
    }
    if ((u32)(frame - 535) <= 209) {
      s16 shade = 745 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(
          0, (Iwram_MulQ16((frame - 534) * 0x10000, 0x600) + 0x780000) >> 16,
          frame - 534, shade);
    }
    if ((u32)(frame - 554) <= 209) {
      s32 x = 0x60000;
      s16 shade = 764 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      x -= Iwram_MulQ16((frame - 553) * 0x10000, 0x600);
      BattleEffect_DrawDiagonalCanvasLine(x >> 16, 127, frame - 553, shade);
    }

    // The healing: rings turning about the centre, then the great glow.
    if ((u32)(frame - 639) <= 110) {
      s32 sinT = Trig_Sin(0x7800);
      s32 cosT = Trig_Cos(0x7800);

      for (j = 0; j != 80; j += 8) {
        if (638 + j < frame && frame < 670 + j) {
          s32 t = frame - j - 638;
          s32 ry = t >> 1;
          s32 tile = gData_08197410[0] + t / 4 * 0x302;

          for (i = 0; i != 128; i++) {
            s32 a = i << 9;
            s32 sx = Trig_Sin(a) * (ry >> 1);
            s32 sy = Trig_Cos(a) * ry;
            s32 px = Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy * 2, sinT);
            s32 py = Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy * 2, cosT);
            s32 py2;

            px = (px >> 16) + (j >> 1) + 64;
            py2 = (py >> 16) - (j >> 1) + 63;
            ((DrawRectangle)draw[0])(canvas, work->sheet + tile, px, py2, 1, 2);
          }
        }
      }
    }
    if ((u32)(frame - 755) <= 2 || frame == 781)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    if (frame == 754) {
      s32 white;
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x5810, 16, 32,
                                           0x10000);
      Func_0815b434(work->sheet + 0x1c10, 32, 0x10000);
      for (i = 0; i != 32; i++) {
        Dma_Set(work->sheet + (i << 4) + 0x1c10,
                work->sheet + (i << 5) + 0x1810, 0x84000008, REG_DMA3);
        for (j = 0; j != 16; j++)
          work->sheet[0x1820 + i * 32 + j] = work->sheet[0x181f + i * 32 - j];
      }
      for (i = 0; i != 32; i++)
        Dma_Set(work->sheet + 0x1bf0 - (i << 5),
                work->sheet + 0x1c10 + (i << 5), 0x84000010, REG_DMA3);
      white = 64;
      for (i = 1; i != 8; i++) {
        u8 *src = work->sheet + 0x1810;
        u8 *out = work->sheet + 0x1810 + i * 0x800;
        u8 v;

        j = 0;
        v = white - i * 8;
        for (; j != 0x800; j++) {
          if (v < *src)
            *out = v;
          else
            *out = *src;
          out++;
          src++;
        }
      }
    }
    if ((u32)(frame - 759) <= 30) {
      s32 sinT = Trig_Sin(0x5800);
      s32 cosT = Trig_Cos(0x5800);
      s32 t = frame - 758;
      s32 tile = gData_08197410[5] + t / 4 * 0x302;

      for (i = 0; i != 96; i++) {
        s32 a = i * 384;
        s32 sx = Trig_Sin(a) * (t << 1);
        s32 sy = Trig_Cos(a) * t;
        s32 px = (Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy, sinT)) >> 16;
        s32 py = (Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy, cosT)) >> 16;

        ((DrawRectangle)draw[0])(canvas, work->sheet + tile, px + 101, py + 26,
                                 6, 12);
      }
    }
    if ((u32)(frame - 783) <= 46) {
      s32 sinT = Trig_Sin(0xe800);
      s32 cosT = Trig_Cos(0xe000);
      s32 t = frame - 782;
      s32 tile = gData_08197410[5] + t / 6 * 0x302;

      for (i = 0; i != 197; i++) {
        s32 a = i * 384;
        s32 sx = Trig_Sin(a) * (t >> 1);
        s32 sy = Trig_Cos(a) * (t << 1);
        s32 px = (Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy, sinT)) >> 16;
        s32 py = (Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy, cosT)) >> 16;

        ((DrawRectangle)draw[0])(canvas, work->sheet + tile, px + 93, py + 32,
                                 6, 12);
      }
    }
    if (frame > 758) {
      void *verts = Heap_AllocEwram(64);
      struct RenderModel *model = Render_AllocateModelList(1);
      s32 place[3];
      s32 grow[3];
      s32 s;

      texture2.logWidth = 5;
      texture2.logHeight = 5;
      texture2.pixels = work->sheet + 0x5810;
      SceneTransform_ResetMatrix();
      place[0] = 0x200000;
      place[1] = -0x180000;
      place[2] = 0;
      SceneTransform_ApplyPosition(place);
      Func_080150e4(0x9800);
      grow[0] = 0x10000;
      grow[1] = (frame - 758) << 10;
      if (grow[1] > 0x10000)
        grow[1] = 0x10000;
      grow[2] = 0;
      Func_080151ac(grow);
      model->kind = 4;
      model->mesh = Data_0819a160;
      model->texture = &texture2;
      model->vertices = verts;
      Render_TransformVertices(Data_0819a140, verts, 8);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
      s = FindFreeSpark();
      if (s != -1) {
        sparks->life[s] = (Random16() & 15) << 15;
        sparks->x[s] = 0xef0000;
        sparks->y[s] = (Random16() & 255) << 16;
        sparks->vx[s] = -(Random16() & 31) << 12;
        sparks->vy[s] = ((Random16() & 31) - 16) << 12;
      }
      if (!(frame & 3)) {
        s32 n = FindFreeParticle();

        if (n != -1) {
          s32 spin = Random16() & 3;

          work->particles[n].x = 0x600000;
          work->particles[n].y = ((Random16() & 7) << 16) + 0x240000;
          work->particles[n].z = frame;
          work->particles[n].velocityX =
              Iwram_MulQ16((spin + 2) << 15, Trig_Sin(0x9800));
          work->particles[n].velocityY =
              -Iwram_MulQ16((spin + 2) << 15, Trig_Cos(0x9800));
          work->particles[n].variant = (Random16() & 3) | 16;
        }
        n = FindFreeParticle();
        if (n != -1) {
          work->particles[n].x = ((Random16() & 31) << 16) + 0x600000;
          work->particles[n].y = (Random16() & 31) << 16;
          work->particles[n].z = frame;
          work->particles[n].velocityX = -((Random16() & 15) << 13);
          work->particles[n].velocityY = (Random16() & 15) << 14;
          work->particles[n].velocityZ = 0x8000;
          work->particles[n].variant = ((Random16() & 3) + 1) | 0x110;
        }
      }
    }
    if (frame > 782) {
      if (frame & 2)
        gBgScroll[1].y = gBgScroll[1].y + (frame & 1) + 2;
      else
        gBgScroll[1].y = gBgScroll[1].y - (frame & 1) - 2;
    }
    if (frame > 816) {
      {
        u16 *fade = FADE_PLTT;

        for (i = 0; i != 128; i++) {
          s32 c = *fade;
          s32 b = (u16)c >> 10 & 31;
          s32 g = (u16)c >> 5 & 31;
          s32 r = c & 31;

          if (r <= 30)
            r++;
          if (g <= 30)
            g++;
          if (b <= 30)
            b++;
          {
            /* FAKEMATCH(temporary): the colour copied into a u16 before its
             * store; as *fade++ = b << 10 | g << 5 | r, 171 of 4851
             * instructions differ in tla-en. */
            u16 v = b << 10 | g << 5 | r;

            *fade++ = v;
          }
        }
      }
    }
    frame++;
    work->transferPending = 1;
    wait(1);
  }

  camera[27] = savedZoom;
  HeapSlot_Release(SLOT_BLITTER);
  Func_08191cc4();
  Iwram_FillWords(canvas, 0x4000, 0);
  while (((volatile struct DmaRegisters *)REG_DMA3)->control & 0x80000000) {
  }
  work->transferPending = 1;
  wait(1);
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  gBgScroll[1].x = savedScroll;
  for (i = 0; i != 36; i++)
    ResourceObject_ReleaseFar(objects[i]);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Swaps two integers without a temporary.
#define XOR_SWAP(a, b)                                                         \
  {                                                                            \
    (a) ^= (b);                                                                \
    (b) ^= (a);                                                                \
    (a) ^= (b);                                                                \
  }

// A triangle of the model: where its three corners start in the vertex
// buffer (in words) and its colour.
struct SolidFace {
  s32 corner[3];
  u8 unk0c[12];
  u8 color;
  u8 unk19[3];
};

// Fills one scanline of the canvas (8x8 tiles, 16 to a row) from xa to xb,
// keeping the brighter of each pixel's colour and color.
#define FILL_SPAN(row, xa, xb)                                                 \
  {                                                                            \
    s32 left = (xa) >> 16;                                                     \
    s32 right = (xb) >> 16;                                                    \
    s32 rowOffset;                                                             \
    s32 x;                                                                     \
                                                                               \
    if (left > right)                                                          \
      XOR_SWAP(left, right);                                                   \
    rowOffset = (((row) & 7) << 3) + (((row) >> 3) << 10);                     \
    for (x = left; x != right; x++) {                                          \
      s32 pixel = ((x >> 3) << 6) + (x & 7) + rowOffset;                       \
                                                                               \
      if (canvas[pixel] < color)                                               \
        canvas[pixel] = color;                                                 \
    }                                                                          \
  }

// An unused solid renderer, the filled twin of Func_08196248. Transforms the
// vertexCount points in the sheet block by the current matrix, then fills
// every face of the effect work's list that faces the camera into the
// canvas, centred at (64, 64), keeping the brighter colour where faces
// overlap. Each triangle is sorted by y and filled in two halves, top to
// middle and middle to bottom, along 16.16 edge slopes.
void Func_0819574c(s32 vertexCount, s32 faceCount) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 *vertices = Ram_HeapSlots->unk64[0];
  struct SolidFace *faces = (struct SolidFace *)Ram_HeapSlots->battleFxWork[0];
  s32 i;
  s32 *ax, *bx, *cx, *cy, *ay, *by;
  s32 facing;
  s32 (*mul)(s32, s32);
  s32 slopeA, slopeB, top, middle, bottom;
  u8 color;
  s32 edgeA, edgeB, row;

  // From here on, the count of words: three to a point.
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  for (i = 0; i != faceCount; i++) {
    // The three corners of each face are copied after the vertices.
    ax = (s32 *)(vertexCount * 4 + (s32)vertices);
    bx = ax + 3;
    cx = ax + 6;
    cy = ax + 7;
    ay = ax + 1;
    by = ax + 4;
    // A local holding the multiply routine's address (one value throughout).
    mul = Iwram_MulQ16;
    Dma_Set(&vertices[faces[i].corner[0]], ax, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], bx, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], cx, 0x84000003, REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing = mul(*bx - *ax, *cy - *ay) - mul(*cx - *ax, *by - *ay);
    if (facing >= 0) {
      color = faces[i].color;
      *ax += 64 << 16;
      *ay += 64 << 16;
      *bx += 64 << 16;
      *by += 64 << 16;
      *cx += 64 << 16;
      *cy += 64 << 16;

      // Sort the corners by y. bx[1] is *by and cx[1] is *cy: the y swaps
      // store b's and c's y through their x pointers, as the native does.
      if (*ay > *by) {
        XOR_SWAP(*ax, *bx);
        *ay ^= *by;
        bx[1] = *by ^ *ay;
        *ay ^= bx[1];
      }
      if (*by > *cy) {
        XOR_SWAP(*bx, *cx);
        bx[1] = *by ^ *cy;
        cx[1] = *cy ^ bx[1];
        *by ^= cx[1];
        if (*ay > *by) {
          XOR_SWAP(*ax, *bx);
          *ay ^= *by;
          bx[1] = *by ^ *ay;
          *ay ^= bx[1];
        }
      }

      top = ax[1] >> 16;
      middle = ax[4] >> 16;
      bottom = HI(ax[7]);
      if (top != middle)
        slopeA = Iwram_RatioMulQ14(ax[4] - ax[1], ax[3] - ax[0]);
      if (top != bottom)
        slopeB = Iwram_RatioMulQ14(ax[7] - ax[1], ax[6] - ax[0]);

      // The upper half, between the short edge and the long one.
      edgeB = ax[0];
      edgeA = ax[0];
      for (row = top; row != middle; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }

      // The lower half, from the middle corner.
      if (middle != bottom)
        slopeA = Iwram_RatioMulQ14(ax[7] - ax[4], ax[6] - ax[3]);
      edgeA = ax[3];
      for (row = middle; row != bottom; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }
    }
  }
}

// One mesh face: its three corners as word offsets into the vertex buffer
// (vertex index * 3), padded to an effect step's 28 bytes.
struct MeshFace {
  s32 vertex[3];
  s32 unk0c[4];
};

#define DIFF_ABS(v) ((v) >= 0 ? (v) : -(v))

// One textured scanline at canvas row y, from xA to xB, keeping the brighter
// of the canvas and texture colour.
#define DRAW_SPAN(y)                                                           \
  {                                                                            \
    u8 *row = work->sheet;                                                     \
    s32 left, right, u, v;                                                     \
    s32 rowOffset;                                                             \
    s32 x;                                                                     \
                                                                               \
    row += ((vA >> 16) << textureShift) + (uA >> 16);                          \
    left = xA >> 16;                                                           \
    right = xB >> 16;                                                          \
    u = 0;                                                                     \
    v = 0;                                                                     \
    if (left > right)                                                          \
      XOR_SWAP(left, right);                                                   \
    rowOffset = (((y) & 7) << 3) + (((y) >> 3) << 10);                         \
    for (x = left; x != right; x++) {                                          \
      s32 texel = (u >> 16) + ((v >> 16) << textureShift);                     \
      s32 at = ((x >> 3) << 6) + (x & 7) + rowOffset;                          \
                                                                               \
      if (canvas[at] < row[texel])                                             \
        canvas[at] = row[texel];                                               \
      u += dudx;                                                               \
      v += dvdx;                                                               \
    }                                                                          \
  }

// The texture steps along a scanline, from the middle corner's row.
#define STEP_RIGHTWARD()                                                       \
  {                                                                            \
    width = DIFF_ABS(ax[3] - ax[0] - Iwram_MulQ16(dxB, ax[4] - ax[1]));        \
    dudx = Iwram_RatioMulQ14(width, uv[1][0] - uv[0][0] -                      \
                                        Iwram_MulQ16(duB, ax[4] - ax[1]));     \
    dvdx = Iwram_RatioMulQ14(width, uv[1][1] - uv[0][1] -                      \
                                        Iwram_MulQ16(dvB, ax[4] - ax[1]));     \
    XOR_SWAP(duA, duB);                                                        \
    XOR_SWAP(dvA, dvB);                                                        \
  }
#define STEP_LEFTWARD()                                                        \
  {                                                                            \
    width = DIFF_ABS(Iwram_MulQ16(dxB, ax[4] - ax[1]) - (ax[3] - ax[0]));      \
    dudx = Iwram_RatioMulQ14(width, Iwram_MulQ16(duB, ax[4] - ax[1]) -         \
                                        (uv[1][0] - uv[0][0]));                \
    dvdx = Iwram_RatioMulQ14(width, Iwram_MulQ16(dvB, ax[4] - ax[1]) -         \
                                        (uv[1][1] - uv[0][1]));                \
  }

// Transforms vertexCount vertices (16.16 x, y, z in the heap's vertex
// buffer) in place, then draws each front-facing face of the mesh at the
// start of the effect work as an affine-textured triangle: uvs gives each
// face's three texture corners (16.16), the texture is the effect sheet,
// textureShift its row length as a power of two. Coordinates are offset by
// 64 pixels into the canvas.
void Func_08195a9c(s32 vertexCount, s32 faceCount, s32 (*uvs)[3][2],
                   s32 textureShift) {
  s32 i;
  s32 *vertices;
  u8 *canvas;
  struct MeshFace *faces;
  s32 *ax, *bx, *cx, *cy, *ay, *by;
  s32 uv[3][2];
  s32 width;
  s32 dxA, dxB, duA, dvA, duB, dvB;
  s32 xA, xB, uA, vA, uB, vB;
  s32 top, middle, bottom;
  s32 dudx, dvdx;
  struct BattleEffectWork *work;
  u32 y;

  vertices = Ram_HeapSlots->unk64[0];
  faces = (struct MeshFace *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  for (i = 0; i != faceCount; i++) {
    // As an integer sum, so the scaled count is added first.
    ax = (s32 *)(vertexCount * 4 + (s32)vertices);
    bx = &vertices[vertexCount + 3];
    cx = &vertices[vertexCount + 6];
    cy = &vertices[vertexCount + 7];
    ay = &vertices[vertexCount + 1];
    by = &vertices[vertexCount + 4];
    Dma_Set(&vertices[faces[i].vertex[0]], ax,
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);
    Dma_Set(&vertices[faces[i].vertex[1]], bx,
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);
    Dma_Set(&vertices[faces[i].vertex[2]], cx,
            (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);

    // Back faces are skipped.
    if (Iwram_MulQ16(*bx - *ax, *cy - *ay) -
            Iwram_MulQ16(*cx - *ax, *by - *ay) >=
        0) {
      work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
      Dma_Set(uvs[i], uv, (DMA_ENABLE | DMA_32BIT) << 16 | 6, REG_DMA3);
      *ax += 64 << 16;
      *ay += 64 << 16;
      *bx += 64 << 16;
      *by += 64 << 16;
      *cx += 64 << 16;
      *cy += 64 << 16;

      // Sort the corners from top to bottom. bx[1] is *by and cx[1] is *cy.
      if (*ay > *by) {
        XOR_SWAP(*ax, *bx);
        *ay ^= *by;
        bx[1] = *by ^ *ay;
        *ay ^= bx[1];
        XOR_SWAP(uv[0][0], uv[1][0]);
        XOR_SWAP(uv[0][1], uv[1][1]);
      }
      if (*by > *cy) {
        XOR_SWAP(*bx, *cx);
        bx[1] = *by ^ *cy;
        cx[1] = *cy ^ bx[1];
        *by ^= cx[1];
        XOR_SWAP(uv[1][0], uv[2][0]);
        XOR_SWAP(uv[1][1], uv[2][1]);
        if (*ay > *by) {
          XOR_SWAP(*ax, *bx);
          *ay ^= *by;
          bx[1] = *by ^ *ay;
          *ay ^= bx[1];
          XOR_SWAP(uv[0][0], uv[1][0]);
          XOR_SWAP(uv[0][1], uv[1][1]);
        }
      }

      top = ax[1] >> 16;
      middle = ax[4] >> 16;
      bottom = ax[7] >> 16;
      if (top != middle) {
        s32 height = ax[4] - ax[1];

        dxA = Iwram_RatioMulQ14(height, ax[3] - ax[0]);
        duA = Iwram_RatioMulQ14(height, uv[1][0] - uv[0][0]);
        dvA = Iwram_RatioMulQ14(height, uv[1][1] - uv[0][1]);
      }
      if (top != bottom) {
        s32 height = ax[7] - ax[1];

        dxB = Iwram_RatioMulQ14(height, ax[6] - ax[0]);
        duB = Iwram_RatioMulQ14(height, uv[2][0] - uv[0][0]);
        dvB = Iwram_RatioMulQ14(height, uv[2][1] - uv[0][1]);
      }
      xB = xA = ax[0];
      uA = uB = uv[0][0];
      vA = vB = uv[0][1];

      if (ax[3] >= ax[6])
        STEP_RIGHTWARD()
      else
        STEP_LEFTWARD()

      // The upper half.
      for (y = top; y != middle; y++) {
        DRAW_SPAN(y);
        xA += dxA;
        xB += dxB;
        uA += duA;
        vA += dvA;
        uB += duB;
        vB += dvB;
      }

      // The lower half: edge A now runs from the middle corner to the bottom.
      if (middle != bottom) {
        s32 height = ax[7] - ax[4];

        dxA = Iwram_RatioMulQ14(height, ax[6] - ax[3]);
        if (ax[3] >= ax[6]) {
          duB = duA;
          dvB = dvA;
          uB = uA;
          vB = vA;
        }
        duA = Iwram_RatioMulQ14(height, uv[2][0] - uv[1][0]);
        dvA = Iwram_RatioMulQ14(height, uv[2][1] - uv[1][1]);
        xA = ax[3];
        uA = uv[1][0];
        vA = uv[1][1];
        if (ax[0] <= xA) {
          STEP_RIGHTWARD();
          uA = uB;
          vA = vB;
        } else
          STEP_LEFTWARD()
        for (y = middle; y != bottom; y++) {
          DRAW_SPAN(y);
          xA += dxA;
          xB += dxB;
          uA += duA;
          vA += dvA;
          /* FAKEMATCH(dead): edge B's values stepped on though nothing reads
           * them again; flow deletes both, and cse2 then stores uv[0] to uB
           * ahead of uA; without them, 4 of 966 instructions differ in
           * tla-en. */
          uB += duB;
          vB += dvB;
        }
      }
    }
  }
}

// A point of the wireframe model, 16.16 fixed point.
struct WirePoint {
  s32 x;
  s32 y;
  s32 z;
};

// A triangle of the wireframe model: where its three corners start in the
// vertex buffer (in words) and the colour its edges are drawn in.
struct WireFace {
  s32 corner[3];
  u8 unk0c[12];
  u8 color;
  u8 unk19[3];
};

// An unused wireframe renderer. Transforms the vertexCount points in the
// sheet block by the current matrix, then draws the three edges of every
// face of the effect work's list that faces the camera (its corners run
// counter-clockwise on screen) as clipped lines into the canvas, centred at
// (64, 64).
void Func_08196248(s32 vertexCount, s32 faceCount) {
  s32 *vertices = Ram_HeapSlots->unk64[0];
  struct WireFace *faces = (struct WireFace *)Ram_HeapSlots->battleFxWork[0];
  s32 i;
  s32 facing;
  s32 color;

  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  // The three corners of each face are copied after the vertices.
  for (i = 0; i != faceCount; i++) {
    // Spelled through integers: the index comes first in the addition.
    s32 *x0 = (s32 *)(vertexCount * 4 + (s32)vertices);
    s32 *x1 = x0 + 3;
    s32 *x2 = x0 + 6;
    s32 *y2 = x0 + 7;
    s32 *y0 = x0 + 1;
    s32 *y1 = x0 + 4;

    Dma_Set(&vertices[faces[i].corner[0]], x0, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], x1, 0x84000003, REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], x2, 0x84000003, REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing =
        Iwram_MulQ16(*x1 - *x0, *y2 - *y0) - Iwram_MulQ16(*x2 - *x0, *y1 - *y0);
    if (facing >= 0) {
      color = faces[i].color;
      *x0 += 64 << 16;
      *y0 += 64 << 16;
      *x1 += 64 << 16;
      *y1 += 64 << 16;
      *x2 += 64 << 16;
      *y2 += 64 << 16;
      Func_08143eb4(HI(*x0), HI(*y0), HI(*x1), HI(*y1), color);
      Func_08143eb4(HI(*x1), HI(*y1), HI(*x2), HI(*y2), color);
      Func_08143eb4(HI(*x0), HI(*y0), HI(*x2), HI(*y2), color);
    }
  }
}

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

/// BattleEffect_BuildBlitter(slot, 7, 7, flags, mode) for a 128 by 128 canvas,
/// with the flags and the mode packed into one word: mode << 4 | flags.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags) {
  return Func_08196404(slot, 7, 7, modeAndFlags & 0xf, (u32)modeAndFlags >> 4);
}

// Func_08196404 walks The Lost Age's blitter template once and writes the
// blitter's ARM code into a heap slot: The Broken Seal's step list
// (battle_effect/blitter.c) with short copies as block moves and longer ones
// by DMA, modes 0 additive, 1 max, 2 transparent, default copy, and the
// second run copied from the first by its measured length.

CAMELOT_ASM("ldmia and stmia of two registers from r0 to r1 with scratch from "
            "r2 up, in the shape of Dma_Set: agscc's block moves leave r0 and "
            "r1 to the allocator")
static __inline__ void CopyWords2(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3}\n\tstmia %1!, {r2, r3}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "memory");
}

CAMELOT_ASM("ldmia and stmia of three registers from r0 to r1 with scratch "
            "from r2 up, in the shape of Dma_Set: agscc's block moves leave r0 "
            "and r1 to the allocator")
static __inline__ void CopyWords3(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3, r4}\n\tstmia %1!, {r2, r3, r4}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "r4", "memory");
}

CAMELOT_ASM("ldmia and stmia of four registers: agscc's block moves stop at "
            "three, so a 16-byte struct copy compiles to 12 and 4")
static __inline__ void CopyWords4(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "r4", "r5", "memory");
}

extern const u32 BattleEffect_PatchTemplateCode[];
extern const u32 BattleEffect_Mask[];
extern const u16 Data_081973f0[];
extern const u32 BattleEffect_CopyReverse[];
extern const u32 BattleEffect_CopyForward[];
extern const u32 BattleEffect_Copy8Reverse[];
extern const u32 BattleEffect_Copy8Forward[];
extern const u32 BattleEffect_TransparentReverse[];
extern const u32 BattleEffect_TransparentForward[];
extern const u32 BattleEffect_Transparent8Reverse[];
extern const u32 BattleEffect_Transparent8Forward[];
extern const u32 BattleEffect_MaxReverse[];
extern const u32 BattleEffect_MaxForward[];
extern const u32 BattleEffect_Max4Reverse[];
extern const u32 BattleEffect_Max4Forward[];
extern const u32 BattleEffect_AdditiveReverse[];
extern const u32 BattleEffect_AdditiveForward[];
extern const u32 BattleEffect_ColorReverse[];
extern const u32 BattleEffect_ColorForward[];

#define BRANCH_FIELD(site, target)                                             \
  ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

#define PUT(word)                                                              \
  do {                                                                         \
    *dst++ = (word);                                                           \
  } while (0)
#define COPY_FROM(from, n)                                                     \
  CopyWords##n(from, dst);                                                     \
  dst += (n)
#define COPY(n)                                                                \
  do {                                                                         \
    COPY_FROM(src, n);                                                         \
    src += (n);                                                                \
  } while (0)
#define COPY_LONG(n)                                                           \
  do {                                                                         \
    Dma_Set(src, dst, control | (n), REG_DMA3);                                \
    dst += (n);                                                                \
    src += (n);                                                                \
  } while (0)
#define COPY_DMA(from, n)                                                      \
  Dma_Set(from, dst, DMA_COPY_WORDS(n), REG_DMA3);                             \
  dst += (n)
#define COPY_TWICE(from, n)                                                    \
  {                                                                            \
    const u32 *part = (from);                                                  \
    u32 control = DMA_COPY_WORDS(n);                                           \
    Dma_Set(part, dst, control, REG_DMA3);                                     \
    dst += (n);                                                                \
    Dma_Set(part, dst, control, REG_DMA3);                                     \
    dst += (n);                                                                \
  }
#define PUT_BRANCH(target)                                                     \
  *dst = *src++ + BRANCH_FIELD(dst, target);                                   \
  dst++
#define PATCH(site)                                                            \
  do {                                                                         \
    *(site) |= BRANCH_FIELD(site, dst);                                        \
  } while (0)

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode) {
  const u32 *src;
  u32 *dst;
  u32 *noRows;
  u32 *noColumns;
  u32 *row;
  u32 *skip;
  u32 *run;
  s32 runSize;
  u32 *runCopy;
  u32 control;
  // Unused: gives the stack frame its 136 bytes.
  u32 frame[34];

  src = BattleEffect_PatchTemplateCode;
  // dst is reused for the block's size; as HeapSlot_Alloc(slot, 436), 31 of
  // 597 instructions differ in tla-en.
  dst = (u32 *)436;
  dst = HeapSlot_Alloc(slot, (s32)dst);

  COPY(4);
  if (mode == 0) {
    COPY_FROM(BattleEffect_Mask, 3);
  }

  if ((flags & 12) == 4) {
    COPY_FROM(src, 3);
  }
  src += 3;
  if ((flags & 12) == 8) {
    COPY_FROM(src, 4);
  }
  src += 4;
  if ((flags & 12) == 12) {
    COPY_FROM(src, 3);
  }
  src += 3;
  if ((flags & 12) == 0) {
    PUT(src[0]);
  }
  src += 1;

  if ((flags & 2) != 0) {
    COPY(2);
    if ((flags & 8) != 0) {
      PUT(src[0]);
    } else {
      PUT(src[1]);
    }
    src += 2;
    COPY(2);
    PUT(src[0] + (1 << heightShift));
    PUT(src[1] + (1 << heightShift));
    src -= 6;
  }
  src += 8;

  PUT(*src++);
  noRows = dst;
  PUT(*src++);

  if ((flags & 1) == 0) {
    s32 width = 1 << widthShift;

    PUT(src[0] + width - 1);
  }
  src += 1;
  if ((flags & 1) != 0) {
    PUT(src[0]);
    PUT(src[1]);
    if ((flags & 4) != 0) {
      PUT(src[2]);
      PUT(src[3]);
    } else {
      PUT(src[4]);
      PUT(src[5]);
    }
    PUT(src[6]);
    PUT(src[7]);
    PUT(src[8] + Data_081973f0[widthShift]);
    if ((flags & 4) != 0) {
      PUT(src[9]);
    } else {
      PUT(src[10]);
    }
    PUT(src[11] + Data_081973f0[widthShift]);
  }
  src += 12;

  PUT(*src++);
  noColumns = dst;
  PUT(*src++);

  control = 0x84000000;
  COPY_LONG(6);

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
  }
  src += 1;

  COPY_LONG(5);

  row = dst;
  PUT(src[0]);
  PUT(src[1] + (1 << (heightShift - 3)) - 1);
  PUT(src[2] + ((widthShift - 3) << 7));
  PUT(src[3]);
  src += 4;

  if ((flags & 1) == 0) {
    PUT(src[0]);
  }
  src += 1;

  skip = dst + 1;
  COPY(2);
  if ((flags & 4) != 0) {
    PUT(src[0]);
  } else {
    PUT(src[1]);
  }
  src += 2;

  run = dst;
  runCopy = dst;
  switch (mode) {
  default: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_CopyReverse;
    } else {
      part = BattleEffect_CopyForward;
    }
    COPY_FROM(part, 2);
    break;
  }
  case 2: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_TransparentReverse;
    } else {
      part = BattleEffect_TransparentForward;
    }
    COPY_FROM(part, 4);
    break;
  }
  case 1: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_MaxReverse;
    } else {
      part = BattleEffect_MaxForward;
    }
    COPY_FROM(part, 4);
    break;
  }
  case 0:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_AdditiveReverse, 6);
    } else {
      COPY_DMA(BattleEffect_AdditiveForward, 6);
    }
    break;
  }

  PUT(*src++);
  PUT_BRANCH(run);
  runSize = (u8 *)dst - (u8 *)runCopy;
  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;
  PATCH(skip);

  skip = dst + 1;
  COPY(2);

  run = dst;
  switch (mode) {
  default:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_Copy8Reverse, 16);
    } else {
      COPY_DMA(BattleEffect_Copy8Forward, 16);
    }
    break;
  case 2:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_Transparent8Reverse, 25);
    } else {
      COPY_DMA(BattleEffect_Transparent8Forward, 25);
    }
    break;
  case 1:
    if ((flags & 4) != 0) {
      COPY_TWICE(BattleEffect_Max4Reverse, 16);
    } else {
      COPY_TWICE(BattleEffect_Max4Forward, 16);
    }
    break;
  case 0:
    if ((flags & 4) != 0) {
      COPY_TWICE(BattleEffect_ColorReverse, 14);
    } else {
      COPY_TWICE(BattleEffect_ColorForward, 14);
    }
    break;
  }

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;

  PUT(*src++);
  PUT_BRANCH(run);
  PATCH(skip);

  PUT(*src++);
  skip = dst;
  PUT(*src++);
  if ((flags & 4) != 0) {
    PUT(src[0]);
  } else {
    PUT(src[1]);
  }
  src += 2;
  // An empty once-only loop keeps the step before the size's load; without
  // it, 2 of 597 instructions differ in tla-en.
  do {
  } while (0);

  {
    u32 words = (u32)runSize / 4;
    /* FAKEMATCH(pin): count fixed in r2, where the rounded size is built; as a
     * plain local, 35 of 597 instructions differ in tla-en. */
    register s32 count __asm__("r2") = words * 4;
    s32 size = count;

    /* FAKEMATCH(barrier): an empty asm that keeps words alive past count's
     * set; without it, 6 of 597 instructions differ in tla-en. */
    __asm__("" : : "r"(words));
    Dma_Set(runCopy, dst, DMA_COPY_WORDS(count / 4), REG_DMA3);
    dst = (u32 *)((u8 *)dst + size);
  }
  PATCH(skip);

  COPY(3);

  PUT_BRANCH(row);
  PATCH(noRows);
  PATCH(noColumns);

  COPY(2);
  {
    const u32 *end = (const u32 *)Data_081973f0;

    return src == end;
  }
}

// The runner for the ARM kernel at Data_081380ac (relocated_kernel_0.s), the
// same wrapper as Render_TransformVertices (below) and
// Message_Find, with two arguments. Nothing in the image calls it.

extern const u8 Data_081380ac[];
extern u8 Data_081380acCodeSize[];

/// Copies the ARM kernel at Data_081380ac into a heap block (DMA 3, words),
/// runs it there with the caller's two arguments and frees the block.
void RamCode_RunKernel0(s32 arg0, s32 arg1) {
  void (*routine)(s32, s32);
  u32 size;

  // The size's load in a once-only block keeps it after the parameters are
  // copied; as a plain assignment, 4 of 31 instructions differ in tla-en.
  do {
    size = (u32)Data_081380acCodeSize;
  } while (0);
  routine = (void (*)(s32, s32))Heap_Alloc(size);
  Dma_Set((const void *)Data_081380ac, routine, DMA_COPY_WORDS(size >> 2),
          REG_DMA3);
  routine(arg0, arg1);
  Heap_Release(routine);
}

s32 Render_ProjectPoint(const void *point, s32 *screen);

extern const u8 Data_0813b9a0[];
extern u8 Data_0813b9a0CodeSize[];

/// Copies the ARM vertex kernel into a heap block and runs it: count packed
/// s8 vertices through the current transform into screen coordinates.
void Render_TransformVertices(const void *corners, void *vertices, s32 count) {
  void (*routine)(const void *, void *, s32);
  u32 size;

  // The size's load in a once-only block keeps it after the parameters are
  // copied; as a plain assignment, 4 of 36 instructions differ in tla-en.
  do {
    size = (u32)Data_0813b9a0CodeSize;
  } while (0);
  routine = (void (*)(const void *, void *, s32))Heap_Alloc(size);
  Dma_Set((const void *)Data_0813b9a0, routine, DMA_COPY_WORDS(size >> 2),
          REG_DMA3);
  routine(corners, vertices, count);
  Heap_Release(routine);
}

/// One projected vertex, as Render_ProjectPoint leaves it.
struct ProjectedVertex {
  s16 x;     ///< screen pixels
  s16 y;     ///< screen pixels
  s16 depth; ///< projected depth
  s16 unk06;
};

/// Projects count model vertices through Render_ProjectPoint into screen
/// coordinates.
void Render_ProjectVertices(struct ModelVertex *vertices,
                            struct ProjectedVertex *screen, s32 count) {
  s32 point[3];

  for (; count != 0; count--) {
    SET_VECTOR(point, vertices->x << 16, vertices->y << 16, vertices->z << 16);
    Render_ProjectPoint(point, point);
    screen->x = point[0];
    screen->y = point[1];
    screen->depth = point[2];
    vertices++;
    screen++;
  }
}

struct PoolEntry {
  s32 head;
  s32 next;
  u8 unk08[16];
  s32 tail;
};

/// Allocates count model entries plus a cleared one that ends the list.
struct PoolEntry *Render_AllocateModelList(s32 count) {
  struct PoolEntry *entries =
      Heap_AllocEwram((count + 1) * sizeof(struct PoolEntry));
  s32 i;

  for (i = 0; i < count + 1; i++) {
    entries[i].head = 0;
    entries[i].next = 0;
    entries[i].tail = 0;
  }
  return entries;
}

struct Texture {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  u32 pixels;
};

u32 Heap_GetRemainingIwram(void);

/// Copies a texture's pixels into a heap block when IWRAM has room for them;
/// returns 1 with copy set up, 0 when there is no room.
s32 Render_CopyTextureToIwram(struct Texture *source, struct Texture *copy) {
  s32 size = (1 << source->logWidth) << source->logHeight;
  u32 pixels;
  s32 misalign;

  if (Heap_GetRemainingIwram() < size + 8)
    return 0;
  *(u16 *)copy = *(u16 *)source;
  copy->pixels = (u32)Heap_Alloc(size + 8);
  pixels = source->pixels;
  misalign = pixels & 3;
  Iwram_CopyWordsEntry((void *)copy->pixels, (void *)(pixels - misalign),
                  (size + misalign + 3) & -4);
  copy->pixels += misalign;
  return 1;
}

/// One entry of a model list. Render_AllocateModelList allocates the list with
/// a cleared entry after the last.
struct Model {
  s32 kind; ///< the rasterizer kernel that draws it; 0 ends the list
  s32 flags;
  const void *mesh;
  void *matrix;
  struct Texture *texture;
  s32 fade;
  u8 unk18;
  u8 frame;
};

/// The texture stays where it is, even when IWRAM has room for a copy.
#define MODEL_TEXTURE_IN_PLACE 4

typedef void (*ModelKernel)(struct Model *model);

// The rasterizer kernels, ARM code that runs from IWRAM, with their sizes in
// bytes and, for three of them, in DMA words.
extern const u8 Data_08138fc8[];
extern const u8 Data_081391c8[];
extern const u8 Data_081393d8[];
extern const u8 Data_081395f8[];
extern const u8 Data_08139aac[];
extern const u8 Data_08139f70[];
extern const u8 Data_0813a444[];
extern const u8 Data_0813a924[];
extern const u8 Data_0813ae14[];
extern const u8 Data_0813b2ec[];
extern const u8 Data_0813b7a0[];
extern u8 Data_08138fc8CodeSize[];
extern u8 Data_081391c8CodeSize[];
extern u8 Data_081393d8CodeSize[];
extern u8 Data_081395f8CodeSize[];
extern u8 Data_08139aacCodeSize[];
extern u8 Data_08139f70CodeSize[];
extern u8 Data_0813a444CodeSize[];
extern u8 Data_0813a924CodeSize[];
extern u8 Data_0813ae14CodeSize[];
extern u8 Data_0813b2ecCodeSize[];
extern u8 Data_0813b7a0CodeSize[];
extern u8 Data_081395f8CodeWords[];
extern u8 Data_0813ae14CodeWords[];
extern u8 Data_0813b2ecCodeWords[];

/// Copies a kernel whose DMA control is a word of its own.
// Kernels 8, 9 and 10 read it from the pool, one word each, two of them equal
// yet stored twice, so it is a symbol of each kernel: written
// 0x84000000 | (size >> 2) the two fold into one pool entry, and as a number
// the size is built in three instructions.
#define LOAD_KERNEL_WORDS(to, code, bytes, words)                              \
  {                                                                            \
    (to) = Heap_Alloc((u32)(bytes));                                           \
    Dma_Set((code), (to), 0x84000000 + (u32)(words), REG_DMA3);                \
  }

/// Copies the model's texture into IWRAM unless it is to stay in place.
#define COPY_TEXTURE()                                                         \
  {                                                                            \
    if (!(model->flags & MODEL_TEXTURE_IN_PLACE) &&                            \
        Render_CopyTextureToIwram(texture, &copy))                             \
      model->texture = &copy;                                                  \
  }

/// Draws a list of models. A run of models of one kind loads that kind's
/// rasterizer into IWRAM once, and it is freed when the kind changes. Before
/// a textured model is drawn, its texture is copied into IWRAM where there is
/// room, and the copy is freed after the model is drawn.
void Render_DrawModelList(struct Model *model) {
  ModelKernel code = NULL;
  s32 lastKind = 0;
  struct Texture copy;
  struct Texture *texture;

  for (;;) {
    texture = model->texture;
    if (model->kind != lastKind && code != NULL)
      Heap_Release(code);
    if (model->kind == 0)
      break;
    switch (model->kind) {
    case 1:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_08138fc8, Data_08138fc8CodeSize);
      code(model);
      break;
    case 11:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_0813b7a0, Data_0813b7a0CodeSize);
      code(model);
      break;
    case 2:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_081391c8, Data_081391c8CodeSize);
      code(model);
      break;
    case 3:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_081393d8, Data_081393d8CodeSize);
      code(model);
      break;
    case 8:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Data_081395f8, Data_081395f8CodeSize,
                          Data_081395f8CodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case 9:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Data_0813ae14, Data_0813ae14CodeSize,
                          Data_0813ae14CodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case 10:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Data_0813b2ec, Data_0813b2ecCodeSize,
                          Data_0813b2ecCodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case 4:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_08139aac, Data_08139aacCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case 5:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_08139f70, Data_08139f70CodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case 6:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_0813a444, Data_0813a444CodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case 7:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_0813a924, Data_0813a924CodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    }
    lastKind = model->kind;
    if (model->texture == &copy) {
      Heap_Release((void *)(model->texture->pixels & ~3));
      model->texture = texture;
    }
    model++;
  }
}
#undef HI
#undef SUMMON16_FRAMES
#undef SUMMON16_SKIP_TO
#undef SLOT_BLITTER
#undef BLITTER
#undef HALF
#undef Data_02012ef4
#undef SLOT_ALT
#undef OBJ_TILES
#undef SCENE
#undef PLTT
#undef STREAK
#undef FADE_PLTT
#undef FILL_WORDS
#undef DECODE_01
#undef XOR_SWAP
#undef FILL_SPAN
#undef DIFF_ABS
#undef DRAW_SPAN
#undef STEP_RIGHTWARD
#undef STEP_LEFTWARD
#undef BRANCH_FIELD
#undef PUT
#undef COPY_FROM
#undef COPY
#undef COPY_LONG
#undef COPY_DMA
#undef COPY_TWICE
#undef PUT_BRANCH
#undef PATCH
#undef MODEL_TEXTURE_IN_PLACE
#undef LOAD_KERNEL_WORDS
#undef COPY_TEXTURE
