#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The marker task's state in the overlay's work area. */
extern u16 Korosseo_MarkerX;
extern u16 Korosseo_MarkerY;
extern u16 Korosseo_MarkerPriority;
extern u16 Korosseo_MarkerBlink;
extern u16 Korosseo_MarkerSteps;
extern u16 Korosseo_MarkerEndX;
extern u16 Korosseo_MarkerEndY;
extern u16 Korosseo_MarkerStartX;
extern u16 Korosseo_MarkerStartY;
extern u16 Korosseo_MarkerStep;

void Korosseo_UpdateMarker(void);
void SceneState_InitHalfwordC6a6Once(void);
s32 Engine_ScheduleCallback(void (*callback)(void), s32 priority);

/* Place the marker at x, y with a priority and start its task. */
void SceneState_StoreParamsAndInitTable(u32 x, u32 y, u32 style)
{
    SceneState_InitHalfwordC6a6Once();

    Korosseo_MarkerX = (u16)x;
    Korosseo_MarkerY = (u16)y;
    Korosseo_MarkerPriority = (u16)(style & 3);
    Korosseo_MarkerBlink = 0;
    Korosseo_MarkerSteps = 0;

    Engine_ScheduleCallback(Korosseo_UpdateMarker, 0xc80);
}

/* Move the marker from where it is to x, y over the given steps. */
void SceneState_InitTableWordsAndLoad3200(u32 x, u32 y, u32 duration)
{
    Korosseo_MarkerEndX = (u16)x;
    Korosseo_MarkerEndY = (u16)y;
    Korosseo_MarkerStartX = Korosseo_MarkerX;
    Korosseo_MarkerStartY = Korosseo_MarkerY;
    Korosseo_MarkerSteps = (u16)duration;
    Korosseo_MarkerStep = 0;

    Engine_ScheduleCallback(Korosseo_UpdateMarker, 0xc80);
}
