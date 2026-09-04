#include "types.h"
#include "gs1_edition.h"

#define Menu_RunWorkspaceResultLoop Func_08029504

#if defined(GS1_EDITION_DE)
#define RESULT_CELL_ADDR 0x03001CD8
#else
#define RESULT_CELL_ADDR 0x03001CC8
#endif

s32 Func_0801db70(void);
s32 Func_080207c4(void);
void Func_0801776c(s32, s32);
s32 Func_0801d4cc(void);
extern char Value_00000c2a;

s32 Menu_RunWorkspaceResultLoop(void)
{
    s32 result;

retry:
    result = Func_0801db70();
    if (result == -1) {
        return -1;
    }
    if (result == 0) {
        if (Func_080207c4() == -1) {
            goto retry;
        }
    } else if (result == 1) {
        Func_0801776c((s32)&Value_00000c2a, 1);
        *(u8 *)RESULT_CELL_ADDR = result;
    } else if (result == 2) {
        if (Func_0801d4cc() == -1) {
            goto retry;
        }
    }
    return 0;
}
