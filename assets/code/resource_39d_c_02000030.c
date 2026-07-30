typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
typedef struct {
    u8 pad[9];
    u8 f0:2;
    u8 f1:2;
    u8 f2:4;
} Rec;

void Func_02000030(void *object, s32 value) {
    Rec *record;
    record = M2C_FIELD(object, Rec *, 0x50);
    record->f1 = value;
}
