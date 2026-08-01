#include "types.h"

/*
 * Func_080a24d0 -- 432 bytes, one prologue at 0x080a24d0 and one epilogue at
 * 0x080a267a.  No live frame or register state leaves the range: the single
 * non-call control edge in the assembly is `b.n .L1`, a jump over the embedded
 * ten-word literal pool, which is plain straight-line flow in C.
 *
 * The routine runs a self-contained modal sub-scene: it snapshots a VRAM
 * window into a scratch buffer, allocates a 2672-byte scene context, drives the
 * interactive part through Func_080a2680, applies the chosen result to the
 * shared display record when the interaction was confirmed, then restores VRAM
 * and tears everything back down.  It returns the Func_080a2680 result code.
 *
 * Correctness fix, veneer audit (mars, 2026-08-01).
 *
 * Neither `Func_08007310` nor `Func_080072f0` is a function.  0x080072e4
 * begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop`
 * entries in the order r0..r7, r8, r9, sl, fp, ip, sp, lr, ending at
 * 0x08007320 -- so 0x08007310 is `__call_via_fp` (index 11) and 0x080072f0 is
 * `__call_via_r3` (index 3).  A `bl` to either calls whatever the named
 * register holds.
 *
 *   0x080a256e  via fp  <- 0x03001388, the relocated IWRAM word copy
 *   0x080a257a  via r3  <- 0x03000168, the relocated IWRAM word fill
 *   0x080a2606  via fp  <- 0x03001388, still in fp from the first site
 *
 * This file's previous header recorded an honest uncertainty about the first
 * site: "the assembly loads 0x03001388 into r3 and copies it into fp
 * immediately before this call.  fp is never read again before the epilogue
 * restores it ... treated here as a dead literal rather than a fourth
 * argument."  The observation was exactly right and the conclusion was the
 * only one available without the bank: fp is not dead and not an argument, it
 * is the call target, and it is read by the *veneer*, which is why nothing in
 * the function body appears to read it.
 *
 * That also answers why the value is not reloaded before the second copy at
 * 0x080a2606.  fp is callee-saved and this function pushes it in its own
 * prologue, so the single write at 0x080a2568 survives the nine intervening
 * calls; there is no other write to fp anywhere in the range, and the one
 * conditional edge (`bne.n 0x080a25c2` at 0x080a259c) rejoins after it.  The
 * resolver reports this site as UNRESOLVED -- correctly, since its backward
 * walk is bounded -- and the answer here comes from a whole-function
 * sole-writer argument instead.
 *
 * Signatures are not guessed.  The word copy's shape comes from the EXACT
 * src/080d40ec.c, which declares 0x03001388 as
 * `void *(*)(void *destination, const void *source, s32 size)`; the fill's
 * comes from the same file plus semantic/main/080ae2f4.c and 08023e70.c,
 * which call 0x03000168 as `(void *destination, u32 size, u32 value)`.
 *
 * Argument counts, per site, from the registers live at the branch:
 * both copies set r0/r1/r2 only, and the fill sets r0/r1/r2 with the callee
 * in r3.  fp is index 11, above the argument registers, so the copies' three
 * drafted arguments are all real and nothing is stripped there; the fill
 * dispatches through r3, so its drafted fourth argument is the callee.
 */

/* Snapshot window: 0x2000 bytes of BG character data at 0x06004000. */
#define VRAM_WINDOW_080A24D0     ((void *)0x06004000)
#define VRAM_WINDOW_SIZE_080A24D0 0x2000

/* Fill pattern used to blank the window while the scene runs. */
#define BLANK_PATTERN_080A24D0   0x33333333u

/* The two relocated IWRAM helpers this owner calls through the veneer bank. */
typedef void *(*WordCopy_080a24d0)(void *destination, const void *source,
                                   s32 size);
typedef void (*WordFill_080a24d0)(void *destination, u32 size, u32 value);
#define WORD_COPY_080A24D0  ((WordCopy_080a24d0)0x03001388)
#define WORD_FILL_080A24D0  ((WordFill_080a24d0)0x03000168)

/* Palette/BG resource handed to Func_08015418 before the window is built. */
#define SCENE_TILEMAP_080A24D0   0x06002500

/* Byte flag inside the object pointed to by GlobalTable.unk_24. */
#define BUSY_FLAG_OFFSET_080A24D0 0xea6

/* Resource slot and size for the scene context allocation. */
#define SCENE_RESOURCE_080A24D0  55
#define SCENE_CONTEXT_SIZE_080A24D0 (167 << 4) /* 2672 */

struct Hardware_080a24d0 {
    u8 pad_00[4];
    u16 active_04;   /* set to 1 on entry, 0 on exit */
};

/*
 * Shared pointer table at 0x03001e68.  Only three of its slots are touched
 * here; the intervening words are opaque to this owner.
 */
struct GlobalTable_080a24d0 {
    struct Hardware_080a24d0 *hardware_00; /* +0x00 */
    u32 unk_04[8];                         /* +0x04 .. +0x20 */
    u8 *busy_owner_24;                     /* +0x24 */
    u32 unk_28[11];                        /* +0x28 .. +0x50 */
    u8 *display_record_54;                 /* +0x54 */
};

/*
 * Scene context allocated from resource slot 55.  Field names are descriptive
 * of use only; the true layout beyond the touched offsets is unknown.
 */
