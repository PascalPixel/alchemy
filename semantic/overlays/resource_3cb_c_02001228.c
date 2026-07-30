typedef signed int s32;

/*
 * Resource 3cb sound-cue step at 0x02001228 (108 bytes, 0x02001228-0x02001293).
 *
 * Complete owner: its own `push {r5, lr}` prologue at 0x02001228 and the
 * matching interworking return `pop {r5} / pop {r1} / bx r1` at 0x02001286.
 * Because the popped return address goes to r1 and not r0, r0 survives the
 * epilogue and IS the owner's result — here the result of the final
 * Func_08015018 call.  The eight bytes at 0x0200128c-0x02001293 are this
 * owner's literal pool (the two cue ids 0x292c and 0x292d, loaded by the two
 * `ldr r0, [pc, ...]` sites); they are data, not the `cmp r1, #44` /
 * `cmp r1, #45` pairs the disassembler prints.
 *
 * Import identities were resolved with `tools/overlay_call_targets.ts`
 * (true_target_offset = stored_displacement + 2); all eleven call sites are
 * import veneers and collapse to six distinct main-image imports:
 *   0x020018e4 -> Func_080f9010  x1
 *   0x020017c4 -> Func_08015038  x2
 *   0x02001714 -> Func_080000c0  x3
 *   0x020017cc -> Func_08015048  x2
 *   0x020017f4 -> Func_08015308  x1
 *   0x020017b4 -> Func_08015018  x2
 * That is 11 placed sites against the inventory's calls=10, which is expected:
 * the inventory field is a lower bound and counts distinct targets.
 *
 * Behaviour: the owner plays two consecutive cues.  For each it starts the cue
 * (Func_08015038), then idles one unit of time (Func_080000c0(1)) until
 * Func_08015048 reports the cue has finished, then releases the handle with
 * Func_08015018(handle, 1).  Between the two cues it issues Func_08015308 once
 * and an extra Func_080000c0(1).
 *
 * Uncertainties: the trailing arguments (5, 4, 1) to Func_08015038 and the
 * second argument 1 to Func_08015018 are literal at every site in this overlay
 * and their meaning is not established here; Func_08015308 takes no set
 * argument register.  Whether the surviving r0 is genuinely consumed by this
 * owner's callers is not observable from the owner itself — the epilogue shape
 * is the evidence for typing it s32.
 *
 * Near-identical sibling: Func_020011bc is byte-identical to this owner apart
 * from its two pool words (it uses 0x292a/0x292b).  Every BL
 * halfword pair is bit-identical between the two, and
 * overlay_call_targets.ts reports the same six callees over the same eleven
 * sites for both, which is the correctness proof for this transcription.
 */

/* Imports. Old-style declarations: two Func_ names in one overlay can be the
 * same import and arities vary per site, so interfaces are left open. */
void Func_080f9010();
void Func_080000c0();
void Func_08015308();

/* Used for their return values, so these need a return type. */
s32 Func_08015038();
s32 Func_08015048();
s32 Func_08015018();

s32 Func_02001228(void)
{
    s32 handle;

    Func_080f9010(85);

    handle = Func_08015038(0x292c, 5, 4, 1);
    while (Func_08015048() == 0) {
        Func_080000c0(1);
    }
    Func_08015308();
    Func_08015018(handle, 1);

    Func_080000c0(1);

    handle = Func_08015038(0x292d, 5, 4, 1);
    while (Func_08015048() == 0) {
        Func_080000c0(1);
    }
    return Func_08015018(handle, 1);
}
