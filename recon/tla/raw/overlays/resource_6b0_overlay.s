.syntax unified
	.thumb
	.section .text.x020081f8,"ax",%progbits
	.balign 4
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {r5, r6, r7, lr}
	ldr r1, .L_020082a8
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_02008232
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02008232
	ldr r0, .L_020082ac
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_02008232
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_02008232:
	cmp r4, #0
	bne .L_02008238
	b .L_02008324
.L_02008238:
	ldr r3, .L_020082b0
	ldr r6, .L_020082b4
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_02008286
	ldr r3, .L_020082b8
	ldr r2, .L_020082bc
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_02008250:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_02008250
.L_02008286:
	ldr r3, .L_020082b4
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_020082c0
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_020082c4
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020082cc
	ldr r3, .L_020082c8
	b .L_020082ce
.L_020082a8:
	.4byte gData_0200443c
.L_020082ac:
	.4byte gData_02004440
.L_020082b0:
	.4byte Data_0200444c
.L_020082b4:
	.4byte gData_02004450
.L_020082b8:
	.4byte Data_02004438
.L_020082bc:
	.4byte gData_02004454
.L_020082c0:
	.4byte gData_02004444
.L_020082c4:
	.4byte gData_02004434
.L_020082c8:
	.4byte Data_02004448
.L_020082cc:
	ldr r3, .L_02008314
.L_020082ce:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_02008318
	ldr r2, .L_0200830c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200831c
	ldr r2, .L_02008320
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02008324
	ldr r3, .L_02008310
	strh r3, [r1]
	b .L_02008324
	.2byte 0x0000
.L_0200830c:
	.4byte 0x00000001
.L_02008310:
	.4byte 0x00000000
.L_02008314:
	.4byte Data_0200444c
.L_02008318:
	.4byte gData_02004434
.L_0200831c:
	.4byte gData_02004454
.L_02008320:
	.4byte gData_02004450
.L_02008324:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020083f6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008446,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200847e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009e64,"ax",%progbits
	.global Func_02001e64
	.thumb_func
Func_02001e64:
	push {r5, r6, r7, lr}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r7, #129
	adds r3, r3, r0
	lsls r7, r7, #2
	ldr r6, .L_0200a194
	str r7, [r3]
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r0, #0
	ldrsh r1, [r3, r0]
	ldr r3, .L_0200a198
	sub sp, #8
	cmp r1, r3
	bne .L_02009f86
	ldr r0, .L_0200a19c
	ldr r1, .L_0200a1a0
	ldr r2, .L_0200a1a4
	ldr r3, .L_0200a1a8
	bl Func_02000038
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #135
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ec6
	movs r0, #130
	lsls r0, r0, #2
	movs r1, #48
	movs r2, #0
	bl Func_02000a78
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #34
	movs r2, #85
	movs r3, #8
	bl Func_02003328
.L_02009ec6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #136
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ef4
	movs r0, #138
	lsls r0, r0, #2
	movs r1, #48
	movs r2, #3
	bl Func_02000a78
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #101
	movs r1, #34
	movs r2, #101
	movs r3, #8
	bl Func_02003328
.L_02009ef4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f22
	movs r0, #252
	lsls r0, r0, #1
	movs r1, #40
	movs r2, #10
	bl Func_02000a78
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #46
	movs r2, #85
	movs r3, #20
	bl Func_02003328
.L_02009f22:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f50
	movs r0, #142
	lsls r0, r0, #2
	movs r1, #40
	movs r2, #2
	bl Func_02000a78
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #101
	movs r1, #46
	movs r2, #101
	movs r3, #20
	bl Func_02003328
.L_02009f50:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #139
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009f60
	b .L_0200a6e6
.L_02009f60:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #54
	movs r1, #7
	movs r2, #33
	movs r3, #7
	bl Func_02003328
	movs r0, #134
	movs r1, #136
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #0
	bl Func_02003380
	b .L_0200a6e6
.L_02009f86:
	ldr r3, .L_0200a1ac
	cmp r1, r3
	beq .L_02009f8e
	b .L_0200a1b4
.L_02009f8e:
	movs r0, #8
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #168
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fb8
	movs r1, #148
	movs r2, #248
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_020033e0
	movs r0, #8
	movs r1, #2
	bl Actor_SetSpritePriority
.L_02009fb8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #129
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fd4
	movs r1, #220
	movs r2, #190
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020033e0
.L_02009fd4:
	ldr r0, .L_0200a1b0
	bl Func_02003490
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r6, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Actor_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #9
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r6, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #17
	bne .L_0200a03c
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a03c
	movs r0, #9
	movs r1, #2
	bl Actor_SetModeById
	movs r1, #182
	movs r2, #240
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl Func_020033e0
.L_0200a03c:
	ldr r3, .L_0200a194
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #18
	bne .L_0200a05c
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a05c
	bl Func_02001d68
.L_0200a05c:
	ldr r5, .L_0200a194
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #10
	beq .L_0200a06e
	b .L_0200a6e6
