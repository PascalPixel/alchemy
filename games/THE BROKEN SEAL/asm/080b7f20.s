@ 戦闘モーション: 現在オブジェクトの位置を3語コピーして変換し、
@ 結果を係数 [r6+24] で Q16 乗算する。戻り値 0。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global BattleMotion_ProjectPosition
	.global Func_080b7f20
	.thumb_func
BattleMotion_ProjectPosition:
Func_080b7f20:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #12
	mov	r8, r1
	bl	Func_080b7dd0
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_080b7f70
	adds	r6, r0, #0
	bl	Func_080b7ed8
@ 位置 (x,y,z) をスタックに写して変換へ渡す。
	ldr	r3, [r5, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	mov	r1, r8
	str	r3, [r0, #8]
	bl	Func_08005268
	ldr	r3, [pc, #24]
	ldr	r1, [r6, #24]
	movs	r0, r0
@ IwramMulQ16ReturnIp で係数を掛ける。
	mov	ip, pc
	bx	r3
	add	sp, #12
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03000118
