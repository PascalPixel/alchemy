/* Draft, differ 37 of 116, written from the listing, as a goto loop (a do-while lets loop.c hoist
 * the kind 0x149 into r11: differ 78). Left: the random test builds 0x8000 and branches on bcs where
 * the native builds 0x7fff and branches on bhi (tried <= 0x7fff, > 0x7fff, < 0x8000, a signed cast),
 * and the origin, speed and scale take r9, r8 and r10 where the native has r8, r10 and r9. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"
#include "iwram_call.h"

extern const u32 Data_02005b68[];
extern const u32 Data_02005bac[];
s32 Math_Sine(s32 angle);

void Func_0200055c(s32 id)
{
    struct ObjectRuntime *origin = Object_GetById(id);
    s32 i = 11;

loop:
    {
        s32 a = Random16Far();
        s32 x = origin->x + (a << 4) - (Random16Far() << 4);
        s32 b = Random16Far();
        s32 c = Random16Far();
        struct ObjectRuntime *object = Func_02005498(0x149, x, (b << 3) + origin->y, (c << 4) + origin->z);

        if (object != NULL) {
            s32 angle = (Random16Far() >> 2) + 0xe000;
            s32 speed = Random16Far() << 3;
            s32 scale = (Random16Far() >> 1) + 0x4000;

            if (Random16Far() <= 0x7fff)
                Func_02005490(object, Data_02005b68);
            else
                Func_02005490(object, Data_02005bac);
            ObjectDispatch_SetSingleChildFlags(object, 0);
            object->velocity_y = (angle << 2) + 0x20000;
            object->velocity_z = Iwram_MulQ16(speed, Math_Cosine(angle));
            {
                s32 vx = Iwram_MulQ16(speed, Math_Sine(angle));

                object->acceleration = 0;
                object->scale_x = scale;
                object->scale_y = scale;
                object->gravity = 0x547a;
                object->velocity_x = vx;
                object->bounce = 0x8000;
            }
            Animation_ApplyChildValues(object, 1);
        }
    }
    if (--i >= 0)
        goto loop;
}
