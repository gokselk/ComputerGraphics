#include "EclipseMap.h"

using namespace std;

bool szChanged = false;
float width;
float height;

void EclipseMap::SetVertices(vector<vertex> &vertices, float radius) {
    float alphaStep = 2.f * static_cast<float>(M_PI) / static_cast<float>(horizontalSplitCount);
    float betaStep = static_cast<float>(M_PI) / static_cast<float>(verticalSplitCount);
    for(int verticalStep = 0; verticalStep <= verticalSplitCount; verticalStep++){
        float beta = static_cast<float>(M_PI) / 2.f - betaStep * static_cast<float>(verticalStep);
        float rcosb = radius * cos(beta);
        float z = radius * sin(beta);
        for(int horizontalStep = 0; horizontalStep <= horizontalSplitCount; horizontalStep++){
            float alpha = static_cast<float>(horizontalStep) * alphaStep;
            float x = rcosb * cos(alpha);
            float y = rcosb * sin(alpha);
            float u = static_cast<float>(horizontalStep) / static_cast<float>(horizontalSplitCount);
            float v = static_cast<float>(verticalStep) / static_cast<float>(verticalSplitCount);
            vertices.push_back(
                    vertex{
                            {x, y, z},
                            glm::normalize(glm::vec3{x, y, z}),
                            {u, v}
                    }
            );
        }
    }
}

