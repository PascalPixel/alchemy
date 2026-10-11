/* Draft of overlay 69a Func_02000e80, the twin of overlay 699's
 * Func_02000fd8 (lightcue.c). differ 6 of 19: the listing loads the count's
 * and the list's addresses after the timer store; this draft loads the
 * count's address before it. The 699 twin holds them back with an asm
 * barrier; a post-increment index (differ 17) and a count pointer (differ 6)
 * did not. */
#include "types.h"
#include "field_event.h"

extern s16 Data_0200229a;
extern struct FieldActor *Data_0200229c[];

struct FieldActor *Object_GetById(s32 id);

void Func_02000e80(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        object->timer = value;
        Data_0200229c[Data_0200229a] = object;
        Data_0200229a++;
    }
}
