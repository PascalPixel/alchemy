.syntax unified
	.thumb
	.global Func_0802dbd0
	.thumb_func
Func_0802dbd0:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_0802da88
	cmp r0, #255
	beq .L_0802dc40
	adds r0, r5, #0
	bl Func_0802dac0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #11
	cmp r3, r1
	bge .L_0802dc02
	subs r3, r0, #1
	cmp r3, #3
	bls .L_0802dc1a
	cmp r0, #6
	beq .L_0802dc1a
	b .L_0802dc40
.L_0802dc02:
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #58
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0802dc1e
	subs r3, r0, #1
	cmp r3, #5
	bhi .L_0802dc40
.L_0802dc1a:
	movs r0, #0
	b .L_0802dc44
.L_0802dc1e:
	cmp r3, #1
	bne .L_0802dc2a
	subs r3, r0, #1
	cmp r3, #7
	bhi .L_0802dc40
	b .L_0802dc1a
.L_0802dc2a:
	cmp r3, #2
	bne .L_0802dc36
	subs r3, r0, #1
	cmp r3, #7
	bhi .L_0802dc3c
	b .L_0802dc1a
.L_0802dc36:
	subs r3, r0, #4
	cmp r3, #4
	bls .L_0802dc1a
.L_0802dc3c:
	cmp r0, #10
	beq .L_0802dc1a
.L_0802dc40:
	movs r0, #1
	negs r0, r0
.L_0802dc44:
	pop {r5, r6, pc}
	.2byte 0x0000
