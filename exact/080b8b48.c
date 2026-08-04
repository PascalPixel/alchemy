#include "types.h"

struct Input_080b8b48 {
    s16 primary_id;
    u8 padding02[8];
    u16 secondary_id;
};

struct Work_080b8b48 {
    u8 padding00[4];
    s32 secondary_is_low_id;
    s32 primary_id;
    u8 padding0c[8];
    s32 count;
    u8 padding18[4];
    s32 unknown1c;
    u8 padding20[4];
    s16 secondary_id;
    u8 padding26[46];
};

struct ObjectSlot_080b8b48 {
    void *object;
};

extern s32 *Data_03001f00;

void Func_080030f8(u32);
s32 Func_080b8808(u32);
void Func_08077008(s32);
u32 Func_08004458(void);
void Func_08015120(s32, s32);
void Func_080151c8(s32);
void Func_080b82c4(s32, s32, s32, s32);
struct ObjectSlot_080b8b48 *Func_080b7dd0(s32);
void Func_08009088(void *, s32);
void Func_080c9008(struct Work_080b8b48 *);
void Func_080b8000(s32);

s32 Func_080b8b48(struct Input_080b8b48 *input)
{
    struct Work_080b8b48 work;

    if (*Data_03001f00 == 0x2000) {
        *Data_03001f00 = 0x2000;
        Func_080030f8(10);
    } else {
        *Data_03001f00 = 0x2000;
        Func_080030f8(30);
    }

    work.primary_id = input->primary_id;
    if (Func_080b8808(work.primary_id) < 0)
        return -1;

    work.secondary_id = input->secondary_id;
    if (Func_080b8808(work.secondary_id) < 0)
        return -1;

    Func_08077008(work.primary_id);
    Func_08077008(work.secondary_id);
    Func_08004458();
    Func_08015120(work.primary_id, 1);
    Func_080151c8(0x814);
    Func_080b82c4(work.primary_id, work.secondary_id, 13, 0);
    Func_08009088(Func_080b7dd0(work.primary_id)->object, 16);
    Func_080b7dd0(work.secondary_id);

    work.count = 1;
    if ((u16)work.secondary_id <= 7)
        work.secondary_is_low_id = 1;
    else
        work.secondary_is_low_id = 0;
    work.unknown1c = 0;

    Func_080030f8(4);
    Func_080c9008(&work);
    Func_080b8000(work.secondary_id);
    Func_080b8000(work.primary_id);
    return 0;
}
