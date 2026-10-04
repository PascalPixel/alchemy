/* 2026-10-04 (main-5): the sp+8/sp+12 exchange is GCSE's PRE order. PRE
 * makes the frame loop's reaching registers for &ground (plus sfp -12) and
 * camera->pos (plus camera 12) in expression-hash bucket order, and their
 * spill slots follow those register numbers. This source reaches GCSE with
 * 392-393 real insns, a 197-bucket table: ground hashes to 124, camera to
 * 160. With 396-399 insns (199 buckets) camera comes first and the whole
 * listing matches: two extra discarded calls left only their own four
 * instructions as differences. So the ROM's source had three or four more
 * insns at GCSE that later passes remove. Not this: one lookup after the
 * palette switch (193 buckets), a fleck pointer in the init loop or a named
 * unused slot result (197). Look for the missing early insns, not orders.
 */
/* 2026-10-04: scoped read/write register-operand trials on initialized work,
 * camera, sheet and canvas at four existing boundaries did not improve 9.
 * Explicit consumed ground/camera pointers, both declaration/assignment
 * orders and read-only/read-write operands before/inside the frame loop,
 * also did not improve 9 (best 51). No trial device was retained.
 */
/* 2026-10-03 T4: independent coordinate-storage and lifetime pass, best 9.
 * The retained T3 source is unchanged. Rejected trials:
 * - One record holding point, projected position and ground: 1110; frame
 *   grows from 76 to 80 bytes and adds a shared base-address live range.
 * - A record holding only point and projected position: 259; the same
 *   ground/camera pointer spill exchange remains.
 * - A one-element ground array, all coordinates scoped around the frame
 *   loop, projected coordinates scoped in the fleck branch, and ground
 *   scoped inside the frame loop: all 9, with identical pointer slots.
 * - Frame < 56 instead of != 56: 229; changes loop termination code.
 * - Separate upper/lower callbacks: 1154 and a 72-byte frame. Reversing
 *   the used callback-pair fields: 13, adding four callback stack changes.
 * - A do/while block around transfer setup: 129, adding two scheduling
 *   differences. An inline helper containing the same setup: 9.
 * No new device or storage was retained. Further work needs a distinct
 * source model; coordinate grouping alone does not solve the spill order.
 */
/* 2026-10-03 T3: score 89 -> 9. All instruction/register differences are
 * resolved; nine stack operands still exchange the ground-position pointer
 * at sp+8 and camera-position pointer at sp+12. No inserted/deleted
 * instructions. Still an uncredited draft; no edition/build claim.
 *
 * A read-only empty work operand before transfer setup changes the work
 * allocation from r11 to r9 and the picture quotient from r9 to r11.
 * This is one measured register-priority device, after the ordinary
 * structural search recorded below failed. It adds no instructions.
 *
 * Bounded trials, rejected unless noted:
 * - Work read-only/read-write boundaries at seven existing statements:
 *   best 9 (read-only before transfer setup retained); other scores
 *   69, 119, 229, 2026. No final result uses a read-write boundary.
 * - Explicit camera/ground pointer locals at six setup positions:
 *   best 38; camera-only 72..708; ground-only 38..968; camera plus
 *   reversed position declarations 73..729. No pointer local retained.
 * - Read-only uses of either position address at six boundaries:
 *   ground 1325..2196; camera 129..458. No extra address use retained.
 * - Read/write or read-only memory operands at four boundaries:
 *   ground 1325..2237; camera 364..1324. No memory operand retained.
 * - All 28 scalar/declaration pairs: 9..25. Then all 5040 orders of
 *   work, canvas, sheet, camera, palette, frame, i (draw stays in its
 *   original slot): 480 at 9; 480 at 15; 120 at 16; 960 at 18;
 *   360 at 19; 960 at 21; 600 at 22; 1080 at 25. Original order retained.
 * - A used two-pointer record, both field/assignment orders and three
 *   declaration positions: 185 or 190. No record retained.
 * The general permuter rejects inline assembly, so no search was run
 * through it on the retained source. Stop this allocation axis here.
 */
