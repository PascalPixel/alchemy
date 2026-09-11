#include "types.h"

#define Scene_RunParticleSequence Func_080f7460

/*
 * Full source-shape draft for the standalone presentation sequence at
 * 0x080f7460.  It is entered from Runtime_BlankDisplayLoadValueAndRun
 * (games/gs1/src/runtime/blank_display_load_value_and_run.c, owner
 * 080f6008) immediately after Audio_PlayCue(9), and it owns its whole
 * screen: it allocates its four blocks, builds two tilemaps and a tile
 * bank, seeds five 21-cell record entries, then runs a frame loop until
 * its state word reaches 10 and finally fades out and releases
 * everything it took.
 *
 * The owner is a confirmed member of the same "work block" family as
 * games/gs1/recon/en/main/080e0c84.c (BattleFx_RunParticleReveal)
 * and games/gs1/src/battle/effects/member_orbit/run.c: same
 * Data_03001e50 heap-cache indexed by allocation id, same 0x782c work
 * block with the per-frame `work[0x7824] = 1` republish, the same
 * 28-byte fixed-point particle record based at 0x02010000, and the same
 * two-entry DrawRectangleFn table called through the `_call_via_r4`
 * trampoline that annotates as `bl Func_080072f4`
 * (games/gs1/asm/080072e4.s, r4 slot at +0x10).
 *
 * What remains uncertain:
 *
 *   1. Every `DMA3_SRC/DST/CTRL` group is one standalone three-register
 *      Thumb store multiple in the reference (`stmia r3!, {r0, r1, r2}`
 *      plus a dead `subs r3, #12`).  As already recorded in
 *      games/gs1/recon/en/main/080e7404.c, gcc 2.96's Thumb backend
 *      emits multi-register transfers only from movmem8b/movmem12b and
 *      those always print an adjacent `ldmia` first, so this draft
 *      writes the three descriptor words separately.  That is the
 *      largest single source of the remaining difference.
 *
 *   2. The reference loads several constants that would fit an 8-bit
 *      `movs` immediate (12, 0x3f, 0x40, 0x41, 0x76, 0x8f, 0x91, 0x93,
 *      0xa0, 0xb4, 0xbf, 0xf0) out of its literal pool while spelling
 *      other small values as immediates.  They are written here with
 *      the project's established `(s32)&Value_XXXXXXXX` absolute-symbol
 *      idiom.  Whether each of them really is a linker constant, or an
 *      ordinary literal the compiler happened to pool, is not
 *      established; the window bounds 0xf0/0xa0 in particular read like
 *      plain screen dimensions, and the reference also pool-loads a
 *      plain zero at 0x080f74dc that this draft spells as a literal 0
 *      because doing so scores better.
 *
 *   3. The third argument of the first Func_080072f0 call is never set
 *      by the reference: r2 still holds the 0x02010000 tile scratch
 *      pointer that the preceding copy loop left there.  This draft
 *      passes that pointer explicitly, which is the only reading that
 *      makes the call well defined, at the cost of one extra pool load.
 *
 *   4. The owner never sets a return value, so it is spelled void here
 *      even though blank_display_load_value_and_run.c declares it s32.
 *
 * Aggregate names are provisional.  Byte-offset accesses are retained
 * where the repository does not yet provide an evidence-backed
 * structure.
 */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

typedef volatile u16 vu16;
typedef volatile u32 vu32;

/* DMA channel 3.  One base register plus word offsets, which is the
   nearest ordinary C gets to the reference's single store multiple. */
#define DMA3 ((vu32 *)0x040000D4)
#define DMA3_SRC DMA3[0]
#define DMA3_DST DMA3[1]
#define DMA3_CTRL DMA3[2]

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, s32 width, s32 height);

/* The 28-byte particle record already established in 080e0c84.c.  Only
   x, y, vx, vy and the countdown at +0x18 are touched here. */
typedef struct {
    s32 x;
    s32 y;
    s32 rot;
    s32 vx;
    s32 vy;
    s32 unk14;
    s32 life;
} Particle;

