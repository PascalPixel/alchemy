/// @file
/// The Ulysses summon and Spider Web, with the effect sprites, grey ramp and
/// circle maps they draw with.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

struct AnimationEntry *AnimationObject_AllocateEmpty(void);
/* VIEW(f15b3b0): routine Animation_ApplyChildArgumentFar as s32
 * Animation_ApplyChildArgumentFar(void *, s32); ability_effects has void
 * Animation_ApplyChildArgumentFar(void *object, s32 value); tried as the 4
 * declarations' void Animation_ApplyChildArgumentFar(void *, s32),
 * Func_0815b3b0 moves: 1 of 46 instructions in tla-ja. */
extern s32 Animation_ApplyChildArgumentFar__f15b3b0(void *, s32) __asm__(
    "Animation_ApplyChildArgumentFar");

/* VIEW(f15b3b0): tag struct EffectSprite as struct EffectSprite__f15b3b0;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:4172: structure has no member
 * named `kind'. */
struct EffectSprite__f15b3b0 {
  u8 unk00[0x0c];
  s32 unk0c;
  u8 unk10;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 unk12;
  u8 kind;
  u8 unk15;
  u8 unk16;
  u8 unk17;
  u8 unk18[2];
  u8 unk1a;
  u8 unk1b;
  u8 unk1c[0x0c];
  struct AnimationEntry *entry;
  u8 unk2c[0x0c];
};

// Takes the first free effect sprite of the 64 with a new animation entry.
struct EffectSprite__f15b3b0 *Func_0815b3b0(s32 kind, s32 value) {
  struct EffectSprite__f15b3b0 *sprite = Ram_HeapSlots->unk0c[1];
  struct EffectSprite__f15b3b0 *found = NULL;
  s32 i = 0;

  while (i <= 63) {
    if (sprite->kind == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->kind = kind;
  sprite->unk15 = value;
  sprite->unk0c = 0x10000;
  sprite->unk11_0 = 0;
  sprite->unk17 = 0;
  sprite->unk16 = 0;
  sprite->unk1b = 1;
  sprite->unk1a = 0;
  sprite->unk12 = 0;
  Animation_ApplyChildArgumentFar__f15b3b0(sprite, 0);
  return found;
}

/// Fills BG palette colours 1 to 63 with a grey ramp, each level i / 2.
void Graphics_LoadGreyRamp(void) {
  s32 i = 0;
  u16 *color = (u16 *)0x05000002;

next:
  {
    s32 level = i / 2;

    i++;
    *color = level << 10 | level << 5 | level;
  }
  color++;
  if (i != 63)
    goto next;
}

// Shaded circles of growing radius, drawn into a half-width byte map.

/// Fills a size by size/2 byte map with circles of growing radius, each
/// shaded by the cosine of its radius (0 to 63).
void Func_0815b434(u8 *map, s32 size, s32 scale) {
  s32 half = size / 2;
  s32 center = half - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWordsEntry(map, half * size);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;

    shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
    shade = shade * scale / 0x10000;

    if (shade > 63)
      shade = 63;
    while (x >= y) {
      col = center - x / 2;
      map[half * (last - y) + col] = shade;
      col = center - y / 2;
      map[half * (last - x) + col] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
      y++;
    }
  }
}

// Shaded circles of growing radius, mirrored into all four quarters of a
// byte map.

/// Func_0815b434 for a size by size map: each circle is drawn in all four
/// quarters, mirrored about the centre row and column.
void Func_0815b510(u8 *map, s32 size, s32 scale) {
  s32 center = size / 2 - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWordsEntry(map, size * size * 2);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;
    s32 row;

    shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
    shade = shade * scale / 0x10000;
    if (shade > 63)
      shade = 63;
    while (x >= y) {
      col = center - x / 2;
      row = last - y;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      col = center - y / 2;
      row = last - x;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
      y++;
    }
  }
}

// Mirrors the first 64 background colours, keeping the backdrop colour.
void Func_0815b634(void) {
  u16 saved[64];
  u16 flipped[64];
  s32 i;

  Iwram_CopyWords(saved, (void *)BG_PLTT, 128);
  for (i = 1; i != 64; i++)
    flipped[i] = saved[64 - i];
  flipped[0] = saved[0];
  Iwram_CopyWords((void *)BG_PLTT, flipped, 128);
  REG_BLDALPHA = 0x810;
}

// A scale of one in 16.16 on both axes.
extern const struct SpriteScale gData_08196e4c;

