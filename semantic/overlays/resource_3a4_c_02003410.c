typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_3a4 owner at 0x02003410, 24 bytes: write 0x1018 into the
 * halfword at scene workspace + 382, and return.
 *
 * THE FOUNDING CASE FOR SWEEP D, and the only one of the five leaves
 * found on 2026-08-01 that fixing sweep B does NOT recover. It is
 * invisible three ways at once:
 *   - no `bl` anywhere in the image targets 0x3410, verified by
 *     resolving every BL-shaped halfword pair across the whole
 *     0x5238-byte image with the `+2` rule;
 *   - no word anywhere in the image holds 0x0200b411 or 0x0200b410, its
 *     published spelling, so even the un-gated sweep B cannot see it;
 *   - it has no `push` prologue for the shaped scan to key on, because
 *     it is a leaf.
 * It was found by arithmetic instead of by signature: 0x02003028's
 * recorded-owner bound said 1024 while the row measured 1000, and the
 * 24-byte remainder was read rather than waved through as pool and
 * padding.
 *
 * Complete owner: 0x02003410 through `bx lr` at 0x0200341e, plus the
 * two-word literal pool at 0x02003420-0x02003427 holding 0x03001ebc and
 * 0x00001018. Both pool words are read by `pc`-relative loads inside the
 * row, so they belong to it and to nothing else. The next recorded owner
 * is 0x02003428.
 *
 * The workspace pointer at 0x03001ebc is the same global 0x02003028
 * writes through, which is what places this row in that row's family
 * rather than leaving it an orphan: 0x02003028 stores to +448 and +456
 * as words, and this one stores to +382 as a halfword. The displacement
 * is built as 191 << 1, the same shifted-literal habit every row on this
 * overlay uses, and it is a plain displacement here -- NOT an instance
 * of the displacement-from-value chain, since 382 is never used as a
 * value and nothing is added to or subtracted from it.
 *
 * 0x1018 is loaded from the pool rather than built, which is why the row
 * needs a literal pool at all.
 *
 * No arguments and no return value: r0 is never read and never written.
 */

extern u8 *Data_03001ebc;   /* the scene workspace pointer */

void Func_02003410(void)
{
    *(u16 *)(Data_03001ebc + (191 << 1)) = 0x1018;
}
