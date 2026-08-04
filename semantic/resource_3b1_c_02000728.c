typedef unsigned char u8;
typedef unsigned short u16;

void Func_0200486c();
int Func_080770c0();
void Func_0808a018();
void Func_0808a020();
void Func_0808a110();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();

extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x02000728, 208 bytes including its nine-word pool.
 * Four ordered story-flag arms select the scene script; the 0x921 arm also
 * publishes a halfword state when 0x925 is clear and 0x924 is set. The sole
 * return is at 0x020007d2, and all 19 calls appear in machine order.
 */
void Func_02000728(void)
{
    Func_0808a018();
    if (Func_080770c0(0x928) != 0) {
        Func_0808a170(0x1eb2);
        Func_0200486c(8);
        Func_0808a1b8(8, 0xd000, 60);
        Func_0808a110(8, 4);
        Func_0200486c(8);
        Func_0808a110(8, 3);
    } else if (Func_080770c0(0x925) != 0) {
        Func_0808a170(0x1e06);
        Func_0808a180(8, 0);
    } else if (Func_080770c0(0x921) != 0) {
        Func_0808a170(0x1dcd);
        Func_0808a180(8, 0);
        if (Func_080770c0(0x925) == 0 && Func_080770c0(0x924) != 0)
            *(u16 *)(Data_03001ebc + 370) = 1;
    } else {
        Func_0808a170(0x1d30);
        Func_0808a180(8, 0);
    }
    Func_0808a020();
}
