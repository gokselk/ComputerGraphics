#include <iostream>
#include "parser.h"

#include "Scene.h"

int main(int argc, char* argv[])
{
    if(argc < 1)
    {
        std::cout << "Usage: ./raytracer <scene_file>" << std::endl;
        return 1;
    }
    parser::Scene parser_scene;
    parser_scene.loadFromXml(argv[1]);
    Scene scene(parser_scene);
    scene.render_multithreaded();
}
