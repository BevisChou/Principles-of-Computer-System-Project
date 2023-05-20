#pragma once

#include "common.hh"
#include <stack>
#include <vector>

#define	get_bit(x, bit)	((x & (1 << bit)) >> bit)
#define	reset_bit(x, bit)	(x &= ~(1 << bit))
#define	set_bit(x, bit)	(x |= (1 << bit))

Word int_to_bits(string);
Word float_to_bits(string);
string bits_to_int(Word);
string bits_to_float(Word);
