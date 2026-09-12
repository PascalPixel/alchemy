#include "types.h"

typedef struct StagedMoveTarget { s32 unk0; s32 unk4; s32 unk8; } StagedMoveTarget;
typedef struct StagedActorRecord { u8 filler0[0x28]; s16 *unk28; } StagedActorRecord;
typedef struct StagedActor {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    StagedActorRecord *unk50;
} StagedActor;

extern s32 Data_0200e190[];
extern s32 Data_0200e1d0[];
extern StagedActor *Func_02000342(StagedMoveTarget *, StagedActor *);
extern s32 Func_02006266(StagedActor *, StagedMoveTarget *);

s32 MapStagedActor_StopBlockedMotion(StagedActor *actor)
{
    StagedMoveTarget target;
    u32 direction_index;
    s32 direction_step;
    StagedActor *blocking_actor;

    direction_index = actor->unk6 >> 12;
    direction_step = Data_0200e190[direction_index];
    target.unk0 = actor->unk8 + (direction_step & 0xffff0000);
    target.unk4 = actor->unkC;
    direction_step = direction_step << 16;
    target.unk8 = actor->unk10 + direction_step;
    blocking_actor = Func_02000342(&target, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = *blocking_actor->unk50->unk28;
        s32 *p = Data_0200e1d0;

        do {
            if (blocking_kind == *p++) goto done;
            kind_index++;
        } while (kind_index <= 5);
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
    direction_step = Data_0200e190[direction_index];
    target.unk0 = actor->unk8 + (direction_step & 0xffff0000);
    target.unk4 = actor->unkC;
    direction_step = direction_step << 16;
    target.unk8 = actor->unk10 + direction_step;
    if (Func_02006266(actor, &target) > 0) {
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
done:
    return 0;
}
