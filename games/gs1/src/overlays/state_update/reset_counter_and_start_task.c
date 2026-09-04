#include "types.h"

#define SceneState_ResetCounterAndStartTask Func_02000134

typedef void (*Task02000134)(void);

extern s32 Data_0200c41c;

extern void Func_02003c5a(Task02000134);

void SceneState_ResetCounterAndStartTask(void)
{
    Task02000134 task;

    Data_0200c41c = 0;
    task = (Task02000134) 0x0200804D;
    Func_02003c5a(task);
    task();
}
