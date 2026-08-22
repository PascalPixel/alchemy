/* Begin a scene, attempt the forward transition, and fall back to pushing the
 * obstructing actor when the transition cannot run.  Complete 28-byte owner
 * from the prologue at 0x02001528 through return/alignment at 0x02001543. */
void Func_02004038(void);
void Func_02004050(void);
int Func_02002798(void);
void Func_020018e0(void);

void Func_02001528(void)
{
    Func_02004038();
    if (Func_02002798() == 0)
        Func_020018e0();
    Func_02004050();
}
