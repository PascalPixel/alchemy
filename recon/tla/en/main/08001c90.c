/* UNMATCHED DRAFT, 2026-10-02. Intended canonical recon/tla/en/main/08001c90.c.
 * Own six TLA ROMs and current raw08001c90.s identify one identical128-byte
 * bank: four32-byte wrappers in ScaleThreeQuarters, Halve, Brighten, Darken order.
 * Saved ordinary C bank160: four40-byte extents,154 complete differences in each
 * edition (literal pools included). Eight whole-bank forms exhausted:
 * fixed_buffer_reviewed_dma: text/bank 216/216, complete differences 211.
 * local_function_pointer: text/bank 216/216, complete differences 211.
 * vla_extent: text/bank 264/264, complete differences 245.
 * scalar_dma_registers: text/bank 192/192, complete differences 183.
 * named_dma_fields: text/bank 160/160, complete differences 154.
 * whole_dma_record: text/bank 200/200, complete differences 192.
 * constant_dma_descriptor: text/bank 184/232, complete differences 222.
 * descriptor_scalar_dma: text/bank 136/184, complete differences 175.
 * No source assembly, register pins, extra unread storage or alternate flags retained.
 * Volatile models actual DMA3 hardware stores. code[] is wholly used executable
 * storage: DMA copies48/32/34/45 words including each Thumb bx-pc entry, ARM body
 * and any kernel pool; ordinary indirect call enters the copied Thumb entry.
 * Native loaders preserve input registers, use add-pc/ldmia for three constants,
 * one stmia DMA setup, then mov lr,sp/second-half BL. C constructs constants,
 * writes three hardware fields separately and calls the same even stack pointer
 * through a saved low register. Native and source stack extents match per function.
 * Scale/Halve ABI is (buffer, backup, bytes); Brighten/Darken ABI is
 * (buffer, packed adjustment, backup, bytes), returns ignored. Kernels remain
 * current maintained assembly, physically named; no fixed-entry alias is added.
 * All six editions now define the wrapper names at their actual raw/scaffold
 * starts. The localized names split the same128-byte scaffold; no code is adopted.
 * Private native wrapper reconstruction proves identities, not source adoption
 * or namespace readiness. No complete build/credit/runnable-equivalence claim.
 */
#include "TYPES.H"
#include "IO_REG.H"

extern const u32 ColorBuffer_ScaleThreeQuartersKernelCode[48];
extern const u32 ColorBuffer_HalveKernelCode[32];
extern const u32 ColorBuffer_BrightenKernelCode[34];
extern const u32 ColorBuffer_DarkenKernelCode[45];

/* LOCAL DRAFT VIEW: DMA3 source, destination and control registers only. */
struct ColorDmaDraftChannel {
    const void *source;
    void *destination;
    u32 control;
};

void ColorBuffer_ScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 code[48];
    volatile struct ColorDmaDraftChannel *dma = (volatile struct ColorDmaDraftChannel *)REG_DMA3;

    dma->source = ColorBuffer_ScaleThreeQuartersKernelCode;
    dma->destination = code;
    dma->control = 0x84000000 | 48;
    ((void (*)(u8 *, u8 *, u32))code)(buffer, backup, bytes);
}

void ColorBuffer_Halve(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 code[32];
    volatile struct ColorDmaDraftChannel *dma = (volatile struct ColorDmaDraftChannel *)REG_DMA3;

    dma->source = ColorBuffer_HalveKernelCode;
    dma->destination = code;
    dma->control = 0x84000000 | 32;
    ((void (*)(u8 *, u8 *, u32))code)(buffer, backup, bytes);
}

void ColorBuffer_Brighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 code[34];
    volatile struct ColorDmaDraftChannel *dma = (volatile struct ColorDmaDraftChannel *)REG_DMA3;

    dma->source = ColorBuffer_BrightenKernelCode;
    dma->destination = code;
    dma->control = 0x84000000 | 34;
    ((void (*)(u8 *, u32, u8 *, u32))code)(buffer, amount, backup, bytes);
}

void ColorBuffer_Darken(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 code[45];
    volatile struct ColorDmaDraftChannel *dma = (volatile struct ColorDmaDraftChannel *)REG_DMA3;

    dma->source = ColorBuffer_DarkenKernelCode;
    dma->destination = code;
    dma->control = 0x84000000 | 45;
    ((void (*)(u8 *, u32, u8 *, u32))code)(buffer, amount, backup, bytes);
}
