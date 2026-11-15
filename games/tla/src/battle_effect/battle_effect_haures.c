/// @file
/// The Haures summon, and the open cylinder a summon's band is drawn on.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

/// Two corners of a wing: its upper edge, then its lower.
struct WingVertex {
  struct ModelVertex upper;
  struct ModelVertex lower;
};

#define SCRATCH (Ram_MapCellBuffer + 0x4000)
extern struct WingVertex gHaures_WingCorners[];
extern const u16 gHaures_BodyPiecePoints[][2];

#define SPARKS ((struct Particle *)SCRATCH)
#define STAMP_TILES(work) (*(u32 *)((u8 *)(work) + 0x7b74))
#define STAMP_CANVAS(work) ((u8 *)(work) + 0x6780)

static __inline__ void PLACE(s32 x, s32 y) {
  Transform_ApplyTranslation(px(x), px(y), 0);
}
#define HAURES_FRAMES 92
/// Haures's sprites: the demon's eleven pieces (SPRITE_HAURES), four violet
/// flames (sprite 660) and two small dots (sprite 661), then the 64x64 stamp.
#define HAURES_BODY_PIECES 11
#define HAURES_FLAMES 12
#define HAURES_FLAME_COUNT 4
#define HAURES_DOTS 16
#define HAURES_DOT_COUNT 2
#define HAURES_STAMP 18
/// The wings: a strip of 32 points, upper and lower edge each, so 31 quads.
#define HAURES_WING_POINTS 32
#define HAURES_WING_QUADS (HAURES_WING_POINTS - 1)