.L_0200a06e:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a084
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020033e0
.L_0200a084:
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a09e
	movs r0, #9
	bl Actor_GetById
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
.L_0200a09e:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0bc
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #44
	movs r3, #86
	bl Func_02003328
.L_0200a0bc:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0dc
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #46
	movs r3, #86
	bl Func_02003328
.L_0200a0dc:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0fc
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #43
	movs r3, #87
	bl Func_02003328
.L_0200a0fc:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a11c
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #45
	movs r3, #87
	bl Func_02003328
.L_0200a11c:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a13a
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #47
	movs r3, #87
	bl Func_02003328
.L_0200a13a:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a15a
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #44
	movs r3, #88
	bl Func_02003328
.L_0200a15a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a17a
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #46
	movs r3, #88
	bl Func_02003328
.L_0200a17a:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	b .L_0200a6e6
	.2byte 0x0000
.L_0200a194:
	.4byte gMainRoutineWork
.L_0200a198:
	.4byte 0x0000012f
.L_0200a19c:
	.4byte gData_0200384c
.L_0200a1a0:
	.4byte gData_0200385c
.L_0200a1a4:
	.4byte gData_02003888
.L_0200a1a8:
	.4byte gData_020038b4
.L_0200a1ac:
	.4byte 0x00000130
.L_0200a1b0:
	.4byte gData_020036e4
.L_0200a1b4:
	ldr r3, .L_0200a518
	cmp r1, r3
	beq .L_0200a1bc
	b .L_0200a2ce
