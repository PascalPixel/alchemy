// Matches: differ 0 of 828 (tla-en, 2026-11-09); 0 in ja, en, de, es, fr and it.
// 9 November (round 8), 16 to 0: the preheader's order. PRE inserts its
// hoisted copies at the end of the preheader, after any set already there, so
// pos = &screen in the init always came first. Two tagged dead stores before
// the loop make the preheader compute camera->pos and &texture itself, before
// pos; gcse then copies the loop's occurrences from there and combine folds
// each into its reaching register, which keeps its late slot. &texture is
// computed as (s32)&center - 8 (or &head less 20), because taking texture's own
// address first makes the bolts' logWidth and logHeight stores bytes (94);
// &screen - 32 shares pos's computation and swaps pos and shade's slots (26).
// frame = 0 goes first, so its reload takes r4 as in the listing (74 inside
// the for, 18 after the stores). The store target does not matter (pos,
// vertices, model, ring, fall, bolt, glow, flash: 0; circles 23).
// 9 November (round 7), unchanged at 16. Raw gcse hashes (scratch cc1 printing
// them, analysis only): &texture 13882, &screen 13914, frame << 8 13956,
// camera->pos 13957, table 347 (buckets 2, 34, 76, 77). The preheader's order
// is not bucket order (camera->pos is inserted before &texture, whose bucket
// is earlier), so a table size that wraps camera->pos first (303, 317, 357, 367)
// would not by itself give the listing's order.
// 9 November (round 6), 26 to 16: frame's reload at the pillars' frame <= 47
// test takes the listing's r1. A tagged dead test before it (if (i == 300)
// growth = 0;) builds 300 in a reload register: the one pick reload spent
// there in the listing; its empty branch is gone after reload. On z, x or a
// 0x10000 compare, 40 to 46; on sway, 662. Left: the preheader's RTL order
// (16). pos = &screen as the first statement of the frame loop (for or
// do-while) is still PRE expression 34, inserted before camera->pos (73) and
// &texture (116): 58. A never-true test of camera->pos against &texture before
// the loop computes the three in the listing's order, but their pseudos then
// take other slots and registers (106).
// 9 November: cells as the fixed Ram_MapCellBuffer in place of gMapCellBuffer, 26.
// 2026-11-08 (devices pass), unchanged at 26: an empty asm before, after or between the frame loop's inits (26 to 30), or pos set through an asm before the loop (265, 279), does not reorder the preheader.
// Left: the frame loop's preheader order (16 rows: the listing computes
// camera->pos, &texture, then pos = &screen; here pos comes first) and frame in r4
// against r1 at the pillars' frame <= 47 test, its constant in r1 against r2 (6).
// 2026-11-08 (latest), unchanged at 26. Found in the dumps: gcse PRE puts its
// hoisted copies at the end of the preheader block in expression-number order
// (order of first occurrence), after any set already in that block, so a pos =
// &screen before the loop always comes first; pos's slot (between y and shade)
// says it is a user variable, not a PRE temporary. With pos = &screen set after
// the bolts' setup and no set before the loop (nothing reads pos on frame 0: no
// ring is live and no bolt is past 11), PRE hoists camera->pos, &texture and
// &screen in the listing's order and cprop leaves the ring's reads of pos alone,
// but pos = R stays in the loop (loop.c moves no set that follows the loop's first
// jump and whose register is read earlier in the loop): 417. Tried without gain:
// the frame loop as do-while, while, or for (;;) with a break (26 each); pos set
// in the for condition or the increment (221), in the frame == 0 block (58), as a
// block local at the loop's top (58), before the circle loop (59), first in the
// function (92); no pos, &screen used directly in the ring and the bolts (156);
// at the pillars z and x with the cosine or sine first, z as 0 - ..., growth as a
// ternary or frame < 48, growth and scale at function level or declared first (26
// each). The pillars' code before the frame reload is the listing's, so reload's
// round-robin choice there differs by state the output does not show (a pseudo
// live in r4 and r0 at the jump, or one reload more earlier); not found. A
// 12-minute random permute (9,865 candidates) found nothing below 26.
// Tried 2026-11-08, later: pos declared after texture (26), after cells (44),
// before i (116); pos = &screen before frame in the for init (30), before the
// circles, cells or the target read (53 to 69); growth as frame * 512 - 0x2000
// or with the constant first (26), defaulted first (41), arms swapped (28), frame
// copied to a block local (26), a once-only block before growth or shade (26,
// 28); 5 or 6 unused locals 32, 8 or 10 the same 26.
// The dead read of shade moved to the first argument settles the movs r1, #4
// tie (28 to 26); the two devices, removed, still cost (no locals 32, a plain
// 4 297, out = pos twice 355). Tried 2026-11-08 without gain: pos = &screen
// first in the for init or before it (32), before the circles (61); &screen in
// the ring loop (28 to 333); growth as (frame - 16) << 9 (253), a default-first
// if (43), arms swapped (30).
// Left: the preheader's RTL order (16 rows) and frame in r4 against r1 (6).
// 2026-11-07: camera->pos and frame << 8 are PRE temporaries numbered in
// hash-bucket order (camera+12 hash 150, frame<<8 156 of 345); 7+ unused locals
// after i raise camera's regno so the slots swap (34 to 28, tagged). Left: the
// preheader's RTL order (listing camera+12, &texture, then pos = &screen; here
// pos first, so reload rotates r0..r2 differently: 16 rows), frame in r4
// against r1 at the pillars (6), and the movs r1, #4 tie (2). Moving pos =
// &screen into the loop: 60; reordering declarations instead of dummies: 52+.
// Earlier: a six-minute random permute (3 jobs, 5077 candidates) found nothing
// below 34. Left: camera->pos and frame << 8 swap slots sp+8 and sp+12 (22
// rows), frame in r4 for r1 at the pillars, and the device's movs r1, #4 tie.
// camera is the tree's struct BattleCamera (same 34). Both pointers are gcse
// PRE reaching registers here (536, 537), numbered in hash-bucket order; their
// raw hashes differ by 6 (13950 and 13956), so no table size near 345 reverses
// them and moving the declarations of camera or frame costs 52 to 156. The
// listing's order fits camera->pos not being PRE'd but hoisted by loop.c, which
// takes the frame loop after the pillar loop; what keeps it from PRE is not
// found. Measured 2026-11-05 (tla-en): differ 34 of 828. The ring entry is
// built in index in place, its first read goes through index, and top shares
// index (r5, as the listing; tag temporary on adoption); steps run ring, then
// i, and flash, glow, bolt, then i. Left: camera + 3 at sp+8 under frame << 8
// at sp+12 (22 rows): the listing's slot says its pseudo is later than anything
// in the frame loop's source, as a gcse temporary would be, and 0817bd88 shows
// the same under its length << 5; a second occurrence of camera + 3 is not
// found (the look-at copied into the turn arms: 87; phase locals: 60, 168).
// Then frame in r4 against r1 at the pillars, and one tie on movs r1, #4. The
// dead read of shade stays tagged. Measured 2026-11-05 (tla-en): differ 88: the
// model's mesh and vertices are typed pointers, not void. Measured 2026-11-05
// (tla-en): differ 92 of 828, counts equal: left, the first draw's x, is a
// local (r9). Left: the ring's entry (the listing builds it in r5 from index in
// place), the preheader's three copies. 2026-11-05: differ 121 of 828 (tla-en).
// The dead read of shade is in, giving the frame of 116. Left: the preheader's
// three stack copies and the ring's entry below. Draft, differ 352 of 828,
// skeleton gap 50 (tla-en, 2026-11-01) BattleEffect_RunLightSurge: same calls
// and control flow, draft 824 instructions. Nearly all of what is left comes
// from one cause: the listing keeps shade at sp+24 and the draft keeps it in
// r1, so its frame is 112 bytes where the listing's is 116 and every stack
// offset is off by 4.
//
// The spill is proven, its source is not. Slots run in pseudo order, so shade
// fails global-alloc, which takes a call crossed, and its two reads each follow
// a set in the final code. So at the life pass flow must see one of two things.
// - A read in a later block that combine folds away (stale flow data):
//   Func_08158ce0(4, 4 + ((shade & 255) >> 8)) leaves no code, 116 bytes,
//   differ 121, skeleton gap 25. That form is a device, so it is out.
// - A path through the pillar loop with no set: else if (frame <= 63) on the
//   third arm spills shade too, differ 147, gap 33, but keeps its compare (831
//   instructions). The test is always true there, so it is not the source
//   either. else if (frame > 23) is threaded away before life and does nothing.
// The real form would remove a read or a path after life and before global.
// Tried without gain: a ternary, a dead shade = 0 after the pillars.
//
// Measured on top of the device, so read these with it in place (differ 121):
// - The preheader sets camera + 3, &texture, pos; the draft sets pos first.
//   pos = &screen in the frame loop or before the ring loop lets PRE hoist it
//   (differ 167 to 175): pos is then a PRE temporary, below shade's slot, where
//   the listing has it above. In the bolts' preheader nothing hoists it (488).
// - sp+8 and sp+12 are swapped (camera + 3, frame << 8). (frame + 16) << 8 is
//   worse, 0x1000 + (frame << 8) and a phase local change nothing.
// - The ring's entry: the listing has lsls r5 / adds r5 in place and reads the
//   first *entry through r5 before mov sl, r5, then top goes in r5. Here the
//   shift goes to a temporary, entry is built in r2. index <<= 1 in place adds
//   a reload (differ 159), reading through index too (205). (index >> 1) * 2
//   folds both shifts away, so the shift stays its own statement.
// - sched2: movs r6, #2 and movs r5, #0 sit above GetBattleObjectSlotFar in the
//   listing; moving the statements does not move them.
//
// Forms that closed structure: turn is if/else with the 0x100 arm first (the
// label there is what makes the listing reread frame; with the arms swapped it
// is lost); BattleEffect_LoadBlitter and Task_Add return s32; sprites is read
// before BattleEffect_StartCanvasLayer; x and y are function locals, x first,
// so x wins r11 and y spills to sp+32; cells is one s32 constant, reloaded at
// each use, with offset + cells and *entry + cells in that order; entry is
// &circles[index], top a local of its own; a size local to the ring loop leaves
// the circle loop's size in r6; fall is indexed from i inside the loop, which
// puts its pointer after the hoisted -0x80000; circles is s16 and each draw
// rereads *entry. The bolts' out comes from pos in one branch and &screen in
// the other: with the same copy in both, the last cprop deletes out and r7 goes
// (differ 436 on the device). That mix is odd as C, and the real form may be
// simpler. The loop keeps the twin's i * 2 + 3 (0818cb74).

