#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "iwram_call.h"
#include "dma.h"
#include "scene.h"
#include "ram_buffer.h"
#include "btldisp.h"
#include "map_scroll.h"
#include "io_reg.h"
#include "heap_state.h"
#include "mem.h"
#include "btlpty.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "btlwork.h"
#include "resource_ids.h"
#include "motobj.h"
#include "btlsys.h"
#include "irq.h"
#include "battle_intro.h"
#include "resource.h"
#include "project.h"
#include "transform.h"

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    /* FAKEMATCH: needed to match: a direct call changes BattlePresentation_BuildTilemap from push {r5, r6, lr} to push {r5, r6, r7, lr} */
    Iwram_FillWords(dst, size, value);
}

extern void *gWorkSlot[64];

extern u8 gDisp[];

void BattlePresentation_BuildTilemap(s32 *destination)
{
    s32 entry;
    u32 index;

    FillWords(destination, 0x100, -1);
    destination += 0x40;
    FillWords(destination, 0x80, 0x03ff03ff);
    entry = (0x0201 << 16) | 0x0200;
    destination += 0x20;
    index = 0;
    do {
        index++;
        *destination++ = entry;
        entry += 0x00020002;
    } while (index <= 239);
    FillWords(destination, 0x280, 0x03ff03ff);
}

void BattlePres_UpdateHBlankScroll(void)
{
    struct BattleAffineHdma *records;
    u16 *record;
    s32 control;

    if (((struct BattleBackgroundView *)*(void **)((u8 *)gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND * sizeof(void *)))->mode == 2) {
        records = *(struct BattleAffineHdma **)&gWorkSlot[10];
        record = records->lines[records->page];
        control = record[0];
        /* FAKEMATCH: needed to match: the do-while keeps the BG2CNT address load after the record read */
        do {
            REG_BG2CNT = control;
        } while (0);
        Dma_Set(record + 1, (void *)0x0400000c, 0xa2600001, &REG_DMA0SAD);
        Dma_Set(&records->affine, (void *)0x04000020, 0x84000004, REG_DMA3);
    }
}

void BattlePresentation_UploadTileVariant(void)
{
    u32 variant = **(u32 **)gDisp - 1;
    if (variant <= 31)
        Dma_Set(BattlePres_TileVariants + (variant >> 2) * 32, (void *)0x06005000, 0x84000008, REG_DMA3);
}

