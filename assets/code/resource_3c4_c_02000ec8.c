typedef signed int s32;
typedef unsigned char u8;
struct Actor { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flags23; };
struct Actor *Func_02003fdc();
s32 Func_02000ec8(struct Actor *actor)
{
    struct Actor *reference;
    u8 *flags_ptr;
    u8 flags;
    reference = Func_02003fdc(0);
    flags_ptr = &actor->flags23;
    flags = *flags_ptr | 2;
    *flags_ptr = flags;
    if (reference->z < actor->z) {
        s32 limit = actor->z - reference->z;
        s32 adjusted;
        limit += 0x00040000;
        adjusted = actor->y;
        adjusted += limit;
        if (reference->y <= adjusted) {
            flags &= 0xfd;
            *flags_ptr = flags;
        }
    }
    return 0;
}
