#include "TYPES.H"

extern u8 *gEffectWork;

void BattleFx_LoadActionEffectResources();
void BattleFx_SetupObjectPair();
void Field_DispatchTypeHandler();
void EventObject_Initialize();
void EffectRuntime_StopCurrentObject();

void GomaIriguchi_SetEntranceFlag(void)
{

    u8 *p5;

    p5 = gEffectWork;
    BattleFx_LoadActionEffectResources(78, 1);
    BattleFx_SetupObjectPair(2, 15);
    {
        u8 *f = (u8 *)((s32)p5 + 0x71c);
        s32 t = 8;

        t |= *f;
        *f = t;
    }
    EventObject_Initialize();
    Field_DispatchTypeHandler(1);
    EffectRuntime_StopCurrentObject();
}
