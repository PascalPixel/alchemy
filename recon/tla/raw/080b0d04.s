.syntax unified
	.thumb
	.global Djinn_Deactivate
	.thumb_func
Djinn_Deactivate:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	adds r6, r1, #0
	mov r10, r2
	bl Owner_GetState
	adds r1, r6, #0
	adds r5, r0, #0
	mov r2, r10
	adds r0, r7, #0
	bl Djinn_IsActive
	mov r8, r0
	cmp r0, #0
	beq .L_080b0d4e
	movs r3, #142
	lsls r3, r3, #1
	adds r2, r6, r3
	ldrb r3, [r5, r2]
	lsls r1, r6, #2
	adds r3, #255
	strb r3, [r5, r2]
	movs r3, #132
	lsls r3, r3, #1
	adds r1, r1, r3
	movs r2, #1
	mov r3, r10
	lsls r2, r3
	ldr r3, [r5, r1]
	adds r0, r7, #0
	bics r3, r2
	str r3, [r5, r1]
	bl Owner_RefreshDerivedData
.L_080b0d4e:
	mov r0, r8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
