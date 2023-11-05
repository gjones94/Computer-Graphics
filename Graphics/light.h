#pragma once
#include <stdint.h>
#include "vector.h"

typedef struct
{
	vec3_t direction;

} light_t;

uint32_t adjust_color_lighting(float percentage, uint32_t color);