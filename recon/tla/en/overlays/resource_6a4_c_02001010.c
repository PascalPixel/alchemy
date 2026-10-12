/* Near miss: differ 2 of 33 (one reordered row): the native loads the fill
 * byte's address before it builds the mask 3; the draft builds the mask first. */
#include "../../../../games/tla/src/field/overlay_6a4/ov6a4.h"

void Func_02001010(struct ObjectRuntime *object)
{
    u8 *step = &BYTE_AT(object, 99);
    s32 phase = *step & 3;

    gDecodeFillByte = 2;
    switch (phase) {
    case 1:
        Animation_ApplyChildValues(object, 13);
        break;
    case 0:
    case 2:
        Animation_ApplyChildValues(object, 4);
        break;
    case 3:
        Animation_ApplyChildValues(object, 4);
        break;
    }
    (*step)++;
}
