.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02000f2c
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {lr}
	ldr r3, .L_02008064
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008068
	cmp r2, r3
	bne .L_02008058
	ldr r0, .L_0200806c
	b .L_02008062
.L_02008058:
	ldr r3, .L_02008070
	movs r0, #0
	cmp r2, r3
	bne .L_02008062
	ldr r0, .L_02008074
.L_02008062:
	pop {pc}
.L_02008064:
	.4byte gPartyState
.L_02008068:
	.4byte 0x000000ae
.L_0200806c:
	.4byte Data_02000f5c
.L_02008070:
	.4byte 0x000000af
.L_02008074:
	.4byte Data_02000f8c
	.section .text.x02008078,"ax",%progbits
	.global Func_02000078
	.thumb_func
Func_02000078:
	ldr r0, .L_0200807c
	bx lr
.L_0200807c:
	.4byte Data_02000fbc
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {lr}
	ldr r3, .L_020080b0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080b4
	cmp r2, r3
	bne .L_02008098
	ldr r0, .L_020080b8
	b .L_020080ae
.L_02008098:
	ldr r3, .L_020080bc
	cmp r2, r3
	bne .L_020080a2
	ldr r0, .L_020080c0
	b .L_020080ae
.L_020080a2:
	ldr r3, .L_020080c4
	cmp r2, r3
	bne .L_020080ac
	ldr r0, .L_020080c8
	b .L_020080ae
.L_020080ac:
	ldr r0, .L_020080cc
.L_020080ae:
	pop {pc}
.L_020080b0:
	.4byte gPartyState
.L_020080b4:
	.4byte 0x000000ae
.L_020080b8:
	.4byte Data_02001044
.L_020080bc:
	.4byte 0x000000af
.L_020080c0:
	.4byte Data_02001194
.L_020080c4:
	.4byte 0x000000b0
.L_020080c8:
	.4byte Data_020012cc
.L_020080cc:
	.4byte Data_0200102c
	.section .text.x020080d0,"ax",%progbits
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push {lr}
	movs r1, #192
	lsls r1, r1, #2
	movs r0, #17
	bl Func_02000bc0
	pop {pc}
	.2byte 0x0000
	.section .text.x020080e0,"ax",%progbits
	.global Func_020000e0
	.thumb_func
Func_020000e0:
	push {lr}
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #1
	movs r0, #18
	bl Func_02000bc0
	pop {pc}
	.section .text.x020080f0,"ax",%progbits
	.global Func_020000f0
	.thumb_func
Func_020000f0:
	push {lr}
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #2
	movs r0, #19
	bl Func_02000bc0
	pop {pc}
	.section .text.x02008100,"ax",%progbits
	.global Func_02000100
	.thumb_func
Func_02000100:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	bl Object_GetById
	movs r2, #85
	adds r5, r0, #0
	adds r2, r2, r5
	movs r3, #3
	strb r3, [r2]
	movs r6, #60
	mov r8, r2
.L_0200811a:
	cmp r6, #0
	beq .L_0200812c
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #40]
	subs r6, #1
	cmp r3, #0
	bne .L_0200811a
.L_0200812c:
	cmp r7, #0
	beq .L_02008136
	adds r0, r7, #0
	bl Func_02000f24
