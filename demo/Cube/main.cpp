#include "../../include/OpenPT/openpt.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace OpenPT;

int main()
{
    #ifdef __clang__
    std::cout<<"Clang"<<std::endl;
    #endif

    std::cout << "Begin importing...\n";

    World world;
    OBJModelImporter::Import(world, "./cornellbox.obj");

    world.meshes[0]->tex = Texture(Vector3f::White, Vector3f::O);
    world.meshes[1]->tex = Texture(Vector3f::X, Vector3f::O);
    world.meshes[2]->tex = Texture(Vector3f::Y, Vector3f::O);
    world.meshes[3]->tex = Texture(Vector3f::White, Vector3f::O);
    world.meshes[4]->tex = Texture(Vector3f::White, Vector3f::O);
    world.meshes[5]->tex = Texture(Vector3f::White, Convert::BlackBody(6000) * 5.0f);
    // world.sky_emission = Convert::BlackBody(4000) * 0.3f;
    // world.ground_reflection = {0.1,0.09,0.07};
    world.PrepareDirectLightSampling();

    std::cout << "Imported " << world.triangles.size() << " triangle(s) and " << world.meshes.size() << " mesh(es).\n";
    world.cameras.push_back(AcademyCamera);

    Matrix4x4f camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 10.0f});
    camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 1.0f})*AffineTransformation::RotationEulerXYZ({0.0f, 0.0f, Convert::DegreeToRadians(-90)}) * AffineTransformation::RotationEulerXYZ({Convert::DegreeToRadians(90), 0.0f, 0.0f}) * camera_mat4;
    world.cameras[0].SetO2W(camera_mat4);

    std::cout << "Begin rendering...\n";

    RenderContext rc(&world, FormatSettings(Size(1920, 1080), Vector2f(16.0f, 9.0f)));
    PathTracingRenderer renderer(&rc, 16);
    clock_t timeStart = clock();
    renderer.Render();
    clock_t timeEnd = clock();
    printf("Render time: %04.2f (sec)\n", (float)(timeEnd - timeStart) / CLOCKS_PER_SEC);

    std::cout << "Exporting...\n";

    std::ofstream os;
    os.open("./cornellbox.bmp");

    BMPExporter exporter(&rc);
    exporter.Export(os);

    os.close();

    std::cout << "Completed.\n";

    return 0;
}
