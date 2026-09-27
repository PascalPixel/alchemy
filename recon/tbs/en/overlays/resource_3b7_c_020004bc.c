/* NONMATCHING: 964 bytes including alignment, candidate 904, 430 differing
 * halfwords, 183 aligned edits (2026-09-27). Unit: torebi-prize-reveal.
 * Corrected the 97-call sequence: wait 8 follows frame (100,20), not the
 * final (100,29) frame. The caller loads item from a word table without
 * narrowing, so keep its s32 argument and the registered service types.
 * Remaining: reference item/height occupy r5/r8; ours occupy r8/r5. The
 * local allocator sees item at 3 refs/186 instructions, width at 30/638,
 * height at 30/636, all in one block. Each direct low-register height store
 * loses the reference's high-register copy. Reusing exact scenes' Call6
 * boundary only swaps width/height low registers: 904 bytes, 432 halfwords,
 * 213 edits; rejected. No parameter-width or declaration sweep is justified
 * by the caller or allocator evidence. */
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

    Engine_MapCopyCellsTo(82, 20, 70, 0, 3, 8);
    Engine_EventWait(3);
    Engine_MapCopyCellsTo(85, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(88, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(91, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(94, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(97, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(100, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);

    Engine_MapCopyCellsTo(79, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(82, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(85, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(88, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(91, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(94, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(97, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(100, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);

    Engine_EventWait(70);
    Engine_AudioPlayCue(126);
    Engine_ItemShowFound(item, 3);
    Engine_PartyGiveItem(item, 0);
    Engine_EventWait(20);

    Engine_MapCopyCellsTo(97, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(94, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(91, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(88, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(85, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(82, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(100, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(97, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(94, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(91, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(88, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(85, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(82, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_MapCopyCellsTo(79, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_EventEnd();
}
