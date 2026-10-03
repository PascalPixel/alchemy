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
