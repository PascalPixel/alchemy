.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/ENTRY.INC"
	.section .text.x020089dc,"ax",%progbits
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	ldr r0, [r0, #80]
	movs r3, #3
	ldrb r2, [r0, #9]
	ands r1, r3
	movs r3, #13
	negs r3, r3
	lsls r1, r1, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
	bx lr
	.2byte 0x0000
	.global Func_020009f4
	.thumb_func
Func_020009f4:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200ada8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020009f4_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200adf0
	adds r0, r5, #0
	movs r1, #14
	bl 0x0200aed0
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200adf8
	adds r0, r5, #0
	b .L_020009f4_1
.L_020009f4_0:
	movs r0, #0
.L_020009f4_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000a4c
	.thumb_func
Func_02000a4c:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200ada8
	adds r5, r0, #0
	cmp r5, #0
	beq 0x02008aa6
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
.L_02000a6e:
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
.L_02000a80:
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200adf0
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200aed0
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_02000a80_0
	.2byte 0x2000
.L_02000a80_0:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.section .text.x02008cc0,"ax",%progbits
	.global Func_02000cc0
	.thumb_func
Func_02000cc0:
	push {lr}
	movs r0, #8
	movs r1, #66
	bl 0x0200af38
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #60
.L_02000cd0_1:
	cmp r5, #0
	beq .L_02000cd0_0
	movs r0, #1
	bl 0x0200ad80
	ldr r3, [r6, #40]
	subs r5, #1
	cmp r3, #0
	bne .L_02000cd0_1
.L_02000cd0_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000cf0
	.thumb_func
Func_02000cf0:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r2, [r6, #72]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r7, [r6, #76]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl 0x0200ad78
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02000cf0_0
	adds r3, #15
.L_02000cf0_0:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r1, [r6, #80]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #30]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #30]
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #68
	bl 0x0200ae38
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200af20
	bl 0x0200adb0
	movs r0, #1
	bl 0x0200ad80
	movs r0, #0
	bl 0x0200ae50
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r0, #12]
	movs r0, #0
	bl 0x0200ae50
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #0
	bl 0x0200ae50
	movs r5, #0
	str r5, [r0, #68]
	movs r0, #0
	bl 0x0200ae50
	adds r0, #85
	strb r5, [r0]
	bl 0x0200af50
	bl 0x0200af60
	movs r0, #30
	bl 0x0200ae30
	movs r0, #204
	bl 0x0200af90
	movs r0, #0
	bl 0x0200ae50
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r0, #24
	bl 0x0200ae30
	movs r0, #0
	bl 0x0200ae50
	mov r10, r0
	add r0, sp, #28
	movs r3, #7
	str r3, [r0, #4]
	ldr r3, [pc, #192]
	str r3, [r0, #36]
	ldr r3, [pc, #192]
	str r3, [r0, #8]
	str r3, [r0, #12]
	mov r8, r0
	movs r7, #0
	add r6, sp, #16
.L_02000d4c_0:
	lsls r5, r7, #12
	adds r0, r5, #0
	bl 0x0200ad90
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl 0x0200ad88
	ldr r3, [r6]
	lsrs r2, r3, #31
	adds r2, r3, r2
	asrs r2, r2, #1
	adds r3, r3, r2
	str r0, [r6, #8]
	str r3, [r6]
	mov r2, r10
	ldr r5, [r2, #8]
	ldr r1, [r2, #12]
	ldr r4, [r6, #4]
	ldr r2, [r2, #16]
	str r0, [sp, #4]
	ldr r0, [pc, #136]
	str r4, [sp, #0]
	str r0, [sp, #8]
	mov r4, r8
	adds r0, r5, #0
	adds r7, #1
	str r4, [sp, #12]
	bl 0x02008ae8
	cmp r7, #16
	bls .L_02000d4c_0
	movs r0, #188
	bl 0x0200af90
	movs r0, #0
	ldr r1, [pc, #112]
	bl 0x0200af08
	movs r0, #0
	movs r1, #22
	bl 0x0200ae90
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl 0x0200ae00
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #80]
	negs r0, r0
	negs r1, r1
	bl 0x0200ae00
	bl 0x0200ae08
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200af08
	movs r0, #0
	bl 0x0200ae50
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #72]
	movs r0, #0
	bl 0x0200ae50
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r0, #68]
	bl 0x0200ae40
	sub sp, #-68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02008cf1
	.4byte 0x0000cccc
	.4byte 0x01090001
	.4byte 0x00000101
	.4byte 0x0000e666
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x0200ae38
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl 0x0200af20
	bl 0x0200adb0
	movs r0, #1
	bl 0x0200ad80
	movs r0, #0
	bl 0x0200ae50
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r0, #12]
	movs r0, #0
	bl 0x0200ae50
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r0, #72]
	movs r0, #0
	bl 0x0200ae50
	movs r5, #0
	str r5, [r0, #68]
	movs r0, #0
	bl 0x0200ae50
	adds r0, #85
	strb r5, [r0]
	movs r0, #0
	bl 0x0200ae50
	movs r1, #0
	bl 0x0200adf0
	bl 0x0200af50
	bl 0x0200af60
	movs r0, #10
	bl 0x0200ae30
	movs r0, #204
	bl 0x0200af90
	movs r0, #0
	bl 0x0200ae50
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r0, #0
	bl 0x0200ae50
	ldr r3, [pc, #36]
	str r3, [r0, #40]
	movs r0, #0
	bl 0x0200ae50
	bl 0x02008cd0
	movs r1, #15
	movs r0, #0
	bl 0x0200aec8
	adds r0, r6, #0
	bl 0x0200af30
	bl 0x0200ae40
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xfffb0000
	.global Func_02000f58
	.thumb_func
Func_02000f58:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000f58_0
	ldr r0, [pc, #36]
	b .L_02000f58_1
.L_02000f58_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000f58_2
	ldr r0, [pc, #36]
	b .L_02000f58_1
.L_02000f58_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000f58_3
	ldr r0, [pc, #32]
	b .L_02000f58_1
.L_02000f58_3:
	ldr r0, [pc, #32]
.L_02000f58_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b0
	.4byte 0x0200b270
	.4byte 0x000000af
	.4byte 0x0200b330
	.4byte 0x000000ae
	.4byte 0x0200b4f8
	.4byte 0x0200b558
	.global Func_02000fac
	.thumb_func
Func_02000fac:
	push {lr}
	ldr r3, [pc, #24]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #16]
	movs r0, #0
	cmp r2, r3
	bne .L_02000fac_0
	ldr r0, [pc, #12]
.L_02000fac_0:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x000000b0
	.4byte 0x0200b5b8
	.global Func_02000fd4
	.thumb_func
Func_02000fd4:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b5f8
	.global Func_02000fdc
	.thumb_func
Func_02000fdc:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000fdc_0
	ldr r0, [pc, #36]
	b .L_02000fdc_1
.L_02000fdc_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000fdc_2
	ldr r0, [pc, #36]
	b .L_02000fdc_1
.L_02000fdc_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000fdc_3
	ldr r0, [pc, #32]
	b .L_02000fdc_1
.L_02000fdc_3:
	ldr r0, [pc, #32]
.L_02000fdc_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b0
	.4byte 0x0200b694
	.4byte 0x000000af
	.4byte 0x0200b754
	.4byte 0x000000ae
	.4byte 0x0200b784
	.4byte 0x0200b88c
	.global Func_02001030
	.thumb_func
Func_02001030:
	push {r5, lr}
	movs r0, #0
	bl 0x0200ae50
	adds r5, r0, #0
	bl 0x0200ae38
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #6
	beq .L_02001030_0
	cmp r3, #18
	bne .L_02001030_1
.L_02001030_0:
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #20
	bne .L_02001030_1
	movs r3, #128
	lsls r3, r3, #24
	movs r1, #128
	str r3, [r5, #56]
	str r3, [r5, #64]
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200af00
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200ae58
	movs r0, #0
	movs r1, #4
	movs r2, #0
	bl 0x0200aea0
	ldr r1, [pc, #100]
	ldrh r0, [r5, #6]
	adds r3, r0, r1
	ldr r1, [pc, #96]
	lsls r3, r3, #16
	ldr r2, [pc, #96]
	cmp r3, r1
	bls .L_02001030_2
	ldr r1, [pc, #96]
	adds r3, r0, r1
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, r2
	bhi .L_02001030_3
.L_02001030_2:
	movs r1, #16
	movs r2, #0
	movs r0, #0
	bl 0x0200ae78
	movs r0, #0
	bl 0x0200ae80
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200aef0
	b .L_02001030_1
.L_02001030_3:
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #0
	bl 0x0200ae78
	movs r0, #0
	bl 0x0200ae80
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200aef0
.L_02001030_1:
	bl 0x0200a820
	bl 0x0200ae40
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00004fff
	.4byte 0x1fff0000
	.4byte 0x00001fff
	.4byte 0xffffcfff
	.global Func_020010f4
	.thumb_func
Func_020010f4:
	push {lr}
	bl 0x0200ae38
	bl 0x020080c4
	bl 0x02009030
	bl 0x0200ae40
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200110c
	.thumb_func
Func_0200110c:
	push {r5, lr}
	movs r0, #9
	bl 0x0200ae50
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200ae50
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #12
	bgt .L_0200110c_0
	movs r0, #8
	bl 0x0200ae50
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #6
	bne .L_0200110c_1
	b .L_0200110c_2
.L_0200110c_0:
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #18
	bne .L_0200110c_1
.L_0200110c_2:
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #20
	bne .L_0200110c_1
	bl 0x020090f4
	b .L_0200110c_3
.L_0200110c_1:
	bl 0x0200af40
.L_0200110c_3:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001158
	.thumb_func
Func_02001158:
	push {lr}
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl 0x0200af20
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200aeb8
	bl 0x0200af58
	bl 0x0200af60
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001184
	.thumb_func
Func_02001184:
	push {lr}
	bl 0x0200ae38
	bl 0x02009158
	movs r0, #11
	bl 0x0200af30
	bl 0x0200ae40
	pop {r0}
	bx r0
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	push {lr}
	bl 0x0200ae38
	movs r0, #232
	movs r2, #145
	lsls r2, r2, #17
	movs r1, #0
	movs r3, #223
	lsls r0, r0, #17
	bl 0x020089f4
	bl 0x02009158
	movs r0, #12
	bl 0x0200af30
	bl 0x0200ae40
.L_020011c0:
	pop {r0}
	bx r0
	.global Func_020011c4
	.thumb_func
Func_020011c4:
	push {lr}
	bl 0x0200ae38
	movs r0, #143
	movs r2, #145
	lsls r2, r2, #17
	movs r1, #0
	movs r3, #223
	lsls r0, r0, #16
	bl 0x020089f4
	movs r0, #242
	movs r2, #143
	lsls r2, r2, #17
	movs r1, #0
	movs r3, #253
	lsls r0, r0, #15
	bl 0x020089f4
	bl 0x02009158
	movs r0, #13
	bl 0x0200af30
	bl 0x0200ae40
	pop {r0}
	bx r0
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	push {lr}
	bl 0x0200ae38
	bl 0x02009158
	movs r0, #15
	bl 0x0200af30
	bl 0x0200ae40
	pop {r0}
	bx r0
	.global Func_02001214
	.thumb_func
Func_02001214:
	push {lr}
	bl 0x0200ae38
	movs r0, #0
	movs r1, #1
	bl 0x0200ae90
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200ae10
	bl 0x0200ae40
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002693
	.global Func_02001238
	.thumb_func
Func_02001238:
	push {lr}
	ldr r0, [pc, #888]
	bl 0x0200ae20
	ldr r0, [pc, #884]
	bl 0x0200ae20
	bl 0x0200ae38
	bl 0x0200af68
	ldr r0, [pc, #876]
	bl 0x0200aed8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ae58
	movs r1, #148
	movs r2, #176
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200ae70
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200aef0
	movs r0, #10
	bl 0x0200ae30
	movs r3, #192
	movs r0, #10
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl 0x0200af70
	movs r1, #8
	movs r3, #192
	movs r0, #1
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #8
	bl 0x0200af70
	movs r3, #192
	movs r0, #2
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #8
	bl 0x0200af70
	movs r3, #192
	movs r2, #16
	lsls r3, r3, #8
	movs r1, #24
	movs r0, #3
	bl 0x0200af70
	movs r0, #3
	bl 0x0200ae80
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200af18
	movs r0, #140
	movs r1, #1
	movs r2, #144
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200af20
	bl 0x0200af28
	movs r0, #20
	bl 0x0200ae30
	movs r1, #3
	movs r0, #11
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r1, #0
	movs r0, #11
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #11
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #14
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r0, #13
	movs r1, #2
	bl 0x0200aea8
	movs r1, #2
	movs r0, #12
	bl 0x0200aeb0
	movs r0, #40
	bl 0x0200ae30
	movs r0, #13
	movs r1, #2
	bl 0x0200aea8
	movs r1, #2
	movs r0, #12
	bl 0x0200aeb0
	movs r0, #40
	bl 0x0200ae30
	movs r0, #13
	movs r1, #2
	bl 0x0200aea8
	movs r1, #2
	movs r0, #12
	bl 0x0200aeb0
	movs r0, #40
	bl 0x0200ae30
	movs r1, #129
	movs r2, #50
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200af00
	movs r1, #0
	movs r0, #12
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl 0x0200aef0
	movs r0, #25
	bl 0x0200ae30
	movs r1, #2
	movs r0, #13
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #13
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #4
	movs r0, #13
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #13
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #4
	movs r0, #11
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #11
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	ldr r1, [pc, #400]
	movs r2, #40
	movs r0, #13
	bl 0x0200af00
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #13
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #13
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	movs r0, #13
	ldr r1, [pc, #356]
	movs r2, #75
	bl 0x0200af00
	movs r0, #14
	ldr r1, [pc, #344]
	movs r2, #60
	bl 0x0200af00
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #12
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #11
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #14
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r0, #14
	movs r1, #0
	bl 0x0200aee8
	movs r0, #140
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200af20
	bl 0x0200af28
	movs r0, #20
	bl 0x0200ae30
	movs r1, #3
	movs r0, #10
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #4
	movs r0, #10
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	movs r2, #60
	movs r0, #14
	ldr r1, [pc, #192]
	bl 0x0200af00
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #4
	movs r0, #3
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #3
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #2
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #2
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #1
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #1
	bl 0x0200aee0
	movs r0, #0
	movs r1, #0
	bl 0x0200ae48
	cmp r0, #0
	bne .L_02001238_0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #3
	movs r0, #10
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r0, #10
	movs r1, #0
	bl 0x0200aee8
	ldr r3, [pc, #40]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001238_1
	.4byte 0x00000988
	.4byte 0x0000098a
	.4byte 0x00002702
	.4byte 0x00000107
	.4byte 0x00000101
	.4byte 0x00000105
	.4byte 0x03001ebc
.L_02001238_0:
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #4
	movs r0, #10
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	ldr r3, [pc, #628]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #10
	movs r1, #0
	bl 0x0200aee8
.L_02001238_1:
	movs r0, #10
	bl 0x0200ae30
	movs r0, #14
	ldr r1, [pc, #600]
	movs r2, #60
	bl 0x0200af00
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ae58
	movs r2, #16
	movs r1, #0
	movs r0, #14
	bl 0x0200af80
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200aef0
	movs r0, #35
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #14
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	movs r1, #129
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200af00
	movs r1, #0
	movs r0, #3
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #2
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #2
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #3
	movs r0, #1
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r1, #0
	movs r0, #1
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200af00
	movs r1, #0
	movs r0, #14
	bl 0x0200aee0
	movs r0, #0
	movs r1, #0
	bl 0x0200ae48
	cmp r0, #0
	bne .L_02001238_2
	movs r0, #30
	bl 0x0200ae30
	movs r1, #4
	movs r0, #14
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r0, #14
	movs r1, #0
	bl 0x0200aee8
	ldr r3, [pc, #340]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001238_3
.L_02001238_2:
	movs r0, #30
	bl 0x0200ae30
	movs r1, #4
	movs r0, #14
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	ldr r3, [pc, #304]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #14
	movs r1, #0
	bl 0x0200aee8
.L_02001238_3:
	movs r0, #10
	bl 0x0200ae30
	movs r1, #129
	movs r2, #50
	movs r0, #10
	lsls r1, r1, #1
	bl 0x0200af00
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #13
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r0, #13
	ldr r1, [pc, #240]
	ldr r2, [pc, #240]
	bl 0x0200ae58
	movs r2, #16
	movs r1, #0
	movs r0, #13
	bl 0x0200af80
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #13
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r0, #12
	ldr r1, [pc, #200]
	ldr r2, [pc, #200]
	bl 0x0200ae58
	movs r1, #0
	movs r2, #16
	movs r0, #12
	bl 0x0200af80
	movs r0, #20
	bl 0x0200ae30
	movs r2, #50
	movs r0, #12
	ldr r1, [pc, #180]
	bl 0x0200af00
	movs r1, #0
	movs r0, #12
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #3
	movs r0, #14
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r0, #10
	bl 0x0200ae30
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200af00
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200aef0
	movs r0, #25
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #10
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee0
	movs r0, #0
	movs r1, #0
	bl 0x0200ae48
	cmp r0, #0
	bne .L_02001238_4
	movs r0, #30
	bl 0x0200ae30
	movs r1, #3
	movs r0, #10
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r0, #10
	movs r1, #0
	bl 0x0200aee8
	ldr r3, [pc, #16]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001238_5
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000101
	.4byte 0x00014ccc
	.4byte 0x0000a666
	.4byte 0x00000107
.L_02001238_4:
	movs r0, #30
	bl 0x0200ae30
	movs r1, #4
	movs r0, #10
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	ldr r3, [pc, #620]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #10
	movs r1, #0
	bl 0x0200aee8
.L_02001238_5:
	movs r0, #10
	bl 0x0200ae30
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200aef0
	movs r0, #35
	bl 0x0200ae30
	movs r1, #3
	movs r0, #14
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl 0x0200aef0
	movs r0, #40
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r2, #50
	movs r0, #12
	movs r1, #13
	bl 0x0200aec0
	movs r0, #12
	movs r1, #3
	bl 0x0200ae90
	movs r1, #3
	movs r0, #13
	bl 0x0200ae98
.L_0200190c:
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #13
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r0, #12
	movs r1, #3
	bl 0x0200ae90
	movs r1, #3
	movs r0, #13
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ae58
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ae58
	movs r0, #12
	movs r1, #32
	movs r2, #0
	bl 0x0200af78
	movs r0, #13
	movs r1, #32
	movs r2, #0
	bl 0x0200af80
	movs r0, #12
	movs r1, #0
	movs r2, #16
	bl 0x0200af78
	movs r0, #13
	movs r1, #16
	movs r2, #0
	bl 0x0200af80
	movs r1, #172
	movs r2, #156
	movs r0, #13
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200ae68
	movs r1, #172
	movs r2, #168
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200ae70
	movs r0, #13
	movs r1, #1
	bl 0x0200ae90
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #13
	bl 0x0200aef0
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #14
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ae58
	movs r1, #164
	movs r2, #156
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200ae70
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ae58
	movs r1, #164
	movs r2, #164
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200ae70
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r0, #1
	ldr r1, [pc, #188]
	ldr r2, [pc, #188]
	bl 0x0200ae58
	movs r0, #2
	ldr r1, [pc, #176]
	ldr r2, [pc, #180]
	bl 0x0200ae58
	movs r0, #3
	ldr r1, [pc, #168]
	ldr r2, [pc, #168]
	bl 0x0200ae58
	movs r0, #1
	movs r1, #2
	bl 0x0200ae90
	movs r0, #0
	bl 0x0200ae50
	cmp r0, #0
	beq .L_0200190c_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200ae60
.L_0200190c_0:
	movs r0, #1
	bl 0x0200ae80
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200ae88
	movs r0, #2
	movs r1, #2
	bl 0x0200ae90
	movs r0, #0
	bl 0x0200ae50
	cmp r0, #0
	beq .L_0200190c_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200ae60
.L_0200190c_1:
	movs r0, #2
	bl 0x0200ae80
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200ae88
	movs r0, #3
	movs r1, #2
	bl 0x0200ae90
	movs r0, #0
	bl 0x0200ae50
	cmp r0, #0
	beq .L_0200190c_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200ae60
.L_0200190c_2:
	movs r0, #3
	bl 0x0200ae80
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x0200ae88
	movs r0, #10
	bl 0x0200ae30
	bl 0x0200ae40
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x00013333
	.4byte 0x00009999
	.global Func_02001b10
	.thumb_func
Func_02001b10:
	push {lr}
	ldr r0, [pc, #1016]
	bl 0x0200ae20
	bl 0x0200ae38
	bl 0x0200af68
	ldr r0, [pc, #1004]
	bl 0x0200aed8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ae58
	movs r1, #148
	movs r2, #156
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200ae70
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200aef0
	movs r0, #10
	bl 0x0200ae30
	movs r0, #1
	movs r1, #0
	movs r2, #16
	movs r3, #0
	bl 0x0200af70
	movs r1, #16
	movs r2, #8
	movs r0, #2
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl 0x0200af70
	movs r1, #16
	movs r2, #24
	movs r3, #0
	negs r1, r1
	movs r0, #3
	bl 0x0200af70
	movs r0, #3
	bl 0x0200ae80
	movs r0, #20
	bl 0x0200ae30
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200af18
	movs r0, #140
	movs r1, #1
	movs r2, #164
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x0200af20
	bl 0x0200af28
	movs r0, #10
	bl 0x0200ae30
	movs r0, #10
	bl 0x0200ae30
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl 0x0200aef0
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	movs r2, #40
.L_02001bcc:
	movs r0, #10
	lsls r1, r1, #1
	bl 0x0200af00
	movs r0, #10
	movs r1, #0
	bl 0x0200aee8
.L_02001bdc:
	movs r0, #10
	movs r1, #4
	movs r2, #13
	bl 0x0200aea0
	movs r1, #4
	movs r2, #30
	movs r0, #10
	bl 0x0200aea0
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200af00
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
.L_02001c0a:
	bl 0x0200af00
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200af00
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #14
	bl 0x0200af00
	movs r0, #10
	bl 0x0200ae30
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #13
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #2
	movs r0, #14
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	movs r1, #129
	movs r2, #40
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200af00
	movs r1, #0
	movs r0, #13
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	movs r0, #12
	ldr r1, [pc, #632]
	movs r2, #50
	bl 0x0200af00
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl 0x0200aef0
	movs r0, #25
	bl 0x0200ae30
	movs r1, #0
	movs r0, #12
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #13
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #3
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #3
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #2
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #2
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #129
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200af00
	movs r1, #0
	movs r0, #1
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	movs r1, #4
	movs r0, #10
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #14
	bl 0x0200aeb0
	movs r0, #25
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #10
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ae58
	movs r2, #40
	movs r0, #10
	movs r1, #0
	negs r2, r2
	bl 0x0200af80
	movs r2, #0
	movs r1, #0
	movs r0, #10
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #3
	movs r0, #14
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200aef0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r0, #12
	movs r1, #3
	bl 0x0200ae90
	movs r1, #3
	movs r0, #13
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #3
	movs r0, #10
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r0, #10
	movs r1, #1
	bl 0x0200af10
	movs r2, #32
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl 0x0200af80
	bl 0x0200a548
	ldr r0, [pc, #200]
	bl 0x0200ae28
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl 0x0200af20
	bl 0x0200ae38
	movs r1, #129
	movs r2, #40
	movs r0, #10
	lsls r1, r1, #1
	bl 0x0200af00
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #3
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200af00
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200af00
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #0
	b .L_02001c0a_0
	.2byte 0x0989
	.2byte 0x0000
	.2byte 0x272f
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x00000301
.L_02001c0a_0:
	bl 0x0200af00
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #70
	bl 0x0200af00
	movs r0, #140
	movs r1, #1
	movs r2, #156
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x0200af20
	bl 0x0200af28
	movs r0, #10
	bl 0x0200ae30
	movs r1, #129
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200af00
	movs r1, #0
	movs r0, #12
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #4
	movs r0, #13
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #13
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #2
	bl 0x0200aeb0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #2
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #2
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #3
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200af00
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #0
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #3
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #4
	movs r0, #1
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #1
	bl 0x0200aee8
	movs r0, #20
	bl 0x0200ae30
	movs r0, #14
	ldr r1, [pc, #472]
	movs r2, #50
	bl 0x0200af00
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #5
	movs r2, #0
	bl 0x0200aef0
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r0, #0
	movs r1, #0
	bl 0x0200ae48
	cmp r0, #0
	bne .L_02001c0a_1
	movs r0, #30
	bl 0x0200ae30
	movs r1, #2
	movs r0, #14
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r0, #14
	movs r1, #0
	bl 0x0200aee8
	ldr r3, [pc, #332]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001c0a_2
.L_02001c0a_1:
	movs r0, #30
	bl 0x0200ae30
	movs r1, #2
	movs r0, #14
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	ldr r3, [pc, #292]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #14
	movs r1, #0
	bl 0x0200aee8
.L_02001c0a_2:
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #10
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r0, #10
	movs r1, #0
	movs r2, #16
	bl 0x0200af80
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #10
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #4
	movs r0, #14
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #3
	movs r0, #10
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #2
	movs r0, #14
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee0
	movs r0, #40
	bl 0x0200ae30
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r0, #10
	ldr r1, [pc, #88]
	movs r2, #60
	bl 0x0200af00
	movs r0, #0
	movs r1, #0
	bl 0x0200ae48
	cmp r0, #0
	bne .L_02001c0a_3
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #10
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #3
	movs r0, #10
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r0, #10
	movs r1, #0
	bl 0x0200aee8
	ldr r3, [pc, #20]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001c0a_4
	.4byte 0x00000103
	.4byte 0x03001ebc
	.4byte 0x00000101
.L_02001c0a_3:
	movs r0, #30
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #10
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #4
	movs r0, #10
	bl 0x0200ae98
	movs r0, #20
	bl 0x0200ae30
	ldr r3, [pc, #568]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #10
	movs r1, #0
	bl 0x0200aee8
.L_02001c0a_4:
	movs r0, #10
	bl 0x0200ae30
	movs r1, #129
	movs r2, #50
	movs r0, #14
	lsls r1, r1, #1
	bl 0x0200af00
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #15
	bl 0x0200ae30
	movs r1, #3
	movs r0, #10
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r0, #20
	bl 0x0200ae30
	movs r1, #2
	movs r0, #14
	bl 0x0200aeb0
	movs r0, #40
	bl 0x0200ae30
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl 0x0200aef0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #3
	movs r0, #14
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r1, #0
	movs r0, #14
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl 0x0200aef0
	movs r0, #25
	bl 0x0200ae30
	movs r1, #2
	movs r0, #10
	bl 0x0200aeb0
	movs r0, #20
	bl 0x0200ae30
	movs r1, #156
	movs r2, #156
	movs r0, #10
	lsls r1, r1, #1
.L_020022e4:
	lsls r2, r2, #1
	bl 0x0200ae70
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200aef0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200aef0
	movs r0, #25
	bl 0x0200ae30
	movs r1, #0
	movs r0, #10
	bl 0x0200aee8
	movs r0, #10
	bl 0x0200ae30
	movs r0, #0
	movs r1, #1
	movs r2, #0
	bl 0x0200aec0
	movs r2, #0
	movs r1, #2
	movs r0, #3
	bl 0x0200aec0
	movs r0, #30
	bl 0x0200ae30
	movs r0, #0
	movs r1, #3
	bl 0x0200ae90
	movs r0, #1
	movs r1, #3
	bl 0x0200ae90
	movs r0, #3
	movs r1, #3
	bl 0x0200ae90
	movs r1, #3
	movs r0, #2
	bl 0x0200ae98
	movs r0, #30
	bl 0x0200ae30
	movs r0, #1
	ldr r1, [pc, #268]
	ldr r2, [pc, #268]
	bl 0x0200ae58
	movs r0, #2
	ldr r1, [pc, #256]
	ldr r2, [pc, #260]
	bl 0x0200ae58
	movs r0, #3
	ldr r1, [pc, #248]
	ldr r2, [pc, #248]
	bl 0x0200ae58
	movs r0, #1
	movs r1, #2
	bl 0x0200ae90
	movs r0, #0
	bl 0x0200ae50
	cmp r0, #0
	beq .L_020022e4_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200ae60
.L_020022e4_0:
	movs r0, #1
	bl 0x0200ae80
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200ae88
	movs r0, #2
	movs r1, #2
	bl 0x0200ae90
	movs r0, #0
	bl 0x0200ae50
	cmp r0, #0
	beq .L_020022e4_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200ae60
.L_020022e4_1:
	movs r0, #2
	bl 0x0200ae80
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200ae88
	movs r0, #3
	movs r1, #2
	bl 0x0200ae90
	movs r0, #0
	bl 0x0200ae50
	cmp r0, #0
	beq .L_020022e4_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200ae60
.L_020022e4_2:
	movs r0, #3
	bl 0x0200ae80
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x0200ae88
	movs r0, #10
	bl 0x0200ae30
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200aef0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #13
	bl 0x0200aef0
	movs r0, #30
	bl 0x0200ae30
	bl 0x0200ae40
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x00013333
	.4byte 0x00009999
	.global Func_0200246c
	.thumb_func
Func_0200246c:
	push {lr}
	ldr r0, [pc, #60]
	sub sp, #8
	bl 0x0200ae18
	cmp r0, #0
	bne .L_0200246c_0
	movs r3, #17
	movs r2, #78
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #78
	movs r2, #1
	movs r3, #2
	bl 0x0200ade0
	b .L_0200246c_1
.L_0200246c_0:
	movs r3, #17
	movs r2, #78
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #34
	movs r1, #78
	movs r2, #1
	movs r3, #2
	bl 0x0200ade0
.L_0200246c_1:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000985
	.global Func_020024b0
	.thumb_func
Func_020024b0:
	push {lr}
	sub sp, #8
	movs r3, #17
	movs r2, #78
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #78
	movs r2, #1
	movs r3, #2
	bl 0x0200ade0
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020024d0
	.thumb_func
Func_020024d0:
	push {lr}
	ldr r0, [pc, #108]
	sub sp, #8
	bl 0x0200ae18
	cmp r0, #0
	bne .L_020024d0_0
	ldr r0, [pc, #96]
	bl 0x0200ae20
	movs r0, #157
	bl 0x0200af90
	bl 0x0200ae38
	movs r1, #140
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #240
	bl 0x0200ae60
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #240
	movs r0, #9
	bl 0x0200ae60
	movs r0, #8
	bl 0x0200ae80
	movs r0, #9
	bl 0x0200ae80
	movs r3, #17
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #81
	movs r1, #14
	movs r2, #4
	movs r3, #1
	bl 0x0200add8
	bl 0x0200ae40
	ldr r0, [pc, #24]
	bl 0x0200ae18
	cmp r0, #0
	bne .L_020024d0_0
	bl 0x02009b10
.L_020024d0_0:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000985
	.4byte 0x00000989
	.global Func_02002548
	.thumb_func
Func_02002548:
	push {lr}
	ldr r0, [pc, #116]
	sub sp, #8
	bl 0x0200ae18
	cmp r0, #0
	beq .L_02002548_0
	ldr r0, [pc, #104]
	bl 0x0200ae28
	movs r0, #157
	bl 0x0200af90
	bl 0x0200ae38
	movs r1, #148
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #240
	bl 0x0200ae60
	movs r1, #156
	lsls r1, r1, #1
	movs r2, #240
	movs r0, #9
	bl 0x0200ae60
	movs r0, #8
	bl 0x0200ae80
	movs r0, #9
	bl 0x0200ae80
	movs r3, #17
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #14
	movs r2, #4
	movs r3, #1
	bl 0x0200add8
	bl 0x0200ae40
	ldr r0, [pc, #32]
	bl 0x0200ae18
	cmp r0, #0
	beq .L_02002548_1
	ldr r0, [pc, #20]
	bl 0x0200ae28
	b .L_02002548_0
.L_02002548_1:
	ldr r0, [pc, #12]
	bl 0x0200ae20
.L_02002548_0:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000985
	.4byte 0x00000301
	.global Func_020025c8
	.thumb_func
Func_020025c8:
	push {r5, r6, lr}
	ldr r3, [pc, #128]
	sub sp, #8
	ldr r5, [r3]
	bl 0x0200ae38
	ldr r2, [pc, #120]
	adds r5, r5, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_020025c8_0
	ldr r0, [pc, #112]
	bl 0x0200ae18
	cmp r0, #0
	bne .L_020025c8_1
	movs r1, #1
	ldr r0, [pc, #104]
	bl 0x0200ae10
	movs r0, #155
	bl 0x0200af90
	movs r5, #17
	movs r1, #78
	movs r2, #1
	movs r3, #2
	movs r6, #78
	movs r0, #35
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ade0
	movs r0, #10
	bl 0x0200ae30
	movs r0, #34
	movs r1, #78
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ade0
	movs r0, #10
	bl 0x0200ae30
	bl 0x0200a4d0
	b .L_020025c8_1
.L_020025c8_0:
	ldr r0, [pc, #44]
	bl 0x0200aed8
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl 0x0200aee8
.L_020025c8_1:
	bl 0x0200ae40
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000cb8
	.4byte 0x00000985
	.4byte 0x00001528
	.4byte 0x00002756
	.global Func_02002660
	.thumb_func
Func_02002660:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	ldr r2, [pc, #56]
	lsrs r3, r3, #12
	lsls r3, r3, #2
	ldr r0, [r2, r3]
	ldr r3, [pc, #52]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	sub sp, #12
	ands r3, r0
	lsls r0, r0, #16
	mov r6, sp
	adds r2, r2, r0
	adds r1, r1, r3
	str r1, [r6]
	str r2, [r6, #8]
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	bl 0x0200add0
	adds r1, r5, #0
	str r0, [r6, #4]
	adds r0, r6, #0
	bl 0x0200806c
	sub sp, #-12
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x0200afd4
	.4byte 0xffff0000
	.global Func_020026a8
	.thumb_func
Func_020026a8:
	ldr r3, [pc, #8]
	ldr r3, [r3]
	movs r2, #1
	adds r3, #52
	strb r2, [r3]
	bx lr
	.4byte 0x03001f30
	.global Func_020026b8
	.thumb_func
Func_020026b8:
	push {lr}
	movs r0, #12
	sub sp, #8
	bl 0x0200ae50
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	cmp r4, #20
	bne .L_020026b8_0
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r1, #12
	bne .L_020026b8_0
	adds r2, r0, #0
	movs r3, #2
	adds r2, #85
	strb r3, [r2]
	movs r2, #192
	lsls r2, r2, #14
	str r2, [r0, #20]
	adds r2, r0, #0
	adds r2, #35
	strb r3, [r2]
	movs r0, #38
	str r1, [sp, #4]
	movs r2, #1
	movs r1, #12
	movs r3, #1
	str r4, [sp, #0]
	bl 0x0200add8
.L_020026b8_0:
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_020026fc
	.thumb_func
Func_020026fc:
	push {r5, lr}
	movs r0, #0
	bl 0x0200ae50
	adds r5, r0, #0
	bl 0x0200a660
	cmp r0, #0
	beq .L_020026fc_0
	ldr r3, [r0, #12]
	ldr r0, [r5, #12]
	subs r2, r3, r0
	cmp r2, #0
	blt .L_020026fc_1
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	bge .L_020026fc_0
	b .L_020026fc_2
.L_020026fc_1:
	movs r2, #128
	subs r3, r0, r3
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_020026fc_0
.L_020026fc_2:
	bl 0x020080c4
.L_020026fc_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002738
	.thumb_func
Func_02002738:
	push {r5, lr}
	movs r0, #0
	bl 0x0200ae50
	adds r5, r0, #0
	bl 0x0200a660
	ldrh r3, [r5, #6]
	movs r1, #128
	lsls r1, r1, #6
	adds r5, r3, r1
	movs r4, #249
	ldr r3, [pc, #72]
	lsls r4, r4, #1
	adds r3, r3, r4
	movs r1, #192
	ldrb r3, [r3]
	adds r2, r0, #0
	lsls r1, r1, #8
	movs r0, #1
	ands r5, r1
	negs r0, r0
	cmp r3, #1
	beq .L_02002738_0
	cmp r2, #0
	bne .L_02002738_1
.L_02002738_0:
	cmp r5, r1
	bne .L_02002738_2
	bl 0x0200af48
.L_02002738_2:
	movs r1, #128
	lsls r1, r1, #7
	cmp r5, r1
	bne .L_02002738_1
	bl 0x0200af40
.L_02002738_1:
	cmp r0, #0
	beq .L_02002738_3
	ldr r3, [pc, #20]
	movs r2, #249
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_02002738_3
	bl 0x0200a6fc
.L_02002738_3:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.global Func_020027a0
	.thumb_func
Func_020027a0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020027a0_0
	ldr r0, [pc, #36]
	b .L_020027a0_1
.L_020027a0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020027a0_2
	ldr r0, [pc, #36]
	b .L_020027a0_1
.L_020027a0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020027a0_3
	ldr r0, [pc, #32]
	b .L_020027a0_1
.L_020027a0_3:
	ldr r0, [pc, #32]
.L_020027a0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b0
	.4byte 0x0200b91c
	.4byte 0x000000af
	.4byte 0x0200b9e8
	.4byte 0x000000ae
	.4byte 0x0200bac0
	.4byte 0x0200bc28
	.global Func_020027f4
	.thumb_func
Func_020027f4:
	push {lr}
	sub sp, #8
	movs r3, #18
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #7
	movs r2, #1
	movs r3, #2
	movs r0, #82
	bl 0x0200add8
	movs r0, #1
	bl 0x0200ad80
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200ae20
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02002820
	.thumb_func
Func_02002820:
	push {r5, r6, lr}
	movs r0, #8
	sub sp, #8
	bl 0x0200ae50
	movs r1, #1
	adds r6, r0, #0
	movs r0, #8
	bl 0x0200aef8
	movs r0, #9
	movs r1, #1
	bl 0x0200aef8
	movs r3, #5
	str r3, [sp, #0]
	movs r5, #19
	movs r0, #69
	movs r1, #19
	movs r2, #3
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200add8
	movs r3, #17
	str r3, [sp, #0]
	movs r0, #69
	movs r1, #19
	movs r2, #3
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200add8
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r1, #3
	movs r2, #1
	movs r3, #1
	movs r0, #3
	bl 0x0200add8
	movs r0, #9
	bl 0x0200ae50
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #3
	movs r2, #1
	movs r3, #1
	bl 0x0200add8
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #1
	sub	sp, #8
	bl 0x0200ad80
	ldr	r3, [pc, #84]
	movs	r2, #224
	ldr	r1, [r3, #0]
	movs	r3, #129
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	str	r3, [r1, r2]
	ldr	r6, [pc, #72]
	ldr	r3, [pc, #76]
	ldrsh	r1, [r6, r2]
	cmp	r1, r3
	beq.n	.L_020028dc
	adds	r2, #130
	adds	r3, r6, r2
	movs	r0, #144
	movs	r2, #1
	strh	r2, [r3, #0]
	lsls	r0, r0, #2
	ldr	r2, [pc, #60]
	adds	r3, r6, r0
	strh	r2, [r3, #0]
	mov	ip, r1
	b.n	.L_02002914
.L_020028dc:
	movs	r0, #12
	bl 0x0200ae50
	adds	r1, r0, #0
	ldr	r3, [r1, #8]
	asrs	r2, r3, #20
	cmp	r2, #20
	beq.n	.L_020028ee
	b.n	.L_02002d3c
.L_020028ee:
	ldr	r3, [r1, #16]
	asrs	r0, r3, #20
	cmp	r0, #12
	beq.n	.L_020028f8
	b.n	.L_02002d3c
.L_020028f8:
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #12
	movs	r0, #38
	b.n	.L_02002af2
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x000000b1
	.2byte 0x00b0
	.2byte 0x0000
.L_02002914:
	cmp	ip, r2
	beq.n	.L_0200291a
	b.n	.L_02002a7a
.L_0200291a:
	movs	r0, #8
	movs	r1, #6
	bl 0x0200aec8
	movs	r0, #9
	movs	r1, #6
	bl 0x0200aec8
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #5
	bne.n	.L_02002950
	ldr	r0, [pc, #180]
	bl 0x0200ae18
	cmp	r0, #0
	bne.n	.L_02002950
	movs	r1, #156
	movs	r2, #164
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae88
.L_02002950:
	bl 0x0200a820
	movs	r0, #9
	bl 0x0200ae50
	adds	r5, r0, #0
	movs	r0, #9
	bl 0x0200ae50
	ldr	r3, [r0, #20]
	movs	r0, #192
	str	r3, [r5, #12]
	lsls	r0, r0, #2
	bl 0x0200ae18
	cmp	r0, #0
	beq.n	.L_0200298a
	movs	r0, #10
	movs	r1, #4
	bl 0x0200ae90
	movs	r0, #10
	bl 0x0200ae50
	movs	r3, #254
	adds	r0, #89
	strb	r3, [r0, #0]
	bl 0x0200a7f4
.L_0200298a:
	movs	r0, #11
	bl 0x0200ae50
	adds	r1, r0, #0
	adds	r2, r1, #0
	movs	r3, #0
	adds	r2, #89
	strb	r3, [r2, #0]
	subs	r2, #54
	strb	r3, [r2, #0]
	adds	r2, #59
	strh	r3, [r2, #0]
	ldr	r2, [r1, #80]
	ldrb	r3, [r2, #9]
	movs	r6, #12
	orrs	r3, r6
	strb	r3, [r2, #9]
	ldr	r3, [r1, #80]
	ldr	r5, [pc, #60]
	adds	r3, #38
	strb	r5, [r3, #0]
	movs	r3, #192
	ldr	r2, [r1, #80]
	lsls	r3, r3, #8
	strh	r3, [r2, #30]
	movs	r1, #0
	movs	r0, #11
	bl 0x0200ae90
	movs	r0, #12
	bl 0x0200ae50
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #89
	strb	r5, [r3, #0]
	adds	r2, r1, #0
	subs	r3, #54
	strb	r5, [r3, #0]
	adds	r2, #94
	movs	r3, #30
	strh	r3, [r2, #0]
	ldr	r2, [r1, #80]
	ldrb	r3, [r2, #9]
	orrs	r3, r6
	strb	r3, [r2, #9]
	ldr	r3, [r1, #80]
	adds	r3, #38
	b.n	.L_020029f4
	.4byte 0x00000000
	.2byte 0x0109
	.2byte 0x0000
.L_020029f4:
	strb	r5, [r3, #0]
	movs	r3, #128
	ldr	r2, [r1, #80]
	lsls	r3, r3, #7
	strh	r3, [r2, #30]
	movs	r1, #0
	movs	r0, #12
	bl 0x0200ae90
	movs	r0, #13
	bl 0x0200ae50
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #89
	strb	r5, [r3, #0]
	adds	r2, r1, #0
	subs	r3, #54
	strb	r5, [r3, #0]
	adds	r2, #94
	movs	r3, #60
	strh	r3, [r2, #0]
	ldr	r2, [r1, #80]
	ldrb	r3, [r2, #9]
	orrs	r3, r6
	strb	r3, [r2, #9]
	ldr	r3, [r1, #80]
	adds	r3, #38
	strb	r5, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #8
	ldr	r3, [r1, #80]
	mov	r8, r2
	mov	r0, r8
	strh	r0, [r3, #30]
	movs	r1, #0
	movs	r0, #13
	bl 0x0200ae90
	movs	r0, #14
	bl 0x0200ae50
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #89
	strb	r5, [r3, #0]
	adds	r2, r1, #0
	subs	r3, #54
	strb	r5, [r3, #0]
	adds	r2, #94
	movs	r3, #90
	strh	r3, [r2, #0]
	ldr	r2, [r1, #80]
	ldrb	r3, [r2, #9]
	orrs	r3, r6
	strb	r3, [r2, #9]
	ldr	r3, [r1, #80]
	adds	r3, #38
	strb	r5, [r3, #0]
	ldr	r3, [r1, #80]
	mov	r2, r8
	strh	r2, [r3, #30]
	movs	r0, #14
	movs	r1, #0
	bl 0x0200ae90
	b.n	.L_02002d3c
.L_02002a7a:
	ldr	r3, [pc, #720]
	cmp	ip, r3
	bne.n	.L_02002b7e
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r6, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #10
	cmp	r3, #7
	bls.n	.L_02002a92
	b.n	.L_02002d3c
.L_02002a92:
	ldr	r2, [pc, #700]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200aabc
	.4byte 0x0200aac4
	.4byte 0x0200ad24
	.4byte 0x0200ad24
	.4byte 0x0200aafc
	.4byte 0x0200ad24
	.4byte 0x0200ab28
	.4byte 0x0200ab54
	.4byte 0x01002098
	.4byte 0xf9aef000
	.4byte 0x01002098
	.4byte 0xf9a6f000
	.4byte 0xd1002800
	.4byte 0x2301e134
	.4byte 0x93002203
	.4byte 0x20789201
	.4byte 0x226d2107
	.4byte 0xf0002307
	.4byte 0x232df971
	.4byte 0x93002209
	.4byte 0x202d9201
	.2byte 0x2108
.L_02002af2:
	movs	r2, #1
	movs	r3, #1
	bl 0x0200add8
	b.n	.L_02002d3c
	.4byte 0x229120dc
	.4byte 0x21000452
	.4byte 0x044023df
	.4byte 0xff74f7fd
	.4byte 0x220d231b
	.4byte 0x92019300
	.4byte 0x210d2016
	.4byte 0x23012201
	.4byte 0xf95cf000
	.4byte 0xf7fe200e
	.4byte 0xe109f9c3
	.4byte 0x229120e0
	.4byte 0x21000452
	.4byte 0x044023df
	.4byte 0xff5ef7fd
	.4byte 0x220a231c
	.4byte 0x92019300
	.4byte 0x210c2016
	.4byte 0x23012201
	.4byte 0xf946f000
	.4byte 0xf7fe2010
	.4byte 0xe0f3f9ad
	.4byte 0x4a7f20e8
	.4byte 0x23df2100
	.4byte 0xf7fd0400
	.4byte 0x230eff49
	.4byte 0x93002221
	.4byte 0x20169201
	.4byte 0x2201210c
	.4byte 0xf0002301
	.4byte 0x2011f931
	.4byte 0xf998f7fe
	.2byte 0xe0de
.L_02002b7e:
	ldr	r3, [pc, #472]
	cmp	r1, r3
	beq.n	.L_02002b86
	b.n	.L_02002d34
.L_02002b86:
	movs	r0, #8
	bl 0x0200ae50
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200ae50
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #128
	ands	r5, r3
	movs	r2, #128
	strb	r5, [r0, #0]
	lsls	r1, r1, #9
	movs	r0, #8
	lsls	r2, r2, #8
	bl 0x0200ae58
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200ae58
	ldr	r0, [pc, #408]
	bl 0x0200ae18
	cmp	r0, #0
	bne.n	.L_02002be8
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r6, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_02002be2
	ldr	r0, [pc, #388]
	bl 0x0200ae20
	b.n	.L_02002be8
.L_02002be2:
	ldr	r0, [pc, #380]
	bl 0x0200ae28
.L_02002be8:
	ldr	r0, [pc, #376]
	bl 0x0200ae18
	cmp	r0, #0
	bne.n	.L_02002c6a
	movs	r0, #10
	ldr	r1, [pc, #368]
	ldr	r2, [pc, #368]
	bl 0x0200ae88
	movs	r1, #140
	movs	r2, #148
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae88
	movs	r1, #156
	movs	r2, #248
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ae88
	movs	r1, #148
	movs	r2, #248
	movs	r0, #13
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ae88
	movs	r1, #160
	movs	r2, #148
	movs	r0, #14
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae88
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #192
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r0, #5
	bl 0x0200ae30
	b.n	.L_02002cc4
.L_02002c6a:
	ldr	r0, [pc, #256]
	bl 0x0200ae18
	cmp	r0, #0
	beq.n	.L_02002cc4
	movs	r1, #156
	movs	r2, #156
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae88
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #176
	movs	r0, #13
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r1, #176
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r0, #5
	bl 0x0200ae30
.L_02002cc4:
	ldr	r0, [pc, #168]
	bl 0x0200ae18
	cmp	r0, #0
	beq.n	.L_02002d14
	movs	r1, #140
	movs	r2, #240
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ae88
	movs	r1, #164
	movs	r2, #240
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ae88
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aef0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aef0
	movs	r3, #17
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #81
	movs	r1, #14
	movs	r2, #4
	movs	r3, #1
	bl 0x0200add8
.L_02002d14:
	ldr	r3, [pc, #92]
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_02002d3c
	ldr	r0, [pc, #52]
	bl 0x0200ae18
	cmp	r0, #0
	bne.n	.L_02002d3c
	bl 0x02008d4c
	b.n	.L_02002d3c
.L_02002d34:
	movs	r0, #12
	movs	r1, #2
	bl 0x0200ae90
.L_02002d3c:
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x000000af
	.4byte 0x0200aa9c
	.4byte 0x02520000
	.4byte 0x000000ae
	.4byte 0x00000109
	.4byte 0x00000301
	.4byte 0x00000988
	.4byte 0xffc00000
	.4byte 0x00000989
	.4byte 0x00000985
	.4byte 0x02000240
	.include "games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IMPORT.INC"
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
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
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200b08c
	.4byte 0x0200b0c4
	.4byte 0x0200b0fc
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x40000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc00001a8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0x400000c8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0003
	.4byte 0x000000d8
	.4byte 0x400000b8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0004
	.4byte 0x00000068
	.4byte 0x40000138
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0005
	.4byte 0x00000128
	.4byte 0x40000138
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0006
	.4byte 0x00000138
	.4byte 0x40000058
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x40000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000e8
	.4byte 0x400001e8
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0x400001e8
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x40000288
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0005
	.4byte 0x000001c8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0006
	.4byte 0x000001f8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0007
	.4byte 0x000001d8
	.4byte 0xc0000120
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0008
	.4byte 0x00000078
	.4byte 0x40000078
	.4byte 0x00300000
	.4byte 0x01200040
	.4byte 0x00000140
	.4byte 0xffff0009
	.4byte 0x000000b8
	.4byte 0xc0000120
	.4byte 0x00300000
	.4byte 0x01200040
	.4byte 0x00000140
	.4byte 0xffff000a
	.4byte 0x000002d8
	.4byte 0x400000a8
	.4byte 0x02700000
	.4byte 0x03600060
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x00000308
	.4byte 0x400000b8
	.4byte 0x02700000
	.4byte 0x03600060
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000001e8
	.4byte 0x400000b8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff000d
	.4byte 0x000000f8
	.4byte 0x40000248
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff000e
	.4byte 0x000001b8
	.4byte 0x400000d8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff000f
	.4byte 0x000000d8
	.4byte 0x40000248
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0010
	.4byte 0x000001c8
	.4byte 0x400000a8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0011
	.4byte 0x000000e8
	.4byte 0x40000218
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000078
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0xc0000170
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0xffff0003
	.4byte 0x000000a8
	.4byte 0x40000158
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000148
	.4byte 0xc00000f0
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0xffff0001
	.4byte 0x00000070
	.4byte 0xc0000168
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x40000020
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffec0060
	.4byte 0x00700140
	.4byte 0x0150fffc
	.4byte 0x0004ffff
	.4byte 0xffec0120
	.4byte 0x01300140
	.4byte 0x0150fffc
	.4byte 0x0005ffff
	.4byte 0xffec0130
	.4byte 0x01400060
	.4byte 0x0070fffc
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000b0
	.4byte 0x001020b1
	.4byte 0x002070af
	.4byte 0x003090af
	.4byte 0x004010ad
	.4byte 0x005030ad
	.4byte 0x006020ad
	.4byte 0x000000af
	.4byte 0x001040af
	.4byte 0x002030b0
	.4byte 0x003080af
	.4byte 0x004010af
	.4byte 0x005020af
	.4byte 0x006020b0
	.4byte 0x007050af
	.4byte 0x008060af
	.4byte 0x0090b0af
	.4byte 0x00a030af
	.4byte 0x00b0c0af
	.4byte 0x00c0d0af
	.4byte 0x00d0e0af
	.4byte 0x00e0f0af
	.4byte 0x00f100af
	.4byte 0x010110af
	.4byte 0x011030ae
	.4byte 0x012020ae
	.4byte 0x000000ae
	.4byte 0x001010ac
	.4byte 0x0020a0af
	.4byte 0x000000b1
	.4byte 0x0011f002
	.4byte 0x002010b0
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00fd
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff00fd
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x0200b140
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0074
	.4byte 0x0200b140
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x0200b140
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0072
	.4byte 0x0200b140
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0072005d
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0128
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00028000
	.4byte 0xffff0128
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00020000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00018000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0xffff0046
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x020090f5
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x0200910d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000026fa
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000026fb
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000026fc
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000026fd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000026fe
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000026ff
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002700
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002701
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02009031
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x02009031
	.4byte 0x00000c15
	.4byte 0x0300000a
	.4byte 0x0200a7f5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000012
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte 0x02009185
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte 0x0200919d
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte 0x020091c5
	.4byte 0x00004602
	.4byte 0xffff001c
	.4byte 0x020091fd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008cc1
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02009215
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x0988000a
	.4byte 0x02009239
	.4byte 0x00000002
	.4byte 0x13010014
	.4byte 0x0200a549
	.4byte 0x00000002
	.4byte 0x03010015
	.4byte 0x0200a549
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte 0x0200a4d1
	.4byte 0x00000000
	.4byte 0x0989000a
	.4byte 0x00002725
	.4byte 0x00000000
	.4byte 0x0989000b
	.4byte 0x00002726
	.4byte 0x00000000
	.4byte 0x0989000c
	.4byte 0x00002727
	.4byte 0x00000000
	.4byte 0x0989000d
	.4byte 0x00002728
	.4byte 0x00000000
	.4byte 0x0989000e
	.4byte 0x00002729
	.4byte 0x00008d15
	.4byte 0x0989000a
	.4byte 0x0000272a
	.4byte 0x00008d15
	.4byte 0x0989000b
	.4byte 0x0000272b
	.4byte 0x00008d15
	.4byte 0x0989000c
	.4byte 0x0000272c
	.4byte 0x00008d15
	.4byte 0x0989000d
	.4byte 0x0000272d
	.4byte 0x00008d15
	.4byte 0x0989000e
	.4byte 0x0000272e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000274c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000274d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000274e
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000274f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002750
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002751
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002752
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002753
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002754
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002755
	.4byte 0x00000003
	.4byte 0xffff003c
	.4byte 0x0200a5c9
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200a46d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200a4b1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte 0x0200a6fd
	.4byte 0x00000202
	.4byte 0xffff0033
	.4byte 0x0200a739
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x0200a6b9
	.4byte 0x10009315
	.4byte 0xffff000c
	.4byte 0x0200a6a9
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte 0x0200a6b9
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000026b7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000026b8
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000026b9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000026ba
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000026c5
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000026c6
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000026c7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000026c8
	.4byte 0x00000413
	.4byte 0x0fbc0064
	.4byte 0x001000c4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
