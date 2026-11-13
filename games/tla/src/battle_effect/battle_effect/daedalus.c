/// @file
/// A symmetrical burst over the target, and the Daedalus summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

struct Model;

#define HEAP_SLOT_BLITTER 104 // Ram_HeapSlots->unk64[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3.
#define BLITTER_MAX 1
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP_XY 3

#define BURST_FRAMES 48
#define BURST_DRAW_FRAMES 28

// The three 64x64 cels of the burst, built in the sheet's first 0x3000
// bytes; the picture loads behind them.
#define CEL_SIZE 0x1000
#define PICTURE_OFFSET 0x4000

// Copies a half-picture of width by height pixels into the bottom middle of
// a 64x64 cel, once as it is and once mirrored, so the cel is symmetrical.
#define MIRROR_HALF(cel, half, width, height)                                  \
  for (y = 0; y != (height); y++) {                                            \
    for (x = 0; x != (width); x++) {                                           \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - (height)) * 64 + 32 - (width) + \
                  x] = work->sheet[(half) + y * (width) + x];                  \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - (height)) * 64 + 31 + (width) - \
                  x] = work->sheet[(half) + y * (width) + x];                  \
    }                                                                          \
  }

// A burst cel's texture: log2 of its width and height in the first word
// (64x64 here), then its pixels.
struct BurstTexture {
  u32 size;
  void *pixels;
};

// One entry of the list Render_DrawModelList draws; kind 6 is a textured quad.

extern char ResourceId_EarthWallSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_FireballSheet;
extern char ResourceId_MarsDjinnSheet;

// The burst quad: its face list and its four corners, 64x64 pixels standing
// on its base.
extern const u8 gData_08199340[];
extern const u8 gData_081991f0[];

// Per mode: the quad's scale (16.16, doubled when used) and its unk14.
extern const s32 gData_081994d0[];
extern const s32 gData_081994e0[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void Resource_LoadAndDecompress(s32 resourceId, void *target, s32 unpack,
                                s32 wait);

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void BattlePlayback_StartFar(s32 phase);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 blitter); // BattleEffect_BuildBlitter
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 member, s32 timer,
                              s32 frames);
struct PoolEntry *Render_AllocateModelList(s32 count);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyUniformScale(s32 scale);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList__ability_effects(struct Model *model);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_EndCanvasLayer(void);

// A symmetrical burst over the target. Three half-pictures of the earth
// wall sheet are mirrored into three 64x64 cels; the burst grows through
// them in its first twelve frames, flickers between the last two until
// frame 28 and is gone, while the screen shakes for all 48 frames. The
// mode picks the palette (Mars Djinn, Mars Djinn, fireball, orange), the
// scale, how hard the screen shakes and the target's flash colour.
void BattleEffect_RunMirroredWall(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct BurstTexture texture;
  struct EffectPosition target;
  struct RenderPrimitive *quad;
  void *vertices;
  s32 frame;
  s32 cel;
  s32 y;
  s32 x;

  // Build the three cels from their halves.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet,
                             work->sheet + PICTURE_OFFSET, 1, 1);
  Iwram_ClearWords(work->sheet, 0x4000);
  MIRROR_HALF(0, PICTURE_OFFSET + 0xa8e, 17, 49)
  MIRROR_HALF(1, PICTURE_OFFSET + 0xdcf, 19, 55)
  MIRROR_HALF(2, PICTURE_OFFSET + 0x11e4, 20, 64)
  if (mode == 3)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  else if (mode == 2)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_FireballSheet),
                    128);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                    128);

  // Centre the canvas's x 64 on the target.
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  REG_BG2PA = 0x100;
  REG_BG2X = (64 - target.x) << 8;
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_MAX, CLIP_XY));
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (mode == 2) {
    work->shakeFrames = 0;
    Sound_PlayFar(SE_POWER_UP);
  } else if (mode == 1 || mode == 3) {
    work->shakeFrames = 8;
    Sound_PlayFar(SE_POWER_UP);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != BURST_FRAMES; frame++) {
    if (frame == 0) {
      if (mode == 2)
        BattleEffect_TintBattler(effect->actors[0], 7, -1, 0, 32);
      else
        BattleEffect_TintBattler(effect->actors[0], 10, -1, 0, 32);
    }
    if (frame == 24)
      BattlePlayback_StartFar(0);
    if (frame == 8 && mode == 0)
      Sound_PlayFar(SE_RECOVERY);

    // Cels 0, 1 and 2 four frames each, then 1 and 2 by turns.
    if (frame < 32) {
      cel = frame / 4;
      if (cel > 2)
        cel = (cel & 1) + 1;
      if (frame < BURST_DRAW_FRAMES) {
        vertices = Heap_AllocEwram(32);
        quad = Render_AllocateModelList(1);
        texture.size = (texture.size & ~0xff) | 6;
        texture.size = (texture.size & ~0xff00) | 6 << 8;
        texture.pixels = work->sheet + cel * CEL_SIZE;
        quad->kind = 6;
        quad->faces = gData_08199340;
        quad->texture = &texture;
        quad->unk14 = gData_081994e0[mode];
        quad->vertices = vertices;
        Transform_Reset();
        SceneTransform_ApplyPosition3(0, (target.y - 58) << 16, 0);
        SceneTransform_ApplyUniformScale(gData_081994d0[mode] << 1);
        Render_TransformVertices(gData_081991f0, vertices, 4);
        Render_DrawModelList(quad);
        Heap_Release(quad);
        Heap_Release(vertices);
      }
    }

    if (mode == 0)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Tiles: pack each horizontal pair of bytes into one byte.

/// Copies 288 rows of 40 bytes, reducing each horizontal pair to one byte.
/// The second byte, from the odd column, survives.
/// stride: source bytes per row; destination uses half as many.
void BattleEffect_PackTileRows(u8 *src, u8 *dst, s32 stride) {
  s32 y = 0;
  s32 h = 288;
  s32 offset = 0;

  do {
    s32 x;

    for (x = 0; x != 40; x++) {
      dst[offset / 2 + x / 2] = src[offset + x];
    }
    offset += stride;
    y++;
  } while (y != h);
}

typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile s32 vs32;

/* VIEW(f17e698): tag struct Particle as struct Particle__f17e698;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:47770: structure has no member
 * named `timer'. */
typedef struct Particle__f17e698 {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 ay;
  s32 timer;
} Particle;

typedef struct Vec3 {
  s32 x;
  s32 y;
  s32 z;
  s32 w;
} Vec3;

typedef struct Vec2 {
  s32 x;
  s32 y;
} Vec2;

typedef struct Sprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 scale;
  u8 vram;
  u8 unk11;
  u16 rotation;
  u8 unk14[2];
  u8 unk16;
  u8 unk17;
} Sprite;

/* VIEW(ability_effects): tag struct SpriteAttributes as struct
 * SpriteAttributes__ability_effects; battle_effect/cybele has another
 * definition; not tried: struct SpriteAttributes__ability_effects was not
 * found. */
