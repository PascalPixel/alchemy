#include "TYPES.H"

s32 BattlePlacement_ContainsId(s16 *entries, s32 id)
{
    u32 i;

    if (id > 7)
        id += 120;
    i = 0;
    do {
        u16 value = *entries++;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#endif
        if (value == 0xff)
            break;
        if (value == id)
            return 1;
    } while (++i < 14);
    return 0;
}
