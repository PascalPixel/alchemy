.syntax unified
	.thumb
	.global Func_0811c538
	.thumb_func
Func_0811c538:
	push {lr}
	movs r3, #192
	ldr r0, .L_0811c58c
	lsls r3, r3, #18
	ldr r1, [r3, #48]
	adds r3, #176
	ldr r4, [r3]
	ldr r3, [r0]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	sub sp, #4
	cmp r3, #0
	beq .L_0811c55a
	ldrh r3, [r1, #54]
	adds r3, r3, r2
	strh r3, [r1, #54]
.L_0811c55a:
	ldr r3, [r0]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0811c56e
	ldrh r3, [r1, #54]
	ldr r2, .L_0811c590
	adds r3, r3, r2
	strh r3, [r1, #54]
.L_0811c56e:
	ldr r3, [r4, #20]
	cmp r3, #0
	bne .L_0811c588
	movs r1, #240
	movs r3, #128
	lsls r3, r3, #9
	lsls r1, r1, #15
	str r3, [sp, #0]
	adds r0, r1, #0
	movs r2, #0
	movs r3, #0
	bl Func_08126548
.L_0811c588:
	add sp, #4
	pop {pc}
.L_0811c58c:
	.4byte gInput
.L_0811c590:
	.4byte 0xfffffe00
