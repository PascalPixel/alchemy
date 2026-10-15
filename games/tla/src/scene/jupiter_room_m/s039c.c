#include "types.h"
#include "field_event.h"
#include "call.h"

struct FieldActor *Object_GetById();
void               Func_02004774();

void Func_0200039c(s32 unused, s32 id)
{
    struct FieldActor *object = Object_GetById(id);

    Call4(Func_02004774, object->x, object->z, 0, -8);
}
