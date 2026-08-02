/* Reset the selected actor's motion fields after refreshing it. */
typedef signed int s32;
typedef unsigned char u8;
u8 *Func_0808a080(s32 slot);
void Func_08009140(u8 *actor);

void Func_02002e44(s32 slot)
{
    u8 *actor = Func_0808a080(slot);
    Func_08009140(actor);
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    *(s32 *)(actor + 56) = (s32)0x80000000;
    *(s32 *)(actor + 64) = (s32)0x80000000;
}
