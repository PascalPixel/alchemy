@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08015f30
	.thumb_func
Func_08015f30:
	push	{r5, lr}
	ldr	r1, [pc, #96]
	movs	r0, #15
	sub	sp, #4
	bl	Func_080048f4
	movs	r3, #0
	adds	r4, r0, #0
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [pc, #80]
	adds	r1, r4, #0
	ldr	r2, [pc, #80]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #76]
	adds	r2, r4, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #72]
	adds	r2, r4, r3
	movs	r3, #99
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	adds	r2, r4, r3
	movs	r3, #15
	strb	r3, [r2, #0]
	ldr	r3, [pc, #64]
	adds	r0, r5, #0
	str	r3, [r5, #0]
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	Func_08015ef4
	bl	Func_08019d0c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl	Func_080041d8
	bl	Func_080173f4
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x000012fc
	.4byte 0x040000d4
	.4byte 0x850004bf
	.4byte 0x00000ea3
	.4byte 0x000012b6
	.4byte 0x00000ea7
	.4byte 0xf000f000
	.4byte 0x85000140
	.4byte 0x080160fd
