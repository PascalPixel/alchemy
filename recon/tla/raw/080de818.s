.syntax unified
	.thumb
	.global Func_080de818
	.thumb_func
Func_080de818:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r0, [r7, r3]
	movs r6, #128
	lsls r0, r0, #9
	bl Trig_Sin
	lsls r6, r6, #11
	adds r1, r0, #0
	ldr r3, .L_080de860
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r5, #56]
	adds r3, r3, r0
	str r3, [r5, #8]
	ldrh r3, [r7]
	adds r3, #1
	strh r3, [r7]
	lsls r3, r3, #16
	asrs r1, r3, #16
	adds r2, r1, #0
	adds r2, #128
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080de856
	adds r3, r1, #0
	adds r3, #255
.L_080de856:
	asrs r3, r3, #7
	lsls r3, r3, #7
	subs r3, r2, r3
	strh r3, [r7]
	pop {r5, r6, r7, pc}
.L_080de860:
	.4byte IwramMulQ16
