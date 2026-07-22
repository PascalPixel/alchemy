typedef unsigned short u16;
typedef signed int s32;

struct Work {
    s32 field00;
    s32 field04;
    u16 field08;
    u16 field0a;
    u16 field0c;
    u16 field0e;
    u16 field10;
    u16 field12;
    u16 field14;
    u16 field16;
    u16 field18;
    u16 field1a;
    u16 field1c;
    u16 field1e;
    u16 field20;
    u16 field22;
};

void Func_08016478(struct Work *work);
void Func_08016178(u16 field0c, u16 field0e, u16 field08, u16 field0a);

void Func_08016418(struct Work *work, s32 release) {
    u16 zero;

    if (work == 0)
        return;

    Func_08016478(work);
    work->field1c = work->field0c;
    work->field1e = work->field0e;
    work->field20 = work->field08;
    zero = 0;
    work->field16 = zero;
    work->field22 = work->field0a;

    if (release != 0) {
        Func_08016178(work->field0c, work->field0e, work->field08, work->field0a);
        work->field00 = zero;
        work->field04 = zero;
        work->field08 = zero;
        work->field0a = zero;
        work->field0c = zero;
        work->field0e = zero;
        work->field10 = zero;
        work->field12 = zero;
        work->field14 = zero;
        work->field16 = zero;
        work->field18 = zero;
        work->field1a = zero;
        work->field1c = zero;
        work->field1e = zero;
        work->field20 = zero;
        work->field22 = zero;
    } else {
        work->field18 = release;
        work->field1a = 4;
    }
}
