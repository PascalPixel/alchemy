@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800655c
	.thumb_func
Func_0800655c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #68]
	ldr	r3, [r3, #0]
	lsls	r3, r3, #26
	lsrs	r3, r3, #30
	movs	r2, #1
	bics	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #56]
	lsls	r3, r3, #3
	adds	r3, r3, r2
	mov	ip, r3
	ldr	r3, [pc, #52]
	ldrh	r2, [r3, #0]
	ldr	r1, [pc, #36]
	movs	r3, #3
	ands	r3, r2
	mov	r8, r1
	ldr	r6, [pc, #44]
	cmp	r3, #3
	beq.n	.L0
	b.n	.L1
.L0:
	ldr	r7, [pc, #40]
	ldr	r5, [r7, #0]
	cmp	r5, #0
	bne.n	.L2
	b.n	.L3
.L2:
	ldrb	r3, [r6, #2]
	cmp	r3, #1
	beq.n	.L4
	b.n	.L5
.L4:
	b.n	.L6
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x04000128
	.4byte 0x02002020
	.4byte 0x03001f64
	.4byte 0x02002220
	.4byte 0x020023ac
.L6:
	mov	r2, ip
	ldrb	r3, [r2, #3]
	movs	r1, #128
	adds	r3, #255
	lsls	r3, r3, #24
	lsls	r1, r1, #17
	cmp	r3, r1
	bhi.n	.L5
	ldr	r0, [pc, #424]
	movs	r2, #127
	mov	lr, r2
	mov	r3, ip
	ldrb	r1, [r0, #0]
	ldrb	r2, [r3, #0]
	mov	r3, lr
	ands	r3, r1
	cmp	r2, r3
	bne.n	.L7
	movs	r1, #0
	mov	lr, r1
	mov	r2, lr
	strb	r2, [r6, #0]
	mov	r3, ip
	ldrb	r4, [r3, #3]
	cmp	r4, #1
	beq.n	.L8
	cmp	r4, #2
	beq.n	.L9
	b.n	.L10
.L8:
	mov	r0, ip
	ldr	r3, [pc, #384]
	adds	r0, #4
	adds	r1, r5, #0
	ldr	r2, [pc, #384]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r7, #0]
	ldr	r2, [pc, #380]
	adds	r3, #20
	str	r3, [r7, #0]
	ldrh	r3, [r2, #0]
	adds	r3, #20
	strh	r3, [r2, #0]
	movs	r1, #128
	ldrb	r3, [r6, #1]
	negs	r1, r1
	adds	r3, #1
	adds	r2, r1, #0
	orrs	r3, r2
	strb	r3, [r6, #1]
	b.n	.L10
.L9:
	mov	r0, ip
	ldr	r3, [pc, #340]
	adds	r0, #4
	adds	r1, r5, #0
	ldr	r2, [pc, #340]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #336]
	ldrh	r3, [r2, #0]
	adds	r3, #20
	strh	r3, [r2, #0]
	mov	r2, lr
	mov	r3, r8
	strb	r4, [r6, #2]
	strb	r2, [r6, #1]
	strb	r3, [r6, #0]
.L10:
	ldr	r3, [pc, #308]
	ldrb	r2, [r3, #0]
	movs	r1, #127
	adds	r2, #1
	ands	r2, r1
	strb	r2, [r3, #0]
	b.n	.L3
.L7:
	ldrb	r2, [r0, #0]
	movs	r4, #128
	adds	r3, r4, #0
	ands	r3, r2
	movs	r1, #128
	cmp	r3, #0
	beq.n	.L11
	ldrb	r1, [r6, #0]
	adds	r3, r4, #0
	ands	r3, r1
	lsls	r3, r3, #24
	lsrs	r2, r3, #24
	cmp	r2, #0
	beq.n	.L12
	mov	r1, r8
	strb	r1, [r6, #0]
	b.n	.L3
.L12:
	lsls	r3, r1, #24
	movs	r1, #128
	lsls	r1, r1, #17
	cmp	r3, r1
	bne.n	.L3
	strb	r2, [r6, #0]
	ldrb	r2, [r0, #0]
	mov	r3, lr
	ands	r3, r2
	strb	r3, [r0, #0]
	b.n	.L3
.L11:
	ldrb	r3, [r0, #0]
	orrs	r3, r1
	strb	r3, [r6, #0]
	ldrb	r3, [r0, #0]
	orrs	r3, r1
	strb	r3, [r0, #0]
	b.n	.L3
.L5:
	movs	r3, #0
	strb	r3, [r6, #0]
.L3:
	ldr	r7, [pc, #236]
	ldr	r0, [r7, #0]
	cmp	r0, #0
	beq.n	.L13
	mov	r2, ip
	ldrb	r2, [r2, #2]
	mov	lr, r2
	cmp	r2, #1
	bne.n	.L14
	mov	r3, ip
	ldrb	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L15
	ldr	r5, [pc, #188]
	mov	r2, ip
	ldrb	r1, [r5, #0]
	ldrb	r3, [r2, #0]
	movs	r4, #127
	subs	r1, r1, r3
	ands	r1, r4
	lsls	r2, r1, #2
	adds	r2, r2, r1
	lsls	r2, r2, #2
	subs	r3, r0, r2
	ldr	r0, [pc, #188]
	str	r3, [r7, #0]
	ldrh	r3, [r0, #0]
	adds	r3, r3, r2
	strh	r3, [r0, #0]
	ldrb	r3, [r5, #0]
	subs	r3, r3, r1
	strb	r3, [r5, #0]
	ldrb	r3, [r5, #0]
	ands	r4, r3
	strb	r4, [r5, #0]
.L15:
	ldr	r4, [pc, #164]
	ldrh	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L14
	ldr	r3, [pc, #140]
	ldr	r0, [r7, #0]
	adds	r1, r6, #4
	ldr	r2, [pc, #140]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r3, [r4, #0]
	ldr	r1, [pc, #148]
	adds	r3, r3, r1
	strh	r3, [r4, #0]
	ldrh	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L16
	mov	r2, lr
	ldr	r3, [pc, #108]
	strb	r2, [r6, #3]
	b.n	.L17
.L16:
	movs	r3, #2
	strb	r3, [r6, #3]
	ldr	r3, [pc, #100]
.L17:
	ldrb	r2, [r3, #0]
	movs	r3, #127
	ands	r3, r2
	strb	r3, [r6, #0]
	ldr	r3, [r7, #0]
	adds	r3, #20
	str	r3, [r7, #0]
	ldr	r3, [pc, #84]
	ldrb	r2, [r3, #0]
	movs	r1, #127
	adds	r2, #1
	ands	r2, r1
	strb	r2, [r3, #0]
.L14:
	ldrb	r3, [r6, #3]
	cmp	r3, #2
	bne.n	.L13
	mov	r1, ip
	ldrb	r3, [r1, #2]
	cmp	r3, #2
	bne.n	.L13
	ldr	r2, [pc, #72]
	movs	r3, #0
	str	r3, [r2, #0]
	strb	r3, [r6, #3]
	movs	r3, #1
	strb	r3, [r6, #0]
.L13:
	ldrb	r3, [r6, #2]
	cmp	r3, #2
	bne.n	.L18
	mov	r2, ip
	ldrb	r3, [r2, #3]
	cmp	r3, #2
	beq.n	.L1
	ldr	r2, [pc, #60]
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L19
.L18:
	movs	r3, #0
	strb	r3, [r6, #2]
	ldr	r3, [pc, #48]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #1
.L19:
	strb	r3, [r6, #2]
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x020023a4
	.4byte 0x040000d4
	.4byte 0x84000005
	.4byte 0x02002238
	.4byte 0x02002080
	.4byte 0x02002008
	.4byte 0x0000ffec
	.4byte 0x020023ac
