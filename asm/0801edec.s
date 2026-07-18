@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801edec
	.thumb_func
Func_0801edec:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #36]
	ldr	r7, [r3, #0]
	sub	sp, #4
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L0
	mov	r0, sp
	ldr	r3, [pc, #16]
	adds	r0, #2
	strh	r3, [r0, #0]
	mov	r1, r8
	ldr	r3, [pc, #16]
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000e0e0
	.4byte 0x03001e8c
	.4byte 0x040000d4
	.4byte 0x810000a0
.L0:
	ldr	r5, [pc, #52]
	adds	r0, r5, #0
	bl	Func_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #40]
	ldr	r0, [pc, #44]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	adds	r1, r7, #0
	bl	Func_080072fc
	adds	r0, r6, #0
	bl	Func_08002df0
.L1:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000214
	.4byte 0x040000d4
	.4byte 0x080158e8
