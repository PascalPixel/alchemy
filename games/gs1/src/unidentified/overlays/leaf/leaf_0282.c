void Func_02007b08(); void Func_02007b46(); int Func_02007ae6(); int Func_02007af8();
void Func_02007bf8(); void Func_02007c0a(); void Func_02007c12(); void Func_02007c2a();
/* Second-phase story selector at 0x0200163c, 84 bytes; eight calls. */
void Func_0200163c(void) {
    Func_02007b08();
    if (Func_02007ae6(0x92e)) Func_02007bf8(0x1edb);
    else if (Func_02007af8(0x937)) Func_02007c0a(0x1edc);
    else Func_02007c12(0x1edd);
    Func_02007c2a(20, 0); Func_02007b46();
}
