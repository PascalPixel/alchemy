typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

void Func_02000104(union Slot *object) {
    u16 *record;

    object[2].w += object[17].w;
    object[3].w += object[18].w;
    object[4].w += object[19].w;
    object[6].w += object[12].w;
    object[7].w += object[13].w;
    record = (u16 *)object[20].p;
    record[15] += object[25].h[0];
}
