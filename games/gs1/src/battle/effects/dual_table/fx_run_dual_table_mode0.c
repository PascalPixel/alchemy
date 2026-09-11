#include "types.h"
#include "scene.h"

/* battle/effects/dual_table/run_mode0.c */
void BattleFx_RunDualTableMode0(s32 arg0)
{
    Battle_Apply(arg0, 0);
}

/* battle/effects/dual_table/run_mode1.c */
void BattleFx_RunDualTableMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
