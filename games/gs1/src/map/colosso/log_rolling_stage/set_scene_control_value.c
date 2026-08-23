/* Store the caller's halfword in the scene workspace field at offset 220. */
#include "colosso_log_rolling_stage.h"

void ColossoLogRollingStage_SetSceneControlValue(u16 value)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    *(u16 *)(workspace + 220) = value;
}
