#include "types.h"

/*
 * Func_080e15e8 - the complete "summon" cutscene driver.
 *
 * The manifest advertises only a 316-byte head (FunctionHead_080e15e8) that
 * allocates a 336-byte frame and then branches straight into sub_080e1724.
 * The real owner is one function whose body was laid out across nine manifest
 * rows, seven of which carry code and two of which are nothing but literal
 * pools.  Every inter-row edge in the original is a plain Thumb `b`, never a
 * `bl`, so no call site here stands in for local control flow.
 *
 * Settled executable ranges (literal pools and alignment excluded):
 *
 *     0x080e15e8 + 120     0x080e1acc + 734
 *     0x080e167c + 134     0x080e1df8 + 950
 *     0x080e1724 + 120     0x080e21e8 + 786
 *     0x080e17c4 + 106
 *     0x080e1848 + 592     (rows 080e17c4 tail and 080e1a48 head are adjacent)
 *
 *                                       total 3,542 executable bytes
 *
 * Excluded as data: 28 bytes interior to row 080e15e8 (0x080e1660), its
 * 32-byte trailing pool (0x080e1704), the 40-byte pool at 0x080e179c, two
 * alignment bytes plus the 24-byte pool at 0x080e182e, the 52-byte pool at
 * 0x080e1a98, and the two whole-row pools the scope tool still counted as
 * code: 0x080e1daa (78 bytes) and 0x080e21ae (58 bytes).  Both disassemble
 * into plausible `lsls`/`movs` pairs but every word in them is the target of
 * a sibling `ldr rN,[pc,#imm]`; they contain no reachable instruction.
 *
 * Call accounting: 130 `bl` sites.  112 are ordinary direct calls.  18 are
 * thunk-bank indirect calls (see the note below): 2x Func_080072f0 (via r3),
 * 12x Func_080072f4 (via r4), 2x Func_080072f8 (via r5), 1x Func_080072fc
 * (via r6) and 1x Func_08007304 (via r8).  Zero `bl` sites encode internal
 * branches.  In addition the original performs two calls that are *not* `bl`
 * at all: `mov ip,pc ; bx r7` at 0x080e19f8 and 0x080e1a12, entering the
 * relocated ARM helper at 0x03000118.  They are modelled below as calls
 * through ScaleFraction_080e15e8.
 *
 * THUNK BANK.  Func_080072e4..Func_08007318 is a `call_via rN` table, not a
 * set of functions.  `bl Func_080072f4` is an indirect call whose target is
 * whatever r4 holds, with r0-r3 and the stack carrying the callee's
 * arguments.  Every such site in this owner is therefore written here as a
 * call through an explicit function pointer, and the register operand is a
 * live value (a renderer entry read from 0x03001f08 / 0x03001f0c, or a
 * relocated IWRAM helper address), never scratch.
 */

typedef void (*Renderer_080e15e8)(void *target, const void *source,
                                  s32 x, s32 y, u32 width, s32 height);

/* 0x0300013c, reached through the call-via-r3 thunk with two arguments. */
typedef s32 (*ArmRatio_080e15e8)(s32 numerator, s32 denominator);

/* 0x03001388, reached through the call-via-r3 thunk with three arguments. */
typedef void (*ArmTransfer_080e15e8)(void *destination, const void *source,
                                     u32 count);

/* 0x03000168, reached through the call-via-r6 thunk. */
typedef void (*ArmFill_080e15e8)(void *destination, u32 size, u32 value);

/*
 * 0x03000118, entered inline with `mov ip,pc ; bx r7`.  Both uses take a
 * product and the constant 0x555 (65536/48 rounded down) and yield the
 * interpolated share of that product, so it behaves as a fixed-point scale.
 * The exact rounding of the relocated helper is not reconstructed here.
 */
typedef s32 (*ScaleFraction_080e15e8)(s32 value, s32 scale);

struct Spark_080e15e8 {
    s32 x;              /* 16.16 */
    s32 y;              /* 16.16 */
    s32 unused_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unused_14;
    s32 life;
};

