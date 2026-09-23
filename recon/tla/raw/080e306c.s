.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014cc0, 0x08014cc0
	.set sub_08020090, 0x08020090
	.set sub_080200c8, 0x080200c8
	.set sub_080200e8, 0x080200e8
	.set sub_08020218, 0x08020218
	.set sub_080d3888, 0x080d3888
	.set sub_080d440c, 0x080d440c
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080e1420, 0x080e1420
	.set sub_080e2264, 0x080e2264
	.set sub_080e249c, 0x080e249c
	.set sub_080e2a68, 0x080e2a68
	.set sub_080e2acc, 0x080e2acc
	.set sub_080e2d40, 0x080e2d40
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb824, 0x080eb824
	.set sub_080eb930, 0x080eb930
	.set sub_081c0010, 0x081c0010
	.global Func_080e306c
	.thumb_func
Func_080e306c:
	push	{lr}
	bl	.L_080e3074
	pop	{pc}
.L_080e3074:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #216
	lsls	r1, r1, #5
	adds	r1, #56
	movs	r0, #92
	sub	sp, #56
	bl	sub_08014cc0
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r9, r0
	ldr	r0, [r3, #108]
	adds	r3, #224
	str	r0, [sp, #40]
	movs	r2, #204
	ldr	r3, [r3, #0]
	lsls	r2, r2, #4
	str	r3, [sp, #36]
	movs	r4, #224
	ldr	r1, [r3, #16]
	movs	r3, #128
	adds	r2, r0, r2
	lsls	r3, r3, #11
	lsls	r4, r4, #8
	mov	sl, r1
	str	r2, [sp, #32]
	str	r3, [sp, #28]
	str	r4, [sp, #24]
	bl	sub_080dc294
	bl	sub_080e2264
	ldr	r1, [sp, #36]
	movs	r6, #0
	ldr	r0, [r1, #16]
	movs	r1, #0
	bl	sub_080e1420
	ldr	r3, [pc, #860]
	mov	r2, sl
	str	r3, [r2, #108]
	ldrh	r3, [r2, #6]
	adds	r2, #100
	adds	r4, r2, #0
	ldr	r0, [pc, #852]
	str	r2, [sp, #12]
	movs	r1, #128
	strh	r3, [r4, #0]
	movs	r4, #248
	lsls	r4, r4, #7
	lsls	r1, r1, #2
.L_080e30e6:
	adds	r2, r6, #0
	adds	r2, #16
	adds	r3, r1, #0
	orrs	r3, r4
	orrs	r3, r2
	strh	r3, [r0, #0]
	adds	r0, #2
	adds	r6, #1
	adds	r1, #32
	cmp	r6, #15
	ble.n	.L_080e30e6
	bl	sub_080eb824
	movs	r7, #192
	ldr	r0, [sp, #36]
	lsls	r7, r7, #18
	adds	r3, r7, #0
	adds	r3, #240
	ldr	r2, [r0, #16]
	ldr	r3, [r3, #0]
	add	r5, sp, #44
	mov	r8, r3
	ldr	r3, [r2, #8]
	adds	r0, r5, #0
	str	r3, [r5, #0]
	movs	r6, #0
	ldr	r3, [r2, #12]
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	bl	sub_080dc390
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	mov	r2, r8
	adds	r2, #168
	str	r3, [r2, #0]
	adds	r2, #4
	movs	r4, #10
	ldrsh	r3, [r5, r4]
	movs	r5, #192
	str	r3, [r2, #0]
	adds	r2, #19
	movs	r3, #1
	strb	r3, [r2, #0]
	subs	r2, #1
	movs	r3, #240
	strb	r3, [r2, #0]
	adds	r2, #2
	movs	r3, #3
	strb	r3, [r2, #0]
	mov	r3, r8
	lsls	r5, r5, #9
	adds	r3, #176
	str	r5, [r3, #0]
	adds	r3, #4
	str	r5, [r3, #0]
	subs	r2, #4
	ldrh	r2, [r2, #0]
	movs	r3, #192
	str	r2, [sp, #0]
	lsls	r3, r3, #24
	mov	r0, r8
	movs	r1, #32
	movs	r2, #32
	bl	sub_080eaf98
	mov	r0, r8
	ldrb	r3, [r0, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r0, #9]
	strb	r3, [r0, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r0, #9]
	adds	r3, r7, #0
	adds	r3, #180
	ldr	r3, [r3, #0]
	mov	r2, sl
	str	r3, [sp, #20]
	ldr	r7, [r7, #96]
	ldr	r3, [pc, #680]
	str	r7, [sp, #16]
	mov	r4, sl
	ldr	r1, [r2, #8]
	ldr	r2, [r2, #12]
	movs	r0, #140
	adds	r2, r2, r3
	lsls	r0, r0, #1
	ldr	r3, [r4, #16]
	bl	sub_080dc10c
	movs	r1, #2
	mov	fp, r0
	bl	sub_08020090
	mov	r0, fp
	str	r5, [r0, #24]
	str	r6, [r0, #28]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #207
	bl	sub_081c0010
	movs	r7, #0
.L_080e31bc:
	lsls	r0, r7, #16
	movs	r1, #20
	bl	sub_08002054
	mov	r1, fp
	str	r0, [r1, #28]
	adds	r7, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r7, #19
	ble.n	.L_080e31bc
	mov	r2, r8
	mov	r3, r8
	adds	r2, #184
	adds	r3, #193
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	mov	r8, r2
	movs	r7, #59
.L_080e31e4:
	movs	r6, #0
.L_080e31e6:
	bl	sub_08014878
	ldr	r4, [sp, #28]
	adds	r5, r0, #0
	lsls	r5, r5, #2
	lsls	r3, r6, #16
	adds	r5, r4, r5
	subs	r5, r5, r3
	bl	sub_08014878
	asrs	r5, r5, #16
	adds	r1, r0, #0
	ldr	r2, [sp, #20]
	adds	r0, r5, #0
	ldr	r3, [sp, #16]
	adds	r6, #1
	bl	sub_080e2a68
	cmp	r6, #3
	ble.n	.L_080e31e6
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #24]
	movs	r2, #224
	adds	r0, r0, r1
	lsls	r2, r2, #13
	str	r0, [sp, #28]
	cmp	r0, r2
	ble.n	.L_080e3220
	str	r2, [sp, #28]
.L_080e3220:
	ldr	r4, [sp, #24]
	lsls	r3, r4, #5
	subs	r3, r3, r4
	lsls	r3, r3, #2
	subs	r3, r3, r4
	cmp	r3, #0
	bge.n	.L_080e3230
	adds	r3, #127
.L_080e3230:
	asrs	r3, r3, #7
	str	r3, [sp, #24]
	mov	r0, r8
	ldr	r3, [r0, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r3, r3, r1
	str	r3, [r0, #0]
	ldr	r2, [sp, #4]
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #1
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080e31e4
	bl	sub_080dc384
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #12
	mov	r0, sl
	add	r3, r9
	mov	r4, fp
	movs	r2, #216
	str	r4, [r3, #0]
	lsls	r2, r2, #5
	ldr	r3, [r0, #8]
	adds	r2, #28
	add	r2, r9
	str	r3, [r2, #0]
	movs	r2, #217
	ldr	r3, [r0, #12]
	lsls	r2, r2, #5
	add	r2, r9
	str	r3, [r2, #0]
	movs	r2, #216
	ldr	r3, [r0, #16]
	lsls	r2, r2, #5
	adds	r2, #36
	add	r2, r9
	str	r3, [r2, #0]
	movs	r2, #216
	ldr	r3, [r0, #8]
	lsls	r2, r2, #5
	adds	r2, #40
	add	r2, r9
	str	r3, [r2, #0]
	movs	r2, #216
	ldr	r3, [r0, #12]
	lsls	r2, r2, #5
	adds	r2, #44
	add	r2, r9
	str	r3, [r2, #0]
	movs	r2, #216
	ldr	r3, [r0, #16]
	lsls	r2, r2, #5
	adds	r2, #48
	add	r2, r9
	str	r3, [r2, #0]
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #20
	movs	r2, #0
	add	r3, r9
	str	r2, [r3, #0]
	movs	r1, #216
	movs	r3, #216
	lsls	r1, r1, #5
	lsls	r3, r3, #5
	adds	r1, #16
	adds	r3, #18
	add	r1, r9
	add	r3, r9
	strh	r2, [r1, #0]
	strh	r2, [r3, #0]
	strh	r2, [r1, #0]
	ldr	r1, [sp, #40]
	movs	r2, #212
	lsls	r2, r2, #4
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	movs	r0, #1
	negs	r0, r0
	movs	r2, #3
	cmp	r1, r0
	bne.n	.L_080e3304
	ldr	r2, [sp, #40]
	movs	r4, #208
	lsls	r4, r4, #4
	adds	r4, #60
	adds	r3, r2, r4
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r2, #2
	cmp	r3, r1
	bne.n	.L_080e3304
	ldr	r3, [sp, #32]
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	mvns	r2, r2
	negs	r3, r2
	orrs	r3, r2
	lsrs	r2, r3, #31
.L_080e3304:
	cmp	r2, #3
	beq.n	.L_080e330a
	b.n	.L_080e343c
.L_080e330a:
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #52
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e3320
	bl	sub_080e2acc
.L_080e3320:
	ldr	r5, [sp, #8]
	movs	r6, #3
	movs	r7, #15
.L_080e3326:
	ldr	r3, [r5, #0]
	movs	r4, #128
	lsls	r4, r4, #3
	adds	r3, r3, r4
	str	r3, [r5, #0]
	ldr	r0, [sp, #4]
	subs	r7, #1
	strb	r6, [r0, #0]
	movs	r0, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080e3326
	movs	r2, #1
	negs	r2, r2
	adds	r0, r2, #0
	adds	r1, r2, #0
	movs	r3, #0
	bl	sub_080d440c
	mov	r4, sl
	mov	r2, sl
	ldr	r3, [r4, #16]
	ldr	r1, [r2, #8]
	mov	r0, sl
	ldr	r2, [r2, #12]
	bl	sub_080200e8
	movs	r1, #0
	mov	r0, sl
	bl	sub_08020218
	ldr	r2, [sp, #36]
	movs	r5, #0
	movs	r1, #24
	ldrsh	r0, [r2, r1]
	movs	r1, #1
	bl	sub_080d3888
	mov	r2, sl
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #40]
	movs	r0, #212
	lsls	r0, r0, #4
	adds	r3, r4, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #216
	lsls	r2, r2, #5
	adds	r2, #40
	add	r2, r9
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	mov	r4, sl
	ldr	r3, [r4, #12]
	movs	r2, #216
	lsls	r2, r2, #5
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r2, #44
	adds	r3, r3, r0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r1, [sp, #40]
	movs	r4, #208
	lsls	r4, r4, #4
	adds	r4, #66
	adds	r3, r1, r4
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r2, #216
	lsls	r2, r2, #5
	adds	r2, #48
	add	r2, r9
	lsls	r3, r3, #16
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #104]
	bl	sub_080145a8
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #16
	add	r3, r9
	movs	r2, #186
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_080e340a
	movs	r6, #216
	lsls	r6, r6, #5
	adds	r6, #16
	mov	r8, r2
	add	r6, r9
.L_080e33f2:
	cmp	r5, #30
	bne.n	.L_080e33fa
	bl	sub_080e249c
.L_080e33fa:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	adds	r5, #1
	cmp	r3, r8
	bne.n	.L_080e33f2
.L_080e340a:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	ldr	r3, [sp, #36]
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080e3422
	mov	r4, sl
	str	r3, [r4, #24]
.L_080e3422:
	movs	r3, #0
	mov	r0, fp
	str	r3, [r0, #24]
	b.n	.L_080e3600
	movs	r0, r0
	.4byte 0x080e3061
	.4byte 0x050003c0
	.4byte 0xfffa0000
	.2byte 0x2ec1
	.2byte 0x080e
.L_080e343c:
	cmp	r2, #2
	beq.n	.L_080e3442
	b.n	.L_080e3578
.L_080e3442:
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #52
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e3458
	bl	sub_080e2acc
.L_080e3458:
	ldr	r5, [sp, #8]
	movs	r6, #3
	movs	r7, #15
.L_080e345e:
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r2, [sp, #4]
	movs	r0, #1
	strb	r6, [r2, #0]
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080e345e
	movs	r2, #1
	negs	r2, r2
	adds	r0, r2, #0
	adds	r1, r2, #0
	movs	r3, #0
	bl	sub_080d440c
	mov	r3, sl
	ldr	r2, [r3, #12]
	ldr	r1, [r3, #8]
	mov	r0, sl
	ldr	r3, [r3, #16]
	bl	sub_080200e8
	movs	r1, #0
	mov	r0, sl
	bl	sub_08020218
	mov	r2, sl
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #208
	ldr	r4, [sp, #40]
	lsls	r0, r0, #4
	adds	r0, #60
	adds	r3, r4, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #216
	lsls	r2, r2, #5
	adds	r2, #40
	add	r2, r9
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	mov	r4, sl
	movs	r2, #216
	ldr	r3, [r4, #12]
	lsls	r2, r2, #5
	adds	r2, #44
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r0, [sp, #40]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #62
	adds	r3, r0, r1
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	movs	r2, #216
	lsls	r2, r2, #5
	adds	r2, #48
	add	r2, r9
	lsls	r3, r3, #16
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #296]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #16
	add	r3, r9
	movs	r2, #186
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	lsls	r2, r2, #2
	adds	r2, #255
	movs	r5, #0
	cmp	r3, r2
	beq.n	.L_080e3530
	movs	r6, #216
	lsls	r6, r6, #5
	adds	r6, #16
	mov	r8, r2
	add	r6, r9
.L_080e3518:
	cmp	r5, #30
	bne.n	.L_080e3520
	bl	sub_080e249c
.L_080e3520:
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	adds	r5, #1
	cmp	r3, r8
	bne.n	.L_080e3518
.L_080e3530:
	ldr	r0, [pc, #228]
	bl	sub_08014644
	movs	r7, #15
.L_080e3538:
	mov	r2, sl
	ldr	r3, [r2, #24]
	ldr	r4, [pc, #220]
	movs	r0, #1
	adds	r3, r3, r4
	str	r3, [r2, #24]
	str	r3, [r2, #28]
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080e3538
	movs	r3, #0
	mov	r0, sl
	str	r3, [r0, #24]
	movs	r7, #31
.L_080e3558:
	mov	r1, fp
	ldr	r3, [r1, #24]
	ldr	r2, [pc, #188]
	ldr	r4, [pc, #192]
	adds	r3, r3, r2
	str	r3, [r1, #24]
	ldr	r3, [r1, #28]
	movs	r0, #1
	adds	r3, r3, r4
	str	r3, [r1, #28]
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080e3558
	b.n	.L_080e3422
.L_080e3578:
	cmp	r2, #1
	bne.n	.L_080e3624
	movs	r0, #136
	bl	sub_081c0010
	mov	r0, fp
	movs	r1, #6
	bl	sub_08020090
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #52
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e35a0
	bl	sub_080e2acc
.L_080e35a0:
	ldr	r5, [sp, #8]
	movs	r6, #3
	movs	r7, #15
.L_080e35a6:
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r2, [sp, #4]
	movs	r0, #1
	strb	r6, [r2, #0]
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080e35a6
	movs	r3, #0
	mov	r4, sl
	str	r3, [r4, #108]
	ldr	r0, [sp, #12]
	mov	r1, sl
	ldrh	r3, [r0, #0]
	movs	r5, #0
	strh	r3, [r1, #6]
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #8
	add	r3, r9
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080e3600
	movs	r6, #216
	lsls	r6, r6, #5
	adds	r6, #8
	add	r6, r9
.L_080e35e8:
	cmp	r5, #30
	bne.n	.L_080e35f0
	bl	sub_080e249c
.L_080e35f0:
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #0
	ldrsh	r3, [r6, r4]
	adds	r5, #1
	cmp	r3, #0
	bne.n	.L_080e35e8
.L_080e3600:
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #52
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e3664
	bl	sub_080e2d40
	b.n	.L_080e3664
	.4byte 0x080e2d65
	.4byte 0xfffff800
	.2byte 0xfe00
	.2byte 0xffff
.L_080e3624:
	movs	r0, #136
	bl	sub_081c0010
	mov	r0, fp
	movs	r1, #6
	bl	sub_08020090
	ldr	r6, [pc, #44]
	ldr	r5, [sp, #8]
	movs	r7, #15
.L_080e3638:
	mov	r0, sl
	ldrh	r3, [r0, #6]
	mov	r1, sl
	adds	r3, r3, r6
	strh	r3, [r1, #6]
	movs	r2, #128
	ldr	r3, [r5, #0]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r4, [sp, #4]
	movs	r3, #3
	strb	r3, [r4, #0]
	movs	r0, #1
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080e3638
	b.n	.L_080e3664
	.2byte 0x1000
	.2byte 0x0000
.L_080e3664:
	movs	r0, #136
	bl	sub_081c0010
	mov	r0, fp
	bl	sub_080200c8
	mov	r0, sl
	movs	r3, #0
	str	r3, [r0, #108]
	ldr	r1, [sp, #12]
	mov	r2, sl
	ldrh	r3, [r1, #0]
	strh	r3, [r2, #6]
	bl	sub_080eb930
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
