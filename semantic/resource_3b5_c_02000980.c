#include "types.h"

/*
 * Resource 3b5, cutscene script step at 0x02000980 (808 bytes; 782 bytes of
 * code, two bytes of alignment, and the 24-byte literal pool at
 * 0x02000c90-0x02000ca7).  The pool map is from a control-flow walk: the owner
 * is straight-line apart from one two-armed test, nothing is branched over,
 * and everything from 0x02000c90 on is reached only as `ldr [pc]` data.
 *
 * Complete owner: `push {r5, lr}` at 0x02000980 and
 * `pop {r5} / pop {r0} / bx r0` at 0x02000c88-0x02000c8c.  r0 holds the popped
 * return address: void.
 *
 * Role: named in advance by the cutscene script table, which holds 0x02008981
 * in a record's handler word — Func_02000980 + the Thumb bit under this
 * overlay's proven 0x02008000 link base.  No arguments.  It is the overlay's
 * climax beat: it stages actors 29, 30 and 32, plays the effect, then splits
 * into a played and a skipped variant.
 *
 * r5 is a long-lived alias that is REASSIGNED mid-owner, which an argument
 * simulator gets silently wrong: it holds the text id 0x1fb6 from 0x020009b4
 * (bumped to 0x1fb9 by `adds r5, #3` at 0x02000a96, which is the id passed to
 * Func_08015038), and from 0x02000ab0 onward it holds the 0x03001ebc
 * workspace-pointer cell instead.  Tracked per use below, not as one variable.
 *
 * The store at 0x02000abc is the displacement/value trap: r2 is built as
 * 250 << 1 = 500 for the displacement, then reloaded with 32 as the value.
 * It is workspace[500] = 32.
 *
 * The eight-instruction sequence at 0x02000be6-0x02000bf2 is the project's
 * skip-beat counter (`movs r3, #236 / lsls #1` off the 0x03001ebc workspace
 * pointer, i.e. the u16 at workspace + 472).  It appears on only one arm.
 *
 * The two arms are NOT merged even where they run the same import sequence:
 * per project rule the multiset is over call SITES, not over distinct
 * behaviour, and folding them would deflate it by roughly twenty.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`; the inventory's
 * `calls=80` is a documented lower bound on distinct targets.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
void Func_020016e0();
void Func_020017c2();
void Func_02001728();
void Func_02001736();
void Func_020017ae();
void Func_0200177c();
void Func_0200178a();
void Func_020017ca();
u8 *Func_02001760();
void Func_0200170e();
void Func_020017ac();
void Func_02001796();
void Func_020017a0();
void Func_020017b4();
void Func_02001840();
void Func_020017d6();
void Func_020017ee();
void Func_020017f6();
void Func_020017fe();
void Func_02001828();
void Func_02001832();
void Func_020017b0();
void Func_0200188a();
void Func_02001894();
void Func_02001844();
void Func_02001854();
void Func_020017da();
void Func_020017e8();
void Func_020017de();
void Func_020017e2();
s32 Func_02001838();
void Func_02001822();
void Func_020018aa();
void Func_02001830();
void Func_02001902();
void Func_02001846();
void Func_020018be();
void Func_02001926();
void Func_02001864();
void Func_0200191c();
void Func_02001872();
void Func_02001946();
void Func_02001952();
void Func_02001890();
void Func_02001900();
void Func_02001910();
void Func_020018a6();
void Func_020018e0();
void Func_020018ea();
void Func_02001904();
void Func_020018ca();
void Func_02001914();
void Func_02001932();
void Func_02001924();
void Func_02001936();
void Func_020018f6();
void Func_0200197e();
void Func_020019d6();
void Func_0200191a();
void Func_02001992();
void Func_02001928();
void Func_020019fa();
void Func_02001938();
void Func_020019fe();
void Func_02001954();
void Func_02001a28();
void Func_02001a34();
void Func_02001972();
void Func_020019e2();
void Func_020019f2();
void Func_02001988();
void Func_020019c2();
void Func_020019cc();
void Func_020019e6();
void Func_020019f8();
void Func_02001a1a();
void Func_02001a24();
void Func_02001a2e();
void Func_020019ce();
void Func_020019ea();

                     

                     

                     

                     

                     

                     

                     

                     

void Func_02000980(void)
{
    s32 text = 0x1fb6;
    u8 *workspace;
    u16 *skipped;

    Func_020016e0();
    Func_020017c2(-1, -1, -1, 0);

    Func_02001728(29, 0x10000, 0x8000);
    Func_02001736(30, 0x10000, 0x8000);

    Func_020017ae(text);

    Func_0200177c(29, 0x480000, 0xd00000);
    Func_0200178a(30, 0x380000, 0xd00000);

    Func_020017ca(32, 15);
    Func_0200170e(Func_02001760(32), 0);
    Func_020017ac(32, 0x5f0000, 0x280000);

    Func_02001796(29, 72, 248);
    Func_020017a0(30, 56, 248);
    Func_020017b4(0, 64, 264);
    Func_02001840(0, 0xc000, 0);

    Func_020017d6(29);
    Func_020017ee(29, 1);
    Func_020017f6(30, 1);
    Func_020017fe(0, 1);
    Func_02001828(29, 0, 0);
    Func_02001832(30, 0, 0);

    Func_020017b0(20);
    Func_0200188a(29, 258);
    Func_02001894(30, 258);
    Func_02001844(29, 2);
    Func_02001854(30, 2);
    Func_020017da(20);
    Func_0200188a(29, 0);
    Func_020017e8(25);

    Func_020017de(52, 0, 12, 7);
    text += 3;
    Func_020017e2(text, 11, 12, 2);

    *(s32 *)(workspace + 500) = 32;
    workspace = *(u8 **)0x03001ebc;

    if (Func_02001838(0, 0) == 0) {
        Func_02001822(20);
        Func_020018aa(30, 2);
        Func_02001830(30);
        Func_02001902(30, 0, 0);
        Func_02001840(30);
        Func_02001846(10);
        Func_020018be(29, 3);
        Func_02001854(20);
        Func_02001926(29, 0, 0);
        Func_02001864(30);
        Func_0200191c(29, 0);
        Func_02001872(20);
        Func_02001946(29, 0x4000, 0);
        Func_02001952(30, 0x4000, 0);
        Func_02001890(30);
        Func_02001900(29, 3);
        Func_02001910(30, 3);
        Func_020018a6(20);
        Func_020018e0(29, 0x1cccc, 0xe666);
        Func_020018ea(30, 0x1cccc, 0xe666);
        Func_02001904(29, 232, 248);
        Func_020018ca(2);
        Func_02001914(30, 232, 248);
        Func_02001932(29);
        Func_02001924(29, 248, 248);
        Func_02001936(30, 248, 248);
    } else {
        Func_020018f6(20);
        Func_0200197e(30, 2);
        Func_02001904(30);
        Func_020019d6(30, 0, 0);
        Func_02001914(30);
        Func_0200191a(10);
        Func_02001992(29, 4);
        Func_02001928(20);
        Func_020019fa(29, 0, 0);
        Func_02001938(30);

        /* Skip-beat counter: the u16 at workspace + 472. */
        *skipped = (u16)(*skipped + 1);
        skipped = (u16 *)(*(u8 **)0x03001ebc + 472);

        Func_020019fe(29, 0);
        Func_02001954(20);
        Func_02001a28(29, 0x4000, 0);
        Func_02001a34(30, 0x4000, 0);
        Func_02001972(30);
        Func_020019e2(29, 3);
        Func_020019f2(30, 3);
        Func_02001988(20);
        Func_020019c2(29, 0x19999, 0xcccc);
        Func_020019cc(30, 0x19999, 0xcccc);
        Func_020019e6(29, 72, 184);
        Func_020019f8(30, 56, 184);
    }

    Func_02001a1a(29, 0, 0);
    Func_02001a24(30, 0, 0);
    Func_02001a2e(32, 0, 0);

    Func_020019ce(0x8c0);
    Func_020019ea();
}
