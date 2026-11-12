#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "battle_action.h"
#include "battle_unit.h"
#include "bg_scroll.h"
#include "dma.h"
#include "effect_step.h"
#include "io_reg.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "task.h"
#include "transform.h"

// The blitter slots read as what they hold, a drawing routine.
#define ALTERNATE_BLITTER_SLOT                                                 \
  (((struct {                                                                  \
     u8 before[188];                                                           \
     DrawRectangle draw;                                                       \
   } *)Ram_HeapSlots)                                                          \
       ->draw)
#define BLITTER_SLOT                                                           \
  (((struct {                                                                  \
     u8 before[104];                                                           \
     DrawRectangle draw;                                                       \
   } *)Ram_HeapSlots)                                                          \
       ->draw)

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; BattleEffect_LoadBlitter takes mode << 4 |
// flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define SPARK_COUNT 64

typedef DrawRectangle BlitterPair[2];

extern char ResourceId_WaveSheet;
extern char ResourceId_BeamImage;
extern char ResourceId_BlueArcSheetA;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_StarCrescentSheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_YellowOrbSheet;
// The strike's six cels: ☀️'s gBeam_Cells and
// gTargetStrike_Reach, _StrikeWidths and _StrikeHeights.
extern u16 Data_0819886c[];
extern u8 Data_08198878[];
extern u8 Data_08198860[];
extern u8 Data_08198866[];
// Byte offsets of the spark cels in the spark sheet, by size - 1 (☀️'s
// gLuckyWheels_SparkCellOffsets).
extern u16 Data_08197424[];

void wait(s32 frames);
u32 Random16(void);
void *Resource_GetTableEntry(s32 id);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);


void HeapSlot_Release(s32 slot);
void *Unit_Get(s32 owner);
s32 Summon_IsEntrySecondaryFlaggedFar(s32 enemyId);
s32 BattleUnit_GetEffectHeightFar(s32 actor);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void EffectPosition_ProjectScaledBattler(s32 actor,
                                         struct EffectPosition *position);
void EffectPosition_ProjectSprite(s32 actor, struct EffectPosition *position);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void Render_ResetTransformState(void);

void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
// Configures the battle display for an effect.
void BattleEffect_ConfigureEffectDisplay(void);
// Sets up the canvas tile map on BG1.
void Func_08143d80(void);
// Marks the transition done and shows the battle view again.
void BattleEffect_HideBg1AndRestoreBackdrop(void);
// Copies the canvas to VRAM when a transfer is pending.
void BattleEffect_ProcessPendingGraphicsTransfer(void);
// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
// BattlePlayback_Start through its far call.
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

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  kind = effect->kind;
  unit = Unit_Get(effect->actor);

  // Setup: the display, the tile map on BG1, the blitters for the side.
  wait(1);
  BattleEffect_ConfigureEffectDisplay();
  Func_08143d80();
  REG_BG1CNT = 0x1f80;
  wait(1);
  if (kind == 5 || kind == 6) {
    if (effect->side == 0) {
      BattleEffect_LoadBlitter(HEAP_BLITTER,
                               BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP_Y));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP_Y));
    } else {
      BattleEffect_LoadBlitter(HEAP_BLITTER,
                               BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP));
    }
  } else {
    if (effect->side == 0) {
      BattleEffect_LoadBlitter(
          HEAP_BLITTER,
          BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      BattleEffect_LoadBlitter(
          HEAP_BLITTER_ALTERNATE,
          BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    } else {
      BattleEffect_LoadBlitter(HEAP_BLITTER,
                               BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP_X));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP_X));
    }
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = ALTERNATE_BLITTER_SLOT;
  wait(1);

  // The beam's sheet for the kind.
  if (kind == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work->sheet, 1, 1);
  } else if (kind == 3) {
    Resource_LoadAndDecompress((s32)&ResourceId_BeamImage, work->sheet, 0, 0);
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
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer,
                             1, 0);
  // Each transfer leaves half the last frame behind.
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The layer is centred on the first target, kept within 128 pixels.
  REG_BG1CNT = 0x1f81;
  EffectPosition_ProjectSprite(effect->actors[0], &anchor);
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
  wait(1);

  // The sparks leave the middle of the target, away from the caster, each
  // living 16 frames more than the last.
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
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
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0x10000, 0x40000,
                                      0x30000, 150);
      else
        Func_08118088(effect->actors[0], 0);
    }
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
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
          BattleEffectKernels_IntegrateVector3(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  gBgScroll[1].y = 32;
  BattleEffect_HideBg1AndRestoreBackdrop();
}
