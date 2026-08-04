typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_377 owner at 0x020001e0, 164 bytes.  Complete owner: 'push {lr}'
 * prologue and the interworking return 'pop {r0} ; bx r0'.  r0 holds the popped
 * return address, so the owner returns nothing; the first call is the
 * argument-less scene bracket, so the owner takes no argument either.
 *
 * A straight-line scripted beat on scene actor 16, bracketed by the established
 * Func_0808a018 / Func_0808a020 pair, with one conditional: when the runtime
 * query Func_0808a070(0, 0) returns exactly 1 the skip-beat counter -- the u16
 * at workspace + 472, the same slot assets/code/resource_383_c_020003bc.c bumps
 * -- is incremented and nothing else changes.  That is the family's
 * "empty else that only increments something" idiom, so the two variants of this
 * beat are the same length.
 *
 * Literal pool: owner offsets 0x9c..0xa3 (0x0200027c and 0x02000280), preceded
 * by an alignment 'movs r0,r0' at 0x0200027a.  The control-flow walk from the
 * prologue ends at the epilogue (0x02000276), so the tail is pool: 0x00000f5b is
 * a cue id and 0x03001ebc is the family workspace pointer cell.
 *
 * All seventeen calls resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 01e0 --json' (17 sites,
 * 11 distinct veneer targets).  Per-target multiset, which the C below matches
 * exactly: Func_0808a010 x4, Func_0808a130 x3, Func_0808a188 x2, and one each of
 * Func_0808a018, Func_0808a170, Func_0808a150, Func_0808a1e8, Func_0808a110,
 * Func_0808a178, Func_0808a070, Func_0808a020.  In order:
 *   0x01e2 -> 0x190c -> Func_0808a018()
 *   0x01ea -> 0x1994 -> Func_0808a130(16, 2)
 *   0x01f0 -> 0x1904 -> Func_0808a010(30)
 *   0x01f6 -> 0x19c4 -> Func_0808a170(0xf5b)
 *   0x0200 -> 0x19ac -> Func_0808a150(0, 16, 10)
 *   0x020a -> 0x19dc -> Func_0808a188(16, 0, 6)
 *   0x0216 -> 0x1a04 -> Func_0808a1e8(16, 258, 0)
 *   0x021e -> 0x1994 -> Func_0808a130(16, 1)
 *   0x0224 -> 0x1904 -> Func_0808a010(20)
 *   0x022c -> 0x1984 -> Func_0808a110(16, 4)
 *   0x0232 -> 0x1904 -> Func_0808a010(20)
 *   0x023a -> 0x19cc -> Func_0808a178(16, 0)
 *   0x0242 -> 0x1924 -> Func_0808a070(0, 0)   result compared against 1
 *   0x025e -> 0x1994 -> Func_0808a130(16, 1)
 *   0x0264 -> 0x1904 -> Func_0808a010(20)
 *   0x026e -> 0x19dc -> Func_0808a188(16, 0, 4)
 *   0x0272 -> 0x1914 -> Func_0808a020()
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 *
 * 'movs r1,#129 ; lsls r1,r1,#1' is 258.
 */

extern u8 *Data_03001ebc;

void Func_0808a018();
void Func_0808a130();
void Func_0808a010();
void Func_0808a170();
void Func_0808a150();
void Func_0808a188();
void Func_0808a1e8();
void Func_0808a110();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a020();

void Func_020001e0(void)
{
    Func_0808a018();

    Func_0808a130(16, 2);
    Func_0808a010(30);
    Func_0808a170(0xF5B);
    Func_0808a150(0, 16, 10);
    Func_0808a188(16, 0, 6);
    Func_0808a1e8(16, 258, 0);

    Func_0808a130(16, 1);
    Func_0808a010(20);
    Func_0808a110(16, 4);
    Func_0808a010(20);
    Func_0808a178(16, 0);

    if (Func_0808a070(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }

    Func_0808a130(16, 1);
    Func_0808a010(20);
    Func_0808a188(16, 0, 4);

    Func_0808a020();
}
