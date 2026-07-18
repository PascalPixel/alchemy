@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bb65c
	.thumb_func
Func_080bb65c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #128
	add	r7, sp, #4
	bl	Func_080040b4
	movs	r1, #0
	str	r1, [sp, #0]
	mov	fp, r0
.L6:
	ldr	r2, [pc, #136]
	ldr	r5, [r2, #0]
	movs	r3, #7
	lsrs	r5, r5, #2
	ands	r5, r3
	ldr	r3, [pc, #128]
	lsls	r5, r5, #7
	adds	r5, r5, r3
	ldr	r3, [pc, #128]
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #128]
	ldr	r1, [r3, #0]
	ldr	r3, [r3, #4]
	adds	r0, r6, #0
	mov	sl, r1
	movs	r1, #4
	mov	r8, r2
	mov	r9, r3
	bl	Func_080039fc
	adds	r0, r6, #0
	movs	r1, #16
	bl	Func_0800393c
	movs	r2, #16
	ldr	r3, [pc, #100]
	strh	r2, [r3, #0]
	movs	r3, #164
	lsls	r3, r3, #8
	str	r3, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #8]
	adds	r1, r5, #0
	mov	r0, fp
	bl	Func_080040d0
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	ands	r0, r3
	ldrh	r3, [r7, #8]
	ands	r3, r2
	mov	r2, sl
	orrs	r3, r0
	ldrh	r1, [r2, #12]
	mov	r2, r9
	strh	r3, [r7, #8]
	ldrh	r3, [r2, #4]
	lsls	r1, r1, #3
	lsrs	r3, r3, #8
	adds	r1, r1, r3
	ldr	r3, [pc, #28]
	adds	r1, #4
	ands	r1, r3
	ldr	r2, [pc, #24]
	ldrh	r3, [r7, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #6]
	mov	r3, r8
	b.n	.L2
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001e40
	.4byte 0x080c3734
	.4byte 0x03001ee4
	.4byte 0x0400004a
	.4byte 0x04000052
.L2:
	ldr	r0, [r3, #0]
	lsls	r0, r0, #12
	bl	Func_08002322
	cmp	r0, #0
	bge.n	.L3
	ldr	r1, [pc, #120]
	adds	r0, r0, r1
.L3:
	mov	r1, sl
	ldrh	r3, [r1, #14]
	asrs	r2, r0, #15
	lsls	r3, r3, #3
	mov	r1, r9
	adds	r2, r2, r3
	ldrh	r3, [r1, #6]
	lsrs	r3, r3, #8
	adds	r3, r3, r2
	adds	r3, #6
	strb	r3, [r7, #4]
	movs	r1, #240
	adds	r0, r7, #0
	bl	Func_08003dec
	ldr	r1, [pc, #92]
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L4
	ldr	r3, [pc, #84]
	ldr	r2, [pc, #84]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L4
	ldr	r3, [sp, #0]
	cmp	r3, #15
	ble.n	.L5
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L4
.L5:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [sp, #0]
	adds	r1, #1
	str	r1, [sp, #0]
	b.n	.L6
.L4:
	movs	r0, #111
	bl	Func_080f9010
	mov	r0, fp
	bl	Func_08003f3c
	movs	r0, #1
	bl	Func_080030f8
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00007fff
	.4byte 0x03001ae8
	.4byte 0x03001c94
	.4byte 0x00000303
