@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a51d0
	.thumb_func
Func_080a51d0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #104]
	ldr	r7, [r3, #0]
	movs	r3, #135
	lsls	r3, r3, #2
	adds	r5, r7, r3
	movs	r2, #188
	ldr	r3, [r5, #0]
	lsls	r2, r2, #1
	adds	r6, r7, r2
	ldrh	r1, [r6, #0]
	ldrb	r2, [r3, #14]
	movs	r0, #2
	movs	r3, #0
	bl	Func_08015298
	ldr	r2, [r5, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r2, [r5, #0]
	movs	r3, #112
	strh	r3, [r2, #6]
	ldr	r2, [r5, #0]
	movs	r3, #8
	strh	r3, [r2, #8]
	ldr	r0, [r5, #0]
	bl	Func_080a17c4
	ldr	r2, [pc, #52]
	adds	r3, r7, r2
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	movs	r3, #134
	lsls	r3, r3, #1
	adds	r5, r7, r3
	ldr	r1, [r5, #0]
	movs	r2, #16
	movs	r3, #0
	bl	Func_08015090
	ldrh	r3, [r6, #0]
	ldr	r0, [pc, #28]
	ands	r0, r3
	ldr	r3, [pc, #28]
	ldr	r1, [r5, #0]
	adds	r0, r0, r3
	movs	r2, #16
	movs	r3, #8
	bl	Func_08015080
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x0000021a
	.4byte 0x000001ff
	.4byte 0x00000182
