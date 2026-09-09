#include "types.h"

/*
 * Scene state interaction for resource_39c.  Data_03001edc is the
 * current-object cell: a pointer read once, then dereferenced to reach the
 * live record.
 */

extern s32 **Data_03001edc;

extern void Func_0200b022(s32 arg0);
extern void Func_0200af6e_a(s32 *object, s32 arg1);
extern void Func_0200af6e_b(s32 *object, s32 arg1);

/*
 * Clear the current record's flag word and, if it has a linked object, reset
 * that object's halfword at +0x64, notify twice and drop the link.  The
 * 72-byte owner includes its three pool words.  Func_0200af6e_a and
 * Func_0200af6e_b are one import called twice with very different second
 * arguments; its parameter meaning is unverified, so each call is left as
 * compiled rather than unified.
 */
void SceneState_ClearCurrentRecordAndReleaseTarget(void)
{
    s32 *rec = *Data_03001edc;
    s32 *target;

    if (rec[0] == 0) {
        return;
    }

    rec[0] = 0;
    Func_0200b022(0x161);

    target = (s32 *)rec[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Func_0200af6e_a(target, (s32)0x0200de2c);
        Func_0200af6e_b(target, 7);
        rec[5] = 0;
    }
}
