typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern s32 Func_0808ba1c(s32 arg0);
extern void Func_080030f8(s32 arg0);

void Func_08092504(s32 arg0) {
    s32 base;
    u8 *ptr;
    volatile s32 saved;
    s32 i;

    base = Func_0808ba1c(arg0);
    if (base != 0 && *(u8 *)(base + 84) == 1) {
        ptr = *(u8 **)(base + 80);
        saved = ptr[36];
        for (i = 0; i <= 89; i++) {
            Func_080030f8(1);
            if (saved != ptr[36]) {
                break;
            }
        }
    }
}
