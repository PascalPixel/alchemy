typedef int s32;

/*
 * resource_3b9 owner at 0x020002a0, 68 bytes: the first of eleven
 * published talk handlers running contiguously from 0x020002a0 to
 * 0x020006bc.
 *
 * PUBLISHED, NOT CALLED. No `bl` in the image reaches it; its Thumb
 * pointer is written into a script record. Found by
 * `bun tools/overlay_published.ts resource_3b9`.
 *
 * THE BANK. Eleven handlers sit behind the 31-line exact-C row at
 * 0x02000238, which is a small selector returning one of five script
 * data pointers -- far too short to reach any of them. Every one opens
 * the scripted-scene bracket, tests the SAME story flag 0x962, and
 * picks a line accordingly. This is the per-actor callback bank shape,
 * with 0x962 acting as the overlay's "has the scene already played"
 * flag rather than one flag per handler.
 *
 * THE ASYMMETRY IS REAL AND IS THE POINT OF READING EACH ARM. The
 * flag-set arm ends with Func_0808a180(10, 0); the flag-clear arm ends
 * with Func_0808a190(10, 0) -- a DIFFERENT callee, not the same one.
 * The two `bl` encodings differ at every site in this overlay whether
 * or not the callee does, so only the resolved target separates them,
 * and an eye passing over the disassembly reads two identical tails.
 *
 * Shape:
 *     open scene
 *     if (flag 0x962 set) line 0x2251, then Func_0808a180(10, 0)
 *     else                line 0x2057, then Func_0808a190(10, 0)
 *     close scene
 *
 * Complete owner: `push {lr}` at 0x020002a0 through `pop {r0} / bx r0`
 * at 0x020002d2-0x020002d4, a zero alignment halfword at 0x020002d6,
 * then the three-word literal pool 0x020002d8-0x020002e3 (0x962,
 * 0x2251, 0x2057); the next owner's prologue is at 0x020002e4.
 *
 * All `bl` targets resolved through the overlay's import-veneer table
 * under the +2 rule (tools/overlay_call_targets.ts).
 *
 * Uncertainty: what distinguishes Func_0808a180 from Func_0808a190 is
 * not established -- only that they are two different functions. Actor
 * id 10 is transcribed from the `movs r0,#10` at both sites.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a190(s32 id, s32 arg1);
extern s32 Func_080770c0(s32 flagId);

void Func_020002a0(void)
{
    Func_0808a018();
    if (Func_080770c0(0x962)) {
        Func_0808a170(0x2251);
        Func_0808a180(10, 0);
    } else {
        Func_0808a170(0x2057);
        Func_0808a190(10, 0);
    }
    Func_0808a020();
}
