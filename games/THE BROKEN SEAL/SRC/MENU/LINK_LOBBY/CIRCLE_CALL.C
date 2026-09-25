#include "TYPES.H"

s32 Engine_GameFlagIsSet(s32 flag);
void Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
void Engine_EventBegin(void);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 actor, s32 mode);
void Engine_TaskWait(s32 frames);
s32 Engine_EventEnd(void);

/* Frames since the attendant last called out. */
extern u32 Data_02009f50;

static __inline__ s32 Value1(s32 (*f)(s32), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(s32), s32 a0)
{
    f(a0);
}

/* Unless flag 0x203 is set, has the attendant call "please step into the
 * circle!" (message 10542) once every 300 frames: flag 0x200 marks a call
 * already made in the current period. */
s32 LinkLobby_CallIntoCircle(void)
{
    s32 set;

    set = Engine_GameFlagIsSet(0x203);
    if (set != 0) {
        return set;
    }
    if (++Data_02009f50 == 300) {
        Data_02009f50 = 0;
        Call1(Engine_GameFlagClear, 0x200);
    }
    set = Value1(Engine_GameFlagIsSet, 0x200);
    if (set != 0) {
        return set;
    }
    Engine_EventBegin();
    Engine_EventSetMessage(10542);
    Engine_EventOpenMessage(8, 0);
    Engine_TaskWait(5);
    Call1(Engine_GameFlagSet, 0x200);
    return Engine_EventEnd();
}
