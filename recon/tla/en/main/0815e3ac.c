// Draft of Func_0815e3ac, the cast sequence BattleFx_DispatchByIdRange
// runs for kinds below 100 (except 10): a beam strikes the target and
// sparks fly from it.

// On screen: the canvas layer opens over the battle view, centred on the
// first target within 128 pixels. A beam reaches out from beside the
// caster towards the target, its picture chosen by the kind: kind 4 a
// six-cel wave (12 frames), kinds 0-2, 5 and 6 a six-cel arc (12 frames),
// kind 3 a six-cel strike that stretches across (18 frames). An orb
// flashes on the target from frame 4 to 15. At frame 4 the target recoils
// (kind 7 motion 4, kinds 5 and 6 motion 1, kind 8 a long push back, the
// rest motion 0); at frame 5 the battle resumes with hit cue 134 for an
// enemy whose summon entry is flagged, 133 otherwise. From frame 4, 64
// sparks fly out of the middle of the target in pairs, away from the
// caster, shrinking as they go. After 32 frames the layer closes.

// Setup: the battle display and the canvas tile map on BG1, two blitters
// for the caster's side (additive and max; kinds 5 and 6 also flip them
// vertically), the beam's sheet for the kind, the palette of the Mars
// Djinn (an enemy caster) or of the star crescent sheet (a party member)
// by DMA, the spark sheet, the orb sheet, the transfer callback (half the
// last frame left behind), and the sparks at the target.

// Port of The Broken Seal's BattleFx_RunBeamSequence, with The Lost
// Age's heap slots, blitter modes (3 minus ☀️'s) and callback order, no
// work->effect store, and these changes: kind 6 joins kind 5, kind 3 has
// its own test, the beam is drawn additive (☀️ draws it with max), the
// recoil depends on the kind, the summon test reads the enemy id, and the
// palette goes by DMA.

// Listing 687 instructions, draft 687, differ 44 (2026-10-18; 50 before the
// three devices below: the argument's store before the RAM base and both
// BG1CNT halfwords pinned to r3, as in BattleFx_RunUndeadSword). Left: rows
// one place apart (stack and pool loads, constants built around them, the
// call arguments' stores), the usual residue; no allocation difference.

// Tried:
// ☀️'s body with The Lost Age's changes, the beam with
// draw[1] as ☀️ has it                                   210 (695 rows)
// the beam with draw[0], as the listing loads [sp, #56]    210 -> 54
// declaration order, a greedy sweep                        54
// the tables without const, as ☀️'s readers extern theirs 54 -> 50
// the start barrier and the two BG1CNT pins                50 -> 44
// an empty asm between draw[0] and draw[1]                 44

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "battle_session.h"
#include "battle_unit.h"
#include "bg_scroll.h"
#include "dma.h"
#include "effect_step.h"
#include "io_reg.h"
#include "motobj.h"
#include "ram_buffer.h"

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; BattleFx_LoadBlitter takes mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define SPARK_COUNT 64

typedef DrawRectangle BlitterPair[2];

extern char ResourceId_WaveSheet;
extern char ResourceId_BeamSequenceImage;
extern char ResourceId_BlueArcSheetA;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_StarCrescentSheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_YellowOrbSheet;
// The strike's six cels: ☀️'s gBeamSequence_CellSheetOffsets and
// gTargetStrike_Reach, _StrikeWidths and _StrikeHeights.
extern u16 Data_0819886c[];
extern u8 Data_08198878[];
extern u8 Data_08198860[];
extern u8 Data_08198866[];
// Byte offsets of the spark cels in the spark sheet, by size - 1 (☀️'s
// gReelGame_SparkCellOffsets).
extern u16 Data_08197424[];

void WaitFrames(s32 frames);
u32 Random16(void);
void *Resource_GetTableEntry(s32 id);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void *UnitGet(s32 owner);
s32 Summon_IsEntrySecondaryFlaggedFar(s32 enemyId);
s32 Battle_GetObjectTableValueFar(s32 actor);
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void EffectPosition_ApplyStepAndYOffset(s32 actor,
                                        struct EffectPosition *position);
