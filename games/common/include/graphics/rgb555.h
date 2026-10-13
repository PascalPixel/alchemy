#ifndef ALCHEMY_RGB555_H
#define ALCHEMY_RGB555_H

// RGB555 の色：赤はビット 0〜4、緑は 5〜9、青は 10〜14

// 色を 1 つ p に書いて次へ進む
// A statement macro, written do { } while (0): both copies of
// PaletteGlow_UpdateSine match through that shape. As a plain statement
// the red add sinks below the shifts (differ 6); with the channel sums
// written as the macro's arguments, 26.
#define Palette_PutRgb555(p, r, g, b)            \
	do                                           \
	{                                            \
		*(p)++ = ((b) << 10) | ((g) << 5) | (r); \
	} while (0)

#endif
