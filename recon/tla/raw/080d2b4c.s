.syntax unified
	.thumb
	.global Func_080d2b4c
	.thumb_func
Func_080d2b4c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r1, #0
	ldr r3, [r3, #108]
	movs r1, #128
	lsls r1, r1, #4
	mov r8, r1
	adds r7, r0, #0
	mov r10, r3
	mov r3, r8
	ands r3, r7
	movs r1, #20
	mov r8, r3
	adds r0, r6, #0
	movs r3, #255
	ands r7, r3
	bl Math_DivU
	movs r1, #20
	adds r5, r0, #0
	adds r0, r6, #0
	bl Math_ModU
	cmp r0, #19
	bne .L_080d2b98
	movs r1, #200
	ldr r2, .L_080d2bfc
	lsls r1, r1, #5
	adds r1, #80
	adds r3, r5, r1
	ldrsb r0, [r2, r3]
	cmp r0, #0
	beq .L_080d2bf4
	subs r0, #1
.L_080d2b98:
	lsls r3, r5, #2
	adds r3, r3, r5
	lsls r3, r3, #2
	adds r6, r3, r0
	mov r3, r8
	cmp r3, #0
	bne .L_080d2bac
	adds r0, r7, #0
	bl Func_080d7524
.L_080d2bac:
	movs r1, #150
	lsls r1, r1, #1
	ldr r2, .L_080d2c00
	adds r3, r6, r1
	mov r1, r8
	orrs r3, r1
	movs r1, #149
	lsls r1, r1, #2
	adds r2, r2, r1
	strh r3, [r2]
	adds r0, r6, #0
	bl Func_080ca1a4
	movs r3, #178
	lsls r3, r3, #1
	add r3, r10
	strh r0, [r3]
	movs r3, #197
	lsls r3, r3, #1
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080d2bec
	bl Func_080cdf5c
	bl ObjectTable_Get
	adds r0, #8
	bl Func_080c9f2c
.L_080d2bec:
	movs r0, #0
	movs r1, #0
	bl Func_080ca5d8
.L_080d2bf4:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080d2bfc:
	.4byte Data_02001000
.L_080d2c00:
	.4byte gPartyState
