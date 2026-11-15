/// @file
/// The Azul summon, the effect steps under gravity and the projection of effect
/// positions onto the screen.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"

struct MeshTriangle;
struct StripQuad;

/// One face of the serpent's mesh: its three corners and their texture
/// coordinates.
struct AzulFace {
  u8 corner[3];
  u8 unk3;
  u8 uv[3][2];
  u8 unkA[2];
};

/// A sprite object as Azul edits it: its tile number in the low ten bits of
/// the halfword at 8, its priority bits in the byte at 9 and its shape at 18.
struct AzulObject {
  u8 unk0[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 unk08 : 4;
  u8 unk0a[8];
  u16 shape;
};

/// Clamps a sprite's projected y to the screen's band.
#define CLAMP_Y(place)                                                         \
  do {                                                                         \
    if ((place)[2] < -0x10000)                                                 \
      (place)[2] = -0x10000;                                                   \
    if ((place)[2] > 0x9fffff)                                                 \
      (place)[2] = 0xa00000;                                                   \
  } while (0)

/// Azul: water and bubbles rush over the foes, the winged serpent swims in
/// from the right and closes them in a great sphere of water, and the sphere
/// bursts over the field in a wave of foam.
///
/// Abilities: Azul.
void BattleEffect_RunAzul(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  DrawRectangle blit2;
  DrawRectangle blit;
  s32 frame;
  struct BattleWork *battle;
  struct Point2D base;
  struct Point2D pos;
  struct Point2D vel;
  struct ModelVertex *verts;
  struct AzulFace *faces;
  u8 *sphere;
  u8 *grid;
  s32 i;
  s32 j;
  s32 k;
  s32 n;
  u32 t;
  u32 u;
  s32 place[4];
  struct ModelTexture texture;
  struct ModelTexture scratchTexture;
  struct Particle *p;
  struct RenderModel *model;
  void *vertices;
  struct AzulObject *obj;
  const u8 *xs;
  u8 *mapCells;
  u8 *planes;
  s32 stripEnd;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(3);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit = Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  BattleEffect_SelectLivingTargets(work->args);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_FRAME + 126);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  battle = Ram_HeapSlots->battleWork;
  verts = (struct ModelVertex *)work->sheet;
  faces = (struct AzulFace *)(work->sheet + 0x1f0);
  sphere = work->sheet + 0x800;
  grid = work->sheet + 0xc00;
  {
    u8 *cells = gMapGridCellBuffer;

    for (i = 0; i < 120; i++) {
      for (n = 0; n < 0x800; n += 64) {
        Iwram_CopyWordsEntry(
            cells, (void *)(0x06008000 + ((i / 8 * 256 + (i & 7)) * 8 + n)), 8);
        cells += 8;
      }
    }
  }
  {
    u8 *cells = work->sheet + 0x2bb8;
    u8 *dst;
    s32 count;

    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet, cells, 1, 1);
    i = 0;
    count = 288;
    dst = work->sheet + 0x2df8;
    for (; i != count; i++) {
      *dst++ = *cells;
      cells += 2;
    }
  }
  wait(1);
  BattleEffect_SpawnObjects(12, 0x28c, 1);
  for (i = 0; i != 16; i++) {
    obj = EffectSprite_Acquire(32, 32, 0x80002000, 0);
    work->sprites[i + 12] = obj;
    Iwram_CopyWordsEntry(obj, work->sprites[5 + (i & 3)], 24);
    ((struct AzulObject *)work->sprites[i])->priority = 1;
  }
  mapCells = gMapGridCellBuffer;
  planes = gData_02014000;
  stripEnd = 0x3a08;
  for (frame = 0; frame != 228; frame++) {
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 15 && frame <= 185) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
      EFFECT_PRESENT_CANVAS(work);
      frame = 186;
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->sprites[i])->shape = 0x4000;
      }
    }
    if (frame <= 8) {
      s32 fade = 24 - frame * 3;

      ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                           0x10000 - fade * 1092, 128);
    }
    if (frame == 186) {
      base.x = px(48);
      base.y = 0x420003;
      vel.x = 0;
      vel.y = -px(6);
    }
    if (frame == 0) {
      REG_BLDALPHA = BLEND_ALPHA(16, 0);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, frame);
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->sprites[i])->shape = 0xc000;
      }
      base.x = px(120);
      base.y = -px(120);
      vel.x = 0;
      vel.y = px(12);
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
      for (n = 0; n != 31; n++) {
        for (i = 0; i != 4; i++) {
          verts[n * 4 + i].x =
              gAzul_PartPoints[n * 5 + i + 1][0] - gAzul_PartPoints[n * 5][0];
          verts[n * 4 + i].y =
              gAzul_PartPoints[n * 5 + i + 1][1] - gAzul_PartPoints[n * 5][1];
          verts[n * 4 + i].z = 0;
        }
        faces[n * 3].corner[1] = 1;
        faces[n * 3].corner[2] = 2;
        faces[n * 3].corner[0] = 0;
        faces[n * 3 + 1].corner[1] = 2;
        faces[n * 3 + 1].corner[2] = 3;
        faces[n * 3 + 1].corner[0] = 0;
        faces[n * 3 + 2].corner[0] = 0;
        faces[n * 3 + 2].corner[1] = 0;
        faces[n * 3 + 2].corner[2] = 0;
        for (i = 0; i != 3; i++) {
          faces[n * 3].uv[i][0] =
              gAzul_PartPoints[n * 5 + faces[n * 3].corner[i] + 1][0];
          faces[n * 3].uv[i][1] =
              gAzul_PartPoints[n * 5 + faces[n * 3].corner[i] + 1][1];
          faces[n * 3 + 1].uv[i][0] =
              gAzul_PartPoints[n * 5 + faces[n * 3 + 1].corner[i] + 1][0];
          faces[n * 3 + 1].uv[i][1] =
              gAzul_PartPoints[n * 5 + faces[n * 3 + 1].corner[i] + 1][1];
        }
      }
      for (i = 0; i != args->targetCount; i++) {
        ((s32 *)BattleObject_GetFar(args->targets[i])->object)[3] = 0;
      }
      REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                   BGCNT_SCREEN_BASE(7);
    }
    t = frame - 10;
    if (t <= 16) {
      REG_BLDALPHA = t << 8 | 0x10;
    }
    if ((u32)(frame - 16) <= 63) {
      if (frame == 16) {
        s32 mask;

        i = 0;
        mask = 31;
        p = &work->particles[31];
        for (; i != 27; i++, p++) {
          p->x = px((Random16() & 63) + 32);
          p->y = px((Random16() & 15) + 120);
          p->velocityX = ((Random16() & mask) - 16) << 13;
          p->velocityY = ((Random16() & mask) - 63) << 12;
          p->variant = (Trig_Cos(i << 9) << 5 >> 16) - 32;
        }
      }
      i = 0;
      p = &work->particles[31];
      for (; i != 27; i++, p++) {
        if (p->variant >= 0) {
          s32 s = Trig_Sin((frame << 10) + i * 0x1a16);

          blit(canvas, work->sheet + 0x2df8, ((p->x + (s << 4)) >> 16) - 6,
               Q_16_16_UPPER_HALF(p->y) - 12, 12, 24);
          BattleEffectKernels_IntegrateVector2(p, 63, -px(1) / 4);
        }
        p->variant++;
      }
    }
    if (frame <= 95 || (u32)(frame - 160) <= 31) {
      u16 *c = (u16 *)(work->unk7300 + 0x340);
      s32 level = frame / 8;

      if (frame > 159) {
        level = (frame - 160) / 4 + 12;
        if (level > 20) {
          level = 20;
        }
      } else if (level > 12) {
        level = 12;
      }
      for (i = 0; i != 15; i++) {
        *c++ = 0;
      }
      for (; i != 135; i++) {
        s32 d = i - 15;
        s32 b = -(d / 15) - level + 40;
        s32 g = -(d / 16) - level + 32;
        s32 r = -(d / 17) - level + 16;

        if (b < 0) {
          b = 0;
        }
        if (b > 31) {
          b = 31;
        }
        if (g < 0) {
          g = 0;
        }
        if (g > 31) {
          g = 31;
        }
        if (r < 0) {
          r = 0;
        }
        if (r > 31) {
          r = 31;
        }
        *c++ = b << 10 | g << 5 | r;
      }
      for (; i != 160; i++) {
        *c++ = 0;
      }
    }
    // DEAD: frame never falls below 0; the source keeps the test.
    if (frame >= 0) {
      base.x += vel.x;
      base.y += vel.y;
    }
    if (frame == 168) {
      base.x = px(48);
      base.y = px(168);
      vel.x = 0;
      vel.y = -px(6);
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->sprites[i])->shape = 0x4000;
      }
      for (i = 0; i != args->targetCount; i++) {
        s32 *actor = BattleObject_GetFar(args->targets[i])->object;

        BattleEffect_StartActorMotion(args->targets[i], 1, 0, 0, 0x40000, 100);
        actor[18] = 0;
      }
    }
    if (frame == 186) {
      vel.y = -px(16);
    }
    if ((frame < 54 || frame > 167) && frame < 214) {
      place[3] = 0;
      place[1] = px(255);
      if (frame > 167) {
        pos.y -= px(6);
      }
      if (frame <= 53) {
        for (i = 0; i != 12; i++) {
          place[0] = px(40 - gAzul_ObjectX[i]) + base.x;
          place[2] = px(128 - gAzul_ObjectZ[i] * 31 / 32) + base.y;
          if (i > 8) {
            place[2] += -px(240);
          }
          CLAMP_Y(place);
          Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                            (s32 *)&gAzul_UnitScale, 0);
        }
        for (i = 0; i != 16; i++) {
          place[0] = px(39 - gAzul_ObjectX[(i & 3) + 5]) + base.x;
          place[2] = px(-(gAzul_ObjectZ[(i & 3) + 5] * 30 / 32) - i / 4 * 60) +
                     base.y + px(66);
          // CLAMP_Y written out: the macro's once-only block holds the clamp's
          // constant after the store (4 of 2209 differ).
          if (place[2] < -px(1))
            place[2] = -px(1);
          if (place[2] > 0x9fffff)
            place[2] = px(160);
          obj = work->sprites[i + 12];
          obj->tile = ((struct AzulObject *)work->sprites[(i & 3) + 5])->tile;
          Render_ApplyProjectedPlacementFar(obj, place, (s32 *)&gAzul_UnitScale,
                                            0);
        }
      } else {
        for (i = 0; i != 12; i++) {
          place[0] = px(gAzul_ObjectX[i]) + base.x;
          place[2] = px(gAzul_ObjectZ[i]) + base.y;
          if (i > 8) {
            place[2] += px(256);
          }
          CLAMP_Y(place);
          Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                            (s32 *)&gAzul_UnitScale, 0);
        }
        i = 0;
        xs = gAzul_ObjectX;
        for (; i != 16; i++) {
          s32 m = i & 3;
          s32 row = m + 5;

          place[0] = px(xs[row]) + base.x - px(1);
          place[2] = px(gAzul_ObjectZ[row] + i / 4 * 64) + base.y + px(64);
          CLAMP_Y(place);
          obj = work->sprites[i + 12];
          obj->tile = ((struct AzulObject *)work->sprites[m + 5])->tile;
          Render_ApplyProjectedPlacementFar(obj, place, (s32 *)&gAzul_UnitScale,
                                            0);
        }
      }
    } else if (frame >= 54 && frame < 168) {
      s32 x;
      s32 y;

      if (frame == 54) {
        for (i = 0; i != 28; i++) {
          ((struct AzulObject *)work->sprites[i])->shape = 0;
        }
        base.x = px(256);
        base.y = px(72);
        vel.x = -px(6);
        vel.y = 0;
      }
      if (frame == 138) {
        vel.x = px(8);
      }
      place[3] = 0;
      place[1] = px(255);
      vel.x = vel.x * 56 / 64;
      vel.y = vel.y * 56 / 64;
      if (frame <= 137) {
        pos.x = (Trig_Sin(frame << 9) << 4) + base.x;
        x = pos.x;
        pos.y = (Trig_Cos(frame << 9) << 3) + base.y;
        y = pos.y;
      } else {
        x = base.x + (Trig_Sin(frame << 9) << 4);
        y = (Trig_Cos(frame << 9) << 3) + base.y;
      }
      for (i = 0; i != 8; i++) {
        place[0] = px(gAzul_LateObjectX[i]) + x;
        place[2] = px(gAzul_LateObjectZ[i]) + y;
        Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                          (s32 *)&gAzul_LateUnitScale, 0);
      }
    }
    if ((u32)(frame - 138) <= 29) {
      s32 d = -0x20000;

      if (frame > 151) {
        d = (frame << 13) + -0x150000;
      }
      pos.x = pos.x + d;
    }
    if (frame == 78) {
      Sound_PlayFar(SE_SPIN);
    }
    if (frame == 138) {
      Sound_PlayFar(SE_104);
    }
    if (frame == 152) {
      Sound_PlayFar(SE_163);
    }
    if (frame == 7) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 10) {
      const u8 *piece;

      Sound_PlayFar(SE_CLATTER);
      i = 0;
      p = work->particles;
      piece = gAzul_PartPoints[0];
      for (; i != 31; i++, piece += 10, p++) {
        s32 cx = piece[0] >> 1;
        s32 cy = piece[1];

        SET_POSITION(p, px(cx), px(cy), 0);
        p->velocityX = (cx - 60) << 12;
        p->velocityY = (cy - 60) << 10;
        p->velocityZ = ((Random16() & 127) << 2) + 512;
        if (Random16() & 1) {
          p->velocityZ = -p->velocityZ;
        }
      }
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
    }
    if (frame == 10 || frame == 186) {
      for (i = 0; i != args->targetCount; i++) {
        struct BattleObject *slot = BattleObject_GetFar(args->targets[i]);
        s32 *actor = slot->object;
        void *record;

        if (frame == 186) {
          ReleaseBattleObjectRecordsFar(args->targets[i]);
        }
        if (frame == 10) {
          for (j = 0; (record = GetMotionRecordFar(slot->object, j)) != 0;
               j++) {
            Sprite_RemoveAnimationFar(record, (s32)slot->shadowEntry);
          }
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_BLUE_TINT,
                                   POSE_DAMAGE, i, 8);
          BattleEffect_StartActorMotion(args->targets[i], 1, 0, 0,
                                        (Random16() << 1) + 0x10000, 100);
          // record is 0 here; the store reuses its register.
          actor[18] = (s32)record;
        }
      }
      if (frame == 186) {
        args->targets[args->targetCount] = 0xff;
        BattleObject_SpawnObjectsForListFar(args->targets, 0, 0);
      }
    }
    if (frame > 9) {
      for (i = 0; i != args->targetCount; i++) {
        s32 *actor = BattleObject_GetFar(args->targets[i])->object;

        actor[10] = actor[10] * 60 / 64;
      }
    }
    if (t <= 43) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 8;
      texture.logHeight = 7;
      texture.pixels = mapCells;
      model->texture = &texture;
      model->kind = 8;
      model->vertices = vertices;
      model->scrollU = gBgScroll[1].x;
      for (i = 0; i != 31; i++) {
        p = &work->particles[i];
        model->mesh = &faces[i * 3];
        Transform_Reset();
        SceneTransform_ApplyPosition3(p->x - px(64), p->y - px(64), 0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        switch (i & 3) {
        case 0:
          Transform_ApplyPitch(p->z);
          Transform_ApplyRoll(p->z);
          break;
        case 1:
          Transform_ApplyPitch(p->z);
          Transform_ApplyRoll(-p->z);
          break;
        case 2:
          Transform_ApplyPitch(p->z);
          Transform_ApplyYaw(p->z);
          break;
        case 3:
          Transform_ApplyRoll(p->z);
          Transform_ApplyYaw(-p->z);
          break;
        }
        Render_TransformVertices(&verts[i * 4], vertices, 4);
        BattleEffectKernels_IntegrateVector3(p, 62, 0);
        if (frame >= (8 - i) * 2 + 16) {
          p->velocityY += -(px(1) / 2);
        }
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    if (frame == 78) {
      s32 angle;
      s32 offset;

      Mesh_BuildGrid(grid, 16, 16, 8, 8);
      k = 0;
      angle = 0;
      offset = 0;
      for (; k != 16; angle += 0x888, offset += 64, k++) {
        s32 y = -Trig_Cos(angle) * 48 >> 16;
        s32 radius = Trig_Sin(angle) * 48 >> 16;
        struct ModelVertex *v;

        j = 0;
        v = (struct ModelVertex *)(sphere + offset);
        for (; j != 16; j++) {
          SET_POSITION(v, Trig_Sin(j << 12) * radius >> 16, y,
                       Trig_Cos(j << 12) * radius >> 16);
          v++;
        }
      }
      Resource_LoadAndDecompress((s32)&ResourceId_WispSheet, gData_02014000, 1,
                                 0);
      for (i = 0; i != 6; i++) {
        for (k = 0; k != 16; k++) {
          for (j = 0; j != 64; j++) {
            ((u8 *)work)[stripEnd + (i * 64 - j) * 16 + k] =
                (planes + i * 2048 + k * 128)[j];
          }
        }
      }
      Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneASheet,
                                 gData_02014000, 1, 0);
      Picture_MirrorIntoQuadrants(gData_02014000, gMapGridCellBuffer, 64, 64);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0,
                                 0);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 152) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, gData_02014000, 0,
                                 0);
    }
    u = frame - 80;
    if (u <= 107) {
      s32 scale;
      s32 shade;
      s32 size;

      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      if ((u32)(frame - 168) <= 17) {
        s32 y;
        s32 f;
        s32 width;
        s32 height;

        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            BLITTER_ADDITIVE);
        blit2 = Ram_HeapSlots->ramCode2;
        i = 0;
        y = (base.y >> 16) - 32;
        width = 16;
        height = 64;
        f = frame;
        for (; i != 2; i++, f += 3) {
          u8 *cells = work->sheet + 0x2f18 + (f % 6 << 10);

          blit(canvas, cells, 30, y, width, height);
          blit2(canvas, cells, 46, y, width, height);
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      }
      if (frame > 185) {
        goto spray;
      }
      model = Render_AllocateModelList(1);
      scale = u * 3 << 8;
      shade = 0;
      if (scale > Q16_ONE) {
        scale = Q16_ONE;
      }
      scratchTexture.logWidth = 7;
      scratchTexture.logHeight = 7;
      scratchTexture.pixels = mapCells;
      model->kind = 7;
      model->texture = &scratchTexture;
      model->mesh = grid;
      model->vertices = verts;
      model->shade = shade;
      model->scrollU = shade;
      model->scrollV = shade;
      Transform_Reset();
      SceneTransform_ApplyPosition3(pos.x / 2 - px(68), pos.y - px(80), 0);
      SceneTransform_ApplyScale3(scale, scale * 2, scale);
      SceneTransform_ApplyPosition3(-px(40), 0, 0);
      Transform_ApplyPitch(frame << 10);
      Transform_ApplyYaw(frame << 10);
      Render_TransformVertices(sphere, verts, 256);
      Render_DrawModelList(model);
      if (frame > 137) {
        shade = 0x5a4 - frame * 10;
        size = (frame << 13) + -0x110000;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > -64) {
          scratchTexture.pixels = gData_02014000;
          scratchTexture.logWidth = 7;
          model->mesh = gBattleEffect_LargeSquareMesh;
          /* FAKEMATCH(dead): a once-only block. */
          do {
            scratchTexture.logHeight = 7;
          } while (0);
          model->shade = shade;
          Transform_Reset();
          SceneTransform_ApplyPosition3(px(44), 0, 0);
          Transform_ApplyYaw(0x3448);
          SceneTransform_ApplyUniformScale(size);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
      if (frame > 151) {
        work->shakeFrames = 2;
        shade = 0;
        size = (frame << 13) + -0x130000;
        scratchTexture.logWidth = 6;
        scratchTexture.logHeight = 6;
        scratchTexture.pixels = gData_02014000;
        model->shade = shade;
        model->mesh = gBattleEffect_SquareMesh;
        Transform_Reset();
        SceneTransform_ApplyPosition3(-px(16), pos.y - px(80), 0);
        SceneTransform_ApplyScale3(size >> 1, size, size >> 1);
        Transform_ApplyRoll(frame << 11);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
    }
    if (frame > 185) {
    spray:
      if (frame == 186) {
        struct Particle *drop = (struct Particle *)gMapGridCellBuffer;

        for (i = 0; i != 256; i++, drop++) {
          s32 angle = i << 8;
          s32 r = Random16() & 127;
          s32 vy;

          drop->velocityX = Trig_Sin(angle) * r >> 5;
          vy = Trig_Cos(angle) * r >> 4;
          drop->x = drop->velocityX * 6 + px(48);
          drop->y = vy * 5;
          drop->velocityY = vy;
          drop->variant = (Random16() & 31) + 32;
        }
      }
      p = (struct Particle *)gMapGridCellBuffer;
      for (i = 0; i != 256; i++, p++) {
        if (p->variant >= 0) {
          blit(canvas, work->sheet + 0x2df8, Q_16_16_UPPER_HALF(p->x) - 6,
               Q_16_16_UPPER_HALF(p->y) - 12, 12, 24);
          BattleEffectKernels_IntegrateVector2(p, 63, px(1));
          if (p->y > px(96)) {
            p->velocityY = -p->velocityY / 2;
            p->velocityX <<= 2;
            p->variant -= 8;
          }
        }
        p->variant--;
      }
    }
    if (frame == 186) {
      BattleBackground_LoadFar(1, battle->background, 24);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
      for (i = 0; i != args->targetCount; i++) {
        EFFECT_STRIKE_TARGET(args, i, 16, KNOCKBACK_HURL);
      }
      Sound_PlayFar(SE_CRASH);
      work->shakeFrames = 16;
    }
    if (frame == 214) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame > 185) {
      BattleCamera_Shake(8, 8);
    } else {
      BattleCamera_Shake(2, 2);
    }
    if (frame > 185) {
      BattleEffect_CountDownBattlerTints();
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmPaletteHBlankDma);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  for (i = 0; i != 28; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

/// Moves a particle one frame in three dimensions.
void Particle_AdvanceWithGravity3D(struct Particle *step, s32 damping,
                                   s32 gravity) {
  s32 vx = step->velocityX;
  s32 vy = step->velocityY;
  s32 vz = step->velocityZ;

  step->x += vx;
  step->y += vy;
  step->z += vz;
  SET_VELOCITY(step, vx * damping / 64, (vy + gravity) * damping / 64,
               vz * damping / 64);
}

/// Moves a particle one frame on the screen.
void Particle_AdvanceWithGravity2D(struct Particle *step, s32 damping,
                                   s32 gravity) {
  s32 x = step->x;
  s32 vx = step->velocityX;
  s32 vy = step->velocityY;

  step->x = x + vx;
  step->y += vy;
  step->velocityX = vx * damping / 64;
  step->velocityY = (vy + gravity) * damping / 64;
}

/// Projects a 16.16 point onto the screen through the current transform, 16
/// pixels higher; returns Render_ProjectPoint's result.
s32 EffectPosition_ProjectPoint(const void *point,
                                struct ScreenPoint *position) {
  s32 result = Render_ProjectPoint(point, ScreenPoint_Coordinates(position));
  position->y -= 16;
  return result;
}

/// Projects a unit's sprite onto the screen, raised by half its height, 16
/// pixels higher.
void EffectPosition_ProjectSprite(s32 id, struct ScreenPoint *position) {
  BattleUnit_ProjectToScreen(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// Projects a unit's place onto the screen at its scale, 16 pixels higher.
void EffectPosition_ProjectScaledBattler(s32 id, struct ScreenPoint *position) {
  BattleMotion_ProjectScaledPositionFar(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// Projects a unit's place onto the screen, 16 pixels higher.
void EffectPosition_ProjectBattler(s32 id, struct ScreenPoint *position) {
  BattleMotion_ProjectPositionFar(id, ScreenPoint_Coordinates(position));
  position->y -= 16;
}

/// Midpoint between the first and last targets.
void BattleEffect_GetTargetsMidpoint(struct ScreenPoint *position) {
  struct BattleEffectWork *work;
  struct ScreenPoint first;
  struct ScreenPoint last;

  work = Ram_HeapSlots->battleEffect;
  EffectPosition_ProjectBattler(work->args->targets[0], &first);
  EffectPosition_ProjectBattler(
      work->args->targets[work->args->targetCount - 1], &last);
  position->x = first.x + (last.x - first.x) / 2;
  position->y = first.y;
  position->depth = first.depth;
}
#undef BLITTER
#undef CLAMP_Y
