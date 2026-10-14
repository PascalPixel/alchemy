/* Draft, differ 22 of 19: the native keeps the index in r5 and reloads the
 * array base each pass (lsls, mov r3 sp, ldrh [r3, r2]) and tests i = 0
 * against count at entry; agscc strength-reduces and reverses every for,
 * while and guarded do/while tried, with u16 or s16 elements. */
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/party_state.h"

void Func_02000160(s32 owner, s32 delta);

void Func_0200017c(s32 delta)
{
    u16 owners[16];
    s32 count;
    s8 i;

    count = Party_ListActiveOwners((s16 *)owners);
    for (i = 0; i < count; i++)
        Func_02000160(owners[i], delta);
}
