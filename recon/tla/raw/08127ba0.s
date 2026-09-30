.syntax unified
	.thumb
	.global Func_08127ba0
	.thumb_func
Func_08127ba0:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #36]
	adds r3, r6, #0
	adds r3, #64
	ldrb r5, [r3]
	bl Owner_GetState
	movs r3, #42
	mov r12, r0
	adds r3, #255
	add r3, r12
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08127c2a
	movs r3, #165
	lsls r3, r3, #1
	add r3, r12
	movs r1, #0
	ldrh r0, [r3]
	cmp r1, r5
	bge .L_08127be6
	ldrh r3, [r6, #16]
	cmp r3, r0
	beq .L_08127be6
	adds r2, r6, #0
	adds r2, #16
.L_08127bd8:
	adds r1, #1
	cmp r1, r5
	bge .L_08127be6
	adds r2, #2
	ldrh r3, [r2]
	cmp r3, r0
	bne .L_08127bd8
.L_08127be6:
	cmp r1, r5
	beq .L_08127c2a
	lsls r1, r1, #2
	adds r3, r1, #0
	adds r3, #28
	ldr r3, [r6, r3]
	cmp r3, #0
	beq .L_08127c2a
	mov r2, r12
	ldrb r3, [r2]
	movs r4, #0
	cmp r3, #0
	beq .L_08127c0e
.L_08127c00:
	adds r4, #1
	cmp r4, #13
	bgt .L_08127c0e
	adds r2, #1
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_08127c00
.L_08127c0e:
	movs r0, #32
	cmp r4, #0
	ble .L_08127c1e
	subs r3, r4, #1
	mov r2, r12
	ldrb r3, [r2, r3]
	adds r0, r3, #0
	subs r0, #49
.L_08127c1e:
	adds r1, #28
	ldr r3, [r6, r1]
	movs r2, #1
	lsls r2, r0
	bics r3, r2
	str r3, [r6, r1]
.L_08127c2a:
	pop {r5, r6, pc}
