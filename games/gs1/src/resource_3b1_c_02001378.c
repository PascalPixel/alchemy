void Func_02007844(); void Func_02007882(); int Func_02007822();
int Func_02007834(); void Func_02007934(); void Func_02007946(); void Func_0200794e();
void Func_02007966();
/* Story selector owner at 0x02001378, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x13cc with distinct local stub addresses). */
void Func_02001378(void) {
    Func_02007844();
    if (Func_02007822(0x92d)) Func_02007934(0x1ece);
    else if (Func_02007834(0x936)) Func_02007946(0x1ecf);
    else Func_0200794e(0x1ed0);
    Func_02007966(0x13, 0); Func_02007882();
}
