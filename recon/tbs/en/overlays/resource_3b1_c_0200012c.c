/* NONMATCHING: 344 of 342 bytes, 29 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: instructions are identical; only the literal pool moves. The reference dumps its pool (the zero link symbol, then the jump-table address) after the barrier that ends the shared 'step++; rise_counter = 0' tail (0x020020e), 200 bytes after the jump-table load; this draft places it at the end of the function in the opposite order. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);
s32 StagedActor_CountdownUntilPositionUnset(u8 *object);
void StagedActor_AdvanceCounter98(u8 *object);

struct Walker {
    u8 unknown_00[0x4c];
    s32 countdown;
    u8 unknown_50[0x16];
    s16 step;
};

extern u8 Data_00000000[];

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Local_0200012c(struct FieldActor *obj)
{
    struct FieldActor *leader;
    struct Walker *walker;

    walker = (struct Walker *)obj;
    leader = Engine_ActorGet(8);
    switch (walker->step) {
    case 0:
        obj->facing = 0xb000;
        goto advance;
    case 2:
        obj->facing = 0;
        goto advance;
    case 4:
        Engine_ObjectSetAnimation(obj, 2);
        Call4(Engine_ObjectSetPosition, (s32)obj, 0x1d40000, 0x200000, 0x2780000);
        walker->countdown = 60;
        walker->step++;
        break;
    case 5:
        if (StagedActor_CountdownUntilPositionUnset((u8 *)obj) != 0) {
            Engine_ObjectSetAnimation(obj, 1);
            obj->rise_counter = 0;
            if (leader->unknown_5b == 0) {
                obj->rise_enabled = 1;
            }
            walker->step++;
        }
        break;
    case 7:
        if (leader->unknown_5b == 0) {
            Engine_ObjectSetAnimation(obj, 3);
            obj->rise_enabled = 2;
        }
    advance:
        walker->step++;
        obj->rise_counter = (s32)Data_00000000;
        break;
    case 9:
        Engine_ObjectSetAnimation(obj, 2);
        Call4(Engine_ObjectSetPosition, (s32)obj, 0x1e00000, 0x200000, 0x2580000);
        walker->countdown = 60;
        walker->step++;
        if (leader->unknown_5b == 0) {
            obj->rise_enabled = 3;
        }
        break;
    case 10:
        if (StagedActor_CountdownUntilPositionUnset((u8 *)obj) != 0) {
            Engine_ObjectSetAnimation(obj, 1);
            obj->rise_counter = 0;
            walker->step++;
        }
        break;
    case 1:
    case 3:
    case 6:
    case 8:
    case 11:
        StagedActor_AdvanceCounter98((u8 *)obj);
        break;
    case 12:
        walker->step = 0;
        break;
    }
}
