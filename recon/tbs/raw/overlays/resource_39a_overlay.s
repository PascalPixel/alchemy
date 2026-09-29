.syntax unified
	.thumb
	.section .text.x02008050,"ax",%progbits
	.balign 4
	.global SceneData_SelectTableBySceneId
	.thumb_func
SceneData_SelectTableBySceneId:
	push {lr}
	ldr r3, [pc, #88]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_02000050_0
	ldr r0, [pc, #76]
	b .L_02000050_1
.L_02000050_0:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000050_2
	ldr r0, [pc, #76]
	b .L_02000050_1
.L_02000050_2:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000050_3
	ldr r0, [pc, #72]
	b .L_02000050_1
.L_02000050_3:
	ldr r3, [pc, #72]
	cmp r2, r3
	bne .L_02000050_4
	ldr r0, [pc, #72]
	b .L_02000050_1
.L_02000050_4:
	ldr r3, [pc, #72]
	cmp r2, r3
	bne .L_02000050_5
	ldr r0, [pc, #68]
	b .L_02000050_1
.L_02000050_5:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000050_6
	ldr r0, [pc, #68]
	b .L_02000050_1
.L_02000050_6:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000050_7
	ldr r0, [pc, #64]
	b .L_02000050_1
.L_02000050_7:
	ldr r0, [pc, #64]
.L_02000050_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000034
	.4byte 0x0200a4bc
	.4byte 0x0000003e
	.4byte 0x0200a504
	.4byte 0x0000003f
	.4byte 0x0200a5f4
	.4byte 0x00000040
	.4byte 0x0200a63c
	.4byte 0x00000041
	.4byte 0x0200a6cc
	.4byte 0x00000042
	.4byte 0x0200a744
	.4byte 0x00000043
	.4byte 0x0200a7bc
	.4byte 0x0200a48c
	.section .text.x020080f8,"ax",%progbits
	.balign 4
	.global SceneData_SelectDataByRuntimeSelector
	.thumb_func
SceneData_SelectDataByRuntimeSelector:
	push {lr}
	ldr r3, [pc, #76]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_020000f8_0
	ldr r0, [pc, #64]
	b .L_020000f8_1
.L_020000f8_0:
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_020000f8_2
	ldr r0, [pc, #64]
	b .L_020000f8_1
.L_020000f8_2:
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_020000f8_3
	ldr r0, [pc, #60]
	b .L_020000f8_1
.L_020000f8_3:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_020000f8_4
	ldr r0, [pc, #60]
	b .L_020000f8_1
.L_020000f8_4:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_020000f8_5
	ldr r0, [pc, #56]
	b .L_020000f8_1
.L_020000f8_5:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_020000f8_6
	ldr r0, [pc, #56]
	b .L_020000f8_1
.L_020000f8_6:
	ldr r0, [pc, #56]
.L_020000f8_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000034
	.4byte 0x0200a9bc
	.4byte 0x0000003e
	.4byte 0x0200a9ec
	.4byte 0x0000003f
	.4byte 0x0200aa4c
	.4byte 0x00000040
	.4byte 0x0200aac4
	.4byte 0x00000041
	.4byte 0x0200ab3c
	.4byte 0x00000043
	.4byte 0x0200ab9c
	.4byte 0x0200a9a4
	.section .text.x02008f30,"ax",%progbits
	.balign 4
	.global SceneData_SelectDataByRuntimeSelectorB
	.thumb_func
SceneData_SelectDataByRuntimeSelectorB:
	push {lr}
	ldr r3, [pc, #88]
	movs r1, #224
.L_02000f36:
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_02000f36_0
	ldr r0, [pc, #76]
	b 0x02008f86
.L_02000f36_0:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000f36_1
	ldr r0, [pc, #76]
	b 0x02008f86
.L_02000f36_1:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000f36_2
	ldr r0, [pc, #72]
	b 0x02008f86
.L_02000f36_2:
	ldr r3, [pc, #72]
	cmp r2, r3
	bne .L_02000f36_3
	ldr r0, [pc, #72]
	b 0x02008f86
.L_02000f36_3:
	ldr r3, [pc, #72]
.L_02000f68:
	cmp r2, r3
	bne .L_02000f68_0
	ldr r0, [pc, #68]
	b .L_02000f68_1
.L_02000f68_0:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000f68_2
	ldr r0, [pc, #68]
	b .L_02000f68_1
.L_02000f68_2:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000f68_3
	ldr r0, [pc, #64]
	b .L_02000f68_1
.L_02000f68_3:
	ldr r0, [pc, #64]
.L_02000f68_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0034
	.2byte 0x0000
	.2byte 0xabd8
	.2byte 0x0200
	.2byte 0x003e
	.2byte 0x0000
	.2byte 0xac08
	.2byte 0x0200
	.2byte 0x003f
	.2byte 0x0000
	.2byte 0xad1c
	.2byte 0x0200
	.2byte 0x0040
	.2byte 0x0000
	.2byte 0xae24
	.2byte 0x0200
	.2byte 0x0041
	.2byte 0x0000
	.4byte 0x0200b058
	.4byte 0x00000042
	.4byte 0x0200b130
	.4byte 0x00000043
	.4byte 0x0200b184
	.4byte 0x0200abcc
	.section .text.x0200970c,"ax",%progbits
	.balign 4
	.global Func_0200170c
	.thumb_func
Func_0200170c:
	push {r5, lr}
	ldr r3, [pc, #24]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200170c_0
	ldr r3, [pc, #20]
	movs r0, #0
	ldr r5, [r3]
	bl 0x0200a350
	str r0, [r5, #24]
.L_0200170c_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200b328
	.4byte 0x03001ee0
	.global Func_02001730
	.thumb_func
Func_02001730:
	push {lr}
	ldr r3, [pc, #20]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001730_0
	ldr r3, [pc, #16]
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #24]
.L_02001730_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200b328
	.4byte 0x03001ee0
	.global ImiruFuchin_ApplyEntryHook
	.thumb_func
ImiruFuchin_ApplyEntryHook:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r0, [pc, #56]
	bl 0x0200a318
	cmp r0, #0
	bne .L_02001750_0
	ldr r3, [pc, #48]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02001750_0
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200a320
	bl 0x020097a8
	b .L_02001750_1
.L_02001750_0:
	bl 0x020097e4
.L_02001750_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000109
	.4byte 0x02000240
	.4byte 0x00000034
	.section .text.x020097e4,"ax",%progbits
	.balign 4
	.global Func_020017e4
	.thumb_func
Func_020017e4:
	push {r5, r6, r7, lr}
	sub sp, #8
	bl 0x02009948
	ldr r6, [pc, #312]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #304]
	cmp r2, r3
	bne .L_020017e4_0
	ldr r0, [pc, #304]
	bl 0x0200a318
	cmp r0, #0
	bne .L_020017e4_1
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	bne .L_020017e4_1
	bl 0x02009b1c
.L_020017e4_1:
	ldr r3, [pc, #268]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r1, #192
	subs r3, #2
	lsls r3, r3, #16
	lsls r1, r1, #10
	cmp r3, r1
	bhi .L_020017e4_2
	movs r5, #226
	lsls r5, r5, #17
	movs r0, #156
	movs r1, #0
	adds r2, r5, #0
	movs r3, #223
	lsls r0, r0, #16
	bl 0x02008ed8
	movs r0, #188
	lsls r0, r0, #16
	movs r1, #0
	adds r2, r5, #0
	movs r3, #223
	bl 0x02008ed8
	b .L_020017e4_2
.L_020017e4_0:
	ldr r3, [pc, #224]
	cmp r2, r3
	bne .L_020017e4_2
	movs r0, #8
	bl 0x0200a350
	ldr r7, [pc, #216]
	adds r3, r0, #0
	adds r3, #85
	movs r5, #0
	str r5, [r7]
	movs r1, #1
	strb r5, [r3]
	str r5, [r0, #12]
	movs r0, #8
	bl 0x0200a398
	movs r1, #15
	movs r0, #8
	bl 0x0200a388
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	blt .L_020017e4_3
	cmp r3, #2
	ble .L_020017e4_4
	cmp r3, #5
	beq .L_020017e4_5
	b .L_020017e4_3
.L_020017e4_4:
	movs r0, #0
	bl 0x0200a3f0
	movs r3, #1
	str r3, [r7]
	b .L_020017e4_3
.L_020017e4_5:
	movs r0, #0
	bl 0x0200a3f0
	movs r3, #1
	str r3, [r7]
	ldr r3, [pc, #144]
	ldr r5, [r3]
	movs r3, #0
	str r3, [r5, #24]
.L_020017e4_3:
	ldr r3, [pc, #116]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #6
	bgt .L_020017e4_2
	movs r0, #130
	lsls r0, r0, #4
	bl 0x0200a318
	cmp r0, #0
	beq .L_020017e4_6
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #57
	movs r2, #19
	movs r3, #57
	bl 0x0200a2d8
	movs r2, #7
	movs r3, #8
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #8
	movs r2, #12
	str r3, [sp, #0]
	bl 0x0200a2d8
	b .L_020017e4_2
.L_020017e4_6:
	ldr r3, [pc, #72]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	ldr r0, [pc, #64]
	movs r1, #1
	bl 0x0200a3b8
	ldr r0, [pc, #56]
	movs r1, #1
	bl 0x0200a3b0
	movs r0, #1
	bl 0x0200a3c0
	movs r0, #1
	bl 0x0200a260
.L_020017e4_2:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000040
	.4byte 0x00000f13
	.4byte 0x00000043
	.4byte 0x0200b328
	.4byte 0x03001ee0
	.4byte 0x03001ebc
	.4byte 0x00203108
	.global Func_02001948
	.thumb_func
Func_02001948:
	push {lr}
	ldr r1, [pc, #212]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #204]
	sub sp, #8
	cmp r2, r3
	bne .L_02001948_0
	movs r3, #8
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #29
	movs r2, #15
	movs r3, #5
	bl 0x0200a2f0
	bl 0x02009050
	b .L_02001948_1
.L_02001948_0:
	ldr r3, [pc, #172]
	cmp r2, r3
	bne .L_02001948_2
	movs r3, #0
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #8
	movs r2, #10
	movs r3, #18
	bl 0x0200a2f0
	bl 0x02009154
	b .L_02001948_1
.L_02001948_2:
	ldr r3, [pc, #144]
	cmp r2, r3
	bne .L_02001948_3
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #1
	beq .L_02001948_3
	movs r3, #12
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #21
	movs r2, #9
	movs r3, #16
	bl 0x0200a2f0
	bl 0x020092cc
	b .L_02001948_1
.L_02001948_3:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_02001948_1
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_02001948_4
	movs r3, #22
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #10
	movs r2, #9
	movs r3, #8
	bl 0x0200a2f0
	b .L_02001948_5
.L_02001948_4:
	movs r3, #20
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #45
	movs r2, #11
	movs r3, #4
	bl 0x0200a2f0
.L_02001948_5:
	bl 0x020095dc
.L_02001948_1:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000003e
	.4byte 0x0000003f
	.4byte 0x00000040
	.4byte 0x00000041
	.global Func_02001a34
	.thumb_func
Func_02001a34:
	push {r5, r6, lr}
	ldr r3, [pc, #100]
	movs r1, #182
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r5, [pc, #96]
	adds r3, r3, r1
	adds r1, #84
	movs r2, #0
	ldrsh r6, [r3, r2]
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_02001a34_0
	cmp r6, #17
	bne .L_02001a34_1
	movs r1, #32
	negs r1, r1
	movs r0, #0
	bl 0x02009ad0
	b .L_02001a34_0
.L_02001a34_1:
	movs r0, #32
	negs r0, r0
	movs r1, #0
	bl 0x02009ad0
.L_02001a34_0:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02001a34_2
	cmp r6, #25
	bne .L_02001a34_2
	ldr r0, [pc, #40]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001a34_2
	movs r0, #0
	movs r1, #32
	bl 0x02009ad0
.L_02001a34_2:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000003f
	.4byte 0x00000040
	.4byte 0x00000309
	.section .text.x02009fac,"ax",%progbits
	.balign 4
	.global Func_02001fac
	.thumb_func
Func_02001fac:
	push {lr}
	ldr r3, [pc, #80]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	bgt .L_02001fac_0
	ldr r3, [pc, #68]
	ldr r2, [r3]
	movs r0, #1
	subs r3, #100
	adds r2, #52
	ldr r1, [r3]
	strb r0, [r2]
	ldr r2, [pc, #56]
	movs r4, #0
	adds r3, r1, r2
	subs r2, #2
	strb r4, [r3]
	adds r3, r1, r2
	strb r0, [r3]
	ldr r3, [pc, #48]
	adds r1, r1, r3
	strb r0, [r1]
	movs r0, #0
	movs r1, #1
	bl 0x0200a3b8
	ldr r0, [pc, #36]
	movs r1, #1
	bl 0x0200a3b0
	movs r0, #16
	bl 0x0200a3c0
	movs r0, #16
	bl 0x0200a260
.L_02001fac_0:
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x03001f30
	.4byte 0x0000053e
	.4byte 0x0000053d
	.4byte 0x00203108
	.section .rodata,"a",%progbits
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200b268
	.4byte 0x0200b2a8
	.4byte 0x0200b2e8
	.global gImiruFuchinKeyHeadings
gImiruFuchinKeyHeadings:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global gImiruFuchinHeadings
gImiruFuchinHeadings:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x00000000
	.global gImiruFuchinDragonsEye
gImiruFuchinDragonsEye:
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0x40000258
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000108
	.4byte 0xc0000121
	.4byte 0x00870000
	.4byte 0x0178003e
	.4byte 0x00000148
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000258
	.4byte 0x00400000
	.4byte 0x01a00040
	.4byte 0x00000290
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc0000278
	.4byte 0x00400000
	.4byte 0x01a00170
	.4byte 0x00000290
	.4byte 0xffff0002
	.4byte 0x00000178
	.4byte 0x400001e8
	.4byte 0x00400000
	.4byte 0x01a00170
	.4byte 0x00000290
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0x400001e8
	.4byte 0x00400000
	.4byte 0x01a00170
	.4byte 0x00000290
	.4byte 0xffff0004
	.4byte 0x000000e8
	.4byte 0x40000198
	.4byte 0x00400000
	.4byte 0x01a00170
	.4byte 0x00000290
	.4byte 0xffff0005
	.4byte 0x000000e8
	.4byte 0xc0000158
	.4byte 0x00400000
	.4byte 0x01a00030
	.4byte 0x00000178
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0xc0000098
	.4byte 0x00400000
	.4byte 0x01a00030
	.4byte 0x00000178
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x400000c8
	.4byte 0x00400000
	.4byte 0x01a00030
	.4byte 0x00000178
	.4byte 0xffff0008
	.4byte 0x000000f8
	.4byte 0x40000068
	.4byte 0x00400000
	.4byte 0x01a00030
	.4byte 0x00000178
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000078
	.4byte 0x00600000
	.4byte 0x01a00040
	.4byte 0x000001e0
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0xc00000c8
	.4byte 0x00600000
	.4byte 0x01a00040
	.4byte 0x000001e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000068
	.4byte 0x00800000
	.4byte 0x01700040
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000b8
	.4byte 0x40000148
	.4byte 0x00600000
	.4byte 0x01a00100
	.4byte 0x00000290
	.4byte 0xffff0003
	.4byte 0x00000098
	.4byte 0x400001b0
	.4byte 0x00600000
	.4byte 0x01a00100
	.4byte 0x00000290
	.4byte 0xffff0004
	.4byte 0x000000b8
	.4byte 0x40000228
	.4byte 0x00600000
	.4byte 0x01a00100
	.4byte 0x00000290
	.4byte 0xffff0005
	.4byte 0x00000118
	.4byte 0xc0000268
	.4byte 0x00600000
	.4byte 0x01a00100
	.4byte 0x00000290
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000148
	.4byte 0x40000048
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0xc0000188
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x400001d8
	.4byte 0x00400000
	.4byte 0x01a001a0
	.4byte 0x00000320
	.4byte 0xffff0004
	.4byte 0x00000158
	.4byte 0xc0000278
	.4byte 0x00400000
	.4byte 0x01a001a0
	.4byte 0x00000320
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000108
	.4byte 0xc00000e8
	.4byte 0x00800000
	.4byte 0x01800010
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x400001d8
	.4byte 0x00400000
	.4byte 0x01a00100
	.4byte 0x000002d0
	.4byte 0xffff0003
	.4byte 0x000000d8
	.4byte 0xc0000298
	.4byte 0x00400000
	.4byte 0x01a00100
	.4byte 0x000002d0
	.4byte 0xffff0004
	.4byte 0x00000118
	.4byte 0xc0000298
	.4byte 0x00400000
	.4byte 0x01a00100
	.4byte 0x000002d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000118
	.4byte 0x40000078
	.4byte 0x00400000
	.4byte 0x01a00000
	.4byte 0x00000158
	.4byte 0xffff0002
	.4byte 0x000000d8
	.4byte 0x40000078
	.4byte 0x00400000
	.4byte 0x01a00000
	.4byte 0x00000158
	.4byte 0xffff0003
	.4byte 0x00000068
	.4byte 0x40000118
	.4byte 0x00400000
	.4byte 0x01a00000
	.4byte 0x00000158
	.4byte 0xffff0004
	.4byte 0x000000c8
	.4byte 0xc0000158
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x00000158
	.4byte 0xffff0005
	.4byte 0x000000f8
	.4byte 0xc0000138
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x00000158
	.4byte 0xffff0006
	.4byte 0x00000128
	.4byte 0xc0000158
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x00000158
	.4byte 0xffff0007
	.4byte 0x000000c8
	.4byte 0x400001b8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff0008
	.4byte 0x000000f8
	.4byte 0x400001b8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff0009
	.4byte 0x00000128
	.4byte 0x400001a8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff000a
	.4byte 0x00000178
	.4byte 0x400001d8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff000b
	.4byte 0x000000f8
	.4byte 0xc0000218
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff000c
	.4byte 0x00000078
	.4byte 0x400002b8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gImiruFuchinExits
gImiruFuchinExits:
	.4byte 0x00000034
	.4byte 0x00109032
	.4byte 0x0000003e
	.4byte 0x0010303c
	.4byte 0x0020203f
	.4byte 0x00304041
	.4byte 0x0040503e
	.4byte 0x0050403e
	.4byte 0x00604040
	.4byte 0x0070c043
	.4byte 0x0080b043
	.4byte 0x0000003f
	.4byte 0x00105040
	.4byte 0x0020203e
	.4byte 0x00000040
	.4byte 0x00103040
	.4byte 0x0020a043
	.4byte 0x00301040
	.4byte 0x0040603e
	.4byte 0x0050103f
	.4byte 0x00000041
	.4byte 0x00103043
	.4byte 0x00203041
	.4byte 0x00302041
	.4byte 0x0040303e
	.4byte 0x00000042
	.4byte 0x00102042
	.4byte 0x00201042
	.4byte 0x00302043
	.4byte 0x00401043
	.4byte 0x00000043
	.4byte 0x00103042
	.4byte 0x00204042
	.4byte 0x00301041
	.4byte 0x00407043
	.4byte 0x00508043
	.4byte 0x00609043
	.4byte 0x00704043
	.4byte 0x00805043
	.4byte 0x00906043
	.4byte 0x00a02040
	.4byte 0x00b0803e
	.4byte 0x00c0703e
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0059005c
	.4byte 0x00000001
	.4byte 0x01070000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0x006e005d
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00df
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008fcd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008ff9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008031
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x0200825d
	.4byte 0x00008602
	.4byte 0xffff000c
	.4byte 0x0200829d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02008e79
	.4byte 0x00004602
	.4byte 0xffff000d
	.4byte 0x020082e1
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x02008315
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008e91
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008e91
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x0200834d
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008ea9
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff001a
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff001b
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff001c
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0xffff0001
	.4byte 0x02009f05
	.4byte 0x00000013
	.4byte 0x0ef10064
	.4byte 0x00500001
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000602
	.4byte 0x0306000b
	.4byte 0x020083b9
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x0200842d
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x02009a35
	.4byte 0x00000602
	.4byte 0xffff0011
	.4byte 0x02008465
	.4byte 0x00008602
	.4byte 0xffff000c
	.4byte 0x020083ed
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff0011
	.4byte 0x02008e79
	.4byte 0x00008602
	.4byte 0xffff000d
	.4byte 0x02009a35
	.4byte 0x00008602
	.4byte 0x0304000e
	.4byte 0x02009a35
	.4byte 0x00004602
	.4byte 0xffff000d
	.4byte 0x020084bd
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x020084f1
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008e91
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008e91
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x02008529
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x02008569
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008ea9
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008041
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte 0x020085b9
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x0200862d
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x02008669
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02009ab1
	.4byte 0x00000602
	.4byte 0xffff000c
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0x130b000c
	.4byte 0x02009ab1
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02008e79
	.4byte 0x00008602
	.4byte 0xffff000e
	.4byte 0x020086fd
	.4byte 0x00000602
	.4byte 0xffff000d
	.4byte 0x02008735
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008e91
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008e91
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte 0x02008769
	.4byte 0x00004602
	.4byte 0xffff0010
	.4byte 0x020087f1
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x020088cd
	.4byte 0x00004602
	.4byte 0x03080011
	.4byte 0x02008921
	.4byte 0x0000c602
	.4byte 0xffff0012
	.4byte 0x020089d1
	.4byte 0x00004602
	.4byte 0x03100012
	.4byte 0x02008a11
	.4byte 0x0000c602
	.4byte 0xffff0013
	.4byte 0x02008a61
	.4byte 0x00004602
	.4byte 0xffff0013
	.4byte 0x02008ab5
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0011
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0012
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0013
	.4byte 0x02008ea9
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte 0x02008b05
	.4byte 0x00004602
	.4byte 0x03080015
	.4byte 0x02008bf5
	.4byte 0x0000c602
	.4byte 0xffff0016
	.4byte 0x02008c25
	.4byte 0x00004602
	.4byte 0xffff0016
	.4byte 0x02008c79
	.4byte 0x00008602
	.4byte 0xffff0017
	.4byte 0x02009ac1
	.4byte 0x00004602
	.4byte 0xffff0017
	.4byte 0x02008ca9
	.4byte 0x0000c602
	.4byte 0xffff0018
	.4byte 0x02008cd9
	.4byte 0x00004602
	.4byte 0xffff0018
	.4byte 0x02008d09
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02008ec1
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x02008ec1
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte 0x02008ec1
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte 0x02008ec1
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte 0x02008ec1
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte 0x02009a35
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x0f13001e
	.4byte 0x02009b9d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x02008d39
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008d6d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02008e79
	.4byte 0x00008602
	.4byte 0x0313000d
	.4byte 0x02008da5
	.4byte 0x00000602
	.4byte 0xffff000e
	.4byte 0x02008dd9
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008e91
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008e91
	.4byte 0x00000602
	.4byte 0xffff000f
	.4byte 0x02008e0d
	.4byte 0x00008602
	.4byte 0xffff0010
	.4byte 0x02008e41
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008ea9
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000013
	.4byte 0x0f6b0064
	.4byte 0x00100008
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0xffff0007
	.4byte 0x02009f21
	.4byte 0x00000013
	.4byte 0x0f140064
	.4byte 0x001000c8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000031
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000031
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x0200970d
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02009731
	.4byte 0x00000003
	.4byte 0xffff0013
	.4byte 0x02009f3d
	.4byte 0x0000e604
	.4byte 0x08200014
	.4byte 0x02009e09
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02009f59
	.4byte 0x40009085
	.4byte 0x08200000
	.4byte 0x02009fad
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02009c09
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02009c09
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02009c09
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
