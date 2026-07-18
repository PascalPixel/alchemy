@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808ee0c
	.thumb_func
Func_0808ee0c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #180]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl	Func_0808ba1c
	ldr	r3, [pc, #168]
	movs	r6, #142
	ldr	r3, [r3, #0]
	lsls	r6, r6, #1
	adds	r4, r3, r6
	ldrb	r3, [r4, #4]
	adds	r5, r0, #0
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L0
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #16]
	mov	r9, r2
	mov	sl, r3
	ldr	r6, [pc, #144]
	ldr	r2, [pc, #144]
	movs	r3, #128
	lsls	r3, r3, #12
	mov	lr, r6
	mov	ip, r2
	mov	fp, r3
.L2:
	ldrb	r3, [r4, #6]
	mov	r6, r9
	lsls	r0, r3, #20
	subs	r2, r6, r0
	adds	r3, r2, #0
	add	r3, lr
	mov	r8, r3
	ldrb	r3, [r4, #7]
	mov	r6, sl
	lsls	r1, r3, #20
	subs	r3, r6, r1
	mov	r6, lr
	adds	r6, r3, r6
	str	r6, [sp, #0]
	ldr	r6, [pc, #108]
	adds	r2, r2, r6
	cmp	r2, ip
	bhi.n	.L1
	adds	r3, r3, r6
	cmp	r3, ip
	bhi.n	.L1
	mov	r2, fp
	adds	r3, r0, r2
	str	r3, [r5, #8]
	adds	r3, r1, r2
	str	r3, [r5, #16]
	ldr	r0, [sp, #0]
	mov	r1, r8
	bl	Func_080044d0
	adds	r1, r0, #0
	lsls	r1, r1, #16
	movs	r0, #160
	adds	r2, r5, #0
	lsrs	r1, r1, #16
	lsls	r0, r0, #13
	adds	r2, #8
	bl	Func_0800447c
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	str	r3, [r5, #64]
	b.n	.L0
.L1:
	adds	r7, #1
	adds	r4, #8
	cmp	r7, #9
	bgt.n	.L0
	ldrb	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L2
.L0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0xfff80000
	.4byte 0x001ffffe
	.4byte 0x0007ffff
