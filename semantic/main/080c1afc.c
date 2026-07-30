typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Candidate_080c1afc {
    s16 record_id;
    s16 score;
};

struct Owner_080c1afc {
    u8 padding00[0x0F];
    u8 value;
};

typedef char Candidate_080c1afc_size[
    sizeof(struct Candidate_080c1afc) == 4 ? 1 : -1
];
typedef char Owner_080c1afc_size[
    sizeof(struct Owner_080c1afc) == 0x10 ? 1 : -1
];

s32 Func_080022ec(s32, s32);
void Func_08002df0(void *);
u32 Func_08004458(void);
struct Candidate_080c1afc *Func_08004970(s32);
struct Owner_080c1afc *Func_08077008(s32);
void Func_080770d0(s32);
u8 Func_080770e0(s32);
const void *Func_08077198(s32);
s32 Func_080b6a60(u16 *);
s32 Func_080c1a34(s32);

s16 Func_080c1afc(s32 *difference)
{
    static const u16 *const flags = (const u16 *)0x080C73F8;
    struct Candidate_080c1afc *candidates;
    struct Owner_080c1afc *owner;
    u16 owners[8];
    s32 owner_count;
    s32 sum;
    s32 level;
    s32 index;
    s32 record_id;
    s32 score;
    s32 minimum;
    s32 selected;
    s32 eligible;
    s16 result;

    eligible = 0;
    candidates = Func_08004970(0x80);

    sum = 0;
    owner_count = Func_080b6a60(owners);
    if (owner_count > 0) {
        for (index = 0; index < owner_count; index++) {
            owner = Func_08077008(owners[index]);
            sum += owner->value;
        }
    }

    level = Func_080022ec(sum, owner_count);
    level += (s8)Func_080770e0(0x3F8);
    if (level <= 0)
        level = 1;
    if (level > 99)
        level = 99;

    for (index = 0; index < 32; index++)
        candidates[index].score = -1;

    for (index = 0; index < 20; index++) {
        Func_08077198(flags[index]);
        Func_080770d0(flags[index] + 0x600);
    }

    for (record_id = 0; record_id <= 0x17B; record_id++) {
        score = Func_080c1a34(record_id);
        if (score >= 0 && score <= level + 3) {
            minimum = 0x3E7;
            selected = -1;
            for (index = 0; index < 32; index++) {
                if (candidates[index].score < minimum) {
                    minimum = candidates[index].score;
                    selected = index;
                }
            }
            if (selected >= 0) {
                candidates[selected].score = score;
                candidates[selected].record_id = record_id;
                eligible++;
            }
        }
    }

    if (eligible > 32)
        eligible = 32;

    if (eligible != 0) {
        selected = ((u32)eligible * Func_08004458()) >> 16;
        result = candidates[selected].record_id;
        *difference = level - candidates[selected].score;
    } else {
        *difference = 0;
        result = 1;
    }

    Func_08002df0(candidates);
    return result;
}
