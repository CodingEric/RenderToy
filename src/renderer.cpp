#define BUFFER(x, y, width) render_context->buffer[y * width + x]

#include "renderer.h"
#include "camera.h"
#include "ray.h"
#include "bvh.h"
#include "surfacepoint.h"

#include <algorithm>
#include <cmath>

namespace RenderToy
{
    IntersectTestRenderer::IntersectTestRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
    }

    void IntersectTestRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);
        float top, right;
        PrepareScreenSpace(cam, top, right);
        float div_resolution_width = 1.0f / float(render_context->format_settings.resolution.width);
        float div_resolution_height = 1.0f / float(render_context->format_settings.resolution.height);

        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) * div_resolution_width, float(y) * div_resolution_height};
                Vector2f screen_coord = {2.0f * right * NDC_coord.x - right, 2.0f * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));
                cast_ray = cam->O2WTransform(cast_ray);

                float t, u, v;
                auto intersected = render_context->bvh->Intersect(cast_ray, t, u, v, nullptr);

                if (intersected != nullptr)
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f::White;
                }
                else
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f::O;
                }
            }
        }
    }

    TestRenderer::TestRenderer(const Size &resolution_)
        : IRenderer(new RenderContext(nullptr, FormatSettings(resolution_, Vector2f::O)))
    {
    }

    void TestRenderer::Render()
    {
        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f(float(x) / render_context->format_settings.resolution.width, 1.0f, 1.0f);
            }
        }
    }

    FormatSettings::FormatSettings(Size resolution_, Vector2f aspect_)
        : resolution(resolution_), aspect(aspect_)
    {
    }

    RenderContext::RenderContext(World *world_, FormatSettings format_settings_)
        : world(world_), format_settings(format_settings_)
    {
        bvh = new BVH(world->triangles);
        buffer = new Vector3f[format_settings.resolution.Area()];
    }

    RenderContext::~RenderContext()
    {
        delete[] buffer;
        delete bvh;
    }

    void IRenderer::PrepareScreenSpace(Camera *cam, float &top, float &right)
    {
        top = (Convert::InchToMM(cam->gate_dimension.y) / 2.0f) / cam->focal_length;
        right = (Convert::InchToMM(cam->gate_dimension.x) / 2.0f) / cam->focal_length;
        // We implement FILL CONVENTION when aspect ratio conflicts with gate dimension.
        // Corresponding to AUTO setting of Sensor Fit in Blender.
        float gate_aspr = cam->gate_dimension.x / cam->gate_dimension.y;
        float film_aspr = render_context->format_settings.resolution.AspectRatio();
        float xscale = 1.0f, yscale = 1.0f;
        if (gate_aspr >= film_aspr)
        {
            // Gate is wider than Film.
            xscale = film_aspr / gate_aspr;
        }
        else
        {
            // Film is wider than Gate.
            yscale = gate_aspr / film_aspr;
        }
        right *= xscale;
        top *= yscale;
    }

    IRenderer::IRenderer(RenderContext *render_context_)
        : render_context(render_context_)
    {
    }

    DepthBufferRenderer::DepthBufferRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
        near = render_context_->world->cameras[render_context_->camera_id].near_clipping_plane;
        far = render_context_->world->cameras[render_context_->camera_id].far_clipping_plane;
    }

    void DepthBufferRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);
        float top, right;
        PrepareScreenSpace(cam, top, right);

        float div_far_minus_near = 1.0f / (far - near);
        float div_resolution_width = 1.0f / float(render_context->format_settings.resolution.width);
        float div_resolution_height = 1.0f / float(render_context->format_settings.resolution.height);

        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) * div_resolution_width, float(y) * div_resolution_height};
                Vector2f screen_coord = {2.0f * right * NDC_coord.x - right, 2.0f * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));
                cast_ray = cam->O2WTransform(cast_ray);

                float t, u, v;
                auto intersected = render_context->bvh->Intersect(cast_ray, t, u, v, nullptr);

                if (intersected != nullptr)
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f(std::clamp((t - near) * div_far_minus_near, 0.0f, 1.0f));
                }
                else
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f(1.0f);
                }
            }
        }
    }

    DepthBufferRenderer::DepthBufferRenderer(RenderContext *render_context_, float near_, float far_)
        : IRenderer(render_context_), near(near_), far(far_)
    {
    }

    NormalRenderer::NormalRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
    }

    void NormalRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);
        float top, right;
        PrepareScreenSpace(cam, top, right);

        float div_resolution_width = 1.0f / float(render_context->format_settings.resolution.width);
        float div_resolution_height = 1.0f / float(render_context->format_settings.resolution.height);

        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) * div_resolution_width, float(y) * div_resolution_height};
                Vector2f screen_coord = {2.0f * right * NDC_coord.x - right, 2.0f * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));
                cast_ray = cam->O2WTransform(cast_ray);

                float t, u, v;
