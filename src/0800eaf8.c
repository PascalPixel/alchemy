#include "types.h"

/* キー入力と設定表の照合。押下キーに対応する番号欄へ1を立てる。
   該当が無ければ表の値をFunc_0800ea60へ渡す。
   キー状態は割り込みで更新されるため、判定ごとに読み直す。 */

extern u8 *Data_03001ebc;
extern u16 Data_02000240[];
extern volatile u32 Data_03001c94;

u32 Func_0800ea60(u32);

s32 Func_0800eaf8(void)
{
    u8 *work = Data_03001ebc;
    s32 result = 0;

    if (work == NULL) {
        return 0;
    }

    if (Data_03001c94 & Data_02000240[266]) {
        s16 *q = (s16 *)(work + 185 * 2);
        s32 v = 1;
        *q = v;
        result = 1;
    } else if (Data_03001c94 & Data_02000240[264]) {
        s16 *q = (s16 *)(work + 186 * 2);
        s32 v = 1;
        *q = v;
        result = 1;
    } else if (Data_03001c94 & Data_02000240[267]) {
        s16 *q = (s16 *)(work + 187 * 2);
        s32 v = 1;
        *q = v;
        result = 1;
    } else if (Data_03001c94 & Data_02000240[268]) {
        result = Func_0800ea60(Data_02000240[272]);
    } else if (Data_03001c94 & Data_02000240[269]) {
        result = Func_0800ea60(Data_02000240[273]);
    }

    return result;
}
