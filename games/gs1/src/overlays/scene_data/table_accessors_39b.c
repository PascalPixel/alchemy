#define FieldScene_RunStepWithValue1632 Func_02000eac

#define SceneData_GetTableac58 Func_02000ea4

#define SceneData_GetTableabf8 Func_02000e9c

#define SceneData_ReturnZero Func_02000e98

#define SceneData_GetTablea928 Func_02000e90
/* Contiguous unnamed leaf-owner run for resource_39b. */

void *SceneData_GetTablea928(void) { return (void *)0x0200a928; }

int SceneData_ReturnZero(void) { return 0; }

void *SceneData_GetTableabf8(void) { return (void *)0x0200abf8; }

void *SceneData_GetTableac58(void) { return (void *)0x0200ac58; }

extern void Func_0200338c();
extern void Func_020033ec();
extern void Func_0200336c();
extern void Func_020033a8();

/* Apply the overlay's common actor-0 presentation preset. */
void FieldScene_RunStepWithValue1632(void)
{
    Func_0200338c();
    Func_020033ec(0, 1);
    Func_0200336c(0x1632, 1);
    Func_020033a8();
}