.L_02008136:
	movs r0, #10
	bl WaitFrames
	movs r3, #0
	mov r2, r8
	strb r3, [r2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008148,"ax",%progbits
	.global Func_02000148
	.thumb_func
Func_02000148:
	push {lr}
	sub sp, #8
	movs r3, #8
	movs r2, #91
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #90
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
	add sp, #8
	pop {pc}
	.section .text.x02008164,"ax",%progbits
	.global Func_02000164
	.thumb_func
Func_02000164:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	movs r3, #8
	movs r2, #91
	adds r6, r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #90
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
	cmp r5, #16
	bne .L_020081c2
	movs r0, #146
	lsls r0, r0, #4
	bl Func_02000e14
	cmp r0, #0
	bne .L_020081c2
	ldr r3, [r6, #8]
	asrs r3, r3, #19
	cmp r3, #11
	bgt .L_020081c2
	movs r1, #181
	movs r0, #16
	bl Func_02000100
	movs r0, #146
	lsls r0, r0, #4
	bl Func_02000e1c
	movs r3, #5
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #27
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
.L_020081c2:
	cmp r5, #19
	bne .L_02008254
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02000e14
	cmp r0, #0
	bne .L_02008254
	ldr r3, [r6, #8]
	asrs r3, r3, #19
	cmp r3, #77
	bne .L_020081f0
	movs r3, #37
	movs r2, #102
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #102
	movs r2, #1
	movs r3, #2
	bl Func_02000e44
.L_020081f0:
	ldr r3, [r6, #8]
	asrs r3, r3, #19
	cmp r3, #79
	bne .L_0200821c
	ldr r3, [r6, #12]
	asrs r3, r3, #19
	cmp r3, #12
	bne .L_0200821c
	movs r0, #19
	movs r1, #181
	bl Func_02000100
	movs r3, #39
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #46
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
.L_0200821c:
	ldr r3, [r6, #8]
	asrs r3, r3, #19
	cmp r3, #81
	bne .L_02008254
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02000e1c
	movs r3, #39
	movs r5, #46
	str r3, [sp, #0]
	movs r0, #41
	movs r1, #46
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02000e44
	movs r3, #40
	str r3, [sp, #0]
	movs r0, #27
	movs r1, #44
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02000e44
.L_02008254:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008258,"ax",%progbits
	.global Func_02000258
	.thumb_func
Func_02000258:
	push {lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #13
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #13
	movs r1, #18
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
	add sp, #8
	pop {pc}
	.section .text.x0200827c,"ax",%progbits
	.global Func_0200027c
	.thumb_func
Func_0200027c:
	push {lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #13
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #11
	movs r1, #14
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
	add sp, #8
	pop {pc}
	.section .text.x020082a0,"ax",%progbits
	.global Func_020002a0
	.thumb_func
Func_020002a0:
	push {lr}
	sub sp, #12
	movs r3, #110
	movs r2, #47
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #110
	movs r1, #64
	movs r2, #14
	movs r3, #12
	bl Func_02000f04
	add sp, #12
	pop {pc}
	.section .text.x020082c0,"ax",%progbits
	.global Func_020002c0
	.thumb_func
Func_020002c0:
	push {r5, r6, lr}
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020082f6
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #116
	bl Func_02000e14
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020082f6
	movs r2, #246
	movs r0, #64
	ldr r1, .L_020082f8
	lsls r2, r2, #16
	bl Func_02000e8c
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r5, #12]
.L_020082f6:
	pop {r5, r6, pc}
.L_020082f8:
	.4byte 0x02910000
	.section .text.x020082fc,"ax",%progbits
	.global Func_020002fc
	.thumb_func
Func_020002fc:
	push {lr}
	movs r0, #64
	bl Object_GetById
	cmp r0, #0
	beq .L_02008312
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02000e8c
.L_02008312:
	pop {pc}
	.section .text.x02008314,"ax",%progbits
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {r5, lr}
	movs r0, #64
	bl Object_GetById
	movs r1, #1
	adds r5, r0, #0
	ldr r0, .L_0200836c
	bl UiText_ShowPositionedMessageAndWait
	cmp r5, #0
	beq .L_0200833a
	ldr r3, [r5, #8]
	cmp r3, #0
	beq .L_0200833a
	movs r0, #130
	lsls r0, r0, #5
	bl Func_02000edc
	b .L_0200836a
.L_0200833a:
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #116
	bl Func_02000e14
	cmp r0, #0
	bne .L_02008362
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r0, .L_02008370
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #173
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	b .L_0200836a
.L_02008362:
	ldr r0, .L_02008374
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_0200836a:
	pop {r5, pc}
.L_0200836c:
	.4byte 0x000022f5
.L_02008370:
	.4byte 0x000023e9
.L_02008374:
	.4byte 0x000023ea
	.section .text.x02008378,"ax",%progbits
	.global Func_02000378
	.thumb_func
Func_02000378:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl Func_02000e14
	cmp r0, #0
	beq .L_020083bc
	bl Func_02000e74
	movs r0, #0
	bl Func_02000ef4
	ldr r0, .L_020083c0
	bl Func_02000ea4
	movs r1, #1
	movs r0, #20
	bl ObjectMotion_SetVariantCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #20
	bl Func_02000eb4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02000e1c
	bl Func_02000e7c
.L_020083bc:
	pop {pc}
	.2byte 0x0000
.L_020083c0:
	.4byte 0x00002316
	.section .text.x020083c4,"ax",%progbits
	.global Func_020003c4
	.thumb_func
Func_020003c4:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02000e14
	cmp r0, #0
	beq .L_020083e4
	ldr r0, .L_020083f4
	bl Func_02000ea4
	movs r0, #20
	movs r1, #0
	bl Func_02000eb4
	b .L_020083f2
.L_020083e4:
	ldr r0, .L_020083f8
	bl Func_02000ea4
	movs r0, #20
	movs r1, #0
	bl Func_02000eac
.L_020083f2:
	pop {pc}
.L_020083f4:
	.4byte 0x00002316
.L_020083f8:
	.4byte 0x00002315
	.section .text.x020083fc,"ax",%progbits
	.global Func_020003fc
	.thumb_func
Func_020003fc:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02000e14
	cmp r0, #0
	beq .L_0200841c
	ldr r0, .L_0200842c
	bl Func_02000ea4
	movs r0, #20
	movs r1, #0
	bl Func_02000eac
	b .L_0200842a
.L_0200841c:
	ldr r0, .L_02008430
	bl Func_02000ea4
	movs r0, #20
	movs r1, #0
	bl Func_02000eac
.L_0200842a:
	pop {pc}
.L_0200842c:
	.4byte 0x0000231a
.L_02008430:
	.4byte 0x00002319
	.section .text.x02008434,"ax",%progbits
	.global Func_02000434
	.thumb_func
Func_02000434:
	push {lr}
	ldr r3, .L_0200845c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008460
	cmp r2, r3
	bne .L_0200844c
	ldr r0, .L_02008464
	b .L_02008458
.L_0200844c:
	ldr r3, .L_02008468
	cmp r2, r3
	bne .L_02008456
	ldr r0, .L_0200846c
	b .L_02008458
.L_02008456:
	ldr r0, .L_02008470
.L_02008458:
	pop {pc}
	.2byte 0x0000
.L_0200845c:
	.4byte gPartyState
.L_02008460:
	.4byte 0x000000ae
.L_02008464:
	.4byte Data_020013c8
.L_02008468:
	.4byte 0x000000af
.L_0200846c:
	.4byte Data_020014b8
.L_02008470:
	.4byte Data_02001554
	.section .text.x02008474,"ax",%progbits
	.global Func_02000474
	.thumb_func
Func_02000474:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	bl Random16Far
	ldrh r6, [r7, #6]
	movs r1, #128
	lsls r1, r1, #10
	adds r5, r0, #0
	adds r0, r6, #0
	adds r5, r5, r1
	bl Math_Cosine
	ldr r2, .L_02008520
	adds r1, r0, #0
	mov r8, r2
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	mov r10, r0
	adds r0, r6, #0
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r7, #8]
	movs r1, #255
	add r3, r10
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	lsls r1, r1, #8
	adds r3, r3, r0
	str r3, [r7, #16]
	ldrh r3, [r7, #6]
	adds r1, #240
	adds r3, r3, r1
	strh r3, [r7, #6]
	adds r5, r7, #0
	adds r5, #102
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #0
	beq .L_020084e4
	subs r3, r2, #1
	strh r3, [r5]
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	strh r3, [r7, #6]
	b .L_020084fc
.L_020084e4:
	bl Random16Far
	lsls r0, r0, #5
	lsrs r0, r0, #16
	cmp r0, #0
	bne .L_020084fc
	bl Random16Far
	lsls r0, r0, #4
	lsrs r0, r0, #16
	adds r0, #8
	strh r0, [r5]
.L_020084fc:
	adds r2, r7, #0
	adds r2, #100
	ldrh r3, [r2]
	movs r1, #142
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #15
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_02008518
	ldr r1, .L_02008524
	adds r0, r7, #0
	bl Func_02000e2c
.L_02008518:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008520:
	.4byte IwramMulQ16
.L_02008524:
	.4byte Data_020015c0
	.section .text.x02008528,"ax",%progbits
	.global Func_02000528
	.thumb_func
Func_02000528:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r1, #0
	movs r2, #20
	sub sp, #12
	adds r7, r0, #0
	mov r8, r1
	mov r10, r2
.L_0200853e:
	ldr r3, [r7, #8]
	mov r5, sp
	str r3, [r5]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	mov r2, r8
	muls r2, r3
	ldr r3, [r7, #12]
	movs r1, #128
	adds r3, r3, r2
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r2, #192
	adds r1, r0, #0
	lsls r2, r2, #10
	lsls r0, r6, #2
	adds r0, r0, r6
	mov r9, r2
	add r0, r9
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	movs r0, #154
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	lsls r0, r0, #1
	bl Func_02000efc
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020085d0
	ldr r3, .L_020085cc
	mov r1, r10
	str r3, [r6, #108]
	adds r3, r6, #0
	adds r3, #100
	strh r1, [r3]
	movs r2, #0
	adds r3, #2
	strh r2, [r3]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	mov r2, r9
	str r3, [r6, #72]
	str r2, [r6, #40]
	bl Random16Far
	ldr r5, .L_020085c8
	adds r3, r6, #0
	adds r3, #35
	strh r0, [r6, #6]
	movs r1, #2
	strb r5, [r3]
	adds r0, r6, #0
	bl Func_02000e5c
	b .L_020085d0
	.2byte 0x0000
.L_020085c8:
	.4byte 0x00000000
.L_020085cc:
	.4byte Func_02000474
.L_020085d0:
	movs r1, #1
	movs r3, #2
	add r8, r1
	negs r3, r3
	mov r2, r8
	add r10, r3
	cmp r2, #15
	ble .L_0200853e
	movs r0, #0
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020085f0,"ax",%progbits
	.global Func_020005f0
	.thumb_func
Func_020005f0:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #164
	ldr r2, [r3]
	movs r4, #0
	movs r1, #4
	ldrsh r3, [r2, r1]
	adds r0, r4, #0
	cmp r3, #8
	bne .L_0200860a
	adds r4, r2, #4
	b .L_02008622
.L_0200860a:
	adds r0, #1
	cmp r0, #7
	bgt .L_02008622
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r1, r3, #2
	adds r3, r1, #4
	ldrsh r3, [r2, r3]
	cmp r3, #8
	bne .L_0200860a
	adds r3, r2, r1
	adds r4, r3, #4
.L_02008622:
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r4, #16]
	movs r3, #64
	str r3, [r4, #4]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008630,"ax",%progbits
	.global Func_02000630
	.thumb_func
Func_02000630:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #164
	ldr r2, [r3]
	movs r4, #0
	movs r1, #4
	ldrsh r3, [r2, r1]
	adds r0, r4, #0
	cmp r3, #8
	bne .L_0200864a
	adds r4, r2, #4
	b .L_02008662
.L_0200864a:
	adds r0, #1
	cmp r0, #7
	bgt .L_02008662
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r1, r3, #2
	adds r3, r1, #4
	ldrsh r3, [r2, r3]
	cmp r3, #8
	bne .L_0200864a
	adds r3, r2, r1
	adds r4, r3, #4
.L_02008662:
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r4, #16]
	ldr r3, [r4, #4]
	adds r3, #128
	str r3, [r4, #4]
	pop {r5, pc}
	.section .text.x02008670,"ax",%progbits
	.global Func_02000670
	.thumb_func
Func_02000670:
	push {r5, r6, lr}
	movs r0, #17
	bl Object_GetById
	ldr r3, .L_020086e0
	adds r5, r0, #0
	ldrb r6, [r3]
	cmp r6, #0
	bne .L_020086de
	movs r1, #202
	bl Func_02000f1c
	movs r0, #140
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	lsls r0, r0, #1
	bl Func_02000efc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020086de
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #28]
	adds r3, r5, #0
	adds r3, #35
	strb r6, [r3]
	movs r1, #2
	bl Func_02000e24
	ldr r1, [r5, #8]
	ldr r3, .L_020086e4
	ldr r2, [r5, #12]
	adds r1, r1, r3
	adds r0, r5, #0
	ldr r3, [r5, #16]
	bl Func_02000e3c
	ldr r1, .L_020086e8
	adds r0, r5, #0
	bl Func_02000e2c
	adds r0, r5, #0
	movs r1, #2
	bl Func_02000e5c
.L_020086de:
	pop {r5, r6, pc}
.L_020086e0:
	.4byte Data_0300122c
.L_020086e4:
	.4byte 0xffe00000
.L_020086e8:
	.4byte Data_020015c4
	.section .text.x020086ec,"ax",%progbits
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #192
	lsls r1, r1, #18
	ldr r3, [r1, #108]
	ldr r6, .L_0200877c
	movs r2, #214
	lsls r2, r2, #1
	mov r8, r1
	movs r1, #152
	adds r3, r3, r2
	ldr r5, .L_02008780
	adds r2, #88
	lsls r1, r1, #2
	str r2, [r3]
	adds r3, r6, r1
	movs r1, #5
	mov r10, r1
	adds r2, #94
	strh r5, [r3]
	adds r3, r6, r2
	mov r2, r10
	strh r2, [r3]
	sub sp, #8
	bl Func_02000f0c
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	movs r1, #240
	orrs r3, r2
	lsls r1, r1, #1
	strb r3, [r0]
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r7, .L_02008778
	cmp r2, r5
	beq .L_02008744
	b .L_0200893c
.L_02008744:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008784
	bl Func_02000dec
	movs r0, #8
	movs r1, #1
	bl Func_02000f14
	movs r0, #9
	movs r1, #1
	bl Func_02000f14
	movs r0, #10
	movs r1, #1
	bl Func_02000f14
	movs r0, #11
	movs r1, #1
	bl Func_02000f14
	movs r0, #12
	movs r1, #1
	bl Func_02000f14
	b .L_02008788
.L_02008778:
	.4byte 0x00000000
.L_0200877c:
	.4byte gPartyState
.L_02008780:
	.4byte 0x000000ae
.L_02008784:
	.4byte Func_02000670
.L_02008788:
	movs r0, #13
	movs r1, #1
	bl Func_02000f14
	movs r0, #14
	movs r1, #1
	bl Func_02000f14
	movs r0, #15
	movs r1, #1
	bl Func_02000f14
	movs r0, #146
	lsls r0, r0, #4
	bl Func_02000e14
	cmp r0, #0
	beq .L_020087e0
	movs r0, #16
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	movs r3, #176
	lsls r3, r3, #15
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r5, #12]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #27
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #6
	movs r1, #27
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
.L_020087e0:
	movs r0, #19
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	movs r0, #144
	adds r3, #35
	lsls r0, r0, #4
	strb r7, [r3]
	adds r0, #33
	bl Func_02000e14
	cmp r0, #0
	beq .L_02008840
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	movs r3, #162
	lsls r3, r3, #18
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r5, #12]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #37
	movs r2, #102
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #102
	movs r2, #1
	movs r3, #2
	bl Func_02000e44
	movs r3, #40
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #27
	movs r1, #44
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
	b .L_0200888a
.L_02008840:
	movs r0, #10
	adds r0, #255
	bl Func_02000e14
	cmp r0, #0
	beq .L_0200888a
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #75
	beq .L_02008868
	movs r3, #37
	movs r2, #102
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #102
	movs r2, #1
	movs r3, #2
	bl Func_02000e44
.L_02008868:
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #79
	bne .L_0200888a
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	movs r2, #46
	movs r3, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #46
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
.L_0200888a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl Func_02000e14
	cmp r0, #0
	bne .L_0200889a
	b .L_02008a36
.L_0200889a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02000e14
	cmp r0, #0
	beq .L_020088aa
	b .L_02008a36
.L_020088aa:
	movs r2, #200
	movs r0, #20
	ldr r1, .L_02008a44
	lsls r2, r2, #18
	bl Func_02000e8c
	movs r5, #49
	movs r0, #53
	movs r1, #52
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000e44
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #53
	movs r1, #52
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02000e44
	movs r0, #20
	movs r1, #5
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #97
	bl Func_02000e14
	cmp r0, #0
	beq .L_020088f2
	b .L_02008a36
.L_020088f2:
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #1
	movs r0, #20
	bl Func_02000ed4
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, .L_02008a48
	bl Func_02000ea4
	movs r0, #20
	movs r1, #0
	bl Func_02000eac
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #97
	bl Func_02000e1c
	bl Func_02000e7c
	b .L_02008a36
.L_0200893c:
	ldr r3, .L_02008a4c
	cmp r2, r3
	bne .L_020089e8
	movs r0, #8
	movs r1, #1
	bl Func_02000f14
	movs r0, #9
	movs r1, #1
	bl Func_02000f14
	movs r0, #10
	movs r1, #1
	bl Func_02000f14
	movs r0, #11
	movs r1, #1
	bl Func_02000f14
	movs r0, #12
	movs r1, #1
	bl Func_02000f14
	movs r0, #13
	movs r1, #1
	bl Func_02000f14
	movs r1, #1
	movs r0, #14
	bl Func_02000f14
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_02008992
	bl Func_02000dd0
.L_02008992:
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02000e14
	cmp r0, #0
	beq .L_020089ae
	movs r1, #191
	movs r3, #212
	lsls r1, r1, #18
	ldr r2, .L_02008a50
	lsls r3, r3, #17
	movs r0, #17
	bl Func_02000b2c
.L_020089ae:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000e14
	cmp r0, #0
	beq .L_020089ca
	movs r3, #212
	ldr r1, .L_02008a54
	ldr r2, .L_02008a58
	lsls r3, r3, #17
	movs r0, #18
	bl Func_02000b2c
.L_020089ca:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl Func_02000e14
	cmp r0, #0
	beq .L_02008a36
	movs r3, #212
	ldr r1, .L_02008a5c
	ldr r2, .L_02008a60
	lsls r3, r3, #17
	movs r0, #19
	bl Func_02000b2c
	b .L_02008a36
.L_020089e8:
	ldr r3, .L_02008a64
	cmp r2, r3
	bne .L_02008a36
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008a04
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02000e8c
.L_02008a04:
	mov r1, r8
	ldr r3, [r1, #108]
	movs r2, #138
	ldr r5, [r3, #84]
	lsls r2, r2, #1
	adds r3, r3, r2
	str r5, [r3]
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	movs r2, #13
	ldr r3, [r5, #16]
	movs r0, #11
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r1, #14
	movs r2, #1
	movs r3, #1
	bl Func_02000e44
.L_02008a36:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a44:
	.4byte 0x031e0000
.L_02008a48:
	.4byte 0x00002314
.L_02008a4c:
	.4byte 0x000000af
.L_02008a50:
	.4byte 0xfe790000
.L_02008a54:
	.4byte 0x03150000
.L_02008a58:
	.4byte 0xff080000
.L_02008a5c:
	.4byte 0x030a0000
.L_02008a60:
	.4byte 0xff890000
.L_02008a64:
	.4byte 0x000000b0
	.section .text.x02008a68,"ax",%progbits
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	push {lr}
	ldr r1, .L_02008aa8
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008aac
	cmp r2, r3
	bne .L_02008aa4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_02008aa4
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	subs r2, #162
	adds r3, r3, r2
	ldr r2, .L_02008ab0
	str r2, [r3, #8]
	movs r2, #137
	lsls r2, r2, #19
	str r2, [r3, #12]
.L_02008aa4:
	movs r0, #0
	pop {pc}
.L_02008aa8:
	.4byte gPartyState
.L_02008aac:
	.4byte 0x000000af
.L_02008ab0:
	.4byte 0xff800000
	.section .text.x02008ab4,"ax",%progbits
	.global Func_02000ab4
	.thumb_func
Func_02000ab4:
	push {r5, r6, lr}
	ldr r2, .L_02008b20
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_02008b1e
	movs r3, #192
	movs r1, #133
	lsls r3, r3, #18
	lsls r1, r1, #2
	ldr r6, [r3, #108]
	ldr r5, [r3, #32]
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_02008ae4
	ldr r2, .L_02008b24
	adds r3, r3, r2
.L_02008ae4:
	ldr r2, [r0, #16]
	asrs r1, r3, #20
	ldr r3, [r0, #12]
	subs r0, r2, r3
	movs r2, #254
	lsls r2, r2, #7
	adds r2, #255
	adds r3, r0, r2
	cmp r3, #0
	bge .L_02008afc
	ldr r2, .L_02008b28
	adds r3, r0, r2
.L_02008afc:
	movs r0, #212
	lsls r0, r0, #1
	asrs r3, r3, #20
	adds r2, r5, r0
	ldr r2, [r2]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r2, [r2, #2]
	subs r3, r2, #1
	cmp r3, #229
	bhi .L_02008b1e
	movs r1, #170
	lsls r1, r1, #1
	adds r3, r6, r1
	strh r2, [r3]
.L_02008b1e:
	pop {r5, r6, pc}
.L_02008b20:
	.4byte gPartyState
.L_02008b24:
	.4byte 0x000fffff
.L_02008b28:
	.4byte 0x00107ffe
	.section .text.x02008b2c,"ax",%progbits
	.global Func_02000b2c
	.thumb_func
Func_02000b2c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	mov r8, r2
	adds r6, r3, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #212
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r4, r0, #0
	ldr r0, [r3]
	adds r3, r5, #0
	cmp r5, #0
	bge .L_02008b56
	ldr r2, .L_02008bb4
	adds r3, r5, r2
.L_02008b56:
	asrs r7, r3, #20
	mov r3, r8
	subs r2, r6, r3
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	adds r1, r2, r3
	cmp r1, #0
	bge .L_02008b6c
	ldr r3, .L_02008bb8
	adds r1, r2, r3
.L_02008b6c:
	asrs r1, r1, #20
	lsls r3, r1, #7
	adds r3, r7, r3
	lsls r3, r3, #2
	adds r0, r0, r3
	movs r3, #255
	strb r3, [r0, #2]
	ldr r0, .L_02008bbc
	movs r3, #128
	ands r5, r0
	ands r6, r0
	lsls r3, r3, #12
	adds r2, r5, r3
	lsls r1, r1, #20
	adds r3, r6, r3
	str r2, [r4, #8]
	str r3, [r4, #16]
	adds r2, r4, #0
	subs r3, r3, r1
	str r3, [r4, #12]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r4, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r4, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008bb4:
	.4byte 0x000fffff
.L_02008bb8:
	.4byte 0x00107ffe
.L_02008bbc:
	.4byte 0xfff00000
	.section .text.x02008bc0,"ax",%progbits
	.global Func_02000bc0
	.thumb_func
Func_02000bc0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	ldr r3, .L_02008dc0
	str r1, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #72]
	str r3, [sp, #8]
	bl Func_02000e74
	movs r0, #0
	bl Func_02000ef4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_02000ec4
	adds r0, r6, #0
	bl Object_GetById
	ldr r2, [r5, #12]
	adds r7, r0, #0
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	adds r0, r6, #0
	bl Func_02000b2c
	ldr r1, .L_02008dc4
	adds r0, r7, #0
	bl Func_02000e2c
	movs r2, #15
	mov r11, r2
.L_02008c24:
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	movs r0, #255
	bl Func_02000e34
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008c9c
	ldr r1, .L_02008dc8
	bl Func_02000e2c
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #6
	lsrs r2, r0, #1
	adds r2, r2, r3
	str r0, [sp, #4]
	mov r0, r10
	mov r9, r2
	bl Math_Cosine
	ldr r2, .L_02008dcc
	lsls r6, r6, #3
	adds r1, r0, #0
	mov r8, r2
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #40]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	mov r3, r9
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r3, [r7, #68]
.L_02008c9c:
	movs r2, #1
	negs r2, r2
	add r11, r2
	mov r3, r11
	cmp r3, #0
	bge .L_02008c24
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #78
	bl Func_02000f24
	movs r0, #217
	bl Func_02000f24
	movs r2, #230
	movs r0, #128
	movs r1, #128
	lsls r2, r2, #8
	lsls r0, r0, #10
	lsls r1, r1, #10
	adds r2, #102
	bl Func_02000e54
	ldr r3, .L_02008dc0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #2
	ldr r0, [r3]
	adds r1, #255
	bl Func_02000ebc
	movs r1, #40
	adds r0, r5, #0
	bl Func_02000e24
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #204
	bl Func_02000f24
	adds r2, r5, #0
	movs r3, #3
	adds r2, #85
	strb r3, [r2]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	movs r6, #0
	str r3, [r5, #72]
	b .L_02008d06
.L_02008d04:
	adds r6, #1
.L_02008d06:
	cmp r6, #179
	bgt .L_02008d18
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bgt .L_02008d04
.L_02008d18:
	movs r0, #188
	bl Func_02000f24
	ldr r3, [sp, #8]
	ldr r6, .L_02008dc0
	str r3, [r5, #72]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_02000ebc
	adds r0, r5, #0
	movs r1, #38
	bl Func_02000e24
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02000e54
	movs r0, #10
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02000e54
	movs r0, #20
	bl WaitFrames
	ldr r2, [r5, #16]
	movs r3, #1
	ldr r1, [r5, #12]
	ldr r0, [r5, #8]
	bl Func_02000ec4
	bl Func_02000ecc
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #1
	adds r0, r5, #0
	bl Func_02000e24
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r0, #20
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r2, r6, r3
	movs r3, #0
	strb r3, [r2]
	bl Func_02000e7c
	ldr r0, [sp, #12]
	bl Func_02000e1c
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008dc0:
	.4byte gPartyState
.L_02008dc4:
	.4byte Data_020016cc
.L_02008dc8:
	.4byte Data_02001688
.L_02008dcc:
	.4byte IwramMulQ16
	.section .text.x02008dd0,"ax",%progbits
	.global Func_02000dd0
	.thumb_func
Func_02000dd0:
	push {lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008de0
	bl Func_02000dec
	pop {pc}
	.2byte 0x0000
.L_02008de0:
	.4byte Func_02000ab4
	.section .rodata.x02008f2c,"a",%progbits
	.global Data_02000f2c
Data_02000f2c:
	.4byte 0xffff0000
	.4byte 0x00000018
	.4byte 0x00000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000f5c
Data_02000f5c:
	.4byte 0xffe40303
	.4byte 0x030b01e4
	.4byte 0x01ecffec
	.4byte 0x0002ffff
	.4byte 0x00b40303
	.4byte 0x030b02c3
	.4byte 0x02cb00bc
	.4byte 0x0003ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000f8c
Data_02000f8c:
	.4byte 0x012800c3
	.4byte 0x00cb0143
	.4byte 0x014b0130
	.4byte 0x0002ffff
	.4byte 0xfdd602c3
	.4byte 0x02cb01c4
	.4byte 0x01ccfdde
	.4byte 0x0003ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000fbc
Data_02000fbc:
	.4byte 0x000000ae
	.4byte 0x101010af
	.4byte 0xffffffff
	.4byte 0x102030ae
	.4byte 0xffffffff
	.4byte 0x103020ae
	.4byte 0xffffffff
	.4byte 0x104010b2
	.4byte 0xffffffff
	.4byte 0x10527002
	.4byte 0xffffffff
	.4byte 0x000000af
	.4byte 0x101010ae
	.4byte 0xffffffff
	.4byte 0x102030af
	.4byte 0xffffffff
	.4byte 0x103020af
	.4byte 0xffffffff
	.4byte 0x104010b0
	.4byte 0xffffffff
	.4byte 0x000000b0
	.4byte 0x101040af
	.4byte 0xffffffff
	.4byte 0x000001ff
.L_0200901c:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_0200102c
Data_0200102c:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001044
Data_02001044:
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte .L_0200901c
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x01270000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00028000
	.4byte 0xffff0118
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte .L_0200901c
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001194
Data_02001194:
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020012cc
Data_020012cc:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0181
	.4byte 0x00000001
	.4byte 0x02900000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff018d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020013c8
Data_020013c8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0x09ab002a
	.4byte Func_02000378
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_020003c4
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte Func_020003fc
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000e
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000f
	.4byte 0x00000000
	.4byte 0x50008905
	.4byte 0xffff0014
	.4byte Func_020002a0
	.4byte 0x10008c15
	.4byte 0x09200010
	.4byte Func_02000148
	.4byte 0x00008c15
	.4byte 0x09200010
	.4byte Func_02000164
	.4byte 0x00008c15
	.4byte 0x09210013
	.4byte Func_02000164
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000164
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020014b8
Data_020014b8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte gHeapSlots + 0x14
	.4byte Func_020000d0
	.4byte 0x00000002
	.4byte 0x03010015
	.4byte Func_020000e0
	.4byte 0x00000002
	.4byte 0x03020016
	.4byte Func_020000f0
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000e
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001554
Data_02001554:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000314
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Func_02000258
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Func_0200027c
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000258
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200027c
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_020002c0
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_020002fc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020015c0
Data_020015c0:
	.4byte 0x00000026
	.global Data_020015c4
Data_020015c4:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0001b333
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00014ccc
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000002e
	.4byte Func_020005f0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x0000002e
	.4byte Func_02000630
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x00000003
	.4byte 0xc0010000
	.4byte 0x0000002e
	.4byte Func_02000528
	.4byte 0x80020000
	.4byte 0x0000002e
	.4byte Func_02000630
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000000e
	.4byte 0xc0020000
	.4byte 0x0000002e
	.4byte Func_02000630
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000026
	.global Data_02001688
Data_02001688:
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_020016cc
Data_020016cc:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000011