/// A sprite's first ten bytes, its attribute 2 fields after eight others.
struct SpriteAttributes {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

typedef struct VramSlot {
  u16 unk0;
  u16 offset;
} VramSlot;

typedef struct Bitmap {
  u32 width : 8;
  u32 height : 8;
  u32 unk2 : 16;
  void *data;
} Bitmap;

/* VIEW(f17e698): tag struct SummonWork as struct SummonWork__f17e698;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:47663: structure has no member
 * named `transferMode'. */
struct SummonWork__f17e698 {
  Particle particles[48];
  u8 unk0540[0x1c0];
  u8 frames[0x7080];
  s32 transferMode;
  s32 transferValue;
  u8 unk_7788[8];
  s32 unk7790;
  s32 unk7794;
  s32 unk7798;
  s32 unk779c;
  u8 unk77a0[8];
  s32 shake;
  u8 unk77ac[0x30];
  Sprite *sprites[57];
  u8 unk78c0[0x28];
  s32 transferPending;
};

struct SummonArgument {
  s32 kind;
  s32 side;
  s32 actor;
  s32 unk0c;
  s32 unk10;
  s32 count;
  s32 variant;
  s32 unk1c;
  s32 unk20;
  s16 actors[8];
};

typedef void (*CopyFunc)(void *, const void *, s32);
typedef void (*ClearFunc)(void *, s32);
typedef void (*BlitFunc)(void *, const void *, s32, s32, s32, s32);

/* VIEW(f17e698): tag struct SystemWork as struct SystemWork__f17e698;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:47662: structure has no member
 * named `map'. */
struct SystemWork__f17e698 {
  u8 unk00[0x24];
  s32 *bank;
  u8 unk28[0x34];
  struct SummonWork__f17e698 *summon;
  u8 *canvas;
  u8 *map;
  void *blit;
  u8 unk6c[0x44];
  s32 *control;
};

#define SYSTEM ((struct SystemWork__f17e698 *)Ram_HeapSlots)
/// Iwram_CopyWords reached from IwramIrqMain rather than at its fixed address,
/// as the trade's snapshot copies reach it: its address is then a pool load,
/// made after the arguments and after the call before it.
#define IWRAM_COPY_WORDS_LINKED                                                \
  ((CopyFunc)(IwramIrqMain + Iwram_CopyWordsOffset))
#define Iwram_Clear Iwram_ClearWordsEntry

#define DECODE_BUFFER Ram_MapCellBuffer
#define PARTICLES ((Particle *)(Ram_MapCellBuffer + 0x4000))

extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_ForestCanopyTiles;
extern char ResourceId_BronzeAutomatonTiles;
extern char ResourceId_MissileTiles;
extern char ResourceId_Data0BA;
extern char ResourceId_Data0BB;
extern char ResourceId_Data0C2;
extern char ResourceId_Data134;
extern char ResourceId_FlashBurstSheet;

extern char ResourceId_SmokeSheet;

extern u16 gBattleEffect_MoteCells[];
extern Vec2 Data_08196ed0;
extern Vec2 Data_08196ed8;
extern Vec2 Data_08196ee0;
extern s8 Data_081994f0[];
extern u8 Data_08199508[];
extern u8 Data_08199514[][2];
extern s32 Data_08199528[][2];
extern u8 Data_08199578[];
extern u16 Data_08199582[];
extern u16 Data_0819958c[];
extern s8 Data_08199590[];
extern u8 Data_08199592[][2];
extern u8 Data_0819959c[][6][2];
extern u16 Data_081995cc[];
extern const u8 gData_08199210[];
extern const u8 gData_08199364[];

void BattleEffect_ConfigureEffectDisplay(void);
void RagingHeat_FillGradientCanvas(void);

void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void BattleBackground_LoadFar(s32, s32, s32);
/* VIEW(f17e698): routine EffectSprite_Acquire as Sprite
 * *EffectSprite_Acquire(s32, s32, u32, s32); the definition has struct
 * EffectSprite__f15b3b0 *EffectSprite_Acquire(s32 kind, s32 value); tried as
 * the definition's struct EffectSprite__f15b3b0 *EffectSprite_Acquire(s32 kind,
 * s32 value), which the object already declares, BattleEffect_RunDaedalus
 * moves: 3 of 3898 instructions in tla-ja. */
extern Sprite *
EffectSprite_Acquire__f17e698(s32, s32, u32,
                              s32) __asm__("EffectSprite_Acquire");

void BattleEffect_FetchRectangleBlitters(s32, void **);
void BattleEffect_StepPaletteToResource(s32);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);

void Transform_ApplyRoll(s32 angle);
void Transform_ApplyPitch(s32);

void BattleEffect_SetupBlendedDisplay(void);
void Palette_LoadBasicSpriteColors(void);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);

void BattleEffect_AdvanceScrollOnInterval(void);

#define ALCHEMY_TYPES_H

