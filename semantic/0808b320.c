#include "types.h"

s32 Func_080770c0(s32);

/* Scans the 16-bit record table at 0x0809e488 for the key formed from the
   (group, member) pair. The table is terminated by a zero record. A record
   with bit 15 set carries a 12-bit payload; the scan remembers the payload of
   the last flagged record it passed before reaching the key (or the end of the
   table) and publishes it at 0x0200042e.

   `payload` is genuinely uninitialised on the paths that reach the publish
   without passing a flagged record: the original code stores whatever the
   register held, and the two-argument signature is already fixed by the
   byte-exact caller src/08091eb0.c, so the live-in register is a local, not a
   hidden argument. */
void Func_0808b320(s32 group, s32 member)
{
    const s16 *entry;
    s32 key;
    s32 value;
    s32 payload;

    key = (u16)(s16)((group << 4) + member);
    entry = (const s16 *)0x0809e488;

    if (Func_080770c0(364) != 0) {
        payload = 18;
        goto publish;
    }

    value = (u16)*entry++;
    if (value == 0)
        goto publish;
    if (value == key)
        goto publish;

scan:
    if ((value & 0x8000) != 0)
        payload = value & 0x0fff;
    value = (u16)*entry++;
    if (value == 0)
        goto publish;
    if (value != key)
        goto scan;

publish:
    *(s16 *)0x0200042e = (s16)payload;
}
