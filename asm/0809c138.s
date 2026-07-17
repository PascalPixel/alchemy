@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809c138
	.thumb_func
Func_0809c138:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #400]
	ldr	r1, [pc, #400]
	movs	r0, #27
	ldr	r6, [r5, #0]
	sub	sp, #24
	bl	Func_080048f4
	movs	r2, #207
	ldr	r1, [pc, #392]
	adds	r7, r0, #0
	lsls	r2, r2, #1
	subs	r5, #8
	adds	r3, r7, r2
	ldr	r5, [r5, #0]
	mov	r9, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	mov	fp, r5
	cmp	r3, #3
	beq.n	.L0
	b.n	.L1
.L0:
	adds	r2, #66
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r2, #1
	adds	r3, #91
	strb	r2, [r3, #0]
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r1, [r2, #0]
	movs	r3, #6
	str	r1, [sp, #4]
	str	r3, [r2, #0]
	bl	Func_08091df4
	bl	Func_08091e20
	adds	r1, r6, #0
	adds	r1, #24
	add	r2, sp, #8
	mov	r0, sp
	str	r1, [sp, #0]
	mov	r8, r2
	movs	r4, #1
	adds	r0, #23
.L2:
	ldrh	r3, [r1, #10]
	strb	r3, [r2, #0]
	adds	r2, #1
	strh	r4, [r1, #10]
	adds	r1, #12
	cmp	r2, r0
	ble.n	.L2
	mov	r1, fp
	movs	r3, #1
	strh	r3, [r1, #4]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #292]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r0, r9
	mov	sl, r3
	bl	Func_08002f40
	movs	r1, #160
	movs	r5, #0
	adds	r4, r0, #0
	ldr	r3, [pc, #276]
	lsls	r1, r1, #19
	ldr	r2, [pc, #276]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #160
	lsls	r3, r3, #19
	movs	r2, #224
	strh	r5, [r3, #0]
	lsls	r2, r2, #1
	ldr	r5, [pc, #264]
	adds	r4, r4, r2
	adds	r1, r5, #0
	adds	r0, r4, #0
	bl	Func_08005340
	ldr	r3, [pc, #244]
	adds	r0, r5, #0
	ldr	r1, [pc, #252]
	ldr	r2, [pc, #252]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #252]
	subs	r3, #202
	strh	r2, [r3, #0]
	movs	r2, #154
	lsls	r2, r2, #5
	subs	r3, #10
	strh	r2, [r3, #0]
	bl	Func_0809bb64
	movs	r1, #200
	ldr	r0, [pc, #236]
	lsls	r1, r1, #4
	bl	Func_080041d8
	movs	r0, #142
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L3
	ldr	r0, [pc, #220]
	movs	r1, #1
	bl	Func_08015040
.L3:
	ldr	r6, [pc, #216]
	movs	r5, #3
.L4:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L4
	ldr	r0, [pc, #188]
	bl	Func_08004278
	bl	Func_0809bcd4
	movs	r2, #64
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	bl	Func_08009238
	ldr	r1, [pc, #180]
	ldr	r0, [pc, #180]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L5
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r3, #1
	adds	r2, r2, r1
	strh	r3, [r1, #0]
	mov	r1, sl
	lsls	r3, r1, #16
	adds	r2, #4
	lsrs	r3, r3, #16
	stmia	r2!, {r3}
	ldr	r3, [pc, #100]
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L5:
	strh	r4, [r0, #0]
	ldr	r1, [sp, #0]
	mov	r2, r8
	adds	r0, r2, #0
	adds	r0, #15
.L6:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strh	r3, [r1, #10]
	adds	r1, #12
	cmp	r2, r0
	ble.n	.L6
	movs	r3, #0
	mov	r2, fp
	strh	r3, [r2, #4]
	bl	Func_08091dc8
	bl	Func_08091e20
	movs	r1, #228
	lsls	r1, r1, #1
	ldr	r2, [sp, #4]
	adds	r3, r7, r1
	adds	r1, #24
	str	r2, [r3, #0]
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #24]
	adds	r3, #91
	strb	r5, [r3, #0]
.L1:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e70
	.4byte 0x00000ccc
	.4byte 0x0000001b
	.4byte 0x04000052
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x02010000
	.4byte 0x06006a00
	.4byte 0x84002580
	.4byte 0x00000682
	.4byte 0x0809bcf9
	.4byte 0x00000985
	.4byte 0x03001b04
	.4byte 0x02002090
	.4byte 0x04000208
