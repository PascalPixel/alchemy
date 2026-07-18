typedef unsigned char u8;
typedef signed int s32;

struct Child_08091c44 {
    u8 padding[36];
    u8 value;
};

struct Object_08091c44 {
    u8 padding[80];
    struct Child_08091c44 *child;
    u8 active;
};

struct Object_08091c44 *Func_0808ba1c(s32);
void Func_080030f8(s32);

void Func_08091c44(s32 arg0, s32 value)
{
    struct Object_08091c44 *object = Func_0808ba1c(arg0);

    if (object != 0 && object->active == 1) {
        s32 index = 0;
        u8 *current = &object->child->value;

        while (index <= 89) {
            Func_080030f8(1);
            if (value != *current) {
                break;
            }
            index++;
        }
    }
}
