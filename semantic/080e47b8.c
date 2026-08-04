#include "types.h"

/*
 * Func_080e47b8 - cutscene / transition player.
 *
 * SCOPE NOTE (boundary evidence, do not re-litigate)
 * --------------------------------------------------
 * The manifest advertises 768 bytes at 080e47b8.  The real owner is one
 * function spanning sixteen manifest rows, 080e47b8 .. 080e660a, closed by the
 * sole epilogue at 080e660a.  The 184-byte frame allocated by `sub sp, #184`
 * and the r8..r11 lifetimes are live across every row, so the rows cannot be
 * split into separate functions.  Ten of the sixteen rows are executable; the
 * six rows carried in asm/executable_gaps/ are pure data:
 *
 *     080e4ddc(48) 080e5790(36) 080e5acc(48) 080e5dee(58) 080e6252(54)
 *         - literal pools
 *     080e53f4(136)
 *         - the 34-entry jump table for the second mode dispatch
 *
 * CALL ACCOUNTING (231 static `bl` sites)
 * ---------------------------------------
 * Fourteen of them are NOT calls.  They are long unconditional branches: the
 * Thumb `b` displacement (+/-2 KiB) cannot reach across a 7 KiB function, so
 * the original build emitted `bl` for the long edges.  lr is dead at every one
 * of these sites because the prologue already pushed lr onto the stack and the
 * epilogue returns through `pop {r0} ; bx r0`, never through lr.  They appear
 * below as ordinary C control flow:
 *
 *     bl 080e52f8  x1   -> loop back to the top of the frame body
 *     bl 080e640e  x9   -> `goto frame_tail`
 *     bl 080e657c  x1   -> skip the frame loop when frame_count == 0
 *     bl 080e65f8  x3   -> `return`
 *
 * Fifty-three more are interworking veneers, not distinct callees.  080072f0,
 * 080072f4 and 080072f8 are `bx r3`, `bx r4` and `bx r5` respectively, so each
 * site is an indirect call through the register loaded immediately before it:
 *
 *     bl 080072f4  x43  -> (*renderers[i])(...)      six-argument blit ABI
 *     bl 080072f8  x8   -> (*(FillFn)0x03000164)(dest, size)
 *     bl 080072f0  x2   -> (*(Fill32Fn)0x03000168)(dest, size, value)
 *
 * The remaining 164 are ordinary direct calls:
 *
 *     08004458 x30 random          08002dd8 x18 flush/commit request
 *     080e46f0 x13 play cue        080e0524 x13 load resource
 *     080e3944 x9  project point   08004278 x8  post callback
 *     080e38b8 x6  advance actor   080d6888 x5  camera shake
 *     080ed408 x4  window setup    080e396c x4  resolve anchor
 *     080049ac x4  matrix reset    080f9010 x3  play sound
 *     080cef64 x3  publish blitter 080b5098 x3  actor record
 *     080b5088 x3  set actor state 08002322 x3  sine
 *     080022ec x3  scaled divide   080dea70 x2  scene handoff
 *     080b5070 x2  actor height    08009088 x2  set object flags
 *     080051d8 x2  matrix compose  08004c1c x2  rotate Z
 *     08004bd4 x2  rotate X        080041d8 x2  install callback
 *     080030f8 x2  wait frames     0800231c x2  cosine
 *     080022fc x2  divide          08004c6c x1  rotate Y
 *     08009080 x1  clear flags     080cdb24 x1  fade variant A
 *     080cd594 x1  fade variant B  080cdbc0 x1  restore normal display
 *     080cd52c x1  per-frame flush 080df9d0 x1  palette upload
 *     080e155c x1  window enable
 *     080d9ac4 x1  scene handoff   080d52a4 x1  scene handoff
 *     080d4604 x1  scene handoff   080b50e8 x1  actor select
 *
 * UNCERTAINTIES are recorded inline next to the code they affect.
 */

/* ------------------------------------------------------------------ */
/* Target-fixed addresses                                             */
/* ------------------------------------------------------------------ */

#define REG16_080E47B8(address) (*(volatile u16 *)(address))
#define REG32_080E47B8(address) (*(volatile u32 *)(address))

#define REG_BLDCNT_080E47B8   0x04000050
#define REG_BLDALPHA_080E47B8 0x04000052
#define REG_BG2PA_080E47B8    0x04000020
#define REG_BG2PD_080E47B8    0x04000028

/* Runtime pointer block at 0x03001e80 / 0x03001eec. */
#define SCENE_BLOCK_080E47B8   0x03001eec
#define OBJECT_POOL_080E47B8   0x03001e80  /* SCENE_BLOCK - 108 */
#define BLIT_HOOK_080E47B8     0x03001f0c  /* 0x03001e50 + 188 */

/* Byte offsets inside the scene state block reached through `state`. */
#define STATE_STRIP_080E47B8    0x5100  /* 162 << 7 : palette strip source   */
#define STATE_WAVE_080E47B8     0x6980  /* 211 << 7 : 160 raster wave slots  */
#define STATE_ACTORS_080E47B8   0x7080  /* 225 << 7 : 64 actor records       */
#define STATE_TIMERS_080E47B8   0x7098  /* actor[i].timer of the same array  */
#define STATE_MODE_080E47B8     0x7780  /* 239 << 7                          */
#define STATE_SPEED_080E47B8    0x7784
#define STATE_LIMIT_080E47B8    0x77a8
#define STATE_DEPTH_080E47B8    0x77b4
#define STATE_PHASE_080E47B8    0x77b8
#define STATE_DIRTY_080E47B8    0x7824
#define STATE_CONTEXT_080E47B8  0x7828

/* IWRAM-resident transfer helpers reached through the `bx rN` veneers. */
#define IWRAM_FILL_080E47B8   0x03000164
#define IWRAM_FILL32_080E47B8 0x03000168

/* VRAM / EWRAM sources referenced by literal address. */
#define VRAM_BG_080E47B8      0x06004000
#define GFX_MAIN_080E47B8     0x02010000
#define GFX_PANEL_080E47B8    0x02010c56
#define GFX_SPARK_080E47B8    0x02013c56
#define GFX_SCRATCH_080E47B8  0x02014000
#define BG_SIZE_080E47B8      0x4000

/* ROM-resident geometry tables. */
#define TBL_SPARK_W_080E47B8    0x080ede48 /* u16, byte-indexed by (w - 2)  */
#define TBL_FLAME_W_080E47B8    0x080edeca /* u8  x4                        */
#define TBL_FLAME_H_080E47B8    0x080eded0 /* u8  x4                        */
#define TBL_FLAME_SRC_080E47B8  0x080edebe /* u16 x4                        */
#define TBL_FLAME_PAL_080E47B8  0x080eedd0 /* u8  x4                        */
#define TBL_RING_W_080E47B8     0x080ede9f /* u8  x12                       */
#define TBL_RING_H_080E47B8     0x080edea5 /* u8  x12                       */
#define TBL_RING_Y_080E47B8     0x080edeab /* u8  x12                       */
#define TBL_RING_SRC_080E47B8   0x080edeb2 /* u16 x12                       */
#define TBL_TITLE_X_080E47B8    0x080eedd4 /* u8 [variant][7]               */
#define TBL_TITLE_Y_080E47B8    0x080eede2 /* u8 x7                         */
#define TBL_SPRAY_SRC_080E47B8  0x080eedea /* u16 x4                        */

