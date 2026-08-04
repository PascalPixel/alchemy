#include "types.h"

struct SequenceRule_0808ace0 {
    s16 area;
    s16 subarea;
    u16 condition;
    s16 first_value;
};

s32 Func_080770c0(s32);
void Func_0808b25c(void);

/*
 * Select the first sequence rule matching the current area, optional subarea,
 * and event condition. Publish its seven sequential values, apply the rule's
 * one-slot shift flag, reset the sequence timing, and refresh the consumer.
 */
void Func_0808ace0(s32 enable_rules) {
    u8 *battle;
    const struct SequenceRule_0808ace0 *rule;
    s16 area;
    s16 subarea;
    s16 value;
    s32 shift_first;
    s32 index;

    battle = *(u8 **)0x03001EBC;
    area = *(s16 *)0x02000400;
    subarea = *(s16 *)0x02000402;
    value = 0;
    shift_first = 0;

    if (enable_rules != 0) {
        rule = (const struct SequenceRule_0808ace0 *)0x0809D170;
        while (rule->area != -1) {
            if (rule->area == area &&
                (rule->subarea == -1 || rule->subarea == subarea) &&
                (((rule->condition & 0x7FFF) == 0x7FFF) ||
                 Func_080770c0((s32)(s16)(rule->condition << 1) >> 1) == 0)) {
                value = rule->first_value;
                shift_first = (s16)((s8)(rule->condition >> 8) >> 7);
                break;
            }
            rule++;
        }
    }

    battle[0x1A0] = 0;
    for (index = 0; index < 7; index++) {
        battle[0x1A1 + index] = (u8)value;
        if (value != 0) {
            value++;
        }
    }
    if (shift_first != 0) {
        battle[0x1A0] = battle[0x1A1];
        battle[0x1A1] = 0;
    }

    *(s32 *)(battle + 0x1A8) = 0;
    *(s32 *)(battle + 0x1AC) = 0x100000;
    Func_0808b25c();
}
