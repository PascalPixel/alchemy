typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

struct Object {
    u8 unknown_000[6];
    u16 current;
    u8 unknown_008[92];
    u16 target;
};

s32 Func_08092a74(struct Object *object)
{
    s32 delta = 0;

    if (object != 0) {
        s32 target = object->target;
        s32 current = object->current;
        delta = (s16)(target - current);
        if (delta != 0) {
            if (delta > 4096) {
                delta = 2048;
            }
            if (delta < -4096) {
                delta = -2048;
            }
            object->current = current + delta;
        }
    }
    return delta;
}
