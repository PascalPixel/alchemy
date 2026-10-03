#include "OWNER_STATE.H"
#include "TYPES.H"

s32 Shop_CanServe(s32 selection, s32 variant);

s32 Shop_ServicePrice(s32 entry_no, s32 kind)
{
    u8 value = ((u8 *)Owner_GetState(entry_no))[0xF];
    s32 result = 0;

    if (kind == 0) {
        result = value * 20;
    } else if (kind == 1) {
        result = 10;
    } else if (kind == 2) {
        result = 50;
    } else if (kind == 3) {
        result = value * 10;
    }
    return result;
}

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
    u8 *entry = Owner_GetState(entry_no);
    s32 result = 0;

    if ((kind == 0 && *(s16 *)(entry + 56) <= 0)
        || (kind == 1 && *(s8 *)(entry + 305) != 0)
        || (kind == 2 && entry[320] != 0)
        || (kind == 3 && *(s8 *)(entry + 304) != 0)) {
        result = 1;
    }
    return result;
}
