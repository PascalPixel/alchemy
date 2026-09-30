.syntax unified
	.thumb
	.global Func_080d3fb0
	.thumb_func
Func_080d3fb0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r7, r0, #0
	mov r9, r3
	bl UiText_OpenMessageAtObject
	mov r10, r0
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	bl ObjectTable_ReadActiveValue
	movs r5, #0
	mov r8, r0
	cmp r7, #7
	bgt .L_080d3ff2
	movs r6, #240
	lsls r6, r6, #4
	adds r6, #255
	ands r6, r7
	adds r0, r6, #0
	bl BattleAction_FindDescriptor
	cmp r0, #0
	bne .L_080d3ff2
	mov r8, r6
.L_080d3ff2:
	mov r0, r8
	bl UiWork_FinalizeEntityMatchingLocalizedIdFar
	movs r3, #220
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	cmp r3, #0
	bne .L_080d4050
	b .L_080d4046
.L_080d4006:
	movs r0, #1
	bl WaitFrames
	movs r3, #150
	adds r5, #1
	lsls r3, r3, #2
	cmp r5, r3
	bhi .L_080d4042
	ldr r1, .L_080d4068
	movs r3, #4
	ldr r2, [r1]
	ands r2, r3
	cmp r2, #0
	beq .L_080d4046
	ldr r2, [r1]
	adds r3, #252
	ands r2, r3
	cmp r2, #0
	beq .L_080d4046
	ldr r2, [r1]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080d4046
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080d4046
.L_080d4042:
	bl UiWork_FinalizePendingCoreFar
.L_080d4046:
	mov r0, r10
	bl UiWork_IsIdleFar
	cmp r0, #0
	beq .L_080d4006
.L_080d4050:
	movs r0, #128
	lsls r0, r0, #24
	bl Func_080d4330
	movs r0, #1
	bl WaitFrames
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080d4068:
	.4byte gInput
