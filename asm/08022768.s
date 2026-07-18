@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08022768
	.thumb_func
Func_08022768:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r4, r0, #0
	adds	r0, r3, #0
	ldr	r3, [pc, #152]
	adds	r7, r2, #0
	ldr	r2, [sp, #28]
	ldr	r3, [r3, #0]
	mov	ip, r2
	mov	r5, ip
	mov	r8, r3
	movs	r3, #1
	ands	r5, r3
	lsls	r5, r5, #12
	mov	ip, r5
	cmp	r4, #0
	bge.n	.L0
	adds	r7, r7, r4
	movs	r4, #0
.L0:
	adds	r3, r4, r7
	cmp	r3, #29
	ble.n	.L1
	movs	r3, #30
	subs	r7, r3, r4
.L1:
	cmp	r1, #0
	bge.n	.L2
	adds	r0, r0, r1
	movs	r1, #0
.L2:
	adds	r3, r1, r0
	cmp	r3, #29
	ble.n	.L3
	movs	r3, #20
	subs	r0, r3, r1
.L3:
	cmp	r7, #0
	ble.n	.L4
	cmp	r0, #0
	ble.n	.L4
	ldr	r2, [pc, #88]
	add	r2, r8
	lsls	r3, r4, #1
	mov	lr, r2
	lsls	r2, r1, #6
	adds	r6, r2, r3
	movs	r3, #2
	mov	sl, r3
.L7:
	mov	r5, r8
	adds	r2, r7, #0
	adds	r4, r6, r5
	cmp	r2, #0
	beq.n	.L5
	ldr	r3, [pc, #68]
	mov	r9, r3
.L6:
	ldrh	r3, [r4, #0]
	mov	r5, r9
	ands	r3, r5
	mov	r5, ip
	orrs	r3, r5
	subs	r2, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r2, #0
	bne.n	.L6
.L5:
	lsrs	r3, r1, #2
	mov	r4, lr
	mov	r2, sl
	lsls	r2, r3
	ldrb	r3, [r4, #0]
	orrs	r2, r3
	strb	r2, [r4, #0]
	subs	r0, #1
	adds	r6, #64
	adds	r1, #1
	cmp	r0, #0
	bne.n	.L7
.L4:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0xffffefff
