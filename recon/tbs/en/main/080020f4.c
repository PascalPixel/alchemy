/* 080020f4: ARM draft (ColorBuffer_DarkenPartialKernel); no admitted game
   ARM build route. First draft, written from recon/tbs/raw/080020f4.S,
   scored at the library ARM setting (analysis only, no credit):
   alchemy diff --camelot-arm (upstream's whole arm_adjust_cost):
   listing 52, draft 52, common 7, differ 90; stock agbcc_arm: 90.
   The listing's ldm r0! / stmia r2! and its strne stores, still under
   the flags of the zero tests, are not produced. Neither compiler can
   emit them: gcc_arm writes ldm/stm with writeback only in
   arm_gen_movstrqi's block copies, whose register lists always start
   at r0 (its load_multiple/store_multiple expanders never write back),
   and the listing's lists are r4-r7. As
   ColorBuffer_DarkenKernel, but a zero word is left as it is. */
typedef unsigned int u32;
typedef unsigned char u8;

void ColorBuffer_DarkenPartialKernel(u32 *buf, u32 amount, u32 *backup, u32 size)
{
    u32 bias;
    u32 ones;
    u32 *end;
    u32 a, b, c, d;
    u32 t, m;

    bias = 0x80808080 - amount;
    ones = 0x01010101;
    end = (u32 *)((u8 *)buf + size);
    do {
        a = buf[0];
        b = buf[1];
        c = buf[2];
        d = buf[3];
        buf += 4;
        backup[0] = a;
        backup[1] = b;
        backup[2] = c;
        backup[3] = d;
        backup += 4;
        if (a != 0) {
            t = a + bias;
            m = (t >> 7) & ones;
            m = (m << 7) - m;
            buf[-4] = t & m;
        }
        if (b != 0) {
            t = b + bias;
            m = (t >> 7) & ones;
            m = (m << 7) - m;
            buf[-3] = t & m;
        }
        if (c != 0) {
            t = c + bias;
            m = (t >> 7) & ones;
            m = (m << 7) - m;
            buf[-2] = t & m;
        }
        if (d != 0) {
            t = d + bias;
            m = (t >> 7) & ones;
            m = (m << 7) - m;
            buf[-1] = t & m;
        }
    } while (buf != end);
}
