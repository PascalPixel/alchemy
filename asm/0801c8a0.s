@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801c8a0
	.thumb_func
Func_0801c8a0:
	push	{r5, r6, r7, lr}
	movs	r3, #0
	adds	r6, r0, #0
	str	r3, [r6, #0]
	adds	r7, r2, #0
	str	r3, [r1, #0]
	movs	r2, #136
	ldr	r3, [pc, #100]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #96]
	ldr	r5, [pc, #96]
	mov	lr, r1
	ands	r2, r3
	movs	r1, #0
	lsrs	r0, r3, #10
	adds	r4, r7, #0
.L2:
	ldrh	r3, [r4, #2]
	cmp	r3, r2
	bne.n	.L0
	ldrh	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L0
	str	r1, [r6, #0]
	b.n	.L1
.L0:
	adds	r1, #1
	adds	r4, #4
	cmp	r1, r5
	ble.n	.L2
.L1:
	ldr	r3, [pc, #64]
	ldr	r6, [pc, #56]
	ldr	r5, [pc, #56]
	movs	r1, #0
	mov	ip, r3
	adds	r0, r7, #0
.L5:
	mov	r2, ip
	ldrh	r4, [r2, #0]
	adds	r3, r6, #0
	ldrh	r2, [r0, #2]
	ands	r3, r4
	cmp	r2, r3
	bne.n	.L3
	ldrh	r2, [r0, #0]
	lsrs	r3, r4, #10
	cmp	r2, r3
	bne.n	.L3
	mov	r3, lr
	str	r1, [r3, #0]
	b.n	.L4
.L3:
	adds	r1, #1
	adds	r0, #4
	cmp	r1, r5
	ble.n	.L5
.L4:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x000003ff
	.4byte 0x000001bf
	.4byte 0x02000462
