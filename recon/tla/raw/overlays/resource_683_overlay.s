.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r1, .L_0200807c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008080
	cmp r2, r3
	bne .L_02008078
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_02008078
	ldr r0, .L_02008084
	b .L_0200807a
.L_02008078:
	ldr r0, .L_02008088
.L_0200807a:
	pop {pc}
.L_0200807c:
	.4byte gPartyState
.L_02008080:
	.4byte 0x000000b7
.L_02008084:
	.4byte Data_020003a8
.L_02008088:
	.4byte Data_02000228
	.section .text.x0200808c,"ax",%progbits
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {lr}
	adds r1, r0, #0
	movs r0, #25
	bl Func_02000178
	pop {pc}
	.section .text.x02008098,"ax",%progbits
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {lr}
	adds r1, r0, #0
	movs r0, #10
	bl Func_02000180
	pop {pc}
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {lr}
	ldr r1, .L_020080e8
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_020080ec
	cmp r2, r3
	bne .L_020080d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_020080d0
	ldr r0, .L_020080f0
	b .L_020080e4
.L_020080d0:
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080e2
	ldr r0, .L_020080f4
	b .L_020080e4
.L_020080e2:
	ldr r0, .L_020080f8
.L_020080e4:
	pop {pc}
	.2byte 0x0000
.L_020080e8:
	.4byte gPartyState
.L_020080ec:
	.4byte 0x000000b7
.L_020080f0:
	.4byte Data_02000780
.L_020080f4:
	.4byte Data_020005ac
.L_020080f8:
	.4byte Data_020003d8
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	movs r2, #133
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	ldr r1, .L_02008140
	str r2, [r3]
	subs r2, #41
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008144
	cmp r2, r3
	bne .L_0200813a
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_0200813a
	bl Func_0200014c
.L_0200813a:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008140:
	.4byte gPartyState
.L_02008144:
	.4byte 0x000000b7
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #48
	adds r2, #93
	str r2, [r3]
	adds r0, #255
	bl GameFlag_ClearBit
	pop {pc}
	.section .rodata.x02008188,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000001a0
	.4byte 0xc0000298
	.4byte 0x01200000
	.4byte 0x02100218
	.4byte 0x000002b8
	.4byte 0xffff0009
	.4byte 0x00000260
	.4byte 0xc0000298
	.4byte 0x01e80000
	.4byte 0x02d80210
	.4byte 0x000002c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000b7
	.4byte 0x101010b6
	.4byte 0xffffffff
	.4byte 0x102020b6
	.4byte 0xffffffff
	.4byte 0x103030b6
	.4byte 0xffffffff
	.4byte 0x104040b6
	.4byte 0xffffffff
	.4byte 0x105050b6
	.4byte 0xffffffff
	.4byte 0x106060b6
	.4byte 0xffffffff
	.4byte 0x0080b086
	.4byte 0x00902083
	.4byte 0x000001ff
	.global Data_02000228
Data_02000228:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00015000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00010000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0001d000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00008000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00018000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0000c000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00010000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00015000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00018000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00015000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x005b0000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020003a8
Data_020003a8:
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x02640000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020003d8
Data_020003d8:
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002406
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002407
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002408
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte 0x00002409
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000240a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000240b
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000240c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000240d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000240e
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000240f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002410
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002411
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002412
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002413
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002414
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002415
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002416
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002417
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002418
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002419
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000241a
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000241b
	.4byte 0x0000c400
	.4byte 0xffff0013
	.4byte Func_0200008c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002431
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002432
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002433
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002434
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002435
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002436
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002437
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002438
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020005ac
Data_020005ac:
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025dd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025de
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000025df
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte 0x000025e0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025e1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025e2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025e3
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000025e4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025e5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000025e6
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025e7
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025e8
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025e9
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025ea
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000025eb
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000025ec
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025ed
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000025ee
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000025ef
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000025f0
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000025f1
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000025f2
	.4byte 0x0000c400
	.4byte 0xffff0013
	.4byte Func_0200008c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000026a8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000026a9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000026aa
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000026ab
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000026ac
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x000026ad
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000026ae
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000026af
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000780
Data_02000780:
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000218d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002191
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
