#include "types.h"

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;
typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;
typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

extern s32 Data_0200b15c[];
extern s32 Data_0200b19c[];
extern Ent *Func_02000342(Desc *, Ent *);
extern s32 Func_02003352(Ent *, Desc *);

#define StopBlockedActorMotion Func_020002a8
s32 StopBlockedActorMotion(Ent *actor)
{
    Desc destination;
    u32 direction_index;
    s32 step;
    Ent *blocking_actor;

    direction_index = actor->unk6 >> 12;
    step = Data_0200b15c[direction_index];
    destination.unk0 = actor->unk8 + (step & 0xffff0000);
    destination.unk4 = actor->unkC;
    step = step << 16;
    destination.unk8 = actor->unk10 + step;
    blocking_actor = Func_02000342(&destination, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = *blocking_actor->unk50->unk28;
        s32 *allowed_kinds = Data_0200b19c;

        do {
            if (blocking_kind == *allowed_kinds++) {
                goto done;
            }
            kind_index++;
        } while (kind_index <= 5);
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
    step = Data_0200b15c[direction_index];
    destination.unk0 = actor->unk8 + (step & 0xffff0000);
    destination.unk4 = actor->unkC;
    step = step << 16;
    destination.unk8 = actor->unk10 + step;
    if (Func_02003352(actor, &destination) > 0) {
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
done:
    return 0;
}