void EffectPosition_ApplyAnimationAndYOffset(s32 actor,
                                             struct EffectPosition *position);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void Render_ResetTransformState(void);
void Transform_SetLookAt(void *camera, void *eye);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
// Configures the battle display for an effect.
void Func_0813ba50(void);
// Sets up the canvas tile map on BG1.
void Func_08143d80(void);
// Marks the transition done and shows the battle view again.
void BattleFx_SetTransitionFlagAndDisplay(void);
// Copies the canvas to VRAM when a transfer is pending.
void BattleFx_ProcessPendingGraphicsTransfer(void);
// Builds a blitter in a heap slot: BattleFx_LoadWork(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleFx_LoadBlitter(s32 slot, s32 modeFlags);
// BattleEventRuntime_BeginPhase through its far call.
void Func_081180e8(s32 hitCue);
// An actor's recoil motion, through a far call.
void Func_08118088(s32 actor, s32 motion);

void Func_0815e3ac(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition anchor;
  struct EffectPosition point;
  struct BattleEffectWork *work;
  void *canvas;
  s32 shake;
  u8 *sheet;
  s32 *camera;
  s32 kind;
  struct BattleUnit *unit;
  BlitterPair draw;
  struct MotionObject *target;
  s32 half;
  s32 frame;
  s32 i;

  /* FAKEMATCH: stores the argument before the 0x03000000 base is started; the
   * native's first instruction after sub sp is str r0 (differ 46 without it).
   */
  __asm__("");
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  kind = effect->kind;
  unit = UnitGet(effect->actor);

  // Setup: the display, the tile map on BG1, the blitters for the side.
  WaitFrames(1);
  Func_0813ba50();
  Func_08143d80();
  {
    /* FAKEMATCH: the native loads the BG1CNT halfword from the pool before it
     * starts the register's address; pinned to r3 in a one-pass block it does
     * (differ 46 without it). */
    register u16 value __asm__("r3");

    do {
      value = 0x1f80;
    } while (0);
    REG_BG1CNT = value;
  }
  WaitFrames(1);
  if (kind == 5 || kind == 6) {
    if (effect->side == 0) {
      BattleFx_LoadBlitter(HEAP_BLITTER,
                    BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                               COMPOSITE_FLIP_Y));
      BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                          COMPOSITE_FLIP_Y));
    } else {
      BattleFx_LoadBlitter(HEAP_BLITTER,
                    BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                               COMPOSITE_FLIP));
      BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                          COMPOSITE_FLIP));
    }
  } else {
    if (effect->side == 0) {
      BattleFx_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_ADDITIVE,
                                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    } else {
      BattleFx_LoadBlitter(HEAP_BLITTER,
                    BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                               COMPOSITE_FLIP_X));
      BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                          COMPOSITE_FLIP_X));
    }
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  WaitFrames(1);

  // The beam's sheet for the kind.
  if (kind == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work->sheet, 1, 1);
  } else if (kind == 3) {
    Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work->sheet,
                               0, 0);
  } else {
    switch (kind) {
    case 0:
    case 1:
    case 5:
    case 6:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1,
                                 1);
      break;
    case 2:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1,
                                 1);
      break;
    }
  }
  // An enemy's sparks take the Mars Djinn's colours, a party member's the
  // star crescent's.
  Dma_Set(Resource_GetTableEntry(effect->actor > 7
                                     ? (s32)&ResourceId_MarsDjinnSmallSheet
                                     : (s32)&ResourceId_StarCrescentSheet),
          (void *)BG_PLTT, 0x84000020, REG_DMA3);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer,
                             1, 0);
  // Each transfer leaves half the last frame behind.
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);

  // The layer is centred on the first target, kept within 128 pixels.
  {
    /* FAKEMATCH: as at the first BG1CNT store (differ 46 without it). */
    register u16 value __asm__("r3");

    do {
      value = 0x1f81;
    } while (0);
    REG_BG1CNT = value;
  }
  EffectPosition_ApplyAnimationAndYOffset(effect->actors[0], &anchor);
  if (effect->side == 0)
    shake = 96 - anchor.x;
  else
    shake = 32 - anchor.x;
  if (shake > 0)
    shake = 0;
  if (shake < -128)
    shake = -128;
  anchor.x += shake;
  gBgScroll[1].x = shake;
  gBgScroll[1].y = 80;
  WaitFrames(1);

  // The sparks leave the middle of the target, away from the caster, each
  // living 16 frames more than the last.
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  half = Battle_GetObjectTableValueFar(effect->actors[0]) / 2;
  for (i = 0; i != SPARK_COUNT; i++) {
    struct EffectStep *spark = &work->particles[i];

    spark->x = target->x;
    spark->y = target->y + half;
    spark->z = target->z;
    spark->velocityX = (Random16() & 255) << 10;
    spark->velocityY = ((Random16() & 255) - 32) << 10;
    spark->velocityZ = ((Random16() & 255) - 127) << 10;
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    // The battle resumes: hit cue 134 for a flagged summon entry, else 133.
    if (frame == 5) {
      if (Summon_IsEntrySecondaryFlaggedFar(unit->enemyId))
        Func_081180e8(134);
      else
        Func_081180e8(133);
    }
    // The target recoils, harder for some kinds.
    if (frame == 4) {
      if (kind == 7)
        Func_08118088(effect->actors[0], 4);
      else if (kind == 5 || kind == 6)
        Func_08118088(effect->actors[0], 1);
      else if (kind == 8)
        BattleFx_StartActorMotion(effect->actors[0], 1, 0x10000, 0x40000,
                                  0x30000, 150);
      else
        Func_08118088(effect->actors[0], 0);
    }
    EffectPosition_ApplyStepAndYOffset(effect->actor, &pos);
    pos.y += 16;
    if (kind == 6)
      pos.y += 16;

    // The beam, from beside the caster towards the target.
    if (kind == 4) {
      // The wave: six cels, two frames each, played backwards.
      if (frame <= 11) {
        if (effect->side == 0)
          draw[0](canvas, work->sheet + (5 - frame / 2) * 0x300,
                  pos.x + shake - 48, pos.y - 8, 48, 16);
        else
          draw[0](canvas, work->sheet + (5 - frame / 2) * 0x300, pos.x + shake,
                  pos.y - 8, 48, 16);
      }
    } else if ((u32)kind <= 2 || kind == 5 || kind == 6) {
      // The arc: six cels, two frames each.
      if (frame <= 11) {
        if (effect->side == 0)
          draw[0](canvas, work->sheet + frame / 2 * 0xd80, pos.x + shake - 48,
                  pos.y - 40, 48, 72);
        else
          draw[0](canvas, work->sheet + frame / 2 * 0xd80, pos.x + shake,
                  pos.y - 40, 48, 72);
      }
    } else if (kind == 3) {
      // The strike: six cels, three frames each, each reaching further.
      if (frame <= 17) {
        s32 cel = frame / 3;

        if (effect->side == 0)
          draw[0](canvas, work->sheet + Data_0819886c[cel],
                  pos.x + Data_08198878[cel] + shake - 58,
                  pos.y - Data_08198866[cel] / 2, Data_08198860[cel],
                  Data_08198866[cel]);
        else
          draw[0](canvas, work->sheet + Data_0819886c[cel],
                  pos.x - Data_08198878[cel] + shake - Data_08198860[cel] + 58,
                  pos.y - Data_08198866[cel] / 2, Data_08198860[cel],
                  Data_08198866[cel]);
      }
    }

    // The orb flashes on the target, six cels, two frames each.
    if (frame >= 4 && frame <= 15)
      draw[0](canvas, Ram_MapCellBuffer + (frame - 4) / 2 * 0x780,
              anchor.x - 16, anchor.y - 24, 40, 48);
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    // From frame 4 the sparks fly, two to each particle, through the two
    // blitters in turn.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 pair = i / 2;
        struct EffectStep *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Render_ProjectPoint(&spark->x, &point.x);
          point.x += shake;
          size >>= 3;
          size += 2;
          draw[pair & 1](canvas, sheet + Data_08197424[size - 1],
                         point.x - size, point.y - size, size * 2, size * 2);
          BattleFxKernels_IntegrateVector3(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Teardown.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  gBgScroll[1].y = 32;
  BattleFx_SetTransitionFlagAndDisplay();
}
