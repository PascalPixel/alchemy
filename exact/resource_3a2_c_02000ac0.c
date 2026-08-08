#include "types.h"

/*
 * resource_3a2 owner at 0x02000ac0, 108 bytes.  Complete owner: `push {lr}`
 * prologue at 0x02000ac0 and the interworking return `pop {r0} / bx r0` at
 * 0x02000b1c.  r0 holds the popped return address, so the owner returns
 * nothing; it takes no arguments.
 *
 * A three-word literal pool sits at 0x02000b20-0x02000b2b, after the epilogue
 * and inside the row's 108-byte span; the control-flow walk from the prologue
 * never reaches it, so it is data:
 *   0x02000b20 = 0x000018b9  text/cue id
 *   0x02000b24 = 0x00000105  actor/portrait id
 *   0x02000b28 = 0x03001ebc  workspace-pointer cell in IWRAM
 *
 * Calls resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a2 0ac0
 * --json` (ten sites, nine distinct veneer targets -- Func_0808a010 is reached
 * twice; the inventory's `calls=10` counts sites here):
 *   0x0ac2 -> veneer 0x14f0 -> Func_0808a018
 *   0x0ac8 -> veneer 0x1580 -> Func_0808a170
 *   0x0ad2 -> veneer 0x15b8 -> Func_0808a1e8
 *   0x0ada -> veneer 0x1588 -> Func_0808a178
 *   0x0ae2 -> veneer 0x1500 -> Func_0808a070
 *   0x0afc -> veneer 0x14e8 -> Func_0808a010
 *   0x0b04 -> veneer 0x1560 -> Func_0808a110
 *   0x0b0a -> veneer 0x14e8 -> Func_0808a010
 *   0x0b14 -> veneer 0x1598 -> Func_0808a188
 *   0x0b18 -> veneer 0x14f8 -> Func_0808a020
 *
 * Behaviour: another yes/no prompt beat in the same family as Func_020000e8.
 * Func_0808a018/Func_0808a020 bracket it; cue 0x18b9 is posted; actor 10 plays
 * its part; Func_0808a070(0, 0) returns the answer.  The tail after the
 * conditional is unconditional -- the `bne` skips only the counter bump.
 *
 * The counter is the same skip-beat counter Func_020000e8 uses: the u16 at
 * workspace + 472, with the displacement built as `movs r3,#236 /
 * lsls r3,r3,#1`.  Note the polarity is the OPPOSITE of Func_020000e8's: here
 * answer 1 bumps it, there answer 1 is the branch that does not.  Both were
 * read from the `cmp r0,#1 / bne` directly.
 *
 * Uncertainties: no register is set before Func_0808a018, so it takes no
 * asserted arguments.  Func_0808a1e8's (10, 0x105, 60) triple is read as
 * actor, id and a 60-frame duration but has no second converted witness in the
 * tree.  Old-style declarations keep every import's interface open.
 */

extern u8 *Data_03001ebc;











extern void Func_02001fb4();
extern void Func_0200204a();
extern void Func_0200208c();
extern void Func_02002064();
extern s32 Func_02001fe4();
extern void Func_02001fe6();
extern void Func_02002066();
extern void Func_02001ff4();
extern void Func_020020ae();
extern void Func_02002012();
void Func_02000ac0(void)
{
    u8 *workspace;

    Func_02001fb4();
    Func_0200204a(0x18b9);
    Func_0200208c(10, 0x105, 60);
    Func_02002064(10, 0);

    if (Func_02001fe4(0, 0) == 1) {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
    }

    Func_02001fe6(20);
    Func_02002066(10, 4);
    Func_02001ff4(20);
    Func_020020ae(10, 0, 20);
    Func_02002012();
}
