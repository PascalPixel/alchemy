typedef unsigned char u8;
typedef signed int s32;

extern void Func_080000c0(s32 frames);

/* Give the actor at most sixty frames to descend to its target height, then
 * clamp the live height to the target so the following scene starts exact. */
void Func_02000cd0(u8 *object)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_080000c0(1);
        frames--;
        if (*(s32 *)(object + 12) <= *(s32 *)(object + 20))
            break;
    }
    *(s32 *)(object + 12) = *(s32 *)(object + 20);
}
