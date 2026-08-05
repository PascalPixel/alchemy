#include "types.h"

/* Prepare service zero, set its +6 halfword, and run the follow-up sequence. */
struct SceneService_02000fcc {
    u16 unknown00[3];
    u16 value06;
};

extern void Func_02003308(void);
extern struct SceneService_02000fcc *Func_02003326(s32 index);
extern void Func_020033f2(s32 value);
extern void Func_020033b6(void);
extern void Func_020033c2(void);
extern void Func_02003390(s32 value);

void Func_02000fcc(void)
{
    struct SceneService_02000fcc *service;

    Func_02003308();
    service = Func_02003326(0);
    service->value06 = 0x4000;
    Func_020033f2(123);
    Func_020033b6();
    Func_020033c2();
    Func_02003390(1);
}
