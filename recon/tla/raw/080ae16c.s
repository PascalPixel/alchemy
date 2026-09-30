.syntax unified
	.thumb
	.global Func_080ae16c
	.thumb_func
Func_080ae16c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	bl Party_CountActiveOwners
	movs r6, #0
	adds r7, r0, #0
	cmp r6, r7
	bge .L_080ae214
.L_080ae180:
	ldr r2, .L_080ae21c
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r6, r1
	ldrb r0, [r2, r3]
	bl Owner_GetState
	adds r5, r0, #0
	ldrh r1, [r5, #52]
	ldrh r3, [r5, #54]
	strh r1, [r5, #56]
	strh r3, [r5, #58]
	lsls r1, r1, #16
	asrs r1, r1, #16
	lsls r0, r1, #14
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080ae1b2
	movs r3, #0
	cmp r0, #0
	blt .L_080ae1b2
	adds r3, r0, #0
.L_080ae1b2:
	strh r3, [r5, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080ae1c6
	movs r2, #56
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_080ae1c6
	movs r3, #1
	strh r3, [r5, #20]
.L_080ae1c6:
	movs r3, #58
	ldrsh r0, [r5, r3]
	movs r2, #54
	ldrsh r1, [r5, r2]
	lsls r0, r0, #14
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080ae1e4
	movs r3, #0
	cmp r0, #0
	blt .L_080ae1e4
	adds r3, r0, #0
.L_080ae1e4:
	strh r3, [r5, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080ae1f8
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_080ae1f8
	movs r3, #1
	strh r3, [r5, #22]
.L_080ae1f8:
	mov r2, r8
	cmp r2, #1
	bne .L_080ae20e
	movs r1, #50
	adds r1, #255
	adds r3, r5, r1
	movs r2, #0
	adds r1, #15
	strb r2, [r3]
	adds r3, r5, r1
	strb r2, [r3]
.L_080ae20e:
	adds r6, #1
	cmp r6, r7
	blt .L_080ae180
.L_080ae214:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ae21c:
	.4byte gPartyState
