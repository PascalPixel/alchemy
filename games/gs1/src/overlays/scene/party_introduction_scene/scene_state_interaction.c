#include "types.h"

#define FieldScene_RunActorNineDialogueByFlags Func_02002674
/*
 * resource_380: the talk handler for actor 9, one of a bank of per-actor
 * published callbacks. The handler is published through a script record
 * rather than called, so nothing here reaches it by name.
 */

extern void Func_02007014(void);
s32 Func_02006fe2(s32 flag);
void Func_020070dc(s32 msg);
void Func_020070f4(s32 id, s32 arg1);
s32 Func_02006ffc(s32 flag);
void Func_020070f6(s32 msg);
void Func_020070fe(s32 msg);
void Func_020070f0(s32 id, s32 arg1, s32 arg2);
void Func_0200704e(s32 frames);
void Func_02007126(s32 id, s32 arg1);
void Func_0200706a(void);

/*
 * A scripted-scene bracket around a two-level story-flag test. When flag
 * 0x83e is set the actor speaks one line and the handler ends; otherwise flag
 * 0x83c chooses between two alternative lines and the longer path also plays
 * a motion, waits ten frames and closes. The two tests read with opposite
 * polarity. The trailing zero arguments have no established meaning. The
 * 108-byte owner includes its five-word literal pool.
 */
void FieldScene_RunActorNineDialogueByFlags(void)
{
    Func_02007014();
    if (Func_02006fe2(0x83e)) {
        Func_020070dc(0x10cb);
        Func_020070f4(9, 0);
    } else {
        if (Func_02006ffc(0x83c) == 0)
            Func_020070f6(0x1079);
        else
            Func_020070fe(0x107b);
        Func_020070f0(9, 0, 0);
        Func_0200704e(10);
        Func_02007126(9, 0);
    }
    Func_0200706a();
}
