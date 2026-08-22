void Func_02007a24(); void Func_02007a50(); int Func_02007a02();
void Func_02007b14(); void Func_02007b1c(); void Func_02007b34();
/* Story selector owner at 0x02001558, 60 bytes; six calls. */
void Func_02001558(void) {
    Func_02007a24();
    if (Func_02007a02(0x933)) Func_02007b14(0x1ed1);
    else Func_02007b1c(0x1ed2);
    Func_02007b34(25, 0); Func_02007a50();
}
