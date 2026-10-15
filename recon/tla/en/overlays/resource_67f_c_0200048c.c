/* Near miss, differ 19 of 91: the listing rebuilds 0xffff at the loop's bottom test and
 * skips a non-matching entry with a branch to it; this draft keeps 0xffff in ip across the loop. */
#include "../../../../games/tla/src/scene/izumo_ruins/izumo_ruins.h"

void Func_0200048c(u16 *list, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 flag;

    while (list[0] != 0xffff) {
        if (list[0] == id) {
            flag = list[1];
            Func_02001488();
            if (!GameFlag_Test(flag + 1)) {
                Func_02001538(185);
                Call3((void (*)())Func_02001470, 0x10000, 0x40000, 0x10000);
                Func_02000974(0);
                WaitFrames(8);
                Func_02001428(object, 2);
                Func_02000bc0(-4, object->terrain_id, id);
                Func_02000974(1);
                WaitFrames(16);
                Call3((void (*)())Func_02001470, -1, -1, 0xe666);
                WaitFrames(8);
                Object_SetPositionAndResetMotion(object, 0, 0, 0);
                WaitFrames(2);
                Func_02001538(188);
                Func_02000aa8();
                WaitFrames(20);
                GameFlag_SetBit(flag);
                GameFlag_SetBit(flag + 1);
            }
            Func_02001490();
            return;
        }
        list += 2;
    }
}
