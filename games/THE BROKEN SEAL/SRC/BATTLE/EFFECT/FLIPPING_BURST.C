#include "CANVAS.H"
#include "RUNTIME_MEM.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "RESOURCE_IDS.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
s32 Battle_GetObjectTableValueFar(s32 member_id);
void Camera_ApplyShake(s32 x, s32 y);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);

/* Battle effect: the actor dashes 90% of the way to the first affected unit;
   both hop (frame 0), are raised by their object-table offsets and flipped
   at frame 11, and the actor springs back at frame 54 while the flash burst
   sheet is drawn over it; from frame 56 sixteen burst cells ring the actor
   on a widening circle for twelve frames, and at frame 64 the offsets and
   flips are undone. The screen shakes throughout. */
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    /* FAKEMATCH: an unused second blitter word; the native frame keeps an 8-byte draw slot (sp+48..55, the upper word never accessed), 84 bytes against 80 without it. */
    DrawRectangleFn draw[2];
    struct BattleObjectSlot *first;
    struct BattleObjectSlot *second;
    struct MotionObject *object;
    struct MotionObject *target;
    s32 x;
    s32 z;
    s32 lift_actor;
    s32 lift_member;
    s32 record[3];
    struct EffectPosition screen;
    s32 frame;
    s32 scale;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    draw[0] = heap_cache[7];
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    first = GetBattleObjectSlotFar(work->effect->actor);
    second = GetBattleObjectSlotFar(work->effect->actors[0]);
    object = first->object;
    target = second->object;
    scale = 90;
    x = object->x + scale * (target->x - object->x) / 100;
    z = object->z + scale * (target->z - object->z) / 100;
    lift_actor = Battle_GetObjectTableValueFar(work->effect->actor);
    lift_member = Battle_GetObjectTableValueFar(work->effect->actors[0]);
    Object_ResetMotion(object);
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);
    object->snap_to_target = 1;
    object->auto_face_motion = 1;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    WaitFrames(20);
    for (frame = 0; frame != 96; frame++) {
        struct BattleCamera *camera;

        camera = gCameraWork;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        if (frame == 0) {
            target->velocity_y = 0xf0000;
            target->vertical_motion_strength = 0x91eb;
            object->velocity_y = 0xf0000;
            object->vertical_motion_strength = 0x91eb;
        }
        if (frame == 11) {
            target->scale_y = -target->scale_y;
            object->scale_y = -object->scale_y;
            object->y += lift_actor;
            target->y += lift_member;
        }
        if (frame == 54) {
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 10);
            target->velocity_y = 0x80000;
            target->vertical_motion_strength = 0xab85;
            object->velocity_y = 0x50000;
            object->vertical_motion_strength = 0x7851;
            object->acceleration = 0x10000;
            object->speed_limit = 0x20000;
            object->auto_face_motion = 0;
            Object_ResetMotion(object);
            Object_SetPosition(object, 0, 0, object->z);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        record[0] = object->x;
        record[1] = object->y;
        record[2] = object->z;
        EffectPosition_ApplyBaseAndYOffset(record, &screen);
        screen.x >>= 1;
        if (frame == 54 || frame == 55)
            draw[0](canvas, work, screen.x - 16, screen.y - 16, 32, 64);
        if (frame >= 56 && frame <= 67) {
            s32 i;

            for (i = 0; i != 16; i++) {
                s32 angle;
                s32 rx;
                s32 ry;

                angle = i << 12;
                rx = screen.x + ((Trig_Sin(angle) * (frame - 46)) >> 16);
                ry = ((Trig_Cos(angle) * (frame - 46)) >> 16) - frame + 100;
                draw[0](canvas, (u8 *)work + (((frame - 56) / 2) << 11), rx - 16, ry, 32, 64);
            }
        }
        if (frame == 64) {
            target->scale_y = -target->scale_y;
            object->scale_y = -object->scale_y;
            object->y -= lift_actor;
            target->y -= lift_member;
            Object_SetMode(object, 0);
        }
        if (frame == 54)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == 0) {
            Audio_PlayCue(136);
            work->shake_frames = 6;
        }
        if (frame == 53)
            work->shake_frames = 6;
        Camera_ApplyShake(16, 16);
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
s32 Battle_GetObjectTableValueFar(s32 member_id);
void Camera_ApplyShake(s32 x, s32 y);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);

void ObjectGroup_TickMemberTimers(void);

/* Per drift step 0..3: a blade cell's width and height and its offset in
   the sword slash sheet. */
extern const u8 BladeRain_CellWidths[];
extern const u8 BladeRain_CellHeights[];
extern const u16 BladeRain_CellSourceOffsets[];

/* Battle effect: a rain of blades. Sixty-four blades are seeded at random
   columns with a sideways drift set by the column; each of 120 frames draws
   sixteen of them, falling once their turn comes and then shrinking, while
   the screen shakes and the first target is hit every fourth frame from
   frame 23 to 87. A blade drifting left is drawn by the second blitter. */
