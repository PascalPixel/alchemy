#include "types.h"
#include "scene.h"
#include "abi/field/check_configured_keys.h"

/* キー入力と設定表の照合。押下キーに対応する番号欄へ1を立てる。
   該当が無ければ表の値をFunc_0800ea60へ渡す。
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
