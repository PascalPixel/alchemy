typedef signed short s16;
typedef signed int s32;

/* Install result 33 in the first active type-0x8a registry record. */
struct RegistryRecord_020027dc {
    s32 state;
    s16 type;
    s16 reserved06;
    s32 result;
};

extern struct RegistryRecord_020027dc Data_0200e3f4[];

void Func_020027dc(void)
{
    struct RegistryRecord_020027dc *record = Data_0200e3f4;

    while (record->state != -1) {
        if (record->state == 2 && record->type == 0x8a) {
            record->state = 1;
            record->result = 33;
            break;
        }
        record++;
    }
}
