typedef unsigned char u8;
typedef signed int s32;

/* The probe record is passed on by value: four words in r0-r3 and the last two
 * copied to the outgoing stack area by an `ldmia`/`stmia` pair. */
struct Resource39fProbe {
    s32 word[6];
};

/* 0x02004246 serves two imports: the one-argument post at site 5 and the
 * six-argument message at site 12. */
extern void Func_020041fe(void);
extern s32 Func_020018d6(struct Resource39fProbe *);
extern void Func_02001a82(struct Resource39fProbe);
extern void Func_020041e8(s32, s32, s32, s32, s32, s32);
extern void Func_02004246(s32);
extern void Func_020042b6(s32, s32);
extern void Func_020042aa(s32, s32, s32);
extern void Func_02004268(s32);
extern void Func_020042d0(s32, s32);
extern void Func_0200438e(s32);
extern u8 *Func_0200429c(s32);
extern void Func_02004246_b(s32, s32, s32, s32, s32, s32);
extern s32 Func_02001752(s32, s32, s32, s32, s32, s32);
extern void Func_020042be(void);
void Func_02001454(void)
{
    struct Resource39fProbe probe;
    s32 fifth;
    s32 sixth;
    s32 height;
    s32 value;

    /* No argument register is written before this branch. */
    Func_020041fe();

    if (Func_020018d6(&probe) != 0) {
        Func_02001a82(probe);

        if (probe.word[1] == 8 && (probe.word[4] >> 20) == 23) {
            fifth = 35;
            sixth = 68;
            Func_020041e8(35, 67, 4, 1, fifth, sixth);
        } else if (probe.word[1] == 10 && (probe.word[2] >> 20) == 35) {
            /* Written here, not at the call: the reference keeps it in a
             * callee-saved register across the whole sequence. */
            value = 0;
            Func_02004246(0x311);
            Func_020042b6(10, 3);
            Func_020042aa(10, -16, 6);
            Func_02004268(30);
            Func_020042d0(10, 8);
            Func_0200438e(240);

            Func_0200429c(10)[35] = 2;

            fifth = 34;
            sixth = 30;
            Func_02004246_b(44, 30, 2, 4, fifth, sixth);
            height = 4;
            Func_02001752(2, 35, 30, 1, height, value);
        }
    }

    /* Common exit; no argument registers are set. */
    Func_020042be();
}