/// Haures: in a blue cavern a dark pool ringed in red swells into a mound under
/// a violet glow and a rune flashes; the demon rears in a burning cave, then
/// charges through the foes in streaks of fire that end in a white flash.
///
/// Abilities: Haures.
void BattleEffect_RunHaures(struct BattleEffectParams *args) {
  struct BattleWork *battle;
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  s32 *transition;
  struct Point2D position;
  struct Point2D velocity;
  s32 morph;
  s32 morphStep;
  s32 dived;
  s32 i;
  s32 wingPlace[4];
  s32 bodyPlace[4];
  struct Scale req;
  struct Scale openingScale;
  struct Scale orbitScale;
  struct ModelTexture texture;
  struct ModelTexture burstTexture;
  struct ModelTexture wideTexture;
  struct ModelTexture bandTexture;
  struct FieldSprite *sprite;
  const u8 *particleX;
  u8 *buffer;
  u8 *shades;
  u16 *dst;

  buffer = gMapGridCellBuffer;
  shades = gHaures_WingShades;
  battle = Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  transition = (s32 *)Ram_HeapSlots->battleBackground;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  REG_BLDCNT = BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG2X = 0x400;
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_SelectLivingTargets(work->args);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Task_Add(BattleEffect_ProcessPendingStampTransfer, TASK_PASS_FRAME);
  morph = 0;
  morphStep = 0;
  dived = 0;
  BattleEffect_SpawnObjects(2, SPRITE_HAURES, 2);
  for (i = 0; i != HAURES_FLAME_COUNT; i++) {
    sprite = Sprite_CreateFar(0x294);
    work->sprites[HAURES_FLAMES + i] = sprite;
    if (sprite != NULL) {
      sprite->nextFrame = 0;
      Sprite_StartAnimationFar(sprite, 0);
      {
        struct FieldSprite *added = work->sprites[HAURES_FLAMES + i];

        added->priority = 1;
        added->blendMode = 1;
      }
    }
  }
  for (i = 0; i != HAURES_DOT_COUNT; i++) {
    sprite = Sprite_CreateFar(0x295);
    work->sprites[HAURES_DOTS + i] = sprite;
    if (sprite != NULL) {
      sprite->nextFrame = 0;
      Sprite_StartAnimationFar(sprite, 0);
      {
        struct FieldSprite *added = work->sprites[HAURES_DOTS + i];

        added->priority = 1;
      }
    }
  }
  sprite =
      EffectSprite_CreateWithTiles(64, 64, OBJ_SIZE_64X64 | OBJ_256_COLORS, 0);
  ((struct FieldSprite **)work->sprites)[HAURES_STAMP] = sprite;
  sprite->blendMode = 1;
  STAMP_TILES(work) = gVramCacheEntries[sprite->vramBlock].offset + OBJ_VRAM0;
  {
    u8 *c = work->sheet;

    Resource_LoadAndDecompress((s32)&ResourceId_Data0F7, c, 0, 0);
    Picture_MirrorIntoQuadrants(c, SCRATCH, 64, 64);
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, c, 1, 0);
    for (i = 0; i != 0x1781; i++, c++) {
      u8 v = *c;

      if (v != 0) {
        *c = (v >> 2) + 224;
      }
    }
  }
  {
    u32 *w;
    u32 mask;

    Resource_LoadAndDecompress((s32)&ResourceId_Data0B4,
                               w = (u32 *)(work->sheet + 0x4000), 0, 0);
    mask = 0x7f7f7f7f;
    for (i = 0; i != 1024; i++) {
      w[i] = (w[i] >> 1) & mask;
    }
  }
  for (i = 0; i != HAURES_WING_QUADS; i++) {
    gHaures_WingStrip[i * 8 + 0] = i * 2;
    gHaures_WingStrip[i * 8 + 1] = i * 2 + 1;
    gHaures_WingStrip[i * 8 + 2] = i * 2 + 2;
    gHaures_WingStrip[i * 8 + 4] = i * 2 + 1;
    gHaures_WingStrip[i * 8 + 5] = i * 2 + 3;
    gHaures_WingStrip[i * 8 + 6] = i * 2 + 2;
  }
  // The strip ends with a face of no corners, cleared through the end's own
  // name.
  Data_02010be0[0] = 0;
  Data_02010be0[1] = 0;
  Data_02010be0[2] = 0;
  {
    dst = &BG_PLTT_COLOR(0, 1);

    for (i = 1; i != 64; i++) {
      s32 b;
      s32 r;
      s32 g;

      if (i <= 31) {
        b = 0;
        g = 0;
        r = i;
        if (i < 0) {
          r = 0;
        }
        *dst = r;
      } else {
        s32 t = i - 32;

        b = t * 4;
        g = t;
        r = b;
        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        }
      }
      *dst++ = RGB(r, g, b);
    }
  }
  {
    dst = &OBJ_PLTT_COLOR(14, 0);

    for (i = 0; i != 16; i++, dst++) {
      s32 b = i * 3 + 2;
      s32 r = i * 2 + 2;

      if (r > RGB_LEVEL_MAX) {
        r = RGB_LEVEL_MAX;
      }
      if (b > RGB_LEVEL_MAX) {
        b = RGB_LEVEL_MAX;
      }
      *dst = b << 10 | (i + 2) << 5 | r;
    }
  }
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_MossCaveBackdrop, 0);
  frame = 0;
  if (!(gInput.repeat & (KEY_A | KEY_B))) {
    do {
      s32 cut = frame - 267;

      if (frame == 0) {
        work->transferValue = frame;
        position.x = px(108);
        position.y = px(120);
        velocity.x = 0;
        velocity.y = 0;
        Iwram_CopyWordsEntry((void *)&BG_PLTT_COLOR(6, 0), battle->palette,
                             256);
        {
          u8 *q = shades;

          for (i = 0; i != HAURES_WING_QUADS; i++, q += 2) {
            q[0] = 1;
            q[1] = 1;
          }
        }
        particleX = gHaures_ParticleX;
        {
          struct Particle *p = work->particles;

          for (i = 0; i != 4; i++, p++, particleX++) {
            p->x = px(*particleX);
            p->y = px(16);
            p->velocityX = 0;
            p->velocityY = 0;
            p->variant = 0;
          }
        }
        {
          for (i = 0; i != 8; i++) {
            struct Particle *p = &work->particles[i + 4];
            p->x = (Random16() & 15) + 24;
            p->y = Random16() & 7;
            p->variant = i * 3;
            p->velocityX = 0;
            p->velocityY = 0;
          }
        }
        {
          struct Particle *p = &work->particles[63];

          p->x = px(60);
          p->y = px(112);
          p->variant = 0;
        }
        for (i = 0; i != HAURES_WING_POINTS; i++) {
          gHaures_WingCorners[i].upper.x =
              Q_16_16_TO_INT(-(Trig_Cos(i * 992) << 6));
          gHaures_WingCorners[i].upper.y =
              Q_16_16_TO_INT(-(Trig_Sin(i * 992) * 18));
          gHaures_WingCorners[i].lower.x = i * 4 - 64;
          gHaures_WingCorners[i].upper.z = 0;
          gHaures_WingCorners[i].lower.x =
              Q_16_16_TO_INT(-(Trig_Cos(i * 992) << 6));
          gHaures_WingCorners[i].lower.y = (u32)(Trig_Sin(i * 992) * 9) >> 15;
          gHaures_WingCorners[i].lower.z = 0;
        }
      }
      if ((u32)frame <= 31) {
        s32 a = frame * 2 / 3 - 35;

        Palette_BrightenSavedBattleBg(a, a, frame - 35);
      }
      if ((u32)(frame - 242) <= 21 && (frame & 1)) {
        Palette_BrightenBattleBg(1, 0, 0);
      }
      if (frame == 277) {
        Palette_BrightenSavedBattleBg(8, -12, -12);
      }
      if (frame > 359) {
        Palette_BrightenSavedBattleBg(8 - (frame - 360) / 4, -12,
                                      (frame - 360) / 8 - 12);
      }
      if (frame == 48) {
        Sound_PlayFar(SE_WHOOSH);
      }
      if (frame == 74) {
        Sound_PlayFar(SE_WHOOSH);
      }
      if (frame == 100) {
        Sound_PlayFar(SE_WHOOSH);
      }
      if (frame == 126) {
        Sound_PlayFar(SE_WHOOSH);
      }
      if (frame == 140) {
        Sound_PlayFar(SE_RUMBLE);
      }
      if (frame <= 266) {
        if (frame <= 639) {
          wingPlace[1] = 0;
          wingPlace[3] = 0;
          {
            struct Particle *p = work->particles;

            for (i = 0; i != 4; i++, p++) {
              if (frame > i * 26) {
                if (p->variant == 0) {
                  wingPlace[0] = p->x;
                  wingPlace[2] = p->y;
                  Render_ApplyProjectedPlacementFar(
                      work->sprites[HAURES_DOTS + (i & 1)], wingPlace,
                      gHaures_UnitScale, 0);
                  BattleEffectKernels_IntegrateVector2(p, 63, px(1) / 8);
                  if (Q_16_16_UPPER_HALF(p->y) > 119) {
                    p->variant = 1;
                  }
                } else if (p->variant <= 23) {
                  wingPlace[0] = p->x;
                  wingPlace[2] = p->y;
                  Sprite_StartAnimationFar(
                      work->sprites[HAURES_FLAMES + (i & 1)],
                      (p->variant / 6) & 3);
                  Render_ApplyProjectedPlacementFar(
                      work->sprites[HAURES_FLAMES + (i & 1)], wingPlace,
                      gHaures_UnitScale, 0);
                  p->variant++;
                }
              }
            }
          }
        }
        if (frame > 46) {
          struct RenderModel *model;
          void *verts;
          s32 t;
          s32 sh;
          s32 s;
          u8 *strip = gHaures_WingStrip;

          verts = Heap_AllocEwram(32);
          model = Render_AllocateModelList(1);
          texture.logWidth = 6;
          t = frame - 47;
          texture.logHeight = 6;
          texture.pixels = work->sheet + 0x4000;
          sh = frame - 111;
          if (sh > 0) {
            sh = 0;
          }
          model->kind = MODEL_SHADED_MAX;
          model->shade = sh;
          model->texture = &texture;
          model->mesh = gBattleEffect_SquareMesh;
          model->vertices = verts;
          Transform_Reset();
          Transform_ApplyTranslation(0, px(36), 0);
          Transform_ApplyPitch(4000);
          Transform_ApplyYaw(-t << 8);
          s = t << 10;
          if (s > 60000) {
            s = 60000;
          }
          Transform_ApplyUniformScale(s);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = work->sheet + 0x1000;
          model->kind = MODEL_SHADED;
          model->texture = &texture;
          model->mesh = gBattleEffect_SmallSquareMeshReversed;
          model->vertices = verts;
          model->shade = sh;
          Transform_Reset();
          Transform_ApplyTranslation(0, px(36), 0);
          s = (t * 3 << 8) + 1024;
          if (s > 38768) {
            s = 38768;
          }
          Transform_ApplyUniformScale(s);
          if (frame > 73) {
            s32 a;
            s32 b;
            s32 n;
            s32 next;
            struct WingVertex *wing = gHaures_WingCorners;

            if (frame == 74) {
              morph = 0;
              morphStep = 0;
            }
            a = morph % 13;
            next = morph + 1;
            b = next % 13;
            n = morphStep;
            morphStep = n + 1;
            if (morphStep == gHaures_WingMorphSteps[a]) {
              morphStep = 0;
              morph = next;
            }
            for (i = 0; i != HAURES_WING_POINTS; i++) {
              wing[i].upper.y =
                  -(gHaures_WingShapes[i + a * HAURES_WING_POINTS] +
                    n *
                        (gHaures_WingShapes[i + b * HAURES_WING_POINTS] -
                         gHaures_WingShapes[i + a * HAURES_WING_POINTS]) /
                        gHaures_WingMorphSteps[a]);
            }
            for (i = 0; i != HAURES_WING_POINTS; i++) {
              gHaures_WingCorners[i].upper.x = i * 4 - 64;
            }
          }
          model->flags = 0;
          model->kind = MODEL_SOLID;
          model->mesh = strip;
          model->vertices = buffer;
          if ((u32)(frame - 236) <= 31) {
            s32 v = frame - 235;

            for (i = 0; i != HAURES_WING_QUADS; i++) {
              gHaures_WingShades[i * 2] = v;
              gHaures_WingShades[i * 2 + 1] = v;
            }
          }
          model->shade = (s32)shades;
          Render_TransformVertices(gHaures_WingCorners, buffer, 64);
          Render_DrawModelList(model);
          Heap_Release(model);
          Heap_Release(verts);
        }
        req = gHaures_ChargeScale;
        bodyPlace[3] = 0;
        if (frame > 149) {

          position.x += velocity.x;
          position.y += velocity.y;
          if (frame <= 159) {
            velocity.y += -px(1) / 4;
          } else if ((u32)(frame - 210) <= 13) {
            velocity.x += -px(1) / 32;
            velocity.y += -px(1) / 16;
          } else if ((u32)(frame - 236) <= 3) {
            velocity.x += px(1) / 8;
            velocity.y += -px(1) / 4;
          }
          velocity.x = velocity.x * 60 / DAMPING_SCALE;
          velocity.y = velocity.y * 60 / DAMPING_SCALE;
          req.x = px(1);
          req.y = px(1);
          for (i = 0; i != 2; i++) {
            bodyPlace[0] = position.x;
            bodyPlace[1] = px(272) - (position.y + i * px(32));
            bodyPlace[2] = px(256);
            Render_ApplyProjectedPlacementFar(work->sprites[i], bodyPlace,
                                              (s32 *)&req, 0);
          }
          if ((u32)(frame - 150) <= 116) {
            BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 6, 6, 3, 1);
            draw[1] = Ram_HeapSlots->ramCodePaired;
            {
              for (i = 0; i != 8; i++) {
                struct Particle *p = &work->particles[i + 4];
                s32 n = (p->variant / 4) % 6;
                u8 *src = work->sheet + gBattleEffect_PuffArcPictures[n];
                u32 w = gBattleEffect_PuffArcPictureWidths[n];

                draw[1](STAMP_CANVAS(work), src, p->x - (w >> 1),
                        p->y + gBattleEffect_PuffArcPictureBiasY[n], w,
                        gBattleEffect_PuffArcPictureHeights[n]);
                if (++p->variant == 24) {
                  p->variant = 0;
                  p->x = (Random16() & 15) + 24;
                  p->y = Random16() & 7;
                }
              }
            }
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
          }
          req.x = frame * 1128 - 103664;
          if (req.x > px(4)) {
            req.x = px(4);
          }
          req.y = frame * 564 - 19064;
          if (req.y > px(2)) {
            req.y = px(2);
          }
          bodyPlace[0] = position.x;
          bodyPlace[1] = px(120) - position.y;
          bodyPlace[2] = px(128);
          Render_ApplyProjectedPlacementFar(work->sprites[HAURES_STAMP],
                                            bodyPlace, (s32 *)&req, 0);
        }
      }
      if (frame == 267) {
        Sound_PlayFar(SE_POWER_UP);
      }
      if (frame == 271) {
        Sound_PlayFar(SE_CLATTER);
      }
      if ((u32)cut <= 9) {
        struct RenderModel *model;
        void *verts;
        s32 s;

        if (frame == 267) {
          u16 *dst = &BG_PLTT_COLOR(0, 1);

          for (i = 1; i != 64; i++) {
            s32 v = i;

            if (v > 31) {
              v = 31;
            }
            if (v < 0) {
              v = 0;
            }
            *dst++ = v;
          }
          Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB,
                                     work->sheet, 1, 0);
          Iwram_CopyWordsEntry(
              (void *)BG_PLTT,
              Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
              SHEET_PALETTE_BYTES);
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(1));
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        burstTexture.logWidth = 7;
        burstTexture.logHeight = 7;
        burstTexture.pixels = SCRATCH;
        model->shade = 0;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &burstTexture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = verts;
        Transform_Reset();
        Transform_ApplyTranslation(0, -px(4), 0);
        s = 0x1d8f0;
        if (frame > 268) {
          s = (frame << 12) - 0xef710;
        }
        Transform_ApplyAxisScale(s >> 1, s, s >> 1);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
        if ((u32)cut <= 5) {
          s32 j;

          Iwram_ClearWords(buffer, 0xd80);
          for (j = 0; j != 72; j++) {
            s32 m;

            for (m = 0; m != 48; m++) {
              buffer[j * 64 + m] = work->sheet[cut * 27 * 128 + j * 48 + m];
            }
          }
          if ((u32)(frame - 267) <= 5) {
            burstTexture.logWidth = 6;
            burstTexture.logHeight = 7;
            burstTexture.pixels = buffer;
            model->mesh = gHaures_BurstMesh;
            Transform_Reset();
            Transform_ApplyTranslation(0, -px(4), 0);
            Transform_ApplyAxisScale(0x1b1e0, 0x29c40, Q16_ONE * 2);
            Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      if (frame > 276) {
        struct RenderModel *model;
        void *verts;
        u8 *cells;
        u8 *mesh;

        if (frame == 277) {
          const u8 *q;

          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
          dived = 1;
          Sprite_DestroyFar(work->sprites[0]);
          Sprite_DestroyFar(work->sprites[1]);
          for (i = 0; i != HAURES_FLAME_COUNT; i++) {
            Sprite_DestroyFar(work->sprites[HAURES_FLAMES + i]);
          }
          for (i = 0; i != HAURES_DOT_COUNT; i++) {
            Sprite_DestroyFar(work->sprites[HAURES_DOTS + i]);
          }
          Sprite_DestroyFar(work->sprites[HAURES_STAMP]);
          BattleEffect_SpawnObjects(11, SPRITE_HAURES, 2);
          q = gHaures_PartPoints;
          {
            struct Particle *p = work->particles;

            for (i = 0; i != HAURES_WING_QUADS; i++, q += 10, p++) {
              s32 x = q[0] >> 1;
              s32 y = q[1];

              p->x = px(x);
              p->y = px(y);
              {
                s32 velocityY = (60 - y) << 10;

                p->velocityX = (x - 120) << 12;
                p->velocityY = velocityY;
              }
              p->z = 0;
              p->velocityX += ((Random16() & 31) - 16) << 13;
              p->velocityY += ((Random16() & 31) - 16) << 13;
              p->velocityZ = (Random16() & 127) * 5 + 640;
              if (Random16() & 1) {
                p->velocityZ = -p->velocityZ;
              }
            }
          }
          velocity.x = 0;
        }
        gBgScroll[1].x += 0xfff4;
        orbitScale = gHaures_OrbitScale;
        bodyPlace[3] = 0;
        position.x = (Trig_Sin(frame << 9) << 3) + px(108);
        position.y = px(56) - (Trig_Cos(frame << 9) << 3);
        if (frame > 396) {
          velocity.x -= (frame - 397) << 14;
          position.x += velocity.x;
          velocity.y -= (frame - 397) << 13;
          // BUG: the fall adds the horizontal speed; velocity.y is never used.
#ifdef BUGFIX
          position.y += velocity.y;
#else
          position.y += velocity.x;
#endif
        }
        for (i = 0; i != HAURES_BODY_PIECES; i++) {
          bodyPlace[0] = px(gHaures_BodyPiecePoints[i][0]) + position.x;
          bodyPlace[1] =
              px(272) - (px(gHaures_BodyPiecePoints[i][1]) + position.y);
          bodyPlace[2] = px(256);
          Render_ApplyProjectedPlacementFar(work->sprites[i], bodyPlace,
                                            (s32 *)&orbitScale, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        cells = work->sheet;
        mesh = work->sheet + 0x800;
        if (frame == 277) {
          s32 none;
          s32 j;

          for (j = 0, none = 0; j != HAURES_WING_QUADS; j++) {
            for (i = 0; i != 4; i++) {
              s8 *dst = (s8 *)cells + j * 16 + i * 4;

              dst[2] = none;
              dst[0] = gHaures_PartPoints[j * 10 + i * 2 + 2] -
                       gHaures_PartPoints[j * 10];
              dst[1] = gHaures_PartPoints[j * 10 + i * 2 + 3] -
                       gHaures_PartPoints[j * 10 + 1];
            }
            mesh[j * 12 + 1] = 1;
            mesh[j * 12 + 2] = 2;
            mesh[j * 12 + 5] = 2;
            mesh[j * 12 + 6] = 3;
            mesh[j * 12 + 0] = none;
            mesh[j * 12 + 4] = none;
            mesh[j * 12 + 8] = none;
            mesh[j * 12 + 9] = none;
            mesh[j * 12 + 10] = none;
          }
        }
        wideTexture.logWidth = 8;
        wideTexture.logHeight = 7;
        wideTexture.pixels = buffer;
        model->kind = MODEL_SOLID;
        model->texture = &wideTexture;
        model->vertices = verts;
        model->shade = (s32)shades;
        {
          /* FAKEMATCH(dead): a never-true test of the byte scroll. */
          s32 scroll = model->scrollU;

          if (scroll > 255) {
            i = 0;
          }
        }
        for (i = 0; i != HAURES_WING_QUADS; i++) {
          gHaures_WingShades[i] = 1;
        }
        {
          for (i = 0; i != HAURES_WING_QUADS; i++) {
            struct Particle *p = &work->particles[i];
            model->mesh = mesh + i * 12;
            if ((u32)(Q_16_16_UPPER_HALF(p->x) + 16) <= 152 &&
                (Q_16_16_TO_INT(p->y)) >= -16 &&
                (Q_16_16_TO_INT(p->y)) <= 136) {
              Transform_Reset();
              Transform_ApplyTranslation(p->x - px(64), p->y - px(64), 0);
              Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
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
              Render_TransformVertices(cells + i * 16, verts, 4);
              Render_DrawModelList(model);
            }
            BattleEffectKernels_IntegrateVector3(p, 62, 0);
            p->x += px(4);
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      if (frame <= 266) {
        work->unk78ec = 1;
      }
      wait(1);
      frame++;
    } while (frame != 440 && !(gInput.repeat & (KEY_A | KEY_B)));
  }
  if (dived == 0) {
    Sprite_DestroyFar(work->sprites[0]);
    Sprite_DestroyFar(work->sprites[1]);
    for (i = 0; i != HAURES_FLAME_COUNT; i++) {
      Sprite_DestroyFar(work->sprites[HAURES_FLAMES + i]);
    }
    for (i = 0; i != HAURES_DOT_COUNT; i++) {
      Sprite_DestroyFar(work->sprites[HAURES_DOTS + i]);
    }
    Sprite_DestroyFar(work->sprites[HAURES_STAMP]);
  } else {
    for (i = 0; i != HAURES_BODY_PIECES; i++) {
      Sprite_DestroyFar(work->sprites[i]);
    }
  }
  transition[4] = 0;
  Task_Remove(BattleEffect_ProcessPendingStampTransfer);
  BattleEffect_SetupBlendedDisplay();
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  BattleEffect_SpawnObjects(11, SPRITE_HAURES, 2);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1, 0);
  {
    Iwram_ClearWords(buffer, 0x6c00);
    for (i = 0; i != 6; i++) {
      s32 j;

      for (j = 0; j != 72; j++) {
        s32 m;

        for (m = 0; m != 48; m++) {
          buffer[(i * 9 * 512) + j * 64 + m] =
              work->sheet[i * 27 * 128 + j * 48 + m];
        }
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = Ram_HeapSlots->ramCode;
  for (frame = 0; frame != HAURES_FRAMES; frame++) {
    struct RenderModel *model;
    void *verts;

    if (frame == 0) {
      position.x = px(270);
      position.y = -px(22);
      velocity.x = -px(12);
      velocity.y = px(6);
      {
        s32 *variant = &SPARKS->variant;

        for (i = 0; i != 512; i++, variant += 7) {
          *variant = -1;
        }
      }
    }
    if (frame == 20) {
      position.x = -px(10);
      position.y = -px(22);
      velocity.x = px(12);
      velocity.y = px(6);
    }
    if (frame == 40) {
      position.x = px(260);
      position.y = px(64);
      velocity.x = -px(8);
      velocity.y = 0;
    }
    {
      for (i = 0; i != 512; i++) {
        struct Particle *p = &((struct Particle *)SCRATCH)[i];
        if (p->variant >= 0) {
          s32 n = (p->variant >> 3) + 1;

          draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                  Q_16_16_UPPER_HALF(p->x) - n / 2,
                  Q_16_16_UPPER_HALF(p->y) - n, n, n * 2);
          BattleEffectKernels_IntegrateVector2(p, 63, -px(1) / 2);
          p->variant--;
        }
      }
    }
    if (frame <= 39) {
      struct Particle *p;

      // The spark row keeps frame * 2 + frame: frame * 3 folds with the row's
      // size.
      for (i = 0, p = ((struct Particle(*)[4])SCRATCH)[frame * 2 + frame];
           i != 12; i++, p++) {
        s32 angle;
        s32 speed;

        p->x = px((Random16() & 63) - 16) + position.x / 2;
        p->y = px(Random16() & 31) + position.y;
        angle = Random16();
        speed = (Random16() & 127) + 128;
        p->velocityX = Trig_Sin(angle) * speed >> 7;
        p->velocityY = Trig_Cos(angle) * speed >> 6;
        p->variant = 16;
      }
    }
    if (frame <= 73) {
      s32 t;

      openingScale = gHaures_OpeningScale;
      bodyPlace[3] = 0;
      t = frame - 20;
      if ((u32)t <= 19) {
        openingScale.x = -0x10001;
        for (i = 0; i != HAURES_BODY_PIECES; i++) {
          bodyPlace[0] = position.x - px(gHaures_BodyPiecePoints[i][0]);
          bodyPlace[1] =
              px(272) - (px(gHaures_BodyPiecePoints[i][1]) + position.y);
          bodyPlace[2] = px(256);
          Render_ApplyProjectedPlacementFar(work->sprites[i], bodyPlace,
                                            (s32 *)&openingScale, 0);
        }
      } else {
        for (i = 0; i != HAURES_BODY_PIECES; i++) {
          bodyPlace[0] = px(gHaures_BodyPiecePoints[i][0]) + position.x;
          bodyPlace[1] =
              px(272) - (px(gHaures_BodyPiecePoints[i][1]) + position.y);
          bodyPlace[2] = px(256);
          Render_ApplyProjectedPlacementFar(work->sprites[i], bodyPlace,
                                            (s32 *)&openingScale, 0);
        }
      }
      position.x += velocity.x;
      position.y += velocity.y;
      if ((u32)frame <= 19) {
        velocity.x += -px(1) / 2;
        velocity.y += px(1) / 2;
      } else if ((u32)t <= 19) {
        velocity.x += px(1) / 2;
        velocity.y += px(1) / 4;
      } else if ((u32)(frame - 40) <= 19) {
        velocity.x += -px(1) / 2;
      }
    }
    if (frame == 10) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 14) {
      Sound_PlayFar(SE_CLATTER);
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, px(4), px(8), px(4),
                                      150);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        work->shakeFrames = 8;
      }
    }
    if (frame == 30) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 34) {
      Sound_PlayFar(SE_CLATTER);
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, px(4), px(8), px(4),
                                      50);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        work->shakeFrames = 8;
      }
    }
    if (frame == 58) {
      Sound_PlayFar(SE_CRASH);
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, px(4), px(8), px(4),
                                      200);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        work->shakeFrames = 8;
      }
    }
    verts = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    model->shade = -8;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->texture = &bandTexture;
    bandTexture.logWidth = 6;
    bandTexture.logHeight = 5;
    model->mesh = gHaures_BandMesh;
    model->vertices = verts;
    bandTexture.pixels = work->sheet;
    model->scrollU = (frame * 8) & 127;
    for (i = 0; i != 2; i++) {
      s32 start = gHaures_BandStarts[i];

      if (frame >= start && frame < start + 16) {
        s32 amp = Trig_Sin((frame - start) << 11) / 8;
        s32 x;
        s32 m;

        for (m = 0; m != 3; m++) {
          s32 y = -4;
          x = m * 20 - 16;
          Transform_Reset();
          if (i == 0) {
            Transform_ApplyTranslation(px(x), px(y), 0);
            Transform_ApplyRoll(-TRIG_TURN / 8);
          } else {
            Transform_ApplyTranslation(px(x - 16), px(y), 0);
            Transform_ApplyRoll(0x27d0);
          }
          Transform_ApplyAxisScale(Q16_ONE * 3, amp, Q16_ONE * 2);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    bandTexture.logWidth = 6;
    bandTexture.logHeight = 5;
    model->mesh = gHaures_BandMesh;
    model->kind = MODEL_SHADED_ADDITIVE;
    if ((u32)(frame - 56) <= 31) {
      s32 sh = 0;
      s32 v;
      s32 left;
      s32 right;
      s16 *corner = verts;

      if (frame > 79) {
        sh = (80 - frame) * 8;
      }
      model->shade = sh;
      model->scrollU = (frame * 8) & 127;
      bandTexture.pixels = work->sheet;
      if (frame <= 71) {
        v = Q_16_16_TO_INT(Trig_Sin((frame << 10) + -0xe000) * 80);
      } else if (frame > 79) {
        v = Q_16_16_TO_INT(Trig_Sin((frame << 10) + -TRIG_TURN) * 80);
      } else {
        v = 80;
      }
      left = 4;
      corner[1] = 60 - v;
      corner[5] = 60 - v;
      right = 123;
      corner[0] = left;
      corner[4] = right;
      corner[8] = left;
      corner[9] = v + 60;
      corner[12] = right;
      corner[13] = v + 60;
      Render_DrawModelList(model);
    }
    Heap_Release(model);
    Heap_Release(verts);
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  for (i = 0; i != HAURES_BODY_PIECES; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// The two rings sit at y = -36 and y = -18.
#define FIRST_RING_Y -36
#define RING_SPACING 18

/// Each side of the band shows an 8 x 16 column of the texture.
#define COLUMN_WIDTH 8
#define COLUMN_HEIGHT 16

/// A mesh triangle: its three vertex numbers and their texture coordinates.
struct MeshTriangle {
  u8 vertex[3];
  u8 unusedA; ///< nothing reads or writes it
  u8 texCoord[3][2];
  u8 unusedB[2]; ///< nothing reads or writes them
};

/// Builds the mesh of an open cylinder, the band Coatlicue shows: two rings of
/// `segments` vertices of radius 96 around the y axis, and two textured
/// triangles for each side of the band, joining the first ring (texture row 0)
/// to the second (row 15). A triangle whose three vertices are 0 ends the list.
/// Coatlicue builds it with 32 segments into the buffers at 0x02014400 and
/// 0x02014800.
void Mesh_BuildCylinderBand(struct MeshVertex *vertices,
                            struct MeshTriangle *triangles, s32 segments) {
  s32 ring;
  s32 i;

  for (ring = 0; ring != 2; ring++) {
    for (i = 0; i != segments; i++) {
      // The vertex: a point on the ring, 96 units out (3 * 0x10000 >> 11).
      vertices[ring * segments + i].x =
          (u32)(Trig_Sin(i * (TRIG_TURN / segments)) * 3) >> 11;
      vertices[ring * segments + i].y = ring * RING_SPACING + FIRST_RING_Y;
      vertices[ring * segments + i].z =
          (u32)(Trig_Cos(i * (TRIG_TURN / segments)) * 3) >> 11;

      // The side from this vertex to the next, as two triangles.
      if (ring < 1) {
        struct MeshTriangle *triangle =
            (struct MeshTriangle *)((ring * segments + i) * 24 +
                                    (u32)triangles);
        s32 left = i * COLUMN_WIDTH;
        s32 right = left + COLUMN_WIDTH - 1;

        triangle[0].texCoord[0][0] = right;
        triangle[0].texCoord[0][1] = 0;
        triangle[0].texCoord[1][0] = right;
        triangle[0].texCoord[1][1] = COLUMN_HEIGHT - 1;
        triangle[0].texCoord[2][0] = left;
        triangle[0].texCoord[2][1] = COLUMN_HEIGHT - 1;
        {
          struct MeshTriangle *second = triangle + 1;

          second->texCoord[0][0] = left;
          second->texCoord[0][1] = 0;
          second->texCoord[1][0] = right;
          second->texCoord[1][1] = 0;
          second->texCoord[2][0] = left;
          second->texCoord[2][1] = COLUMN_HEIGHT - 1;

          // (next, next on ring 1, this on ring 1), then (this, next, this on
          // ring 1).
          triangle->vertex[0] = (i + 1) % segments + ring * segments;
          triangle->vertex[1] = (i + 1) % segments + segments + ring * segments;
          triangle->vertex[2] = i % segments + segments + ring * segments;
          second->vertex[0] = i % segments + ring * segments;
          second->vertex[1] = (i + 1) % segments + ring * segments;
          second->vertex[2] = i % segments + segments + ring * segments;
        }
      }
    }
  }

  // The end of the list.
  triangles[segments * 2].vertex[0] = 0;
  triangles[segments * 2].vertex[1] = 0;
  triangles[segments * 2].vertex[2] = 0;
}
#undef BLITTER
#undef SCRATCH
#undef SPARKS
#undef STAMP_TILES
#undef STAMP_CANVAS
#undef FIRST_RING_Y
#undef RING_SPACING
#undef COLUMN_WIDTH
#undef COLUMN_HEIGHT
