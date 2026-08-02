typedef signed short s16;
typedef signed int s32;

/* Scene-two presentation script, including its two overlay-local setup hooks. */
extern s16 Data_02000240[];

extern void Func_02001df4();
extern void Func_0808a018();
extern s32 Func_02001ffc();
extern void Func_0808a170();
extern void Func_020008ec();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern void Func_0808a180();
extern void Func_02000950();
extern void Func_02002ba8();
extern void Func_0808a010();
extern void Func_0808a090();
extern void Func_020030e8();
extern void Func_02000970();
extern void Func_0808a2a0();
extern void Func_02002d84();
extern void Func_0808a200();
extern void Func_020020b8();
extern void Func_02002114();
extern void Func_0808a020();

void Func_02001424(s32 scene)
{
    s32 status;

    if (Data_02000240[225] == 2) {
        Func_02001df4(scene);
        return;
    }

    Func_0808a018();
    status = Func_02001ffc(scene, 2);

    if (status == 0) {
        Func_0808a170(0x20a2);
        Func_020008ec();
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x3d80000, -1, 0xe80000, 1);
        Func_0808a218();
        Func_0808a180(scene, 0);
        Func_02000950();
        Func_0808a180(scene, 0);
        Func_02002ba8(0, 0x438, 0x108);
        Func_0808a010(15);
        Func_0808a090(0, 0x18000, 0xc000);
        Func_020030e8(0, 0x438, 216);
        Func_020030e8(0, 0x428, 216);
        Func_02000970();
        Func_0808a2a0();
        Func_0808a210(-1, -1, -1, 0);
        Func_0808a180(scene, 0);
        Func_02002d84(0);
        Func_0808a200(0, 0);
        Func_020020b8(scene, 2);
    } else if (status == 1) {
        Func_0808a170(0x20a1);
        Func_0808a180(scene, 0);
    }

    Func_02002114(status, scene, 2);
    Func_0808a020();
}
