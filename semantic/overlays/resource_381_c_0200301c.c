typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 381, sparkle-field per-frame task at 0x0200301c
 * (752 bytes, 10 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7,fp / mov r6,sl / mov r5,r9 / push {r5,r6,r7} / mov r7,r8 /
 * push {r7}` and `sub sp, #68` at 0x0200301c, with the matching unwind at
 * 0x020032e2 ending `pop {r0} / bx r0`.  The popped branch register IS r0, so
 * it holds the return address and the owner is `void` — as it must be, since
 * 0x0200330c installs this address (pool word 0x0200b01d = 0x301c + the Thumb
 * bit) through Func_080000d0.
 *
 * Pool map by control-flow walk: bytes 0x020032f4-0x0200330b, six words, all
 * past the final `bx` and all reached only by `ldr rN,[pc,#imm]` from the
 * body.  Five are in-image data tables under the proven 0x02008000 base
 * (0x0200ba0c -> 0x3a0c, 0x0200ba2a -> 0x3a2a, 0x0200ba68 -> 0x3a68,
 * 0x0200ba90 -> 0x3a90, 0x0200ba48 -> 0x3a48; every one EVEN, so data) and
 * the sixth, 0x00001999, is a plain constant — it is the ground height the
 * bounce clamps to, and it sits below the link band so the band test rules it
 * out as an address immediately.
 *
 * Workspace layout is the one 0x0200330c builds and the byte-exact sibling
 * `assets/code/resource_381_c_02002ba0.c` corroborates: ten 40-byte particles
 * and a u16 count at byte 400.  The 68-byte frame is almost entirely spilled
 * copies of one particle's fields, which is why m2c-shaped drafts of this
 * function read as twenty unrelated stack slots; they are five accumulators, a
 * position triple, three table cursors, the entry pointer and the index.
 *
 * `tick` is a countdown, not a flag: `adds #255 / lsls #24 / lsrs #24` is a
 * u8 decrement, and a non-zero result branches straight to the write-back
 * tail, so the whole body — including all ten call sites — runs one frame in
 * three.  The same `+255` idiom decrements `blend` at 0x02003224.
 *
 * The three byte tables are read as interleaved triples: index `3*i` selects
 * the x/y/z members of Data_0200ba0c (jitter amplitude), Data_0200ba2a
 * (angular step) and Data_0200ba48 (a SIGNED direction, read with `ldrsb`,
 * whose only meaningful values are +1, -1 and "neither", the last of which
 * zeroes the accumulator).  The assembly spells the second and third members
 * through a pointer held at sp+4 rather than re-indexing; that is the same
 * address either way and is written as `[tri + 1]` / `[tri + 2]` here.
 *
 * `Func_030003f0` is a relocated IWRAM helper reached through the ordinary
 * veneer table with no r3/r4 load, so it is a plain two-argument call and not
 * a `call_via` site — the documented shape.  It divides the 16.16-shifted
 * jitter by 1000.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 10 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_030003f0 x3, Func_080000f8 x3, Func_08000118 x2, Func_08000120 x1,
 *   Func_08000148 x1.
 * The three Func_080000f8 draws and the three divides are separate sites — the
 * x, y and z jitters are computed independently and folding them into a loop
 * would deflate the multiset by four.
 */

/* Old-style declarations: overlay import arities vary per call site. */
u8 *Func_08000148();    /* fetch or allocate a workspace by id and size */
s32 Func_080000f8();    /* pseudo-random draw, used as a 16.16 fraction */
s32 Func_08000118();    /* sine of a binary angle */
s32 Func_08000120();    /* cosine of a binary angle */
s32 Func_030003f0();    /* relocated IWRAM divide */

extern u8 Data_0200ba0c[];      /* jitter amplitude, x/y/z per particle */
extern u8 Data_0200ba2a[];      /* angular step, x/y/z per particle */
extern s8 Data_0200ba48[];      /* jitter direction, x/y/z per particle */
extern s32 Data_0200ba68[];     /* per-particle apex height, 16.16 */
extern s32 Data_0200ba90[];     /* per-particle fall speed, 16.16 */

typedef struct {
    u8 *record;         /* +0  the scene entity this particle drives */
    s32 x, y, z;        /* +4  16.16 position accumulators */
    s32 px, py, pz;     /* +16 16.16 phase accumulators */
    s32 height;         /* +28 */
    s32 fall;           /* +32 */
    u8 tick;            /* +36 countdown between updates */
    u8 blend;           /* +37 ground-contact blend counter */
    u8 pad[2];
} Particle;

typedef struct {
    Particle particle[10];
    u16 count;          /* +400 */
} Field;

void Func_0200301c(void)
{
    Field *field;
    Particle *particle;
    u8 *record;
    u32 index;
    u32 tri;
    u32 blend;
    u32 tick;
    s32 px, py, pz;
    s32 height, fall;
    s32 x, y, z;
    s32 jx, jy, jz;
    s32 dx, dy, dz;
    s32 dir;

    field = (Field *)Func_08000148(33, 404);
    if (field->count == 0) {
        return;
    }

    particle = field->particle;
    index = 0;
    tri = 0;

    do {
        px = particle->px;
        py = particle->py;
        pz = particle->pz;
        height = particle->height;
        fall = particle->fall;
        record = particle->record;

        x = particle->x;
        y = particle->y;
        z = particle->z;

        blend = particle->blend;

        tick = (u8)(particle->tick - 1);
        if (tick != 0) {
            goto store;
        }
        tick = 3;

        if (blend == 0) {
            height += fall;
            if (height >= Data_0200ba68[index]) {
                /* Apex: turn the fall around. */
                fall = -Data_0200ba90[index];
            } else if (height <= 0x1999) {
                /* Ground contact: bounce, and blank the driven record for a
                 * 24-frame settle while the accumulators are held aside. */
                fall = Data_0200ba90[index];
                height = 0x1999;
                x = *(s32 *)(record + 8);
                y = *(s32 *)(record + 12);
                z = *(s32 *)(record + 16);
                *(s32 *)(record + 8) = 0;
                *(s32 *)(record + 12) = 0;
                *(s32 *)(record + 16) = 0;
                blend = 24;
            }
            *(s32 *)(record + 24) = height;
            *(s32 *)(record + 28) = height;
        }

        jx = (Data_0200ba0c[tri] * Func_080000f8()) >> 16;
        jy = (Data_0200ba0c[tri + 1] * Func_080000f8()) >> 16;
        jz = (Data_0200ba0c[tri + 2] * Func_080000f8()) >> 16;

        jx = jx != 0 ? Func_030003f0(jx << 16, 1000) : 0;
        jy = jy != 0 ? Func_030003f0(jy << 16, 1000) : 0;
        jz = jz != 0 ? Func_030003f0(jz << 16, 1000) : 0;

        dir = Data_0200ba48[tri];
        if (dir == 1) {
            px += jx;
        } else {
            px -= jx;
            if (dir != -1) {
                px = 0;
            }
        }

        dir = Data_0200ba48[tri + 1];
        if (dir == 1) {
            py += jy;
        } else {
            py -= jy;
            if (dir != -1) {
                py = 0;
            }
        }

        dir = Data_0200ba48[tri + 2];
        if (dir == 1) {
            pz += jz;
        } else {
            pz -= jz;
            if (dir != -1) {
                pz = 0;
            }
        }

        dx = Func_08000118(Data_0200ba2a[tri] * px) << 1;
        dy = Func_08000118(Data_0200ba2a[tri + 1] * py) << 1;
        dz = Func_08000120(Data_0200ba2a[tri + 2] * pz) << 1;

        if (blend != 0) {
            /* Settling: the offsets accumulate off-record and are pushed back
             * only on the frame the counter runs out. */
            x += dx;
            blend = (u8)(blend - 1);
            y += dy;
            z += dz;
            if (blend == 0) {
                *(s32 *)(record + 8) = x;
                *(s32 *)(record + 56) = x;
                if (jy != 0) {
                    *(s32 *)(record + 12) = y;
                    *(s32 *)(record + 60) = y;
                }
                *(s32 *)(record + 16) = z;
                *(s32 *)(record + 64) = z;
            }
        } else {
            *(s32 *)(record + 8) += dx;
            *(s32 *)(record + 56) = *(s32 *)(record + 8);
            if (jy != 0) {
                *(s32 *)(record + 12) += dy;
                *(s32 *)(record + 60) = *(s32 *)(record + 12);
            }
            *(s32 *)(record + 16) += dz;
            *(s32 *)(record + 64) = *(s32 *)(record + 16);
        }

store:
        particle->px = px;
        particle->py = py;
        particle->pz = pz;
        particle->height = height;
        particle->fall = fall;
        particle->blend = (u8)blend;
        particle->x = x;
        particle->y = y;
        particle->z = z;
        particle->tick = (u8)tick;

        tri += 3;
        index++;
        particle++;
    } while (index != field->count);
}
