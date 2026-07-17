@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a524c
	.thumb_func
Func_080a524c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r2, #17
	adds	r5, r0, #0
	movs	r3, #10
	movs	r0, #13
	bl	Func_08015010
	ldr	r3, [pc, #264]
	ands	r5, r3
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	Func_08077018
	ldr	r3, [pc, #256]
	adds	r5, r5, r3
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #24
	movs	r3, #0
	bl	Func_08015080
	ldr	r5, [pc, #244]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #1
	bl	Func_08015080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #24
	bl	Func_08015080
	ldr	r5, [pc, #220]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #40
	adds	r5, #1
	bl	Func_08015080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #24
	movs	r3, #56
	bl	Func_08015080
	movs	r6, #1
	movs	r0, #104
	movs	r1, #86
	mov	r8, r6
	bl	Func_080a1ac0
	b.n	.L0
.L6:
	lsls	r1, r6, #4
	adds	r1, #70
	movs	r0, #104
	bl	Func_080a1a40
	ldr	r5, [pc, #172]
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r2, #1
	movs	r0, #111
	subs	r6, #1
	mov	r8, r2
	bl	Func_080f9010
.L1:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #1
	movs	r0, #111
	adds	r6, #1
	mov	r8, r3
	bl	Func_080f9010
.L2:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L3
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L4
	movs	r3, #0
	adds	r0, r6, #2
	movs	r1, #2
	mov	r8, r3
	bl	Func_080022fc
	adds	r6, r0, #0
.L4:
	ldr	r1, [pc, #92]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L3
.L5:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	movs	r0, #113
	bl	Func_080f9010
	movs	r6, #1
.L3:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L7
	movs	r6, #1
.L7:
	adds	r0, r7, #0
	movs	r1, #1
	bl	Func_08015018
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
	.4byte 0x00000182
	.4byte 0x00000ad4
	.4byte 0x00000b2c
	.4byte 0x03001b04
	.4byte 0x03001c94
