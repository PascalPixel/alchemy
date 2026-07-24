typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct SubQueueItem_08011b00 {
    u32 unk00;
    u16 unk04;
    u16 unk06;
    u16 unk08;
    u16 unk0A;
    u16 unk0C[16];
};

struct Queue_08011b00 {
    struct SubQueueItem_08011b00 slots[4];
    u16 count;
};

void *Func_080048f4(s32, s32);

void Func_08011b00(void) {
    struct Queue_08011b00 *base;
    struct SubQueueItem_08011b00 *ptr;
    u16 i;
    u16 j;

    base = (struct Queue_08011b00 *) Func_080048f4(28, sizeof(struct Queue_08011b00));
    ptr = base->slots;
    for (i = 0; i != 4; i++) {
        ptr->unk00 = 0;
        ptr->unk04 = 0;
        ptr->unk06 = 0;
        ptr->unk08 = 0;
        ptr->unk0A = 0;
        for (j = 0; j != 16; j++) {
            ptr->unk0C[j] = 0;
        }
        ptr++;
    }
    base->count = 0;
}
