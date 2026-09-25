/* NONMATCHING: 962 bytes, candidate 904, 425 differing halfwords, 187
 * halfword edits (2026-09-25). FieldScene_RunScene3b7SequenceA, meant for
 * FIELD/TOREBI_IZUMI/F_004BC.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: Fresh complete reconstruction: all 97 calls checked against the
 * listing. Corrected item reward call types from actual targets. Each map
 * copy still uses a low-register height instead of the reference
 * high-register copy, leaving 58 bytes missing. WALL: Register allocation
 * and stack-argument setup of the repeated six-argument map copy. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventWait(s32 frames);
void Engine_AudioPlayCue(s32 cue);
void Engine_ObjectMotionArmCallback(s32 actor, s32 angle, s32 frames);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y,
                           s32 width, s32 height);
void Main_0808a1b8(s32 object_id, s32 angle, s32 wait);

s32 Main_0808a060(s32 item, s32 unused);
void Main_0808a398(s32 item, s32 flags);

void Func_020004bc(s32 item)
{
    Engine_EventBegin();
    Engine_EventWait(30);
    Engine_AudioPlayCue(148);
    Engine_EventWait(100);
    Main_0808a1b8(0, 0xc000, 0);
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
    Engine_EventWait(8);

    Engine_EventWait(70);
    Engine_AudioPlayCue(126);
    Main_0808a398(item, 3);
    Main_0808a060(item, 0);
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
