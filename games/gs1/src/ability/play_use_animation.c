#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_08077080();
void Func_080a2438(s32);

#define Ability_PlayUseAnimation Func_080aa460

void Ability_PlayUseAnimation(void)
{
    s32 animation_type;
    u32 target_type;
    void *ability;

    ability = Func_08077080();
    animation_type = 0xf & FIELD(ability, u8 *, 1);
    switch (animation_type) {
    case 1:
        Func_080a2438(0x7e);

    case 11:
        Func_080a2438(0x7e);
        return;
    default:
        target_type = FIELD(ability, u8 *, 3) - 1;
        switch (target_type) {
        case 4:
            Func_080a2438(0x52);
            return;
        case 2:
            Func_080a2438(0x54);
            return;
        default:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            Func_080a2438(0x5b);

        case 0:
        case 1:
        case 30:
        case 31:
            return;
        }
        break;
    }
}
