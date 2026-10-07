#include "BTLSYS.H"
#include "TYPES.H"
#include "SCENE.H"

s32 Battle_ClassifyEntryKind(const struct BtlAct *entry)
{
    s32 b3;
    s32 kind;
    s32 low;
    s32 ret;
    u8 k;

    ret = 0;
    low = entry->tgt_flg & 0xF;
    if (low == 1) {
        ret = 1;
    }
    if (low == 0xB) {
        ret = 2;
    }
    b3 = (s8)entry->efx;
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
    if (BtlEfxRevChkFar(entry->efx) != 0) {
        ret = 5;
    }
    return ret;
}
