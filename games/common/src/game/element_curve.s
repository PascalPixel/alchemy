@ The element power and resistance curve, the same in both games: pairs of
@ an input and its value in 256ths, from 200 down to -200, between which
@ Curve_LookupScaledValue interpolates. Each game's element tables include
@ it in place.
	.global gElement_PowerResistCurve
gElement_PowerResistCurve:
	.2byte 200, 256
	.2byte 100, 128
	.2byte 0, 0
	.2byte -100, -128
	.2byte -200, -256
