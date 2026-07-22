typedef signed int s32;
typedef unsigned char u8;
typedef unsigned int u32;

void Func_08003fa4(void *, s32, void *);
extern u8 Data_08031864;
extern u8 Value_08031864;
void Func_08031864(void);

s32 Func_0801a32c(u32 value, s32 unused, void *destination)
{
    void *source;
    switch (value) {
    case 1:
        source = &Data_08031864;
        break;
    case 2:
        source = &Value_08031864;
        break;
    case 3:
        source = Func_08031864;
        break;
    case 0:
    default:
        source = (void *)0x08031864;
        break;
    }
    Func_08003fa4(destination, 32, source);
    return 1;
}
