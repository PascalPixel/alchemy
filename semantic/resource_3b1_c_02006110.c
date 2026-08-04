typedef unsigned char u8;
typedef signed int s32;

int Func_02004fa8();
void Func_0200486c();
void Func_02004880();
void Func_02004890();
void Func_020048e8();
void Func_020063ac(u8 *, u8 *);
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a1b8();
void Func_0808a248();
void Func_0808a360();
void Func_0808a368();
void Func_0808a370();

extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x02006110, 668 bytes including its seven-word
 * trailing pool. It stages four scene actors, switches the scene state at
 * +448 from 0x201 to 0x202, then closes with two paired flag-array scans.
 * The sole return is at 0x0200638e. All 63 BL sites across 23 resolved targets
 * appear below in machine order.
 */
void Func_02006110(void)
{
    int actor0;
    int actor1;
    int actor2;
    int actor3;

    actor0 = Func_02004fa8(0, 0);
    actor1 = Func_02004fa8(1, 0);
    actor2 = Func_02004fa8(2, 0);
    actor3 = Func_02004fa8(3, 0);
    Func_0808a018();
    Func_020048e8(10, 0, 0);
    Func_02004890((void *)8, 472, 0x90, 0x5000);
    Func_02004890((void *)27, 0x198, 0x8e, 0x3000);
    *(s32 *)(Data_03001ebc + 448) = 0x201;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);
    Func_0808a138(27, 1);
    Func_0808a170(0x1f78);
    Func_0200486c(27);
    Func_0808a130(actor0, 2);
    Func_0808a130(actor1, 2);
    Func_0808a130(actor2, 2);
    Func_0808a138(actor3, 2);
    Func_0808a010(20);
    Func_0808a1b8(actor0, 0, 0);
    Func_0808a1b8(actor1, 0x8000, 0);
    Func_0808a1b8(actor2, 0, 0);
    Func_0808a1b8(actor3, 0x8000, 40);
    Func_0808a090(actor0, 0x10000, 0x8000);
    Func_0808a090(actor1, 0x10000, 0x8000);
    Func_0808a090(actor2, 0x10000, 0x8000);
    Func_0808a090(actor3, 0x10000, 0x8000);
    Func_0808a0c8(actor0, 0x1d6, 0xac);
    Func_0808a0c8(actor1, 0x19a, 0xac);
    Func_0808a0c8(actor2, 0x1d6, 0xcc);
    Func_0808a0d0(actor3, 0x19a, 0xcc);
    Func_0808a100(actor0, 1);
    Func_0808a100(actor1, 1);
    Func_0808a100(actor2, 1);
    Func_0808a1b8(actor1, 0xd000, 0);
    Func_0808a1b8(actor0, 0xb000, 0);
    Func_0808a1b8(actor3, 0xd000, 0);
    Func_0808a1b8(actor2, 0xb000, 20);
    Func_0808a138(27, 1);
    Func_0200486c(27);
    Func_0808a100(actor0, 3);
    Func_0808a100(actor1, 3);
    Func_0808a100(actor2, 3);
    Func_0808a110(actor3, 3);
    Func_0200486c(27);
    Func_0808a100(actor0, 3);
    Func_0808a100(actor1, 3);
    Func_0808a100(actor2, 3);
    Func_0808a110(actor3, 3);
    Func_0808a1b8(27, 0, 0);
    Func_02004880(0, 0x8000);
    Func_0808a110(0, 3);
    Func_0808a110(27, 3);
    Func_0808a090(27, 0x10000, 0x8000);
    Func_0808a0d0(27, 0x198, 0x84);
    Func_0808a0d0(27, 0x1bc, 0x84);
    Func_0808a0f0(27, 0, 0);
    *(s32 *)(Data_03001ebc + 448) = 0x202;
    Func_0808a368();
    Func_0808a370();
    Func_020063ac((u8 *)0x92c, (u8 *)0x935);
    Func_020063ac((u8 *)0x917, (u8 *)0x990);
    Func_080770d0(0x8a0);
    Func_0808a248(10);
}
