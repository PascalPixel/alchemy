@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800ff54
	.thumb_func
Func_0800ff54:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #144]
	lsls	r0, r0, #11
	adds	r3, r3, r0
	mov	lr, r3
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	movs	r4, #127
	asrs	r3, r3, #1
	ands	r3, r4
	lsls	r6, r3, #7
	lsrs	r3, r1, #31
	movs	r0, #30
	adds	r3, r1, r3
	ands	r2, r0
	asrs	r7, r3, #1
	mov	r9, r1
	lsls	r5, r2, #5
	ands	r7, r4
	movs	r2, #1
	mov	r4, r9
	ands	r4, r0
	mov	ip, r2
	mov	r3, ip
	mov	r9, r4
	movs	r2, #240
	movs	r4, #254
	ands	r3, r1
	lsls	r4, r4, #6
	lsls	r2, r2, #2
	mov	ip, r3
	movs	r0, #0
	mov	sl, r4
	mov	r8, r2
.L0:
	adds	r3, r6, r7
	ldr	r4, [pc, #80]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldr	r1, [r3, #0]
	lsls	r1, r1, #20
	lsrs	r1, r1, #18
	ldr	r3, [pc, #72]
	add	r1, ip
	lsls	r1, r1, #1
	mov	r4, r9
	adds	r2, r1, r3
	adds	r3, r5, r4
	add	r3, ip
	ldrh	r2, [r2, #0]
	lsls	r3, r3, #1
	add	r3, lr
	ldr	r4, [pc, #56]
	strh	r2, [r3, #0]
	adds	r2, r1, r4
	ldrh	r2, [r2, #0]
	adds	r3, #64
	strh	r2, [r3, #0]
	adds	r6, #128
	mov	r2, sl
	adds	r5, #64
	mov	r3, r8
	adds	r0, #1
	ands	r6, r2
	ands	r5, r3
	cmp	r0, #10
	bls.n	.L0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x06002800
	.4byte 0x02010000
	.4byte 0x02020000
	.4byte 0x02020004
