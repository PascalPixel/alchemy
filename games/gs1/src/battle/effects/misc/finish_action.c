#include "scene.h"
#include "abi/battle/effects/misc/finish_action.h"
#include "event_runtime.h"

extern struct EventValueWork gCell;

void ScheduleCallback(u32);

void Battle_UpdateModeFromShoulderButtons(void);

void BattleFx_FinishAction(void)
{
    ScheduleCallback((u32)Battle_UpdateModeFromShoulderButtons);
    Battle_Apply(gCell.value, 1);
    Battle_Run();
}
