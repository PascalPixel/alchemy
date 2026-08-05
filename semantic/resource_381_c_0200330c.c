#include "types.h"

/*
 * Resource 381, sparkle-field installer at 0x0200330c
 * (200 bytes, 5 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7,fp / mov r6,sl / mov r5,r9 / push {r5,r6,r7} / mov r7,r8 /
 * push {r7}` at 0x0200330c, and the matching unwind at 0x020033ae ending
 * `pop {r0} / bx r0`.  The popped branch register IS r0, so it holds the
 * return address and the owner is `void`.  Bytes 0x020033c0-0x020033d3 are the
 * five-word literal pool.
 *
 * LINK BASE, proven here twice over.  The overlay is linked at 0x02008000, so
 * an in-image address is `pool_word - 0x8000`:
 *   0x0200b01d is ODD -> Thumb entry at file offset 0x301c, i.e.
 *     Func_0200301c — the per-frame task this owner installs, and the next
 *     row in this same conversion batch.  A callback, not a data array.
 *   0x0200ba68 and 0x0200ba90 are EVEN -> in-image word tables at 0x3a68 and
 *     0x3a90.  Parity settles kind without any dataflow.
 * The pool also holds 0x040000d4 (DMA3 source register) and 0x85000065, which
 * are hardware, not image addresses — both are outside the 0x02008000-0x0200bfff
 * band, so the band test classifies them before anything else does.
 *
 * The `stmia r3!, {r0, r1, r2}` at 0x0200333a is a DMA3 clear, not a struct
 * copy: r3 is 0x040000d4, so the three words land in DMA3SAD / DMA3DAD /
 * DMA3CNT.  The source is the zeroed stack word, the destination the freshly
 * fetched workspace, and 0x85000065 is enable | 32-bit | source-fixed with a
 * count of 0x65 = 101 words = 404 bytes — exactly the workspace size requested
 * from Func_08000148.  The `subs r3, #12` that follows only rewinds the
 * auto-incremented register and is dead.
 *
 * Workspace layout, confirmed against the byte-exact sibling
 * `assets/code/resource_381_c_02002ba0.c`, which drives the same records
 * through `Data_0200bb40` / `Data_0200bb10`: ten 40-byte particles followed by
 * a u16 count at byte 400.  The installer fills only `record`, `height`,
 * `fall` and `tick`; the DMA clear above is what makes the other fields zero.
 *
 * The requested count is clamped to ten by `cmp r2,#10 / bls` — the array is
 * fixed-size, so a caller asking for more silently gets ten.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 5 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_0808a080 x2, Func_080000d0 x1, Func_08000148 x1, Func_080091e8 x1.
 * The two Func_0808a080 sites are genuinely distinct — the first stores the
 * record, the second feeds Func_080091e8 — so they are NOT folded into one
 * local.
 */

/* Old-style declarations: overlay import arities vary per call site. */
u8 *Func_08000148();    /* fetch or allocate a workspace by id and size */
void Func_080000d0();   /* install a per-frame task (callback, period) */
void Func_080091e8();   /* per-record presentation reset */
u8 *Func_0808a080();    /* scene entity record by selector */

void Func_0200301c(void);   /* this overlay: the per-frame task installed below */

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

void Func_0200330c(s32 slot, u32 requested)
{
    Field *field;
    Particle *particle;
    u32 *dma;
    u32 zero;
    u32 count;
    u32 index;
    u8 *record;

    field = (Field *)Func_08000148(33, 404);

    /* DMA3: fixed-source 32-bit fill of the whole 404-byte workspace. */
    zero = 0;
    dma[0] = (u32)&zero;
    dma = (u32 *)0x040000d4;
    dma[1] = (u32)field;
    dma[2] = 0x85000065;

    count = requested;
    if (count > 10) {
        count = 10;
    }

    particle = field->particle;
    index = 0;
    if (count != 0) {
        do {
            record = Func_0808a080(slot);
            *(u8 *)(*(u8 **)(record + 0x50) + 38) = 0;
            particle->record = record;
            record[85] = 0;

            Func_080091e8(Func_0808a080(slot), 1);

            particle->height = Data_0200ba68[index];
            particle->fall = -Data_0200ba90[index];
            particle->tick = 3;

            index++;
            particle++;
            slot++;
        } while (index != count);
    }

    field->count = (u16)count;

    Func_080000d0(Func_0200301c, 3200);
}
