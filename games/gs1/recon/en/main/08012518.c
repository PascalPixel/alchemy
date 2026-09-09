/* main:08012518 - interactive icon/metadata monitor loop.
 *
 * The routine never returns: it sets up a small four column editor over the
 * shared icon grid (the ui/icon helpers Ui_SetGridColumnByte5/Byte6,
 * Ui_SetGridColumnNumber and Ui_FillGridColumnFromMetadata each push one
 * field of one column) and then spins forever on the pad.
 *
 * Outer stage (label "restart" below) rebuilds the scene: it clears the
 * scheduler, reallocates the 160 byte work block, re-registers the resource
 * for the number held in column 0, rebuilds the ten grid records and pushes
 * every column's current state back through the helpers.
 *
 * Inner stage reads the two shared pad cells: ADDR_03001AE8 is the held mask
 * and ADDR_03001B04 is the repeat/trigger mask.  With the held bit 8 down the
 * direction keys nudge the (x, y) placement handed to Func_08012b2c; otherwise
 * they move the column cursor `slot` and the edited field selector `mode`.
 * Shoulder-shaped bits 256/512 in the trigger mask decrement/increment the
 * field named by `mode`, trigger bit 1 applies or toggles a column, and
 * trigger bit 4 leaves through Func_08002f0c and restarts the outer stage.
 *
 * Reading aids the reader should not misread:
 *   - Ui_SetGridColumnByte5 and Ui_SetGridColumnByte6 are named for the grid
 *     record field they write, not for the column field they read.  This owner
 *     hands column offset 5 to Ui_SetGridColumnByte6 and column offset 6 to
 *     Ui_SetGridColumnByte5; the reference's ldrsb displacements are the
 *     evidence, and the two are not interchangeable.
 *   - The pointer Func_0800bc70 returns is the 56 byte record
 *     ResourceMetadata_Register walks.  Byte 38 of that record is the same
 *     byte the ADDR_03001C94 toggle loop below rewrites with a stride of 56.
 *   - The project registers Func_08002f0c as RuntimeDispatch_ReservedStatusZero
 *     and its recovered body takes no arguments and returns 0, yet this call
 *     site sets up two.  The raw spelling is kept so the call site stays
 *     honest about what the reference passes.
 *   - The first DMA fill really does transfer one word into the 160 byte work
 *     block: the pooled control word is 0x85000001.  That is the reference's
 *     behaviour, not a truncation here.
 *
 * Uncertain: the roles of the two byte fields at column offsets 5 and 6 (only
 * their 0..3 and 0..15 wrap ranges are evidenced), the meaning of the mask
 * bits, and what the 0x08185000 argument handed to Func_08002f0c selects.
 *
 * Known residual against the reference, all compiler-decision rather than
 * control flow:
 *   - The reference keeps the held-mask pointer in fp and the trigger-mask
 *     pointer in r9 and spills `mode` to the frame; this spelling keeps
 *     `mode` in r9 and rematerialises one of the two addresses, which costs
 *     four bytes of frame and shifts every stack displacement.
 *   - The reference dispatches the switch with an unsigned compare chain.
 *     Spelling `mode` (or the switch operand) unsigned reproduces that chain
 *     exactly but lets the compiler cross-jump the two duplicated case 0
 *     tails, dropping fourteen real instructions, so it is not used here.
 *   - The reference merges each DMA register triple into one stmia; through
 *     a volatile register block the three stores stay separate.
 *   - The reference materialises the constants 1, 17 and 18 from the literal
 *     pool where an ordinary literal here becomes an immediate.
 */

#include "types.h"
#include "global_cells.h"
#include "metadata_lookup.h"

#define Ui_RunIconMonitor Func_08012518

/* One editable column of the icon grid.  Eight bytes: the reference indexes
 * the array with slot << 3 and reads offsets 0, 2, 4, 5 and 6 only. */
struct IconColumn {
    s16 no;  /* metadata number displayed in this column */
    s16 val; /* 0..99 value handed to Ui_FillGridColumnFromMetadata */
    u8 on;   /* column applied */
    s8 b5;   /* wraps 0..3;  pushed through Ui_SetGridColumnByte6 */
    s8 b6;   /* wraps 0..15; pushed through Ui_SetGridColumnByte5 */
};

/* DMA channel 3 registers; the two fills below zero the work block and the
 * column array. */
struct DmaChannel {
    const void *src;
    void *dst;
    u32 cnt;
};

