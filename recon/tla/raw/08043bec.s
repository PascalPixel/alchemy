.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_08014cc0, 0x08014cc0
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_08039418, 0x08039418
	.set sub_0803f9c0, 0x0803f9c0
	.set sub_08041c54, 0x08041c54
	.set sub_08041fa4, 0x08041fa4
	.set sub_08042010, 0x08042010
	.set sub_080420a8, 0x080420a8
	.set sub_080420fc, 0x080420fc
	.set sub_08042188, 0x08042188
	.set sub_08042214, 0x08042214
	.set sub_08042244, 0x08042244
	.set sub_080431b0, 0x080431b0
	.set sub_080439c4, 0x080439c4
	.set sub_080439d8, 0x080439d8
	.set sub_080439e8, 0x080439e8
	.set sub_08043a64, 0x08043a64
	.set sub_08043b34, 0x08043b34
	.set sub_08044f88, 0x08044f88
	.set sub_08045018, 0x08045018
	.set sub_080ad2c8, 0x080ad2c8
	.set sub_080f8070, 0x080f8070
	.set sub_080f8078, 0x080f8078
	.set sub_080f8080, 0x080f8080
	.set sub_080f8090, 0x080f8090
	.set sub_081c0010, 0x081c0010
	.global Func_08043bec
	.thumb_func
Func_08043bec:
.L_08043bec:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	sub	sp, #8
	cmp	r7, #0
	beq.n	.L_08043c28
	movs	r3, #16
	adds	r5, r1, #0
	movs	r4, #0
	mov	r8, r3
	movs	r6, #3
	adds	r5, #40
