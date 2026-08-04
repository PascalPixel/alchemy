#include "types.h"

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

void Func_02000104(void *arg0)
{
    union Slot *s = (union Slot *)arg0;
    union Slot *obj;

    s[2].w += s[17].w;
    s[3].w += s[18].w;
    s[4].w += s[19].w;
    s[6].w += s[12].w;
    s[7].w += s[13].w;
    obj = (union Slot *)s[20].p;
    obj[7].h[1] += s[25].h[0];
}
