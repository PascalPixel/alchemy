#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
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
#include "TRANSFORM.H"
#include "BATTLE_WORK.H"
#include "IO_WRITE_QUEUE.H"
#include "MAP_SCROLL.H"
#include "HEAP_STATE.H"

#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)
#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)


void Audio_PlayCue(s32 cue);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);

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
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    draw[0] = heap_cache[7];
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 1152);
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
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
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
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
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
                draw[0](canvas, work->sheet + (((frame - 56) / 2) << 11), rx - 16, ry, 32, 64);
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
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}


void Audio_PlayCue(s32 cue);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);



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
    REG_BG2PA = 0x100;
    REG_BLDALPHA = 0x1000;
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)heap_cache[46 - 39];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)heap_cache[47 - 39];
    blit = draw;
    Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work->sheet, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 1152);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
    REG_BG2X = (64 - position.x) << 8;
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
            REG_BLDALPHA = frame | 0x1000;
            if (frame == 16)
                REG_BLDCNT = 0;
        }
        if (frame > 103) {
            REG_BLDALPHA = (120 - frame) | 0x1000;
            if (frame == 104)
                REG_BLDCNT = 0x3f44;
        }
        for (i = 15; i != -1; i--) {
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
                blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
                    (blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
                    (blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1),
                    wide, high);
                if (frame >= i * 4 + 16) {
                    blade->x += blade->velocity_x;
                    blade->y += blade->velocity_y;
                }
            } else {
                blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
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
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

#define gFlecks ((struct EffectStep *)Ram_MapCellBuffer)

void AudioCommand_PlayFar(s32 value);

struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

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

    work = Runtime_AllocateHeapBlock(39, sizeof *work);
    canvas = Runtime_AllocateHeapBlock(40, 0x4000);
    sheet = Runtime_AllocateHeapBlock(41, 1550);
    camera = gCameraWork;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    work->fade_frames = 24;
    work->fade_step = 0;
    REG_BLDALPHA = 0x100c;
    REG_BG2PA = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
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
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 1152);
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
            REG_BLDALPHA = (112 - frame * 2) | 0x1000;
        if (frame == 16)
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 20);
        if (frame <= 55) {
            s32 n;
            u8 *glow;

            i = frame / 2;
            n = i % 4;
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            draw.lower = (Ram_WorkSlot)[47];
            draw.lower(canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
            n = frame / 4 % 3;
            glow = work->sheet + n * 1032 + 0x1100;
            draw.lower(canvas, glow, 40, ground.y - 36, 24, 43);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            draw.lower = (Ram_WorkSlot)[47];
            n = i % 4;
            draw.lower(canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
            draw.lower(canvas, glow, 64, ground.y - 36, 24, 43);
            Runtime_ReleaseHeapBlock(47);
        }
        GetBattleObjectSlotFar(work->effect->actor);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
        for (i = 0; i != 32; i++) {
            struct EffectStep *fleck = &gFlecks[i];

            if (fleck->variant >= 0) {
                s32 size;

                point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
                point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
                point[1] = fleck->y;
                fleck->x += 1024;
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
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
    Runtime_ReleaseHeapBlock(41);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    /* FAKEMATCH: a scheduling barrier the native order needs. */
    __asm__("");
    /* FAKEMATCH: a scheduling barrier the native order needs. */
    __asm__("");
}

extern u8 BattlePieceStartX[];
extern u8 BattlePieceStartY[];
extern u8 BattlePieceWidth[];
extern u8 BattlePieceHeight[];
extern u16 BattlePieceOffset[];

#define PLOT(x, y) (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 + (y) / 8 * 2048) = 2)

void BattleEffect_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect)
{
    /* FAKEMATCH: never read; the native frame keeps 12 unused bytes at sp+44..55, exactly this struct. */
    struct EffectPosition position;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw1;
    DrawRectangle draw0;
    struct BattleSession *battle;
    struct BattleBackgroundView *view;
    void **slots;
    void **cache;
    s32 rmax;
    struct EffectStep *piece;
    s32 r;
    s32 i;
    s32 px;
    s32 j;
    s32 k;
    s32 d;
    s32 py;
    s32 x1;
    s32 size;
    s32 left;
    s32 left2;
    s32 y1;
    s32 x2;
    s32 y2;
    u8 *start_y;
    u8 *start_x;
    s32 cx;
    s32 cy;
    s32 cell_size;
    u8 *cells;
    u32 cell_fill;

    work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
    size = 0x4000;
    canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, size);
    Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
    slots = gWorkSlot + HEAP_SLOT_BATTLE;
    battle = slots[0];
    view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
    work->effect = effect;
    BattleFx_ClearActorTimers();
    view->second_mode = 1;
    gBgScroll[1].y = 32;
    BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
    REG_BG2CNT = 0x784;
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    view->second_mode = 0;
    REG_BG2X = 0;
    REG_BG2Y = 0xfffff000;
    REG_BG2PA = 0x80;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 0x100;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x1088;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x1088;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;

    i = 0;
    for (k = 0; k != 16; k++) {
        for (j = 0; j != 8; j++, i += 2) {
            s16 entry;

            entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
            *(volatile s16 *)(0x06003800 + i) = entry;
        }
    }

    Iwram_ClearWords(canvas, size);
    Iwram_ClearWords((void *)0x06004000, size);
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
    REG_BLDALPHA = 0x1010;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask, work, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
    cache = gWorkSlot;
    draw0 = cache[HEAP_SLOT_BLITTER];
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
    draw1 = cache[HEAP_SLOT_BLITTER_ALTERNATE];

    /* FAKEMATCH: the copy and fill operands set here, before the piece loop, so CSE does not share them and the pair reloads both as the native does; written at the calls they leave 45/1051. */
    cell_size = 0x7800;
    cells = Ram_MapCellBuffer;
    cell_fill = 0x01010101;
    start_y = BattlePieceStartY;
    start_x = BattlePieceStartX;
    r = 0;
    piece = work->particles;
    for (i = 0; i != 33; i++) {
        s32 x, y, vx, vy;

        x = *start_x++ << 16;
        y = *start_y++ << 16;
        piece->x = x;
        piece->y = y;
        vx = (x - 0x200000) >> 2;
        vy = (y - 0x3c0000) >> 2;
        piece->velocity_x = vx;
        piece->velocity_y = vy;
        piece++;
    }

    Iwram_CopyWords(cells, (void *)0x06008000, cell_size);
    Iwram_FillWords(cells, cell_size, cell_fill);
    view->busy = 1;
    work->saved_bg1_x = gBgScroll[1].x;
    work->saved_bg1_y = gBgScroll[1].y;
    gBgScroll[1].x = 0;
    IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f81, 0x20000);
    Iwram_FillWords((void *)0x050000c0, 0x100, 0x7fff7fff);
    /* FAKEMATCH: r5 clobber prunes draw1's r5 preference (its load address is in r5); without it the piece constants swap r5/r7 and the fill routine takes r6, 173/1052. */
    asm volatile("" ::: "r5");
    Audio_PlayCue(212);
    ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 3, 0, 30);

    for (frame = 0; frame != 128; frame++) {
        if (frame == 2)
            Audio_PlayCue(212);
        if (frame == 3)
            Audio_PlayCue(212);
        if (frame == 28)
            ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 3, -1, 0);
        if (frame == 32)
            Audio_PlayCue(149);
        if (frame == 5) {
            Audio_PlayCue(145);
            /* FAKEMATCH: barrier; sched2 otherwise loads 0x77a0 before reloading work, 2/1051. */
            asm("");
            gBgScroll[1].x = work->saved_bg1_x;
            BattleBackground_LoadFar(1, battle->background, -1);
        }

        if (frame > 7) {
            u16 *pal;

            pal = (u16 *)0x050000c0;
            for (i = 0; i != 128; i++) {
                s32 red = *pal & 0x1f;
                s32 green = (*pal >> 5) & 0x1f;
                s32 blue = (*pal >> 10) & 0x1f;
                s32 goal_red = battle->palette[i] & 0x1f;
                s32 goal_green = (battle->palette[i] >> 5) & 0x1f;
                s32 goal_blue = (battle->palette[i] >> 10) & 0x1f;

                if (red < goal_red)
                    red++;
                else if (red > goal_red)
                    red--;
                if (green < goal_green)
                    green++;
                else if (green > goal_green)
                    green--;
                if (blue < goal_blue)
                    blue++;
                else if (blue > goal_blue)
                    blue--;
                *pal++ = (blue << 10) | (green << 5) | red;
            }
        }

        if (frame == 4)
            Iwram_FillWords((void *)0x06008000, 0x7800, 0x02020202);

        if (frame <= 3) {
            s32 v;

            v = frame * 4 + 8;
            rmax = frame * 32;
            BG_PLTT[2] = (v << 10) | (v << 5) | v;
            cx = 96;
            cy = 60;
            for (; r != rmax; r++) {
                px = r;
                py = 0;
                d = r;
                while (px >= py) {
                    x2 = px + cx;
                    x1 = cx - px;
                    left = x1;
                    y1 = cy - py;
                    y2 = py + cy;
                    if (y1 < 0)
                        y1 = 0;
                    if (y2 > 119)
                        y2 = 119;
                    if (x1 < 0)
                        x1 = 0;
                    if (x2 > 255)
                        x2 = 255;
                    PLOT(x2, y2);
                    PLOT(x2, y1);
                    PLOT(x1, y2);
                    PLOT(x1, y1);
                    x1 = left + 1;
                    x2 = px + 97;
                    if (x1 < 0)
                        x1 = 0;
                    if (x2 > 255)
                        x2 = 255;
                    PLOT(x2, y2);
                    PLOT(x2, y1);
                    PLOT(x1, y2);
                    PLOT(x1, y1);

                    x2 = py + cx;
                    x1 = cx - py;
                    left2 = x1;
                    y2 = px + cy;
                    y1 = cy - px;
                    if (x1 < 0)
                        x1 = 0;
                    if (x2 > 255)
                        x2 = 255;
                    if (y1 < 0)
                        y1 = 0;
                    if (y2 > 119)
                        y2 = 119;
                    PLOT(x2, y2);
                    PLOT(x2, y1);
                    PLOT(x1, y2);
                    PLOT(x1, y1);
                    x1 = left2 + 1;
                    x2 = py + 97;
                    if (x1 < 0)
                        x1 = 0;
                    if (x2 > 255)
                        x2 = 255;
                    PLOT(x2, y2);
                    PLOT(x2, y1);
                    PLOT(x1, y2);
                    PLOT(x1, y1);

                    d -= py * 2 + 1;
                    if (d < 0) {
                        d += px * 2 - 2;
                        px--;
                    }
                    py++;
                }
            }
            Iwram_CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
        }

        if (frame <= 50) {
            for (i = 0; i != 33; i++) {
                piece = &work->particles[i];
                draw0(canvas, (u8 *)work + BattlePieceOffset[i],
                      ((s16 *)&piece->x)[1], ((s16 *)&piece->y)[1],
                      BattlePieceWidth[i], BattlePieceHeight[i]);
                if (frame > 3)
                    EffectStep_AdvanceWithGravity2D(piece, 64, 0x4000);
            }
        }

        if (frame >= 8 && frame <= 50) {
            s32 v;

            v = frame - 8;
            if (v > 31)
                v = 31;
            BG_PLTT[1] = (v << 10) | (v << 5) | v;
        }

        if (frame == 51) {
            Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 0);
            for (i = 1; i != 64; i++) {
                s32 v;

                v = i / 2;
                if (v < 0)
                    v = 0;
                BG_PLTT[i] = (v << 10) | ((v / 2) << 5) | v;
            }
            REG_BLDCNT = 0x3f44;
            for (i = 0; i != 32; i++) {
                piece = &work->particles[i];
                piece->x = ((Random16() & 31) + 32) << 16;
                piece->y = ((Random16() & 31) + 80) << 16;
                piece->velocity_x = ((Random16() & 0x1ff) - 0x100) << 12;
                piece->velocity_y = 0;
                piece->variant = 0;
            }
            work->transfer_mode = 2;
            work->transfer_value = 50;
        }

        if (frame > 52) {
            for (i = 0; i != 32; i++) {
                piece = &work->particles[i];
                if (frame >= i / 4 + 52 && piece->variant <= 39) {
                    s32 cell;

                    cell = piece->variant / 4;
                    if (cell > 5)
                        cell = 5;
                    draw1(canvas, (u8 *)work + (cell << 11),
                          ((s16 *)&piece->x)[1] - 16, ((s16 *)&piece->y)[1] - 32, 32, 64);
                    EffectStep_AdvanceWithGravity2D(piece, 60, -0x1000);
                    piece->variant++;
                }
            }
        }

        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 1, -1, 0);
    gBgScroll[1].x = work->saved_bg1_x;
    gBgScroll[1].y = 32;
    BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 0);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}
