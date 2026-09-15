#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"

extern volatile u32 gIw;
void WaitFrames(s32);

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)gIw;
        WaitFrames(1);
    }
}

void RuntimeDispatch_ReservedNoOpA(void)
{
}

void RuntimeDispatch_ReservedNoOpB(void)
{
}

void RuntimeDispatch_ReservedNoOpC(void)
{
}

void RuntimeDispatch_ReservedNoOpD(void)
{
}

void RuntimeDispatch_ReservedNoOpE(void)
{
}

s32 RuntimeDispatch_ReturnZero(void)
{
    return 0;
}
