.syntax unified
	.thumb
	.global Func_080ec1d0
	.thumb_func
Func_080ec1d0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r1
	mov r8, r0
	bl Func_080cdf5c
	movs r7, #143
	movs r3, #192
	lsls r3, r3, #18
	lsls r7, r7, #1
	ldr r5, [r3, #108]
	adds r6, r0, #0
	adds r0, r7, #0
	bl GameFlag_ClearBit
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080ec27a
	adds r0, r6, #0
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080ec288
	movs r3, #10
	ldrsh r6, [r0, r3]
	movs r2, #18
	ldrsh r5, [r0, r2]
	movs r3, #197
	lsls r3, r3, #3
	adds r3, #255
	cmp r5, r3
	bgt .L_080ec244
	movs r2, #184
	lsls r2, r2, #6
	adds r2, #223
	cmp r6, r2
	bgt .L_080ec238
	adds r0, r7, #0
	bl GameFlag_SetBit
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #174
	adds r6, #65
	b .L_080ec242
.L_080ec238:
	ldr r2, .L_080ec290
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #196
	adds r6, r6, r2
.L_080ec242:
	adds r5, r5, r3
.L_080ec244:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #85
	muls r3, r6
	cmp r3, #0
	bge .L_080ec258
	movs r2, #252
	lsls r2, r2, #6
	adds r2, #255
	adds r3, r3, r2
.L_080ec258:
	ldr r2, .L_080ec294
	asrs r3, r3, #14
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2]
	lsls r3, r5, #2
	adds r3, r3, r5
	lsls r0, r3, #7
	cmp r0, #0
	bge .L_080ec274
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	adds r0, r0, r3
.L_080ec274:
	asrs r3, r0, #14
	subs r3, #112
	b .L_080ec284
.L_080ec27a:
	movs r3, #132
	lsls r3, r3, #1
	mov r2, r8
	str r3, [r2]
	movs r3, #16
.L_080ec284:
	mov r2, r10
	str r3, [r2]
.L_080ec288:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080ec290:
	.4byte 0xfffffd8d
.L_080ec294:
	.4byte 0xfffffef3
