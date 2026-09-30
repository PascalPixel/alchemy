.syntax unified
	.thumb
	.global Func_0803aaa4
	.thumb_func
Func_0803aaa4:
	push {lr}
	ldrh r2, [r0]
	movs r1, #0
	adds r0, #2
	cmp r2, #0
	beq .L_0803aada
	ldr r4, .L_0803aae0
.L_0803aab2:
	cmp r2, #32
	bne .L_0803aaba
	adds r1, #4
	b .L_0803aad2
.L_0803aaba:
	cmp r2, #255
	bhi .L_0803aad0
	adds r3, r2, #0
	subs r3, #222
	cmp r3, #1
	bls .L_0803aad2
	adds r3, #190
	lsls r3, r3, #5
	ldrh r3, [r3, r4]
	adds r1, r1, r3
	b .L_0803aad2
.L_0803aad0:
	adds r1, #10
.L_0803aad2:
	ldrh r2, [r0]
	adds r0, #2
	cmp r2, #0
	bne .L_0803aab2
.L_0803aada:
	adds r0, r1, #0
	pop {pc}
	.2byte 0x0000
.L_0803aae0:
	.4byte UiText_Glyphs
