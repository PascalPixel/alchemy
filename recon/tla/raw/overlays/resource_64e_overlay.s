	.include "games/common/include/game/ed_asm.h"
.syntax unified
	.thumb
	.section .text.x0200814e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008150,"ax",%progbits
	.global Func_02000150
	.thumb_func
Func_02000150:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02008304
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Engine_ActorGet
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_02008198
	cmp r7, #0
	beq .L_02008198
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020081a0
.L_02008198:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020081a0:
	mov r3, r10
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020081ae
	b .L_020082f6
.L_020081ae:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02001b2c
	ldr r2, .L_02008308
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02001b34
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	ldr r3, [sp, #0]
	mov r1, r11
	strb r5, [r3, #26]
	ldr r3, .L_0200830c
	str r1, [r6, #68]
	str r3, [r6, #108]
	ldr r3, [sp, #36]
	mov r2, r9
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	adds r0, r6, #0
	str r3, [r6, #76]
	ldr r3, [r2, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r3, #100
	adds r3, r3, r6
	mov r9, r3
	ldr r3, .L_02008310
	mov r2, r8
	mov r1, r9
	ands r3, r2
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r1]
	cmp r3, #0
	beq .L_020082f6
	cmp r7, #0
	beq .L_020082f6
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r2
	cmp r3, #0
	beq .L_0200822c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200822c:
	movs r3, #128
	lsls r3, r3, #10
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200824c
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_0200824c:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02008260
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008260:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020082a6
	ldr r3, .L_02008308
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200828e
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082a0
.L_0200828e:
	ldr r2, .L_02008310
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008310
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082a0:
	bl __divsi3
	str r0, [r6, #52]
.L_020082a6:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020082c2
	adds r0, r6, #0
	movs r1, #1
	bl Func_02001b2c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001b34
.L_020082c2:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020082d4
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_020082d4:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020082e6
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_020082e6:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020082f6
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_020082f6:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008304:
	.4byte gPartyState
.L_02008308:
	.4byte Data_02001dcc
.L_0200830c:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_02008310:
	.4byte 0xffff0000
	.section .text.x0200834c,"ax",%progbits
	.balign 4
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r1, r0, #0
	adds r5, r3, #0
	movs r4, #8
	adds r5, #52
.L_0200835c:
	ldmia r5!, {r0}
	ldr r2, [r1]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008382
	ldr r2, [r1, #4]
	ldr r3, [r0, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008382
	ldr r2, [r1, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_0200838a
.L_02008382:
	adds r4, #1
	cmp r4, #63
	bls .L_0200835c
	movs r0, #0
.L_0200838a:
	pop {r5, pc}
	.section .text.x0200838c,"ax",%progbits
	.global Func_0200038c
	.thumb_func
Func_0200038c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02008504
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #12
	bl Engine_ActorGet
	ldrh r3, [r0, #6]
	ldr r1, .L_02008508
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r2, .L_0200850c
	mov r9, r1
	ldr r1, [r1, r5]
	mov r10, r2
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r0, #8]
	mov r7, sp
	adds r3, r3, r2
	str r3, [r7]
	lsls r1, r1, #16
	ldr r3, [r0, #12]
	mov r8, r0
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	adds r0, r7, #0
	adds r3, r3, r1
	str r3, [r7, #8]
	mov r1, r8
	bl Func_0200034c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020083e2
	b .L_020084f8
.L_020083e2:
	mov r0, r9
	ldr r1, [r0, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	lsls r1, r1, #16
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r7, #0
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r6, #0
	bl Func_0200034c
	cmp r0, #0
	beq .L_02008418
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020084f8
.L_02008418:
	ldr r3, [r6, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #13
	ldr r3, [r6, #12]
	adds r1, r6, #0
	adds r3, r3, r0
	str r3, [r7, #4]
	adds r0, r7, #0
	ldr r3, [r6, #16]
	str r3, [r7, #8]
	bl Func_0200034c
	cmp r0, #0
	beq .L_02008444
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020084f8
.L_02008444:
	adds r2, r6, #0
	adds r2, #34
	movs r3, #2
	strb r3, [r2]
	mov r2, r9
	ldr r1, [r2, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	lsls r1, r1, #16
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r6, #0
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02001b64
	cmp r0, #0
	bgt .L_020084f8
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	bne .L_020084f8
	movs r1, #8
	mov r0, r8
	movs r5, #204
	bl Func_02001b2c
	lsls r5, r5, #6
	movs r0, #15
	bl WaitFrames
	adds r5, #51
	movs r0, #185
	bl Engine_AudioPlayCue
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02001b44
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02001b44
	adds r0, r6, #0
	bl Func_02001b4c
	bl Func_02001cbc
	ldr r3, [r7]
	mov r1, r10
	str r3, [r6, #8]
	ldr r3, [r7, #8]
	str r1, [r6, #36]
	str r3, [r6, #16]
	str r1, [r6, #44]
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #24
	str r3, [r2, #56]
	str r3, [r2, #64]
	movs r0, #10
	ldrsh r3, [r2, r0]
	str r1, [r2, #36]
	lsls r3, r3, #16
	str r1, [r2, #44]
	str r3, [r2, #8]
	movs r1, #18
	ldrsh r3, [r2, r1]
	mov r0, r8
	lsls r3, r3, #16
	str r3, [r2, #16]
	movs r1, #1
	bl Func_02001b2c
.L_020084f8:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008504:
	.4byte gPartyState
.L_02008508:
	.4byte Data_02001d8c
.L_0200850c:
	.4byte 0xffff0000
	.section .text.x02008510,"ax",%progbits
	.global Func_02000510
	.thumb_func
Func_02000510:
	push {r5, r6, lr}
	adds r5, r3, #0
	ldr r3, [sp, #12]
	ldr r6, [sp, #16]
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	beq .L_02008562
	cmp r0, #2
	bhi .L_02008538
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r0, r0, #1
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r0, [r4, r3]
	b .L_0200853a
.L_02008538:
	ldr r0, .L_02008568
.L_0200853a:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	movs r1, #0
	adds r0, r0, r3
	cmp r1, r12
	bcs .L_02008562
.L_02008548:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r5
	bcs .L_0200855c
.L_02008552:
	adds r2, #1
	strb r6, [r3, #2]
	adds r3, #4
	cmp r2, r5
	bcc .L_02008552
.L_0200855c:
	adds r1, #1
	cmp r1, r12
	bcc .L_02008548
.L_02008562:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008568:
	.4byte gMapCellBuffer
	.4byte 0x00004770
	.section .text.x020085a8,"ax",%progbits
	.balign 4
	.global Func_020005a8
	.thumb_func
Func_020005a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200864c
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02008640
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	movs r2, #16
	movs r3, #3
	add r2, sp
	subs r3, r3, r0
	str r3, [r2]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #14
	str r3, [r2, #4]
	mov r8, r2
	bl Random16Far
	lsls r3, r0, #3
	mov r2, r10
	adds r3, r3, r0
	ldr r6, [r2, #8]
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	adds r6, r6, r3
	bl Random16Far
	mov r2, r10
	lsls r0, r0, #5
	ldr r5, [r2, #12]
	lsrs r0, r0, #16
	movs r3, #32
	subs r3, r3, r0
	lsls r3, r3, #16
	adds r5, r5, r3
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r3, #160
	lsls r3, r3, #11
	lsls r0, r0, #16
	adds r0, r0, r3
	movs r1, #10
	bl __divsi3
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r3, [sp, #8]
	mov r3, r8
	str r0, [sp, #0]
	str r3, [sp, #12]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	bl Func_02000150
.L_02008640:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200864c:
	.4byte gFrameCount
	.section .text.x02008a54,"ax",%progbits
	.balign 4
	.if EDITION_INTERNATIONAL
	.global VinasuChojo_Start
	.thumb_func
VinasuChojo_Start:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	.if EDITION_INTERNATIONAL
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	ldr r1, .L_02008b20
	str r2, [r3]
	subs r2, #34
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	.else
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	ldr r3, .L_02008b20
	subs r2, #34
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	.endif
	cmp r3, #99
	bne .L_02008aa2
	.if EDITION_INTERNATIONAL
	ldr r3, .L_02008b24
	movs r0, #242
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #243
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #22
	strh r3, [r2]
	.endif
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	bl Func_02000b2c
	bl Func_02000d30
	bl Engine_EventEnd
	b .L_02008b1a
.L_02008aa2:
	movs r0, #10
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #17
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #18
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #19
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #20
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #21
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #22
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #23
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #24
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #25
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
.L_02008b1a:
	movs r0, #0
	pop {pc}
	.if EDITION_INTERNATIONAL
	.2byte 0x0000
	.endif
.L_02008b20:
	.4byte gPartyState
	.if EDITION_INTERNATIONAL
.L_02008b24:
	.4byte 0x00000005
	.endif
	.endif
	.section .rodata.x02009ce4,"a",%progbits
.L_02009ce4:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
.L_02009d1c:
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
.L_02009d54:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
	.global Data_02001d8c
Data_02001d8c:
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
	.global Data_02001dcc
Data_02001dcc:
	.4byte .L_02009ce4
	.4byte .L_02009d1c
	.4byte .L_02009d54
	.global Data_02001dd8
Data_02001dd8:
	.4byte 0x00000027
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte OverlayObject_UpdateEveryFourFrames
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00011999
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00011999
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000e666
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0003
	.4byte 0x00000158
	.4byte 0xc00000e8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0004
	.4byte 0x000001d8
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0005
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0x40000068
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0xc00000f8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0008
	.4byte 0x000003c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000358
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x000002c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000d
	.4byte 0x00000068
	.4byte 0xc00001f8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000e
	.4byte 0x00000068
	.4byte 0x400001d8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff0014
	.4byte 0x00000208
	.4byte 0x400002b8
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0015
	.4byte 0x00000158
	.4byte 0x40000238
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0016
	.4byte 0x000002d8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0017
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0018
	.4byte 0x00000188
	.4byte 0x400003b8
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff0019
	.4byte 0x00000188
	.4byte 0x40000358
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff001a
	.4byte 0x000002f8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff001e
	.4byte 0x00000058
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff001f
	.4byte 0x000000b8
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0020
	.4byte 0x000000b8
	.4byte 0x40000288
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0062
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0063
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02400000
	.4byte 0xffff02e0
	.4byte 0x000003c8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000005
	.4byte 0x00109004
	.4byte 0x00203005
	.4byte 0x00302005
	.4byte 0x0041f005
	.4byte 0x0051e005
	.4byte 0x00607005
	.4byte 0x00706005
	.4byte 0x00820005
	.4byte 0x00916005
	.4byte 0x00a0c005
	.4byte 0x00b0d005
	.4byte 0x00c0a005
	.4byte 0x00d0b005
	.4byte 0x00e18005
	.4byte 0x0141a005
	.4byte 0x01519005
	.4byte 0x01609005
	.4byte 0x0180e005
	.4byte 0x01915005
	.4byte 0x01a14005
	.4byte 0x01e05005
	.4byte 0x01f04005
	.4byte 0x02008005
	.4byte 0x000001ff
.L_0200a15c:
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte SceneActor_ApplyCounterLowBitsAsMode
	.4byte 0x00000011
.L_0200a180:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte SceneActor_ClearSingleChildField
	.4byte 0x00000011
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0102c000
	.4byte 0xffff01c4
	.4byte .L_0200a180
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gScenePlacementsEntrance99
gScenePlacementsEntrance99:
	.4byte 0xffff0004
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000031
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000031
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000031
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000031
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000031
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000021
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000021
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000021
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000021
	.4byte 0xffff001f
	.4byte 0x0000001f
	.4byte 0x00000021
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000002
	.4byte 0x0200002d
	.4byte Func_020006ac
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte Func_0200078c
	.4byte 0x00000003
	.4byte 0xffff0023
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0024
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0025
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0026
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0027
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte VinasuChojo_ReadTrueHeart
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte SceneState_ApplySixRectsAfterFlag161
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte SceneState_ApplySixRectsAfter161
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