void BattleFx_RunBladeRain(struct BattleEffectArgument *efx)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle *blit;
    struct EffectStep *blade;
    s32 frame;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)0x04000052 = 0x1000;
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)heap_cache[46 - 39];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)heap_cache[47 - 39];
    blit = draw;
    Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
    *(s32 *)0x04000028 = (64 - position.x) << 8;
    for (i = 0; i != 64; i++) {
        s32 column;

        blade = &work->particles[i];
        column = Random16() % 96 + 16;
        blade->x = column;
        blade->y = (24 - i / 4) << 16;
        if (column <= 43)
            blade->velocity_x = 3;
        else if (column <= 51)
            blade->velocity_x = 2;
        else if (column <= 59)
            blade->velocity_x = 1;
        else if (column <= 67)
            blade->velocity_x = 0;
        else if (column <= 75)
            blade->velocity_x = -1;
        else if (column <= 83)
            blade->velocity_x = -2;
        else
            blade->velocity_x = -3;
        blade->velocity_x <<= 17;
        blade->velocity_y = 0x80000;
        blade->x <<= 16;
    }
    Audio_PlayCue(212);
    for (frame = 0; frame != 120; frame++) {
        if (frame <= 16) {
            *(u16 *)0x04000052 = frame | 0x1000;
            if (frame == 16)
                *(u16 *)0x04000050 = 0;
        }
        if (frame > 103) {
            *(u16 *)0x04000052 = (0x78 - frame) | 0x1000;
            if (frame == 104)
                *(u16 *)0x04000050 = 0x3f44;
        }
        for (i = 15; i != -1; i--) {
            /* The blitter index, drift >> 31, is computed before the call's
               operands, so the allocator ranks the drift above the width and
               height and rotates their three registers. */
            /* FAKEMATCH: the drift is pinned to r6, as the reference keeps it. */
            register s32 drift __asm__("r6");
            s32 speed;
            s32 cell;
            u32 wide;
            u32 high;

            blade = &work->particles[i];
            drift = blade->velocity_x;
            speed = drift;
            if (drift < 0)
                speed = -drift;
            cell = speed >> 17;
            if (frame < i * 4 + 25) {
                blit[(u32)drift >> 31](canvas, (u8 *)work + BladeRain_CellSourceOffsets[cell],
                    (blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
                    (blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1),
                    wide, high);
                if (frame >= i * 4 + 16) {
                    blade->x += blade->velocity_x;
                    blade->y += blade->velocity_y;
                }
            } else {
                blit[(u32)drift >> 31](canvas, (u8 *)work + BladeRain_CellSourceOffsets[cell],
                    (blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
                    (blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1),
                    wide, high -= 4);
            }
        }
        if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 2);
            work->shake_frames = 1;
            if ((frame & 7) == 0)
                Audio_PlayCue(133);
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

/* Draft, same instructions, two swaps left: the ROM keeps the work block in
   r9 and the pillar picture quotient in r11 (their allocation priorities are
   0.197 and 0.200 here, one reference apart), and it spills the camera
   position pointer above the ground position pointer, not below.
   2026-10-02: 14,372 source permutations left score 89 (16 register-only,
   9 stack-only). Pinning work to r9 scored 5325; an r11 clobber just after
   allocation scored 189, after camera setup 409. Naming the camera's pos
   pointer scored 152 and moved the effect pointer's spill too. None kept. */

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
void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect)
{
    struct EffectPosition ground;
    struct EffectPosition position;
    s32 point[3];
    struct BattleEffectWork *work;
    void *canvas;
    struct BlitterPair draw;
    u8 *sheet;
    struct BattleCamera *camera;
    s32 frame;
    void *palette;
    s32 i;

    work = (struct BattleEffectWork *)Runtime_AllocateHeapBlock(39, sizeof *work);
    canvas = (void *)Runtime_AllocateHeapBlock(40, 0x4000);
    sheet = (u8 *)Runtime_AllocateHeapBlock(41, 0x60e);
    camera = gCameraWork;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    work->fade_frames = 24;
    work->fade_step = 0;
    REG_BLDALPHA = 0x100c;
    REG_BG2PA = 0x100;
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
    {
        /* FAKEMATCH: called through a pointer, the routine's address loads before the palette's. */
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

        copy((void *)BG_PLTT, palette, 128);
    }

    for (i = 0; i != 128; i++) {
        gFlecks[i].y = 0;
        gFlecks[i].x = Random16() & 0xffff;
        gFlecks[i].z = (Random16() & 0x1ff) + i * 2;
        gFlecks[i].variant = -i;
    }
    /* FAKEMATCH: read-only work use exchanges r11/r9, fixing 16 register differences. */
    __asm__ ("" : : "r" (work));
    /* FAKEMATCH: a scheduling barrier the native order needs. */
    __asm__("");
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
        REG_BG2X = (64 - ground.x) << 8;
        if (frame > 49)
            REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
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
    /* FAKEMATCH: a scheduling barrier the native order needs. */
    __asm__("");
    /* FAKEMATCH: a scheduling barrier the native order needs. */
    __asm__("");
}
