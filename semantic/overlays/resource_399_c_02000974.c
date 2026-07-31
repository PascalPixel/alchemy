typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_399 scripted step at 0x02000974, 200 bytes.
 *
 * Complete owner: `push {r5, lr}` at 0x02000974 and the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000a1e.  The popped register is r0, so
 * it holds the return address and nothing is returned — the owner is `void`.
 * Bytes 0x02000a24-0x02000a3b are the six-word literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_399
 * 0974 --annotate` (an overlay `bl` stores `target - 2`, so the disassembler's
 * own annotations are wrong on every overlay).  18 sites, 9 distinct imports:
 *   Func_0808a080(0)        scene-record accessor; +6 is the heading halfword
 *   Func_080770c0(0x881)    story-flag query, result tested
 *   Func_0808a018/020       scripted-section brackets
 *   Func_0808a148(sel,a,b)  scene request
 *   Func_0808a170(line)     message line
 *   Func_0808a180(sel,0)    step/advance
 *   Func_0808a1b8(sel,h,f)  orientation h over f frames
 *   Func_080b0008(12, sel)  the pair-handoff import this overlay uses twice
 * Per-target multiset against the assembly: 0808a080 x1, 080770c0 x1,
 * 080b0008 x2, 0808a018 x2, 0808a148 x2, 0808a170 x3, 0808a180 x3,
 * 0808a1b8 x2, 0808a020 x2 = 18, which is what the C below spells.
 *
 * THE HEADING TEST.  `ldrsh r5,[record,#6]` then, in each flag arm,
 * `lsls r3,r5,#16 ; adds r3,r3,#0x5fff0000 ; cmp r3,#0x3ffe0000 ; bhi`.  The
 * comparison is UNSIGNED, so this is the usual biased-range test on a 16.16
 * angle: the heading placed in the high halfword, rotated back by 0x5fff and
 * accepted when it lands in the low 0x3ffe0000 window — i.e. the actor is
 * facing within one quadrant of a reference direction.  The identical test is
 * assembled twice, once per arm of the flag; it is hoisted into `facing_near`
 * here, which does not change the call multiset.
 *
 * ASYMMETRY WORTH NOTING, NOT A TRANSCRIPTION ERROR: three of the four arms
 * are wrapped in the Func_0808a018/Func_0808a020 bracket and the last one
 * (flag clear, heading outside the window) is not — it runs from
 * Func_0808a148 straight into the shared epilogue.  The bytes are unambiguous:
 * 0x020009fa is `movs r2,#10`, with no `bl` to the opening bracket, and
 * 0x02000a1a's Func_0808a1b8 is followed directly by the `pop`.
 *
 * `movs r1,#128 ; lsls #7` builds 0x4000 and `movs r1,#160 ; lsls #7` builds
 * 0x5000 — quarter and five-sixteenths of a turn in the 16-bit angle unit.
 */

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Old-style declarations are mandatory in
 * overlay sources: arities vary between sites. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a148();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_080b0008();
u8 *Func_0808a080();
s32 Func_080770c0();

void Func_02000974(void)
{
    s16 heading;
    s32 facing_near;

    heading = *(s16 *)(Func_0808a080(0) + 6);
    facing_near = ((u32)(((s32)heading << 16) + 0x5fff0000) <= 0x3ffe0000);

    if (Func_080770c0(0x881) != 0) {
        if (facing_near) {
            Func_080b0008(12, 15);
        } else {
            Func_0808a018();
            Func_0808a148(15, 0, 0);
            Func_0808a170(0x164f);
            Func_0808a180(15, 0);
            Func_0808a1b8(15, 0x4000, 0);
            Func_0808a020();
        }
    } else {
        if (facing_near) {
            Func_0808a018();
            Func_0808a170(0x1546);
            Func_0808a180(14, 0);
            Func_080b0008(12, 14);
            Func_0808a020();
        } else {
            /* No section bracket on this arm; see the note above. */
            Func_0808a148(14, 0, 10);
            Func_0808a170(0x1547);
            Func_0808a180(14, 0);
            Func_0808a1b8(14, 0x5000, 10);
        }
    }
}