void EclipseMap::SetIndices(vector<unsigned int> &indices){
    for(int verticalStep = 0; verticalStep < verticalSplitCount; verticalStep++){
        int k1 = verticalStep * (horizontalSplitCount + 1);
        int k2 = k1 + horizontalSplitCount + 1;
        for(int horizontalStep = 0; horizontalStep < horizontalSplitCount; horizontalStep++, k1++, k2++){
            if(verticalStep != 0){
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if(verticalStep != horizontalSplitCount - 1){
                indices.push_back(k1+1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
    szChanged = true;
    width = (float)w;
    height = (float)h;
}

void EclipseMap::Render(const char *coloredTexturePath, const char *greyTexturePath, const char *moonTexturePath) {
    // Open window
    GLFWwindow *window = openWindow(windowName, screenWidth, screenHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Moon commands
    // Load shaders
    GLuint moonShaderID = initShaders("moonShader.vert", "moonShader.frag");

    initMoonColoredTexture(moonTexturePath, moonShaderID);

    
    // TODO: Set moonVertices
    SetVertices(moonVertices, moonRadius);
    SetIndices(moonIndices);

    // TODO: Configure Buffers

    // VAO
    glGenVertexArrays(1, &moonVAO);
    glBindVertexArray(moonVAO);

    // VBO
    glGenBuffers(1, &moonVBO);
    glBindBuffer(GL_ARRAY_BUFFER, moonVBO);

    // Terrain vertexes
    glBufferData(GL_ARRAY_BUFFER, moonVertices.size() * sizeof(vertex), moonVertices.data() , GL_STATIC_DRAW);

    glGenBuffers(1, &moonEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moonEBO);

    // Terrain indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, moonIndices.size() * sizeof(GLuint), moonIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, texture));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    

    // World commands
    // Load shaders
    GLuint worldShaderID = initShaders("worldShader.vert", "worldShader.frag");

    initColoredTexture(coloredTexturePath, worldShaderID);
    initGreyTexture(greyTexturePath, worldShaderID);

    // TODO: Set worldVertices
    SetVertices(worldVertices, radius);
    SetIndices(worldIndices);
    
    // TODO: Configure Buffers

    //modelMatrix = glm::rotate(modelMatrix, (float) glm::radians(-60.0), glm::vec3(1, 0, 0));


    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Terrain vertexes
    glBufferData(GL_ARRAY_BUFFER, worldVertices.size() * sizeof(vertex), worldVertices.data() , GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Terrain indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, worldIndices.size() * sizeof(GLuint), worldIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, texture));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Main rendering loop
    do {
        glViewport(0, 0, screenWidth, screenHeight);

        glClearStencil(0);
        glClearDepth(1.0f);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if(szChanged){
            screenHeight = (int)height;
            screenWidth = (int)width;
            aspectRatio = width / height;
            szChanged = false;
        }

        handleKeyPress(window);

        // TODO: Manipulate rotation variables

        orbitDegree += 1.25f;
        if(orbitDegree >= 360.0f){
            orbitDegree -= 360.0f;
        }
        earthDegree += 125.0f / static_cast<float>(horizontalSplitCount);
        if(earthDegree >= 360.0f){
            earthDegree -= 360.0f;
        }
        moonDegree += 125.0f / static_cast<float>(horizontalSplitCount);
        if(moonDegree >= 360.0f){
            moonDegree -= 360.0f;
        }

        cameraPosition += speed * cameraDirection;

        
        // TODO: Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColor);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, moonTextureColor);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureGrey);
        
        // TODO: Use moonShaderID program
        glUseProgram(moonShaderID);
        
        // TODO: Update camera at every frame

        modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(moonDegree), glm::vec3(0, 0, 1));
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
        projectionMatrix = glm::perspective(glm::radians(projectionAngle), aspectRatio, near, far);
        modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        
        // TODO: Update uniform variables at every frame

        glUniform3fv(glGetUniformLocation(moonShaderID, "lightPosition"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(moonShaderID, "cameraPosition"), 1, glm::value_ptr(cameraPosition));

        glUniformMatrix4fv(glGetUniformLocation(moonShaderID, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(glGetUniformLocation(moonShaderID, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(moonShaderID, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(moonShaderID, "MVP"), 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

        glUniform1f(glGetUniformLocation(moonShaderID, "heightFactor"), heightFactor);

        glUniform1f(glGetUniformLocation(moonShaderID, "orbitDegree"), orbitDegree);

        glUniform1f(glGetUniformLocation(moonShaderID, "moonImageWidth"), imageWidth);
        glUniform1f(glGetUniformLocation(moonShaderID, "moonImageHeight"), imageHeight);


        glUniform1i(glGetUniformLocation(moonShaderID, "TexColor"), 0);
        glUniform1i(glGetUniformLocation(moonShaderID, "MoonTexColor"), 1);
        glUniform1i(glGetUniformLocation(moonShaderID, "TexGrey"), 2);
        glUniform1f(glGetUniformLocation(moonShaderID, "textureOffset"), textureOffset);

        
        // TODO: Bind moon vertex array

        glBindVertexArray(moonVAO);

        // TODO: Draw moon object

        glDrawElements(GL_TRIANGLES, moonIndices.size(), GL_UNSIGNED_INT, nullptr);

        
        /*************************/

        // TODO: Use worldShaderID program
        glUseProgram(worldShaderID);
        
        // TODO: Update camera at every frame

        modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(earthDegree), glm::vec3(0, 0, 1));
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
        projectionMatrix = glm::perspective(projectionAngle, aspectRatio, near, far);
        modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        // TODO: Update uniform variables at every frame

        glUniform3fv(glGetUniformLocation(worldShaderID, "lightPosition"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(worldShaderID, "cameraPosition"), 1, glm::value_ptr(cameraPosition));

        glUniformMatrix4fv(glGetUniformLocation(worldShaderID, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(glGetUniformLocation(worldShaderID, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(worldShaderID, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(worldShaderID, "MVP"), 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

        glUniform1f(glGetUniformLocation(worldShaderID, "heightFactor"), heightFactor);

        glUniform1f(glGetUniformLocation(worldShaderID, "orbitDegree"), orbitDegree);

        glUniform1f(glGetUniformLocation(worldShaderID, "ImageWidth"), imageWidth);
        glUniform1f(glGetUniformLocation(worldShaderID, "ImageHeight"), imageHeight);


        glUniform1i(glGetUniformLocation(worldShaderID, "TexColor"), 0);
        glUniform1i(glGetUniformLocation(worldShaderID, "MoonTexColor"), 1);
        glUniform1i(glGetUniformLocation(worldShaderID, "TexGrey"), 2);
        glUniform1f(glGetUniformLocation(worldShaderID, "textureOffset"), textureOffset);
        
        // TODO: Bind world vertex array
        glBindVertexArray(VAO);
        
        // TODO: Draw world object

        glDrawElements(GL_TRIANGLES, worldIndices.size(), GL_UNSIGNED_INT, nullptr);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));

    // Delete buffers
    glDeleteBuffers(1, &moonVAO);
    glDeleteBuffers(1, &moonVBO);
    glDeleteBuffers(1, &moonEBO);

    
    // Delete buffers
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
   
    glDeleteProgram(moonShaderID);
    glDeleteProgram(worldShaderID);

    // Close window
    glfwTerminate();
}

void EclipseMap::handleKeyPress(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        heightFactor += 5;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        heightFactor -= 5;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraUp = glm::rotate(cameraUp, -0.05f, cameraAxis);
        cameraDirection = glm::rotate(cameraDirection, -0.05f, cameraAxis);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraUp = glm::rotate(cameraUp, 0.05f, cameraAxis);
        cameraDirection = glm::rotate(cameraDirection, 0.05f, cameraAxis);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraAxis = glm::rotate(cameraAxis, 0.05f, cameraUp);
        cameraDirection = glm::rotate(cameraDirection, 0.05f, cameraUp);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraAxis = glm::rotate(cameraAxis, -0.05f, cameraUp);
        cameraDirection = glm::rotate(cameraDirection, -0.05f, cameraUp);
    }
    if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        speed += .01f;
    if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        speed -= .01f;
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        speed = 0;
    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        cameraDirection = cameraStartDirection;
        cameraPosition = cameraStartPosition;
        cameraAxis = glm::cross(cameraUp, cameraDirection);
        cameraUp = cameraStartUp;
        pitch = startPitch;
        yaw = startYaw;
        speed = 0;
    }
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pKeyPressed) {
        pKeyPressed = true;
        if(displayFormat == displayFormatOptions::fullScreen){
            displayFormat = displayFormatOptions::windowed;
            glfwSetWindowMonitor(window, nullptr, window_x, window_y, window_w, window_h, 0);
            glViewport(0, 0, window_w, window_h);
        }
        else {
            displayFormat = displayFormatOptions::fullScreen;
            glfwGetWindowPos(window, &window_x, &window_y);
            glfwGetWindowSize(window, &window_w, &window_h);
            auto monitor = glfwGetPrimaryMonitor();
            auto mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
            glViewport(0, 0, mode->width, mode->height);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
        pKeyPressed = false;
    }
}

GLFWwindow *EclipseMap::openWindow(const char *windowName, int width, int height) {
    if (!glfwInit()) {
        getchar();
        return 0;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, windowName, NULL, NULL);
    glfwSetWindowMonitor(window, NULL, 1, 31, screenWidth, screenHeight, mode->refreshRate);

    if (window == NULL) {
        getchar();
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        getchar();
        glfwTerminate();
        return 0;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0, 0, 0, 0);

    return window;
}


void EclipseMap::initColoredTexture(const char *filename, GLuint shader) {
    int width, height;
    glGenTextures(1, &textureColor);
    cout << shader << endl;
    glBindTexture(GL_TEXTURE_2D, textureColor);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
   

    imageWidth = width;
    imageHeight = height;

    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "TexColor"), 0);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);

}

void EclipseMap::initGreyTexture(const char *filename, GLuint shader) {

    glGenTextures(1, &textureGrey);
    glBindTexture(GL_TEXTURE_2D, textureGrey);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
  



    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "TexGrey"), 1);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);

}

void EclipseMap::initMoonColoredTexture(const char *filename, GLuint shader) {
    int width, height;
    glGenTextures(1, &moonTextureColor);
    cout << shader << endl;
    glBindTexture(GL_TEXTURE_2D, moonTextureColor);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
   

    imageWidth = width;
    imageHeight = height;

    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "MoonTexColor"), 2);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);

}