.syntax unified
	.thumb
	.global GetMotionRecord
	.thumb_func
GetMotionRecord:
	push {lr}
	cmp r0, #0
	beq .L_0811bdd8
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	movs r4, #15
	ands r4, r3
	cmp r4, #1
	bne .L_0811bdcc
	cmp r1, #0
	bne .L_0811bdd8
	ldr r0, [r0, #80]
	b .L_0811bdda
.L_0811bdcc:
	cmp r4, #2
	bne .L_0811bdd8
	ldr r3, [r0, #80]
	lsls r2, r1, #2
	ldr r0, [r2, r3]
	b .L_0811bdda
.L_0811bdd8:
	movs r0, #0
.L_0811bdda:
	pop {pc}
