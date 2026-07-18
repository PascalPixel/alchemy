@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa8d4
	.thumb_func
Func_080fa8d4:
	push	{r4, r5, r6, r7, lr}
	ldr	r0, [pc, #72]
	ldr	r6, [r0, #0]
	ldr	r1, [r6, #0]
	ldr	r0, [pc, #68]
	cmp	r1, r0
	bne.n	.L0
	adds	r0, r1, #1
	str	r0, [r6, #0]
	movs	r5, #12
	adds	r4, r6, #0
	adds	r4, #80
	movs	r0, #0
.L1:
	strb	r0, [r4, #0]
	subs	r5, #1
	adds	r4, #64
	cmp	r5, #0
	bgt.n	.L1
	ldr	r4, [r6, #28]
	cmp	r4, #0
	beq.n	.L2
	movs	r5, #1
	movs	r7, #0
.L3:
	lsls	r0, r5, #24
	lsrs	r0, r0, #24
	ldr	r1, [r6, #44]
	bl	Func_080072e8
	strb	r7, [r4, #0]
	adds	r5, #1
	adds	r4, #64
	cmp	r5, #4
	ble.n	.L3
.L2:
	ldr	r0, [pc, #12]
	str	r0, [r6, #0]
.L0:
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03007ff0
	.4byte 0x68736d53
