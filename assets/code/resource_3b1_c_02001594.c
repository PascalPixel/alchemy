void Func_02007a60(); void Func_02007a9e(); int Func_02007a3e(); int Func_02007a50();
void Func_02007b50(); void Func_02007b62(); void Func_02007b6a(); void Func_02007b82();
/* Second-phase story selector at 0x02001594, 84 bytes; eight calls. */
void Func_02001594(void) {
    Func_02007a60();
    if (Func_02007a3e(0x92c)) Func_02007b50(0x1edb);
    else if (Func_02007a50(0x935)) Func_02007b62(0x1edc);
    else Func_02007b6a(0x1edd);
    Func_02007b82(18, 0); Func_02007a9e();
}
