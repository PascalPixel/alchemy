/* NONMATCHING: 168 of 168 bytes, 2 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 2 halfwords: the reference sets r1 = 5 before loading the count into r0 for Main_08015120 The -fsched-verbose=5 sched2 dump shows both argument insns ready together after the Main_08015118 call; the ldrh wins on priority (load latency), so the reference must reach sched2 with the count load not yet ready or the r1 store earlier in RTL. Direct, cast, CallN, reversed-parameter inline wrappers, volatile and typed-prototype spellings all keep 2. */
#include "TYPES.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    u16 counts[512];
    s32 words[256];
};

struct LobbyActor {
    u8 unknown_00[6];
    u16 facing;
};

struct LobbyActor *Engine_ActorGet(s32 actor);
void Engine_EventBegin(void);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_EventSetMessage(s32 message);
void Main_08015118(void);
void Main_08015120(s32 value, s32 digits);
s32 Engine_EventShowMessage(s32 actor, s32 mode);

extern union GameStateRows gGameStateRows;
extern u8 Data_0000297b[], Data_0000297d[], Data_00002988[], Data_00002989[];

static __inline__ void Call2(void (*f)(s32, s32), s32 a0, s32 a1)
{
    f(a0, a1);
}

s32 Local_02000d68(s32 actor)
{
    u32 facing = Engine_ActorGet(0)->facing;
    u16 *count;
    s32 base;
    s32 message;

    Engine_EventBegin();
    Engine_ActorFaceActor(actor, gGameStateRows.words[125], 0);
    if ((u32)(facing - 0xa001) <= 0x3ffe) {
        base = (s32)Data_0000297b;
        count = &gGameStateRows.counts[342];
        if (*count == 0) {
            message = (s32)Data_00002988;
            goto say;
        }
    } else {
        base = (s32)Data_0000297d;
        count = &gGameStateRows.counts[345];
        if (*count == 0)
            goto none;
    }
    Main_08015118();
    Call2(Main_08015120, *count, 5);
    message = base + 1;
say:
    Engine_EventSetMessage(message);
    return Engine_EventShowMessage(actor, 0);
none:
    Engine_EventSetMessage((s32)Data_00002989);
    return Engine_EventShowMessage(actor, 0);
}
