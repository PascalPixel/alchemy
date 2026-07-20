typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080030f8(s32);

void Func_0800c4ec(void *object) {
    s32 count;

    count = 0;
    if (*(s32 *)((FIELD(object, s16 *, 4) * 4) + FIELD(object, s32 *, 0)) != 0x10) {
        do {
            Func_080030f8(1);
            count++;
            if (count > 0x12B) {
                break;
            }
        } while (*(s32 *)((FIELD(object, s16 *, 4) * 4) + FIELD(object, s32 *, 0)) != 0x10);
    }
}
