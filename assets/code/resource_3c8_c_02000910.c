typedef unsigned int u32;
typedef signed int s32;

struct Position {
    u32 filler00[2];
    s32 x;
    s32 y;
    s32 z;
};

extern struct Position *Func_020057c2(s32);
extern struct Position *Func_020057ca(s32);
extern void Func_020056e8(s32);

void Func_02000910(s32 group, s32 index)
{
    struct Position *first;
    struct Position *second;
    s32 value;

    first = Func_020057c2(group);
    second = Func_020057ca(index);
    if (first->z <= second->z) {
        value = first->x;
        first->x = second->x;
        second->x = value;

        value = first->y;
        first->y = second->y;
        second->y = value;

        value = first->z;
        first->z = second->z;
        second->z = value;
        Func_020056e8(1);
    }
}
