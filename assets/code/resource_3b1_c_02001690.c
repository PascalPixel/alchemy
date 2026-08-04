void Func_02007b5c(); void Func_02007b88(); int Func_02007b3a();
void Func_02007c4c(); void Func_02007c54(); void Func_02007c6c();
/* Second-phase story selector at 0x02001690, 60 bytes; six calls. */
void Func_02001690(void) {
    Func_02007b5c();
    if (Func_02007b3a(0x92f)) Func_02007c4c(0x1ede);
    else Func_02007c54(0x1edf);
    Func_02007c6c(21, 0); Func_02007b88();
}
