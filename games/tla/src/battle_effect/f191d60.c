#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "input.h"
#include "heap_release.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "task.h"
#include "resource.h"

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

#include "render_model.h"

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
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Graphics_LoadGreyRamp(void);
void BattleEffect_SelectLivingTargets(struct FxPartyList *party);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void Func_0816442c(s32 formation, s32 x, s32 y);
void Func_0816467c(s32 formation, s32 x, s32 y);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer, s32 frames);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void Func_08164a4c(s32 blue, s32 green, s32 red);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase);
void Sound_PlayFar(s32 cue);

void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
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

#include "motobj.h"
#include "bg_scroll.h"
#include "battle_display.h"

#include "graphics.h"

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
struct ModelVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

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
