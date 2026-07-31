/* Routed -fsched-low-dest-first: the reference sets r0 before the finishing
 * `lsls r1` at three of these sites. */
void Func_020025c4();           /* scripted-scene bracket: open */
void Func_020026c8();           /* play animation id for n frames */
void Func_02002688();           /* select an entity presentation */
void Func_020025d6();           /* wait n frames */
void Func_020026ac();           /* show a dialogue line by id */
void Func_020026ce();           /* wait for the slot's action, with a duration */
void Func_020026b0();           /* scene request; interface not established */
void Func_020025f6();
void Func_020026e8();
void Func_02002704();           /* face slot along heading */
void Func_02002620();           /* scripted-scene bracket: close */

void Func_02000150(void)
{
    Func_020025c4();

    Func_020026c8(14, 258, 0);
    Func_02002688(14, 2);
    Func_020025d6(40);

    Func_020026ac(0x1764);

    Func_020026ce(14, 0, 20);
    Func_020026b0(14, 0, 0);
    Func_020025f6(20);
    Func_020026e8(14, 0, 10);
    Func_02002704(14, 0xb000, 10);

    Func_02002620();
}