#ifdef USE_PREDEFINED_NORMAL
                auto intersected = render_context->bvh->Intersect(cast_ray, t, u, v, nullptr);
#else
                Vector3f position;
                auto intersected = render_context->bvh->Intersect(cast_ray, position, nullptr);
                SurfacePoint sp(intersected, position);
#endif

                if (intersected != nullptr)
                {
#ifdef USE_PREDEFINED_NORMAL
                    BUFFER(x, y, render_context->format_settings.resolution.width) = (((1 - u - v) * intersected->norm[0] + u * intersected->norm[1] + v * intersected->norm[2]) + Vector3f::White) / 2.0f;
#else
                    auto normal = sp.GetNormal();
                    if (Vector3f::Dot(normal, -cast_ray.direction) < 0.0f)
                    {
                        normal = -normal;
                    }
                    BUFFER(x, y, render_context->format_settings.resolution.width) = (normal + Vector3f::White) / 2.0f;
#endif
                }
                else
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f::O;
                }
            }
        }
    }

    PathTracingRenderer::PathTracingRenderer(RenderContext *render_context_, const int iteration_count_)
        : IRenderer(render_context_), iteration_count(iteration_count_)
    {
    }

    void PathTracingRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);
        float top, right;
        PrepareScreenSpace(cam, top, right);

        float div_resolution_width = 1.0f / float(render_context->format_settings.resolution.width);
        float div_resolution_height = 1.0f / float(render_context->format_settings.resolution.height);

