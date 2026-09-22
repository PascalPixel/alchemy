.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143b20, 0x08143b20
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1fc, 0x0815e1fc
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815e22c, 0x0815e22c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_0818b474, 0x0818b474
	.set sub_0818b6bc, 0x0818b6bc
	.set sub_0818b700, 0x0818b700
	.set sub_0818b730, 0x0818b730
	.set sub_0818b75a, 0x0818b75a
	.set sub_0818b784, 0x0818b784
	.set sub_0818b7ae, 0x0818b7ae
	.set sub_0818b7d8, 0x0818b7d8
	.set sub_0818bc98, 0x0818bc98
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0818ad40
Overlay_0818ad40:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	str	r0, [sp, #48]
	str	r1, [sp, #44]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	ldr	r0, [r3, #92]
	movs	r5, #128
	str	r1, [sp, #40]
	lsls	r5, r5, #6
	adds	r5, #1
	mov	sl, r0
	adds	r0, r5, #0
	ldr	r6, [r3, #100]
	bl	sub_08143a88
	ldr	r2, [sp, #48]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0818ad7e
	adds	r0, r5, #0
	bl	sub_08143b20
	b.n	.L_0818ad84
.L_0818ad7e:
	adds	r0, r5, #0
	bl	sub_08143a88
.L_0818ad84:
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [sp, #44]
	cmp	r3, #3
	bne.n	.L_0818adb4
	ldr	r4, [sp, #48]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0818ada6
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	b.n	.L_0818adce
.L_0818ada6:
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
	b.n	.L_0818adce
	.2byte 0x0210
	.2byte 0x0000
.L_0818adb4:
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0818adc6
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_0818adce
.L_0818adc6:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_0818adce:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #0
	str	r3, [sp, #32]
	str	r1, [sp, #28]
	str	r1, [sp, #24]
	str	r1, [sp, #20]
	movs	r2, #0
	ldr	r0, [pc, #212]
	adds	r1, r6, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #44]
	cmp	r2, #0
	bne.n	.L_0818ae10
	ldr	r0, [pc, #200]
	ldr	r1, [pc, #204]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	movs	r3, #0
	ldr	r0, [pc, #192]
	add	r1, sl
	movs	r2, #0
	bl	sub_08157cf4
	movs	r3, #96
	b.n	.L_0818af02
.L_0818ae10:
	ldr	r4, [sp, #44]
	cmp	r4, #1
	bne.n	.L_0818ae38
	ldr	r0, [pc, #176]
	ldr	r1, [pc, #164]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #156]
	add	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r6, #80
	str	r6, [sp, #16]
	b.n	.L_0818af04
.L_0818ae38:
	ldr	r0, [sp, #44]
	cmp	r0, #3
	bne.n	.L_0818aed8
	ldr	r0, [pc, #140]
	ldr	r1, [pc, #124]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #128]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	movs	r5, #178
	bl	sub_08157cf4
	lsls	r5, r5, #6
	mov	r2, sl
	adds	r1, r2, r5
	movs	r3, #0
	ldr	r0, [pc, #108]
	movs	r2, #0
	movs	r6, #174
	bl	sub_08157cf4
	lsls	r6, r6, #2
	movs	r3, #1
	mov	r8, r3
	mov	ip, r6
	movs	r7, #6
	mov	lr, r5
.L_0818ae7a:
	mov	r1, sl
	adds	r3, r6, r5
	movs	r0, #0
	adds	r4, r7, #0
	add	r1, lr
	add	r3, sl
.L_0818ae86:
	ldrb	r2, [r1, #0]
	adds	r1, #1
	cmp	r2, #0
	ble.n	.L_0818ae96
	subs	r2, r2, r4
	cmp	r2, #0
	bgt.n	.L_0818ae96
	movs	r2, #1
.L_0818ae96:
	adds	r0, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	cmp	r0, ip
	bne.n	.L_0818ae86
	movs	r0, #1
	movs	r4, #174
	add	r8, r0
	lsls	r4, r4, #2
	mov	r1, r8
	adds	r6, r6, r4
	adds	r7, #6
	cmp	r1, #8
	bne.n	.L_0818ae7a
	movs	r2, #116
	str	r2, [sp, #16]
	b.n	.L_0818af04
	.4byte 0x00000134
	.4byte 0x000000e3
	.4byte 0x0201603e
	.4byte 0x000000cf
	.4byte 0x000000e2
	.4byte 0x000000e5
	.4byte 0x0000015f
	.2byte 0x0160
	.2byte 0x0000
.L_0818aed8:
	ldr	r0, [pc, #128]
	ldr	r1, [pc, #132]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #120]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [pc, #112]
	ldr	r1, [pc, #112]
	movs	r2, #1
	bl	sub_08157cf4
	movs	r3, #88
.L_0818af02:
	str	r3, [sp, #16]
.L_0818af04:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r5, #200
	add	r2, sl
	movs	r3, #0
	lsls	r5, r5, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #80]
	adds	r1, r5, #0
	bl	sub_080145a8
	ldr	r4, [sp, #44]
	cmp	r4, #1
	bne.n	.L_0818af34
	ldr	r0, [pc, #68]
	adds	r1, r5, #0
	bl	sub_080145a8
.L_0818af34:
	movs	r2, #128
	ldr	r3, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [sp, #16]
	movs	r6, #0
	mov	fp, r6
	cmp	r0, #0
	bne.n	.L_0818af4c
	bl	.L_0818b80a
.L_0818af4c:
	movs	r1, #48
	subs	r0, #8
	negs	r1, r1
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	b.n	.L_0818af78
	.4byte 0x00000100
	.4byte 0x000000e4
	.4byte 0x0201603e
	.4byte 0x0000016f
	.4byte 0x00000170
	.4byte 0x02010000
	.4byte 0x08143001
	.2byte 0x2475
	.2byte 0x0815
.L_0818af78:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_0818afe4
	movs	r0, #190
	bl	sub_081c0010
	ldr	r3, [sp, #48]
	add	r6, sp, #72
	ldr	r0, [r3, #8]
	adds	r1, r6, #0
	bl	sub_0815e1fc
	add	r5, sp, #60
	adds	r0, r5, #0
	bl	sub_0815e22c
	ldr	r4, [sp, #44]
	cmp	r4, #2
	bne.n	.L_0818afb2
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0818afac
	ldr	r3, [r5, #0]
	adds	r3, #32
	b.n	.L_0818afb0
.L_0818afac:
	ldr	r3, [r5, #0]
	subs	r3, #32
.L_0818afb0:
	str	r3, [r5, #0]
.L_0818afb2:
	ldr	r3, [r6, #0]
	movs	r2, #56
	movs	r1, #128
	subs	r3, r2, r3
	lsls	r1, r1, #19
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	movs	r1, #0
	ldr	r3, [r6, #0]
	str	r1, [sp, #28]
	subs	r2, r2, r3
	str	r2, [sp, #24]
	mov	r3, sl
	movs	r2, #1
	mov	r8, r1
	negs	r2, r2
	adds	r3, #24
.L_0818afd6:
	movs	r4, #1
	add	r8, r4
	mov	r6, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r6, #64
	bne.n	.L_0818afd6
.L_0818afe4:
	ldr	r0, [sp, #12]
	cmp	fp, r0
	blt.n	.L_0818b008
	ldr	r3, [sp, #16]
	mov	r1, fp
	subs	r2, r1, r3
	adds	r2, #8
	ldr	r3, [pc, #40]
	adds	r1, r2, #0
	movs	r0, #128
	adds	r1, #8
	lsls	r2, r2, #1
	lsls	r0, r0, #19
	lsls	r1, r1, #8
	subs	r3, r3, r2
	adds	r0, #82
	orrs	r1, r3
	strh	r1, [r0, #0]
.L_0818b008:
	mov	r4, fp
	cmp	r4, #48
	bne.n	.L_0818b020
	ldr	r6, [sp, #44]
	cmp	r6, #1
	bne.n	.L_0818b020
	movs	r0, #138
	bl	sub_081c0010
	b.n	.L_0818b020
	.2byte 0x0010
	.2byte 0x0000
.L_0818b020:
	mov	r0, fp
	cmp	r0, #47
	bgt.n	.L_0818b07a
	add	r6, sp, #72
	ldr	r2, [r6, #0]
	ldr	r3, [sp, #60]
	movs	r1, #48
	subs	r3, r3, r2
	mov	r0, fp
	muls	r0, r3
	adds	r5, r2, #0
	bl	sub_08002054
	subs	r5, #64
	adds	r3, r5, r0
	cmp	r3, #0
	bge.n	.L_0818b048
	str	r3, [sp, #20]
	movs	r3, #0
	b.n	.L_0818b052
.L_0818b048:
	cmp	r3, #112
	ble.n	.L_0818b052
	subs	r3, #112
	str	r3, [sp, #20]
	movs	r3, #112
.L_0818b052:
	movs	r2, #128
	lsls	r3, r3, #8
	lsls	r2, r2, #19
	negs	r3, r3
	adds	r2, #40
	str	r3, [r2, #0]
	str	r3, [sp, #24]
	movs	r2, #128
	ldr	r3, [r6, #4]
	lsls	r2, r2, #3
	subs	r3, #1
	str	r3, [r6, #4]
	ldr	r1, [sp, #28]
	movs	r3, #128
	adds	r1, r1, r2
	lsls	r3, r3, #8
	str	r1, [sp, #28]
	cmp	r1, r3
	ble.n	.L_0818b07a
	str	r3, [sp, #28]
.L_0818b07a:
	ldr	r4, [sp, #44]
	cmp	r4, #0
	beq.n	.L_0818b150
	cmp	r4, #1
	bne.n	.L_0818b0b4
	movs	r6, #225
	lsls	r6, r6, #7
	movs	r0, #0
	mov	r1, fp
	add	r6, sl
	mov	r8, r0
	lsls	r5, r1, #12
.L_0818b092:
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r2, [sp, #24]
	lsls	r0, r0, #2
	asrs	r0, r0, #10
	movs	r4, #1
	subs	r0, r2, r0
	movs	r3, #128
	add	r8, r4
	stmia	r6!, {r0}
	lsls	r3, r3, #5
	mov	r0, r8
	adds	r5, r5, r3
	cmp	r0, #160
	bne.n	.L_0818b092
	b.n	.L_0818b14a
.L_0818b0b4:
	ldr	r3, [sp, #44]
	ldr	r1, [pc, #332]
	subs	r3, #2
	mov	r9, r1
	cmp	r3, #1
	bhi.n	.L_0818b0ce
	mov	r3, fp
	subs	r3, #40
	cmp	r3, #7
	bhi.n	.L_0818b0ce
	ldr	r0, [pc, #316]
	bl	sub_0815f0a0
.L_0818b0ce:
	mov	r2, fp
	cmp	r2, #51
	ble.n	.L_0818b0e8
	ldr	r3, [sp, #44]
	cmp	r3, #2
	bne.n	.L_0818b0e2
	ldr	r0, [pc, #304]
	bl	sub_0815f0a0
	b.n	.L_0818b0e8
.L_0818b0e2:
	ldr	r0, [pc, #300]
	bl	sub_0815f0a0
.L_0818b0e8:
	mov	r4, fp
	cmp	r4, #48
	bne.n	.L_0818b106
	movs	r6, #128
	lsls	r6, r6, #1
	movs	r7, #15
	mov	r5, r9
	add	r6, r9
.L_0818b0f8:
	bl	sub_08014878
	ands	r0, r7
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_0818b0f8
.L_0818b106:
	ldr	r6, [sp, #8]
	cmp	r6, #15
	bhi.n	.L_0818b14a
	ldr	r7, [pc, #260]
	movs	r0, #0
	movs	r1, #7
	mov	r8, r0
	mov	lr, r1
	mov	ip, r6
	movs	r5, #0
	movs	r4, #0
.L_0818b11c:
	mov	r2, lr
	mov	r3, r8
	ands	r3, r2
	lsls	r3, r3, #5
	mov	r6, r9
	movs	r0, #0
	adds	r1, r4, r7
	adds	r2, r3, r6
.L_0818b12c:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r3, ip
	bne.n	.L_0818b136
	strb	r5, [r1, #0]
.L_0818b136:
	adds	r0, #1
	adds	r1, #1
	cmp	r0, #32
	bne.n	.L_0818b12c
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r4, #32
	cmp	r1, #64
	bne.n	.L_0818b11c
.L_0818b14a:
	ldr	r2, [sp, #44]
	cmp	r2, #0
	bne.n	.L_0818b154
.L_0818b150:
	movs	r3, #120
	b.n	.L_0818b15e
.L_0818b154:
	ldr	r4, [sp, #44]
	movs	r3, #72
	cmp	r4, #3
	beq.n	.L_0818b15e
	movs	r3, #80
.L_0818b15e:
	cmp	fp, r3
	blt.n	.L_0818b164
	b.n	.L_0818b27a
.L_0818b164:
	movs	r0, #32
	bl	sub_08014dac
	adds	r6, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #164]
	ldr	r3, [sp, #52]
	adds	r5, r0, #0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #156]
	str	r6, [r5, #12]
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #52]
	ldr	r3, [pc, #132]
	add	r2, sp, #52
	str	r3, [r2, #4]
	movs	r3, #8
	str	r3, [r5, #0]
	ldr	r3, [pc, #136]
	str	r2, [r5, #16]
	str	r3, [r5, #8]
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_080151e4
	ldr	r1, [sp, #20]
	ldr	r2, [pc, #116]
	lsls	r0, r1, #16
	ldr	r1, [sp, #76]
	lsls	r1, r1, #16
	adds	r1, r1, r2
	movs	r2, #0
	bl	sub_08015160
	mov	r3, fp
	cmp	r3, #47
	bgt.n	.L_0818b1f4
	ldr	r4, [sp, #48]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0818b1da
	ldr	r2, [pc, #88]
	mov	r1, fp
	lsls	r0, r1, #10
	adds	r0, r0, r2
	bl	sub_080150e4
	b.n	.L_0818b1e6
.L_0818b1da:
	movs	r0, #52
	mov	r3, fp
	subs	r0, r0, r3
	lsls	r0, r0, #10
	bl	sub_080150e4
.L_0818b1e6:
	movs	r0, #52
	mov	r4, fp
	subs	r0, r0, r4
	lsls	r0, r0, #10
	bl	sub_08015024
	b.n	.L_0818b240
.L_0818b1f4:
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0818b230
	ldr	r0, [pc, #44]
	bl	sub_080150e4
	b.n	.L_0818b238
	.4byte 0x0201683e
	.4byte 0x00000161
	.4byte 0x00000170
	.4byte 0x0000015f
	.4byte 0x0201603e
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x081992b0
	.4byte 0xffc00000
	.4byte 0xffff3000
	.2byte 0xf000
	.2byte 0xffff
.L_0818b230:
	.2byte 0x2080
	lsls	r0, r0, #5
	bl	sub_080150e4
.L_0818b238:
	movs	r0, #128
	lsls	r0, r0, #5
	bl	sub_08015024
.L_0818b240:
	movs	r0, #184
	lsls	r0, r0, #5
	adds	r0, #112
	bl	sub_08015068
	ldr	r0, [sp, #28]
	bl	sub_0801521c
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #10
	bl	sub_080151e4
	adds	r1, r6, #0
	movs	r2, #4
	ldr	r0, [pc, #104]
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	bl	sub_08013164
	adds	r0, r6, #0
	bl	sub_08013164
.L_0818b27a:
	ldr	r1, [sp, #44]
	cmp	r1, #1
	bne.n	.L_0818b372
	mov	r2, fp
	cmp	r2, #48
	bne.n	.L_0818b2ac
	movs	r0, #136
	bl	sub_081180e8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
.L_0818b2ac:
	ldr	r3, [sp, #8]
	cmp	r3, #31
	bhi.n	.L_0818b304
	lsls	r2, r3, #1
	lsls	r3, r3, #4
	subs	r3, r3, r2
	movs	r4, #0
	lsls	r3, r3, #2
	mov	r0, sl
	mov	r8, r4
	movs	r6, #127
	adds	r5, r3, r0
	b.n	.L_0818b2d0
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x91e0
	.2byte 0x0819
.L_0818b2d0:
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r6
	adds	r0, #64
	add	r8, r1
	lsls	r0, r0, #11
	movs	r3, #0
	mov	r2, r8
	str	r0, [r5, #16]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #2
	bne.n	.L_0818b2d0
.L_0818b304:
	movs	r3, #0
	mov	r8, r3
	mov	r5, sl
.L_0818b30a:
	ldr	r1, [r5, #24]
	cmp	r1, #15
	bhi.n	.L_0818b366
	cmp	r1, #0
	bge.n	.L_0818b316
	adds	r1, #3
.L_0818b316:
	asrs	r1, r1, #2
	lsls	r1, r1, #10
	movs	r4, #224
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	lsls	r4, r4, #3
	ldr	r0, [sp, #20]
	add	r1, sl
	adds	r1, r1, r4
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	adds	r2, r0, r2
	movs	r0, #32
	subs	r3, #16
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #40]
	ldr	r6, [sp, #32]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4a86
	adds	r0, r5, #0
	movs	r1, #62
	bl	sub_08138086
	ldr	r3, [r5, #4]
	movs	r2, #192
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_0818b360
	movs	r3, #0
	str	r3, [r5, #16]
	ldr	r3, [r5, #12]
	str	r2, [r5, #4]
	lsls	r3, r3, #1
	str	r3, [r5, #12]
.L_0818b360:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0818b366:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_0818b30a
.L_0818b372:
	ldr	r2, [sp, #44]
	cmp	r2, #0
	bne.n	.L_0818b426
	mov	r3, fp
	cmp	r3, #48
	bne.n	.L_0818b384
	movs	r0, #212
	bl	sub_081c0010
.L_0818b384:
	mov	r4, fp
	cmp	r4, #64
	bne.n	.L_0818b392
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
.L_0818b392:
	ldr	r6, [sp, #8]
	cmp	r6, #31
	bhi.n	.L_0818b3cc
	ldr	r0, [sp, #8]
	movs	r3, #128
	lsls	r5, r0, #3
	subs	r5, r5, r0
	lsls	r5, r5, #2
	add	r5, sl
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r6, #127
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	ldr	r1, [sp, #44]
	str	r1, [r5, #24]
.L_0818b3cc:
	movs	r2, #0
	mov	r8, r2
	mov	r5, sl
.L_0818b3d2:
	ldr	r1, [r5, #24]
	cmp	r1, #63
	bhi.n	.L_0818b41a
	cmp	r1, #0
	bge.n	.L_0818b3de
	adds	r1, #3
.L_0818b3de:
	movs	r3, #3
	asrs	r1, r1, #2
	ands	r1, r3
	ldr	r3, [pc, #376]
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	ldr	r6, [sp, #20]
	lsls	r1, r1, #10
	adds	r1, r1, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	adds	r2, r6, r2
	movs	r0, #32
	subs	r3, #16
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0818b41a:
	movs	r6, #1
	add	r8, r6
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_0818b3d2
.L_0818b426:
	ldr	r1, [sp, #44]
	cmp	r1, #3
	beq.n	.L_0818b42e
	b.n	.L_0818b5f2
.L_0818b42e:
	mov	r2, fp
	cmp	r2, #47
	bgt.n	.L_0818b436
	b.n	.L_0818b5f2
.L_0818b436:
	ldr	r3, [sp, #8]
	mov	r9, r3
	cmp	r3, #23
	bgt.n	sub_0818b474
	adds	r0, r3, #0
	cmp	r3, #0
	bge.n	.L_0818b448
	mov	r0, fp
	subs	r0, #45
.L_0818b448:
	movs	r1, #6
	asrs	r0, r0, #2
	bl	sub_08002064
	lsls	r1, r0, #4
	subs	r1, r1, r0
	ldr	r2, [sp, #20]
	movs	r3, #24
	lsls	r1, r1, #6
	movs	r4, #224
	add	r1, sl
	lsls	r4, r4, #3
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	adds	r1, r1, r4
	adds	r2, #52
	ldr	r0, [sp, #40]
	movs	r3, #44
	ldr	r6, [sp, #32]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4648
	cmp	r0, #24
	bne.n	.L_0818b536
	movs	r5, #1
	movs	r0, #212
	negs	r5, r5
	bl	sub_081c0010
	adds	r0, r5, #0
	bl	sub_081180e8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #200]
	ldr	r0, [sp, #40]
	ldr	r2, [pc, #200]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0c
	movs	r3, #16
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	adds	r2, r5, #0
	bl	sub_0814cd48
	movs	r3, #0
	mov	r8, r3
	mov	r7, sl
.L_0818b4be:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	mov	r3, r8
	adds	r3, #48
	lsls	r3, r3, #16
	movs	r5, #127
	str	r3, [r7, #4]
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r5, #64
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r4, #1
	movs	r3, #255
	add	r8, r4
	ands	r3, r0
	mov	r6, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r6, #64
	bne.n	.L_0818b4be
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [pc, #40]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
.L_0818b536:
	mov	r3, r9
	subs	r3, #24
	cmp	r3, #36
	bhi.n	.L_0818b5f2
	mov	r0, r9
	movs	r1, #0
	cmp	r0, #28
	ble.n	.L_0818b56c
	cmp	r3, #0
	bge.n	.L_0818b54c
	adds	r3, #3
.L_0818b54c:
	asrs	r1, r3, #2
	cmp	r1, #7
	ble.n	.L_0818b56c
	movs	r1, #7
	b.n	.L_0818b56c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0xfffff000
	.4byte 0x0201683e
	.4byte 0x03000260
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_0818b56c:
	movs	r3, #174
	lsls	r3, r3, #2
	adds	r7, r1, #0
	muls	r7, r3
	movs	r2, #0
	mov	r8, r2
	mov	r6, sl
.L_0818b57a:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0818b582
	adds	r3, #3
.L_0818b582:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	r4, r8
	subs	r3, r4, r3
	lsls	r2, r3, #1
	adds	r5, r2, r3
	ldr	r3, [r6, #24]
	mov	r1, r9
	adds	r0, r3, r1
	cmp	r0, #0
	bge.n	.L_0818b59a
	adds	r0, #7
.L_0818b59a:
	movs	r1, #3
	asrs	r0, r0, #3
	bl	sub_08002064
	ldr	r2, [pc, #656]
	adds	r0, r5, r0
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	movs	r2, #178
	adds	r1, r7, r1
	lsls	r2, r2, #6
	add	r1, sl
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #636]
	ldr	r4, [sp, #20]
	ldrb	r5, [r3, r0]
	adds	r2, r4, r2
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r6, r4]
	ldr	r4, [pc, #624]
	ldrb	r4, [r4, r0]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #32
	bne.n	.L_0818b57a
.L_0818b5f2:
	ldr	r2, [sp, #44]
	cmp	r2, #2
	beq.n	.L_0818b5fa
	b.n	sub_0818b7d8
.L_0818b5fa:
	mov	r3, fp
	cmp	r3, #60
	bne.n	.L_0818b606
	movs	r0, #212
	bl	sub_081c0010
.L_0818b606:
	mov	r4, fp
	cmp	r4, #64
	bne.n	.L_0818b654
	movs	r0, #144
	bl	sub_081c0010
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
	ldr	r1, [sp, #48]
	movs	r3, #110
	movs	r6, #36
	ldrsh	r0, [r1, r6]
	movs	r2, #128
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r2, r2, #10
	movs	r1, #1
	lsls	r3, r3, #12
	str	r2, [sp, #0]
	bl	sub_0815f000
	ldr	r3, [sp, #48]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #4
	str	r3, [r2, #0]
.L_0818b654:
	mov	r4, fp
	cmp	r4, #51
	bgt.n	.L_0818b65c
	b.n	sub_0818b7d8
.L_0818b65c:
	cmp	r4, #63
	bgt.n	.L_0818b68c
	ldr	r6, [sp, #48]
	ldr	r1, [pc, #476]
	ldr	r2, [r6, #4]
	ldr	r0, [sp, #20]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldrb	r2, [r1, r3]
	ldr	r3, [pc, #468]
	movs	r1, #57
	ldrb	r3, [r3, #0]
	str	r1, [sp, #0]
	movs	r1, #98
	str	r1, [sp, #4]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r2, r0, r2
	add	r1, sl
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe0a5
.L_0818b68c:
	mov	r6, fp
	cmp	r6, #67
	bgt.n	sub_0818b6bc
	ldr	r0, [sp, #48]
	ldr	r1, [pc, #424]
	ldr	r2, [r0, #4]
	ldr	r4, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldrb	r2, [r1, r3]
	ldr	r1, [sp, #20]
	ldr	r3, [pc, #416]
	adds	r2, r1, r2
	movs	r1, #57
	ldrb	r3, [r3, #0]
	str	r1, [sp, #0]
	movs	r1, #98
	str	r1, [sp, #4]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [sp, #40]
	add	r1, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9e0c
	ldr	r7, [pc, #384]
	ldr	r2, [r6, #4]
	ldr	r6, [pc, #384]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #1
	movs	r1, #99
	ldrb	r2, [r7, r3]
	ldrb	r3, [r6, #1]
	str	r1, [sp, #0]
	movs	r1, #69
	ldr	r0, [sp, #20]
	str	r1, [sp, #4]
	movs	r1, #224
	lsls	r1, r1, #5
	adds	r1, #210
	adds	r2, r0, r2
	ldr	r5, [sp, #32]
	ldr	r0, [sp, #40]
	add	r1, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x465b
	subs	r3, #64
	cmp	r3, #1
	bhi.n	sub_0818b700
	movs	r1, #128
	ldr	r3, [pc, #336]
	ldr	r0, [sp, #40]
	lsls	r1, r1, #7
	ldr	r2, [pc, #336]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465b
	subs	r3, #66
	cmp	r3, #1
	bhi.n	sub_0818b730
	ldr	r1, [sp, #48]
	ldr	r0, [sp, #40]
	ldr	r2, [r1, #4]
	movs	r1, #128
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #2
	ldrb	r2, [r7, r3]
	ldr	r3, [sp, #20]
	adds	r2, r3, r2
	ldrb	r3, [r6, #2]
	str	r1, [sp, #0]
	movs	r1, #91
	str	r1, [sp, #4]
	movs	r1, #220
	lsls	r1, r1, #6
	adds	r1, #129
	add	r1, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x465b
	subs	r3, #68
	cmp	r3, #1
	bhi.n	sub_0818b75a
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #20]
	ldr	r2, [r4, #4]
	movs	r1, #128
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #3
	ldrb	r2, [r7, r3]
	ldrb	r3, [r6, #3]
	str	r1, [sp, #0]
	movs	r1, #91
	adds	r2, r0, r2
	str	r1, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #248]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x465b
	subs	r3, #70
	cmp	r3, #1
	bhi.n	sub_0818b784
	ldr	r1, [sp, #48]
	ldr	r0, [sp, #40]
	ldr	r2, [r1, #4]
	movs	r1, #128
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #4
	ldrb	r2, [r7, r3]
	ldr	r3, [sp, #20]
	adds	r2, r3, r2
	ldrb	r3, [r6, #4]
	str	r1, [sp, #0]
	movs	r1, #59
	str	r1, [sp, #4]
	ldr	r1, [pc, #212]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x465b
	subs	r3, #72
	cmp	r3, #1
	bhi.n	sub_0818b7ae
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #20]
	ldr	r2, [r4, #4]
	movs	r1, #122
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #5
	ldrb	r2, [r7, r3]
	ldrb	r3, [r6, #5]
	str	r1, [sp, #0]
	movs	r1, #29
	adds	r2, r0, r2
	str	r1, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #172]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x465b
	subs	r3, #74
	cmp	r3, #1
	bhi.n	sub_0818b7d8
	ldr	r1, [sp, #48]
	ldr	r0, [sp, #40]
	ldr	r2, [r1, #4]
	movs	r1, #76
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #6
	ldrb	r2, [r7, r3]
	ldr	r3, [sp, #20]
	adds	r2, r3, r2
	ldrb	r3, [r6, #6]
	str	r1, [sp, #0]
	movs	r1, #25
	str	r1, [sp, #4]
	ldr	r1, [pc, #136]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2004
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #16]
	movs	r6, #1
	adds	r4, #1
	add	fp, r6
	str	r4, [sp, #8]
	cmp	fp, r0
	beq.n	.L_0818b80a
	bl	.L_0818af78
.L_0818b80a:
	ldr	r1, [sp, #44]
	cmp	r1, #1
	bne.n	.L_0818b816
	ldr	r0, [pc, #76]
	bl	sub_08014644
.L_0818b816:
	ldr	r0, [pc, #76]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081974dc
	.4byte 0x081974f4
	.4byte 0x08197500
	.4byte 0x0819750c
	.4byte 0x0819751a
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02010000
	.4byte 0x02012d80
	.4byte 0x02014b00
	.4byte 0x020158d2
	.4byte 0x08152475
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	str	r0, [sp, #60]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	str	r0, [sp, #56]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #52]
	ldr	r2, [r5, #100]
	str	r2, [sp, #36]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [sp, #56]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r3, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #56]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	ldr	r1, [sp, #60]
	movs	r3, #80
	ldr	r0, [r1, #8]
	movs	r2, #36
	ldrsh	r1, [r1, r2]
	movs	r2, #8
	b.n	.L_0818b8d4
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_0818b8d4:
	bl	sub_08157530
	movs	r0, #8
	bl	sub_08013560
	movs	r1, #23
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	ldr	r0, [pc, #820]
	ldr	r1, [pc, #824]
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #40]
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #36]
	movs	r3, #0
	ldr	r0, [pc, #808]
	bl	sub_08157cf4
	ldr	r0, [pc, #808]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #800]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4668
	mov	r1, sp
	movs	r3, #0
	adds	r0, #72
	adds	r1, #64
	str	r3, [sp, #48]
	str	r0, [sp, #20]
	str	r1, [sp, #24]
.L_0818b926:
	ldr	r2, [sp, #48]
	cmp	r2, #0
	bne.n	.L_0818b946
	movs	r3, #0
	mov	r9, r3
	ldr	r3, [pc, #768]
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #2
.L_0818b93a:
	movs	r0, #1
	add	r9, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_0818b93a
.L_0818b946:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #32]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [sp, #60]
	str	r0, [sp, #28]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	ldr	r1, [sp, #20]
	bl	sub_0815e21c
	ldr	r2, [pc, #724]
	ldr	r3, [sp, #64]
	movs	r0, #7
	ands	r3, r2
	ldr	r2, [pc, #720]
	orrs	r3, r0
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r1, [pc, #684]
	ldr	r2, [sp, #24]
	str	r3, [sp, #64]
	str	r1, [r2, #4]
	ldr	r0, [sp, #28]
	ldr	r3, [pc, #700]
	movs	r1, #7
	str	r3, [r0, #8]
	str	r1, [r0, #0]
	str	r2, [r0, #16]
	ldr	r2, [sp, #32]
	movs	r3, #0
	str	r2, [r0, #12]
	ldr	r0, [sp, #56]
	movs	r1, #120
	str	r1, [sp, #16]
	str	r3, [sp, #12]
	mov	r9, r3
	mov	sl, r0
.L_0818b99c:
	ldr	r2, [pc, #676]
	mov	r0, r9
	ldrb	r3, [r2, r0]
	ldr	r1, [sp, #48]
	cmp	r1, r3
	bne.n	.L_0818ba8c
	ldr	r3, [sp, #20]
	ldr	r0, [sp, #12]
	ldr	r1, [pc, #664]
	movs	r2, #0
	mov	fp, r2
	mov	r8, r3
	adds	r7, r0, r1
.L_0818b9b6:
	mov	r2, r8
	ldr	r3, [r2, #0]
	mov	r0, r8
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r5, #255
	ldr	r3, [r0, #4]
	subs	r3, #24
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0818ba0a
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #580]
	adds	r3, r3, r2
	b.n	.L_0818ba12
.L_0818ba0a:
	ldr	r3, [r7, #12]
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r3, r3, r0
.L_0818ba12:
	str	r3, [r7, #12]
	mov	r1, r8
	ldr	r3, [r1, #0]
	mov	r2, sl
	str	r3, [r2, #0]
	ldr	r3, [r1, #4]
	subs	r3, #24
	str	r3, [r2, #4]
	bl	sub_08014878
	movs	r3, #7
	ands	r0, r3
	adds	r0, #16
	str	r0, [r7, #24]
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	adds	r7, #28
	cmp	r1, #32
	bne.n	.L_0818b9b6
	mov	r2, r9
	cmp	r2, #2
	bne.n	.L_0818ba48
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_0818ba4e
.L_0818ba48:
	movs	r0, #134
	bl	sub_081c0010
.L_0818ba4e:
	ldr	r1, [sp, #60]
	ldr	r2, [sp, #16]
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	movs	r3, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #12
	movs	r1, #1
	lsls	r2, r2, #10
	bl	sub_0815f000
	ldr	r1, [sp, #60]
	movs	r5, #8
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	movs	r2, #5
	movs	r3, #0
	movs	r1, #7
	str	r5, [sp, #0]
	bl	sub_0814cd48
	movs	r0, #238
	ldr	r2, [sp, #56]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r3, r2, r0
	str	r5, [r3, #0]
	ldr	r2, [pc, #440]
.L_0818ba8c:
	mov	r1, r9
	ldrb	r2, [r2, r1]
	ldr	r3, [sp, #48]
	cmp	r3, r2
	blt.n	.L_0818bb18
	ldr	r0, [sp, #48]
	adds	r3, r2, #0
	adds	r3, #16
	cmp	r0, r3
	bge.n	.L_0818bb18
	subs	r1, r0, r2
	lsls	r2, r1, #3
	movs	r3, #64
	subs	r2, r3, r2
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r3, #208
	muls	r3, r1
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r5, r3, r1
	cmp	r2, #0
	ble.n	.L_0818babc
	movs	r2, #0
.L_0818babc:
	movs	r3, #64
	negs	r3, r3
	cmp	r2, r3
	ble.n	.L_0818bb18
	ldr	r0, [sp, #28]
	str	r2, [r0, #20]
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	mov	r1, sl
	ldr	r0, [r1, #0]
	ldr	r1, [r1, #4]
	subs	r0, #128
	subs	r1, #64
	lsls	r1, r1, #16
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	mov	r2, r9
	movs	r3, #128
	lsls	r0, r2, #14
	lsls	r3, r3, #6
	adds	r0, r0, r3
	bl	sub_080150e4
	ldr	r0, [pc, #336]
	bl	sub_08015024
	adds	r0, r5, #0
	bl	sub_0801521c
	ldr	r0, [pc, #328]
	ldr	r1, [sp, #32]
	movs	r2, #4
	bl	sub_08196958
	ldr	r0, [sp, #28]
	bl	sub_08196a7c
.L_0818bb18:
	ldr	r1, [sp, #16]
	movs	r0, #28
	ldr	r2, [sp, #12]
	add	sl, r0
	movs	r3, #224
	movs	r0, #1
	adds	r1, #20
	lsls	r3, r3, #4
	add	r9, r0
	str	r1, [sp, #16]
	adds	r2, r2, r3
	mov	r1, r9
	str	r2, [sp, #12]
	cmp	r1, #3
	beq.n	.L_0818bb38
	b.n	.L_0818b99c
.L_0818bb38:
	ldr	r3, [pc, #284]
	ldr	r0, [sp, #40]
	ldr	r7, [pc, #264]
	movs	r2, #0
	mov	r9, r2
	mov	fp, r3
	mov	sl, r0
.L_0818bb46:
	ldr	r6, [r7, #24]
	cmp	r6, #0
	blt.n	.L_0818bbbc
	asrs	r6, r6, #2
	adds	r6, #3
	lsls	r1, r6, #1
	mov	r8, r1
	mov	r4, r8
	subs	r4, #2
	mov	r2, fp
	ldrh	r1, [r2, r4]
	ldr	r3, [sp, #36]
	movs	r0, #2
	ldrsh	r2, [r7, r0]
	adds	r1, r3, r1
	lsrs	r5, r6, #31
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	adds	r5, r6, r5
	asrs	r5, r5, #1
	mov	r0, r8
	str	r0, [sp, #4]
	str	r4, [sp, #8]
	str	r6, [sp, #0]
	ldr	r0, [sp, #52]
	subs	r3, r3, r6
	subs	r2, r2, r5
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #62
	movs	r2, #0
	bl	sub_08138086
	ldr	r4, [sp, #8]
	mov	r2, fp
	ldrh	r1, [r2, r4]
	ldr	r3, [sp, #36]
	movs	r0, #2
	ldrsh	r2, [r7, r0]
	adds	r1, r3, r1
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	mov	r0, r8
	subs	r3, r3, r6
	str	r0, [sp, #4]
	subs	r2, r2, r5
	str	r6, [sp, #0]
	ldr	r0, [sp, #52]
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #62
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_0818bbbc:
	movs	r1, #1
	movs	r2, #128
	add	r9, r1
	lsls	r2, r2, #2
	adds	r7, #28
	cmp	r9, r2
	bne.n	.L_0818bb46
	ldr	r0, [sp, #28]
	bl	sub_08013164
	ldr	r0, [sp, #32]
	bl	sub_08013164
	movs	r1, #8
	movs	r0, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r0, #240
	ldr	r3, [sp, #56]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r3, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #48]
	adds	r1, #1
	str	r1, [sp, #48]
	cmp	r1, #50
	beq.n	.L_0818bc02
	b.n	.L_0818b926
.L_0818bc02:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #80]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000000c2
	.4byte 0x02010000
	.4byte 0x00000134
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x02014018
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0x08199d94
	.4byte 0x02014000
	.4byte 0xffff8000
	.4byte 0xfffff000
	.4byte 0x08199210
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_0818bc98
	pop	{pc}