#define ROM_U8_080E47B8(base, index)   (((const u8 *)(base))[(index)])
#define ROM_U16_080E47B8(base, index)  (((const u16 *)(base))[(index)])
/* The spark table is addressed by a byte displacement, not an element index. */
#define SPARK_SRC_080E47B8(bytes) \
    (*(const u16 *)((const u8 *)TBL_SPARK_W_080E47B8 + (bytes)))

/* ------------------------------------------------------------------ */
/* Types                                                              */
/* ------------------------------------------------------------------ */

/*
 * Six-argument blit published by Func_080cef64 into a two-slot table.  The
 * assembly reaches it as `bl 080072f4`, which is the veneer `bx r4`, with r4
 * holding the table entry.  Arguments five and six travel on the stack.
 */
typedef void (*Blit_080e47b8)(s32 target, const void *source,
                              s32 x, s32 y, s32 width, s32 height);

typedef void (*Fill_080e47b8)(void *destination, s32 size);
typedef void (*Fill32_080e47b8)(void *destination, s32 size, u32 value);

/* 28-byte record shared by the scene actor array and the 0x02014000 scratch
 * array.  The first three words are a position for most modes but a countdown
 * pack for the intro shower at 080e4bc4; the middle three are Euler angles
 * there and a velocity everywhere else.  Left as neutral names because the
 * same storage is genuinely used both ways. */
struct Actor_080e47b8 {
    s32 word00;
    s32 word04;
    s32 word08;
    s32 word0c;
    s32 word10;
    s32 word14;
    s32 timer;
};

/* Scene context stored at state + STATE_CONTEXT.  Only the touched fields are
 * named; the record is larger. */
struct Context_080e47b8 {
    s32 field00;
    s32 field04;   /* variant selector: 0 = left-facing, 1 = right-facing  */
    s32 field08;   /* subject handle                                       */
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    s16 field24;   /* actor id, read signed                                */
};

/* Record returned indirectly by Func_080b5098. */
struct Subject_080e47b8 {
    s32 field00;
    s32 field04;
    s32 x;         /* +0x08 */
    s32 y;         /* +0x0c */
    s32 z;         /* +0x10 */
};

/* Object handled by Func_08009088 / Func_08009080. */
struct Object_080e47b8 {
    s32 field00;
    s32 field04;
    s32 x;         /* +0x08 */
    s32 y;         /* +0x0c */
    s32 z;         /* +0x10 */
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    s32 field24;
    s32 field28;
    s32 field2c;
    s32 field30;
    s32 field34;
    s32 field38;
    s32 field3c;
    s32 field40;
    s32 field44;
    s32 field48;
};

/* ------------------------------------------------------------------ */
/* External entry points                                              */
/* ------------------------------------------------------------------ */

void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdb24(s32 mode);
void Func_080cdbc0(void);
void Func_080cef64(s32 handle, Blit_080e47b8 *out_table);
void Func_080d4604(void *context, s32 request);
void Func_080d52a4(void *context);
void Func_080d6888(s32 actor, s32 a, s32 b, s32 c, s32 d);
void Func_080d9ac4(void *context);
void Func_080dea70(void *context, s32 request);
void Func_080df9d0(void *destination, const void *source, s32 count, s32 size);
void Func_080e0524(s32 resource, void *destination, s32 a, s32 b);
void Func_080e155c(s32 a, s32 b);
void Func_080e38b8(struct Actor_080e47b8 *actor, s32 a, s32 b);
void Func_080e3944(const struct Actor_080e47b8 *actor, s32 *out_point);
void Func_080e396c(s32 handle, s32 *out_point);
void Func_080e46f0(s32 cue);
s32 Func_080b5070(s32 actor);
void Func_080b5088(s32 actor, s32 state);
struct Subject_080e47b8 **Func_080b5098(s32 handle);
void Func_080b50e8(s32 actor);
void Func_080f9010(s32 sound);
void Func_08002dd8(s32 request);
s32 Func_080022ec(s32 value, s32 shift);
s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_080030f8(s32 frames);
void Func_080041d8(s32 callback, s32 size);
void Func_08004278(s32 callback);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004bd4(s32 angle);
void Func_08004c1c(s32 angle);
void Func_08004c6c(s32 angle);
void Func_080051d8(void *matrix, void *out);
void Func_08009080(struct Object_080e47b8 *object, s32 flags);
void Func_08009088(struct Object_080e47b8 *object, s32 flags);

/* ------------------------------------------------------------------ */
/* Static data reconstructed from the two dispatch tables              */
/* ------------------------------------------------------------------ */

/*
 * Jump table at 080e4924, 101 entries wide, collapsed to the resource id each
 * target loads.  Every target performs the same
 * Func_080e0524(id, GFX_SPARK, 1, 1); zero means the target was the shared
 * "no overlay" continuation at 080e4ade.
 */
static const u8 kOverlayId_080e47b8[101] = {
    0x94, 0x90, 0x92, 0x8e, 0x94, 0x8e, 0x90, 0x94,
    0x94, 0x94, 0x94, 0x94, 0x94, 0x94, 0x92, 0x92,
    0x92, 0x92, 0x92, 0x92, 0x8e, 0x8e, 0x8e, 0x8e,
    0x8e, 0x8e, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x94, 0x8e, 0x8e, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0x92
};

/*
 * Jump table at 080e53f4, 34 entries wide, collapsed the same way.  Every
 * reachable target is `Func_080e46f0(cue)`; zero means the entry pointed at
 * the shared fallthrough at 080e551a.
 */
static const u8 kCueId_080e47b8[34] = {
    0x8d, 0xa3, 0xa4, 0xb4, 0x00, 0x7d, 0x00, 0x00,
    0xc3, 0xa0, 0x8d, 0x00, 0xbb, 0xbb, 0x6f, 0x00,
    0x00, 0x00, 0xb9, 0xc0, 0xb4, 0x00, 0xb4, 0x7d,
    0x00, 0xbb, 0x00, 0x00, 0xa3, 0xa4, 0x00, 0x79,
    0x00, 0x53
};

/* ------------------------------------------------------------------ */
/* Small shared shapes                                                */
/* ------------------------------------------------------------------ */

/* The original divides signed values by two with the round-toward-zero
 * idiom `(v + (v >>> 31)) >> 1`.  Plain C division reproduces it. */
static s32 Half_080e47b8(s32 value)
{
    return value / 2;
}

static void Fill_BgAndTarget_080e47b8(s32 target)
{
    Fill_080e47b8 fill = (Fill_080e47b8)IWRAM_FILL_080E47B8;

    fill((void *)VRAM_BG_080E47B8, BG_SIZE_080E47B8);
    fill((void *)target, BG_SIZE_080E47B8);
}