struct SceneContext_080a24d0 {
    u8 unk_00[0x24];
    void *list_24;                    /* +0x24, handed to Func_08015278 */
    u8 unk_28[0x10c - 0x28];
    s32 widget_10c;                   /* +0x10c, Func_08015010 handle */
    u8 unk_110[0x174 - 0x110];
    u16 secondary_value_174;          /* +0x174 */
    u8 unk_176[0x208 - 0x176];
    u8 subobject_208[0x219 - 0x208];  /* +0x208, passed to Func_08077158 */
    u8 subobject_id_219;              /* +0x219 */
    u8 unk_21a[SCENE_CONTEXT_SIZE_080A24D0 - 0x21a];
};

/* Fields written into the display record at GlobalTable.display_record_54. */
#define DISPLAY_PACKED_OFFSET_080A24D0    0x180
#define DISPLAY_SECONDARY_OFFSET_080A24D0 0x19a

/* Bit layout of the packed halfword written at +0x180. */
#define DISPLAY_LOW_MASK_080A24D0  0x1ff
#define DISPLAY_HIGH_SHIFT_080A24D0 10

void *Func_08004970(u32 size);
void *Func_080048b0(s32 resource, u32 size);
void Func_08002dd8(s32 resource);
void Func_08002df0(void *block);
void Func_080030f8(u32 frames);
void Func_08015408(s32, s32, s32, s32);
void Func_08015410(s32, s32, s32, s32);
void Func_08015418(s32 resource);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_080153e0(s32 enable);
void Func_08015278(void *list);
void Func_080152a8(void);
s32 Func_08077158(void *subobject);
void Func_080a1050(void);
void Func_080a1070(void);
void Func_080a1090(s32 mode);
void Func_080a2144(s32 index);
void Func_080a2474(void);
void Func_080a2490(void);
s32 Func_080a2680(s32 *out_high, s32 *out_unused, s32 *out_low);
void Func_080a3354(s32, s32, s32, s32);
void Func_080a34c0(void);
void Func_080a5534(void);
void Func_080ae8dc(void);
void Func_0808a548(void);

s32 Func_080a24d0(void)
{
    struct GlobalTable_080a24d0 *globals =
        (struct GlobalTable_080a24d0 *)0x03001e68;
    struct SceneContext_080a24d0 *scene;
    void *vram_backup;
    s32 result;
    s32 out_high;
    s32 out_unused;
    s32 out_low;
    u8 *display;
    u8 *busy_owner;

    vram_backup = Func_08004970(VRAM_WINDOW_SIZE_080A24D0);
    scene = (struct SceneContext_080a24d0 *)Func_080048b0(
        SCENE_RESOURCE_080A24D0, SCENE_CONTEXT_SIZE_080A24D0);

    globals->hardware_00->active_04 = 1;

    Func_08015408(0, 0, 30, 20);
    Func_080030f8(1);
    Func_080a1090(0);

    scene->subobject_id_219 = (u8)Func_08077158(&scene->subobject_208[0]);

    Func_080a3354(0, 3, 0, 7);
    Func_080a5534();
    Func_080a2144(14);
    Func_08015418(SCENE_TILEMAP_080A24D0);
    scene->widget_10c = Func_08015010(13, 0, 17, 3, 2);
    Func_080a1070();

    /* 0x080a256e: bl __call_via_fp, fp = 0x03001388 loaded at 0x080a2568. */
    WORD_COPY_080A24D0(vram_backup, VRAM_WINDOW_080A24D0,
                       VRAM_WINDOW_SIZE_080A24D0);
    /* 0x080a257a: bl __call_via_r3, r3 = 0x03000168 loaded at 0x080a2572. */
    WORD_FILL_080A24D0(VRAM_WINDOW_080A24D0, VRAM_WINDOW_SIZE_080A24D0,
                       BLANK_PATTERN_080A24D0);
    Func_080153e0(1);
    Func_080a2474();

    /*
     * Func_080a2680 writes three stack slots.  Only the first and third are
     * consumed; the middle slot is written and discarded by this owner.
     */
    result = Func_080a2680(&out_high, &out_unused, &out_low);
    Func_080a2490();

    if (result == 1) {
        display = globals->display_record_54;
        *(u16 *)(display + DISPLAY_PACKED_OFFSET_080A24D0) =
            (u16)(((u32)out_high << DISPLAY_HIGH_SHIFT_080A24D0) |
                  ((u32)out_low & DISPLAY_LOW_MASK_080A24D0));
        *(u16 *)(display + DISPLAY_SECONDARY_OFFSET_080A24D0) =
            scene->secondary_value_174;
    }

    Func_08015278(scene->list_24);

    busy_owner = globals->busy_owner_24;
    busy_owner[BUSY_FLAG_OFFSET_080A24D0] = 1;

    Func_080a34c0();
    Func_08015408(0, 0, 30, 20);
    Func_080ae8dc();
    Func_08002dd8(SCENE_RESOURCE_080A24D0);

    globals->hardware_00->active_04 = 0;

    Func_080152a8();
    Func_080153e0(0);
    /* 0x080a2606: bl __call_via_fp again; fp still holds 0x03001388. */
    WORD_COPY_080A24D0(VRAM_WINDOW_080A24D0, vram_backup,
                       VRAM_WINDOW_SIZE_080A24D0);

    /*
     * The busy owner pointer is re-read from the table here; the assembly keeps
     * the table+0x24 address live in r6 across the calls above and reloads the
     * pointed-to value.
     */
    busy_owner = globals->busy_owner_24;
    busy_owner[BUSY_FLAG_OFFSET_080A24D0] = 0;

    Func_08002df0(vram_backup);
    Func_080030f8(1);
    Func_080a1050();
    Func_080030f8(1);
    Func_08015410(0, 0, 30, 20);

    busy_owner = globals->busy_owner_24;
    busy_owner[BUSY_FLAG_OFFSET_080A24D0] = 0;

    Func_0808a548();

    return result;
}
