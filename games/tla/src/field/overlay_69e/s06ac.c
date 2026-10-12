#include "types.h"
#include "ram_buffer.h"
#include "flag_call.h"

struct SparkWork {
    u8 unknown_0000[0x1d9c];
    s16 resource;
};

extern s32 Data_020033dc;
extern u8 MsgFieldShowAnemosPower[];

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Resource_ResetEntry(s32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void Event_WaitTransitionFrames(void);
void Battle_WaitMode0(s32 frames);
s32 Object_SetModeById(u32 id, s32 mode);
void Func_02001538(void);
void Func_02002898(s32 message, s32 a1, s32 a2);
void Func_020028b0(void);
void Func_020028b8(void);
void Func_02002978(s32 a0, s32 a1);
void Func_02002980(s32 a0, s32 a1);
void Func_02002988(s32 frames);
void Func_020029a8(s32 a0);

static inline void Func_02002978_(s32 a0, s32 a1)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002978(a0, a1);
}

static inline void Func_02002980_(s32 a0, s32 a1)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002980(a0, a1);
}

void Func_020006ac(void)
{
    if (!Flag_IsSet(0xa0e)) {
        Flag_Set(0xa0e);
        Func_020028b0();
        Func_020029a8(0);
        Func_02002980_(0x10000, 0);
        Func_02002978_(0x10003, 1);
        Func_02002988(30);
        Event_WaitTransitionFrames();
        Object_SetModeById(0, 1);
        Battle_WaitMode0(30);
        Func_02002898((s32)MsgFieldShowAnemosPower, 0, -2);
        Func_02002978_(0x10000, 0);
        Func_02002988(30);
        Func_020028b8();
    }
}

void Func_02000724(void)
{
    GameFlag_ClearBit(0xa0e);
}
