#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/act/run_zero.h"

void BattlePres_RunWithZeroArguments(void)
{
  Battle_Apply((unsigned long) 0, 0);
}
