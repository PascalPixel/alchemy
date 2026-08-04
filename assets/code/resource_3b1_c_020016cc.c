void Func_02007b98(); void Func_02007bd8(); int Func_02007b78(); int Func_02007b8a();
void Func_02007c8a(); void Func_02007c9c(); void Func_02007ca4(); void Func_02007cbc();
/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void Func_020016cc(void) {
    Func_02007b98();
    if (Func_02007b78(0x930)) Func_02007c8a(0x1edb);
    else if (Func_02007b8a(0x939)) Func_02007c9c(0x1edc);
    else Func_02007ca4(0x1edd);
    Func_02007cbc(22, 0); Func_02007bd8();
}
