/*
 * resource_3c9: a scripted-scene bracket around three setup steps.
 */

extern void Func_020095f8(void);
extern void Func_02009610(void);
/* The three inner steps are old-style: their signatures are not established. */
extern void Func_02007490();
extern void Func_02007768();
extern void Func_02007b0c();

void FieldScene_RunThreeStepsInBracket(void)
{
    Func_020095f8();
    Func_02007490();
    Func_02007768();
    Func_02007b0c();
    Func_02009610();
}
