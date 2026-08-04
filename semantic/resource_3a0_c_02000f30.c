typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

u8 *Func_0808a080();
void Func_0808a018(); void Func_0808a020(); void Func_0808a010();
void Func_0808a098(); void Func_0808a0a0(); void Func_0808a0f0();
void Func_0808a138(); void Func_0808a148(); void Func_0808a170();
void Func_0808a180(); void Func_0808a5e8();
void Func_080000d0(); void Func_080000d8();
s32 Func_080770c0(); void Func_080770c8(); void Func_080f9010();
void Func_0200013c();

/*
 * resource_3a0 actor-19 scripted scene at 0x02000f30 (304 bytes).
 *
 * Complete saved-high-register owner through the interworking return at
 * 0x0200103c, alignment and eight referenced pool words through 0x0200105f.
 * The three object spawns differ only in their signed field44 value; every one
 * of the 21 BL sites remains explicit and in machine order.
 */
void Func_02000f30(void)
{
    u8 *actor;
    u8 *owner;

    Func_0808a018();
    Func_0808a098(19, (u8 *)0x020096a0);
    Func_080000d0((u8 *)0x02008359, 3200);
    Func_0808a0a0(19);
    Func_080f9010(124);

    Func_0200013c(0x00a80000, 0x00080000, 0x01380000,
                  0x00020001, 0, 0, 0x00020001, 0);
    Func_0200013c(0x00a80000, 0x00080000, 0x01380000,
                  0x00003333, 0, 0, 0x00020001, 0);
    Func_0200013c(0x00a80000, 0x00080000, 0x01380000,
                  (s32)0xffffcccd, 0, 0, 0x00020001, 0);

    Func_080000d8((u8 *)0x02008359);
    actor = Func_0808a080(19);
    owner = *(u8 **)(actor + 80);
    *(u16 *)(owner + 30) = 0x8000;
    Func_0808a0f0(21, 0x00a80000, 0x01380000);
    Func_0808a010(20);
    Func_0808a148(14, 19, 0);
    Func_0808a138(14, 2);
    Func_0808a010(10);
    Func_0808a170(0x17fd);

    if (Func_080770c0(0x203) != 0) {
        u8 *state = *(u8 **)0x03001ebc;
        *(u16 *)(state + 472) += 1;
    }

    Func_0808a180(14, 0);
    Func_080770c8(0x203);
    Func_0808a5e8();
    Func_0808a020();
}
