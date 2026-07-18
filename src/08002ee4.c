typedef unsigned int u32;

extern volatile u32 Data_03001c94;
void Func_080030f8(u32);

void Func_08002ee4(void)
{
    for (;;) {
        (void)Data_03001c94;
        Func_080030f8(1);
    }
}
