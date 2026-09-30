.syntax unified
	.thumb
	.global Func_080396dc
	.thumb_func
Func_080396dc:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r1, #215
	lsls r1, r1, #3
	adds r2, r3, r1
	movs r5, #0
	movs r1, #0
	b .L_080396f4
.L_080396f0:
	adds r2, #40
	adds r1, #1
.L_080396f4:
	cmp r1, #3
	beq .L_08039706
	ldr r3, [r2]
	cmp r3, #0
	beq .L_08039704
	ldrh r3, [r3, #20]
	cmp r3, #0
	beq .L_080396f0
.L_08039704:
	adds r5, r2, #0
.L_08039706:
	cmp r5, #0
	beq .L_08039728
	ldr r3, [r5]
	cmp r3, #0
	beq .L_08039718
	bl Func_080396a0
	movs r3, #0
	strh r3, [r5, #6]
.L_08039718:
	movs r3, #0
	strh r3, [r5, #4]
	strh r3, [r5, #20]
	movs r2, #15
	strh r3, [r5, #24]
	movs r3, #10
	strh r2, [r5, #22]
	strh r3, [r5, #26]
.L_08039728:
	pop {r5, pc}
	.2byte 0x0000
