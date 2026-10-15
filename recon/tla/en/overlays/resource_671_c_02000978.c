/* Draft, differ 70 of 76, a first form written from the listing (push.c's Func_02000c38 is the
 * nearest relative). Left: the native keeps dx in r1 and builds its constants in r4, and reloads
 * actor->z after the first clamp, where CSE here carries the stored value into the second test. */
#include "types.h"
#include "object_runtime.h"

extern s32 Data_02005af0;
struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_0200584c(void);
struct ObjectRuntime *Func_020005b4(struct ObjectRuntime *origin, s32 id);
void Func_020006ac(struct ObjectRuntime *object, s32 dx, s32 dz, s32 velocity);

void Func_02000978(void)
{
    s32 *speed = &Data_02005af0;

    if (*speed != 0) {
        struct ObjectRuntime *actor = Object_GetById(Func_0200584c());
        struct ObjectRuntime *found = Func_020005b4(actor, 359);
        s32 dx = *speed << 15;

        if (found != NULL) {
            s32 diff;

            if (found->angle == 0x8000)
                dx = -dx;
            if (found->z - actor->z < -0x20000)
                actor->z = found->z + 0x20000;
            if (found->z - actor->z > 0x20000)
                actor->z = found->z - 0x20000;
            actor->x += dx;
            diff = found->x - actor->x;
            if (diff >= 0 ? diff >= 0x140000 : actor->x - found->x >= 0x140000) {
                s32 sign;

                diff = actor->x - found->x;
                sign = 0;
                if (diff != 0) {
                    sign = 1;
                    if (diff <= 0)
                        sign = -1;
                }
                Func_020006ac(actor, sign * 0xc0000, 0, 0x20000);
            }
        }
    }
}
