/* NONMATCHING: complete 316-byte owner including switch table and own pool;
 * candidate 284, 112 differing halfwords, 80 aligned edits (2026-09-26).
 * Actor-25 event-table entries reference this callback; message 0x12ad is
 * the refusal response. Restored EventEnd and all three data bindings.
 * Three bounded hypotheses: volatile aggregate reads gave 332/316 and
 * 93 edits; a full actor-tail aggregate gave 340/316 and 90 edits; a signed
 * state pointer and shared update tail move the pool to the end but merge
 * the two forward arms. Signed loads still expand to ldrh plus shifts;
 * the reference keeps separate forward arms and a pool-loaded decrement.
 * The old 288-byte comparison omitted part of this owner's own pool. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const u8 Data_0200d8bc[];
extern const u8 Data_0200d858[];
extern const u8 *Data_0200e4d8[][4];

void KuupuappuHeya_RunActor25Response(void)
{
    struct FieldActor *actor;
    /* FAKEMATCH: explicit reloads model the observed shared-state reads. */
    volatile s16 *side;
    u32 facing;
    s32 half;
    s32 next;

    actor = Engine_ActorGet(25);
    facing = actor->facing & 0xf000;
    side = (volatile s16 *)&actor->unknown_64;
    half = *side >> 1;
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(25, 2);
    Engine_EventSetMessage(0x12ad);
    Engine_EventShowMessage(25, 0);
    Engine_ActorSetSpeed(25, 0x38000, 0x1c000);
    switch (*side) {
    case 4:
        if (facing > 0x2000 && facing < 0xa000) {
            Engine_ActorEnableActionCallback(25, Data_0200d8bc);
            next = 2;
        } else {
            Engine_ActorEnableActionCallback(25, Data_0200d858);
            next = 3;
        }
        break;
    case 0:
    case 2:
        if (facing > 0x2000 && facing < 0xa000) {
            Engine_ActorEnableActionCallback(25, Data_0200e4d8[half][*side]);
            next = *side - half * 2 + 1;
            break;
        }
        goto back;
    case 1:
    case 3:
        if (facing > 0x6000 && facing < 0xe000) {
            Engine_ActorEnableActionCallback(25, Data_0200e4d8[half][*side]);
            next = *side - half * 2 + 1;
            break;
        }
    back:
        Engine_ActorEnableActionCallback(25, Data_0200e4d8[half ^ 1][*side]);
        next = *side - half * 2 - 1;
        break;
    default:
        goto normalize;
    }
    *side = next;
normalize:
    *side &= 3;
    Engine_ActorStartAction(25);
    Engine_EventEnd();
}
