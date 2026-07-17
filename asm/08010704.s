@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08010704
	.thumb_func
Func_08010704:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	lr, r3
	ldr	r3, [sp, #28]
	mov	ip, r2
	ldr	r2, [sp, #24]
	ldr	r4, [pc, #100]
	lsls	r3, r3, #7
	adds	r3, r3, r2
	lsls	r1, r1, #7
	mov	r8, r4
	lsls	r3, r3, #2
	adds	r1, r1, r0
	add	r3, r8
	lsls	r1, r1, #2
	mov	r2, lr
	mov	sl, r3
	add	r8, r1
	cmp	r2, #0
	ble.n	.L0
	movs	r5, #0
.L3:
	lsrs	r3, r5, #7
	mov	r4, sl
	mov	r2, r8
	adds	r1, r4, r3
	adds	r0, r2, r3
	mov	r3, ip
	cmp	r3, #0
	ble.n	.L1
	ldr	r7, [pc, #60]
	ldr	r6, [pc, #60]
	movs	r4, #0
.L2:
	ldr	r2, [r1, #0]
	ldmia	r0!, {r3}
	ands	r2, r7
	ands	r3, r6
	orrs	r2, r3
	stmia	r1!, {r2}
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r4, r2
	adds	r4, r3, #0
	lsrs	r3, r4, #16
	cmp	r3, ip
	blt.n	.L2
.L1:
	movs	r4, #128
	lsls	r4, r4, #9
	adds	r3, r5, r4
	adds	r5, r3, #0
	lsrs	r3, r5, #16
	cmp	r3, lr
	blt.n	.L3
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02010000
	.4byte 0x00000fff
	.4byte 0xfffff000
