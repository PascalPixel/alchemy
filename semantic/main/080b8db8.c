#include "types.h"
#include "motion_object.h"

struct BattleState_080b8db8 {
    u8 pad_00[0x38];
    s16 value;
};

struct Scratch_080b8db8 {
    u8 a;
    u8 b;
    u8 c;
    u8 d;
};

struct BattleState_080b8db8 *Func_08077008(s32);
struct BattleObjectSlot *Func_080b7dd0(s32);
void Func_08009080(struct MotionObject *, s32);
void Func_08015130(s32);
void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_080151c8(s32);
void Func_080b8ec4(s32);

void Func_080b8db8(s32 id, s32 amount, s32 announce, s32 preserve)
{
    volatile struct Scratch_080b8db8 scratch;
    struct BattleState_080b8db8 *state;

    if (preserve == 0) {
        scratch.a = 0;
        scratch.b = 0;
        scratch.c = 0;
        scratch.d = 0;
    }

    state = Func_08077008(id);
    state->value -= amount;
    if (state->value < 0)
        state->value = 0;

    Func_08009080(Func_080b7dd0(id)->object, 5);
    Func_08015130(0);
    Func_08015118();

    if ((u32)id <= 7) {
        if (announce != 0)
            Func_080151c8(0x823);
        Func_08015120(amount, 5);
        Func_08015120(id, 1);
        Func_080151c8(0x827);
    } else {
        if (announce != 0)
            Func_080151c8(0x822);
        Func_08015120(amount, 5);
        Func_08015120(id, 1);
        Func_080151c8(0x826);
        Func_08015120(id, 1);
    }

    Func_080b8ec4(id);

    if ((u32)id <= 7) {
        if (state->value <= 0) {
            Func_08015120(id, 1);
            Func_080151c8(0x825);
        }
    } else {
        if (state->value <= 0) {
            Func_08015120(id, 1);
            Func_080151c8(0x838);
        }
    }

    Func_08009080(Func_080b7dd0(id)->object, 1);
}
