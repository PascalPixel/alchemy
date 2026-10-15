#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

void SummonFx_RunCanvasMode0(void *action)
{
    SummonFx_RunCanvasEffect(action, 0);
}

void SummonFx_RunCanvasMode3(void *action)
{
    SummonFx_RunCanvasEffect(action, 3);
}

void SummonFx_RunCanvasMode1(void *action)
{
    SummonFx_RunCanvasEffect(action, 1);
}

void SummonFx_RunCanvasMode2(void *action)
{
    SummonFx_RunCanvasEffect(action, 2);
}
