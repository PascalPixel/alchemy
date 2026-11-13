/// @file
/// Star Mine and the enemy's Rising Dragon.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);

void BattlePlayback_StartFar(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
/* VIEW(f17d174): routine GetBattleObjectSlotFar as void
 * **GetBattleObjectSlotFar(s32 objectId); motobj.h has struct BattleObj
 * *GetBattleObjectSlotFar(s32 objectId); tried as the 11 declarations' void
 * **GetBattleObjectSlotFar(s32 objectId), it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:28796: conflicting types for
 * `GetBattleObjectSlotFar'. */
extern void **
GetBattleObjectSlotFar__f17d174(s32 objectId) __asm__("GetBattleObjectSlotFar");
void *Resource_GetTableEntry(s32 resource);

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_BlueCrescentSheet;
extern char ResourceId_Data134;
extern char ResourceId_OrangePaletteB;
extern u16 gBattleEffect_MoteCells[];

#define HI(v) (((s16 *)&(v))[1])
#define STAR_MINE_FRAMES 144
#define STARS 6
#define STAR_SPARKS 48
#define STAR_DROPS 192
#define DROPS ((struct EffectStep *)Ram_MapCellBuffer)

// Abilities: Star Mine.
//
// Six stars (a 24 by 48 picture mirrored both ways from the sheet's two
// halves) are thrown from the caster, each one at a target in turn; from
// frame 8 * i + 11 each is drawn toward its target and bursts once it has
// passed it, into four sparks and 32 drops, pushing the target back.
void BattleEffect_RunStarMine(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  DrawRectangle blitters[2];
  struct EffectPosition pos;
  struct EffectStep *star;
  struct EffectStep *particle;
  struct EffectStep *drop;
  s32 *camera;
  s32 delay;
  s32 member;
  s32 firstDrop;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet, work->sheet, 1,
                             1);

  // The star: each half of the sheet's 24-wide picture mirrored left to
  // right and top to bottom.
  for (i = 0; i != 2; i++) {
    u8 *picture = work->sheet + 0x1200 + i * 24 * 48;
    s32 y;

    for (y = 0; y != 24; y++) {
      s32 x;

      for (x = 0; x != 12; x++) {
        picture[y * 24 + x] = work->sheet[i * 24 * 24 + y * 24 + x * 2 + 1];
        picture[y * 24 + 23 - x] = work->sheet[i * 24 * 24 + y * 24 + x * 2];
        picture[(47 - y) * 24 + x] =
            work->sheet[i * 24 * 24 + y * 24 + x * 2 + 1];
        picture[(47 - y) * 24 + 23 - x] =
            work->sheet[i * 24 * 24 + y * 24 + x * 2];
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       128);
  work->transferMode = 2; // keeps 50 or 75 percent (see transferMode)
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != STAR_MINE_FRAMES; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 0) {
      struct EffectStep *thrown;

      for (i = 0, thrown = work->particles; i != STARS; i++, thrown++) {
        struct StarTarget *caster =
            *GetBattleObjectSlotFar__f17d174(effect->actor);
        s32 height = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
        s32 velocity;

        thrown->x = caster->x;
        thrown->y = caster->y + height;
        thrown->z = caster->z;
        velocity = ((Random16() & 255) - 128) << 8;
        // DEAD: both stores are overwritten by the 0 below; the ROM keeps them.
        thrown->velocityX = velocity;
        if (thrown->x > 0) {
          thrown->velocityX = -velocity;
        }
        thrown->velocityX = 0;
        thrown->velocityY = ((Random16() % 400) - 128) << 9;
        thrown->velocityZ = ((Random16() & 255) - 128) << 10;
        thrown->variant = 0;
      }
      for (i = 0; i != STAR_SPARKS; i++) {
        work->particles[STARS + i].variant = -1;
      }
      for (i = 0; i != STAR_DROPS; i++) {
        DROPS[i].variant = -1;
      }
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    for (i = 0, star = work->particles, delay = 0, firstDrop = 0; i != STARS;
         star++, delay += 8, firstDrop += 32, i++) {
      s32 k;
      struct StarTarget *target;
      s32 height;
      s32 picture;
      s32 dx;
      s32 dy;
      s32 dz;
      s32 a;
      s32 b;

      member = i % effect->count;
      target = *GetBattleObjectSlotFar__f17d174(effect->actors[member]);
      height = BattleUnit_GetEffectHeightFar(effect->actors[member]) / 2;
      if (star->variant < 0) {
        continue;
      }
      picture = ((frame + i) / 2) & 1;
      EffectPosition_ProjectPoint(star, &pos);
      pos.x >>= 1;
      blitters[0](canvas, work->sheet + 0x1200 + picture * 24 * 48, pos.x - 12,
                  pos.y - 24, 24, 48);
      BattleEffectKernels_IntegrateVector3(star, 63, 0);
      if (frame <= delay + 10) {
        continue;
      }
      dx = (target->x - star->x) >> 10;
      dy = (target->y + height - star->y) >> 10;
      dz = (target->z - star->z) >> 10;
      star->velocityX += dx;
      star->velocityY += dy;
      star->velocityZ += dz;
      a = star->x;
      if (a < 0) {
        a = -a;
      }
      b = target->x;
      if (b < 0) {
        b = -b;
      }
      if (a <= b) {
        continue;
      }

      // Passed its target: the burst.
      for (k = 0; k != 4; k++) {
        particle = &work->particles[STARS + delay + k];
        particle->x = pos.x << 16;
        particle->y = pos.y << 16;
        particle->velocityX = ((Random16() & 127) - 64) << 12;
        particle->velocityY = (192 - (Random16() & 255)) << 10;
        particle->variant = 0;
      }
      for (k = 0; k != 32; k++) {
        s32 angle;
        s32 speed;

        drop = &DROPS[firstDrop + k];
        drop->x = pos.x << 16;
        drop->y = pos.y << 16;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = (Random16() & 255) + 127;
        drop->velocityX = (Trig_Sin(angle) * speed) >> 5;
        drop->velocityY = (Trig_Cos(angle) * speed) >> 5;
        drop->variant = (Random16() & 15) + 16;
      }
      BattlePlayback_StartFar(134);
      Sound_PlayFar(145);
      BattleEffect_StartActorMotion(effect->actors[member], 1, 0x20000, 0x80000,
                                    0x20000, i * 5 + 110);
      BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 16);
      work->shakeFrames = 4;
      star->variant = -1;
    }

    for (i = 0; i != STAR_SPARKS; i++) {
      particle = &work->particles[STARS + i];
      if ((u32)particle->variant <= 23) {
        if (particle->variant > 1) {
          blitters[0](canvas,
                      work->sheet + 0x4000 + ((particle->variant / 4) << 11),
                      HI(particle->x) - 16, HI(particle->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(particle, 60, -0x1000);
        particle->variant++;
      }
    }
    for (i = 0; i != STAR_DROPS; i++) {
      particle = &DROPS[i];
      if (particle->variant >= 0) {
        s32 size = (particle->variant >> 3) + 1;

        blitters[0](canvas, sprites + gBattleEffect_MoteCells[size - 1],
                    HI(particle->x) - size / 2, HI(particle->y) - size, size,
                    size * 2);
        BattleEffectKernels_IntegrateVector2(particle, 60, 0x1000);
        particle->variant--;
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

/// Fills the 32x30 tilemap at screen block 31: the upper 15 rows from tile
/// 0x210 on, the lower 15 from tile 48 on, each row repeating 16 tiles.
void BattleEffect_BuildRepeatingTilemap(void) {
  u16 *dst = (u16 *)0x0600f800;
  s32 y;
  s32 x;

  for (y = 0; y != 30; y++) {
    if (y > 14) {
      for (x = 0; x != 32; x++)
        *dst++ = y * 32 + (x & 15) + 48;
    } else {
      for (x = 0; x != 32; x++)
        *dst++ = y * 32 + (x & 15) + 0x210;
    }
  }
}

struct FxPartyList;
struct Model;
struct StripQuad;

/* VIEW(f17d174): tag struct RenderModel as struct RenderModel__f17d174;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunRisingDragonEnemy moves: 5 of 1028
 * instructions in tla-ja. */
struct RenderModel__f17d174 {
  s32 kind;
  s32 unk04;
  const u8 *mesh;
  s16 *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter

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

void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void SceneTransform_ApplyRoll(s32 angle);

void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);

void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

void BattleBackground_LoadFar(s32 layer, s32 resourceId, s32 mode);
void Palette_BrightenBgEntries(s32 red, s32 green, s32 blue);

extern char ResourceId_SmokeBandSheet;

extern char ResourceId_ClawSlashSheet;

extern char ResourceId_RedHazeBackdrop;
struct CornerRow {
  struct Corner corners[2];
};
extern u8 gMapCellBuffer[];
// 256 sparks. The listing's pool holds this address plus each constant offset
// as one word (0x02014018, 0x02014e00, 0x02014e18), which only a constant
// address folds to; through the symbol, each offset becomes an extra add.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
extern u8 Data_02011000[];
extern u8 Data_02012000[];

extern const u8 gData_08199340[];
extern const u8 Data_081991e0[];

#define SEGMENTS 33

// Abilities: Rising Dragon, the enemy's.
//
// The dragon bursts up under the target and carries it into the sky: the
// backdrop changes to a red haze scrolling past as the target is lifted,
// swaying, then is dropped back to the field at frame 166 and lands at 179.
void BattleEffect_RunRisingDragonEnemy(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *vertices;
  u8 *strip;
  struct CornerRow *cells;
  DrawRectangle blit[2];
  u8 *sprites;
  s32 *victim;
  u16 *camera;
  s16 savedYaw;
  s32 height;
  s32 savedMode;
  s32 savedX;
  s32 savedZ;
  struct EffectPosition source;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel__f17d174 *model;
  struct ModelTexture *tex;
  s32 zero;
  s32 burstFrame;
  s32 yaw;
  struct EffectStep *spawned;
  struct EffectStep *particle;
  struct EffectStep *scatter;
  s32 frame;
  s32 i;
  s32 j;

  vertices = Data_02011000;
  strip = Data_02012000;
  cells = (struct CornerRow *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(1);
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  victim = *GetBattleObjectSlotFar__f17d174(effect->actors[0]);
  camera = (u16 *)Ram_HeapSlots->cameraWork;
  savedYaw = camera[27];
  height = 0;
  savedMode = victim[18];
  savedX = victim[2];
  savedZ = victim[4];
  EffectPosition_ProjectBattler(effect->actors[0], &source);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  REG_BG2PA = 0x80;
  Render_BuildQuadStrip(strip, 4, 32, 32);

  for (frame = 0; frame != 186; frame++) {
    if (frame == 0) {
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
      for (i = 0; i != 128; i++) {
        SPARKS[128 + i].variant = -1;
      }
      for (i = 0, spawned = SPARKS; i != 128; i++, spawned++) {
        s32 angle;
        s32 speed;

        spawned->x = (source.x / 2) << 16;
        spawned->y = 240 << 15;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = (Random16() & 1023) + 32;
        spawned->velocityX = (Trig_Sin(angle) * speed) >> 6;
        spawned->velocityY = (Trig_Cos(angle) * speed) >> 6;
        spawned->variant = (Random16() & 7) + 32;
      }
    }
    if (frame == 4) {
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 2);
      work->shakeFrames = 8;
      Sound_PlayFar(145);
    }
    if (frame == 163) {
      REG_BG2CNT = 0x784;
    }
    if (frame == 164) {
      BattleBackground_LoadFar(1, ((u16 *)Ram_HeapSlots->battleWork)[824], 24);
    }
    if (frame == 165) {
      gCameraSceneParameters[4] = 120;
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    }
    if (frame == 166) {
      camera[27] = savedYaw;
      victim[18] = savedMode;
      victim[2] = savedX;
      victim[3] = 200 << 15;
      victim[4] = savedZ;
      gBgScroll[1].y = 32;
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 16);
    }
    if (frame == 179) {
      BattlePlayback_StartFar(134);
      work->shakeFrames = 8;
    }
    if (frame > 165) {
      work->transferMode = 3;
      work->transferValue = 0x04040404;
      goto next;
    }

    if (frame == 59) {
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 60) {
      u16 *color;
      s32 max; // One constant, set in the for: red takes its 31 from it.

      // Into the sky: the red haze, its palette made grey-blue.
      BattleBackground_LoadFar(1, (s32)&ResourceId_RedHazeBackdrop, 0);
      for (color = (u16 *)(BG_PLTT + 96), i = 0, max = 31; i != 128;
           i++, color++) {
        s32 entry = *color;
        s32 red;
        u16 green;
        s32 blue;

        red = max & entry;
        /* FAKEMATCH(dead): green is taken and clamped, then never used; as no
         * green at all, 329 of 1034 instructions differ in tla-en. */
        green = ((u16)entry >> 5) & 31;
        blue = ((u16)entry >> 10) & 31;
        // DEAD: both channels are already at most 31.
        if (red > 31) {
          red = 31;
        }
        if (blue > 31) {
          blue = 31;
        }
        green = green > 31 ? 31 : green;
        *color = (red << 10) | (red << 5) | blue;
      }
      BattleEffect_BuildRepeatingTilemap();
      gBgScroll[1].y = 104;
      gCameraSceneParameters[4] = 240;
      if (effect->side == 0) {
        yaw = 0x4000;
        camera[27] = yaw;
      } else {
        yaw = 0xc000;
        camera[27] = yaw;
      }
      for (i = 0; i != 256; i++) {
        SPARKS[i].variant = -1;
      }
      height = 220;
    }
    if (frame == 134) {
      Sound_PlayFar(145);
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame > 59) {
      s32 scroll = gBgScroll[1].y;
      s32 climb;

      if (scroll > 104) {
        scroll -= 0x10000;
      }
      scroll -= 8;
      if (scroll < -16) {
        scroll += 120;
      }
      gBgScroll[1].y = scroll;
      climb = 4 - (frame - 60) / 8;
      if (climb < 0) {
        climb = 0;
      }
      if (frame > 119) {
        climb = frame - 120;
      }
      height -= climb;
    } else {
      height = 200 - frame * 8;
    }
    if (height <= 55) {
      height = 56;
    }
    victim[18] = 0;
    if ((u32)(frame - 4) <= 55) {
      victim[3] = (frame * 9 << 16) - (18 << 16);
    } else if (frame > 59) {
      victim[2] = 0;
      victim[3] = (294 - height) << 16;
      victim[4] = -(Trig_Sin(frame << 10) << 3);
      if (frame == 60) {
        EffectPosition_ProjectBattler(effect->actors[0], &source);
      }
      EffectPosition_ProjectBattler(effect->actors[0], &target);
    }
    if ((u32)(frame - 152) <= 15) {
      work->transferMode = 1;
      work->transferValue = 0x20202020;
      if (frame > 162) {
        work->transferValue = 0x3f3f3f3f;
      }
      Palette_BrightenBgEntries(2, 2, 2);
    }
    // DEAD: frame is never negative.
    if (frame < 0) {
      goto next;
    }

    // The dragon's body, swaying.
    for (j = 0; j != SEGMENTS; j++) {
      for (i = 0; i != 2; i++) {
        ((struct Corner *)(cells + j))[i].x =
            i * 20 + ((Trig_Sin(j * 2018 + (frame << 10)) * 2) >> 16) - 10;
        ((struct Corner *)(cells + j))[i].z = 0;
        ((struct Corner *)(cells + j))[i].y = j * 2 - 64;
      }
    }
    zero = 0;
    model = Render_AllocateModelList(1);
    tex = &texture;
    model->kind = 6;
    model->shade = zero;
    model->scrollV = zero;
    model->vertices = vertices;
    tex->logWidth = 6;
    tex->logHeight = 5;
    model->texture = tex;
    tex->pixels = work->sheet;
    model->mesh = strip;
    {
      s32 u = frame << 2;

      /* FAKEMATCH(barrier): an empty asm that claims to read frame, giving
       * frame's reload a fifth dependent, so sched2 takes it before the
       * strip's pool load; without it, 2 of 1034 instructions differ in
       * tla-en. */
      asm("" : "+l"(u) : "l"(frame));
      model->scrollU = 127 & ~u;
    }
    Render_ResetTransformState();
    SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
    Transform_Push();
    Func_08015160((source.x - 128) << 16, height << 16, 0);
    if (effect->side == 1) {
      SceneTransform_ApplyYaw(0x8000);
    }
    Func_0801521c(0x21770);
    Render_TransformVertices(cells, vertices, 66);
    Render_DrawModelList(model);
    Transform_Pop();
    if (frame <= 133 && (u32)(frame - 17) > 42) {
      // The claw.
      tex->logWidth = 6;
      tex->logHeight = 6;
      model->mesh = gData_08199340;
      model->scrollU = zero;
      tex->pixels = work->sheet + 0x4000;
      Func_08015160((target.x - 124) << 16, (height - 144) << 16, 0);
      SceneTransform_ApplyRoll(0x4000);
      SceneTransform_ApplyScale3(0x19c40, 0x14e20, 0x10000);
      Render_TransformVertices(Data_081991e0, vertices, 4);
      Render_DrawModelList(model);
    }
    Heap_Release(model);

    if (frame == 134) {
      for (i = 0, scatter = SPARKS; i != 256; i++, scatter++) {
        s32 angle;
        s32 speed;

        scatter->x = (source.x / 2) << 16;
        scatter->y = 0;
        angle = (Random16() & 0x7fff) - 0x4000;
        speed = (Random16() & 255) + 32;
        scatter->velocityX = (Trig_Sin(angle) * speed) >> 5;
        scatter->velocityY = (Trig_Cos(angle) * speed) >> 5;
        scatter->variant = (Random16() & 7) + 32;
      }
    }
    burstFrame = frame - 134;
    if ((u32)burstFrame <= 7) {
      s32 first = burstFrame * 4;
      for (i = 0; i != 4; i++) {
        struct EffectStep *burst = &work->particles[first + i];
        s32 angle;
        s32 speed;

        burst->x = (source.x / 2) << 16;
        burst->y = 0;
        angle = (Random16() & 0x7fff) - 0x4000;
        speed = (Random16() & 255) + 32;
        burst->velocityX = (Trig_Sin(angle) * speed) >> 5;
        burst->velocityY = (Trig_Cos(angle) * speed) >> 5;
        burst->variant = 0;
      }
    }
    for (i = 0, particle = work->particles; i != 32; i++, particle++) {
      s32 picture = particle->variant;

      if ((u32)picture <= 23) {
        if (picture > 1) {
          blit[0](canvas, work->sheet + 0x800 + ((picture / 4) << 11),
                  HI(particle->x) - 16, HI(particle->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(particle, 60, -0x1000);
        particle->variant++;
      }
    }
    if ((u32)frame <= 133) {
      struct EffectStep *trail;
      s32 startY;
      s32 slot;
      s32 first;
      s32 y;

      slot = (frame - 16) & 63;
      y = 120;
      if (frame > 59) {
        y = height - 80;
      }
      first = slot * 2;
      for (i = 0, startY = y << 16, trail = &SPARKS[128 + first]; i != 2;
           i++, trail++) {
        s32 random;
        s32 angle;
        s32 speed;
        s32 x = source.x / 2;

        trail->y = startY;
        trail->x = x << 16;
        random = Random16() & 0x7fff;
        angle = random + 0x4000;
        if (frame > 59) {
          angle = random + 0xc000;
        }
        speed = (Random16() & 255) + 32;
        trail->velocityX = (Trig_Sin(angle) * speed) >> 6;
        trail->velocityY = (Trig_Cos(angle) * speed) >> 6;
        if (frame > 59) {
          trail->velocityX /= 2;
        }
        trail->variant = (Random16() & 7) + 32;
      }
    }
    {
      struct EffectStep *spark;
      s32 gravity = -0x1000;

      if (frame > 59) {
        gravity = 0x4000;
      }
      for (i = 0, spark = SPARKS; i != 256; i++, spark++) {
        s32 size;

        if (spark->variant < 0) {
          continue;
        }
        size = (spark->variant - 8) / 8;
        if (frame > 59) {
          size += 2;
        }
        if (size <= 0) {
          size = 1;
        }
        blit[0](canvas, sprites + gBattleEffect_MoteCells[size - 1],
                HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
        spark->variant--;
      }
    }
  next:
    BattleEffect_CountDownBattlerTints();
    if ((u32)(frame - 32) > 133) {
      BattleCamera_Shake(8, 8);
    }
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void Func_0817e0c0(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Bone Charge (kind 311).
void BattleEffect_RunBoneCharge(s32 a0) { Func_0817e0c0(a0, 4); }

/// Abilities: Brute Force, Troll (kind 314).
void BattleEffect_RunBruteForce(s32 a0) { Func_0817e0c0(a0, 1); }

/// Abilities: Onslaught (kind 326).
void BattleEffect_RunOnslaught(s32 a0) { Func_0817e0c0(a0, 2); }

/// Abilities: Headbutt (kind 339).
void BattleEffect_RunHeadbutt(s32 a0) { Func_0817e0c0(a0, 3); }

/// Abilities: Stun Muscle (kind 433).
void BattleEffect_RunStunMuscle(s32 a0) { Func_0817e0c0(a0, 4); }

/// Abilities: Living Armor, Minotaur, Power Crush (kind 457).
void BattleEffect_RunLivingArmor(s32 a0) { Func_0817e0c0(a0, 5); }
#undef HI
#undef STAR_MINE_FRAMES
#undef STARS
#undef STAR_SPARKS
#undef STAR_DROPS
#undef DROPS
#undef SLOT_BLITTER
#undef BLITTER
#undef SPARKS
#undef SEGMENTS
