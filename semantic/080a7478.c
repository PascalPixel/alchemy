/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Callee signatures here are established, not guessed: 0x03001388 is the
 * word copy declared in the EXACT src/080d40ec.c, and 0x03000168 is the fill
 * documented in semantic/main/080e15e8.c as (destination, size, value).
 */
#include "types.h"

/*
 * Func_080a7478 -- 600 bytes, 080a7478..080a769f executable, literal pools at
 * 080a75a0..080a75d7 and 080a76bc..080a76cf.  One prologue, one epilogue, no
 * continuation into another range: the whole owner is contained here.
 *
 * It is a modal sub-screen driver: it allocates a workspace and two save
 * buffers, snapshots palette RAM and a VRAM tile block, builds the sub-screen
 * state, runs a blocking body (Func_080a76d0) and then restores everything it
 * touched before returning that body's result.
 */

#define REG8_080A7478(address)  (*(volatile u8 *)(address))
#define REG16_080A7478(address) (*(volatile u16 *)(address))
#define REG32_080A7478(address) (*(volatile u32 *)(address))

struct DmaChannel_080a7478 {
    const void *source;
    void *destination;
    u32 control;
};

/* Live-object table roots read through absolute pointers in the pool. */
#define OBJECT_ROOT_080A7478 (*(u8 **)0x03001e68)
#define STATUS_ROOT_080A7478 (*(u8 **)0x03001e8c)
#define STATUS_FLAG_080A7478 0x0ea6

/* Workspace byte offsets, all derived from the assembly's shifted literals. */
#define WS_TEXT_STATE_080A7478   0x0208 /* 130 << 2 */
#define WS_TEXT_RESULT_080A7478  0x0219
#define WS_WINDOW_080A7478       0x010c /* 134 << 1 */
#define WS_ROW_TABLE_080A7478    0x0152 /* 169 << 1, 8 halfwords descending */
#define WS_DONE_FLAG_080A7478    0x0220 /* 136 << 2 */
#define WS_COLUMN_TABLE_080A7478 0x0234 /* 141 << 2 */
#define WS_HANDLE_080A7478       0x0024

void *Func_080048b0(s32 resource, s32 size);
void *Func_08004970(s32 size);
void Func_08015408(s32 x, s32 y, s32 width, s32 height);
void Func_080030f8(s32 frames);
void Func_080a1070(void);
void Func_080a1090(s32 mode);
s32 Func_08077158(void *state);
void Func_080a8034(s32 a, s32 b, s32 c, s32 d);
void Func_080a2144(s32 index);

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
typedef void (*ArmFill)(void *destination, u32 size, u32 value);
void Func_080153e0(s32 enable);
s32 Func_08015010(s32 kind, s32 x, s32 y, s32 width, s32 layer);
s32 Func_08077290(s32 slot);
void Func_080ad274(s32 window, s32 mode);
void Func_08015418(void *destination);
void Func_080a2474(void);
s32 Func_080a76d0(void);
void Func_080a2490(void);
void Func_08015278(s32 window);
void Func_080ad318(void);
void Func_080a1050(void);
void Func_080152a8(void);
void Func_08002df0(void *allocation);
void Func_080a34c0(void);
void Func_08002dd8(s32 resource);
void Func_08015410(s32 x, s32 y, s32 width, s32 height);

/*
 * Immediate DMA3 transfer.  The original inlines four of these as
 * `stmia r3!, {r0, r1, r2}` over 0x040000d4, so no wait loop is performed
 * between them; each 32-bit control word is written last, exactly as here.
 */
static void Dma3Copy_080a7478(const void *source, void *destination, u32 control)
{
    struct DmaChannel_080a7478 *dma3 =
        (struct DmaChannel_080a7478 *)0x040000d4;

    dma3->source = source;
    dma3->destination = destination;
    dma3->control = control;
}

/* The four palette fixups issued back-to-back at 080a7508..080a7530. */
static void ApplyPaletteOverrides_080a7478(void)
{
    Dma3Copy_080a7478((const void *)0x05000200, (void *)0x05000000,
                      0x80000010);
    Dma3Copy_080a7478((const void *)0x050001c8, (void *)0x0500001c,
                      0x80000001);
    Dma3Copy_080a7478((const void *)0x05000200, (void *)0x05000020,
                      0x80000010);
    Dma3Copy_080a7478((const void *)0x050001e8, (void *)0x0500003c,
                      0x80000001);
}

