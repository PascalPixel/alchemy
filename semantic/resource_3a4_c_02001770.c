typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x02001770, 200 bytes: the per-frame update of a
 * bouncing record. While it is unheld, drag it down by 1.5 per frame,
 * floor it at the limit in +20 and thump on arrival; run a free-running
 * 60-frame counter at +100 that fires a sound and relaunches at zero; and
 * decay a second short counter at +102 that runs one tint step on its
 * second-to-last frame.
 *
 * A PUBLISHED CALLBACK (sweep B): no `bl` in the image reaches it. It
 * takes ONE argument (r0, moved to r5 on the first instruction and used
 * for every access in the body) and returns void.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02001770 through the
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02001828-0x0200182c, one alignment halfword, and the two-word literal
 * pool 0x02001830-0x02001837; the next owner is 0x02001838, which is
 * itself still residue. measureSpan reports 190, undersizing by the pool.
 *
 * Six call sites, all resolved with the `+2` rule via
 * `bun tools/overlay_call_targets.ts resource_3a4 0x1770 0x1838` and none
 * read from the listing's arrows, which print six 0x02005xxx addresses
 * that are not functions:
 *   0x200179a -> Func_080091f0(-1, -1, 0xe666)
 *   0x20017a8 -> Func_08009080(record, 1)
 *   0x20017c2 -> Func_080f9010(229)                 established, a sound id
 *   0x20017d6 -> Func_080091f0(0x10000, 0, 0x10000)
 *   0x20017fe -> Func_080f9010(152)
 *   0x200180a -> Func_08009080(record, 2)
 *
 * SAME RECORD FAMILY as resource_3a4_c_02002a48.c. Both rows drive a
 * halfword counter at +100 and a word at +104, and 0x02002b58 stamps
 * +100, +102 and +104 when it spawns one. This row reads +102 as well and
 * uses +12/+20 as a position and its floor, +40 as a held/unheld word and
 * byte +91 as the flag mirroring it. Roles are recorded by offset; the
 * record layout is not being declared as a type here.
 *
 * Two shapes worth naming because either could be transcribed wrong.
 * First, both halfword tests are done as `value << 16` compared against a
 * shifted literal -- `v << 16 == 0x10000` is `v == 1` and
 * `v << 16 == 0x3c0000` is `v == 60`, not a comparison against the big
 * constants the listing shows. Second, the two arms under the floor test
 * CONVERGE: the sound-and-relaunch arm ends by re-reading +20 at
 * 0x20017da and both arms share the single store at 0x20017dc, so
 * `*(record + 12) = *(record + 20)` happens on both paths and the arms are
 * not independent blocks.
 *
 * One transcription choice stated plainly: the zero written to +104 at
 * 0x20017cc is r7, which holds *(record + 40) and is known to be zero
 * because the enclosing branch tested it so. It is written as 0.
 */

extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_08009080(u8 *record, s32 arg1);
extern void Func_080f9010(s32 sound_id);

void Func_02001770(u8 *record)
{
    u16 counter;

    if (*(s16 *)(record + 102) != 0) {
        counter = (u16)(*(u16 *)(record + 102) - 1);
        *(u16 *)(record + 102) = counter;
        if (counter == 1) {
            Func_080091f0(-1, -1, 0xe666);
        }
    }

    if (*(s32 *)(record + 40) == 0) {
        Func_08009080(record, 1);
        *(s32 *)(record + 12) += (s32)0xfffe8000;
        if (*(s32 *)(record + 12) < *(s32 *)(record + 20)) {
            if (*(s32 *)(record + 104) != 0) {
                Func_080f9010(229);
                *(s32 *)(record + 104) = 0;
                *(u16 *)(record + 102) = 4;
                Func_080091f0(128 << 9, 0, 128 << 9);
            }
            *(s32 *)(record + 12) = *(s32 *)(record + 20);
        }
        record[91] = 1;
    } else {
        record[91] = 0;
    }

    if (*(s16 *)(record + 100) == 0) {
        Func_080f9010(152);
        *(s32 *)(record + 104) = 1;
        Func_08009080(record, 2);
        *(s32 *)(record + 40) = 192 << 10;
    }

    counter = (u16)(*(u16 *)(record + 100) + 1);
    *(u16 *)(record + 100) = counter;
    if (counter == 60) {
        *(u16 *)(record + 100) = 0;
    }
}
