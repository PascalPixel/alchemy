typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3a2 owner at 0x020000e8, 88 bytes.  Complete owner: `push {lr}`
 * prologue at 0x020000e8 and the interworking return `pop {r0} / bx r0` at
 * 0x02000134.  r0 holds the popped return address, so the owner returns
 * nothing; it takes no arguments.
 *
 * A two-word literal pool sits at 0x02000138-0x0200013f, after the epilogue and
 * inside the row's 88-byte span.  The control-flow walk from the prologue never
 * reaches it, so it is data, not the `adds`/`subs` the disassembler prints:
 *   0x02000138 = 0x00001958  (text/cue id)
 *   0x0200013c = 0x03001ebc  (the IWRAM cell holding this overlay family's
 *                             workspace pointer, the same one
 *                             assets/code/resource_371_c_020004a0.c loads)
 *
 * Calls resolved with `bun tools/overlay_call_targets.ts resource_3a2 00e8
 * --json` (eight sites, eight distinct veneer targets):
 *   0x00ea -> veneer 0x14f0 -> Func_0808a018
 *   0x00f0 -> veneer 0x1580 -> Func_0808a170
 *   0x00f8 -> veneer 0x1588 -> Func_0808a178
 *   0x0100 -> veneer 0x1500 -> Func_0808a070
 *   0x010a -> veneer 0x14e8 -> Func_0808a010
 *   0x0112 -> veneer 0x1590 -> Func_0808a180
 *   0x012c -> veneer 0x15a0 -> Func_0808a190
 *   0x0130 -> veneer 0x14f8 -> Func_0808a020
 *
 * Behaviour: a yes/no prompt beat.  Func_0808a018/Func_0808a020 bracket it;
 * Func_0808a170 posts cue 0x1958; Func_0808a178(10, 0) runs actor 10's part of
 * the prompt; Func_0808a070(0, 0) returns the player's answer.  On answer 1 the
 * beat waits 20 frames and continues through Func_0808a180(10, 0); otherwise it
 * bumps the u16 at workspace + 472 and continues through Func_0808a190(10, 0).
 *
 * That counter bump is the established skip-beat counter for this overlay
 * family -- HANDOVER records the same `u16` at `workspace + 472` on the absent
 * side of scene-variant tests in resource_391.  The displacement is built as
 * `movs r3,#236 / lsls r3,r3,#1` = 472.
 *
 * Uncertainties: no register is set before Func_0808a018, so it is spelled with
 * no arguments.  Func_0808a070's result is compared against the immediate 1
 * only, so no wider return domain is asserted.  Old-style declarations keep
 * every import's interface open, as required for overlay sources.
 */

extern u8 *Data_03001ebc;

void Func_0808a018();
void Func_0808a170();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a010();
void Func_0808a180();
void Func_0808a190();
void Func_0808a020();

void Func_020000e8(void)
{
    u8 *workspace;

    Func_0808a018();
    Func_0808a170(0x1958);
    Func_0808a178(10, 0);

    if (Func_0808a070(0, 0) == 1) {
        Func_0808a010(20);
        Func_0808a180(10, 0);
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
        Func_0808a190(10, 0);
    }

    Func_0808a020();
}
