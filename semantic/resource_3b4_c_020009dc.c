typedef signed int s32;
extern void Func_0808a4f8(s32 actorId, s32 mode, s32 frames);

/* Complete 16-byte actor-15 mode wrapper before the no-op leaf at 0x9ec. */
void Func_020009dc(void)
{
    Func_0808a4f8(15, 0, 6);
}
