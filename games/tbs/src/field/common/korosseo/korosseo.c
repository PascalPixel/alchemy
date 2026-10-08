#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "affine.h"
#include "vram_block.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "tbs_edition.h"
#include "mem.h"
#include "field_event.h"
#include "dma.h"
#include "resource_ids.h"
#include "call.h"
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"


static __inline__ void Dma_Wait(volatile u32 *dma)
{
    while (dma[2] & 0x80000000)
        ;
}

struct Sprite { u32 words[3]; };

struct Half { u16 value; };

extern s16 Korosseo_ModeTaskTimer;
extern s16 Korosseo_ModeMoveTarget;
extern s16 Korosseo_ModeMoveDuration;
extern s16 Korosseo_ModeTaskMode;
extern s16 Korosseo_ModeTaskParam;
extern s32 Korosseo_ModeTaskPosition;
extern s16 *Korosseo_ModeTaskScript;

#define QueueRegister(address, value) \
{ \
    u32 saved; \
    s32 cnt; \
    do { /* FAKEMATCH: needed to match: removing this one-pass boundary changes measured instruction scheduling. */ saved = *ime; } while (0); \
    *ime = (u16)(u32)ime; \
    cnt = queue->count; \
    if (cnt < 32) { \
        u32 *entry = (u32 *)((u8 *)queue + cnt * 12 + 4); \
        *(u16 *)&queue->count = cnt + 1; \
        *entry++ = (value); \
        *entry++ = (address); \
        *entry = 0x20000; \
    } \
    *ime = saved; \
}

void Korosseo_LoadPortrait(s32 id)
{
    u8 *buf;
    s32 off;

    buf = Runtime_BumpAllocateAlternatePool(0x1c00 + KOROSSEO_PALETTE_BYTES);
    if (Korosseo_PortraitSlot == -1)
        Korosseo_PortraitSlot = Resource_FindFreeEntry();
    off = Korosseo_PortraitPaletteOffsets[id];
    if (id == 8)
        id = 4;
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Lettering), buf);
    Dma_Set(buf + off, (void *)&OBJ_PLTT_COLOR(15, 0), DMA_ENABLE32 | DMA_32BIT | 8, REG_DMA3);
    Call3(Engine_VramLoad, Korosseo_PortraitSlot, 0x400, (id << 10) + (s32)buf + KOROSSEO_PALETTE_BYTES);
    Dma_Wait(REG_DMA3);
    Runtime_BumpFree(buf);
}

