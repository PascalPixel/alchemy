#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct Work;
struct Slot;

struct Globals_080174f8 {
    void *state;
    u8 padding4[0x54];
    void *control;
};

extern volatile struct Globals_080174f8 Data_03001e8c;

s32 Func_08018038(s32, s32);
struct Work *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08017248(s32, s32, s32, s32, s32);
struct Slot *Func_08016670(struct Work *, s32, s32);
void Func_08016418(struct Work *, s32);

void Func_080174f8(s32 argument)
{
    s32 index;
    s32 result;
    s32 one;
    s32 active_offset;
    struct Work *existing;
    struct Work *work;
    void *state;
    void *control;

    state = Data_03001e8c.state;
    control = Data_03001e8c.control;
    result = 0;
    FIELD(state, s8, 0xEA5) = 2;
    index = Func_08018038(argument, 1);
    one = 1;
    FIELD(state, s8, 0xEA5) = one;
    active_offset = 0xEB0 + index * 2;

    if (FIELD(state, u16, active_offset) != 0) {
        existing = FIELD(control, struct Work *, 0);
        if (existing != NULL) {
            goto use_existing;
        }
        {
            work = Func_080162d4(0, 15, 30, 6, 10);
            existing = work;
            FIELD(control, struct Work *, 0) = existing;
            Func_08017248(0, 15, 30, 6, one);
            FIELD(control, s32, 8) = result;
            goto have_work;
        }
use_existing:
        work = existing;
have_work:
        if (work != NULL) {
            result = (s32)Func_08016670(work, index, FIELD(control, s32, 8));
            FIELD(control, s32, 4) = result;
            FIELD(control, s32, 8) = 0;
            if (result == 0) {
                Func_08016418(work, one);
            }
        }
    }
}
