typedef signed int s32;

extern void Func_080f9010(s32 soundId);
extern void Func_020017d8(s32 actorId, s32 cellX, s32 cellZ);
extern void Func_080770d0(s32 flagId);
extern void Func_080000c0(s32 frames);
extern void Func_02001770(void);

/* Mirror the preceding transition on the negative grid axis, latch its event
 * flag, wait for the map update, and close on the paired sound cue. */
void Func_020018f4(void)
{
    Func_080f9010(241);
    Func_020017d8(11, -112, 0);
    Func_020017d8(11, -80, 0);
    Func_080770d0(0x301);
    Func_080000c0(2);
    Func_02001770();
    Func_080f9010(0x121);
}
