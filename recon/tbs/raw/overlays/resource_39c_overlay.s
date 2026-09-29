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
	.section .text.x0200cfcc,"ax",%progbits
	.p2align 2
	.global Makyuri_SpawnLightObjects
	.thumb_func
Makyuri_SpawnLightObjects:
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