.L_0200a1bc:
	movs r0, #14
	bl Actor_GetById
	adds r3, r0, #0
	adds r3, #85
	movs r7, #2
	strb r7, [r3]
	ldr r3, .L_0200a51c
	movs r5, #1
	str r3, [r0, #20]
	str r3, [r0, #12]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r6, r0
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Actor_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a218
	movs r1, #224
	movs r2, #212
	movs r0, #8
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200a218:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #131
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a254
	movs r1, #134
	movs r2, #180
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020033e0
	movs r0, #51
	movs r1, #20
	movs r2, #31
	movs r3, #20
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl Func_02003328
	movs r0, #248
	movs r1, #168
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_02003380
.L_0200a254:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #132
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a270
	movs r1, #186
	movs r2, #228
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200a270:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #133
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2c6
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a2b8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #132
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a2b8
	movs r0, #12
	bl Actor_GetById
	movs r1, #10
	ldrsh r5, [r0, r1]
	movs r0, #12
	bl Actor_GetById
	movs r3, #18
	ldrsh r2, [r0, r3]
	lsls r5, r5, #16
	lsls r2, r2, #16
	movs r0, #13
	adds r1, r5, #0
	bl Func_020033e0
.L_0200a2b8:
	movs r1, #182
	movs r2, #236
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200a2c6:
	ldr r0, .L_0200a520
	bl Func_02003490
	b .L_0200a6e6
.L_0200a2ce:
	ldr r3, .L_0200a524
	cmp r1, r3
	beq .L_0200a2d6
	b .L_0200a52c
.L_0200a2d6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #134
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2f2
	movs r1, #132
	movs r2, #180
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200a2f2:
	ldr r0, .L_0200a528
	bl Func_02003490
	movs r0, #241
	lsls r0, r0, #1
	adds r2, r6, r0
	ldrh r3, [r2]
	movs r1, #0
	ldrsh r2, [r2, r1]
	cmp r2, #1
	beq .L_0200a30a
	b .L_0200a4a2
.L_0200a30a:
	movs r3, #68
	movs r1, #160
	movs r2, #224
	str r3, [sp, #0]
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r3, #18
	movs r0, #33
	bl Func_02002a74
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a35e
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #8
	bl Actor_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #8
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	b .L_0200a3a8
.L_0200a35e:
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl Func_020033e0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r0, r7, #0
	bl GameFlag_SetBit
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
.L_0200a3a8:
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a3c2
	movs r0, #8
	bl Actor_GetById
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
.L_0200a3c2:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a3e0
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #8
	movs r3, #69
	bl Func_02003328
.L_0200a3e0:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a400
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #6
	movs r3, #71
	bl Func_02003328
.L_0200a400:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a420
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #5
	movs r3, #75
	bl Func_02003328
.L_0200a420:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a440
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #8
	movs r3, #75
	bl Func_02003328
.L_0200a440:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a460
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #11
	movs r3, #75
	bl Func_02003328
.L_0200a460:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a47e
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #8
	movs r3, #73
	bl Func_02003328
.L_0200a47e:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a48e
	b .L_0200a6e6
.L_0200a48e:
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #10
	movs r3, #71
	bl Func_02003328
	b .L_0200a6e6
.L_0200a4a2:
	cmp r2, #11
	bne .L_0200a4c0
	movs r3, #68
	movs r1, #160
	movs r2, #224
	lsls r1, r1, #2
	lsls r2, r2, #2
	str r3, [sp, #0]
	movs r0, #33
	movs r3, #18
	bl Func_02002a74
	movs r0, #22
	movs r1, #0
	b .L_0200a4ec
.L_0200a4c0:
	cmp r2, #12
	bne .L_0200a4f4
	movs r3, #68
	movs r1, #160
	movs r2, #224
	str r3, [sp, #0]
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #35
	movs r3, #18
	bl Func_02002a74
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a4e6
	b .L_0200a6e6
.L_0200a4e6:
	movs r1, #36
	negs r1, r1
	movs r0, #36
.L_0200a4ec:
	adds r2, r7, #0
	bl Func_02003190
	b .L_0200a6e6
.L_0200a4f4:
	subs r3, #4
	movs r0, #128
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bls .L_0200a502
	b .L_0200a6e6
.L_0200a502:
	movs r3, #68
	movs r1, #160
	movs r2, #224
	str r3, [sp, #0]
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #35
	movs r3, #18
	bl Func_02002a74
	b .L_0200a6e6
.L_0200a518:
	.4byte 0x00000131
.L_0200a51c:
	.4byte 0xfff20000
.L_0200a520:
	.4byte gData_020036ea
.L_0200a524:
	.4byte 0x00000132
.L_0200a528:
	.4byte gData_020036f6
.L_0200a52c:
	ldr r3, .L_0200a6ec
	cmp r1, r3
	beq .L_0200a534
	b .L_0200a6e6
.L_0200a534:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r6, r1
	ldr r5, [r2, #32]
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #11
	bls .L_0200a548
	b .L_0200a6e6
.L_0200a548:
	ldr r2, .L_0200a6f0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200a550:
	.4byte .L_0200a606
	.4byte .L_0200a606
	.4byte .L_0200a63a
	.4byte .L_0200a63a
	.4byte .L_0200a686
	.4byte .L_0200a686
	.4byte .L_0200a580
	.4byte .L_0200a6e6
	.4byte .L_0200a6e6
	.4byte .L_0200a6e6
	.4byte .L_0200a6e6
	.4byte .L_0200a596
.L_0200a580:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a596
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020033e0
.L_0200a596:
	ldr r6, .L_0200a6f4
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r6, r0
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #11
	bl Actor_SetModeById
	movs r0, #11
	bl Actor_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #11
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #11
	bl Actor_SetModeById
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a5fa
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	bne .L_0200a5fa
	bl Func_02001df4
	b .L_0200a6e6
.L_0200a5fa:
	movs r0, #11
	bl Actor_GetById
	ldr r3, .L_0200a6f8
	str r3, [r0, #12]
	b .L_0200a6e6
.L_0200a606:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a61e
	movs r1, #240
	movs r2, #240
	movs r0, #8
	lsls r1, r1, #15
	b .L_0200a650
.L_0200a61e:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #1
	movs r2, #11
	movs r3, #4
	bl Func_02003328
	movs r0, #184
	movs r1, #176
	lsls r0, r0, #16
	b .L_0200a67a
.L_0200a63a:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a660
	movs r1, #138
	movs r2, #240
	movs r0, #9
	lsls r1, r1, #18
.L_0200a650:
	lsls r2, r2, #15
	bl Func_020033e0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200a6e6
.L_0200a660:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #1
	movs r2, #26
	movs r3, #4
	bl Func_02003328
	movs r0, #212
	movs r1, #176
	lsls r0, r0, #17
.L_0200a67a:
	lsls r1, r1, #15
	movs r2, #0
	movs r3, #2
	bl Func_02003380
	b .L_0200a6e6
.L_0200a686:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a6ac
	movs r1, #198
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl Func_020033e0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200a6d0
.L_0200a6ac:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #1
	movs r2, #53
	movs r3, #4
	bl Func_02003328
	movs r0, #214
	movs r1, #176
	lsls r0, r0, #18
	lsls r1, r1, #15
	movs r2, #0
	movs r3, #2
	bl Func_02003380
.L_0200a6d0:
	adds r2, r5, #0
	adds r2, #236
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #12
	adds r3, r3, r1
	str r3, [r2]
	adds r2, #8
	ldr r3, [r2]
	adds r3, r3, r1
	str r3, [r2]
.L_0200a6e6:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0200a6ec:
	.4byte 0x00000133
.L_0200a6f0:
	.4byte .L_0200a550
.L_0200a6f4:
	.4byte gMainRoutineWork
.L_0200a6f8:
	.4byte 0xfff00000
	.section .text.x0200a746,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200af46,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global gData_02004434
gData_02004434:
	.space 0x00000004
	.global Data_02004438
Data_02004438:
	.space 0x00000004
	.global gData_0200443c
gData_0200443c:
	.space 0x00000004
	.global gData_02004440
gData_02004440:
	.space 0x00000004
	.global gData_02004444
gData_02004444:
	.space 0x00000004
	.global Data_02004448
Data_02004448:
	.space 0x00000004
	.global Data_0200444c
Data_0200444c:
	.space 0x00000004
	.global gData_02004450
gData_02004450:
	.space 0x00000004
	.global gData_02004454
gData_02004454:
	.space 0x00000004
	.global Data_02004458
Data_02004458:
