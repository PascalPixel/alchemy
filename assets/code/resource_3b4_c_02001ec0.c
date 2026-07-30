typedef signed int s32;

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Frame *Func_02004406(s32);
extern void Func_020044be(s32, s32, s32);

void Func_02001ec0(void)
{
    struct Frame *frame = Func_02004406(9);

    Func_020044be(107, frame->f08, frame->f10 + 0x10000);
}
