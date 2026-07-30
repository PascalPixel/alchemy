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
 */

/* Snapshot window: 0x2000 bytes of BG character data at 0x06004000. */
#define VRAM_WINDOW_080A24D0     ((void *)0x06004000)
#define VRAM_WINDOW_SIZE_080A24D0 0x2000

/* Fill pattern and fill target used to blank the window while the scene runs. */
#define BLANK_PATTERN_080A24D0   0x33333333u
#define FILL_TARGET_080A24D0     0x03000168u

/* Palette/BG resource handed to Func_08015418 before the window is built. */
#define SCENE_TILEMAP_080A24D0   0x06002500

/* Fourth word loaded next to the first Func_08007310 call.  See note below. */
#define UNUSED_LITERAL_080A24D0  0x03001388u

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
void Func_08007310(void *destination, const void *source, u32 size);
void Func_080072f0(void *destination, u32 size, u32 value, u32 target);
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

    /*
     * Uncertainty: the assembly loads UNUSED_LITERAL_080A24D0 into r3 and
     * copies it into fp immediately before this call.  fp is never read again
     * before the epilogue restores it, and the value is not reloaded for the
     * matching restore call below, so it is treated here as a dead literal
     * rather than a fourth argument to Func_08007310.
     */
    Func_08007310(vram_backup, VRAM_WINDOW_080A24D0,
                  VRAM_WINDOW_SIZE_080A24D0);
    Func_080072f0(VRAM_WINDOW_080A24D0, VRAM_WINDOW_SIZE_080A24D0,
                  BLANK_PATTERN_080A24D0, FILL_TARGET_080A24D0);
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
    Func_08007310(VRAM_WINDOW_080A24D0, vram_backup,
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
