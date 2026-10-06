/* 08001f38: ARM draft (ColorBuffer_DarkenKernel); no admitted game ARM
   build route. First draft, written from recon/tbs/raw/08001f38.S, scored
   at the library ARM setting (analysis only, no credit):
   alchemy diff --camelot-arm (upstream's whole arm_adjust_cost):
   listing 32, draft 44, common 5, differ 66; stock agbcc_arm: 66.
   The two constants build exactly as listed (rsb, add, add, add for
   0x80808080 - amount; mov, add, orr lsl #16 for 0x01010101) and the
   push starts at r5; the listing's ldm r0 / stmia r2! / stmia r0! are
   not produced: the four loads become ldr, ldr, ldm and the stores
   single str. Trial: the backup as one 16-byte struct copy
   (*(struct Quad *)backup = *(struct Quad *)buf): 66, the block move
   loads all four words again and stores them with stm, no writeback.
   Each word holds packed
   colour bytes; the kernel backs a block of four words up and subtracts
   amount from every byte, stopping at zero. */
typedef unsigned int u32;
typedef unsigned char u8;

void ColorBuffer_DarkenKernel(u32 *buf, u32 amount, u32 *backup, u32 size)
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
        backup[0] = a;
        backup[1] = b;
        backup[2] = c;
        backup[3] = d;
        backup += 4;
        t = a + bias;
        m = (t >> 7) & ones;
        m = (m << 7) - m;
        a = t & m;
        t = b + bias;
        m = (t >> 7) & ones;
        m = (m << 7) - m;
        b = t & m;
        t = c + bias;
        m = (t >> 7) & ones;
        m = (m << 7) - m;
        c = t & m;
        t = d + bias;
        m = (t >> 7) & ones;
        m = (m << 7) - m;
        d = t & m;
        buf[0] = a;
        buf[1] = b;
        buf[2] = c;
        buf[3] = d;
        buf += 4;
    } while (buf != end);
}
