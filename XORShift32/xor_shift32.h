#pragma once

#include "constants.h"

static inline uint XORShift32(uint *state){
    uint x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return *state = x;
}

static inline uint GetRandomUint(const uint maxValue, uint *state){
    return XORShift32(state) % (maxValue + 1);
}