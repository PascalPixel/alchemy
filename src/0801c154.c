typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
struct Obj {
    u8 padding[4];
    s8 kind;
    u8 padding2;
    u16 index : 9;
    u16 rest : 7;
};
void Func_08003dec(s32 *arg0, s32 arg1);
void Func_0801c154(struct Obj *obj, s32 arg1, s32 arg2) {
    obj->index = arg1;
    obj->kind = arg2;
    Func_08003dec((s32 *) obj, 0xFC);
}
