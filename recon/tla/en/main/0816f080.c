// Differ 112 of 568 (tla-en, 2026-11-06).
// Fourth round, 6 November: the r6/r7 swap is gone (146 -> 112). row is now the plain
// row = y * 32, and the first pair's second store spells x * 2 as x + x. With x + x, CSE
// cannot share row + x * 2 between the two stores, so row has two uses, a loop.c lifetime
// of 6 instead of 2, and is moved in the first loop pass as a single set; simplify_giv_expr
// then looks through it, so the first pair's pointer init computes y << 5 itself, after the
// second pair's. That puts one more insn between the second pair's pointer (now 40 insns
// live) and the third pair's +1 pointer (31), which then ranks first and takes r6 as in the
// listing. Every other spelling of that store measured (x * 2 + 1 grouped or reordered,
// row + 1, 0x1001 +, x + x in both stores) stays at 350 or 378.
// Left: the splash scale (below, Left 2), which also shifts every literal pool load by 4,
// and the zero stores' scheduling (Left 3).
// Third round, 6 November, none better. The r6/r7 swap is global's order: giv 421 (the second
// pair's decremented pointer, 14 refs over 39 insns) outranks giv 463 (the third pair's +1
// pointer, 11 over 31), takes r6, and reload's spill set then takes r7 from 463 at insn 1747
// (464 = mirrorRow + 0x1701). 463 first needs 421 at a live length of 40 or 463 at 30.
// Measured: mirrorRow pinned to r6 (440) or r7 (528), both stop its hoisting; the +1 mirror
// pointer hand-stepped and pinned to r6 (442); an empty asm before the row stores, the mirror
// stores, inside the if or after it (146 each); splash = 25000 through an output asm before
// the frame loop, halved with / 2 (205). 6912 PERM combinations of x, y, v, row and mirrorRow
// types and of the second pair's and the +1 mirror store's spellings: none below 146; 12
// minutes of random permuter (seed 5222): none.
// Second round, 6 November, none better: 40 minutes of permuter (seed 7104);
// splash = 25000 through an empty asm with an output, before the splash loop
// (225: the halving is hoisted with it) or inside its if (225: loop.c moves the
// asm and the halving). The listing's asrs alone, with no sign fix, says the
// halving is a / 2 whose operand combine knows is not negative, so the 25000 is
// a pseudo neither CSE nor GCSE's cprop sees as a constant; no form found here.
// Also tried 6 November: splash = 25000 at width = 32, or as width * 781, halved
// with >> 1 (146); reset after the call as well (220); 5 minutes of permuter: a
// reused zero temporary 144, nothing plain better. The second and third glow
// pairs swap r6 and r7, the splash holds 12500 where the native holds 25000, and the
// model and glow zero stores schedule differently. Tried today, none better: a
// splash local set to 25000 at the splash loop top, the if top, before the frame
// loop or before Render_ResetTransformState (146: 12500 still hoisted); set after
// Func_08015160 (231: nothing hoisted, frame moves to r11); model->shade = 0
// last (150); glow->variant = 0 before glow->y (146); (size - 1 - x) and
// (size - (x + 1)) spellings (146); (offset + size - x - 1) * 2 (386).
// Measured 2026-11-05 (tla-en): 146 of 568, unchanged. 216 spellings of the glow's stores and
// 80 loop, row and mirror forms: none better; 4000 random declaration orders: no effect.
// 2026-11-05, unchanged at 146 of 568: a scale local inside the splash loop, with
// its half shifted, does hold 25000 (draft 563) but moves the rest to 231.
// Draft, differ 146 of 568, skeleton gap 23 (tla-en, 2026-11-01)
// Everything but a 25000 and one register swap agrees; the frame, the 47 calls
// and every stack slot match. What each glow form below is for:
// - y's loop is label and goto: the native rebuilds the 0x1700-style constants
//   per row, y * 32 and the mirror row from y, and steps offset by hand with
//   add lr, r11. Any real loop hoists or reduces all of those.
// - width is set before the palette loop so size and half are chains only
//   cse2 folds. A constant size or half is const-propagated by gcse, which
//   folds the x loop's top test and turns offset += size into add #16.
// - (until the fourth round) row was built in two sets inside the if: as
//   "row = y * 32" with x * 2 in both stores it is too short lived to be moved
//   by the first loop pass and the first pair stays two pointers.
// - mirrorRow is two sets so that -32 stays in it, ahead of the 0x1700 offsets.
//   As one expression fold moves the -32 last.
//
// Left, 1 (settled in the fourth round, kept for its evidence): the native gives the third pair's second pointer r6 and the second
// pair's r7; here it is the other way, and the spill registers then use r7
// where the native uses r6, which is most of the 146. The second pair's
// priority is 42 / 39 against 33 / 31, so it needs a live length of 40. The
// listing's order says how: size * 2 is computed after the first pair's start,
// so it sits inside that range, while here giv 409, (size - x - 1) * 2, is
// reduced in the first pass and its start comes first. Raising the loop to 49
// insns did not stop that reduction. Tried: mirror, column, back, source and
// rows locals; other spellings of the second and fourth pairs (all no change
// or worse); glow declaration orders (no effect).
// Left, 2: the splash scale. The native holds 25000 in r9 and halves it with
// asr in the loop. Here 12500 is hoisted and 25000 rebuilt. Any local holding
// it is const-propagated. A chain from an earlier-block constant (splashFull =
// 50000, as width does for size) stops that, but the halving is then hoisted
// too and combine folds it (differ 171), and 50000 has no evidence.
// Left, 3: scheduling at glow->variant = 0 and model->shade = 0, where the
// native loads the zero into r4 earlier. model->shade = 0 first gained 4.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
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
void BattleEffect_LoadBlitter(s32 slot,
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
void Actor_SetAnimationSpeedFar(void *object, s32 value);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

extern char ResourceId_Data134;
extern char ResourceId_Data0D8;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const s8 Data_08198bea[];   // the drops' bounce speeds
extern const u8 Data_08199244[];   // the glow's and the splashes' mesh
extern const u8 gData_08199210[];  // its four corners
extern u32 gCameraSceneParameters[];

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)
#define HI(v) (((s16 *)&(v))[1])
#define WYRD_FRAMES 120
#define WYRD_DROPS 16

