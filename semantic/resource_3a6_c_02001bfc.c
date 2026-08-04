typedef signed int s32;

extern void Func_0808a100(s32 actorId, s32 state);

/* Advance actor eleven through the two presentation states used at scene end. */
void Func_02001bfc(void)
{
    Func_0808a100(11, 1);
    Func_0808a100(11, 2);
}
