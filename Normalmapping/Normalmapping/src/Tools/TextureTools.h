#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

namespace TextureTools {
    GLuint loadTexture(std::string fileName, bool flipYAxis = false);
}
