#include "types.h"

#define SceneDialogue_RunActor21Line Func_02000b98
extern unsigned char Value_00001f00;

void Func_02004e88(void);
void Func_02004ea2(void);
void Func_02004f4e(s32 no);
s32 Func_02004f76(s32 no, s32 val);

/*
 * One-line dialogue callback in overlay resource_3af.  It is installed into
 * the script-record table rather than called.
 */

/*
 * Open the scripted-scene bracket, speak the line, hand actor 21 back, close
 * the bracket.  The 32-byte owner ends with its one pool word, 0x1f00, the
 * dialogue line id: it is passed as the address of Value_00001f00 so the id
 * stays a pool word and is not folded into an immediate.  The actor and line
 * ids are transcribed only, and what the installing record represents is not
 * established.
 */
void SceneDialogue_RunActor21Line(void)
{
    Func_02004e88();
    Func_02004f4e((s32)&Value_00001f00);
    Func_02004f76(21, 0);
    Func_02004ea2();
}
