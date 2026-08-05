#include "types.h"

extern s32 Func_080770c0(s32 flag);
extern void Func_08009180(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 layer);

struct WindowRule {
    s16 flag;
    s16 enabled;
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
};

/* Open each enabled table window whose story flag is currently active. */
void Func_020017ec(void)
{
    const struct WindowRule *rule = (const struct WindowRule *)0x02009ca8;

    while (rule->flag != -1) {
        rule++;
        if (Func_080770c0(rule->flag) != 0 && rule->enabled != 0) {
            Func_08009180(rule->left, rule->top,
                          rule->right, rule->bottom, 1, 1);
        }
    }
}
