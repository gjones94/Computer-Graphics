#include "utilities.h"
#include "light.h"

uint32_t adjust_color_lighting(float percentage, uint32_t color)
{
	uint32_t light_adjuster = (int) (255 * (float) percentage);

	if (light_adjuster > 255)
	{
		light_adjuster = 255;
	}

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
