#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "stb/stb_image.h"

namespace TextureTools {
    GLuint loadTexture(std::string fileName, bool flipYAxis = false);
	GLuint loadCubemap(std::vector<std::string> faces);
}
