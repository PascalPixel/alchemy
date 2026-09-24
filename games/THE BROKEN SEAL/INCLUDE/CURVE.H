#ifndef ALCHEMY_CURVE_H
#define ALCHEMY_CURVE_H

#include "TYPES.H"

enum {
    CURVE_VALUE_SHIFT = 19,
    CURVE_HALF_STEPS = 8,
    CURVE_FULL_STEPS = 16,
};

extern u8 Curve_LerpWeightTable[];
extern u8 Curve_SampleIndexTable[];

#endif
