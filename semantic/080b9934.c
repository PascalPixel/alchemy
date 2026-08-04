#include "types.h"

struct BattleDecision_080b9934 {
    s16 owner;
    u16 unknown_02;
    u16 weight;
    s16 state;
    u16 action;
    s16 target;
    u8 unknown_0c[4];
};

void Func_080b90ac(void);
void Func_080b98b4(s32);
void Func_080770c8(s32);
void Func_080b8fd4(s32);
void Func_08015218(void);
s32 Func_080b920c(struct BattleDecision_080b9934 *);
s32 Func_080b90f8(void);
s32 Func_080b9724(struct BattleDecision_080b9934 *, s32);
s32 Func_080b60a0(void);
s32 Func_080b9324(struct BattleDecision_080b9934 *);
void Func_080b9470(struct BattleDecision_080b9934 *, s32);
u8 *Func_08077008(s32);
void Func_080770d0(s32);
void Func_080b7f9c(void);

s32 Func_080b9934(struct BattleDecision_080b9934 *decisions)
{
    u8 *battle = *(u8 **)0x03001e74;
    s32 count;
    s32 index;

    for (index = 0; index < 20; index++) {
        struct BattleDecision_080b9934 *entry =
            (struct BattleDecision_080b9934 *)(battle + 0x2ec) + index;

        entry->owner = 0xff;
        entry->weight = 0x8000;
    }

    Func_080b90ac();
    Func_080b98b4(8);
    Func_080770c8(0x16b);
    Func_080b8fd4(0);
    Func_08015218();

    if (battle[0x45] != 2) {
        count = Func_080b920c(decisions);
        if (count < 0)
            goto cleanup;
        if (count != 0 && decisions[0].state == 0x63 &&
            Func_080b90f8() == 0) {
            battle[0x45] = 2;
        }
    } else {
        count = 0;
    }

    if ((*(u8 **)0x03001e74)[0x44] != 0) {
        s32 added = Func_080b9724(decisions, count);

        if (Func_080b60a0() < 0 || added < 0) {
            count = -1;
            goto cleanup;
        }
        count += added;
    } else {
        count += Func_080b9324(&decisions[count]);
    }

    Func_080b9470(decisions, count);
    for (index = 0; index < count; index++) {
        s32 state = decisions[index].state;

        if (state == 3 || state == 7) {
            u8 *owner = Func_08077008(decisions[index].owner);

            owner[0x12b] = (state != 3) + 1;
        }
    }

cleanup:
    Func_080770d0(0x16b);
    Func_080b7f9c();
    **(s32 **)0x03001f00 = 0x2000;
    return count;
}
