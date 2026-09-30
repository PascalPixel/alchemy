.syntax unified
	.thumb
	.global Func_080133c4
	.thumb_func
Func_080133c4:
	push {r5, lr}
	ldr r5, .L_08013414
	movs r4, #0
	strh r4, [r5]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08013418
	ldr r1, .L_0801341c
	ldr r2, .L_08013420
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08013424
	ldr r2, .L_08013428
	ldr r0, .L_0801342c
	str r3, [r2]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_08013430
	adds r2, #14
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	subs r3, #208
	strh r4, [r3]
	movs r2, #195
	ldr r3, .L_08013434
	lsls r2, r2, #8
	adds r2, #255
	strh r2, [r3]
	movs r2, #192
	lsls r2, r2, #6
	adds r2, #1
	adds r3, #206
	strh r2, [r3]
	movs r3, #1
	strh r3, [r5]
	pop {r5, pc}
.L_08013414:
	.4byte 0x04000208
.L_08013418:
	.4byte IwramRuntime_Rom
.L_0801341c:
	.4byte IwramIrqMain
.L_08013420:
	.4byte 0x84000400
.L_08013424:
	.4byte IwramIrqMain
.L_08013428:
	.4byte Data_03007ffc
.L_0801342c:
	.4byte Data_080178b4
.L_08013430:
	.4byte Data_030001e4
.L_08013434:
	.4byte 0x04000132
