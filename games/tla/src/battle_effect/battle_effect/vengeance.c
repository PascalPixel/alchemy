/// @file
/// Vengeance and Life Shear, a geyser, swarms of stars and a column of bubbles,
/// and the Coatlicue summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

// Vengeance: the avenger's blades.

struct FxPartyList;
struct Model;
struct StripQuad;

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
#define HI(v) (((s16 *)&(v))[1]) // the pixel half of a 16.16 value

extern char ResourceId_Data0B0;
extern const u8 Data_081995f3[][7]; // per level: which blades strike

#define VENGEANCE_FRAMES 124
#define BLADES 7

/// Abilities: Vengeance.
///
/// The avenger (a tiled background in a window) slides in from the far
/// side and raises its arm every twelve frames from frame 22; each time a
/// blade (one of seven) flies across, and those the level picks strike the
/// target, the last one hardest. Everything is mirrored on the right side.
void BattleEffect_RunVengeance(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  // The plain and the flipped blitter, as one pair: as two locals the
  // blade loop no longer hoists them from memory.
  DrawRectangle draw[2];
  struct EffectPosition pos;
  struct EffectStep *blade;
  s32 backgroundX;
  s32 speed;
  s32 raised;
  s32 row;
  s32 column;
  s32 i;
  s32 strikes;
  s32 mask;
  s32 frames;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  mask = 7;
  BattleEffect_StartTiledCanvas(0);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x410;

  // The avenger's tile map: the body, and its arm in rows 3 to 6.
  i = 0;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 16; column++, i += 2) {
      s32 tileX;
      s32 tileY;
      s32 tile;
      s16 entry;

      if (effect->side == 0) {
        if (column > 7) {
          goto body;
        }
        if ((u32)(row - 3) > 3) {
          goto blank;
        }
        tileX = column;
        tileY = row + 9;
      } else {
        if (column <= 7) {
        body:
          if (row > 11) {
            goto blank;
          }
          tileX = column & mask;
          tileY = row;
        } else {
          if ((u32)(row - 3) > 3) {
            goto blank;
          }
          tileX = column & mask;
          tileY = row + 9;
        }
      }
      goto store;
    blank:
      tileX = 0;
      tileY = 0;
    store:
      tile = (tileY * 8 + tileX) * 2;
      entry = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + i) = entry;
    }
  }
  REG_BG2Y = 0xffffd800;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x2888;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x2888;
  REG_WININ = 0x3737;
  REG_WINOUT = 0x2033;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B0, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(
      SLOT_BLITTER,
      BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(
      188, BLITTER(BLIT_TRANSPARENT,
                   COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  backgroundX = 0x560000;
  speed = 0x100000;
  frames = VENGEANCE_FRAMES;

  for (frame = 0; frame != frames; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
    if (frame <= 31) {
      if (effect->side == 0) {
        camera->yaw = (s16)camera->yaw - 128;
      } else {
        camera->yaw = (s16)camera->yaw + 128;
      }
    }
    if (frame >= frames - 16) {
      REG_BLDALPHA = (frames - frame - 1) | 0x1000;
    }
    if (frame == 0) {
      if (effect->side == 0) {
        backgroundX = 0x560000;
        speed = 0x100000;
      } else {
        backgroundX = 0xffaa0000;
        speed = 0xfff00000;
      }
      for (i = 0; i != BLADES; i++) {
        blade = &work->particles[i];
        if (effect->side == 0) {
          blade->x = px(160);
          blade->velocityX = px(12);
        } else {
          blade->x = px(32);
          blade->velocityX = -(px(12));
        }
        blade->y = px(12);
        strikes = Data_081995f3[effect->variant][i];
        if (strikes != 0) {
          strikes = 1;
        }
        blade->z = strikes;
        blade->velocityY = (Random16() & 0xffff) - 0x8000;
        blade->velocityZ = 0;
        blade->variant = 0;
      }
    }

    // The avenger slides in, its window following.
    if (frame <= 24) {
      REG_BG2X = -(backgroundX >> 16) << 8;
      if (effect->side == 0) {
        REG_WIN0H = ((backgroundX >> 16) + 120) << 8 | 0xf0;
        REG_WIN1H = ((backgroundX >> 16) + 120) << 8 | 0xf0;
      } else {
        REG_WIN0H = (backgroundX >> 16) + 240;
        REG_WIN1H = (backgroundX >> 16) + 240;
      }
    } else {
      REG_BG2X = -(backgroundX >> 16) << 8;
      REG_WIN0H = 0xf0;
      REG_WIN1H = 0xf0;
    }
    backgroundX -= speed;
    speed = speed * 52 / 64;

    raised = 0;
    if (frame > 105) {
      raised = 1;
    } else if (frame > 21 && (frame - 22) % 12 > 3) {
      raised = 1;
    }
    if (effect->side == 0) {
      if (raised == 0) {
        draw[0](canvas, work->sheet, 32, 0, 88, 96);
      } else {
        draw[0](canvas, work->sheet + 0x2100, 32, 0, 88, 96);
      }
    } else if (raised == 0) {
      draw[1](canvas, work->sheet, 8, 0, 88, 96);
    } else {
      draw[1](canvas, work->sheet + 0x2100, 8, 0, 88, 96);
    }

    // The blades.
    for (i = 0; i != BLADES; i++) {
      blade = &work->particles[i];
      if (frame < i * 12 + 22 && (frame > 21 || i != 0)) {
        continue;
      }
      if (frame >= i * 12 + 26 && blade->variant == 0) {
        blade->x -= blade->velocityX;
        blade->y += blade->velocityY;
      }
      if (blade->variant > 4) {
        continue;
      }
      if (blade->variant > 0) {
        blade->variant++;
      }
      if (blade->z == 1 && blade->variant == 0) {
        s32 hit = 0;

        if (effect->side == 0) {
          if ((blade->x >> 16) < pos.x / 2 + 16) {
            hit = 1;
          }
        } else if ((blade->x >> 16) > pos.x / 2 + 48) {
          hit = 1;
        }
        if (hit == 1) {
          work->shakeFrames = 4;
          if (i == 6) {
            BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
            BattlePlayback_StartFar(134);
          } else {
            BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000,
                                          0x80000, 0x20000, i * 10 + 110);
            Sound_PlayFar(SE_IMPACT);
          }
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
          blade->variant = 1;
          if (effect->side == 0) {
            blade->x = pos.x / 2 - HI(blade->x);
          } else {
            blade->x = pos.x / 2 - HI(blade->x) + 128;
          }
        }
      }
      if (effect->side == 0) {
        if (blade->variant == 0) {
          draw[0](canvas, work->sheet + 0x42cc, (blade->x >> 16) - 111,
                  HI(blade->y) + 24, 51, 12);
          draw[0](canvas, work->sheet + 0x42cc, HI(blade->x) + 17,
                  HI(blade->y) + 96, 51, 12);
          draw[0](canvas, work->sheet + 0x4200, HI(blade->x) - 128,
                  HI(blade->y) + 24, 17, 12);
          draw[0](canvas, work->sheet + 0x4200, HI(blade->x), HI(blade->y) + 96,
                  17, 12);
        } else {
          draw[0](canvas, work->sheet + 0x42cc, pos.x / 2 + blade->x - 111,
                  HI(blade->y) + 24, 51, 12);
          draw[0](canvas, work->sheet + 0x42cc, pos.x / 2 + blade->x + 17,
                  HI(blade->y) + 96, 51, 12);
        }
      } else if (blade->variant == 0) {
        draw[1](canvas, work->sheet + 0x42cc, blade->x >> 16, HI(blade->y) + 24,
                51, 12);
        draw[1](canvas, work->sheet + 0x42cc, HI(blade->x) - 128,
                HI(blade->y) + 96, 51, 12);
        draw[1](canvas, work->sheet + 0x4200, HI(blade->x) + 51,
                HI(blade->y) + 24, 17, 12);
        draw[1](canvas, work->sheet + 0x4200, HI(blade->x) - 77,
                HI(blade->y) + 96, 17, 12);
      } else {
        draw[1](canvas, work->sheet + 0x42cc, pos.x / 2 + blade->x,
                HI(blade->y) + 24, 51, 12);
        draw[1](canvas, work->sheet + 0x42cc, pos.x / 2 + blade->x - 128,
                HI(blade->y) + 96, 51, 12);
      }
    }

    // The avenger's eye, while the arm is down.
    if (raised == 0) {
      if (effect->side == 0) {
        draw[0](canvas, work->sheet + 0x4530, 88, 29, 14, 13);
      } else {
        draw[1](canvas, work->sheet + 0x4530, 26, 29, 14, 13);
      }
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(1, 4);
    work->transferPending = 1;
    wait(1);
  }
  REG_WIN0V = 0x1088;
  REG_WIN1V = 0x1088;
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

// The scythe, in the work's first particle: swing is its velocityZ and timer
// its variant.
struct Scythe {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 swing;
  s32 timer;
};

extern char ResourceId_Data0F5;
extern u8 gMapCellBuffer[];
extern u8 Data_020112e8[];        // the reaper's raised pose
extern u8 Data_0201276b[];        // the blade's texture
extern u8 Data_0201476b[];        // the handle's texture
extern u8 Data_0201576b[];        // the flash's texture
extern const u8 Data_08199608[];  // the blade's mesh
extern const u8 Data_0819962c[];  // the handle's mesh
extern const u8 Data_08199650[];  // their corners
extern const u8 Data_08199244[];  // the flash's mesh
extern const u8 gData_081991f0[]; // its corners

#define LIFE_SHEAR_FRAMES 90

// Abilities: Life Shear.
//
// The reaper (a tiled background picture) glides in from the far side and
// swings its scythe (a two-part model) through the target at frame 50, with
// a four-blade flash; on the right side everything is mirrored. From frame
// 62 the reaper drifts off and the scythe falls away.
void BattleEffect_RunLifeShear(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit[2];
  struct EffectPosition target;
  struct ModelTexture texture;
  s32 reaperX;
  s32 reaperY;
  s32 speed;
  struct EffectPosition *position;
  s32 shift;
  s32 frame;
  void *vertices;
  struct RenderModel *model;
  struct Scythe *scythe;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  if (effect->side == 0) {
    BattleEffect_StartTiledCanvas(0x2001);
  } else {
    BattleEffect_StartTiledCanvasFilled(0x2001);
  }
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x610;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F5, gMapCellBuffer, 1, 1);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER,
        BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                             BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X |
                                                           COMPOSITE_CLIP_Y |
                                                           COMPOSITE_FLIP_X));
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  reaperX = 0;
  reaperY = 0;
  speed = 0;
  EffectPosition_ProjectScaledBattler(effect->actors[0], position = &target);

  for (frame = 0; frame != LIFE_SHEAR_FRAMES; frame++) {
    s32 x;
    s32 phase;
    s32 scale;
    s32 timer;
    s32 zero;
    s32 still;
    s32 width;

    shift = 0;
    if (frame == 0) {
      Sound_PlayFar(SE_CURSE_FALL);
      if (effect->side == 0) {
        work->particles[0].x = px(32);
        work->particles[0].y = 0xffc00000;
        work->particles[0].z = 0xffffe3d0;
      } else {
        work->particles[0].x = 0xffe00000;
        work->particles[0].y = 0xffc00000;
        work->particles[0].z = 0xffffe3d0;
      }
      work->particles[0].velocityZ = 0;
      work->particles[0].variant = 0;
      reaperY = 0x180000;
      if (effect->side == 0) {
        reaperX = (position->x + 176) << 16;
        speed = 0xf0000;
      } else {
        reaperX = (position->x - 304) << 16;
        speed = 0xfff10000;
      }
    }
    if (frame > 61) {
      if (effect->side == 0) {
        reaperX += 0x20000;
      } else {
        reaperX -= 0x20000;
      }
      reaperY -= 0x4aaaa;
    }
    reaperX -= speed;
    speed = speed * 60 / 64;
    x = reaperX >> 16;
    if (x > 120) {
      shift = x - 120;
      x = 120;
    } else if (x < 0) {
      shift = x;
      x = 0;
    }
    REG_BG2X = -x << 8;
    if ((u32)(frame - 24) <= 39) {
      x = effect->side == 0 ? 64 : 5;
      blit[0](canvas, Data_020112e8, x + shift, reaperY >> 16, 59, 89);
    } else {
      if (effect->side == 0) {
        x = frame * 2 + 16;
        if (x > 64) {
          x = 64;
        }
      } else {
        x = 57 - frame * 2;
        if (x <= 8) {
          x = 9;
        }
      }
      blit[0](canvas, gMapCellBuffer, x + shift, reaperY >> 16, 55, 88);
    }

    // The scythe's path: still, a backswing at 24, the strike at 33.
    i = 0;
    still = 0;
    scythe = (struct Scythe *)work->particles;
    for (; i != 1; i++, scythe++) {
      if (++scythe->timer <= 0) {
        continue;
      }
      if (scythe->timer == 1) {
        scythe->velocityX = still;
        scythe->velocityY = still;
      }
      if (scythe->timer == 24) {
        scythe->velocityX = -(px(4));
        scythe->velocityY = px(8);
      }
      if (scythe->timer == 33) {
        scythe->velocityX = px(4);
        scythe->velocityY = -(px(8));
        work->shakeFrames = 4;
        speed = 0;
        Sound_PlayFar(SE_IMPACT);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
      }
      if (scythe->timer == 35) {
        scythe->velocityX = still;
        scythe->velocityY = still;
      }
      if (frame > 63) {
        s32 fall = (frame << 7) - 0x2000;

        scythe->velocityY -= px(1) / 4;
        if (fall > 0x400) {
          fall = 0x400;
        }
        scythe->z -= fall;
      }
      if (scythe->timer > 8) {
        if (effect->side == 0) {
          scythe->x += scythe->velocityX;
        } else {
          scythe->x -= scythe->velocityX;
        }
        scythe->y += scythe->velocityY;
      }
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    zero = 0;
    phase = frame - 50;
    if (phase == -4) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (phase == 0) {
      BattlePlayback_StartFar(134);
      work->shakeFrames = 12;
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
    }
    timer = work->particles[0].variant;
    if ((u32)(frame - 44) <= 13) {
      work->particles[0].velocityZ++;
    }
    if (timer >= 0) {
      scale = 0xb1e0;
      if (frame > 63) {
        scale = scale - (frame << 9) + Q16_ONE / 2;
      }
      width = 7;
      if (scale > 0) {
        model->shade = zero;
        model->kind = 9;
        model->vertices = vertices;
        model->texture = &texture;
        Transform_Reset();
        SceneTransform_ApplyPosition3(work->particles[0].x + (shift << 16),
                                      work->particles[0].y, 0);
        if (effect->side == 1) {
          Transform_ApplyYaw(TRIG_TURN / 2);
        }
        Transform_ApplyRoll(work->particles[0].z);
        SceneTransform_ApplyUniformScale(scale);
        texture.logHeight = 5;
        texture.pixels = Data_0201476b;
        texture.logWidth = width;
        model->mesh = Data_0819962c;
        Transform_Push();
        Transform_ApplyRoll(
            (Trig_Sin(work->particles[0].velocityZ << 11) >> 3) -
            TRIG_TURN / 8);
        Render_TransformVertices(Data_08199650, vertices, 4);
        Render_DrawModelList(model);
        Transform_Pop();
        model->mesh = Data_08199608;
        texture.logWidth = width;
        texture.logHeight = 6;
        texture.pixels = Data_0201276b;
        Transform_ApplyRoll(
            TRIG_TURN / 8 -
            (Trig_Sin(work->particles[0].velocityZ << 11) >> 3));
        SceneTransform_ApplyScale3(Q16_ONE, 0x15f90, Q16_ONE);
        Render_TransformVertices(Data_08199650, vertices, 4);
        Render_DrawModelList(model);
      }

      // The flash: four blades, shrinking over eight frames.
      if ((u32)phase <= 7) {
        scale = 0x78e20 - (frame << 13);
        Transform_Reset();
        if (effect->side == 0) {
          SceneTransform_ApplyPosition3(0xfff00000, 0, 0);
        } else {
          SceneTransform_ApplyPosition3(0x100000, 0, 0);
        }
        SceneTransform_ApplyScale3(scale, scale, scale);
        if (effect->side == 1) {
          Transform_ApplyYaw(TRIG_TURN / 2);
        }
        model->kind = 6;
        model->mesh = Data_08199244;
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = Data_0201576b;
        for (i = 0; i != 4; i++) {
          Transform_Push();
          Transform_ApplyRoll(i * (TRIG_TURN / 4) + 0x5650);
          SceneTransform_ApplyScale3(
              Q16_ONE / 4, ((i & 1) << 15) + Q16_ONE * 3 / 4, Q16_ONE / 2);
          SceneTransform_ApplyUniformScale(Q16_ONE);
          Render_TransformVertices(gData_081991f0, vertices, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

void BattleEffect_RunGeyser(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Purgatory (kind 114).
/* VIEW(f1811d4): tag struct Texture as struct Texture__f1811d4;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:49937: structure has no member
 * named `size'. */
DEFINE_EFFECT_KIND(BattleEffect_RunPurgatory, BattleEffect_RunGeyser, 0)

/// Abilities: Djinn Blast (kind 461).
DEFINE_EFFECT_KIND(BattleEffect_RunDjinnBlast, BattleEffect_RunGeyser, 2)

/// Abilities: Djinn Storm (kind 462).
DEFINE_EFFECT_KIND(BattleEffect_RunDjinnStorm, BattleEffect_RunGeyser, 1)

#define HEAP_SLOT_BLITTER 104           // Ram_HeapSlots->unk64[1]
#define HEAP_SLOT_BLITTER_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

#define GEYSER_FRAMES 54
#define GEYSER_DRAW_FRAMES 40
#define GEYSER_PEAK_FRAME 20 // the result phase starts
#define DROP_COUNT 512       // drops in the map cell buffer, used in a ring

// A texture: log2 of its width and height in the first word, then its
// pixels.
struct Texture__f1811d4 {
  u32 size;
  void *pixels;
};

// One entry of the list Render_DrawModelList draws; kind 7 is a textured mesh
// whose texture scrolls by scrollU and scrollV.
/* VIEW(f1811d4): tag struct RenderPrimitive as struct RenderPrimitive__f1811d4;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:49933: structure has no member
 * named `scrollU'. */
struct RenderPrimitive__f1811d4 {
  s32 kind;
  s32 unk04;
  const void *faces;
  void *vertices;
  struct Texture__f1811d4 *texture;
  s32 brightness;
  u8 scrollU;
  u8 scrollV;
};

extern char ResourceId_Data134;
extern char ResourceId_CrossTextureSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0D2;
extern char ResourceId_BlastSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_JupiterDjinnSheet;

// The life of each drop in the map cell buffer: gMapCellBuffer's
// EffectSteps from their variant field on.
extern s32 Data_02010018[];

// Per mode: the drops' gravity, and their smallest cel.
extern const s32 Data_08199660[];
extern const s8 Data_0819966c[];

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern u16 gBattleEffect_MoteCells[];

// The geyser: a mesh of sixteen corners and its faces.
extern const u8 gData_08198c6c[];
extern const u8 Data_08198df8[];

static inline void CopyPalette(const void *palette) {
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, 128);
}

void BattleEffect_GetTargetsMidpoint(struct EffectPosition *midpoint);
void BattleEffect_StepPaletteToResource(s32 resourceId);

/// A geyser between the targets. A textured column, its texture scrolling
/// upward, stands on the floor at the targets' midpoint, and drops burst
/// from its foot, fall back under gravity and bounce once on the floor.
/// Mode 0 (kind 114): the cross texture in orange; the column rises and
/// falls on a sine and 200 drops burst at once. Mode 1 (kind 462): the
/// column keeps widening and fades from frame 32, with 200 drops and 16 more
/// each frame to frame 23. Mode 2 (kind 461): a thin column with 100 drops
/// and 2 more each frame. In modes 1 and 2 the palette also steps through
/// the Mercury, Mars and Jupiter Djinn's. At frame 20 the target is knocked
/// back and the result phase starts.
void BattleEffect_RunGeyser(struct BattleEffectArgument *effect, s32 mode) {
  struct Texture__f1811d4 texture;
  DrawRectangle draw[2];
  struct EffectPosition midpoint;
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *sparkCels = Ram_HeapSlots->unk64[0];
  struct EffectStep *drop;
  struct RenderPrimitive__f1811d4 *column;
  void *vertices;
  s32 frame;
  s32 i;
  s32 count;
  s32 scale;
  s32 brightness;

  BattleEffect_StartCanvasLayer(1);
  if (mode == 0)
    REG_BLDALPHA = 0x0c10;
  else
    REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    s32 spawned;
    work->transferMode = 2;
    work->transferValue = 75;
    Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
    spawned = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
    if (mode == 0) {
      Resource_LoadAndDecompress((s32)&ResourceId_CrossTextureSheet,
                                 work->sheet, 1, 1);
      CopyPalette(Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB));
    } else {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0D2, work->sheet, 0, 0);
      CopyPalette(Resource_GetTableEntry((s32)&ResourceId_BlastSheet));
    }

    for (frame = 0; frame != GEYSER_FRAMES; frame++) {
      // The geyser follows the targets' midpoint while it rises.
      if (frame < GEYSER_PEAK_FRAME)
        BattleEffect_GetTargetsMidpoint(&midpoint);
      if (frame == 0) {
        Sound_PlayFar(SE_CLATTER);
        /* FAKEMATCH(dead): an empty once-only loop after the call. */
        do {
        } while (0);
        work->shakeFrames = 16;
      }
      if (frame == GEYSER_PEAK_FRAME) {
        work->shakeFrames = 8;
        BattlePlayback_StartFar(134);
        BattleMotion_ApplyKnockbackFar(effect->actors[0], 1);
      }

      // The first frame clears every drop.
      if (frame == 0) {
        s32 *life;

        spawned = 0;
        i = 0;
        life = Data_02010018;
        for (; i != DROP_COUNT; i++) {
          *life = -1;
          life += 7;
        }
      }

      // Modes 1 and 2 step the palette three times a frame from frame 8:
      // toward the Mercury Djinn's, from frame 20 the Mars Djinn's and from
      // frame 32 the Jupiter Djinn's.
      if (mode == 1 || mode == 2) {
        for (i = 0; i != 3; i++) {
          if (frame >= 8) {
            if (frame < 20)
              BattleEffect_StepPaletteToResource(
                  (s32)&ResourceId_MercuryDjinnSheet);
            else if (frame < 32)
              BattleEffect_StepPaletteToResource(
                  (s32)&ResourceId_MarsDjinnSheet);
            else
              BattleEffect_StepPaletteToResource(
                  (s32)&ResourceId_JupiterDjinnSheet);
          }
        }
      }

      // How many drops burst this frame.
      count = 0;
      if (mode == 2) {
        if (frame == 0)
          count = 100;
        if (frame >= 1 && frame < 24)
          count = 2;
      } else {
        if (frame == 0)
          count = 200;
        if (mode == 1 || mode == 2) {
          if (frame >= 1 && frame < 24)
            count = 16;
        }
      }
      for (i = 0; i != count; i++, spawned++) {
        s32 speed;
        s32 angle;

        drop = &((struct EffectStep *)Ram_MapCellBuffer)[spawned & 511];
        speed = (Random16() & 1023) + 32;
        angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
        drop->x = (midpoint.x / 2) << 16;
        drop->y = px(108);
        if (mode == 2)
          drop->velocityX = (Trig_Sin(angle) * speed) >> 8;
        else
          drop->velocityX = (Trig_Sin(angle) * speed) >> 6;
        drop->velocityY = -(Trig_Cos(angle) * speed * 2) >> 7;
        drop->variant = (Random16() & 7) + 48;
      }

      // Each drop falls, bounces off the floor at y 120, and is drawn while
      // it is on the canvas, its cel shrinking as it burns down.
      drop = (struct EffectStep *)Ram_MapCellBuffer;
      for (i = 0; i != DROP_COUNT; i++, drop++) {
        if (drop->variant > 0) {
          s32 x;
          s32 y;

          drop->variant--;
          BattleEffectKernels_IntegrateVector2(drop, 60, Data_08199660[mode]);
          y = drop->y >> 16;
          if (y > 120) {
            drop->velocityY = -drop->velocityY / 2;
          } else if (drop->x >= 0 && (x = drop->x >> 16) <= 126 &&
                     drop->y >= 0) {
            s32 size = (drop->variant - 16) / 8;

            if (size < Data_0819966c[mode])
              size = Data_0819966c[mode];
            draw[i & 1](canvas, sparkCels + gBattleEffect_MoteCells[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }

      // The column, for the first 40 frames.
      if (frame < GEYSER_DRAW_FRAMES) {
        vertices = Heap_AllocEwram(512);
        column = Render_AllocateModelList(1);
        texture.size = (texture.size & ~0xff) | 5;
        texture.size = (texture.size & ~0xff00) | 5 << 8;
        texture.pixels = work->sheet;
        column->scrollU = (frame << 2) & 31;
        column->scrollV = (frame << 1) & 31;
        Transform_Reset();
        SceneTransform_ApplyPosition3((midpoint.x / 2 - 64) << 16, -px(16), 0);
        Transform_ApplyPitch(2000);
        if (mode == 0) {
          // Up on a sine in 16 frames, held, and down again from frame 24.
          if (frame < 16)
            scale = Trig_Sin(frame << 10);
          else if (frame < 24)
            scale = Q16_ONE;
          else
            scale = Trig_Sin((frame << 10) - TRIG_TURN / 8);
          brightness = -32;
        } else if (mode == 1) {
          // Wider every frame, fading from frame 32.
          scale = frame << 12;
          brightness = -32;
          if (frame > 31)
            brightness = 224 - (frame << 3);
        } else {
          scale = Q16_ONE / 4;
          brightness = -32;
          if (frame > 31)
            brightness = 224 - (frame << 3);
        }
        column->kind = 7;
        column->brightness = brightness;
        column->faces = Data_08198df8;
        column->texture = &texture;
        column->vertices = vertices;
        SceneTransform_ApplyScale3(scale * 2, Q16_ONE * 7, scale * 2);
        Render_TransformVertices(gData_08198c6c, vertices, 16);
        Render_DrawModelList(column);
        Heap_Release(column);
        Heap_Release(vertices);
      }

      BattleCamera_Shake(4, 16);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }
  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void BattleEffect_RunStarSwarm(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Djinn Stun (kind 465).
DEFINE_EFFECT_KIND(BattleEffect_RunDjinnStun, BattleEffect_RunStarSwarm, 0)

/// Abilities: Mystic Call (kind 466).
DEFINE_EFFECT_KIND(BattleEffect_RunMysticCall, BattleEffect_RunStarSwarm, 1)

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3.
#define BLITTER_ADDITIVE 0

#define STARS_PER_TARGET 64 // each target's slice of the star buffer
#define STAR_TOTAL 384

void SceneTransform_ApplyPosition(const s32 *position);

/// Swarms of stars over the targets. Each target gets a swarm, eight frames
/// after the one before: its stars burst from a point that rises out of it,
/// each turning about the point on one of four axes and drawn smaller the
/// deeper it is, and from frame 24 they are pulled back into the point,
/// each one gone once it has arrived. The palette steps through the
/// Mercury, Mars and Jupiter Djinn's. Mode 0: 16 stars a target, the
/// target flashes as its swarm starts, and the effect lasts eight frames
/// a target more than 38. Mode 1: 48 stars, a flash at the start and 55
/// frames later, 72 frames in all.
void BattleEffect_RunStarSwarm(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  DrawRectangle draw[2];
  s32 member;
  u8 *sparkCels = Ram_HeapSlots->unk64[0];
  s32 frames;
  struct BattleCamera *camera;
  s32 frame;
  s32 i;
  struct EffectPosition screen;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_ADDITIVE, 7));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (mode == 0)
    frames = effect->count * 8 + 38;
  else
    frames = 72;
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != frames; frame++) {
    camera = Ram_HeapSlots->cameraWork;

    // The first frame sets each target's point on it, rising, and every
    // star at its point with a random push across the floor.
    if (frame == 0) {
      for (i = 0; i != effect->count; i++) {
        struct MotionObject *object =
            BattleObject_GetFar(effect->actors[i])->object;

        work->particles[i].x = object->x;
        work->particles[i].y = 0;
        work->particles[i].z = object->z;
        if (mode == 1) {
          work->particles[i].velocityX = 0;
          work->particles[i].velocityY = px(1) / 2;
        } else {
          work->particles[i].velocityX = -object->x / 40;
          work->particles[i].velocityY = px(3);
        }
        work->particles[i].velocityZ = 0;
      }
      for (i = 0; i != STAR_TOTAL; i++) {
        struct EffectStep *star = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        s32 angle = Random16() & 0xffff;
        s32 speed = 255; // the mask starts in speed's own register

        speed &= Random16();

        SET_POSITION(star, 0, ((Random16() & 31) + 20) << 16, 0);
        speed += 128;
        star->velocityX = (Trig_Sin(angle) * speed) >> 5;
        star->velocityY = 0;
        star->velocityZ = (Trig_Cos(angle) * speed) >> 5;
        star->variant = 0;
      }
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry((s32)&ResourceId_BlastSheet),
                           128);
    }

    // Two palette steps a frame from frame 8.
    for (i = 0; i != 2; i++) {
      if (frame >= 8) {
        if (frame < 20)
          BattleEffect_StepPaletteToResource(
              (s32)&ResourceId_MercuryDjinnSheet);
        else if (frame < 32)
          BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
        else
          BattleEffect_StepPaletteToResource(
              (s32)&ResourceId_JupiterDjinnSheet);
      }
    }

    for (member = 0; member != effect->count; member++) {
      struct EffectStep *point = &work->particles[member];
      s32 start = member;
      s32 starCount;

      // Set in two steps: as one giv the loop pass would turn it into a walker.
      start *= 8;
      starCount = 16;
      if (mode != 0)
        starCount = 48;
      if (frame == start) {
        if (mode == 0)
          BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 16);
        else
          BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 16);
      }
      if (frame == start + 55 && mode != 0)
        BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 16);

      // The swarm turns about its point, which rises from frame 4.
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      if (frame >= start + 4 && start < 44)
        BattleEffectKernels_IntegrateVector3(point, 64, 0);
      SceneTransform_ApplyPosition(&point->x);

      for (i = 0; i != starCount; i++) {
        struct EffectStep *star =
            &((struct EffectStep *)
                  Ram_MapCellBuffer)[member * STARS_PER_TARGET + i];

        if (frame >= i / 8 + start && star->variant == 0) {
          s32 size;

          Transform_Push();
          switch ((member + i) & 3) {
          case 0:
            Transform_ApplyYaw(frame * (i + 8) * 32);
            break;
          case 1:
            Transform_ApplyPitch(-frame * (i + 8) * 32);
            break;
          case 2:
            Transform_ApplyRoll(-frame * (i + 8) * 32);
            break;
          case 3:
            Transform_ApplyPitch(-frame * (i + 8) * 32);
            Transform_ApplyRoll(-frame * (i + 8) * 32);
            break;
          }
          EffectPosition_ProjectPoint(&star->x, &screen);
          screen.x >>= 1;
          Transform_Pop();
          if (screen.depth < 250)
            screen.depth = 250;
          if (screen.depth > 0x27a)
            screen.depth = 0x27a;
          size = 9 - (screen.depth - 250) / 64;
          draw[0](canvas, sparkCels + gBattleEffect_MoteCells[size - 1],
                  screen.x - size / 2, screen.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(star, 60, 0);

          // The pull back into the point; a star within 2047 of it across
          // the floor is gone.
          if (frame >= i / 8 + 24) {
            s32 dx = -star->x >> 7;
            s32 dy = -star->y >> 7;
            s32 dz = -star->z >> 7;

            star->velocityX += dx;
            star->velocityY += dy;
            star->velocityZ += dz;
            star->velocityX = star->velocityX * 62 / 64;
            star->velocityY = star->velocityY * 62 / 64;
            star->velocityZ = star->velocityZ * 62 / 64;
            if ((dx >= -2047 && dx <= 2047) && (dz >= -2047 && dz <= 2047))
              star->variant = -1;
          }
        }
      }
    }

    BattleCamera_Shake(4, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void BattleEffect_RunBubbleColumn(struct BattleEffectArgument *effect);

/// Unused: nothing calls it. Opens the canvas layer, sets alpha blending and a
/// 24-frame palette fade counter (read by Palette_StepFadeTransfer), then runs
/// the effect body BattleEffect_RunBubbleColumn.
void BattleEffect_StartBubbleColumn(s32 arg) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];

  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  *(s32 *)(work + 0x77b4) = 24;
  *(s32 *)(work + 0x77b8) = 0;
  BattleEffect_RunBubbleColumn(arg);
}

// The blitter heap slots, by byte offset into Ram_HeapSlots.

// The Lost Age numbers its blitter modes from the other end of The Broken
// Seal's: additive 0, max 1, transparent 2, copy 3.
#define BLITTER_MAX 1

#define BUBBLE_COUNT 256
#define BUBBLE_FRAMES 60
#define BUBBLE_HIT_CUE 126

extern char ResourceId_BlueBeamSheet;
extern char ResourceId_Data0FD;

// 256 bubbles past the map cells. Each is an EffectStep with its cel in z
// and its life in variant (-1 once it has left the top of the canvas).
extern struct EffectStep gData_02015000[];

// The fifteen cels of Resource_Data0FD, a glowing oval from 16x32 down to
// 3x4: their widths, heights and byte offsets into the sheet.
extern u8 gData_0819966f[];
extern u8 gData_0819967e[];
extern u16 gData_0819968e[];

/// A column of bubbles. On the first frame 256 glowing ovals of random size
/// appear over the bottom left of the canvas; for 60 frames they float up
/// and drift sideways, drawn with the max blitter in the blue beam's palette,
/// and each one is gone once it has risen past the top. From frame 18 the
/// targets flash one by one, eight frames apart, each with the hit sound.
void BattleEffect_RunBubbleColumn(struct BattleEffectArgument *effect) {
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 frame;
  s32 i;
  struct EffectStep *bubble;

  // Each frame the transfer task copies the canvas and clears it.
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 50;
  BattleEffect_LoadBlitter(
      HEAP_SLOT_BLITTER,
      BLITTER(BLITTER_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The blue beam's palette colours the bubble cels.
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlueBeamSheet),
                       128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FD, work->sheet, 0, 0);

  for (frame = 0; frame != BUBBLE_FRAMES; frame++) {
    // The first frame scatters every bubble below the bottom left.
    if (frame == 0) {
      struct EffectStep *spawn = gData_02015000;

      for (i = 0; i != BUBBLE_COUNT; i++) {
        spawn->x = (Random16() & 127) << 16;
        spawn->y = ((Random16() & 127) + 120) << 16;
        spawn->velocityX = ((Random16() & 31) - 16) << 13;
        spawn->velocityY = (8 - (Random16() & 31)) << 12;
        spawn->z = (Random16() & 7) + 5;
        spawn->variant = 32;
        spawn++;
      }
    }

    // Draw each bubble centred on its place, let it rise, and drop it once
    // it is wholly above the canvas.
    bubble = gData_02015000;
    for (i = 0; i != BUBBLE_COUNT; i++) {
      if (bubble->variant >= 0) {
        s32 cel = bubble->z;
        u32 width;
        u32 height;

        draw[0](canvas, work->sheet + gData_0819968e[cel],
                HI(bubble->x) - ((width = gData_0819966f[cel]) >> 1),
                HI(bubble->y) - ((height = gData_0819967e[cel]) >> 1), width,
                height);
        BattleEffectKernels_IntegrateVector2(bubble, 63, -0x4000);
        if (bubble->y < -(gData_0819967e[cel] << 16))
          bubble->variant = -1;
      }
      bubble++;
    }

    // The targets flash in turn from frame 18; the first hit also starts
    // battle phase 126.
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          BattleEffect_TintBattler(effect->actors[i], 7, -1, i, 16);
          BattlePlayback_StartFar(BUBBLE_HIT_CUE);
          Sound_PlayFar(BUBBLE_HIT_CUE);
        } else {
          Sound_PlayFar(BUBBLE_HIT_CUE);
          BattleEffect_TintBattler(effect->actors[i], 7, -1, i, 16);
        }
      }
    }

    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(HEAP_SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Coatlicue, the serpent goddess's summon.

struct MeshTriangle;
struct MeshVertex;

/// The effect sprite EffectSprite_CreateWithTiles returns, as far as this
/// function reads it.
struct SpriteView {
  s32 unk00;
  s32 attributes;
  u32 tile : 10;
  u32 priority : 2;
  u32 rest : 20;
  s32 unk0c;
  u8 vramEntry;
};

#define EFFECT_STEPS ((struct EffectStep *)(Ram_MapCellBuffer + 0x5000))
extern u8 Data_02014400[];
extern u8 Data_02014800[];
extern const struct WordPair Data_08196ef0;
extern u8 Data_081996ac[];
extern u8 Data_081996bb[];
extern u16 Data_081996ca[];
extern u16 Data_081996e8[];
extern u8 Data_081996fa[];
extern u8 Data_08199703[];
extern u8 Data_0819970c[];
extern u8 Data_08199715[];
extern s8 Data_0819971e[];
extern s8 Data_08199727[];
extern s8 Data_08199730[];
extern s8 Data_08199739[];
extern u8 Data_08199742[];
extern u16 Data_08199746[];
extern u16 Data_0819974e[];
extern u16 Data_08199754[];
extern u8 Data_08199756[];
extern u16 Data_08199758[];
extern u16 Data_0819975c[];
extern u16 Data_08199760[];
extern u16 Data_08199764[];
extern const u8 gData_08199210[];
extern const u8 gData_08199364[];
extern char ResourceId_BellMaidenTiles;
extern char ResourceId_BlueFlameSheet;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0FB;
extern char ResourceId_Data0FC;

void BattleEffect_ConfigureEffectDisplay(void);
void RagingHeat_FillGradientCanvas(void);
void BattleEffect_ArmPaletteHBlankDma(void);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);

void Palette_LoadBasicSpriteColors(void);
void Graphics_LoadGreyRamp(void);
void Palette_BrightenBgEntries(s32 red, s32 green, s32 blue);
void Palette_BrightenSavedBattleBg(s32 r, s32 g, s32 b);

#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->vramEntry].offset)
#define COLOR_RED(c) ((c) & 31)
#define COLOR_GREEN(c) (((u16)(c) >> 5) & 31)
#define COLOR_BLUE(c) (((u16)(c) >> 10) & 31)
#define HALF_HI(v) (((s16 *)&(v))[1])

// Fills the scanline gradient behind the serpent: dark rows above and below,
// a ramp shaded by level in between.
#define FILL_GRADIENT(table, i, level, c)                                      \
  do {                                                                         \
    for (; i != 135; i++) {                                                    \
      s32 t_ = i - 15;                                                         \
      s32 b_ = (level) - t_ / 15 + 32;                                         \
      s32 g_ = (level) - t_ / 16 + 16;                                         \
      s32 r_ = (level) - t_ / 17;                                              \
                                                                               \
      if (b_ < 0)                                                              \
        b_ = 0;                                                                \
      if (b_ > 31)                                                             \
        b_ = 31;                                                               \
      if (g_ < 0)                                                              \
        g_ = 0;                                                                \
      if (g_ > 31)                                                             \
        g_ = 31;                                                               \
      if (r_ < 0)                                                              \
        r_ = 0;                                                                \
      if (r_ > 31)                                                             \
        r_ = 31;                                                               \
      c = b_ << 10 | g_ << 5 | r_;                                             \
      *table++ = c;                                                            \
    }                                                                          \
    for (; i != 160; i++) {                                                    \
      *table++ = 0;                                                            \
    }                                                                          \
  } while (0)

// Draws cell n of the stone sheet at offset base, centred on (x, y).
#define BLIT_CELL(base, n, x, y)                                               \
  do {                                                                         \
    u8 *src_ = work->sheet + (base) + Data_081996ca[n];                        \
    u8 w_ = Data_081996ac[n];                                                  \
    s32 x_ = (x) - (w_ >> 1);                                                  \
    u8 h_ = Data_081996bb[n];                                                  \
                                                                               \
    blit[0](canvas, src_, x_, (y) - (h_ >> 1), w_, h_);                        \
  } while (0)

// Draws one spark of the falling stone sheet and drops it when it leaves the
// top of the screen.
#define DRAW_SPARK(p)                                                          \
  do {                                                                         \
    s32 n_ = (p)->z;                                                           \
                                                                               \
    {                                                                          \
      u8 *src_ = work->sheet + 0x4800 + Data_081996ca[n_];                     \
      s32 x0_ = HALF_HI((p)->x);                                               \
      u8 w_ = Data_081996ac[n_];                                               \
      s32 x_ = x0_ - (w_ >> 1);                                                \
      s32 y_ = HALF_HI((p)->y);                                                \
      u8 h_ = Data_081996bb[n_];                                               \
                                                                               \
      blit[0](canvas, src_, x_, y_ - (h_ >> 1), w_, h_);                       \
    }                                                                          \
    BattleEffectKernels_IntegrateVector2(p, 63, -0x4000);                      \
    if ((p)->y < -(Data_081996bb[n_] << 16)) {                                 \
      (p)->variant = -1;                                                       \
    }                                                                          \
  } while (0)

/// Coatlicue: the ground opens over a green dusk, the serpent goddess rises in
/// a spiral of stone, her coils sweep the field and her light heals the party.
void BattleEffect_RunCoatlicue(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  u8 *cellBuffer = gMapCellBuffer;
  // Only blit[0] is used; the array takes an 8-byte slot in the frame.
  DrawRectangle blit[2];
  struct WordPair grid;
  s32 skip;
  u16 *table;
  s32 frame;
  s32 i;
  s32 k;
  s32 y;
  s32 level;
  s32 color;
  s32 s;
  s32 sh;
  s32 ty;
  s32 spin;
  s32 zero;
  u8 *coilMesh = Data_02014800;
  s16 list[14];
  s32 place[4];
  struct WordPair req;
  struct EffectPosition pos;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct SpriteView *obj;
  struct EffectStep *p;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  RagingHeat_FillGradientCanvas();
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  y = -80;
  for (frame = 0; frame != 27; frame++) {
    y += 4;
    REG_BG2X = y << 8;
    work->transferPending = 1;
    wait(1);
  }
  zero = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  table = (u16 *)(work->unk7300 + 0x340);
  level = -8;
  color = 0;
  for (i = 0; i != 15; i++) {
    *table++ = color;
  }
  FILL_GRADIENT(table, i, level, color);
  Task_Add(BattleEffect_ArmPaletteHBlankDma, 3326);
  skip = 0;
  Mesh_BuildCylinderBand(Data_02014400, coilMesh, 32);
  list[0] = 255;
  BattleActor_SpawnObjectsForListFar(list, 0);
  {
    u8 *e = Resource_GetTableEntry((s32)&ResourceId_BellMaidenTiles);

    Iwram_CopyWords((void *)OBJ_PLTT, e, 512);
    Decompress_TaggedLz(e + 512, cellBuffer);
    {
      u8 *cells;

      i = 0;
      cells = gMapCellBuffer;
      for (; i != 12; cells += 1024, i++) {
        obj = EffectSprite_CreateWithTiles(32, 32, 0x80002000, 0);
        obj->priority = 2;
        work->objects[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 1024);
      }
    }
  }
  Graphics_LoadGreyRamp();
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FB, cellBuffer, 0, 0);
  Picture_MirrorIntoQuadrants(cellBuffer, work->sheet, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FC, work->sheet + 0x4000, 0,
                             0);
  {
    u8 *src = work->sheet + 0x4800;

    Resource_LoadAndDecompress((s32)&ResourceId_Data0FD, src, 0, 0);
    for (i = 0; i != 3000; i++) {
      work->sheet[0x53b8 + i] = src[i] >> 1;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cellBuffer, 0, 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  gCameraSceneParameters[4] = 240;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = 0x1010;
  for (frame = 0; frame != 602; frame++) {
    spin = ((frame - 1) << 9) + 512;
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame <= 581 && skip == 0) {
      skip = 1;
    }
    if (skip > 0) {
      s32 *value = &work->transferValue;
      s32 v = skip * 0x101;

      v = (v + (v << 16)) << 2;
      *value = v;
      if (v > 0x3f3f3f3f) {
        *value = 0x3f3f3f3f;
      }
      work->transferMode = 1;
      if (++skip == 19) {
        break;
      }
    }
    if (frame == 0) {
      for (i = 0; i != 384; i++) {
        EFFECT_STEPS[i].variant = -1;
      }
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
      grid.a = 0x580000;
      grid.b = 0x240000;
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 82) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
      Sound_PlayFar(SE_PSYNERGY_START);
    }
    if (frame == 210) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
      Sound_PlayFar(SE_PSYNERGY_START);
    }
    if (frame == 274) {
      Sound_PlayFar(SE_139);
    }
    if (frame == 310) {
      Sound_PlayFar(SE_CURSE_FALL);
    }
    if (frame == 316) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 466) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == 498) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 503) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 508) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 520) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 540) {
      Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
    }
    if (frame <= 1999999) {
      u16 *row = (u16 *)(work->unk7300 + 0x340);
      s32 shade = frame / 32 - 8;

      if (shade > 8) {
        shade = 8;
      }
      color = 0;
      for (i = 0; i != 15; i++) {
        *row++ = color;
      }
      if (frame > 539) {
        shade = (frame - 540) / 2 + 8;
      } else if (frame > 311) {
        shade = 14 - (frame - 312) / 4;
        if (shade <= 7) {
          shade = 8;
        }
      } else if (frame > 299) {
        shade += (frame - 300) / 2;
        if (shade > 14) {
          shade = 14;
        }
      }
      i = 15;
      FILL_GRADIENT(row, i, shade, color);
    }
    if ((u32)(frame - 282) <= 17) {
      s32 g = (frame - 282) / 2 + 14;
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 b = frame - 253;
      u32 c;

      if (g > 31) {
        g = 31;
      }
      if (b > 31) {
        b = 31;
      }
      i = 0;
      c = b << 10 | g << 5;
      for (; i != 256; i++) {
        *dst++ = c;
      }
    }
    if (frame > 299) {
      u16 *e = Resource_GetTableEntry((s32)&ResourceId_BellMaidenTiles);
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 tr = 31;
      s32 tg = 31;

      k = 15;

      if (frame > 315) {
        tr = 4;
        tg = 12;
        k = 16;
      }
      if (frame > 337) {
        tr = 0;
        tg = 0;
        k = 0;
      }
      for (i = 0; i != 256; i++, e++, dst++) {
        s32 c = *dst;
        s32 s = *e;
        s32 cb = COLOR_BLUE(c);
        s32 cg = COLOR_GREEN(c);
        s32 cr = COLOR_RED(c);
        s32 sb = COLOR_BLUE(s) + k;
        s32 sg = COLOR_GREEN(s) + tg;
        s32 t = COLOR_RED(s);
        s32 sr;

        /* FAKEMATCH(barrier): t held apart from s through an empty asm. */
        __asm__("" : "=r"(t) : "0"(t));
        sr = t + tr;
        if (cb < sb - 1) {
          cb += 2;
        } else if (cb > sb + 1) {
          cb -= 2;
        }
        if (cg < sg - 1) {
          cg += 2;
        } else if (cg > sg + 1) {
          cg -= 2;
        }
        if (cr < sr - 1) {
          cr += 2;
        } else if (cr > sr + 1) {
          cr -= 2;
        }
        if (cb > 31) {
          cb = 31;
        }
        if (cg > 31) {
          cg = 31;
        }
        if (cr > 31) {
          cr = 31;
        }
        if (cb < 0) {
          cb = 0;
        }
        if (cg < 0) {
          cg = 0;
        }
        if (cr < 0) {
          cr = 0;
        }
        *dst = cb << 10 | cg << 5 | cr;
      }
    }
    if (frame > 519) {
      s32 d = frame - 520;
      u16 *e = Resource_GetTableEntry((s32)&ResourceId_BellMaidenTiles);
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 half;

      i = 0;
      half = d / 2;
      for (; i != 256; i++, e++, dst++) {
        s32 b;
        s32 g;
        s32 r;
        s32 s;
        s32 t;

        s = *e;
        /* FAKEMATCH(temporary): red through a temporary set twice. */
        t = 31;
        t &= s;
        r = t + d;
        g = COLOR_GREEN(s) + half;
        b = COLOR_BLUE(s) + d / 4;

        if (r > 31) {
          r = 31;
        }
        if (g > 31) {
          g = 31;
        }
        if (b > 31) {
          b = 31;
        }
        *dst = b << 10 | g << 5 | r;
      }
    }
    if ((u32)(frame - 292) <= 253) {
      req = Data_08196ef0;
      place[1] = 0xff0000;
      place[3] = zero;
      if (frame == 360) {
        grid.b = 0x240000;
      }
      if ((u32)(frame - 396) <= 111) {
        grid.b += 0x4000;
      }
      if (frame > 539) {
        grid.b += 0x100000;
      }
      for (i = 0; i != 12; i++) {
        place[0] = ((i % 3) << 21) + grid.a;
        place[2] = ((i / 3) << 21) + grid.b;
        Render_ApplyProjectedPlacementFar(work->objects[i], place, (s32 *)&req,
                                          0);
      }
    }
    {
      s32 d = frame - 274;
      if ((u32)d <= 23) {
        struct EffectStep *p = &(&EFFECT_STEPS[80])[d];
        {
          s32 x = ((Random16() & 15) + 52) << 16;

          p->y = px(100);
          p->x = x;
        }
        p->velocityX = ((Random16() & 31) - 16) << 12;
        p->velocityY = -((Random16() & 15) + 8) << 12;
        p->z = (Random16() & 7) + 5;
        p->variant = 32;
      }
    }
    if (frame == 310) {
      struct EffectStep *p = EFFECT_STEPS;

      for (i = 0; i != 80; i++, p++) {
        p->x = ((Random16() & 63) + 28) << 16;
        p->y = (Random16() % 80 + 56) << 16;
        p->velocityX = ((Random16() & 31) - 16) << 12;
        p->velocityY = (8 - (Random16() & 31)) << 12;
        p->z = (Random16() & 7) + 5;
        p->variant = 32;
      }
    }
    for (k = 0; k != 9; k++) {
      if (frame == Data_081996e8[k]) {
        for (i = 0; i != 7; i++) {
          struct EffectStep *p = &work->particles[(k % 9) * 7 + i];

          p->x = (Random16() % Data_0819970c[k] + Data_081996fa[k]) << 16;
          p->y = ((Random16() & 3) + i * Data_08199715[k] + Data_08199703[k])
                 << 16;
          p->velocityX = Data_0819971e[k] << 12;
          p->velocityY = Data_08199727[k] << 12;
          p->z = ((Random16() & 3) + Data_08199730[k]) << 16;
          p->velocityZ = Data_08199739[k] << 10;
          p->variant = zero;
        }
      }
    }
    if (frame <= 359) {
      p = EFFECT_STEPS;

      for (i = 0; i != 384; i++, p++) {
        if (p->variant >= 0) {
          DRAW_SPARK(p);
        }
      }
    } else {
      Transform_Reset();
      SceneTransform_ApplyPosition3(0, 0, 0x1000000);
      if (frame > 539) {
        SceneTransform_ApplyScale3(frame * 1500 - 0xc4c10, Q16_ONE, Q16_ONE);
      } else if (frame > 517) {
        SceneTransform_ApplyScale3(0, 0, Q16_ONE);
      } else if (frame > 485) {
        s32 s = 0x103000 - (frame << 11);

        SceneTransform_ApplyScale3(s, s, Q16_ONE);
      }
      if (frame > 539) {
        Transform_ApplyYaw(frame << 10);
      } else if (frame > 517) {
        Transform_ApplyYaw(TRIG_TURN);
        Transform_ApplyPitch(TRIG_TURN / 2);
      } else if (frame > 453) {
        s32 a = (frame - 454) << 10;

        Transform_ApplyYaw(a);
        Transform_ApplyPitch(Trig_Sin(a) >> 3);
      }
      SceneTransform_ApplyScale3(55536, 45536, 27768);
      if (frame == 360) {
        struct EffectStep *p;

        if (skip == 0) {
          work->transferMode = 2;
          work->transferValue = 75;
        }
        p = EFFECT_STEPS;
        for (i = 0; i != 384; i++, p++) {
          p->variant = Random16() & 3;
          SET_POSITION(p, (Random16() % 160 - 80) << 16,
                       (Random16() % 160 + 48) << 16,
                       (Random16() % 160 - 80) << 16);
          p->velocityX = zero;
          p->velocityY =
              (-((Random16() & 63) + 32) << 10) - (1 << (16 - p->variant));
          p->velocityZ = Random16();
        }
      }
      if (frame == 540) {
        struct EffectStep *p = EFFECT_STEPS;
        s32 m;

        i = 0;
        m = 63;
        for (; i != 384; i++, p++) {
          p->y = zero;
          p->velocityY = -((Random16() & m) + 32) << 15;
        }
      }
      {
        p = EFFECT_STEPS;

        for (i = 0; i != 80; i++, p++) {
          s32 n;
          struct EffectPosition *q;

          if (p->variant < 0) {
            continue;
          }
          q = &pos;
          EffectPosition_ProjectPoint(p, q);
          q->depth = HALF_HI(q->depth) - 228;
          n = q->depth * 6 / 50 + Data_08199742[p->variant];
          if (n < 0) {
            n = 0;
          }
          if (n > 14) {
            n = 14;
          }
          {
            s32 x = HALF_HI(q->x);

            q->y = HALF_HI(q->y);
            q->x = x + 60;
          }
          if (frame > 539) {
            q->y = q->y + (frame << 4) - 8608;
          } else if (frame > 517) {
            q->y = q->y + 16;
          } else if (frame <= 453) {
            q->y = q->y + 80;
          } else {
            q->y = q->y - frame + 534;
          }
          if (p->variant <= 1) {
            BLIT_CELL(0x4800, n, q->x, q->y);
          } else {
            BLIT_CELL(0x53b8, n, q->x, q->y);
          }
          if (frame <= 453 || frame >= i / 16 + 540) {
            p->y += p->velocityY;
          }
        }
      }
    }
    for (k = 0; k != 9; k++) {
      s32 top = 0;

      for (i = 0; i != 7; i++) {
        struct EffectStep *step = &work->particles[k * 7 + i];

        if (work->particles[k * 7 + 6].variant >= 0) {
          s32 n = 14 - HALF_HI(step->z);
          s32 x =
              (step->x + (Trig_Sin((frame - (i % 7) * 8) << 10) << 2)) >> 16;

          if (i == 0) {
            top = step->y;
          }
          if (n < 0) {
            n = 0;
          }
          {
            u8 *src = work->sheet + 0x53b8 + Data_081996ca[n];
            u8 w = Data_081996ac[n];
            s32 x0 = x - (w >> 1);
            s32 y0 = top >> 16;
            u8 h = Data_081996bb[n];

            blit[0](canvas, src, x0, y0 - (h >> 1), w, h);
          }
          BattleEffectKernels_IntegrateVector3(step, 64, 0);
          if (step->z < 0) {
            step->z = zero;
          }
          if (i == 6 && step->y < -(Data_081996bb[n] << 16)) {
            step->variant = -1;
          }
          top = top + step->z + 0x80000;
        }
      }
    }
    {
      void *verts;
      s32 t;
      struct RenderModel *model;

      verts = Heap_AllocEwram(512);
      model = Render_AllocateModelList(1);
      model->texture = &texture;
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->mesh = gData_08199364;
      model->vertices = verts;
      t = frame - 210;
      if ((u32)t <= 107) {
        s32 pitch;

        ty = (frame * 3 << 13) - 0x4ec000;
        pitch = 4000;
        if (ty > 0x180000) {
          ty = 0x180000;
        }
        s = frame * 611 - 111926;
        if (s > 55536) {
          s = 55536;
        }
        if (frame > 273) {
          ty = (frame << 15) - 0x710000;
          if (ty > 0x200000) {
            ty = 0x200000;
          }
          pitch = 21536 - (frame << 6);
          if (pitch <= 2999) {
            pitch = 3000;
          }
          s = s + frame * 1875 - 513750;
          if (s > 85536) {
            s = 85536;
          }
        }
        model->kind = 6;
        texture.pixels = work->sheet;
        sh = (frame << 3) - 1744;
        if (sh > 0) {
          sh = 0;
        }
        if (frame > 309) {
          sh = (310 - frame) << 3;
        }
        model->shade = sh;
        Transform_Reset();
        SceneTransform_ApplyPosition3(0, ty, 0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        SceneTransform_ApplyUniformScale(s);
        if (frame <= 273) {
          Transform_ApplyRoll(spin - 0x12400);
        }
        Transform_ApplyPitch(pitch);
        Transform_ApplyYaw(spin);
        Render_TransformVertices(gData_08199210, verts, 64);
        Render_DrawModelList(model);
      }
      if ((u32)(frame - 273) <= 7 && skip == 0) {
        s32 d = frame - 273;
        s32 *value;
        s32 v;

        work->transferMode = 1;
        value = &work->transferValue;
        v = d * 0x101;
        v = v + (v << 16);
        *value = 0x3f3f3f3f - (v << 2);
      }
      if (frame == 281 && skip == 0) {
        work->transferMode = 2;
        work->transferValue = 50;
      }
      if ((u32)(frame - 274) <= 27) {
        s = (frame << 11) - 0x85000;
        texture.pixels = cellBuffer;
        model->kind = 7;
        sh = 0;
        if (frame > 293) {
          sh = (294 - frame) << 3;
        }
        model->shade = sh;
        Transform_Reset();
        SceneTransform_ApplyPosition3(0, 0x1c0000, 0);
        SceneTransform_ApplyUniformScale(s);
        Transform_ApplyPitch(4000);
        Transform_ApplyYaw(frame << 9);
        Render_TransformVertices(gData_08199210, verts, 64);
        Render_DrawModelList(model);
      }
      if (frame > 80) {
        for (i = 0; i != 4; i++) {
          s32 st = Data_08199746[i];

          if (frame > st && frame < st + 10) {
            s32 d;

            texture.pixels = gMapCellBuffer;
            sh = -16;
            d = frame - st;
            model->kind = 6;
            model->shade = sh;
            Transform_Reset();
            s = d << 13;
            if (i > 1) {
              s32 ty = -0x2c0000;

              if (frame > 539) {
                ty += (frame - 539) << 20;
              }
              SceneTransform_ApplyPosition3(-0x40000, ty, 0);
              SceneTransform_ApplyScale3(s, s, s);
            } else {
              SceneTransform_ApplyScale3(s, d << 14, s);
            }
            Transform_ApplyPitch(TRIG_TURN / 4);
            Transform_ApplyYaw(frame << 9);
            Render_TransformVertices(gData_08199210, verts, 64);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame > 80) {
        for (i = 0; i != 3; i++) {
          if (frame == Data_0819974e[i]) {
            Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
          }
          if (frame > Data_0819974e[i] && frame < Data_0819974e[i] + 10) {
            s32 d = frame - Data_0819974e[i];
            s = 0x14000 - (d << 13);
            texture.pixels = cellBuffer;
            model->kind = 7;
            sh = d * 16 - 56;
            if (sh > 0) {
              sh = 0;
            }
            model->shade = sh;
            Transform_Reset();
            SceneTransform_ApplyPosition3(-0x40000, -0x2c0000, 0);
            SceneTransform_ApplyScale3(s, s << 1, s);
            Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
            Transform_ApplyPitch(-TRIG_TURN / 16);
            Transform_ApplyYaw(frame << 9);
            Render_TransformVertices(gData_08199210, verts, 64);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame > 80) {
        for (i = 0; i != 3; i++) {
          s32 st = Data_08199754[i];

          if (frame > st && frame < st + 100) {
            ty = -0x2c0000;
            s = (frame - st) * 2978;
            if (s > 65535) {
              s = 0x10000;
            }
            texture.pixels = work->sheet;
            sh = -16;
            model->kind = 6;
            model->shade = sh;
            Transform_Reset();
            if (frame > 539) {
              ty = ((frame - 539) << 20) + -0x2c0000;
            }
            SceneTransform_ApplyPosition3(-0x40000, ty, 0);
            SceneTransform_ApplyScale3(s, s / 2, s);
            Transform_ApplyPitch(TRIG_TURN / 4);
            Transform_ApplyYaw(frame << 11);
            Render_TransformVertices(gData_08199210, verts, 64);
            Render_DrawModelList(model);
          }
        }
      }
      texture.logWidth = 7;
      texture.logHeight = 4;
      texture.pixels = work->sheet + 0x4000;
      model->kind = 7;
      model->mesh = coilMesh;
      model->vertices = verts;
      model->scrollU = zero;
      model->scrollV = zero;
      for (i = 0; i != 2; i++) {
        if (frame < Data_08199756[i]) {
          continue;
        }
        if (frame <= 293) {
          s = Data_08199758[i] + (frame - Data_08199756[i]) * Data_08199760[i];
          if (s > Data_08199764[i]) {
            s = Data_08199764[i];
          }
          if (frame > 209) {
            s -= t * Data_08199760[i];
          }
          Transform_Reset();
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
          if (frame > 273) {
            s = s + (frame << 13) + -0x224000;
          }
          SceneTransform_ApplyUniformScale(s);
          if (i == 0) {
            if (frame > 209) {
              s32 v = t;

              if (v > 48) {
                v = 48;
              }
              SceneTransform_ApplyPosition3(0, v << 16, 0);
            }
            Transform_ApplyRoll(Data_0819975c[i] - (frame << 8));
            sh = -16;
          } else {
            s32 x = -48;

            if (frame > 127) {
              x = frame - 176;
            }
            if (x > -16) {
              x = -16;
            }
            SceneTransform_ApplyPosition3(x << 16, 0, 0);
            Transform_ApplyRoll(Data_0819975c[i] + (frame << 8));
            if (frame > 209) {
              Transform_ApplyRoll((frame << 8) + -0xd200);
            }
            sh = ((frame - Data_08199756[i]) << 2) - 64;
            if (sh > -16) {
              sh = -16;
            }
          }
          if (frame > 273) {
            sh = 1080 - (frame << 2);
            if (i == 0) {
              Transform_ApplyRoll((274 - frame) << 10);
            } else {
              Transform_ApplyRoll((frame << 9) + -0x22400);
            }
          }
        } else {
          sh = (frame << 2) - (i << 3) - 1240;
          if (sh > -16) {
            sh = -16;
          }
          s = i * 26384 - (frame << 12) + 0x138710;
          if (s <= 16383) {
            continue;
          }
          Transform_Reset();
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
          SceneTransform_ApplyPosition3(0, 0x200000, 0);
          SceneTransform_ApplyUniformScale(s);
        }
        if (sh > -64) {
          model->shade = sh;
          Transform_ApplyPitch((frame - i * 82) * 22 - 3000);
          Transform_ApplyYaw(-frame << 10);
          Render_TransformVertices(Data_02014400, verts, 64);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }
    while (*(volatile u32 *)&work->vblanksSinceTransfer <= 1) {
    }
    work->transferPending = 1;
    wait(1);
  }
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), 0x4000, 0x3f3f3f3f);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  for (i = 0; i != 12; i++) {
    Sprite_DestroyFar(work->objects[i]);
  }
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)(BG_PLTT + 80), 0x80000010,
          (volatile u32 *)0x040000d4);
  BG_PLTT[94] = BG_PLTT[244];
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);
  Task_Remove(BattleEffect_ArmPaletteHBlankDma);
  gCameraSceneParameters[4] = 120;
  wait(1);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                       128);
  Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
  Palette_BrightenSavedBattleBg(0, 0, 11);
  frame = 0;
  for (; frame != 60; frame++) {
    s32 s;
    s32 lift;
    struct RenderModel *model2;
    void *verts2;
    s32 d;

    if (frame == 16) {
      work->transferMode = 2;
      work->transferValue = 75;
    }
    if (frame > 52) {
      Palette_BrightenBgEntries(-4, -4, -4);
    }
    if (frame > 48) {
      s32 fade = frame - 48;

      Palette_BrightenSavedBattleBg(-fade, -fade, 11 - fade * 2);
    }
    if (frame == 0) {
      work->transferMode = 3;
      work->transferValue = 0x04040404;
      {
        struct EffectStep *p = EFFECT_STEPS;

        for (i = 0; i != 256; i++, p++) {
          p->x = (Random16() & 127) << 16;
          p->y = ((Random16() & 255) + 120) << 16;
          p->velocityX = ((Random16() & 31) - 16) << 13;
          p->velocityY = (8 - (Random16() & 31)) << 12;
          p->z = (Random16() & 7) + 5;
          p->variant = 32;
        }
      }
    }
    for (i = 0; i != 256; i++) {
      struct EffectStep *p = &EFFECT_STEPS[i];

      if (p->variant >= 0) {
        DRAW_SPARK(p);
      }
    }
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          BattleEffect_TintBattler(effect->actors[0], 7, -1, 0, 16);
          BattlePlayback_StartFar(126);
          Sound_PlayFar(SE_RECOVERY);
        } else {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(effect->actors[i], 7, -1, i, 16);
        }
      }
    }
    verts2 = Heap_AllocEwram(512);
    model2 = Render_AllocateModelList(1);
    model2->texture = &texture2;
    texture2.logWidth = 7;
    texture2.logHeight = 7;
    model2->mesh = gData_08199364;
    model2->vertices = verts2;
    model2->kind = 6;
    texture2.pixels = work->sheet;
    i = 0;
    lift = frame * 8 - 128;
    s = (frame << 13) + -0x20000;
    d = frame - 16;
    for (; i != 3; lift -= 64, s += -0x10000, i++, d -= 8) {
      if (d >= 0) {
        s32 sh2 = 0;

        if (d > 15) {
          sh2 = (16 - d) << 3;
        }
        model2->shade = sh2;
        if (sh2 > -64) {
          Transform_Reset();
          SceneTransform_ApplyPosition3(0, (112 - lift) << 16, 0);
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
          SceneTransform_ApplyUniformScale(s);
          Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
          Transform_ApplyPitch(-TRIG_TURN / 16);
          Transform_ApplyYaw(frame << 10);
          Render_TransformVertices(gData_08199210, verts2, 4);
          Render_DrawModelList(model2);
        }
      }
    }
    Heap_Release(model2);
    Heap_Release(verts2);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef VENGEANCE_FRAMES
#undef BLADES
#undef LIFE_SHEAR_FRAMES
#undef HEAP_SLOT_BLITTER
#undef HEAP_SLOT_BLITTER_ALTERNATE
#undef GEYSER_FRAMES
#undef GEYSER_DRAW_FRAMES
#undef GEYSER_PEAK_FRAME
#undef DROP_COUNT
#undef BLITTER_ADDITIVE
#undef STARS_PER_TARGET
#undef STAR_TOTAL
#undef BLITTER_MAX
#undef BUBBLE_COUNT
#undef BUBBLE_FRAMES
#undef BUBBLE_HIT_CUE
#undef EFFECT_STEPS
#undef OBJ_TILES
#undef COLOR_RED
#undef COLOR_GREEN
#undef COLOR_BLUE
#undef HALF_HI
#undef FILL_GRADIENT
#undef BLIT_CELL
#undef DRAW_SPARK