void Korosseo_UpdateModeTask(void)
{
    /* FAKEMATCH: needed to match: the queue read boundary and count-store alias keep separate cursors for each write. */
    /* FAKEMATCH: needed to match: a halfword zero aggregate keeps the interior literal pools. */
    u32 *write = Korosseo_ModeTaskSprites;
    struct Sprite *sprite = (struct Sprite *)write;
    s32 tile = gVramBlockCache[Korosseo_PortraitSlot].offset >> 5;
    u32 flags;
    s32 scale, blend, pos;
    s32 matrix, i, x, y, left;
    struct AffineTransform work;
    struct IoWriteQueue *queue;
    volatile u16 *ime;

commands:
    if (Korosseo_ModeTaskTimer != 0)
        goto render;
    {
        switch (*Korosseo_ModeTaskScript++) {
        case 0x4000:
            Korosseo_ModeTaskPosition = *Korosseo_ModeTaskScript++ << 8;
            Korosseo_ModeMoveTarget = *Korosseo_ModeTaskScript++;
            Korosseo_ModeMoveDuration = 0;
            break;
        case 0x3000:
            Korosseo_ModeMoveStart = Korosseo_ModeMoveTarget;
            Korosseo_ModeMoveTarget = *Korosseo_ModeTaskScript++;
            Korosseo_ModeMoveDuration = *Korosseo_ModeTaskScript++;
            Korosseo_ModeMoveStep = 0;
            break;
        case 0x1000:
            Korosseo_ModeScaleStart = Korosseo_ModeScaleTarget;
            Korosseo_ModeScaleTarget = *Korosseo_ModeTaskScript++;
            Korosseo_ModeScaleDuration = *Korosseo_ModeTaskScript++;
            Korosseo_ModeScaleStep = 0;
            break;
        case 0x2000:
            Korosseo_ModeBlendStart = Korosseo_ModeBlendTarget;
            Korosseo_ModeBlendTarget = *Korosseo_ModeTaskScript++;
            Korosseo_ModeBlendDuration = *Korosseo_ModeTaskScript++;
            Korosseo_ModeBlendStep = 0;
            break;
        case 0x7fff:
            Korosseo_ModeTaskTimer = *Korosseo_ModeTaskScript++;
            break;
        case -1:
            Engine_TaskRemoveCallback(Korosseo_UpdateModeTask);
            Resource_ResetEntry(Korosseo_PortraitSlot);
            return;
        }
    }
    goto commands;
render:
    Korosseo_ModeTaskTimer--;
    if (Korosseo_ModeScaleDuration == 0) {
        scale = Korosseo_ModeScaleTarget;
    } else {
        struct Half zero = { 0 };
        s32 duration, start, progress, target;
        duration = Korosseo_ModeScaleDuration;
        start = Korosseo_ModeScaleStart;
        target = Korosseo_ModeScaleTarget;
        progress = ++Korosseo_ModeScaleStep;
        scale = start + (target - start) * progress / duration;
        if (progress >= duration)
            Korosseo_ModeScaleDuration = zero.value;
    }
    if (Korosseo_ModeBlendDuration == 0) {
        blend = Korosseo_ModeBlendTarget;
    } else {
        struct Half zero = { 0 };
        s32 duration, start, progress, target;
        duration = Korosseo_ModeBlendDuration;
        start = Korosseo_ModeBlendStart;
        target = Korosseo_ModeBlendTarget;
        progress = ++Korosseo_ModeBlendStep;
        blend = start + (target - start) * progress / duration;
        if (progress >= duration)
            Korosseo_ModeBlendDuration = zero.value;
    }
    if (Korosseo_ModeMoveDuration == 0) {
        pos = Korosseo_ModeMoveTarget;
    } else {
        struct Half zero = { 0 };
        s32 duration, start, progress, target;
        duration = Korosseo_ModeMoveDuration;
        start = Korosseo_ModeMoveStart;
        target = Korosseo_ModeMoveTarget;
        progress = ++Korosseo_ModeMoveStep;
        pos = start + (target - start) * progress / duration;
        if (progress >= duration)
            Korosseo_ModeMoveDuration = zero.value;
    }
    work.angle = 0;
    work.scale_x = scale;
    work.scale_y = scale;
    matrix = (s16)AffineMatrix_BuildForEffect(&work);
    Korosseo_ModeTaskPosition += pos;
    pos = Korosseo_ModeTaskPosition / 256;
    switch (Korosseo_ModeTaskMode) {
    case 1: {
        u32 attr;
        i = 0;
        y = 56;
        flags = 0x80004000;
        attr = matrix << 25;
        for (; i < 4; i++) {
            x = pos + scale * (i * 32 - 48) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | tile;
                Runtime_PushSlotEntry(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
    case 3: {
        u32 attr;
        i = 0;
        y = 48;
        flags = 0x80004000;
        attr = matrix << 25;
        for (; i < 2; i++) {
            x = pos + scale * (i * 32 - 16) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | (tile + Korosseo_ModeTaskParam);
                Runtime_PushSlotEntry(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
#if defined(TBS_EDITION_FR)
    case 4: {
        u32 attr;
        i = 0;
        y = 64;
        flags = 0x80004000;
        attr = matrix << 25;
        for (; i < 2; i++) {
            x = pos + scale * (i * 32 - 16) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | (tile + Korosseo_ModeTaskParam);
                Runtime_PushSlotEntry(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
#else
    case 4:
        y = 48;
        flags = 0xc0004000;
        left = pos + 56;
        if ((u32)(pos + 120) < 304) {
            x = left & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Korosseo_ModeTaskParam);
            Runtime_PushSlotEntry(sprite, 236);
        }
        break;
#endif
    case 2:
        y = 48;
        flags = 0x80000000;
        left = pos + 88;
        if ((u32)(pos + 152) < 304) {
            x = left & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Korosseo_ModeTaskParam);
            Runtime_PushSlotEntry(sprite, 236);
        }
        break;
    }
    queue = &gIoWriteQueue;
    ime = &REG_IME;
    QueueRegister((u32)&REG_BLDCNT, 0x3f00)
    QueueRegister((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend)
}
