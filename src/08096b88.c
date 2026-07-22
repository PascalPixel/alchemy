typedef signed int s32;
typedef unsigned char u8;

extern volatile s32 Data_03001e40;
s32 Func_08002304(s32, s32) __attribute__((const));

void Func_08096b88(void *owner)
{
    s32 state;
    void *target;
    s32 initial_count;
    s32 count;
    void **entry;
    void *current;
    volatile s32 *global;
    s32 value;

    state = *(u8 *)((u8 *)owner + 84);
    if (state == 1) {
        target = *(void **)((u8 *)owner + 80);
        if (target != 0 && (*(u8 *)((u8 *)target + 29) & state) == 0) {
            initial_count = *(u8 *)((u8 *)target + 39);
            if (initial_count != 0) {
                global = &Data_03001e40;
                entry = (void **)((u8 *)target + 40);
                count = initial_count;
                do {
                    current = *entry++;
                    value = Func_08002304(*global, 6);
                    count--;
                    *(u8 *)((u8 *)current + 5) = value;
                } while (count != 0);
            }
            *(u8 *)((u8 *)target + 37) = 1;
        }
    }
}
