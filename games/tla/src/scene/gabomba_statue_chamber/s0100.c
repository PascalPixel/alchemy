#include "gabomba_statue_chamber.h"
#include "call.h"

void Func_02001d88(s32 a, s32 b, s32 c);
void Func_02001eb8(s32 a, s32 b);
void Func_02001ec0(s32 a, s32 b);
void Func_02001ec8(s32 frames);

void Func_02000100(void)
{
    volatile u16 *color = (volatile u16 *)0x0500021e;
    struct GabombaStatueChamberEventWork *work;

    Object_SetModeById(8, 2);
    Call2((void (*)())Func_02001ec0, 0x10000, 2);
    Call2((void (*)())Func_02001eb8, 0x10003, 2);
    Func_02001ec8(20);
    WaitFrames(20);
    *color = 0x7fff;
    WaitFrames(1);
    work = Ram_HeapSlots->event_work;
    Func_02001d88((s16)work->field_1c4++, 0, 2);
    Call2((void (*)())Func_02001eb8, 0x10000, 2);
    Func_02001ec8(20);
    WaitFrames(20);
    Object_SetModeById(8, 1);
}
