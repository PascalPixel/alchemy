void Func_02007c94(); void Func_02007cc0(); int Func_02007c72();
void Func_02007d84(); void Func_02007d8c(); void Func_02007da4();
/* Second-phase story selector at 0x020017c8, 60 bytes; six calls. */
void Func_020017c8(void) {
    Func_02007c94();
    if (Func_02007c72(0x933)) Func_02007d84(0x1ede);
    else Func_02007d8c(0x1edf);
    Func_02007da4(25, 0); Func_02007cc0();
}
