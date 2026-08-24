void Func_02007898(); void Func_020078d6(); int Func_02007876();
int Func_02007888(); void Func_02007988(); void Func_0200799a(); void Func_020079a2();
void Func_020079ba();
/* Story selector owner at 0x020013cc, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x1378 with distinct local stub addresses). */
void Func_020013cc(void) {
    Func_02007898();
    if (Func_02007876(0x92e)) Func_02007988(0x1ece);
    else if (Func_02007888(0x937)) Func_0200799a(0x1ecf);
    else Func_020079a2(0x1ed0);
    Func_020079ba(0x14, 0); Func_020078d6();
}
