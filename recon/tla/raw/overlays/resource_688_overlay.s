.syntax unified
	.thumb
	.section .text.x02008310,"ax",%progbits
	.balign 4
	.global Func_02000310
	.thumb_func
Func_02000310:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #20
	ldr r7, [r3, #108]
	adds r5, r0, #0
	movs r3, #0
	mov r10, r1
	str r3, [sp, #16]
	str r3, [sp, #12]
	movs r6, #56
	cmp r5, #8
	beq .L_02008332
	adds r6, r5, #0
.L_02008332:
	adds r0, r6, #0
	bl BattleEffect_GetResourceId
	mov r8, r0
	cmp r5, #7
	bne .L_02008366
	movs r3, #10
	str r3, [sp, #16]
	str r3, [sp, #12]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	add r4, sp, #4
	add r2, sp, #12
	add r3, sp, #8
	add r1, sp, #16
	str r4, [sp, #0]
	bl Text_GetMessageSize
	ldr r3, [sp, #16]
	ldr r2, [sp, #8]
	adds r3, r3, r2
	subs r5, r3, #5
	b .L_02008370
.L_02008366:
	movs r3, #5
	str r3, [sp, #16]
	movs r3, #10
	str r3, [sp, #12]
	movs r5, #5
.L_02008370:
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r7, r3
	ldrh r0, [r2]
	adds r3, r0, #1
	strh r3, [r2]
	mov r2, r8
	lsls r3, r2, #16
	lsls r0, r0, #16
	movs r2, #2
	orrs r3, r2
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	asrs r0, r0, #16
	bl Message_OpenWindow
	adds r2, r5, #0
	adds r0, r6, #0
	movs r1, #0
	movs r3, #5
	bl Func_0200053c
	adds r5, r0, #0
	b .L_020083a6
.L_020083a0:
	movs r0, #1
	bl wait
.L_020083a6:
	bl Message_AllTyped
	cmp r0, #0
	beq .L_020083a0
	mov r0, r10
	bl FieldEvent_Wait
	adds r0, r5, #0
	movs r1, #2
	bl Window_Close
	bl Message_CloseFinishedWindows
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
