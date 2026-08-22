@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809a7f4
	.thumb_func
Func_0809a7f4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #104]
	ldrh	r6, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r9, r0
	adds	r0, r6, #0
	mov	sl, r2
	bl	Func_0800231c
	ldr	r5, [pc, #112]
	adds	r1, r0, #0
	mov	r0, sl
	mov	ip, pc
	bx	r5
	mov	r8, r0
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r1, r0, #0
	mov	r0, sl
	mov	ip, pc
	bx	r5
	mov	r2, r9
	ldr	r3, [r2, #8]
	add	r3, r8
	str	r3, [r7, #8]
	ldr	r3, [r2, #16]
	adds	r3, r3, r0
	str	r3, [r7, #16]
	movs	r0, #128
	ldrh	r3, [r7, #6]
	lsls	r0, r0, #4
	adds	r3, r3, r0
	strh	r3, [r7, #6]
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r0, #242
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #15
	lsls	r3, r3, #16
	movs	r1, #0
	cmp	r3, r0
	bne.n	.L0
	ldr	r3, [pc, #44]
	str	r3, [r7, #108]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r1, [r2, #0]
	strh	r1, [r3, #0]
	ldr	r3, [pc, #36]
	str	r3, [r7, #72]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r7, #40]
	bl	Func_08004458
	strh	r0, [r7, #6]
.L0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03000118
	.4byte 0x0809a739
	.4byte 0x00001999
