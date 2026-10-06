/* 08000948: ARM draft (IwramSqrt); no admitted game ARM build route.
   First draft, written from recon/tbs/raw/08000948.S, scored at the
   library ARM setting (analysis only, no credit):
   alchemy diff --camelot-arm (upstream's whole arm_adjust_cost):
   listing 15, draft 23, common 0, differ 38; stock agbcc_arm: 38.
   Both compilers turn the two conditional subtractions into conditional
   moves (rsb r2 = root - bit, then addls/addhi and movhi) and hold
   bit >> 1 in its own register, so the draft needs r4, ip and lr and
   saves lr; the listing tests n with movs, returns with bxmi lr, and has
   only cmp; subhi after each square, in r0-r3.
   Trials: alchemy permute --camelot-arm, 2.5 minutes, 8 jobs: best
   differ 32 (weighted 2660 -> 1300) with bit >> 1 read into a temporary
   before the first test and that temporary reused for the second
   square; an unnatural spelling, the conditional moves remain, so it
   is not taken. */
typedef int s32;
typedef unsigned int u32;

s32 IwramSqrt(s32 n)
{
    u32 root;
    u32 bit;

    root = 0;
    if (n < 0)
        return root;
    bit = 0x8000;
    do {
        root += bit;
        if (root * root > (u32)n)
            root -= bit;
        root += bit >> 1;
        if (root * root > (u32)n)
            root -= bit >> 1;
    } while ((bit >>= 2) != 0);
    return root;
}
