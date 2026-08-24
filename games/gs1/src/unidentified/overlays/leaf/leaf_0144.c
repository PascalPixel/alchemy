void Func_020077f0(); void Func_0200782e(); int Func_020077ce();
int Func_020077e0(); void Func_020078e0(); void Func_020078f2(); void Func_020078fa();
void Func_02007912();
/* Story selector owner at 0x02001324, 84 bytes; eight calls. Per-site call
 * veneers (raw asm confirms each callee slot uses a distinct local stub
 * even across the three near-identical "twin" owners at 0x1324/1378/13cc). */
void Func_02001324(void) {
    Func_020077f0();
    if (Func_020077ce(0x92c)) Func_020078e0(0x1ece);
    else if (Func_020077e0(0x935)) Func_020078f2(0x1ecf);
    else Func_020078fa(0x1ed0);
    Func_02007912(0x12, 0); Func_0200782e();
}
