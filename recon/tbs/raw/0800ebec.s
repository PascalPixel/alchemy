.syntax unified
	.thumb
	.set sub_0800447c, 0x0800447c
	.set sub_080044d0, 0x080044d0
	.set sub_080045d4, 0x080045d4
	.set sub_0800ba30, 0x0800ba30
	.set sub_0800c150, 0x0800c150
	.set sub_0800c2d8, 0x0800c2d8
	.set sub_0800c300, 0x0800c300
	.set sub_0800d14c, 0x0800d14c
	.set sub_0800d924, 0x0800d924
	.set sub_0800eaf8, 0x0800eaf8
	.set sub_0800eba0, 0x0800eba0
	.set sub_080120dc, 0x080120dc
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080f9010, 0x080f9010
	.global FieldObject_UpdatePlayerControl
	.global Func_0800ebec
	.thumb_func
FieldObject_UpdatePlayerControl:
Func_0800ebec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #104
	adds	r7, r0, #0
	movs	r0, #0
	str	r0, [sp, #20]
	str	r0, [sp, #16]
	ldr	r5, [pc, #488]
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0800ec40
	movs	r0, #175
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0800ec40
	movs	r2, #128
	ldr	r1, [pc, #472]
	movs	r0, #0
	lsls	r2, r2, #2
.L_0800ec20:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L_0800ec2a
	adds	r0, #1
.L_0800ec2a:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_0800ec20
	adds	r3, r0, #0
	subs	r3, #136
	cmp	r3, #0
	bge.n	.L_0800ec3e
	movs	r0, #135
	bl	sub_080f9010
.L_0800ec3e:
	ldr	r5, [pc, #432]
.L_0800ec40:
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0800ec76
	ldr	r5, [pc, #432]
	movs	r2, #128
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800ec78
	adds	r3, r2, #0
.L_0800ec56:
	subs	r3, #1
	cmp	r3, #0
	bne.n	.L_0800ec56
	movs	r3, #95
.L_0800ec5e:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L_0800ec5e
	movs	r3, #63
.L_0800ec66:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L_0800ec66
	movs	r3, #63
.L_0800ec6e:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L_0800ec6e
	b.n	.L_0800ec78
.L_0800ec76:
	ldr	r5, [pc, #384]
.L_0800ec78:
	ldr	r3, [pc, #384]
	movs	r1, #135
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldrh	r2, [r3, #0]
	ldr	r3, [r5, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800ec9c
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r2, #5
	str	r3, [r7, #52]
	str	r2, [sp, #8]
	b.n	.L_0800ecac
.L_0800ec9c:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	movs	r3, #2
	str	r3, [sp, #8]
.L_0800ecac:
	ldr	r0, [pc, #336]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0800ecd4
	ldr	r5, [pc, #320]
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800ecd6
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	movs	r0, #5
	str	r3, [r7, #52]
	str	r0, [sp, #8]
	b.n	.L_0800ecd6
.L_0800ecd4:
	ldr	r5, [pc, #288]
.L_0800ecd6:
	ldr	r3, [r5, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ldr	r1, [pc, #292]
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	lsls	r3, r3, #16
	str	r3, [sp, #4]
	lsrs	r1, r3, #16
	ldr	r3, [pc, #284]
	cmp	r1, r3
	bne.n	.L_0800ecfa
	ldr	r0, [sp, #20]
	movs	r3, #4
	orrs	r0, r3
	str	r0, [sp, #20]
	b.n	.L_0800f0c6
.L_0800ecfa:
	movs	r2, #0
	movs	r3, #92
	str	r2, [sp, #20]
	add	r3, sp
	mov	fp, r3
	ldr	r3, [r7, #8]
	mov	r0, fp
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #12
	mov	r2, fp
	bl	sub_0800447c
	ldr	r3, [pc, #208]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0800ed38
	ldr	r3, [r5, #0]
	movs	r2, #128
	ldr	r1, [sp, #4]
	lsls	r2, r2, #2
	asrs	r1, r1, #16
	ands	r3, r2
	str	r1, [sp, #12]
	cmp	r3, #0
	beq.n	.L_0800ed38
	b.n	.L_0800f0c6
.L_0800ed38:
	adds	r0, r7, #0
	mov	r1, fp
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ee14
	ldr	r3, [r7, #8]
	add	r5, sp, #80
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	ldr	r3, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #12
	movs	r0, #128
	mov	r8, r2
	lsrs	r6, r3, #16
	lsls	r0, r0, #5
	adds	r1, r6, r0
	adds	r2, r5, #0
	mov	r0, r8
	bl	sub_0800447c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ee14
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	ldr	r2, [pc, #136]
	mov	r0, r8
	adds	r1, r6, r2
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ee14
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r1, r6, r3
	mov	r0, r8
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ee14
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #68]
	str	r3, [r5, #8]
	adds	r1, r6, r0
	adds	r2, r5, #0
	mov	r0, r8
	bl	sub_0800447c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ee14
	ldr	r1, [sp, #4]
	asrs	r1, r1, #16
	lsls	r2, r1, #16
	str	r1, [sp, #12]
	str	r2, [sp, #0]
	b.n	.L_0800ef44
	.4byte 0x03001f54
	.4byte 0x03001810
	.4byte 0x03001ae8
	.4byte 0x02000240
	.4byte 0x0000017f
	.4byte 0x08013254
	.4byte 0x0000ffff
	.4byte 0xfffff000
	.2byte 0xe000
	.2byte 0xffff
.L_0800ee14:
	.2byte 0x9801
	add	r3, sp, #24
	movs	r1, #128
	mov	sl, r3
	lsls	r1, r1, #5
	lsrs	r3, r0, #16
	adds	r2, r3, r1
	ldr	r1, [pc, #868]
	mov	r0, sl
	strh	r2, [r0, #0]
	adds	r2, r3, r1
	movs	r1, #128
	lsls	r1, r1, #6
	strh	r2, [r0, #2]
	adds	r2, r3, r1
	ldr	r1, [pc, #856]
	strh	r2, [r0, #4]
	adds	r2, r3, r1
	movs	r1, #192
	lsls	r1, r1, #6
	strh	r2, [r0, #6]
	adds	r2, r3, r1
	ldr	r1, [pc, #844]
	strh	r2, [r0, #8]
	adds	r3, r3, r1
	mov	r2, sl
	strh	r3, [r2, #10]
	movs	r3, #0
	mov	r9, r3
	mov	r8, fp
.L_0800ee50:
	mov	r0, r9
	lsls	r3, r0, #1
	mov	r1, sl
	ldrsh	r1, [r1, r3]
	str	r1, [sp, #12]
	ldr	r3, [r7, #8]
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	lsls	r1, r1, #16
	str	r3, [r0, #8]
	lsrs	r6, r1, #16
	movs	r0, #128
	str	r1, [sp, #0]
	lsls	r0, r0, #12
	adds	r1, r6, #0
	mov	r2, r8
	bl	sub_0800447c
	adds	r0, r7, #0
	mov	r1, r8
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ef24
	ldr	r3, [r7, #8]
	add	r5, sp, #80
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r2, #128
	lsls	r2, r2, #5
	movs	r0, #128
	adds	r1, r6, r2
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ef24
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	ldr	r3, [pc, #712]
	movs	r0, #128
	adds	r1, r6, r3
	lsls	r0, r0, #12
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ef24
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #6
	adds	r1, r6, r0
	movs	r0, #128
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ef24
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	ldr	r2, [pc, #640]
	movs	r0, #128
	adds	r1, r6, r2
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	beq.n	.L_0800ef44
.L_0800ef24:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #6
	blt.n	.L_0800ee50
	ldr	r3, [r7, #8]
	mov	r1, fp
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	str	r3, [r1, #8]
	ldr	r2, [sp, #20]
	movs	r3, #1
	orrs	r2, r3
	str	r2, [sp, #20]
.L_0800ef44:
	add	r3, sp, #68
	mov	fp, r3
	ldr	r3, [r7, #8]
	mov	r0, fp
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	ldr	r2, [sp, #0]
	movs	r0, #128
	lsrs	r1, r2, #16
	lsls	r0, r0, #11
	mov	r2, fp
	bl	sub_0800447c
	ldr	r3, [pc, #556]
	ldr	r3, [r3, #0]
	mov	r8, r3
	mov	r6, r8
	movs	r3, #63
	mov	r9, r3
	adds	r6, #8
.L_0800ef72:
	ldrh	r3, [r7, #32]
	mov	r0, r8
	subs	r1, r3, #2
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0800ef80
	b.n	.L_0800f09a
.L_0800ef80:
	mov	r3, r8
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0800ef90
	b.n	.L_0800f09a
.L_0800ef90:
	cmp	r8, r7
	bne.n	.L_0800ef96
	b.n	.L_0800f09a
.L_0800ef96:
	ldrh	r3, [r6, #24]
	adds	r0, r6, #0
	subs	r3, #2
	mov	r2, fp
	bl	sub_0800eba0
	cmp	r0, #0
	blt.n	.L_0800f09a
	ldr	r3, [r6, #80]
	ldr	r2, [pc, #492]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #2
	cmp	r3, r1
	bne.n	.L_0800f092
	ldr	r3, [r7, #16]
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #0]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	subs	r1, r1, r3
	bl	sub_080044d0
	ldr	r3, [r6, #0]
	add	r5, sp, #80
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	lsls	r0, r0, #16
	ldr	r3, [r6, #8]
	asrs	r2, r0, #16
	lsrs	r0, r0, #16
	mov	sl, r0
	movs	r0, #128
	str	r3, [r5, #8]
	lsls	r0, r0, #7
	str	r2, [sp, #12]
	mov	r1, sl
	adds	r2, r5, #0
	bl	sub_0800447c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0800d924
	cmp	r0, #0
	bne.n	.L_0800f092
	ldr	r3, [r6, #0]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	ldr	r3, [r6, #8]
	movs	r0, #160
	lsls	r0, r0, #12
	mov	r1, sl
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800f092
	ldr	r3, [r6, #0]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	movs	r1, #128
	ldr	r3, [r6, #8]
	lsls	r1, r1, #5
	movs	r0, #160
	add	r1, sl
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800f092
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800f092
	ldr	r3, [r6, #0]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	ldr	r1, [pc, #304]
	ldr	r3, [r6, #8]
	movs	r0, #160
	add	r1, sl
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800f092
	movs	r0, #128
	lsls	r0, r0, #7
	mov	r1, sl
	adds	r2, r6, #0
	bl	sub_0800447c
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	str	r3, [r6, #56]
	ldr	r3, [sp, #16]
	movs	r0, #1
	orrs	r3, r0
	str	r3, [sp, #16]
	b.n	.L_0800f09a
.L_0800f092:
	ldr	r1, [sp, #20]
	movs	r3, #2
	orrs	r1, r3
	str	r1, [sp, #20]
.L_0800f09a:
	movs	r2, #1
	negs	r2, r2
	add	r9, r2
	movs	r3, #112
	mov	r0, r9
	adds	r6, #112
	add	r8, r3
	cmp	r0, #0
	blt.n	.L_0800f0ae
	b.n	.L_0800ef72
.L_0800f0ae:
	ldr	r1, [sp, #20]
	cmp	r1, #0
	bne.n	.L_0800f0c6
	ldr	r2, [sp, #16]
	cmp	r2, #0
	beq.n	.L_0800f0c6
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r7, #52]
.L_0800f0c6:
	ldr	r3, [pc, #212]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0800f0ee
	ldr	r0, [sp, #20]
	movs	r2, #3
	ands	r2, r0
	cmp	r2, #0
	beq.n	.L_0800f0e6
	movs	r1, #206
	lsls	r1, r1, #1
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0800f0ee
.L_0800f0e6:
	movs	r0, #206
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
.L_0800f0ee:
	ldr	r1, [sp, #16]
	cmp	r1, #0
	beq.n	.L_0800f0fe
	adds	r0, r7, #0
	movs	r1, #8
	bl	sub_0800c300
	b.n	.L_0800f130
.L_0800f0fe:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	beq.n	.L_0800f128
	ldr	r3, [pc, #152]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	movs	r5, #9
	cmp	r3, #0
	bne.n	.L_0800f11e
	movs	r5, #22
.L_0800f11e:
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0800c300
	b.n	.L_0800f130
.L_0800f128:
	adds	r0, r7, #0
	ldr	r1, [sp, #8]
	bl	sub_0800c300
.L_0800f130:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	beq.n	.L_0800f1a4
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800f170
	ldr	r0, [sp, #4]
	ldrh	r1, [r7, #6]
	lsrs	r3, r0, #16
	subs	r3, r3, r1
	lsls	r3, r3, #16
	movs	r2, #128
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_0800f164
	adds	r3, r2, #0
.L_0800f164:
	ldr	r2, [pc, #32]
	cmp	r3, r2
	bge.n	.L_0800f16c
	adds	r3, r2, #0
.L_0800f16c:
	adds	r3, r1, r3
	strh	r3, [r7, #6]
.L_0800f170:
	movs	r1, #100
	adds	r1, r1, r7
	mov	sl, r1
	movs	r3, #0
	mov	r2, sl
	strh	r3, [r2, #0]
	adds	r2, r7, #0
	adds	r2, #102
	movs	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_0800f1fa
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.4byte 0x03001e64
	.4byte 0xff000200
	.4byte 0x03001ebc
	.2byte 0x0240
	.2byte 0x0200
.L_0800f1a4:
	add	r3, sp, #92
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	adds	r0, r7, #0
	ldr	r3, [r3, #8]
	bl	sub_0800d14c
	ldr	r1, [r7, #36]
	ldr	r4, [pc, #252]
	adds	r0, r1, #0
	mov	ip, pc
	bx	r4
	ldr	r1, [r7, #44]
	adds	r3, r0, #0
	adds	r0, r1, #0
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	sub_080045d4
	ldr	r3, [sp, #20]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	lsls	r1, r2, #16
	adds	r2, r7, #0
	adds	r2, #36
	lsrs	r1, r1, #16
	bl	sub_0800447c
	movs	r3, #100
	adds	r3, r3, r7
	mov	sl, r3
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_0800f1fa
	subs	r3, r2, #1
	mov	r1, sl
	strh	r3, [r1, #0]
.L_0800f1fa:
	ldr	r3, [pc, #188]
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #4]
	ldrb	r3, [r3, #23]
	lsrs	r2, r2, #16
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0800f2d8
	mov	r1, sl
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_0800f2d8
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L_0800f2d8
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	movs	r0, #25
	bl	sub_0800c150
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0800f2d8
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #140]
	str	r3, [r6, #20]
	ldr	r5, [r6, #80]
	bl	sub_0800c2d8
	adds	r2, r6, #0
	movs	r3, #2
	adds	r2, #35
	add	r0, sp, #20
	strb	r3, [r2, #0]
	ldrb	r0, [r0, #0]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	cmp	r5, #0
	beq.n	.L_0800f270
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_0800ba30
	add	r1, sp, #20
	adds	r3, r5, #0
	ldrb	r1, [r1, #0]
	adds	r3, #38
	strb	r1, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	add	r3, r8
	strh	r3, [r5, #30]
	ldrb	r3, [r5, #9]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r5, #9]
.L_0800f270:
	movs	r2, #102
	adds	r2, r2, r7
	mov	r8, r2
	mov	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	ldrh	r2, [r2, #0]
	cmp	r3, #2
	bne.n	.L_0800f294
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_0800ba30
	add	r2, sp, #20
	ldrh	r2, [r2, #0]
	mov	r3, r8
	strh	r2, [r3, #0]
	ldr	r2, [pc, #28]
.L_0800f294:
	lsls	r3, r2, #16
	cmp	r3, #0
	beq.n	.L_0800f2a0
	movs	r3, #128
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
.L_0800f2a0:
	ldr	r3, [sp, #8]
	cmp	r3, #5
	bne.n	.L_0800f2c0
	movs	r3, #12
	mov	r0, sl
	strh	r3, [r0, #0]
	b.n	.L_0800f2c6
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03000118
	.4byte 0x03001e70
	.2byte 0x3274
	.2byte 0x0801
.L_0800f2c0:
	movs	r3, #18
	mov	r1, sl
	strh	r3, [r1, #0]
.L_0800f2c6:
	mov	r2, r8
	ldrh	r3, [r2, #0]
	ldr	r2, [pc, #8]
	mov	r0, r8
	eors	r3, r2
	strh	r3, [r0, #0]
	b.n	.L_0800f2d8
	.2byte 0x0001
	.2byte 0x0000
.L_0800f2d8:
	bl	sub_0800eaf8
	ldrh	r3, [r7, #4]
	adds	r3, #1
	movs	r0, #1
	strh	r3, [r7, #4]
	add	sp, #104
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