void BattlePres_AdvanceTransitionTimer(void)
{
    s32 scroll;
    struct BattleCamera *camera;
    u32 *timer;
    struct BgScroll *offset;
    u32 frame;
    u32 next;

    timer = *(u32 **)Ram_Disp;
    frame = *timer;
    camera = *(struct BattleCamera **)Ram_CameraWork;
    scroll = 0x34 - frame;
    if (scroll > 0x20)
        scroll = 0x20;
    offset = (struct BgScroll *)Ram_BgScroll;
    if (scroll < 0)
        scroll = 0;
    offset->y = scroll;
    if (frame <= 0x50U)
        camera->yaw = 45 * frame * 8 + 0xaf80;
    next = (*timer = *timer + 1);
    if (next <= 0x50U) {
        BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
        return;
    }
    BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

void BattlePres_DrawTransitionRows(void)
{
    u32 i;
    s32 frame;
    s32 aligned;
    s32 tile;
    u32 row;
    u16 *tiles;

    frame = **(s32 **)gDisp;
    if (frame >= 0 && frame <= 79) {
        tile = (7 & frame) + 0xf081;
        if (frame >= 0) {
            aligned = frame;
        } else {
            aligned = frame + 7;
        }
        row = 13 - (aligned >> 3);
        i = 0;
        tiles = (u16 *)((row << 6) + 0x06006000);
        do {
            i++;
            *tiles = tile;
            tiles++;
        } while (i != 32);

        tile = tile | 0x800;
        aligned = frame;
        if (frame < 0) {
            aligned = aligned + 7;
        }
        row = (aligned >> 3) + 13;
        if (row <= 20) {
            i = 0;
            tiles = (u16 *)((row << 6) + 0x06006000);
            do {
                i++;
                *tiles = tile;
                tiles++;
            } while (i != 32);
        }
    }
}

void Graphics_ClearBg0Vofs(void)
{
    u32 zero = 0;

    REG_BG0VOFS = zero;
}

#define BattleActor_SpawnObjectsForList ((void (*)(u16*,s32))BattleActor_SpawnObjectsForList)
#define Graphics_BuildSequentialTileTable ((void (*)(void*))Graphics_BuildSequentialTileTable)
#define BattlePresentation_BuildTilemap ((void (*)(void*))BattlePresentation_BuildTilemap)

#define gTransitionWork ((u8 (*))&gTransitionWork)

struct DisplayRegisters {
    u16 control;
    u16 green_swap;
};

struct TransitionActorList {
    u8 unknown_00[20];
    s32 count;
    u8 unknown_18[12];
    u16 ids[24];
};

void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind)
{
    volatile u32 fill;
    struct TransitionActorList list;
    u16 party[14];
    u16 actors[14];
    struct BattleBackgroundView *work;
    s32 *timer;
    u16 *map;
    s32 tile;
    u32 i;
    u32 j;
    s32 count;
    s32 id;
    struct BtlObj *object;
    struct BtlWork *session;
    u16 *ids;
    s32 n;

    work = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    timer = Runtime_AllocateHeapBlock(42, 4);
    if (kind != 0x15b) {
        {

            /* FAKEMATCH: the channel address is pinned to r3 so the display control can be addressed from it. */
            register volatile u32 *dma __asm__("r3") = REG_DMA3;

            Dma_Set(BattlePres_CurtainTiles, (void *)0x06005020, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 32, (void *)0x06005040, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 64, (void *)0x06005060, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 96, (void *)0x06005080, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 128, (void *)0x060050a0, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 160, (void *)0x060050c0, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 192, (void *)0x060050e0, 0x84000008, dma);
            ((struct DisplayRegisters *)((u8 *)dma - 212))->control = 1;
        }
        work->second_mode = 1;
        work->mode = 1;
        work->busy = 0;
        fill = 0x33333333;
        Dma_Set((void *)&fill, (void *)0x06005000, 0x85000008, REG_DMA3);
        fill = 0;
        Dma_Set((void *)&fill, (void *)0x06005100, 0x85000008, REG_DMA3);
        REG_BG1CNT = 0xc04;
        REG_BG0CNT |= 2;
        work->mode = 2;
        map = (u16 *)0x06006000;
        for (i = 0; i <= 31; i++) {
            if (i <= 20)
                tile = 0xf080;
            else
                tile = 0xf088;
            for (j = 0; j <= 31; j++)
                *map++ = tile;
        }
        {
        s32 start = 0;

        gBgScroll[0].y = 32;
        gBgScroll[1].y = 32;
        gBgScroll[1].x = 8;
        WaitFrames(1);
        REG_WIN0H = 0xf0;
        REG_WIN0V = 0x88;
        REG_WIN1H = 0xf0;
        REG_WIN1V = 0x88;
        REG_WININ = 0x3537;
        REG_WINOUT = 0x3f21;
        QueueIoWriteDelay2(0x04000000, 0x7741);
        BattlePres_SetupTransitionScene(0, 0, 0, 180);
        *timer = start;
        Scheduler_AddOrUpdateCallback(BattlePres_AdvanceTransitionTimer, 3200);
        Scheduler_AddOrUpdateCallback(BattlePres_DrawTransitionRows, 0x480);
        Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
        gBgScroll[0].y = 32;
        WaitFrames(1);
        UiWindow_CreateWithLayoutBoundsFar(BtlWk->pty_st);
        WaitFrames(20);
        QueueIoWriteDelay10(0x04000008, 2);
        QueueIoWriteDelay6(0x04000008, 0);
        BattleIntro_AnnounceEncounter(enemy_count);
        Scheduler_RemoveCallback(BattlePres_AdvanceTransitionTimer);
        Scheduler_RemoveCallback(BattlePres_DrawTransitionRows);
        gBgScroll[0].y = start;
        Runtime_SetIrqHandler(2, 0, 0);
        }
    } else {
        session = *(gWorkSlot + HEAP_SLOT_BATTLE);
        work->second_mode = 1;
        work->busy = 0;
        count = BattleParty_ListActorIds(3, (u16 *)party);
        for (i = 0; i != count; i++) {
            id = i + 120;
            if ((s32)i <= 7)
                id = i;
            object = BtlObjGet(id);
            if (UnitGetFar(id)->class_id != 148)
                object->scale = 0xb333;
        }
        IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
        WaitFrames(1);
        {
        s32 start = 0;

        session->background = (u32)&ResourceId_VinasuChojoStormBackdrop;
        list.count = BattleParty_ListActorIds(2, list.ids);
        list.ids[list.count] = 0xff;
        BattleActor_SpawnObjectsForList(list.ids, 0);
        BattleEffect_RunTileAndPaletteAnimationFar(&list);
        BattlePres_SetupTransitionScene(0, 0, 0, 100);
        *timer = start;
        }
        Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
        WaitFrames(1);
        WaitFrames(20);
        UiWindow_CreateWithLayoutBoundsFar(BtlWk->pty_st);
        QueueIoWriteDelay10(0x04000008, 2);
        QueueIoWriteDelay6(0x04000008, 0);
        REG_BLDCNT = 0x3f40;
        ids = actors;
        n = BattleParty_ListActorIds(3, (u16 *)ids);
        actors[n] = 0xff;
        BattleActor_SpawnObjectsForList(ids, 0);
        n = BattleParty_ListActorIds(1, (u16 *)ids);
        for (i = 0; i != n; i++)
            BattlePres_SetActorRecordMode(ids[i], 1);
        for (i = 0; i != 16; i++) {
            REG_BLDALPHA = i | 0x1000;
            WaitFrames(1);
        }
        for (i = 0; i != n; i++)
            BattlePres_SetActorRecordMode(ids[i], 0);
        BattleIntro_AnnounceEncounter(enemy_count);
        gBgScroll[0].y = 0;
        WaitFrames(1);
        Runtime_SetIrqHandler(2, 0, 0);
    }
    Runtime_SetIrqHandler(2, 0, 0);
    REG_BG1CNT = 0x1f83;
    WaitFrames(1);
    REG_BG1CNT = 0x1f83;
    REG_BG0CNT &= 0xfffd;
    gBgScroll[1].x = 8;
    REG_DISPCNT = 0x1541;
    Runtime_ReleaseHeapBlock(42);
}

