.syntax unified
	.thumb
	.global Func_0811de78
	.thumb_func
Func_0811de78:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #176
	ldr r3, [r3]
	adds r6, r0, #0
	mov r8, r3
	movs r3, #128
	ldrb r5, [r6]
	lsls r3, r3, #6
	ldr r7, [r2, #36]
	mov r2, r8
	str r3, [r2]
	movs r3, #1
	movs r1, #0
	str r3, [r2, #16]
	movs r0, #0
	sub sp, #32
	mov r10, r1
	bl BattlePres_SetActorModes
	cmp r5, #7
	bhi .L_0811df1e
	adds r3, r7, #0
	adds r3, #69
	ldrb r3, [r3]
	mov r2, r10
	cmp r3, #2
	beq .L_0811debc
	movs r2, #1
.L_0811debc:
	cmp r2, #0
	bne .L_0811decc
	ldr r0, .L_0811df6c
	bl Func_080381c0 + 0x8
	bl BattlePresentation_WaitForAdvance
	b .L_0811df5a
.L_0811decc:
	add r7, sp, #4
	movs r0, #1
	adds r1, r7, #0
	bl BattleParty_ListLivingUnits
	movs r3, #1
	subs r6, r0, #1
	negs r3, r3
	cmp r6, r3
	beq .L_0811df18
	lsls r5, r6, #1
.L_0811dee2:
	ldrsh r0, [r7, r5]
	bl Owner_GetState
	movs r1, #60
	adds r2, r0, #0
	adds r1, #255
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811df0c
	adds r1, #1
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811df0c
	ldrsh r0, [r5, r7]
	bl Func_0811bec8
	movs r0, #8
	bl WaitFrames
.L_0811df0c:
	movs r3, #1
	subs r6, #1
	negs r3, r3
	subs r5, #2
	cmp r6, r3
	bne .L_0811dee2
.L_0811df18:
	movs r1, #1
	mov r10, r1
	b .L_0811df5a
.L_0811df1e:
	bl Random16
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	cmp r3, #6
	bhi .L_0811df50
	ldrb r0, [r6]
	mov r2, sp
	movs r3, #255
	strh r0, [r2]
	strh r3, [r2, #2]
	bl Func_0811bec8
	movs r0, #8
	bl WaitFrames
	ldrb r0, [r6]
	bl Func_0811f3b8
	ldrb r0, [r6]
	bl Func_0811bc64
	b .L_0811df5a
.L_0811df50:
	ldr r0, .L_0811df6c
	bl Func_080381c0 + 0x8
	bl BattlePresentation_WaitForAdvance
.L_0811df5a:
	movs r3, #0
	mov r2, r8
	mov r0, r10
	str r3, [r2, #16]
	add sp, #32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0811df6c:
	.4byte 0x00000c9c
