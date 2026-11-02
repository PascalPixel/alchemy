// Draft, differ 1075 of 2781, skeleton gap 107/106 (tla-en, 2026-11-01)
// BattleEffect_RunCoatlicue: same calls and control flow, draft 2780 of 2781
// instructions, frame 164 and every stack slot as in the native. Registers
// agree for frame (r11), i (r8), k (r10), model and the last loop's locals.
// What is left, by cause; the first is worth most of the differ:
// - Palette ease, sr. Reload hands out spill registers in turn, so everything
//   from this loop on is one rotation off. The native computes the red mask
//   into a fresh register (mov r3, r9; ands r3, r1; adds r1, r3, r2), which
//   takes r2 for tr and evicts sg to sp+8, three reloads the draft lacks.
//   regmove's fixup_match_1 merges that temporary into s because s dies at the
//   and and the temporary dies at the add. Native needs a form where it does
//   not: the and's first operand the mask, or the temporary set twice. Tried,
//   none closes it: s as the outer s or as an entry variable shared by both
//   palette loops, sr += tr (the mask loses r9 and rematerializes, frame 160),
//   sg likewise, tr + red, (u8) and (u16) red, u16 v then s, a red local, every
//   order of sb, sg, sr. Fix this first, then re-read everything below.
// - The second palette's red: the native has movs r2, #31; ands r2, r3 ahead of
//   the shared shift, keeping the green and blue pool mask apart. Red first
//   makes CSE take the halfword mask as the low part of red's register. Only
//   r = 31; r &= s does it, a forced temporary, so blue, green, red stays; its
//   b.n is a literal pool jump that follows when the length is right.
// - spin is set at the top of the frame loop so loop.c reduces it: a giv with
//   add cost 2 needs a replaceable long-lived use plus one more site, which is
//   why Yaw(frame << 9) stays spelled out once. GCSE then folds the two i-loop
//   Yaw sites into the reduced slot, where the native keeps mov, lsls. A spin
//   set only at the model block is not replaceable and is not reduced.
// - The cells loop has the object slots in r7 and cells in r5, the native the
//   reverse, with i's zero set earlier. Tried: stepped pointer, slot pointer,
//   indexed, shifted index, operand order, inner obj, moved i.
// Measured: gData_02015000 is a constant address, the native pools its +24 and
// a symbol leaves an add; gMapCellBuffer is the set-once local for its first
// five uses and the symbol for the three texture.pixels, chosen per site. Row
// is no variable: particles[k * 7 + i] in a real for gives the native's stepped
// k * 7. Data_ tables are not const, the native rereads them after calls;
// colours as bitfields extract with two shifts where the native shifts and
// masks, so COLOR_* cast to u16 for the halfword mask the native pools; the
// model s and the second palette's s are one variable, sh and ty one each
// across the model blocks; i is also the particle loops' inner counter; k
// doubles as the palette's blue target, which blocks r4. blit[2] and grid are
// 8-byte locals, which gives the unused word at sp+68 and the pair at sp+56.
// BLIT_CELL's order (cell offset, width, height) sets the live length of the
// height table's address, and that decides frame's register. CopyWords is
// inline so its constant size stays out of CSE (without it, differ 1366); it
// carries no tag, the question against S2's wrapper kind is open.
// ResourceId_BellMaidenTiles (0xa7), ResourceId_BlueFlameSheet (0x130), Data0FB
// and Data0FC are not yet named in resource_directory.s; adoption names them
// there. They are symbols because the native loads ids from the pool.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct ModelTexture {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  const void *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
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
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource,
                               s32 mode); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
s32 Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
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

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "bg_scroll.h"
#include "dma.h"
#include "input.h"
#include "io_write_queue.h"

// The effect sprite Func_0815b290 returns, as far as this function reads it.
struct SpriteView {
  s32 unk00;
  s32 attributes;
  u32 tile : 10;
  u32 priority : 2;
  u32 rest : 20;
  s32 unk0c;
  u8 vramEntry;
};

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern struct EffectStep gData_02015000[];
#define Ram_EffectSteps FIXED_ADDRESS((struct EffectStep *), 0x02015000, gData_02015000)
extern u8 Data_02014400[];
extern u8 Data_02014800[];
extern u32 gCameraSceneParameters[];
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
extern char ResourceId_Data0FD;
extern char ResourceId_Data134;

struct SpriteView *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void Func_08179e6c(void);
void BattleEffect_ArmPaletteHBlankDma(void);
void Func_081885f0(void *vertices, void *triangles, s32 count);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries
void Func_08164b2c(s32 r, s32 g, s32 b);
void Heap_Release(void *buffer);

