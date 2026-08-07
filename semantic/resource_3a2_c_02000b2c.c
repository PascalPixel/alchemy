/*
 * resource_3a2 owner at 0x02000b2c, 260 bytes.  Complete owner: `push {lr}`
 * prologue at 0x02000b2c and the interworking return `pop {r0} / bx r0` at
 * 0x02000c24.  r0 holds the popped return address, so the owner returns
 * nothing; it takes no arguments.  The body is straight-line: a control-flow
 * walk from the prologue contains no branch at all, so every halfword from
 * 0x02000b2c to 0x02000c27 is code and the two words that follow are pool.
 *
 * Literal pool at 0x02000c28-0x02000c2f, inside the row's 260-byte span and
 * never reached as an instruction:
 *   0x02000c28 = 0x000019cf  text/cue id
 *   0x02000c2c = 0x000008b1  closing cue id
 *
 * All 32 call sites resolved with `bun tools/lib/overlay_call_targets.ts
 * resource_3a2 0b2c --json`; they collapse to 17 distinct veneer targets (the
 * inventory's `calls=30` is a known lower bound).  The veneer map used below:
 *   0x14d8 -> Func_080770c8   0x1580 -> Func_0808a170
 *   0x14e8 -> Func_0808a010   0x1598 -> Func_0808a188
 *   0x14f0 -> Func_0808a018   0x15a8 -> Func_0808a1b8
 *   0x14f8 -> Func_0808a020   0x15b8 -> Func_0808a1e8
 *   0x1558 -> Func_0808a100   0x15c0 -> Func_0808a1f0
 *   0x1560 -> Func_0808a110   0x15d0 -> Func_0808a218
 *   0x1570 -> Func_0808a130   0x15d8 -> Func_0808a220
 *   0x1578 -> Func_0808a138   0x15e8 -> Func_0808a360
 *                             0x15f0 -> Func_0808a370
 *
 * Placed multiset (32 = 32): Func_0808a010 x6, Func_0808a188 x5,
 * Func_0808a1b8 x3, Func_0808a110 x3, Func_0808a138 x2, and one each of
 * Func_0808a018, Func_0808a360, Func_0808a370, Func_0808a220, Func_0808a218,
 * Func_0808a170, Func_0808a1e8, Func_0808a130, Func_0808a1f0, Func_0808a100,
 * Func_0808a020, Func_080770c8.
 *
 * Behaviour: a long scripted conversation beat driving actors 8, 9 and 10.
 * Func_0808a010(n) waits n frames, Func_0808a188(actor, 0, 20) and
 * Func_0808a1b8(actor, heading, 20) are the movement/turn pair, Func_0808a110
 * and Func_0808a138 change an actor's pose index, and Func_0808a170 posts the
 * dialogue cue.  Func_0808a018 opens the beat and Func_0808a020 closes it; the
 * trailing Func_080770c8(0x8b1) is the progress cue, the same shape as
 * semantic/overlays/resource_39f_c_020021b0.c's closing Func_080770c8(0x89d).
 *
 * The headings are all built by shift: 192<<7 = 0x6000, 208<<8 = 0xd000,
 * 176<<8 = 0xb000; the id 129<<1 = 258 appears twice.
 *
 * Uncertainties: Func_0808a218 is reached with no register set since the
 * preceding call, so it is spelled with no arguments.  Func_0808a1f0(10, 258)
 * sets only r0 and r1; r2 holds a call-clobbered value there and is not
 * asserted.  Old-style declarations keep every import's interface open, as
 * required for overlay sources.
 */

void Func_02002020();
void Func_0200211c();
void Func_02002128();
void Func_02002026();
void Func_020020f2();
void Func_0200212a();
void Func_02002126();
void Func_020020d6();
void Func_0200204c();
void Func_020020ea();
void Func_0200210c();
void Func_020020f4();
void Func_0200206a();
void Func_02002124();
void Func_02002082();
void Func_0200213c();
void Func_02002158();
void Func_02002152();
void Func_02002122();
void Func_020020b0();
void Func_0200218c();
void Func_020020ca();
void Func_02002184();
void Func_02002164();
void Func_020021be();
void Func_020020ec();
void Func_020021b8();
void Func_02002170();
void Func_02002114();
void Func_020020fa();

                     

                     

                     

                     

                     

void Func_02000b2c(void)
{
    Func_02002020();
    Func_0200211c();
    Func_02002128();

    Func_02002026(30);
    Func_020020f2(0, 0x6000, 0);
    Func_0200212a(9, 1);
    Func_02002126();

    Func_020020d6(8, 2);
    Func_0200204c(20);
    Func_020020ea(0x19cf);
    Func_0200210c(8, 0, 20);

    Func_020020f4(9, 1);
    Func_0200206a(20);
    Func_02002124(9, 0, 20);

    Func_020020f4(8, 3);
    Func_02002082(20);
    Func_0200213c(8, 0, 20);

    Func_02002158(10, 0xd000, 20);
    Func_02002152(10, 0, 20);

    Func_02002122(8, 3);
    Func_020020b0(60);
    Func_0200218c(8, 258, 60);

    Func_0200213c(8, 4);
    Func_020020ca(20);
    Func_02002184(8, 0, 20);

    Func_02002164(10, 2);
    Func_020021be(10, 258);
    Func_020020ec(60);
    Func_020021b8(10, 0xb000, 20);

    Func_02002170(9, 5);
    Func_02002114();
    Func_020020fa(0x8b1);
}
