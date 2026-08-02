typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u16 Data_02001078[];
extern u8 Data_02000240[];
extern u8 Value_00006774;
extern u8 Value_00000952;

extern u8 *Func_08077394(s32);
extern void Func_08079ae8(s32);
extern void Func_08077428(s32);
extern void Func_08079374(void *);

void Func_0807a7a0(void)
{
    u16 *source = Data_02001078;
    s32 header;
    s32 magic;
    s32 index;

    header = *source;
    magic = (s32)&Value_00006774;
    source++;
    if (header != magic)
        return;

    index = 0;
    do {
        u16 *destination = (u16 *)(Func_08077394(index) + 216);
        s32 count = 14;

        do {
            *destination = *source;
            source++;
            destination++;
            count--;
        } while (count >= 0);
        Func_08079ae8(index);
        Func_08077428(index);
        index++;
    } while (index <= 3);

    {
        u8 *state = (u8 *)0x02000240;
        s32 offset = 136;

        offset <<= 2;
        *(u16 *)(state + offset) = *source++;
        offset += 2;
        *(u16 *)(state + offset) = *source++;
        {
            s32 tailOffset = 252;

            tailOffset <<= 1;
            *(u16 *)(state + tailOffset) = *source;
        }
        offset -= 40;
        *(u16 *)(state + offset) = source[1];
    }
    source = Data_02001078;
    *source = 0;
    Func_08079374(&Value_00000952);
}
