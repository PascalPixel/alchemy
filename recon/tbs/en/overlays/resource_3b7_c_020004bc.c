/* NONMATCHING: 964 bytes including alignment, candidate 904, 432 differing
 * halfwords, 213 aligned edits (2026-09-27). Unit: torebi-prize-reveal.
 * Corrected the 97-call sequence: wait 8 follows frame (100,20), not the
 * final (100,29) frame. The caller loads item from a word table without
 * narrowing, so keep its s32 argument and the registered service types.
 * Remaining: reference item/height occupy r5/r8; ours occupy r8/r5. The
 * local allocator sees item at 3 refs/186 instructions, width at 30/638,
 * height at 30/636, all in one block. Each direct low-register height store
 * loses the reference's high-register copy. Reusing exact scenes' Call6
 * boundary only swaps width/height low registers: 904 bytes, 432 halfwords,
 * 213 edits; rejected. No parameter-width or declaration sweep is justified
 * by the caller or allocator evidence.
 *
 * 2026-09-27 fixed-callee transfer, one model only:
 * Exact 757956aba VinasuHeya_ShiftBridge and EAST_PARTICLE_WAVE.C use
 * FIELD_EVENT.H:Map_CopyCellsTo to give dimensions a named inline boundary.
 * Before testing, RTL showed its six SI formals, width/height at incoming
 * stack offsets 0/+4 and a fixed Engine_MapCopyCellsTo call, unlike generic
 * Call6's extra function-pointer formal. Prediction: call-owned dimensions
 * would admit item r5 / height r8 without changing the 97 calls or frame8.
 * Changed only the 29 map calls to the existing named helper. Result is
 * 904/964, 432 halfwords / 213 aligned edits, byte-identical by cmp to the
 * preserved generic Call6 candidate. Full normalized diff and entire owner
 * were read, including native tail alignment; no literal pools are present.
 * In lreg width/height become USER SI pseudos 38/39, each 30 uses across
 * 638 instructions and 87 calls, allocated r5/r6. Item 32 retains 3 uses /
 * 186 instructions / 52 calls and r8. Thus the named helper changes formal
 * ancestry but not the rejected allocation or missing high-register copies.
 * Immediate rejection as briefed: no follow-up, parameter-width, declaration
 * or setup sweep. Preserve this trial in Git, then restore canonical direct
 * Engine_MapCopyCellsTo calls (904/430/183). No function or alignment credit;
 * no shared header, binding, compiler, tooling or other owner was changed. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void TorebiIzumi_RevealPrize(s32 item)
{
    Engine_EventBegin();
    Engine_EventWait(30);
    Engine_AudioPlayCue(148);
    Engine_EventWait(100);
    Engine_ObjectMotionArmCallback(0, 0xc000, 0);
    Engine_EventWait(40);

    Map_CopyCellsTo(82, 20, 70, 0, 3, 8);
    Engine_EventWait(3);
    Map_CopyCellsTo(85, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(88, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(91, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(94, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(97, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(100, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);

    Map_CopyCellsTo(79, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(82, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(85, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(88, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(91, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(94, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(97, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(100, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);

    Engine_EventWait(70);
    Engine_AudioPlayCue(126);
    Engine_ItemShowFound(item, 3);
    Engine_PartyGiveItem(item, 0);
    Engine_EventWait(20);

    Map_CopyCellsTo(97, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(94, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(91, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(88, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(85, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(82, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(100, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(97, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(94, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(91, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(88, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(85, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(82, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(79, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_EventEnd();
}
