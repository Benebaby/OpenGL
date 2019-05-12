#include <iostream>
#include <stdio.h>
#include <string>
#include "stb/stb_image.h"
#include "TextureTools.h"

namespace TextureTools {
    GLuint loadTexture(std::string fileName, bool flipYAxis){
        int width, height, bytesPerPixel;
        unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &bytesPerPixel, 0);

        //create new texture
        GLuint textureHandle;
        glGenTextures(1, &textureHandle);
     
        //bind the texture
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        if(flipYAxis){
        		int size = width*bytesPerPixel;
        	    	unsigned char * tmpBuffer = new unsigned char[size];
        	    	for (int i=0;i<height/2;i++){
        	    	    // copy row i to tmp
        	    	    memcpy(tmpBuffer,
        	    	    		   	   data+width*bytesPerPixel*i,
        	    	    		   	   size);
        	    	    // copy row h-i-1 to i
        	    	    memcpy(data+width*bytesPerPixel*i,
        	    	    		   	   data+width*bytesPerPixel*(height-i-1),
        	    	    		   	   size);
        	    	    // copy tmp to row h-i-1
        	    	    memcpy(data+width*bytesPerPixel*(height-i-1),
        	    	    		   	   tmpBuffer,
        	    	    		   	   size);
        	    	    }
        	    	    delete [] tmpBuffer;
        }
     
        //send image data to the new texture
        if (bytesPerPixel < 3) {
            std::cout << "ERROR: Unable to open image"  << fileName << std::endl;
            return -1;
        } else if (bytesPerPixel == 3){
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (bytesPerPixel == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        //texture settings
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);

        std::cout << "SUCCESS: image loaded from " << fileName << std::endl;
        return textureHandle;
    }
}
