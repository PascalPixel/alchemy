/*
 * Draft of overlay 68f Func_02000404: sets up each switch object of a table
 * that ends on 0xffff. 34 instructions differ from one cause: the listing
 * builds 0xffff again at the bottom of the loop, the draft keeps the first
 * test's 0xffff in r10 across the loop (the loop pass moves it out). The
 * rows are three halfwords; a struct row is padded to 8 bytes. Tried: for,
 * while and a guarded do-while, a one-pass block before the loop, and the
 * calls through the call layer, (u16)-1 and a static const end marker: no
 * change. The loop dump says why: the draft builds 0xffff in three pseudos
 * and the first (255, life 4, savings 2) is worth moving; the listing fits
 * one pseudo set three times, or one constant split after the loop pass,
 * which the loop pass leaves. With the marker as a variable set to 0xffff
 * at each test ("while (end = 0xffff, row[0] != end)") the loop rebuilds it
 * and 20 differ: the first test then reads through the copied pointer and
 * the loop top does not read the id again.
 */
#include "types.h"
#include "field_event.h"

struct FieldActor *Object_GetById();
void ObjectDispatch_SetSingleChildFlags();
s32 GameFlag_Test();
void Func_02002f80();
void Func_02000338();

void Func_02000404(u16 *row)
{
    while (row[0] != 0xffff) {
        struct FieldActor *object = Object_GetById(row[0]);
        s16 flag = row[2];
        s16 animation = row[1];

        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02002f80(object, (u16)animation + GameFlag_Test((u16)flag));
        object->motionFlags = 0;
        object->collisionFlags &= 0xfe;
        object->timer = flag;
        row += 3;
        Func_02000338(object);
    }
}