// A frame callback of summon 17 (BattleEffect_RunUlysses adds and removes it):
// places effect object 7 at the last particle's point (x across, y in depth)
// at a scale of one, moves the particle by its velocity (its depth speed
// grows for 20 frames, then shrinks, and its sideways speed grows from frame
// 12), and steps the object's animation through cels 7 to 14, seven frames
// each.
void Func_0815b68c(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct SpriteScale scale = gData_08196e4c;
  struct SpritePlacement place;

  place.groundHeight = 0;

  place.height = 0;

  place.x = work->particles[63].x;
  place.y = work->particles[63].y;
  Render_ApplyProjectedPlacementFar(work->objects[7], &place.x, &scale.x, 0);
  work->particles[63].x += work->particles[63].velocityX;
  work->particles[63].y += work->particles[63].velocityY;
  if (work->frame <= 19)
    work->particles[63].velocityY += 0x1000;
  else
    work->particles[63].velocityY -= 0x1000;
  if (work->frame > 11)
    work->particles[63].velocityX += 0x1000;
  Animation_ApplyChildArgumentFar__f15b3b0(work->objects[7],
                                           work->frame / 7 % 8 + 7);
  work->frame++;
}

struct FxPartyList;
struct Model;
struct StripQuad;

/* VIEW(f15b3b0): tag struct ModelTexture as struct ModelTexture__f15b3b0;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:4524: storage size of `texture'
 * isn't known. */
struct ModelTexture__f15b3b0 {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

/* VIEW(f15b3b0): tag struct RenderModel as struct RenderModel__f15b3b0;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:4819: dereferencing pointer to
 * incomplete type. */
struct RenderModel__f15b3b0 {
  s32 kind;
  s32 unk04;
  const u8 *mesh;
  s16 *vertices;
  struct ModelTexture__f15b3b0 *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
// Closes the canvas layer.
void BattleEffect_EndCanvasLayer(void);
// Copies the canvas to VRAM when a transfer is pending.
// Counts down the actors' flash timers.
void BattleEffect_CountDownBattlerTints(void);
// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
// Flashes an actor in a palette for some frames.
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
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
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void SceneTransform_ApplyRoll(s32 angle);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
// An actor's recoil motion, through a far call.
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattlePlayback_StartFar(s32 phase);
// Shakes the camera while work->shakeFrames lasts.
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
#define HI(v) ((v) >> 16)

extern char ResourceId_Data0B1;
extern char ResourceId_Data0B3;
extern char ResourceId_RuneCircleSheet;
extern char ResourceId_Data0C8;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_RuneSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_YellowOrbSheet;
extern char ResourceId_EmberStreakSheet;
extern u8 gMapCellBuffer[];
extern u8 Data_02010200[];
extern u16 gBattleEffect_FlarePictures[];
extern u16 gBattleEffect_PuffArcCells[];
extern u8 gData_08197467[];
extern u8 gBattleEffect_PuffArcCellBiasY[];
extern u8 gBattleEffect_PuffArcCellHeights[];
extern const u16 Data_08198686[];
extern const u8 Data_08198662[];
extern const u8 Data_08198674[];
extern const s8 Data_081986aa[];
extern const u8 Data_08198654[];
extern const u8 Data_0819865b[];
struct PlacementSource {
  s32 unk00;
  s32 unk04;
};

extern const struct PlacementSource Data_08196e54;
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];

void BattleEffect_ProcessPendingBlock2Transfer(void);
void Display_ApplyBg2Affine(void);
// Steps the background palette fade.
void BattleEffect_StepPaletteToResource(s32 palette);

void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);

