/* NONMATCHING: 832 of 964 bytes, 458 differing halfwords, 323 aligned edits.
 * Complete own-ROM extent 020033a0..02003764 includes eight final pool words.
 * Verified equivalent twins: resource_3bb:02003638 and resource_3bc:020040d0.
 * 2026-09-26 audited all imports and preserved this baseline. The address-owned
 * cursor hypothesis predicted stack writebacks after each append. A one-field
 * aggregate plus inline append gave 832/453/320; a pointer/address union gave
 * identical bytes. Both still promoted the cursor, failing that invariant.
 * A tagged volatile cursor gave 952/468/346 and the reference's 20-byte frame,
 * but introduced repeated cursor reads absent from the reference. Rejected.
 * Three trials closed; no declaration permutations and no new DONE bytes.
 * The reference separates the entry walker, sprite cursor and state pointer,
 * and holds OAM shape/palette constants across calls. This source still folds
 * those roles. Reopen only with new alias/lifetime evidence, not a size gain. */
#include "TYPES.H"
#include "DMA.H"

s32 Engine_GameFlagIsSet(s32 flag);
s32 Engine_BumpAllocateAlternatePool(s32 size);
void Engine_ResourceDecodeType01(const void *source, void *destination);
void Engine_VramLoad(s32 id, s32 size, void *buffer);
void Engine_BumpFree(void *buffer);
void Engine_VramRelease(s32 id);
void Engine_OamSubmitRecord(void *entry, s32 mode);
struct KawaActor *Engine_ObjectTableGet(s32 actor);
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

void Scene_RunScene3baSequenceA(void)
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
        buffer = (u8 *)Engine_BumpAllocateAlternatePool(0x200);
        Engine_ResourceDecodeType01((const void *)0x200bf14, buffer);
        Engine_VramLoad(*id, 0x200, buffer);
        Engine_BumpFree(buffer);
    }
    if (state->rise == 0) {
        Engine_VramRelease(((struct KawaState *)p)->id);
        return;
    }
    y = (state->rise * 6 - 8) & 0xff;
    *p++ = 0;
    *p++ = ((104 - count * 16) << 16) | y | 0x8000;
    *p++ = tile | 0xe400;
    Engine_OamSubmitRecord(entry, 255);
    entry += 12;
    for (i = 0; i < count; i++) {
        *p++ = 0;
        *p++ = ((96 - i * 16) << 16) | y | 0x40000000;
        *p++ = (tile + 2) | 0xe400;
        Engine_OamSubmitRecord(entry, 255);
        entry += 12;
    }
    i = 0;
    *p++ = i;
    tall = 0x8000;
    *p++ = (112 << 16) | y | tall;
    *p++ = (tile + 6) | 0xe400;
    Engine_OamSubmitRecord(entry, 255);
    entry += 12;
    *p++ = i;
    *p++ = (120 << 16) | y | tall | 0x10000000;
    *p++ = (tile + 6) | 0xe400;
    Engine_OamSubmitRecord(entry, 255);
    entry += 12;
    for (; i < count; i++) {
        p[0] = 0;
        p[1] = ((128 + i * 16) << 16) | y | 0x40000000 | 0x10000000;
        p[2] = (tile + 2) | 0xe400;
        p += 3;
        Engine_OamSubmitRecord(entry, 255);
        entry += 12;
    }
    *p++ = 0;
    *p++ = ((count * 16 + 128) << 16) | y | 0x8000 | 0x10000000;
    *p++ = tile | 0xe400;
    Engine_OamSubmitRecord(entry, 255);
    entry += 12;
    if ((Data_03001e40 & 15) <= 4)
        return;
    actor = Engine_ObjectTableGet(state->marker_a);
    if (actor != 0) {
        x = Engine_MathDivide(actor->x - state->origin_x, 0xe0000) + 112;
        y = (Engine_MathDivide(actor->z - state->origin_z, 0xe0000) + state->rise * 6 - 4) & 0xff;
        *p++ = 0;
        *p++ = (x << 16) | y | 0x40000000;
        *p++ = (tile + 12) | 0xe400;
        Engine_OamSubmitRecord(entry, 255);
        entry += 12;
    }
    actor = Engine_ObjectTableGet(state->marker_b);
    if (actor != 0) {
        x = Engine_MathDivide(actor->x - state->origin_x, 0xe0000) + 112;
        y = (Engine_MathDivide(actor->z - state->origin_z, 0xe0000) + state->rise * 6 - 4) & 0xff;
        *p++ = 0;
        *p++ = (x << 16) | y | 0x40000000;
        *p = (tile + 8) | 0xe400;
        Engine_OamSubmitRecord(entry, 255);
    }
}
