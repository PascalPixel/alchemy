/*
 * resource_3c6 cutscene script at 0x02001090, 300 bytes
 * (0x02001090-0x020011bb).
 *
 * Complete owner: `push {lr}` at 0x02001090 through the single interworking
 * epilogue `pop {r0} / bx r0` at 0x020011b4.  0x020011b8-0x020011bb is this
 * owner's literal pool (0x000028b0), past the `bx` and never reached as an
 * instruction; the next inventory row begins at 0x020011bc.  The epilogue
 * pops the return address into r0, so the owner is void.  Nothing reads
 * r0-r3 before they are first written, so the owner takes no arguments.
 *
 * Entirely straight-line: no branch of any kind between the prologue and the
 * epilogue, so the body is a literal transcription of the script.  Slots 1, 2,
 * 3, 22 and the group selectors 0/1/2/3 are the only entities touched.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c).
 * The single pool word here is a small integer, not an address.
 *
 * Call accounting: 37 `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c6 1090` - 7 distinct targets,
 * all import veneers, no prologue and no `call_via` site.  The multiset of
 * `Func_...(` calls below equals that multiset exactly: Func_0808a010 x16,
 * Func_0808a180 x7, Func_0808a1e8 x5, Func_0808a110 x4, Func_0808a138 x2,
 * Func_0808a1b8 x2, Func_0808a170 x1.  (The inventory's `calls=37` agrees.)
 *
 * UNCERTAINTY: Func_0808a1e8's second argument is 256 at four sites and 258
 * at one, each built as `movs r1,#128` / `movs r1,#129` followed by
 * `lsls r1, r1, #1`.  Two neighbouring animation ids rather than a flag pair
 * is the reading that fits the surrounding waits, but it is not proven here.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay. */
void Func_0200265e();
void Func_020025c4();
void Func_02002654();
void Func_020025d2();
void Func_0200268a();
void Func_020025e0();
void Func_02002660();
void Func_020025ee();
void Func_020026a6();
void Func_020025fc();
void Func_020026d0();
void Func_020026dc();
void Func_020026e8();
void Func_020026f4();
void Func_02002632();
void Func_020026c2();
void Func_02002640();
void Func_020026f8();
void Func_0200264e();
void Func_020026ce();
void Func_0200265c();
void Func_02002714();
void Func_0200266a();
void Func_0200273e();
void Func_0200272e();
void Func_02002684();
void Func_02002704();
void Func_02002692();
void Func_0200274a();
void Func_020026a0();
void Func_02002764();
void Func_020026b2();
void Func_02002776();
void Func_020026c4();
void Func_02002744();
void Func_020026d2();
void Func_0200278a();
                                /* wait n frames */
                                /* scene-presentation request */
                                /* select an entity presentation */
                                /* show a dialogue line by id */
                                /* wait for the slot's action to finish */
                                /* face the slot along a heading */
                                /* play animation id for n frames */

void Func_02001090(void)
{
    Func_0200265e(0x28b0);
    Func_020025c4(20);

    Func_02002654(22, 2);
    Func_020025d2(20);
    Func_0200268a(22, 0);
    Func_020025e0(10);

    Func_02002660(22, 4);
    Func_020025ee(20);
    Func_020026a6(22, 0);
    Func_020025fc(10);

    Func_020026d0(0, 256, 0);
    Func_020026dc(1, 256, 0);
    Func_020026e8(3, 256, 0);
    Func_020026f4(2, 256, 55);
    Func_02002632(10);

    Func_020026c2(1, 2);
    Func_02002640(20);
    Func_020026f8(1, 0);
    Func_0200264e(10);

    Func_020026ce(22, 4);
    Func_0200265c(20);
    Func_02002714(22, 0);
    Func_0200266a(10);

    Func_0200273e(3, 258, 40);
    Func_0200272e(3, 0);
    Func_02002684(10);

    Func_02002704(1, 3);
    Func_02002692(30);
    Func_0200274a(1, 0);
    Func_020026a0(10);

    Func_02002764(2, 0x8000, 0);
    Func_020026b2(65);
    Func_02002776(2, 0xc000, 0);
    Func_020026c4(40);

    Func_02002744(2, 3);
    Func_020026d2(30);
    Func_0200278a(2, 0);
}
