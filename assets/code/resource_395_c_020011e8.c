typedef signed int s32;

struct Frame {
    s32 pad00[6];
    s32 f18;
    s32 pad1c[7];
    s32 f38;
    s32 f3c;
    s32 f40;
};

extern void Func_02002b52(struct Frame *);

s32 Func_020011e8(struct Frame *frame)
{
    frame->f18 += 0x1EB8;
    if (frame->f38 == 0x80000000) {
        if (frame->f3c == frame->f38) {
            if (frame->f40 == frame->f3c) {
                Func_02002b52(frame);
            }
        }
    }
    return 1;
}
