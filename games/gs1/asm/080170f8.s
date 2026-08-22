@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080170f8
	.thumb_func
Func_080170f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r3, #0
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	mov	fp, r3
	lsls	r3, r1, #5
	adds	r3, r3, r0
	mov	r8, r2
	lsls	r3, r3, #1
	mov	r2, fp
	adds	r5, r3, r2
	mov	r3, r8
	cmp	r3, #1
	bhi.n	.L0
	b.n	.L1
.L0:
	cmp	r7, #1
	bhi.n	.L2
	b.n	.L1
.L2:
	cmp	r3, #30
	bls.n	.L3
	b.n	.L1
.L3:
	cmp	r7, #30
	bls.n	.L4
	b.n	.L1
.L4:
	adds	r3, r7, #0
	mov	r2, r8
	bl	Func_0801e260
	ldr	r3, [pc, #64]
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L5
	ldr	r3, [pc, #40]
	b.n	.L6
.L5:
	ldr	r3, [pc, #40]
.L6:
	strh	r3, [r5, #0]
	adds	r5, #2
	movs	r2, #2
	negs	r2, r2
	add	r2, r8
	adds	r0, r5, #0
	ldr	r1, [pc, #40]
	mov	sl, r2
	bl	Func_080170c4
	ldr	r3, [pc, #28]
	add	r3, fp
	ldrb	r3, [r3, #0]
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L7
	ldr	r3, [pc, #8]
	b.n	.L8
	.4byte 0x0000f01c
	.4byte 0x0000f010
	.4byte 0x0000f41c
	.4byte 0x03001e8c
	.4byte 0x00000ea4
	.4byte 0xf011f011
.L7:
	ldr	r3, [pc, #48]
.L8:
	strh	r3, [r5, #0]
	adds	r5, #2
	movs	r3, #32
	mov	r2, r8
	subs	r3, r3, r2
	lsls	r3, r3, #1
	movs	r6, #1
	subs	r7, #1
	adds	r5, r5, r3
	cmp	r6, r7
	bcs.n	.L9
	mov	r9, r3
.L12:
	ldr	r3, [pc, #28]
	mov	r2, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r2, #2
	beq.n	.L10
	adds	r0, r5, #0
	ldr	r1, [pc, #16]
	mov	r2, sl
	bl	Func_080170c4
	b.n	.L11
	movs	r0, r0
	.4byte 0x0000f012
	.4byte 0x0000f016
	.4byte 0xf020f020
.L11:
	adds	r5, r0, #0
.L10:
	ldr	r3, [pc, #28]
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	add	r5, r9
	cmp	r6, r7
	bcc.n	.L12
.L9:
	ldr	r3, [pc, #20]
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L13
	ldr	r3, [pc, #8]
	b.n	.L14
	movs	r0, r0
	.4byte 0x0000f017
	.4byte 0x0000f81c
	.4byte 0x00000ea4
.L13:
	ldr	r3, [pc, #28]
.L14:
	strh	r3, [r5, #0]
	adds	r5, #2
	adds	r0, r5, #0
	ldr	r1, [pc, #28]
	mov	r2, sl
	bl	Func_080170c4
	ldr	r3, [pc, #24]
	add	r3, fp
	ldrb	r3, [r3, #0]
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L15
	ldr	r3, [pc, #4]
	b.n	.L16
	.4byte 0x0000f013
	.4byte 0x0000fc1c
	.4byte 0xf014f014
	.4byte 0x00000ea4
.L15:
	ldr	r3, [pc, #24]
.L16:
	strh	r3, [r5, #0]
	ldr	r2, [pc, #24]
	movs	r3, #1
	add	r2, fp
	strb	r3, [r2, #0]
.L1:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000f015
	.4byte 0x00000ea3
