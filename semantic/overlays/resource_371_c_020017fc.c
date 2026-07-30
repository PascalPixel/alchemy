typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 371 owner at 0x020017fc (140 bytes, 14 distinct call targets).
 *
 * Complete owner: `push {r5, lr}` at 0x020017fc and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x0200186c.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes 0x02001872-0x02001887 are
 * alignment plus the literal pool (0x6666, 0x3333, 0x14a8, 0x918, 0x927).
 *
 * A cutscene set-up step: acquire actor 8, then issue a fixed sequence of
 * camera/actor commands.  Every call is accounted for; the 14 targets in
 * source order are 0x02005afe, 0x02005ae4, 0x02005c0e, 0x020059a4,
 * 0x02005b86, 0x02005c1e, 0x02005c7a, 0x02005b54, 0x02005b96, 0x02005c9a,
 * 0x02005ca6, 0x02005af4, 0x02005c72, 0x02005b4e — matching the inventory's
 * distinct-target count of 14 exactly.
 *
 * Argument recovery: `bl 0x02005ae4` at 0x02001806 sets no register, but r0
 * still holds the object returned by 0x02005afe (it was only copied into r5,
 * not overwritten), so the dataflow is preserved as an argument.  The five
 * calls that set nothing at all and follow another `bl` — 0x02005c7a,
 * 0x02005c9a, 0x02005ca6, 0x02005b4e — take call-clobbered registers, so no
 * arguments are asserted for them.
 *
 * `movs #1 ; negs` three times builds the (-1, -1, -1, 0) argument list at
 * 0x0200180a, and 160 << 9 is the 0x14000 written to +0x18/+0x1c (the same
 * amplitude pair the owners at 0x02003fb4/0x02004004 write).
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style
 * declarations, because the interfaces are unknown and one import can be
 * reached with different argument counts.
 */

u8 *Func_02005afe();
void Func_02005ae4();
void Func_02005c0e();
void Func_020059a4();
void Func_02005b86();
void Func_02005c1e();
void Func_02005c7a();
void Func_02005b54();
void Func_02005b96();
void Func_02005c9a();
void Func_02005ca6();
void Func_02005af4();
void Func_02005c72();
void Func_02005b4e();

void Func_020017fc(void)
{
    u8 *actor = Func_02005afe(8);

    Func_02005ae4(actor);
    Func_02005c0e(-1, -1, -1, 0);
    Func_020059a4(1);
    Func_02005b86(0, 0, 0);

    *(s32 *)(actor + 0x1c) = 0x14000;
    *(s32 *)(actor + 0x18) = 0x14000;

    Func_02005c1e(8, 1);
    Func_02005c7a();
    Func_02005b54(8, 0x6666, 0x3333);
    Func_02005b96(8, 0x14a8, 0x918);
    Func_02005c9a();
    Func_02005ca6();
    Func_02005af4(0x927);
    Func_02005c72(102);
    Func_02005b4e();
}
