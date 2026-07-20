typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_08077008(s32);
s32 Func_080b8808(s32);
void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_080151c8(s32);

s32 Func_080b8888(s16 *argument) {
    s32 id;
    s32 result;
    void *object;

    id = *argument;
    object = Func_08077008(id);
    if (Func_080b8808(id) < 0) {
        return -1;
    }
    result = 0;
    if (FIELD(object, s16 *, 0x38) <= 0) {
        return result;
    }
    Func_08015118();
    Func_08015120(id, 1);
    Func_080151c8(0x816);
    return 0;
}
