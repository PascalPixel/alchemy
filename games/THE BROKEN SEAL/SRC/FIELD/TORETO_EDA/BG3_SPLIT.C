#include "TYPES.H"

void Effect_UpdateBg3HofsByVcount(void);
void Effect_SetBg3HofsSplit(void);
void Runtime_SetIrqHandler(s32 slot, s32 mode, void (*handler)(void));
s32 Engine_ScheduleCallback(void (*callback)(void), s32 priority);

/* Start the BG3 split: run the scroll update from the vertical-count
 * interrupt and schedule the task that prepares its values. */
void ToretoEda_StartBg3Split(void)
{
    Runtime_SetIrqHandler(1, 0, Effect_UpdateBg3HofsByVcount);
    Engine_ScheduleCallback(Effect_SetBg3HofsSplit, 0xc80);
}
