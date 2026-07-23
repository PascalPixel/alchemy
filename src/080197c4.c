typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_08016418(void *, s32);
void Func_080030f8(u32);

extern u8 *Data_03001e8c;

struct Work080197c4 {
    u8 padding00[0x16];
    u16 flag;
    s32 value;
};

struct Slot080197c4 {
    struct Work080197c4 *work;
    u8 padding04[0x24];
};

struct DirectWork080197c4 {
    u8 padding00[0x16];
    u16 flag;
    s32 value;
    u8 padding1c[8];
};

void Func_080197c4(void)
{
    u8 *state;
    struct Slot080197c4 *slot;
    struct DirectWork080197c4 *direct;
    u32 done;
    struct Work080197c4 *work;
    struct Work080197c4 *pollWork;
    s32 index;
    u16 flag;

    state = Data_03001e8c;
    slot = (struct Slot080197c4 *)(state + 0x620);
    direct = (struct DirectWork080197c4 *)(state + 0x500);
    index = 0;
    do {
        work = slot->work;
        if (work != 0 && work->flag != 0)
            Func_08016418(work, 0);
        index++;
        slot++;
    } while (index != 3);

poll:
    done = 1;
    slot = (struct Slot080197c4 *)(state + 0x620);
    index = 0;
    do {
        pollWork = slot->work;
        if (pollWork != 0) {
            if (pollWork->value == 0) {
                flag = pollWork->flag;
                if (flag == 0)
                    slot->work = (struct Work080197c4 *)(u32)flag;
                else
                    done = 0;
            } else {
                done = 0;
            }
        }
        index++;
        slot++;
    } while (index != 3);
    index = 0;
    if (!done) {
        Func_080030f8(1);
        goto poll;
    }
    goto directTest;
directLoop:
    if (direct->flag != 0)
        Func_08016418(direct, 0);
    direct++;
    index++;
directTest:
    if (index != 8)
        goto directLoop;
}