.L_08043c06:
	ldrb	r0, [r5, #0]
	mov	r3, r8
	lsls	r0, r0, #24
	str	r3, [sp, #0]
	asrs	r0, r0, #24
	adds	r3, r4, #0
	movs	r1, #2
	adds	r2, r7, #0
	str	r4, [sp, #4]
	bl	sub_08042214
	ldr	r4, [sp, #4]
	subs	r6, #1
	adds	r5, #1
	adds	r4, #24
	cmp	r6, #0
	bge.n	.L_08043c06
.L_08043c28:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_08043c30:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	sub	sp, #20
	adds	r7, r1, #0
	cmp	r5, #0
	beq.n	.L_08043cc4
	bl	sub_080393fc
	movs	r3, #4
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #4
	movs	r3, #13
	bl	sub_08041c54
	adds	r0, r7, #0
	adds	r0, #16
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_080420fc
	ldr	r0, [pc, #104]
	adds	r1, r5, #0
	movs	r2, #72
	movs	r3, #0
	bl	sub_08042188
	movs	r6, #0
	ldrb	r0, [r7, #28]
	movs	r1, #2
	adds	r2, r5, #0
	movs	r3, #80
	str	r6, [sp, #0]
	bl	sub_08042214
	ldr	r3, [pc, #80]
	ldrb	r0, [r7, #29]
	adds	r1, r5, #0
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #16
	bl	sub_08042010
	movs	r2, #0
	movs	r3, #32
	ldr	r0, [pc, #64]
	adds	r1, r5, #0
	bl	sub_08042010
	ldr	r0, [r7, #32]
	add	r1, sp, #4
	bl	sub_080431b0
	adds	r1, r5, #0
	movs	r2, #48
	movs	r3, #40
	bl	sub_08042188
	movs	r6, #48
	ldr	r0, [r7, #36]
	movs	r1, #6
	adds	r2, r5, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl	sub_08042244
	ldr	r0, [pc, #24]
	adds	r1, r5, #0
	movs	r2, #48
	movs	r3, #48
	bl	sub_08042010
.L_08043cc4:
	add	sp, #20
	pop	{r5, r6, r7, pc}
	.4byte 0x0805f640
	.4byte 0x00000b63
	.4byte 0x0000000a
	.2byte 0x1235
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r1
	movs	r1, #192
	lsls	r1, r1, #4
	mov	r8, r0
	adds	r1, #236
	movs	r0, #220
	sub	sp, #40
	bl	sub_08014cc0
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r1, #0
	adds	r3, #204
	movs	r2, #1
	ldr	r7, [r3, #0]
	adds	r5, r0, #0
	str	r1, [sp, #32]
	str	r1, [sp, #28]
	str	r1, [sp, #24]
	str	r2, [sp, #12]
	bl	sub_080ad2c8
	mov	r3, r8
	str	r0, [sp, #8]
	cmp	r3, #0
	bge.n	.L_08043d1e
	movs	r1, #0
	mov	r8, r1
.L_08043d1e:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_08043d70
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r4, #0
	b.n	.L_08043d5a
.L_08043d36:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #2
	ble.n	.L_08043d44
	movs	r1, #0
	mov	r8, r1
.L_08043d44:
	adds	r4, #1
	cmp	r4, #2
	ble.n	.L_08043d4c
	b.n	.L_08043e9c
.L_08043d4c:
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L_08043d5a:
	cmp	r3, #0
	beq.n	.L_08043d36
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_08043d36
	b.n	.L_08043e9c
.L_08043d70:
	mov	r2, sl
	cmp	r2, #4
	bne.n	.L_08043dc0
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r4, #0
	b.n	.L_08043daa
.L_08043d88:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_08043d96
	movs	r1, #0
	mov	r8, r1
.L_08043d96:
	adds	r4, #1
	cmp	r4, #2
	bgt.n	.L_08043e9c
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L_08043daa:
	cmp	r3, #0
	beq.n	.L_08043d88
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08043d88
	b.n	.L_08043e9c
.L_08043dc0:
	mov	r2, sl
	cmp	r2, #5
	bne.n	.L_08043e10
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r4, #0
	b.n	.L_08043dfa
.L_08043dd8:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #2
	ble.n	.L_08043de6
	movs	r1, #0
	mov	r8, r1
.L_08043de6:
	adds	r4, #1
	cmp	r4, #2
	bgt.n	.L_08043e9c
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L_08043dfa:
	cmp	r3, #0
	beq.n	.L_08043dd8
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08043dd8
	b.n	.L_08043e9c
.L_08043e10:
	mov	r2, sl
	cmp	r2, #6
	bne.n	.L_08043e60
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r4, #0
	b.n	.L_08043e4a
.L_08043e28:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #2
	ble.n	.L_08043e36
	movs	r1, #0
	mov	r8, r1
.L_08043e36:
	adds	r4, #1
	cmp	r4, #2
	bgt.n	.L_08043e9c
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L_08043e4a:
	cmp	r3, #0
	beq.n	.L_08043e28
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_08043e28
	b.n	.L_08043e9c
.L_08043e60:
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_08043eb2
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r4, #0
	cmp	r3, #0
	bne.n	.L_08043e9c
	adds	r3, r2, r7
	adds	r2, r3, r1
.L_08043e7e:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r2, #64
	cmp	r3, #2
	ble.n	.L_08043e90
	movs	r3, #0
	adds	r2, r7, r1
	mov	r8, r3
.L_08043e90:
	adds	r4, #1
	cmp	r4, #2
	bgt.n	.L_08043e9c
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08043e7e
.L_08043e9c:
	cmp	r4, #3
	bne.n	.L_08043eb2
	movs	r5, #2
	negs	r5, r5
	b.n	.L_08044330
.L_08043ea6:
	movs	r0, #113
	movs	r5, #1
	bl	sub_081c0010
	negs	r5, r5
	b.n	.L_080442de
.L_08043eb2:
	add	r0, sp, #36
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #840]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_080439c4
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #2
	movs	r2, #28
	movs	r0, #1
	movs	r3, #7
	bl	sub_08039260
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #60
	adds	r1, r7, r1
	movs	r2, #192
	lsls	r2, r2, #6
	str	r1, [sp, #20]
	movs	r4, #0
	adds	r2, #88
	mov	r9, r0
	mov	fp, r4
	adds	r6, r7, r2
.L_08043ef2:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_08043f04
	ldr	r0, [pc, #788]
	mov	r1, r9
	movs	r2, #10
	mov	r3, fp
	str	r4, [sp, #4]
	b.n	.L_08043f7e
.L_08043f04:
	ldrh	r3, [r6, #26]
	ldr	r1, [sp, #8]
	cmp	r3, r1
	bcs.n	.L_08043f18
	ldr	r0, [pc, #772]
	mov	r1, r9
	movs	r2, #10
	mov	r3, fp
	str	r4, [sp, #4]
	b.n	.L_08043f7e
.L_08043f18:
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #28]
	cmp	r2, r3
	beq.n	.L_08043f2c
	ldr	r0, [pc, #756]
	mov	r1, r9
	movs	r2, #10
	mov	r3, fp
	str	r4, [sp, #4]
	b.n	.L_08043f7e
.L_08043f2c:
	mov	r2, sl
	cmp	r2, #5
	bne.n	.L_08043f46
	movs	r3, #21
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_08043f46
	ldr	r0, [pc, #736]
	mov	r1, r9
	movs	r2, #10
	mov	r3, fp
	str	r4, [sp, #4]
	b.n	.L_08043f7e
.L_08043f46:
	mov	r3, sl
	cmp	r3, #6
	bne.n	.L_08043f60
	movs	r3, #23
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_08043f60
	ldr	r0, [pc, #712]
	mov	r1, r9
	movs	r2, #10
	mov	r3, fp
	str	r4, [sp, #4]
	b.n	.L_08043f7e
.L_08043f60:
	ldr	r0, [sp, #20]
	mov	r5, fp
	adds	r0, #16
	mov	r1, r9
	movs	r2, #12
	adds	r3, r5, #0
	str	r4, [sp, #4]
	bl	sub_080420a8
	ldr	r3, [pc, #688]
	ldrh	r0, [r6, #2]
	mov	r1, r9
	adds	r0, r0, r3
	movs	r2, #72
	adds	r3, r5, #0
.L_08043f7e:
	bl	sub_08041fa4
	ldr	r4, [sp, #4]
	ldr	r2, [sp, #20]
	movs	r1, #16
	adds	r2, #64
	adds	r4, #1
	add	fp, r1
	adds	r6, #64
	str	r2, [sp, #20]
	cmp	r4, #2
	ble.n	.L_08043ef2
	movs	r3, #24
	negs	r3, r3
	mov	r1, r9
	movs	r2, #72
	mov	r0, sl
	bl	sub_08044f88
	movs	r5, #144
	str	r0, [sp, #16]
	bl	sub_080f8070
	movs	r4, #0
.L_08043fae:
	adds	r0, r4, #0
	adds	r1, r5, #0
	movs	r2, #128
	str	r4, [sp, #4]
	bl	sub_080f8090
	ldr	r4, [sp, #4]
	adds	r5, #24
	adds	r4, #1
	cmp	r4, #3
	ble.n	.L_08043fae
	movs	r0, #0
	bl	sub_080f8080
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #2
	mov	fp, r3
.L_08043fd4:
	ldr	r1, [sp, #12]
	cmp	r1, #0
	bne.n	.L_08043fdc
	b.n	.L_08044142
.L_08043fdc:
	movs	r2, #0
	movs	r1, #192
	mov	r3, r8
	str	r2, [sp, #12]
	lsls	r1, r1, #6
	lsls	r5, r3, #6
	adds	r1, #88
	adds	r3, r5, r1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_080440c6
	movs	r2, #192
	lsls	r2, r2, #6
	adds	r2, #112
	adds	r3, r5, r2
	ldrb	r0, [r7, r3]
	adds	r3, r7, r3
	ldrb	r1, [r3, #1]
	bl	sub_0803f9c0
	ldr	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L_0804401c
	mov	r1, fp
	str	r1, [sp, #0]
	movs	r0, #1
	movs	r1, #10
	movs	r2, #14
	movs	r3, #9
	bl	sub_08039260
	str	r0, [sp, #32]
.L_0804401c:
	movs	r2, #192
	lsls	r2, r2, #6
	adds	r3, r7, r5
	adds	r2, #60
	adds	r6, r3, r2
	ldr	r0, [sp, #32]
	adds	r1, r6, #0
	bl	.L_08043c30
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne.n	.L_0804404c
	mov	r1, fp
	str	r1, [sp, #0]
	movs	r0, #16
	movs	r1, #10
	movs	r2, #13
	movs	r3, #3
	bl	sub_08039260
	str	r0, [sp, #28]
.L_0804404c:
	bl	sub_08043b34
	movs	r1, #0
	ldr	r0, [sp, #28]
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_08043a64
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #192
	lsls	r2, r2, #6
	movs	r1, #192
	adds	r2, #100
	lsls	r1, r1, #6
	adds	r3, r5, r2
	adds	r1, #101
	ldrsb	r2, [r7, r3]
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	adds	r1, #1
	adds	r2, r2, r3
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	adds	r1, #1
	adds	r2, r2, r3
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	cmn	r2, r3
	beq.n	.L_080440b2
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_080440a2
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r0, #16
	movs	r1, #14
	movs	r2, #13
	movs	r3, #5
	bl	sub_08039260
	str	r0, [sp, #24]
.L_080440a2:
	ldr	r0, [sp, #24]
	adds	r1, r6, #0
	bl	.L_08043bec
	movs	r0, #1
	bl	sub_080f8080
	b.n	.L_0804410a
.L_080440b2:
	movs	r0, #0
	bl	sub_080f8080
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	sub_0803939c
	movs	r1, #0
	str	r1, [sp, #24]
	b.n	.L_0804410a
.L_080440c6:
	ldr	r2, [pc, #352]
	movs	r1, #147
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #38
	adds	r2, r2, r3
	ldrb	r1, [r2, #0]
	bl	sub_0803f9c0
	movs	r0, #0
	bl	sub_080f8080
	bl	sub_08043b34
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	sub_0803939c
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	sub_0803939c
	movs	r1, #2
	ldr	r0, [sp, #32]
	bl	sub_0803939c
	movs	r1, #0
	str	r1, [sp, #24]
	str	r1, [sp, #28]
	str	r1, [sp, #32]
.L_0804410a:
	mov	r0, r9
	bl	sub_08039418
	mov	r2, fp
	str	r2, [sp, #0]
	mov	r0, r9
	movs	r1, #0
	movs	r2, #2
	movs	r3, #27
	bl	sub_08041c54
	movs	r3, #4
	str	r3, [sp, #0]
	mov	r0, r9
	movs	r1, #0
	movs	r2, #4
	movs	r3, #27
	bl	sub_08041c54
	mov	r3, r8
	lsls	r2, r3, #1
	movs	r3, #1
	str	r3, [sp, #0]
	mov	r0, r9
	movs	r1, #0
	movs	r3, #26
	bl	sub_080439e8
.L_08044142:
	ldr	r0, [sp, #16]
	bl	sub_08045018
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #220]
	movs	r3, #64
	ldr	r2, [r1, #12]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080441f4
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #1
	str	r1, [sp, #12]
	b.n	.L_080441e0
.L_08044166:
	movs	r1, #192
	mov	r3, r8
	lsls	r1, r1, #6
	lsls	r2, r3, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_080441e0
	mov	r3, sl
	cmp	r3, #1
	bne.n	.L_0804418e
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080441e0
.L_0804418e:
	mov	r3, sl
	cmp	r3, #4
	bne.n	.L_080441a8
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #108
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080441e0
.L_080441a8:
	mov	r3, sl
	cmp	r3, #5
	bne.n	.L_080441c2
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #108
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080441e0
.L_080441c2:
	mov	r3, sl
	cmp	r3, #6
	beq.n	.L_080441ca
	b.n	.L_08043fd4
.L_080441ca:
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #108
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080441e0
	b.n	.L_08043fd4
.L_080441e0:
	mov	r0, r8
	adds	r0, #2
	movs	r1, #3
	bl	sub_08002064
	mov	r2, sl
	mov	r8, r0
	cmp	r2, #0
	bne.n	.L_08044166
	b.n	.L_08043fd4
.L_080441f4:
	ldr	r2, [r1, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080442be
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	str	r3, [sp, #12]
	b.n	.L_080442aa
	movs	r0, r0
	.4byte 0x8500033b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000e58
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
.L_08044230:
	mov	r1, r8
	lsls	r2, r1, #6
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #88
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_080442aa
	mov	r3, sl
	cmp	r3, #1
	bne.n	.L_08044258
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080442aa
.L_08044258:
	mov	r3, sl
	cmp	r3, #4
	bne.n	.L_08044272
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #108
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080442aa
.L_08044272:
	mov	r3, sl
	cmp	r3, #5
	bne.n	.L_0804428c
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #108
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080442aa
.L_0804428c:
	mov	r3, sl
	cmp	r3, #6
	beq.n	.L_08044294
	b.n	.L_08043fd4
.L_08044294:
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #108
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080442aa
	b.n	.L_08043fd4
.L_080442aa:
	mov	r0, r8
	adds	r0, #4
	movs	r1, #3
	bl	sub_08002064
	mov	r2, sl
	mov	r8, r0
	cmp	r2, #0
	bne.n	.L_08044230
	b.n	.L_08043fd4
.L_080442be:
	ldr	r3, [r1, #4]
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080442ca
	b.n	.L_08043ea6
.L_080442ca:
	ldr	r3, [r1, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080442d6
	b.n	.L_08043fd4
.L_080442d6:
	movs	r0, #112
	bl	sub_081c0010
	mov	r5, r8
.L_080442de:
	bl	sub_080f8078
	bl	sub_08043b34
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	sub_0803939c
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	sub_0803939c
	movs	r1, #2
	ldr	r0, [sp, #32]
	bl	sub_0803939c
	movs	r1, #2
	mov	r0, r9
	bl	sub_0803939c
	bl	sub_080439d8
	movs	r0, #220
	bl	sub_0801314c
	ldr	r3, [pc, #44]
	movs	r1, #147
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #38
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl	sub_0803f9c0
	movs	r0, #1
	bl	sub_08013560
.L_08044330:
	adds	r0, r5, #0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
