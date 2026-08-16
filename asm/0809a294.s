@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809a294
	.thumb_func
Func_0809a294:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #272]
	ldr	r3, [r3, #0]
	sub	sp, #20
	mov	sl, r3
	bl	Func_08097384
	mov	r3, sl
	ldr	r0, [r3, #4]
	add	r5, sp, #8
	str	r0, [r5, #0]
	ldr	r1, [r3, #8]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r1, r1, r3
	str	r1, [r5, #4]
	mov	r3, sl
	ldr	r2, [r3, #12]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r0, r0, r3
	movs	r3, #128
	str	r2, [r5, #8]
	lsls	r3, r3, #8
	bl	Func_0809a3c4
	ldr	r3, [pc, #224]
	str	r0, [sp, #0]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	ldr	r2, [r5, #8]
	movs	r3, #0
	bl	Func_0809a3c4
	str	r0, [sp, #4]
	movs	r0, #15
	mov	fp, sp
	bl	Func_080030f8
	movs	r0, #1
	mov	r7, fp
	mov	r8, r0
.L1:
	ldmia	r7!, {r6}
	cmp	r6, #0
	beq.n	.L0
	movs	r1, #192
	ldrh	r2, [r6, #6]
	adds	r0, r6, #0
	lsls	r1, r1, #13
	bl	Func_08096bec
.L0:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L1
	ldr	r0, [sp, #0]
	bl	Func_08009158
	movs	r0, #134
	bl	Func_080f9010
	movs	r0, #128
	movs	r3, #23
	lsls	r0, r0, #10
	adds	r7, r5, #0
	mov	r8, r3
	mov	r9, r0
.L3:
	mov	r3, sl
	ldr	r1, [r3, #4]
	str	r1, [r7, #0]
	movs	r0, #128
	ldr	r2, [r3, #8]
	lsls	r0, r0, #13
	adds	r2, r2, r0
	str	r2, [r7, #4]
	ldr	r3, [r3, #12]
	ldr	r0, [pc, #124]
	str	r3, [r7, #8]
	bl	Func_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L2
	ldr	r1, [pc, #112]
	bl	Func_08009098
	bl	Func_08004458
	mov	r3, r9
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	add	r0, r9
	movs	r3, #0
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r5, r5, #3
	adds	r5, r5, r0
	bl	Func_08004458
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	Func_08096bec
.L2:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L3
	ldr	r0, [sp, #0]
	bl	Func_080090d0
	mov	r3, fp
	ldr	r0, [r3, #4]
	bl	Func_080090d0
	bl	Func_0809748c
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0xffe00000
	.4byte 0x0000011d
	.4byte 0x0809f0d4
