.syntax unified
	.thumb
	.global Func_080fcf5c
	.thumb_func
Func_080fcf5c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	adds r4, r0, #0
	lsls r0, r0, #2
	ldr r7, [r3]
	mov r8, r0
	mov r3, r8
	adds r3, #20
	ldr r0, [r7, r3]
	movs r5, #0
	movs r3, #1
	strb r3, [r0, #5]
	strh r5, [r0, #12]
	movs r0, #134
	lsls r0, r0, #2
	adds r3, r7, r0
	ldr r2, [r3]
	subs r0, #3
	movs r3, #13
	strb r3, [r2, #5]
	adds r3, r7, r0
	ldrb r3, [r3]
	adds r4, #28
	adds r2, r7, #2
	ldrsb r1, [r7, r4]
	strb r3, [r2, r4]
	movs r2, #1
	negs r2, r2
	cmp r1, r2
	bne .L_080fcfac
	ldr r3, .L_080fcfa8
	movs r6, #0
	strb r3, [r7, r4]
	b .L_080fcfba
.L_080fcfa8:
	.4byte 0x00000000
.L_080fcfac:
	lsls r6, r1, #1
	adds r0, r6, r1
	lsls r0, r0, #3
	subs r0, #10
	movs r1, #16
	bl Func_080f8ab4
.L_080fcfba:
	movs r5, #129
	lsls r5, r5, #2
	adds r3, r6, r5
	ldrh r0, [r7, r3]
	bl Owner_GetState
	movs r3, #226
	lsls r3, r3, #1
	adds r6, r7, r3
	adds r1, r6, #0
	movs r2, #2
	bl Func_080fd6f0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	adds r5, r7, r5
	adds r1, r6, #0
	strb r0, [r3]
	adds r0, r5, #0
	bl Func_080fd000
	mov r3, r8
	adds r3, #20
	adds r5, r0, #0
	ldr r0, [r7, r3]
	bl UiIcon_PrepareObject
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
