#include "types.h"
#include "resource.h"
#include "system.h"
extern u8 gData_03001f38[];

struct MenuSelectionState {
    u8 padding000[0x78];
    void *work;
    u8 padding07c[8];
    u8 resourceIds[8];
    s16 selection;
    s16 itemCount;
    s16 field090;
    s16 resourceBase;
};

extern struct MenuSelectionState *gMenuSelectWork;
extern u8 gMenu_SelectionStepDelays[];
extern u8 MsgCommandName;

void RenderOutput_PrepareForRedraw(void *work);
void UiText_DrawCharacterAtOffset(s32 resourceId, void *work, s32 x, s32 y);
void Audio_PlayCue(s32 sound_id);

s32 Menu_SelectResource(s32 start, s32 goal)
{
    s16 resourceBase;
    s16 cur;
    s32 diff;
    s32 resourceId;
    s32 dist;
    s32 pos;
    s32 step;
    s32 delay;
    const u8 *table;
    struct MenuSelectionState *state;

    state = gMenuSelectWork;
    step = 1;
    delay = 12;
    state->selection = (s16)start;
    if (goal < start)
        step = -1;
    pos = start;

    for (;;) {
        RenderOutput_PrepareForRedraw(state->work);
        resourceBase = state->resourceBase;
        if (resourceBase != 0) {
            resourceId = resourceBase + state->selection;
        } else {
            resourceId = state->resourceIds[state->selection] + (s32)&MsgCommandName;
        }
        UiText_DrawCharacterAtOffset(resourceId, state->work, 0, 0);

        cur = state->selection;
        table = gMenu_SelectionStepDelays;
        diff = cur - goal;
        dist = AbsoluteDifference(diff, cur, goal);
        WaitFrames(table[dist] + delay);

        if (pos == goal)
            break;

        state->selection = (s16)((u16)state->selection + step);
        Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
        delay = 0;
        pos += step;
    }

    WaitFrames(48);
    Audio_PlayCue(SOUND_MENU_CONFIRM);
    return goal;
}
