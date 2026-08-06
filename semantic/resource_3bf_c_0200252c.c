#include "types.h"

/*
 * Resource 3bf party placement at 0x0200252c.
 *
 * Three lookups, each with the same shape: query slot 0, and if the query
 * returns a non-null record, forward that record's fields at +8 and +16 to the
 * matching per-actor call.  The actor codes are 2, 3 and 1, in that order,
 * and that same 2/3/1 ordering is repeated by the placement block below, which
 * is the evidence that the three sequences describe the same three actors.
 *
 * The placement block gives every actor the shared pair (0xb333, 0x5999) and
 * then its own (x, y): actor 2 gets (456, 192), actor 3 gets (440, 184) and
 * actor 1 gets (448, 240).  Each of those coordinates is materialised with the
 * usual `movs`/`lsls` immediate pair, so they are plain constants, not pointers.
 *
 * The trailing block enables actors 2, 1 and 3 and gives each the same
 * (12, 0) pair, then runs a final call with 15.
 *
 * The callees are other entry points of this same overlay module, named by the
 * tree's offset convention; none of them is reconstructed here.  Note that
 * Func_02007c08 is reached twice with different argument shapes — once as the
 * actor-1 record forwarder and once as an actor-2 coordinate setter — which is
 * why the record forwarders and the setters are declared separately below only
 * where their shapes agree, and why no common prototype is invented.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints (the dispatcher at 0x02004638 is the clearest
 * witness), so the module is linked at 0x02008000.  Symbol names here keep the
 * tree's printed-offset spelling, matching the byte-exact sources under
 * assets/code.
 */
typedef struct ActorRecord {
    s32 pad0;
    s32 pad4;
    s32 field8;
    s32 pad12;
    s32 field16;
} ActorRecord;











extern s32 Func_02007b82();
extern void Func_02007be0();
extern s32 Func_02007b96();
extern void Func_02007bf4();
extern s32 Func_02007baa();
extern void Func_02007c08();
extern void Func_02007c7a();
extern void Func_02007bd4();
extern void Func_02007bea();
extern void Func_02007c1e();
extern void Func_02007c00();
extern void Func_02007c34();
extern void Func_02007c52();
extern void Func_02007c94();
extern void Func_02007c62();
extern void Func_02007c68();
extern void Func_02007caa();
extern void Func_02007cb4();
extern void Func_02007c12();
void Func_0200252c(void)
{
    ActorRecord *record;

    record = Func_02007b82(0);
    if (record != 0) {
        Func_02007be0(2, record->field8, record->field16);
    }

    record = Func_02007b96(0);
    if (record != 0) {
        Func_02007bf4(3, record->field8, record->field16);
    }

    record = Func_02007baa(0);
    if (record != 0) {
        Func_02007c08(1, record->field8, record->field16);
    }

    Func_02007c7a(0, 0, 0);

    Func_02007bd4(2, 0xb333, 0x5999);
    Func_02007c08(2, 456, 192);

    Func_02007bea(3, 0xb333, 0x5999);
    Func_02007c1e(3, 440, 184);

    Func_02007c00(1, 0xb333, 0x5999);
    Func_02007c34(1, 448, 240);

    Func_02007c52(2);
    Func_02007c94(2, 12, 0);

    Func_02007c62(1);
    Func_02007c68(3);
    Func_02007caa(1, 12, 0);
    Func_02007cb4(3, 12, 0);

    Func_02007c12(15);
}
