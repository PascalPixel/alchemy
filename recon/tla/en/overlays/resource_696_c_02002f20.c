/* Near miss, differ 2 of 42 (one reordered row): the twin of The Broken Seal's KorosseoKabe_InitializeModeTask, with the script chosen by an else-if chain and priority 0x480. The native loads both cell addresses before it copies mode and param out of r0 and r1; this draft loads the second address after the copies. sched2: the second address load and the two argument copies all have priority 4 and tie on insn order, the copies first. Tried 2026-10-15: s32, u32, s16 and u16 parameters (s16/u16 worse); the twin's if chain (differ 16); the overlay's Ov696_TaskAdd and OV696_TASK_ADD layers (same); stores swapped (12); pointer temporaries (same) with an empty asm barrier on one or both (8, 4) or a one-pass block around their loads (6, 4); alchemy permute 4 minutes (no gain). */
#include "../../../../games/tla/src/field/overlay_696/ov696.h"

extern u16 Data_020064b0;
extern u16 Data_02006494;
extern u16 Data_020064bc;
extern s16 *Data_020064c0;
extern u16 Data_02006504;
extern u16 Data_0200649c;
extern s32 Data_020064a0;
extern s16 Data_0200515a[];
extern s16 Data_02004b58[];
extern s16 Data_02005186[];
extern s16 Data_02004b80[];
extern s16 Data_02005204[];

void Func_02002a20(void);

void Func_02002f20(s32 mode, s32 param)
{
    s32 handler;

    Data_020064b0 = mode;
    Data_02006494 = (param << 4);

    {
        s32 budget = 0x480;
        Scheduler_AddOrUpdateCallback(Func_02002a20, budget);
    }

    handler = (s32)Data_0200515a;
    if (mode == 2)
    {
        handler = (s32)Data_02004b58;
    }
    else if (mode == 4)
    {
        handler = (s32)Data_02005186;
    }
    else if (mode == 3)
    {
        if (param != 0)
        {
            handler = (s32)Data_02004b80;
        }
        else
        {
            handler = (s32)Data_02005204;
        }
    }

    Data_020064bc = 0;
    Data_020064c0 = (s16 *)handler;
    Data_02006504 = 0;
    Data_0200649c = 0;
    Data_020064a0 = 0;
}
