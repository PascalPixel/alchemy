typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 3a3 scripted sequence at 0x0200015c (84 bytes,
 * 0x0200015c .. 0x020001af, of which 0x020001a6 .. 0x020001af is alignment
 * plus the literal pool).
 *
 * Complete owner: `push {lr}` at 0x0200015c and the interworking return
 * `pop {r0} / bx r0` at 0x020001a2, so the owner returns nothing.
 *
 * All 7 call sites are placed (row reports calls=7): Func_0808a018,
 * Func_0808a170, Func_0808a178, Func_0808a070, Func_0808a180, Func_0808a190,
 * Func_0808a020, one each.
 *
 * The `else` arm is the project's skip-beat counter, recognised by its
 * constants (`movs r3,#236 / lsls r3,#1` = 472, off the `0x03001ebc`
 * workspace pointer cell).  It bumps a u16 progress counter and then takes the
 * variant tail call.  The two arms are otherwise equal-length scene variants.
 *
 * `0x03001ebc` is a pointer CELL (`ldr r3,[pc] / ldr r2,[r3]`), not the
 * workspace.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
void Func_0808a018();       /* opens a scripted sequence */
void Func_0808a170();       /* queues a cue / script id */
void Func_0808a178();
s32 Func_0808a070();        /* result is compared against 1 */
void Func_0808a180();
void Func_0808a190();
void Func_0808a020();       /* closes the scripted sequence */

extern u8 *Data_03001ebc;   /* pointer cell holding the overlay workspace */

void Func_0200015c(void)
{
    u8 *workspace;

    Func_0808a018();
    Func_0808a170(0x18bd);
    /* r1 is set before r0; the argument order is unchanged. */
    Func_0808a178(8, 0);

    if (Func_0808a070(0, 0) == 1) {
        Func_0808a180(8, 0);
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        Func_0808a190(8, 0);
    }

    Func_0808a020();
}
