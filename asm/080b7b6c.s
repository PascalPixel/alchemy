@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b7b6c
	.thumb_func
Func_080b7b6c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r1, [sp, #20]
	str	r0, [sp, #24]
	movs	r1, #0
	mov	fp, r1
.L2:
	ldr	r0, [sp, #24]
	mov	r1, fp
	bl	Func_080b770c
	cmp	r0, #0
	bne.n	.L0
	mov	r0, fp
	mov	r2, fp
	adds	r0, #120
	cmp	r2, #7
	bgt.n	.L1
	mov	r0, fp
.L1:
	bl	Func_080b7b30
.L0:
	movs	r3, #1
	add	fp, r3
	mov	r1, fp
	cmp	r1, #13
	ble.n	.L2
	ldr	r3, [pc, #144]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L3
	movs	r0, #1
	bl	Func_080030f8
.L3:
	ldr	r3, [sp, #24]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, #255
	bne.n	.L4
	b.n	.L5
.L4:
	ldr	r1, [sp, #24]
	str	r1, [sp, #8]
.L21:
	ldr	r2, [sp, #16]
	cmp	r2, #254
	bne.n	.L6
	b.n	.L7
.L6:
	adds	r0, r2, #0
	bl	Func_080b7dd0
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L8
	b.n	.L7
.L8:
	ldr	r0, [sp, #16]
	adds	r1, r7, #0
	bl	Func_080b78e4
	ldr	r3, [r7, #0]
	mov	r8, r3
	cmp	r3, #0
	bne.n	.L9
	b.n	.L7
.L9:
	mov	r1, r8
	adds	r1, #84
	ldrb	r2, [r1, #0]
	str	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L10
	b.n	.L7
.L10:
	ldrh	r2, [r7, #4]
	ldr	r3, [pc, #48]
	ands	r3, r2
	movs	r2, #238
	lsls	r2, r2, #1
	cmp	r3, r2
	beq.n	.L11
	adds	r2, #7
	cmp	r3, r2
	bne.n	.L12
.L11:
	ldr	r3, [pc, #36]
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #24]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	r9, r2
	movs	r3, #8
	add	r3, r9
	mov	sl, r3
	movs	r3, #2
	strb	r3, [r1, #0]
	mov	r2, r8
	ldrh	r5, [r7, #4]
	mov	r1, sl
	b.n	.L13
	movs	r0, r0
	.4byte 0x00000fff
	.4byte 0x03001a10
	.4byte 0x03001e68
.L13:
	str	r1, [r2, #80]
	ldr	r3, [pc, #376]
	movs	r1, #16
	mov	r0, sl
	bl	Func_080072f0
	adds	r0, r5, #0
	bl	Func_08009030
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L14
	ldr	r0, [r6, #24]
	ldr	r1, [r7, #24]
	ldr	r3, [pc, #352]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r6, #24]
	adds	r0, r5, #0
	bl	Func_08185000
	ldrb	r3, [r0, #9]
	mov	r1, r8
	lsrs	r3, r3, #1
	mov	r2, sl
	strh	r3, [r1, #32]
	movs	r3, #12
	add	r3, r9
	str	r6, [r2, #0]
	mov	sl, r3
.L14:
	add	r1, sp, #12
	adds	r3, r6, #0
	ldrb	r1, [r1, #0]
	ldr	r2, [pc, #316]
	adds	r3, #38
	strb	r1, [r3, #0]
	adds	r0, r5, r2
	bl	Func_08009030
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L15
	ldr	r0, [r6, #24]
	ldr	r1, [r7, #24]
	ldr	r3, [pc, #292]
	mov	ip, pc
	bx	r3
	mov	r1, sl
	str	r0, [r6, #24]
	str	r6, [r1, #0]
.L15:
	add	r2, sp, #12
	adds	r3, r6, #0
	ldrb	r2, [r2, #0]
	adds	r3, #38
	b.n	.L16
.L12:
	ldrh	r0, [r7, #4]
	str	r1, [sp, #4]
	bl	Func_08009030
	adds	r6, r0, #0
	ldr	r1, [sp, #4]
	cmp	r6, #0
	beq.n	.L17
	movs	r4, #1
	strb	r4, [r1, #0]
	mov	r3, r8
	str	r6, [r3, #80]
	ldr	r0, [r6, #24]
	ldr	r1, [r7, #24]
	ldr	r3, [pc, #240]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r6, #24]
	ldr	r5, [r6, #40]
	ldr	r3, [r7, #20]
	strb	r4, [r5, #6]
	strb	r3, [r5, #5]
	ldrh	r5, [r7, #6]
	cmp	r5, #0
	beq.n	.L18
	adds	r1, r5, #0
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl	Func_08009048
	ldr	r4, [sp, #0]
	adds	r5, r0, #0
	strb	r4, [r5, #6]
.L18:
	ldrh	r5, [r7, #8]
	cmp	r5, #0
	beq.n	.L19
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_08009048
	adds	r5, r0, #0
	str	r5, [r7, #32]
	movs	r1, #0
	bl	Func_08009070
	movs	r3, #3
	strb	r3, [r5, #6]
.L19:
	ldrh	r5, [r7, #10]
	cmp	r5, #0
	beq.n	.L17
	adds	r3, r6, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	cmp	r3, #32
	bne.n	.L20
	ldr	r5, [pc, #164]
.L20:
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_08009048
	add	r1, sp, #12
	ldrb	r1, [r1, #0]
	adds	r3, r6, #0
	adds	r5, r0, #0
	adds	r3, #38
	adds	r2, r1, #0
	str	r5, [r7, #36]
	strb	r1, [r5, #6]
.L16:
	strb	r2, [r3, #0]
.L17:
	ldr	r0, [sp, #16]
	bl	Func_080b7aac
.L7:
	ldr	r3, [sp, #8]
	movs	r1, #1
	add	fp, r1
	adds	r3, #2
	mov	r2, fp
	str	r3, [sp, #8]
	cmp	r2, #13
	bgt.n	.L5
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [sp, #16]
	cmp	r3, #255
	beq.n	.L5
	b.n	.L21
.L5:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	beq.n	.L22
	ldr	r2, [sp, #24]
	movs	r1, #0
	ldrsh	r5, [r2, r1]
	movs	r3, #0
	mov	fp, r3
	cmp	r5, #255
	beq.n	.L22
.L24:
	ldr	r1, [sp, #24]
	ldrsh	r3, [r3, r1]
	cmp	r3, #254
	beq.n	.L23
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L23
	ldr	r7, [r7, #0]
	mov	r8, r7
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L23
	adds	r0, r5, #0
	bl	Func_080b7aac
.L23:
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #13
	bgt.n	.L22
	ldr	r1, [sp, #24]
	lsls	r3, r2, #1
	ldrsh	r5, [r3, r1]
	cmp	r5, #255
	bne.n	.L24
.L22:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03000164
	.4byte 0x03000118
	.4byte 0x00002001
	.4byte 0x000001ff
