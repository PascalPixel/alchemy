typedef signed int s32;
typedef unsigned int u32;

extern u32 Data_0200c41c;

extern void Func_02003c86(s32);
extern void Func_02003c9a(s32);

void Func_02000178(void)
{
    s32 i;

    Func_02003c86(10);
    i = 0;
    if (Data_0200c41c != 22) {
        do {
            Func_02003c9a(1);
            i++;
            if (i > 119) {
                break;
            }
        } while (Data_0200c41c != 22);
    }
}
