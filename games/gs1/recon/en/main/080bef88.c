#include "types.h"

struct BattleUnitRecord
{
    u8 pad00[76];
    s32 field_76_pad;
    s32 field_80;
    s32 field_84;
    s32 field_88;
};

struct BattleActionRecord
{
    s8 field_00;
    s8 field_01;
    u8 field_02;
    u8 field_03;
    u8 pad04[26];
    s8 field_30;
};

s32 Func_080772f8(s32);
s32 Func_080022ec(s32, s32);
s32 Func_080771a0(void);
s32 Func_08077080(s32);
s32 Func_08077178(s32, s32, s32, s32, s32);
s32 Func_080772b8(s32);
s32 Func_080bd3c8(s32);

void Func_080bef88(struct BattleActionRecord *action, struct BattleUnitRecord *unit, s32 actorId)
{
    s32 threshold;
    s32 rate;
    s32 stepSign;
    struct BattleActionRecord *entry;
    s32 kind;
    s32 kind2;
    s32 delta;
    s32 i;

    threshold = Func_080772f8(unit->field_76_pad);
    threshold = Func_080022ec(threshold << 16, 200);
    rate = threshold;

    if ((Func_080771a0() & 0xff) <= rate)
    {
        action->field_30 = 1;
    }
    else
    {
        entry = (struct BattleActionRecord *)Func_08077080(actorId);
        unit->field_80 = entry->field_02;
        unit->field_88 = 0;
        unit->field_76_pad = actorId;

        kind = entry->field_03;
        kind2 = kind;

        if (kind == 65 || kind == 68)
        {
            stepSign = 153;
        }
        else if (kind == 41 || kind == 42 || kind == 43 || kind == 44)
        {
            if (kind == 65 || kind == 41)
            {
                stepSign = 32;
            }
            else
            {
                stepSign = 64;
            }
        }
        else
        {
            stepSign = 0;
        }

        if (kind2 == 65 || kind2 == 41)
        {
            delta = 1;
        }
        else
        {
            delta = 2;
        }

        if ((Func_080771a0() & 0xff) < stepSign)
        {
            if (action->field_01 > 0)
            {
                for (i = 0; i < action->field_01; i++)
                {
                    ((u8 *)action)[30 + i] += delta;
                }
            }
        }
    }
}
