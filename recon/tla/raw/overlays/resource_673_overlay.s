.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r0, #8
	movs r1, #10
	bl Func_0200122c
	pop {pc}
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	sub sp, #12
	movs r2, #57
	movs r3, #16
	movs r1, #63
	str r2, [sp, #4]
	movs r0, #7
	movs r2, #1
	str r3, [sp, #0]
	str r1, [sp, #8]
	bl Func_02001274
	add sp, #12
	pop {pc}
	.section .text.x02008060,"ax",%progbits
	.global Func_02000060
	.thumb_func
Func_02000060:
	push {lr}
	sub sp, #12
	movs r2, #63
	movs r1, #52
	movs r3, #5
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #22
	movs r1, #15
	movs r2, #0
	str r3, [sp, #0]
	bl Func_02001274
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {lr}
	sub sp, #12
	movs r2, #56
	movs r1, #58
	movs r3, #3
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #7
	movs r1, #22
	movs r2, #0
	str r3, [sp, #0]
	bl Func_02001274
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x020080a0,"ax",%progbits
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {lr}
	sub sp, #12
	movs r3, #3
	movs r2, #66
	movs r1, #57
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #44
	movs r1, #25
	movs r2, #0
	movs r3, #4
	bl Func_02001274
	add sp, #12
	pop {pc}
	.section .text.x020080c0,"ax",%progbits
	.global Func_020000c0
	.thumb_func
Func_020000c0:
	push {lr}
	sub sp, #12
	movs r3, #8
	movs r2, #61
	movs r1, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #16
	movs r1, #13
	movs r2, #0
	movs r3, #4
	bl Func_02001274
	add sp, #12
	pop {pc}
	.section .text.x020080e0,"ax",%progbits
	.global Func_020000e0
	.thumb_func
Func_020000e0:
	push {lr}
	sub sp, #12
	movs r2, #57
	movs r1, #52
	movs r3, #2
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #21
	movs r1, #9
	movs r2, #0
	str r3, [sp, #0]
	bl Func_02001274
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x02008100,"ax",%progbits
	.global Func_02000100
	.thumb_func
Func_02000100:
	push {r5, r6, lr}
	ldr r3, .L_02008188
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r2, #63
	movs r1, #52
	movs r3, #5
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #15
	movs r0, #22
	movs r2, #0
	str r3, [sp, #0]
	bl Func_02001274
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #83
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008184
	movs r1, #196
	movs r2, #172
	movs r0, #64
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #24
	bne .L_02008184
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #21
	bne .L_02008184
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_02008184:
	add sp, #12
	pop {r5, r6, pc}
.L_02008188:
	.4byte gPartyState
	.section .text.x0200818c,"ax",%progbits
	.global Func_0200018c
	.thumb_func
Func_0200018c:
	push {lr}
	sub sp, #8
	movs r3, #9
	movs r2, #65
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #83
	movs r2, #3
	movs r3, #1
	movs r0, #51
	bl Func_0200116c
	movs r0, #3
	bl Func_0200121c
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020081b0,"ax",%progbits
	.global Func_020001b0
	.thumb_func
Func_020001b0:
	push {lr}
	sub sp, #8
	movs r3, #18
	movs r2, #66
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #83
	movs r2, #3
	movs r3, #1
	movs r0, #51
	bl Func_0200116c
	movs r0, #4
	bl Func_0200121c
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020081d4,"ax",%progbits
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {lr}
	movs r0, #5
	bl Func_0200121c
	pop {pc}
	.2byte 0x0000
	.section .text.x020081e0,"ax",%progbits
	.global Func_020001e0
	.thumb_func
Func_020001e0:
	push {r5, r6, lr}
	ldr r3, .L_0200825c
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r2, #56
	movs r1, #58
	movs r3, #3
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #22
	movs r0, #7
	movs r2, #0
	str r3, [sp, #0]
	bl Func_02001274
	movs r1, #136
	movs r2, #188
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_02008256
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02008256
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_02008256:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200825c:
	.4byte gPartyState
	.section .text.x02008260,"ax",%progbits
	.global Func_02000260
	.thumb_func
Func_02000260:
	push {r5, r6, lr}
	ldr r3, .L_020082dc
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #3
	movs r2, #56
	movs r1, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r3, #5
	adds r5, r0, #0
	movs r1, #13
	movs r0, #5
	movs r2, #0
	bl Func_02001274
	movs r1, #136
	movs r2, #248
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_020082d8
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #15
	bne .L_020082d8
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_020082d8:
	add sp, #12
	pop {r5, r6, pc}
.L_020082dc:
	.4byte gPartyState
	.section .text.x020082e0,"ax",%progbits
	.global Func_020002e0
	.thumb_func
Func_020002e0:
	push {r5, r6, lr}
	ldr r3, .L_02008360
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #3
	movs r2, #56
	movs r1, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r3, #5
	adds r5, r0, #0
	movs r1, #13
	movs r0, #5
	movs r2, #0
	bl Func_02001274
	movs r1, #152
	movs r2, #132
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #9
	bne .L_0200835a
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #16
	bne .L_0200835a
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	movs r1, #16
	ldr r0, [r6]
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_0200835a:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008360:
	.4byte gPartyState
	.section .text.x02008364,"ax",%progbits
	.global Func_02000364
	.thumb_func
Func_02000364:
	push {r5, r6, lr}
	ldr r3, .L_020083e0
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #3
	movs r2, #56
	movs r1, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r3, #5
	adds r5, r0, #0
	movs r1, #13
	movs r0, #5
	movs r2, #0
	bl Func_02001274
	movs r1, #136
	movs r2, #140
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_020083dc
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_020083dc
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_020083dc:
	add sp, #12
	pop {r5, r6, pc}
.L_020083e0:
	.4byte gPartyState
	.section .text.x020083e4,"ax",%progbits
	.global Func_020003e4
	.thumb_func
Func_020003e4:
	push {r5, r6, lr}
	ldr r3, .L_02008460
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #3
	movs r2, #56
	movs r1, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r3, #5
	adds r5, r0, #0
	movs r1, #13
	movs r0, #5
	movs r2, #0
	bl Func_02001274
	movs r1, #208
	movs r2, #140
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #6
	bne .L_0200845c
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_0200845c
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_0200845c:
	add sp, #12
	pop {r5, r6, pc}
.L_02008460:
	.4byte gPartyState
	.section .text.x02008464,"ax",%progbits
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {r5, r6, lr}
	ldr r3, .L_020084e0
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #3
	movs r2, #56
	movs r1, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r3, #5
	adds r5, r0, #0
	movs r1, #13
	movs r0, #5
	movs r2, #0
	bl Func_02001274
	movs r1, #176
	movs r2, #132
	movs r0, #13
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #5
	bne .L_020084dc
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #16
	bne .L_020084dc
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_020084dc:
	add sp, #12
	pop {r5, r6, pc}
.L_020084e0:
	.4byte gPartyState
	.section .text.x020084e4,"ax",%progbits
	.global Func_020004e4
	.thumb_func
Func_020004e4:
	push {r5, r6, lr}
	ldr r3, .L_02008560
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #3
	movs r2, #56
	movs r1, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r3, #5
	adds r5, r0, #0
	movs r1, #13
	movs r0, #5
	movs r2, #0
	bl Func_02001274
	movs r1, #208
	movs r2, #248
	movs r0, #14
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #6
	bne .L_0200855c
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #15
	bne .L_0200855c
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_0200855c:
	add sp, #12
	pop {r5, r6, pc}
.L_02008560:
	.4byte gPartyState
	.section .text.x02008564,"ax",%progbits
	.global Func_02000564
	.thumb_func
Func_02000564:
	push {r5, r6, lr}
	ldr r3, .L_020085e4
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #8
	movs r2, #61
	movs r1, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r3, #4
	adds r5, r0, #0
	movs r1, #13
	movs r0, #16
	movs r2, #0
	bl Func_02001274
	movs r1, #140
	movs r2, #148
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020011cc
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_020085de
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #18
	bne .L_020085de
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001204
	movs r2, #16
	ldr r0, [r6]
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020011a4
.L_020085de:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020085e4:
	.4byte gPartyState
	.section .text.x020085e8,"ax",%progbits
	.global Func_020005e8
	.thumb_func
Func_020005e8:
	push {lr}
	sub sp, #12
	movs r3, #5
	movs r2, #60
	movs r1, #56
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #7
	movs r1, #23
	movs r2, #0
	movs r3, #4
	bl Func_02001274
	add sp, #12
	pop {pc}
	.section .text.x02008608,"ax",%progbits
	.global Func_02000608
	.thumb_func
Func_02000608:
	push {lr}
	sub sp, #12
	movs r3, #5
	movs r2, #60
	movs r1, #56
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #7
	movs r1, #23
	movs r2, #0
	movs r3, #4
	bl Func_02001274
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #181
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008662
	movs r2, #212
	movs r0, #249
	movs r1, #168
	lsls r2, r2, #1
	bl Func_0200127c
	ldr r2, .L_02008668
	movs r3, #149
	lsls r3, r3, #2
	adds r1, r2, r3
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #181
	strh r3, [r1]
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r2, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #106
	movs r1, #0
	bl Func_02001224
.L_02008662:
	add sp, #12
	pop {pc}
	.2byte 0x0000
.L_02008668:
	.4byte gPartyState
	.section .text.x0200866c,"ax",%progbits
	.global Func_0200066c
	.thumb_func
Func_0200066c:
	push {r5, lr}
	sub sp, #8
	movs r3, #11
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl Func_0200116c
	movs r3, #14
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #4
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200869c,"ax",%progbits
	.global Func_0200069c
	.thumb_func
Func_0200069c:
	push {r5, lr}
	sub sp, #8
	movs r3, #11
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl Func_0200116c
	movs r3, #14
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #5
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020086cc,"ax",%progbits
	.global Func_020006cc
	.thumb_func
Func_020006cc:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #23
	movs r3, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.section .text.x02008704,"ax",%progbits
	.global Func_02000704
	.thumb_func
Func_02000704:
	push {r5, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #15
	movs r3, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008740,"ax",%progbits
	.global Func_02000740
	.thumb_func
Func_02000740:
	push {r5, lr}
	movs r0, #10
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #16
	movs r3, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200877c,"ax",%progbits
	.global Func_0200077c
	.thumb_func
Func_0200077c:
	push {r5, lr}
	movs r0, #11
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #17
	movs r3, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020087b8,"ax",%progbits
	.global Func_020007b8
	.thumb_func
Func_020007b8:
	push {r5, lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #17
	movs r3, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.section .text.x020087f0,"ax",%progbits
	.global Func_020007f0
	.thumb_func
Func_020007f0:
	push {r5, lr}
	movs r0, #13
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #16
	movs r3, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200882c,"ax",%progbits
	.global Func_0200082c
	.thumb_func
Func_0200082c:
	push {r5, lr}
	movs r0, #14
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #15
	movs r3, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008868,"ax",%progbits
	.global Func_02000868
	.thumb_func
Func_02000868:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #18
	movs r3, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #18
	movs r2, #1
	movs r3, #1
	bl Func_02001164
	add sp, #8
	pop {r5, pc}
	.section .text.x020088a0,"ax",%progbits
	.global Func_020008a0
	.thumb_func
Func_020008a0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #230
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r3, [r3]
	adds r6, r0, #0
	mov r8, r3
	ldr r3, [r6, #8]
	sub sp, #12
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #12]
	adds r7, r1, #0
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r1, r5, #0
	adds r3, r3, r7
	str r3, [r5, #8]
	bl Func_02001174
	ldr r3, [r6, #8]
	movs r2, #128
	str r3, [r5]
	ldr r3, [r6, #12]
	lsls r2, r2, #12
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	adds r3, r3, r2
	adds r1, r5, #0
	str r3, [r5, #8]
	bl Func_02001174
	cmp r0, #0
	bgt .L_0200893c
	ldr r2, .L_02008944
	ldr r3, [r6, #8]
	adds r0, r6, #0
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r6, #12]
	adds r1, r5, #0
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r5, #8]
	bl Func_02001174
	cmp r0, #0
	bgt .L_0200893c
	ldr r2, .L_02008948
	ldr r3, [r6, #8]
	adds r0, r6, #0
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r6, #12]
	ldr r2, .L_02008944
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r1, r5, #0
	adds r3, r3, r2
	str r3, [r5, #8]
	bl Func_02001174
	cmp r0, #0
	bgt .L_0200893c
	mov r2, r8
	ldr r3, [r2, #16]
	adds r3, r3, r7
	str r3, [r2, #16]
	ldr r3, [r6, #16]
	adds r3, r3, r7
	str r3, [r6, #16]
.L_0200893c:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008944:
	.4byte 0x0005b333
.L_02008948:
	.4byte 0xfffa4ccd
	.section .text.x0200894c,"ax",%progbits
	.global Func_0200094c
	.thumb_func
Func_0200094c:
	push {r5, r6, lr}
	ldr r3, .L_020089b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, .L_020089b4
	movs r2, #7
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_020089a2
	ldr r2, [r6, #12]
	movs r3, #192
	lsls r3, r3, #11
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	movs r0, #14
	bl Func_0200115c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008998
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Func_0200114c
	ldr r1, .L_020089b8
	adds r0, r5, #0
	bl Func_02001154
.L_02008998:
	movs r0, #184
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02001284
.L_020089a2:
	movs r1, #217
	lsls r1, r1, #8
	adds r1, #153
	adds r0, r6, #0
	bl Func_020008a0
	pop {r5, r6, pc}
.L_020089b0:
	.4byte gPartyState
.L_020089b4:
	.4byte Data_0300122c
.L_020089b8:
	.4byte Data_0200128c
	.section .text.x020089bc,"ax",%progbits
	.global Func_020009bc
	.thumb_func
Func_020009bc:
	ldr r3, [r0, #24]
	ldr r2, .L_020089d0
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	movs r0, #0
	bx lr
	.2byte 0x0000
.L_020089d0:
	.4byte 0xfffff334
	.section .text.x020089d4,"ax",%progbits
	.global Func_020009d4
	.thumb_func
Func_020009d4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #9
	bl Object_GetById
	mov r9, r0
	movs r0, #64
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	mov r10, r3
	bl Func_0200119c
	movs r0, #0
	bl Func_0200125c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02001244
	movs r1, #1
	ldr r0, .L_02008d7c
	bl Func_0200123c
	movs r0, #60
	bl Func_0200124c
	movs r0, #168
	movs r1, #1
	movs r2, #158
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #26
	bl Func_02001284
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Object_SetModeById
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02001284
	movs r0, #60
	bl Battle_WaitMode0
	ldr r0, .L_02008d80
	bl Func_020011e4
	movs r2, #226
	lsls r2, r2, #1
	add r10, r2
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	movs r1, #168
	movs r2, #152
	lsls r1, r1, #16
	movs r0, #64
	lsls r2, r2, #16
	bl Func_020011cc
	adds r2, r6, #0
	movs r3, #0
	mov r8, r3
	adds r2, #85
	movs r3, #6
	strb r3, [r2]
	movs r2, #128
	ldr r3, [r6, #20]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r6, #20]
	movs r0, #220
	bl Func_02001284
	movs r0, #206
	movs r2, #0
	movs r3, #0
	movs r1, #0
	lsls r0, r0, #1
	bl Func_0200115c
	ldr r1, .L_02008d84
	adds r5, r0, #0
	bl Func_02001154
	adds r0, r5, #0
	movs r1, #1
	bl Animation_ApplyChildValues
	adds r3, r5, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r2, #128
	ldr r3, [r6, #8]
	lsls r2, r2, #12
	str r3, [r5, #8]
	movs r0, #15
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r6, #16]
	str r3, [r5, #16]
	ldr r3, [r6, #20]
	movs r6, #128
	str r3, [r5, #20]
	ldr r3, .L_02008d88
	lsls r6, r6, #9
	str r3, [r5, #108]
	bl Battle_WaitMode0
	mov r3, r8
	str r3, [r5, #8]
	str r3, [r5, #12]
	str r3, [r5, #16]
	mov r2, r10
	ldrh r0, [r2]
	str r3, [r5, #108]
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	ldr r3, .L_02008d8c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #166
	movs r2, #188
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPosition
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r1, [r5]
	movs r0, #9
	bl Func_020011d4
	mov r3, r9
	str r6, [r3, #48]
	movs r1, #184
	movs r2, #188
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #0
	bl Func_020011ec
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	mov r2, r10
	ldrh r0, [r2]
	movs r1, #0
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	negs r2, r2
	bl Func_0200118c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_020011fc
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_020011ec
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_020011fc
	movs r0, #30
	bl Battle_WaitMode0
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_020011fc
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_020011ec
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_020011fc
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_020011fc
	movs r0, #60
	bl Battle_WaitMode0
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	asrs r0, r0, #16
	movs r1, #0
	negs r2, r2
	bl Func_0200118c
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_020011fc
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_020011fc
	movs r0, #60
	bl Battle_WaitMode0
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	negs r2, r2
	movs r1, #0
	asrs r0, r0, #16
	bl Func_0200118c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_020011ec
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	mov r3, r10
	ldrh r0, [r3]
	mov r2, r10
	adds r3, r0, #1
	strh r3, [r2]
	lsls r0, r0, #16
	movs r2, #3
	negs r2, r2
	movs r1, #0
	asrs r0, r0, #16
	bl Func_0200118c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008d2e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_02008d2e:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl Func_020011cc
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #206
	bl GameFlag_SetBit
	bl Func_02001264
	adds r0, r6, #0
	movs r1, #0
	bl Func_0200123c
	movs r0, #60
	bl Func_0200124c
	ldr r0, [r5]
	movs r1, #1
	bl Func_02001214
	movs r0, #60
	bl Battle_WaitMode0
	bl Func_020011a4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_02008d7c:
	.4byte 0x00403108
.L_02008d80:
	.4byte 0x00001fe1
.L_02008d84:
	.4byte Data_020012a4
.L_02008d88:
	.4byte Func_020009bc
.L_02008d8c:
	.4byte gPartyState
	.section .text.x02008d90,"ax",%progbits
	.global Func_02000d90
	.thumb_func
Func_02000d90:
	push {lr}
	movs r0, #64
	bl Object_GetById
	cmp r0, #0
	beq .L_02008daa
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_02008daa
	movs r0, #130
	lsls r0, r0, #5
	bl Func_02001234
.L_02008daa:
	pop {pc}
	.section .text.x02008db4,"ax",%progbits
	.global Func_02000db4
	.thumb_func
Func_02000db4:
	push {lr}
	ldr r3, .L_02008dd0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008dd4
	movs r0, #0
	cmp r2, r3
	bne .L_02008dcc
	ldr r0, .L_02008dd8
.L_02008dcc:
	pop {pc}
	.2byte 0x0000
.L_02008dd0:
	.4byte gPartyState
.L_02008dd4:
	.4byte 0x0000007e
.L_02008dd8:
	.4byte Data_02001310
	.section .text.x02008de4,"ax",%progbits
	.global Func_02000de4
	.thumb_func
Func_02000de4:
	push {lr}
	ldr r3, .L_02008e28
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008e2c
	cmp r2, r3
	bne .L_02008dfc
	ldr r0, .L_02008e30
	b .L_02008e26
.L_02008dfc:
	ldr r3, .L_02008e34
	cmp r2, r3
	bne .L_02008e06
	ldr r0, .L_02008e38
	b .L_02008e26
.L_02008e06:
	ldr r3, .L_02008e3c
	cmp r2, r3
	bne .L_02008e10
	ldr r0, .L_02008e40
	b .L_02008e26
.L_02008e10:
	ldr r3, .L_02008e44
	cmp r2, r3
	bne .L_02008e1a
	ldr r0, .L_02008e48
	b .L_02008e26
.L_02008e1a:
	ldr r3, .L_02008e4c
	cmp r2, r3
	bne .L_02008e24
	ldr r0, .L_02008e50
	b .L_02008e26
.L_02008e24:
	ldr r0, .L_02008e54
.L_02008e26:
	pop {pc}
.L_02008e28:
	.4byte gPartyState
.L_02008e2c:
	.4byte 0x0000007e
.L_02008e30:
	.4byte Data_02001390
.L_02008e34:
	.4byte 0x0000007f
.L_02008e38:
	.4byte Data_020013a8
.L_02008e3c:
	.4byte 0x00000080
.L_02008e40:
	.4byte Data_020013d8
.L_02008e44:
	.4byte 0x00000081
.L_02008e48:
	.4byte Data_02001498
.L_02008e4c:
	.4byte 0x00000082
.L_02008e50:
	.4byte Data_02001510
.L_02008e54:
	.4byte Data_02001378
	.section .text.x02008e58,"ax",%progbits
	.global Func_02000e58
	.thumb_func
Func_02000e58:
	push {lr}
	ldr r3, .L_02008e9c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008ea0
	cmp r2, r3
	bne .L_02008e70
	ldr r0, .L_02008ea4
	b .L_02008e9a
.L_02008e70:
	ldr r3, .L_02008ea8
	cmp r2, r3
	bne .L_02008e7a
	ldr r0, .L_02008eac
	b .L_02008e9a
.L_02008e7a:
	ldr r3, .L_02008eb0
	cmp r2, r3
	bne .L_02008e84
	ldr r0, .L_02008eb4
	b .L_02008e9a
.L_02008e84:
	ldr r3, .L_02008eb8
	cmp r2, r3
	bne .L_02008e8e
	ldr r0, .L_02008ebc
	b .L_02008e9a
.L_02008e8e:
	ldr r3, .L_02008ec0
	cmp r2, r3
	bne .L_02008e98
	ldr r0, .L_02008ec4
	b .L_02008e9a
.L_02008e98:
	ldr r0, .L_02008ec8
.L_02008e9a:
	pop {pc}
.L_02008e9c:
	.4byte gPartyState
.L_02008ea0:
	.4byte 0x0000007e
.L_02008ea4:
	.4byte Data_0200154c
.L_02008ea8:
	.4byte 0x0000007f
.L_02008eac:
	.4byte Data_02001594
.L_02008eb0:
	.4byte 0x00000080
.L_02008eb4:
	.4byte Data_020015c4
.L_02008eb8:
	.4byte 0x00000081
.L_02008ebc:
	.4byte Data_020016cc
.L_02008ec0:
	.4byte 0x00000082
.L_02008ec4:
	.4byte Data_02001714
.L_02008ec8:
	.4byte Data_02001540
	.section .text.x02008ecc,"ax",%progbits
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	movs r0, #0
	sub sp, #8
	bl Func_02001254
	ldr r5, .L_02009118
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200911c
	cmp r2, r3
	bne .L_02008f4e
	adds r1, #2
	adds r3, r5, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #1
	bne .L_02008f4e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f2a
	ldr r3, .L_02009120
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r2, r5, r0
	adds r1, #98
	strh r3, [r2]
	adds r3, r5, r1
	strh r6, [r3]
	b .L_02008f4e
.L_02008f2a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f4e
	ldr r2, .L_02009124
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r3, r5, r0
	adds r1, #98
	strh r2, [r3]
	adds r2, r5, r1
	movs r3, #3
	strh r3, [r2]
.L_02008f4e:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f84
	ldr r1, .L_02009118
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009128
	cmp r2, r3
	bne .L_02008f84
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #2
	ble .L_02008f84
	adds r2, #50
	adds r3, r1, r2
	ldr r0, [r3]
	bl Func_0200126c
.L_02008f84:
	ldr r3, .L_02009118
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200912c
	cmp r2, r3
	bne .L_0200907e
	adds r0, #32
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fb4
	movs r3, #8
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_02008fb4:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fd6
	movs r3, #8
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_02008fd6:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ff8
	movs r3, #9
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_02008ff8:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200901a
	movs r3, #8
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_0200901a:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200903a
	movs r3, #6
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_0200903a:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200905c
	movs r3, #5
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_0200905c:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200907e
	movs r3, #6
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_0200907e:
	ldr r3, .L_02009118
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009130
	cmp r2, r3
	bne .L_020090b0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090b0
	movs r3, #17
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #18
	movs r2, #1
	movs r3, #1
	bl Func_02001164
.L_020090b0:
	ldr r3, .L_02009118
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009134
	cmp r2, r3
	bne .L_02009112
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #206
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009112
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #147
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009112
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009112
	movs r0, #64
	bl Object_GetById
	movs r1, #168
	movs r2, #152
	adds r5, r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #64
	bl Func_020011cc
	adds r2, r5, #0
	movs r3, #6
	adds r2, #85
	strb r3, [r2]
	movs r1, #128
	ldr r3, [r5, #20]
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5, #20]
.L_02009112:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
.L_02009118:
	.4byte gPartyState
.L_0200911c:
	.4byte 0x0000007e
.L_02009120:
	.4byte 0x0000007b
.L_02009124:
	.4byte 0x00000078
.L_02009128:
	.4byte 0x0000007f
.L_0200912c:
	.4byte 0x00000080
.L_02009130:
	.4byte 0x00000082
.L_02009134:
	.4byte 0x00000081
	.section .rodata.x0200928c,"a",%progbits
	.global Data_0200128c
Data_0200128c:
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_020012a4
Data_020012a4:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001310
Data_02001310:
	.4byte 0x002e0184
	.4byte 0x018c0114
	.4byte 0x011c0036
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x0000007e
	.4byte 0x0010207c
	.4byte 0x0020107f
	.4byte 0x0030307f
	.4byte 0x0040407f
	.4byte 0x0050507f
	.4byte 0x0000007f
	.4byte 0x0010207e
	.4byte 0x00201080
	.4byte 0x00000080
	.4byte 0x0010207f
	.4byte 0x00201082
	.4byte 0x00000081
	.4byte 0x00102082
	.4byte 0x00000082
	.4byte 0x00102080
	.4byte 0x00201081
	.4byte 0x000001ff
	.global Data_02001378
Data_02001378:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001390
Data_02001390:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020013a8
Data_020013a8:
	.4byte 0x003a00f3
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020013d8
Data_020013d8:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
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
	.global Data_02001498
Data_02001498:
	.4byte 0xffff0177
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001510
Data_02001510:
	.4byte 0xffff0122
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
	.global Data_02001540
Data_02001540:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200154c
Data_0200154c:
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte Func_02000044
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte Func_0200018c
	.4byte 0x50008905
	.4byte 0xffff002c
	.4byte Func_020001b0
	.4byte 0x50008905
	.4byte 0xffff002d
	.4byte Func_020001d4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001594
Data_02001594:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_02000038
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020015c4
Data_020015c4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_0200066c
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_0200069c
	.4byte 0x50008905
	.4byte 0xffff0028
	.4byte Func_02000060
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte Func_02000080
	.4byte 0x50008905
	.4byte 0xffff002a
	.4byte Func_020001e0
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte Func_02000260
	.4byte 0x50008905
	.4byte 0xffff002c
	.4byte Func_020002e0
	.4byte 0x50008905
	.4byte 0xffff002d
	.4byte Func_02000364
	.4byte 0x50008905
	.4byte 0xffff002e
	.4byte Func_020003e4
	.4byte 0x50008905
	.4byte 0xffff002f
	.4byte Func_02000464
	.4byte 0x50008905
	.4byte 0xffff0030
	.4byte Func_020004e4
	.4byte 0x50008905
	.4byte 0xffff0031
	.4byte Func_02000100
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte Func_020006cc
	.4byte 0x00001815
	.4byte 0x02010009
	.4byte Func_02000704
	.4byte 0x00001815
	.4byte 0x0202000a
	.4byte Func_02000740
	.4byte 0x00001815
	.4byte 0x0203000b
	.4byte Func_0200077c
	.4byte 0x00001815
	.4byte 0x0204000c
	.4byte Func_020007b8
	.4byte 0x00001815
	.4byte 0x0205000d
	.4byte Func_020007f0
	.4byte 0x00001815
	.4byte 0x0206000e
	.4byte Func_0200082c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020016cc
Data_020016cc:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x09ce0028
	.4byte Func_020009d4
	.4byte 0x00000000
	.4byte 0x0f930008
	.4byte Func_02000d90
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte Func_020005e8
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte Func_02000608
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001714
Data_02001714:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50008905
	.4byte 0xffff0028
	.4byte Func_020000a0
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte Func_020000c0
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte Func_020000e0
	.4byte 0x50008905
	.4byte 0xffff002a
	.4byte Func_02000564
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte Func_0200094c
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte Func_02000868
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
