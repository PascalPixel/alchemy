#include "types.h"
#include "scene.h"
#include "abi/display/blend/disable_run_script.h"

void DisplayBlend_RunScript(void);

void DisplayBlend_DisableRunScript(void)
{
    Sys_Check((u32)DisplayBlend_RunScript);
}
