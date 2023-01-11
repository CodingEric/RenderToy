/*
 *  OpenPT - Mesh Object Module
 *  File created on 2023/1/11
 *  Last edited on 2023/1/11
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef MESHOBJ_H
#define MESHOBJ_H

#include "mathfunc.h"

#include <vector>
#include <string>
#include <tuple>
#include <array>

namespace OpenPT
{
    class Mesh
    {
    public:
        std::string &get_name();
        std::vector<Vector3f> &get_geo_vert();
        std::vector<Vector3f> &get_vert_norm();
        std::vector<Vector2f> &get_vert_tex();

    private:
        std::string name;
        std::vector<Vector3f> geo_vert;
        std::vector<Vector3f> vert_norm;
        std::vector<Vector2f> vert_tex;

        // Currently, we only support triangulated mesh!
        // TODO <tianyu@illumiart.net>: Too ugly! Needs refactor.
        std::vector<std::array<std::array<int, 3>, 3>> face;
    };
};

#endif