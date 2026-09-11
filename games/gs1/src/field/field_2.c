#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* field/store_assigned_key_value.c */
extern u8 *gWork;
s32 GameFlag_IsSet(s32);

static __inline__ void StoreHalfword(u8 *address, s32 value)
{
    *(s16 *)address = value;
}

u32 Field_StoreAssignedKeyValue(u32 value)
{
    u32 no = value >> 14;
    u32 ret = 0x3FFF & value;
    u8 *state = gWork;

    if (GameFlag_IsSet(0x107) != 0) {
        StoreHalfword(state + 0x182, 0xFA);
    } else if (*(s16 *)(state + 0x19E) == 3) {
        if (*(volatile u32 *)ADDR_03001C94 & 0x100) {
            StoreHalfword(state + 0x182, 0xFC88);
        } else if (*(volatile u32 *)ADDR_03001C94 & 0x200) {
            StoreHalfword(state + 0x182, 0xFC87);
        }
    } else {
        switch (no) {
        case 0:
            StoreHalfword(state + 0x17E, ret);
            break;
        case 1:
            StoreHalfword(state + 0x180, ret);
            break;
        }
    }

    return ret;
}

/* field/check_configured_keys.c */
/* キー入力と設定表の照合。押下キーに対応する番号欄へ1を立てる。
   該当が無ければ表の値を Field_StoreAssignedKeyValue へ渡す。
   キー状態は割り込みで更新されるため、判定ごとに読み直す。 */

extern u8 *gWork;
extern u16 gCell[];
extern volatile u32 gIw;

s32 Field_CheckConfiguredKeys(void)
{
    u8 *work = gWork;
    s32 ret = 0;

    if (work == NULL) {
        return 0;
    }

    if (gIw & gCell[266]) {
        s16 *q = (s16 *)(work + 185 * 2);
        s32 v = 1;
        *q = v;
        ret = 1;
    } else if (gIw & gCell[264]) {
        s16 *q = (s16 *)(work + 186 * 2);
        s32 v = 1;
        *q = v;
        ret = 1;
    } else if (gIw & gCell[267]) {
        s16 *q = (s16 *)(work + 187 * 2);
        s32 v = 1;
        *q = v;
        ret = 1;
    } else if (gIw & gCell[268]) {
        ret = Field_Do(gCell[272]);
    } else if (gIw & gCell[269]) {
        ret = Field_Do(gCell[273]);
    }

    return ret;
}
