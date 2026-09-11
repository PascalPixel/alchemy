#include "types.h"
#include "scene.h"
#include "abi/owner/refresh_and_reset_zero.h"

void Owner_RefreshAndResetZero(void)
{
    Sys_Check();
    Sys_Check2(0);
}
