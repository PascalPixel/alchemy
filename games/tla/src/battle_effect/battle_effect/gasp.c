/// @file
/// The body that sixteen abilities' effects share, from Gasp to Demon Eye.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

struct FxPartyList;
struct Model;
struct StripQuad;

/* VIEW(f173aac): tag struct RenderModel as struct RenderModel__f173aac;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunGaspFamily moves: 13 of 4112 instructions in
 * tla-ja. */
struct RenderModel__f173aac {
  s32 kind;
  s32 unk04;
  const struct ModelFace *mesh;
  struct ProjectedVertex *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

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
void Render_DrawModelList__ability_effects(struct Model *model);
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
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
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

// A second, larger particle table, always reached at its fixed address.
#define EXTRA_PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))

extern u16 gBattleEffect_MoteCells[];
extern const s8 Data_08198c28[];
extern const u8 Data_08198c34[];
extern const u8 Data_08198c64[];
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
extern const u8 gData_081991b0[];
extern const u8 Data_081991d0[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199210[];
extern const u8 Data_08199244[];
extern const u8 gData_08199268[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

extern char ResourceId_BlastSheet;
extern char ResourceId_Data0B4;
extern char ResourceId_Data0C1;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0C9;
extern char ResourceId_LightBurstSheet;
extern char ResourceId_CornerGlowSheet;
extern char ResourceId_Data0D7;
extern char ResourceId_Data0D8;
extern char ResourceId_SmokeBandSheet;
extern char ResourceId_EffectPalette02;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_VenusDjinnSheet;
extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_MercuryDjinnSmallSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data134;
extern char ResourceId_SmokeSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;

void BattleEffect_StepPaletteToResource(s32 resource);
void Palette_BrightenSavedBattleBg(s32 red, s32 green, s32 blue);
void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase);
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);

void Graphics_LoadGreyRamp(void);
void Palette_LoadBasicSpriteColors(void);

// The user's figure: a 20 x 40 cell drawn over the canvas.
#define FIGURE(work) ((work)->sheet + 0x65c0)
// Loads a sheet's colours into the background palette.
#define LOAD_BG_PALETTE(resource)                                              \
  {                                                                            \
    void *palette = Resource_GetTableEntry((s32)&(resource));                  \
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;             \
                                                                               \
    copy((void *)BG_PLTT, palette, 128);                                       \
  }
// Sets the transform for one of modes 11 and 14's spinning particles: at its
// position, half as wide (the canvas holds every other column), turned by z.
#define PLACE_PARTICLE(p)                                                      \
  do {                                                                         \
    Transform_Reset();                                                         \
    SceneTransform_ApplyPosition3((p)->x - 0x400000, (p)->y - 0x400000, 0);    \
    SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE);                 \
    Transform_ApplyRoll((p)->z);                                               \
  } while (0)
// DEAD: each test reads the count twice and drops the first read; both loads
// are in the ROM.
#define WAIT_TRANSFER(work)                                                    \
  while (*(volatile u32 *)&(work)->vblanksSinceTransfer,                       \
         *(volatile u32 *)&(work)->vblanksSinceTransfer <= 1)

