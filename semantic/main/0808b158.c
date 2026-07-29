typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Rule0808b158 {
    u16 selector_a;
    u16 selector_b;
    void *result;
};

extern s32 Func_0808a8d0(s32 arg0);

void *Func_0808b158(s32 arg0, s32 arg1)
{
    void *result;
    s32 alternate;
    struct Rule0808b158 *rule;
    u16 selector_b;
    u16 selector_a;
    u16 selector_a_raw;

    rule = (struct Rule0808b158 *)0x0809ddd8;
    result = (void *)0;
    alternate = Func_0808a8d0(arg0);
    selector_a_raw = rule->selector_a;
    selector_a = selector_a_raw;
    if ((s16)selector_a != -1) {
loop:
        if ((((u8 *)rule)[3] & 0x80) != 0) {
            if ((s16)selector_a == arg0)
                goto selector_b_check;
            goto next;
        }
        if ((s16)selector_a_raw == alternate) {
selector_b_check:
            selector_b = rule->selector_b;
            if (((selector_b & 0x7fff) == 0x7fff)
                || (((s32)(selector_b << 17) >> 17) == arg1)) {
                result = rule->result;
            } else {
                goto next;
            }
        } else {
next:
            rule++;
            selector_a = rule->selector_a;
            selector_a_raw = selector_a;
            if ((s16)selector_a != -1)
                goto loop;
        }
    }
    return result;
}
