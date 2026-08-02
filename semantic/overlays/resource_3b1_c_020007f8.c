typedef unsigned char u8;
typedef unsigned short u16;

void Func_0200486c();
void Func_02004880();
int Func_080770c0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
int Func_0808a070();
void Func_0808a130();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();

extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x020007f8, 176 bytes including alignment and its
 * four-word pool. Flag 0x925 selects the actor-8 answer sequence; its answer
 * branch either performs the movement or advances the shared scene counter.
 * The sole return is at 0x02000894, and all 17 calls appear in machine order.
 */
void Func_020007f8(void)
{
    Func_0808a018();
    if (Func_080770c0(0x925) != 0) {
        Func_0808a130(8, 2);
        Func_0808a170(0x1e13);
        Func_0200486c(8);
        Func_0808a148(8, 0, 10);
        Func_0808a178(8, 0);
        if (Func_0808a070(0, 0) == 0) {
            Func_0808a010(40);
            Func_0200486c(8);
            Func_02004880(8, 0x3000);
            Func_0808a180(8, 0);
        } else {
            *(u16 *)(Data_03001ebc + 472) += 2;
            Func_0808a180(8, 0);
            Func_0808a1b8(8, 0x3000, 0);
        }
    } else {
        Func_0808a170(0x1d4e);
        Func_0808a180(8, 0);
    }
    Func_0808a020();
}