// Abilities: Wyrd Curse.
//
// A 32x32 glow is mirrored from a shaded disc, and BG palette 0 becomes a
// dark ramp. Sixteen drops form over the target; from frame 0 one falls
// every eight frames, and on reaching the ground it bounces away and adds
// its weight to the glow (the last particle), which grows over the target
// and fades from frame 64. From frame 64 six splashes, four frames apart,
// rise from the drops, and from frame 80 the target is struck six times.
void BattleEffect_RunWyrdCurse(struct BattleEffectArgument *effect) {
  struct ModelTexture texture;
  struct EffectPosition screen;
  void *canvas;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  void *caster;
  struct BattleEffectWork *work;
  s32 i;
  struct EffectStep *drop;
  struct EffectStep *last;
  u16 *palette;
  s32 size;
  s32 width;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
  } else {
    BattleEffect_LoadBlitter(104,
                             BLIT_MODE(BLIT_MAX, BLIT_CLIP | COMPOSITE_FLIP_X));
  }
  draw[0] = BLITTER;
  work->transferMode = 2;
  work->transferValue = 50;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  width = 32;
  palette = (u16 *)BG_PLTT + 1;
  caster = *GetBattleObjectSlotFar(effect->actor);
  Iwram_ClearWords((void *)BG_PLTT, 128);
  for (i = 0; i != 63; i++) {
    *palette++ = i / 3 + 8;
  }

  // The glow: a 16-pixel disc mirrored into the four quarters of 32x32.
  {
    s32 y;
    s32 x;
    s32 half;
    s32 offset;

    size = width / 2;
    half = size / 2;
    Iwram_ClearWords(work->sheet + 0x1000, 0x800);
    Func_0815b434(work->sheet, size, 0x10000);
    y = 0;
    offset = 0;
  nextRow:
    for (x = 0; x != half; x++) {
      u8 v = work->sheet[offset / 2 + x];

      if (v != 0) {
        s32 mirrorRow;
        s32 row;

        row = y * 32;
        work->sheet[0x1000 + row + x * 2] = v;
        // x + x: the same byte as x * 2 + 1; see the header.
        work->sheet[0x1000 + row + x + x + 1] = v;
        work->sheet[0x1000 + (offset + (size - x - 1)) * 2] = v;
        work->sheet[0x1000 + (offset + (size - x - 1)) * 2 + 1] = v;
        mirrorRow = (32 - y) * 32;
        mirrorRow -= 32;
        work->sheet[0x1000 + mirrorRow + x * 2] = v;
        work->sheet[0x1000 + mirrorRow + x * 2 + 1] = v;
        work->sheet[0x1000 + mirrorRow + (size - x - 1) * 2] = v;
        work->sheet[0x1000 + mirrorRow + (size - x - 1) * 2 + 1] = v;
      }
    }
    y++;
    offset += size;
    if (y != size) {
      goto nextRow;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0D8, work->sheet + 0x4000, 0,
                             0);

  for (frame = 0; frame != WYRD_FRAMES; frame++) {
    struct RenderModel *model;
    void *vertices;

    s32 level;

    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actors[0], &screen);
      for (i = 0, drop = work->particles; i != WYRD_DROPS; i++, drop++) {
        drop->x = (screen.x / 2 + (Random16() & 15) - 8) << 16;
        drop->y = -8;
        drop->velocityX = 0;
        drop->velocityY = 0;
        drop->z = (Random16() & 3) + 4;
        drop->velocityZ = 160 - i * 16;
        drop->variant = 0;
      }
      {
        struct EffectStep *glow = &work->particles[63];

        glow->x = (screen.x / 2) << 16;
        glow->y = 0x700000;
        glow->variant = 0;
      }
    }

    // The falling drops, one more every eight frames.
    if (frame <= 63) {
      for (i = 0, drop = work->particles; i != 6; i++, drop++) {
        if (frame > i * 8 && drop->variant == 0) {
          s32 dropSize = drop->z;

          draw[0](canvas, sprites + gData_08197410[dropSize - 1],
                  HI(drop->x) - dropSize / 2, HI(drop->y) - dropSize, dropSize,
                  dropSize * 2);
          BattleEffectKernels_IntegrateVector2(drop, 64, 0x8000);
          if (HI(drop->y) > 111) {
            drop->variant = 1;
            drop->velocityX = ((Random16() & 31) - 16) << 12;
            drop->velocityX = Data_08198bea[i % 4] << 13;
            drop->velocityY = 0;
          }
        }
      }
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    texture.logHeight = 5;
    texture.pixels = work->sheet + 0x1000;
    model->shade = 0;
    model->kind = 6;
    model->mesh = Data_08199244;
    model->texture = &texture;
    model->vertices = vertices;

    // The glow gathers what the landed drops bring.
    {
      struct EffectStep *glow = &work->particles[63];
      struct EffectStep *landed = work->particles;

      for (i = 0; i != 8; i++, landed++) {
        if (landed->variant > 0) {
          glow->variant += landed->velocityZ;
          landed->velocityZ /= 2;
        }
      }
    }
    last = &work->particles[63];
    level = last->variant;
    if (level > 0) {
      s32 scale = (level << 13) / 824;

      if (frame > 63) {
        last->variant = level - 24;
      }
      Render_ResetTransformState();
      Func_08015160(last->x - 0x400000, last->y - 0x400000, 0);
      Func_080151e4(scale * 2, scale, scale * 2);
      SceneTransform_ApplyPitch(-0x2000);
      Render_TransformVertices(gData_08199210, vertices, 4);
      Render_DrawModelList(model);
    }

    // The splashes, from frame 64.
    texture.logWidth = 5;
    texture.logHeight = 5;
    model->kind = 7;
    model->mesh = Data_08199244;
    for (i = 0, drop = work->particles; i != 6; i++, drop++) {
      if (frame > i * 4 + 64) {
        model->shade = 0;
        texture.pixels = work->sheet + 0x4000 + ((i % 2) << 10);
        Render_ResetTransformState();
        Func_08015160(drop->x - 0x400000, drop->y - 0x300000, 0);
        Func_080151e4(12500, 25000, 12500);
        SceneTransform_ApplyPitch(-0x4000);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
        BattleEffectKernels_IntegrateVector2(drop, 60, -0x8000);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);

    if (frame == 64) {
      Sound_PlayFar(147);
    }
    for (i = 0; i != 6; i++) {
      if (frame == i * 4 + 80) {
        work->shakeFrames = 2;
        Func_08118088(effect->actors[0], 0);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 2);
        if (i == 0) {
          Func_081180e8(134);
        } else {
          Sound_PlayFar(133);
        }
      }
    }
    Func_08158ce0(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Actor_SetAnimationSpeedFar(caster, 16);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
