/// @file
/// Crystal: a spinning crystal and drifting shards.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

struct Model;

struct BattleObjectPosition {
  u8 unk00[8];
  s32 x;
};

#define BLITTER ((DrawRectangle)Ram_HeapSlots->ramCode)
#define CRYSTAL_FRAMES 103
#define CRYSTAL_SHARDS 32

/// Abilities: Crystal.
///
/// Sixteen shaded discs, 2 to 32 pixels across, are built in the map cell
/// buffer. A pillar of light rises at the target for 20 frames while a
/// textured crystal spins up over it from frame 16; from frame 48 thirty-two
/// shards, one every two frames, drift up from around the target, pulsing in
/// size and jittering until they settle, and the targets flash at frames 64,
/// 68, ... before the scene fades out from frame 96.
void BattleEffect_RunCrystal(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u16 *discOffsets;
  s32 center[3];
  s32 other[3];
  struct ScreenPoint screen;
  struct ModelTexture texture;
  s32 frame;
  s32 i;
  s32 size;
  s32 offset;
  s32 cells;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  discOffsets = (u16 *)(work->sheet + 0x2710);
  cells = (s32)gMapGridCellBuffer;
  Iwram_ClearWords((void *)cells, 0x8000);
  for (offset = 0, i = 0, size = 2; i != 16; size += 2, i++) {
    BattleEffect_DrawShadedCircles((u8 *)(offset + cells), size, 0x10000);
    discOffsets[i] = offset;
    offset += (i * 2 + 3) * size / 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FB, work->sheet, 0, 0);
  Picture_MirrorIntoQuadrants(work->sheet, gData_02014000, 64, 64);
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_VENUS, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
      center, other);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + 0x65c0, 1, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlastSheet),
                       128);
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  if (args->side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  }
  draw[0] = BLITTER;
  {
    void *camera;
    struct BattleObjectPosition *target;

    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
    camera = Ram_HeapSlots->cameraWork;
    target = BattleObject_GetFar(args->targets[0])->object;

    for (frame = 0; frame != CRYSTAL_FRAMES; frame++) {
      if (frame == 0) {
        struct Particle *shard;
        for (i = 0, shard = work->particles; i != CRYSTAL_SHARDS;
             i++, shard++) {
          s32 angle = Random16();
          s32 radius = (Random16() & 63) + 80;

          SET_POSITION(shard, target->x + ((Trig_Sin(angle) * radius) >> 1),
                       px(8), (Trig_Cos(angle) * radius) >> 1);
          shard->velocityX = Random16();
          shard->velocityY = (Random16() & 0xffff) + 0x40000;
          shard->velocityZ = (Random16() & 7) + 4;
          shard->variant = -1;
        }
      }
      if (frame > 95) {
        REG_BLDALPHA = (0xce - frame * 2) | 0x1000;
      }
      Transform_Reset();
      Transform_SetLookAt(camera, (u8 *)camera + 12);

      // The pillar of light.
      if (frame <= 19) {
        s32 x = ((Trig_Sin(frame << 11) * 8) >> 16) + center[0] / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + center[1] - 24;

        y += frame * 3;

        draw[0](canvas, work->sheet + 0x65c0, x, y, 20, 40);
        if (frame <= 3) {
          draw[0](canvas, work->sheet + 0x65c0, x, y, 20, 40);
        }
      }

      // The shards, one more every two frames from frame 48.
      if (frame > 47) {
        struct Particle *shard;
        for (i = 0, shard = work->particles; i != CRYSTAL_SHARDS;
             i++, shard++) {
          if (frame >= i * 2 + 48) {
            s32 width;

            EffectPosition_ProjectPoint(&shard->x, &screen);
            screen.x >>= 1;
            width = shard->velocityZ;
            width += Trig_Sin(shard->velocityX) * width / 2 >> 16;
            shard->velocityX += px(1) / 64;
            width *= 2;
            if ((u32)(width - 2) <= 61) {
              s32 cell = (width - 2) / 2;
              s32 x = screen.x;
              s32 y = screen.y;

              if (shard->variant == 0) {
                x += (Random16() & 7) - 4;
                y += (Random16() & 7) - 4;
              }
              Canvas_BlitAtFoot((u8 *)((s16)discOffsets[cell] + cells), x, y,
                                width);
            }
            shard->y += shard->velocityY;
          }
        }
      }

      for (i = 0; i != args->targetCount; i++) {
        if (frame == i * 4 + 64) {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 24);
        }
      }

      // The crystal, spinning and growing from frame 16.
      Transform_Reset();
      gCameraSceneParameters[4] = 104;
      {
        void *vertices = Heap_AllocEwram(32);
        struct RenderModel *model = Render_AllocateModelList(1);

        model->kind = 6;
        model->texture = &texture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = vertices;
        texture.logWidth = 7;
        texture.logHeight = 7;
        texture.pixels = gData_02014000;
        if (frame > 15) {
          s32 scale = (frame << 12) - Q16_ONE;

          if (scale > 0x1bd50) {
            scale = 0x1bd50;
          }
          model->shade = 0;
          Transform_ApplyPitch(-0xbb8);
          SceneTransform_ApplyPosition3((center[0] / 2 - 64) << 16, 0x280000,
                                        0);
          SceneTransform_ApplyUniformScale(scale >> 1);
          Transform_ApplyYaw(frame << 9);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(vertices);
      }
      gCameraSceneParameters[3] = 120;
      gCameraSceneParameters[4] = 120;
      EFFECT_END_FRAME(work);
    }
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef BLITTER
#undef CRYSTAL_FRAMES
#undef CRYSTAL_SHARDS
