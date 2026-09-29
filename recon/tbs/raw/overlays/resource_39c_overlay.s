.syntax unified
	.thumb
	.section .text.x02009db4,"ax",%progbits
	.p2align 2
	.global FieldScene_RunPrimarySequence
	.thumb_func
FieldScene_RunPrimarySequence:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r0, #211
	sub	sp, #64
	bl 0x0200dc64
	cmp	r6, #0
	bne.n	.L_02001df4
	movs	r5, #1
	movs	r0, #111
	movs	r1, #57
	movs	r2, #113
	movs	r3, #42
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200da04
	movs	r0, #111
	movs	r1, #59
	movs	r2, #113
	movs	r3, #43
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200da04
	b.n	.L_02001e3c
.L_02001df4:
	cmp	r6, #1
	bne.n	.L_02001e1a
	movs	r0, #113
	movs	r1, #58
	movs	r2, #112
	movs	r3, #46
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200da04
	movs	r0, #115
	movs	r1, #58
	movs	r2, #113
	movs	r3, #46
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200da04
	b.n	.L_02001e3c
.L_02001e1a:
	movs	r5, #1
	movs	r0, #115
	movs	r1, #57
	movs	r2, #116
	movs	r3, #44
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200da04
	movs	r0, #113
	movs	r1, #57
	movs	r2, #115
	movs	r3, #44
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200da04
.L_02001e3c:
	mov	r2, sp
	adds	r2, #24
	movs	r3, #7
	str	r2, [sp, #16]
	str	r3, [r2, #4]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	ldr	r2, [pc, #460]
	mov	sl, r3
	movs	r3, #1
	mov	fp, r2
	mov	r9, r3
.L_02001e5a:
	movs	r2, #0
	mov	r3, sl
	str	r2, [sp, #20]
	lsls	r2, r3, #20
	movs	r3, #203
	lsls	r3, r3, #18
	subs	r3, r3, r2
	mov	r8, r3
	movs	r3, #176
	lsls	r3, r3, #18
	adds	r7, r2, r3
.L_02001e70:
	ldr	r3, [sp, #20]
	mov	r2, r9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001f68
	cmp	r6, #0
	bne.n	.L_02001ec8
	bl 0x0200d994
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r5, r0, #1
	adds	r5, r5, r0
	lsls	r3, r5, #4
	adds	r5, r5, r3
	lsls	r3, r5, #8
	adds	r5, r5, r3
	bl 0x0200d994
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r2, r3, #4
	adds	r3, r3, r2
	lsls	r2, r3, #8
	adds	r3, r3, r2
	add	r3, fp
	str	r3, [sp, #4]
	movs	r3, #144
	lsls	r3, r3, #12
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	add	r5, fp
	movs	r0, #198
	str	r3, [sp, #12]
	lsls	r0, r0, #18
	movs	r1, #0
	adds	r2, r7, #0
	adds	r3, r5, #0
	str	r6, [sp, #0]
	bl 0x0200813c
	b.n	.L_02001f62
.L_02001ec8:
	cmp	r6, #1
	bne.n	.L_02001f1a
	bl 0x0200d994
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r5, r0, #1
	adds	r5, r5, r0
	lsls	r3, r5, #4
	adds	r5, r5, r3
	lsls	r3, r5, #8
	adds	r5, r5, r3
	bl 0x0200d994
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r2, r3, #4
	adds	r3, r3, r2
	lsls	r2, r3, #8
	adds	r3, r3, r2
	add	r3, fp
	str	r3, [sp, #4]
	movs	r3, #144
	lsls	r3, r3, #12
	movs	r2, #192
	lsls	r2, r2, #15
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	add	r5, fp
	adds	r0, r7, r2
	movs	r2, #0
	str	r2, [sp, #0]
	str	r3, [sp, #12]
	movs	r1, #0
	ldr	r2, [pc, #272]
	adds	r3, r5, #0
	bl 0x0200813c
	b.n	.L_02001f62
.L_02001f1a:
	bl 0x0200d994
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r5, r0, #1
	adds	r5, r5, r0
	lsls	r3, r5, #4
	adds	r5, r5, r3
	lsls	r3, r5, #8
	adds	r5, r5, r3
	bl 0x0200d994
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r2, r3, #4
	adds	r3, r3, r2
	lsls	r2, r3, #8
	adds	r3, r3, r2
	add	r3, fp
	movs	r2, #0
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #16]
	movs	r3, #144
	lsls	r3, r3, #12
	add	r5, fp
	str	r3, [sp, #8]
	str	r2, [sp, #12]
	mov	r0, r8
	movs	r1, #0
	ldr	r2, [pc, #204]
	adds	r3, r5, #0
	bl 0x0200813c
.L_02001f62:
	movs	r0, #1
	bl 0x0200daac
.L_02001f68:
	ldr	r3, [pc, #192]
	add	r8, r3
	ldr	r3, [sp, #20]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, #1
	adds	r7, r7, r2
	str	r3, [sp, #20]
	cmp	r3, #7
	bhi.n	.L_02001f7e
	b.n	.L_02001e70
.L_02001f7e:
	cmp	r6, #0
	bne.n	.L_02001fac
	mov	r2, r9
	mov	r3, sl
	adds	r3, #43
.L_02001f88:
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #111
	movs	r1, #58
	movs	r2, #113
	bl 0x0200da04
	mov	r2, r9
	mov	r3, sl
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	adds	r3, #44
	movs	r0, #111
	movs	r1, #59
	movs	r2, #113
	bl 0x0200da04
	b.n	.L_02002002
.L_02001fac:
	cmp	r6, #1
	bne.n	.L_02001fd6
	mov	r2, sl
	adds	r2, #113
	movs	r0, #114
	movs	r1, #58
	movs	r3, #46
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200da04
	mov	r2, sl
	adds	r2, #114
	movs	r0, #115
	movs	r1, #58
	movs	r3, #46
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200da04
	b.n	.L_02002002
.L_02001fd6:
	mov	r3, sl
	movs	r2, #115
	subs	r2, r2, r3
	mov	r3, r9
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #114
	movs	r1, #57
	movs	r3, #44
	bl 0x0200da04
	mov	r3, sl
	movs	r2, #114
	subs	r2, r2, r3
	mov	r3, r9
	str	r3, [sp, #0]
	str	r3, [sp, #4]
.L_02001ff8:
	movs	r0, #113
	movs	r1, #57
	movs	r3, #44
	bl 0x0200da04
.L_02002002:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #1
	bhi.n	.L_0200200e
	b.n	.L_02001e5a
.L_0200200e:
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
.L_02002018:
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xffff3334
	.4byte 0x02ea0000
	.4byte 0x02ca0000
	.2byte 0x0000
	.2byte 0xffff
	.section .text.x0200bd20,"ax",%progbits
	.p2align 2
	.global Func_02003d20
	.thumb_func
Func_02003d20:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r0, [pc, #884]
	sub sp, #8
	bl 0x0200da8c
	ldr r2, [pc, #880]
	ldr r3, [pc, #880]
	adds r1, r2, r3
	movs r3, #11
	strh r3, [r1]
	movs r1, #144
	ldr r3, [pc, #876]
	lsls r1, r1, #2
	adds r2, r2, r1
	strh r3, [r2]
	ldr r3, [pc, #872]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	movs r2, #253
	ldr r3, [pc, #860]
	lsls r2, r2, #6
	strh r2, [r3]
	ldr r2, [pc, #856]
	adds r3, #2
	strh r2, [r3]
	ldr r1, [pc, #856]
	movs r0, #21
	bl 0x0200cfcc
	movs r0, #0
	bl 0x0200dc1c
	ldr r0, [pc, #844]
	bl 0x0200da84
	cmp r0, #0
	beq .L_02003d20_0
	movs r1, #200
	ldr r0, [pc, #836]
	lsls r1, r1, #4
	bl 0x0200d984
	b .L_02003d20_1
.L_02003d20_0:
	bl 0x02008dfc
.L_02003d20_1:
	ldr r1, [pc, #792]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #816]
	cmp r2, r3
	beq .L_02003d20_2
	b .L_02003d20_3
.L_02003d20_2:
	ldr r2, [pc, #772]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #14
	bls .L_02003d20_4
	b .L_02003d20_3
.L_02003d20_4:
	ldr r2, [pc, #792]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	pop {r2, r4, r5, r6, r7, pc}
	.2byte 0x0200
	.2byte 0xbdf4
	.2byte 0x0200
	.2byte 0xbdf4
	.2byte 0x0200
	.2byte 0xbe2a
	.2byte 0x0200
	.2byte 0xbe16
	.2byte 0x0200
	.2byte 0xbe16
	.2byte 0x0200
	.2byte 0xbf56
	.2byte 0x0200
	.2byte 0xbf56
	.2byte 0x0200
	.2byte 0xbf10
	.2byte 0x0200
	.2byte 0xbf10
	.2byte 0x0200
	.2byte 0xbf56
	.2byte 0x0200
	.2byte 0xbf56
	.2byte 0x0200
	.2byte 0xbf56
	.2byte 0x0200
	.2byte 0xbf56
	.2byte 0x0200
	.2byte 0xbe1e
	.2byte 0x0200
	.2byte 0x48b1
	.2byte 0xf001
	.2byte 0xfe45
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe0aa
	.2byte 0x2302
	.2byte 0x2205
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2042
	.2byte 0x2105
	.2byte 0x221b
	.2byte 0x2317
	.2byte 0xf001
	.2byte 0xfe04
	.2byte 0xe09f
	.2byte 0x20aa
	.2byte 0xf001
	.2byte 0xff1c
	.2byte 0xe09b
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfe58
	.2byte 0x2101
	.2byte 0xf7fc
	.2byte 0xf903
	.2byte 0x48a8
	.2byte 0xf001
	.2byte 0xfe2a
	.2byte 0x2800
	.2byte 0xd018
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfda1
	.2byte 0x21a8
	.2byte 0x2280
	.2byte 0x2009
	.2byte 0x0489
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xfe6e
	.2byte 0x21b0
	.2byte 0x22c0
	.2byte 0x200a
	.2byte 0x0489
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xfe67
	.2byte 0x21a2
	.2byte 0x22f0
	.2byte 0x200b
	.2byte 0x0489
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xfe60
	.2byte 0xe011
	.2byte 0x489a
	.2byte 0xf001
	.2byte 0xfe0c
	.2byte 0x2800
	.2byte 0xd10c
	.2byte 0x20c4
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfe0e
	.2byte 0x4896
	.2byte 0xf001
	.2byte 0xfe0b
	.2byte 0x4896
	.2byte 0xf001
	.2byte 0xfe08
	.2byte 0x4895
	.2byte 0xf001
	.2byte 0xfe05
	.2byte 0x2009
	.2byte 0xf001
	.2byte 0xfe22
	.2byte 0x2101
	.2byte 0xf7fc
	.2byte 0xf8cd
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfe1c
	.2byte 0x2101
	.2byte 0xf7fc
	.2byte 0xf8c7
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfe16
	.2byte 0x2101
	.2byte 0xf7fc
	.2byte 0xf8c1
	.2byte 0x2009
	.2byte 0xf7fc
	.2byte 0xfe78
	.2byte 0x200a
	.2byte 0xf7fc
	.2byte 0xfe75
	.2byte 0x200b
	.2byte 0xf7fc
	.2byte 0xfe72
	.2byte 0x200c
	.2byte 0xf001
	.2byte 0xfe07
	.2byte 0x2101
	.2byte 0xf7fc
	.2byte 0xf8b2
	.2byte 0x20c4
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfdd8
	.2byte 0x2800
	.2byte 0xd03e
	.2byte 0x2501
	.2byte 0x2077
	.2byte 0x2109
	.2byte 0x226d
	.2byte 0x230b
	.2byte 0x9500
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfd8d
	.2byte 0x487a
	.2byte 0xf001
	.2byte 0xfdca
	.2byte 0x2800
	.2byte 0xd007
	.2byte 0x2076
	.2byte 0x2109
	.2byte 0x2268
	.2byte 0x230d
	.2byte 0x9500
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfd80
	.2byte 0xf001
	.2byte 0xfd6e
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfd37
	.2byte 0xe022
	.2byte 0x4873
	.2byte 0xf001
	.2byte 0xfdb7
	.2byte 0x2800
	.2byte 0xd10c
	.2byte 0x21ae
	.2byte 0x229e
	.2byte 0x2003
	.2byte 0x0489
	.2byte 0x0492
	.2byte 0xf001
	.2byte 0xfdfe
	.2byte 0x2003
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfe31
	.2byte 0xe010
	.2byte 0x21c2
	.2byte 0x229e
	.2byte 0x2008
	.2byte 0x0489
	.2byte 0x0492
	.2byte 0xf001
	.2byte 0xfdf1
	.2byte 0x232e
	.2byte 0x2227
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x206e
	.2byte 0x2127
	.2byte 0x2205
	.2byte 0x2301
	.2byte 0xf001
	.2byte 0xfd63
.L_02003d20_3:
	ldr r1, [pc, #328]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #384]
	cmp r2, r3
	beq .L_02003d20_5
	b .L_02003d20_6
.L_02003d20_5:
	ldr r2, [pc, #308]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #8
	bls .L_02003d20_7
	b .L_02003d20_6
.L_02003d20_7:
	ldr r2, [pc, #360]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	.2byte 0xbfac
	.2byte 0x0200
	.2byte 0xbfac
	.2byte 0x0200
	.2byte 0xc0f0
	.2byte 0x0200
	.2byte 0xc0f0
	.2byte 0x0200
	.2byte 0xc1f4
	.2byte 0x0200
	.2byte 0xc1f4
	.2byte 0x0200
	.2byte 0xc1a4
	.2byte 0x0200
	.2byte 0xc1a4
	.2byte 0x0200
	.2byte 0xc1a4
	.2byte 0x0200
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfd91
	.2byte 0x2101
	.2byte 0xf7fc
	.2byte 0xf83c
	.2byte 0x2009
	.2byte 0xf001
	.2byte 0xfd8b
	.2byte 0x2101
	.2byte 0xf7fc
	.2byte 0xf836
	.2byte 0x4849
	.2byte 0xf001
	.2byte 0xfd5d
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe111
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfcd3
	.2byte 0x20d3
	.2byte 0xf001
	.2byte 0xfe44
	.2byte 0x21b8
	.2byte 0x2284
	.2byte 0x2008
	.2byte 0x0409
	.2byte 0x0492
	.2byte 0xf001
	.2byte 0xfd9d
	.2byte 0x2309
	.2byte 0x9300
	.2byte 0x251f
	.2byte 0x200b
	.2byte 0x211f
	.2byte 0x2201
	.2byte 0x2304
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfd0f
	.2byte 0x230b
	.2byte 0x9300
	.2byte 0x2007
	.2byte 0x211e
	.2byte 0x2201
	.2byte 0x2304
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfd06
	.2byte 0x2501
	.2byte 0x2602
	.2byte 0x204a
	.2byte 0x213a
	.2byte 0x2246
	.2byte 0x2320
	.2byte 0x9500
	.2byte 0x9601
	.2byte 0xf001
	.2byte 0xfcf0
	.2byte 0x204a
	.2byte 0x213b
	.2byte 0x2246
	.2byte 0x2322
	.2byte 0x9500
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfce8
	.2byte 0x2303
	.2byte 0x9300
	.2byte 0x4698
	.2byte 0x204c
	.2byte 0x213c
	.2byte 0x224a
	.2byte 0x2326
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfcde
	.2byte 0x204d
	.2byte 0x213c
	.2byte 0x224c
	.2byte 0x2326
	.2byte 0x9600
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfcd6
	.2byte 0x4641
	.2byte 0x9101
	.2byte 0x204b
	.2byte 0x213a
	.2byte 0x2256
	.2byte 0x2329
	.2byte 0x9500
	.2byte 0xf001
	.2byte 0xfccd
	.2byte 0x204b
	.2byte 0x213b
	.2byte 0x2256
	.2byte 0x232b
	.2byte 0x9500
	.2byte 0x9601
	.2byte 0xf001
	.2byte 0xfcc5
	.2byte 0x204c
	.2byte 0x213b
	.2byte 0x2250
	.2byte 0x2331
	.2byte 0x9600
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfcbd
	.2byte 0x204d
	.2byte 0x213b
	.2byte 0x2252
	.2byte 0x2331
	.2byte 0x9600
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfcb5
	.2byte 0xe0ab
	.4byte 0x00000111
	.4byte 0x02000240
	.4byte 0x00000242
	.4byte 0x00000039
	.4byte 0x03001ebc
	.4byte 0x04000050
	.4byte 0x00001010
	.4byte 0x02001000
	.4byte 0x00000875
	.4byte 0x02008d59
	.4byte 0x00000036
	.4byte 0x0200bdb8
	.2byte 0x0876
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x0311
	.2byte 0x0000
	.2byte 0x0312
	.2byte 0x0000
	.2byte 0x0313
	.2byte 0x0000
	.2byte 0x0873
	.2byte 0x0000
	.4byte 0x00000037
	.4byte 0x0200bf88
	.2byte 0x0302
	.2byte 0x0000
	.2byte 0xf001
	.2byte 0xfc78
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfc41
	.2byte 0x48d6
	.2byte 0xf001
	.2byte 0xfcc2
	.2byte 0x2800
	.2byte 0xd016
	.2byte 0x48d4
	.2byte 0xf001
	.2byte 0xfcbd
	.2byte 0x2800
	.2byte 0xd011
	.2byte 0x2501
	.2byte 0x2005
	.2byte 0x2102
	.2byte 0x2205
	.2byte 0x230b
	.2byte 0x9500
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfc72
	.2byte 0x2302
	.2byte 0x9301
	.2byte 0x2009
	.2byte 0x2101
	.2byte 0x2209
	.2byte 0x2307
	.2byte 0x9500
	.2byte 0xf001
	.2byte 0xfc69
	.2byte 0x48ca
	.2byte 0xf001
	.2byte 0xfca6
	.2byte 0x2800
	.2byte 0xd05b
	.2byte 0x21b0
	.2byte 0x22d8
	.2byte 0x03c9
	.2byte 0x0412
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfced
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfcc2
	.2byte 0x4ac3
	.2byte 0x68c3
	.2byte 0x189b
	.2byte 0x60c3
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfcbb
	.2byte 0x1c05
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfcb7
	.2byte 0x68c3
	.2byte 0x63eb
	.2byte 0x2302
	.2byte 0x2501
	.2byte 0x9301
	.2byte 0x2009
	.2byte 0x2101
	.2byte 0x2209
	.2byte 0x2307
	.2byte 0x9500
	.2byte 0xf001
	.2byte 0xfc43
	.2byte 0x2005
	.2byte 0x2102
	.2byte 0x2205
	.2byte 0x230b
	.2byte 0x9500
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfc3b
	.2byte 0x2309
	.2byte 0x220a
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2009
	.2byte 0x2105
	.2byte 0x2201
	.2byte 0x2301
	.2byte 0xf001
	.2byte 0xfc3d
	.2byte 0xe027
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfc95
	.2byte 0x2101
	.2byte 0xf7fb
	.2byte 0xff40
	.2byte 0x48ac
	.2byte 0xf001
	.2byte 0xfc67
	.2byte 0x2800
	.2byte 0xd01c
	.2byte 0x2000
	.2byte 0xf7fd
	.2byte 0xfaf4
	.2byte 0x2327
	.2byte 0x9301
	.2byte 0x252a
	.2byte 0x202a
	.2byte 0x2129
	.2byte 0x2204
	.2byte 0x2301
	.2byte 0x9500
	.2byte 0xf001
	.2byte 0xfc24
	.2byte 0x2329
	.2byte 0x9301
	.2byte 0x202a
	.2byte 0x2128
	.2byte 0x2204
	.2byte 0x2301
	.2byte 0x9500
	.2byte 0xf001
	.2byte 0xfc1b
	.2byte 0x21b0
	.2byte 0x22a0
	.2byte 0x200a
	.2byte 0x0489
	.2byte 0x0492
	.2byte 0xf001
	.2byte 0xfc98
.L_02003d20_6:
	ldr r1, [pc, #624]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #616]
	cmp r2, r3
	beq .L_02003d20_8
	b .L_02003d20_9
.L_02003d20_8:
	ldr r2, [pc, #604]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #15
	bls .L_02003d20_10
	b .L_02003d20_9
.L_02003d20_10:
	ldr r2, [pc, #592]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	stmia r3!, {r7}
	lsls r0, r0, #8
	stmia r3!, {r7}
	lsls r0, r0, #8
	.2byte 0xc37c
	lsls r0, r0, #8
	stmia r2!, {r2, r5, r6}
	lsls r0, r0, #8
	stmia r2!, {r2, r5, r6}
	lsls r0, r0, #8
	stmia r2!, {r2, r5, r6}
	lsls r0, r0, #8
	stmia r3!, {r3, r7}
	lsls r0, r0, #8
	stmia r3!, {r3, r7}
	lsls r0, r0, #8
	stmia r3!, {r3, r7}
	lsls r0, r0, #8
	.2byte 0xc34a
	lsls r0, r0, #8
	.2byte 0xc34a
	lsls r0, r0, #8
	stmia r3!, {r7}
	lsls r0, r0, #8
	.2byte 0xc37c
	lsls r0, r0, #8
	.2byte 0xc4b6
	lsls r0, r0, #8
	.2byte 0xc5ac
	lsls r0, r0, #8
	.2byte 0xc47c
	lsls r0, r0, #8
	movs r0, #15
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
	movs r0, #16
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
	movs r0, #17
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
	movs r0, #18
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
	movs r0, #19
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
	movs r0, #0
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
	movs r7, #158
	movs r6, #204
	movs r5, #0
	lsls r7, r7, #18
	lsls r6, r6, #2
.L_02003d20_11:
	adds r0, r6, #0
	bl 0x0200da84
	cmp r0, #0
	beq .L_02003d20_15
	adds r0, r5, #0
.L_02003d20_12:
	movs r2, #176
	adds r0, #15
	adds r1, r7, #0
	lsls r2, r2, #15
	bl 0x0200db24
.L_02003d20_13:
	b .L_02003d20_16
.L_02003d20_15:
	adds r0, r6, #1
	bl 0x0200da84
	cmp r0, #0
	beq .L_02003d20_16
.L_02003d20_14:
	movs r3, #128
	adds r0, r5, #0
	lsls r3, r3, #14
	movs r2, #176
	adds r0, #15
	adds r1, r7, r3
	lsls r2, r2, #15
	bl 0x0200db24
.L_02003d20_16:
	movs r1, #128
	lsls r1, r1, #15
	adds r5, #1
	adds r7, r7, r1
	adds r6, #2
	cmp r5, #3
	bls .L_02003d20_11
	movs r0, #206
	lsls r0, r0, #2
	bl 0x0200da84
	cmp r0, #0
	beq .L_02003d20_17
	movs r1, #230
	movs r2, #176
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl 0x0200db24
	movs r3, #58
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #53
	movs r1, #10
	b .L_02003d20_18
.L_02003d20_17:
	ldr r0, [pc, #336]
	bl 0x0200da84
	cmp r0, #0
	bne .L_02003d20_19
	b .L_02003d20_9
.L_02003d20_19:
	movs r1, #238
	movs r2, #176
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl 0x0200db24
	movs r3, #58
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #53
	movs r1, #10
	b .L_02003d20_18
	.2byte 0x20d2
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfb99
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe189
	.2byte 0x21e4
	.2byte 0x22a4
	.2byte 0x2014
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfbdf
	.2byte 0x231f
	.2byte 0x2214
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x201d
	.2byte 0x2114
.L_02003d20_18:
	movs r2, #1
	movs r3, #1
	bl 0x0200da1c
	b .L_02003d20_9
	.2byte 0xf7ff
	.2byte 0xfa04
	.2byte 0x20aa
	.2byte 0xf001
	.2byte 0xfc67
	.2byte 0xe171
	.2byte 0x4832
	.2byte 0xf001
	.2byte 0xfb7b
	.2byte 0x2800
	.2byte 0xd024
	.2byte 0x4831
	.2byte 0xf001
	.2byte 0xfb76
	.2byte 0x2800
	.2byte 0xd01f
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfb99
	.2byte 0x4b2f
	.2byte 0x60c3
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfb94
	.2byte 0x1c05
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfb90
	.2byte 0x68c3
	.2byte 0x211d
	.2byte 0x63eb
	.2byte 0x2006
	.2byte 0x2501
	.2byte 0x220a
	.2byte 0x2317
	.2byte 0x9500
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfb1d
	.2byte 0x2302
	.2byte 0x9301
	.2byte 0x200a
	.2byte 0x211c
	.2byte 0x220a
	.2byte 0x2312
	.2byte 0x9500
	.2byte 0xf001
	.2byte 0xfb14
	.2byte 0x4826
	.2byte 0xf001
	.2byte 0xfb51
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe141
	.2byte 0x21a8
	.2byte 0x22bc
	.2byte 0x0409
	.2byte 0x0452
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfb97
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfb6c
	.2byte 0x4a18
	.2byte 0x68c3
	.2byte 0x189b
	.2byte 0x60c3
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfb65
	.2byte 0x1c05
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfb61
	.2byte 0x68c3
	.2byte 0x211d
	.2byte 0x63eb
	.2byte 0x2006
	.2byte 0x2501
	.2byte 0x220a
	.2byte 0x2317
	.2byte 0x9500
	.2byte 0x9501
	.2byte 0xf001
	.2byte 0xfaee
	.2byte 0x2302
	.2byte 0x9301
	.2byte 0x200a
	.2byte 0x211c
	.2byte 0x220a
	.2byte 0x2312
	.2byte 0x9500
	.2byte 0xf001
	.2byte 0xfae5
	.2byte 0x230a
	.2byte 0x2213
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x200a
	.2byte 0x2110
	.2byte 0x2201
	.2byte 0x2301
	.2byte 0xf001
	.2byte 0xfae7
	.2byte 0xf001
	.2byte 0xfac9
	.2byte 0xe10b
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x0256
	.2byte 0x0000
	.2byte 0x0874
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0x0306
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000038
	.4byte 0x0200c224
	.4byte 0x00000339
	.2byte 0x0878
	.2byte 0x0000
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfa7d
	.2byte 0x21cc
	.2byte 0x2298
	.2byte 0x200a
	.2byte 0x0489
	.2byte 0x0492
	.2byte 0xf001
	.2byte 0xfb4a
	.2byte 0x21c2
	.2byte 0x2290
	.2byte 0x0492
	.2byte 0x0489
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfb43
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfb18
	.2byte 0x2101
	.2byte 0xf7fb
	.2byte 0xfdc3
	.2byte 0x2000
	.2byte 0xf7fd
	.2byte 0xfdc0
	.2byte 0x2001
	.2byte 0xf7fd
	.2byte 0xfc7f
	.2byte 0x4beb
	.2byte 0x21e1
	.2byte 0x0049
	.2byte 0x185d
	.2byte 0x2200
	.2byte 0x5eab
	.2byte 0x2b0e
	.2byte 0xd102
	.2byte 0x20d3
	.2byte 0xf001
	.2byte 0xfbcc
	.2byte 0x2009
	.2byte 0xf001
	.2byte 0xfb01
	.2byte 0x2101
	.2byte 0xf7fb
	.2byte 0xfdac
	.2byte 0x2102
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfb5e
	.2byte 0x200a
	.2byte 0xf001
	.2byte 0xfaf7
	.2byte 0x2302
	.2byte 0x3022
	.2byte 0x7003
	.2byte 0x200b
	.2byte 0xf001
	.2byte 0xfaf1
	.2byte 0x2101
	.2byte 0xf7fb
	.2byte 0xfd9c
	.2byte 0x200c
	.2byte 0xf001
	.2byte 0xfaeb
	.2byte 0x2101
	.2byte 0xf7fb
	.2byte 0xfd96
	.2byte 0x200d
	.2byte 0xf001
	.2byte 0xfae5
	.2byte 0x2101
	.2byte 0xf7fb
	.2byte 0xfd90
	.2byte 0x200e
	.2byte 0xf001
	.2byte 0xfadf
	.2byte 0x2101
	.2byte 0xf7fb
	.2byte 0xfd8a
	.2byte 0x200a
	.2byte 0xf7fc
	.2byte 0xfb41
	.2byte 0x200b
	.2byte 0xf7fc
	.2byte 0xfb3e
	.2byte 0x2100
	.2byte 0x5e6b
	.2byte 0x2b0e
	.2byte 0xd000
	.2byte 0xe09c
	.2byte 0x48cd
	.2byte 0xf001
	.2byte 0xfaa6
	.2byte 0x2800
	.2byte 0xd10d
	.2byte 0x20c6
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfaa8
	.2byte 0x48c9
	.2byte 0xf001
	.2byte 0xfaa5
	.2byte 0x48c9
	.2byte 0xf001
	.2byte 0xfaa2
	.2byte 0x48c8
	.2byte 0xf001
	.2byte 0xfa9f
	.2byte 0xe089
	.2byte 0x2000
	.2byte 0xf7fd
	.2byte 0xfd69
	.2byte 0x48c3
	.2byte 0xf001
	.2byte 0xfa90
	.2byte 0x2800
	.2byte 0xd00f
	.2byte 0x2002
	.2byte 0xf7fd
	.2byte 0xfc23
	.2byte 0x2009
	.2byte 0xf001
	.2byte 0xfab0
	.2byte 0x6903
	.2byte 0x151b
	.2byte 0x2b2c
	.2byte 0xd177
	.2byte 0x21c8
	.2byte 0x48be
	.2byte 0x0109
	.2byte 0xf001
	.2byte 0xf9ff
	.2byte 0xe071
	.2byte 0x48b9
	.2byte 0xf001
	.2byte 0xfa7b
	.2byte 0x2800
	.2byte 0xd003
	.2byte 0x2001
	.2byte 0xf7fd
	.2byte 0xfc0e
	.2byte 0xe068
	.2byte 0x48b6
	.2byte 0xf001
	.2byte 0xfa72
	.2byte 0x2800
	.2byte 0xd163
	.2byte 0x2000
	.2byte 0xf7fd
	.2byte 0xfc05
	.2byte 0xe05f
	.2byte 0xf001
	.2byte 0xfa82
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfa8f
	.2byte 0x2101
	.2byte 0xf7fb
	.2byte 0xfd3a
	.2byte 0x210f
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfad4
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfa85
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfa2e
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfa7f
	.2byte 0x2390
	.2byte 0x041b
	.2byte 0x60c3
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfa79
	.2byte 0x2500
	.2byte 0x3055
	.2byte 0x7005
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfa73
	.2byte 0x6445
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfa6f
	.2byte 0x4ba1
	.2byte 0x6483
	.2byte 0x4ba1
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x3ac0
	.2byte 0x601a
	.2byte 0xf001
	.2byte 0xfaf4
	.2byte 0xf001
	.2byte 0xfafa
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfa5f
	.2byte 0x2303
	.2byte 0x3055
	.2byte 0x7003
	.2byte 0x20bd
	.2byte 0xf001
	.2byte 0xfb21
	.2byte 0x2020
	.2byte 0xf001
	.2byte 0xfa42
	.2byte 0x20bc
	.2byte 0xf001
	.2byte 0xfb1b
	.2byte 0x2008
	.2byte 0xf001
	.2byte 0xfa50
	.2byte 0x2102
	.2byte 0xf7fb
	.2byte 0xfcfb
	.2byte 0x20c0
	.2byte 0x21c0
	.2byte 0x2280
	.2byte 0x0280
	.2byte 0x0289
	.2byte 0x0252
	.2byte 0xf001
	.2byte 0xf9f9
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x4249
	.2byte 0x4a8c
	.2byte 0x4240
	.2byte 0xf001
	.2byte 0xf9f2
	.2byte 0xf001
	.2byte 0xf9f4
	.2byte 0x203c
	.2byte 0xf001
	.2byte 0xfa25
	.2byte 0x2010
	.2byte 0xf001
	.2byte 0xfab2
	.2byte 0xf001
	.2byte 0xfa28
.L_02003d20_9:
	ldr r1, [pc, #500]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #524]
	cmp r2, r3
	beq .L_02003d20_20
	b .L_02003d20_21
.L_02003d20_20:
	ldr r2, [pc, #480]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	bne .L_02003d20_22
	b .L_02003d20_23
.L_02003d20_22:
	cmp r3, #11
	bgt .L_02003d20_24
	cmp r3, #10
	beq .L_02003d20_25
	b .L_02003d20_21
.L_02003d20_24:
	cmp r3, #12
	bne .L_02003d20_26
	b .L_02003d20_27
.L_02003d20_26:
	cmp r3, #15
	beq .L_02003d20_28
	b .L_02003d20_21
.L_02003d20_25:
	movs r1, #200
	ldr r0, [pc, #480]
	lsls r1, r1, #4
	bl 0x0200d984
	ldr r0, [pc, #436]
	bl 0x0200da84
	cmp r0, #0
	beq .L_02003d20_29
	b .L_02003d20_21
.L_02003d20_29:
	bl 0x0200b788
	movs r0, #170
	bl 0x0200dc54
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200dbe4
	movs r1, #1
	ldr r0, [pc, #440]
	bl 0x0200dbdc
	movs r0, #30
	bl 0x0200dbec
	bl 0x0200dc04
	movs r1, #1
	movs r0, #0
	bl 0x0200db2c
	movs r0, #30
	bl 0x0200daac
	ldr r0, [pc, #416]
	movs r1, #0
	movs r2, #0
	bl 0x0200da5c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200dbdc
	movs r0, #30
	bl 0x0200dbec
	b .L_02003d20_21
.L_02003d20_28:
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200db24
	bl 0x0200da14
	movs r2, #0
	ldr r3, [pc, #376]
	strh r2, [r3]
	movs r0, #9
	bl 0x0200dad4
	ldr r3, [pc, #368]
	movs r1, #1
	str r3, [r0, #24]
	movs r0, #14
	bl 0x0200db9c
	movs r0, #15
	movs r1, #1
	bl 0x0200db9c
	movs r0, #16
	movs r1, #1
	bl 0x0200db9c
	ldr r0, [pc, #288]
	bl 0x0200da84
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02003d20_30
	movs r3, #40
	movs r2, #34
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #34
	movs r2, #5
	movs r3, #4
	bl 0x0200da1c
	movs r3, #5
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #91
	movs r2, #40
	movs r3, #91
	bl 0x0200da04
	b .L_02003d20_21
.L_02003d20_30:
	movs r1, #206
	movs r2, #150
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200db24
	movs r3, #52
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #4
	movs r2, #3
	movs r1, #36
	movs r0, #116
	bl 0x0200da1c
	movs r0, #10
	bl 0x0200dad4
	adds r0, #85
	strb r5, [r0]
	movs r0, #10
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
	bl 0x0200dab4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl 0x0200dbb4
	bl 0x0200dbc4
	adds r0, #85
	strb r5, [r0]
	movs r1, #1
	movs r0, #0
	bl 0x0200db9c
	movs r0, #13
	movs r1, #1
	bl 0x0200db9c
.L_02003d20_31:
	bl 0x0200cc68
	bl 0x0200da0c
	bl 0x0200dabc
	b .L_02003d20_21
.L_02003d20_27:
	movs r0, #1
	bl 0x0200daac
	movs r0, #0
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
.L_02003d20_23:
	movs r3, #40
	movs r2, #34
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #34
	movs r2, #5
	movs r3, #4
	bl 0x0200da1c
	movs r3, #4
	str r3, [sp, #4]
	movs r0, #45
	movs r5, #5
	movs r1, #91
	movs r2, #40
	movs r3, #91
	str r5, [sp, #0]
	bl 0x0200da04
	ldr r0, [pc, #116]
	bl 0x0200da84
.L_02003d20_32:
	cmp r0, #0
	bne .L_02003d20_33
	movs r3, #6
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #45
	movs r2, #50
	movs r3, #45
	str r5, [sp, #0]
	bl 0x0200da04
	movs r3, #3
	str r3, [sp, #4]
	movs r0, #50
	movs r1, #105
	movs r2, #50
	movs r3, #109
	str r5, [sp, #0]
	bl 0x0200da04
	bl 0x0200d9e4
	movs r0, #1
	bl 0x0200d97c
	b .L_02003d20_34
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000109
	.2byte 0x0319
	.2byte 0x0000
	.2byte 0x031a
	.2byte 0x0000
	.2byte 0x031b
	.2byte 0x0000
	.2byte 0xa2c5
	.2byte 0x0200
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0xe666
	.2byte 0x0000
	.4byte 0x00000039
	.4byte 0x0200adcd
	.4byte 0x00010003
	.4byte 0x00001633
	.4byte 0x04000050
	.4byte 0xffff0000
	.4byte 0x00000881
.L_02003d20_33:
	movs r0, #14
	movs r1, #1
	bl 0x0200db9c
	movs r0, #15
	movs r1, #1
	bl 0x0200db9c
	movs r0, #16
	movs r1, #1
	bl 0x0200db9c
.L_02003d20_34:
	movs r0, #9
	bl 0x0200dad4
	ldr r3, [pc, #308]
	str r3, [r0, #24]
	ldr r0, [pc, #308]
	bl 0x0200da84
	cmp r0, #0
	bne .L_02003d20_35
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200db24
	movs r1, #206
	movs r2, #150
	lsls r2, r2, #18
	movs r0, #10
	lsls r1, r1, #18
	bl 0x0200db24
	movs r0, #10
	movs r1, #1
	bl 0x0200db9c
	movs r3, #52
	movs r2, #37
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #116
	movs r1, #37
	movs r2, #3
	movs r3, #3
	bl 0x0200da1c
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #126
	movs r1, #35
	movs r2, #116
	movs r3, #35
	bl 0x0200da04
	movs r1, #200
	ldr r0, [pc, #228]
	lsls r1, r1, #4
	bl 0x0200d984
	b .L_02003d20_21
.L_02003d20_35:
	ldr r0, [pc, #220]
	bl 0x0200da84
	cmp r0, #0
	bne .L_02003d20_36
	movs r0, #135
	lsls r0, r0, #4
	bl 0x0200da84
	cmp r0, #0
	bne .L_02003d20_37
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200db94
	movs r0, #3
	movs r1, #16
	bl 0x0200db2c
	b .L_02003d20_38
.L_02003d20_37:
	movs r1, #210
	movs r2, #158
	movs r0, #3
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200db24
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200db94
.L_02003d20_38:
	movs r0, #3
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #126
	movs r1, #35
	movs r2, #116
	movs r3, #35
	bl 0x0200da04
	movs r1, #200
	ldr r0, [pc, #116]
	lsls r1, r1, #4
	bl 0x0200d984
	b .L_02003d20_40
.L_02003d20_36:
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200db24
	movs r1, #206
	movs r2, #150
.L_02003d20_39:
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200db24
	movs r3, #52
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #4
	movs r0, #116
	movs r1, #36
	movs r2, #3
	bl 0x0200da1c
	movs r0, #10
	bl 0x0200dad4
	movs r3, #254
	adds r0, #89
	strb r3, [r0]
	movs r0, #1
	bl 0x0200daac
.L_02003d20_40:
	movs r0, #10
	bl 0x0200dad4
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #10
	bl 0x0200dad4
	movs r1, #1
	bl 0x02008030
.L_02003d20_21:
	movs r0, #0
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0xffff0000
	.4byte 0x0000082b
	.4byte 0x0200a649
	.4byte 0x00000871
	.global Func_02004a08
	.thumb_func
Func_02004a08:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl 0x0200dad4
	ldr r3, [r0, #8]
	cmp r3, #0
	bge 0x0200ca1c
.L_02004a18:
	ldr r2, [pc, #556]
	adds r3, r3, r2
	asrs r3, r3, #20
	cmp r3, #48
	beq .L_02004a18_0
	b 0x0200cc40
.L_02004a18_0:
	bl 0x0200dab4
	ldr r5, [pc, #544]
	adds r0, r5, #0
.L_02004a2c:
	bl 0x0200db7c
	movs r0, #20
	bl 0x0200daac
	movs r0, #3
	movs r1, #1
	bl 0x0200db4c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200db94
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200db8c
	movs r1, #3
	movs r0, #3
	bl 0x0200db34
	movs r0, #20
	bl 0x0200daac
	movs r1, #3
	movs r0, #0
	bl 0x0200db34
	movs r0, #20
	bl 0x0200daac
	movs r0, #60
	bl 0x0200daac
	movs r1, #16
	movs r0, #3
	bl 0x0200db2c
	movs r0, #50
	bl 0x0200daac
	movs r0, #3
	movs r1, #1
	bl 0x0200db2c
	movs r1, #0
	movs r0, #3
	bl 0x0200db84
	movs r0, #0
	movs r1, #0
	bl 0x0200dacc
	cmp r0, #1
	bne .L_02004a2c_0
	movs r0, #20
	bl 0x0200daac
	movs r1, #2
	movs r0, #3
	bl 0x0200db4c
	movs r0, #20
	bl 0x0200daac
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200db8c
	movs r1, #4
	movs r0, #3
	bl 0x0200db34
	movs r0, #20
	bl 0x0200daac
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200db8c
	movs r1, #3
	movs r0, #3
	bl 0x0200db34
	movs r0, #20
	bl 0x0200daac
	movs r1, #0
	movs r0, #3
	bl 0x0200db84
	movs r0, #0
	movs r1, #0
	bl 0x0200dacc
	cmp r0, #1
	bne .L_02004a2c_0
	movs r0, #20
	bl 0x0200daac
	movs r1, #4
	movs r0, #3
	bl 0x0200db34
	movs r0, #20
	bl 0x0200daac
	adds r0, r5, #5
	b .L_02004a2c_1
.L_02004a2c_2:
	movs r0, #20
	bl 0x0200daac
	movs r1, #4
	movs r0, #3
	bl 0x0200db34
	movs r0, #20
	bl 0x0200daac
	ldr r0, [pc, #296]
.L_02004a2c_1:
	bl 0x0200db7c
	movs r1, #0
	movs r0, #3
	bl 0x0200db84
	movs r0, #0
	movs r1, #0
	bl 0x0200dacc
	cmp r0, #1
	beq .L_02004a2c_2
.L_02004a2c_0:
	ldr r0, [pc, #276]
	bl 0x0200db7c
	movs r0, #3
	ldr r1, [pc, #272]
	ldr r2, [pc, #272]
	bl 0x0200dadc
	movs r1, #182
	movs r2, #158
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #3
	bl 0x0200db0c
	movs r0, #20
	bl 0x0200daac
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200db8c
	movs r0, #3
	movs r1, #16
	bl 0x0200db2c
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200db8c
	movs r0, #3
.L_02004b80:
	movs r1, #1
	bl 0x0200db2c
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200db5c
	movs r1, #4
	movs r0, #3
	bl 0x0200db34
	movs r0, #20
	bl 0x0200daac
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200db8c
	movs r2, #90
	movs r0, #3
	ldr r1, [pc, #176]
	bl 0x0200dba4
	movs r1, #3
	movs r0, #3
	bl 0x0200db34
	movs r0, #20
	bl 0x0200daac
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200db8c
	movs r1, #1
	movs r0, #3
	bl 0x0200dac4
	movs r0, #68
	bl 0x0200da8c
	movs r1, #1
	movs r2, #0
	movs r0, #3
	bl 0x0200da9c
	movs r1, #1
	movs r2, #0
	movs r0, #3
	bl 0x0200daa4
	movs r0, #3
	bl 0x0200da6c
	movs r0, #3
	movs r1, #2
	bl 0x0200db2c
	movs r0, #0
	bl 0x0200dad4
	cmp r0, #0
	beq .L_02004b80_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200daf4
.L_02004b80_0:
	movs r0, #3
	bl 0x0200db1c
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200db24
	movs r3, #46
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #39
	movs r2, #5
	movs r3, #1
	bl 0x0200da1c
	ldr r0, [pc, #44]
	bl 0x0200da8c
	bl 0x0200dabc
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0xffff
	.2byte 0x000f
	.2byte 0x1591
	.2byte 0x0000
	.2byte 0x1639
	.2byte 0x0000
	.2byte 0x1597
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x00000873
	.section .text.x0200cfcc,"ax",%progbits
	.p2align 2
	.global Func_02004fcc
	.thumb_func
Func_02004fcc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	mov r9, r0
	movs r1, #4
	movs r0, #35
	sub sp, #4
	bl 0x0200d9b4
	mov r2, r8
	str r2, [r0]
	ldr r0, [pc, #304]
	bl 0x0200da84
	adds r3, r0, #0
	cmp r3, #0
	bne .L_02004fcc_0
	mov r0, sp
	str r3, [r0]
	mov r1, r8
	ldr r3, [pc, #292]
	ldr r2, [pc, #292]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r9
	mov r0, r8
	str r3, [r0, #4]
	b .L_02004fcc_1
.L_02004fcc_0:
	ldr r3, [pc, #284]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl 0x0200dc14
	adds r7, r0, #0
	ldr r4, [r7, #16]
	adds r3, r4, #0
	cmp r4, #0
	bge .L_02004fcc_2
	ldr r0, [pc, #264]
	adds r3, r4, r0
.L_02004fcc_2:
	ldr r1, [r7, #8]
	asrs r3, r3, #20
	lsls r2, r3, #7
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02004fcc_3
	ldr r0, [pc, #248]
	adds r3, r1, r0
.L_02004fcc_3:
	asrs r3, r3, #20
	adds r3, r2, r3
	ldr r2, [pc, #244]
	lsls r3, r3, #2
	mov r0, r8
	adds r2, r2, r3
	ldr r3, [r0]
	mov r10, r2
	cmp r3, #0
	beq .L_02004fcc_4
	ldr r3, [r0, #20]
	cmp r3, #0
	beq .L_02004fcc_4
	ldr r2, [r7, #12]
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r0, #26
	adds r3, r4, #0
	bl 0x0200d9d4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02004fcc_5
	ldr r3, [r7, #20]
	ldr r1, [pc, #200]
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl 0x0200d9cc
	adds r2, r5, #0
	movs r3, #4
	adds r2, #85
	str r7, [r5, #104]
	strb r3, [r2]
	ldr r0, [pc, #184]
	ldr r3, [r5, #12]
	adds r3, r3, r0
	str r3, [r5, #12]
	cmp r6, #0
	beq .L_02004fcc_6
	mov r2, r8
	ldr r3, [r2]
	movs r1, #6
	subs r1, r1, r3
	adds r0, r6, #0
	bl 0x0200d9bc
	adds r2, r6, #0
	adds r2, #38
	movs r3, #0
	strb r3, [r2]
	ldrb r2, [r6, #9]
	subs r3, #13
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
.L_02004fcc_6:
	mov r3, r8
	str r5, [r3, #20]
	b .L_02004fcc_5
.L_02004fcc_4:
	movs r3, #0
	mov r0, r8
	str r3, [r0, #20]
.L_02004fcc_5:
	mov r2, r10
	ldrb r3, [r2, #2]
	cmp r3, r9
	bne .L_02004fcc_7
	mov r0, r8
	ldr r3, [r0, #24]
	cmp r3, #0
	beq .L_02004fcc_7
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	movs r0, #26
	bl 0x0200d9d4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02004fcc_1
	ldr r3, [r7, #20]
	ldr r1, [pc, #96]
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl 0x0200d9cc
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	adds r2, r5, #0
	movs r3, #2
	adds r2, #35
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	cmp r6, #0
	beq .L_02004fcc_8
	adds r0, r6, #0
	movs r1, #6
	bl 0x0200d9bc
	adds r2, r6, #0
	ldr r3, [pc, #8]
	adds r2, #38
	strb r3, [r2]
.L_02004fcc_8:
	mov r2, r8
	str r5, [r2, #24]
	b .L_02004fcc_1
	.4byte 0x00000000
	.4byte 0x00000109
	.4byte 0x040000d4
	.4byte 0x85000007
	.4byte 0x02000240
	.4byte 0x000fffff
	.4byte 0x02010000
	.4byte 0x0200de38
	.4byte 0xffff8000
	.4byte 0x0200de20
.L_02004fcc_7:
	movs r3, #0
	mov r0, r8
	str r3, [r0, #24]
.L_02004fcc_1:
	sub sp, #-4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x0200d5c0,"ax",%progbits
	.p2align 2
	.global Makyuri_RunActorMove
	.thumb_func
Makyuri_RunActorMove:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #440]
	ldr r2, [r3]
	subs r3, #32
	ldr r3, [r3]
	sub sp, #28
	ldr r2, [r2]
	movs r0, #250
	str r3, [sp, #12]
	ldr r3, [pc, #428]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	ldr r1, [sp, #12]
	lsls r3, r3, #2
	adds r3, #20
	ldr r7, [r1, r3]
	mov r8, r2
	adds r2, r7, #0
	adds r2, #85
	str r2, [sp, #0]
	ldrb r3, [r2]
	str r3, [sp, #4]
	ldr r3, [pc, #404]
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ldr r1, [pc, #400]
	ands r3, r2
	lsls r3, r3, #1
	ldrh r6, [r1, r3]
	ldrsh r3, [r1, r3]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_020055c0_0
	b .L_020055c0_1
.L_020055c0_0:
	movs r2, #16
	ldr r4, [r7, #8]
	ldr r1, [pc, #380]
	add r2, sp
	mov r11, r2
	movs r2, #128
	ands r4, r1
	lsls r2, r2, #12
	adds r5, r4, r2
	mov r3, r11
	str r5, [r3]
	ldr r3, [r7, #20]
	mov r0, r11
	str r3, [r0, #4]
	ldr r0, [r7, #16]
	ands r0, r1
	adds r2, r0, r2
	mov r1, r11
	str r2, [r1, #8]
	cmp r2, #0
	bge .L_020055c0_2
	ldr r3, [pc, #344]
	adds r2, r0, r3
.L_020055c0_2:
	asrs r3, r2, #20
	lsls r2, r3, #7
	adds r3, r5, #0
	cmp r3, #0
	bge .L_020055c0_3
	ldr r0, [pc, #332]
	adds r3, r4, r0
.L_020055c0_3:
	asrs r3, r3, #20
	adds r3, r2, r3
	ldr r1, [pc, #328]
	lsls r3, r3, #2
	movs r0, #128
	adds r5, r3, r1
	mov r2, r11
	lsls r0, r0, #14
	adds r1, r6, #0
	bl 0x0200d9ac
	mov r2, r11
	ldr r3, [r2, #8]
	cmp r3, #0
	bge .L_020055c0_4
	ldr r0, [pc, #304]
	adds r3, r3, r0
.L_020055c0_4:
	asrs r3, r3, #20
	mov r1, r11
	lsls r2, r3, #7
	ldr r3, [r1]
	cmp r3, #0
	bge .L_020055c0_5
	ldr r0, [pc, #288]
	adds r3, r3, r0
.L_020055c0_5:
	asrs r3, r3, #20
	adds r3, r2, r3
	ldr r1, [pc, #276]
	lsls r3, r3, #2
	adds r1, r3, r1
	str r1, [sp, #8]
	mov r2, r8
	ldrb r3, [r5, #2]
	ldr r1, [r2, #4]
	cmp r3, r1
	beq .L_020055c0_6
	ldr r0, [sp, #8]
	ldrb r3, [r0, #2]
	cmp r3, r1
	bne .L_020055c0_6
	ldr r3, [r2]
	cmp r3, #0
	bne .L_020055c0_6
	b .L_020055c0_1
.L_020055c0_6:
	bl 0x0200dab4
	adds r0, r7, #0
	add r1, sp, #16
	bl 0x0200da24
	mov r10, r0
	cmp r0, #0
	beq .L_020055c0_7
	b .L_020055c0_1
.L_020055c0_7:
	mov r1, r8
	ldr r5, [r1, #24]
	cmp r5, #0
	beq .L_020055c0_8
	adds r3, r5, #0
	adds r3, #100
	mov r2, r10
	strh r2, [r3]
	ldr r1, [pc, #216]
	adds r0, r5, #0
	bl 0x0200d9cc
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200d9c4
	mov r3, r10
	mov r0, r8
	str r3, [r0, #24]
.L_020055c0_8:
	ldr r1, [sp, #8]
	mov r0, r8
	ldrb r2, [r1, #2]
	ldr r3, [r0, #4]
	cmp r2, r3
	bne .L_020055c0_9
	ldr r3, [r0]
	cmp r3, #0
	beq .L_020055c0_9
	ldr r6, [r0, #20]
	movs r0, #26
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	bl 0x0200d9d4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020055c0_10
	ldr r1, [r5, #80]
	ldr r3, [r6, #20]
	mov r9, r1
	str r3, [r5, #20]
	ldr r1, [pc, #152]
	bl 0x0200d9cc
	adds r3, r5, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	mov r0, r10
	adds r3, #15
	adds r2, r5, #0
	strh r0, [r3]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	mov r2, r11
	mov r0, r11
	ldr r1, [r2]
	ldr r3, [r0, #8]
	ldr r2, [r2, #4]
	adds r0, r5, #0
	bl 0x0200d9ec
	mov r1, r9
	cmp r1, #0
	beq .L_020055c0_11
	mov r0, r9
	movs r1, #6
	bl 0x0200d9bc
	mov r2, r9
	ldr r3, [pc, #40]
	adds r2, #38
	strb r3, [r2]
.L_020055c0_11:
	mov r2, r8
	str r5, [r2, #24]
.L_020055c0_10:
	mov r0, r8
	ldr r3, [r0]
	subs r5, r3, #1
	str r5, [r0]
	cmp r5, #0
	bne .L_020055c0_12
	ldr r0, [r0, #20]
	bl 0x0200d9dc
	mov r1, r8
	str r5, [r1, #20]
	ldr r0, [pc, #52]
	bl 0x0200da94
	b .L_020055c0_9
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001edc
	.4byte 0x02000240
	.4byte 0x03001ae8
	.4byte 0x0200de44
	.4byte 0xfff00000
	.4byte 0x0017ffff
	.4byte 0x02010000
	.4byte 0x000fffff
	.4byte 0x0200de2c
	.4byte 0x0200de20
	.4byte 0x00000161
.L_020055c0_12:
	mov r2, r8
	ldr r0, [r2, #20]
	cmp r0, #0
	beq .L_020055c0_9
	movs r1, #6
	subs r1, r1, r5
	bl 0x0200d9c4
.L_020055c0_9:
	movs r1, #6
	adds r0, r7, #0
	bl 0x0200d9c4
	movs r0, #3
	bl 0x0200d97c
	movs r0, #152
	bl 0x0200dc64
	adds r0, r7, #0
	movs r1, #7
	bl 0x0200d9c4
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	ldr r3, [sp, #0]
	ldrb r2, [r3]
	ldr r0, [sp, #0]
	movs r3, #126
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200da2c
	mov r3, r11
	movs r2, #2
	ldrsh r1, [r3, r2]
	movs r0, #10
	ldrsh r2, [r3, r0]
	movs r0, #0
	bl 0x0200dafc
	movs r1, #6
	adds r0, r7, #0
	bl 0x0200d9c4
	movs r0, #2
	bl 0x0200d97c
	ldr r1, [sp, #8]
	mov r0, r8
	ldrb r2, [r1, #2]
	ldr r3, [r0, #4]
	cmp r2, r3
	beq .L_020055c0_13
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200da2c
	b .L_020055c0_14
.L_020055c0_13:
	movs r0, #215
	bl 0x0200dc64
.L_020055c0_14:
	movs r0, #1
	bl 0x0200d97c
	add r1, sp, #4
	ldr r2, [sp, #0]
	ldrb r1, [r1]
	strb r1, [r2]
	ldr r3, [sp, #8]
	mov r0, r8
	ldrb r2, [r3, #2]
	ldr r3, [r0, #4]
	cmp r2, r3
	bne .L_020055c0_15
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_020055c0_15
	movs r1, #18
	adds r0, r7, #0
	bl 0x0200d9c4
	movs r0, #241
	bl 0x0200dc64
	movs r1, #15
	ldr r6, [pc, #132]
	movs r5, #0
	mov r10, r1
	b .L_020055c0_16
.L_020055c0_18:
	movs r0, #1
	bl 0x0200d97c
	adds r5, #1
.L_020055c0_16:
	adds r3, r5, #0
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	bne .L_020055c0_17
	adds r0, r7, #0
	bl 0x0200d158
.L_020055c0_17:
	cmp r5, #31
	ble .L_020055c0_18
	ldr r3, [r6]
	cmp r3, #0
	beq .L_020055c0_18
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200dc64
	movs r0, #1
	bl 0x0200d97c
	mov r0, r8
	ldr r3, [r0, #12]
	str r3, [r7, #8]
	ldr r3, [r0, #16]
	movs r1, #1
	str r3, [r7, #16]
	adds r0, r7, #0
	bl 0x0200da2c
.L_020055c0_15:
	mov r1, r8
	movs r3, #0
	str r3, [r1, #8]
	bl 0x0200dabc
	movs r0, #216
	ldr r2, [sp, #12]
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #128
	ldr r4, [pc, #44]
	ldr r0, [r3]
	lsls r1, r1, #14
	mov r12, pc
	bx r4
	.2byte 0x9903
	.2byte 0x23da
	.2byte 0x005b
	.2byte 0x18ca
	.2byte 0x6813
	.2byte 0x181b
	.2byte 0x6013
.L_020055c0_1:
	sub sp, #-28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001c94
	.4byte 0x03000118
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.global MakyuriHeya_RiseScript
MakyuriHeya_RiseScript:
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
	.global MakyuriHeya_SparkScript
MakyuriHeya_SparkScript:
	.4byte 0x00000022
	.4byte 0x0200d1b1
	.4byte 0x0000001b
	.4byte 0x00000022
	.4byte 0x0200d1f1
	.4byte 0x00000010
	.global Makyuri_StartMoveScript
Makyuri_StartMoveScript:
	.4byte 0x00000022
	.4byte 0x0200d219
	.4byte 0x0000001b
	.global Makyuri_PillarScript
Makyuri_PillarScript:
	.4byte 0x00000022
	.4byte 0x0200d245
	.4byte 0x00000010
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200dca8
	.4byte 0x0200dce0
	.4byte 0x0200dd18
	.global MakyuriHeya_SparkBurstScript
MakyuriHeya_SparkBurstScript:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global MakyuriHeya_PushScriptA
MakyuriHeya_PushScriptA:
	.4byte 0x00000000
	.4byte 0x0000002d
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000022
	.4byte 0x0200a2ed
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000010
	.global MakyuriHeya_PushScriptB
MakyuriHeya_PushScriptB:
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000022
	.4byte 0x0200a2ed
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x0200a305
	.4byte 0x00000010
	.global MakyuriHeya_PushScriptD
MakyuriHeya_PushScriptD:
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000022
	.4byte 0x0200a2ed
	.4byte 0x00000003
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x0200a305
	.4byte 0x00000010
	.global MakyuriHeya_PushScriptC
MakyuriHeya_PushScriptC:
	.4byte 0x00000000
	.4byte 0x0000002d
	.4byte 0x00000022
	.4byte 0x0200a2ed
	.4byte 0x00000010
	.global gPaletteCycleRed
gPaletteCycleRed:
	.4byte 0x00000000
	.global gPaletteCycleGreen
gPaletteCycleGreen:
	.4byte 0x00000000
	.global gPaletteCycleBlue
gPaletteCycleBlue:
	.4byte 0x00000000
	.global MakyuriHeya_GateCells
MakyuriHeya_GateCells:
	.4byte 0x0001000f
	.4byte 0x00020001
	.4byte 0x000d000a
	.4byte 0x00010001
	.4byte 0x00060002
	.4byte 0x0001000b
	.4byte 0x00020001
	.4byte 0x00090006
	.4byte 0x00010001
	.4byte 0x00060002
	.2byte 0xffff
	.global MakyuriHeya_GateCloseCells
MakyuriHeya_GateCloseCells:
	.2byte 0x0009
	.4byte 0x00010001
	.4byte 0x000a0002
	.4byte 0x0001000b
	.4byte 0x00020001
	.4byte 0x000d0006
	.4byte 0x00010001
	.4byte 0x00060002
	.4byte 0x0001000f
	.4byte 0x00020001
	.4byte 0xffff0006
	.global MakyuriHeya_FloorSwitchCells
MakyuriHeya_FloorSwitchCells:
	.4byte 0x001c0010
	.4byte 0x00020001
	.4byte 0x000e000a
	.4byte 0x0001001c
	.4byte 0x00060002
	.4byte 0x001c000c
	.4byte 0x00020001
	.4byte 0x000a0006
	.4byte 0x0001001c
	.4byte 0x00060002
	.2byte 0xffff
	.global MakyuriHeya_FloorSwitchCloseCells
MakyuriHeya_FloorSwitchCloseCells:
	.2byte 0x000a
	.4byte 0x0001001c
	.4byte 0x000a0002
	.4byte 0x001c000c
	.4byte 0x00020001
	.4byte 0x000e0006
	.4byte 0x0001001c
	.4byte 0x00060002
	.4byte 0x001c0010
	.4byte 0x00020001
	.4byte 0xffff0006
	.global MakyuriHeya_ColumnCells
MakyuriHeya_ColumnCells:
	.4byte 0x0023007e
	.4byte 0x00020001
	.4byte 0x007e0002
	.4byte 0x00010026
	.4byte 0x00020002
	.4byte 0x0029007e
	.4byte 0x00020001
	.4byte 0x007e0002
	.4byte 0x0001002c
	.4byte 0x00020002
	.4byte 0x002f007e
	.4byte 0x00020001
	.4byte 0xffff0002
	.global gMakyuriHeyaEntrancesOther
gMakyuriHeyaEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000038
	.4byte 0xc00001b8
	.4byte 0x00100000
	.4byte 0x01f00010
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0xc00001b8
	.4byte 0x00100000
	.4byte 0x01f00010
	.4byte 0x000001c0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0x00100000
	.4byte 0x01f00010
	.4byte 0x000001c0
	.4byte 0xffff0004
	.4byte 0x000002f8
	.4byte 0xc0000158
	.4byte 0x02200000
	.4byte 0x03300010
	.4byte 0x00000160
	.4byte 0xffff0005
	.4byte 0x00000078
	.4byte 0xc00003a8
	.4byte 0x00400000
	.4byte 0x01800310
	.4byte 0x000003b0
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0xc00003a8
	.4byte 0x00400000
	.4byte 0x01800310
	.4byte 0x000003b0
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x40000248
	.4byte 0x00500000
	.4byte 0x01400210
	.4byte 0x000002b0
	.4byte 0xffff0008
	.4byte 0x00000118
	.4byte 0x40000248
	.4byte 0x00500000
	.4byte 0x01400210
	.4byte 0x000002b0
	.4byte 0xffff0009
	.4byte 0x00000308
	.4byte 0x400001d8
	.4byte 0x02800000
	.4byte 0x038001a0
	.4byte 0x000002c0
	.4byte 0xffff000a
	.4byte 0x00000358
	.4byte 0x40000208
	.4byte 0x02800000
	.4byte 0x038001a0
	.4byte 0x000002c0
	.4byte 0xffff000b
	.4byte 0x000001d8
	.4byte 0xc00002b8
	.4byte 0x01600000
	.4byte 0x02500200
	.4byte 0x000002d0
	.4byte 0xffff000f
	.4byte 0x000002c8
	.4byte 0x40000108
	.4byte 0x02200000
	.4byte 0x03300010
	.4byte 0x00000160
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEntrances2
gMakyuriHeyaEntrances2:
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0x400001d8
	.4byte 0x00200000
	.4byte 0x01e00190
	.4byte 0x00000350
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0xc0000338
	.4byte 0x00200000
	.4byte 0x01e00190
	.4byte 0x00000350
	.4byte 0xffff0003
	.4byte 0x00000098
	.4byte 0x400000b8
	.4byte 0x00300000
	.4byte 0x02500010
	.4byte 0x000001a0
	.4byte 0xffff0004
	.4byte 0x000001e8
	.4byte 0x40000088
	.4byte 0x00300000
	.4byte 0x02500010
	.4byte 0x000001a0
	.4byte 0xffff0005
	.4byte 0x000002d8
	.4byte 0x40000058
	.4byte 0x02900000
	.4byte 0x03800010
	.4byte 0x00000110
	.4byte 0xffff0006
	.4byte 0x000002d8
	.4byte 0x400000d8
	.4byte 0x02900000
	.4byte 0x03800010
	.4byte 0x00000110
	.4byte 0xffff0007
	.4byte 0x00000278
	.4byte 0xc00002c8
	.4byte 0x02400000
	.4byte 0x034001e0
	.4byte 0x000002e0
	.4byte 0xffff0008
	.4byte 0x00000318
	.4byte 0xc00002c8
	.4byte 0x02400000
	.4byte 0x034001e0
	.4byte 0x000002e0
	.4byte 0xffff0009
	.4byte 0x000002c8
	.4byte 0x40000238
	.4byte 0x02400000
	.4byte 0x034001e0
	.4byte 0x000002e0
	.4byte 0xffff000a
	.4byte 0x00000378
	.4byte 0xc00001b8
	.4byte 0x02f80000
	.4byte 0x04100130
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEntrances3
gMakyuriHeyaEntrances3:
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000058
	.4byte 0xc00000b8
	.4byte 0x00200000
	.4byte 0x02200020
	.4byte 0x000000d0
	.4byte 0xffff0002
	.4byte 0x000001f8
	.4byte 0xc00000b8
	.4byte 0x00200000
	.4byte 0x02200020
	.4byte 0x000000d0
	.4byte 0xffff0003
	.4byte 0x00000108
	.4byte 0x40000078
	.4byte 0x00200000
	.4byte 0x02200020
	.4byte 0x000000d0
	.4byte 0xffff0004
	.4byte 0x00000288
	.4byte 0xc00000a8
	.4byte 0x02500000
	.4byte 0x03e00010
	.4byte 0x00000140
	.4byte 0xffff0005
	.4byte 0x00000308
	.4byte 0x40000058
	.4byte 0x02500000
	.4byte 0x03e00010
	.4byte 0x00000140
	.4byte 0xffff0006
	.4byte 0x000003a8
	.4byte 0x40000058
	.4byte 0x02500000
	.4byte 0x03e00010
	.4byte 0x00000140
	.4byte 0xffff0007
	.4byte 0x00000058
	.4byte 0x40000148
	.4byte 0x00300000
	.4byte 0x01200110
	.4byte 0x000001b0
	.4byte 0xffff0008
	.4byte 0x000000a8
	.4byte 0x40000148
	.4byte 0x00300000
	.4byte 0x01200110
	.4byte 0x000001b0
	.4byte 0xffff0009
	.4byte 0x000000f8
	.4byte 0x40000148
	.4byte 0x00300000
	.4byte 0x01200110
	.4byte 0x000001b0
	.4byte 0xffff000a
	.4byte 0x000001d8
	.4byte 0x40000148
	.4byte 0x01600000
	.4byte 0x02500110
	.4byte 0x000001b0
	.4byte 0xffff000b
	.4byte 0x000001a8
	.4byte 0xc0000198
	.4byte 0x01600000
	.4byte 0x02500110
	.4byte 0x000001b0
	.4byte 0xffff000c
	.4byte 0x00000058
	.4byte 0x40000258
	.4byte 0x00300000
	.4byte 0x02200200
	.4byte 0x000002a0
	.4byte 0xffff000d
	.4byte 0x000001c8
	.4byte 0x40000258
	.4byte 0x00300000
	.4byte 0x02200200
	.4byte 0x000002a0
	.4byte 0xffff000e
	.4byte 0x00000378
	.4byte 0x400002c8
	.4byte 0x02b00000
	.4byte 0x03a00180
	.4byte 0x00000330
	.4byte 0xffff000f
	.4byte 0x000000a8
	.4byte 0x40000170
	.4byte 0x00280000
	.4byte 0x01280100
	.4byte 0x000001c0
	.4byte 0xffff0010
	.4byte 0x000002f8
	.4byte 0x40000248
	.4byte 0x02b00000
	.4byte 0x03a00180
	.4byte 0x00000330
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEntrances4
gMakyuriHeyaEntrances4:
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000e8
	.4byte 0x40000068
	.4byte 0x00400000
	.4byte 0x01200020
	.4byte 0x000001f8
	.4byte 0xffff0002
	.4byte 0x000000e8
	.4byte 0xc00001e8
	.4byte 0x00400000
	.4byte 0x01200020
	.4byte 0x000001f8
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x40000068
	.4byte 0x01600000
	.4byte 0x02500020
	.4byte 0x00000148
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0xc0000138
	.4byte 0x01600000
	.4byte 0x02500020
	.4byte 0x00000148
	.4byte 0xffff000a
	.4byte 0x00000318
	.4byte 0x400000d8
	.4byte 0x02980000
	.4byte 0x0398007c
	.4byte 0x00000124
	.4byte 0xffff000b
	.4byte 0x00000348
	.4byte 0xc0000358
	.4byte 0x02400000
	.4byte 0x03c00148
	.4byte 0x00000390
	.4byte 0xffff000c
	.4byte 0x00000348
	.4byte 0x40000258
	.4byte 0x02400000
	.4byte 0x03c00148
	.4byte 0x00000390
	.4byte 0xffff000d
	.4byte 0x00000368
	.4byte 0x40000258
	.4byte 0x02d00000
	.4byte 0x03c00220
	.4byte 0x00000390
	.4byte 0xffff000f
	.4byte 0x000002a8
	.4byte 0x40000248
	.4byte 0x02400000
	.4byte 0x03c00148
	.4byte 0x00000390
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriHeya_SceneTable
MakyuriHeya_SceneTable:
	.4byte 0x00000036
	.4byte 0x0010b035
	.4byte 0x0020c035
	.4byte 0x00309036
	.4byte 0x0040f035
	.4byte 0x0050e035
	.4byte 0x00607038
	.4byte 0x00703037
	.4byte 0x00806035
	.4byte 0x00903036
	.4byte 0x00a05036
	.4byte 0x00b0a036
	.4byte 0x00c01037
	.4byte 0x00d19035
	.4byte 0x00000037
	.4byte 0x0010b036
	.4byte 0x00202037
	.4byte 0x00301038
	.4byte 0x00401039
	.4byte 0x00503038
	.4byte 0x00606036
	.4byte 0x00704037
	.4byte 0x00805038
	.4byte 0x00906038
	.4byte 0x00a02038
	.4byte 0x00000038
	.4byte 0x00105037
	.4byte 0x00209037
	.4byte 0x0030a037
	.4byte 0x00408038
	.4byte 0x00507037
	.4byte 0x00608037
	.4byte 0x00708036
	.4byte 0x0080e038
	.4byte 0x00904038
	.4byte 0x00a03039
	.4byte 0x00b0d038
	.4byte 0x00c02039
	.4byte 0x00d0b038
	.4byte 0x00e09038
	.4byte 0x00f0f038
	.4byte 0x01010038
	.4byte 0x00000039
	.4byte 0x00106037
	.4byte 0x0020c038
	.4byte 0x0030a038
	.4byte 0x0040a039
	.4byte 0x00504039
	.4byte 0x0060a002
	.4byte 0x00701035
	.4byte 0x0085a002
	.4byte 0x0090503a
	.4byte 0x000001ff
	.global gMakyuriHeyaPlacementsOther
gMakyuriHeyaPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaPlacements1
gMakyuriHeyaPlacements1:
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000007
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000007
	.4byte 0x02c00000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x012b0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaPlacements2
gMakyuriHeyaPlacements2:
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x024b0000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000007
	.4byte 0x02c00000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0x0045005b
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaPlacements3
gMakyuriHeyaPlacements3:
	.4byte 0x187700e0
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0x087700e0
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000007
	.4byte 0x03300000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x027b0000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x027b0000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x027b0000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaPlacements4
gMakyuriHeyaPlacements4:
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x0002c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0x188100e0
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x1881006c
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00014000
	.4byte 0x1881006a
	.4byte 0x00000002
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0x18810067
	.4byte 0x00000002
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0x1881006f
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEvents1
gMakyuriHeyaEvents1:
	.4byte 0x00000001
	.4byte 0xffff0034
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0035
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff003b
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff0036
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff003e
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff0039
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0037
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0038
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x18750020
	.4byte 0x02008ef1
	.4byte 0x00000202
	.4byte 0x1875001f
	.4byte 0x02008efd
	.4byte 0x00000002
	.4byte 0x1875001f
	.4byte 0x02008ef1
	.4byte 0x00000a02
	.4byte 0x18750015
	.4byte 0x02008efd
	.4byte 0x00000002
	.4byte 0x18750015
	.4byte 0x02008ee5
	.4byte 0x00000202
	.4byte 0x1875000b
	.4byte 0x02008efd
	.4byte 0x00000002
	.4byte 0x1875000b
	.4byte 0x02008f09
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x0200b6ad
	.4byte 0x00000202
	.4byte 0xffff0019
	.4byte 0x02009165
	.4byte 0x00000000
	.4byte 0x08730003
	.4byte 0x0000158f
	.4byte 0x00008d15
	.4byte 0x08730003
	.4byte 0x00001590
	.4byte 0x10009585
	.4byte 0xffff0000
	.4byte 0x0200d949
	.4byte 0x00008c15
	.4byte 0x08730008
	.4byte 0x0200ca09
	.4byte 0x00000013
	.4byte 0x0f640064
	.4byte 0x001000e3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEvents2
gMakyuriHeyaEvents2:
	.4byte 0x00000001
	.4byte 0xffff0034
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0038
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0039
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0035
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0036
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0037
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff003a
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff003b
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000000a
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x0200b6ad
	.4byte 0x00000202
	.4byte 0xffff0001
	.4byte 0x020099b9
	.4byte 0x00000002
	.4byte 0x13020002
	.4byte 0x02009341
	.4byte 0x00000002
	.4byte 0x13020003
	.4byte 0x02009341
	.4byte 0x00000002
	.4byte 0x13020004
	.4byte 0x02009341
	.4byte 0x00000202
	.4byte 0x0874000a
	.4byte 0x02009409
	.4byte 0x00000002
	.4byte 0x0874000c
	.4byte 0x020094cd
	.4byte 0x00000002
	.4byte 0x0874000a
	.4byte 0x02009569
	.4byte 0x00000002
	.4byte 0x0874000d
	.4byte 0x02009569
	.4byte 0x00000202
	.4byte 0xffff002d
	.4byte 0x020099b9
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte 0x02009341
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008cc1
	.4byte 0x00000003
	.2byte 0x0014
	.2byte 0xffff
	.2byte 0xb5dd
.L_02006d22:
	lsls	r0, r0, #8
	str	r5, [sp, #532]
.L_02006d26:
	asrs	r0, r0, #32
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xd949
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r3, r1
	lsrs	r4, r6, #1
	str	r4, [sp, #132]
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r5, r4, #1
	lsrs	r6, r4, #29
	lsls	r2, r7, #2
	movs	r0, r2
	movs	r3, r2
	movs	r0, r0
	lsls	r6, r4, #1
	lsrs	r7, r4, #29
	lsls	r5, r6, #2
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.global gMakyuriHeyaEvents3
gMakyuriHeyaEvents3:
	movs	r1, r0
	movs	r0, r0
	movs	r3, r6
.L_02006d66:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r4, r6
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	stmia	r6!, {r1}
	movs	r0, r0
	movs	r5, r6
.L_02006d7e:
	.2byte 0xffff
	.2byte 0x9061
	lsls	r0, r0, #8
	movs	r1, r6
	movs	r0, r0
.L_02006d88:
	movs	r1, r7
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
.L_02006d90:
	movs	r1, r0
	movs	r0, r0
	movs	r3, r7
	.2byte 0xffff
	.2byte 0x0009
	movs	r0, r0
	movs	r1, r4
	movs	r0, r0
	movs	r2, r7
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r7
	.2byte 0xffff
	.2byte 0x000b
	movs	r0, r0
.L_02006db4:
	movs	r1, r4
	movs	r0, r0
	movs	r4, r7
	.2byte 0xffff
	.2byte 0x000a
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r7
	.2byte 0xffff
	.2byte 0x000c
	movs	r0, r0
	stmia	r6!, {r1}
	movs	r0, r0
	movs	r7, r7
	.2byte 0xffff
	.2byte 0x9081
	lsls	r0, r0, #8
	movs	r1, r0
	movs	r0, r0
	movs	r6, r6
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
.L_02006de4:
	movs	r1, r0
	movs	r0, r0
	movs	r7, r6
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r0, r7
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
.L_02006dfc:
	movs	r1, r6
	movs	r0, r0
	lsls	r0, r0, #1
	.2byte 0xffff
	.2byte 0x000e
	movs	r0, r0
	lsls	r2, r0, #8
	movs	r0, r0
	movs	r2, r1
	lsrs	r0, r7, #1
	ldr	r3, [sp, #868]
	lsls	r0, r0, #8
	lsls	r2, r0, #8
	movs	r0, r0
	movs	r4, r1
	lsrs	r0, r7, #1
	ldr	r3, [sp, #868]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r4, r1
	lsrs	r0, r7, #1
	ldr	r4, [sp, #628]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r2, r1
	lsrs	r0, r7, #1
	ldr	r5, [sp, #244]
	lsls	r0, r0, #8
	lsls	r2, r0, #8
	movs	r0, r0
.L_02006e3c:
	movs	r0, r5
	.2byte 0xffff
	.2byte 0xa319
.L_02006e42:
	lsls	r0, r0, #8
	lsls	r2, r0, #8
	movs	r0, r0
	movs	r7, r5
	.2byte 0xffff
	.2byte 0xa505
	lsls	r0, r0, #8
	movs	r3, r0
	movs	r0, r0
	movs	r4, r2
	.2byte 0xffff
	.2byte 0xb5dd
	lsls	r0, r0, #8
	str	r5, [sp, #532]
	asrs	r0, r0, #32
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xd949
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r0, r1
	lsrs	r0, r7, #1
	ldr	r3, [sp, #964]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r7, r1
	.2byte 0xffff
	.2byte 0xa51d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
.L_02006e82:
	movs	r0, r0
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xa51d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
.L_02006e90:
	movs	r1, r2
	.2byte 0xffff
	.2byte 0xa51d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r2, r2
	.2byte 0xffff
	.2byte 0xa51d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
.L_02006ea6:
	movs	r0, r0
	movs	r3, r2
	.2byte 0xffff
	.2byte 0xa51d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
.L_02006eb2:
	movs	r0, r0
	movs	r4, r2
.L_02006eb6:
	.2byte 0xffff
	.2byte 0xa601
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
.L_02006ec0:
	.2byte 0x0000
	.2byte 0x0000
.L_02006ec4:
	.2byte 0x0000
	.2byte 0x0000
	.global gMakyuriHeyaEventsOther
gMakyuriHeyaEventsOther:
	movs	r1, r0
	movs	r0, r0
	movs	r4, r5
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r3, r5
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r3, r6
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r4, r6
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
.L_02006ef8:
	movs	r1, r6
	movs	r0, r0
.L_02006efc:
	movs	r5, r6
.L_02006efe:
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r6
	.2byte 0xffff
	.2byte 0x0004
.L_02006f0e:
	movs	r0, r0
	stmia	r6!, {r1}
	movs	r0, r0
	movs	r7, r6
	.2byte 0xffff
	.2byte 0x90a1
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r3, r0
	lsrs	r0, r6, #1
	add	r6, pc, #532
	lsls	r0, r0, #8
	ldrh	r5, [r2, #40]
	movs	r0, r0
.L_02006f2c:
	lsls	r3, r0, #16
	lsrs	r0, r6, #1
	add	r6, pc, #532
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r3, r0
.L_02006f3a:
	lsrs	r1, r6, #1
	add	r0, sp, #84
	lsls	r0, r0, #8
.L_02006f40:
	ldrh	r5, [r2, #40]
.L_02006f42:
	movs	r0, r0
	movs	r3, r0
	lsrs	r1, r6, #1
.L_02006f48:
	asrs	r6, r7, #21
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r6, r1
	adds	r1, r0, r2
	asrs	r2, r5, #24
	movs	r0, r0
.L_02006f58:
	movs	r0, r0
	movs	r0, r0
	movs	r7, r1
	adds	r1, r0, r2
	asrs	r3, r5, #24
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r0, r2
	adds	r1, r0, r2
	asrs	r4, r5, #24
.L_02006f6e:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r1, r2
	adds	r1, r0, r2
	asrs	r5, r5, #24
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r6, r1
	adds	r1, r0, r2
	asrs	r6, r5, #24
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r7, r1
	adds	r1, r0, r2
.L_02006f90:
	asrs	r7, r5, #24
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r0, r2
	adds	r1, r0, r2
	asrs	r0, r6, #24
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r1, r2
	adds	r1, r0, r2
	asrs	r1, r6, #24
	movs	r0, r0
	movs	r3, r0
	movs	r0, r0
	movs	r2, r1
.L_02006fb2:
	.2byte 0xffff
	.2byte 0x8f15
.L_02006fb6:
	lsls	r0, r0, #8
	movs	r3, r0
	movs	r0, r0
.L_02006fbc:
	movs	r2, r5
	.2byte 0xffff
	.2byte 0xb5b9
	lsls	r0, r0, #8
.L_02006fc4:
	movs	r3, r0
	movs	r0, r0
	movs	r5, r0
	lsls	r0, r0, #8
	.2byte 0xb601
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r2, r1
	lsrs	r3, r5, #32
	movs	r0, r0
	movs	r0, r0
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r2, r1
.L_02006fe2:
	lsrs	r1, r6, #1
.L_02006fe4:
	add	r0, sp, #708
	lsls	r0, r0, #8
	ldrb	r5, [r2, r4]
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0xaf69
.L_02006ff2:
	lsls	r0, r0, #8
.L_02006ff4:
	str	r5, [sp, #532]
	asrs	r0, r0, #32
	movs	r0, r0
.L_02006ffa:
	.2byte 0xffff
	.2byte 0xd949
	lsls	r0, r0, #8
	.2byte 0xb904
	movs	r0, r0
	movs	r5, r0
	adds	r1, r0, r2
	.2byte 0xb661
	lsls	r0, r0, #8
.L_0200700c:
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
.L_02007012:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