/* Five of these open the 0x61c state block; the stride is proved by the
   28-byte walk of both seeding loops.  Byte 27 is natural alignment
   padding, not a proved field. */
typedef struct {
    s32 unk00;
    u8 cells[21];
    u8 unk19;
    u8 unk1a;
} SceneEntry;

/* Allocation-id keyed heap cache shared by this family. */
extern void *Data_03001e50[];

/* IWRAM cell this owner clears before the sequence starts. */
extern u8 Data_0200024c[];

/* Halfword table of sprite offsets into the 0x60e sprite block, indexed
   by the particle's remaining-life bucket. */
extern u16 Data_080f86f8[];

extern u8 Value_0000000c;
extern u8 Value_0000003f;
extern u8 Value_00000040;
extern u8 Value_00000041;
extern u8 Value_00000076;
extern u8 Value_0000008f;
extern u8 Value_00000091;
extern u8 Value_00000093;
extern u8 Value_000000a0;
extern u8 Value_000000b4;
extern u8 Value_000000bf;
extern u8 Value_000000f0;

void *Func_080048b0(s32 id, s32 size);
void *Func_080048f4(s32 id, s32 size);
void Func_08002f3c(s32 id);
void Func_080040e8(void);
void *Func_08002f40(s32 id);
u32 Func_08005340(const void *source, void *destination);
void Func_080072f0(void *dest, s32 size, s32 source, void *state);
void Func_08015000(void);
void Func_080f731c(void);
u32 Func_08004458(void);
s32 Func_08002304(s32 value, s32 divisor);
s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_080022ec(s32 value, s32 divisor);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_080c9000(s32 id, s32 a, s32 b, s32 c, s32 d);
s32 Func_080f6038(u16 *source, u16 *destination, s32 scale, s32 count);
s32 Func_080772e0(s32 id);
void *Func_08015010(s32 kind, s32 x, s32 y, s32 width, s32 height);
void Func_08015080(s32 text, void *window, s32 x, s32 y);
void Func_080f6148(void);
void Func_080f61e8(s32 id);
void Func_080f62b8(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
void Func_080030f8(s32 frames);

void Scene_RunParticleSequence(void)
{
    u8 *sprites;
    u8 *canvas;
    u8 *work;
    u8 *state;
    u8 *tiles;
    u8 *resource;
    u8 *source;
    u16 *shade;
    u16 *tile;
    u16 *cells;
    s32 *record;
    void *window;
    SceneEntry *entry;
    Particle *particle;
    DrawRectangleFn routine[2];
    s32 pick[8];
    s32 flags[7];
    s32 row;
    s32 col;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;
    s32 rank;
    s32 frame;
    s32 phase;
    s32 half;
    s32 speed;
    s32 angle;
    s32 x;
    s32 y;
    s32 px;
    s32 py;
    s32 scale;

    sprites = (u8 *)Func_080048b0(41, 0x60E);
    canvas = (u8 *)Func_080048b0(40, 0x8000);
    work = (u8 *)Func_080048f4(39, 0x782C);
    state = (u8 *)Func_080048f4(45, 0x61C);
    tiles = (u8 *)0x02010000;
    Func_08002f3c((s32)&Value_0000000c);
    Data_0200024c[288] = 0xFF;
    M2C_FIELD(state, u16 *, 0xA2) = 0;
    M2C_FIELD(state, s32 *, 0x98) = 1;
    Func_080040e8();

    /* Screen block 5: a 20 x 32 name table whose interior rectangle is
       one shared tile and whose remaining cells run consecutively. */
    *(u8 *)0x03001D18 = 0;
    tile = (u16 *)0x06002800;
    cnt = 0;
    for (row = 0; row != 20; row++) {
        for (col = 0; col != 32; col++) {
            if (col >= 5 && col <= 24 && row > 2 && row <= 13) {
                *tile = 0xA1A6;
            } else if (col > 29) {
                *tile = 0;
            } else {
                *tile = (u16)(0xA1A8 + cnt);
                cnt++;
            }
            tile++;
        }
    }

    Func_08005340(Func_08002f40((s32)&Value_00000076), sprites);

    resource = (u8 *)Func_08002f40((s32)&Value_0000003f);
    DMA3_SRC = (u32)resource;
    DMA3_DST = 0x05000140;
    DMA3_CTRL = 0x84000008;
    Func_08005340(resource + 32, tiles);

    /* Move the decompressed 8bpp tiles into the character block, leaving
       out the cells the shared interior tile already covers. */
    cnt = 0;
    for (row = 0; row != 20; row++) {
        source = tiles + ((row * 15) << 6);
        for (col = 0; col != 30; col++) {
            if (!(col >= 5 && col <= 24 && row > 2 && row <= 13)) {
                DMA3_SRC = (u32)source;
                DMA3_DST = 0x0600B500 + cnt;
                DMA3_CTRL = 0x84000008;
                cnt += 32;
            }
            source += 32;
        }
    }

    Func_080072f0((void *)0x06002D00, 768, (s32)tiles, (void *)0x03000164);

    /* Screen block 6: consecutive tiles inside rows 2..16, one filler
       tile outside them. */
    tile = (u16 *)0x06003000;
    for (row = 0; row != 20; row++) {
        for (col = 0; col != 32; col++) {
            if (row >= 2 && row <= 16) {
                *tile = (u16)(row * 32 + col + 148);
            } else {
                *tile = (u16)(s32)&Value_000000bf;
            }
            tile++;
        }
    }

    *(vu16 *)0x0400000A = 0x0509;
    *(vu16 *)0x0400000C = 0x0680;
    cells = (u16 *)0x03001AD0;
    cells[0] = 0;
    cells[1] = 0;
    cells[2] = 0;
    cells[3] = 0;
    cells[4] = 0;
    cells[5] = 0;
    *(vu16 *)0x04000048 = 0x3737;
    *(vu16 *)0x0400004A = 0x2727;
    *(vu16 *)0x04000050 = 0x3F44;
    *(vu16 *)0x04000052 = 0x1010;
    *(vu16 *)0x04000014 = 0;
    *(vu16 *)0x04000018 = 0;
    *(vu16 *)0x04000016 = 0xFF60;
    *(vu16 *)0x0400001A = 0xFF60;
    *(vu16 *)0x04000040 = 0x28C8;
    *(vu16 *)0x04000044 = 0x1878;
    *(vu16 *)0x04000040 = (u16)(s32)&Value_000000f0;
    *(vu16 *)0x04000044 = (u16)(s32)&Value_000000a0;
    *(vu16 *)0x04000042 = (u16)(s32)&Value_000000f0;
    *(vu16 *)0x04000046 = (u16)(s32)&Value_000000a0;

    shade = (u16 *)(work + 0x200);
    M2C_FIELD(state, s32 *, 0x8C) = 0;
    M2C_FIELD(state, s32 *, 0x90) = 0;
    M2C_FIELD(state, s32 *, 0x94) = 0;
    M2C_FIELD(work, s32 *, 0x778C) = 0;
    M2C_FIELD(state, s32 *, 0xA8) = 0;

    resource = (u8 *)Func_08002f40((s32)&Value_0000008f);
    DMA3_SRC = (u32)resource;
    DMA3_DST = 0x05000000;
    DMA3_CTRL = 0x84000020;
    *(vu16 *)0x05000080 = 0x2F8B;
    *(vu16 *)0x05000082 = 0x5BF6;

    resource = (u8 *)Func_08002f40((s32)&Value_00000040);
    DMA3_SRC = (u32)resource;
    DMA3_DST = 0x05000200;
    DMA3_CTRL = 0x84000078;
    Func_08005340(resource + 480, tiles);
    DMA3_SRC = (u32)tiles;
    DMA3_DST = 0x06010000;
    DMA3_CTRL = 0x84001B30;

    resource = (u8 *)Func_08002f40((s32)&Value_00000041);
    DMA3_SRC = (u32)resource;
    DMA3_DST = 0x050003E0;
    DMA3_CTRL = 0x84000008;
    Func_08005340(resource + 32, tiles);
    DMA3_SRC = (u32)tiles;
    DMA3_DST = 0x06016E00;
    DMA3_CTRL = 0x84000480;
    Func_08015000();
    Func_080f731c();

    /* Seed the five records: 21 cells of 0..4 each. */
    entry = (SceneEntry *)state;
    for (i = 0; i != 5; i++) {
        entry->unk00 = 8;
        entry->unk19 = 0;
        entry->unk1a = 0xFF;
        for (j = 0; j != 21; j++) {
            entry->cells[j] = (u8)Func_08002304((s32)Func_08004458(), 5);
        }
        entry++;
    }

    /* Then overwrite eight distinct cells of each record with the ranks
       0, 1, 2, 3, 4, 5, 5, 5. */
    entry = (SceneEntry *)state;
    for (i = 0; i != 5; i++) {
        for (j = 0; j != 8; j++) {
            pick[j] = Func_08002304((s32)Func_08004458(), 21);
            for (k = 0; k != j; k++) {
                if (pick[j] == pick[k]) {
                    j--;
                    break;
                }
            }
        }
        for (j = 0; j != 8; j++) {
            rank = j;
            if (rank > 5) {
                rank = 5;
            }
            entry->cells[pick[j]] = (u8)rank;
        }
        entry++;
    }

    Func_080c9000(46, 8, 7, 3, 2);
    routine[0] = (DrawRectangleFn)Data_03001e50[46];
    Func_080c9000(47, 8, 7, 3, 3);
    routine[1] = (DrawRectangleFn)Data_03001e50[47];
    Func_080072f0(canvas, 0x8000, 0, (void *)0x03000168);

    DMA3_SRC = (u32)canvas;
    DMA3_DST = 0x06003500;
    DMA3_CTRL = 0x84002000;
    DMA3_SRC = 0x05000000;
    DMA3_DST = (u32)work;
    DMA3_CTRL = 0x84000080;
    DMA3_SRC = 0x05000200;
    DMA3_DST = (u32)shade;
    DMA3_CTRL = 0x84000080;
    Func_080f6038(shade, (u16 *)0x05000200, 0, 256);
    Func_080f6038((u16 *)work, (u16 *)0x05000000, 0, 256);
    *(vu16 *)0x04000000 = 0x3740;

    if (Func_080772e0(228) == 1) {
        window = Func_08015010(6, 16, 18, 3, 6);
        M2C_FIELD(state, void **, 0x4C8) = window;
        Func_08015080(0x909, window, 0, 0);
    } else {
        window = Func_08015010(2, 16, 26, 4, 6);
        M2C_FIELD(state, void **, 0x4C8) = window;
        Func_08015080(0x908, window, 0, 0);
        Func_08015080(0x909, M2C_FIELD(state, void **, 0x4C8), 0, 8);
    }

    M2C_FIELD(work, s32 *, 0x7824) = 0;
    Func_080041d8((void *)0x080F6441, 1152);
    Func_080041d8((void *)0x080F60A1, 1152);

    frame = 0;
    while (M2C_FIELD(state, s32 *, 0x8C) != 10) {
        if (frame <= 16) {
            Func_080f6038(shade, (u16 *)0x05000200,
                frame << 12, 256);
            Func_080f6038((u16 *)work, (u16 *)0x05000000, frame << 12, 256);
        }

        if (M2C_FIELD(state, s32 *, 0x8C) == 3) {
            phase = Func_080022fc(frame, 80);
            if (phase <= 15) {
                Func_080f61e8((s32)&Value_00000091);
            } else if (phase <= 31) {
                Func_080f61e8((s32)&Value_00000093);
            } else if (phase <= 47) {
                Func_080f61e8((s32)&Value_000000b4);
            } else if (phase <= 63) {
                Func_080f61e8((s32)&Value_000000a0);
            } else {
                Func_080f61e8((s32)&Value_0000008f);
            }

            if (M2C_FIELD(state, s32 *, 0xA8) <= 15) {
                Func_080f6148();
            }
            if (M2C_FIELD(state, s32 *, 0xA8) > 16 && (frame & 7) == 0) {
                x = (s32)(((Func_08004458() & 0x7F) + 56) << 16);
                y = (s32)(((Func_08004458() & 0x1F) + 48) << 16);
                particle = (Particle *)tiles + ((frame / 8) & 3) * 256;
                for (i = 0; i != 256; i++) {
                    speed = (s32)(Func_08004458() & 0xFF) + 64;
                    angle = (s32)(Func_08004458() & 0xFFFF);
                    particle->x = x;
                    particle->y = y;
                    particle->vx = (speed * Func_08002322(angle)) >> 6;
                    particle->vy = (-(speed * Func_0800231c(angle))) >> 6;
                    particle->life = (s32)(Func_08004458() & 0xF) + 16;
                    particle++;
                }
            }

            particle = (Particle *)tiles;
            for (i = 0; i != 1024; i++) {
                if (particle->life > 0) {
                    px = particle->x;
                    particle->life--;
                    if ((u32)px <= 0x00FFFFFF) {
                        py = particle->y;
                        if (py <= 0x007FFFFF && py >= 0) {
                            half = Func_080022ec(particle->life, 12) + 1;
                            routine[i & 1](canvas,
                                sprites + Data_080f86f8[half - 1],
                                (px >> 16) - half,
                                (py >> 16) - half,
                                half * 2, half * 2);
                        }
                    }
                    particle->x += particle->vx;
                    particle->y += particle->vy;
                    particle->vx = particle->vx * 60 / 64;
                    particle->vy = particle->vy * 60 / 64;
                }
                particle++;
            }
        }

        if (M2C_FIELD(state, s32 *, 0x8C) == 0
            || M2C_FIELD(state, s32 *, 0x8C) == 2) {
            for (i = 0; i != 7; i++) {
                flags[i] = 0;
            }
            if (M2C_FIELD(state, s32 *, 0x8C) == 0) {
                flags[3] = 1;
                if (M2C_FIELD(state, s32 *, 0x98) > 1) {
                    flags[4] = 1;
                    flags[2] = 1;
                }
                if (M2C_FIELD(state, s32 *, 0x98) > 2) {
                    flags[5] = 1;
                    flags[1] = 1;
                }
                if (M2C_FIELD(state, s32 *, 0x98) > 3) {
                    flags[6] = 1;
                    flags[0] = 1;
                }
            } else if ((frame & 7) <= 3) {
                record = (s32 *)(state + 0xAC);
                for (i = 0; i != 7; i++) {
                    flags[i] = *record;
                    record++;
                }
            }

            for (k = 0; k != 3; k++) {
                rank = 65 - (k != 1);
                if (flags[1] != 0) {
                    Func_080f62b8(20, k + 19, 200, k + 19, rank);
                }
                if (flags[2] != 0) {
                    Func_080f62b8(28, k + 35, 200, k + 35, rank);
                }
                if (flags[3] != 0) {
                    Func_080f62b8(20, k + 51, 200, k + 51, rank);
                }
                if (flags[4] != 0) {
                    Func_080f62b8(28, k + 67, 200, k + 67, rank);
                }
                if (flags[5] != 0) {
                    Func_080f62b8(20, k + 83, 200, k + 83, rank);
                }
                if (flags[0] != 0) {
                    Func_080f62b8(28, k + 5, 200, k + 91, rank);
                }
                if (flags[6] != 0) {
                    Func_080f62b8(28, k + 97, 200, k + 11, rank);
                }
            }
        }

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    }

    for (i = 0; i != 17; i++) {
        scale = 0x10000 - (i << 12);
        Func_080f6038(shade, (u16 *)0x05000200, scale, 256);
        Func_080f6038((u16 *)work, (u16 *)0x05000000, scale, 256);
        Func_080030f8(1);
    }

    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((void *)0x080F60A1);
    Func_08004278((void *)0x080F6441);
    Func_08002dd8(45);
    Func_08002dd8(40);
    Func_08002dd8(39);
    Func_08002dd8(41);
}
