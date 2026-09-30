.syntax unified
	.thumb
	.global Func_0802db88
	.thumb_func
Func_0802db88:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Func_0802da88
	cmp r0, #255
	beq .L_0802dbca
	adds r0, r5, #0
	bl Func_0802dac0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #58
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0802dbbc
	cmp r3, #1
	beq .L_0802dbbc
	cmp r3, #2
	bne .L_0802dbca
.L_0802dbbc:
	subs r3, r0, #1
	cmp r3, #3
	bls .L_0802dbc6
	cmp r0, #6
	bne .L_0802dbca
.L_0802dbc6:
	movs r0, #0
	b .L_0802dbce
.L_0802dbca:
	movs r0, #1
	negs r0, r0
.L_0802dbce:
	pop {r5, pc}
