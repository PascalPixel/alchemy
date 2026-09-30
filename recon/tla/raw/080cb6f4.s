.syntax unified
	.thumb
	.global Func_080cb6f4
	.thumb_func
Func_080cb6f4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	mov r8, r1
	cmp r5, #0
	bge .L_080cb72c
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	movs r1, #0
	bl Func_080d172c
	movs r0, #4
	bl Func_080d17ac
	movs r2, #10
	negs r2, r2
	cmp r5, r2
	bge .L_080cb724
	movs r0, #134
	bl Audio_PlayCue
	b .L_080cb732
.L_080cb724:
	movs r0, #133
	bl Audio_PlayCue
	b .L_080cb732
.L_080cb72c:
	movs r0, #126
	bl Audio_PlayCue
.L_080cb732:
	bl Func_080ad0f0
	cmp r0, #0
	ble .L_080cb77c
	ldr r3, .L_080cb784
	movs r2, #134
	lsls r2, r2, #2
	adds r7, r3, r2
	adds r6, r0, #0
.L_080cb744:
	ldrb r0, [r7]
	bl Owner_GetState
	mov r3, r8
	adds r1, r5, #0
	cmp r3, #0
	beq .L_080cb76e
	movs r2, #52
	ldrsh r3, [r0, r2]
	movs r1, #100
	adds r0, r5, #0
	muls r0, r3
	bl Math_Div
	adds r1, r0, #0
	cmp r1, #0
	bne .L_080cb76e
	adds r1, r5, #0
	cmp r1, #0
	bge .L_080cb76e
	negs r1, r1
.L_080cb76e:
	ldrb r0, [r7]
	subs r6, #1
	bl Owner_AdjustFirstValueFar
	adds r7, #1
	cmp r6, #0
	bne .L_080cb744
.L_080cb77c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cb784:
	.4byte gPartyState
