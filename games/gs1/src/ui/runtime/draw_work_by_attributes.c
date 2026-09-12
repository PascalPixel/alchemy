#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08017248(s32, s32, s32, s32, s32);
void Func_080170f8(s32, s32, s32, s32);
typedef void (*UiFillFn)(s32 dst, s32 size, s32 value);

void UiWork_DrawByAttributes(void *arg0)
{
    u32 attr;
    u32 tmp;
    u32 v0;
    u32 v1;
    u32 v2;
    u32 v3;
    s32 dst;
    UiFillFn fill;
    void *work;

    /* 描画属性に従い転送方法を切り替える。 */
    work = *(void **)ADDR_03001E8C;
    tmp = FIELD_AT_OFFSET(arg0, u16 *, 0xA);
    attr = FIELD_AT_OFFSET(arg0, u16 *, 0x16);
    v3 = tmp;
    tmp = 0;
    FIELD_AT_OFFSET(arg0, s16 *, 0x1A) = tmp;
    v0 = FIELD_AT_OFFSET(arg0, u16 *, 0xC);
    v1 = FIELD_AT_OFFSET(arg0, u16 *, 0xE);
    v2 = FIELD_AT_OFFSET(arg0, u16 *, 8);
    if (8 & attr) {
        if (0x20 & attr) {
            Func_080170f8(v0, v1, v2, v3);
            fill = (UiFillFn)0x03000168;
            dst = 0x06002500;
            fill(dst, 0xF00, 0x44444444);
        } else {
            fill = (UiFillFn)0x03000168;
            dst = 0x06002500;
            fill(dst, 0xF00, 0);
        }
        Func_08017248(v0, v1, v2, v3, 0);
    } else {
        Func_080170f8(v0, v1, v2, v3);
    }
    FIELD_AT_OFFSET(work, s8 *, RENDER_DIRTY_OFS) = 1;
}
