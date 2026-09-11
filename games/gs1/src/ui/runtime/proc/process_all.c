#include "types.h"
#include "scene.h"
#include "abi/ui/runtime/proc/process_all.h"

void UiWork_ProcessRenderChannels(void);

void UiWork_ProcessAll(void)
{
    Ui_Check();
    UiWork_ProcessRenderChannels();
    Ui_Check2();
}
