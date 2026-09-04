#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define UiWork_CopyParamsToRenderWork Func_080167ac

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void UiWork_CopyParamsToRenderWork(void *work) {
    u32 p2;
    s32 p1;
    s32 z1;
    u32 p0;
    u32 z0;
    void *render;

    render = *(void **)ADDR_03001E8C;
    p0 = (s32) (FIELD_AT_OFFSET(work, u16 *, 0x16)); FIELD_AT_OFFSET(render, u16 *, RENDER_PARAM_OFS) = (u16) p0;
    p1 = (s32) (FIELD_AT_OFFSET(work, u16 *, 0x18)); (s32) z0 = 0; FIELD_AT_OFFSET(render, u16 *, RENDER_WORD_OFS) = (u16) p1;
    p2 = (FIELD_AT_OFFSET(work, u16 *, 0x1A)); (s32) z1 = 0; FIELD_AT_OFFSET(render, u16 *, RENDER_WORD2_OFS) = (u16) p2;
}
