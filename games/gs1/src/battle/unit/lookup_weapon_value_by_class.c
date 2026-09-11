#include "types.h"
#include "scene.h"
#include "abi/battle/unit/lookup_weapon_value_by_class.h"

u8 *Runtime_GetObject(s32);

extern u16 gRom[];
extern u16 gRom2[];
extern u16 gRom3[];
extern u16 gRom4[];
extern u16 gRom5[];

s32 BattleUnit_LookupWeaponValueByClass(s32 id)
{
    u8 *state;
    s32 entry;
    s32 result;

    state = Runtime_GetObject(id);
    entry = Battle_Apply(id, 1);
    result = 0;
    if (entry >= 0) {
        s32 ofs;
        s32 sel;

        ofs = entry * 2 + 216;
        sel = Battle_Do(*(u16 *)(state + ofs) & 0x1FF);
        switch (state[296]) {
        case 0:
            result = gRom[sel];
            break;
        case 1:
            result = gRom2[sel];
            break;
        case 2:
            result = gRom3[sel];
            break;
        case 3:
            result = gRom4[sel];
            break;
        case 4:
            break;
        case 5:
            result = gRom5[sel];
            break;
        }
    }
    return result;
}
