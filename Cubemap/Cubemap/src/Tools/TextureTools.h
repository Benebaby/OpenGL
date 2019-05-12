#ifndef __TEXTURETOOLS_H
#define __TEXTURETOOLS_H
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "stb/stb_image.h"

namespace TextureTools {
    GLuint loadTexture(std::string fileName, bool flipYAxis = false);
	GLuint loadCubemap(std::vector<std::string> faces);
}

#endif /* __TEXTURETOOLS_H */