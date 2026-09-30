#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The mode task's records in the river overlay's data. */
extern u8 KorosseoKawa_RoundSpans[];
extern u8 KorosseoKawa_ModeRecordTwo[];
extern u8 KorosseoKawa_ModeRecordFour[];
extern u8 KorosseoKawa_SpanA[];
extern u8 KorosseoKawa_SpanB[];

/* The mode task's state in the overlay's work area. */
extern u16 Korosseo_ModeTaskMode;
extern u16 Korosseo_ModeTaskParam;
extern u16 Korosseo_ModeTaskTimer;
extern s32 Korosseo_ModeTaskScript;
extern u16 Korosseo_ModeMoveTarget;
extern u16 Korosseo_ModeMoveDuration;
extern s32 Korosseo_ModeTaskPosition;

void Korosseo_UpdateModeTask(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

/*
 * The mode task's per-frame routine is installed as a callback.  The branch
 * chain picks one of five mode records by mode, consulting param only when
 * mode is 3.  The stores that follow reset the rest of the task's state.
 */
void SceneData_SelectBlockAndResetCounters(u32 mode, u32 param)
{
    s32 handler;

    Korosseo_ModeTaskMode = (u16)mode;
    Korosseo_ModeTaskParam = (u16)(param << 4);

    Scheduler_AddOrUpdateCallback(Korosseo_UpdateModeTask, 0xc80);

    handler = (s32)KorosseoKawa_RoundSpans;
    if (mode == 2) {
        handler = (s32)KorosseoKawa_ModeRecordTwo;
    }
    if (mode == 4) {
        handler = (s32)KorosseoKawa_ModeRecordFour;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)KorosseoKawa_SpanB;
        } else {
            handler = (s32)KorosseoKawa_SpanA;
        }
    }

    Korosseo_ModeTaskTimer = 0;
    Korosseo_ModeTaskScript = handler;
    Korosseo_ModeMoveTarget = 0;
    Korosseo_ModeMoveDuration = 0;
    Korosseo_ModeTaskPosition = 0;
}
