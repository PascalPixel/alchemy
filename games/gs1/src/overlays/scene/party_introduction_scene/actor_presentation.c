#include "types.h"

#define SceneDialogue_RunActorFiveTalk Func_020026e0
/*
 * resource_380: the talk handler for actor 5, published through a script
 * record rather than called.
 */
extern void Func_02007080(void);

s32 Func_0200704e(s32 flag);
void Func_02007148(s32 msg);
void Func_02007160(s32 id, s32 arg1);
s32 Func_02007068(s32 flag);
void Func_02007162(s32 msg);
void Func_0200716a(s32 msg);
void Func_0200715c(s32 id, s32 arg1, s32 arg2);
void Func_020070ba(s32 frames);
void Func_02007192(s32 id, s32 arg1);
void Func_020070d6(void);

/*
 * A scripted-scene bracket around a two-level story-flag test. Flag 0x83e
 * selects a single line; otherwise flag 0x83c chooses between two lines and
 * the longer path plays a motion, waits ten frames and closes. The two tests
 * read with opposite polarity. The trailing zero arguments have no
 * established meaning. The 108-byte owner includes its five-word pool.
 */
void SceneDialogue_RunActorFiveTalk(void)
{
    Func_02007080();
    if (Func_0200704e(0x83e)) {
        Func_02007148(0x10c9);
        Func_02007160(5, 0);
    } else {
        if (Func_02007068(0x83c) == 0)
            Func_02007162(0x107a);
        else
            Func_0200716a(0x107c);
        Func_0200715c(5, 0, 0);
        Func_020070ba(10);
        Func_02007192(5, 0);
    }
    Func_020070d6();
}
