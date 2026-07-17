@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809bb64
	.thumb_func
Func_0809bb64:
	push	{r5, r6, r7, lr}
	movs	r0, #32
	sub	sp, #12
	bl	Func_08004970
	ldr	r7, [pc, #312]
	adds	r1, r7, #0
	adds	r1, #32
	ldr	r3, [pc, #308]
	movs	r2, #250
	str	r1, [sp, #4]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	Func_08092054
	adds	r6, r0, #0
	bl	Func_08004080
	movs	r3, #0
	strh	r0, [r7, #0]
	add	r0, sp, #8
	str	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #280]
	ldr	r2, [pc, #280]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #255
	str	r3, [r5, #0]
	ldr	r3, [pc, #276]
	str	r3, [r5, #4]
	movs	r3, #68
	str	r3, [r5, #32]
	movs	r3, #162
	lsls	r3, r3, #1
	str	r3, [r5, #36]
	movs	r3, #119
	str	r3, [r5, #64]
	ldr	r3, [pc, #260]
	str	r3, [r5, #68]
	movs	r3, #255
	lsls	r3, r3, #4
	str	r3, [r5, #96]
	ldr	r3, [pc, #256]
	str	r3, [r5, #100]
	ldr	r3, [pc, #256]
	str	r3, [r5, #104]
	ldr	r3, [pc, #256]
	movs	r2, #136
	str	r3, [r5, #108]
	movs	r3, #136
	lsls	r2, r2, #1
	lsls	r3, r3, #5
	str	r2, [r5, #8]
	str	r2, [r5, #40]
	str	r2, [r5, #72]
	str	r3, [r5, #112]
	movs	r1, #128
	adds	r2, r5, #0
	ldrh	r0, [r7, #0]
	bl	Func_08003fa4
	movs	r3, #128
	lsls	r3, r3, #3
	orrs	r0, r3
	ldr	r3, [sp, #4]
	movs	r2, #0
	movs	r1, #0
.L0:
	str	r1, [r3, #0]
	str	r1, [r3, #4]
	str	r0, [r3, #8]
	ldr	r4, [sp, #4]
	adds	r2, #1
	adds	r4, #12
	adds	r3, #12
	str	r4, [sp, #4]
	cmp	r2, #65
	bls.n	.L0
	adds	r0, r5, #0
	bl	Func_08002df0
	movs	r0, #142
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L1
	movs	r3, #240
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	movs	r3, #160
	lsls	r3, r3, #15
	b.n	.L2
.L1:
	ldr	r2, [r6, #8]
	movs	r1, #240
	lsls	r1, r1, #24
	adds	r2, r2, r1
	asrs	r2, r2, #16
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L3
	ldr	r2, [pc, #148]
	adds	r3, r3, r2
.L3:
	asrs	r3, r3, #12
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	movs	r3, #18
	ldrsh	r2, [r6, r3]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #5
	cmp	r0, #0
	bge.n	.L4
	ldr	r4, [pc, #124]
	adds	r0, r0, r4
.L4:
	asrs	r3, r0, #12
	lsls	r3, r3, #16
.L2:
	str	r3, [r7, #8]
	bl	Func_080153b8
	strh	r0, [r7, #2]
	ldrh	r3, [r7, #2]
	ldr	r2, [pc, #108]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r5, [r3, #2]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	movs	r0, #0
	bl	Func_08015010
	ldr	r3, [pc, #68]
	strh	r3, [r7, #18]
	adds	r3, #1
	str	r3, [r7, #24]
	str	r0, [r7, #28]
	ldr	r2, [sp, #4]
	movs	r6, #0
	stmia	r2!, {r6}
	movs	r3, #128
	adds	r1, r2, #0
	lsls	r3, r3, #23
	str	r1, [sp, #4]
	stmia	r2!, {r3}
	movs	r3, #128
	lsrs	r5, r5, #5
	lsls	r3, r3, #3
	adds	r4, r2, #0
	orrs	r3, r5
	str	r4, [sp, #4]
	str	r3, [r2, #0]
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02010000
	.4byte 0x02000240
	.4byte 0x040000d4
	.4byte 0x85000020
	.4byte 0x000001ff
	.4byte 0x00000177
	.4byte 0x0000ffff
	.4byte 0x0001ffff
	.4byte 0x00011ff0
	.4byte 0x00000fff
	.4byte 0x03001b10
