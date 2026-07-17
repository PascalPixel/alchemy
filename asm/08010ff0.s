@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08010ff0
	.thumb_func
Func_08010ff0:
	push	{r5, r6, lr}
	ldr	r1, [pc, #204]
	movs	r2, #200
	ldmia	r1!, {r3}
	lsls	r2, r2, #4
	adds	r6, r3, r2
	movs	r3, #128
	lsls	r3, r3, #19
	ldrh	r2, [r3, #0]
	ldr	r1, [r1, #0]
	ldr	r3, [pc, #188]
	mov	ip, r1
	ands	r3, r2
	ldr	r1, [pc, #188]
	lsls	r3, r3, #16
	ldrh	r2, [r1, #10]
	asrs	r5, r3, #16
	ldr	r3, [pc, #184]
	ands	r3, r2
	strh	r3, [r1, #10]
	ldr	r3, [pc, #180]
	ldrh	r2, [r1, #10]
	ands	r3, r2
	strh	r3, [r1, #10]
	ldr	r4, [pc, #176]
	ldrh	r3, [r1, #10]
	cmp	r6, #0
	beq.n	.L0
	ldr	r3, [pc, #172]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #10
	adds	r0, r6, r0
	ldmia	r0!, {r3}
	str	r3, [r4, #0]
	ldmia	r0!, {r3}
	adds	r4, #4
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	ldr	r2, [pc, #128]
	str	r3, [r4, #0]
	adds	r3, r1, #0
	subs	r1, #144
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	movs	r3, #128
	lsls	r3, r3, #1
	add	r3, ip
	movs	r2, #130
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #1
	add	r2, ip
	strh	r3, [r2, #0]
	movs	r3, #129
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r0, [r3, #0]
	movs	r3, #131
	lsls	r3, r3, #1
	add	r3, ip
	strh	r0, [r3, #0]
	ldrh	r1, [r2, #0]
	movs	r3, #0
	cmp	r1, #199
	bhi.n	.L1
	lsls	r2, r0, #16
	lsrs	r2, r2, #16
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	lsls	r3, r3, #1
	cmp	r1, r2
	bhi.n	.L1
	movs	r3, #0
	cmp	r1, #0
	bne.n	.L1
	movs	r3, #2
.L1:
	orrs	r5, r3
	lsls	r3, r5, #16
	movs	r2, #128
	lsrs	r3, r3, #16
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #1
	add	r2, ip
	movs	r3, #0
	strh	r3, [r2, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e6c
	.4byte 0x0000fff8
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x04000020
	.4byte 0x03001e40
	.4byte 0xa6600008
