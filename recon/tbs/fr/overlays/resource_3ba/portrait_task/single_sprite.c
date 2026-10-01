/* NONMATCHING: French Colosso mode-four sprite layout, 2026-10-01.
 * This generic case draws one 64x32 sprite at y=48. French draws two 32x32
 * sprites at y=64; this complete callback is 1264 bytes instead of 1312
 * in all three trial overlays.
 */
#include "TBS_EDITION.H"
/* Colosso: decode the portrait sheet into a scratch block, copy portrait
 * id's palette into object palette 15 and its 1 KB of tiles into the cached
 * VRAM slot (claimed on first use). Portrait 8 shares tiles with 4. The same
 * function sits in each of the three Colosso trial overlays. */
#include "DMA.H"
#include "RESOURCE_IDS.H"
#include "CALL.H"
/* Colosso: the trial overlays' scripted sprite task. Each frame it runs the
 * mode script's commands until a wait, interpolates the sprite's scale, the
 * blend and the horizontal position toward their targets, draws the mode's
 * sprites through an affine matrix and queues the blend registers. The same
 * function sits in each of the three Colosso trial overlays, with its
 * variables in each overlay's own work. */
#include "TYPES.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"

extern s16 Korosseo_PortraitSlot;
extern u8 Korosseo_PortraitPaletteOffsets[];
u8 *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(u8 *block);
s32 Resource_FindFreeEntry(void);
s32 Resource_GetTableEntry(s32 id);
void Resource_DecodeType01(s32 entry, u8 *destination);
void VramBlock_LoadCached(s32 slot, s32 size, s32 source);

static __inline__ void Dma_Wait(volatile u32 *dma)
{
    while (dma[2] & 0x80000000)
        ;
}

struct Sprite { u32 words[3]; };

struct SpriteTile { u16 pad, base; };

struct SpriteTransform { unsigned x : 16; unsigned y : 16; unsigned angle : 16; unsigned pad : 16; };

/* FAKEMATCH: a halfword zero aggregate keeps the interior literal pools. */
struct Half { u16 value; };

extern struct SpriteTile gVramBlockCache[];
extern s16 Korosseo_PortraitSlot, Korosseo_ModeTaskTimer, Korosseo_ModeMoveTarget, Korosseo_ModeMoveDuration;
extern s16 Korosseo_ModeMoveStart, Korosseo_ModeMoveStep, Korosseo_ModeScaleTarget, Korosseo_ModeScaleStart;
extern s16 Korosseo_ModeScaleDuration, Korosseo_ModeScaleStep, Korosseo_ModeBlendTarget, Korosseo_ModeBlendStart;
extern s16 Korosseo_ModeBlendDuration, Korosseo_ModeBlendStep, Korosseo_ModeTaskMode, Korosseo_ModeTaskParam;
extern s32 Korosseo_ModeTaskPosition;
extern s16 *Korosseo_ModeTaskScript;
extern u32 Korosseo_ModeTaskSprites[];
extern s32 Engine_TaskRemoveCallback(void (*fn)(void));
extern void Resource_ResetEntry(s32 slot);
extern s32 AffineMatrix_BuildForEffect(struct SpriteTransform *work);
extern void Runtime_PushSlotEntry(void *sprite, s32 priority);

/* FAKEMATCH: transfer the exact queue read boundary and count-store alias.
 * Each publication owns its cursor; only the hardware pointers persist. */
#define QueueRegister(address, value) \
{ \
    u32 saved; \
    s32 cnt; \
    do { /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling; see its retained draft. */ saved = *ime; } while (0); \
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

void Korosseo_UpdateModeTask(void)
{
    u32 *write = Korosseo_ModeTaskSprites;
    struct Sprite *sprite = (struct Sprite *)write;
    s32 tile = gVramBlockCache[Korosseo_PortraitSlot].base >> 5;
    s32 scale, blend, pos;
    s32 matrix, i, x, y, left;
    u32 flags;
    struct SpriteTransform work;
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
    work.x = scale;
    work.y = scale;
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
    QueueRegister(0x04000050, 0x3f00)
    QueueRegister(0x04000052, ((16 - blend) << 8) | blend)
}