/// The shared body of sixteen abilities' effects, named for the first (its
/// wrappers are in rime.c): Gasp (mode 0), Reflux (2), Djinn Counter (3), Mold
/// and Meld (4), Shine (5), Lull (6), Counter-rush (7), Demon Eye (8), Emu and
/// its kind's four other abilities (9), Call Dullahan and Formina Sage (10),
/// Beat Dance (11), Element Swap (12), Cage (13), Clarion Cry (14) and Psy
/// Boost (15). Mode 1 has no caller. Each mode loads its own sheets, then draws
/// a canvas layer and a model layer every frame for the mode's length.
void BattleEffect_RunGaspFamily(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  DrawRectangle draw[2];
  s32 frames;
  u8 *sprites = Ram_HeapSlots->unk64[0];
  s32 *camera = Ram_HeapSlots->cameraWork;
  u8 *cells = Ram_MapCellBuffer;
  struct RenderModel__f173aac *model;
  void *verts;
  struct ModelTexture texture;
  s32 screen[3];
  s32 other[3];
  s32 frame;
  s32 i;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (mode == 0) {
    BattleEffect_RunDjinniEntrance(effect, (effect->side ^ 1) << 4 | 35, screen,
                                   other);
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                               FIGURE(work), 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D8, work->sheet, 0, 0);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    frames = 84;
  } else if (mode == 1) {
    BattleEffect_RunDjinniEntrance(effect, (effect->side ^ 1) << 4 | 33, screen,
                                   other);
    Resource_LoadAndDecompress((s32)&ResourceId_LightBurstSheet, work->sheet, 1,
                               0);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               FIGURE(work), 1, 1);
    LOAD_BG_PALETTE(ResourceId_OrangePaletteB);
    frames = 48;
  } else if (mode == 4) {
    s32 j;
    s32 k;

    BattleEffect_RunDjinniEntrance(effect, (effect->side ^ 1) << 4 | 0x120,
                                   screen, other);
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                               FIGURE(work), 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + 0x1000, 1, 0);
    {
      s32 (*clear)(void *, s32) = Iwram_ClearWordsEntry;

      clear(work->sheet, 0x1000);
    }
    for (k = 0; k != 40; k++)
      for (j = 0; j != 40; j++)
        work->sheet[0x30c + k * 64 + j] = work->sheet[0x1000 + k * 40 + j];
    frames = 42;
  } else if (mode == 6) {
    BattleEffect_RunDjinniEntrance(effect, (effect->side ^ 1) << 4 | 131,
                                   screen, other);
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                               FIGURE(work), 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    frames = 80;
    Sound_PlayFar(MUS_084);
  } else if (mode == 11) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    LOAD_BG_PALETTE(ResourceId_MarsDjinnSmallSheet);
    frames = 64;
  } else if (mode == 14) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    LOAD_BG_PALETTE(ResourceId_MercuryDjinnSmallSheet);
    work->fadeFrames = 24;
    work->fadeStep = 0;
    frames = 64;
  } else if (mode == 13) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                               1);
    frames = 40;
  } else if (mode == 12) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cells, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_CornerGlowSheet,
                               work->sheet + 0x4000, 1, 1);
    Picture_MirrorIntoQuadrants(work->sheet + 0x4000, work->sheet, 64, 64);
    work->fadeFrames = 24;
    work->fadeStep = 0;
    frames = 48;
  } else if (mode == 15) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cells, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_CornerGlowSheet,
                               work->sheet + 0x4000, 1, 1);
    Picture_MirrorIntoQuadrants(work->sheet + 0x4000, work->sheet, 64, 64);
    work->fadeFrames = 24;
    work->fadeStep = 0;
    frames = 48;
  } else if (mode == 5) {
    BattleEffect_RunDjinniEntrance(effect, effect->side << 4 | 226, screen,
                                   other);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                               FIGURE(work), 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x4000,
                               0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cells, 0, 0);
    LOAD_BG_PALETTE(ResourceId_OrangePaletteB);
    frames = 82;
  } else if (mode == 10) {
    s32 j;
    s32 k;

    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, cells, 1,
                               1);
    Iwram_ClearWordsEntry(work->sheet, 0x4000);
    for (i = 0; i != 4; i++)
      for (k = 0; k != 120; k++)
        for (j = 0; j != 24; j++)
          work->sheet[4 + i * 0x1000 + k * 32 + j] =
              cells[(i * 45 << 6) + k * 24 + j];
    LOAD_BG_PALETTE(ResourceId_EffectPalette02);
    frames = 60;
  } else if (mode == 7) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet, 0, 0);
    LOAD_BG_PALETTE(ResourceId_MercuryDjinnSmallSheet);
    frames = 76;
  } else if (mode == 8) {
    Resource_LoadAndDecompress((s32)&ResourceId_CornerGlowSheet,
                               work->sheet + 0x4000, 1, 1);
    Picture_MirrorIntoQuadrants(work->sheet + 0x4000, work->sheet, 64, 64);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    frames = 16;
  } else if (mode == 9) {
    u8 *p;

    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet + 0x3080,
                               0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 0);
    for (i = 0, p = work->sheet; i != 0x1b00; i++, p++) {
      *p <<= 1;
      if (*p > 63)
        *p = 63;
    }
    if (effect->variant == 2) {
      LOAD_BG_PALETTE(ResourceId_OrangePaletteB);
    } else {
      LOAD_BG_PALETTE(ResourceId_BlastSheet);
    }
    BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 10, 80);
    frames = 41;
  } else {
    if (mode == 2) {
      BattleEffect_RunDjinniEntrance(effect, (effect->side ^ 1) << 4 | 2,
                                     screen, other);
      frames = 76;
    } else {
      work->fadeFrames = 24;
      work->fadeStep = 0;
      frames = 70;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                               FIGURE(work), 1, 1);
    LOAD_BG_PALETTE(ResourceId_OrangePaletteB);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  if (mode == 0 || mode == 6)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (mode == 2 || mode == 7 || mode == 4) {
    if (effect->side == 0)
      BattleEffect_LoadBlitter(SLOT_BLITTER, 7);
    else
      BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  } else if (mode == 3 || mode == 5) {
    if (effect->side == 1)
      BattleEffect_LoadBlitter(SLOT_BLITTER, 7);
    else
      BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  } else {
    if (effect->side == 0)
      BattleEffect_LoadBlitter(SLOT_BLITTER, 23);
    else
      BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (frame = 0; frame != frames; frame++) {
    s32 a;
    s32 twice;
    s32 x;
    s32 y;

    // The canvas layer.
    if (mode == 0) {
      s32 *lines = (s32 *)(work->sheet + 0x6980);

      for (i = 0; i != 160; i++)
        *lines++ = (Random16() & 31) << 4;
    } else if (mode == 6) {
      s32 *lines = (s32 *)(work->sheet + 0x6980);

      for (i = 0; i != 160; i++)
        *lines++ = (0x60000 - Trig_Sin((frame << 11) + i * 0x800) * 6) >> 10;
    } else if (mode == 1 && frame > 31) {
      u16 *pal = (u16 *)(BG_PLTT + 1);

      for (i = 0; i != 63; i++, pal++) {
        s32 r = *pal & 31;
        s32 g = *pal >> 5 & 31;
        s32 b = *pal >> 10 & 31;

        if (frame > 40) {
          r += 4;
          g += 4;
          b += 4;
        } else {
          r += 2;
          g += 2;
          b += 2;
        }
        if (r > 31)
          r = 31;
        if (g > 31)
          g = 31;
        if (b > 31)
          b = 31;
        *pal = b << 10 | g << 5 | r;
      }
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(1));
    }

    if (mode == 3) {
      struct EffectPosition to;
      struct EffectPosition from;

      EffectPosition_ProjectScaledBattler(effect->actor, &from);
      EffectPosition_ProjectScaledBattler(effect->actors[0], &to);
      from.x /= 2;
      to.x /= 2;
      if ((u32)(frame - 32) <= 8) {
        s32 n = frame - 31;
        s32 dx;
        s32 head;
        s32 tail;
        s32 step;
        s32 acc;

        if (n > 8)
          n = 8;
        dx = to.x - from.x;
        head = from.x + Data_08198c28[n] * dx / 16;
        tail = from.x + Data_08198c28[n - 1] * dx / 16;
        for (i = 0, step = head - tail, acc = 0; i != 4; i++, acc += step)
          draw[0](canvas, FIGURE(work), tail + acc / 3 - 10, from.y - 20, 20,
                  40);
      }
      if (frame > 37) {
        if (frame == 38) {
          for (i = 0; i != 63; i++) {
            struct EffectStep *p = &work->particles[i];
            s32 s;

            p->x = to.x << 16;
            p->y = to.y << 16;
            a = Random16();
            s = (Random16() & 511) + 256;
            p->velocityX = Trig_Sin(a) * s >> 8;
            p->velocityY = Trig_Cos(a) * s >> 7;
            p->variant = (Random16() & 15) + 16;
          }
          BattlePlayback_StartFar(134);
          BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
          work->shakeFrames = 8;
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
        }
        for (i = 0; i != 63; i++) {
          struct EffectStep *p = &work->particles[i];

          if (p->variant >= 0) {
            s32 n = (p->variant >> 3) + 2;

            draw[0](canvas, sprites + gBattleEffect_MoteCells[n - 1],
                    HI(p->x) - n / 2, HI(p->y) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, 0x400);
            p->variant--;
          }
        }
      }
    } else if (mode == 6) {
      s32 b = frame << 11;
      s32 x;
      s32 y;

      x = (Trig_Sin(b) * 20 >> 16) + screen[0] / 2 - 10;
      y = (Trig_Cos(b) * 4 >> 16) + screen[1] - 24;
      if (frame > 44) {
        twice = frame * 2;
        y = y - twice + 88;
      }
      draw[0](canvas, FIGURE(work), x, y, 20, 40);
      BattleEffect_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
    } else if (mode == 2 || mode == 7) {
      struct EffectPosition target;

      if (mode == 2) {
        s32 r = frame;

        x = screen[0] / 2 - 10;
        y = screen[1] - 20;
        if (r > 12)
          r = 12;
        a = frame << 9;
        x -= Trig_Sin(a) * r * 2 >> 16;
        y -= (Trig_Cos(a) * r >> 16) - r;
        if (frame <= 61)
          draw[0](canvas, FIGURE(work), x, y, 20, 40);
      }
      if (frame == 0)
        Sound_PlayFar(SE_SWIRL);
      if (frame == 62)
        Sound_PlayFar(SE_139);
      if (frame == 0) {
        for (i = 0; i != 63; i++) {
          struct EffectStep *p = &work->particles[i];

          p->z = Random16();
          p->velocityZ = (Random16() & 63) + 64;
          Random16();
          p->variant = i + 32;
        }
      }
      EffectPosition_ProjectScaledBattler(effect->actors[0], &target);
      for (i = 0; i != 48; i++) {
        struct EffectStep *p = &work->particles[i];

        if ((u32)p->variant <= 31) {
          s32 n = p->variant / 8 + 2;
          s32 x = (Trig_Sin(p->z) * p->velocityZ / 2 >> 16) + target.x / 2;
          s32 y = (Trig_Cos(p->z) * p->velocityZ >> 16) + target.y;

          p->z += 512;
          p->velocityZ -= 4;
          if (p->velocityZ > 0)
            draw[0](canvas, sprites + gBattleEffect_MoteCells[n - 1], x - n / 2,
                    y - n, n, n * 2);
        }
        p->variant--;
      }
    } else if (mode == 5) {
      struct EffectStep *ball = &work->particles[63];
      struct EffectPosition pos;
      s32 x;
      s32 y;

      if (frame == 0) {
        ball->velocityX = other[0];
        ball->velocityY = other[1];
        ball->velocityZ = other[2];
      }
      if (frame <= 47) {
        s32 *user = BattleObject_GetFar(effect->actor)->object;

        ball->velocityX += (user[2] - other[0]) / 40;
        ball->velocityY += (0x3c0000 - other[1]) / 40;
        ball->velocityZ += (user[4] - other[2]) / 40;
      } else {
        s32 *target = BattleObject_GetFar(effect->actors[0])->object;
        struct EffectStep *fall = &work->particles[62];

        if (frame == 48) {
          SET_POSITION(fall, 0, 0, 0);
          fall->velocityX = (target[2] - ball->velocityX) / 32;
          fall->velocityY = -ball->velocityY / 32;
          fall->velocityZ = (target[4] - ball->velocityZ) / 32;
        }
        ball->velocityX += fall->x;
        ball->velocityY += fall->y;
        ball->velocityZ += fall->z;
        fall->x += fall->velocityX;
        fall->y += fall->velocityY;
        fall->z += fall->velocityZ;
      }
      Transform_Reset();
      Transform_SetLookAt(camera, camera + 3);
      EffectPosition_ProjectPoint(&ball->velocityX, &pos);
      x = pos.x >> 1;
      pos.x = x;
      ball->x = x;
      y = pos.y;
      ball->y = y;
      draw[0](canvas, FIGURE(work), x - 10, y - 20, 20, 40);
      if (frame == 0)
        Sound_PlayFar(SE_SPIN);
      if (frame == 55) {
        struct EffectStep *flash = &work->particles[61];

        flash->x = pos.x;
        flash->y = pos.y;
        BattlePlayback_StartFar(145);
        BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 24);
        work->shakeFrames = 16;
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == 0) {
      s32 b = frame << 11;
      s32 x;
      s32 y;

      x = (Trig_Sin(b) * 20 >> 16) + screen[0] / 2 - 10;
      y = (Trig_Cos(b) * 4 >> 16) + screen[1] - 24;
      if (frame > 48) {
        twice = frame * 2;
        y = y - twice + 96;
      }
      if (y > -40)
        draw[0](canvas, FIGURE(work), x, y, 20, 40);
    }

    // The model layer.
    Transform_Reset();
    gCameraSceneParameters[4] = 104;
    verts = Heap_AllocEwram(0x400);
    model = Render_AllocateModelList(1);
    model->texture = &texture;
    model->vertices = verts;
    if (mode == 0) {
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *p = &work->particles[i];

          p->x = i << 15;
          p->y = px(56);
          p->velocityX = (Random16() & 511) + 512;
          p->velocityY = -((Random16() & 127) + 612) << 7;
          p->variant = -i * 8;
        }
      }
      if (frame == 4)
        Sound_PlayFar(SE_BEAST_CRY);
      if (frame == 48)
        BattlePlayback_StartFar(-1);
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 7;
      model->mesh = Data_08199244;
      for (i = 0; i != 6; i++) {
        struct EffectStep *p = &work->particles[i];

        if (p->variant >= 0) {
          s32 size = 30000;
          s32 shade = -16;

          if (p->variant <= 6)
            shade = p->variant * 8 - 64;
          if (p->variant > 31) {
            shade = (32 - p->variant) * 4;
            if (shade < -64)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 2 << 10);
          Transform_Reset();
          p->y += p->velocityY;
          p->x += p->velocityX;
          SceneTransform_ApplyPosition3(
              (Trig_Sin(p->x) << 4) + ((screen[0] / 2 - 64) << 16), p->y, 0);
          SceneTransform_ApplyScale3(size >> 1, size, size >> 1);
          Transform_ApplyPitch(-TRIG_TURN / 4);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
        p->variant++;
      }
    } else if (mode == 6) {
      s32 k;

      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = Data_08199244;
      model->vertices = verts;
      for (k = 0; k != 1; k++) {
        if (frame >= k * 32) {
          s32 t = frame - k * 32;
          s32 scale = (t << 10) + Q16_ONE / 16;
          s32 shade = 0;
          /* FAKEMATCH(pin): half kept in r5. */
          register s32 half __asm__("r5");

          if (t > 63) {
            shade = (64 - t) * 4;
            if (shade < -64)
              continue;
          }
          model->shade = shade;
          half = scale >> 1;
          Transform_Reset();
          SceneTransform_ApplyScale3(half, scale, scale);
          Transform_ApplyRoll((frame << 8) + TRIG_TURN * 3 / 4);
          Transform_ApplyPitch((frame << 6) - 0x4770);
          for (i = 0; i != 12; i++) {
            texture.pixels = work->sheet + (i % 3 << 10);
            Transform_Push();
            Transform_ApplyRoll(frame * 356 + i * 5461);
            SceneTransform_ApplyPosition3(0, 0x740000, 0);
            SceneTransform_ApplyScale3(half, half, half);
            Render_TransformVertices(gData_08199210, verts, 4);
            Transform_Pop();
            Render_DrawModelList(model);
          }
        }
      }
    } else if (mode == 1) {
      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gData_08199340;
      model->kind = 6;
      texture.pixels = work->sheet;
      for (i = 0; i != effect->count; i++) {
        struct EffectPosition pos;

        EffectPosition_ProjectScaledBattler(effect->actors[i], &pos);
        Transform_Reset();
        SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16,
                                      (pos.y - 48) << 16, 0);
        Transform_ApplyPitch(TRIG_TURN / 4);
        Transform_ApplyYaw((frame << 9) - TRIG_TURN / 16);
        SceneTransform_ApplyScale3(frame * 2596 >> 1, frame * 2596 >> 1,
                                   frame * 2596 >> 1);
        Render_TransformVertices(gData_08199210, verts, 4);
        Render_DrawModelList(model);
      }
    } else if (mode == 5) {
      if (frame <= 47) {
        struct EffectStep *ball;
        s32 twist;

        model->shade = -16;
        texture.logWidth = 5;
        texture.logHeight = 2;
        texture.pixels = work->sheet + 0x4000;
        model->texture = &texture;
        model->kind = 7;
        model->mesh = Data_08198c34;
        model->vertices = verts;
        if (frame == 0) {
          for (i = 0; i != 16; i++) {
            struct EffectStep *p = &work->particles[i];

            p->x = Random16() & 0xffff;
            p->y = (Random16() & 255) + 128;
            if (Random16() & 1)
              p->y = -p->y;
            p->variant = -i * 2;
          }
        }
        for (i = 0, ball = &work->particles[63], twist = frame * 2; i != 8;
             i++) {
          struct EffectStep *p = &work->particles[i];

          p->variant++;
          p->x += p->y * 2;
          model->scrollV = (twist + i * 7) & 31;
          if ((u32)p->variant <= 31) {
            Transform_Reset();
            SceneTransform_ApplyPosition3((ball->x << 16) - 0x400000,
                                          (ball->y << 16) - 0x400000, 0);
            SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE);
            Transform_ApplyRoll(p->x);
            SceneTransform_ApplyScale3(Trig_Sin(p->variant * 682) * 3 +
                                           TRIG_TURN * 3 / 4,
                                       0x20000, 0x10000);
            Render_TransformVertices(gData_081991b0, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
      {
        struct EffectPosition pos;

        EffectPosition_ProjectBattler(effect->actors[0], &pos);
        texture.logWidth = 7;
        texture.logHeight = 7;
        texture.pixels = cells;
        model->kind = 7;
        model->texture = &texture;
        model->mesh = gData_08199364;
        model->vertices = verts;
        for (i = 0; i != 3; i++) {
          s32 t = frame - i * 4 - 55;

          if (t >= 0) {
            s32 scale = (t * 3 << 12) + Q16_ONE / 4;
            s32 shade = 0;

            if (t > 7)
              shade = (8 - t) * 8;
            model->shade = shade;
            if (shade > -64) {
              Transform_Reset();
              SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16,
                                            (pos.y - 64) << 16, 0);
              SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
              Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
              Transform_ApplyPitch(-TRIG_TURN / 16);
              SceneTransform_ApplyUniformScale(scale);
              Render_TransformVertices(gData_08199210, verts, 4);
              Render_DrawModelList(model);
            }
          }
        }
      }
    } else if (mode == 4) {
      if ((u32)(frame - 16) <= 64) {
        s32 t = frame - 16;
        s32 scale;
        struct EffectPosition pos;

        if (t <= 23)
          work->transferValue = 75;
        else
          work->transferValue = 50;
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gData_08199340;
        model->kind = 7;
        texture.pixels = work->sheet;
        EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
        for (i = 0, scale = t * -4592 + Q16_ONE; i != 2;
             scale += Q16_ONE / 4, i++) {
          if (scale > 0) {
            s32 shade = -(i << 4);

            if (shade > 0)
              shade = 0;
            model->shade = shade;
            Transform_Reset();
            SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16,
                                          (pos.y - 48) << 16, 0);
            Transform_ApplyPitch(TRIG_TURN * 3 / 4);
            SceneTransform_ApplyScale3(scale, scale, scale);
            Render_TransformVertices(gData_08199210, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame == 14) {
        Sound_PlayFar(SE_POWER_UP);
      }
      if (frame == 24) {
        BattleEffect_TintBattler(effect->actors[0], 7, -1, 0, 12);
      }
    } else if (mode == 9) {
      struct EffectPosition from;
      struct EffectStep *p;

      EffectPosition_ProjectBattler(effect->actor, &from);
      if (frame == 0) {
        struct EffectPosition to;
        struct EffectStep *ball = &work->particles[63];
        struct EffectStep *q;

        EffectPosition_ProjectScaledBattler(effect->actors[0], &to);
        ball->x = to.x;
        ball->y = to.y;
        for (i = 0; i != 32; i++)
          work->particles[i].variant = -1;
        for (i = 0, q = EXTRA_PARTICLES; i != 256; i++, q++) {
          s32 s = 511;
          s32 a;

          s &= Random16();
          a = Random16();
          s += 64;
          q->velocityX = Trig_Sin(a) * s >> 6;
          q->velocityY = Trig_Cos(a) * s >> 4;
          if (effect->variant == 0) {
            q->velocityX /= 4;
            q->velocityY /= 4;
          } else if (effect->variant == 1) {
            q->velocityX /= 2;
            q->velocityY /= 2;
          }
          q->x = to.x / 2 << 16;
          q->y = (to.y + 16) << 16;
          q->variant = (Random16() & 15) + 8;
        }
      }
      if (frame <= 9) {
        s32 v;
        s32 mask = 255;

        i = 0;
        twice = frame * 2;
        // Index first, as a plain integer sum: the add takes the offset first.
        p = (void *)((twice * 28) + (s32)work->particles);
        p->x = from.x / 2 << 16;
        p->y = (from.y + 8) << 16;
        v = ((Random16() & mask) + 256) << 8;
        p->velocityX = v;
        if (effect->side == 1)
          p->velocityX = -v;
        p->velocityY = -((Random16() & mask) + 256) << 4;
        p->variant = i;
      }
      for (i = 0; i != 32; i++) {
        p = &work->particles[i];

        if ((u32)p->variant <= 23) {
          draw[0](canvas, work->sheet + p->variant / 4 * 1152, HI(p->x) - 12,
                  HI(p->y) - 24, 24, 48);
          BattleEffectKernels_IntegrateVector2(p, 60, -0x1000);
          p->variant++;
        }
      }
      if (frame > 9) {
        for (i = 0; i != Data_08198c64[effect->variant]; i++) {
          struct EffectStep *q = &EXTRA_PARTICLES[i];

          if (q->variant >= 0) {
            s32 n = q->variant / 4 + 3;

            draw[0](canvas, sprites + gBattleEffect_MoteCells[n - 1],
                    HI(q->x) - n / 2, HI(q->y) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(q, 62, 0);
            q->variant--;
          }
        }
      }
      if (frame > 9) {
        struct EffectStep *ball = &work->particles[63];
        s32 scale = ((frame - 10) << 15) + Q16_ONE / 4;
        s32 shade = 0;

        if (effect->variant == 0)
          scale /= 4;
        else if (effect->variant == 1)
          scale /= 2;
        if (frame > 15)
          shade = (16 - frame) * 8;
        if (shade > -64) {
          model->shade = shade;
          texture.logWidth = 7;
          texture.logHeight = 7;
          model->mesh = gData_08199364;
          model->kind = 7;
          texture.pixels = work->sheet + 0x3080;
          Transform_Reset();
          SceneTransform_ApplyPosition3((ball->x / 2 - 64) << 16,
                                        (ball->y - 48) << 16, 0);
          SceneTransform_ApplyScale3(scale / 4, scale, scale / 2);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
      if (frame == 10) {
        BattlePlayback_StartFar(134);
        work->shakeFrames = effect->variant * 2 + 8;
        BattleMotion_ApplyKnockbackFar(effect->actors[0], 0);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 12);
      }
      BattleCamera_Shake(effect->variant * 2 + 8, effect->variant * 2 + 8);
    } else if (mode == 11) {
      if (frame == 0) {
        struct EffectPosition pos;

        EffectPosition_ProjectBattler(effect->actors[0], &pos);
        for (i = 0; i != 32; i++) {
          struct EffectStep *p = &work->particles[i];
          s32 s = 255;
          s32 a;

          s &= Random16();
          a = (Random16() & 0x7fff) + 0x4000;
          s += 128;
          p->velocityX = Trig_Sin(a) * s >> 7;
          p->velocityY = Trig_Cos(a) * s >> 6;
          p->velocityZ = px(1) / 64;
          SET_POSITION(p, pos.x / 2 << 16,
                       (pos.y - (Random16() & 31) + 16) << 16, Random16());
        }
        Sound_PlayFar(SE_POWER_UP);
        work->shakeFrames = 16;
        BattleEffect_TintBattler(effect->actors[0], 10, -1, 0, 32);
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = Data_08199244;
      model->vertices = verts;
      for (i = 0; i != 12; i++) {
        struct EffectStep *p = &work->particles[i];

        if (frame >= i * 2) {
          s32 t = frame - i * 2;
          s32 shade = 0;

          if (t > 23) {
            shade = (24 - t) * 4;
            if (shade < -64)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 3 << 10);
          PLACE_PARTICLE(p);
          SceneTransform_ApplyUniformScale(Q16_ONE / 2);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
          BattleEffectKernels_IntegrateVector3(p, 60, 0);
        }
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == 14) {
      if (frame == 0) {
        struct EffectPosition pos;

        EffectPosition_ProjectBattler(effect->actor, &pos);
        for (i = 0; i != 32; i++) {
          struct EffectStep *p = &work->particles[i];
          s32 s = 255;
          s32 a;

          s &= Random16();
          a = (Random16() & 0x7fff) + 0x4000;
          s += 128;
          p->velocityX = Trig_Sin(a) * s >> 6;
          p->velocityY = Trig_Cos(a) * s >> 6;
          p->velocityZ = px(1) / 64;
          SET_POSITION(p, pos.x / 2 << 16,
                       (pos.y - (Random16() & 31) - 16) << 16, Random16());
        }
        Sound_PlayFar(SE_619);
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = Data_08199244;
      model->vertices = verts;
      for (i = 0; i != 12; i++) {
        struct EffectStep *p = &work->particles[i];

        if (frame >= i * 2) {
          s32 t = frame - i * 2;
          s32 shade = 0;

          if (t > 23) {
            shade = (24 - t) * 4;
            if (shade < -64)
              continue;
          }
          model->shade = shade;
          texture.pixels = work->sheet + (i % 3 << 10);
          PLACE_PARTICLE(p);
          SceneTransform_ApplyUniformScale(Q16_ONE / 2);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
          BattleEffectKernels_IntegrateVector3(p, 60, 0);
        }
      }
    } else if (mode == 10) {
      struct EffectPosition pos;

      if (frame == 0) {
        void *user = BattleObject_GetFar(effect->actor)->object;

        Actor_SelectAnimationFar(user, 0);
        Actor_SelectAnimationFar(user, 2);
        Actor_SetAnimationSpeedFar(user, 0);
        Sound_PlayFar(SE_POWER_UP);
      }
      if (frame == 14)
        Actor_SetAnimationSpeedFar(BattleObject_GetFar(effect->actor)->object,
                                   16);
      if (frame == 22)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == 26) {
        BattlePlayback_StartFar(144);
        work->shakeFrames = 8;
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 12);
        BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
      }
      if (frame > 27) {
        struct EffectStep *q;
        struct EffectStep *p;

        if (frame == 28) {
          EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
          for (q = EXTRA_PARTICLES, i = 0; i != 128; i++, q++) {
            s32 s = 511;
            s32 a;

            s &= Random16();
            a = Random16();
            s += 32;
            q->velocityX = Trig_Sin(a) * s >> 7;
            q->velocityY = Trig_Cos(a) * s >> 6;
            q->x = (pos.x / 2 << 16) + q->velocityX * 2;
            q->y = ((pos.y + 16) << 16) + q->velocityY * 2;
            q->variant = (Random16() & 15) + 16;
          }
        }
        for (p = EXTRA_PARTICLES, i = 0; i != 64; i++, p++) {
          if (p->variant >= 0) {
            s32 j;

            for (j = 0; j != 4; j++) {
              s32 n = p->variant / 8 + 1;

              draw[0](canvas, sprites + gBattleEffect_MoteCells[n - 1],
                      HI(p->x) - n / 2, HI(p->y) - n, n, n * 2);
              BattleEffectKernels_IntegrateVector2(p, 63, 0x800);
              if (p->y > 0x680000) {
                p->y = px(104);
                p->velocityY = -p->velocityY / 2;
              }
            }
            p->variant--;
          }
        }
      }
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      texture.logWidth = 5;
      texture.logHeight = 7;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = gData_08199268;
      model->vertices = verts;
      Transform_Reset();
      SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE);
      if (effect->side == 0)
        SceneTransform_ApplyPosition3((pos.x - 136) << 16, (pos.y - 48) << 16,
                                      0);
      else
        SceneTransform_ApplyPosition3((pos.x - 108) << 16, (pos.y - 56) << 16,
                                      0);
      for (i = 0; i != 4; i++) {
        s32 scale;

        Transform_Push();
        texture.pixels = work->sheet + ((Random16() & 3) << 12);
        texture.logWidth = 5;
        texture.logHeight = 7;
        if (frame > 23) {
          s32 d = frame * 16 - i * 2 - 352;
          s32 angle;

          if (d < 0)
            continue;
          if (d > 80)
            d = 80;
          if (effect->side == 0)
            SceneTransform_ApplyPosition3(d * 3 << 13, d * 7 << 11, 0);
          else
            SceneTransform_ApplyPosition3(-(d * 3 << 13), d * 7 << 11, 0);
          angle = d * 48152 / 80 + TRIG_TURN * 3 / 8;
          if (effect->side == 0)
            angle = -angle;
          Transform_ApplyRoll(angle);
        } else {
          if (i != 0)
            continue;
          Transform_ApplyRoll(effect->side == 0 ? -TRIG_TURN * 3 / 8
                                                : TRIG_TURN * 3 / 8);
        }
        model->shade = -i << 3;
        if (frame <= 23)
          scale = frame * 2730;
        else if (frame > 31)
          scale = Q16_ONE * 4 - (frame * 3 << 11);
        else
          scale = Q16_ONE;
        if (scale > 0) {
          SceneTransform_ApplyScale3(scale * 6, scale * 2, Q16_ONE);
          Render_TransformVertices(Data_081991d0, verts, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == 8) {
      struct EffectPosition pos;

      work->transferValue = 50;
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->mesh = gData_08199364;
      model->kind = 6;
      work->transferMode = 1;
      work->transferValue = 0;
      texture.pixels = work->sheet;
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      if (frame <= 15) {
        for (i = 0; i != 2; i++) {
          s32 scale = Trig_Sin(frame << 11) * 5;

          if (scale > 0) {
            model->shade = 0;
            Transform_Reset();
            SceneTransform_ApplyPosition3((pos.x / 2 + i * 16 - 72) << 16,
                                          (pos.y - 64) << 16, 0);
            SceneTransform_ApplyScale3(scale / 2, scale, scale / 2);
            Transform_ApplyRoll(TRIG_TURN / 8);
            Render_TransformVertices(Data_081991e0, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame == 8) {
        Sound_PlayFar(SE_POWER_UP);
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
        for (i = 0; i != effect->count; i++)
          BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 7);
      }
      BattleCamera_Shake(8, 8);
    } else if (mode == 13) {
      struct EffectPosition pos;
      s32 shade;

      if (frame == 0)
        Sound_PlayFar(SE_104);
      EffectPosition_ProjectBattler(effect->actors[0], &pos);
      shade = frame * 4 - 64;
      if (shade >= -16)
        shade = -16;
      model->shade = shade;
      model->texture = &texture;
      model->vertices = verts;
      model->kind = 6;
      model->mesh = Data_08198ec4;
      texture.pixels = work->sheet;
      texture.logWidth = 7;
      texture.logHeight = 4;
      if (frame == 26) {
        work->shakeFrames = 8;
        BattlePlayback_StartFar(134);
      }
      for (i = 0; i != 8; i++) {
        s32 scale = Q16_ONE * 2;
        s32 start = i * 2 + 16;

        if (frame >= start)
          scale = Q16_ONE * 2 - ((frame - start) << 13);
        if (scale > Q16_ONE / 2) {
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Transform_Reset();
          SceneTransform_ApplyPosition3((pos.x / 2 - 60) << 16, 0x80000, 0);
          SceneTransform_ApplyScale3(scale >> 1, scale, Q16_ONE);
          Transform_ApplyPitch((i << 13) + 4000);
          Transform_ApplyYaw(frame << 9);
          Transform_ApplyRoll((frame + i * 16) << 9);
          SceneTransform_ApplyScale3(Q16_ONE, Q16_ONE / 2, Q16_ONE);
          Render_TransformVertices(Data_08198cac, verts, 32);
          Render_DrawModelList(model);
        }
      }
    } else if (mode == 12) {
      struct EffectPosition pos;
      s32 j;

      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = cells;
      model->mesh = gData_08199364;
      model->kind = 6;
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      Transform_Reset();
      SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16, (pos.y - 48) << 16,
                                    0);
      if (frame <= 15) {
        s32 scale = Q16_ONE * 3 - (frame * 3 << 12);

        model->shade = 0;
        SceneTransform_ApplyScale3(scale >> 1, scale, scale >> 1);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
      Transform_Reset();
      SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16, (pos.y - 48) << 16,
                                    0);
      if ((u32)(frame - 12) <= 31) {
        s32 scale = Trig_Sin((frame << 10) - TRIG_TURN * 3 / 16) * 3;

        texture.pixels = work->sheet;
        SceneTransform_ApplyScale3(scale / 2, scale, scale / 2);
        Transform_ApplyRoll((frame << 9) + TRIG_TURN / 8);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
      if (frame == 0) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &work->particles[i];

          Random16();
          p->x = 768 + i * 24;
          p->velocityX = -p->x / 48;
          p->y = Random16();
        }
        LOAD_BG_PALETTE(ResourceId_BlastSheet);
      }
      if (frame <= 15) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &work->particles[i];

          for (j = 0; j != 3; j++) {
            s32 x = (Trig_Sin(p->y) * p->x >> 21) + pos.x / 2;
            s32 y = (p->x * Trig_Cos(p->y) >> 20) + 60;
            s32 n = 4;

            draw[0](canvas, sprites + gBattleEffect_MoteCells[n - 1], x - n / 2,
                    y - n, n, n * 2);
            p->x += p->velocityX;
          }
        }
      }
      for (i = 0; i != 3; i++) {
        if (frame > 7) {
          if (frame <= 15)
            BattleEffect_StepPaletteToResource(
                (s32)&ResourceId_MercuryDjinnSheet);
          else if (frame <= 23)
            BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
          else
            BattleEffect_StepPaletteToResource(
                (s32)&ResourceId_JupiterDjinnSheet);
        }
      }
      if (frame == 16) {
        Sound_PlayFar(SE_POWER_UP);
        BattleEffect_TintBattler(effect->actors[0], 7, -1, 0, 30);
      }
      if (frame == 28)
        BattlePlayback_StartFar(-1);
    } else if (mode == 15) {
      struct EffectPosition pos;

      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = cells;
      model->mesh = gData_08199364;
      model->kind = 6;
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      for (i = 0; i != 2; i++) {
        s32 t = frame - i * 8;

        Transform_Reset();
        SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16,
                                      (pos.y - 48) << 16, 0);
        if ((u32)t <= 15) {
          s32 scale = Q16_ONE * 4 - (t << 14);
          s32 shade = 0;

          if (t <= 7)
            shade = t * 8 - 64;
          model->shade = shade;
          SceneTransform_ApplyScale3(scale / 2, scale, scale / 2);
          Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
          Transform_ApplyPitch(-TRIG_TURN / 16);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Transform_Reset();
      SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16, (pos.y - 48) << 16,
                                    0);
      if ((u32)(frame - 12) <= 31) {
        s32 scale = Trig_Sin((frame << 10) - TRIG_TURN * 3 / 16) * 2;

        texture.pixels = work->sheet;
        SceneTransform_ApplyScale3(scale / 2, scale, scale / 2);
        Transform_ApplyRoll((frame << 9) + TRIG_TURN / 8);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
      if (frame == 0)
        Graphics_LoadGreyRamp();
      if (frame == 16) {
        Sound_PlayFar(SE_POWER_UP);
        BattleEffect_TintBattler(effect->actors[0], 7, -1, 0, 30);
      }
      if (frame == 28)
        BattlePlayback_StartFar(-1);
    } else {
      s32 scale;

      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gData_08199340;
      model->kind = 7;
      texture.pixels = work->sheet;
      if (mode == 2 || mode == 7) {
        if (frame <= 31)
          scale = frame * 1500;
        else if (frame > 59)
          scale = 228000 - frame * 3000;
        else
          scale = 48000;
      } else {
        if (frame <= 31)
          scale = frame * 1500;
        else if (frame > 39)
          scale = 168000 - frame * 3000;
        else
          scale = 48000;
      }
      if (scale > 0) {
        struct EffectPosition pos;

        if (mode == 2 || mode == 7)
          EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
        else
          EffectPosition_ProjectScaledBattler(effect->actor, &pos);
        Transform_Reset();
        SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16,
                                      (pos.y - 48) << 16, 0);
        Transform_ApplyPitch(TRIG_TURN / 4);
        SceneTransform_ApplyScale3(scale / 3, scale, scale);
        if (mode == 2)
          Transform_ApplyYaw(-frame << 10);
        else
          Transform_ApplyYaw(frame << 10);
        Render_TransformVertices(gData_08199210, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    BattleEffect_CountDownBattlerTints();
    if (mode == 1)
      WAIT_TRANSFER(work) {}
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(SLOT_BLITTER);
  if (mode == 0 || mode == 6)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef EXTRA_PARTICLES
#undef FIGURE
#undef LOAD_BG_PALETTE
#undef PLACE_PARTICLE
#undef WAIT_TRANSFER
