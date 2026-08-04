#include "types.h"

struct Input_080ba584 {
    u8 primary_id;
    u8 padding_01;
    u8 secondary_id;
};

struct Work_080ba584 {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 one;
    s32 count;
    s32 mode;
    s32 unknown_1c;
    u8 padding_20[4];
    s16 members[24];
};

struct Motion_080ba584 {
    u8 padding_00[8];
    s32 x;
    u8 padding_0c[4];
    s32 z;
};

struct Slot_080ba584 {
    struct Motion_080ba584 *object;
};

struct Child_080ba584 {
    s16 value;
};

struct Record_080ba584 {
    u8 padding_00[40];
    struct Child_080ba584 *child;
};

extern s32 *Data_03001f00;

struct Slot_080ba584 *Func_080b7dd0(s32 id);
s32 Func_080044d0(s32 first, s32 second);
void Func_080030f8(s32 frames);
void Func_080c10e8(s32 first, s32 second);
void Func_080b9d34(void *input, struct Work_080ba584 *work);
void Func_08077008(s32 id);
struct Record_080ba584 *Func_080b7f70(
    struct Motion_080ba584 *object, s32 index);
s32 Func_08009260(s32 value, s32 second, s32 third);
void Func_080b82c4(s32 first, s32 second, s32 divisor, s32 initial_y);
void Func_08009088(struct Motion_080ba584 *object, s32 action);
void Func_080b8178(s32 id);
void Func_080b8000(s32 id);
void Func_080c9008(struct Work_080ba584 *work);
void Func_080bb938(void);

s32 Func_080ba584(struct Input_080ba584 *input, s32 flags)
{
    struct Work_080ba584 work;
    struct Input_080ba584 *saved_input;
    struct Motion_080ba584 *object;
    struct Record_080ba584 *record;
    s32 *facing;
    s32 angle;
    s32 adjusted;
    s32 facing_angle;
    s32 first_coordinate;
    s32 second_coordinate;
    s32 divisor;
    s32 scripted;

    facing = Data_03001f00;
    saved_input = input;
    object = Func_080b7dd0(saved_input->primary_id)->object;
    second_coordinate = object->z;
    first_coordinate = object->x;
    angle = (u16)Func_080044d0(first_coordinate, second_coordinate);
    adjusted = angle - 0x2000;
    if (saved_input->primary_id > 7)
        adjusted = angle + 0x6000;
    adjusted &= 0x7fff;
    facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

    if (*facing == facing_angle) {
        *facing = facing_angle;
        Func_080030f8(5);
    } else {
        *facing = facing_angle;
        Func_080030f8(20);
    }

    Func_080c10e8(0, 0);
    Func_080b9d34(saved_input, &work);
    Func_08077008(work.primary_id);
    Func_08077008(saved_input->secondary_id);

    scripted = flags & 2;
    record = Func_080b7f70(
        Func_080b7dd0(saved_input->primary_id)->object, 0);
    divisor = Func_08009260(record->child->value, 2, 1);
    Func_080b82c4(work.primary_id, saved_input->secondary_id, divisor, 0);
    Func_08009088(Func_080b7dd0(work.primary_id)->object, 16);
    Func_080b7dd0(saved_input->secondary_id);

    if (saved_input->secondary_id <= 7)
        work.secondary_is_low_id = 1;
    else
        work.secondary_is_low_id = 0;
    if (scripted != 0) {
        Func_080030f8(10);
        Func_080b8178(saved_input->secondary_id);
        Func_080030f8(2);
        Func_080030f8(4);
        Func_080030f8(10);
        Func_080b8000(saved_input->secondary_id);
    } else {
        Func_080c9008(&work);
        Func_080bb938();
        Func_080b8000(saved_input->secondary_id);
    }
    Func_080b8000(work.primary_id);
    return 0;
}
