typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 371 owner at 0x020006ec (880 bytes) — the overlay's scene entry
 * point.  The image's fixed dispatch table at file offset 0 reaches it: its
 * first entry is `ldr r4,[pc,#0] / bx r4 / .word 0x020086ed`, and under this
 * overlay's proven 0x02008000 link base that word is this address plus the
 * Thumb bit.
 *
 * Complete owner: `push {r5, lr}` at 0x020006ec and `pop {r5} ; pop {r1} ;
 * bx r1` at 0x02000a2e.  The popped register is r1, not r0, so r0 survives
 * and IS the result — the constant 0 set at 0x02000a2c.  The 880-byte span
 * decomposes as head (0x020006ec-0x020007c7), an 80-entry jump table
 * (0x020007c8-0x02000907, data), the case bodies
 * (0x02000908-0x02000a33) and the literal pool
 * (0x02000a34-0x02000a8b).
 *
 * This is the fourth independent 0x02008000 witness in the overlay: the jump
 * table's base is loaded as the pool word 0x020087c8 while the table sits at
 * file offset 0x7c8, and all seventeen distinct entries are 0x8000 above case
 * bodies inside this function.  `mov pc, r3` is the dispatch, not a return.
 * Two more pool words are in-image code addresses with the Thumb bit set:
 * 0x02008599 is `Func_02000598` (the proximity sweep, registered here with
 * period 3200) and 0x0200b679 is `Func_02003678` (`push {r5, r6, lr}` at
 * file offset 0x3678).  Both are written below as the function pointers they
 * are.
 *
 * 0x0200e79c is a 32-bit RAM slot at exactly the first byte past the end of
 * the 0x679c-byte image, so it is scratch adjacent to the loaded overlay
 * rather than image data; it is kept under its raw pool spelling.
 *
 * The dispatch selector is `Data_02000240[225]` — the same scene id the
 * table-selector owner at 0x0200037c reads — biased by 1 and bounded
 * unsigned at 79, so the covered ids are 1..80 and everything else takes the
 * default arm.  The scene id is re-read from memory at the switch, after the
 * head has run its calls; that reload is preserved.
 *
 * UNCERTAINTY: the inventory row reports `calls=18` while 49 distinct `bl`
 * targets are present.  The undercount is a decoding artifact of the
 * 320-byte jump table sitting inside the executable span — it disassembles
 * as plausible `ldrh` pairs — not a missing call.  Every `bl` is placed
 * below.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).  Old-style declarations, because the
 * interfaces are unknown.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;
extern s32 Data_0200e79c;

/* This overlay's own owners, installed here as timed callbacks. */
void Func_02000598(void);
void Func_02003678(void);

void Func_020039fc();
void Func_0808a238();
void Func_0808a018();
void Func_080772e8();
void Func_0808a248();
void Func_080000c0();
void Func_0808a208();
void Func_080770d0();
s32 Func_080770c0();
void Func_080000d0();
void Func_080091d0();
void Func_080770c8();
void Func_0808a0f0();
u8 *Func_0808a080();
void Func_02002768();
void Func_020024a8();
void Func_02000a8c();
void Func_02000c28();
void Func_020017fc();
void Func_02001888();
void Func_02001938();
void Func_020019e8();
void Func_02001a98();
void Func_02001b5c();
void Func_02002274();
void Func_0200357c();
void Func_02003130();
s32 Func_080770e0();
void Func_020032f0();
void Func_02002cb4();

s32 Func_020006ec(void)
{
    s16 *scene = &Data_02000240[225];
    u8 *workspace;
    u8 *object;

    if (*scene == 99) {
        Func_080770c8(352);
        Func_080770c8(0x161);
        Func_080770c8(0x163);
    }

    if (*scene == 90) {
        Func_020039fc(0);
        Func_0808a238(0x3a, 1);
        return 0;
    }
    if (*scene == 91) {
        Func_020039fc(1);
        Func_0808a238(0xbb, 93);
        return 0;
    }
    if (*scene == 78) {
        Func_0808a018();
        Func_080772e8(242);
        Func_0808a248(112);
        return 0;
    }

    Func_080770c8(324);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 0x400;
    *(s32 *)(workspace + 456) = 16;

    Func_080000c0(1);
    Func_0808a208(0x80000, 0x10000);
    Func_080770d0(0x12f);
    Func_080000d0(Func_02000598, 3200);
    if (Func_080770c0(0x90a) == 0) {
        Func_080091d0(128, 256, 176, 56);
    }

    switch (*scene) {
    case 1:
        if (Func_080770c0(0x815) != 0) {
            break;
        }
        Func_080770c8(0x815);
        Func_080770c8(0x85c);
        break;

    case 33:
        if (Func_080770c0(0x109) != 0) {
            if (Func_080770c0(0x85d) != 0) {
                break;
            }
            if (Func_080770c0(564) == 0) {
                break;
            }
            Data_0200e79c = 55;
            Func_0808a0f0(55, 0x17940000, 0x0d480000);
            object = Func_0808a080(Data_0200e79c);
            *(u16 *)(object + 6) = 0x3000;
            Func_02002768(Data_0200e79c);
            break;
        }
        if (Func_080770c0(0x85d) != 0) {
            break;
        }
        if (Func_080770c0(0x9b8) != 0) {
            break;
        }
        Func_020024a8();
        break;

    case 49:
        if (Func_080770c0(0x94f) != 0) {
            break;
        }
        if (Func_080770c0(0x941) == 0) {
            break;
        }
        Func_02000a8c();
        break;

    case 64:
        if (Func_080770c0(0x85a) != 0) {
            break;
        }
        Func_02000c28();
        break;

    case 65: Func_020017fc(); break;
    case 66: Func_02001888(); break;
    case 67: Func_02001938(); break;
    case 68: Func_020019e8(); break;
    case 69: Func_02001a98(); break;
    case 70: Func_02001b5c(); break;
    case 71: Func_02002274(); break;
    case 72: Func_0200357c(); break;
    case 73: Func_02003130(); break;

    case 74:
    case 76:
    case 77:
        Func_080770c8(284);
        if (Func_080770e0(760) == 0) {
            break;
        }
        ((u8 *)Data_02000240)[498] = 2;
        Func_080000d0(Func_02003678, 3200);
        break;

    case 75: Func_020032f0(); break;
    case 80: Func_02002cb4(); break;

    default:
        /* Ids 2..32, 34..48, 50..63, 78, 79 and anything outside 1..80. */
        *(s32 *)(Func_0808a080(53) + 0x18) = 0x14000;
        *(s32 *)(Func_0808a080(53) + 0x1c) = 0x14000;
        break;
    }

    return 0;
}
