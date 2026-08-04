void Func_02007ab4(); void Func_02007af2(); int Func_02007a92(); int Func_02007aa4();
void Func_02007ba4(); void Func_02007bb6(); void Func_02007bbe(); void Func_02007bd6();
/* Second-phase story selector at 0x020015e8, 84 bytes; eight calls. */
void Func_020015e8(void) {
    Func_02007ab4();
    if (Func_02007a92(0x92d)) Func_02007ba4(0x1edb);
    else if (Func_02007aa4(0x936)) Func_02007bb6(0x1edc);
    else Func_02007bbe(0x1edd);
    Func_02007bd6(19, 0); Func_02007af2();
}
