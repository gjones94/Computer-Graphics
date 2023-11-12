#include "utilities.h"
#include "light.h"

uint32_t get_shaded_color(float light_percentage, uint32_t color)
{
	float scaledMax = 1.0f;

	//Results of a unit vector dot product of surface normal to a light source. 
	float dotProductMaximum = 1.0f;
	float dotProductMinimum = -1.0f;
	float range = dotProductMaximum - dotProductMinimum;

	// Scale lighting due to coordinates being between -1 and 1, need to change to be from 0 to 1 to apply proper percentage
	float percentageScaled = ((light_percentage - dotProductMinimum) / range) * scaledMax;

	uint32_t light_adjuster = (int) (255 * (float) percentageScaled);

	uint32_t light_mask = 0;

	// lock the transparency
	uint32_t transparency = 0xFF << 24;
	light_mask |= transparency;

	// BLUE
	light_mask |= (light_adjuster << 16);

	// GREEN
	light_mask |= (light_adjuster << 8);

	// RED
	light_mask |= (light_adjuster);
	
	// Apply adjustment to the color
	color = color & light_mask;

	return color;
}
