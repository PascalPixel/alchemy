#include "types.h"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_0200259e(s32);
extern struct Actor *Func_020025a6(s32);
extern void Func_0200265e(s32, s32);

void Func_02001090(void)
{
    struct Actor *current = Func_0200259e(0);
    struct Actor *other = Func_020025a6(14);

    if (current->f10 <= other->f10) {
        Func_0200265e(14, 1);
    }
}
