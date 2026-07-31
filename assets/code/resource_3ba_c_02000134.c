typedef signed int s32;

typedef void (*Task02000134)(void);

extern s32 Data_0200c41c;

extern void Func_02003c5a(Task02000134);

void Func_02000134(void)
{
    Task02000134 task;

    Data_0200c41c = 0;
    task = (Task02000134) 0x0200804D;
    Func_02003c5a(task);
    task();
}
