#include "types.h"

/* resource_3bc state-machine slot 4, complete 468-byte owner and pool. */

extern s16 Data_02000240[];

























extern void Func_0200499e(void);
extern void Func_02006b18(void);
extern s32 Func_02004bb4(s32 actor, s32 slot);
extern void Func_02006bde(s32 message);
extern void Func_02006c32(s32 x, s32 z);
extern void Func_02006c4c(s32 x, s32 y, s32 z, s32 mode);
extern void Func_02006c58(void);
extern void Func_02006b4e(s32 frames);
extern void Func_02006c1e(s32 actor, s32 mode);
extern void Func_02006c26(s32 actor, s32 mode);
extern void Func_020057b2(s32 actor, s32 x, s32 z);
extern void Func_02006ba8(s32 actor, s32 x, s32 z);
extern void Func_02006bdc(s32 actor, s32 x, s32 z);
extern void Func_02006c68(s32 actor, s32 speed, s32 frames);
extern void Func_02006cd4(void);
extern void Func_02006cae(s32 x, s32 y, s32 z, s32 mode);
extern void Func_02006be4(s32 actor, s32 x, s32 z);
extern u8 * Func_02006bda(s32 actor);
extern void Func_02006c00(s32 actor, s32 x, s32 z);
extern void Func_02006c50(s32 actor, s32 mode);
extern void Func_02006b02(void *record, s32 x, s32 z, s32 y);
extern void Func_02006b10(void *record);
extern void Func_02006c70(s32 actor, s32 mode);
extern void Func_02006b24(void *record, s32 x, s32 z, s32 y);
extern void Func_02006b32(void *record);
extern void Func_02006c92(s32 actor, s32 mode);
extern void Func_02006b44(void *record, s32 x, s32 z, s32 y);
extern void Func_02006b52(void *record);
extern void Func_02006cb2(s32 actor, s32 mode);
extern void Func_02006b64(void *record, s32 x, s32 z, s32 y);
extern void Func_02006b72(void *record);
extern void Func_02006cd2(s32 actor, s32 mode);
extern void Func_02006d2a(s32 actor, s32 mode);
extern void Func_02005a8c(s32 actor);
extern void Func_02006d68(s32 actor, s32 mode);
extern void Func_02004dd0(s32 actor, s32 slot);
extern void Func_02006d3e(s32 message);
extern void Func_02006d56(s32 actor, s32 mode);
extern void Func_02004e4c(s32 state, s32 actor, s32 slot);
extern void Func_02006cac(void);
void Func_020020f0(s32 actor)
{
    s32 state;
    s32 x;
    s32 z;
    s32 moved_x;
    s32 moved_z;
    u8 *record;

    if (Data_02000240[225] == 2) {
        Func_0200499e();
        return;
    }

    Func_02006b18();
    state = Func_02004bb4(actor, 4);

    if (state == 0) {
        Func_02006bde(0x20bf);
        Func_02006c32(0x30000, 0x6000);
        Func_02006c4c(0x3580000, -1, 0xa80000, 1);
        Func_02006c58();
        Func_02006b4e(30);
        Func_02006c1e(actor, 0);
        Func_02006c26(actor, 0);
        Func_020057b2(0, 0x330, 0xc8);
        Func_02006ba8(0, 0x10000, 0x8000);
        Func_02006bdc(0, 0x348, 0xc8);
        Func_02006c68(0, 0xc000, 20);
        Func_02006cd4();
        Func_02006cae(-1, -1, -1, 0);
        Func_02006be4(0, 0x8000, 0x4000);

        record = Func_02006bda(0);
        x = *(s32 *)(record + 8);
        z = *(s32 *)(record + 12);
        Func_02006c00(0, 0x8000, 0x4000);

        Func_02006c50(0, 10);
        moved_z = z + 0x60000;
        Func_02006b02(record, x, moved_z, *(s32 *)(record + 0x10));
        Func_02006b10(record);

        Func_02006c70(0, 14);
        Func_02006b24(record, moved_x, moved_z, *(s32 *)(record + 0x10));
        moved_x = x + 0x400000;
        Func_02006b32(record);

        Func_02006c92(0, 10);
        moved_z = z + 0x360000;
        Func_02006b44(record, moved_x, moved_z, *(s32 *)(record + 0x10));
        Func_02006b52(record);

        Func_02006cb2(0, 15);
        Func_02006b64(record, x + 0x300000, moved_z,
                      *(s32 *)(record + 0x10));
        Func_02006b72(record);

        Func_02006cd2(0, 12);
        Func_02006d2a(actor, 0);
        Func_02005a8c(0);
        Func_02006d68(0, 0);
        Func_02004dd0(actor, 4);
    } else if (state == 1) {
        Func_02006d3e(0x20be);
        Func_02006d56(actor, 0);
    }

    Func_02004e4c(state, actor, 4);
    Func_02006cac();
}
