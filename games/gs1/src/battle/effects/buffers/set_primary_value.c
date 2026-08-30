#include "types.h"
#include "global_cells.h"

void BattleEffect_SetPrimaryBufferValue(unsigned int value)
{
  s16 *primary_buffer;
  primary_buffer = *((s16 **) ADDR_03001ED0);
  if (0 != primary_buffer)
  {
    *primary_buffer = value;
  }
}
