#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_02002d10();
s32 Func_02002fae();
s32 Func_02006cb8();
void Func_02006cec();
s32 Func_02006cf4_a();
s32 Func_02006cf4_b();
void Func_02003232(s32);
void Func_02006d4a(void);
s32 Func_02003006(s32);
void Func_02006cd4(s32);
s32 Func_02003054();
void Func_02006d28();
void Func_02006d2e();
s32 Func_02006d76();
void Func_02006da2();
s32 Func_020030b4();
void Func_02006d88();
void Func_02006d8e();
s32 Func_02006dd6();
void Func_02006e02();

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void FieldScene_RunScene3bfSequenceA(void)
{
    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1(Func_02006cb8, 0x214) == 0) {
        if (Value0(Func_02002d10) == 0) {
            if (Data_02000240[294] == 0) {
                if (Value1(Func_02002fae, 17) != 0) {
                    Call1(Func_02006cec, 0x215);
                    Call1(Func_02006cf4_a, 0x214);
                }
            }
            if (Value1(Func_02006cf4_b, 0x214) != 0) {
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 94;

                    *target = shown;
                }
            }
        }
    }
}

void RunActor17SceneStep(void)
{
    Func_02003232(17);
    Func_02006d4a();
}

void TriggerSceneStage95FromActor12(void)
{
    u8 *scene_state = Data_03001ebc;

    if (Func_02003006(12) != 0 && Data_02000240[294] == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Func_02006cd4(0x02009719);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 95;
        *scene_stage = next_stage;
    }
}

void FieldScene_RunScene3bfSequenceB(void)
{
    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1(Func_02006d76, 0x225) == 0) {
        if (Value1(Func_02003054, 13) != 0) {
            if (Data_02000240[294] == 0) {
                Call1(Func_02006da2, 0x225);
                Call1(Func_02006d28, 0x200975d);
                Call1(Func_02006d2e, 0x20097bd);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
            }
        }
    }
}

void FieldScene_RunScene3bfSequenceC(void)
{
    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1(Func_02006dd6, 0x225) == 0) {
        if (Value1(Func_020030b4, 21) != 0) {
            if (Data_02000240[294] == 0) {
                Call1(Func_02006e02, 0x225);
                Call1(Func_02006d88, 0x20097bd);
                Call1(Func_02006d8e, 0x200975d);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
            }
        }
    }
}
