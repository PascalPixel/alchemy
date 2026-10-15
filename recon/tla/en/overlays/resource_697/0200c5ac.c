/* differ 14: the native builds the end marker 0xffff again for the test at the bottom of the loop; here the two tests share one register, which the loop pass then keeps in ip (the shared constant lives across the whole loop, so moving it pays). Tried: return or break after the found entry, the increment in the then-branch, continue. */
#include "../../../../../games/tla/src/scene/trial_road/trial_road.h"

struct TrapEntry {
    u16 actor;
    u16 flag;
};

void Task_Wait(s32 frames);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02005880(void);
void Func_02005888(void);
void Func_02005838(s32 a, s32 b, s32 c);
void Func_02004a94(s32 value);
void Func_02004bc8(void);
void Func_02004ce0(s32 a, s32 b, s32 c);

static __inline__ void Func_02005838_(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005838(a, b, c);
}

void Func_020045ac(const void *data, s32 id)
{
    const struct TrapEntry *entry = data;
    struct ObjectRuntime *object = Object_GetById(id);
    s32 flag;

    while (entry->actor != 0xffff) {
        if (entry->actor != id) {
            entry++;
            continue;
        }
        {
            flag = entry->flag;
            Func_02005880();
            if (!GameFlag_Test(flag + 1)) {
                Func_02005a68(185);
                Func_02005838_(0x10000, 0x40000, 0x10000);
                Func_02004a94(0);
                Task_Wait(8);
                Func_020057c8(object, 2);
                Func_02004ce0(-4, OBJECT_BYTE(object, 34), id);
                Func_02004a94(1);
                Task_Wait(16);
                Func_02005838_(-1, -1, 0xe666);
                Task_Wait(8);
                Object_SetPositionAndResetMotion(object, 0, 0, 0);
                Task_Wait(2);
                Func_02005a68(188);
                Func_02004bc8();
                Task_Wait(20);
                GameFlag_SetBit(flag);
                GameFlag_SetBit(flag + 1);
            }
            Func_02005888();
            break;
        }
    }
}
