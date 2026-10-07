#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "MAP.H"
#include "FRAME.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"

void Func_02003bac(s32 a, s32 b);
void Func_02003bb4(s32 owner, u16 value);
void Field_DispatchTypeHandler(s32 type);
void Func_02003bbc(void);
void Func_02003bc4(void);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);

void Func_02000fcc(struct ObjectRuntime *object)
{
    if (gFrameCount & object->unknown_5c[7])
        Object_SetPartAttribute(object, 10);
    else
        Object_SetPartAttribute(object, 0);
}
