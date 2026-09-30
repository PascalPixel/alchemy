.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_0200205c
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {lr}
	ldr r3, .L_0200805c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008060
	movs r0, #0
	cmp r2, r3
	bne .L_02008058
	ldr r0, .L_02008064
.L_02008058:
	pop {pc}
	.2byte 0x0000
.L_0200805c:
	.4byte gPartyState
.L_02008060:
	.4byte 0x00000078
.L_02008064:
	.4byte Data_0200a08c
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	ldr r0, .L_0200806c
	bx lr
.L_0200806c:
	.4byte Data_020020ac
	.section .text.x02008070,"ax",%progbits
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {lr}
	ldr r3, .L_02008098
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200809c
	cmp r2, r3
	bne .L_02008088
	ldr r0, .L_020080a0
	b .L_02008094
.L_02008088:
	ldr r3, .L_020080a4
	cmp r2, r3
	bne .L_02008092
	ldr r0, .L_020080a8
	b .L_02008094
.L_02008092:
	ldr r0, .L_020080ac
.L_02008094:
	pop {pc}
	.2byte 0x0000
.L_02008098:
	.4byte gPartyState
.L_0200809c:
	.4byte 0x00000078
.L_020080a0:
	.4byte Data_020020f8
.L_020080a4:
	.4byte 0x0000007c
.L_020080a8:
	.4byte Data_020021e8
.L_020080ac:
	.4byte Data_02002458
	.section .text.x020080b0,"ax",%progbits
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	push {lr}
	ldr r3, .L_020080e0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080e4
	cmp r2, r3
	bne .L_020080c8
	ldr r0, .L_020080e8
	b .L_020080de
.L_020080c8:
	ldr r3, .L_020080ec
	cmp r2, r3
	bne .L_020080d2
	ldr r0, .L_020080f0
	b .L_020080de
.L_020080d2:
	ldr r3, .L_020080f4
	cmp r2, r3
	bne .L_020080dc
	ldr r0, .L_020080f8
	b .L_020080de
.L_020080dc:
	ldr r0, .L_020080fc
.L_020080de:
	pop {pc}
.L_020080e0:
	.4byte gPartyState
.L_020080e4:
	.4byte 0x00000078
.L_020080e8:
	.4byte Data_02002470
.L_020080ec:
	.4byte 0x0000007a
.L_020080f0:
	.4byte Data_020024a0
.L_020080f4:
	.4byte 0x0000007c
.L_020080f8:
	.4byte Data_020024dc
.L_020080fc:
	.4byte Data_02002530
	.section .text.x02008100,"ax",%progbits
	.global Func_02000100
	.thumb_func
Func_02000100:
	push {r5, lr}
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001ec0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	adds r0, #3
	bl Func_02001eb8
	movs r0, #20
	bl Func_02001ec8
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02008170
	ldr r3, .L_0200816c
	movs r0, #1
	strh r3, [r5]
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r0, [r3]
	movs r1, #0
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	movs r2, #2
	bl Func_02001d88
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Func_02001eb8
	movs r0, #20
	bl Func_02001ec8
	movs r0, #20
	b .L_02008174
	.2byte 0x0000
.L_0200816c:
	.4byte 0x00007fff
.L_02008170:
	.4byte 0x0500021e
.L_02008174:
	bl WaitFrames
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008184,"ax",%progbits
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	movs r0, #26
	bl Func_02001f10
	movs r0, #212
	bl Func_02001f10
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001ec0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001eb8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_02001ec0
	movs r1, #1
	ldr r0, .L_020083b4
	bl Func_02001eb8
	movs r0, #40
	bl Func_02001ec8
	movs r0, #40
	bl WaitFrames
	movs r0, #197
	bl Func_02001f10
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020083b8
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008222
	movs r5, #0
