.syntax unified
	.thumb
	.global Func_080434ec
	.thumb_func
Func_080434ec:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r6, #0
	bl Func_0801596c
	cmp r0, #0
	beq .L_08043506
	ldr r0, .L_08043528
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	subs r6, #9
	b .L_0804351e
.L_08043506:
	ldr r1, .L_0804352c
	adds r0, r5, #0
	bl Func_08042f10
	cmp r0, #0
	beq .L_0804351e
	ldr r0, .L_08043530
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r6, #3
	negs r6, r6
.L_0804351e:
	bl Func_0801613c
	adds r0, r6, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08043528:
	.4byte 0x0000000b
.L_0804352c:
	.4byte Data_02000000
.L_08043530:
	.4byte 0x0000000c
