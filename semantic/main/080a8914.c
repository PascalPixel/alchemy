typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015090(s32, s32, s32, s32);
s32 Func_08015098(s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
void Func_080150b8(s32);
void Func_080030f8(u32);
s32 Func_08077008(s32);
extern u8 Value_00000b0e[];

void Func_080a8914(s32 arg0, s32 arg1, s32 arg2) {
    s32 state;
    s32 option;
    u8 *textBase;
    s32 offset8;
    s32 offset16;
    s32 offset24;
    s32 amount;
    s32 mask;
    void *global;

    global = *(void **)0x03001F2C;
    state = Func_08077008(arg1);
    mask = 1;
    M2C_FIELD(M2C_FIELD(global, void **, 0x17C), s8 *, 5) = mask;
    mask += 255;
    option = arg2 & mask;
    if (option == 0) {
        Func_08015068(arg0, 0, 0, 0x80, 0x28);
    }
    Func_08015090(state, arg0, 0x28, 0);
    Func_08015080(M2C_FIELD(state, u8 *, 0x129) + 0x741, arg0, 0, 0x20);
    Func_08015090(0x080AF22C, arg0, 0x68, 0);
    Func_080150b8(0xF);
    Func_080150b0(M2C_FIELD(state, u8 *, 0xF), 2, arg0, 0x80, 0);
    Func_08015090(0x080AF234, arg0, 0x28, 0x10);
    offset16 = 0x10;
    Func_080150b0(M2C_FIELD(state, s16 *, 0x38), 4, arg0, 0x48, offset16);
    Func_080150b0(M2C_FIELD(state, s16 *, 0x34), 4, arg0, 0x70, offset16);
    Func_08015098(0x080AF230, arg0, 0x68, offset16);
    Func_08015090(0x080AF238, arg0, 0x28, 0x18);
    offset24 = 0x18;
    Func_080150b0(M2C_FIELD(state, s16 *, 0x3A), 4, arg0, 0x48, offset24);
    Func_080150b0(M2C_FIELD(state, s16 *, 0x36), 4, arg0, 0x70, offset24);
    Func_08015098(0x080AF230, arg0, 0x68, offset24);
    textBase = Value_00000b0e;
    Func_08015080((s32)textBase, arg0, 0x28, 8);
    amount = M2C_FIELD(state, s32 *, 0x124);
    offset8 = 8;
    Func_080150b0(amount, 7, arg0, 0x58, offset8);
    if (option == 0) {
        Func_080030f8(1);
        Func_08015068(arg0, 0x90, 0, 0xE0, 0x28);
    }
    Func_08015080((s32)(textBase - 23), arg0, 0x98, 0);
    Func_08015080((s32)(textBase - 22), arg0, 0x98, offset8);
    Func_08015080((s32)(textBase - 21), arg0, 0x98, offset16);
    Func_08015080((s32)(textBase - 20), arg0, 0x98, offset24);
    Func_080150b0(M2C_FIELD(state, u16 *, 0x3C), 3, arg0, 0xC8, 0);
    Func_080150b0(M2C_FIELD(state, u16 *, 0x3E), 3, arg0, 0xC8, 8);
    Func_080150b0(M2C_FIELD(state, u16 *, 0x40), 3, arg0, 0xC8, 0x10);
    Func_080150b0(M2C_FIELD(state, u8 *, 0x42), 3, arg0, 0xC8, 0x18);
}
