typedef unsigned char u8;
typedef signed int s32;

struct State_080792fc {
    u8 padding[15];
    u8 count;
};

struct State_080792fc *Func_08077394(s32);
s32 Func_0807905c(s32, u8 *);
void Func_08077428(s32);

void Func_080792fc(s32 owner, s32 target) {
    u8 temporary[16];
    s32 count = Func_08077394(owner)->count;

    while (count < target) {
        Func_0807905c(owner, temporary);
        count++;
    }
    Func_08077428(owner);
}
