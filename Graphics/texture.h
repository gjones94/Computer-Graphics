#pragma once
#include <stdint.h>

typedef struct {

    float u;
	float v;

} text2_t;

extern const int TEXTURE_WIDTH;
extern const int TEXTURE_HEIGHT;
extern const uint8_t REDBRICK_TEXTURE[];

extern uint32_t* mesh_texture;

void swap_textures(text2_t* uv1, text2_t* uv2);
