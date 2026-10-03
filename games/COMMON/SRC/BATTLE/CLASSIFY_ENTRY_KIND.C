#include "BATTLE_RUNTIME.H"
#include "TYPES.H"
#include "SCENE.H"

/* battle/classify_entry_kind.c */
s32 Battle_ClassifyEntryKind(const struct BattleAction *entry)
{
    s32 b3;
    s32 kind;
    s32 low;
    s32 ret;
    u8 k;

    ret = 0;
    low = entry->target_flags & 0xF;
    if (low == 1) {
        ret = 1;
    }
    if (low == 0xB) {
        ret = 2;
    }
    b3 = (s8)entry->effect;
    kind = b3;
    k = kind;
    if (k == 3) {
        ret = 3;
    }
    if (k == 4) {
        ret = 4;
    }
    if (k == 0x40) {
        ret = 6;
    }
    if (BattleFx_IsReviveFar(entry->effect) != 0) {
        ret = 5;
    }
    return ret;
}
