@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f731c
	.thumb_func
Func_080f731c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	ldr	r1, [r3, #0]
	subs	r3, #24
	ldr	r3, [r3, #0]
	movs	r2, #128
	mov	sl, r3
	ldr	r3, [pc, #136]
	mov	r8, r1
	sub	sp, #4
	movs	r5, #0
	movs	r6, #0
	movs	r1, #0
	lsls	r2, r2, #4
.L0:
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r6, r2
	bne.n	.L0
	movs	r2, #225
	lsls	r2, r2, #7
	ldr	r0, [pc, #112]
	ldr	r1, [pc, #116]
	ldr	r7, [pc, #116]
	movs	r6, #0
	movs	r4, #0
	add	r2, sl
.L1:
	adds	r3, r5, #0
	adds	r3, #24
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	ldrb	r3, [r1, #0]
	adds	r6, #1
	str	r0, [r2, #4]
	str	r4, [r2, #16]
	str	r4, [r2, #24]
	adds	r1, #1
	adds	r5, r5, r3
	adds	r0, r0, r7
	adds	r2, #28
	cmp	r6, #8
	bne.n	.L1
	movs	r3, #155
	lsls	r3, r3, #3
	ldr	r2, [pc, #56]
	movs	r6, #0
	add	r3, r8
.L2:
	adds	r6, #1
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r6, #160
	bne.n	.L2
	ldr	r7, [pc, #64]
	movs	r6, #0
	add	r7, r8
.L4:
	ldr	r3, [pc, #60]
	adds	r5, r6, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	Func_0800231c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r3, r3, #15
	strh	r3, [r7, #0]
	adds	r0, r5, #0
	bl	Func_0800231c
	movs	r2, #110
	subs	r2, r2, r6
	movs	r3, #155
	lsls	r3, r3, #3
	b.n	.L3
	.4byte 0x00000000
	.4byte 0x03001f04
	.4byte 0x02010018
	.4byte 0xffe00000
	.4byte 0x080f8736
	.4byte 0xfff80000
	.4byte 0x00000506
	.4byte 0x00000199
.L3:
	lsls	r2, r2, #1
	adds	r2, r2, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r3, r3, #15
	mov	r1, r8
	adds	r6, #1
	adds	r7, #2
	strh	r3, [r1, r2]
	cmp	r6, #40
	bne.n	.L4
	mov	r3, r8
	movs	r2, #0
	adds	r3, #148
	str	r2, [r3, #0]
	movs	r1, #239
	subs	r3, #8
	str	r2, [r3, #0]
	lsls	r1, r1, #7
	adds	r3, #4
	str	r2, [r3, #0]
	add	r1, sl
	movs	r3, #1
	str	r3, [r1, #0]
	ldr	r3, [pc, #72]
	add	r3, sl
	str	r2, [r3, #0]
	ldr	r3, [pc, #72]
	strh	r2, [r3, #0]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #12
	movs	r3, #4
	movs	r0, #18
	bl	Func_08015010
	ldr	r6, [pc, #56]
	ldr	r5, [pc, #56]
	adds	r1, r0, #0
	add	r6, r8
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	subs	r5, #1
	bl	Func_08015080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x04000050
	.4byte 0x000004cc
	.4byte 0x00000905