void BattleEffect_RunDaedalus(struct SummonArgument *arg) {
  s16 targets[14];
  Vec3 pos;
  Vec3 pos2;
  Vec3 pos3;
  Vec2 scale;
  void *blit[2];
  Vec2 scale2;
  Bitmap bitmap;
  Vec2 scale3;
  u8 *dest;
  s32 frame;
  u8 *map;
  s32 *ctl;
  s32 *bank;
  s32 camera;
  s32 prevCamera;
  s32 waveX;
  s32 waveScroll;
  s32 shipY;
  s32 wavePhase;
  s32 savedY;
  s32 bgX;
  struct SummonWork__f17e698 *work;
  s32 k;
  s32 i;
  s32 j;
  // The map rows are 16 words apart; held in a local for its whole life.
  s32 step = 16;
  // Reused: the second phase's ring radius, and the 0 the third phase stores.
  s32 radius;

  dest = SYSTEM->canvas;
  work = SYSTEM->summon;
  map = SYSTEM->map;
  ctl = SYSTEM->control;
  bank = SYSTEM->bank;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  RagingHeat_FillGradientCanvas();
  BattleEffect_LoadBlitter(104, 3);
  blit[0] = SYSTEM->blit;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  camera = 0;
  waveScroll = 0;
  shipY = -0x1d0000;
  wavePhase = 0;
  savedY = gBgScroll[1].x;
  bgX = -80;

  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u16 *col;
      s32 n;
      s32 w;
      s32 off;
      u8 *mp;
      s32 k;
      s32 r, g, b;
      s32 v;
      const u8 *src;
      Sprite *obj;

      VramCache_FreeEntry(bank[21]);
      gCameraSceneParameters[4] = 240;
      targets[0] = 255;
      BattleActor_SpawnObjectsForListFar(targets, 0);
      BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
      for (col = (u16 *)(BG_PLTT + 6 * 16), i = 0; i != 128; i++, col++) {
        s32 mid;
        s32 s;

        s = *col & 0x1f;
        mid = (*col >> 5) & 0x1f;
        b = ((*col >> 10) & 0x1f) - 4;
        g = mid - 4;
        r = s - 4;
        if (g < 0)
          g = 0;
        if (r < 0)
          r = 0;
        if (b < 0)
          b = 0;
        if (g > 31)
          g = 31;
        if (r > 31)
          r = 31;
        if (b > 31)
          b = 31;
        *col = (b << 10) | (r << 5) | g;
      }
      work->unk7790 = 0;
      work->unk7794 = 5;
      work->unk7798 = -1;
      work->unk779c = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, 0x480);
      ctl[4] = 1;
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->frames, 0, 0);
      off = 0x700;
      n = 3;
      for (j = 0; j != 10; j++) {
        k = j * step;
        i = 0;
        // v, the loop's value local, first holds the row's byte offset.
        v = k * 2;
        mp = (u8 *)(v + (s32)map);
        do {
          v = ((u8 *)work)[gBattleEffect_MoteCells[n] + i + off];
          /* FAKEMATCH(dead): w set to 32 inside the row loop, after the load.
           */
          w = 32;
          if (v != 0) {
            v = v - j * 4 - 40;
            if (v <= 0)
              v = 1;
          }
          *mp++ = v;
        } while (++i != w);
      }
      src = (const u8 *)Resource_GetTableEntry(
          (s32)&ResourceId_ForestCanopyTiles);
      Iwram_CopyWords((void *)(OBJ_PLTT + 15 * 16), src, 32);
      src += 32;
      Decompress_TaggedLz(src, work->frames);
      for (j = 0; j != 2; j++) {
        for (i = 0; i != 8; i++) {
          obj = EffectSprite_CreateWithTiles(32, 32, 0x80000000, 0xf000);
          work->sprites[j * 9 + i] = obj;
          obj->priority = 1;
          Iwram_CopyWords(
              (void *)(gResourceTableEntries[obj->vram].offset + 0x06010000),
              &work->frames[(j << 12) + i * 0x200], 0x200);
        }
        obj = EffectSprite_Acquire__f17e698(32, 32, 0x80000000, 0xf000);
        work->sprites[j * 9 + 8] = obj;
        Iwram_CopyWords(obj, work->sprites[j * 9], 24);
        for (i = 0; i != 8; i++)
          work->particles[j * 9 + i].timer = work->sprites[j * 9 + i]->tile;
      }
      src = (const u8 *)Resource_GetTableEntry(
          (s32)&ResourceId_BronzeAutomatonTiles);
      Iwram_CopyWords((void *)OBJ_PLTT, src, 0x1c0);
      src += 0x1c0;
      Decompress_TaggedLz(src, DECODE_BUFFER);
      for (i = 0; i != 21; i++) {
        obj = EffectSprite_CreateWithTiles(32, 32, 0x80002000, 0);
        obj->priority = 3;
        work->sprites[i + 18] = obj;
        Iwram_CopyWords(
            (void *)(gResourceTableEntries[obj->vram].offset + 0x06010000),
            DECODE_BUFFER + i * 0x400, 0x400);
      }
      obj = EffectSprite_CreateWithTiles(32, 64, 0xc000a000, 0);
      // Once-only: the store alone in its block.
      do {
        work->sprites[i + 18] = obj;
      } while (0);
      obj->priority = 3;
      /* FAKEMATCH(barrier): an asm operand cuts i, known 21, from the copy's
       * source. */
      __asm__("" : "=r"(i) : "0"(i));
      Iwram_CopyWords(
          (void *)(gResourceTableEntries[obj->vram].offset + 0x06010000),
          DECODE_BUFFER + i * 0x400, 0x800);
      src = (const u8 *)Resource_GetTableEntry((s32)&ResourceId_MissileTiles);
      Iwram_CopyWords((void *)(OBJ_PLTT + 14 * 16), src, 32);
      src += 32;
      Decompress_TaggedLz(src, DECODE_BUFFER);
      obj = EffectSprite_CreateWithTiles(16, 8, 0x4000, 0xe000);
      obj->priority = 1;
      work->sprites[41] = obj;
      Iwram_CopyWords(
          (void *)(gResourceTableEntries[obj->vram].offset + 0x06010000),
          DECODE_BUFFER, 0x100);
      /* FAKEMATCH(barrier): an empty asm. */
      __asm__("");
      for (i = 0; i != 15; i++) {
        work->sprites[42 + i] =
            EffectSprite_Acquire__f17e698(16, 8, 0x4000, 0xf000);
        Iwram_CopyWords(work->sprites[42 + i], work->sprites[41], 24);
        work->sprites[42 + i]->tile = work->sprites[41]->tile;
        if (i > 6)
          work->sprites[42 + i]->priority = 2;
      }
    }
    if (frame > 27) {
      Vec3 *posp;
      s32 row;
      Sprite **slot;

      scale = Data_08196ed0;
      pos.w = 0;
      pos.y = 0xff0000;
      waveScroll += 0x8000;
      waveX = waveScroll + camera;
      if (waveX > 0x1fffff) {
        wavePhase = (wavePhase + 7) % 8;
        waveX -= 0x200000;
        waveScroll -= 0x200000;
      }
      for (j = 0; j != 2; j++) {
        for (i = 0; i != 9; i++) {
          Sprite *obj;
          s32 v;

          posp = &pos;
          {
            s32 h;

            k = (i << 21) + waveX;
            h = (j << 5) + 103;
            posp->x = k;
            posp->z = h << 16;
          }
          if (i == 8) {
            obj = work->sprites[j * 9 + 8];
            v = work->particles[j * 9 + (wavePhase + 71) % 8].timer;
          } else {
            row = j * 9;
            k = row;
            k += i;
            obj = work->sprites[k];
            v = work->particles[row + (wavePhase + 64 + i) % 8].timer;
          }
          ((struct SpriteAttributes *)obj)->tile = v & 0x3ff;
          if (pos.x > 0xffffff)
            pos.x -= 0x1200000;
          Render_ApplyProjectedPlacementFar(work->sprites[j * 9 + i], &pos,
                                            &scale, 0);
        }
      }
    }
    bgX += 4;
    *(s32 *)0x04000028 = bgX << 8;
    work->transferPending = 1;
    wait(1);
  }

  {
    void **blits;
    u16 *pal;
    s32 swayX;
    s32 swayY;
    s32 bobX;
    s32 bobY;
    s32 *ringX;
    void *pixels;

    HeapSlot_Release(104);
    Iwram_Clear(dest, 0x3c00);
    Iwram_Clear((void *)BG_CHAR_BLOCK(1), 0x4000);
    *(vu16 *)0x0400000c = 0x786;
    *(vu16 *)0x04000020 = 0x80;
    *(s32 *)0x04000028 = 0;
    blits = blit;
    BattleEffect_FetchRectangleBlitters(0, blits);
    work->unk7790 = 0;
    work->unk7794 = 5;
    work->unk7798 = -1;
    work->unk779c = 0;
    ctl[4] = 1;
    Resource_LoadAndDecompress((s32)&ResourceId_Data0BA, work->frames, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->frames + 0x6c0, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0BB, work->frames + 0x36c0,
                               0, 0);
    pixels = DECODE_BUFFER;
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, pixels, 0, 0);
    Iwram_CopyWords(
        (void *)BG_PLTT,
        (const void *)Resource_GetTableEntry((s32)&ResourceId_SmokeSheet), 128);
    work->transferMode = 2;
    work->transferValue = 50;
    *(vu16 *)0x04000052 = 0x1010;
    *(vu16 *)0x04000050 = 0x3f44;
    for (i = 0; i != 224; i++) {
      pal = (u16 *)OBJ_PLTT + i;
      if (((*pal >> 5) & 0x1f) == 31)
        break;
    }
    for (i = 0; i != 512; i++)
      PARTICLES[i].timer = -1;

    frame = 0;
    if (!(gInput.held & KEY_B) && !(gInput.repeat & KEY_A)) {
    phase2:
      if (frame == 0) {
        *(vu16 *)0x04000020 = 0x100;
        *pal = 0x190b;
        shipY = -0x1d0000;
        camera = 0;
        work->sprites[39]->rotation = TRIG_TURN * 3 / 4;
        work->sprites[38]->rotation = 0;
        work->sprites[31]->rotation = 0;
        work->particles[20].x = 0;
        work->particles[20].y = 0;
        work->particles[20].vx = -0x60000;
        work->particles[20].vy = 0;
        work->particles[20].timer = 0;
        work->particles[23].x = 0;
        work->particles[23].y = 0;
        work->particles[23].vx = -0xc0000;
        work->particles[23].vy = 0;
        work->particles[23].timer = 0;
        work->particles[21].x = 0;
        work->particles[21].y = 0;
        work->particles[21].vx = -0x10000;
        work->particles[21].vy = 0;
        work->particles[21].timer = 0;
        work->particles[22].x = 0xa00000;
        work->particles[22].y = 0;
        work->particles[22].vx = 0;
        work->particles[22].vy = 0;
        work->particles[22].timer = 0;
        for (i = 0; i != 8; i++) {
          Particle *p;

          p = &work->particles[32 + i];
          p->x = ((i / 3) << 18) + 0xb00000;
          p->y = ((i % 3) * 3 << 17) + 0x4a0000;
          p->vx = 0;
          p->vy = 0;
          p->timer = 0;
        }
        for (i = 0; i != 8; i++) {
          Particle *p;

          p = &work->particles[40 + i];
          p->x = ((i / 3) << 18) + 0x700000;
          p->y = ((i % 3) * 3 << 17) + 0x520000;
          p->vx = 0;
          p->vy = 0;
          p->timer = 0;
        }
      }
      prevCamera = camera;
      if (frame >= 542 && frame < 558) {
        s32 r, g, b;

        r = *pal & 0x1f;
        g = (*pal >> 5) & 0x1f;
        b = (*pal >> 10) & 0x1f;
        if (r < 31)
          r++;
        if (g < 20)
          g++;
        if (b < 6)
          b++;
        *pal = (b << 10) | (g << 5) | r;
      }
      if (frame > 557) {
        s32 r, g, b;

        r = *pal & 0x1f;
        g = (*pal >> 5) & 0x1f;
        b = (*pal >> 10) & 0x1f;
        if (r > 20)
          r--;
        if (g > 0)
          g--;
        if (b > 0)
          b--;
        *pal = (b << 10) | (g << 5) | r;
      }
      if (frame == 280) {
        Iwram_CopyWords(
            (void *)BG_PLTT,
            (const void *)Resource_GetTableEntry((s32)&ResourceId_SmokeSheet),
            128);
        work->particles[20].timer = 1;
      }
      if (frame == 288)
        work->particles[23].timer = 1;
      if (frame == 279) {
        *(vu16 *)0x04000020 = 0x80;
        *(vu16 *)0x0400000c = 0x784;
        *(vu16 *)0x04000020 = 0x80;
      }
      if (frame == 506)
        work->particles[21].timer = 1;
      if (frame == 489) {
        *(vu16 *)0x04000020 = 0x80;
        *(vu16 *)0x0400000c = 0x784;
        work->unk7798 = 0;
      }
      if (frame == 528) {
        *(vu16 *)0x04000020 = 0x80;
        *(vu16 *)0x0400000c = 0x785;
      }
      if (frame == 0) {
        work->transferMode = 1;
        work->transferValue = 0;
        work->transferMode = 2;
        work->transferValue = 50;
        for (i = 0; i != 10; i++) {
          Particle *p;

          p = &PARTICLES[502 + i];
          p->x = Data_08199514[i][0] << 16;
          p->y = Data_08199514[i][1] << 16;
          p->vx = Data_08199528[i][0];
          p->vy = Data_08199528[i][1];
          p->timer = Random16() & 127;
        }
      }
      for (i = 0; i != 10; i++) {
        Particle *p;

        p = &PARTICLES[502 + i];
        if (frame >= Data_08199578[i]) {
          ((BlitFunc)blit[0])(
              dest,
              &work->frames[(Data_08199508[(p->timer / 4) % 12] << 8) + 0x36c0],
              p->x >> 16, p->y >> 16, 16, 16);
          p->x += p->vx;
          p->y += p->vy;
          p->timer++;
        }
      }
      if (frame <= 473)
        waveScroll += 0x8000;
      waveX = waveScroll + camera;
      if (waveX >= 0x200000) {
        wavePhase = (wavePhase + 7) % 8;
        waveX -= 0x200000;
        waveScroll -= 0x200000;
      }
      if (shipY < 0)
        shipY += 0x2000;
      scale2 = Data_08196ed8;
      bobX = 0;
      bobY = 0;
      if (frame > 537) {
        swayX = Trig_Cos(0x21a00) << 2;
        swayY = Trig_Sin(0x21a00) << 2;
        if (frame >= 542 && frame < 558) {
          s32 v;

          v = (frame << 11) - 0x10f000;
          bobX = Trig_Sin(v) * 3;
          bobY = Trig_Sin(v) * 2;
        }
        if (frame >= 560 && frame < 576) {
          s32 v;

          v = (frame << 11) - 0x118000;
          bobX = Trig_Sin(v) * 2;
          bobY = Trig_Sin(v) * 3;
        }
      } else {
        swayX = Trig_Cos(frame << 8) << 2;
        swayY = Trig_Sin(frame << 8) << 2;
      }
      if (swayY > 0)
        swayY = -swayY;
      swayY -= shipY * 2;
      pos2.w = 0;
      pos2.y = 0xff0000;
      for (i = 0; i != 5; i++) {
        if (frame == Data_08199582[i]) {
          work->shake = 23;
          Sound_PlayFar(SE_CLATTER);
        }
      }
      if (frame == 510)
        Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
      if (frame == 568) {
        work->shake = 10;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 578)
        Sound_PlayFar(SE_163);
      for (i = 0; i != 16; i++) {
        Particle *p;

        p = &work->particles[32 + i];
        if (p->timer != 0) {
          Particle *q;

          p->timer++;
          if (p->x > -0x100000) {
            p->x += p->vx;
            p->y += p->vy;
            if (p->vy > -0x10000 && p->vy < 0x10000)
              p->vy += p->ay;
          }
          if (p->vx > -0x80000)
            p->vx -= 0x8000;
          q = &PARTICLES[i * 32 + (p->timer & 31)];
          q->timer = 0;
          q->x = p->x / 2;
          q->y = p->y - 0x140000;
          q->vx = 0;
          q->vy = 0;
        }
        if (work->particles[20].timer == (i & 7) * 6 + 10) {
          Sound_PlayFar(SE_ITEM_BREAK);
          p->timer = 1;
          p->x += swayX;
          p->y += swayY - (Data_081994f0[work->shake] << 16);
          p->vy = (Random16() & 0xffff) - 0x8000;
          p->ay = (Random16() & 0x3fff) - 0x2000;
        }
      }
      if (frame <= 489) {
        for (i = 8; i != 16; i++) {
          Particle *p;

          p = &work->particles[32 + i];
          pos2.x = p->x + camera;
          pos2.z = p->y;
          if (p->timer != 0)
            Render_ApplyProjectedPlacementFar(work->sprites[40 + i], &pos2,
                                              &scale2, 0);
        }
      }
      if (frame == 578) {
        work->unk7794 = 1;
        work->unk7790 = 0;
      }
      if (frame >= 578 && frame < 594)
        work->unk7798 = ~((frame - 578) / 8);
      {
        for (i = 0; i != 2; i++) {
          pos2.x = camera - swayX + bobX + 0x780000;
          pos2.z = (((i << 5) - Data_081994f0[work->shake]) << 16) + swayY +
                   bobY + 0x640000;
          Render_ApplyProjectedPlacementFar(work->sprites[32 + i], &pos2,
                                            &scale2, 0);
        }
      }
      if (frame <= 316) {
        Particle *p;

        p = &work->particles[23];
        if (p->timer != 0) {
          p->timer++;
          p->x += p->vx;
          p->y += p->vy;
          p->vy += 0x2000;
          p->vx = p->vx * 48 / 64;
          p->vy = p->vy * 63 / 64;
          work->sprites[38]->rotation += 0x200;
        }
        pos2.x = p->x - swayX + camera + 0x780000;
        pos2.z = ((99 - Data_081994f0[work->shake]) << 16) + swayY + p->y;
        Render_ApplyProjectedPlacementFar(work->sprites[38], &pos2, &scale2, 0);
      }
      {
        Particle *p;
        s32 v;

        v = camera + bobX;
        pos2.x = v + 0x8e0000;
        pos2.z = ((72 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[18], &pos2, &scale2, 0);
        pos2.x = v + 0xae0000;
        pos2.z = ((72 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[19], &pos2, &scale2, 0);
        v += 0x800000;
        pos2.x = v;
        pos2.z = ((104 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[20], &pos2, &scale2, 0);
        p = &work->particles[22];
        if (frame > 602)
          BattleEffect_StepPaletteToResource((s32)&ResourceId_OrangePaletteB);
        if (frame == 570) {
          p->x = 0x980000;
          p->y = ((92 - Data_081994f0[work->shake]) << 16) + swayY;
          p->timer = 1;
          p->vx = -0x40000;
          p->vy = -0x5000;
          for (i = 0; i != 16; i++) {
            Particle *q;

            q = &PARTICLES[312 + i];
            q->x = ((Random16() & 15) + 56) << 16;
            q->y = ((Random16() & 31) + 64) << 16;
            q->vx = -((Random16() & 31) + 8) << 12;
            q->vy = -((Random16() & 31) + 8) << 12;
            q->timer = 0;
          }
          for (i = 0; i != 32; i++)
            PARTICLES[400 + i].timer = -1;
          for (i = 0; i != 32; i++)
            PARTICLES[432 + i].timer = -1;
        }
        if (p->timer != 0) {
          if (p->timer > 9) {
            Particle *q;

            q = &PARTICLES[400 + (p->timer & 31)];
            q->timer = 0;
            q->vx = -(Trig_Sin(work->sprites[39]->rotation) * 2);
            q->vy = Trig_Cos(work->sprites[39]->rotation) * 2;
            q->x = (p->x + camera) / 2 -
                   ((Trig_Sin(work->sprites[39]->rotation) * 27) >> 1);
            q->y = p->y + Trig_Cos(work->sprites[39]->rotation) * 27 - 0xc0000;
          }
          if (((p->timer >= 50 && p->timer < 60) ||
               (p->timer >= 113 && p->timer < 200)) &&
              (p->timer & 3) == 0) {
            Particle *q;

            q = &PARTICLES[432 + (p->timer & 31)];
            q->timer = 0;
            q->vx = ((Random16() & 31) + 16) << 12;
            q->vy = (-Random16() | ~15) << 14;
            q->x = 0x580000;
            q->y = 0x800000;
          }
          prevCamera = camera;
          {
            Particle *f;

            f = &work->particles[22];
            if (f->timer > 229)
              camera +=
                  (0x980000 - (f->x + ((f->timer - 230) << 17)) - camera) / 4;
            else if (f->timer > 0)
              camera += (0x980000 - f->x - camera) / 4;
          }
          if (p->timer > 1) {
            p->x += p->vx;
            p->y += p->vy;
          }
          if (p->timer > 7) {
            p->vx = p->vx * 63 / 64;
            p->vy = p->vy * 63 / 64 + 0x8c8;
          }
          if (p->timer < 80)
            p->vy += 0x200;
          if (p->timer > 29)
            p->vx -= 0x4000;
          if (p->timer >= 40 && p->timer < 68)
            p->vy -= 0x1000;
          if (p->timer >= 50 && p->timer < 60) {
            work->sprites[39]->rotation = (p->timer << 7) - 0x5900;
            p->vy -= 0x1000;
          }
          if (p->timer >= 108 && p->timer < 140) {
            if (p->timer >= 120 && p->timer < 123)
              work->sprites[39]->rotation -= 0x180;
            else
              work->sprites[39]->rotation += 0x100;
            p->vy -= 0x1000;
          }
          if (p->timer >= 140 && p->timer < 160)
            p->vy -= 0x800;
          if (p->timer > 159)
            p->vy -= 0x2000;
          for (i = 0; i != 32; i++) {
            Particle *q;

            q = &PARTICLES[400 + i];
            if ((u32)q->timer <= 5) {
              ((BlitFunc)blits[1])(dest, &work->frames[q->timer * 288],
                                   (q->x >> 16) - 2, (q->y >> 16) - 12, 12, 24);
              BattleEffectKernels_IntegrateVector2(q, 64, 0);
              q->timer++;
            }
          }
          for (i = 0; i != 32; i++) {
            Particle *q;

            q = &PARTICLES[432 + i];
            if ((u32)q->timer <= 23) {
              ((BlitFunc)blits[1])(
                  dest, &work->frames[(q->timer / 4 << 11) + 0x6c0],
                  (q->x >> 16) - 16, (q->y >> 16) - 32, 32, 64);
              q->vx += 0x1000;
              BattleEffectKernels_IntegrateVector2(q, 64, 0);
              q->timer++;
            }
          }
          p->timer++;
        }
        scale2.x = 0x103ff;
        scale2.y = 0x103ff;
        if (p->timer == 0) {
          pos2.x = camera + bobX + 0x980000;
          pos2.z = ((92 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        } else {
          pos2.x = p->x + camera;
          pos2.z = p->y;
        }
        Render_ApplyProjectedPlacementFar(work->sprites[39], &pos2, &scale2, 0);
      }
      work->sprites[39]->unk16 = 0;
      work->sprites[39]->unk17 = 32;
      if (frame >= 568) {
        s32 *beam;
        void *buffer;
        s32 v;
        s32 z;

        buffer = Heap_AllocEwram(512);
        beam = Render_AllocateModelList(1);
        bitmap.width = 7;
        bitmap.height = 7;
        bitmap.data = pixels;
        beam[4] = (s32)&bitmap;
        beam[1] = 0;
        beam[2] = (s32)gData_08199364;
        beam[0] = 7;
        beam[3] = (s32)buffer;
        v = frame - 568;
        z = 96 - (v << 4);
        if (z > 0)
          z = 0;
        if (z >= -63) {
          beam[5] = z;
          Transform_Reset();
          SceneTransform_ApplyPosition3(camera + 0x20000, 0x80000, 0);
          Transform_ApplyRoll(0x4300);
          Transform_ApplyPitch(-TRIG_TURN / 32);
          SceneTransform_ApplyUniformScale((v << 14) + 0x4180);
          Render_TransformVertices(gData_08199210, buffer, 4);
          Render_DrawModelList(beam);
        }
        Heap_Release(beam);
        Heap_Release(buffer);
      }
      if (frame == 568) {
        work->particles[30].x = 0;
        work->particles[30].vx = 0x80000;
      }
      for (j = 0; j != 1; j++) {
        ringX = &work->particles[30].x;
        if (frame >= Data_0819958c[j] && frame < Data_0819958c[j] + 26) {
          s32 n;

          radius = *ringX >> 16;
          n = (frame - Data_0819958c[j]) / 2;
          *ringX += work->particles[30].vx;
          work->particles[30].vx = work->particles[30].vx * 48 / 64;
          if (n <= 5) {
            for (i = 0; i != 32; i++) {
              s32 v;

              v = (Trig_Sin(i << 11) * radius / 2 >> 16) + Data_08199590[j];
              ((BlitFunc)blit[0])(dest, &work->frames[n * 288], v - 6,
                                  (Trig_Cos(i << 11) * radius * 2 >> 16) + 60,
                                  12, 24);
            }
          }
        }
      }
      scale2.x = Q16_ONE;
      scale2.y = Q16_ONE;
      work->sprites[21]->priority = 2;
      work->sprites[22]->priority = 2;
      work->sprites[23]->priority = 2;
      work->sprites[24]->priority = 2;
      {
        s32 v;

        v = prevCamera + bobX;
        pos2.x = v + 0xa00000;
        pos2.z = ((104 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[21], &pos2, &scale2, 0);
        pos2.x = v + 0x800000;
        pos2.z = ((136 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[22], &pos2, &scale2, 0);
        pos2.x = v + 0xa00000;
        pos2.z = ((136 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[23], &pos2, &scale2, 0);
        pos2.x = v + 0xc00000;
        pos2.z = ((136 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[24], &pos2, &scale2, 0);
        work->sprites[25]->priority = 2;
        pos2.x = v + 0x800000;
        pos2.z = ((104 - Data_081994f0[work->shake]) << 16) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[25], &pos2, &scale2, 0);
      }
      for (i = 0; i != 5; i++) {
        pos2.x =
            (Data_08199592[i][0] << 16) + swayX + prevCamera + bobX + 0xb00000;
        pos2.z = ((Data_08199592[i][1] - Data_081994f0[work->shake]) << 16) +
                 swayY + bobY + 0x540000;
        work->sprites[26 + i]->priority = 2;
        Render_ApplyProjectedPlacementFar(work->sprites[26 + i], &pos2, &scale2,
                                          0);
      }
      if (frame <= 489) {
        for (i = 0; i != 8; i++) {
          Particle *p;

          p = &work->particles[32 + i];
          pos2.x = p->x + camera;
          pos2.z = p->y;
          if (p->timer != 0)
            Render_ApplyProjectedPlacementFar(work->sprites[41 + i], &pos2,
                                              &scale2, 0);
        }
      }
      if (frame < 540) {
        Particle *p;

        p = &work->particles[21];
        if (p->timer == 1) {
          p->x += p->vx;
          p->y += p->vy;
          p->vy += 0x2000;
          p->vx = p->vx * 60 / 64;
          p->vy = p->vy * 63 / 64;
        }
        for (i = 0; i != 4; i++) {
          pos2.x = ((i & 1) << 21) + p->x + camera + 0x800000;
          pos2.z = ((((i / 2) << 5) - Data_081994f0[work->shake]) << 16) +
                   swayY + p->y + 0x680000;
          work->sprites[34 + i]->priority = 1;
          Render_ApplyProjectedPlacementFar(work->sprites[34 + i], &pos2,
                                            &scale2, 0);
        }
      }
      if (frame <= 316) {
        Particle *p;

        p = &work->particles[20];
        if (p->timer != 0) {
          p->timer++;
          p->x += p->vx;
          p->y += p->vy;
          p->vy += 0x2000;
          p->vx = p->vx * 48 / 64;
          p->vy = p->vy * 63 / 64;
          work->sprites[31]->rotation += 0x100;
        }
        pos2.x = swayX + p->x + camera + 0xb80000;
        pos2.z = ((92 - Data_081994f0[work->shake]) << 16) + swayY + p->y;
        work->sprites[31]->priority = 1;
        Render_ApplyProjectedPlacementFar(work->sprites[31], &pos2, &scale2, 0);
      }
      {
        Vec3 *wave;
        s32 row;
        Sprite **slot;

        for (j = 0; j != 2; j++) {
          for (i = 0; i != 9; i++) {
            Sprite *obj;
            s32 v;

            wave = &pos2;
            wave->x = (i << 21) + waveX;
            wave->z = (((j << 5) - Data_081994f0[work->shake]) << 16) + shipY +
                      0x840000;
            if (i == 8) {
              work->sprites[j * 9 + i]->tile =
                  work->particles[j * 9 + (wavePhase + 71) % 8].timer & 0x3ff;
            } else {
              row = j * 9;
              k = row;
              k += i;
              work->sprites[k]->tile =
                  work->particles[row + (wavePhase + 64 + i) % 8].timer & 0x3ff;
            }
            if (pos2.x > 0xffffff)
              pos2.x -= 0x1200000;
            Render_ApplyProjectedPlacementFar(work->sprites[j * 9 + i], &pos2,
                                              &scale2, 0);
          }
        }
      }
      for (j = 0; j != 4; j++) {
        for (i = 0; i != 6; i++) {
          if (frame >= i * 2 + 490 && frame < i * 2 + 502) {
            s32 x, y, n;

            x = Data_0819959c[j][i][0];
            n = (frame - (i * 2 + 490)) / 2;
            y = Data_0819959c[j][i][1] + (swayY >> 16) -
                Data_081994f0[work->shake];
            ((BlitFunc)blit[0])(dest, &work->frames[n * 288], x - 6, y - 12, 12,
                                24);
          }
        }
      }
      for (i = 0; i != 16; i++) {
        for (j = 0; j != 32; j++) {
          Particle *q;

          q = &PARTICLES[i * 32 + j];
          if ((u32)q->timer <= 5) {
            ((BlitFunc)blit[0])(dest, &work->frames[q->timer * 288],
                                (q->x >> 16) - 2, (q->y >> 16) - 12, 12, 24);
            q->x += q->vx;
            q->y += q->vy;
            q->vy -= 0x4000;
            q->timer++;
          }
        }
      }
      {
        s32 *shake;
        s32 v;
        struct BgScroll *bg;
        s32 *params;

        shake = &work->shake;
        bg = gBgScroll;
        v = Data_081994f0[*shake];
        bg[1].y = v + 32;
        gCameraSceneParameters[4] = 120 - v;
        if (*shake > 0)
          *shake -= 1;
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
      if (frame != 800 && !(gInput.held & KEY_B) && !(gInput.repeat & KEY_A))
        goto phase2;
    }

    Task_Remove(BattleEffect_AdvanceScrollOnInterval);
    gBgScroll[1].x = savedY;
    ctl[4] = 0;
    BattleEffect_SetupBlendedDisplay();
    for (i = 0; i != 57; i++)
      Sprite_DestroyFar(work->sprites[i]);
    bank[21] = VramCache_AllocEntry(128);
    Palette_LoadBasicSpriteColors();
    Dma_Set((void *)OBJ_PLTT, (void *)(BG_PLTT + 5 * 16), 0x80000010,
            (vu32 *)0x040000d4);
    *(u16 *)(BG_PLTT + 94) = *(u16 *)(BG_PLTT + 244);
    work->transferMode = 2;
    work->transferValue = 50;
    *(s32 *)0x04000028 = 0;
    *(vu16 *)0x04000050 = 0x3f44;
    *(vu16 *)0x04000020 = 0x80;
    *(vu16 *)0x0400000c = 0x784;
    *(vu16 *)0x04000052 = 0x1010;
    arg->actors[arg->count] = 0xff;
    BattleActor_SpawnObjectsForListFar(arg->actors, 0);
    BattleEffect_SpawnObjects(1, 663, 1);
    for (i = 0; i != 15; i++) {
      work->sprites[1 + i] =
          EffectSprite_Acquire__f17e698(32, 16, 0x80006000, 0);
      IWRAM_COPY_WORDS_LINKED(work->sprites[1 + i], work->sprites[0], 24);
      work->sprites[1 + i]->unk16 = 32;
      work->sprites[1 + i]->unk17 = 8;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->frames,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0BA, work->frames + 0x3000,
                               0, 0);
    {
      const void *colors =
          (const void *)Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
      CopyFunc copy = IWRAM_COPY_WORDS_LINKED;

      copy((void *)BG_PLTT, colors, 128);
    }

    for (frame = 0; frame != 109; frame++) {
      if (frame == 0) {
        s32 z;
        Particle *p;

        i = 0;
        radius = 0;
        for (; i != 16; i++) {
          /* FAKEMATCH(dead): a store nothing reads. */
          if (work == NULL)
            k = 0;
          p = &work->particles[i];
          z = i * -0x100;
          p->timer = radius;
          Random16();
          p->ay = radius;
          p->z = z - (*(vs32 *)&p->ay * 40) - 0x800;
          p->vx = -Trig_Cos(p->z) * 8;
          p->vy = -Trig_Sin(p->z) * 8;
          p->x = ((Random16() % 80 + 80) << 16) - p->vx * 24;
          p->y = (((Random16() & 7) + 96) << 16) - p->vy * 24;
          work->sprites[i]->rotation = p->z;
          work->sprites[i]->unk16 = radius;
          work->sprites[i]->unk17 = 8;
        }
        for (i = 0; i != 512; i++)
          PARTICLES[i].timer = -1;
      }
      scale3 = Data_08196ee0;
      pos3.w = 0;
      pos3.y = 0xff0000;
      for (i = 0; i != 16; i++) {
        Particle *p;

        p = &work->particles[i];
        if (frame >= Data_081995cc[i] && p->timer == 0) {
          pos3.x = p->x;
          pos3.z = p->y;
          if (pos3.z > 0 && pos3.z < 0x800000)
            Render_ApplyProjectedPlacementFar(work->sprites[i], &pos3, &scale3,
                                              0);
          if (p->y > 0x6fffff) {
            p->timer = 1;
            for (j = 0; j != 2; j++) {
              s32 speed;
              s32 angle;
              Particle *q;

              q = &PARTICLES[i * 8 + j];
              speed = (Random16() & 31) + 32;
              angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
              q->x = p->x / 2;
              q->y = p->y;
              q->vx = Trig_Sin(angle) * speed >> 4;
              q->vy = Trig_Cos(angle) * speed >> 4;
              q->timer = 0;
            }
            Sound_PlayFar(SE_CLATTER);
            for (j = 0; j != arg->count; j++) {
              BattleEffect_StartActorMotion(arg->actors[j], 1, 0, 0, 0x20000,
                                            100);
              BattleEffect_TintBattler(arg->actors[j], 7, 5, j, 7);
              work->shake = 4;
            }
          } else {
            Particle *q;

            {
              s32 m = (frame - Data_081995cc[i]) % 16;
              q = &PARTICLES[(i << 4) + m + 256];
            }
            q->x = p->x / 2;
            q->y = p->y - 0x100000;
            q->vx = 0;
            q->vy = 0;
            q->timer = 0;
          }
          work->sprites[i]->rotation = p->z;
          BattleEffectKernels_IntegrateVector3(p, 64, 0);
          p->vx = -Trig_Cos(work->sprites[i]->rotation) * 8;
          p->vy = -Trig_Sin(work->sprites[i]->rotation) * 8;
          work->sprites[i]->tile = work->sprites[0]->tile;
        }
      }
      for (i = 0; i != 256; i++) {
        Particle *q;

        q = &PARTICLES[256 + i];
        if ((u32)q->timer <= 7) {
          s32 v;

          v = q->timer - 2;
          if (v >= 0)
            ((BlitFunc)blits[1])(dest, &work->frames[v * 288 + 0x3000],
                                 (q->x >> 16) - 6, (q->y >> 16) - 12, 12, 24);
          q->timer++;
        }
      }
      for (i = 0; i != 128; i++) {
        Particle *q;

        q = &PARTICLES[i];
        if ((u32)q->timer <= 17) {
          s32 v;

          v = q->timer / 3;
          ((BlitFunc)blits[i & 1])(dest, &work->frames[v << 11],
                                   (q->x >> 16) - 16, (q->y >> 16) - 48, 32,
                                   64);
          BattleEffectKernels_IntegrateVector2(q, 56, -0x2000);
          q->timer++;
        }
      }
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
    for (i = 0; i != 16; i++)
      Sprite_DestroyFar(work->sprites[i]);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    BattleEffect_EndCanvasLayer();
  }
}

#undef BLITTER

struct FxPartyList;

/* VIEW(f17e698): tag struct RenderModel as struct RenderModel__f17e698;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunDaedalusAlt moves: 2 of 572 instructions in
 * tla-ja. */
struct RenderModel__f17e698 {
  s32 kind;
  s32 unk04;
  const void *mesh;
  s32 *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *cells, s32 x, s32 y, s32 size);

void Transform_ApplyYaw(s32 angle);

void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);

void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets

void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar

extern char ResourceId_Data0C9;
extern u8 gMapCellBuffer[];

extern const u8 Data_081995ec[]; // the seven strike frames
/* VIEW(f17e698): tag struct PlacementSource as struct PlacementSource__f17e698;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:48946: structure has no member
 * named `x'. */
struct PlacementSource__f17e698 {
  s32 x;
  s32 y;
};
extern const struct PlacementSource__f17e698
    Data_08196ee8;                // the summon's placement source
extern const u8 Data_08199220[];  // the rings' mesh
extern const u8 gData_081991b0[]; // their corners
/* VIEW(f17e698): variable gData_02014000 as extern struct EffectStep
 * gData_02014000[]; f17e0c0 has extern u8 gData_02014000[]; tried as the rest
 * of the object declares it, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:48951: request for member
 * `variant' in something not a structure or union. */
extern struct EffectStep
    gData_02014000__f17e698[] __asm__("gData_02014000"); // the sparks

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)
#define HI(v) (((s16 *)&(v))[1])
#define DAEDALUS_FRAMES 108
#define DAEDALUS_SPARKS 64

// The summoned object's display fields that the scene sets.
struct SummonObject {
  u8 unk00[18];
  u16 unk12;
  u8 unk14[2];
  u8 unk16;
  u8 unk17;
};

// Abilities: Daedalus (the second scene).
//
// The summoned object glides in from the right and down over 92 frames
// while, from frame 56, a shaded disc swells at the centre and six rings of
// light spin out from it. At the seven frames of Data_081995ec every target
// is knocked back and flashes; from frame 80 eight bursts of eight sparks,
// one a frame, scatter over the canvas.
void BattleEffect_RunDaedalusAlt(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  s32 request[4];
  struct PlacementSource__f17e698 source;
  s32 frame;
  s32 i;
  s32 j;
  s32 centerX;
  s32 centerY;
  s32 burst;
  struct EffectStep *spark;
  s32 mask;
  s32 discX;
  s32 ringOffset;
  s32 ringX;
  s32 discY;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_ConfigureEffectDisplay();
  REG_WIN0V = 0x1080;
  REG_WININ = 0x2737;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_SpawnObjects(1, 0x296, 3);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_ADDITIVE, BLIT_CLIP));
  draw[0] = BLITTER;
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  frame = 0;
  burst = -80 * 8;
  do {
    if (frame == 0) {
      discX = 0x400000;
      ringOffset = -0x400000;
      ringX = 0x420000;
      discY = 0x500000;
      work->transferMode = 2;
      work->transferValue = 50;
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    }
    if (frame == 56) {
      Sound_PlayFar(SE_SPIN);
    }
    for (i = 0; i != 7; i++) {
      if (frame == Data_081995ec[i]) {
        for (j = 0; j != effect->count; j++) {
          BattleEffect_StartActorMotion(effect->actors[j], 1, 0x20000, 0x80000,
                                        0x20000, 120);
          BattleEffect_TintBattler(effect->actors[j], 7, 5, j, 7);
        }
        work->shakeFrames = 4;
        Sound_PlayFar(SE_CRASH);
      }
    }

    // From frame 80: eight bursts of sparks, one a frame.
    if (frame > 79) {
      // One pointer to the sparks for the burst and the draw loop below.
      struct EffectStep *sparks;

      if (frame == 80) {
        // UB: the lives are cleared through one s32 pointer stepped a whole
        // step at a time, past the step's scalar members.
        s32 *life = &gData_02014000__f17e698[0].variant;

        for (i = 0; i != DAEDALUS_SPARKS; i++) {
          *life = -1;
          life += sizeof(struct EffectStep) / sizeof(s32);
        }
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, 0x3c00, 0x3f3f3f3f);
      }
      sparks = gData_02014000__f17e698;
      if ((u32)(frame - 80) <= 7) {
        centerX = ((Random16() & 63) + 28) << 16;
        centerY = ((Random16() & 31) + 80) << 16;

        j = 0;
        mask = 15;
        spark = sparks + burst;
        for (; j != 8; j++, spark++) {
          s32 speed;
          s32 angle;
          speed = 31;
          speed &= Random16();
          angle = Random16();

          spark->x = centerX + (((Random16() & mask) - 8) << 16);
          spark->y = centerY + (((Random16() & mask) - 8) << 16);
          speed += 32;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 3;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 3;
          spark->variant = 0;
        }
      }
      {
        struct EffectStep *mote;

        for (i = 0, mote = sparks; i != DAEDALUS_SPARKS; i++, mote++) {
          if ((u32)mote->variant <= 17) {
            draw[0](canvas, work->sheet + ((mote->variant / 3) << 11),
                    HI(mote->x) - 16, HI(mote->y) - 48, 32, 64);
            BattleEffectKernels_IntegrateVector2(mote, 62, -0x800);
            mote->variant++;
          }
        }
      }
    }

    // The summoned object glides in over 92 frames.
    if (frame <= 91) {
      struct SummonObject *object;

      source = Data_08196ee8;
      object = work->objects[0];
      object->unk17 = 32;
      object->unk16 = 0;
      request[3] = 0;
      object->unk12 = 0x8800;
      request[1] = 0xff0000;
      if (frame <= 63) {
        request[0] = (152 - frame / 4) << 16;
        request[2] = (frame << 17) - 0x400000;
      } else if ((u32)(frame - 64) <= 63) {
        s32 t = frame - 64;

        request[0] = (136 - t / 16) << 16;
        request[2] = ((t / 2) << 16) + 0x400000;
      } else {
        request[0] = 136 << 16;
        request[2] = 0x400000;
      }
      Render_ApplyProjectedPlacementFar(work->objects[0], request, &source.x,
                                        0);
    }

    // Frames 56 to 91: the disc and the rings.
    if ((u32)(frame - 56) <= 35) {
      s32 size;
      s32 grow = frame * 2 - 104;
      struct RenderModel__f17e698 *model;
      void *vertices;
      struct EffectStep *ring;
      u8 *cells;

      /* FAKEMATCH(temporary): the disc's x copied into the loop counter i. */
      i = discX;
      size = 128;
      if (grow <= 64) {
        size = grow * 2;
        cells = gMapCellBuffer;
        BattleEffect_DrawShadedCircles(cells, size, 0x10000);
      }
      cells = gMapCellBuffer;
      Canvas_BlitAtFoot(cells, i >> 16, discY >> 16, size);
      if (frame == 56) {
        s32 delay;

        i = 0;
        delay = 0;
        for (ring = work->particles; i != 16; i++, ring++) {
          ring->x = Random16() & 0xffff;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1) {
            ring->y = -ring->y;
          }
          ring->variant = delay;
          delay -= 4;
        }
        Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                                   work->sheet + 0x4000, 0, 0);
      }
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      model->shade = 0;
      texture.logWidth = 5;
      texture.logHeight = 2;
      model->vertices = vertices;
      texture.pixels = work->sheet + 0x4000;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = Data_08199220;

      for (i = 0; i != 6; i++) {
        ring = &work->particles[i];
        ring->variant += 2;
        ring->x += ring->y * 2;
        if ((u32)ring->variant <= 47) {
          s32 k;

          Transform_Reset();
          k = i & 3;
          SceneTransform_ApplyPosition3((k << 17) + ringX + ringOffset,
                                        discY - ((k + 2) << 16) + ringOffset,
                                        0);
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE);
          Transform_ApplyRoll(ring->x);
          SceneTransform_ApplyScale3(Trig_Sin(ring->variant * 682) + TRIG_TURN,
                                     0x20000, 0x10000);
          Render_TransformVertices(gData_081991b0, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    burst += 8;
    frame++;
  } while (frame != DAEDALUS_FRAMES);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  Sprite_DestroyFar(work->objects[0]);
  BattleEffect_EndCanvasLayer();
}
#undef HEAP_SLOT_BLITTER
#undef BLITTER_MAX
#undef BLITTER
#undef CLIP_XY
#undef BURST_FRAMES
#undef BURST_DRAW_FRAMES
#undef CEL_SIZE
#undef PICTURE_OFFSET
#undef MIRROR_HALF
#undef SYSTEM
#undef IWRAM_COPY_WORDS_LINKED
#undef Iwram_Clear
#undef DECODE_BUFFER
#undef PARTICLES
#undef ALCHEMY_TYPES_H
#undef BLIT_MODE
#undef BLIT_CLIP
#undef HI
#undef DAEDALUS_FRAMES
#undef DAEDALUS_SPARKS
