#include "types.h"

struct ObjectSlot_080babdc {
    void *object;
};

struct Runtime_080babdc {
    u8 padding_00[65];
    u8 mode;
};

extern struct Runtime_080babdc *Data_03001e74;

void *Func_08077008(s32 id);
struct ObjectSlot_080babdc *Func_080b7dd0(s32 id);
void Func_08009080(void *object, s32 mode);
void Func_080152b8(u16 *selection);
void Func_080ba918(void *object, s32 value);
void Func_080030f8(s32 frames);
s32 Func_080b6cd0(s32 id);
void Func_08015130(s32 mode);

void Func_080babdc(s32 id)
{
    u16 selection[2];
    s32 remaining;

    Func_08077008(id);
    Func_08009080(Func_080b7dd0(id)->object, 5);

    remaining = 1;
    do {
        selection[1] = 0xff;
        selection[0] = id;
        Func_080152b8(selection);
        Func_080ba918(Func_080b7dd0(id)->object, 7);
        Func_080030f8(2);

        selection[0] = id;
        Func_080152b8(selection);
        Func_080ba918(Func_080b7dd0(id)->object, Func_080b6cd0(id));
        Func_080030f8(2);
        remaining--;
    } while (remaining >= 0);

    Func_08015130(Data_03001e74->mode);
}
