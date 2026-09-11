#include "types.h"
#include "scene.h"
#include "gs1_edition.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

#if defined(GS1_EDITION_JA)
#define FINAL_ARG 2
#else
#define FINAL_ARG 1
#endif

/*
 * This owner's view of gCell, which games/gs1/include/battle_effect_runtime.h
 * declares as `struct BattleWork`. Two fields are evidence here; the paddings
 * are arithmetic to reach them.
 */
struct Work_080b0444 {
    u8 padding0[0x10];
    s32 value10;
    u8 padding14[0x108];
    s8 value11c;
};

extern struct Work_080b0444 gCell;

void *Runtime_GetObject(s32);

/* 固定値を設定し、3つの項目フラグを1にする。 */
s32 Battle_ApplyPresetItemsAndFlags(void)
{
    gCell.value10 = 0x30d40;
    gCell.value11c = 0x1c;
    Battle_Apply(1, Battle_Apply2(1, 0x48d));
    Battle_Apply(0, Battle_Apply2(0, 0x40b));
    Battle_Apply2(2, 0xe7);
    FIELD((void *)Runtime_GetObject(3), s8 *, 0x131) = 1;
    FIELD((void *)Runtime_GetObject(5), s8 *, 0x131) = 1;
    FIELD((void *)Runtime_GetObject(2), s8 *, 0x140) = 1;
    Battle_Apply3(FINAL_ARG, 0x1e);
    return 0;
}
