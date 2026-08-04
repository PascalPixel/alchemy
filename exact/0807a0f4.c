#include "types.h"

extern u16 Data_02000240[];

struct OwnerState_0807a0f4 {
    u8 padding[280];
    u8 values[4];
};

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
    s32 result;
    u8 *owners;

    if (Func_08079338(entry) != 0)
        return -1;

    result = Func_080795fc();
    if (bestOwner < result) {
        s32 ownerOffset = 252;

        owners = (u8 *)Data_02000240 + ownerOffset * 2;
        count = result;
        do {
            u8 *ownerState = Func_08077394(*owners);

            if (((struct OwnerState_0807a0f4 *)ownerState)->values[index] <= 9 &&
                (ownerState += 280, 1)) {
                s32 value = 0;
                s32 i = 3;

                do {
                    u8 byte = *ownerState;
                    ownerState++;
                    value += byte;
                    i--;
                } while (i >= 0);

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