.L_020081f0:
	cmp r5, #20
	bne .L_02008200
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02001e88
.L_02008200:
	ldr r6, .L_020083b8
	movs r1, #128
	ldr r2, [r6]
	lsls r1, r1, #8
	ldr r3, [r2, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r2, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #79
	bls .L_020081f0
	ldr r3, [r6]
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
.L_02008222:
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	ldr r3, .L_020083bc
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	ldr r0, [r6]
	lsls r1, r1, #1
	movs r2, #108
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r1, [r6]
	movs r0, #9
	bl Func_02001e20
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #182
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #124
	bl ObjectMotion_SetPositionAndReset
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	ldr r0, .L_020083c0
	bl Func_02001e48
	bl Func_02000100
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	bl Func_02001e60
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_02001e90
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02001e70
	movs r2, #20
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	bl Func_02001e60
	bl Func_02000100
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r6]
	bl Func_02001e88
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_02001e88
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #6
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	ldrh r3, [r0, #6]
	ldr r1, .L_020083b0
	strh r3, [r5, #6]
	movs r0, #1
	mov r8, r1
	bl WaitFrames
	movs r0, #14
	movs r1, #10
	bl Func_02001e20
	movs r2, #0
	movs r1, #0
	movs r0, #10
	bl Func_02001e10
	movs r0, #110
	bl Func_02001f10
	movs r1, #5
	movs r0, #14
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	movs r2, #0
	b .L_020083c4
	.2byte 0x0000
.L_020083b0:
	.4byte 0x00000000
.L_020083b4:
	.4byte 0x00403108
.L_020083b8:
	.4byte Data_02002058
.L_020083bc:
	.4byte gPartyState
.L_020083c0:
	.4byte 0x00001fb5
.L_020083c4:
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Object_SetModeById
	movs r0, #110
	bl Func_02001f10
	movs r1, #6
	movs r0, #14
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #14
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02000100
	movs r0, #14
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #14
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #14
	movs r1, #0
	bl Func_02001e60
	ldr r1, .L_0200859c
	movs r0, #14
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #14
	movs r1, #0
	bl Func_02001e60
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_020085a0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #164
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #14
	bl Func_02001e88
	movs r2, #20
	movs r0, #14
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #14
	movs r1, #0
	bl Func_02001e60
	movs r1, #196
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #220
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02001e10
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #12
	bl Object_GetById
	mov r2, r8
	adds r0, #85
	strb r2, [r0]
	ldr r1, .L_020085a4
	movs r0, #12
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02000100
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r6]
	bl Func_02001e88
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_02001e88
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #80
	movs r0, #9
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02000100
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	ldr r0, [r6]
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001ec0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	adds r0, #3
	bl Func_02001eb8
	movs r0, #10
	bl Func_02001ec8
	movs r0, #10
	bl WaitFrames
	ldr r5, .L_020085a8
	ldr r3, .L_02008598
	movs r7, #192
	strh r3, [r5]
	movs r0, #1
	lsls r7, r7, #18
	bl WaitFrames
	ldr r3, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r0, [r3]
	movs r1, #0
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	movs r2, #2
	bl Func_02001d88
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Func_02001eb8
	movs r0, #20
	bl Func_02001ec8
	movs r0, #20
	b .L_020085ac
.L_02008598:
	.4byte 0x00007fff
.L_0200859c:
	.4byte Data_02001f78
.L_020085a0:
	.4byte 0x00019999
.L_020085a4:
	.4byte Data_02001f9c
.L_020085a8:
	.4byte 0x0500021e
.L_020085ac:
	bl WaitFrames
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	ldr r0, [r6]
	bl Func_02001e88
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r6]
	bl Func_02001e88
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #9
	bl Func_02001e88
	bl Func_02000100
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl ObjectMotion_ArmCallback
	ldr r1, .L_02008948
	movs r0, #12
	bl Object_SetActionCallbackAndRefreshById
	bl Func_02000100
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001e90
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_02001e90
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	bl Func_02000100
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #13
	bl Func_02001e88
	bl Func_02000100
	movs r0, #9
	movs r1, #0
	movs r2, #40
	bl Func_02001e58
	bl Func_02000100
	movs r2, #10
	movs r0, #9
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	bl Func_02000100
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	bl Func_02000100
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02001e60
	bl Func_02000100
	movs r0, #0
	bl Func_02001f10
	movs r0, #220
	bl Func_02001f10
	movs r1, #3
	movs r0, #8
	bl Object_SetModeById
	movs r0, #13
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r1, #212
	movs r2, #208
	lsls r2, r2, #15
	movs r0, #13
	lsls r1, r1, #17
	bl Func_02001e10
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02001eb8
	movs r0, #40
	bl Func_02001ec8
	movs r0, #40
	bl WaitFrames
	bl Func_02001094
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl Func_02001e10
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02001eb8
	movs r1, #1
	ldr r0, .L_0200894c
	bl Func_02001eb8
	movs r0, #60
	bl Func_02001ec8
	movs r0, #60
	bl WaitFrames
	bl Func_02000100
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02001eb8
	movs r0, #20
	bl Func_02001ec8
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02008950
	movs r2, #3
	ldr r3, [r5]
	movs r0, #10
	adds r3, #85
	strb r2, [r3]
	bl Battle_WaitMode0
	movs r0, #54
	adds r0, #255
	bl Func_02001f10
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #25
	bl Func_02001f10
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #196
	movs r2, #104
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02001e70
	ldr r0, [r6]
	movs r1, #28
	bl Object_SetModeById
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02008868
	ldr r0, [r6]
	bl Object_GetById
	ldr r4, [r5]
	mov r2, r8
	adds r3, r4, #0
	adds r3, #85
	strb r2, [r3]
	movs r3, #144
	ldr r2, [r0, #12]
	lsls r3, r3, #14
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	adds r0, r4, #0
	bl Func_02001d48
	ldr r0, [r5]
	bl Func_02001d50
.L_02008868:
	movs r0, #242
	movs r1, #0
	bl PartyInventory_GiveItem
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0200887a
	bl Func_02001d30
.L_0200887a:
	ldr r0, [r6]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020088ce
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02001e60
	ldr r2, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020088ec
.L_020088ce:
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	movs r1, #3
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
.L_020088ec:
	movs r1, #188
	movs r2, #124
	lsls r1, r1, #1
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e70
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	ldr r3, .L_02008954
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008958
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008972
.L_02008948:
	.4byte Data_02001fe4
.L_0200894c:
	.4byte 0x00403108
.L_02008950:
	.4byte Data_02002058
.L_02008954:
	.4byte gPartyState
.L_02008958:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02001e60
.L_02008972:
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	ldr r7, .L_02008a84
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r7, r2
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	movs r0, #9
	movs r1, #0
	bl Func_02001e60
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r5, .L_02008a88
	movs r0, #9
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	adds r1, r5, #0
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #0
	bl Func_02001eb8
	movs r6, #192
	movs r0, #60
	bl Func_02001ec8
	lsls r6, r6, #18
	movs r0, #64
	bl WaitFrames
	ldr r1, [r6, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r5, #218
	adds r3, #85
	str r3, [r2]
	lsls r5, r5, #1
	movs r3, #10
	str r3, [r1, r5]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02001eb8
	ldr r3, [r6, #108]
	movs r6, #1
	str r6, [r3, r5]
	movs r0, #1
	bl Func_02001ec8
	bl Func_02001d78
	movs r1, #0
	movs r2, #3
	ldr r0, .L_02008a8c
	bl Func_02001d88
	bl Func_02001d80
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_ClearBit
	ldr r2, .L_02008a90
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r7, r1
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #98
	adds r3, r7, r2
	ldr r2, .L_02008a94
	subs r1, #124
	strh r6, [r3]
	adds r3, r7, r1
	strh r2, [r3]
	movs r3, #243
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #6
	strh r3, [r2]
	movs r0, #3
	bl Func_02001eb0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008a84:
	.4byte gPartyState
.L_02008a88:
	.4byte Data_0200200c
.L_02008a8c:
	.4byte 0x00001fdf
.L_02008a90:
	.4byte 0x0000006e
.L_02008a94:
	.4byte 0x00000070
	.section .text.x02008a98,"ax",%progbits
	.global Func_02000a98
	.thumb_func
Func_02000a98:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r5, .L_02008b58
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #133
	ldr r3, [r3, #108]
	lsls r2, r2, #2
	adds r5, r5, r2
	adds r6, r0, #0
	ldr r0, [r5]
	mov r8, r1
	mov r9, r3
	bl Object_GetById
	mov r10, r0
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	mov r2, r10
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	adds r1, r6, #0
	mov r2, r8
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	bl Object_RefreshSelectorById
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	movs r1, #13
	bl Object_SetModeById
	lsls r6, r6, #16
	mov r3, r8
	lsls r3, r3, #16
	ldr r2, .L_02008b5c
	adds r1, r6, #0
	mov r0, r10
	mov r8, r3
	bl Func_02001d48
	mov r0, r10
	bl Func_02001d50
	movs r1, #10
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #123
	bl Func_02001f10
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	add r9, r2
	mov r2, r9
	movs r3, #0
	ldrsh r0, [r2, r3]
	bl Func_02001eb0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_02008b58:
	.4byte gPartyState
.L_02008b5c:
	.4byte 0xfff40000
	.section .text.x02008b60,"ax",%progbits
	.global Func_02000b60
	.thumb_func
Func_02000b60:
	push {lr}
	movs r0, #156
	lsls r0, r0, #1
	movs r1, #144
	bl Func_02000a98
	pop {pc}
	.2byte 0x0000
	.section .text.x02008b70,"ax",%progbits
	.global Func_02000b70
	.thumb_func
Func_02000b70:
	push {lr}
	ldr r3, .L_02008be0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r4, #26
	bne .L_02008bae
	cmp r1, #5
	bne .L_02008b9c
	ldr r3, .L_02008be4
	movs r2, #128
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02008bdc
.L_02008b9c:
	cmp r1, #7
	bne .L_02008bd2
	ldr r3, .L_02008be4
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008bd2
	b .L_02008bdc
.L_02008bae:
	cmp r1, #6
	bne .L_02008bd2
	cmp r4, #25
	bne .L_02008bc2
	ldr r3, .L_02008be4
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02008bdc
.L_02008bc2:
	cmp r4, #27
	bne .L_02008bd2
	ldr r3, .L_02008be4
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02008bdc
.L_02008bd2:
	movs r0, #212
	lsls r0, r0, #1
	movs r1, #100
	bl Func_02000a98
.L_02008bdc:
	pop {pc}
	.2byte 0x0000
.L_02008be0:
	.4byte gPartyState
.L_02008be4:
	.4byte gInput
	.section .text.x02008be8,"ax",%progbits
	.global Func_02000be8
	.thumb_func
Func_02000be8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	movs r2, #188
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r1, r1, r3
	adds r2, r2, r3
	mov r10, r1
	mov r8, r2
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	bl Func_02001d60
	movs r7, #1
	movs r6, #0
.L_02008c18:
	adds r3, r6, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02008c3e
	adds r1, r7, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02008c2a
	adds r3, r7, #4
.L_02008c2a:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r7, r1, #0
.L_02008c3e:
	mov r1, r10
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	adds r6, #1
	adds r3, r3, r2
	movs r2, #176
	lsls r2, r2, #1
	str r3, [r1, #12]
	cmp r6, r2
	blt .L_02008c18
	bl Func_02001d40
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r5, .L_02008d34
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #184
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #154
	lsls r0, r0, #1
	bl Func_02001f10
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	cmp r6, #0
	beq .L_02008cf2
.L_02008cae:
	adds r3, r6, #0
	subs r3, #33
	cmp r3, #62
	bhi .L_02008cd4
	adds r3, r7, #1
	adds r2, r3, #0
	cmp r3, #0
	bge .L_02008cc0
	adds r2, r7, #4
.L_02008cc0:
	asrs r1, r2, #2
	movs r3, #16
	subs r3, r3, r1
	lsls r2, r1, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	subs r7, #1
.L_02008cd4:
	mov r1, r10
	ldr r3, [r1, #12]
	ldr r2, .L_02008d38
	movs r0, #1
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	subs r6, #1
	adds r3, r3, r2
	str r3, [r1, #12]
	bl WaitFrames
	cmp r6, #0
	bne .L_02008cae
.L_02008cf2:
	ldr r3, .L_02008d34
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001f10
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_02001f10
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d34:
	.4byte gPartyState
.L_02008d38:
	.4byte 0xffff0000
	.section .text.x02008d3c,"ax",%progbits
	.global Func_02000d3c
	.thumb_func
Func_02000d3c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #188
	adds r1, r1, r3
	lsls r2, r2, #1
	adds r7, r3, r2
	mov r8, r1
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	bl Func_02001d60
	ldr r5, .L_02008e4c
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	negs r2, r2
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02001e70
	movs r0, #154
	lsls r0, r0, #1
	bl Func_02001f10
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r6, #1
	movs r5, #0
.L_02008dae:
	adds r3, r5, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02008dd4
	adds r1, r6, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02008dc0
	adds r3, r6, #4
.L_02008dc0:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r6, r1, #0
.L_02008dd4:
	mov r1, r8
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	bl WaitFrames
	movs r2, #96
	adds r5, #1
	adds r2, #255
	cmp r5, r2
	ble .L_02008dae
	ldr r5, .L_02008e4c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #1
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001f10
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_02001f10
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #1
	adds r0, #34
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e4c:
	.4byte gPartyState
	.section .text.x02008e50,"ax",%progbits
	.global Func_02000e50
	.thumb_func
Func_02000e50:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008e60,"ax",%progbits
	.global Func_02000e60
	.thumb_func
Func_02000e60:
	push {lr}
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	movs r0, #196
	movs r1, #1
	movs r2, #240
	lsls r2, r2, #15
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02001ea8
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_02008eec
	bl Func_02001e48
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	bl Func_02001e90
	movs r0, #10
	movs r1, #0
	bl Func_02001e60
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #10
	movs r1, #0
	bl Func_02001e60
	movs r0, #10
	movs r1, #0
	bl Func_02001e70
	movs r0, #10
	movs r1, #0
	bl Func_02001e60
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #10
	bl Func_02001e60
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {pc}
.L_02008eec:
	.4byte 0x00001faf
	.section .text.x02008ef0,"ax",%progbits
	.global Func_02000ef0
	.thumb_func
Func_02000ef0:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	str r3, [r5, #8]
	pop {r5, pc}
	.section .text.x02008f00,"ax",%progbits
	.global Func_02000f00
	.thumb_func
Func_02000f00:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	movs r2, #144
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	movs r0, #234
	adds r0, #255
	lsls r2, r2, #13
	ldr r5, .L_02008f80
	bl Func_02001d28
	movs r7, #0
	str r0, [r5]
	cmp r0, #0
	beq .L_02008f7e
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	adds r3, #85
	adds r2, r0, #0
	strb r7, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	ldr r3, .L_02008f84
	movs r1, #193
	str r3, [r0, #108]
	lsls r1, r1, #3
	strb r7, [r6, #26]
	strb r7, [r6, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #230
	bl Func_02001d90
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_02008f7e:
	pop {r5, r6, r7, pc}
.L_02008f80:
	.4byte Data_02002058
.L_02008f84:
	.4byte Func_02000ef0
	.section .text.x02008f88,"ax",%progbits
	.global Func_02000f88
	.thumb_func
Func_02000f88:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	movs r2, #144
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	movs r0, #234
	adds r0, #255
	lsls r2, r2, #13
	ldr r5, .L_02009008
	bl Func_02001d28
	movs r7, #0
	str r0, [r5]
	cmp r0, #0
	beq .L_02009006
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	adds r3, #85
	adds r2, r0, #0
	strb r7, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	ldr r3, .L_0200900c
	movs r1, #193
	str r3, [r0, #108]
	lsls r1, r1, #3
	strb r7, [r6, #26]
	strb r7, [r6, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #242
	bl Func_02001d90
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_02009006:
	pop {r5, r6, r7, pc}
.L_02009008:
	.4byte Data_02002058
.L_0200900c:
	.4byte Func_02000ef0
	.section .text.x02009010,"ax",%progbits
	.global Func_02001010
	.thumb_func
Func_02001010:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #11
	str r3, [sp, #4]
	movs r6, #5
	movs r0, #85
	movs r1, #78
	movs r2, #22
	movs r3, #79
	str r6, [sp, #0]
	bl Func_02001d58
	movs r3, #22
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #78
	movs r2, #5
	movs r3, #11
	bl Func_02001d68
	movs r5, #2
	movs r0, #86
	movs r1, #30
	movs r2, #86
	movs r3, #20
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001d58
	movs r3, #9
	str r3, [sp, #0]
	movs r0, #86
	movs r1, #30
	movs r2, #84
	movs r3, #22
	str r5, [sp, #4]
	bl Func_02001d58
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02009064,"ax",%progbits
	.global Func_02001064
	.thumb_func
Func_02001064:
	push {r5, lr}
	sub sp, #8
	movs r3, #23
	str r3, [sp, #0]
	movs r5, #9
	movs r0, #23
	movs r1, #1
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001d68
	movs r3, #25
	str r3, [sp, #0]
	movs r0, #23
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001d68
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009094,"ax",%progbits
	.global Func_02001094
	.thumb_func
Func_02001094:
	push {r5, lr}
	sub sp, #8
	movs r5, #1
	movs r0, #0
	movs r1, #1
	movs r2, #26
	movs r3, #6
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001d58
	movs r0, #0
	movs r1, #65
	movs r2, #26
	movs r3, #70
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001d58
	movs r3, #25
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #2
	movs r2, #3
	movs r3, #3
	bl Func_02001d68
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020090d4,"ax",%progbits
	.global Func_020010d4
	.thumb_func
Func_020010d4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #13
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #13
	bl Object_GetById
	movs r1, #196
	movs r3, #134
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02009514
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	ldr r0, [r2]
	mov r10, r2
	movs r3, #224
	movs r1, #180
	movs r2, #184
	lsls r3, r3, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	mov r8, r3
	bl Func_02001e18
	movs r1, #188
	movs r2, #184
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #16
	mov r3, r8
	bl Func_02001e18
	movs r1, #180
	movs r2, #168
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #16
	mov r3, r8
	bl Func_02001e18
	movs r2, #160
	lsls r2, r2, #8
	mov r9, r2
	movs r2, #168
	movs r0, #6
	ldr r1, .L_02009518
	lsls r2, r2, #16
	mov r3, r9
	bl Func_02001e18
	movs r1, #204
	movs r2, #184
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #16
	mov r3, r9
	bl Func_02001e18
	cmp r7, #0
	beq .L_020091d4
	ldr r6, [r7, #80]
	movs r3, #0
	ldrb r2, [r6, #5]
	strb r3, [r6, #26]
	strb r3, [r6, #27]
	subs r3, #33
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r7, #0
	adds r2, #92
	strb r3, [r6, #9]
	movs r1, #193
	movs r3, #1
	strb r3, [r2]
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #242
	bl Func_02001d90
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_020091d4:
	movs r6, #192
	lsls r6, r6, #18
	bl Func_02001010
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #1
	str r5, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #10
	movs r1, #4
	movs r0, #7
	bl ObjectMotion_Launch
	ldr r0, .L_0200951c
	bl Func_02001e48
	movs r0, #7
	movs r1, #0
	bl Func_02001e60
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_02001e90
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02001e60
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_02001e88
	movs r1, #0
	movs r0, #5
	bl Func_02001e60
	movs r0, #13
	bl Object_GetById
	movs r1, #196
	movs r3, #170
	movs r2, #0
	lsls r3, r3, #17
	lsls r1, r1, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #78
	bl Func_02001f10
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #2
	bl Func_02001eb8
	movs r0, #40
	bl Func_02001ec8
	movs r0, #40
	bl WaitFrames
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #10
	movs r1, #2
	bl Object_SetModeById
	mov r3, r10
	ldr r0, [r3]
	movs r1, #13
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #13
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #13
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #13
	bl Object_LinkObjectAndSetCallback
	movs r1, #13
	movs r0, #11
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02001e98
	movs r0, #196
	movs r1, #1
	movs r2, #246
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #13
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	movs r2, #246
	movs r0, #13
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #9
	movs r1, #1
	bl Object_SetModeById
	movs r0, #10
	movs r1, #1
	bl Object_SetModeById
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #40]
	movs r2, #204
	movs r3, #128
	lsls r3, r3, #8
	lsls r2, r2, #8
	str r3, [r7, #72]
	movs r0, #13
	ldr r1, .L_02009520
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	lsls r1, r1, #1
	movs r2, #234
	movs r0, #13
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #196
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #13
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	lsls r1, r1, #1
	movs r0, #13
	movs r2, #160
	bl ObjectMotion_SetPositionAndCommit
	mov r2, r10
	ldr r0, [r2]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #11
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl Func_02001e10
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02001eb8
	movs r0, #40
	bl Func_02001ec8
	movs r0, #40
	bl WaitFrames
	bl Func_02001ef0
	movs r2, #40
	adds r1, r5, #0
	movs r0, #11
	bl Func_02001e88
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_02001e88
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02001e60
	movs r0, #5
	mov r1, r8
	bl Func_02001e70
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02001e60
	movs r0, #5
	mov r1, r9
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #6
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #200
	movs r2, #172
	movs r0, #6
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #9
	movs r0, #6
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02001e60
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02001e60
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02001e60
	movs r2, #0
	adds r1, r5, #0
	movs r0, #11
	bl Func_02001e88
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_02001e70
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	mov r3, r10
	ldr r0, [r3]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #0
	bl Func_02001f10
	movs r3, #208
	movs r1, #200
	movs r2, #160
	lsls r3, r3, #8
	lsls r2, r2, #17
	lsls r1, r1, #17
	movs r0, #12
	bl Func_02001e18
	movs r0, #1
	bl WaitFrames
	movs r0, #12
	movs r1, #0
	bl Func_02001e60
	movs r1, #1
	movs r0, #6
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	mov r2, r10
	movs r1, #128
	ldr r0, [r2]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #11
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #204
	lsls r1, r1, #6
	ldr r0, .L_02009520
	adds r1, #51
	bl Func_02001e98
	movs r0, #196
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	b .L_02009524
.L_02009514:
	.4byte gPartyState
.L_02009518:
	.4byte 0x01a70000
.L_0200951c:
	.4byte 0x00001f60
.L_02009520:
	.4byte 0x00019999
.L_02009524:
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_02009700
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #202
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #220
	bl ObjectMotion_SetPositionAndReset
	movs r0, #12
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r1, #0
	movs r0, #12
	bl UiText_OpenMessageAtObject
	mov r3, r10
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200958c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl Func_02001e90
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	bl Func_02001e60
	ldr r2, [r6, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020095ae
.L_0200958c:
	ldr r2, [r6, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #4
	adds r3, #1
	strh r3, [r2]
	adds r1, #255
	movs r0, #12
	movs r2, #20
	bl Func_02001e88
	movs r0, #12
	movs r1, #0
	bl Func_02001e60
.L_020095ae:
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_02001e88
	movs r0, #7
	movs r1, #0
	bl Func_02001e60
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #12
	bl Func_02001e88
	movs r0, #12
	movs r1, #0
	bl Func_02001e60
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_02001e88
	movs r0, #6
	movs r1, #0
	bl Func_02001e60
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl Func_02001e90
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	bl Func_02001e60
	ldr r3, .L_02009704
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02001e60
	movs r1, #6
	adds r1, #255
	movs r2, #20
	movs r0, #5
	bl Func_02001e88
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #210
	movs r2, #228
	movs r0, #5
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #5
	bl Func_02001e70
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009708
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #206
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #222
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #12
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200970c
	movs r0, #5
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #12
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #86
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #2
	bl Func_02001eb0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009700:
	.4byte 0x00013333
.L_02009704:
	.4byte gPartyState
.L_02009708:
	.4byte 0x00019999
.L_0200970c:
	.4byte Data_02001f18
	.section .text.x02009710,"ax",%progbits
	.global Func_02001710
	.thumb_func
Func_02001710:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	bl Func_02001010
	bl Func_02001d40
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_02009a94
	movs r2, #133
	lsls r2, r2, #2
	movs r5, #224
	adds r6, r6, r2
	lsls r5, r5, #8
	movs r1, #180
	movs r2, #216
	adds r3, r5, #0
	ldr r0, [r6]
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001e18
	movs r1, #188
	movs r2, #216
	adds r3, r5, #0
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001e18
	movs r1, #180
	movs r2, #200
	movs r3, #0
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r5, #160
	mov r10, r3
	lsls r5, r5, #8
	bl Func_02001e18
	movs r1, #204
	movs r2, #216
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_02001e18
	movs r2, #128
	lsls r2, r2, #8
	mov r9, r2
	movs r1, #212
	movs r2, #200
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #16
	mov r3, r9
	bl Func_02001e18
	movs r3, #176
	movs r1, #196
	movs r2, #200
	lsls r1, r1, #17
	lsls r3, r3, #8
	lsls r2, r2, #16
	movs r0, #12
	movs r7, #192
	bl Func_02001e18
	lsls r7, r7, #18
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	mov r8, r2
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #208
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #7
	bl Func_02001e70
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #12
	bl Func_02001e88
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	lsls r1, r1, #1
	movs r2, #168
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #210
	bl Func_02001f10
	bl Func_02000f00
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #185
	bl Func_02001f10
	movs r0, #8
	bl Object_GetById
	mov r3, r10
	adds r0, #85
	strb r3, [r0]
	bl Func_02001064
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #8
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #212
	movs r2, #152
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #12
	bl Func_02001e70
	ldr r0, .L_02009a98
	bl Func_02001e48
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02001e60
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #196
	lsls r1, r1, #1
	movs r2, #140
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_02001e10
	movs r0, #1
	bl WaitFrames
	mov r1, r8
	movs r2, #20
	movs r0, #5
	bl Func_02001e88
	movs r0, #5
	mov r1, r9
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #5
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200993a
	movs r0, #7
	movs r1, #0
	bl Func_02001e70
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02001e60
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009960
.L_0200993a:
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #7
	adds r3, #1
	strh r3, [r2]
	mov r1, r9
	bl Func_02001e70
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02001e60
	bl Func_02001f08
.L_02009960:
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #6
	bl Func_02001e90
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02001e60
	movs r0, #11
	movs r1, #0
	bl Func_02001e70
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02001e70
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_02001e70
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #11
	movs r1, #0
	bl Func_02001e60
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009a94
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #11
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #6
	ldr r1, .L_02009a9c
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_02009aa0
	movs r0, #11
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a94:
	.4byte gPartyState
.L_02009a98:
	.4byte 0x00001f7f
.L_02009a9c:
	.4byte 0x00013333
.L_02009aa0:
	.4byte Data_02001f4c
	.section .text.x02009aa4,"ax",%progbits
	.global Func_02001aa4
	.thumb_func
Func_02001aa4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Func_02001f00
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	ldr r3, .L_02009b00
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009b04
	cmp r2, r3
	bne .L_02009ae4
	bl Func_02001b14
	b .L_02009afa
.L_02009ae4:
	ldr r3, .L_02009b08
	cmp r2, r3
	bne .L_02009af0
	bl Func_02001ba8
	b .L_02009afa
.L_02009af0:
	ldr r3, .L_02009b0c
	cmp r2, r3
	bne .L_02009afa
	bl Func_02001c44
.L_02009afa:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02009b00:
	.4byte gPartyState
.L_02009b04:
	.4byte 0x00000078
.L_02009b08:
	.4byte 0x0000007a
.L_02009b0c:
	.4byte 0x0000007c
	.section .text.x02009b10,"ax",%progbits
	.global Func_02001b10
	.thumb_func
Func_02001b10:
	movs r0, #0
	bx lr
	.section .text.x02009b14,"ax",%progbits
	.global Func_02001b14
	.thumb_func
Func_02001b14:
	push {lr}
	movs r0, #8
	bl Func_02001e80
	movs r0, #9
	bl Func_02001e80
	movs r0, #10
	bl Func_02001e80
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009b5e
	bl Func_02001010
	bl Func_02001064
	movs r1, #212
	movs r2, #152
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001e10
	movs r0, #1
	bl WaitFrames
	bl Func_02000f00
	b .L_02009b7e
.L_02009b5e:
	ldr r3, .L_02009ba4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_02009b74
	bl Func_020010d4
	b .L_02009ba2
.L_02009b74:
	cmp r3, #6
	bne .L_02009b7e
	bl Func_02001710
	b .L_02009ba2
.L_02009b7e:
	ldr r3, .L_02009ba4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02009ba2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_SetBit
.L_02009ba2:
	pop {pc}
.L_02009ba4:
	.4byte gPartyState
	.section .text.x02009ba8,"ax",%progbits
	.global Func_02001ba8
	.thumb_func
Func_02001ba8:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c28
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c3c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r7, r3, r2
	adds r2, #112
	adds r6, r3, r2
	movs r5, #1
	bl Func_02001d60
	movs r4, #0
.L_02009bdc:
	adds r3, r4, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02009c02
	adds r1, r5, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02009bee
	adds r3, r5, #4
.L_02009bee:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r5, r1, #0
.L_02009c02:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	adds r4, #1
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r3, #176
	lsls r3, r3, #1
	cmp r4, r3
	blt .L_02009bdc
	bl Func_02001d40
	movs r0, #1
	bl WaitFrames
	b .L_02009c3c
.L_02009c28:
	ldr r3, .L_02009c40
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_02009c3c
	bl Func_02000be8
.L_02009c3c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c40:
	.4byte gPartyState
	.section .text.x02009c44,"ax",%progbits
	.global Func_02001c44
	.thumb_func
Func_02001c44:
	push {r5, lr}
	movs r5, #0
.L_02009c48:
	adds r0, r5, #0
	adds r0, #15
	movs r1, #2
	adds r5, #1
	bl ObjectMotion_SetActionVariant
	cmp r5, #17
	bls .L_02009c48
	movs r0, #8
	bl Func_02001e80
	movs r0, #8
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #12]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c86
	bl Func_02001094
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02001e10
	b .L_02009c8a
.L_02009c86:
	bl Func_02000f88
.L_02009c8a:
	ldr r5, .L_02009ce4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_02009ce2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009cbe
	ldr r2, .L_02009ce8
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r5, r1
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #98
	adds r2, r5, r3
	movs r3, #1
	b .L_02009ce0
.L_02009cbe:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ce2
	ldr r2, .L_02009cec
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r5, r1
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #98
	adds r2, r5, r3
	movs r3, #3
.L_02009ce0:
	strh r3, [r2]
.L_02009ce2:
	pop {r5, pc}
.L_02009ce4:
	.4byte gPartyState
.L_02009ce8:
	.4byte 0x0000007b
.L_02009cec:
	.4byte 0x00000078
	.section .rodata.x02009f18,"a",%progbits
	.global Data_02001f18
Data_02001f18:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02001f4c
Data_02001f4c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02001f78
Data_02001f78:
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00000800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02001f9c
Data_02001f9c:
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00400000
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000040
	.4byte 0xc0010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02001fe4
Data_02001fe4:
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000050
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_0200200c
Data_0200200c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00ec0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
.L_0200a048:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x00000011
	.global Data_02002058
Data_02002058:
	.4byte 0x00000000
	.global Data_0200205c
Data_0200205c:
	.4byte 0xffff0000
	.4byte 0x00000188
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200a08c
Data_0200a08c:
	.4byte 0x001001d0
	.4byte 0x01e00090
	.4byte 0x00a00020
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020020ac
Data_020020ac:
	.4byte 0x00000078
	.4byte 0x10102077
	.4byte 0xffffffff
	.4byte 0x10201079
	.4byte 0xffffffff
	.4byte 0x1030406f
	.4byte 0x0000086b
	.4byte 0x10304072
	.4byte 0xffffffff
	.4byte 0x1040107a
	.4byte 0xffffffff
	.4byte 0x0000007a
	.4byte 0x00104078
	.4byte 0x0020107c
	.4byte 0x0000007c
	.4byte 0x0010207a
	.4byte 0x0020107e
	.4byte 0x00309070
	.4byte 0x000001ff
	.global Data_020020f8
Data_020020f8:
	.4byte 0xffff0177
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0169
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00020000
	.4byte 0xffff0169
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x01020000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
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
	.global Data_020021e8
Data_020021e8:
	.4byte 0xffff0177
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002d000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00024000
	.4byte 0xffff01e7
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff001b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002d000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte .L_0200a048
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002458
Data_02002458:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002470
Data_02002470:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_02000b60
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020024a0
Data_020024a0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x02060003
	.4byte Func_02000d3c
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000e50
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020024dc
Data_020024dc:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000202
	.4byte 0xffff0002
	.4byte Func_02000b70
	.4byte 0x00000002
	.4byte 0x08fe000a
	.4byte Func_02000e60
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001fb3
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001fb4
	.4byte 0x0000c403
	.4byte 0x08ff000b
	.4byte Func_02000184
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002530
Data_02002530:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
