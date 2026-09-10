#include "types.h"

#define SceneActor_TrackOriginColumnForSlot Func_0200152c
#define SceneActor_RunWhenActor9AtTile45x43 Func_02001c28
#define ActorPresentation_PlaceActorFourteenOnActorNine Func_02001e94
#define SceneActor_PassActorNinePositionWithId107 Func_02001ec0
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern u8 Data_03001ebc[];
extern s32 Data_0200af74[];

s32 Func_02003362();
s32 Func_020033bc();
s32 Func_020033c4();
s32 Func_020033ce();
s32 Func_020033d6();
void Func_02003404();
s32 *Func_02003a7a(s32);
void Func_02003ac8(s32, s32, s32);
void Func_02003b86(s32);
void Func_02003ae4(s32, s32, s32);
void Func_02003b10(s32, s32, s32);
void Func_02003b1e(s32);
s32 *Func_0200416e(s32);
void Func_02003816(void);
struct Actor *Func_020043da(s32);
struct Actor *Func_020043e2(s32);
struct Frame *Func_02004406(s32);
void Func_020044be(s32, s32, s32);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    return f(a0);
}

void Func_02000e50(void)
{
    extern u8 Data_02000240[];

    s32 dst;
    s32 src;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    u8 *work;

    work = *(volatile s32 *)Data_03001ebc;
    if (Value1(Func_02003362, 0x9ca) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)Data_02000240 + off24a) != 15) {
            idx = *(s16 *)((s32)work + 0x16c);
            dst = Value1(Func_020033bc, 15);
            src = Value1(Func_020033c4, 0);
            *(volatile s32 *)(dst + 48) = *(volatile s32 *)(src + 48);
            dst = Value1(Func_020033ce, 15);
            src = Value1(Func_020033d6, 0);
            *(volatile s32 *)(dst + 52) = *(volatile s32 *)(src + 48);
            idx -= 30;
            tbl = 0x0200a808;
            idx <<= 3;
            idx4 = idx + 4;
            Func_02003404(15, *(volatile s32 *)(tbl + idx), *(volatile s32 *)(tbl + idx4));
        }
    }
}

void SceneActor_TrackOriginColumnForSlot(s32 no)
{
    extern s16 Data_02000240[];

    s32 *pos = Func_02003a7a(0);
    s32 col = pos[2] / 0x100000;   /* +8  */
    s32 row = pos[4] / 0x100000;      /* +16 */
    s32 slot = no + 10;

    if (Data_02000240[293] == slot) return;
    if (col == Data_0200af74[no]) return;

    Func_02003ac8(slot, 0x48000, 0x24000);
    Func_02003b86(188);
    Func_02003ae4(slot, (col << 4) + 8, 360);

    Data_0200af74[no] = col;

    if (row <= 22) {
        Func_02003b10(0, 0, 8);
    }
    Func_02003b1e(0);
}

void SceneActor_RunWhenActor9AtTile45x43(void)
{
    s32 *slot = Func_0200416e(9);
    s32 x = slot[2] / 0x100000;
    s32 z = slot[4] / 0x100000;

    if (x == 45 && z == 43) {
        Func_02003816();
    }
}

void ActorPresentation_PlaceActorFourteenOnActorNine(void)
{
    struct Actor *target = Func_020043da(14);
    struct Actor *source = Func_020043e2(9);

    target->f0c = 0x200000;
    target->f08 = source->f08;
    target->f10 = source->f10 + 0x10000;
}

void SceneActor_PassActorNinePositionWithId107(void)
{
    struct Frame *frame = Func_02004406(9);

    Func_020044be(107, frame->f08, frame->f10 + 0x10000);
}
