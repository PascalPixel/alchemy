#include "types.h"

/* battle/unit/lookup_weapon_value_by_class.c */
void *Runtime_GetObject(s32 actor_id);
s32 Battle_Apply(s32, s32);
u32 Battle_Do(s32);

extern u16 RomBytes_080c2a1c[];
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
            result = RomBytes_080c2a1c[sel];
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
