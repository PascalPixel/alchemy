#include "types.h"

/*
 * Staged actor effect sequence for resource_39b.  Data_03001edc is the
 * current-object cell: a pointer read once, then dereferenced to reach the
 * live record.
 */

extern s32 **Data_03001edc;

extern void Func_020044ca(s32 arg0);
extern void Func_02004456(s32 *object, s32 arg1);
extern void Func_02004456_a(s32 *object, s32 arg1);

/*
 * Clear the current record's flag word and, if it has a linked object, reset
 * that object's halfword at +0x64, notify twice and drop the link.  The
 * 72-byte owner includes its three pool words.  Func_02004456 and
 * Func_02004456_a are one import called twice with very different second
 * arguments; its parameter meaning is unverified, so each call is left as
 * compiled rather than unified.
 */
void Func_02001fe8(void)
{
    s32 *record = *Data_03001edc;
    s32 *target;

    if (record[0] == 0) {
        return;
    }

    record[0] = 0;
    Func_020044ca(0x161);

    target = (s32 *)record[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Func_02004456(target, (s32)0x0200a7dc);
        Func_02004456_a(target, 7);
        record[5] = 0;
    }
}
