typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 3b1 script-table selector at 0x0200037c.
 *
 * OWNER AND POOL MAP, from a control-flow walk.  The advertised 464-byte
 * span is exact and splits as:
 *   0x0200037c-0x0200039b  code (prologue and dispatch)
 *   0x0200039c-0x020003fb  DATA: the 24-entry `mov pc, r3` jump table
 *   0x020003fc-0x0200050f  code (the case bodies and the shared epilogue)
 *   0x02000510-0x0200054b  DATA: the trailing literal pool, 15 words
 * The jump table ABUTS its own first case body: the table ends at
 * 0x020003fb and case 0 starts at 0x020003fc with no branch between them,
 * so neither "run to the next label" nor "run to the next branch target"
 * finds the boundary.  Only the control-flow walk does.  The row's
 * advertised 36 "code" bytes are simply where the linear walk stopped.
 *
 * LINK BASE 0x02008000.  The table base is the pool word 0x0200839c against
 * a table physically at file offset 0x39c, and every entry points 0x8000
 * past the case body it selects, with no Thumb bit — correct for `mov pc`,
 * which does not interwork.  The values this owner RETURNS resolve the same
 * way: 0x0200eda8, 0x0200eeb0, 0x0200ed78, 0x0200efe8, 0x0200ed48,
 * 0x0200ec58, 0x0200ebf8 and 0x0200ebe0 are in-image data at file offsets
 * 0x6da8, 0x6eb0, 0x6d78, 0x6fe8, 0x6d48, 0x6c58, 0x6bf8 and 0x6be0 — all
 * inside the image, all even, and all in the big data band this overlay's
 * inventory rows 0x671c onward describe.  The neighbouring byte-exact
 * sources agree: assets/code/resource_3b1_c_02000340.c returns 0x0200e984
 * and 0x0200e96c the same way.
 *
 * Signature.  The return address is popped into r1, so r0 survives and IS
 * the result: this owner returns one of eight in-image table addresses.  It
 * takes no arguments.
 *
 * The selector is the SIGNED halfword at Data_02000240 + 450, built as
 * 225 << 1, then decremented by one before the unsigned range check — so
 * selector values 1..24 map to table indices 0..23 and everything else
 * (including 0 and any negative) falls to the default.  0x02000240 is the
 * cross-overlay RAM global block, not an in-image address: it lies BELOW the
 * 0x02008000 link band, so it cannot be a file offset here.  The byte-exact
 * sibling at 0x02000340 reads index 224 of the same block (byte offset 448),
 * one halfword earlier than this owner's 450.
 *
 * The overlay image is writable EWRAM, so the byte stores below are real
 * mutations of the returned tables, not a misreading of a const region.
 *
 * Eleven call sites, every one of them the event-flag test Func_080770c0,
 * against a manifest `calls=0` — the field comes from the linear walk that
 * stopped at the jump table.  Those eleven are also every Func_080770c0 site
 * in the whole overlay.  Several arms share a return, and each shared return
 * is written once with a `goto` rather than copied per arm: the eight `case`
 * groups below reach only six distinct bodies, and duplicating them would
 * inflate the per-target multiset.
 */

s32 Func_080770c0();  /* veneer 0x64a0 — event-flag test */

/* In-image script tables, at file offsets 0x6be0-0x6fe8 under the proven
 * 0x02008000 link base. */
#define TABLE_6BE0 ((u8 *) 0x0200ebe0)
#define TABLE_6BF8 ((u8 *) 0x0200ebf8)
#define TABLE_6C58 ((u8 *) 0x0200ec58)
#define TABLE_6D48 ((u8 *) 0x0200ed48)
#define TABLE_6D78 ((u8 *) 0x0200ed78)
#define TABLE_6DA8 ((u8 *) 0x0200eda8)
#define TABLE_6EB0 ((u8 *) 0x0200eeb0)
#define TABLE_6FE8 ((u8 *) 0x0200efe8)

s32 Func_0200037c(void)
{
    s32 selector;
    u8 *table;

    /* 225 << 1 = byte offset 450 into the cross-overlay RAM block. */
    selector = *(s16 *) ((u8 *) 0x02000240 + 450) - 1;

    if ((unsigned int) selector > 23) {
        goto table_6be0;
    }

    switch (selector) {
    case 0:
    case 1:
    case 10:
        if (Func_080770c0(0x93e) != 0) {
            goto table_6da8;
        }
        if (Func_080770c0(0x928) != 0) {
            /* 138 << 4. */
            if (Func_080770c0(0x8a0) != 0) {
                table = TABLE_6EB0;
                table[22] = 2;
                table[70] = 2;
                table[118] = 2;
                table[142] = 2;
                table[214] = 2;
                table[190] = 2;
                table[166] = 1;
                table[94] = 2;
            }
            return (s32) TABLE_6EB0;
        }
        if (Func_080770c0(0x911) != 0) {
            if (Func_080770c0(0x925) == 0) {
                goto table_6da8;
            }
            table = TABLE_6DA8;
            table[22] = 2;
            table[118] = 2;
            table[46] = 2;
            table[94] = 2;
            goto table_6da8;
        }
        return (s32) TABLE_6D78;

    case 3:
    case 11:
    case 15:
    case 17:
    case 19:
    case 20:
    case 22:
    case 23:
        return (s32) TABLE_6FE8;

    case 14:
    case 16:
    case 18:
        table = TABLE_6FE8;
        table[22] = 2;
        table[46] = 2;
        table[94] = 1;
        table[118] = 2;
        table[142] = 2;
        table[166] = 2;
        table[190] = 2;
        table[214] = 1;
        table[238] = 2;
        return (s32) TABLE_6FE8;

    case 4:
        if (Func_080770c0(0x93e) != 0) {
            return (s32) TABLE_6D48;
        }
        if (Func_080770c0(0x911) == 0) {
            goto table_6bf8;
        }
        if (Func_080770c0(0x922) == 0) {
            goto table_6be0;
        }
        if (Func_080770c0(0x8a0) != 0) {
            TABLE_6C58[46] = 1;
        }
        if (Func_080770c0(0x925) != 0) {
            if (Func_080770c0(0x8a0) == 0) {
                /* The stored value is the call's own r0, which is 0 here. */
                TABLE_6C58[22] = 0;
            }
        }
        return (s32) TABLE_6C58;

    case 9:
    case 12:
    case 13:
    case 21:
        goto table_6bf8;

    default:
        /* Indices 2, 5, 6, 7 and 8 all select this body. */
        goto table_6be0;
    }

table_6da8:
    return (s32) TABLE_6DA8;

table_6bf8:
    return (s32) TABLE_6BF8;

table_6be0:
    return (s32) TABLE_6BE0;
}
