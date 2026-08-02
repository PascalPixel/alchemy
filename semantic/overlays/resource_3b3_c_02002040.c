typedef unsigned char u8;
typedef signed int s32;

u8 *Func_0808a080();
s32 Func_02000350();
s32 Func_02001268();
void Func_02001fd4();
void Func_0808a2a8();

/*
 * Complete 64-byte position-probe dispatcher.  It probes one unit above entity
 * 0, takes the local success action when occupied, otherwise runs the fallback
 * action only when the second local predicate is false.
 */
void Func_02002040(void)
{
    u8 *entity = Func_0808a080(0);
    s32 position[3];

    position[0] = *(s32 *)(entity + 8);
    position[1] = *(s32 *)(entity + 12);
    position[2] = *(s32 *)(entity + 16) + 0x100000;
    if (Func_02000350(position) != 0) {
        Func_02001fd4();
    } else if (Func_02001268() == 0) {
        Func_0808a2a8();
    }
}