#include "CANVAS.H"
#include "RUNTIME_MEM.H"
/* Draft, same instructions, two swaps left: the ROM keeps the work block in
   r9 and the pillar picture quotient in r11 (their allocation priorities are
   0.197 and 0.200 here, one reference apart), and it spills the camera
   position pointer above the ground position pointer, not below.
   2026-10-02: 14,372 source permutations left score 89 (16 register-only,
   9 stack-only). Pinning work to r9 scored 5325; an r11 clobber just after
   allocation scored 189, after camera setup 409. Naming the camera's pos
   pointer scored 152 and moved the effect pointer's spill too. None kept. */
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

extern DrawRectangle gWorkSlot[];
extern u16 BattleFx6_FlareCells[];

#define gFlecks ((struct EffectStep *)Ram_MapCellBuffer)

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);

struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

/* Battle effect: a pillar rises over the actor in two mirrored halves while
   flecks spiral up around it. The effect brings its own work block, canvas
   and sheet. */
void Unnamed_080cb7f8(struct BattleEffectArgument *effect)
{
    struct EffectPosition ground;
    struct EffectPosition position;
    s32 point[3];
    struct BattleEffectWork *work;
    void *canvas;
    struct BlitterPair draw;
    u8 *sheet;
    struct BattleCamera *camera;
    void *palette;
    s32 frame;
    s32 i;

    work = (struct BattleEffectWork *)Runtime_AllocateHeapBlock(39, sizeof *work);
    canvas = (void *)Runtime_AllocateHeapBlock(40, 0x4000);
    sheet = (u8 *)Runtime_AllocateHeapBlock(41, 0x60e);
    camera = gCameraWork;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    work->fade_frames = 24;
    work->fade_step = 0;
    *(volatile u16 *)0x04000052 = 0x100c;
    *(volatile u16 *)0x04000020 = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
    switch (work->effect->kind) {
    case 0:
        palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
        break;
    case 1:
        palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
        break;
    case 2:
        palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
        break;
    default:
        palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
        break;
    }
    { s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords; copy((void *)0x05000000, palette, 128); }

    for (i = 0; i != 128; i++) {
        gFlecks[i].y = 0;
        gFlecks[i].x = Random16() & 0xffff;
        gFlecks[i].z = (Random16() & 0x1ff) + i * 2;
        gFlecks[i].variant = -i;
    }
    /* FAKEMATCH: read-only work use exchanges r11/r9, fixing 16 register differences. */
    __asm__ ("" : : "r" (work));
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    draw.upper = gWorkSlot[46];
    work->effect->variant++;
    if (work->effect->variant <= 0)
        work->effect->variant = 1;
    if (work->effect->variant > 4)
        work->effect->variant = 4;
    AudioCommand_PlayFar(212);

    for (frame = 0; frame != 56; frame++) {
        EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &ground);
        *(volatile s32 *)0x04000028 = (64 - ground.x) << 8;
        if (frame > 49)
            *(volatile u16 *)0x04000052 = (0x70 - frame * 2) | 0x1000;
        if (frame == 16)
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 20);
        if (frame <= 55) {
            s32 n;
            u8 *glow;

            i = frame / 2;
            n = i % 4;
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            draw.lower = ((DrawRectangle *)Ram_WorkSlot)[47];
            draw.lower(canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
            n = frame / 4 % 3;
            glow = work->sheet + n * 1032 + 0x1100;
            draw.lower(canvas, glow, 40, ground.y - 36, 24, 43);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            draw.lower = ((DrawRectangle *)Ram_WorkSlot)[47];
            n = i % 4;
            draw.lower(canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
            draw.lower(canvas, glow, 64, ground.y - 36, 24, 43);
            Runtime_ReleaseHeapBlock(47);
        }
        GetBattleObjectSlotFar(work->effect->actor);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        for (i = 0; i != 32; i++) {
            struct EffectStep *fleck = &gFlecks[i];

            if (fleck->variant >= 0) {
                s32 size;

                point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
                point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
                point[1] = fleck->y;
                fleck->x += 0x400;
                fleck->y += 0x50000;
                fleck->z += 64;
                EffectPosition_ApplyBaseAndYOffset(point, &position);
                position.x /= 2;
                size = (i & 1) + work->effect->variant;
                draw.upper(canvas, sheet + BattleFx6_FlareCells[size - 1],
                    position.x - size, position.y - size, size * 2, size * 2);
            }
            fleck->variant++;
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
    Runtime_ReleaseHeapBlock(41);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
}
