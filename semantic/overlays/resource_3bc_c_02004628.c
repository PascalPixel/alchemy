typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3bc owner at 0x02004628, 360 bytes: the "push" mechanic --
 * STRUCTURALLY IDENTICAL to semantic/overlays/resource_3bb_c_02003b90.c
 * (same probe-then-probe-then-probe-then-legality-check shape, same speed
 * constant 0x3333, same sound ids 238/288, same field offsets 34/36/44/48/
 * 52/89), just in this overlay with its own direction table and in-overlay
 * lookup callee. That sibling's header is the reference for the shape;
 * this header records only what differs.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the fp/sl/r9/r8 saves and a
 * 12-byte frame (the three-word position scratch every probe and move
 * uses) at 0x02004628, through the matching interworking return `pop {r0}
 * / bx r0` at 0x200476e-0x2004778 (also unwinding the high registers). No
 * parameters, void.
 *
 * Not found by the structural inventory walk (unindexed); three incoming
 * call sites per overlay_unindexed.ts. Thirteen outgoing calls over seven
 * distinct targets, all resolved with the `+2` rule against the raw image
 * (this owner starts outside a recorded row, so overlay_call_targets.ts's
 * own table does not cover it -- resolved with `bun tools/overlay_show.ts
 * resource_3bc 4628 | bun tools/overlay_call_targets.ts resource_3bc 4628
 * 477e --annotate`):
 *   0x2004642 -> veneer -> Func_0808a080(selector)          scene-record accessor, established
 *   0x2004674, 0x200469e, 0x20046ca -> raw prologue -> Func_20045e0(position)   in-overlay lookup, sibling item-28 owner already drafted in resource_3bc_c_02004790.c as a two-argument probe-rejection call; here (as in resource_3bb's Func_02003b48) all three sites also load r1 with a record pointer that the structurally-identical resource_3bb sibling proves is NOT part of that overlay's own lookup's interface -- inferred by analogy here, not independently proven for this overlay's Func_20045e0
 *   0x200470c -> veneer -> Func_080091d8(occupant, position)   established, signed, tested with `bgt` (resource_3bb_c_02003b90.c, resource_3c4_c_02000cd0.c)
 *   0x200471a, 0x200476a -> veneer -> Func_08009080(subject, mode)   established
 *   0x2004720 -> veneer -> Func_080000c0(15)                established, advance the task scheduler
 *   0x2004730, 0x2004740 -> veneer -> Func_08009150(record, x, y, z)   established, move a record to a position
 *   0x2004746, 0x2004754 -> veneer -> Func_080f9010(id)     established, play a sound cue; ids 238 then 288 (144 << 1), the exact pair resource_3bb_c_02003b90.c documents around the same Func_08009158 call
 *   0x200474c -> veneer -> Func_08009158(occupant)          unestablished, same call as resource_3bc_c_020022f4.c / 02003b80.c
 *
 * Data_0200cfc0 (this overlay's direction table, indexed by the subject's
 * facing nibble at +6) is this overlay's counterpart to resource_3bb's
 * Data_0200c3d4 -- same packed-step shape (high half added to x, low half
 * shifted up 16 and added to z).
 */

extern s32 Data_02000240[];    /* subject selector, byte offset 500 */
extern s32 Data_0200cfc0[];    /* direction table, indexed by facing nibble */

u8 *Func_0808a080();           /* scene-record accessor, established */
u8 *Func_020045e0();           /* in-overlay probe lookup, sibling item-28 owner */
s32 Func_080091d8();           /* established, legality check */
void Func_08009080();          /* established (record, mode) */
void Func_080000c0();          /* established, advance the task scheduler */
void Func_08009150();          /* established (record, x, y, z) */
void Func_080f9010();          /* established, play a sound cue */
void Func_08009158();          /* unestablished, single argument */

void Func_02004628(void)
{
    u8 *subject;
    u8 *occupant;
    u8 *blocker;
    s32 facing;
    s32 step;
    s32 position[3];

    subject = Func_0808a080(*(s32 *)((u8 *)Data_02000240 + 500));
    facing = *(unsigned short *)(subject + 6) >> 12;

    step = Data_0200cfc0[facing];
    position[0] = *(s32 *)(subject + 8) + (step & (s32)0xffff0000);
    position[1] = *(s32 *)(subject + 12);
    position[2] = *(s32 *)(subject + 16) + (step << 16);

    occupant = Func_020045e0(position);
    if (occupant == 0) {
        return;
    }

    step = Data_0200cfc0[facing];
    position[0] = *(s32 *)(occupant + 8) + (step & (s32)0xffff0000);
    position[1] = *(s32 *)(occupant + 12);
    position[2] = *(s32 *)(occupant + 16) + (step << 16);

    blocker = Func_020045e0(position);
    if (blocker != 0 && (blocker[89] & 1) != 0) {
        return;
    }

    position[0] = *(s32 *)(occupant + 8);
    position[1] = *(s32 *)(occupant + 12) + 0x100000;   /* 128 << 13 */
    position[2] = *(s32 *)(occupant + 16);

    blocker = Func_020045e0(position);
    if (blocker != 0 && (blocker[89] & 1) != 0) {
        return;
    }

    occupant[34] = 2;

    step = Data_0200cfc0[facing];
    position[0] = *(s32 *)(occupant + 8) + (step & (s32)0xffff0000);
    position[1] = *(s32 *)(occupant + 12);
    position[2] = *(s32 *)(occupant + 16) + (step << 16);

    if (Func_080091d8(occupant, position) > 0) {
        return;
    }

    Func_08009080(subject, 8);
    Func_080000c0(15);

    *(s32 *)(occupant + 48) = 0x3333;
    *(s32 *)(occupant + 52) = 0x3333;
    Func_08009150(occupant, position[0], position[1], position[2]);

    *(s32 *)(subject + 48) = 0x3333;
    *(s32 *)(subject + 52) = 0x3333;
    Func_08009150(subject, position[0], position[1], position[2]);

    Func_080f9010(238);
    Func_08009158(occupant);
    Func_080f9010(288);

    *(s32 *)(occupant + 8) = position[0];
    *(s32 *)(occupant + 16) = position[2];
    *(s32 *)(occupant + 36) = 0;
    *(s32 *)(occupant + 44) = 0;

    Func_08009080(subject, 1);
}
