#include "types.h"

#define StagedActor_ResetMotionIfBlockedAhead Func_020002a8

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;
typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;
typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

extern s32 Data_0200aef8[];
extern s32 Data_0200af38[];
extern Ent *Func_02000342(Desc *, Ent *);
extern s32 Func_02003076(Ent *, Desc *);

s32 StagedActor_ResetMotionIfBlockedAhead(Ent *actor)
{
    Desc probe;
    u32 dir;
    s32 step;
    Ent *target;

    dir = actor->unk6 >> 12;
    step = Data_0200aef8[dir];
    probe.unk0 = actor->unk8 + (step & 0xffff0000);
    probe.unk4 = actor->unkC;
    step = step << 16;
    probe.unk8 = actor->unk10 + step;
    target = Func_02000342(&probe, actor);
    if (target != 0) {
        u32 i = 0;
        s32 v = *target->unk50->unk28;
        s32 *p = Data_0200af38;

        do {
            if (v == *p++) {
                goto done;
            }
            i++;
        } while (i <= 5);
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
    step = Data_0200aef8[dir];
    probe.unk0 = actor->unk8 + (step & 0xffff0000);
    probe.unk4 = actor->unkC;
    step = step << 16;
    probe.unk8 = actor->unk10 + step;
    if (Func_02003076(actor, &probe) > 0) {
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
done:
    return 0;
}