struct Half {
    u16 v;
};

void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
    struct BtlWork *screen = BtlWk;
    u16 *palette = screen->palette;
    u32 ime;
    u32 *slot = &ime;

    /* FAKEMATCH: needed to match: the one-pass block and the loop on a zero local keep the IME save through slot's register */
    do {
        u16 *ime_reg = (u16 *)0x04000208;
        s32 once = 0;

        do {
            *slot = *ime_reg;
            *ime_reg = (u32)ime_reg;
        } while (once);
    } while (0);

    if (level == 0) {
        Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
    } else {
        Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0, screen->brightness = 0x10000 - level * 1092, 128);
    }
    {
        u32 saved = *slot;

        REG_IME = saved;
    }
}

void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade)
{
    /* FAKEMATCH: needed to match: one-pass IME save, as in the IO write queue, and a one-halfword struct mask held across the fade loop */

    struct BattleBackgroundView *transition = *(struct BattleBackgroundView **)gTransitionWork;

    if (transition->mode == 0) {
        Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
    }
    transition->mode = mode;

    if (mode == 1) {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        {
            /* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
            do {
                ime = &REG_IME;
                saved = *ime;
            } while (0);
            *ime = (u16)ime;
            count = q->count;
            if (count <= 31) {
                u32 *destination = q->entries[count];
                *(u16 *)&q->count = count + 1;
                *destination++ = 0x1f83;
                *destination++ = 0x0400000a;
                *destination = 0x20000;
            }
            *ime = saved;
        }
    }

    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;

    if (fade == 0x80) {
        Dma_Set(BtlWk->palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
    } else if (fade != 0) {
        u16 *source = BtlWk->palette;
        u16 *destination = (u16 *)0x050000c0;
        s32 i;
        struct Half mask;

        mask.v = 0x1f;
        for (i = 0; i != 128; i++) {
            s32 red = source[i] & 31;
            s32 green = (source[i] >> 5) & mask.v;
            s32 blue = (source[i] >> 10) & mask.v;

            if (red > fade) {
                red -= fade;
            } else {
                red = 0;
            }
            if (green > fade) {
                green -= fade;
            } else {
                green = 0;
            }
            if (blue > fade) {
                blue -= fade;
            } else {
                blue = 0;
            }
            destination[i] = (blue << 10) | (green << 5) | red;
        }
    }

    Graphics_BuildSequentialTileTable((void *)0x06003800);
    BattlePresentation_BuildTilemap((void *)0x0600f800);
}

#undef gTransitionWork

#define gTransitionWork (*(struct BattleBackgroundView * *)&gTransitionWork)

void BattlePresentation_InitializeWorkAndResetState(void)
{
    struct BattleAffineHdma *work;
    struct BattleBackgroundView *state;
    volatile u32 zero;
    work = Runtime_AllocateBlock(10, sizeof *work);
    state = gTransitionWork;
    zero = 0;
    Dma_Set(&zero, work, 0x850000a8, REG_DMA3);
    state->mode = 0;
}

void Runtime_ReleaseHeapBlock10(void)
{
    Runtime_ReleaseHeapBlock(10);
}

typedef void (*BitDecoder)(const u8 *source, void *destination);

void BattleBackground_Load(s32 mode, s32 resource, s32 level)
{
    struct BattleBackgroundView *view = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    u8 *data = Resource_GetTableEntry(resource);
    struct BtlWork *session = *(gWorkSlot + HEAP_SLOT_BATTLE);
    u16 *palette;

    /* FAKEMATCH: needed to match: the one-pass block keeps the size load after the session load and the decoder call after the copy */
    do {
        u32 size = (u32)BitDecoder_Size;
        void *decoder = Runtime_AllocateHeapBlock(49, size);

        Dma_Set((void *)BitDecoder_DecodeImage, decoder, 0x84000000 | (size >> 2), REG_DMA3);
    } while (0);
    {
        u32 offset = 0x100;
        void *vram;

        /* FAKEMATCH: the empty statement keeps the tile offset a load of its own, ahead of the VRAM address. */
        __asm__("" : "+r"(offset));
        vram = (void *)0x06008000;
        ((BitDecoder)*(gWorkSlot + 49))(data + offset, vram);
    }
    Runtime_ReleaseHeapBlock(49);
    palette = session->palette;
    Dma_Set(data, palette, 0x84000040, REG_DMA3);
    if (level >= 0)
        Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0, session->brightness = 0x10000 - level * 1092, 128);
    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;
    Graphics_BuildSequentialTileTable((void *)0x06003800);
    BattlePresentation_BuildTilemap((void *)0x0600f800);
    Iwram_ClearWords((void *)0x0600ffc0, 64);
    if (view->mode == 0)
        Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
    view->mode = mode;
    if (mode == 1)
        REG_BG1CNT = 0x1f83;
}

#define Camera_StoreSceneParameters ((void (*)(s32,u32,s32))Camera_StoreSceneParameters)
#define SceneTransform_ApplyPosition ((void (*)(s32*))SceneTransform_ApplyPosition)

static __inline__ s32 DivQ16(s32 divisor, s32 value)
{
    /* FAKEMATCH: needed to match: a direct call finishes the value shift before the divisor load */
    return Iwram_RatioMulQ14(divisor, value);
}

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale)
{
    struct BattleBackgroundView *view = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    struct BattleAffineHdma *work = *(gWorkSlot + 10);
    struct BattleCamera *camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
    u16 wrap;
    s32 horizon;
    u16 *line;
    struct BattleAffine *affine;
    s32 ratio;
    s32 step;
    s32 x;
    s32 y;
    u32 i;
    u32 count;

    horizon = 0x800;
    wrap = 0;
    if (scale >= 0x10000) {
        wrap = 0x2000;
        horizon = 0x6800 + -(s16)camera->yaw * 3;
    }
    if (work == NULL)
        return;
    if ((view->mode == 1 || view->second_mode == 1) && view->busy == 0)
        gBgScroll[1].x = horizon >> 8;
    if (view->mode != 2)
        return;
    line = work->lines[work->page ^ 1];
    ratio = DivQ16(scale, 0x10000);
    step = ratio >> 8;
    affine = &work->affine;
    affine->pa = step;
    affine->pb = 0;
    affine->pc = 0;
    affine->pd = step;
    x = Iwram_MulQ16(ratio, Iwram_MulQ16(cx, scale - 0x10000));
    y = Iwram_MulQ16(ratio, Iwram_MulQ16(cy, scale - 0x10000));
    affine->x = ((x + 0x7fff) >> 8) + ox + horizon;
    y = ((y + 0x7fff) >> 8) + oy - 0x1000;
    affine->y = y;
    count = (DivQ16((s16)step, 0x4000 - y) >> 16) + 1;
    i = 0;
    if (!GameFlag_TestFar(0x16b)) {
        for (; i < 16; i++)
            *line++ = 0x3f8e;
    }
    if (count > 136)
        count = 136;
    for (; i < count; i++)
        *line++ = wrap | 0x478a;
    for (; i < 136; i++)
        *line++ = wrap | 0x478e;
    for (; i < 160; i++)
        *line++ = 0x3f8e;
    work->page ^= 1;
}

