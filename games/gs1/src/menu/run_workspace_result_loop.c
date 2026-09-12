#include "types.h"
#include "scene.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE)
#define RESULT_CELL_ADDR 0x03001CD8
#else
#define RESULT_CELL_ADDR 0x03001CC8
#endif

extern char Value_00000c2a;

s32 Menu_RunWorkspaceResultLoop(void)
{
    s32 result;

retry:
    result = Menu_Check();
    if (result == -1) {
        return -1;
    }
    if (result == 0) {
        if (Save_WriteSelectedSlot() == -1) {
            goto retry;
        }
    } else if (result == 1) {
        Menu_Apply((s32)&Value_00000c2a, 1);
        *(u8 *)RESULT_CELL_ADDR = result;
    } else if (result == 2) {
        if (Menu_RunWorkspaceOptions() == -1) {
            goto retry;
        }
    }
    return 0;
}