struct Cast_080e15e8 {
    u8 reserved_00[20];
    s32 member_count;   /* +20 */
    u8 reserved_18[12];
    s16 member_id[1];   /* +36, one s16 per member */
};

#define REG16_080E15E8(address) (*(volatile u16 *)(address))

#define SPARKS_080E15E8      ((struct Spark_080e15e8 *)0x02010c58)
#define SPARK_COUNT_080E15E8 0x38e

s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 resource);
void *Func_08002f40(s32 resource);
void Func_080030f8(s32 frames);
s32 Func_080041d8(const void *task, u32 priority);
void Func_08004278(const void *task);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004bd4(s32 value);
void Func_08004c1c(s32 value);
void Func_08004c6c(s32 value);
void Func_08004cb4(const s32 *vector);
void Func_08004cf0(const s32 *vector);
void Func_08005258(s32 arg0, s32 arg1, s32 arg2);
void Func_08009038(void *handle);
void Func_080b5040(s32 arg0, u16 arg1, s32 arg2);
void Func_080b5088(s16 member, s32 arg1);
void Func_080b50e8(s32 sound);
void Func_080c9048(void);
void Func_080cd104(s32 arg0, s32 arg1);
void Func_080cd52c(void);
void Func_080cd594(s32 size);
void Func_080cdbc0(void);
void Func_080d40ec(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_080d6750(void *cast);
void Func_080d67dc(void);
void Func_080d6888(s16 member, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080dbb24(s32 arg0, s32 arg1, s32 arg2);
void Func_080e0524(s32 resource, void *destination, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, u32 arg1);
void Func_080e3908(struct Spark_080e15e8 *spark, s32 arg1, s32 arg2);
void Func_080e3944(const s32 *source, s32 *destination);
void Func_080e6d3c(s32 arg0, s32 arg1, s32 arg2);
void Func_080e6eac(s32 arg0, s32 arg1, s32 arg2);
void Func_080ed408(s32 resource, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 sound);

/*
 * Read-only tables in the same bank as this owner.  Their contents are ROM
 * data, not reconstructed here; only their addresses and element types are
 * evidenced by the loads.
 */
#define PATH_TABLE_080E15E8   ((const s16 *)0x080eda98)
#define SPRITE_OFFSET_080E15E8 ((const u16 *)0x080ede48)
#define BURST_OFFSET_080E15E8  ((const u16 *)0x080edebe)
#define BURST_WIDTH_080E15E8   ((const u8 *)0x080edeca)
#define BURST_HEIGHT_080E15E8  ((const u8 *)0x080eded0)
#define BURST_PALETTE_080E15E8 ((const u8 *)0x080eecae)
#define CHIME_TABLE_080E15E8   ((const u8 *)0x080eecaa)

/*
 * Fill up to `wanted` free spark slots.  The original inlines this three
 * times; the three copies differ only in the constants passed here, and all
 * three draw their random numbers in the same order (speed, angle, life).
 */
static void SpawnSparks_080e15e8(s32 x, s32 y,
                                 u32 speed_mask, s32 speed_bias,
                                 u32 angle_mask, s32 angle_bias,
                                 s32 life_bias, s32 wanted)
{
    struct Spark_080e15e8 *spark = SPARKS_080E15E8;
    s32 spawned = 0;
    s32 index;

    for (index = 0; index < SPARK_COUNT_080E15E8; index++, spark++) {
        s32 speed;
        s32 angle;

        if (spark->life != 0) {
            continue;
        }

        speed = (s32)(Func_08004458() & speed_mask);
        angle = (s32)(Func_08004458() & angle_mask) + angle_bias;

        spark->x = x;
        spark->y = y;
        speed += speed_bias;
        spark->velocity_x = (speed * Func_08002322(angle)) >> 7;
        spark->velocity_y = (-((speed * Func_0800231c(angle)) * 2)) >> 7;
        spark->life = (s32)(Func_08004458() & 7) + life_bias;

        spawned++;
        if (spawned == wanted) {
            break;
        }
    }
}

/* Chime every cast member once.  Used at the two spark bursts. */
static void ChimeCast_080e15e8(struct Cast_080e15e8 *const *slot,
                               s32 fixed_palette, s32 arg4)
{
    s32 member = 0;

    if ((*slot)->member_count == 0) {
        return;
    }

    do {
        s32 palette = fixed_palette;

        if (palette < 0) {
            /* The second burst randomises the palette per member. */
            palette = CHIME_TABLE_080E15E8[Func_08004458() & 3];
        }
        Func_080d6888((*slot)->member_id[member], palette, 5, member, arg4);
        Func_080b5088((*slot)->member_id[member], 4);
        member++;
    } while (member != (*slot)->member_count);
}

/* Signed halving, spelled in the original as `(v + (u32)(v>>31)>>31) >> 1`. */
static s32 Half_080e15e8(s32 value)
{
    return (value + (s32)((u32)(value >> 31) >> 31)) >> 1;
}

void Func_080e15e8(struct Cast_080e15e8 *cast)
{
    /*
     * Live frame state.  The offsets in the comments are the original
     * sp-relative slots inside the 336-byte frame; they are recorded because
     * the frame stays live across every row boundary of this owner.
     */
    u8 *workspace = *(u8 **)0x03001eec;              /* sp+128 */
    void *canvas = *(void **)0x03001ef0;             /* sp+124 */
    u8 *sprites = *(u8 **)0x03001ef4;                /* sp+120 */
    s32 *controller = *(s32 **)0x03001f00;           /* sp+132 */

    Renderer_080e15e8 overlay;                       /* sp+108, from 0x03001f08 */
    Renderer_080e15e8 sprite_draw;                   /* sp+88,  from 0x03001f0c */

    s32 step;                                        /* sp+116 */
    s32 slide_a = (s32)0xfff26c00;                   /* sp+16  */
    s32 slide_b = 0;                                 /* sp+20  */
    s32 slide_c = 0;                                 /* sp+24  */
    s32 slide_d = (s32)0xfffff460;                   /* sp+28  */
    s32 scroll_x = 0;                                /* sp+80  */
    s32 scroll_y = 0;                                /* sp+76  */
    s32 scroll_dx = 0;                               /* sp+72  */
    s32 scroll_dy = 0;                               /* sp+68  */
    s32 orbit_level = 0;                             /* sp+100 */
    s32 sprite_size = 0;                             /* sp+104 */

    s32 path[6][3];                                  /* sp+160 */
    s32 vector[3];                                   /* sp+292 */
    s32 shear[3];                                    /* sp+148 */
    s32 depth[3];                                    /* sp+136 */
    u8 fade[32];                                     /* sp+304 */

    s32 row;
    s32 column;
    s32 index;

    *(struct Cast_080e15e8 **)(workspace + 0x7828) = cast;
    Func_080cd594(0x2000);

    REG16_080E15E8(0x04000020) = 0x0100;
    Func_080e0524(0xbc, workspace, 1, 1);
    Func_080e0524(0xbc, workspace, 1, 1);
    Func_080e0524(0x75, workspace + 0x1800, 0, 0);
    Func_080e0524(0x73, sprites, 0, 0);

    /*
     * Build eight progressively dimmed copies of the 770-byte glyph strip
     * that resource 0x73 just produced.  The `< 0` guard is in the original
     * although the source bytes are unsigned and can never trip it.
     */
    for (row = 0; row < 8; row++) {
        const u8 *source = sprites;
        u8 *destination = workspace + 10000 + row * 770;
        s32 ceiling = 64 - row * 7;

        for (column = 0; column < 770; column++) {
            s32 level = *source++;

            if (level > ceiling) {
                level = ceiling;
            }
            if (level < 0) {
                level = 0;
            }
            *destination++ = (u8)level;
        }
    }

    Func_080c9048();

    *(volatile u16 *)0x05000000 = 0;
    *(volatile u16 *)0x05000002 = 0;
    REG16_080E15E8(0x0400000c) = 0x2784;

    *(s32 *)(workspace + 0x7790) = 0;
    *(s32 *)(workspace + 0x7794) = 2;
    *(s32 *)(workspace + 0x7798) = 1;
    *(s32 *)(workspace + 0x779c) = 0;
    controller[4] = 1;

    /* --- row 080e1724 ------------------------------------------------- */

    Func_080041d8((const void *)0x080c90e5, 0x480);
    Func_080041d8((const void *)0x080cd261, 0x480);

    *(s32 *)(workspace + 0x7780) = 0;
    Func_080cd104(0, 0);
    Func_080b5040(1, 0x3d, 0);
    *(s32 *)0x03001cf0 = 240;
    Func_080cd104(0, 1);

    REG16_080E15E8(0x04000020) = 0x0080;
    Func_080ed408(46, 7, 7, 3, 3);

    overlay = *(Renderer_080e15e8 *)0x03001f08;

    REG16_080E15E8(0x04000000) = 0x7741;
    REG16_080E15E8(0x04000020) = 0x0080;
    REG16_080E15E8(0x04000052) = 0x100f;
    REG16_080E15E8(0x04000050) = 0x3f44;
    *(s32 *)(workspace + 0x7780) = 2;

    /* --- row 080e17c4 ------------------------------------------------- */

    *(s32 *)(workspace + 0x7784) = 75;

    /*
     * A 32-entry brightness ramp, written outward from both ends of the
     * frame's last 32 bytes and clipped to the 6-bit colour range.
     */
    for (index = 0; index < 16; index++) {
        s32 level = 3 + 8 * index;

        fade[index] = (u8)level;
        fade[31 - index] = (u8)level;
        if (fade[index] > 63) {
            fade[index] = 63;
        }
        if (fade[31 - index] > 63) {
            fade[31 - index] = 63;
        }
    }

    REG16_080E15E8(0x0400000c) = 0x0784;

    step = 0;

    /*
     * ------------------------------------------------------------------
     * Phase 1: the approach.  The original enters this loop at its exit
     * test (0x080e1cc2) and jumps back to the body at 0x080e1848.
     * ------------------------------------------------------------------
     */
    for (;;) {
        s32 fade_row;
        s32 orbit;

        if (step == 170) {
            break;
        }

        sprite_size = 2;
        orbit_level = slide_b;

        if (step == 16) {
            Func_080f9010(140);
        }
        if (step == 132) {
            Func_080f9010(131);
        }
        if (step == 151) {
            Func_080f9010(145);
        }
        if ((*(u32 *)0x03001b04 & 3) != 0) {
            break;
        }

        /* ---- body (0x080e1848) ---- */

        Func_080d40ec(slide_b, 0xaaab, 0x5555, 0);

        if (step == 150) {
            *(s32 *)(workspace + 0x7780) = 1;
            *(s32 *)(workspace + 0x7784) = 0x1a1a1a1a;
        } else {
            *(s32 *)(workspace + 0x7780) = sprite_size;
            *(s32 *)(workspace + 0x7784) = 75;
        }

        {
            ArmRatio_080e15e8 ratio = (ArmRatio_080e15e8)0x0300013c;

            /* bl Func_080072f0 == call through r3 */
            Func_08005258(0x01fe0000, ratio(0x01fe0000, 0xc000), 0x7fff0000);
        }
        Func_080049ac();

        if (step > 128) {
            s32 lean = step - 128;

            if (lean > 22) {
                lean = 20;
            }
            shear[0] = 0;
            shear[1] = -lean << 17;
            shear[2] = 0x02000000;

            sprite_size = (lean >> 2) + 2;
            if (sprite_size > 8) {
                sprite_size = 8;
            }

            Func_08004cb4(shear);
            Func_08004bd4((s32)0xfffff000);
            Func_08004c6c(0x8000);
            Func_08004bd4(0x8000);
            Func_08004c1c(lean << 12);

            if (step > 150) {
                depth[0] = slide_a;
                depth[1] = slide_a;
                depth[2] = slide_a;
                sprite_size = 5;
            } else {
                s32 near = 0x10000 - ((lean * 3) << 10);

                depth[0] = near;
                depth[1] = near;
                depth[2] = near;
            }
            Func_08004cf0(depth);
        } else {
            shear[0] = 0;
            shear[1] = 0;
            shear[2] = 0x01000000;

            Func_08004cb4(shear);
            Func_08004bd4((s32)0xfffff000);
            Func_08004c6c(step << 8);
            Func_08004bd4(step << 8);
        }

        if (step <= 149) {
            /*
             * Six control points read from 0x080eda98 as (x, y) s16 pairs,
             * projected into screen space through Func_080e3944.
             */
            for (index = 0; index < 6; index++) {
                vector[0] = (PATH_TABLE_080E15E8[index * 2] - 96) << 16;
                vector[1] = 0;
                vector[2] = (PATH_TABLE_080E15E8[index * 2 + 1] - 96) << 16;

                Func_080e3944(vector, path[index]);

                path[index][0] = (path[index][0] >> 17) + 64;
                path[index][1] = *(s16 *)((u8 *)path[index] + 6) + 60;
            }

            /*
             * Three segments of the trail.  `span` walks from slide_c down in
             * steps of 48; each segment interpolates between two control
             * points using the relocated ARM helper at 0x03000118.
             */
            {
                ScaleFraction_080e15e8 scale =
                    (ScaleFraction_080e15e8)0x03000118;
                s32 span = slide_c;

                for (index = 0; index < 3; index++, span -= 48) {
                    s32 count = span - 256;
                    s32 t;

                    if (count > 48) {
                        count = 48;
                    }
                    if (count <= 0) {
                        continue;
                    }

                    for (t = 0; t < count; t++) {
                        s32 x = path[index * 2][0];
                        s32 y = path[index * 2][1];
                        u32 offset;

                        /* mov ip,pc ; bx r7 -- inline ARM helper entry */
                        x += scale(t * (path[index * 2 + 1][0] - x), 0x555);
                        y += scale(t * (path[index * 2 + 1][1] - y), 0x555);

                        offset = SPRITE_OFFSET_080E15E8[sprite_size - 1];

                        /* bl Func_080072f4 == call through r4 */
                        overlay(canvas, sprites + offset,
                                x - Half_080e15e8(sprite_size),
                                y - sprite_size,
                                (u32)sprite_size, sprite_size * 2);
                    }
                }
            }
        }

        if (step <= 179) {
            fade_row = 0;
            if (step > 155) {
                fade_row = step - 156;
            }
            if (fade_row > 7) {
                fade_row = 7;
            }

            if (step > 139) {
                Func_080ed408(47, 7, 7, 3, 2);
            } else {
                Func_080ed408(47, 7, 7, 3, 3);
            }
            sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;

            orbit = orbit_level;
            if (orbit > 128) {
                orbit = 128;
            }

            for (index = 0; index < orbit; index++) {
                s32 angle = index << 9;
                s32 size = sprite_size + 1;
                s32 strip = 770 * fade_row;
                u32 offset;

                vector[0] = (Func_08002322(angle) * 3) << 5;
                vector[2] = -((Func_0800231c(angle) * 3) << 5);
                /* vector[1] keeps the zero written by the control-point pass */

                Func_080e3944(vector, path[0]);

                path[0][0] = (path[0][0] >> 17) + 64;
                path[0][1] = *(s16 *)((u8 *)path[0] + 6) + 60;

                offset = SPRITE_OFFSET_080E15E8[size - 1];

                /* bl Func_080072f4 == call through r4 */
                sprite_draw(canvas,
                            workspace + strip + offset + 10000,
                            path[0][0] - Half_080e15e8(size),
                            path[0][1] - size,
                            (u32)size, size * 2);
            }

            Func_08002dd8(47);
        }

        if ((u32)(step - 151) <= 16) {
            s32 rows = slide_d;
            s32 top = 0;
            s32 line;

            if (step > 151) {
                top = step - 152;
            }
            if (top > 15) {
                top = 15;
            }

            /* Upper half of the ramp, sliding in. */
            for (line = 1; line < 10; line++) {
                if (top + line <= 15) {
                    /* bl Func_080072f8 == call through r5 */
                    overlay(canvas, &fade[top + line],
                            (s32)(top + 49 + line - 1), 16 - line,
                            (u32)(30 - (top * 2) - (line - 1) * 2), 1);
                }
            }

            /* Lower half, one line per accumulated row. */
            for (line = 0; line < rows; line++) {
                /* bl Func_080072f8 == call through r5 */
                overlay(canvas, &fade[top],
                        (s32)(top + 48), line + 16,
                        (u32)(32 - top * 2), 1);
            }

            Func_080ed408(47, 7, 7, 3, 2);
            sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;
            /* bl Func_080072f4 == call through r4 */
            sprite_draw(canvas, workspace, 32, slide_d - 56, 32, 96);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 7, 2);
            sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;
            /* bl Func_080072f4 == call through r4 */
            sprite_draw(canvas, workspace, 64, slide_d - 56, 32, 96);
            Func_08002dd8(47);
        }

        *(s32 *)(workspace + 0x7824) = 1;
        Func_080030f8(1);

        slide_d += 20;
        slide_c += 4;
        slide_b += 2;
        slide_a += 0x1800;
        step++;
    }

    /*
     * ------------------------------------------------------------------
     * Hand-over (0x080e1d00): retire the approach tasks, load the arrival
     * resources and reseed the spark field.
     * ------------------------------------------------------------------
     */
    controller[4] = 0;
    Func_08004278((const void *)0x080c90e5);
    Func_080d67dc();
    Func_080d6750(*(void **)(workspace + 0x7828));
    Func_080dbb24(9, 0x173, 1);

    Func_080e0524(0xce, (void *)0x02010000, 1, 0);
    Func_080e0524(0xd1, workspace, 1, 1);
    Func_080e0524(0x66, workspace + 0x6000, 1, 0);

    {
        ArmTransfer_080e15e8 transfer = (ArmTransfer_080e15e8)0x03001388;

        /* bl Func_080072f0 == call through r3 */
        transfer((void *)0x05000000, Func_08002f40(0xcf), 128);
    }

    Func_080e0524(0x74, sprites, 0, 0);
    REG16_080E15E8(0x04000052) = 0x1010;

    scroll_x = 0x00500000;
    scroll_y = 0x00400000;
    scroll_dx = 0;
    scroll_dy = 0;

    for (index = 0; index < SPARK_COUNT_080E15E8; index++) {
        SPARKS_080E15E8[index].life = 0;
    }

    /* Sixteen ambient motes carried in the workspace, not in the spark pool. */
    {
        struct Spark_080e15e8 *mote =
            (struct Spark_080e15e8 *)(workspace + 0x7080);

        for (index = 0; index < 16; index++, mote++) {
            mote->x = (s32)(Func_08004458() & 31) - 16;
            mote->y = (s32)(Func_08004458() & 63);
            mote->life = -(s32)(Func_08004458() & 15);
        }
    }

    *(s32 *)(workspace + 0x7780) = 2;
    *(s32 *)(workspace + 0x7784) = 50;
    Func_080f9010(145);

    /*
     * ------------------------------------------------------------------
     * Phase 2: the arrival, 192 frames.
     * ------------------------------------------------------------------
     */
    for (step = 0;; step++) {
        u8 *plate = workspace + 0x6000;
        s32 phase = step - 80;
        s32 flare_x;
        s32 flare_y;
        s32 flare_x2;
        s32 flare_y2;

        if (step == 20) {
            Func_080e0524(0x62, workspace, 1, 0);
        }

        if ((u32)phase <= 59 && (step & 7) == 0) {
            Func_080f9010(134);
        }
        if (step == 140) {
            Func_080b50e8(134);
        }

        /*
         * The original wraps the next block in a one-iteration loop over an
         * unused bank index, so the two 128-wide bank offsets it computes are
         * always zero.  It is written out flat here.
         */
        if (step >= 8 && step < 17) {
            if (step >= 9 && step < 12) {
                /* bl Func_080072f4 == call through r4 */
                overlay(canvas, workspace, 36, 0, 48, 112);
            }
            if (step >= 12 && step < 16) {
                /* bl Func_080072f4 == call through r4 */
                overlay(canvas, workspace + 0x1500, 36, 0, 48, 112);
            }
            if (step == 10) {
                SpawnSparks_080e15e8(60 << 16, 112 << 16,
                                     0x3ff, 32,
                                     0x7fff, (s32)0xffffc000,
                                     32, 512);
                *(s32 *)(workspace + 0x77a8) = 8;

                ChimeCast_080e15e8(
                    (struct Cast_080e15e8 *const *)(workspace + 0x7828), 7, 8);
            }
        }

        if ((u32)phase <= 63) {
            s32 slot;

            /* Six fixed frames clear the plate through the ARM fill helper. */
            {
                ArmFill_080e15e8 fill = (ArmFill_080e15e8)0x03000168;

                for (slot = 0; slot < 6; slot++) {
                    if (step == 80 + slot * 8) {
                        /* bl Func_080072fc == call through r6 */
                        fill(canvas, 0x4000, 0x10101010);
                    }
                }
            }

            for (slot = 0; slot < 30; slot++) {
                s32 base = 80 + slot * 2;

                if (step < base) {
                    continue;
                }
                if (step < base + 2) {
                    s32 jitter_x = (s32)(Func_08004458() & 3);
                    s32 jitter_y = (s32)(Func_08004458() & 3);
                    s32 tile;

                    Func_080ed408(47, 7, 7, 3, 2);
                    tile = Func_080022fc(slot, 3);

                    sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;
                    /* bl Func_080072f4 == call through r4 */
                    sprite_draw(canvas, workspace + tile * 5184,
                                jitter_x - 3, jitter_y + 32, 72, 72);
                    Func_08002dd8(47);
                }

                if (step != base) {
                    continue;
                }

                SpawnSparks_080e15e8(0x00100000, 0x00500000,
                                     0x3ff, 32,
                                     0xffff, 0,
                                     32, 16);
                *(s32 *)(workspace + 0x77a8) = 8;

                ChimeCast_080e15e8(
                    (struct Cast_080e15e8 *const *)(workspace + 0x7828), -1, 4);
            }

            /* The four-quadrant flare that follows the tile sweep. */
            {
                s32 radius = (s32)(Func_08004458() & 7) + 8;
                s32 angle = (s32)(Func_08004458() & 0xffff);

                flare_x = ((radius * Func_08002322(angle)) >> 16) + 60;
                flare_x2 = flare_x + 12;
                flare_y = 32 - ((radius * Func_0800231c(angle)) >> 16);
                flare_y2 = flare_y - 24;

                Func_080ed408(47, 7, 7, 3, 2);
                sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;
                /* bl Func_080072f4 == call through r4 */
                sprite_draw(canvas, plate, flare_x, flare_y2, 12, 24);
                Func_08002dd8(47);

                Func_080ed408(47, 7, 7, 7, 2);
                sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;
                /* bl Func_080072f4 == call through r4 */
                sprite_draw(canvas, plate, flare_x2, flare_y2, 12, 24);
                Func_08002dd8(47);

                Func_080ed408(47, 7, 7, 11, 2);
                sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;
                /* bl Func_080072f4 == call through r4 */
                sprite_draw(canvas, plate, flare_x, flare_y, 12, 24);
                Func_08002dd8(47);

                Func_080ed408(47, 7, 7, 15, 2);
                sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;
                /* bl Func_080072f4 == call through r4 */
                sprite_draw(canvas, plate, flare_x2, flare_y, 12, 24);
                Func_08002dd8(47);

                SpawnSparks_080e15e8(flare_x2 << 16, flare_y << 16,
                                     0x3f, 64,
                                     0xffff, 0,
                                     16, 4);
            }
        }

        /* ---- shared tail (0x080e22e0) ---- */

        Func_080ed408(47, 7, 7, 15, 2);
        sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;

        for (index = 0; index < SPARK_COUNT_080E15E8; index++) {
            struct Spark_080e15e8 *spark = &SPARKS_080E15E8[index];

            if (spark->life <= 0) {
                continue;
            }

            spark->life--;
            Func_080e3908(spark, 60, 0);

            if (spark->y > 0x00780000) {
                /* Bounce: halve and reverse the vertical velocity. */
                spark->velocity_y = Half_080e15e8(-spark->velocity_y);
                continue;
            }
            if ((u32)spark->x > 0x007effff || spark->y < 0) {
                continue;
            }

            {
                s32 life = spark->life;
                s32 size;
                s32 pitch;

                if (life < 0) {
                    life += 15;
                }
                size = (life >> 4) + 1;
                pitch = size * 2;

                /* bl Func_08007304 == call through r8 */
                sprite_draw(canvas,
                            sprites + SPRITE_OFFSET_080E15E8[size - 1],
                            (spark->x >> 16) - Half_080e15e8(size),
                            (spark->y >> 16) - size,
                            (u32)size, pitch);
            }
        }

        Func_08002dd8(47);

        if (step > 15) {
            if (step == 32) {
                scroll_dx = 0x00040000;
                scroll_dy = (s32)0xffffc000;
            }
            if (step > 31) {
                s32 damped;

                scroll_x += scroll_dx;
                scroll_y += scroll_dy;

                damped = scroll_dx * 60;
                if (damped < 0) {
                    damped += 63;
                }
                scroll_dx = damped >> 6;

                damped = scroll_dy * 60;
                if (damped < 0) {
                    damped += 63;
                }
                scroll_dy = damped >> 6;
            }
            Func_080e6d3c(0, scroll_x, scroll_y);
        }

        if ((u32)(step - 16) <= 127) {
            s32 centre = scroll_x >> 17;

            for (index = 0; index < 3; index++) {
                s32 slot = index & 3;
                s32 angle = (s32)(Func_08004458() & 0xffff);
                s32 x;
                s32 y;
                s32 palette;

                x = ((Func_08002322(angle) << 3) >> 16) + centre
                    - (BURST_WIDTH_080E15E8[slot] >> 1);
                y = (((Func_0800231c(angle) * 5) << 3) >> 16)
                    - (BURST_HEIGHT_080E15E8[slot] >> 1);

                palette = 3 | BURST_PALETTE_080E15E8[Func_08004458() & 3];
                Func_080ed408(47, 7, 7, palette, 2);

                sprite_draw = *(Renderer_080e15e8 *)0x03001f0c;
                /* bl Func_080072f4 == call through r4 */
                sprite_draw(canvas,
                            (u8 *)0x02010000 + BURST_OFFSET_080E15E8[slot],
                            x, y + 56,
                            BURST_WIDTH_080E15E8[slot],
                            BURST_HEIGHT_080E15E8[slot]);
                Func_08002dd8(47);
            }
        }

        if (step > 31) {
            Func_080e155c(4, 4);
        } else {
            Func_080e155c(8, 16);
        }

        Func_080cd52c();
        *(s32 *)(workspace + 0x7824) = 1;
        Func_080030f8(1);

        if (step + 1 == 192) {
            break;
        }
    }

    Func_08002dd8(46);
    Func_08004278((const void *)0x080cd261);
    Func_080e6eac(0, scroll_x, scroll_y);

    {
        void **handles = (void **)(workspace + 0x77d8);

        for (index = 0; index < 9; index++) {
            Func_08009038(*handles++);
        }
    }

    Func_080cdbc0();
}
