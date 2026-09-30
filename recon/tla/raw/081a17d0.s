.syntax unified
	.thumb
	.global Func_081a17d0
	.thumb_func
Func_081a17d0:
	push {r5, r6, r7, lr}
	ldr r0, .L_081a1808
	ldr r1, .L_081a180c
	movs r2, #0
	ldrsh r3, [r0, r2]
	ldr r2, [r1]
	lsls r3, r3, #16
	movs r5, #0
	cmp r2, r3
	beq .L_081a1804
	adds r7, r1, #0
	adds r6, r0, #0
.L_081a17e8:
	movs r0, #1
	bl WaitFrames
	movs r3, #44
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	bgt .L_081a1804
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r7]
	lsls r3, r3, #16
	cmp r2, r3
	bne .L_081a17e8
.L_081a1804:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a1808:
	.4byte Data_02007520
.L_081a180c:
	.4byte Data_0200751c
