#include "types.h"

/* Scene-three presentation script and its longer two-wave actor staging. */
extern s16 Data_02000240[];

extern void Func_02001df4();
extern void Func_0808a018();
extern s32 Func_02001ffc();
extern void Func_0808a170();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern void Func_0808a010();
extern void Func_0808a180();
extern void Func_02002ba8();
extern void Func_0808a090();
extern void Func_020030e8();
extern void Func_02000310();
extern void Func_0808a100();
extern void Func_0808a0d0();
extern void Func_0808a1b8();
extern void Func_02002d84();
extern void Func_0808a200();
extern void Func_0808a0f0();
extern void Func_020020b8();
extern void Func_02002114();
extern void Func_0808a020();

void Func_02001538(s32 scene)
{
    s32 status;

    if (Data_02000240[225] == 2) {
        Func_02001df4(scene);
        return;
    }

    Func_0808a018();
    status = Func_02001ffc(scene, 3);

    if (status == 0) {
        Func_0808a170(0x20a6);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x2f00000, -1, 0xc00000, 1);
        Func_0808a218();
        Func_0808a010(60);
        Func_0808a208(0x10000, 0x2000);
        Func_0808a210(0x2f00000, -1, 0xe00000, 1);
        Func_0808a218();
        Func_0808a180(scene, 0);
        Func_02002ba8(0, 0x358, 0x108);
        Func_0808a010(10);
        Func_0808a090(0, 0x18000, 0xc000);
        Func_020030e8(0, 0x358, 0x108);
        Func_020030e8(0, 0x358, 232);
        Func_0808a180(scene, 0);
        Func_020030e8(0, 0x348, 232);
        Func_0808a010(10);
        Func_02000310(33, -64, 0);
        Func_0808a210(0x2f00000, -1, 0xd80000, 1);
        Func_0808a100(0, 1);
        Func_0808a010(10);
        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0d0(0, 0x2f8, 232);
        Func_0808a010(10);
        Func_0808a1b8(0, 0x4000, 30);
        Func_0808a180(scene, 0);
        Func_02002d84(0);
        Func_0808a200(0, 0);
        Func_0808a0f0(33, 0x3480000, 0xe80000);
        Func_020020b8(scene, 3);
    } else if (status == 1) {
        Func_0808a170(0x20a5);
        Func_0808a180(scene, 0);
    }

    Func_02002114(status, scene, 3);
    Func_0808a020();
}
