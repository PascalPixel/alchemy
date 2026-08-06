#include "types.h"

/*
 * resource_3bd owner at 0x02000de8 (196 bytes).  It prepares the active
 * actor for a one-step move/effect transition.  The actor pointer comes from
 * the word at Data_02000240 + 500 (the same selector word used by the
 * resource_3bb movement sibling), and its original position is copied to a
 * three-word stack probe.  Func_08000128 advances that probe by 0x200000 at
 * the actor's quadrant heading; Func_080091d8 rejects the transition when the
 * resulting position is blocked.
 *
 * On success the body is the established movement/effect sequence: modes 6
 * and 7 bracket a six-frame wait and sound 0x98, the three speed words are
 * written at +0x30/+0x34/+0x28, the +0x55 byte is masked, the actor is seated,
 * and its coarse x/z cells are sent to Func_0808a0c0.  The byte is restored
 * from the saved value before the scene bracket closes.  The owner is void;
 * all 13 BL sites resolve to the ordinary imports listed below.
 */

extern u8 Data_02000240[];












extern u8 * Func_02004ac4(s32 id);
extern void Func_02004a46(s32 distance, s32 heading, s32 *position);
extern s32 Func_02004a86(u8 *record, s32 *position);
extern void Func_02004ae6(void);
extern void Func_02004a66(u8 *record, s32 mode);
extern void Func_02004a44(s32 frames);
extern void Func_02004cba(s32 sound_id);
extern void Func_02004a7a(u8 *record, s32 mode);
extern void Func_02004ad4(u8 *record, s32 mode);
extern void Func_02004b7a(s32 id, s32 x, s32 z);
extern void Func_02004ab2(u8 *record, s32 mode);
extern void Func_02004af2(u8 *record, s32 mode);
extern void Func_02004b52(void);
void Func_02000de8(void)
{
    u8 *record = Func_02004ac4(*(s32 *)(Data_02000240 + 500));
    u8 savedFlags = record[0x55];
    s32 position[3];

    position[2] = *(s32 *)(record + 16);
    position[0] = *(s32 *)(record + 8);
    Func_02004a46(0x200000, *(u16 *)(record + 6) & 0xf000, position);
    position[1] = *(s32 *)(record + 12);

    if (Func_02004a86(record, position) != 0)
        return;

    Func_02004ae6();
    Func_02004a66(record, 6);
    Func_02004a44(6);
    Func_02004cba(0x98);
    Func_02004a7a(record, 7);

    *(s32 *)(record + 0x28) = 0x40000;
    *(s32 *)(record + 0x30) = 0x30000;
    *(s32 *)(record + 0x34) = 0x20000;
    record[0x55] &= 0x7e;

    Func_02004ad4(record, 0);
    Func_02004b7a(
        0,
        (s16)(position[0] >> 16),
        (s16)(position[2] >> 16));
    Func_02004ab2(record, 6);
    Func_02004af2(record, 1);
    record[0x55] = savedFlags;
    Func_02004b52();
}
