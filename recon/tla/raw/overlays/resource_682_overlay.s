.syntax unified
	.thumb
	.section .text.x02008a80,"ax",%progbits
	.global Func_02000a80
	.thumb_func
Func_02000a80:
	push {lr}
	ldr r3, .L_02008b14
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008b10
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #124
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008a9c:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #96
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008a9c
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #98
	strh r0, [r3]
	adds r3, #58
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008ac6:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #128
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008ac6
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #130
	strh r0, [r3]
	adds r3, #58
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008af0:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #160
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008af0
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #162
	strh r0, [r3]
.L_02008b10:
	pop {pc}
	.2byte 0x0000
.L_02008b14:
	.4byte gFrameCount
	.section .text.x02008c40,"ax",%progbits
	.global Func_02000c40
	.thumb_func
Func_02000c40:
	push {r5, r6, r7, lr}
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r1, #0
	lsls r3, r3, #16
	movs r0, #244
	asrs r7, r3, #16
	lsls r0, r0, #1
	adds r3, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl Func_020029ec
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008c84
	movs r1, #1
	ldr r5, [r6, #80]
	bl Func_020029dc
	ldr r1, .L_02008c8c
	adds r0, r6, #0
	bl Func_020029e4
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #16]
	ldr r1, .L_02008c88
	adds r2, #9
	strh r3, [r2]
	strb r1, [r5, #26]
	strh r7, [r5, #18]
.L_02008c84:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008c88:
	.4byte 0x00000000
.L_02008c8c:
	.4byte Data_02003620
	.section .text.x02008d4c,"ax",%progbits
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [sp, #32]
	mov r8, r1
	mov r10, r2
	ldr r7, [r3, #108]
	bl ObjectTable_ReadActiveValue
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectTable_ReadActiveValue
	adds r5, r0, #0
	adds r0, r6, #0
	bl BattleFx_GetResourceId
	movs r2, #226
	lsls r2, r2, #1
	adds r1, r7, r2
	adds r3, r0, #0
	ldrh r0, [r1]
	lsls r3, r3, #16
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r1]
	asrs r0, r0, #16
	mov r1, r8
	mov r2, r10
	bl UiText_OpenMessageWindow
	adds r0, r6, #0
	ldr r3, [sp, #28]
	movs r1, #0
	mov r2, r9
	bl Func_02002a2c
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_02008db6
	b .L_02008dae
.L_02008da8:
	ldr r0, [sp, #52]
	bl WaitFrames
.L_02008dae:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_02008da8
.L_02008db6:
	adds r0, r5, #0
	bl BattleFx_GetResourceId
	movs r2, #226
	lsls r2, r2, #1
	adds r1, r7, r2
	adds r3, r0, #0
	ldrh r0, [r1]
	lsls r3, r3, #16
	adds r2, r0, #1
	strh r2, [r1]
	lsls r0, r0, #16
	ldr r1, [sp, #36]
	ldr r2, [sp, #40]
	asrs r0, r0, #16
	bl UiText_OpenMessageWindow
	adds r0, r5, #0
	movs r1, #0
	ldr r2, [sp, #44]
	ldr r3, [sp, #48]
	bl Func_02002a2c
	b .L_02008dec
.L_02008de6:
	movs r0, #1
	bl WaitFrames
.L_02008dec:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_02008de6
	movs r0, #1
	bl WaitFrames
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