#pragma omp parallel for
        for (int i = 0; i < iteration_count; ++i)
        {
            for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
            {
                for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
                {
                    if (x == 638 && y == 720 - 509)
                    {
                        const int a = 2;
                    }

                    // (x, y) is the point in Raster Space.
                    Vector2f NDC_coord = {float(x) * div_resolution_width, float(y) * div_resolution_height};
                    Vector2f screen_coord = {2.0f * right * NDC_coord.x - right, 2.0f * top * NDC_coord.y - top};

                    // Blender convention: Camera directing towards -z.
                    Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));
                    cast_ray = cam->O2WTransform(cast_ray);

                    RayState state;
                    BUFFER(x, y, render_context->format_settings.resolution.width) += Radiance(cast_ray.src, cast_ray.direction, nullptr, state, 0, 0.0f) / static_cast<float>(iteration_count);
                }
            }
        }
    }

    const Vector3f PathTracingRenderer::Radiance(const Vector3f &ray_src, const Vector3f &ray_dir, const Triangle *last_hit, RayState &state, const int depth, const float last_bsdfpdf) const
    {
        if (depth > 4)
        {
            return Vector3f::O;
        }
        // intersect ray with scene
        const Triangle *hit_obj = nullptr;
        Vector3f hitPosition;
        Ray cast_ray(ray_src, ray_dir);
        hit_obj = render_context->bvh->Intersect(cast_ray, hitPosition, last_hit);

        Vector3f radiance;
        if (hit_obj != nullptr)
        {
            SurfacePoint surface_point(hit_obj, hitPosition);
            auto ffnormal = surface_point.GetNormal();
            if (Vector3f::Dot(-ray_dir, ffnormal) < 0.0f)
            {
                ffnormal = -ffnormal;
                state.eta = surface_point.GetMaterial()->ior;
                state.absorption = Vector3f::O;
            }
            else
            {
                state.eta = 1.0f / surface_point.GetMaterial()->ior;
            }

            float self_emission_pdf;
            auto self_emission = surface_point.GetEmission(ray_src, -ray_dir, true, self_emission_pdf);
            if (depth == 0)
            {
                radiance += self_emission;
            }
            else
            {
                radiance += PowerHeuristic(last_bsdfpdf, self_emission_pdf) * self_emission;
            }

            auto bounce_ratio = Vector3f::Pow(Vector3f(M_Ef32), -state.absorption * (hitPosition - ray_src).Length());

            radiance += DirectLight(state, ray_dir, surface_point) * bounce_ratio;
            Vector3f nextDirection;
            Vector3f color;
            float bsdfpdf;
            if (surface_point.GetNextDirection(-ray_dir, nextDirection, color, bsdfpdf, state))
            {
                state.absorption = -Vector3f::Log(surface_point.GetMaterial()->extinction) / surface_point.GetMaterial()->at_distance;

                if (bsdfpdf > 0.0f)
                {
                    radiance += bounce_ratio * color / bsdfpdf * Radiance(surface_point.GetPosition(), nextDirection, surface_point.GetHitTriangle(), state, depth + 1, bsdfpdf);
                }
            }
        }
        else
        {
            radiance = render_context->world->GetDefaultEmission(-ray_dir);
        }

        return radiance;
    }

    const Vector3f PathTracingRenderer::DirectLight(const RayState state, const Vector3f &original_ray_dir, const SurfacePoint &surface_point) const
    {

        Vector3f ret;
        auto tri = surface_point.GetHitTriangle();
        auto normal = tri->NormalC();
        if (Vector3f::Dot(-original_ray_dir, normal) < 0.0f)
        {
            normal = -normal; // TODO: 优化重复计算
        }

        Vector3f emit_pos;
        const Triangle *emit_triangle = nullptr;
        render_context->world->SampleEmitter(emit_pos, emit_triangle);

        if (emit_triangle != nullptr)
        {
            const Vector3f dir_to_emitter((emit_pos - surface_point.GetPosition()).Normalized());

            const Triangle *test_triangle = nullptr;
            Vector3f test_triangle_hit;
            test_triangle = render_context->bvh->Intersect(Ray(surface_point.GetPosition(), dir_to_emitter), test_triangle_hit, tri);
            
            if ((test_triangle == nullptr) | (test_triangle == emit_triangle))
            {

                float lightpdf;
                Vector3f emission_in = SurfacePoint(emit_triangle, emit_pos).GetEmission(surface_point.GetPosition(), -dir_to_emitter, true, lightpdf);

                /*
                -original_ray_dir     dir_to_emitter
                            ^         ^
                             \normal /
                              \  |  /
                               \ | /
                                \|/
                          -------*-------
                          surface_point
                */
                float bsdfpdf;
                auto f = surface_point.GetMaterial()->DisneyEval(state, -original_ray_dir, normal, dir_to_emitter, bsdfpdf);

                float weight = 1.0f;
                weight = PowerHeuristic(lightpdf, bsdfpdf);

                if (bsdfpdf > 0.0f)
                {
                    ret += weight * f * emission_in * static_cast<float>(render_context->world->CountEmitters()) /** std::abs(in_dot)*/ / lightpdf;
                }
            }
        }

        return ret;
    }

    AlbedoRenderer::AlbedoRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
    }

    void AlbedoRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);
        float top, right;
        PrepareScreenSpace(cam, top, right);

        float div_resolution_width = 1.0f / float(render_context->format_settings.resolution.width);
        float div_resolution_height = 1.0f / float(render_context->format_settings.resolution.height);

#pragma omp parallel for
        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) * div_resolution_width, float(y) * div_resolution_height};
                Vector2f screen_coord = {2.0f * right * NDC_coord.x - right, 2.0f * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));
                cast_ray = cam->O2WTransform(cast_ray);

                float t, u, v;
                auto intersected = render_context->bvh->Intersect(cast_ray, t, u, v, nullptr);

                if (intersected != nullptr)
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = intersected->parent->tex->base_color;
                }
                else
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f::O;
                }
            }
        }
    }
}
