#include "types.h"
#include "party_state.h"

void *Func_08077394(s32 owner);
s32 Func_08079338(void *entry);
void Func_08079358(void *entry);
s32 Func_080795fc(void);
s32 Func_0807a1b4(s32 owner, s32 index, u8 *state);
u32 *Func_0807a458(s32 owner, s32 index, u8 *state);

s32 Func_0807a0f4(s32 index, u8 *state)
{
    void *entry = state + index * 20 + 48;
    s32 bestOwner = 0;
    s32 bestValue = 999;
    s32 count;
    u8 *owners;

    if (Func_08079338(entry) != 0)
        return -1;

    count = Func_080795fc();
    if (bestOwner < count) {
        owners = Data_02000240.active_owners;
        do {
            u8 *ownerState = Func_08077394(*owners);

            if (ownerState[280 + index] <= 9) {
                s32 value = 0;
                s32 i = 0;

                do {
                    value += ownerState[280 + i];
                    i++;
                } while (i <= 3);

                if (bestValue > value) {
                    bestValue = value;
                    bestOwner = *owners;
                }
            }
            count--;
            owners++;
        } while (count != 0);
    }

    if (bestValue == 999)
        return -2;

    Func_0807a1b4(bestOwner, index, state);
    Func_0807a458(bestOwner, index, state);
    Func_08079358(entry);
    return bestOwner;
}
