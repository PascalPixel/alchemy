.syntax unified
	.thumb
	.global Func_0810b378
	.thumb_func
Func_0810b378:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r1, #160
	ldr r3, .L_0810b3e8
	lsls r1, r1, #3
	adds r1, #4
	ldrsb r0, [r3, r0]
	adds r3, r6, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #0
	sub sp, #4
	mov r10, r0
	movs r7, #0
	cmp r2, r3
	bge .L_0810b3da
	adds r3, r6, #2
	movs r5, #153
	mov r8, r3
	lsls r5, r5, #3
.L_0810b3ae:
	mov r1, r8
	ldrsh r0, [r1, r5]
	str r2, [sp, #0]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	ldr r2, [sp, #0]
	cmp r3, #0
	beq .L_0810b3c4
	adds r2, #1
.L_0810b3c4:
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #4
	adds r3, r6, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r7, #1
	adds r5, #2
	cmp r7, r3
	blt .L_0810b3ae
.L_0810b3da:
	mov r0, r10
	muls r0, r2
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0810b3e8:
	.4byte Data_0810cf2c
