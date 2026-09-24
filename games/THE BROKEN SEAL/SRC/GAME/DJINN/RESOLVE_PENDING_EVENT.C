#include "TYPES.H"
#include "SYSTEM.H"

/* Party-state word 0x234 holds a pending Djinn event: the top nibble is the
   kind and the low bits the game flag. Flags 300-380 are the eighty Djinn,
   twenty per element. */
struct DjinnEventState {
    u8 unknown_000[0x234];
    s16 pending;
    s16 result;
};

struct MapActor {
    s16 id;
    s16 event;
};

extern struct DjinnEventState Data_02000240;

s32 Math_Div(s32, s32);
s32 Math_Mod(s32, s32);
void GameFlag_SetBitFar(s32);
struct MapActor *BattleAction_FindDescriptor(s32 id);
void BattleFx_RunPageEffectForSlot(s32 actor, s32 element, s32 index);

void Djinn_ResolvePendingEvent(s32 capture)
{
    struct DjinnEventState *st = &Data_02000240;
    s16 *pending = &st->pending;
    s32 kind = *pending & 0xf000;
    s32 flag = (u16)*pending & 0xfff;

    if (capture == 0) {
        if (kind == 0) {
            flag &= 0x7ff;
            if ((u32)(flag - 300) > 80)
                return;
            if (st->result > 0 && st->result != 999)
                return;
            GameFlag_SetBitFar(flag - 172);
            *pending = kind;
        } else if (kind == 0x1000) {
            if (st->result == 1)
                GameFlag_SetBitFar(flag);
            *pending = capture;
        }
        return;
    }
    if (kind == 0) {
        flag &= 0x7ff;
        if ((u32)(flag - 300) <= 80) {
            flag &= 0x7ff;
            if (st->result > 0) {
                s32 djinn = flag - 300;
                s32 element = Math_Div(djinn, 20);
                s32 index = Math_Mod(djinn, 20);
                s32 actor;

                for (actor = 8; actor <= 65; actor++) {
                    struct MapActor *found = BattleAction_FindDescriptor(actor);
                    if (found != 0 && found->event - 48 == flag - 300) {
                        WaitFrames(40);
                        BattleFx_RunPageEffectForSlot(actor, element, index);
                        break;
                    }
                }
            }
        }
    }
    Data_02000240.pending = 0;
}
