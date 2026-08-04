typedef unsigned char u8;
typedef signed int s32;

u8 *Func_02004b64();
s32 Func_020023b2();
s32 Func_020032d8();
void Func_0200403e();
void Func_02004c0c();

/*
 * Complete 64-byte position-probe dispatcher.  It probes one unit above entity
 * 0, takes the local success action when occupied, otherwise runs the fallback
 * action only when the second local predicate is false.
 */
void Func_02002040(void)
{
    u8 *entity = Func_02004b64(0);
    s32 position[3];

    position[0] = *(s32 *)(entity + 8);
    position[1] = *(s32 *)(entity + 12);
    position[2] = *(s32 *)(entity + 16) + 0x100000;
    if (Func_020023b2(position) != 0) {
        Func_0200403e();
    } else if (Func_020032d8() == 0) {
        Func_02004c0c();
    }
}
