typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[94];
    s16 counter;
};

extern s32 Func_02003bec(void);
extern s32 Func_02003bfa(void);

s32 Func_020000ac(struct Object *object)
{
    s32 counter = *(u16 *)&object->counter;
    s32 loaded = object->counter;
    s32 amount;

    if (loaded == 0) {
        object->x += (u32)(Func_02003bec() << 15) >> 16;
        amount = (u32)(Func_02003bfa() * 80) >> 16;
        object->counter = amount;
        if (amount == 0) {
            goto done;
        }
        counter = amount;
    }
    object->counter = counter - 1;
done:
    return 1;
}