#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "types.h"

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const u8 *mesh;
  s16 *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);
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

void Func_0815f0a0(s32 palette);

struct SurgeTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

extern char ResourceId_Data0C4;
extern char ResourceId_Data0C9;
extern char ResourceId_Data134;
extern char ResourceId_EffectPalette02;
extern char ResourceId_MercuryDjinnSheet;
extern u8 gMapCellBuffer[];
extern const u8 Data_08199244[];   // a quad's mesh
extern const u8 Data_081991e0[];   // its corners
extern const s32 Data_08198bfc[4]; // per pillar: its size,
extern const s8 Data_08198c0c[4];  // its shade
extern s8 Data_08198c10[4];        // and its offset

#define LIGHT_SURGE_FRAMES 74
#define SURGE_BOLTS 14

// Abilities: Light Surge.
//
// The camera swings around the target while 14 bolts of light rain onto
// it, four frames apart; each lands with a flash and a ring of light (two
// rings of shaded circles), and four pillars of light rise and sway over
// the target.
void BattleEffect_RunLightSurge(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 i;
  /* FAKEMATCH(dead): seven unused locals that raise camera's register
   * number, so gcse's hash puts camera->pos after frame << 8 and their
   * slots come out as the listing's; as no locals, 8 of 828 instructions
   * differ in tla-en. */
  s32 unused1;
  s32 unused2;
  s32 unused3;
  s32 unused4;
  s32 unused5;
  s32 unused6;
  s32 unused7;
  struct BattleCamera *camera;
  s16 savedYaw;
  struct SurgeTarget *target;
  s16 *circles;
  s32 x;
  s32 y;
  struct EffectPosition screen;
  struct EffectPosition *pos;
  s32 head[3];
  struct EffectPosition center;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *fall;
  struct EffectStep *bolt;
  struct EffectStep *ring;
  struct EffectStep *flash;
  struct EffectStep *glow;
  s16 *vertices;
  s32 offset;
  s32 size;
  s32 shade;
  u8 *sprites;
  s32 cells;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C4, work->sheet + 0x1000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x4000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                  128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = camera->yaw;
  target = *GetBattleObjectSlotFar(effect->actors[0]);

  // Shaded circles of 2 to 64 pixels, packed in the map buffer.
  cells = (s32)gMapCellBuffer;
  circles = (s16 *)work->sheet;
  offset = 0;
  size = 2;
  for (i = 0; i != 32; i++) {
    Func_0815b434((u8 *)(offset + cells), size, 0x10000);
    circles[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }

  frame = 0;
  /* FAKEMATCH(dead): pos first takes the camera's look-at point, a store the
   * loop's pos = &screen overwrites: gcse then copies camera->pos from here
   * instead of inserting it after pos, as the listing orders the preheader; as
   * the function without it, 16 of 828 instructions differ in tla-en. */
  pos = (struct EffectPosition *)camera->pos;
  /* FAKEMATCH(dead): the texture's address, computed as center's address
   * less 8 so that texture's own address is still first taken by the bolts
   * (whose bit-field stores then stay one word), stored where vertices is set
   * again before any read: gcse copies it from here, before pos, as the
   * listing orders the preheader; as the function without it, 8 of 828
   * instructions differ in tla-en. */
  vertices = (s16 *)((s32)&center - 8);
  for (pos = &screen; frame != LIGHT_SURGE_FRAMES; frame++) {
    if (frame == 64) {
      Func_081180e8(134);
    }
    if (frame > 31 && (frame & 3) == 0) {
      Func_0815f0a0((s32)&ResourceId_MercuryDjinnSheet);
    }
    if (frame == 0) {
      camera->yaw = savedYaw;
      for (i = 0; i != 32; i++) {
        work->particles[i].variant = -1;
      }
      for (i = 0; i != 32; i++) {
        s32 radius;
        s32 angle;
        s32 z;

        fall = &work->particles[32 + i];
        radius = (Random16() & 63) + 32;
        angle = Random16() & 0xffff;

        fall->x = Trig_Sin(angle) * radius;
        fall->z = Trig_Cos(angle) * radius;
        fall->y = 0x6e0000;
        radius = (Random16() & 31) + 4;
        fall->velocityX = target->x + Trig_Sin(angle) * radius;
        z = target->z + Trig_Cos(angle) * radius;
        fall->velocityZ = z;
        fall->velocityY = -0x80000;
        fall->velocityX = (fall->velocityX - fall->x) / 6;
        fall->velocityY = (-0x80000 - fall->y) / 6;
        fall->velocityZ = (z - fall->z) / 6;
        fall->variant = -i * 4;
      }
    }
    if ((u32)(frame - 20) <= 59) {
      s32 turn;

      if (frame <= 71) {
        turn = 0x100;
      } else {
        turn = 0xa00 - (frame << 5);
      }
      if (effect->side == 0) {
        camera->yaw += turn;
      } else {
        camera->yaw -= turn;
      }
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    // The rings: four quarter circles each, upper halves for the first 16.
    for (i = 0, ring = work->particles; i != 28; ring++, i++) {
      void *cel;
      s32 half;
      s32 index;
      s16 *entry;
      s32 left;
      s32 size;

      if ((u32)ring->variant > 15) {
        continue;
      }
      size = ((Trig_Sin(ring->variant << 11) * ring->velocityX) >> 16) * 2;
      index = size - 2;
      ring->variant++;
      if ((u32)index > 61) {
        continue;
      }
      EffectPosition_ProjectPoint(ring, pos);
      index >>= 1;
      x = pos->x / 2;
      y = pos->y;
      BattleEffect_LoadBlitter(
          188, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      index <<= 1;
      index += (s32)circles;
      half = size >> 1;
      cel = (void *)(*(s16 *)index + cells);
      entry = (s16 *)index;
      left = x - half;
      index = y - size;
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, cel, left, index, half,
                                               size);
      HeapSlot_Release(188);
      BattleEffect_LoadBlitter(188, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP_X));
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (void *)(*entry + cells),
                                               x, index, half, size);
      HeapSlot_Release(188);
      if (i > 15) {
        BattleEffect_LoadBlitter(188, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                            COMPOSITE_CLIP_Y |
                                                            COMPOSITE_FLIP_Y));
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, (void *)(*entry + cells), x - half, y, half, size);
        HeapSlot_Release(188);
        BattleEffect_LoadBlitter(188, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                            COMPOSITE_CLIP_Y |
                                                            COMPOSITE_FLIP));
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, (void *)(*entry + cells), x, y, half, size);
        HeapSlot_Release(188);
      }
    }

    // The bolts: a streak from the sky to the ground, six frames long.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    shade = 0;
    texture.logWidth = 5;
    texture.logHeight = 2;
    model->texture = &texture;
    model->mesh = Data_08199244;
    model->vertices = vertices;
    model->shade = shade;
    model->kind = 7;
    texture.pixels = work->sheet + 0x4000;
    for (i = 0, flash = work->particles, glow = &work->particles[16],
        bolt = &work->particles[32];
         i != SURGE_BOLTS; flash++, glow++, bolt++, i++) {
      s32 t = ++bolt->variant;
      struct EffectPosition *out;

      if ((u32)t > 17) {
        continue;
      }
      if (t > 11) {
        head[0] = bolt->x + bolt->velocityX * (t - 12);
        head[1] = bolt->y + bolt->velocityY * (bolt->variant - 12);
        head[2] = bolt->z + bolt->velocityZ * (bolt->variant - 12);
        if (bolt->variant == 16) {
          glow->x = head[0];
          glow->y = head[1];
          glow->z = head[2];
          glow->velocityX = (Random16() & 15) + 8;
          glow->variant = 0;
        }
        out = pos;
        EffectPosition_ProjectPoint((struct EffectStep *)head, out);
      } else {
        out = &screen;
        EffectPosition_ProjectPoint(bolt, out);
      }
      vertices[0] = out->x / 2 - 2;
      vertices[1] = out->y;
      vertices[4] = out->x / 2 + 2;
      vertices[5] = out->y;
      if (bolt->variant <= 5) {
        head[0] = bolt->x + bolt->velocityX * bolt->variant;
        head[1] = bolt->y + bolt->velocityY * bolt->variant;
        head[2] = bolt->z + bolt->velocityZ * bolt->variant;
      } else {
        head[0] = bolt->x + bolt->velocityX * 6;
        head[1] = bolt->y + bolt->velocityY * 6;
        head[2] = bolt->z + bolt->velocityZ * 6;
      }
      if (bolt->variant == 6) {
        flash->x = head[0];
        flash->y = head[1];
        flash->z = head[2];
        flash->velocityX = (Random16() & 7) + 24;
        flash->variant = 0;
        work->shakeFrames = 4;
        Sound_PlayFar(145);
        Func_08118088(effect->actors[0], 0);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 2);
      }
      EffectPosition_ProjectPoint((struct EffectStep *)head, out);
      vertices[8] = out->x / 2 - 2;
      vertices[9] = out->y;
      vertices[12] = out->x / 2 + 2;
      vertices[13] = out->y;
      Render_DrawModelList(model);
    }

    // The pillars.
    if ((u32)(frame - 16) <= 63) {
      EffectPosition_ProjectBattler(effect->actors[0], &center);
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = work->sheet + 0x1000;
      model->kind = 6;
      model->mesh = Data_08199244;
      for (i = 0; i != 4; i++) {
        s32 sway = Trig_Sin((frame << 8) + 0x1000);
        s32 x =
            ((center.x / 2 - 64) << 16) -
            (Data_08198c10[i] * sway / 0x10000) * Trig_Sin((s16)camera->yaw);
        s32 z =
            -((Data_08198c10[i] * sway / 0x10000) * Trig_Cos((s16)camera->yaw));
        s32 growth;
        u32 scale;

        /* FAKEMATCH(dead): a store both arms below overwrite; its test builds
         * 300 in a reload register, the one pick before frame's reload that
         * sends it to r1; without it, 10 of 828 instructions differ in
         * tla-en. */
        if (i == 300)
          growth = 0;
        if (frame <= 47) {
          growth = Trig_Sin((frame << 9) - 0x2000);
        } else {
          growth = 0x10000;
        }
        scale = (u32)(Data_08198bfc[i] * growth) >> 16;
        if (frame > 63) {
          shade = Data_08198c0c[i] - frame * 8 + 512;
        } else if (frame <= 23) {
          shade = Data_08198c0c[i] + frame * 8 - 192;
        } else {
          shade = Data_08198c0c[i];
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(x, z, 0);
        Func_080151e4(scale, scale * 2, scale);
        Render_TransformVertices(Data_081991e0, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    /* FAKEMATCH(dead): a read of shade the compiler folds to nothing, which
     * spills shade to the listing's slot; as a plain 4, 283 of 828 instructions
     * differ in tla-en. */
    Func_08158ce0(4 + ((shade & 255) >> 8), 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
