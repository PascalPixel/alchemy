typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 3cb party-roster copy at 0x0200053c (68 bytes,
 * 0x0200053c-0x0200057f).
 *
 * Complete owner: `push {r5, lr}` prologue at 0x0200053c and the matching
 * interworking return `pop {r5} / pop {r1} / bx r1` at 0x02000572.  The popped
 * return address lands in r1, so r0 survives the epilogue and IS the result;
 * r0 still holds the clamped count computed at entry, so this owner returns
 * that count.
 *
 * The eight bytes at 0x02000578-0x0200057f are this owner's literal pool
 * (0x000000ff and 0x02000240).  They are reached only by the two
 * `ldr r3, [pc, ...]` sites and are never executed — the `lsls` pairs the
 * disassembler prints there are data.
 *
 * The single call site was resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2): 0x02000540 reaches the
 * import veneer at 0x02001834, whose trailing word is the main-image
 * Func_08077148.  sites=1 matches the inventory's calls=1.
 *
 * Data_02000240 is the overlay's own workspace record; the byte-exact sibling
 * assets/code/resource_3cb_c_02000060.c reads it as `s16 Data_02000240[]` at
 * index 225 (byte offset 450).  This owner reads a byte run starting at byte
 * offset 504 (built as 252 << 1), so it is declared as bytes here.  The same
 * run at +504 is scanned by Func_02000f30, which stops on a 0xff terminator —
 * consistent with the 0xff this owner appends.
 *
 * Behaviour: clamp the value returned by Func_08077148 to at most 3, then, if
 * it is positive, copy that many bytes from the roster run into the caller's
 * halfword buffer, zero-extending each byte.  A null destination pointer is
 * accepted and makes the whole thing a no-op walk (the source cursor still
 * advances, the destination is never written).  A 0x00ff terminator halfword
 * is appended when the destination is non-null.
 *
 * Uncertainty: the comparisons at 0x02000544 (`ble`) and 0x0200054c (`ble`)
 * are signed, so a negative Func_08077148 result skips the copy entirely and
 * is returned unchanged.  Nothing here establishes whether that can happen.
 */

extern u8 Data_02000240[];

/* Old-style declaration: the import's real interface is not established here,
 * but it is used for its return value so it needs a return type. */
s32 Func_08077148();

s32 Func_0200053c(u16 *dest)
{
    s32 count = Func_08077148();

    if (count > 3) {
        count = 3;
    }

    if (count > 0) {
        const u8 *src = &Data_02000240[504];
        s32 remaining = count;

        do {
            u8 entry = *src++;

            if (dest != 0) {
                *dest = (u16)entry;
                dest++;
            }
            remaining--;
        } while (remaining != 0);
    }

    if (dest != 0) {
        *dest = 0x00ff;
    }

    return count;
}