void *Runtime_AllocateBlock(s32, s32);
void Resource_InitializeTable(void);
void Scheduler_ResetTaskTable(void);
void Scheduler_AddOrUpdateCallback(s32, s32);
void Func_08004858(void);
void Blend_SetDarkenTarget0(s32);
void Func_080030f8(s32);
void Func_08002f0c(s32, void *);
void Func_0800c004(s32);
void Func_0800b6b8(s32, void *, s32, s32);
u8 *Func_0800bc70(s32);
s32 ResourceMetadata_Register(u8 *, s32);
s32 Ui_FindNextNumberWithMetadata(s32, s32);
void Func_08012b2c(s32, s32, void *);
void Ui_SetGridColumnByte5(s32, s32);
void Ui_SetGridColumnByte6(s32, s32);
void Ui_FillGridColumnFromMetadata(s32, s32);
void Ui_SetGridColumnNumber(s32, s32);

void Ui_RunIconMonitor(void)
{
    struct IconColumn ent[4];
    volatile struct DmaChannel *dma;
    volatile u32 *keys;
    volatile u32 *trig;
    void *work;
    u8 *md;
    u8 *h;
    u8 *tbl;
    s32 zero;
    s32 no;
    s32 x;
    s32 y;
    s32 mode;
    s32 flag;
    s32 slot;
    u32 cnt;
    s32 alt;
    s32 moved;
    u32 i;

    keys = (volatile u32 *)ADDR_03001AE8;
    trig = (volatile u32 *)ADDR_03001B04;
    x = 144;
    y = 96;
    mode = 1;
    flag = 1;
    slot = 0;
    work = Runtime_AllocateBlock(9, 160);
    *(u8 *)0x03001C90 = 3;

    dma = (volatile struct DmaChannel *)0x040000D4;
    zero = 0;
    dma->src = &zero;
    dma->dst = work;
    dma->cnt = 0x85000001;
    zero = 0;
    dma->src = &zero;
    dma->dst = ent;
    dma->cnt = 0x85000008;

    no = Ui_FindNextNumberWithMetadata(-1, 1);
    for (i = 0; i <= 3; i++) {
        ent[i].val = 1;
        ent[i].b5 = 1;
        ent[i].no = no;
    }
    ent[0].on = 1;
    *(u8 *)0x03001F54 = 2;
    *(volatile u16 *)0x04000050 = 0x3F42;
    *(volatile u16 *)0x05000000 = 0x01E0;
    *(volatile u16 *)0x04000000 = 0x1140;
    Blend_SetDarkenTarget0(1);

restart:
    Func_08004858();
    Scheduler_ResetTaskTable();
    work = Runtime_AllocateBlock(9, 160);
    Resource_InitializeTable();
    Func_0800c004(2);
    Func_0800b6b8(0, (void *)0x02010000, ent[0].no, 0);
    md = (u8 *)Func_08185000(ent[0].no);
    if (md[4] == 20) {
        Func_0800b6b8(1, (void *)0x02018000, ent[0].no + 1, 0);
    }

    for (i = 0; i <= 9; i++) {
        md = (u8 *)Func_08185000(ent[0].no);
        alt = 0;
        if (md[4] == 20 && (i & 1) != 0) {
            alt = 1;
        }
        h = Func_0800bc70(ent[0].no + alt + (alt << 12));
        ResourceMetadata_Register(h, ent[1].no);
        ResourceMetadata_Register(h, ent[2].no);
        ResourceMetadata_Register(h, ent[3].no);
        h[38] = (u8)flag;
    }

    for (i = 0; i <= 3; i++) {
        if (ent[i].on != 0) {
            Ui_SetGridColumnByte6(i, ent[i].b5);
        } else {
            Ui_SetGridColumnByte6(i, 8);
        }
        Ui_SetGridColumnByte5(i, ent[i].b6);
        Ui_FillGridColumnFromMetadata(i, ent[i].val);
    }

    Func_08012b2c(x, y, work);
    /* Thumb entry of the per-frame hook that draws the monitor. */
    Scheduler_AddOrUpdateCallback(0x08012E29, 3200);

    for (;;) {
        Func_080030f8(1);

        if ((*keys & 8) != 0) {
            if ((*keys & 32) != 0) {
                x--;
            }
            if ((*keys & 16) != 0) {
                x++;
            }
            if ((*keys & 64) != 0) {
                y--;
            }
            if ((*keys & 128) != 0) {
                y++;
            }
            Func_08012b2c(x, y, work);
        } else {
            /* The reference opens this arm with a load of the trigger cell
               whose result is discarded before the first test. */
            (void)*trig;
            if ((*trig & 64) != 0) {
                slot = (slot - 1) & 3;
            }
            if ((*trig & 128) != 0) {
                slot = (slot + 1) & 3;
            }
            if ((*trig & 32) != 0) {
                mode = (mode - 1) & 3;
            }
            if ((*trig & 16) != 0) {
                mode++;
                mode = mode & 3;
            }
        }

        if ((*(s32 *)ADDR_03001C94 & 8) != 0) {
            tbl = *(u8 **)ADDR_03001E60;
            flag = flag ^ 1;
            tbl += 38;
            for (i = 0; i <= 9; i++) {
                *tbl = (u8)flag;
                tbl += 56;
            }
        }

        switch (mode) {
        case 0:
            if (slot == 1) {
                break;
            }
            if ((*trig & 512) != 0) {
                ent[slot].val--;
                if (ent[slot].val < 0) {
                    ent[slot].val = 0;
                }
                if (slot == 0) {
                    ent[1].val = ent[0].val;
                }
                continue;
            }
            if ((*trig & 256) == 0) {
                break;
            }
            ent[slot].val++;
            if (ent[slot].val > 99) {
                ent[slot].val = 99;
            }
            if (slot == 0) {
                ent[1].val = ent[0].val;
            }
            continue;

        case 1:
            cnt = 1;
            moved = 0;
            if ((*keys & 8) != 0) {
                cnt = 10;
            }
            if ((*trig & 512) != 0) {
                for (i = 0; i < cnt; i++) {
                    ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, -1);
                }
                moved = 1;
            }
            if ((*trig & 256) != 0) {
                for (i = 0; i < cnt; i++) {
                    ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, 1);
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (slot == 0) {
                goto restart;
            }
            if (ent[slot].on == 0) {
                continue;
            }
            Ui_SetGridColumnNumber(slot, ent[slot].no);
            Ui_SetGridColumnByte6(slot, ent[slot].b5);
            Ui_SetGridColumnByte5(slot, ent[slot].b6);
            Ui_FillGridColumnFromMetadata(slot, ent[slot].val);
            continue;

        case 2:
            moved = 0;
            if ((*trig & 512) != 0) {
                ent[slot].b5--;
                if (ent[slot].b5 < 0) {
                    ent[slot].b5 = 3;
                }
                moved = 1;
            }
            if ((*trig & 256) != 0) {
                ent[slot].b5++;
                if (ent[slot].b5 > 3) {
                    ent[slot].b5 = 0;
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (ent[slot].on == 0) {
                continue;
            }
            Ui_SetGridColumnByte6(slot, ent[slot].b5);
            continue;

        case 3:
            moved = 0;
            if ((*trig & 512) != 0) {
                ent[slot].b6--;
                if (ent[slot].b6 < 0) {
                    ent[slot].b6 = 15;
                }
                moved = 1;
            }
            if ((*trig & 256) != 0) {
                ent[slot].b6++;
                if (ent[slot].b6 > 15) {
                    ent[slot].b6 = 0;
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (ent[slot].on == 0) {
                continue;
            }
            Ui_SetGridColumnByte5(slot, ent[slot].b6);
            continue;

        default:
            break;
        }

        if ((*trig & 1) != 0) {
            if (mode == 0) {
                if (slot != 1) {
                    if (ent[slot].on == 0) {
                        continue;
                    }
                    Ui_FillGridColumnFromMetadata(slot, ent[slot].val);
                    if (slot != 0) {
                        continue;
                    }
                    Ui_FillGridColumnFromMetadata(1, ent[1].val);
                    continue;
                }
            } else if (slot != 0) {
                ent[slot].on = ent[slot].on ^ 1;
                if (ent[slot].on != 0) {
                    Ui_SetGridColumnByte6(slot, ent[slot].b5);
                } else {
                    Ui_SetGridColumnByte6(slot, 8);
                }
                continue;
            }
        }

        if ((*trig & 4) != 0) {
            Scheduler_ResetTaskTable();
            if ((*keys & 2) != 0) {
                Func_08002f0c(17, (void *)0x08185000);
            } else {
                Func_08002f0c(18, (void *)0x08185000);
            }
            goto restart;
        }
    }
}
