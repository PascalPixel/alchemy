/// @file
/// The water-column scene of Whorl, Gale, Lethe Albion, Poison Flow and Raging
/// Flood.
// Part of f15585c.c.

#include "../f15585c_internal.h"

struct FxPartyList;
struct Model;
struct ModelVertex;
struct ProjectedVertex;
struct StripQuad;

/* VIEW(f171bb0): tag struct ModelTexture as struct ModelTexture__f171bb0; f15585c.c has another definition; tried with the definition the rest of the object has, Func_08171bb0 moves: 1 of 1741 instructions in tla-ja. */
struct ModelTexture__f171bb0 {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const u8 *pixels;
};

/* VIEW(f171bb0): tag struct RenderModel as struct RenderModel__f171bb0; f15585c.c has another definition. */
struct RenderModel__f171bb0 {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture__f171bb0 *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Func_080150e4(s32 angle);  // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
/* VIEW(f171bb0): routine GetBattleObjectSlotFar as void **GetBattleObjectSlotFar(s32 objectId); motobj.h has struct BattleObj *GetBattleObjectSlotFar(s32 objectId); tried as the 11 declarations' void **GetBattleObjectSlotFar(s32 objectId), it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:20521: conflicting types for `GetBattleObjectSlotFar'. */
extern void **GetBattleObjectSlotFar__f171bb0(s32 objectId) __asm__("GetBattleObjectSlotFar");
void *Resource_GetTableEntry(s32 resource);

void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
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
// The ring's rows of vertices, two (x, y, z, pad) each. The buffers in this
// function are constants built from the fixed buffer, not their own symbols.
#define MAP_CELLS ((s8 (*)[2][4])Ram_MapCellBuffer)

extern char ResourceId_RuneSheet, ResourceId_BlueFlameSheet, ResourceId_Data0B4;
extern char ResourceId_FlashBurstSheet, ResourceId_Data0D2, ResourceId_Data0D3;
extern char ResourceId_Data134, ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_MarsDjinnSheet, ResourceId_WaterSpraySheet;

extern u8 gMapCellBuffer[];
extern u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const u8 gData_08199340[];
extern const u8 gData_08199210[];

void Render_ProjectVertices(struct ModelVertex *vertices, struct ProjectedVertex *screen, s32 count);
void SceneTransform_ApplyPosition(const s32 *pos);

// The water-column scene shared by Whorl (0), Gale (1), Lethe Albion (2),
// Poison Flow (3) and Raging Flood (4): a twisting funnel of water (or wind)
// rises around the target, with spray, bubbles and a burst.
void Func_08171bb0(struct BattleEffectArgument *effect, u32 mode) {
  struct BattleEffectWork *work;
  u8 *canvas;
  // Unused: the second of The Broken Seal's blitter pair gives the stack frame
  // its 176 bytes (the unused word at sp 76); as one DrawRectangle the frame is
  // 172 bytes and every stack offset below it moves.
  DrawRectangle blit[2];
  s32 frame;
  s32 *camera;
  u8 *sprites;
  s32 frames;
  u8 *unit;
  s32 *target;
  struct RenderModel__f171bb0 *model;
  struct ModelTexture__f171bb0 texture;
  s32 columns;
  s32 shade;
  s32 i;
  s32 j;
  // One constant for its whole life: the three palette copies' destination.
  void *palette = (void *)BG_PLTT;
  void *vertices = Ram_MapCellBuffer + 0x1000;
  u8 *sheet = Ram_MapCellBuffer + 0x4000;
  void *mesh = Ram_MapCellBuffer + 0x2000;
  s32 point[3];
  s32 screen[3];
  s32 pos[3];
  struct EffectPosition proj;
  struct EffectPosition apos;
  struct EffectPosition spos;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  unit = Unit_Get(effect->actors[0]);
  BattleEffect_StartCanvasLayer(0);
  if (mode <= 1) {
    frames = 38;
    Func_0815585c(effect, (effect->side << 4) | 163, screen, point);
    REG_BG2CNT = 0x785;
  } else if (mode == 2 || mode == 4) {
    s32 *caster = *GetBattleObjectSlotFar__f171bb0(effect->actor);

    if (mode == 2) {
      s32 row;
      s32 k;
      s32 size = 0x1000;

      Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, sheet + size, 1,
                                 0);
      Iwram_ClearWordsEntry(sheet, size);
      for (row = 0; row != 48; row++) {
        for (k = 0; k != 24; k++) {
          (Ram_MapCellBuffer + 0x4208)[row * 64 + k] =
              sheet[(row * 2 + row) * 8 + 0x1000 + k];
          sheet[row * 64 + 0x237 - k] = sheet[(row * 2 + row) * 8 + 0x1000 + k];
        }
      }
      Iwram_CopyWordsEntry(palette,
                      Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                      128);
    } else {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, sheet, 0, 0);
    }
    pos[0] = caster[2];
    pos[1] = caster[3];
    pos[2] = caster[4];
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               Ram_MapCellBuffer + 0x5000, 1, 0);
    frames = 52;
  } else {
    REG_BG2CNT = 0x785;
    frames = 52;
  }
  if (!effect->side) {
    BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER, 23);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  target = *GetBattleObjectSlotFar__f171bb0(effect->actors[0]);
  work->particles[63].x = target[2];
  work->particles[63].y = target[3];
  work->particles[63].z = target[4];
  work->particles[63].velocityX = target[6];
  work->particles[63].velocityY = target[7];
  if (mode == 2 || mode == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D2, work->sheet, 0, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D3, work->sheet, 0, 0);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet + 0x2000, 1, 1);
  if (mode == 3) {
    Iwram_CopyWordsEntry(
        palette, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
  }
  if (mode == 4 || mode == 2) {
    Iwram_CopyWordsEntry(
        palette, Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet), 128);
  }
  columns = (mode == 2 || mode == 4) ? 8 : 4;
  Render_BuildQuadStrip(mesh, columns, 32, 32);

  frame = 0;
  if (frames != 0) {
    do {
      if (frame == 0) {
        Sound_PlayFar(199);
      }
      if (frame == 32) {
        Sound_PlayFar(206);
      }
      if (frame == 0) {
        target[2] = work->particles[63].x;
        target[3] = work->particles[63].y;
        target[4] = work->particles[63].z;
        target[6] = work->particles[63].velocityX;
        target[7] = work->particles[63].velocityY;
        if (mode == 0) {
          for (i = 0; i != 63; i++) {
            struct EffectStep *q = &work->particles[i];

            if (effect->side == 1) {
              q->x = 0xc80000;
            } else {
              q->x = -0xc80000;
            }
            q->y = ((Random16() & 127) - 64) << 16;
            q->z = ((Random16() & 127) - 64) << 16;
            q->velocityX = q->x;
            q->velocityZ = q->z;
            q->velocityY = q->y;
          }
        } else if (mode == 1) {
          {
            struct EffectStep *q = work->particles;

            for (i = 0; i != 63; i++, q++) {
              q->x = point[0];
              q->y = ((Random16() & 31) << 16) + 0x400000;
              q->z = 0;
              q->velocityX = ((Random16() & 63) + 64) << 13;
              if (q->x > 0) {
                q->velocityX = -q->velocityX;
              }
              q->velocityY = (Random16() | -128) << 11;
              q->velocityZ = 0;
            }
          }
        }
      }
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      if (mode <= 1) {
        s32 k = 4;

        pos[0] = point[0] + (Trig_Sin(frame << 11) << 4);
        if (frame > 39) {
          k = frame - 36;
        }
        pos[1] = point[1] + (k << 16) - Trig_Cos(frame * 3304) * k;
        if (frame > 27) {
          pos[1] = pos[1] + (frame << 18) - 0x700000;
        }
        {
          /* FAKEMATCH(temporary): out stands in for screen's address, so
           * point's address stays out of r5 and k gets it; as screen, 18 of
           * 1748 instructions differ in tla-en. */
          s32 *out = screen;

          pos[2] = point[2];
          EffectPosition_ProjectPoint(pos, (struct EffectPosition *)out);
          blit[0](canvas, work->sheet + 0x2000, out[0] / 2 - 10, out[1] - 20,
                  20, 40);
        }
      }
      if (frame > 7) {
        if (mode <= 1 && frame <= 47) {
          work->shakeFrames = 3;
        }
        if (mode == 0) {
          if (frame == 32) {
            if (effect->variant == 1) {
              target[3] = 0x7c0000;
            } else {
              work->shakeFrames = 8;
              Func_081180e8(134);
              target[6] = work->particles[63].velocityX;
              target[7] = work->particles[63].velocityY;
              BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000,
                                            0x80000, 0x40000, 150);
            }
          } else if (frame <= 19) {
            target[18] = mode;
            target[2] += (pos[0] - work->particles[63].x) / 128;
            target[3] += (pos[1] - work->particles[63].y) / 128;
            target[4] += (pos[2] - work->particles[63].z) / 128;
          } else if (frame <= 31 && (effect->variant == 1 || frame <= 23)) {
            target[2] += (pos[0] - work->particles[63].x) / 16;
            target[3] += (pos[1] - work->particles[63].y) / 16;
            target[4] += (pos[2] - work->particles[63].z) / 16;
            target[6] += (0x4000 - work->particles[63].velocityX) / 16;
            target[7] += (0x4000 - work->particles[63].velocityY) / 16;
          }
        } else if (mode == 1) {
          target[18] = 0;
          if (frame > 55) {
            target[3] = 0x900000;
          } else if (frame <= 31) {
            if (target[2] > 0) {
              target[2] += 0x8000;
            } else {
              target[2] -= 0x8000;
            }
          } else {
            if (target[2] > 0) {
              target[2] += 0x100000;
            } else {
              target[2] -= 0x100000;
            }
          }
        }
      }
      if (frame > 0 && mode <= 1) {
        {
          struct EffectStep *q = work->particles;

          for (i = 0; i != 16; i++, q++) {
            if (frame >= i && frame < i + 30) {
              s32 n = (i & 3) + 1;

              if (mode == 0) {
                q->x += (pos[0] - q->velocityX) / 32;
                q->y += (pos[1] - q->velocityY) / 32;
                q->z += (pos[2] - q->velocityZ) / 32;
              } else {
                BattleEffectKernels_IntegrateVector3(q, 62, 1024);
              }
              EffectPosition_ProjectPoint(q, &proj);
              blit[0](canvas, sprites + gData_08197410[n - 1],
                      proj.x / 2 - n / 2, proj.y - n, n, n * 2);
            }
          }
        }
      }

      if (mode == 2 || mode == 4) {
        for (j = 0; j != 33; j++) {
          s32 r = j;

          if (r > 28) {
            r = 28;
          }
          for (i = 0; i != 2; i++) {
            // One zero for the row, as its own local: a vertex's stores hoist
            // a zero, and this mode needs it back.
            s32 zero = 0;

            MAP_CELLS[j][i][0] = -j * 4 + 64;
            MAP_CELLS[j][i][1] =
                ((Trig_Cos((j << 10) - (frame << 11)) * r) >> 16) + i * 48 - 24;
            MAP_CELLS[j][i][2] = zero;
          }
        }
      } else if (mode == 3) {
        for (j = 0; j != 33; j++) {
          s32 r = j * 4;
          s32 w = j * 2 + 4;

          if (r > 64) {
            r = 64;
          }
          for (i = 0; i != 2; i++) {
            MAP_CELLS[j][i][0] = -j * 3;
            MAP_CELLS[j][i][1] =
                ((Trig_Cos((j << 10) - (frame << 11)) * r) >> 16) +
                (i * w << 1) - w;
            MAP_CELLS[j][i][2] = 0;
          }
        }
      } else {
        for (j = 0; j != 33; j++) {
          s32 r = j * 2 + j;
          s32 w = j * 2;

          if (r > 96) {
            r = 96;
          }
          if (w > 40) {
            w = 40;
          }
          for (i = 0; i != 2; i++) {
            // -4 * j, built on the row's j * 2 so it is worked out here.
            MAP_CELLS[j][i][0] = -(j + j) * 2;
            MAP_CELLS[j][i][1] = ((Trig_Cos((frame + j * 2) << 10) * r) >> 16) +
                                 (i * w << 1) - w;
            MAP_CELLS[j][i][2] = 0;
          }
        }
      }

      model = Render_AllocateModelList(1);
      if (mode == 4) {
        if (frame <= 45) {
          shade = frame * 4 - 64;
        } else {
          shade = frame * 8 - 376;
        }
        if (shade > -8) {
          shade = -8;
        }
      } else {
        if (frame <= 45) {
          shade = frame * 4 - 64;
        } else {
          shade = 348 - frame * 8;
        }
        if (shade > -20) {
          shade = -20;
        }
      }
      if (shade < -64) {
        shade = -64;
      }
      if (mode == 2 || mode == 4) {
        texture.logWidth = 5;
      } else {
        texture.logWidth = 7;
      }
      texture.logHeight = 5;
      texture.pixels = work->sheet;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = mesh;
      model->vertices = vertices;
      model->shade = shade;
      if (mode == 2 || mode == 4) {
        EffectPosition_ProjectBattler(effect->actors[0], &apos);
        if (frame == 16) {
          Func_081180e8(144);
          BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0x70000,
                                        100);
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        }
        if (frame == 0) {
          {
            struct EffectStep *q = work->particles;

            for (i = 0; i != 63; i++, q++) {
              s32 r = (Random16() & 255) + 256;
              s32 a = (Random16() & 0x7fff) + 0x4000;

              q->velocityX = (Trig_Sin(a) * r) >> 7;
              q->velocityY = (Trig_Cos(a) * r) >> 7;
              q->x = (apos.x / 2) << 16;
              q->y = (apos.y - 8) << 16;
              q->variant = 0;
            }
          }
        }
        if (frame > 15) {
          {
            struct EffectStep *q = work->particles;

            for (i = 0; i != 8; i++, q++) {
              if (q->variant <= 23) {
                blit[0](canvas,
                        (Ram_MapCellBuffer + 0x5000) + ((q->variant / 4) << 11),
                        (s16)(q->x >> 16) - 16, (s16)(q->y >> 16) - 32, 32, 64);
                BattleEffectKernels_IntegrateVector2(q, 60, -0x2000);
                q->variant++;
              }
            }
          }
        }
        model->scrollU = ~(frame << 3) & 127;
        model->scrollV = 0;
        for (i = 0; i != 3; i++) {
          s32 y = 64 - ((Trig_Sin(frame * 630) << 6) >> 16);

          Transform_Push();
          Func_08015160(0, ((y - i * 8) << 16) + 0x300000, 0);
          if (effect->side == 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          Func_080150e4((i << 10) + 256);
          SceneTransform_ApplyPitch((i << 13) - (frame << 11));
          Func_080151e4(0x21770, 0x10000, 0x10000);
          Render_TransformVertices(gMapCellBuffer, vertices, 66);
          Render_DrawModelList(model);
          Transform_Pop();
        }
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gData_08199340;
        model->vertices = vertices;
        model->shade = shade;
        model->scrollU = 0;
        model->scrollV = 0;
        texture.pixels = sheet;
        for (i = 0; i != 2; i++) {
          s32 t = frame - i * 4 - 16;

          if (t >= 0) {
            s32 s = (t * 3) << 12;

            shade = 0;
            if (t > 7) {
              shade = (8 - t) * 8;
            }
            model->shade = shade;
            if (shade > -64) {
              Render_ResetTransformState();
              Func_08015160((apos.x / 2 - 64) << 16, (apos.y - 64) << 16, 0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              Func_080150e4((i << 14) + 0x2000);
              SceneTransform_ApplyPitch(-0x1000);
              SceneTransform_ApplyYaw(frame << 12);
              Func_0801521c(s);
              Render_TransformVertices(gData_08199210, vertices, 4);
              Render_DrawModelList(model);
            }
          }
        }
      } else if (mode == 3) {
        EffectPosition_ProjectScaledBattler(effect->actor, &spos);
        if (frame == 8) {
          Sound_PlayFar(144);
        }
        if (frame == 16) {
          Func_081180e8(134);
        }
        for (i = 0; i != effect->count; i++) {
          if (frame == i * 4 + 16) {
            Sound_PlayFar(134);
            BattleEffect_StartActorMotion(effect->actors[i], 1, 0, 0, 0x70000,
                                          100);
            Func_0814cd48(effect->actors[i], 7, 5, i, 8);
            work->shakeFrames = 8;
          }
        }
        model->scrollV = 0;
        // A once-only loop: its i terms are the other modes' ring index, here
        // always 0.
        for (i = 0; i != 1; i++) {
          s32 scroll = ~((frame + i * 6) << 3);

          // Masked in its own statement: with the mask in one expression the
          // complement, not the scroll, is the running value.
          scroll &= 127;
          model->scrollU = scroll;
          Render_ResetTransformState();
          Func_08015160((spos.x / 2 - 64) << 16, (spos.y << 16) - 0x340000, 0);
          if (effect->side == 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          SceneTransform_ApplyPitch(i << 15);
          Func_080150e4((Trig_Sin((frame + i * 8) << 11) << 11) >> 16);
          Func_080151e4(0x14e20, 0x8000, 0x10000);
          Render_TransformVertices(gMapCellBuffer, vertices, 66);
          Render_DrawModelList(model);
        }
      } else {
        if (frame <= 29) {
          shade = frame * 4 - 64;
        } else {
          shade = 220 - frame * 8;
        }
        if (shade > -20) {
          shade = -20;
        }
        if (shade > -64) {
          model->shade = shade;
          Func_080151e4(0x8000, 0x10000, 0x10000);
          gCameraSceneParameters[3] = 60;
          if (mode == 0) {
            model->scrollU = (frame << 3) & 127;
          } else {
            model->scrollU = ~(frame << 3) & 127;
          }
          model->scrollV = 0;
          SceneTransform_ApplyPosition(pos);
          Func_08015160(0, 0x100000, 0);
          for (i = 0; i != 3; i++) {
            Transform_Push();
            if (effect->side == 1) {
              SceneTransform_ApplyYaw(0x8000);
            }
            Func_080150e4((i << 10) + 3096);
            if (mode != 1) {
              SceneTransform_ApplyPitch((frame + i * 4) << 11);
            } else {
              SceneTransform_ApplyPitch((i << 13) - (frame << 11));
            }
            Func_080151e4(0x40000, 0x10000, 0x10000);
            Render_ProjectVertices(gMapCellBuffer, vertices, 66);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
        gCameraSceneParameters[3] = 120;
      }
      Heap_Release(model);
      if (mode == 2 || mode == 4) {
        BattleCamera_Shake(8, 8);
      } else {
        BattleCamera_Shake(4, 4);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != frames);
  }

  if (unit[0x129] && mode == 0 && effect->variant == 1) {
    target[2] = work->particles[63].x;
    target[3] = work->particles[63].y;
    target[4] = work->particles[63].z;
    target[6] = work->particles[63].velocityX;
    target[7] = work->particles[63].velocityY;
  }
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

struct BattleEffectArgument;

void Func_08172b00(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Core (kind 236).
void BattleEffect_RunCore(s32 a0) { Func_08172b00(a0, 0); }

/// Abilities: Cannon (kind 246).
void BattleEffect_RunCannon(s32 a0) { Func_08172b00(a0, 1); }
#undef SLOT_BLITTER
#undef BLITTER
#undef MAP_CELLS
