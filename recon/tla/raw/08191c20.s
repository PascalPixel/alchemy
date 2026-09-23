.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0800205c, 0x0800205c
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014c4c, 0x08014c4c
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015128, 0x08015128
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_08020048, 0x08020048
	.set sub_08118010, 0x08118010
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08143eb4, 0x08143eb4
	.set sub_08144aac, 0x08144aac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b410, 0x0815b410
	.set sub_0815f000, 0x0815f000
	.set sub_0816442c, 0x0816442c
	.set sub_0816467c, 0x0816467c
	.set sub_08164a4c, 0x08164a4c
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08191c20
	.thumb_func
Func_08191c20:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r1, #0
	mov	r9, r3
	lsls	r3, r5, #1
	mov	r8, r3
	adds	r7, r0, #0
	mov	sl, r2
	movs	r6, #0
	mov	r1, r8
	cmp	r3, #63
	bls.n	.L_08191c40
	movs	r1, #63
.L_08191c40:
	movs	r0, #63
	bl	sub_0800205c
	adds	r1, r0, #0
	cmp	r1, #5
	bhi.n	.L_08191c4e
	movs	r1, #6
.L_08191c4e:
	lsls	r1, r1, #1
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_08191c88
	adds	r2, r7, #0
.L_08191c58:
	adds	r6, r6, r1
	mov	r3, r9
	muls	r3, r6
	asrs	r3, r3, #16
	cmp	r3, #63
	ble.n	.L_08191c66
	movs	r3, #63
.L_08191c66:
	adds	r0, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r0, r5
	bne.n	.L_08191c58
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_08191c88
	adds	r2, r5, r7
	subs	r1, r2, #1
