#include "types.h"

/*
 * resource_3af owner at 0x020029d4, 424 bytes: a full scripted scene,
 * linear from end to end -- no conditional branch anywhere in the body,
 * so the whole thing is one straight sequence.
 *
 * Opens the scripted-scene bracket (Func_0808a018), registers the
 * 0x0200d160 data block shared with this overlay's 0x0200185c,
 * 0x02001920 and 0x02001d0c entries, and advances one frame.
 *
 * Staging: actor 20 at (0xb60000, 0x026a0000), actor 23 at (0xee0000,
 * 0x02720000), actor 22 at (0x010c0000, 0x02a60000). Actor 22 gets its
 * record's u16 +6 cleared and Func_0808a098 with the data pointer
 * 0x0200c980; actor 21 gets bit 7 set in record byte +89, the
 * 0xcccc/0x6666 fixed-point pair through Func_0808a090, and
 * Func_0808a098 with 0x0200c628 -- the same pair and the same pointer
 * actor 21 receives in the 0x0200185c scene entry, so this is the same
 * actor in the same costume.
 *
 * The transition-mode write is the plain form: `movs r3,#128 /
 * lsls r3,#1` gives the value 256 and `movs r6,#224 / lsls r6,#1` the
 * displacement 448, with the workspace pointer cached in r5 and the
 * displacement in r6 across the whole body -- the closing write at
 * 0x02002b24 reuses both to store 0x209 (521). Note 521, not the
 * canonical 513 and not 0x202: transcribe the pool word.
 *
 * The beat itself is a long, ordinary run of scene calls; the only
 * things worth naming are the two intra-overlay helpers it leans on
 * heavily -- Func_02003a00(id, value) (four sites) and
 * Func_020039ec(id) (two sites), both already tracked byte-exact in
 * assets/code -- and Func_02000bb8, this overlay's once-only flag-0x271
 * reward grant, called near the end. That is the SECOND caller found
 * for 0xbb8, after 0x02001c14, and both calls sit inside a scripted
 * beat rather than a dispatcher.
 *
 * The close is a variation worth noting: the mode word is written and
 * then the scene ends with Func_0808a020 directly. There is no
 * Func_0808a368/Func_0808a370 pair, unlike 0x02001c14 and 0x02001d0c --
 * the mode write and the bracket helpers are not welded together.
 *
 * Story flags: sets 0x92b (Func_080770c8) and clears 0x302
 * (Func_080770d0) on the way out. 0x92b joins the 0x920/0x922/0x923
 * family that 0x02001684 tests, so this scene is a producer for that
 * arrangement chooser.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020029d4 through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02002b3a-0x02002b3e; fifteen
 * trailing pool words (0x0200d160, 0x026a0000, 0x02720000, 0x02a60000,
 * 0x0200c980, 0x00006666, 0x0000cccc, 0x0200c628, 0x03001ebc,
 * 0x00019999, 0x00001ee1, 0x00000105, 0x00000209, 0x0000092b,
 * 0x00000302) end at 0x02002b7b, and the next owner (0x02002b7c,
 * already tracked byte-exact) begins with `push {r5, lr}` at
 * 0x02002b7c.
 *
 * Second read done (over 400 bytes): every `movs`/`lsls` immediate pair
 * re-derived and every pool word re-checked against the resolved
 * address, not against pool order. The one correction it produced was
 * Func_0808a090(20, 0x19999, 0xcccc) -- the first argument word is
 * 0x00019999, a five-digit value, not the 0x9999 that appears
 * everywhere else in this overlay.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3af 29d4 2b7c, the +2
 * rule against the raw image).
 *
 * Uncertainty: actor ids and argument roles by shape; the coordinates,
 * fixed-point pairs and data pointers are transcribed, meanings
 * unknown.
 */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a030(s32 dataBlock);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_02000bb8(void);
extern void Func_02003a00(s32 id, s32 arg1);
extern void Func_020039ec(s32 id);

void Func_020029d4(void)
{
    u8 *record;
    u8 *workspace;

    Func_0808a018();
    Func_0808a030(0x0200d160);
    Func_080000c0(1);

    Func_0808a0f0(20, 182 << 16, 0x026a0000);
    Func_0808a0f0(23, 238 << 16, 0x02720000);
    Func_0808a0f0(22, 134 << 17, 0x02a60000);
    record = Func_0808a080(22);
    *(u16 *)(record + 6) = 0;
    Func_0808a098(22, 0x0200c980);
    record = Func_0808a080(21);
    record[89] |= 0x80;
    Func_0808a090(21, 0xcccc, 0x6666);
    Func_0808a098(21, 0x0200c628);

    *(s32 *)(workspace + 448) = 128 << 1;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    workspace = Data_03001ebc;

    Func_0808a090(20, 0x19999, 0xcccc);
    Func_0808a0d0(20, 182, 137 << 2);
    Func_02003a00(20, 0);
    Func_02003a00(0, 128 << 8);
    Func_0808a138(20, 1);
    Func_0808a170(0x1ee1);
    Func_020039ec(20);
    Func_0808a110(0, 3);
    Func_0808a010(40);
    Func_0808a1b8(20, 160 << 7, 20);
    Func_0808a1e8(20, 0x105, 60);
    Func_0808a188(20, 0, 40);
    Func_02003a00(20, 0);
    Func_020039ec(20);
    Func_0808a110(0, 3);
    Func_0808a110(20, 3);
    Func_0808a0d0(20, 182, 150 << 2);
    Func_0808a0d0(20, 216, 150 << 2);
    Func_02003a00(20, 192 << 8);

    Func_02000bb8();

    Func_0808a010(10);
    Func_0808a0d0(20, 216, 145 << 2);
    Func_0808a0f0(20, 0, 0);

    *(s32 *)(workspace + 448) = 0x209;
    workspace = Data_03001ebc;
    Func_080770c8(0x92b);
    Func_080770d0(0x302);
    Func_0808a020();
}
