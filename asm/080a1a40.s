@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a1a40
	.thumb_func
Func_080a1a40:
	push	{r5, r6, lr}
	ldr	r3, [pc, #96]
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #96]
	mov	lr, r3
	ldr	r3, [r3, #0]
	movs	r6, #7
	lsrs	r3, r3, #1
	mov	ip, r6
	ands	r3, r6
	ldr	r2, [pc, #84]
	ldr	r6, [r5, #16]
	ldrb	r2, [r2, r3]
	ldrh	r3, [r6, #12]
	adds	r2, r2, r0
	lsls	r3, r3, #3
	ldr	r4, [r5, #20]
	adds	r2, r2, r3
	ldr	r5, [pc, #52]
	adds	r2, #8
	ldr	r3, [pc, #52]
	strh	r2, [r4, #6]
	ands	r2, r5
	ldrh	r0, [r4, #22]
	ands	r2, r3
	ldr	r3, [pc, #60]
	ands	r3, r0
	orrs	r3, r2
	mov	r0, lr
	strh	r3, [r4, #22]
	ldr	r3, [r0, #0]
	ldr	r2, [pc, #52]
	mov	r0, ip
	lsrs	r3, r3, #1
	ands	r3, r0
	ldrb	r3, [r2, r3]
	ldrh	r2, [r6, #14]
	adds	r3, r3, r1
	lsls	r2, r2, #3
	adds	r3, r3, r2
	adds	r3, #8
	strh	r3, [r4, #8]
	ands	r3, r5
	strb	r3, [r4, #20]
	b.n	.L0
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x03001f2c
	.4byte 0x03001e40
	.4byte 0x080af294
	.4byte 0xfffffe00
	.4byte 0x080af29d
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
