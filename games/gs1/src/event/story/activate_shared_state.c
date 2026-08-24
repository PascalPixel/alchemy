#include "types.h"

/* Complete prologue-less shared-state activation leaf through its pool. */
struct StorySharedState {
    u8 reserved00[52];
    u8 active;
};

extern struct StorySharedState *Data_03001f30;

void StoryScene_ActivateSharedState(void)
{
    Data_03001f30->active = 1;
}