void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y)
{
    s32 span = 0x01fe0000;
    /* FAKEMATCH: needed to match: mode intentionally remains uninitialized to preserve the match */
    s32 mode;
    struct BattleCamera *scene = gCameraWork;
    s32 *pos = scene->pos;
    s32 scale = (mode << 16) / 100;
    s32 render_bounds[3];
    s32 measured_bounds[3];
    s32 source_bounds[3];
    s32 (*blend)(s32, s32);
    s32 alpha;
    s32 half;
    s32 width;
    u32 result;

    pos[0] = x;
    pos[1] = depth;
    pos[2] = y;
    source_bounds[0] = 0;
    alpha = 0xc000;
    source_bounds[1] = 0;
    source_bounds[2] = 0;

    blend = Iwram_RatioMulQ14;
    result = blend(span, alpha);
    Camera_StoreSceneParameters(span, result, span * 2);
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw((s16)scene->yaw);
    SceneTransform_ApplyPitch((s16)scene->pitch);
    render_bounds[0] = 0;
    render_bounds[1] = 0;
    render_bounds[2] = scene->distance;
    Iwram_TransformVector(render_bounds, (s32 *)scene);
    gProjection.center_x = 120;
    gProjection.center_y = 120;
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(scene->eye, pos);
    Render_ProjectPoint(source_bounds, measured_bounds);

    BattleCamera_SetRange(
        0x780000,
        0x780000,
        (120 - measured_bounds[0]) << 8,
        (120 - measured_bounds[1]) << 8,
        scale);
    half = scale * 0xff;
    width = half * 2;
    result = blend(width, alpha);
    Camera_StoreSceneParameters(width, result, half * 4);
}

