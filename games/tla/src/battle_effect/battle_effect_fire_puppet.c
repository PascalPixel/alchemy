/// @file
/// Fire Puppet and Whiplash.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

#define FIRE_PUPPET_FRAMES 103
#define PUPPET_SEGMENTS 33
/// The arm's corners, their projections and its strip's mesh, in the map cell
/// buffer.
#define PUPPET_STRIP_VERTICES (Ram_MapCellBuffer + 4096)
#define PUPPET_STRIP_MESH (Ram_MapCellBuffer + 8192)
/// The last particle keeps the target's position, and its scale in velocityX
/// and velocityY.
#define FIRE_PUPPET_SAVED_TARGET 63

/// Where GhostSkullTexture is decoded in the effect's pictures.
#define FIRE_PUPPET_GHOST_SKULL 16384

/// From the caster rises a flaming arm (33 segments of two vertices each, a
/// scrolling textured strip) that reaches up and over to 1424 units, with a
/// hand at its end; at frame 54 it strikes, hitting every target, and from
/// frame 74 it shrinks back. The target's position is kept in the work's
/// last particle.
///
/// Abilities: Fire Puppet.
void BattleEffect_RunFirePuppet(struct BattleEffectParams *args) {
  u8 *buffer;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sprites;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct ScreenPoint pos;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 endX;
  s32 endY;
  s32 i;

  buffer = PUPPET_STRIP_MESH;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  target = BattleObject_GetFar(args->targets[0])->object;
  // Default: set for real below, when j reaches 32.
  endX = 0;
  endY = 0;
  work->particles[FIRE_PUPPET_SAVED_TARGET].x = target->x;
  work->particles[FIRE_PUPPET_SAVED_TARGET].y = target->y;
  work->particles[FIRE_PUPPET_SAVED_TARGET].z = target->z;
  work->particles[FIRE_PUPPET_SAVED_TARGET].velocityX = target->scaleX;
  work->particles[FIRE_PUPPET_SAVED_TARGET].velocityY = target->scaleY;
  Resource_LoadAndDecompress(RESOURCE_STREAK_TEXTURE, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GHOST_SKULL_TEXTURE,
                             work->sheet + FIRE_PUPPET_GHOST_SKULL, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SHEET),
                       SHEET_PALETTE_BYTES);
  REG_BG2PA = Q_8_8(0.5);
  Render_BuildQuadStrip(buffer, 4, 32, 32);

  for (frame = 0; frame != FIRE_PUPPET_FRAMES; frame++) {
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    if (frame == 0) {
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_GREY, POSE_PARTY_HELD,
                               0, -1);
    }
    if (frame == 8) {
      Sound_PlayFar(SE_CURSE);
    }
    if (frame == 100) {
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, POSE_ENEMY_HELD,
                               0, -1);
    }
    if (frame == 54) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      Sound_PlayFar(SE_POWER_UP);
      EFFECT_SHOW_DAMAGE(SE_POWER_UP);
      work->shakeFrames = 8;
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(2),
                                      toFIXED(4), toFIXED(4), 150);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 8);
      }
    }
    if (frame > 15) {
      s32 length;
      s32 j;
      struct ModelVertex(*cells)[2] =
          (struct ModelVertex(*)[2])Ram_MapCellBuffer;
      u8 *projected = PUPPET_STRIP_VERTICES;
      s32 shade;

      length = frame * 44 - 704;
      if (length > 1424) {
        length = 1424;
      }
      if (frame > 73) {
        length = 4680 - frame * 44;
      }

      // The arm's vertices, thicker at its root.
      for (j = 0; j != PUPPET_SEGMENTS; j++) {
        s32 width = 50 - (FIXEDtoINT(Trig_Sin(j * 862 + 4310) * 44));
        s32 wobble = (FIXEDtoINT(Trig_Sin((frame + j) << 11) << 4)) * j / 32;
        s32 outer;

        i = 0;
        outer = width + wobble;
        do {
          cells[j][i].x =
              (FIXEDtoINT(Trig_Cos(j * length) * (i * width + wobble + 16))) -
              outer / 2 - 16;
          cells[j][i].y = -(
              FIXEDtoINT(Trig_Sin(j * length) * (i * width + j + wobble + 32)));
          cells[j][i].z = 0;
          i++;
        } while (i != 2);
        if (j == PUPPET_SEGMENTS - 1) {
          endX =
              (FIXEDtoINT(Trig_Cos(length << 5) * (width / 2 + wobble + 16))) -
              (width + wobble) / 2 - 16;
          endY = -(FIXEDtoINT(Trig_Sin(length << 5) *
                              (width / 2 + j + wobble + 32)));
        }
      }

      model = Render_AllocateModelList(1);
      shade = 0;
      texture.logWidth = 7;
      texture.logHeight = 5;
      texture.pixels = work->sheet;
      model->kind = MODEL_SHADED_MAX;
      model->mesh = buffer;
      model->vertices = projected;
      model->shade = shade;
      model->texture = &texture;
      EffectPosition_ProjectScaledBattler(args->user, &pos);
      model->scrollV = 0;
      model->scrollU = 127 & ~(frame << 2);
      Matrix_Initialize();
      Matrix_Translate(INTtoFIXED(pos.x / 2 - 64),
                       INTtoFIXED(pos.y) - toFIXED(42), 0);
      Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
      if (args->side == EFFECT_SIDE_PARTY) {
        Matrix_RotateY(DEGtoANG(180));
      }
      Matrix_RotateZ(DEGtoANG(90));
      if (frame > 73) {
        Matrix_ScaleUniform((212 << 10) - (frame << 11));
      }
      Render_TransformVertices(Ram_MapCellBuffer, PUPPET_STRIP_VERTICES, 66);
      Render_DrawModelList(model);

      // The hand at the arm's end.
      model->mesh = gBattleEffect_SquareMesh;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->vertices = projected;
      model->scrollU = 0;
      model->scrollV = 0;
      texture.pixels = work->sheet + FIRE_PUPPET_GHOST_SKULL;
      model->shade = 0;
      Matrix_Translate(INTtoFIXED(endX), INTtoFIXED(endY), 0);
      Matrix_RotateZ(22384 - (length << 5));
      Matrix_ScaleUniform(length * 23 + FIXED_ONE / 2);
      Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(4, 4);
    EFFECT_PRESENT_CANVAS(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#define SWING_FRAMES 52
/// Whiplash's sheet: the whip's slash in five cels, one after another.
#define WHIPLASH_SLASH_A 0
#define WHIPLASH_SLASH_A_WIDTH 48
#define WHIPLASH_SLASH_A_HEIGHT 96
#define WHIPLASH_SLASH_B (WHIPLASH_SLASH_A + PICTURE_BYTES(WHIPLASH_SLASH_A))
#define WHIPLASH_SLASH_B_WIDTH 38
#define WHIPLASH_SLASH_B_HEIGHT 92
#define WHIPLASH_SLASH_C (WHIPLASH_SLASH_B + PICTURE_BYTES(WHIPLASH_SLASH_B))
#define WHIPLASH_SLASH_C_WIDTH 30
#define WHIPLASH_SLASH_C_HEIGHT 76
#define WHIPLASH_SLASH_D (WHIPLASH_SLASH_C + PICTURE_BYTES(WHIPLASH_SLASH_C))
#define WHIPLASH_SLASH_D_WIDTH 56
#define WHIPLASH_SLASH_D_HEIGHT 96
#define WHIPLASH_SLASH_E (WHIPLASH_SLASH_D + PICTURE_BYTES(WHIPLASH_SLASH_D))
#define WHIPLASH_SLASH_E_WIDTH 56
#define WHIPLASH_SLASH_E_HEIGHT 88
#define SWING_HIT_FRAME 18
#define SPARK_COUNT 256

/// A swing. A five-cel slash sweeps across the caster's front, six frames a
/// cel, mirrored on the far side; at frame 18 the target is knocked back and
/// flashes, the result phase starts, and 256 sparks spray from the slash's
/// end toward the target, half of them falling and half rising as they burn
/// down.
///
/// Abilities: Whiplash.
void BattleEffect_RunWhiplash(struct BattleEffectParams *args) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  u8 *canvas;
  struct ScreenPoint caster;
  s32 frame;
  struct Particle *spark;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u8 *sparkCels;
  s32 i;

  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleEffect;
  canvas = heap->battleCanvas;
  sparkCels = heap->battleSheet;

  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = (DrawRectangle)heap->ramCode;
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[0] = (DrawRectangle)heap->ramCode;
  }
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sparkCels, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_SLASH_FLAME_SHEET, work->sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_F),
                       SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != SWING_FRAMES; frame++) {
    // The slash, its cels anchored to the caster's front.
    EffectPosition_ProjectBattler(args->user, &caster);
    if (args->side == EFFECT_SIDE_ENEMIES) {
      switch (frame / 6) {
      case 0:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_A,
                            caster.x / 2 - 48, caster.y - 120);
        break;
      case 1:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_B,
                            caster.x / 2 - 38, caster.y - 120);
        break;
      case 2:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_C,
                            caster.x / 2 - 38, caster.y - 100);
        break;
      case 3:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_D,
                            caster.x / 2 - 64, caster.y - 88);
        break;
      case 4:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_E,
                            caster.x / 2 - 64, caster.y - 88);
        break;
      }
    } else {
      switch (frame / 6) {
      case 0:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_A,
                            caster.x / 2, caster.y - 120);
        break;
      case 1:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_B,
                            caster.x / 2, caster.y - 120);
        break;
      case 2:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_C,
                            caster.x / 2, caster.y - 100);
        break;
      case 3:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_D,
                            caster.x / 2, caster.y - 88);
        break;
      case 4:
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, WHIPLASH_SLASH_E,
                            caster.x / 2, caster.y - 88);
        break;
      }
    }

    // The hit, and the sparks sprayed from 48 pixels out.
    if (frame == SWING_HIT_FRAME) {
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HOP);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      work->shakeFrames = 8;
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 angle;
        s32 speed;

        spark = &((struct Particle *)Ram_MapCellBuffer)[i];
        if (args->side == EFFECT_SIDE_ENEMIES)
          spark->x = INTtoFIXED(caster.x / 2 - 48);
        else
          spark->x = INTtoFIXED(caster.x / 2 + 48);
        spark->y = INTtoFIXED(caster.y - (Random16() & 7));
        angle = (Random16() & 0x7fff) + DEGtoANG(90);
        speed = Random16() & 255;
        if (args->side == EFFECT_SIDE_ENEMIES)
          spark->velocityX = ((Trig_Sin(angle) * speed) >> 4) - toFIXED(8);
        else
          spark->velocityX = ((Trig_Sin(angle) * speed) >> 4) + toFIXED(8);
        spark->velocityY = (Trig_Cos(angle) * (speed + 64)) >> 5;
        spark->variant = (Random16() & 15) + 16;
      }
    }
    if (frame > SWING_HIT_FRAME - 1) {
      for (i = 0; i != SPARK_COUNT; i++) {
        spark = &((struct Particle *)Ram_MapCellBuffer)[i];
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 3) + 1;

          draw[0](canvas, sparkCels + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(spark->x) - size / 2,
                  FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
          if (i & 1)
            Particle_AdvanceWithGravity2D(spark, 60, -toFIXED(1) / 8);
          else
            Particle_AdvanceWithGravity2D(spark, 60, toFIXED(1) / 16);
          spark->variant--;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef BLITTER
#undef FIRE_PUPPET_FRAMES
#undef PUPPET_SEGMENTS
#undef PUPPET_STRIP_VERTICES
#undef PUPPET_STRIP_MESH
#undef SWING_FRAMES
#undef SWING_HIT_FRAME
#undef SPARK_COUNT