// Ulysses: the hero's ship sails out of a whirlpool, its crew of six
// warriors leap at the foe and the ship's broadside lands as a storm of
// explosions.
void BattleEffect_RunUlysses(struct BattleEffectArgument *effect) {
  u8 *canvas;
  s32 *transition;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  s32 dmaArmed;
  s32 skip;
  DrawRectangle *blit;
  struct {
    s32 x;
    s32 z;
  } ship;
  s32 loaded;
  s32 draw;
  s32 i;
  u8 *cells;
  u8 *picture;
  s32 place[4];
  DrawRectangle blits[2];
  struct ModelTexture__f15b3b0 texture;
  struct ModelTexture__f15b3b0 texture2;
  struct PlacementSource req;
  u8 *battle;
  s32 mask;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  transition = Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  dmaArmed = 0;
  skip = 0;
  BattleEffect_SelectLivingTargets(work->effect);
  for (frame = 0, mask = 31; frame != 32; frame++) {
    volatile u16 *c = (volatile u16 *)(BG_PLTT + 96);

    for (i = 0; i != 128; i++, c++) {
      s32 v = *c;
      s32 r = mask & v;
      s32 g;
      s32 b;

      g = (u16)(((u16)v >> 5) & 31);
      b = (u16)(((u16)v >> 10) & 31);
      if (g == 0) {
        if (b > 0) {
          b--;
        }
        if (r > 0) {
          r--;
        }
      }
      if (g > 0) {
        g--;
      }
      *c = b << 10 | g << 5 | r;
    }
    wait(1);
  }
  BattleEffect_StartCanvasLayer(0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1141, 0x20000);
  wait(1);
  {
    u32 *map = (u32 *)0x0600f800;
    u32 v;
    s32 y;
    s32 x;

    Iwram_FillWords(map, 256, -1);
    map += 64;
    Iwram_FillWords(map, 128, 0x03ff03ff);
    v = (u32)Data_02010200;
    map += 32;
    for (y = 0; y != 16; y++) {
      for (x = 0; x != 8; x++) {
        *map++ = v;
        v += 0x20002;
      }
      for (x = 0; x != 8; x++) {
        *map++ = 0x03ff03ff;
      }
    }
  }
  BattleEffect_SpawnObjects(8, 0x28f, 3);
  REG_BG2PA = 0x80;
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f46;
  REG_BG1CNT = 0x1f83;
  wait(1);
  blit = blits;
  BattleEffect_FetchRectangleBlitters(0, (void **)blit);
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  cells = work->sheet + 0x1781;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B1, cells, 0, 0);
  for (i = 0; i != 0x2300; i++, cells++) {
    if (*cells) {
      *cells += 128;
    }
  }
  picture = work->sheet + 0x1781;
  blits[0](canvas, picture, 24, 8, 80, 112);
  work->transferPending = 1;
  wait(1);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B3, picture, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sprites, 0, 0);
  loaded = 0;
  ship.x = 0xc80000;
  ship.z = 0x540000;
  transition[4] = 1;
  Iwram_ClearWords(canvas, 0x3c00);
  REG_DISPCNT = 0x7141;
  wait(1);
  for (frame = 0; frame != 326; frame++) {
    struct EffectStep *step;
    draw = 1;
    if (gInput.repeat & 3) {
      skip = 1;
    }
    // The listing masks the frame with draw (still 1) here, not a literal 1.
    if ((frame & draw) && skip == 1) {
      draw = 0;
    }
    if (frame == 0) {
      struct EffectStep *p;

      if (!loaded) {
        Task_Add(BattleEffect_ProcessPendingBlock2Transfer, 0xc8f);
        Task_Add(Display_ApplyBg2Affine, 0xcff);
        Resource_LoadAndDecompress((s32)&ResourceId_RuneCircleSheet,
                                   work->sheet + 0x2781, 1, 0);
        loaded = 1;
      }
      gBgScroll[1].x = 0xff90;
      REG_BLDALPHA = 0x1010;
      REG_DISPCNT = 0x7741;
      REG_BG2CNT = 0x784;
      REG_BG1CNT = 0x1f80;
      work->transferMode = 2;
      work->transferValue = 75;
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
      for (i = 0; i != 9; i++) {
        p = &work->particles[i];
        p->variant = -2 * i;
        p->x = (Random16() & 15) + 60;
        p->y = (Random16() & 15) + 44;
      }
    }
    if (frame <= 75) {
      s32 k = (frame / 4) % 18;
      s32 x = 60 - ((Trig_Sin(frame * 860) << 3) >> 16);
      s32 y = frame - 8;
      u8 *source = work->sheet + 0x1781 + Data_08198686[k];
      u8 w = Data_08198662[k];
      s32 left = x - (w >> 1);
      u8 h = Data_08198674[k];

      blit[1](canvas, source, left, y - (h >> 1), w, h);
    }
    if ((u32)(frame - 76) <= 31) {
      struct EffectStep *p;

      for (i = 0, p = work->particles; i != 4; i++, p++) {
        if ((u32)p->variant <= 17) {
          s32 k = p->variant / 3;
          s32 side = i & 1;
          u8 *source = work->sheet + gBattleEffect_PuffArcCells[k];
          u8 w = gData_08197467[k];

          blit[side](canvas, source, p->x - (w >> 1),
                     p->y + gBattleEffect_PuffArcCellBiasY[k] - 32, w,
                     gBattleEffect_PuffArcCellHeights[k]);
        }
        p->variant++;
      }
    }
    if (frame == 108) {
      REG_BLDALPHA = 16;
    }
    if ((u32)(frame - 108) <= 8) {
      REG_BLDALPHA = ((frame - 108) * 2) << 8 | 16;
    }
    if ((u32)(frame - 92) <= 39) {
      s32 t = frame - 92;
      s32 z = Trig_Cos(t * 800) / 3 + 0x10000;
      s32 q16;
      s32 level = -1;

      if (z <= 0x107ff) {
        z = 0x10800;
      }
      q16 = 0x1000000 / z;
      *(volatile u16 *)&work->bg2pa;
      *(volatile u16 *)&work->bg2pa = q16;
      *(volatile u16 *)&work->bg2pd;
      *(volatile u16 *)&work->bg2pd = q16;
      work->bg2x = (64 - 0xb80000 / z) << 8;
      work->bg2y = (64 - 0x4c0000 / z) << 8;
      if (t <= 15) {
        level = t;
      } else if ((u32)(frame - 116) <= 8) {
        level = 64 - t * 2;
      }
      if (level != -1) {
        for (i = 0; i != 32; i++) {
          s32 g = (i * ((level * 9 * 4) / 16)) / 64;
          s32 r = (i * level * 3) / 64;

          ((u16 *)(BG_PLTT + 128))[i] = r << 10 | g;
        }
      }
      if (frame == 116) {
        REG_DISPCNT = 0x7341;
      }
    }
    if ((u32)(frame - 124) <= 7) {
      volatile u16 *c = (volatile u16 *)BG_PLTT;

      for (i = 0; i != 64; i++, c++) {
        s32 v = *c;
        s32 low = 31;
        s32 r;
        s32 g;
        s32 b;

        low &= v;
        r = low - 3;
        g = ((u16)(((u16)v >> 5) & 31)) - 3;
        b = ((u16)(((u16)v >> 10) & 31)) - 3;

        if (r < 0)
          r = 0;
        if (g < 0)
          g = 0;
        if (b < 0)
          b = 0;
        *c = b << 10 | g << 5 | r;
      }
    }
    if (frame == 132) {
      if (loaded == 1) {
        Task_Remove(BattleEffect_ProcessPendingBlock2Transfer);
        Task_Remove(Display_ApplyBg2Affine);
        Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc8f);
        Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet,
                                   work->sheet + 0x1e97, 1, 0);
        loaded = 0;
      }
      REG_DISPCNT = 0x7141;
      REG_BG2X = -0x800;
      REG_BG2Y = -0x1000;
      REG_BG2PA = 0xcc;
      REG_BG2PD = 0x100;
      REG_BG2CNT = 0x784;
      REG_BLDALPHA = 0x1010;
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry((s32)&ResourceId_RuneSheet),
                           128);
    }
    if (frame == 133) {
      const s8 *src;
      struct EffectStep *p;
      struct EffectStep *q;

      Iwram_FillWords(canvas, 0x3c00, 0);
      Iwram_ClearWords((void *)0x06004000, 0x3c00);
      REG_DISPCNT = 0x7541;
      src = Data_081986aa;
      q = (struct EffectStep *)(work->sheet + 0x4b98);
      for (i = 0; i != 6; i++, src += 5) {
        p = &work->particles[i];
        p->x = src[0] << 16;
        p->variant = -8 * i;
        /* FAKEMATCH(barrier): an empty asm parts the particle's first stores
         * from the rest; as no asm, 44 of 1932 instructions differ in tla-en.
         */
        __asm__("");
        p->y = src[1] << 16;
        p->z = src[2] << 6;
        p->velocityX = src[3] << 15;
        p->velocityY = src[4] << 15;
        p->velocityZ = 0;
      }
      for (i = 0; i != 96; i++) {
        q[i].variant = -1;
      }
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C8, gMapCellBuffer, 0,
                                 0);
    }
    if (frame == 76) {
      Sound_PlayFar(138);
    }
    if (frame == 266) {
      Sound_PlayFar(138);
    }

    if ((u32)(frame - 133) <= 132) {
      void *verts;
      struct RenderModel__f15b3b0 *model;
      struct EffectStep *sparks;
      u8 *portraits = gMapCellBuffer;
      struct EffectStep *q;
      s32 shade;
      s32 j;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      shade = 0;
      sparks = (struct EffectStep *)(work->sheet + 0x4b98);
      if (frame > 196) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
      }
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->kind = 7;
      model->shade = shade;
      model->texture = &texture;
      model->mesh = gData_08199340;
      model->vertices = verts;
      for (i = 0, step = work->particles; i != 6; step++, i++) {
        s32 delay = i * 8;
        struct EffectStep *anchor = step + 8;
        s32 t;

        if (++step->variant == 0) {
          Sound_PlayFar(138);
        }
        t = step->variant;
        if (t == 60) {
          Sound_PlayFar(212);
          t = step->variant;
        }
        if (t == 64) {
          Sound_PlayFar(134);
          t = step->variant;
        }
        if ((u32)t <= 35) {
          s32 k = (t / 6) % 6;
          u8 *source = work->sheet + gBattleEffect_PuffArcCells[k];
          u8 w = gData_08197467[k];

          blits[0](canvas, source, HI(step->x) - (w >> 1),
                   HI(step->y) + gBattleEffect_PuffArcCellBiasY[k] - 36, w,
                   gBattleEffect_PuffArcCellHeights[k]);
          t = step->variant;
        }
        if ((u32)(t - 12) <= 31) {
          s32 k = ((t - 12) / 4) % 18;
          u8 *source = work->sheet + 0x1781 + Data_08198686[k];
          s32 x = HI(step->x);
          u8 w = Data_08198662[k];
          s32 left = x - (w >> 1);
          s32 y = HI(step->y);
          u8 h = Data_08198674[k];

          blit[1](canvas, source, left, y - (h >> 1), w, h);
          t = step->variant;
        }
        if ((u32)(t - 44) <= 21) {
          s32 s = (Trig_Sin((t - 44) << 10) * 48) / 64;
          s32 vx;
          s32 x = step->x;
          s32 mx = x - 0x400000;
          s32 my = step->y - 0x400000;
          s32 u = t - 59;

          if (u < 0) {
            u = 0;
          }
          vx = step->velocityX;
          step->y += step->velocityY;
          x += vx;
          step->x = x;
          if (x <= 0x3fffff) {
            step->velocityX = vx + 0x8000;
          }
          if (step->y <= 0x3bffff) {
            step->velocityY += 0x8000;
          } else {
            step->velocityY -= 0x8000;
          }
          if (step->z >= 0) {
            step->z += (Trig_Sin(step->velocityZ) * 3 << 8) >> 16;
          } else {
            step->z -= (Trig_Sin(step->velocityZ) * 3 << 8) >> 16;
          }
          step->velocityZ += 1024;
          if (draw) {
            // Offset first, as the listing adds it.
            texture.pixels = (u8 *)((u << 12) + (s32)portraits);
            Render_ResetTransformState();
            Func_08015160(mx, my, 0);
            SceneTransform_ApplyPitch(0xc000);
            SceneTransform_ApplyYaw(step->z);
            Func_0801521c(s);
            Render_TransformVertices(gData_08199210, verts, 4);
            Render_DrawModelList(model);
          }
          t = step->variant;
        }
        if (t == 64) {
          anchor->x = HI(step->x);
          anchor->y = HI(step->y);
          j = 0;
          {
            s32 first = i * 16;

            // Offset first, as the listing adds it.
            q = (struct EffectStep *)(first * (s32)sizeof(*q) + (s32)sparks);
          }
          for (; j != 16; j++, q++) {
            s32 a = Random16() & 0xffff;
            s32 sp = (Random16() & 15) + 8;

            q->variant = (Random16() & 7) + 8;
            q->x = step->x;
            q->y = step->y;
            q->velocityX = (Trig_Sin(a) * sp) / 4;
            q->velocityY = (Trig_Cos(a) * sp) / 4;
          }
          for (j = 0; j != effect->count; j++) {
            s32 m = (Random16() << 2) + 0x10000;

            BattleEffect_StartActorMotion(effect->actors[j], 1, 0x20000,
                                          0x80000, m,
                                          delay + (Random16() & 31) + 52);
            BattleEffect_TintBattler(effect->actors[j], 7, -1, j, 4);
          }
        }
        if (draw && (u32)(step->variant - 64) <= 11) {
          s32 k = ((step->variant - 64) / 2) % 6;

          blit[1](canvas, work->sheet + 0x1e97 + k * 0x780, anchor->x - 20,
                  anchor->y - 24, 40, 48);
        }
      }
      for (i = 0; i != 96; i++) {
        q = &sparks[i];
        if (q->variant >= 0) {
          s32 n = q->variant / 4 + 2;

          if (draw) {
            blit[1](canvas, sprites + gBattleEffect_FlarePictures[n - 1],
                    HI(q->x) - n, HI(q->y) - n, n * 2, n * 2);
          }
          BattleEffectKernels_IntegrateVector2(q, 60, 0x2000);
          q->variant--;
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    if (draw && frame <= 131) {
      void *verts;
      struct RenderModel__f15b3b0 *model;
      s32 j;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      /* FAKEMATCH(dead): j takes frame * 3 + 1 here and 0 below before any
       * read; the scale step's first value is then built from this frame * 3,
       * where the listing builds it; as no store, 284 of 1932 instructions
       * differ in tla-en. */
      j = frame * 3 + 1;
      model->shade = 0;
      work->transferMode = 2;
      work->transferValue = 50;
      texture2.logWidth = 7;
      texture2.logHeight = 7;
      model->texture = &texture2;
      texture2.pixels = work->sheet + 0x2781;
      model->kind = 7;
      model->mesh = gData_08199364;
      model->vertices = verts;
      j = 0;
      for (; j != 2; j++) {
        s32 t = frame - 76 - j * 16;

        if (t >= 0) {
          // One scale for both arms: the else arm's constant then keeps a
          // register of its own.
          s32 sc;

          Render_ResetTransformState();
          Func_08015160(0x80000, 0, 0);
          SceneTransform_ApplyPitch(0x4000);
          if (j == 0) {
            sc = t * 3072 - 0x12000;
            if (t <= 39) {
              sc = t << 11;
              if (sc > 0xc000) {
                sc = 0xc000;
              }
            }
            Func_0801521c(sc);
            SceneTransform_ApplyYaw(-t * 512);
          } else {
            if (t > 23) {
              sc = 0x1c000 - (t << 11);
              Func_0801521c(sc);
            } else {
              SceneTransform_ApplyPitch(0x3ff0 - t * 682);
              SceneTransform_ApplyRoll(t * 169 + 0xf028);
            }
            SceneTransform_ApplyYaw(t << 9);
          }
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    if ((u32)(frame - 108) <= 179) {
      req = Data_08196e54;
      if (frame > 131) {
        s32 u = frame - 132;

        ship.x = (Trig_Sin(u << 11) << 1) + 0xc80000;
        ship.z = (Trig_Sin(u << 10) << 1) + 0x540000;
      }
      if (draw) {
        place[1] = place[3] = 0;
        for (i = 0; i != 7; i++) {
          place[0] = (Data_08198654[i] << 16) + ship.x - 0x200000;
          place[2] = (Data_0819865b[i] << 16) + ship.z - 0x200000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)&req, 0);
        }
      }
    }
    if (frame > 265) {
      if (frame == 266) {
        struct EffectStep *p;

        REG_BG2X = -0x7000;
        REG_BG2PA = 0x100;
        REG_BG2PD = 0x100;
        for (i = 0, p = work->particles; i != 32; i++, p++) {
          p->x = Random16() % 48 + 40;
          p->y = Random16() % 96 + 16;
          p->variant = -i / 2;
        }
      }
      for (i = 0, step = work->particles; i != 32; i++, step++) {
        if ((u32)step->variant <= 23 && draw) {
          s32 k = (step->variant / 4) % 6;
          u8 *source = work->sheet + gBattleEffect_PuffArcCells[k];
          u8 w = gData_08197467[k];

          blits[0](canvas, source, step->x - (w >> 1),
                   step->y + gBattleEffect_PuffArcCellBiasY[k] - 48, w,
                   gBattleEffect_PuffArcCellHeights[k]);
        }
        step->variant++;
      }
    }
    if (frame == 288) {
      Task_Add(Func_0815b68c, 1152);
      work->particles[63].x = 0xbc0000;
      work->particles[63].y = 0x600000;
      work->particles[63].velocityX = 0;
      work->particles[63].velocityY = 0;
      work->frame = 0;
      dmaArmed = 1;
    }
    if (draw) {
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  BattlePlayback_StartFar(134);
  REG_BG1CNT = 0x1f83;
  wait(1);
  // Once-only blocks: the listing loads each fill's destination before its
  // size; the first destination is read before its block, whose loop note then
  // keeps the size and the call's address behind that load.
  {
    void *destination = Ram_HeapSlots->battleFxWork[1];

    do {
      Iwram_FillWords(destination, 0x4000, 0);
    } while (0);
  }
  do {
    Iwram_FillWords((void *)0x06004000, 0x3c00, 0);
  } while (0);
  Iwram_FillWords((void *)0x06008000, 0x7800, 0);
  wait(1);
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(SLOT_BLITTER);
  transition[4] = 0;
  battle = Ram_HeapSlots->battleWork;
  Sound_PlayFar(390);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Iwram_ClearWords((void *)(BG_PLTT + 96), 256);
  wait(1);
  BattleBackground_LoadFar(2, *(u16 *)(battle + 0x670), -1);
  REG_BG2CNT = 0x787;
  Iwram_ClearWords((void *)0x06004000, 0x3c00);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  REG_BLDCNT = 0;
  wait(1);
  for (frame = 0; frame != 40; frame++) {
    u16 *src = (u16 *)(battle + 0x56c);
    volatile u16 *dst = (volatile u16 *)(BG_PLTT + 96);

    for (i = 0; i != 128; i++, src++, dst++) {
      s32 v = *src;
      s32 r = 31;
      s32 g;
      s32 b;
      s32 nr;
      s32 ng;
      s32 nb;

      r &= v;
      g = (u16)(((u16)v >> 5) & 31);
      b = (u16)(((u16)v >> 10) & 31);
      nr = r + frame - 30;
      ng = g + frame - 40;
      nb = b + frame - 30;
      if (nr < 0) {
        nr = 0;
      } else if (nr > r) {
        nr = r;
      }
      if (ng < 0) {
        ng = 0;
      } else if (ng > g) {
        ng = g;
      }
      if (nb < 0) {
        nb = 0;
      } else if (nb > b) {
        nb = b;
      }
      *dst = nb << 10 | ng << 5 | nr;
    }
    wait(1);
  }
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  if (dmaArmed == 1) {
    Task_Remove(Func_0815b68c);
  }
  for (i = 0; i != 8; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
}

void BattleEffect_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Poison Beat (kind 382).
void BattleEffect_RunPoisonBeat(void *object) {
  BattleEffect_RunSwirlSwipe(object, 0);
}

/// Abilities: Spinning Beat (kind 383).
void BattleEffect_RunSpinningBeat(void *object) {
  BattleEffect_RunSwirlSwipe(object, 1);
}

// Byte offset of the blitter slot in gHeapSlots (unk64[1]).
#define HEAP_BLITTER 104

extern char ResourceId_MagentaSwirlSheet;
extern char ResourceId_MagentaTailSheet;
extern char ResourceId_LimePalette;
// The swirl's six cels: ☀️'s gSpiderWeb_Cells, _CellX (six
// for each side), _CellWidths, _CellBiasY and _CellHeights.
extern const u16 gSpiderWeb_Cells[];
extern const u8 gSpiderWeb_CellX[];
extern const u8 gSpiderWeb_CellWidths[];
extern const s8 gData_081986ec[];
extern const u8 gSpiderWeb_CellHeights[];

void BattleEffect_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  struct MotionObject *object;
  struct EffectPosition position;
  // Only draw[0] is used; the pair takes two registers (see the header).
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 cel;

  // Setup: the canvas at a scale of one, a transparent blitter, mirrored
  // for side 0.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleFxWork[0];
  canvas = slots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  if (effect->side == 1) {
    BattleEffect_LoadBlitter(
        HEAP_BLITTER,
        BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    draw[0] = (DrawRectangle)slots->unk64[1];
  } else {
    BattleEffect_LoadBlitter(HEAP_BLITTER,
                             BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X |
                                                           COMPOSITE_CLIP_Y |
                                                           COMPOSITE_FLIP_X));
    draw[0] = (DrawRectangle)slots->unk64[1];
  }
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet,
                             Ram_MapCellBuffer, 1, 0);
  if (mode == 0) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;
    copy((void *)0x05000000, palette, 128);
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // BG2 is placed on the first target.
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  EffectPosition_ProjectScaledBattler(effect->actors[0], &position);
  if (effect->side == 0)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != 1)
    frames = 48;

  for (frame = 0; frame != frames; frame++) {
    // The swirl: six cels, four frames each, the last two from the tail.
    cel = frame / 4;
    if (cel <= 5) {
      if (cel <= 3)
        draw[0](canvas, work->sheet + gSpiderWeb_Cells[cel],
                gSpiderWeb_CellX[cel + effect->side * 6],
                gData_081986ec[cel] + 32, gSpiderWeb_CellWidths[cel],
                gSpiderWeb_CellHeights[cel]);
      else
        draw[0](canvas, Ram_MapCellBuffer + gSpiderWeb_Cells[cel],
                gSpiderWeb_CellX[cel + effect->side * 6],
                gData_081986ec[cel] + 32, gSpiderWeb_CellWidths[cel],
                gSpiderWeb_CellHeights[cel]);
    }
    // The hit.
    if (frame == 8) {
      if (mode == 0) {
        BattlePlayback_StartFar(0x85);
        BattleMotion_ApplyKnockbackFar(effect->actors[0], 1);
      } else {
        Sound_PlayFar(0x86);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
      }
      work->shakeFrames = 8;
    }
    // Mode 1 throws the target up at frame 13; it lands at frame 65.
    if (mode == 1) {
      if (frame == 13) {
        object->velocityY = 0xc0000;
        object->verticalMotionStrength = 0x7851;
        object->verticalMotionPhase = 0x4000;
      }
      if (frame == 65) {
        work->shakeFrames = 4;
        BattlePlayback_StartFar(0x86);
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

extern char ResourceId_SpiderWebSheet;

// The middle between the first and the last target.
void Func_0815e22c(struct EffectPosition *middle);

void SummonEffect_RunCanvasMode1(void *action);

// Builds the transparent blitter for one quarter.
#define LOAD_QUARTER(flags)                                                    \
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT, flags))

// Draws one quarter of the web with the blitter just built.
#define DRAW_QUARTER(x, y)                                                     \
  ((DrawRectangle)Ram_HeapSlots->unk64[1])(canvas, work->sheet, x, y, size,    \
                                           size)

void BattleEffect_RunSpiderWeb(struct BattleEffectArgument *effect) {
  struct EffectPosition middle;
  void *canvas;
  struct BattleEffectWork *work;
  volatile u16 *blend;
  s32 size;
  s32 frame;
  s32 i;

  // Setup: the canvas on BG2, blending, the layer on the targets' middle.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(2);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1000;
  Func_0815e22c(&middle);
  REG_BG2X = (64 - middle.x) << 8;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1,
                             1);
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(143);
  blend = &REG_BLDALPHA;

  frame = 0;
  size = 32;
  do {
    // It fades in over nine frames and out over the last ten.
    if (frame <= 8)
      *blend = (frame << 1) | 0x1000;
    if (frame > 53)
      *blend = (0x7c - (frame << 1)) | 0x1000;

    // The four quarters: the one picture as it is, mirrored, flipped, and
    // both. Each blitter is built, drawn with and released before the next
    // is built; each once-only block holds a draw, its release and the next
    // build (as one block per quarter, 12 differ); the last draw needs none.
    /* FAKEMATCH(signature): the three releases between draws called as
     * returning s64, so each call sets r1 and the scheduler sets a draw's x
     * and y before its sheet; as plain void calls, 6 of 228 instructions
     * differ in tla-en. */
    LOAD_QUARTER(CLIP);
    do {
      DRAW_QUARTER(33, 41);
      ((s64 (*)(s32))HeapSlot_Release)(HEAP_BLITTER);
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP_X);
    } while (0);
    do {
      DRAW_QUARTER(64, 41);
      ((s64 (*)(s32))HeapSlot_Release)(HEAP_BLITTER);
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP_Y);
    } while (0);
    do {
      DRAW_QUARTER(33, 72);
      ((s64 (*)(s32))HeapSlot_Release)(HEAP_BLITTER);
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP);
    } while (0);
    DRAW_QUARTER(64, 72);
    HeapSlot_Release(HEAP_BLITTER);

    // The battle resumes, hit cue 143; every target flashes at frame 10.
    if (frame == 32)
      BattlePlayback_StartFar(143);
    for (i = 0; i != effect->count; i++) {
      if (frame == 10)
        BattleEffect_TintBattler(effect->actors[i], 7, -1, i, 8);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 63);

  // Teardown: clear both canvases and hand over to the summon canvas.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords((void *)0x06004000, 0x3c00);
  Iwram_ClearWords(canvas, 0x3c00);
  effect->prelude = 0;
  Task_Remove(Palette_StepFadeTransfer);
  SummonEffect_RunCanvasMode1(effect);
}

#undef LOAD_QUARTER
#undef DRAW_QUARTER
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef HEAP_BLITTER
#undef CLIP
#undef LOAD_QUARTER
#undef DRAW_QUARTER