#undef Camera_StoreSceneParameters
#undef SceneTransform_ApplyPosition

#define Camera_StoreSceneParameters ((int (*)())Camera_StoreSceneParameters)
#define Render_ResetTransformState ((int (*)())Render_ResetTransformState)
#define SceneTransform_ApplyPitch ((int (*)())SceneTransform_ApplyPitch)
#define SceneTransform_ApplyPosition ((int (*)())SceneTransform_ApplyPosition)
#define SceneTransform_ApplyYaw ((int (*)())SceneTransform_ApplyYaw)

void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom)
{
    struct BattleCamera *scene = gCameraWork;
    s32 *pos = scene->pos;
    s32 scale = (zoom << 16) / 100;
    s32 render_bounds[3];
    s32 measured_bounds[3];
    s32 source_bounds[3];
    s32 span = 0x01fe0000;
    s32 (*blend)(s32, s32);
    s32 alpha;
    s32 half;
    s32 width;
    u32 result;

    pos[0] = x;
    pos[1] = depth;
    pos[2] = y;
    source_bounds[0] = 0;
    alpha = 0xc000;
    source_bounds[1] = 0;
    source_bounds[2] = 0;

    blend = Iwram_RatioMulQ14;
    result = blend(span, alpha);
    Camera_StoreSceneParameters(span, result, span * 2);
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw((s16)scene->yaw);
    SceneTransform_ApplyPitch((s16)scene->pitch);
    render_bounds[0] = 0;
    render_bounds[1] = 0;
    render_bounds[2] = span;
    Iwram_TransformVector(render_bounds, (s32 *)scene);
    gProjection.center_x = 120;
    gProjection.center_y = 120;
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(scene->eye, pos);
    Render_ProjectPoint(source_bounds, measured_bounds);

    BattleCamera_SetRange(
        0x780000,
        0x780000,
        (120 - measured_bounds[0]) << 8,
        (120 - measured_bounds[1]) << 8,
        scale);
    half = scale * 0xff;
    width = half * 2;
    result = blend(width, alpha);
    Camera_StoreSceneParameters(width, result, half * 4);
}

#undef Camera_StoreSceneParameters
#undef Render_ResetTransformState
#undef SceneTransform_ApplyPitch
#undef SceneTransform_ApplyPosition
#undef SceneTransform_ApplyYaw
