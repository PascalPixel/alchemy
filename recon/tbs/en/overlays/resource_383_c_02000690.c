/* NONMATCHING: 344 of 288 bytes, 143 differing halfwords (2026-09-24). Hand-written
 * from the jump-table disassembly as a single-overlay unit source (Engine_*
 * names bind at their import veneers, listing offset + 0x8000). Control flow
 * and calls are right. Remaining: the reference re-reads the state halfword
 * with ldrsh for the switch and each table index, where this keeps a copy
 * in r1; its case tails share one strh of the new state before the & 3; the
 * range tests load 0x7ffe into r0. */
#include "TYPES.H"

u8 *Engine_ActorGet();
void Engine_EventBegin();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorSetSpeed();
void Engine_ActorEnableActionCallback();
void Engine_ActorStartAction();
void Engine_EventEnd();

struct Half {
    s16 v;
};

/* Turn actor 25 a quarter step from the way it faces and start its motion. */
void Local_02000690(void)
{
    u8 *actor = Engine_ActorGet(25);
    s32 dir = *(u16 *)(actor + 6) & 0xf000;
    struct Half *state = (struct Half *)(actor + 100);
    s32 half = state->v >> 1;

    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(25, 2);
    Engine_EventSetMessage(0x12ad);
    Engine_EventShowMessage(25, 0);
    Engine_ActorSetSpeed(25, 0x38000, 0x1c000);
    switch (state->v) {
    case 4:
        if (dir > 0x2000 && dir < 0xa000) {
            Engine_ActorEnableActionCallback(25, 0x200d8bc);
            { s32 next = 2; state->v = next; }
        } else {
            Engine_ActorEnableActionCallback(25, 0x200d858);
            { s32 next = 3; state->v = next; }
        }
        break;
    case 0:
    case 2:
        if (dir > 0x2000 && dir < 0xa000) {
            Engine_ActorEnableActionCallback(25, ((u32 *)0x0200e4d8)[half * 4 + state->v]);
            state->v = state->v - half * 2 + 1;
        } else {
            Engine_ActorEnableActionCallback(25, ((u32 *)0x0200e4d8)[(half ^ 1) * 4 + state->v]);
            state->v = state->v - half * 2 - 1;
        }
        break;
    case 1:
    case 3:
        if (dir > 0x6000 && dir < 0xe000) {
            Engine_ActorEnableActionCallback(25, ((u32 *)0x0200e4d8)[half * 4 + state->v]);
            state->v = state->v - half * 2 + 1;
        } else {
            Engine_ActorEnableActionCallback(25, ((u32 *)0x0200e4d8)[(half ^ 1) * 4 + state->v]);
            state->v = state->v - half * 2 - 1;
        }
        break;
    }
    state->v &= 3;
    Engine_ActorStartAction(25);
    Engine_EventEnd();
}
