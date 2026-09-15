@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801f088
	.thumb_func
Func_0801f088:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r1, [sp, #16]
	mov	r9, r3
	ldr	r3, [pc, #300]
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	ldr	r1, [pc, #300]
	mov	r0, r9
	str	r3, [sp, #8]
	str	r0, [sp, #4]
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	adds	r5, r2, #0
	cmp	r3, #0
	bne.n	.L0
	bl	Func_080045e8
	ldr	r3, [pc, #280]
	ldr	r1, [pc, #284]
	ldr	r2, [pc, #284]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #284]
	ldr	r2, [pc, #284]
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #0]
.L0:
	ldrh	r3, [r6, #12]
	ldr	r2, [sp, #16]
	adds	r2, r2, r3
	str	r2, [sp, #16]
	ldrh	r3, [r6, #14]
	adds	r5, r5, r3
	lsls	r5, r5, #5
	movs	r3, #4
	str	r5, [sp, #0]
	str	r3, [sp, #12]
.L13:
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #8]
	adds	r3, r0, r1
	lsls	r3, r3, #1
	ldrh	r3, [r2, r3]
	mov	sl, r3
	ldr	r3, [pc, #248]
	mov	r8, r3
	ldr	r3, [pc, #248]
	mov	r1, sl
	ldr	r0, [pc, #248]
	ands	r1, r3
	mov	r2, r9
	mov	ip, r0
	mov	sl, r1
	cmp	r2, #7
	ble.n	.L1
	ldr	r3, [pc, #240]
	ldr	r0, [pc, #240]
	mov	r8, r3
	b.n	.L2
.L1:
	mov	r1, r9
	cmp	r1, #0
	blt.n	.L3
	lsls	r1, r1, #2
	mov	r2, r8
	lsls	r2, r1
	mov	r8, r2
	ldr	r3, [pc, #216]
	movs	r2, #32
	subs	r2, r2, r1
	lsrs	r3, r2
	mov	r0, r8
	orrs	r0, r3
	mov	r3, ip
	lsls	r3, r1
	mov	ip, r3
	ldr	r3, [pc, #204]
	mov	r8, r0
	lsrs	r3, r2
	mov	r0, ip
	orrs	r0, r3
.L2:
	mov	ip, r0
.L3:
	ldr	r2, [pc, #196]
	movs	r1, #0
	mov	lr, r1
	mov	fp, r2
	movs	r7, #0
	b.n	.L4
.L11:
	mov	r3, sl
	lsls	r6, r3, #5
	mov	r0, fp
	subs	r3, r6, r7
	ldr	r4, [r3, r0]
	movs	r1, #0
	movs	r0, #0
	movs	r5, #15
.L9:
	adds	r2, r4, #0
	ands	r2, r5
	cmp	r2, #14
	bne.n	.L5
	lsls	r2, r1, #2
	adds	r3, r5, #0
	lsls	r3, r2
	mov	r2, r8
	b.n	.L6
.L5:
	cmp	r2, #1
	bne.n	.L7
	lsls	r2, r1, #2
	adds	r3, r5, #0
	lsls	r3, r2
	mov	r2, ip
.L6:
	ands	r3, r2
	orrs	r0, r3
	b.n	.L8
.L7:
	lsls	r3, r1, #2
	lsls	r2, r3
	orrs	r0, r2
.L8:
	adds	r1, #1
	lsrs	r4, r4, #4
	cmp	r1, #7
	ble.n	.L9
	subs	r3, r6, r7
	mov	r1, fp
	movs	r2, #1
	str	r0, [r3, r1]
	adds	r7, #4
	add	lr, r2
.L4:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L10
	mov	r0, lr
	cmp	r0, #2
	ble.n	.L11
	b.n	.L12
.L10:
	mov	r1, lr
	cmp	r1, #0
	ble.n	.L11
.L12:
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #16]
	movs	r2, #8
	negs	r2, r2
	subs	r3, #1
	adds	r0, #1
	add	r9, r2
	str	r3, [sp, #12]
	str	r0, [sp, #16]
	cmp	r3, #0
	bge.n	.L13
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x00000ea5
	.4byte 0x040000d4
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x050001dc
	.4byte 0x22222222
	.4byte 0x000003ff
	.4byte 0xcccccccc
	.4byte 0x88888888
	.4byte 0xdddddddd
	.4byte 0x0600001c
