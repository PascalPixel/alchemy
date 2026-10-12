/* Near miss: differ 2 of 48 (one reordered row): the native copies the anchor
 * to r1 before it adds 6 to the radius; the draft copies it after the
 * multiply. The twin of ☀️'s SceneEffect_UpdateCounterDrivenOrbit. */
#include "../../../../games/tla/src/field/overlay_6ae/ov6ae.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_020000e4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *anchor = Object_GetById(8);
    u16 *pangle = (u16 *)&object->action;
    s32 angle = *pangle;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Math_Cosine(angle);
    along = anchor->x + cosine * (object->speed_limit + object->unknown_5c[6] + 6);
    object->x = along;
    sine = Math_Sine(angle);
    across = anchor->z + sine * (object->unknown_5c[6] + 4);
    object->z = across;
    object->target_x = object->x;
    object->target_z = across;
    *pangle -= 0x800;
}
