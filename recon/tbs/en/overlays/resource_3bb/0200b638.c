/* NONMATCHING: the Colosso race gauge, identical in resource_3ba
 * (0x0200b3a0, Scene_RunScene3baSequenceA), resource_3bb (0x0200b638,
 * FieldScene_RunExtendedActorSequence) and resource_3bc (0x0200c0d0,
 * FieldScene_RunScene3bcSequenceB), 964 bytes each, after
 * COMMON/KOROSSEO/PATH_RIVAL.C in all three. Meant for
 * FIELD/COMMON/KOROSSEO/GAUGE.C once it matches; it then needs shared labels
 * for the 32-byte palette and the graphics that follow it in each overlay
 * (3bb 0x0200c174/KorosseoKabe_MarkerGraphics, 3ba 0x0200bef4/
 * KorosseoKawa_ImageData, 3bc 0x0200cd60/gColossoSceneDescriptor) and a
 * Resource_ActivateEntry label on each overlay's unlabeled veneer.
 *
 * Remaining difference (alchemy permute 13670; best permuted 7015): the
 * reference keeps the OAM write pointer on the stack (sp+12) and a second
 * copy of the gauge pointer at sp+16, with y, the push pointer, the size
 * and attribute constants and the segment count in r7-r11; this draft gets
 * a 16-byte frame and different allocation. Push(entry++) matches the
 * reference's increment-before-call; the OR operand order follows it.
 */
#include "TYPES.H"
#include "DMA.H"

struct VramBlock {
    u16 pad;
    u16 base;
};

struct GaugeActor {
    u8 pad00[8];
    s32 x;
    u8 pad0c[4];
    s32 z;
};

struct OamEntry {
    s32 link;
    s32 attr;
    s32 tile;
};

struct KorosseoGauge {
    struct OamEntry oam[18];
    s16 slot;
    s16 level;
    s16 hold;
    s16 rival;
    s16 player;
    u16 pad_e2;
    u16 pad_e4;
    s16 segments;
    s32 centre_x;
    s32 centre_z;
};

extern struct VramBlock gVramBlockCache[];
extern struct KorosseoGauge *gKorosseoWork;
extern u32 gFrameCount;
extern u16 Korosseo_GaugePalette[];
extern u8 Korosseo_GaugeGraphics[];

s32 Engine_GameFlagIsSet(s32 flag);
u8 *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(u8 *block);
void Resource_DecodeType01(u8 *source, u8 *destination);
void Engine_VramLoad(s32 slot, s32 size, u8 *source);
s32 Resource_ActivateEntry(u32 slot);
void Runtime_PushSlotEntry(struct OamEntry *entry, s32 priority);
struct GaugeActor *Engine_ActorLookup(s32 actor);
s32 Engine_MathDivide(s32 dividend, s32 divisor);

void Korosseo_DrawGauge(void)
{
    struct KorosseoGauge *gauge = gKorosseoWork;
    s32 *dst = (s32 *)gauge;
    struct OamEntry *entry = gauge->oam;
    s32 tile;
    u32 segments;
    u32 i;
    s32 y;
    s32 x;
    s32 width;
    u8 *buf;
    struct GaugeActor *actor;

    tile = gVramBlockCache[gauge->slot].base >> 5;
    segments = gauge->segments;
    if (gauge->hold != 0) {
        gauge->level = 2;
    } else if (Engine_GameFlagIsSet(262)) {
        if (gauge->level > 0)
            gauge->level--;
    } else if (gauge->level <= 1) {
        if (++gauge->level == 1) {
            Dma_Set(Korosseo_GaugePalette, (void *)0x050003c0, 0x80000010, (volatile u32 *)0x040000d4);
            buf = Runtime_BumpAllocateAlternatePool(512);
            Resource_DecodeType01(Korosseo_GaugeGraphics, buf);
            Engine_VramLoad(gauge->slot, 512, buf);
            Runtime_BumpFree(buf);
        }
    }
    if (gauge->level == 0) {
        Resource_ActivateEntry(gauge->slot);
        return;
    }

    y = (gauge->level * 6 - 8) & 255;
    width = segments * 16;
    *dst++ = 0;
    *dst++ = ((104 - width) << 16) | y | 0x8000;
    *dst++ = tile | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);
    for (i = 0; i < segments; i++) {
        *dst++ = 0;
        *dst++ = ((96 - i * 16) << 16) | y | 0x40000000;
        *dst++ = (tile + 2) | 0xe400;
        Runtime_PushSlotEntry(entry++, 255);
    }
    *dst++ = 0;
    *dst++ = (112 << 16) | y | 0x8000;
    *dst++ = (tile + 6) | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);
    *dst++ = 0;
    *dst++ = (120 << 16) | y | 0x8000 | 0x10000000;
    *dst++ = (tile + 6) | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);
    for (i = 0; i < segments; i++) {
        *dst++ = 0;
        *dst++ = y | ((128 + i * 16) << 16) | 0x40000000 | 0x10000000;
        *dst++ = (tile + 2) | 0xe400;
        Runtime_PushSlotEntry(entry++, 255);
    }
    *dst++ = 0;
    *dst++ = y | ((width + 128) << 16) | 0x8000 | 0x10000000;
    *dst++ = tile | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);

    if ((gFrameCount & 15) > 4) {
        actor = Engine_ActorLookup(gauge->player);
        if (actor != 0) {
            x = Engine_MathDivide(actor->x - gauge->centre_x, 0xe0000) + 112;
            y = (Engine_MathDivide(actor->z - gauge->centre_z, 0xe0000) + gauge->level * 6 - 4) & 255;
            *dst++ = 0;
            *dst++ = y | (x << 16) | 0x40000000;
            *dst++ = (tile + 12) | 0xe400;
            Runtime_PushSlotEntry(entry++, 255);
        }
        actor = Engine_ActorLookup(gauge->rival);
        if (actor != 0) {
            x = Engine_MathDivide(actor->x - gauge->centre_x, 0xe0000) + 112;
            y = (Engine_MathDivide(actor->z - gauge->centre_z, 0xe0000) + gauge->level * 6 - 4) & 255;
            *dst++ = 0;
            *dst++ = y | (x << 16) | 0x40000000;
            *dst = (tile + 8) | 0xe400;
            Runtime_PushSlotEntry(entry, 255);
        }
    }
}
