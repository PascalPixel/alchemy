/* NONMATCHING: 832 of 964 bytes, 458 differing halfwords (2026-09-24). Hand-written from the
 * disassembly; twins resource_3bb:02003638 and resource_3bc:020040d0 are binding-equivalent, so the
 * finished source becomes one instanced unit. Matched shape: the rise branches spell state->rise
 * directly (gcse gives each branch its own address copy and the join reloads with ldrsh); the
 * in-loop *p++ stores give the separate loop address giv. Remaining: the reference keeps three
 * copies of the work pointer (r8 entry walker, sp12 sprite pointer, sp16 state) and spills the
 * sprite pointer, re-reading the id through it on the early return; the candidate unifies them
 * and keeps p in r6. The OAM attribute constants (0x8000, 0x40000000, 0xe400) live in r9/sl across
 * calls instead of folding; a u32 tall = 0x8000 local reproduces that for the middle pair only. */
#include "TYPES.H"
#include "DMA.H"

s32 Engine_GameFlagIsSet(s32 flag);
s32 Main_08000170(s32 size);
void Main_080001a8(const void *source, void *destination);
void Engine_VramLoad(s32 id, s32 size, void *buffer);
void Main_08000178(void *buffer);
void Main_080001c0(s32 id);
void Main_080001e8(void *entry, s32 mode);
struct KawaActor *Main_0808a400(s32 actor);
s32 Engine_MathDivide(s32 dividend, s32 divisor);

struct KawaActor {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
};

struct KawaState {
    u8 unknown_00[216];
    s16 id;
    s16 rise;
    s16 raised;
    s16 marker_b;
    s16 marker_a;
    u8 unknown_e2[4];
    s16 count;
    s32 origin_x;
    s32 origin_z;
};

struct TileEntry {
    u16 unknown_0;
    u16 tile;
};

extern u8 *Data_03001f3c;
extern struct TileEntry Data_03001b10[];
extern u32 Data_03001e40;

void Func_020033a0(void)
{
    struct KawaState *state;
    u32 *p;
    u8 *entry;
    s16 *id;
    s16 *rise;
    u32 tile;
    s32 count;
    s32 y;
    u32 i;
    u32 tall;
    s32 x;
    u8 *buffer;
    struct KawaActor *actor;

    entry = Data_03001f3c;
    p = (u32 *)entry;
    state = (struct KawaState *)entry;
    id = &state->id;
    tile = Data_03001b10[*id].tile >> 5;
    count = state->count;
    if (state->raised != 0) {
        state->rise = 2;
    } else if (Engine_GameFlagIsSet(0x106)) {
        if (state->rise > 0)
            state->rise--;
    } else if (state->rise <= 1 && ++state->rise == 1) {
        Dma_Set((const void *)0x200bef4, (void *)0x50003c0, 0x80000010, (volatile u32 *)0x040000d4);
        buffer = (u8 *)Main_08000170(0x200);
        Main_080001a8((const void *)0x200bf14, buffer);
        Engine_VramLoad(*id, 0x200, buffer);
        Main_08000178(buffer);
    }
    if (state->rise == 0) {
        Main_080001c0(((struct KawaState *)p)->id);
        return;
    }
    y = (state->rise * 6 - 8) & 0xff;
    *p++ = 0;
    *p++ = ((104 - count * 16) << 16) | y | 0x8000;
    *p++ = tile | 0xe400;
    Main_080001e8(entry, 255);
    entry += 12;
    for (i = 0; i < count; i++) {
        *p++ = 0;
        *p++ = ((96 - i * 16) << 16) | y | 0x40000000;
        *p++ = (tile + 2) | 0xe400;
        Main_080001e8(entry, 255);
        entry += 12;
    }
    i = 0;
    *p++ = i;
    tall = 0x8000;
    *p++ = (112 << 16) | y | tall;
    *p++ = (tile + 6) | 0xe400;
    Main_080001e8(entry, 255);
    entry += 12;
    *p++ = i;
    *p++ = (120 << 16) | y | tall | 0x10000000;
    *p++ = (tile + 6) | 0xe400;
    Main_080001e8(entry, 255);
    entry += 12;
    for (; i < count; i++) {
        p[0] = 0;
        p[1] = ((128 + i * 16) << 16) | y | 0x40000000 | 0x10000000;
        p[2] = (tile + 2) | 0xe400;
        p += 3;
        Main_080001e8(entry, 255);
        entry += 12;
    }
    *p++ = 0;
    *p++ = ((count * 16 + 128) << 16) | y | 0x8000 | 0x10000000;
    *p++ = tile | 0xe400;
    Main_080001e8(entry, 255);
    entry += 12;
    if ((Data_03001e40 & 15) <= 4)
        return;
    actor = Main_0808a400(state->marker_a);
    if (actor != 0) {
        x = Engine_MathDivide(actor->x - state->origin_x, 0xe0000) + 112;
        y = (Engine_MathDivide(actor->z - state->origin_z, 0xe0000) + state->rise * 6 - 4) & 0xff;
        *p++ = 0;
        *p++ = (x << 16) | y | 0x40000000;
        *p++ = (tile + 12) | 0xe400;
        Main_080001e8(entry, 255);
        entry += 12;
    }
    actor = Main_0808a400(state->marker_b);
    if (actor != 0) {
        x = Engine_MathDivide(actor->x - state->origin_x, 0xe0000) + 112;
        y = (Engine_MathDivide(actor->z - state->origin_z, 0xe0000) + state->rise * 6 - 4) & 0xff;
        *p++ = 0;
        *p++ = (x << 16) | y | 0x40000000;
        *p = (tile + 8) | 0xe400;
        Main_080001e8(entry, 255);
    }
}
