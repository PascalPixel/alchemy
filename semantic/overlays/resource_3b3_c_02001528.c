/* Begin a scene, attempt the forward transition, and fall back to pushing the
 * obstructing actor when the transition cannot run.  Complete 28-byte owner
 * from the prologue at 0x02001528 through return/alignment at 0x02001543. */
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern int Func_02001268(void);
extern void Func_020003a8(void);

void Func_02001528(void)
{
    Func_0808a018();
    if (Func_02001268() == 0)
        Func_020003a8();
    Func_0808a020();
}
