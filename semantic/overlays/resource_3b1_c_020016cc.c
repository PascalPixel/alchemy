void Func_0808a018(); void Func_0808a020(); int Func_080770c0();
void Func_0808a170(); void Func_0808a180();
/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void Func_020016cc(void) {
    Func_0808a018();
    if (Func_080770c0(0x930)) Func_0808a170(0x1edb);
    else if (Func_080770c0(0x939)) Func_0808a170(0x1edc);
    else Func_0808a170(0x1edd);
    Func_0808a180(8, 0); Func_0808a020();
}
