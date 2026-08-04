typedef signed short s16;
typedef signed int s32;

/*
 * Complete palette-task teardown owner.  It removes the per-frame palette
 * callback, releases the currently held graphics slot, then marks the slot as
 * vacant for the next scene.
 */
extern s16 Data_0200dace;
extern void Func_0200b91c(void);
extern void Func_0200836e(void (*callback)(void));
extern void Func_020083c8(s32 slot);

void Func_02003b18(void)
{
    Func_0200836e(Func_0200b91c);
    Func_020083c8(Data_0200dace);
    Data_0200dace = -1;
}
