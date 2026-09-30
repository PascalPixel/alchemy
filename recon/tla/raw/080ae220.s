.syntax unified
	.thumb
	.global Func_080ae220
	.thumb_func
Func_080ae220:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl Party_CountActiveOwners
	movs r7, #0
	mov r8, r0
	cmp r7, r8
	bge .L_080ae2ec
.L_080ae232:
	ldr r2, .L_080ae2f4
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r7, r1
	ldrb r6, [r2, r3]
	ldr r3, .L_080ae2f8
	movs r5, #0
	ldrb r3, [r3, r6]
	cmp r3, #0
	bne .L_080ae258
	movs r0, #136
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ae270
	movs r0, #137
	lsls r0, r0, #1
	b .L_080ae268
.L_080ae258:
	movs r0, #18
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ae270
	movs r0, #20
	adds r0, #255
.L_080ae268:
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ae272
.L_080ae270:
	movs r5, #1
.L_080ae272:
	cmp r5, #0
	beq .L_080ae2e6
	adds r0, r6, #0
	bl Owner_GetState
	adds r5, r0, #0
	ldrh r3, [r5, #54]
	strh r3, [r5, #58]
	movs r2, #56
	ldrsh r0, [r5, r2]
	movs r3, #52
	ldrsh r1, [r5, r3]
	lsls r0, r0, #14
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080ae2a0
	movs r3, #0
	cmp r0, #0
	blt .L_080ae2a0
	adds r3, r0, #0
.L_080ae2a0:
	strh r3, [r5, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080ae2b4
	movs r1, #56
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_080ae2b4
	movs r3, #1
	strh r3, [r5, #20]
.L_080ae2b4:
	movs r2, #58
	ldrsh r0, [r5, r2]
	movs r3, #54
	ldrsh r1, [r5, r3]
	lsls r0, r0, #14
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080ae2d2
	movs r3, #0
	cmp r0, #0
	blt .L_080ae2d2
	adds r3, r0, #0
.L_080ae2d2:
	strh r3, [r5, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080ae2e6
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_080ae2e6
	movs r3, #1
	strh r3, [r5, #22]
.L_080ae2e6:
	adds r7, #1
	cmp r7, r8
	blt .L_080ae232
.L_080ae2ec:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ae2f4:
	.4byte gPartyState
.L_080ae2f8:
	.4byte Data_080b127c
