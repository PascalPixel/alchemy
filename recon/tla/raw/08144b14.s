.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_08144d3e, 0x08144d3e
	.set sub_08144fdc, 0x08144fdc
	.set sub_0814512c, 0x0814512c
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_08144b14
	.thumb_func
Func_08144b14:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	fp, r0
	ldr	r0, [r3, #92]
	sub	sp, #140
	str	r0, [sp, #56]
	movs	r0, #0
	ldr	r3, [r3, #96]
	adds	r5, r1, #0
	str	r3, [sp, #52]
	bl	sub_081435e0
	mov	r1, fp
	ldr	r3, [r1, #28]
	cmp	r3, #1
	bne.n	.L_08144b54
	ldr	r1, [r1, #4]
	add	r2, sp, #128
	eors	r1, r3
	lsls	r1, r1, #4
	orrs	r1, r5
	add	r3, sp, #116
	mov	r0, fp
	bl	sub_0815585c
.L_08144b54:
	ldr	r2, [sp, #56]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #188]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r3, #184
	ldr	r2, [sp, #56]
	lsls	r3, r3, #6
	adds	r3, #16
	adds	r1, r2, r3
	ldr	r0, [pc, #172]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	cmp	r5, #0
	bne.n	.L_08144b8c
	ldr	r5, [sp, #56]
	movs	r2, #216
	lsls	r2, r2, #7
	adds	r2, #192
	adds	r1, r5, r2
	ldr	r0, [pc, #152]
	b.n	.L_08144b9c
.L_08144b8c:
	cmp	r5, #1
	bne.n	.L_08144ba6
	ldr	r3, [sp, #56]
	movs	r5, #216
	lsls	r5, r5, #7
	adds	r5, #192
	adds	r1, r3, r5
	ldr	r0, [pc, #140]
.L_08144b9c:
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	.L_08144bba
.L_08144ba6:
	ldr	r2, [sp, #56]
	movs	r3, #216
	lsls	r3, r3, #7
	adds	r3, #192
	adds	r1, r2, r3
	ldr	r0, [pc, #120]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_08144bba:
	movs	r5, #0
	mov	r9, r5
	ldr	r5, [pc, #112]
.L_08144bc0:
	movs	r3, #0
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #32
	negs	r3, r3
	orrs	r3, r0
	lsls	r3, r3, #14
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	movs	r1, #128
	movs	r0, #1
	add	r9, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r9, r1
	bne.n	.L_08144bc0
	ldr	r2, [pc, #36]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #32
	strh	r2, [r3, #0]
	mov	r2, fp
	ldr	r3, [r2, #20]
	cmp	r3, #1
	bne.n	.L_08144c34
	add	r5, sp, #104
	movs	r3, #36
	ldrsh	r0, [r2, r3]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r2, [r5, #0]
	movs	r3, #64
	subs	r3, r3, r2
	str	r3, [sp, #44]
	b.n	.L_08144c46
	.4byte 0x00000100
	.4byte 0x0000013c
	.4byte 0x00000175
	.4byte 0x00000156
	.4byte 0x00000154
	.4byte 0x00000150
	.2byte 0x0000
	.2byte 0x0201
.L_08144c34:
	mov	r5, fp
	ldr	r3, [r5, #4]
	movs	r0, #112
	negs	r0, r0
	str	r0, [sp, #44]
	cmp	r3, #1
	beq.n	.L_08144c46
	movs	r1, #0
	str	r1, [sp, #44]
.L_08144c46:
	ldr	r5, [sp, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	lsls	r3, r5, #8
	adds	r2, #40
	str	r3, [r2, #0]
	ldr	r0, [sp, #56]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	movs	r1, #200
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #416]
	bl	sub_080145a8
	movs	r5, #0
	str	r5, [sp, #48]
	mov	r0, fp
	ldr	r3, [r0, #20]
	movs	r1, #116
	lsls	r3, r3, #4
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_08144c88
	b.n	.L_08145084
.L_08144c88:
	mov	r2, sp
	adds	r2, #60
	str	r2, [sp, #32]
.L_08144c8e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	str	r3, [sp, #40]
	ldr	r3, [sp, #48]
	cmp	r3, #64
	bne.n	.L_08144ca2
	movs	r0, #212
	bl	sub_081c0010
.L_08144ca2:
	ldr	r5, [sp, #48]
	cmp	r5, #80
	bne.n	.L_08144cae
	movs	r0, #0
	bl	sub_081180e8
.L_08144cae:
	mov	r0, fp
	ldr	r7, [r0, #28]
	cmp	r7, #1
	bne.n	.L_08144d4a
	ldr	r1, [sp, #48]
	lsls	r6, r1, #11
	adds	r0, r6, #0
	bl	sub_08002096
	negs	r0, r0
	add	r5, sp, #128
	lsls	r3, r0, #2
	ldr	r2, [r5, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #2
	asrs	r3, r3, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #44]
	adds	r0, r6, #0
	adds	r3, r3, r2
	subs	r3, #20
	mov	r8, r3
	bl	sub_08002090
	ldr	r3, [r5, #4]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r0, r0, r3
	mov	r3, fp
	adds	r6, r0, #0
	ldr	r0, [r3, #4]
	ldr	r1, [sp, #32]
	eors	r0, r7
	bl	sub_08144aac
	ldr	r5, [sp, #48]
	subs	r6, #24
	cmp	r5, #32
	ble.n	.L_08144d04
	lsls	r3, r5, #1
	subs	r3, r6, r3
	adds	r6, r3, #0
	adds	r6, #64
.L_08144d04:
	ldr	r0, [sp, #56]
	movs	r5, #40
	movs	r1, #216
	ldr	r2, [sp, #32]
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	lsls	r1, r1, #7
	adds	r1, #192
	adds	r7, r0, r1
	ldr	r4, [r2, #4]
	adds	r3, r6, #0
	ldr	r0, [sp, #52]
	adds	r1, r7, #0
	mov	r2, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b0c
	cmp	r3, #3
	bgt.n	sub_08144d3e
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r5, [sp, #32]
	ldr	r0, [sp, #52]
	ldr	r4, [r5, #4]
	adds	r1, r7, #0
	mov	r2, r8
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
.L_08144d4a:
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #23
	movs	r0, #188
	str	r3, [sp, #60]
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #32]
	mov	r2, fp
	str	r3, [r0, #4]
	ldr	r3, [r2, #20]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_08144d7c
	b.n	.L_08144e9c
.L_08144d7c:
	ldr	r3, [sp, #40]
	movs	r5, #36
	adds	r3, #12
	str	r3, [sp, #36]
	str	r5, [sp, #20]
	str	r1, [sp, #16]
	add	r7, sp, #68
.L_08144d8a:
	ldr	r1, [sp, #20]
	mov	r3, fp
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	mov	r0, r8
	lsls	r0, r0, #4
	mov	sl, r0
	bl	sub_08014de4
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #36]
	bl	sub_080156e8
	ldr	r3, [r5, #8]
	adds	r0, r7, #0
	str	r3, [r7, #0]
	movs	r3, #0
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	str	r3, [r7, #8]
	bl	sub_08015128
	ldr	r1, [sp, #48]
	cmp	r1, sl
	ble.n	.L_08144e7e
	ldr	r3, [sp, #16]
	ldr	r5, [pc, #80]
	movs	r2, #0
	mov	r9, r2
	adds	r6, r3, r5
.L_08144dca:
	mov	r0, r9
	ldr	r1, [sp, #48]
	lsls	r3, r0, #3
	add	r3, sl
	cmp	r1, r3
	ble.n	.L_08144e72
	ldr	r3, [r6, #4]
	movs	r2, #128
	lsls	r2, r2, #12
	cmp	r3, r2
	ble.n	.L_08144e72
	add	r5, sp, #92
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	ldr	r0, [sp, #44]
	adds	r3, r3, r0
	str	r3, [r5, #0]
	ldr	r0, [r6, #24]
	lsls	r0, r0, #10
	bl	sub_08002096
	movs	r3, #1
	mov	r1, r9
	lsls	r0, r0, #4
	ands	r3, r1
	asrs	r0, r0, #16
	cmp	r3, #0
	beq.n	.L_08144e18
	ldr	r3, [r5, #0]
	subs	r3, r3, r0
	b.n	.L_08144e1c
	movs	r0, r0
	.4byte 0x08143001
	.2byte 0x0000
	.2byte 0x0201
.L_08144e18:
	ldr	r3, [r5, #0]
	adds	r3, r3, r0
.L_08144e1c:
	str	r3, [r5, #0]
	mov	r2, r9
	movs	r3, #1
	ands	r3, r2
	ldr	r0, [sp, #32]
	ldr	r2, [r6, #24]
	lsls	r3, r3, #2
	adds	r4, r3, r0
	ldr	r1, [pc, #620]
	cmp	r2, #0
	bge.n	.L_08144e34
	adds	r2, #7
.L_08144e34:
	movs	r3, #7
	asrs	r2, r2, #3
	ands	r2, r3
	ldrb	r3, [r1, r2]
	ldr	r2, [sp, #56]
	lsls	r1, r3, #3
	adds	r1, r1, r3
	movs	r3, #184
	lsls	r1, r1, #6
	lsls	r3, r3, #6
	movs	r0, #24
	adds	r1, r2, r1
	adds	r3, #16
	ldr	r2, [r5, #0]
	adds	r1, r1, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r3, #12
	subs	r2, #12
	ldr	r4, [r4, #0]
	ldr	r0, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4d8e
	ldr	r3, [r6, #4]
	adds	r3, r3, r5
	str	r3, [r6, #4]
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
.L_08144e72:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r6, #28
	cmp	r1, #8
	bne.n	.L_08144dca
.L_08144e7e:
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r3, r3, r5
	adds	r2, #2
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	mov	r1, fp
	ldr	r3, [r1, #20]
	movs	r0, #1
	add	r8, r0
	cmp	r8, r3
	beq.n	.L_08144e9c
	b.n	.L_08144d8a
.L_08144e9c:
	movs	r2, #0
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L_08144ea6
	b.n	.L_08145050
.L_08144ea6:
	movs	r5, #36
	movs	r0, #72
	str	r5, [sp, #24]
	str	r0, [sp, #28]
.L_08144eae:
	ldr	r2, [sp, #48]
	ldr	r5, [sp, #28]
	mov	r1, r9
	lsls	r1, r1, #4
	mov	r8, r1
	cmp	r2, r5
	bge.n	.L_08144ebe
	b.n	.L_0814503a
.L_08144ebe:
	ldr	r1, [sp, #24]
	mov	r3, fp
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	bl	sub_08014de4
	ldr	r0, [sp, #40]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080156e8
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #28]
	cmp	r0, r1
	bne.n	.L_08144ef6
	ldr	r2, [sp, #24]
	mov	r1, fp
	ldrsh	r0, [r2, r1]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #1
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_08144ef6:
	ldr	r4, [sp, #28]
	ldr	r2, [sp, #48]
	adds	r4, #16
	cmp	r2, r4
	bne.n	.L_08144f1a
	ldr	r2, [sp, #24]
	mov	r3, fp
	ldrsh	r0, [r3, r2]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldr	r4, [sp, #8]
.L_08144f1a:
	ldr	r3, [r5, #8]
	add	r0, sp, #80
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	add	r7, sp, #92
	ldr	r3, [r5, #16]
	adds	r1, r7, #0
	str	r3, [r0, #8]
	str	r4, [sp, #8]
	bl	sub_0815e1ec
	ldr	r3, [r7, #0]
	ldr	r5, [sp, #44]
	ldr	r4, [sp, #8]
	adds	r2, r3, r5
	str	r2, [r7, #0]
	ldr	r0, [sp, #48]
	mov	r3, r8
	adds	r3, #104
	cmp	r0, r3
	bge.n	.L_08145030
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_08144f4e
	adds	r3, #3
.L_08144f4e:
	asrs	r6, r3, #2
	ldr	r3, [sp, #48]
	movs	r1, #6
	mov	sl, r1
	cmp	r3, r4
	ble.n	.L_08144f70
	mov	r5, r8
	subs	r0, r3, r5
	movs	r1, #3
	subs	r0, #88
	str	r2, [sp, #12]
	bl	sub_08002054
	mov	r1, sl
	ldr	r2, [sp, #12]
	subs	r1, r1, r0
	mov	sl, r1
.L_08144f70:
	cmp	r6, #2
	ble.n	.L_08144f7a
	movs	r3, #1
	ands	r3, r6
	adds	r6, r3, #1
.L_08144f7a:
	ldr	r5, [sp, #48]
	mov	r3, r8
	adds	r3, #100
	cmp	r5, r3
	bge.n	sub_08144fdc
	ldr	r0, [pc, #284]
	lsls	r5, r6, #1
	ldrh	r1, [r0, r5]
	ldr	r3, [sp, #56]
	movs	r0, #224
	adds	r1, r3, r1
	ldr	r3, [pc, #276]
	lsls	r0, r0, #3
	ldrb	r4, [r3, r6]
	ldr	r3, [pc, #276]
	adds	r1, r1, r0
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	subs	r2, r2, r4
	subs	r3, r3, r0
	adds	r3, #8
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x483c
	ldr	r2, [sp, #56]
	ldrh	r1, [r0, r5]
	ldr	r5, [pc, #244]
	movs	r3, #224
	ldrb	r4, [r5, r6]
	ldr	r5, [pc, #232]
	adds	r1, r2, r1
	ldrb	r0, [r5, r6]
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [r7, #4]
	ldr	r2, [r7, #0]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	subs	r3, r3, r4
	adds	r3, #8
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x687b
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_08145036
	ldr	r2, [sp, #56]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #5
	adds	r6, r7, #0
	adds	r5, r2, r3
	movs	r7, #1
.L_08144ff4:
	ldr	r2, [r6, #0]
	mov	r0, sl
	subs	r2, r2, r0
	str	r0, [sp, #0]
	ldr	r4, [sp, #60]
	mov	r3, r8
	str	r7, [sp, #4]
	ldr	r0, [sp, #52]
	adds	r1, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4651
	ldr	r2, [r6, #0]
	ldr	r3, [sp, #32]
	str	r1, [sp, #0]
	str	r7, [sp, #4]
	ldr	r0, [sp, #52]
	ldr	r4, [r3, #4]
	adds	r1, r5, #0
	mov	r3, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6873
	movs	r0, #1
	add	r8, r0
	cmp	r8, r3
	bne.n	.L_08144ff4
	mov	r1, fp
	ldr	r3, [r1, #20]
	b.n	.L_0814503a
.L_08145030:
	mov	r2, fp
	ldr	r3, [r2, #20]
	b.n	.L_0814503a
.L_08145036:
	mov	r5, fp
	ldr	r3, [r5, #20]
.L_0814503a:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #28]
	movs	r2, #1
	adds	r0, #2
	adds	r1, #16
	add	r9, r2
	str	r0, [sp, #24]
	str	r1, [sp, #28]
	cmp	r9, r3
	beq.n	.L_08145050
	b.n	.L_08144eae
.L_08145050:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	movs	r5, #240
	ldr	r3, [sp, #56]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #48]
	mov	r1, fp
	adds	r0, #1
	str	r0, [sp, #48]
	ldr	r3, [r1, #20]
	lsls	r3, r3, #4
	adds	r3, #116
	cmp	r0, r3
	beq.n	.L_08145084
	b.n	.L_08144c8e
.L_08145084:
	ldr	r0, [pc, #40]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #140
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0819786c
	.4byte 0xffff0000
	.4byte 0x08197874
	.4byte 0x0819787a
	.4byte 0x0819787d
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #4
	bl	sub_0814512c
	pop	{pc}
	.align 2, 0
