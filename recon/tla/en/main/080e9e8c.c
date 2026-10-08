/* Near miss: listing 43 instructions, draft 43, common 34, differ 18; the rest differs in instruction order. */
#include "types.h"
#include "object_runtime.h"

void Func_080e9e8c(struct ObjectRuntime *object)
{
    u8 *anim = object->animation;
    u16 *counter = (u16 *)((u8 *)object + 100);
    u8 *sprite = *(u8 **)(anim + 40);

    switch (*counter & 3) {
    case 0:
        sprite[5] = 7;
        anim[25] = 1;
        anim[26] = 2;
        break;
    case 1:
        anim[25] = 1;
        anim[26] = 0;
        break;
    case 2:
        sprite[5] = 7;
        anim[26] = 0;
        anim[25] = 1;
        break;
    case 3:
        sprite[5] = 0;
        anim[25] = 1;
        break;
    }
    (*counter)++;
}
