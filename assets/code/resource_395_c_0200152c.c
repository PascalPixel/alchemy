typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

struct Frame {
    s32 pad00[2];
    s32 x;
    s32 y;
    s32 z;
    s32 pad14;
    s32 angle0;
    s32 angle1;
    s32 pad20[6];
    s32 anchorX;
    s32 anchorY;
    s32 anchorZ;
    s32 pad44[3];
    u8 *owner;
    s32 pad54[4];
    s16 step;
    s16 heading;
};

void Func_02002e82();
void Func_02002eb4();
void Func_02002eda();

void Func_0200152c(struct Frame *frame)
{
    s32 position[3];
    s32 step = frame->step;
    s32 heading;

    if (step <= 119) {
        position[0] = frame->anchorX;
        position[1] = frame->anchorY;
        position[2] = frame->anchorZ;
        heading = frame->heading;
        Func_02002e82(step << 16, step * 768 + heading, position);
        frame->x = position[0];
        frame->y = position[1];
        frame->z = position[2];
        frame->angle0 += 0x147;
        frame->angle1 += 0x147;
        frame->step++;
    } else {
        Func_02002eb4(frame->owner[0x1c]);
        Func_02002eda(frame);
    }
}
