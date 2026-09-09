#include "types.h"

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

s32 Func_02002364(void);      /* site 0x1086 */
void Func_02002224();         /* site 0x1092 */
void Func_0200590c();         /* site 0x109e */

s32 Func_02007520();          /* site 0x107e; story flag test */
void Func_02007556();         /* site 0x108c */
void Func_02007642();         /* site 0x1098 */
void Func_020075f8();         /* site 0x10a6 */
u8 *Func_02007596();          /* site 0x10ac; returns the actor record */
void Func_020075d8();         /* site 0x10be */
void Func_02007606();         /* site 0x10c4 */
void Func_02007618();         /* site 0x10ce */
void Func_020075a4();         /* site 0x10d2 */

s32 Func_0200757c();          /* site 0x10da; story flag test */
s32 Func_0200758c();          /* site 0x10ea; story flag test */
s32 Func_0200759c();          /* site 0x10fa; story flag test */
void Func_0200290e();         /* site 0x1108; the three selected variants share it */
void Func_0200291a();         /* site 0x1114; the default variant has its own site */

/*
 * With story flag 0x300 set, opens a scripted sequence, creates a local
 * object, shows message 0x1ea6, configures it as slot 2, moves it onto actor
 * 0's signed halfword coordinates when actor 0 exists, then releases and
 * closes the sequence.  Otherwise flags 0x92b, 0x92a and 0x929 select the
 * setup call's third argument.  The 204-byte owner includes an alignment
 * halfword and its nine pool words.
 */
void FieldScene_RunActor16FlagDialogue(void)
{
    s32 obj;
    u8 *actor;

    if (Func_02007520(0x300) != 0) {
        obj = Func_02002364();
        Func_02007556();
        Func_02002224(obj);
        Func_02007642(0x1ea6);
        Func_0200590c(16);
        Func_020075f8(obj, 2);

        actor = Func_02007596(0);
        if (actor != 0) {
            Func_020075d8(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Func_02007606(obj);
        Func_02007618(obj, 0, 0);
        Func_020075a4();
    } else {
        if (Func_0200757c(0x92b) != 0) {
            Func_0200290e(16, 0x1e91, 0x998);
        } else if (Func_0200758c(0x92a) != 0) {
            Func_0200290e(16, 0x1e91, 0x91f);
        } else if (Func_0200759c(0x929) != 0) {
            Func_0200290e(16, 0x1e91, 0x93d);
        } else {
            Func_0200291a(16, 0x1e91, 0x934);
        }
    }
}
