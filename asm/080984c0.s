@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080984c0
	.thumb_func
Func_080984c0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #180]
	adds	r2, r3, #0
	ldr	r1, [r3, #0]
	subs	r2, #100
	subs	r3, #116
	ldr	r6, [r3, #0]
	ldr	r7, [r2, #0]
	ldr	r2, [pc, #168]
	adds	r5, r6, r2
	mov	r8, r1
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	sub	sp, #4
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #167
	bl	Func_080f9010
	ldr	r0, [pc, #152]
	bl	Func_08004278
	ldr	r1, [pc, #148]
	movs	r2, #0
	strh	r2, [r5, #0]
	adds	r3, r6, r1
	movs	r5, #128
	strh	r2, [r3, #0]
	movs	r0, #0
	lsls	r5, r5, #9
	bl	Func_08098294
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08091200
	movs	r0, #1
	bl	Func_08091254
	movs	r0, #0
	movs	r1, #0
	bl	Func_08091220
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_08091200
	movs	r0, #30
	bl	Func_08091254
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [pc, #92]
	mov	r2, sp
	movs	r1, #8
	bl	Func_0808e4b4
	cmp	r0, #0
	beq.n	.L1
	ldr	r3, [pc, #80]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	ldr	r2, [sp, #0]
	bl	Func_08096b28
.L1:
	mov	r3, r8
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L0
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #56]
	adds	r2, r7, r3
	ldr	r3, [pc, #20]
	strb	r3, [r2, #0]
	adds	r3, r7, r1
	movs	r2, #1
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r0, #10
	bl	Func_080030f8
	b.n	.L0
	.4byte 0x00000000
	.4byte 0x03001f30
	.4byte 0x00000cb8
	.4byte 0x080982dd
	.4byte 0x00000cba
	.4byte 0x40000005
	.4byte 0x02000240
	.4byte 0x0000053e
	.4byte 0x0000053c
.L0:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
