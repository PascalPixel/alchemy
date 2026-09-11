#include "types.h"
#include "staged_actor.h"

#define Actor_PushPuzzleActor Func_02000db4

extern u32 Data_02009d3c[];
extern s16 Data_02000240[];
struct StagedActor *Func_02001e94(s32);
struct StagedActor *Func_02001b6e(s32, s32);
s32 Func_02001ea2(struct StagedActor *, s32 *);
void Func_02001e66(struct StagedActor *, s32);
void Func_02001e4e(s32);
void Func_02001ff4(s32);
void Func_02001ea4(struct StagedActor *, s32, s32, s32);
void Func_02001eb6(struct StagedActor *, s32, s32, s32);
void Func_02001ec4(struct StagedActor *);
void Func_02001eb2(struct StagedActor *, s32);
void Func_02001b44(void);
void Func_02001868(void);
void Func_02001e48(void);

void Actor_PushPuzzleActor(void)
{
    s32 pos[3];
    struct StagedActor *player;
    struct StagedActor *actor;
    u32 dir;
    s32 step;
    u32 offset;
    s32 zero;
    s32 rate;
    s32 scene;

    player = Func_02001e94(0);
    dir = player->direction_and_kind >> 12;
    step = Data_02009d3c[dir];
    actor = Func_02001b6e((player->x.parts.cell + (step >> 16)) >> 4,
                         (player->z.parts.cell + (s16)step) >> 4);
    if (actor != 0) {
        zero = 0;
        actor->transition_mode = 2;
        offset = Data_02009d3c[dir];
        pos[0] = actor->x.value + (offset & 0xffff0000);
        pos[1] = actor->y;
        pos[2] = actor->z.value + (offset << 16);
        if (Func_02001ea2(actor, pos) <= 0) {
            Func_02001e66(player, 8);
            rate = 0x3333;
            Func_02001e4e(15);
            Func_02001ff4(185);
            actor->move_rate_x = rate;
            actor->move_rate_z = rate;
            Func_02001ea4(actor, pos[0], pos[1], pos[2]);
            player->move_rate_x = rate;
            player->move_rate_z = rate;
            Func_02001eb6(player, pos[0], pos[1], pos[2]);
            Func_02001ec4(actor);
            actor->x.value = pos[0];
            actor->z.value = pos[2];
            actor->unknown_24 = zero;
            actor->unknown_2c = zero;
            Func_02001eb2(player, 1);
            scene = Data_02000240[224];
            if (scene == 35)
                Func_02001b44();
            else if (scene == 30)
                Func_02001868();
            else if (scene == 32)
                Func_02001e48();
        }
    }
}
