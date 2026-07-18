@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cdd58
	.thumb_func
Func_080cdd58:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #104]
	ldr	r2, [r3, #124]
	ldr	r5, [r3, #0]
	adds	r3, #140
	ldr	r6, [r3, #0]
	mov	r8, r2
	bl	Func_080cd508
	movs	r3, #201
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrh	r1, [r5, #0]
	movs	r0, #2
	movs	r2, #0
	bl	Func_080b5038
	ldr	r2, [pc, #76]
	movs	r1, #0
	movs	r3, #32
	strh	r3, [r2, #6]
	str	r1, [r6, #12]
	ldr	r3, [pc, #68]
	movs	r1, #64
	ldr	r0, [pc, #68]
	bl	Func_080072f0
	movs	r1, #128
	movs	r2, #1
	negs	r2, r2
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #64]
	lsls	r1, r1, #2
	bl	Func_080072f0
	movs	r2, #128
	lsls	r2, r2, #1
	ldr	r5, [pc, #56]
	ldr	r7, [pc, #24]
	movs	r0, #0
	movs	r6, #0
	mov	ip, r2
	movs	r4, #0
.L3:
	mov	r3, ip
	movs	r1, #0
	adds	r2, r4, r3
.L2:
	cmp	r1, #15
	ble.n	.L0
	adds	r3, r0, r5
	strh	r7, [r3, #0]
	b.n	.L1
	movs	r0, r0
	.4byte 0x000000ff
	.4byte 0x03001e74
	.4byte 0x03001ad0
	.4byte 0x03000164
	.4byte 0x06003fc0
	.4byte 0x03000168
	.4byte 0x0600f900
	.4byte 0x0600fb00
.L0:
	adds	r3, r0, r5
	strh	r2, [r3, #0]
.L1:
	adds	r1, #1
	adds	r2, #1
	adds	r0, #2
	cmp	r1, #32
	bne.n	.L2
	adds	r6, #1
	adds	r4, #16
	cmp	r6, #16
	bne.n	.L3
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #10
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r1, [pc, #40]
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #40]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	b.n	.L4
	.4byte 0x00007741
	.4byte 0x00001f81
	.4byte 0x00003f42
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.4byte 0x04000040
	.4byte 0x04000048
.L4:
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #8
	movs	r1, #128
	strh	r3, [r2, #0]
	ldr	r5, [pc, #28]
	mov	r0, r8
	lsls	r1, r1, #7
	bl	Func_080072f8
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [pc, #20]
	bl	Func_080072f8
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L5
	.4byte 0x0000100e
	.4byte 0x03000164
	.4byte 0x06004000
.L5:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
