@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801cbd4
	.thumb_func
Func_0801cbd4:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r0, [pc, #104]
	adds	r6, r2, #0
	adds	r2, r5, r0
	ldrh	r0, [r2, #0]
	ldr	r4, [pc, #100]
	lsls	r0, r0, #16
	mov	ip, pc
	bx	r4
	movs	r1, #175
	lsls	r1, r1, #3
	adds	r2, r5, r1
	asrs	r7, r0, #16
	ldrh	r0, [r2, #0]
	adds	r1, r6, #0
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	ldr	r2, [pc, #76]
	adds	r5, r5, r2
	asrs	r6, r0, #16
	ldrh	r0, [r5, #0]
	adds	r1, r3, #0
	lsls	r0, r0, #16
	mov	ip, pc
	bx	r4
	asrs	r0, r0, #16
	cmp	r7, #0
	bge.n	.L0
	movs	r7, #0
.L0:
	cmp	r6, #0
	bge.n	.L1
	movs	r6, #0
.L1:
	cmp	r0, #0
	bge.n	.L2
	movs	r0, #0
.L2:
	cmp	r7, #31
	ble.n	.L3
	movs	r7, #31
.L3:
	cmp	r6, #31
	ble.n	.L4
	movs	r6, #31
.L4:
	cmp	r0, #31
	ble.n	.L5
	movs	r0, #31
.L5:
	lsls	r3, r6, #5
	lsls	r0, r0, #10
	adds	r0, r0, r3
	adds	r0, r7, r0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000576
	.4byte 0x03000118
	.4byte 0x0000057a
