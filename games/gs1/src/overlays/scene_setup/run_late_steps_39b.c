#define FieldScene_RunActor8StepWithTableA820 Func_02001150

#define FieldScene_RunIndexedStep63 Func_02001144
/* Contiguous unnamed leaf-owner run for resource_39b. */

extern void Func_020036ee();
void FieldScene_RunIndexedStep63(void)
{
    Func_020036ee(63);
}

/* Clear the scene flag and point actor 8 at its first local path. */
extern void Func_02003622();
extern void Func_0200365a();
void FieldScene_RunActor8StepWithTableA820(void)
{
    Func_02003622(0x205);
    Func_0200365a(8, (void *)0x0200a820);
}
