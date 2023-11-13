#pragma once
#include <stdint.h>
#include "vector.h"

typedef struct
{
	vec3_t direction;

} light_t;

/// <summary>
/// Returns color adjusted by brightness percentage
/// </summary>
/// <param name="light_percentage: floating point scale between 0 and 1, with 1 being bright, and 0 being dark"></param>
/// <param name="color: The color being adjusted"></param>
/// <returns></returns>
uint32_t get_shaded_color(float light_intensity, uint32_t color);