/* ------------------------------------------------------------------ */

void Func_080e47b8(void *context_argument, s32 mode)
{
    u8 *state;
    s32 target;                 /* [sp,#88]  render target handle          */
    void *object_pool;          /* [sp,#80]                                */
    s32 tile_base;              /* [sp,#76]                                */

    struct Context_080e47b8 *context;
    struct Subject_080e47b8 *subject;   /* [sp,#28] */
    struct Object_080e47b8 *object;     /* r7 across the setup block */

    Blit_080e47b8 renderers[2]; /* [sp,#104]  published by Func_080cef64   */
    s32 scratch[3];             /* [sp,#112]                               */
    s32 point[3];               /* [sp,#124]                               */
    s32 drift[3];               /* [sp,#136]                               */
    s32 origin[3];              /* [sp,#148]                               */
    s32 anchor[3];              /* [sp,#160]                               */
    s32 focus[3];               /* [sp,#172]                               */

    s32 saved24, saved28, saved2c, saved34, saved48;
    s32 camera_z = 0;           /* [sp,#72]  only live for mode 32         */
    s32 camera_dz = 0;          /* [sp,#68]                                */
    s32 frame_count;            /* [sp,#64]                                */
    s32 frame;                  /* [sp,#84]                                */
    s32 warmup;                 /* the 0..31 preroll counter, same slot    */
    s32 mode_minus_two;         /* [sp,#20]                                */
    s32 mode_minus_four;        /* [sp,#16]                                */
    s32 i;
    s32 active;
    s32 value;

    state       = *(u8 **)SCENE_BLOCK_080E47B8;
    target      = *(s32 *)(SCENE_BLOCK_080E47B8 + 4);
    object_pool = *(void **)OBJECT_POOL_080E47B8;
    tile_base   = *(s32 *)(SCENE_BLOCK_080E47B8 + 8);

    *(void **)(state + STATE_CONTEXT_080E47B8) = context_argument;

    if (mode == 11 || mode == 8 || mode == 32) {
        Func_080cdb24(0);
    } else {
        Func_080cd594(0);
    }

    REG16_080E47B8(REG_BLDALPHA_080E47B8) = 0x1010;

    Func_080e0524(0x73, (void *)tile_base, 0, 0);
    Func_080e0524(0x96, state, 1, 0);
    Func_080e0524(0x99, (void *)GFX_MAIN_080E47B8, 1, 0);
    Func_080df9d0((void *)GFX_MAIN_080E47B8, state + STATE_STRIP_080E47B8,
                  40, 288);

    /* Per-mode backdrop resource. */
    if (mode == 5 || mode == 23) {
        Func_080e0524(0x7d, (void *)GFX_MAIN_080E47B8, 1, 0);
    } else if (mode == 12) {
        Func_080e0524(0xa9, (void *)GFX_MAIN_080E47B8, 1, 0);
    } else if (mode == 6 || mode == 27) {
        Func_080e0524(0xce, (void *)GFX_MAIN_080E47B8, 1, 0);
        Func_080e0524(0xc4, (void *)GFX_PANEL_080E47B8, 1, 0);
    } else if (mode == 31) {
        Func_080e0524(0x79, (void *)GFX_MAIN_080E47B8, 1, 1);
    } else if (mode == 8) {
        Func_080e0524(0xc3, (void *)GFX_MAIN_080E47B8, 1, 1);
    } else if (mode == 14) {
        Func_080e0524(0x6f, (void *)GFX_MAIN_080E47B8, 1, 0);
    } else if (mode == 30) {
        Func_080e0524(0xce, (void *)GFX_MAIN_080E47B8, 1, 0);
    } else if (mode == 16) {
        Func_080e0524(0xb8, (void *)GFX_MAIN_080E47B8, 1, 0);
    } else if (mode == 20) {
        Func_080e0524(0xb4, (void *)GFX_MAIN_080E47B8, 1, 0);
    } else if ((u32)(mode - 33) <= 1) {
        Func_080e0524(0x53, (void *)GFX_MAIN_080E47B8, 1, 0);
    } else if (mode != 11 && mode != 32) {
        Func_080e0524(0x9e, (void *)GFX_MAIN_080E47B8, 1, 0);
    }

    /* Jump table at 080e4924.  Modes above 100 skip it entirely. */
    if ((u32)mode <= 100 && kOverlayId_080e47b8[mode] != 0) {
        Func_080e0524(kOverlayId_080e47b8[mode],
                      (void *)GFX_SPARK_080E47B8, 1, 1);
    }

    *(s32 *)(state + STATE_MODE_080E47B8) = 2;
    *(s32 *)(state + STATE_SPEED_080E47B8) = (mode == 12) ? 75 : 50;

    Func_080041d8(0x080cd261, 0x480);

    context = *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
    Func_080e396c(context->field24, anchor);
    Func_080e396c(context->field08, focus);
    Func_080cef64(context->field04, renderers);

    *(s32 *)(state + STATE_DEPTH_080E47B8) = 24;
    *(s32 *)(state + STATE_PHASE_080E47B8) = 0;

    object = (struct Object_080e47b8 *)Func_080b5098(context->field08)[0];

    /* Seed the 64 scene actors as a falling shower.  word00 is a countdown
     * here, word0c/10/14 are the Euler angles fed to the matrix helpers. */
    {
        struct Actor_080e47b8 *actor =
            (struct Actor_080e47b8 *)(state + STATE_ACTORS_080E47B8);

        for (i = 0; i < 64; i++) {
            actor[i].word00 = (s32)(Func_08004458() & 0x3f) + 32;
            actor[i].word04 = 0;
            actor[i].word08 = 0;
            actor[i].word0c = (s32)(Func_08004458() & 0xffff);
            actor[i].word10 = (s32)(Func_08004458() & 0xffff);
            actor[i].word14 = (s32)(Func_08004458() & 0xffff);
        }
    }

    Func_08009088(object, 0);

    origin[0] = object->x;
    origin[1] = object->y + 0x00500000;
    origin[2] = object->z;

    saved24 = object->field24;
    saved28 = object->field28;
    saved2c = object->field2c;
    saved34 = object->field34;
    saved48 = object->field48;
    object->field24 = 0;
    object->field28 = 0;
    object->field2c = 0;
    object->field34 = 0;
    object->field48 = 0;

    context = *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
    Func_080e396c(context->field08, focus);
    focus[0] = Half_080e47b8(focus[0]);

    Func_080f9010(212);

    /* -------------------------------------------------------------- */
    /* 32-frame preroll shower                                        */
    /* -------------------------------------------------------------- */
    for (warmup = 0; warmup != 32; warmup++) {
        struct Actor_080e47b8 *actor =
            (struct Actor_080e47b8 *)(state + STATE_ACTORS_080E47B8);

        active = 0;
        for (i = 0; i < 64; i++, actor++) {
            if (actor->word00 < 0) {
                continue;
            }

            /* The staggered release threshold uses i/4 with the same
             * round-toward-zero shape the compiler emitted. */
            if (warmup >= i / 4) {
                Func_080049ac();
                Func_08004c6c(actor->word14);
                Func_08004bd4(actor->word0c);
                Func_08004c1c(actor->word10);
                Func_080e3944(actor, point);

                point[0] = Half_080e47b8(point[0]) + focus[0];
                if (mode <= 7) {
                    point[1] = point[1] + focus[1] - 8;
                } else if (mode == 35) {
                    point[1] = point[1] + focus[1] + 44;
                } else {
                    point[1] = point[1] + focus[1] + 12;
                }
                if (point[2] < -60) {
                    point[2] = -60;
                }
                if (point[2] > 60) {
                    point[2] = 60;
                }
                point[2] += 60;

                renderers[0](target,
                             (const void *)(tile_base + SPARK_SRC_080E47B8(8)),
                             point[0] - 2, point[1] - 5, 5, 10);

                actor->word00 -= 4;
            }
            active++;
        }

        if (mode <= 7 && active <= 63) {
            Func_080049ac();
            Func_080051d8(object_pool, (u8 *)object_pool + 12);
            Func_080e3944((const struct Actor_080e47b8 *)origin, point);
            point[0] = Half_080e47b8(point[0]);
            renderers[0](target, (const void *)GFX_SPARK_080E47B8,
                         point[0] - 10, point[1] - 4, 20, 40);
        }

        *(s32 *)(state + STATE_DIRTY_080E47B8) = 1;
        Func_080030f8(1);
    }

    context = *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);

    if (mode == 11) {
        REG16_080E47B8(REG_BG2PA_080E47B8) = 0x0100;
        if (context->field04 == 0) {
            REG32_080E47B8(REG_BG2PD_080E47B8) =
                (u32)((warmup - anchor[0]) << 8);
        } else {
            REG32_080E47B8(REG_BG2PD_080E47B8) = (u32)((96 - anchor[0]) << 8);
        }
    }

    if (mode == 32) {
        REG16_080E47B8(REG_BG2PA_080E47B8) = 0x0100;
        if (context->field04 == 0) {
            camera_z  = (s32)0xff800000;
            camera_dz = 0x000c0000;
        } else {
            camera_z  = 0x00080000;
            camera_dz = (s32)0xfff40000;
        }
        REG32_080E47B8(REG_BG2PD_080E47B8) = (u32)((camera_z >> 16) << 8);
    }

    if (mode == 8) {
        REG16_080E47B8(REG_BG2PA_080E47B8) = 0x0100;
        REG32_080E47B8(REG_BG2PD_080E47B8) = (u32)((64 - anchor[0]) << 8);
        *(s32 *)(state + STATE_MODE_080E47B8) = 1;
        *(s32 *)(state + STATE_SPEED_080E47B8) = 0;
        Fill_BgAndTarget_080e47b8(target);
        REG16_080E47B8(REG_BLDCNT_080E47B8) = 0;
    }

    if (mode == 31) {
        REG16_080E47B8(REG_BG2PA_080E47B8) = 0x0100;
        value = (context->field04 == 0) ? 32 : 96;
        REG32_080E47B8(REG_BG2PD_080E47B8) = (u32)((value - anchor[0]) << 8);
    }

    /* Modes that abandon the sequence right here. */
    if (mode == 15 || mode == 17 || mode == 24 || mode == 26) {
        Fill_BgAndTarget_080e47b8(target);
        (*(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8))
            ->field1c = 0;
        Func_08004278(0x080cd4b5);
        Func_08004278(0x080cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        Func_08009080(object, 3);
        if (mode == 15) {
            Func_080dea70(context_argument, 9);
        }
        if (mode == 24) {
            Func_080d52a4(context_argument);
        }
        if (mode == 26) {
            Func_080dea70(context_argument, 8);
        }
        return;                                 /* bl 080e65f8 */
    }

    Func_08009088(object, 16);
    object->field24 = saved24;
    object->field28 = saved28;
    object->field2c = saved2c;
    object->field34 = saved34;
    object->field48 = saved48;

    if (mode == 35) {
        Fill_BgAndTarget_080e47b8(target);
        (*(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8))
            ->field1c = 0;
        Func_08004278(0x080cd4b5);
        Func_08004278(0x080cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        ((struct Context_080e47b8 *)context_argument)->field18 = 3;
        Func_080d4604(context_argument, 2);
        return;                                 /* bl 080e65f8 */
    }

    context = *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
    subject = Func_080b5098(context->field24)[0];

    drift[0] = Func_080022ec(subject->x - origin[0], 6);
    drift[1] = Func_080022ec(subject->y - origin[1] + 0x001e0000, 6);
    drift[2] = Func_080022ec(subject->z - origin[2], 6);

    {
        struct Actor_080e47b8 *actor =
            (struct Actor_080e47b8 *)(state + STATE_ACTORS_080E47B8);

        for (i = 0; i < 64; i++) {
            actor[i].timer = 0;
        }
    }

    if (mode != 14) {
        struct Actor_080e47b8 *actor =
            (struct Actor_080e47b8 *)(state + STATE_ACTORS_080E47B8);
        s32 height = Half_080e47b8(Func_080b5070(context->field24));

        for (i = 0; i < 32; i++, actor++) {
            actor->word04 = height;
            actor->word00 = subject->x;
            actor->word08 = subject->z;
            actor->word0c = ((s32)(Func_08004458() & 0xff) - 127) << 12;
            if (mode == 31) {
                actor->word10 = ((s32)(Func_08004458() & 0xff) - 64) << 10;
            } else {
                actor->word10 = ((s32)(Func_08004458() & 0xff) - 64) << 12;
            }
            actor->word14 = ((s32)(Func_08004458() & 0xff) - 127) << 12;
            actor->timer = i / 2 + 32;
        }
    }

    if (mode == 11) {
        Func_080e0524(0xab, state, 1, 1);
        Func_080e0524(0xac, (void *)GFX_MAIN_080E47B8, 1, 0);
        REG16_080E47B8(REG_BLDALPHA_080E47B8) = 0x0e10;
    }

    if (mode == 32) {
        Func_080e0524(0xad, state, 1, 1);
        Func_080e0524(0xae, (void *)GFX_MAIN_080E47B8, 1, 0);
        REG16_080E47B8(REG_BLDALPHA_080E47B8) = 0x0e10;
    }

    if (mode != 7 && mode != 13 && mode != 18 &&
        mode != 11 && mode != 32 && mode != 19) {
        struct Actor_080e47b8 *actor =
            (struct Actor_080e47b8 *)GFX_SCRATCH_080E47B8;
        s32 lift = (mode == 12) ? 0 : 0x00140000;

        for (i = 0; i < 64; i++, actor++) {
            actor->word04 = lift;
            actor->word00 = subject->x;
            actor->word08 = subject->z;
            if (mode == 5 || mode == 23) {
                actor->word0c = ((s32)(Func_08004458() & 0xff) - 127) << 11;
                actor->word10 = (s32)(Func_08004458() & 0xff) << 11;
                actor->word14 = ((s32)(Func_08004458() & 0xff) - 127) << 11;
            } else if (mode == 25) {
                actor->word0c = ((s32)(Func_08004458() & 0xff) - 127) << 11;
                actor->word10 = (s32)(Func_08004458() & 0x7f) << 10;
                actor->word14 = ((s32)(Func_08004458() & 0xff) - 127) << 11;
            } else {
                actor->word0c = ((s32)(Func_08004458() & 0xff) - 127) << 10;
                actor->word10 = (s32)(Func_08004458() & 0x7f) << 10;
                actor->word14 = ((s32)(Func_08004458() & 0xff) - 127) << 10;
            }
            actor->timer = 0;
        }
    }

    mode_minus_two = mode - 2;
    if ((u32)mode_minus_two <= 1 || mode == 12 || mode == 22 ||
        mode == 29 || mode == 28) {
        Func_080041d8(0x080dbb9d, 0x480);
    }

    mode_minus_four = mode - 4;
    if ((u32)mode_minus_four <= 2 || mode == 23 || mode == 30 || mode == 27 ||
        mode == 33 || mode == 34 || mode == 100) {
        frame_count = 32;
    } else if (mode <= 3 || mode == 8 || mode == 9 || mode == 10 ||
               mode == 22 || mode == 25 || mode == 29 || mode == 31 ||
               mode == 14) {
        frame_count = 48;
    } else if (mode == 21) {
        frame_count = 20;
    } else if (mode == 11 || mode == 32 || mode == 20) {
        frame_count = 40;
    } else if (mode == 28 || mode == 12) {
        frame_count = 64;
    } else {
        frame_count = 80;
    }

    /* -------------------------------------------------------------- */
    /* Main frame loop.  `bl 080e52f8` is its back edge; `bl 080e657c` */
    /* at 080e52f4 is the zero-length guard.                          */
    /* -------------------------------------------------------------- */
    for (frame = 0; frame < frame_count; frame++) {

        if (mode != 11 && mode != 32) {
            s32 *wave = (s32 *)(state + STATE_WAVE_080E47B8);
            s32 phase = frame << 12;

            for (i = 0; i < 160; i++) {
                wave[i] = (0x00040000 - (Func_08002322(phase) << 2)) >> 10;
                phase += 0x800;
            }
        }

        if (frame <= 2) {
            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
            Func_080e396c(context->field08, focus);
            focus[0] = Half_080e47b8(focus[0]);
            focus[1] += 16;
        }

        if (mode != 11 && mode != 8 && mode != 32 &&
            mode != 33 && mode != 34 && frame <= 11) {
            s32 step = 3456 * (frame / 2);

            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
            if (context->field04 == 0) {
                renderers[0](target, state + step,
                             focus[0] - 32, focus[1] - 40, 48, 72);
            } else {
                renderers[0](target, state + step,
                             focus[0], focus[1] - 40, 48, 72);
            }
        }

        /* Jump table at 080e53f4. */
        if (mode <= 33 && kCueId_080e47b8[mode] != 0) {
            Func_080e46f0(kCueId_080e47b8[mode]);
        }

        if (mode != 11 && mode != 8 && mode != 32) {
            if ((u32)(frame - 4) <= 11) {
                s32 step = 960 * ((frame - 4) / 2);

                renderers[1](target, state + step + STATE_STRIP_080E47B8,
                             Half_080e47b8(anchor[0]) - 8, focus[1] - 24,
                             20, 48);
            }

            Func_080049ac();
            Func_080051d8(object_pool, (u8 *)object_pool + 12);

            if (frame > 3) {
                for (i = 0; i < 128; i++) {
                    s32 slot = i / 2;
                    struct Actor_080e47b8 *actor =
                        (struct Actor_080e47b8 *)
                        (state + STATE_ACTORS_080E47B8) + slot;
                    s32 life = actor->timer;
                    s32 half;
                    s32 span;

                    if (life <= 0) {
                        continue;
                    }
                    Func_080e3944(actor, scratch);
                    half = (life >> 4) + 1;
                    span = half * 2;
                    scratch[0] = Half_080e47b8(scratch[0]);

                    renderers[slot & 1](
                        target,
                        (const void *)(tile_base + SPARK_SRC_080E47B8(span - 2)),
                        scratch[0] - half / 2, scratch[1] - half,
                        half, span);

                    Func_080e38b8(actor, 60, (s32)0xfffff000);
                    actor->timer -= 1;
                }
            }
        }

        if (mode == 7 || mode == 13 || mode == 18 || mode == 19) {
            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);

            if (frame == 50) {
                Func_080d6888(context->field08, 7, -1, -1, 0);
            }
            if (frame == 79) {
                Func_080d6888(context->field08, 0, -1, -1, 0);
            }
            if (frame == 12) {
                struct Actor_080e47b8 *actor =
                    (struct Actor_080e47b8 *)GFX_SCRATCH_080E47B8;

                for (i = 0; i < 64; i++, actor++) {
                    actor->word00 = subject->x;
                    actor->word04 = 0x00140000;
                    actor->word08 = subject->z;
                    actor->word0c =
                        ((s32)(Func_08004458() & 0xff) - 128) << 10;
                    actor->word10 =
                        ((s32)(Func_08004458() & 0xff) - 128) << 10;
                    actor->word14 =
                        ((s32)(Func_08004458() & 0xff) - 128) << 10;
                    actor->timer = 0;
                }
            }
            if (frame <= 11) {
                goto frame_tail;                /* bl 080e640e */
            }

            {
                struct Subject_080e47b8 *homing =
                    Func_080b5098(context->field08)[0];
                s32 lift = Half_080e47b8(Func_080b5070(context->field08));
                struct Actor_080e47b8 *actor =
                    (struct Actor_080e47b8 *)GFX_SCRATCH_080E47B8;

                for (i = 0; i < 32; i++, actor++) {
                    s32 size;
                    s32 span;

                    if (actor->timer < 0) {
                        continue;
                    }
                    size = (i & 1) + 6;
                    Func_080e3944(actor, scratch);
                    scratch[0] >>= 1;
                    span = size * 2;

                    renderers[0](
                        target,
                        (const void *)(tile_base +
                                       SPARK_SRC_080E47B8(span - 2)),
                        scratch[0] - (size >> 1), scratch[1] - size,
                        size, span);

                    Func_080e38b8(actor, 62, 0);

                    if (frame > i + 22) {
                        s32 dx = (homing->x - actor->word00) >> 8;
                        s32 dy = (homing->y + lift - actor->word04) >> 8;
                        s32 dz = (homing->z - actor->word08) >> 8;

                        actor->word0c += dx;
                        actor->word10 += dy;
                        actor->word14 += dz;

                        if ((u32)(dx + 0xfff) <= 0x1ffe &&
                            (u32)(dz + 0xfff) <= 0x1ffe) {
                            actor->timer = -1;
                        }
                    }
                }
            }
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 21) {
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 6 || mode == 27) {
            if ((u32)(frame - 6) <= 13) {
                s32 step = frame;

                for (i = 0; i < 2; i++, step += 3) {
                    s32 cell = 3 * ((step / 2) & 3);

                    renderers[0](target,
                                 (const void *)(GFX_PANEL_080E47B8 +
                                                960 * cell),
                                 Half_080e47b8(anchor[0]) - 8, 0, 24, 104);
                }
            }
            if ((u32)(frame - 8) > 15) {
                goto frame_tail;                /* bl 080e640e */
            }

            for (i = 0; i < 3; i++) {
                s32 slot = i & 3;
                s32 angle = (s32)(Func_08004458() & 0xffff);
                s32 x;
                s32 y;
                s32 palette;

                x = ((Func_08002322(angle) << 3) >> 16)
                    + Half_080e47b8(anchor[0])
                    - (ROM_U8_080E47B8(TBL_FLAME_W_080E47B8, slot) >> 1);
                y = ((Func_0800231c(angle) << 5) >> 16)
                    - (ROM_U8_080E47B8(TBL_FLAME_H_080E47B8, slot) >> 1);

                Func_08002dd8(47);
                Func_08002dd8(46);

                palette = 3 | ROM_U8_080E47B8(TBL_FLAME_PAL_080E47B8,
                                              Func_08004458() & 3);
                Func_080ed408(47, 7, 7, palette, 2);

                ((Blit_080e47b8)*(s32 *)BLIT_HOOK_080E47B8)(
                    target,
                    (const void *)(ROM_U16_080E47B8(TBL_FLAME_SRC_080E47B8,
                                                    slot) + GFX_MAIN_080E47B8),
                    x, y + 56,
                    ROM_U8_080E47B8(TBL_FLAME_W_080E47B8, slot),
                    ROM_U8_080E47B8(TBL_FLAME_H_080E47B8, slot));

                Func_08002dd8(47);
                context = *(struct Context_080e47b8 **)
                    (state + STATE_CONTEXT_080E47B8);
                Func_080cef64(context->field04, renderers);
            }
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 14) {
            s32 centre;
            s32 slide;
            s32 scroll;
            s32 hook;

            Func_08002dd8(47);
            Func_08002dd8(46);
            if (frame > 23) {
                goto publish_blitter;
            }

            centre = Half_080e47b8(anchor[0]);
            slide  = (frame << 5) - 232;
            if (slide > 0) {
                slide = 0;
            }
            scroll = (frame << 4) - 48;
            while (scroll > 104) {
                scroll -= 104;
            }

            Func_080ed408(47, 7, 7, 3, 2);
            hook = *(s32 *)BLIT_HOOK_080E47B8;

            ((Blit_080e47b8)hook)(target, (const void *)GFX_MAIN_080E47B8,
                                  centre - 8, slide + scroll - 104, 17, 104);
            hook = *(s32 *)BLIT_HOOK_080E47B8;
            ((Blit_080e47b8)hook)(target, (const void *)GFX_MAIN_080E47B8,
                                  centre - 8, slide + scroll, 17,
                                  104 - scroll);
            hook = *(s32 *)BLIT_HOOK_080E47B8;
            ((Blit_080e47b8)hook)(target, (const void *)0x020106e8,
                                  centre - 17, slide + 47, 34, 65);

            Func_08002dd8(47);

            if (frame == 8) {
                *(s32 *)(state + STATE_LIMIT_080E47B8) = 8;
            }
            if (frame <= 1) {
                goto publish_blitter;
            }

            {
                struct Actor_080e47b8 *actor =
                    (struct Actor_080e47b8 *)(state + STATE_ACTORS_080E47B8);
                s32 spawned = 0;

                for (i = 0; i < 64; i++, actor++) {
                    if (actor->timer != 0) {
                        continue;
                    }
                    actor->word00 = subject->x;
                    actor->word04 = 0x00140000;
                    actor->word08 = subject->z;
                    actor->word0c =
                        ((s32)(Func_08004458() & 0xff) - 127) << 12;
                    actor->word10 =
                        ((s32)(Func_08004458() & 0xff) - 64) << 10;
                    actor->word14 =
                        ((s32)(Func_08004458() & 0xff) - 127) << 12;
                    actor->timer = i / 2 + 32;
                    if (++spawned == 4) {
                        break;
                    }
                }
            }
            goto publish_blitter;
        }

        if (mode == 31) {
            s32 centre;

            Func_08002dd8(47);
            Func_08002dd8(46);
            if ((u32)(frame - 4) > 19) {
                goto publish_blitter;
            }

            centre = Half_080e47b8(anchor[0]);

            Func_080ed408(47, 7, 7, 3, 2);
            ((Blit_080e47b8)*(s32 *)BLIT_HOOK_080E47B8)(
                target, (const void *)GFX_MAIN_080E47B8,
                centre - 24, 48, 24, 48);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 7, 2);
            ((Blit_080e47b8)*(s32 *)BLIT_HOOK_080E47B8)(
                target, (const void *)GFX_MAIN_080E47B8,
                centre, 48, 24, 48);
            Func_08002dd8(47);
            goto publish_blitter;
        }

        if (mode == 30) {
            s32 cell;
            s32 y;

            if (frame > 15) {
                REG16_080E47B8(REG_BLDALPHA_080E47B8) =
                    (u16)((32 - frame) | 0x1000);
            }
            if (frame <= 5) {
                goto frame_tail;                /* bl 080e640e */
            }

            cell = 5 * Func_080022fc(frame / 2, 3);
            y = Half_080e47b8(anchor[0]) - 20;

            renderers[0](target,
                         (const void *)(GFX_PANEL_080E47B8 + (cell << 9)),
                         y, 16, 40, 32);
            renderers[0](target,
                         (const void *)(0x02012a56 + (cell << 8)),
                         y, 48, 40, 32);
            renderers[0](target,
                         (const void *)(0x02011156 + (cell << 9)),
                         y, 80, 40, 32);
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 5 || mode == 23) {
            struct Actor_080e47b8 *actor =
                (struct Actor_080e47b8 *)GFX_SCRATCH_080E47B8;

            for (i = 0; i < 16; i++, actor++) {
                s32 step;

                if (frame < i / 2 + 4 || actor->timer > 11) {
                    continue;
                }
                step = Half_080e47b8(actor->timer);
                Func_080e3944(actor, scratch);
                scratch[0] = Half_080e47b8(scratch[0]);

                renderers[0](target,
                             (const void *)(GFX_MAIN_080E47B8 + (step << 11)),
                             scratch[0] - 16, scratch[1] - 32, 32, 64);

                Func_080e38b8(actor, 60, 0x1000);
                actor->timer += 1;
            }
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 4) {
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 11) {
            s32 angle = frame << 9;
            s32 baseline;
            s32 variant;

            /* The sine result is computed and discarded; only the cosine
             * feeds the baseline.  Kept because the call is observable. */
            (void)Func_08002322(angle);
            baseline = ((Func_0800231c(angle) << 2) >> 16)
                       + *(const s16 *)((const u8 *)anchor + 6) + 16;

            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
            variant = context->field04 * 7;

            if (frame <= 3) {
                renderers[0](target, state,
                             ROM_U8_080E47B8(TBL_TITLE_X_080E47B8, variant),
                             baseline +
                                 ROM_U8_080E47B8(TBL_TITLE_Y_080E47B8, 0),
                             57, 98);
                goto frame_tail;                /* bl 080e640e */
            }

            if (frame <= 7) {
                renderers[0](target, state,
                             ROM_U8_080E47B8(TBL_TITLE_X_080E47B8, variant),
                             baseline +
                                 ROM_U8_080E47B8(TBL_TITLE_Y_080E47B8, 0),
                             57, 98);
            }

            renderers[0](target, state + 0x15d2,
                         ROM_U8_080E47B8(TBL_TITLE_X_080E47B8, variant + 1),
                         baseline + ROM_U8_080E47B8(TBL_TITLE_Y_080E47B8, 1),
                         99, 69);

            if ((u32)(frame - 4) <= 1) {
                ((Fill32_080e47b8)IWRAM_FILL32_080E47B8)(
                    (void *)target, BG_SIZE_080E47B8, 0x3f3f3f3f);
            }
            if ((u32)(frame - 6) <= 1) {
                renderers[0](target, state + 0x3081,
                             ROM_U8_080E47B8(TBL_TITLE_X_080E47B8,
                                             variant + 2),
                             baseline +
                                 ROM_U8_080E47B8(TBL_TITLE_Y_080E47B8, 2),
                             128, 91);
            }
            if ((u32)(frame - 8) <= 1) {
                renderers[0](target, (const void *)GFX_MAIN_080E47B8,
                             ROM_U8_080E47B8(TBL_TITLE_X_080E47B8,
                                             variant + 3),
                             baseline +
                                 ROM_U8_080E47B8(TBL_TITLE_Y_080E47B8, 3),
                             128, 91);
            }
            if ((u32)(frame - 10) <= 1) {
                renderers[0](target, (const void *)0x02012d80,
                             ROM_U8_080E47B8(TBL_TITLE_X_080E47B8,
                                             variant + 4),
                             baseline +
                                 ROM_U8_080E47B8(TBL_TITLE_Y_080E47B8, 4),
                             128, 59);
            }
            if ((u32)(frame - 12) <= 1) {
                renderers[0](target, (const void *)0x02014b00,
                             ROM_U8_080E47B8(TBL_TITLE_X_080E47B8,
                                             variant + 5),
                             baseline +
                                 ROM_U8_080E47B8(TBL_TITLE_Y_080E47B8, 5),
                             122, 29);
            }
            if ((u32)(frame - 14) <= 1) {
                renderers[0](target, (const void *)0x020158d2,
                             ROM_U8_080E47B8(TBL_TITLE_X_080E47B8,
                                             variant + 6),
                             baseline +
                                 ROM_U8_080E47B8(TBL_TITLE_Y_080E47B8, 6),
                             76, 25);
            }
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 32) {
            s32 mirrored;

            camera_z += camera_dz;
            if (frame > 6) {
                camera_dz = (camera_dz * 3) << 4;
                if (camera_dz < 0) {
                    camera_dz += 63;
                }
                camera_dz >>= 6;
            }
            REG32_080E47B8(REG_BG2PD_080E47B8) = (u32)((camera_z >> 16) << 8);

            if ((u32)(frame - 16) <= 15) {
                REG16_080E47B8(REG_BLDALPHA_080E47B8) =
                    (u16)((16 - (frame - 16)) | 0x1000);
            }
            if ((u32)(frame - 4) <= 1) {
                ((Fill32_080e47b8)IWRAM_FILL32_080E47B8)(
                    (void *)target, BG_SIZE_080E47B8, 0x3f3f3f3f);
            }

            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
            mirrored = (context->field04 == 1);

            if (frame <= 3) {
                renderers[0](target, state, mirrored ? 0 : 48, 24, 80, 104);
                goto frame_tail;                /* bl 080e640e */
            }
            if (frame <= 7) {
                renderers[0](target, state, mirrored ? 0 : 48, 24, 80, 104);
            }

            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
            if (context->field04 == 1) {
                renderers[0](target, state + 0x1e00, 16, 16, 80, 104);
            } else {
                renderers[0](target, state + 0x1e00, 32, 16, 80, 104);
            }

            if ((u32)(frame - 6) <= 1) {
                renderers[0](target, state + 0x3e80, 0, 16, 128, 91);
            }
            if ((u32)(frame - 8) <= 1) {
                renderers[0](target, (const void *)GFX_MAIN_080E47B8,
                             0, 16, 128, 91);
            }
            if ((u32)(frame - 10) <= 1) {
                renderers[0](target, (const void *)0x02012d80,
                             0, 16, 128, 59);
            }
            if ((u32)(frame - 12) <= 1) {
                renderers[0](target, (const void *)0x02014b00,
                             0, 16, 128, 29);
            }
            if ((u32)(frame - 14) <= 1) {
                renderers[0](target, (const void *)0x02015980,
                             0, 16, 128, 26);
            }
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 20) {
            for (i = 0; i < 12; i++) {
                s32 cell;
                s32 x;
                s32 offset;
                s32 slot;

                if (frame < i + 6 || frame >= i + 18) {
                    continue;
                }
                cell = (frame - i - 6) / 2;
                x = Half_080e47b8(anchor[0])
                    - (ROM_U8_080E47B8(TBL_RING_W_080E47B8, cell) >> 1);
                offset = 3 * ((i + 1) / 2);
                if ((i & 1) != 0) {
                    x += offset;
                } else {
                    x -= offset;
                }

                if (i == 0) {
                    slot = 1;
                } else {
                    slot = (((i - 1) & 3) > 1) ? 1 : 0;
                }

                renderers[slot](
                    target,
                    (const void *)(ROM_U16_080E47B8(TBL_RING_SRC_080E47B8,
                                                    cell) + GFX_MAIN_080E47B8),
                    x,
                    ROM_U8_080E47B8(TBL_RING_Y_080E47B8, cell) + 48,
                    ROM_U8_080E47B8(TBL_RING_W_080E47B8, cell),
                    ROM_U8_080E47B8(TBL_RING_H_080E47B8, cell));
            }
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 16) {
            struct Actor_080e47b8 *actor;

            if (frame == 0) {
                actor = (struct Actor_080e47b8 *)GFX_SCRATCH_080E47B8;
                for (i = 0; i < 64; i++, actor++) {
                    actor->word00 = (s32)(Func_08004458() & 0x7f) + 32;
                    actor->word04 = 0;
                    actor->word08 = 0;
                    actor->word0c = (s32)(Func_08004458() & 0xffff);
                    actor->word10 = (s32)(Func_08004458() & 0xffff);
                    actor->word14 = (s32)(Func_08004458() & 0xffff);
                }
                *(s32 *)(0x020146e4 + 4) = 159;
            }

            actor = (struct Actor_080e47b8 *)GFX_SCRATCH_080E47B8;
            for (i = 0; i < 64; i++, actor++) {
                s32 cell;
                s32 y;

                if (actor->word00 < 0 || frame < i / 2) {
                    continue;
                }
                cell = i & 3;

                Func_080049ac();
                Func_08004bd4(actor->word0c);
                Func_08004c1c(actor->word10);
                Func_080e3944(actor, scratch);

                scratch[0] = Half_080e47b8(scratch[0])
                             + Half_080e47b8(anchor[0]);
                y = scratch[1] + anchor[1];
                scratch[1] = y + 32;

                renderers[1](
                    target,
                    (const void *)(ROM_U16_080E47B8(TBL_SPRAY_SRC_080E47B8,
                                                    cell) + GFX_MAIN_080E47B8),
                    scratch[0] - 4, y + 28, 8, 8);

                actor->word00 -= 6;
                if (actor->word00 < 0 && ((i & 7) == 0 || i == 63)) {
                    Func_080f9010(133);
                    context = *(struct Context_080e47b8 **)
                        (state + STATE_CONTEXT_080E47B8);
                    Func_080d6888(context->field24, 7, 5, 0, 4);
                }
            }
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 8) {
            s32 height;

            if ((u32)(frame - 5) > 44) {
                goto frame_tail;                /* bl 080e640e */
            }
            height = (frame > 25) ? (196 - frame * 4) : (frame * 16 - 64);
            if (height > 96) {
                height = 96;
            }
            renderers[0](target, (const void *)GFX_MAIN_080E47B8,
                         48, 104 - height, 32, height);
            goto frame_tail;                    /* bl 080e640e */
        }

        if ((u32)(mode - 33) <= 1) {
            s32 spread;
            s32 x;

            if (frame > 5) {
                goto frame_tail;                /* bl 080e640e */
            }
            spread = 3 * (6 - frame);

            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
            if (context->field04 == 0) {
                x = Half_080e47b8(anchor[0]) + spread * 2;
            } else {
                x = Half_080e47b8(anchor[0]) - spread * 2;
            }

            renderers[1](target, (const void *)GFX_MAIN_080E47B8,
                         x - 16, anchor[1] - spread * 4 + 24 - 32, 32, 64);
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode == 12) {
            struct Actor_080e47b8 *actor =
                (struct Actor_080e47b8 *)GFX_SCRATCH_080E47B8;

            if (frame > 47) {
                REG16_080E47B8(REG_BLDALPHA_080E47B8) =
                    (u16)((64 - frame) | 0x1000);
            }

            for (i = 0; i < 16; i++, actor++) {
                s32 cell = Func_080022fc(i, 3);

                Func_080e3944(actor, scratch);
                scratch[0] = Half_080e47b8(scratch[0]);

                renderers[i & 1](target,
                                 (const void *)(GFX_MAIN_080E47B8 +
                                                ((cell * 9) << 6)),
                                 scratch[0] - 12, scratch[1] - 12, 24, 24);

                Func_080e38b8(actor, 60, 1 << ((i & 3) + 11));
                actor->timer += 1;
            }
            goto frame_tail;                    /* bl 080e640e */
        }

        if (mode != 100) {
            struct Actor_080e47b8 *actor =
                (struct Actor_080e47b8 *)GFX_SCRATCH_080E47B8;

            for (i = 0; i < 16; i++, actor++) {
                s32 cell;

                if (frame < i + 4 || actor->timer > 23) {
                    continue;
                }
                cell = actor->timer / 4;

                Func_080e3944(actor, scratch);
                scratch[0] = Half_080e47b8(scratch[0]);

                renderers[i & 1](target,
                                 (const void *)(GFX_MAIN_080E47B8 +
                                                ((cell * 9) << 7)),
                                 scratch[0] - 12, scratch[1] - 24, 24, 48);

                if (mode == 25) {
                    Func_080e38b8(actor, 60, 0x400);
                } else {
                    Func_080e38b8(actor, 60, 0x1000);
                }
                actor->timer += 1;
            }
        }

    frame_tail:                                 /* 080e640e */

        if (mode <= 7 && frame <= 5) {
            Func_080e3944((const struct Actor_080e47b8 *)origin, scratch);
            scratch[0] = Half_080e47b8(scratch[0]);

            renderers[1](target, (const void *)GFX_SPARK_080E47B8,
                         scratch[0] - 10, scratch[1] - 4, 20, 40);

            origin[0] += drift[0];
            origin[1] += drift[1];
            origin[2] += drift[2];
        }

        if (frame == 3) {
            Func_080b50e8(-1);
        }
        if (frame == 4) {
            Func_080f9010(134);
        }

        if (frame == 6) {
            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);

            if ((u32)mode_minus_four <= 1 || mode == 7 || mode == 13 ||
                mode == 18 || mode == 19 || mode == 23 || mode == 34 ||
                mode == 100) {
                Func_080b5088(context->field24, 4);
                *(s32 *)(state + STATE_LIMIT_080E47B8) = 8;
            } else if (mode == 20 || mode == 14 || mode == 33) {
                Func_080b5088(context->field24, 1);
                *(s32 *)(state + STATE_LIMIT_080E47B8) = 2;
            } else if (mode == 30 || mode == 8) {
                Func_080b5088(context->field24, 3);
                *(s32 *)(state + STATE_LIMIT_080E47B8) = 8;
            }

            /* The original re-tests frame == 6 here; kept for fidelity. */
            if (frame == 6) {
                Func_080d6888(context->field24, 7, 5, 0, 4);
            }
        }

        if (frame == 14) {
            context =
                *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
            Func_080d6888(context->field24, 7, 5, 0, 4);
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(state + STATE_DIRTY_080E47B8) = 1;
        Func_080030f8(1);
        continue;

    publish_blitter:                            /* 080e5ab6 */
        context =
            *(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8);
        Func_080cef64(context->field04, renderers);
        goto frame_tail;                        /* bl 080e640e */
    }

    /* -------------------------------------------------------------- */
    /* Teardown (080e657c)                                            */
    /* -------------------------------------------------------------- */
    if (mode == 21) {
        Fill_BgAndTarget_080e47b8(target);
        (*(struct Context_080e47b8 **)(state + STATE_CONTEXT_080E47B8))
            ->field1c = 0;
        Func_08004278(0x080cd4b5);
        Func_08004278(0x080cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        Func_080d9ac4(context_argument);
    } else {
        if ((u32)mode_minus_two <= 1 || mode == 12 || mode == 22 ||
            mode == 28 || mode == 29) {
            Func_08004278(0x080dbb9d);
        }
        Func_08004278(0x080cd261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        Func_080cdbc0();
    }
}
