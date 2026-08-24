#include "types.h"
#include "global_cells.h"

void BattleEffect_SetPrimaryBufferValue(unsigned int arg0)
{
  s16 *temp_r3;
  temp_r3 = *((s16 **) ADDR_03001ED0);
  if (0 != temp_r3)
  {
    *temp_r3 = arg0;
  }
}
