/*
 * resource_3bb owner at 0x0200004c, 2 bytes: `bx lr`. A DO-NOTHING function.
 *
 * The whole owner is one halfword. It takes no argument it reads, touches no
 * memory, and returns immediately -- and it is PUBLISHED TWICE, from words at
 * image offsets 0x4928, 0x4934, which are data-table slots and not the header. Two
 * table entries install it as a handler, so this is a deliberate no-op
 * callback, not padding and not dead space.
 *
 * SPAN IS 2 BYTES. There is no prologue, no pool, and nothing past the `bx
 * lr`: the halfword at the next offset is zero alignment before the first
 * recorded owner. A two-byte owner is the smallest thing on this tree and is
 * exactly what an unkeyed sweep is for -- it has no push for sweep C, and it
 * would read as alignment to anyone measuring by eye.
 *
 * WHAT IT DOES NOT SETTLE: the return value. `bx lr` leaves r0 untouched, so
 * a caller reading a result gets whatever it passed in. Whether the two table
 * slots expect a void handler or an identity is not decidable from this row.
 *
 * 0 of 0 callees.
 */

void Func_0200004c(void)
{
}