#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->vramEntry].tile)
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
    BLIT_CELL(0x4800, n_, HALF_HI((p)->x), HALF_HI((p)->y));                   \
    BattleEffectKernels_IntegrateVector2(p, 63, -0x4000);                      \
    if ((p)->y < -(Data_081996bb[n_] << 16)) {                                 \
      (p)->variant = -1;                                                       \
    }                                                                          \
  } while (0)

static inline void CopyWords(void *destination, const void *source, s32 size) {
  Iwram_CopyWords(destination, source, size);
}

// Coatlicue: the ground opens over a green dusk, the serpent goddess rises in
// a spiral of stone, her coils sweep the field and her light heals the party.
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
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  y = -80;
  for (frame = 0; frame != 27; frame++) {
    y += 4;
    REG_BG2X = y << 8;
    work->transferPending = 1;
    wait(1);
  }
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
  Func_081885f0(Data_02014400, Data_02014800, 32);
  list[0] = 255;
  BattleActor_SpawnObjectsForListFar(list, 0);
  {
    u8 *e = Resource_GetTableEntry((s32)&ResourceId_BellMaidenTiles);

    CopyWords((void *)OBJ_PLTT, e, 512);
    Resource_DecodeType01(e + 512, cellBuffer);
    for (i = 0; i != 12; i++) {
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      obj->priority = 2;
      work->objects[i] = obj;
      CopyWords((void *)OBJ_TILES(obj), cellBuffer + i * 1024, 1024);
    }
  }
  Graphics_LoadGreyRamp();
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FB, cellBuffer, 0, 0);
  Func_0816ae40(cellBuffer, work->sheet, 64, 64);
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
    spin = frame << 9;
    if ((gInput.repeat & 3) && frame <= 581 && skip == 0) {
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
        Ram_EffectSteps[i].variant = -1;
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
      Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
      Sound_PlayFar(130);
    }
    if (frame == 210) {
      Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
      Sound_PlayFar(130);
    }
    if (frame == 274) {
      Sound_PlayFar(139);
    }
    if (frame == 310) {
      Sound_PlayFar(206);
    }
    if (frame == 316) {
      Sound_PlayFar(154);
    }
    if (frame == 466) {
      Sound_PlayFar(142);
    }
    if (frame == 498) {
      Sound_PlayFar(154);
    }
    if (frame == 503) {
      Sound_PlayFar(154);
    }
    if (frame == 508) {
      Sound_PlayFar(154);
    }
    if (frame == 520) {
      Sound_PlayFar(212);
    }
    if (frame == 540) {
      Sound_PlayFar(186);
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
      s32 b = frame - 253;
      u16 *dst = (u16 *)OBJ_PLTT;
      u32 c;

      if (g > 31) {
        g = 31;
      }
      if (b > 31) {
        b = 31;
      }
      c = b << 10 | g << 5;
      for (i = 0; i != 256; i++) {
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
        s32 sr = COLOR_RED(s) + tr;

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
      s32 half = d / 2;

      for (i = 0; i != 256; i++, e++, dst++) {
        s32 b;
        s32 g;
        s32 r;

        s = *e;
        b = COLOR_BLUE(s);
        g = COLOR_GREEN(s) + half;
        r = COLOR_RED(s) + d;
        b += d / 4;

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
      place[3] = 0;
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
    if ((u32)(frame - 274) <= 23) {
      p = &Ram_EffectSteps[frame - 274 + 80];
      {
        s32 x = ((Random16() & 15) + 52) << 16;

        p->y = 0x640000;
        p->x = x;
      }
      p->velocityX = ((Random16() & 31) - 16) << 12;
      p->velocityY = -((Random16() & 15) + 8) << 12;
      p->z = (Random16() & 7) + 5;
      p->variant = 32;
    }
    if (frame == 310) {
      p = Ram_EffectSteps;

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
          p->variant = 0;
        }
      }
    }
    if (frame <= 359) {
      p = Ram_EffectSteps;

      for (i = 0; i != 384; i++, p++) {
        if (p->variant >= 0) {
          DRAW_SPARK(p);
        }
      }
    } else {
      Render_ResetTransformState();
      Func_08015160(0, 0, 0x1000000);
      if (frame > 539) {
        Func_080151e4(frame * 1500 - 0xc4c10, 0x10000, 0x10000);
      } else if (frame > 517) {
        Func_080151e4(0, 0, 0x10000);
      } else if (frame > 485) {
        s32 s = 0x103000 - (frame << 11);

        Func_080151e4(s, s, 0x10000);
      }
      if (frame > 539) {
        SceneTransform_ApplyYaw(frame << 10);
      } else if (frame > 517) {
        SceneTransform_ApplyYaw(0x10000);
        SceneTransform_ApplyPitch(0x8000);
      } else if (frame > 453) {
        s32 a = (frame - 454) << 10;

        SceneTransform_ApplyYaw(a);
        SceneTransform_ApplyPitch(Trig_Sin(a) >> 3);
      }
      Func_080151e4(55536, 45536, 27768);
      if (frame == 360) {
        struct EffectStep *p = Ram_EffectSteps;

        if (skip == 0) {
          work->transferMode = 2;
          work->transferValue = 75;
        }
        for (i = 0; i != 384; i++, p++) {
          p->variant = Random16() & 3;
          p->x = (Random16() % 160 - 80) << 16;
          p->y = (Random16() % 160 + 48) << 16;
          p->z = (Random16() % 160 - 80) << 16;
          p->velocityX = 0;
          p->velocityY =
              (-((Random16() & 63) + 32) << 10) - (1 << (16 - p->variant));
          p->velocityZ = Random16();
        }
      }
      if (frame == 540) {
        p = Ram_EffectSteps;

        for (i = 0; i != 384; i++, p++) {
          p->y = 0;
          p->velocityY = -((Random16() & 63) + 32) << 15;
        }
      }
      {
        p = Ram_EffectSteps;

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
          q->y = HALF_HI(q->y);
          q->x = HALF_HI(q->x) + 60;
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
          BLIT_CELL(0x53b8, n, x, top >> 16);
          BattleEffectKernels_IntegrateVector3(step, 64, 0);
          if (step->z < 0) {
            step->z = 0;
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
        s32 pitch = 4000;

        ty = (frame * 3 << 13) - 0x4ec000;
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
        Render_ResetTransformState();
        Func_08015160(0, ty, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_0801521c(s);
        if (frame <= 273) {
          Func_080150e4(spin - 0x12400);
        }
        SceneTransform_ApplyPitch(pitch);
        SceneTransform_ApplyYaw(spin);
        Render_TransformVertices(gData_08199210, verts, 64);
        Render_DrawModelList(model);
      }
      if ((u32)(frame - 273) <= 7 && skip == 0) {
        s32 d = frame - 273;
        s32 v;

        work->transferMode = 1;
        v = d * 0x101;
        v = v + (v << 16);
        work->transferValue = 0x3f3f3f3f - (v << 2);
      }
      if (frame == 281 && skip == 0) {
        work->transferMode = 2;
        work->transferValue = 50;
      }
      if ((u32)(frame - 274) <= 27) {
        s = (frame << 11) - 0x85000;
        texture.pixels = gMapCellBuffer;
        model->kind = 7;
        sh = 0;
        if (frame > 293) {
          sh = (294 - frame) << 3;
        }
        model->shade = sh;
        Render_ResetTransformState();
        Func_08015160(0, 0x1c0000, 0);
        Func_0801521c(s);
        SceneTransform_ApplyPitch(4000);
        SceneTransform_ApplyYaw(frame << 9);
        Render_TransformVertices(gData_08199210, verts, 64);
        Render_DrawModelList(model);
      }
      if (frame > 80) {
        for (i = 0; i != 4; i++) {
          s32 st = Data_08199746[i];

          if (frame > st && frame < st + 10) {
            s32 d;

            sh = -16;
            texture.pixels = gMapCellBuffer;
            model->kind = 6;
            model->shade = sh;
            d = frame - st;
            Render_ResetTransformState();
            s = d << 13;
            if (i > 1) {
              ty = -0x2c0000;
              if (frame > 539) {
                ty += (frame - 539) << 20;
              }
              Func_08015160(-0x40000, ty, 0);
              Func_080151e4(s, s, s);
            } else {
              Func_080151e4(s, d << 14, s);
            }
            SceneTransform_ApplyPitch(0x4000);
            SceneTransform_ApplyYaw(frame << 9);
            Render_TransformVertices(gData_08199210, verts, 64);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame > 80) {
        for (i = 0; i != 3; i++) {
          if (frame == Data_0819974e[i]) {
            Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
          }
          if (frame > Data_0819974e[i] && frame < Data_0819974e[i] + 10) {
            s32 d = frame - Data_0819974e[i];
            sh = d * 16 - 56;
            s = 0x14000 - (d << 13);
            texture.pixels = gMapCellBuffer;
            model->kind = 7;
            if (sh > 0) {
              sh = 0;
            }
            model->shade = sh;
            Render_ResetTransformState();
            Func_08015160(-0x40000, -0x2c0000, 0);
            Func_080151e4(s, s << 1, s);
            Func_080150e4((i << 14) + 0x2000);
            SceneTransform_ApplyPitch(-0x1000);
            SceneTransform_ApplyYaw(frame << 9);
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
            sh = -16;
            s = (frame - st) * 2978;
            if (s > 65535) {
              s = 0x10000;
            }
            texture.pixels = work->sheet;
            model->kind = 6;
            model->shade = sh;
            Render_ResetTransformState();
            if (frame > 539) {
              ty = ((frame - 539) << 20) + -0x2c0000;
            }
            Func_08015160(-0x40000, ty, 0);
            Func_080151e4(s, s / 2, s);
            SceneTransform_ApplyPitch(0x4000);
            SceneTransform_ApplyYaw(frame << 11);
            Render_TransformVertices(gData_08199210, verts, 64);
            Render_DrawModelList(model);
          }
        }
      }
      {
        u8 *sheet = work->sheet + 0x4000;

        texture.logWidth = 7;
        texture.logHeight = 4;
        texture.pixels = sheet;
      }
      model->kind = 7;
      model->mesh = Data_02014800;
      model->vertices = verts;
      model->scrollU = 0;
      model->scrollV = 0;
      for (i = 0; i != 2; i++) {
        if (frame < Data_08199756[i]) {
          continue;
        }
        if (frame <= 293) {
          s = Data_08199758[i] + Data_08199760[i] * (frame - Data_08199756[i]);
          if (s > Data_08199764[i]) {
            s = Data_08199764[i];
          }
          if (frame > 209) {
            s -= Data_08199760[i] * t;
          }
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          if (frame > 273) {
            s = s + (frame << 13) + -0x224000;
          }
          Func_0801521c(s);
          if (i == 0) {
            if (frame > 209) {
              s32 v = t;

              if (v > 48) {
                v = 48;
              }
              Func_08015160(0, v << 16, 0);
            }
            Func_080150e4(Data_0819975c[i] - (frame << 8));
            sh = -16;
          } else {
            s32 x = -48;

            if (frame > 127) {
              x = frame - 176;
            }
            if (x > -16) {
              x = -16;
            }
            Func_08015160(x << 16, 0, 0);
            Func_080150e4(Data_0819975c[i] + (frame << 8));
            if (frame > 209) {
              Func_080150e4((frame << 8) + -0xd200);
            }
            sh = ((frame - Data_08199756[i]) << 2) - 64;
            if (sh > -16) {
              sh = -16;
            }
          }
          if (frame > 273) {
            sh = 1080 - (frame << 2);
            if (i == 0) {
              Func_080150e4((274 - frame) << 10);
            } else {
              Func_080150e4((frame << 9) + -0x22400);
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
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_08015160(0, 0x200000, 0);
          Func_0801521c(s);
        }
        if (sh > -64) {
          model->shade = sh;
          SceneTransform_ApplyPitch((frame - i * 82) * 22 - 3000);
          SceneTransform_ApplyYaw(-frame << 10);
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
  Iwram_FillWords((void *)0x06004000, 0x4000, 0x3f3f3f3f);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  for (i = 0; i != 12; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)(BG_PLTT + 80), 0x80000010,
          (volatile u32 *)0x040000d4);
  BG_PLTT[94] = BG_PLTT[244];
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);
  Task_Remove(BattleEffect_ArmPaletteHBlankDma);
  gCameraSceneParameters[4] = 120;
  wait(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 512);
  Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
  Func_08164b2c(0, 0, 11);
  for (frame = 0; frame != 60; frame++) {
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
      Func_08164a4c(-4, -4, -4);
    }
    if (frame > 48) {
      d = frame - 48;
      Func_08164b2c(-d, -d, 11 - d * 2);
    }
    if (frame == 0) {
      work->transferMode = 3;
      work->transferValue = 0x04040404;
      {
        struct EffectStep *p = Ram_EffectSteps;

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
    {
      p = Ram_EffectSteps;

      for (i = 0; i != 256; i++, p++) {
        if (p->variant >= 0) {
          DRAW_SPARK(p);
        }
      }
    }
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          Func_0814cd48(effect->actors[0], 7, -1, 0, 16);
          Func_081180e8(126);
          Sound_PlayFar(126);
        } else {
          Sound_PlayFar(126);
          Func_0814cd48(effect->actors[i], 7, -1, i, 16);
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
    lift = frame * 8 - 128;
    s = (frame << 13) + -0x20000;
    d = frame - 16;
    for (i = 0; i != 3; i++, lift -= 64, s += -0x10000, d -= 8) {
      if (d >= 0) {
        s32 sh2 = 0;

        if (d > 15) {
          sh2 = (16 - d) << 3;
        }
        model2->shade = sh2;
        if (sh2 > -64) {
          Render_ResetTransformState();
          Func_08015160(0, (112 - lift) << 16, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_0801521c(s);
          Func_080150e4((i << 14) + 0x2000);
          SceneTransform_ApplyPitch(-0x1000);
          SceneTransform_ApplyYaw(frame << 10);
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
