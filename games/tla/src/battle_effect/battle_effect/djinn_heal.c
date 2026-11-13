/// @file
/// Sixteen tumbling textured quads, the Zagan summon, and the camera turned by
/// phases.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "module/battle_effect/battle_effect.h"
#include "module/system/system.h"
#include "gamepak/far_call/field.h"
#include "resource_ids.h"

struct Model;

#define HEAP_BLITTER 104 // ☀️ HEAP_SLOT_BLITTER

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.

#define QUAD_FRAMES 64
#define QUAD_COUNT 16

// Where the quads' texture and the Djinn sit in the sheet.
#define SHEET_TEXTURE 0x2710
#define SHEET_DJINN 0x65c0

void Render_DrawModelList(struct Model *model);

// Sixteen textured quads tumble down over the targets (or, from the first
// target, rise), three frames apart, for 64 frames; with a prelude a Djinn
// circles over the canvas and a fan of light lines spreads from it. At
// frame 56 every target flashes.
void Func_0814b9e8(struct BattleEffectArgument *effect, s32 mode) {
  s32 anchor[3];
  s32 size[3];
  struct EffectPosition pos;
  struct Texture texture;
  s32 y;
  struct BattleEffectWork *work;
  struct EffectStep *quad;
  s32 x;
  s32 frame;
  struct RenderJob *job;
  void *vertices;
  void *canvas;
  // Unused: draws[1] gives the stack frame its 4 bytes.
  DrawRectangle draws[2];
  s32 i;
  s32 height;
  s32 fade;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);

  // The mode's canvas, sheets and palette.
  if (mode == QUADS_MERCURY) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 33, anchor, size);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D0,
                               work->sheet + SHEET_TEXTURE, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SHEET_DJINN, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
  } else if (mode == QUADS_MARS) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 2, anchor, size);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SHEET_DJINN, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FeatherSheet,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  } else {
    EffectPosition_ProjectBattler(effect->actors[0], &pos);
    anchor[0] = pos.x;
    Resource_LoadAndDecompress((s32)&ResourceId_FeatherSheet,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Centre the canvas on the anchor; the blitter mirrors for side 0.
  REG_BG2PA = 0x100;
  REG_BG2X = (64 - anchor[0]) << 8;
  if (effect->side == 0)
    BattleEffect_LoadBlitter(HEAP_BLITTER,
                             BLITTER_ADDITIVE << 4 | COMPOSITE_CLIP_X |
                                 COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  else
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 |
                                               COMPOSITE_CLIP_X |
                                               COMPOSITE_CLIP_Y);
  draws[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (frame = 0; frame != QUAD_FRAMES; frame++) {
    if (frame == 8)
      Sound_PlayFar(246);

    // The quads start above the screen (or at the target for mode 2), each
    // with its own heading, spin and size.
    if (frame == 0) {
      height = -0x200000;
      for (i = 0; i != 32; i++) {
        quad = &work->particles[i];
        quad->x = Random16() & 0xffff;
        Random16();
        quad->z = 1 << 16;
        if (mode == QUADS_RISING)
          quad->y = 32 << 16;
        else
          quad->y = height;
        quad->velocityY = ((Random16() & 0xffff) << 1) + 0x10000;
        quad->velocityX = Random16() & 0xffff;
        quad->velocityZ = Random16() & 0xffff;
        quad->variant = (Random16() & 0x3fff) + 0x2000;
        height += -0x40000;
      }
    }

    // Frame 56: the next phase, and every target flashes.
    if (frame == 56) {
      Sound_PlayFar(212);
      BattlePlayback_StartFar(-1);
      for (i = 0; i != effect->count; i++)
        BattleEffect_TintBattler(effect->actors[i], 7, -1, i, 8);
    }

    // With a prelude the Djinn circles over the anchor and climbs away
    // after frame 32, and a fan of light lines spreads from the canvas's
    // middle while the canvas fades.
    if (effect->prelude == 1) {
      x = ((-Trig_Sin(frame << 11) * 20) >> 16) + 44;
      y = ((Trig_Cos(frame << 11) * 4) >> 16) + anchor[1] - 24;
      if (frame > 32)
        y = y - frame * 4 + 128;
      draws[0](canvas, work->sheet + SHEET_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draws[0](canvas, work->sheet + SHEET_DJINN, x, y, 40, 40);
      // DEAD: frame never passes 63 here; the source keeps the test.
      if (frame <= 79) {
        s32 angle;
        s32 lineX;
        s32 lineY;

        Iwram_FillWordsEntry(work->sheet, 128,
                             0x28282828 - frame / 2 * 0x01010101);
        i = 0;
        angle = frame << 9;
        for (; i != 16; i++) {
          lineX = ((Trig_Sin(angle) * frame) >> 16) + 64;
          lineY = ((Trig_Cos(angle) * frame) / 8 >> 16) - 16;
          draws[0](canvas, work->sheet, lineX, lineY, 1, 128);
          angle += 0x1000;
        }
      }
    }

    // The quads: each a render job of one textured face, placed by its own
    // translation, scale, pitch and roll.
    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;
    // DEAD: frame never passes 63 here; the source keeps the test.
    if (frame <= 63) {
      vertices = Heap_AllocEwram(32);
      job = Render_AllocateModelList(1);
      fade = 0;
      if (frame > 55)
        fade = (56 - frame) * 8;
      job->fade = fade;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + SHEET_TEXTURE;
      job->kind = 6;
      job->texture = &texture;
      job->faces = gData_08199340;
      job->vertices = vertices;

      for (i = 0; i != QUAD_COUNT; i++) {
        s32 scale;
        s32 delay;

        quad = &work->particles[i];
        delay = i * 3;
        scale = quad->variant;

        if (frame >= delay) {
          s32 across;
          s32 deep;

          Render_ResetTransformState();
          SceneTransform_ApplyPitch(3000);
          if (mode == QUADS_RISING)
            quad->y -= quad->velocityY;
          else
            quad->y += quad->velocityY;
          if (quad->velocityY < -0x20000)
            quad->z += 0x10000;
          else
            quad->z += 0xc000;
          across = Trig_Sin(quad->x) * ((s16 *)&quad->z)[1];
          deep = Trig_Cos(quad->x) * ((s16 *)&quad->z)[1];
          quad->x += 0x200;
          Func_08015160(across, quad->y, deep);
          SceneTransform_ApplyScale3(scale, scale, scale);
          SceneTransform_ApplyPitch(quad->velocityX);
          SceneTransform_ApplyRoll(quad->velocityZ);
          quad->velocityX += 0x400;
          quad->velocityZ += 0x400;
          Render_TransformVertices(gData_08199210, vertices, 4);
          Render_DrawModelList(job);
        }
      }
      Heap_Release(job);
      Heap_Release(vertices);
    }
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#define ZAGAN_PARTS 12

// Abilities: Zagan.
//
// The screen slides over to a white field and petals blow across it; Zagan
// (twelve objects) gallops in from the right as the backdrop darkens, rears,
// and at frame 157 swings its axe across the targets, striking four times
// while the screen shakes and rings of dust spread. Holding A or B draws
// only every other frame.
void BattleEffect_RunZagan(struct BattleEffectArgument *effect) {
  u8 *battle;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  s32 *transition;
  s32 savedScroll;
  DrawRectangle *blit;
  struct RequestPair position;
  struct RequestPair velocity;
  s32 skip;
  DrawRectangle blitters[2];
  struct RequestPair request;
  s32 placement[4];
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct EffectStep *p;
  s32 none;
  u8 *ringPixels;
  s32 size;
  s32 scroll;
  s32 frame;
  s32 i;
  s32 j;

  none = 0;
  ringPixels = gData_02014000; // the rings' texture, for the whole function
  battle = (u8 *)Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  savedScroll = gBgScroll[1].x;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = none;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The screen slides over to the field.
  for (scroll = -80, frame = 0; frame != 28; frame++) {
    if (frame == 27) {
      gCameraSceneParameters[4] = 240;
      BattleEffect_SelectLivingTargets(work->effect);
      work->scrollStepX = -8;
      transition[4] = 1;
      Iwram_FillWordsEntry((void *)(BG_PLTT + 96), 256, 0x7fff7fff);
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->sheet, 0, 0);
      // Ten ever-fainter copies of one mote.
      for (j = 0; j != 10; j++) {
        size = 4;
        for (i = 0; i != size * size * 2; i++) {
          s32 offset = gData_08197410[size - 1] + i + 0x700;
          s32 value = ((u8 *)work)[offset];

          if (value != 0) {
            value -= j * 7;
            if (value <= 0) {
              value = 1;
            }
          }
          sprites[size * size * j * 2 + i] = value;
        }
      }
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, work->sheet, 1,
                                 1);
      for (i = 0; i != 32; i++) {
        work->sheet[0x5dc0 + i] = i + 16;
        work->sheet[0x5de0 + i] = i + 16;
        work->sheet[0x5e00 + i] = i + 16;
      }
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  blit = blitters;
  BattleEffect_FetchRectangleBlitters(0, (void **)blit);
  work->transferMode = 2;
  work->transferValue = 50;
  skip = 0;
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       128);
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);
  BattleEffect_SpawnObjects(ZAGAN_PARTS, 658, 1);
  REG_BG2X = skip;
  REG_BG2PA = 0x80;

  for (frame = 0; frame != 200; frame++) {
    if (gInput.repeat & 3) {
      skip = 1;
    }
    if (frame == 0) {
      Sound_PlayFar(212);
    }
    if (frame == 32) {
      Sound_PlayFar(191);
    }
    if ((u32)(frame - 78) <= 31) {
      REG_BLDALPHA = (16 - (frame - 78) / 2) | 0x1000;
    }
    if (frame == 140) {
      REG_BLDALPHA = 0x1010;
    }
    if (frame == 157) {
      BattlePlayback_StartFar(145);
      goto strike;
    }
    if (frame == 161 || frame == 165 || frame == 169) {
    strike:
      for (i = 0; i != effect->count; i++) {
        BattleMotion_ApplyKnockbackFar(effect->actors[i], 4);
        BattleEffect_TintBattler(effect->actors[i], 7, -1, i, 4);
      }
    }
    if (frame == 0) {
      transition[4] = 1;
      position.x = 240 << 16;
      position.y = 0x400000;
      velocity.x = -0x140000;
      velocity.y = 0;
      for (i = 0; i != ZAGAN_PARTS; i++) {
        Animation_ApplyChildArgumentFar(work->objects[i], i + 12);
      }
      REG_BLDALPHA = 0x1010;
      Iwram_FillWordsEntry((void *)(BG_PLTT + 96), 256, 0x7fff7fff);
      work->scrollStepX = -8;
      gCameraSceneParameters[3] = -968;
      gCameraSceneParameters[4] = 120;
      gBgScroll[1].x = savedScroll + 1128;
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, work->sheet, 1,
                                 1);
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
      work->transferMode = 2;
      work->transferValue = 50;
    }
    gCameraSceneParameters[3] -= work->scrollStepX;
    gBgScroll[1].x += work->scrollStepX;

    // The backdrop darkens toward the battle's palette.
    if (!(frame & 1) && (u32)(frame - 32) <= 111) {
      u16 *from = Resource_GetTableEntry(*(u16 *)(battle + 0x670));
      u16 *to = (u16 *)(BG_PLTT + 96);

      for (i = 0; i != 128; i++, from++, to++) {
        s32 b = (*to >> 10) & 31;
        s32 g = (*to >> 5) & 31;
        s32 r = *to & 31;
        s32 blue = ((*from >> 10) & 31) - 2;
        s32 green = ((*from >> 5) & 31) - 10;
        s32 red = (*from & 31) - 6;

        if (blue < 0) {
          blue = 0;
        }
        if (green < 0) {
          green = 0;
        }
        if (red < 0) {
          red = 0;
        }
        if (b > blue) {
          b--;
        }
        if (g > green) {
          g--;
        }
        if (r > red) {
          r--;
        }
        *to = (b << 10) | (g << 5) | r;
      }
    }
    if ((u32)(frame - 64) <= 35) {
      velocity.x += 0x8000;
      velocity.y += -0x1000;
    }
    if (frame > 177) {
      velocity.x += -0x20000;
      velocity.y += -0x10000;
    } else if (frame > 139) {
      velocity.x += -40000;
      velocity.y += 0x2000;
    } else if (frame > 99) {
      velocity.x += -0x2000;
      velocity.y += 0x800;
    }
    if (frame == 156) {
      Iwram_FillWordsEntry(canvas, 0x3c00, 0x3f3f3f3f);
    }
    if (frame == 157) {
      Resource_LoadAndDecompress((s32)&ResourceId_CrescentRowSheet, work->sheet,
                                 1, 0);
      work->shakeFrames = 16;
      for (i = 0; i != ZAGAN_PARTS - 1; i++) {
        Animation_ApplyChildArgumentFar(work->objects[i], i + 24);
      }
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if ((u32)(frame - 156) <= 7) {
      blitters[0](canvas, work->sheet, 12, 0, 58, 120);
    }
    for (i = 0; i != 8; i++) {
      if (frame == i * 6 + 114) {
        work->scrollStepX++;
      }
    }
    if (frame == 163) {
      gBgScroll[1].x += -20;
    }
    if (frame == 167) {
      gBgScroll[1].x += -10;
    }
    if (frame == 171) {
      gBgScroll[1].x += -10;
    }

    // Petals on the wind.
    if (frame == 0) {
      s32 row;

      for (i = 0, row = 3, p = work->particles; i != 20; i++, p++, row += 6) {
        p->x = (Random16() & 127) - 160;
        p->y = row;
        p->variant = Random16() & 1;
        p->velocityX = (Random16() & 3) + p->variant * 8 + 8;
      }
    }
    if ((u32)frame <= 119) {
      s32 odd;

      for (i = 0, odd = frame & 1, p = work->particles; i != 20; i++, p++) {
        if (!odd || !skip) {
          blitters[0](canvas, work->sheet + 0x5dc0, p->x, p->y, 32,
                      p->variant + 1);
        }
        p->x += p->velocityX;
        if (p->x > 128) {
          p->x -= 160;
        }
      }
    }
    if ((u32)frame > 195) {
      goto shake;
    }

    // Zagan.
    request = Data_08196e0c;
    placement[3] = 0;
    placement[1] = 0;
    if (!(frame & 1) || !skip) {
      if (frame > 156) {
        for (i = 0; i != ZAGAN_PARTS - 1; i++) {
          placement[0] = (Data_08197a38[i][0] << 16) + position.x - 0x200000;
          placement[2] = (Data_08197a38[i][1] << 16) + position.y + 0x100000;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            (s32 *)&request, 0);
        }
      } else {
        for (i = 0; i != ZAGAN_PARTS; i++) {
          placement[0] = (Data_08197a64[i][0] << 16) + position.x;
          placement[2] = (Data_08197a64[i][1] << 16) + position.y - 0x100000;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            (s32 *)&request, 0);
        }
      }
    }
    if (frame > 177) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 63 / 64;
      velocity.y = velocity.y * 63 / 64;
    } else if (frame > 149) {
      if (position.y > 0x37ffff) {
        velocity.x = 0;
        velocity.y = 0;
      }
      position.x += velocity.x;
      position.y += velocity.y;
    } else if (frame <= 59) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 58 / 64;
      velocity.y = velocity.y * 62 / 64;
    } else {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 58 / 64;
      velocity.y = velocity.y * 61 / 64;
    }
    if ((!(frame & 1) || !skip) && frame <= 119) {
      blit[1](canvas, work->sheet + ((frame / 3) & 3) * 6000,
              (position.x / 2 >> 16) - 12, (position.y >> 16) - 58, 60, 100);
    }
    if (frame == 157) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, ringPixels, 0, 0);
    }
    if ((frame & 1) && skip) {
      goto shake;
    }

    // Rings of dust from the blow.
    if (frame <= 156) {
      goto next;
    }
    {
      s32 k;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = ringPixels;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != 4; k++) {
        s32 start = Data_08197a94[k] + 157;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (((frame - start) * Data_08197a98[k] * 125) << 4) + 0x8000;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(-0x100000, Data_08197a9c[k] << 16, 0);
        SceneTransform_ApplyScale3(scale, scale * 4 / 3, scale);
        SceneTransform_ApplyPitch(3000);
        model->scrollU = ((frame + k * 5) & 7) * 16;
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
  shake:
    if (frame > 156) {
      if (work->shakeFrames > 0) {
        s32 offsetX = (Random16() & 3) - 8;
        s32 randomY = Random16() & 15;
        s32 offsetY = randomY - 8;

        gBgScroll[1].y = randomY + 24;
        gCameraSceneParameters[3] = 120 - offsetX;
        gCameraSceneParameters[4] = 120 - offsetY;
        work->shakeFrames--;
      } else {
        gBgScroll[1].y = work->savedBg1Y;
        gCameraSceneParameters[3] = 120;
        gCameraSceneParameters[4] = 120;
      }
    }
  next:
    BattleEffect_CountDownBattlerTints();
    if (!(frame & 1) || !skip) {
      work->transferPending = 1;
      wait(1);
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  transition[4] = 0;
  for (i = 0; i != ZAGAN_PARTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}

struct ShakeWork {
  u8 unk0000[54];
  u16 offset;
  u8 unk0038[0x7774];
  s32 amount;
  s32 phase;
};

/// A task: turns the camera by the effect's delta, the whole
/// delta on phase 1 and half of it on the frames in between.
void Camera_ApplyPhasedDelta(void) {
  struct ShakeWork *work = (struct ShakeWork *)Ram_HeapSlots->battleFxWork[0];
  struct ShakeWork *camera = Ram_HeapSlots->cameraWork;

  if (work->phase == 1) {
    camera->offset += work->amount;
    work->phase = 0;
  } else {
    camera->offset += work->amount / 2;
    if (work->phase == 2)
      work->phase = 0;
    else
      work->phase = 2;
  }
}
