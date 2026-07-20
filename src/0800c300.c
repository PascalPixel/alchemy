typedef unsigned char u8;
typedef signed int s32;
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_0800ba30(void *, s32);

void Func_0800c300(void *object, s32 argument) {
    void **items;
    s32 count;
    void *item;

    if (object != 0) {
        switch (FIELD(object, u8 *, 0x54) & 0xF) {
        case 1:
            Func_0800ba30(FIELD(object, void **, 0x50), argument);
            break;
        case 2:
            items = FIELD(object, void ***, 0x50);
            count = 3;
            do {
                item = *items++;
                if (item != 0) {
                    Func_0800ba30(item, argument);
                }
                count--;
            } while (count >= 0);
            break;
        }
    }
}
