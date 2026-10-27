.syntax unified
	.thumb
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
	bl BattleEffect_GetResourceId
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
	bl BattleEffect_GetResourceId
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
