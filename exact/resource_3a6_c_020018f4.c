#include "types.h"







/* Mirror the preceding transition on the negative grid axis, latch its event
 * flag, wait for the map update, and close on the paired sound cue. */
extern void Func_02003656(s32 soundId);
extern void Func_020030de(s32 actorId, s32 cellX, s32 cellZ);
extern void Func_020030ea(s32 actorId, s32 cellX, s32 cellZ);
extern void Func_020035b4(s32 flagId);
extern void Func_0200353a(s32 frames);
extern void Func_02003092(void);
extern void Func_02003684(s32 soundId);
void Func_020018f4(void)
{
    Func_02003656(241);
    Func_020030de(11, -112, 0);
    Func_020030ea(11, -80, 0);
    Func_020035b4(0x301);
    Func_0200353a(2);
    Func_02003092();
    Func_02003684(0x121);
}
