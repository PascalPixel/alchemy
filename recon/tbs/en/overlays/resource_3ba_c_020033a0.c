/* NONMATCHING: 856 of 964 bytes, 465 differing halfwords, 320 aligned edits.
 * Complete own-ROM extent 020033a0..02003764 includes eight final pool words.
 * Verified equivalent twins: resource_3bb:02003638 and resource_3bc:020040d0.
 * 2026-09-26 audited all imports and preserved this baseline. The address-owned
 * cursor hypothesis predicted stack writebacks after each append. A one-field
 * aggregate plus inline append gave 832/453/320; a pointer/address union gave
 * identical bytes. Both still promoted the cursor, failing that invariant.
 * A tagged volatile cursor gave 952/468/346 and the reference's 20-byte frame,
 * but introduced repeated cursor reads absent from the reference. Rejected.
 * 2026-09-27: canonical value-returning Engine_VramLoad is byte-identical
 * to this baseline. A one-element cursor array is also byte-identical: no
 * stack cursor writebacks appear. Both axes are closed without new bytes.
 * Three earlier trials closed; no declaration permutations and no new DONE bytes.
 * The reference separates the entry walker, sprite cursor and state pointer,
 * and holds OAM shape/palette constants across calls. This source still folds
 * those roles. Reopen only with new alias/lifetime evidence, not a size gain.
 * 2026-09-27 scene lane H1: typed 12-byte OAM records and post-increment
 * submission reproduce entry advancement before the call. A phase-shared shape
 * local still folds y|0x8000 across the middle calls; allocation dumps retain
 * the write cursor in r7, state at sp+12 and a 16-byte frame, not the required
 * cursor/state at sp+12/sp+16 and 20-byte frame. No complete owner is exact.
 * H2: initialize the shared counter before the first submit and advance it
 * before first-loop submission, as observed in the ROM. The first-loop counter
 * now advances before the call but lives in sl; the shape spills at sp+4.
 * The write cursor remains r7 and the frame remains 16 bytes. Counter timing
 * alone cannot recover the cursor/constant lifetime; this axis is closed.
 * H3: the work's first 216 bytes are eighteen typed OAM records; deriving
 * entry and the write cursor from that array emits exactly H2's bytes.
 * Complete normalized diffs and allocation dumps close this aggregate axis.
 * All three ROM extents are equivalent (including eight pool words); do not
 * replay these trials on 3bb/3bc. The required unresolved invariant is a
 * write cursor at sp+12, saved state at sp+16, y in r7, entry in r8, and
 * separate shape/palette lifetimes in r9/sl, with a 20-byte frame.
 * H4: the exact Haidia EXTENDED_SEQUENCE.C witness at 3c4fe6cee prompted
 * FIELD_EVENT.H's FieldActor return type and separate nullable marker scopes.
 * The complete emitted extent is byte-identical to H3 (856/465/320): both
 * actors already allocate to r6. Unlike Haidia, there is no earlier child
 * record lifetime to separate here. Typed coordinate ownership alone leaves
 * the cursor/frame/constant disagreement unchanged; this axis is closed.
 * Four scene-lane structural attempts checkpointed, zero exact function bytes
 * and zero alignment bytes. All three owners remain C not yet written.
 * Astra OAM transfer (2026-09-27): three post-increment word stores in
 * the second loop, as in TITLE.C and the improved ship row, give 864/964
 * bytes, 466 halfwords / 313 edits. Frame grows from 16 to the required 20,
 * but spills tile at sp+12, not the cursor; cursor stays r7. Full diff
 * rejects the frame alone as proof of the source layout. Adding one shared
 * palette=0xe400 variable gives 880/460/333 and frame24, still cursor r7.
 * A separate bounded row-phase model explicitly precomputes tile/shape,
 * then uses goto loops (second row walks packed x by 16<<16). It gives
 * 848/462/325, frame16, cursor r7 and lost duplicate cursor updates.
 * Both follow-ups are rejected; retain the previous 856-byte canonical.
 * All complete differences and literal pools were read. No twin adoption.
 */
#include "TYPES.H"
#include "DMA.H"
#include "FIELD_EVENT.H"

s32 Engine_BumpAllocateAlternatePool(s32 size);
void Engine_BumpFree(void *buffer);
void Engine_VramRelease(s32 id);
void Engine_OamSubmitRecord(void *entry, s32 mode);
struct FieldActor *Engine_ObjectTableGet(s32 actor);

