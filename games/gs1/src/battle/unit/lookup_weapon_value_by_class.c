#include "types.h"

#define BattleUnit_LookupWeaponValueByClass Func_080b6eb4

u8 *Runtime_GetObject(s32);
s32 Func_08077228(s32, s32);
u32 Func_080b6e7c(s32 arg0);
extern u16 Data_080c2a1c[];
extern u16 Data_080c2a2a[];
extern u16 Data_080c2a38[];
extern u16 Data_080c2a46[];
extern u16 Data_080c2a54[];

s32 BattleUnit_LookupWeaponValueByClass(s32 id) {
    u8 *state;
    s32 entry;
    s32 result;

    state = Runtime_GetObject(id);
    entry = Func_08077228(id, 1);
    result = 0;
    if (entry >= 0) {
        s32 ofs;
        s32 sel;

        ofs = entry * 2 + 216;
        sel = Func_080b6e7c(*(u16 *)(state + ofs) & 0x1FF);
        switch (state[296]) {
        case 0:
            result = Data_080c2a1c[sel];
            break;
        case 1:
            result = Data_080c2a2a[sel];
            break;
        case 2:
            result = Data_080c2a38[sel];
            break;
        case 3:
            result = Data_080c2a46[sel];
            break;
        case 4:
            break;
        case 5:
            result = Data_080c2a54[sel];
            break;
        }
    }
    return result;
}
