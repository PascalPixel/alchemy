	.include "games/tla/include/game/ed_asm.h"
.syntax unified
	.thumb
	.section .text.x0200814e,"ax",%progbits
	.2byte 0x0000
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
	bl Import_ActorGet
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
	bl Actor_FindInCell
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
	bl Actor_FindInCell
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
	bl Actor_FindInCell
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
	bl wait
	adds r5, #51
	movs r0, #185
	bl Import_SoundPlay
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
	.4byte gSequenceParentWork
.L_02008508:
	.4byte gData_02001d8c
.L_0200850c:
	.4byte 0xffff0000
	.section .text.x02008a54,"ax",%progbits
	.balign 4
	.ifndef TLA_EDITION_JA
	.global VenusLighthouse_Start
	.thumb_func
VenusLighthouse_Start:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
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
	cmp r3, #99
	bne .L_02008aa2
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
	bl Import_EventStart
	movs r0, #0
	bl Import_EventResolvePendingActions
	bl Func_02000b2c
	bl Func_02000d30
	bl Import_EventEnd
	b .L_02008b1a
.L_02008aa2:
	movs r0, #10
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #17
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #18
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #19
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #20
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #21
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #22
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #23
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #24
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #25
	bl Import_ActorGet
	movs r1, #6
	bl Actor_SetPartAttribute
.L_02008b1a:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008b20:
	.4byte gSequenceParentWork
.L_02008b24:
	.4byte SceneId_VenusLighthouse
	.endif
