#include "types.h"

#define FieldScene_RunIndexedStep17 Func_02000fac
#define FieldScene_RunIndexedStep18 Func_02000fb8
#define FieldScene_RunIndexedStep19 Func_02000fc4
#define SceneData_GetTablead60 Func_02000fd0
#define FieldScene_RunIndexedStep63 Func_02001144
#define FieldScene_RunActor8StepWithTableA820 Func_02001150
void Func_02001e82(int page);
void Func_02001e8e(int page);
void Func_02001e9a(int page);
void Func_020036ee();
void Func_02003622();
void Func_0200365a();

/* Contiguous unnamed leaf-owner run for resource_39b. */

/* Clear the scene flag and point actor 8 at its first local path. */

void FieldScene_RunIndexedStep17(void)
{
    Func_02001e82(17);
}

void FieldScene_RunIndexedStep18(void)
{
    Func_02001e8e(18);
}

void FieldScene_RunIndexedStep19(void)
{
    Func_02001e9a(19);
}

void *SceneData_GetTablead60(void) { return (void *)0x0200ad60; }

void FieldScene_RunIndexedStep63(void)
{
    Func_020036ee(63);
}

void FieldScene_RunActor8StepWithTableA820(void)
{
    Func_02003622(0x205);
    Func_0200365a(8, (void *)0x0200a820);
}
