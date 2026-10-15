/*
 * Draft of Func_080d1e18, the twin of Func_080d1dac with object mode 1 and
 * priority 0. 8 instructions differ: the table address is loaded after its
 * offset is begun, and the -13 mask is negated before attribute byte 9 is
 * loaded, where the listing loads first.
 */
#include "types.h"
#include "resource.h"
#include "vram_tab.h"

struct EffectSprite {
    u8 unk00[4];
    u16 y : 8;
    u16 affineMode : 2;
    u16 objectMode : 2;
    u16 mosaic : 1;
    u16 palette256 : 1;
    u16 shape : 2;
    u16 attr1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unk0a[20];
    u16 depth;
};

void Func_080eaf98();

void Func_080d1e18(struct EffectSprite *sprite, s32 a, s32 b, s32 c)
{
    Func_080eaf98(sprite, a, b, c, ResourceTableEntries[94].offset >> 5);
    sprite->depth = 0;
    sprite->priority = 0;
    sprite->palette256 = 0;
    sprite->palette = 0;
    sprite->objectMode = 1;
}
