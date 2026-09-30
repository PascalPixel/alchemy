.syntax unified
	.thumb
	.global Func_080cb788
	.thumb_func
Func_080cb788:
	push {r5, r6, r7, lr}
	movs r7, #0
	bl Party_CountActiveOwnersFar
	cmp r7, r0
	bge .L_080cb806
	ldr r3, .L_080cb828
	movs r2, #134
	lsls r2, r2, #2
	adds r6, r3, r2
	adds r5, r0, #0
.L_080cb79e:
	ldrb r0, [r6]
	bl Owner_GetState
	movs r2, #50
	adds r2, #255
	adds r3, r0, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	beq .L_080cb7ba
	cmp r3, #2
	beq .L_080cb7d8
	b .L_080cb7f6
.L_080cb7ba:
	movs r3, #52
	ldrsh r0, [r0, r3]
	movs r1, #20
	adds r0, #10
	bl Math_Div
	negs r1, r0
	cmp r1, #0
	bne .L_080cb7d0
	movs r1, #1
	negs r1, r1
.L_080cb7d0:
	cmp r7, #0
	bgt .L_080cb7f8
	movs r7, #1
	b .L_080cb7f8
.L_080cb7d8:
	movs r2, #52
	ldrsh r0, [r0, r2]
	movs r1, #10
	adds r0, #5
	bl Math_Div
	negs r1, r0
	cmp r1, #0
	bne .L_080cb7ee
	movs r1, #1
	negs r1, r1
.L_080cb7ee:
	cmp r7, #1
	bgt .L_080cb7f8
	movs r7, #2
	b .L_080cb7f8
.L_080cb7f6:
	movs r1, #0
.L_080cb7f8:
	ldrb r0, [r6]
	subs r5, #1
	bl Owner_AdjustFirstValueFar
	adds r6, #1
	cmp r5, #0
	bne .L_080cb79e
.L_080cb806:
	cmp r7, #0
	beq .L_080cb822
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	movs r1, #0
	bl Func_080d172c
	movs r0, #4
	bl Func_080d17ac
	movs r0, #133
	bl Audio_PlayCue
.L_080cb822:
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cb828:
	.4byte gPartyState
