#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/reset_run.h"
#include "colosso_log_rolling_stage.h"

typedef void (*Entry02000188)(void);

extern s32 gOv;
extern s32 gOv2;

void Colosso_ResetAndRunSceneTask(void)
{
    Entry02000188 entry;

    gOv2 = 0;
    entry = (Entry02000188) 0x0200804D;
    gOv = 0;
    Map_EntOp(entry);
    entry();
}
