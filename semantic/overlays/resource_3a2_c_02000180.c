typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3a2 owner at 0x02000180, 108 bytes.  Complete owner: `push {lr}`
 * prologue at 0x02000180 and the interworking return `pop {r0} / bx r0` at
 * 0x020001d6.  r0 holds the popped return address, so the owner returns
 * nothing; it takes no arguments (r0 is set to 188 immediately).
 *
 * Straight-line body, no branches.  A four-word literal pool sits at
 * 0x020001dc-0x020001eb (0x020001da is the alignment halfword after the `bx`),
 * inside the row's 108-byte span but never reached by a control-flow walk from
 * the prologue, so it is data:
 *   0x020001dc = 0x02009788  in-image data address
 *   0x020001e0 = 0x0000cccc
 *   0x020001e4 = 0x00006666
 *   0x020001e8 = 0x03001ebc  workspace-pointer cell in IWRAM
 *
 * LINK BASE: 0x02009788 is even and lands at file offset 0x1788 under the
 * 0x02008000 base that HANDOVER records as confirmed for this overlay family --
 * past the import band, which ends at 0x1620, so it is in-image data.  Per the
 * parity rule (odd = Thumb entry, even = data) it is a data block, and it is
 * spelled here with the raw pool value as its symbol name, per convention.
 *
 * Calls resolved with `bun tools/overlay_call_targets.ts resource_3a2 0180
 * --json` (eight sites, eight distinct veneer targets; the inventory's
 * `calls=7` is a known lower bound):
 *   0x0184 -> veneer 0x1618 -> Func_080f9010
 *   0x018e -> veneer 0x14a8 -> Func_08009178
 *   0x0194 -> veneer 0x1508 -> Func_0808a080
 *   0x01a4 -> veneer 0x1510 -> Func_0808a090
 *   0x01ba -> veneer 0x1558 -> Func_0808a100
 *   0x01c6 -> veneer 0x1540 -> Func_0808a0e0
 *   0x01cc -> veneer 0x14e8 -> Func_0808a010
 *   0x01d2 -> veneer 0x15e0 -> Func_0808a248
 *
 * Behaviour: a scene entry.  Func_080f9010(188) selects the map/room resource,
 * Func_08009178 installs the block at 0x02009788 at (67, 6), the player record
 * (Func_0808a080(0), the established slot-record accessor) has its byte at +85
 * cleared, and Func_0808a090(0, 0xcccc, 0x6666) sets the same 16.16 pair
 * semantic/overlays/resource_3aa_c_02001494.c writes for its own entities.
 * Then the workspace word at +448 is set to 32, a camera/step pair is issued,
 * 16 frames pass and Func_0808a248(2) closes.
 *
 * TRAP NOTE: `movs r2,#224 / lsls r2,r2,#1` = 448 is used first as the store's
 * DISPLACEMENT and then `subs r2,#192` = 32 as the stored VALUE -- the
 * one-register-two-roles shape HANDOVER warns about.  It is not 448-192+... in
 * either role.
 *
 * Uncertainties: Func_08009178's third argument 6 and second 67 are read as a
 * placement pair by analogy with the six-argument Func_080091c0/Func_08009180
 * family in this overlay; only the three set registers are asserted.  The
 * record byte at +85 is cleared, matching the byte at +89 written by
 * Func_02001144 in the same overlay.  Old-style declarations keep every
 * import's interface open.
 */

extern u8 Data_02009788[];
extern u8 *Data_03001ebc;

void Func_080f9010();
void Func_08009178();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a100();
void Func_0808a0e0();
void Func_0808a010();
void Func_0808a248();

void Func_02000180(void)
{
    u8 *workspace;

    Func_080f9010(188);
    Func_08009178(Data_02009788, 67, 6);

    Func_0808a080(0)[85] = 0;
    Func_0808a090(0, 0xcccc, 0x6666);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 32;

    Func_0808a100(0, 2);
    Func_0808a0e0(0, 0, -16);
    Func_0808a010(16);
    Func_0808a248(2);
}
