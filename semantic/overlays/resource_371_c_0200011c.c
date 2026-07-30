typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

/*
 * Resource 371 owner at 0x0200011c (30 bytes, 1 call).
 *
 * Complete owner: `push {lr}` at 0x0200011c and the matching interworking
 * return `pop {r0} ; bx r0` at 0x02000136.  r0 holds the popped return
 * address there, so the owner returns nothing.
 *
 * Call-target convention (shared by every file in this overlay, semantic and
 * byte-exact alike): a `bl` is named by the address its call site computes.
 * That address is a stable identity for an import, not a place to
 * disassemble — this overlay is relocated at load time, and its encoded
 * branch targets land in the import veneer band above the code (0x02004320+
 * is an 8-byte-per-entry `ldr r4,[pc,#0] / bx r4 / .word <main-image
 * address>` table) or past the end of the image entirely.
 *
 * UNCERTAINTY: the original clobbers r4 as scratch without saving it
 * (`movs r4,#0 ; ldrsh r3,[r2,r4]` is only a sign-extending load of the
 * halfword at +0x64).  Nothing observable depends on it, so it is not
 * modelled.
 */

/* Old-style declaration: the import's interface is not known, and the same
 * import may be reached with different argument counts elsewhere. */
void Func_02004348();

void Func_0200011c(u8 *object)
{
    u16 *counter = (u16 *)(object + 0x64);

    if (*(s16 *)counter > 0) {
        Func_02004348(object);
    } else {
        *counter = (u16)(*counter + 1);
    }
}
