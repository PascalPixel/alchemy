/* Near miss, differ 2 of 42 (one reordered row): the twin of The Broken Seal's SceneData_SelectBlockAndResetCounters, with the script chosen by an else-if chain. The native loads both cell addresses before it copies mode and param out of r0 and r1 (ldr r3; ldr r2; adds r5; adds r6); this draft loads the second address after the copies. Tried 2026-10-12: (u16) casts as the twin has (same); both addresses in pointer temporaries (same), with an empty asm barrier after them (the copies move above both loads, differ 4) or between the two stores (differ 4). sched2 dump: the second address load (priority 4) ties with the two argument copies and loses on insn order; the native needs it at priority 5 or ahead of the copies. */
#include "../../../../games/tla/src/field/overlay_696/ov696.h"

extern u16 Data_020064b0;
extern u16 Data_02006494;
extern u16 Data_020064bc;
extern const void *Data_020064c0;
extern u16 Data_02006504;
extern u16 Data_0200649c;
extern s32 Data_020064a0;
extern u16 Data_0200515a[];
extern s16 Data_02004b58[];
extern u8 Data_02005186[];
extern u8 Data_02004b80[];
extern u8 Data_02005204[];

void Func_02002a20(void);

void Func_02002f20(s32 mode, s32 param)
{
    const void *script;

    Data_020064b0 = mode;
    Data_02006494 = param << 4;
    Scheduler_AddOrUpdateCallback(Func_02002a20, 0x480);

    script = Data_0200515a;
    if (mode == 2) {
        script = Data_02004b58;
    } else if (mode == 4) {
        script = Data_02005186;
    } else if (mode == 3) {
        if (param != 0)
            script = Data_02004b80;
        else
            script = Data_02005204;
    }

    Data_020064bc = 0;
    Data_020064c0 = script;
    Data_02006504 = 0;
    Data_0200649c = 0;
    Data_020064a0 = 0;
}
