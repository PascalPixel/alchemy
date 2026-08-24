void Func_02007bec(); void Func_02007c2a(); int Func_02007bca(); int Func_02007bdc();
void Func_02007cdc(); void Func_02007cee(); void Func_02007cf6(); void Func_02007d0e();
/* Second-phase story selector at 0x02001720, 84 bytes; eight calls. */
void Func_02001720(void) {
    Func_02007bec();
    if (Func_02007bca(0x931)) Func_02007cdc(0x1edb);
    else if (Func_02007bdc(0x93a)) Func_02007cee(0x1edc);
    else Func_02007cf6(0x1edd);
    Func_02007d0e(23, 0); Func_02007c2a();
}
