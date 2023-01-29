#include "surfacepoint.h"
#include "meshobj.h"

#include <cmath>

namespace RenderToy
{
    SurfacePoint::SurfacePoint(const Triangle *triangle_, const Vector3f &position_)
        : triangle(triangle_), position(position_)
    {
    }

    const Vector3f SurfacePoint::GetEmission(const Vector3f &to_pos, const Vector3f &out_dir, const bool is_solid_angle, float &pdf) const
    {
        /*             current point
        emitter    ----*----
                        \
                         \
                          \ out_dir
                           \
                            v
                         ----*----     receiver
                              to_pos
        */
        const Vector3f ray(to_pos - position);
        const float distance2 = ray.Dot(ray);
        const float cos_area = out_dir.Dot(triangle->NormalC()) * triangle->AreaC();
        const float solid_angle = is_solid_angle ? cos_area / (distance2 >= EPS ? distance2 : EPS) : 1.0f / (distance2 >= EPS ? distance2 : EPS);
        pdf = std::abs(1.0f / solid_angle); // TODO: Optimize. abs for culling.
        return triangle->parent->tex->emission;
    }

    bool SurfacePoint::GetNextDirection(const Vector3f &in_dir, Vector3f &out_dir, Vector3f &color_o, float &pdf, RayState &state)
    {
        pdf = 0.0f;
        Vector3f f = Vector3f(0.0f);
        auto N = GetNormal();
        if (Vector3f::Dot(in_dir, N) < 0.0f)
        {
            N = -N;
        }

        auto V = in_dir;
        auto mat = GetMaterial();

        float r1 = Random::Float();
        float r2 = Random::Float();

        Vector3f T, B;
        Onb(N, T, B);
        V = ToLocal(T, B, N, V); // NDotL = L.z; NDotV = V.z; NDotH = H.z

        // Specular and sheen color
        Vector3f specCol, sheenCol;

        float eta = state.eta;
        mat->GetSpecColor(eta, specCol, sheenCol);

        // Lobe weights
        float diffuseWt, specReflectWt, specRefractWt, clearcoatWt;
        // TODO: Recheck fresnel. Not sure if correct. VDotN produces fireflies with rough dielectric.
        // VDotH matches Mitsuba and gets rid of all fireflies but H isn't available at this stage
        float approxFresnel = mat->FresnelMix(eta, V.z);
        mat->GetLobeProbabilities(eta, specCol, approxFresnel, diffuseWt, specReflectWt, specRefractWt, clearcoatWt);

        // CDF for picking a lobe
        float cdf[4];
        cdf[0] = diffuseWt;
        cdf[1] = cdf[0] + specReflectWt;
        cdf[2] = cdf[1] + specRefractWt;
        cdf[3] = cdf[2] + clearcoatWt;

        if (r1 < cdf[0]) // Diffuse Reflection Lobe
        {
            r1 /= cdf[0];
            out_dir = CosineSampleHemisphere(r1, r2);

            Vector3f H = (out_dir + V).Normalized();

            f = mat->EvalDiffuse(sheenCol, V, out_dir, H, pdf);
            pdf *= diffuseWt;
        }
        else if (r1 < cdf[1]) // Specular Reflection Lobe
        {
            r1 = (r1 - cdf[0]) / (cdf[1] - cdf[0]);
            Vector3f H = SampleGGXVNDF(V, mat->roughness, r1, r2);

            if (H.z < 0.0)
                H = -H;

            out_dir = (Reflect(-V, H)).Normalized();

            f = mat->EvalSpecReflection(eta, specCol, V, out_dir, H, pdf);
            pdf *= specReflectWt;
        }
        else if (r1 < cdf[2]) // Specular Refraction Lobe
        {
            r1 = (r1 - cdf[1]) / (cdf[2] - cdf[1]);
            Vector3f H = SampleGGXVNDF(V, mat->roughness, r1, r2);

            if (H.z < 0.0)
                H = -H;

            out_dir = Refract(-V, H, eta).Normalized();

            f = mat->EvalSpecRefraction(eta, V, out_dir, H, pdf);
            pdf *= specRefractWt;
        }
        else // Clearcoat Lobe
        {
            r1 = (r1 - cdf[2]) / (1.0 - cdf[2]);
            Vector3f H = SampleGTR1(mat->clearcoat_roughness, r1, r2);

            if (H.z < 0.0)
                H = -H;

            out_dir = Reflect(-V, H).Normalized();

            f = mat->EvalClearcoat(V, out_dir, H, pdf);
            pdf *= clearcoatWt;
        }

        out_dir = ToWorld(T, B, N, out_dir);
        color_o = f * std::abs(Vector3f::Dot(N, out_dir));

        return !(out_dir == Vector3f::O);
    }

    const Triangle *SurfacePoint::GetHitTriangle()
    {
        return triangle;
    }

    const Triangle *SurfacePoint::GetHitTriangle() const
    {
        return triangle;
    }

    Vector3f &SurfacePoint::GetPosition()
    {
        return position;
    }

    const Vector3f &SurfacePoint::GetPosition() const
    {
        return position;
    }

    const Mesh *SurfacePoint::GetHitMesh() const
    {
        return triangle->parent;
    }

    const PrincipledBSDF *SurfacePoint::GetMaterial() const
    {
        return triangle->parent->tex;
    }

    const Vector3f SurfacePoint::GetNormal() const
    {
        return triangle->NormalC();
    }
}