typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

struct MapRule {
    s16 key;
    u16 condition;
    s16 required_flag;
    s16 result;
};

extern u8 Data_02000240[];
extern const struct MapRule Data_0809d9f0[];

s32 Func_080770c0(s32 flag);

void Func_0808b090(void)
{
    const struct MapRule *rule = Data_0809d9f0;
    s16 primary_key = *(s16 *)(Data_02000240 + 0x1c0);
    s16 condition = *(s16 *)(Data_02000240 + 0x1c2);
    s16 alternate_key = *(s16 *)(Data_02000240 + 0x1cc);
    s16 result = 18;

    while (rule->key != -1) {
        s32 key_matches;
        s32 condition_matches;

        if (rule->condition & 0x8000)
            key_matches = rule->key == primary_key;
        else
            key_matches = rule->key == alternate_key;

        condition_matches =
            ((rule->condition & 0x7fff) == 0x7fff) ||
            (((s32)(rule->condition << 17) >> 17) == condition);

        if (key_matches && condition_matches &&
            (rule->required_flag == -1 ||
             Func_080770c0(rule->required_flag) != 0)) {
            result = rule->result;
            break;
        }
        rule++;
    }

    *(s16 *)(Data_02000240 + 0x1f0) = result;
}