s32 Func_080a7478(void)
{
    u8 *workspace;
    void *palette_save;
    void *tile_save;
    u16 *row;
    u16 *column;
    s32 index;
    s32 value;
    s32 result;

    workspace = (u8 *)Func_080048b0(55, 167 << 4); /* 2672 bytes */
    palette_save = Func_08004970(64);
    tile_save = Func_08004970(128 << 6); /* 8192 bytes */

    /* Suspend the shared object list while the sub-screen owns the screen. */
    *(u16 *)(OBJECT_ROOT_080A7478 + 4) = 1;

    Func_08015408(0, 0, 30, 20);
    Func_080030f8(1);
    Func_080a1070();
    Func_080a1090(0);

    *(u16 *)(workspace + WS_DONE_FLAG_080A7478) = 0;
    workspace[WS_TEXT_RESULT_080A7478] =
        (u8)Func_08077158(workspace + WS_TEXT_STATE_080A7478);

    Func_080a8034(0, 3, 0, 7);

    /* Snapshot palette RAM and the 8 KiB tile block before overwriting them. */
    ((WordCopy)0x03001388)(palette_save, (const void *)0x05000000, 64);
    Func_080a2144(14);
    ApplyPaletteOverrides_080a7478();
    ((WordCopy)0x03001388)(tile_save, (const void *)0x06004000, 128 << 6);
    ((ArmFill)0x03000168)((void *)0x06004000, 128 << 6, 0x33333333);

    Func_080153e0(1);
    *(s32 *)(workspace + WS_WINDOW_080A7478) =
        Func_08015010(13, 0, 17, 5, 2);

    /* Eight descending halfwords ending at WS_ROW_TABLE, all set to 30. */
    row = (u16 *)(workspace + WS_ROW_TABLE_080A7478);
    for (index = 7; index >= 0; index--) {
        *row = 30;
        row--;
    }

    if (Func_08077290(-1) != 0)
        Func_080ad274(*(s32 *)(workspace + WS_WINDOW_080A7478), 0);

    /*
     * Four column entries: x positions 130, 162, 194, 226 written as
     * halfwords, with a constant 0x80 stored eight bytes further on.
     */
    column = (u16 *)(workspace + WS_COLUMN_TABLE_080A7478);
    value = 130;
    for (index = 3; index >= 0; index--) {
        column[0] = (u16)value;
        column[4] = 0x80;
        value += 32;
        column++;
    }

    Func_08015418((void *)0x06002500);
    Func_080a2474();
    *(u16 *)(workspace + WS_DONE_FLAG_080A7478) = 0;

    result = Func_080a76d0(); /* blocking sub-screen body */

    Func_080a2490();
    Func_08015278(*(s32 *)(workspace + WS_HANDLE_080A7478));
    Func_080ad318();
    Func_080a1050();
    Func_08015408(0, 0, 30, 20);
    Func_080030f8(1);
    Func_080152a8();
    Func_080153e0(0);
    Func_080030f8(1);

    /* Restore what was snapshotted, then release both save buffers. */
    ((WordCopy)0x03001388)((void *)0x05000000, palette_save, 64);
    ((WordCopy)0x03001388)((void *)0x06004000, tile_save, 128 << 6);
    Func_08002df0(tile_save);
    Func_08002df0(palette_save);

    STATUS_ROOT_080A7478[STATUS_FLAG_080A7478] = 1;
    Func_080a34c0();
    Func_08015408(0, 0, 30, 20);
    Func_08002dd8(55);

    *(u16 *)(OBJECT_ROOT_080A7478 + 4) = 0;
    Func_080030f8(1);
    Func_08015410(0, 0, 30, 20);
    STATUS_ROOT_080A7478[STATUS_FLAG_080A7478] = 0;

    return result;
}

/*
 * Known uncertainties
 * -------------------
 * - Func_080a7478 takes no register arguments (r0..r3 are all written before
 *   any use), so it is reconstructed as `void`.  It returns the value produced
 *   by Func_080a76d0 in r0.
 * - CORRECTED by the veneer audit (2026-08-01).  This note previously said
 *   the two `ldr r5, =0x03001388` loads at 0x080a74f8 and 0x080a763e were
 *   DEAD and omitted them.  They are not dead: they are the callee loads for
 *   the four `bl 0x080072f8` sites, which are `__call_via_r5`.  Each load
 *   feeds the two calls that follow it.  The file had already worked out that
 *   0x080072f0 is `call via r3` and that its fourth argument is the real
 *   helper -- it simply did not carry that reading across to r5, and so wrote
 *   off the r5 callees as scheduling residue.
 * - Func_080072f8's argument order is taken as (destination, source, size)
 *   from the symmetric snapshot/restore pair; that reading is unchanged, and
 *   it agrees with the word copy's signature in the exact src/080d40ec.c.
 * - Workspace field names describe observed use, not original names.  The
 *   allocation is 2672 bytes and only the offsets listed above are touched
 *   here; it is modelled as a byte array rather than a fabricated struct.
 * - The workspace handle at offset 0x24 is written by callees, not by this
 *   function; it is read back as a signed word for Func_08015278.
 */
