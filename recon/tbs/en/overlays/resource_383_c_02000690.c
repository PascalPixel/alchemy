/* NONMATCHING: 340 of 288 bytes, 88 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: through a pointer to the side halfword, the switch value is loaded once as ldrh and shared with the table index (the reference reloads it with ldrsh in each case) and the constant stores go to the pool; through the actor struct the loads match but the actor stays live in r5 instead of the field address. The reference also puts its literal pool at the end (this draft dumps one after case 4). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct Side {
    s16 value;
};

struct Turner {
    u8 unknown_00[0x64];
    struct Side side;
};

extern const u8 Data_0200d8bc[];
extern const u8 Data_0200d858[];
extern const u8 *Data_0200e4d8[][4];

void Func_02000690(void)
{
    struct FieldActor *actor;
    struct Side *side;
    u32 facing;
    s32 half;

    actor = Engine_ActorGet(25);
    facing = actor->facing & 0xf000;
    side = &((struct Turner *)actor)->side;
    half = side->value >> 1;
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(25, 2);
    Engine_EventSetMessage(0x12ad);
    Engine_EventShowMessage(25, 0);
    Engine_ActorSetSpeed(25, 0x38000, 0x1c000);
    switch (side->value) {
    case 4:
        if (facing > 0x2000 && facing < 0xa000) {
            Engine_ActorEnableActionCallback(25, Data_0200d8bc);
            side->value = 2;
        } else {
            Engine_ActorEnableActionCallback(25, Data_0200d858);
            side->value = 3;
        }
        break;
    case 0:
    case 2:
        if (facing > 0x2000 && facing < 0xa000) {
            Engine_ActorEnableActionCallback(25, Data_0200e4d8[half][side->value]);
            side->value = side->value - half * 2 + 1;
            break;
        }
        goto back;
    case 1:
    case 3:
        if (facing > 0x6000 && facing < 0xe000) {
            Engine_ActorEnableActionCallback(25, Data_0200e4d8[half][side->value]);
            side->value = side->value - half * 2 + 1;
            break;
        }
    back:
        Engine_ActorEnableActionCallback(25, Data_0200e4d8[half ^ 1][side->value]);
        side->value = side->value - half * 2 - 1;
        break;
    }
    side->value &= 3;
    Engine_ActorStartAction(25);
    Engine_EventEnd();
}
