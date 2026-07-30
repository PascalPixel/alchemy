typedef signed int s32;

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Frame *Func_02003c38(s32);

s32 Func_02000098(struct Frame *dest)
{
    struct Frame *source = Func_02003c38(8);

    dest->f08 = source->f08;
    dest->f0c = 0xFFF40000;
    dest->f10 = source->f10;
    return 0;
}
