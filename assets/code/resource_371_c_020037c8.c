#include "types.h"

/* Complete prologue-less shared-state activation leaf through its pool. */
struct SharedState_020037c8 {
    u8 reserved00[52];
    u8 active;
};

extern struct SharedState_020037c8 *Data_03001f30;

void Func_020037c8(void)
{
    Data_03001f30->active = 1;
}
