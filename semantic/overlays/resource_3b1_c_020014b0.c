void Func_0808a018(); void Func_0808a020(); int Func_080770c0();
void Func_0808a170(); void Func_0808a180();
/* Story selector owner at 0x020014b0, 84 bytes; eight calls. */
void Func_020014b0(void) {
    Func_0808a018();
    if (Func_080770c0(0x931)) Func_0808a170(0x1ece);
    else if (Func_080770c0(0x93a)) Func_0808a170(0x1ecf);
    else Func_0808a170(0x1ed0);
    Func_0808a180(8, 0); Func_0808a020();
}