struct OamEntry { u32 header, pos, tile; };

struct KawaState {
    struct OamEntry oam[18];
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

extern struct KawaState *Data_03001f3c;
extern struct TileEntry gVramBlockCache[];
extern u32 gFrameCount;

void Scene_RunScene3baSequenceA(void)
{
    struct KawaState *state;
    u32 *p;
    struct OamEntry *entry;
    s16 *id;
    s16 *rise;
    u32 tile;
    s32 count;
    s32 y;
    u32 i;
    u32 shape;
    s32 x;
    u8 *buffer;

    state = Data_03001f3c;
    entry = state->oam;
    p = &entry->header;
    id = &state->id;
    tile = gVramBlockCache[*id].tile >> 5;
    count = state->count;
    if (state->raised != 0) {
        state->rise = 2;
    } else if (Engine_GameFlagIsSet(0x106)) {
        if (state->rise > 0)
            state->rise--;
    } else if (state->rise <= 1 && ++state->rise == 1) {
        Dma_Set((const void *)0x200bef4, (void *)0x50003c0, 0x80000010, (volatile u32 *)0x040000d4);
        buffer = (u8 *)Engine_BumpAllocateAlternatePool(0x200);
        Engine_ResourceDecodeType01((const u8 *)0x200bf14, buffer);
        Engine_VramLoad(*id, 0x200, buffer);
        Engine_BumpFree(buffer);
    }
    if (state->rise == 0) {
        Engine_VramRelease(((struct KawaState *)p)->id);
        return;
    }
    y = (state->rise * 6 - 8) & 0xff;
    shape = 0x8000;
    *p++ = 0;
    *p++ = ((104 - count * 16) << 16) | y | shape;
    *p++ = tile | 0xe400;
    i = 0;
    Engine_OamSubmitRecord(entry++, 255);
    for (; i < count;) {
        shape = 0x40000000;
        *p++ = 0;
        *p++ = ((96 - i * 16) << 16) | y | shape;
        *p++ = (tile + 2) | 0xe400;
        i++;
        Engine_OamSubmitRecord(entry++, 255);
    }
    i = 0;
    *p++ = i;
    shape = 0x8000;
    *p++ = (112 << 16) | y | shape;
    *p++ = (tile + 6) | 0xe400;
    Engine_OamSubmitRecord(entry++, 255);
    *p++ = i;
    *p++ = (120 << 16) | y | shape | 0x10000000;
    *p++ = (tile + 6) | 0xe400;
    Engine_OamSubmitRecord(entry++, 255);
    for (; i < count; i++) {
        shape = 0x40000000;
        p[0] = 0;
        p[1] = ((128 + i * 16) << 16) | y | shape | 0x10000000;
        p[2] = (tile + 2) | 0xe400;
        p += 3;
        Engine_OamSubmitRecord(entry++, 255);
    }
    shape = 0x8000;
    *p++ = 0;
    *p++ = ((count * 16 + 128) << 16) | y | shape | 0x10000000;
    *p++ = tile | 0xe400;
    Engine_OamSubmitRecord(entry++, 255);
    if ((gFrameCount & 15) <= 4)
        return;
    shape = 0x40000000;
    {
        struct FieldActor *actor = Engine_ObjectTableGet(state->marker_a);
        if (actor != 0) {
            x = Engine_MathDivide(actor->x.fixed - state->origin_x, 0xe0000) + 112;
            y = (Engine_MathDivide(actor->z.fixed - state->origin_z, 0xe0000) + state->rise * 6 - 4) & 0xff;
            *p++ = 0;
            *p++ = (x << 16) | y | shape;
            *p++ = (tile + 12) | 0xe400;
            Engine_OamSubmitRecord(entry++, 255);
        }
    }
    {
        struct FieldActor *actor = Engine_ObjectTableGet(state->marker_b);
        if (actor != 0) {
            x = Engine_MathDivide(actor->x.fixed - state->origin_x, 0xe0000) + 112;
            y = (Engine_MathDivide(actor->z.fixed - state->origin_z, 0xe0000) + state->rise * 6 - 4) & 0xff;
            *p++ = 0;
            *p++ = (x << 16) | y | shape;
            *p = (tile + 8) | 0xe400;
            Engine_OamSubmitRecord(entry, 255);
        }
    }
}
