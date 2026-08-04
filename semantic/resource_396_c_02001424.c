typedef signed int s32;

extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_080000c0(s32 frames);

/* Play one of thirteen actor-8 pose sequences, then hold for twelve frames. */
void Func_02001424(s32 sequence)
{
    if (sequence == 0) {
        Func_0808a100(8, 1);
        Func_080000c0(6);
        Func_0808a100(8, 3);
        goto done;
    }
    if (sequence == 2) {
        Func_0808a100(8, 1);
        Func_080000c0(6);
        Func_0808a100(8, 5);
        goto done;
    }
    if (sequence == 3) {
        Func_0808a100(8, 1);
        Func_080000c0(6);
        Func_0808a100(8, 4);
        goto done;
    }
    if (sequence == 4) {
        Func_0808a100(8, 1);
        Func_080000c0(6);
        Func_0808a100(8, 3);
        Func_080000c0(6);
        Func_0808a100(8, 1);
        Func_080000c0(6);
        Func_0808a100(8, 3);
        Func_080000c0(6);
    }
    if (sequence == 1 || sequence == 4) {
        Func_0808a100(8, 1);
        goto done;
    }
    if (sequence == 5) {
        Func_0808a100(8, 1);
        Func_080000c0(6);
        Func_0808a100(8, 2);
        goto done;
    }
    if (sequence == 7) {
        Func_0808a100(8, 6);
        Func_080000c0(6);
        Func_0808a100(8, 8);
        goto done;
    }
    if (sequence == 9) {
        Func_0808a100(8, 6);
        Func_080000c0(6);
        Func_0808a100(8, 9);
        goto done;
    }
    if (sequence == 10) {
        Func_0808a100(8, 6);
        Func_080000c0(6);
        Func_0808a100(8, 10);
        goto done;
    }
    if (sequence == 11) {
        Func_0808a100(8, 6);
        Func_080000c0(6);
        Func_0808a100(8, 8);
        Func_080000c0(6);
        Func_0808a100(8, 6);
        Func_080000c0(6);
        Func_0808a100(8, 8);
        Func_080000c0(6);
    }
    if (sequence == 8 || sequence == 11) {
        Func_0808a100(8, 6);
        goto done;
    }
    if (sequence == 12)
        Func_0808a100(8, 6);

done:
    Func_080000c0(12);
}
