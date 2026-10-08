#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct DepthPart {
    u8 unknown_00[9];
    u8 unknown_09_0 : 2;
    u8 priority : 2;
    u8 unknown_09_4 : 4;
};

struct DepthMapBase {
    u8 unknown_00[4];
    s32 base;
};

struct DepthScroll {
    s32 unknown_00;
    s32 offset;
};

struct DepthMap {
    struct DepthMapBase *base;
    u8 unknown_04[0xe0];
    struct DepthScroll scroll;
};

s32 Render_ProjectPoint(s32 *point, s32 *screen);
void Func_080eb01c(void *work);

void Func_080dba44(void)
{
    Func_080eb01c((u8 *)Ram_HeapSlots->effect_work + 0x7c8);
}
