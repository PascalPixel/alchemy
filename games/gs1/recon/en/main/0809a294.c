#include "types.h"

#define RunBattleEffect11 Func_0809a294

extern void *Data_03001f30;

void Func_08097384(void);
void *Func_0809a3c4(s32, s32, s32, s32);
void Func_080030f8(s32);
void Func_08096bec(void *, s32, s32);
void Func_08009158(void *);
void Func_080f9010(s32);
void *Func_08096c80(s32);
void Func_08009098(void *, u32);
u32 Func_08004458(void);
void Func_080090d0(void *);
void Func_0809748c(void);

void RunBattleEffect11(void)
{
    void *origin = Data_03001f30;
    void *anchors[2];
    s32 position[3];
    s32 *pos;
    s32 index;

    Func_08097384();
    position[2] = *(s32 *)((s8 *)origin + 12);
    position[1] = *(s32 *)((s8 *)origin + 8) + 0x100000;
    position[0] = *(s32 *)((s8 *)origin + 4);
    anchors[0] = Func_0809a3c4(
        position[0] + 0x200000, position[1], position[2], 0x8000);
    anchors[1] = Func_0809a3c4(
        position[0] - 0x200000, position[1], position[2], 0);

    Func_080030f8(15);
    for (index = 0; index < 2; index++) {
        void *anchor = anchors[index];

        if (anchor != 0)
            Func_08096bec(anchor, 0x180000, *(u16 *)((s8 *)anchor + 6));
    }

    Func_08009158(anchors[0]);
    Func_080f9010(0x86);
    pos = position;
    for (index = 0; index < 24; index++) {
        void *object;
        s32 distance;
        s32 zero;

        pos[1] = *(s32 *)((s8 *)origin + 8) + 0x100000;
        pos[0] = *(s32 *)((s8 *)origin + 4);
        pos[2] = *(s32 *)((s8 *)origin + 12);
        object = Func_08096c80(0x11d);
        if (object != 0) {
            Func_08009098(object, 0x0809f0d4);
            *(s32 *)((s8 *)object + 0x34) = 0x20000;
            *(s32 *)((s8 *)object + 0x30) = Func_08004458() + 0x20000;
            zero = 0;
            *(s8 *)((s8 *)object + 0x55) = zero;
            distance = Func_08004458() * 24 + 0x80000;
            Func_08096bec(object, distance, Func_08004458());
        }
    }

    Func_080090d0(anchors[0]);
    Func_080090d0(anchors[1]);
    Func_0809748c();
}
