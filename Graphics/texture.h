#pragma once
#include <stdint.h>
#include "upng.h"

typedef struct {

    float u;
	float v;

} text2_t;

extern int texture_width;
extern int texture_height;
extern const uint8_t REDBRICK_TEXTURE[];

extern uint32_t* mesh_texture;
extern upng_t* png_texture;

void load_png_texture(char* filename);
void swap_textures(text2_t* uv1, text2_t* uv2);
