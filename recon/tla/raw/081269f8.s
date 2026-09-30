.syntax unified
	.thumb
	.global Func_081269f8
	.thumb_func
Func_081269f8:
	push {r5, lr}
	ldr r5, .L_08126ae0
	movs r2, #8
	ldr r3, [r5]
	ands r3, r2
	cmp r3, #0
	beq .L_08126abe
.L_08126a06:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #36]
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08126a24
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #84
	adds r2, r1, r3
	ldr r3, [r2]
	subs r3, #1
	str r3, [r2]
.L_08126a24:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_08126a3c
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #84
	adds r2, r1, r3
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_08126a3c:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08126a56
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #84
	adds r2, r1, r3
	ldr r3, [r2]
	subs r3, #100
	str r3, [r2]
.L_08126a56:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08126a70
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #84
	adds r2, r1, r3
	ldr r3, [r2]
	adds r3, #100
	str r3, [r2]
.L_08126a70:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08126a88
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #84
	adds r2, r1, r3
	ldr r3, [r2]
	subs r3, #10
	str r3, [r2]
.L_08126a88:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08126aa0
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #84
	adds r2, r1, r3
	ldr r3, [r2]
	adds r3, #10
	str r3, [r2]
.L_08126aa0:
	ldr r3, [r5, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08126ab6
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #84
	adds r3, r1, r2
	ldr r0, [r3]
	b .L_08126abe
.L_08126ab6:
	movs r0, #1
	bl WaitFrames
	b .L_08126a06
.L_08126abe:
	ldr r3, [r5]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_08126acc
	movs r0, #136
	lsls r0, r0, #2
.L_08126acc:
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08126adc
	movs r0, #144
	adds r0, #255
.L_08126adc:
	pop {r5, pc}
	.2byte 0x0000
.L_08126ae0:
	.4byte gInput
