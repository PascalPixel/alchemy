typedef unsigned char u8;
typedef signed int s32;

extern u8 **Func_08000148(s32 group, s32 slot);
extern void Func_080090d0(u8 *object);

/* Release the scene object's optional attachment and clear its publication. */
void Func_02005948(void)
{
    u8 **publication = Func_08000148(35, 4);
    u8 *state;
    u8 *object;

    if (publication == 0)
        return;

    state = *publication;
    object = *(u8 **)(state + 20);
    if (object == 0)
        return;

    Func_080090d0(object);
    *(u8 **)(state + 20) = 0;
}
