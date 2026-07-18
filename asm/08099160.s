@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08099160
	.thumb_func
Func_08099160:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #364]
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #20]
	sub	sp, #16
	mov	r8, r3
	mov	r9, r2
	bl	Func_08097384
	movs	r0, #115
	bl	Func_080f9010
	movs	r3, #15
	add	r7, sp, #4
	mov	sl, r3
.L3:
	movs	r0, #232
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	bl	Func_08004458
	movs	r2, #128
	lsls	r2, r2, #8
	lsrs	r0, r0, #1
	adds	r0, r0, r2
	str	r0, [r6, #28]
	str	r0, [r6, #24]
	bl	Func_08004458
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L1
	ldr	r3, [pc, #292]
	b.n	.L2
.L1:
	ldr	r3, [pc, #292]
.L2:
	str	r3, [r6, #108]
	bl	Func_08004458
	adds	r2, r6, #0
	adds	r2, #100
	movs	r3, #60
	strh	r0, [r6, #6]
	strh	r3, [r2, #0]
	bl	Func_08004458
	adds	r3, r6, #0
	adds	r3, #102
	movs	r1, #9
	strh	r0, [r3, #0]
	adds	r0, r6, #0
	bl	Func_08009240
	mov	r2, r8
	ldr	r3, [r2, #4]
	str	r3, [r7, #0]
	ldr	r3, [r2, #8]
	str	r3, [r7, #4]
	ldr	r3, [r2, #12]
	str	r3, [r7, #8]
	bl	Func_08004458
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #10
	lsls	r5, r5, #2
	adds	r5, r5, r3
	bl	Func_08004458
	adds	r2, r7, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r3, [r7, #0]
	str	r3, [r6, #56]
	ldr	r3, [r7, #4]
	str	r3, [r6, #60]
	ldr	r3, [r7, #8]
	str	r3, [r6, #64]
.L0:
	movs	r0, #3
	bl	Func_080030f8
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L3
	movs	r0, #10
	bl	Func_080030f8
	movs	r0, #115
	bl	Func_080f9010
	movs	r0, #50
	bl	Func_080030f8
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L4
	mov	r3, r8
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L4
	movs	r0, #212
	bl	Func_080f9010
	movs	r3, #15
	mov	sl, r3
.L5:
	movs	r1, #7
	mov	r0, r9
	bl	Func_08009240
	movs	r0, #1
	bl	Func_080030f8
	mov	r0, r9
	movs	r1, #0
	bl	Func_08009240
	movs	r0, #4
	bl	Func_080030f8
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L5
	mov	r3, r8
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L6
	movs	r0, #220
	bl	Func_080f9010
	mov	r0, r9
	movs	r1, #2
	bl	Func_08009080
.L6:
	ldr	r3, [pc, #72]
	mov	r2, r9
	str	r3, [r2, #108]
	ldr	r0, [pc, #72]
	mov	r2, sp
	movs	r1, #6
	bl	Func_0808e4b4
	cmp	r0, #0
	beq.n	.L7
	ldr	r3, [pc, #60]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	ldr	r2, [sp, #0]
	bl	Func_08096b28
.L7:
	movs	r0, #20
	bl	Func_080030f8
.L4:
	bl	Func_0809748c
	add	sp, #16
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x08099071
	.4byte 0x080990cd
	.4byte 0x08099019
	.4byte 0x50000005
	.4byte 0x02000240
