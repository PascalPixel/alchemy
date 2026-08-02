void Func_020003a8();
void Func_02000f78();
void Func_0808a018();
void Func_0808a020();

/*
 * resource_3a6 owner at 0x020010c8, 24 bytes: open the scene scheduler,
 * initialize it, close it, then run the preceding tile-27 rain sequence.
 */
void Func_020010c8(void)
{
    Func_0808a018();
    Func_020003a8();
    Func_0808a020();
    Func_02000f78();
}
