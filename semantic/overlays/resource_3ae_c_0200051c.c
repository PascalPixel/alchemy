typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3ae, owner at 0x0200051c (364 bytes including its six-word pool at
 * 0x02000670..0x02000686).
 *
 * Role known in advance from the call graph: the in-image handler table holds
 * the odd word 0x0200851d == Func_0200051c + 1 as the payload of the entry
 * keyed 0xffff0008, so this is a table-dispatched interaction handler invoked
 * with no arguments.
 *
 * Prologue `push {r5,r6,r7,lr}` + `mov r7,r8 / push {r7}` + `sub sp,#4`;
 * epilogue `add sp,#4 / pop {r3} / mov r8,r3 / pop {r5,r6,r7} / pop {r0} /
 * bx r0`.  The return address is popped into r0, so r0 does not survive and
 * the owner is void.  The 4-byte frame carries the fifth argument of the
 * Func_080150xx text-window imports.
 *
 * This is a purchase transaction.  r8 is set once at the top to
 * `movs r2,#150 / lsls r2,r2,#2` == 600, the price, and is used three times:
 * as the amount shown in the window, as the affordability comparand, and
 * negated as the amount charged.  r6 holds `Data_02000240`, the cross-overlay
 * workspace, whose WORD at byte offset 16 is the player's coin total (read
 * twice, compared unsigned against the price, and printed).
 *
 * `cmp r8,r3 / bls` is an UNSIGNED `price <= coins` test that branches to the
 * purchase arm, which is what identifies which arm is which: only that arm
 * calls Func_08077230 with the negated price and Func_0808a060(235, 0).
 *
 * `Data_03001ebc` is the RAM progress block established by the byte-exact
 * sibling `assets/code/resource_3ae_c_02000ad4.c`.  Its unsigned halfword at
 * byte offset 472 — the same counter Func_02000260 and Func_02000328 bump —
 * is advanced by 1 when the offer is accepted, by 1 again if the purchase then
 * fails for want of coins, and by 3 when it succeeds.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong).  30 call
 * sites against the inventory's calls=29; every `bl` in the span is
 * transcribed below in order, so the set is complete.  Distinct callees:
 * Func_0808a018, Func_080770c0, Func_0808a170, Func_0808a180, Func_0808a178,
 * Func_0808a070, Func_0808a188, Func_08015120, Func_08015010, Func_08015080,
 * Func_080150b0, Func_08015018, Func_0808a110, Func_0808a010, Func_080f9010,
 * Func_0808a060, Func_080770c8, Func_08077230, Func_0808a020.
 *
 * The two Func_0808a180(8, 0) calls, the three Func_08015018(window, 2) closes
 * and the three Func_0808a010(10) waits are DISTINCT sites on distinct paths
 * and are not merged.
 *
 * Uncertainties: 0x8a5 is read as an event-flag id and 0x1d0b / 0x1d04 as cue
 * ids from their argument positions.  0x0c8a is passed to Func_08015080 in the
 * position a string/format id would occupy.  113 passed to Func_080f9010 and
 * 235 to Func_0808a060 are preserved raw; 235 is the same id Func_02000328
 * looks up through Func_08077040/Func_08077038, which corroborates that it is
 * an item id.  Func_0808a070's result is compared against 1 rather than 0, so
 * the menu returns a selection index, not a boolean.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
s32 Func_080770c0();
void Func_080770c8();
s32 Func_0808a070();
void Func_0808a060();
void Func_0808a110();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_08015120();
s32 Func_08015010();
void Func_08015018();
void Func_08015080();
void Func_080150b0();
void Func_080f9010();
void Func_08077230();

extern u8 *Data_03001ebc;
extern u8 Data_02000240[];
#define COINS (*(u32 *)(Data_02000240 + 16))

void Func_0200051c(void)
{
    const s32 price = 600;      /* movs r2,#150 / lsls r2,r2,#2 */
    s32 window;

    Func_0808a018();

    if (Func_080770c0(0x8a5) != 0) {
        Func_0808a170(0x1d0b);
        Func_0808a180(8, 0);
        return;                 /* branches past Func_0808a020 to the epilogue */
    }

    Func_0808a170(0x1d04);
    Func_0808a178(8, 0);

    if (Func_0808a070(0, 0) == 1) {
        Func_0808a188(8, 0, 10);
        goto close;
    }

    *(u16 *)(Data_03001ebc + 472) += 1;

    Func_08015120(price, 5);
    Func_0808a178(8, 0);

    window = Func_08015010(19, 8, 11, 4, 2);
    Func_08015080(0xc8a, window, 0, 0);
    Func_080150b0(COINS, 6, window, 24, 8);

    if (Func_0808a070(-1, 0) == 1) {
        /* Cancelled at the confirmation prompt. */
        Func_08015018(window, 2);
        Func_0808a110(0, 4);
        Func_0808a010(10);
    } else if ((u32)price > COINS) {
        /* Not enough coins.  The comparison is unsigned in the original. */
        Func_08015018(window, 2);
        Func_0808a110(0, 3);
        Func_0808a010(10);
        *(u16 *)(Data_03001ebc + 472) += 1;
        Func_080f9010(113);
    } else {
        Func_08015018(window, 2);
        Func_0808a110(0, 3);
        Func_0808a010(10);
        *(u16 *)(Data_03001ebc + 472) += 3;
        Func_0808a180(8, 0);
        Func_0808a060(235, 0);
        Func_080770c8(0x8a5);
        Func_08077230(-price);
        goto close;
    }

    Func_0808a180(8, 0);

close:
    Func_0808a020();
}
