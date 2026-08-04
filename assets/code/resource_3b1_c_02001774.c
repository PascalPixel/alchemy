void Func_02007c40(); void Func_02007c7e(); int Func_02007c1e(); int Func_02007c30();
void Func_02007d30(); void Func_02007d42(); void Func_02007d4a(); void Func_02007d62();
/* Second-phase story selector at 0x02001774, 84 bytes; eight calls. */
void Func_02001774(void) {
    Func_02007c40();
    if (Func_02007c1e(0x932)) Func_02007d30(0x1edb);
    else if (Func_02007c30(0x93b)) Func_02007d42(0x1edc);
    else Func_02007d4a(0x1edd);
    Func_02007d62(24, 0); Func_02007c7e();
}
