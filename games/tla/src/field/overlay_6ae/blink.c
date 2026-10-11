#include "ov6ae.h"

void Func_02001eb0(struct ObjectRuntime *object)
{
    if ((Data_02006b64 >> Data_02006b6c) & 1)
        Object_SetPartAttribute(object, 15);
    else
        Object_SetPartAttribute(object, 0);
    Data_02006b64++;
}

void Func_02001ee4(struct ObjectRuntime *object)
{
    if ((Data_02006b64 >> Data_02006b6c) & 1)
        Object_SetPartAttribute(object, 0);
    else
        Object_SetPartAttribute(object, 15);
}