.L_08191c7a:
	ldrb	r3, [r1, #0]
	adds	r0, #1
	strb	r3, [r2, #0]
	subs	r1, #1
	adds	r2, #1
	cmp	r0, r5
	bne.n	.L_08191c7a
.L_08191c88:
	mov	r3, sl
	movs	r6, #1
	cmp	r3, #1
	beq.n	.L_08191cba
	mov	lr, r8
	adds	r4, r5, #0
.L_08191c94:
	mov	r3, lr
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_08191cb2
	lsls	r3, r4, #1
	mov	ip, r8
	adds	r1, r3, r7
	adds	r2, r7, #0
.L_08191ca4:
	ldrb	r3, [r2, #0]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, ip
	bne.n	.L_08191ca4
.L_08191cb2:
	adds	r6, #1
	adds	r4, r4, r5
	cmp	r6, sl
	bne.n	.L_08191c94
.L_08191cba:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #176
	ldr	r2, [r2, #0]
	ldr	r1, [r3, #92]
	mov	sl, r2
	movs	r2, #240
	ldr	r3, [r3, #36]
	lsls	r2, r2, #7
	adds	r2, #240
	adds	r1, r1, r2
	mov	r9, r3
	ldr	r3, [r1, #0]
	movs	r7, #0
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_08191d30
	movs	r3, #0
	adds	r6, r1, #0
	mov	r8, r3
	movs	r5, #36
.L_08191cfa:
	ldr	r3, [r6, #0]
	adds	r7, #1
	ldrsh	r0, [r3, r5]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	mov	r3, r8
	str	r3, [r2, #8]
	movs	r3, #240
	lsls	r3, r3, #15
	str	r3, [r2, #12]
	mov	r3, r8
	str	r3, [r2, #16]
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r2, #72]
	ldr	r3, [r6, #0]
	movs	r1, #1
	ldrsh	r0, [r3, r5]
	bl	sub_08118088
	ldr	r3, [r6, #0]
	adds	r5, #2
	ldr	r3, [r3, #20]
	cmp	r7, r3
	bne.n	.L_08191cfa
.L_08191d30:
	mov	r2, sl
	movs	r3, #0
	str	r3, [r2, #16]
	ldr	r2, [pc, #36]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	bl	sub_08014c4c
	movs	r3, #206
	lsls	r3, r3, #3
	add	r3, r9
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #24
	bl	sub_08118040
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x11e0
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	str	r0, [sp, #40]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	str	r0, [sp, #36]
	movs	r0, #0
	ldr	r1, [r3, #92]
	ldr	r3, [r3, #100]
	mov	fp, r1
	str	r3, [sp, #32]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #52
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #28]
	bl	sub_08144aac
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #220
	lsls	r1, r1, #6
	ldr	r0, [pc, #24]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #16]
	ldr	r1, [sp, #32]
	b.n	.L_08191dd8
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000013e
	.4byte 0x000000c1
	.2byte 0x0134
	.2byte 0x0000
.L_08191dd8:
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	bl	sub_0815b410
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #756]
	bl	sub_080145a8
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [sp, #20]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, fp
	movs	r4, #128
	lsls	r4, r4, #15
	ldr	r0, [r3, #0]
	str	r4, [sp, #24]
	bl	sub_0814cc4c
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r0, #12
	adds	r1, #142
	movs	r2, #2
	bl	sub_08152404
	movs	r0, #0
	mov	r9, r0
.L_08191e36:
	ldr	r3, [pc, #704]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08191e56
	mov	r1, r9
	cmp	r1, #231
	bgt.n	.L_08191e56
	cmp	r1, #15
	ble.n	.L_08191e56
	movs	r0, #144
	bl	sub_081c0010
	movs	r2, #232
	mov	r9, r2
.L_08191e56:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_08191f3c
	movs	r4, #0
	movs	r2, #1
	mov	r3, fp
	mov	sl, r4
	negs	r2, r2
	adds	r3, #24
.L_08191e68:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #64
	bne.n	.L_08191e68
	ldr	r5, [pc, #644]
	movs	r2, #0
	mov	sl, r2
	movs	r6, #0
.L_08191e7e:
	bl	sub_08014878
	mov	r3, sl
	str	r3, [r5, #24]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #64
	movs	r4, #1
	movs	r0, #128
	lsls	r3, r3, #16
	add	sl, r4
	lsls	r0, r0, #1
	str	r3, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	adds	r5, #28
	cmp	sl, r0
	bne.n	.L_08191e7e
	movs	r1, #0
	mov	sl, r1
	movs	r7, #0
	movs	r6, #0
	mov	r5, fp
.L_08191eb2:
	mov	r2, sl
	adds	r3, r6, r2
	adds	r3, #44
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	str	r7, [r5, #12]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	adds	r3, #2
	negs	r3, r3
	lsls	r3, r3, #16
	str	r3, [r5, #16]
	str	r7, [r5, #20]
	bl	sub_08014878
	mov	r3, sl
	negs	r2, r3
	movs	r4, #1
	movs	r3, #15
	ands	r3, r0
	add	sl, r4
	subs	r2, r2, r3
	mov	r0, sl
	str	r2, [r5, #24]
	adds	r6, #3
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_08191eb2
	movs	r5, #168
	movs	r1, #0
	lsls	r5, r5, #2
	mov	sl, r1
	movs	r7, #3
	movs	r6, #0
	add	r5, fp
.L_08191f02:
	movs	r3, #216
	lsls	r3, r3, #14
	str	r3, [r5, #0]
	movs	r3, #224
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	str	r6, [r5, #24]
	bl	sub_08014878
	movs	r2, #128
	ands	r0, r7
	lsls	r2, r2, #10
	lsls	r0, r0, #16
	adds	r0, r0, r2
	negs	r0, r0
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r7
	negs	r0, r0
	add	sl, r3
	lsls	r0, r0, #15
	mov	r4, sl
	str	r0, [r5, #16]
	subs	r6, #2
	adds	r5, #28
	cmp	r4, #16
	bne.n	.L_08191f02
.L_08191f3c:
	movs	r0, #16
	adds	r0, #255
	cmp	r9, r0
	ble.n	.L_08191f46
	b.n	.L_08192118
.L_08191f46:
	mov	r2, r9
	mov	r3, r9
	subs	r2, #91
	subs	r3, #217
	str	r2, [sp, #12]
	str	r3, [sp, #16]
	ldr	r7, [pc, #424]
	movs	r1, #0
	mov	sl, r1
.L_08191f58:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	beq.n	.L_08191f60
	b.n	.L_081920c4
.L_08191f60:
	ldr	r2, [pc, #412]
	movs	r4, #7
	mov	r3, sl
	ands	r3, r4
	ldrb	r6, [r2, r3]
	ldr	r2, [pc, #408]
	lsls	r0, r6, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #32]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r7, r3]
	asrs	r3, r6, #1
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r7, r4]
	str	r0, [sp, #4]
	subs	r3, r3, r6
	str	r6, [sp, #0]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9903
	lsls	r0, r6, #14
	mov	r8, r0
	cmp	r1, #28
	bhi.n	.L_08191fde
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bne.n	.L_08191fde
	movs	r4, #6
	ldrsh	r3, [r7, r4]
	cmp	r3, #113
	bgt.n	.L_08191fda
	bl	sub_08014878
	movs	r5, #63
	ands	r5, r0
	ldr	r3, [r7, #0]
	movs	r0, #224
	lsls	r0, r0, #14
	lsls	r5, r5, #16
	adds	r5, r5, r0
	str	r5, [r7, #12]
	subs	r5, r5, r3
	ldr	r3, [r7, #4]
	movs	r0, #228
	lsls	r0, r0, #15
	subs	r0, r0, r3
	mov	r1, r8
	bl	sub_08002054
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_08002054
	lsls	r6, r6, #14
	str	r0, [r7, #12]
	b.n	.L_08191fde
.L_08191fda:
	lsls	r6, r6, #14
	mov	r8, r6
.L_08191fde:
	mov	r3, r9
	subs	r3, #142
	cmp	r3, #19
	bhi.n	.L_08192046
	ldr	r3, [r7, #0]
	ldr	r1, [pc, #284]
	adds	r5, r3, r1
	cmp	r5, #0
	bge.n	.L_08191ff2
	negs	r5, r5
.L_08191ff2:
	bl	sub_08014878
	ldr	r2, [r7, #4]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	movs	r3, #31
	asrs	r2, r2, #1
	ands	r3, r0
	lsls	r3, r3, #16
	subs	r2, r2, r5
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #15
	adds	r1, r2, r3
	cmp	r1, #0
	bge.n	.L_0819201a
	movs	r4, #192
	lsls	r4, r4, #15
	adds	r4, #31
	adds	r1, r2, r4
.L_0819201a:
	ldr	r3, [r7, #0]
	ldr	r0, [pc, #236]
	asrs	r2, r1, #5
	cmp	r3, r0
	bgt.n	.L_0819202a
	cmp	r2, #0
	ble.n	.L_0819202a
	negs	r2, r2
.L_0819202a:
	movs	r1, #192
	lsls	r1, r1, #15
	cmp	r3, r1
	ble.n	.L_08192038
	cmp	r2, #0
	bge.n	.L_08192038
	negs	r2, r2
.L_08192038:
	cmp	r2, #0
	bge.n	.L_0819203e
	adds	r2, #7
.L_0819203e:
	ldr	r3, [r7, #12]
	asrs	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r7, #12]
.L_08192046:
	ldr	r0, [r7, #4]
	asrs	r0, r0, #5
	bl	sub_08002096
	movs	r1, #5
	bl	sub_08002054
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #0]
	adds	r2, r2, r0
	adds	r3, r3, r2
	ldr	r2, [r7, #16]
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	add	r2, r8
	adds	r3, r3, r2
	movs	r2, #228
	lsls	r2, r2, #15
	str	r3, [r7, #4]
	cmp	r3, r2
	ble.n	.L_081920c8
	mov	r3, r9
	cmp	r3, #215
	ble.n	.L_081920a2
	movs	r3, #240
	movs	r4, #0
	lsls	r3, r3, #15
	str	r4, [r7, #24]
	str	r3, [r7, #0]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	subs	r3, #8
	lsls	r3, r3, #16
	movs	r0, #0
	str	r3, [r7, #12]
	str	r0, [r7, #16]
	b.n	.L_081920c8
.L_081920a2:
	bl	sub_08014878
	movs	r5, #255
	ands	r0, r5
	str	r0, [r7, #24]
	bl	sub_08014878
	ldr	r3, [pc, #92]
	ands	r0, r5
	subs	r0, #64
	movs	r1, #0
	lsls	r0, r0, #16
	str	r0, [r7, #0]
	str	r3, [r7, #4]
	str	r1, [r7, #12]
	str	r1, [r7, #16]
	b.n	.L_081920c8
.L_081920c4:
	subs	r3, #1
	str	r3, [r7, #24]
.L_081920c8:
	ldr	r2, [sp, #16]
	cmp	r2, #28
	bhi.n	.L_081920e0
	ldr	r3, [r7, #12]
	ldr	r4, [pc, #64]
	movs	r0, #128
	adds	r3, r3, r4
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #6
	adds	r3, r3, r0
	str	r3, [r7, #16]
.L_081920e0:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #1
	adds	r7, #28
	cmp	sl, r2
	beq.n	.L_081920f0
	b.n	.L_08191f58
.L_081920f0:
	b.n	.L_0819211e
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x03001150
	.4byte 0x02010000
	.4byte 0x08196f28
	.4byte 0x08197410
	.4byte 0xffa00000
	.4byte 0x005fffff
	.4byte 0xfff80000
	.2byte 0xe000
	.2byte 0xffff
.L_08192118:
	.2byte 0x464b
	subs	r3, #91
	str	r3, [sp, #12]
.L_0819211e:
	ldr	r4, [sp, #12]
	cmp	r4, #164
	bhi.n	.L_081921e4
	movs	r0, #0
	mov	sl, r0
	movs	r7, #0
	mov	r5, fp
	movs	r6, #0
.L_0819212e:
	ldr	r4, [r5, #20]
	cmp	r4, #2
	beq.n	.L_081921d6
	ldr	r1, [r5, #24]
	cmp	r1, #23
	bhi.n	.L_08192174
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, fp
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	ldr	r0, [sp, #28]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	ldr	r1, [r5, #24]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
.L_08192174:
	adds	r3, r1, #1
	str	r3, [r5, #24]
	cmp	r3, #24
	bne.n	.L_081921b6
	mov	r1, sl
	adds	r3, r6, r1
	adds	r3, #44
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	str	r7, [r5, #24]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	adds	r3, #2
	negs	r3, r3
	lsls	r3, r3, #16
	str	r3, [r5, #16]
	mov	r3, r9
	subs	r3, #97
	str	r7, [r5, #20]
	cmp	r3, #42
	bhi.n	.L_081921ac
	movs	r3, #1
	b.n	.L_081921b4
.L_081921ac:
	mov	r2, r9
	cmp	r2, #140
	ble.n	.L_081921b6
	movs	r3, #2
.L_081921b4:
	str	r3, [r5, #20]
.L_081921b6:
	mov	r3, r9
	cmp	r3, #136
	bne.n	.L_081921d6
	ldr	r3, [r5, #0]
	ldr	r4, [pc, #840]
	cmp	r3, r4
	bgt.n	.L_081921cc
	ldr	r3, [r5, #12]
	ldr	r0, [pc, #836]
	adds	r3, r3, r0
	b.n	.L_081921d4
.L_081921cc:
	ldr	r3, [r5, #12]
	movs	r1, #128
	lsls	r1, r1, #11
	adds	r3, r3, r1
.L_081921d4:
	str	r3, [r5, #12]
.L_081921d6:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r5, #28
	adds	r6, #3
	cmp	r3, #12
	bne.n	.L_0819212e
.L_081921e4:
	mov	r3, r9
	subs	r3, #200
	cmp	r3, #71
	bhi.n	.L_08192270
	mov	r4, r9
	cmp	r4, #200
	ble.n	.L_08192270
	movs	r5, #168
	movs	r0, #0
	lsls	r5, r5, #2
	mov	sl, r0
	add	r5, fp
.L_081921fc:
	ldr	r1, [r5, #24]
	cmp	r1, #23
	bhi.n	.L_08192238
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r2, #224
	lsls	r2, r2, #3
	movs	r0, #32
	add	r1, fp
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	ldr	r1, [r5, #24]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
.L_08192238:
	adds	r3, r1, #1
	str	r3, [r5, #24]
	cmp	r3, #24
	bne.n	.L_08192264
	movs	r3, #216
	lsls	r3, r3, #14
	str	r3, [r5, #0]
	movs	r3, #224
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	movs	r0, #128
	lsls	r3, r3, #16
	lsls	r0, r0, #10
	adds	r3, r3, r0
	negs	r3, r3
	str	r3, [r5, #12]
	movs	r3, #0
	str	r3, [r5, #24]
.L_08192264:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_081921fc
.L_08192270:
	mov	r3, r9
	cmp	r3, #135
	ble.n	.L_0819235c
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #648]
	ldr	r3, [sp, #44]
	movs	r4, #7
	ands	r3, r2
	ldr	r2, [pc, #644]
	orrs	r3, r4
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #44]
	movs	r3, #220
	lsls	r3, r3, #6
	add	r3, fp
	add	r2, sp, #44
	str	r3, [r2, #4]
	ldr	r3, [pc, #624]
	adds	r6, r0, #0
	str	r3, [r6, #8]
	mov	r0, r8
	movs	r3, #0
	str	r4, [r6, #0]
	str	r2, [r6, #16]
	str	r0, [r6, #12]
	strb	r3, [r6, #24]
	strb	r3, [r6, #25]
	mov	sl, r3
	mov	r7, r9
.L_081922be:
	ldr	r3, [pc, #604]
	mov	r1, sl
	ldrb	r3, [r3, r1]
	adds	r1, r3, #0
	adds	r1, #136
	cmp	r9, r1
	ble.n	.L_08192344
	mov	r3, r9
	subs	r2, r3, r1
	ldr	r3, [pc, #588]
	mov	r4, sl
	ldrb	r3, [r3, r4]
	movs	r0, #128
	muls	r2, r3
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r2
	lsls	r3, r3, #4
	lsls	r0, r0, #8
	mov	r2, r9
	adds	r5, r3, r0
	subs	r3, r1, r2
	lsls	r3, r3, #3
	adds	r3, #56
	cmp	r3, #0
	ble.n	.L_081922f6
	movs	r3, #0
.L_081922f6:
	movs	r4, #64
	negs	r4, r4
	cmp	r3, r4
	ble.n	.L_08192344
	str	r3, [r6, #20]
	bl	sub_08014de4
	ldr	r3, [pc, #540]
	mov	r0, sl
	ldrsb	r1, [r3, r0]
	movs	r0, #128
	lsls	r1, r1, #16
	lsls	r0, r0, #13
	movs	r2, #0
	bl	sub_08015160
	adds	r1, r5, #0
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r0, #184
	bl	sub_08015024
	movs	r1, #7
	adds	r3, r7, #0
	ands	r3, r1
	lsls	r3, r3, #4
	ldr	r0, [pc, #500]
	strb	r3, [r6, #24]
	mov	r1, r8
	movs	r2, #32
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_08192344:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r7, #5
	cmp	r3, #4
	bne.n	.L_081922be
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
.L_0819235c:
	mov	r4, r9
	cmp	r4, #139
	ble.n	.L_0819236c
	movs	r0, #5
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #24]
	bl	sub_0816442c
.L_0819236c:
	movs	r0, #0
	mov	sl, r0
.L_08192370:
	mov	r1, sl
	lsls	r3, r1, #3
	adds	r3, #212
	cmp	r9, r3
	bne.n	.L_081923da
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #240
	mov	r2, fp
	ldr	r3, [r2, r3]
	movs	r7, #0
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_081923da
	movs	r6, #240
	lsls	r6, r6, #7
	lsls	r1, r1, #4
	adds	r6, #240
	movs	r4, #128
	mov	r8, r1
	add	r6, fp
	lsls	r4, r4, #9
	movs	r5, #36
.L_0819239e:
	ldr	r3, [r6, #0]
	movs	r2, #5
	ldrsh	r0, [r3, r5]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldr	r2, [sp, #40]
	ldr	r4, [sp, #8]
	ldrsh	r0, [r5, r2]
	mov	r3, r8
	adds	r3, #100
	str	r3, [sp, #4]
	movs	r3, #128
	adds	r2, r4, #0
	lsls	r3, r3, #11
	movs	r1, #1
	str	r4, [sp, #0]
	bl	sub_0815f000
	ldr	r3, [r6, #0]
	adds	r7, #1
	ldr	r3, [r3, #20]
	adds	r5, #2
	ldr	r4, [sp, #8]
	cmp	r7, r3
	bne.n	.L_0819239e
.L_081923da:
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	cmp	r0, #8
	bne.n	.L_08192370
	mov	r3, r9
	subs	r3, #201
	cmp	r3, #54
	bhi.n	.L_081923f8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
.L_081923f8:
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_08192404
	movs	r0, #246
	bl	sub_081c0010
.L_08192404:
	mov	r2, r9
	cmp	r2, #90
	bne.n	.L_08192410
	movs	r0, #225
	bl	sub_081c0010
.L_08192410:
	mov	r3, r9
	cmp	r3, #140
	bne.n	.L_0819241c
	movs	r0, #154
	bl	sub_081c0010
.L_0819241c:
	mov	r4, r9
	cmp	r4, #200
	bne.n	.L_08192428
	movs	r0, #208
	bl	sub_081c0010
.L_08192428:
	mov	r0, r9
	cmp	r0, #232
	bne.n	.L_08192434
	movs	r0, #144
	bl	sub_081c0010
.L_08192434:
	movs	r1, #136
	lsls	r1, r1, #1
	cmp	r9, r1
	bne.n	.L_0819245e
	movs	r0, #134
	bl	sub_081180e8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r3, [pc, #216]
	adds	r2, #132
	add	r2, fp
	str	r3, [r2, #0]
	bl	sub_0815b410
	b.n	.L_08192492
.L_0819245e:
	ldr	r3, [pc, #176]
	add	r3, r9
	cmp	r3, #15
	bhi.n	.L_08192492
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, fp
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r2, #238
	ldr	r3, [pc, #188]
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	str	r3, [r2, #0]
	movs	r3, #14
	adds	r3, #255
	cmp	r9, r3
	ble.n	.L_08192488
	ldr	r3, [pc, #172]
	str	r3, [r2, #0]
.L_08192488:
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164a4c
.L_08192492:
	movs	r0, #4
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	movs	r4, #1
	movs	r0, #1
	str	r3, [r2, #0]
	add	r9, r4
	bl	sub_08013560
	movs	r0, #148
	lsls	r0, r0, #1
	cmp	r9, r0
	beq.n	.L_081924be
	b.n	.L_08191e36
.L_081924be:
	ldr	r0, [pc, #120]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	movs	r5, #238
	ldr	r1, [sp, #20]
	movs	r0, #5
	ldr	r2, [sp, #24]
	lsls	r5, r5, #7
	bl	sub_0816467c
	adds	r5, #220
	movs	r1, #0
	mov	sl, r1
	add	r5, fp
.L_081924e6:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #12
	bne.n	.L_081924e6
	bl	sub_08143bb8
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x004fffff
	.4byte 0xfffc0000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08198ec4
	.4byte 0x08199f34
	.4byte 0x08199f38
	.4byte 0x08199f3c
	.4byte 0x08198cac
	.4byte 0x02020202
	.4byte 0x10101010
	.4byte 0x3f3f3f3f
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	ldr	r1, [r2, #92]
	mov	r9, r3
	movs	r2, #128
	ldr	r3, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #20
	strh	r3, [r2, #0]
	ldr	r2, [pc, #40]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #240
	adds	r1, r1, r2
	ldr	r3, [r1, #0]
	sub	sp, #28
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_081925c8
	adds	r6, r1, #0
	movs	r3, #0
	movs	r1, #36
	mov	sl, r3
	mov	r8, r1
	b.n	.L_0819258c
	.4byte 0x00000000
	.2byte 0x11e0
	.2byte 0x0300
.L_0819258c:
	ldr	r3, [r6, #0]
	mov	r2, r8
	ldrsh	r0, [r3, r2]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	mov	r2, sl
	str	r2, [r5, #8]
	bl	sub_08014878
	movs	r2, #15
	ands	r2, r0
	movs	r3, #16
	subs	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	movs	r1, #2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	mov	r3, sl
	str	r3, [r5, #72]
	ldr	r3, [r6, #0]
	adds	r7, #1
	ldr	r3, [r3, #20]
	add	r8, r1
	cmp	r7, r3
	bne.n	.L_0819258c
.L_081925c8:
	mov	r0, sp
	movs	r3, #255
	strh	r3, [r0, #0]
	movs	r1, #0
	bl	sub_08118010
	ldr	r1, [pc, #56]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	ldr	r5, [pc, #44]
	movs	r4, #160
	lsls	r4, r4, #19
	adds	r4, #192
	movs	r7, #0
.L_081925e8:
	ldrh	r2, [r4, #0]
	movs	r0, #31
	lsls	r3, r2, #16
	lsrs	r1, r3, #26
	ands	r0, r2
	lsrs	r3, r3, #21
	ands	r1, r5
	ands	r3, r5
	subs	r0, #16
	subs	r3, #24
	subs	r1, #20
	cmp	r0, #0
	bge.n	.L_08192604
	movs	r0, #0
.L_08192604:
	cmp	r3, #0
	bge.n	.L_08192614
	movs	r3, #0
	b.n	.L_08192614
	.4byte 0x0000001f
	.2byte 0x0045
	.2byte 0x0000
.L_08192614:
	cmp	r1, #0
	bge.n	.L_0819261a
	movs	r1, #0
.L_0819261a:
	lsls	r3, r3, #10
	lsls	r2, r0, #5
	orrs	r3, r2
	orrs	r3, r1
	adds	r7, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, #128
	bne.n	.L_081925e8
	mov	r2, r9
	movs	r3, #1
	str	r3, [r2, #16]
	ldr	r2, [pc, #16]
	movs	r3, #0
	strh	r3, [r2, #4]
	add	sp, #28
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x1120
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r1, [sp, #24]
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #104]
	ldr	r1, [r3, #92]
	mov	sl, r2
	ldr	r2, [r3, #96]
	adds	r3, #188
	str	r4, [sp, #16]
	ldr	r4, [r3, #0]
	mov	fp, r1
	mov	r9, r2
	str	r4, [sp, #20]
	cmp	r6, #0
	bne.n	.L_081926a6
	lsrs	r5, r0, #1
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #3
	adds	r1, r1, r5
	movs	r2, #136
	lsls	r1, r1, #5
	lsls	r2, r2, #6
	adds	r2, #18
	add	r1, fp
	adds	r1, r1, r2
	ldr	r2, [sp, #24]
	movs	r0, #20
	mov	r3, sl
	str	r0, [sp, #0]
	movs	r0, #40
	str	r0, [sp, #4]
	subs	r2, #10
	subs	r3, #20
	mov	r0, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe013
.L_081926a6:
	lsrs	r5, r0, #1
	lsls	r1, r5, #3
	adds	r1, r1, r5
	movs	r3, #166
	lsls	r1, r1, #5
	lsls	r3, r3, #7
	ldr	r2, [sp, #24]
	adds	r3, #18
	movs	r0, #12
	add	r1, fp
	adds	r1, r1, r3
	str	r0, [sp, #0]
	mov	r3, sl
	movs	r0, #24
	str	r0, [sp, #4]
	subs	r2, #6
	subs	r3, #12
	mov	r0, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x0072
	movs	r3, #8
	subs	r7, r3, r2
	movs	r4, #1
	lsrs	r6, r7, #1
	mov	r1, sl
	mov	r8, r4
	adds	r4, r1, r6
	cmp	r4, #112
	bgt.n	.L_0819272a
	subs	r2, r7, #1
	lsls	r3, r7, #1
	str	r2, [sp, #8]
	str	r3, [sp, #12]
.L_081926ea:
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #72]
	lsls	r3, r1, #1
	ldrh	r1, [r2, r3]
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsls	r3, r3, #7
	adds	r3, r3, r5
	lsls	r3, r3, #1
	adds	r1, r1, r3
	ldr	r3, [sp, #24]
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, fp
	adds	r1, r1, r2
	subs	r2, r3, r6
	ldr	r3, [sp, #12]
	str	r7, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, r9
	adds	r3, r4, #0
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	add	r8, r1
	mov	r3, r8
	muls	r3, r6
	mov	r2, sl
	adds	r4, r2, r3
	cmp	r4, #112
	ble.n	.L_081926ea
.L_0819272a:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x7410
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	sub	sp, #52
	mov	sl, r0
	movs	r0, #48
	str	r3, [sp, #4]
	mov	r8, r1
	mov	fp, r2
	bl	sub_08014dac
	adds	r7, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r3, [pc, #168]
	add	r5, sp, #28
	adds	r2, r5, #0
	mov	r9, r0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldr	r2, [pc, #156]
	ldr	r3, [sp, #8]
	movs	r0, #4
	ands	r3, r2
	ldr	r2, [pc, #152]
	orrs	r3, r0
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #3
	ldr	r1, [sp, #4]
	orrs	r3, r2
	movs	r2, #160
	lsls	r2, r2, #4
	mov	r4, sl
	adds	r2, #2
	add	r6, sp, #8
	str	r3, [sp, #8]
	mov	r0, sl
	negs	r3, r4
	adds	r2, r1, r2
	mov	r1, fp
	str	r2, [r6, #4]
	strb	r3, [r5, #0]
	strb	r4, [r5, #4]
	strb	r3, [r5, #8]
	strb	r0, [r5, #12]
	strb	r3, [r5, #16]
	strb	r0, [r5, #20]
	strb	r1, [r5, #21]
	strb	r1, [r5, #17]
	bl	sub_08014de4
	mov	r2, r8
	ldr	r3, [pc, #100]
	lsls	r2, r2, #16
	mov	r8, r2
	mov	r1, fp
	add	r8, r3
	movs	r3, #63
	subs	r3, r3, r1
	add	r0, sp, #16
	mov	r4, r8
	lsls	r3, r3, #16
	movs	r2, #0
	str	r4, [r0, #0]
	str	r3, [r0, #4]
	str	r2, [r0, #8]
	bl	sub_08015128
	mov	r4, r9
	movs	r3, #4
	str	r3, [r4, #0]
	ldr	r3, [pc, #64]
	str	r6, [r4, #16]
	str	r3, [r4, #8]
	str	r7, [r4, #12]
	adds	r1, r7, #0
	movs	r2, #6
	adds	r0, r5, #0
	bl	sub_08196958
	mov	r0, r9
	bl	sub_08196a7c
	mov	r0, r9
	bl	sub_08013164
	adds	r0, r7, #0
	bl	sub_08013164
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08196f30
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0xffc10000
	.2byte 0x9f4c
	.2byte 0x0819
	push	{lr}
	asrs	r2, r2, #1
	lsls	r3, r3, #24
	sub	sp, #4
	lsrs	r4, r3, #24
	adds	r0, r0, r2
	subs	r1, r1, r2
	cmp	r2, #48
	ble.n	.L_0819283c
	movs	r2, #48
.L_0819283c:
	adds	r3, r1, r2
	subs	r2, r0, r2
	str	r4, [sp, #0]
	bl	sub_08143eb4
	add	sp, #4
	pop	{pc}
	.align 2, 0
