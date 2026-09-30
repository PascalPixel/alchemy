.syntax unified
	.thumb
	.global Func_081084f4
	.thumb_func
Func_081084f4:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #250
	adds r3, r5, r2
	adds r6, r0, #0
	ldrh r0, [r3]
	bl Func_080c85c8
	adds r7, r0, #0
	bl Func_08038140
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #6
	adds r5, r5, r3
	movs r1, #0
	ldrsb r1, [r5, r1]
	cmp r1, #3
	bne .L_0810852c
	ldr r3, .L_0810856c
	ldr r2, .L_08108570
	subs r3, r3, r2
	adds r6, r6, r3
.L_0810852c:
	cmp r1, #2
	bne .L_08108538
	ldr r3, .L_08108574
	ldr r2, .L_08108570
	subs r3, r3, r2
	adds r6, r6, r3
.L_08108538:
	cmp r1, #0
	bne .L_08108544
	ldr r3, .L_08108578
	ldr r2, .L_08108570
	subs r3, r3, r2
	adds r6, r6, r3
.L_08108544:
	lsls r3, r7, #16
	movs r2, #34
	orrs r3, r2
	adds r0, r6, #0
	movs r1, #5
	movs r2, #0
	bl UiText_OpenMessageWindowFar
	b .L_0810855c
.L_08108556:
	movs r0, #1
	bl WaitFrames
.L_0810855c:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_08108556
	movs r0, #1
	bl WaitFrames
	pop {r5, r6, r7, pc}
.L_0810856c:
	.4byte 0x00001317
.L_08108570:
	.4byte 0x0000124c
.L_08108574:
	.4byte 0x00001277
.L_08108578:
	.4byte 0x000012a2
