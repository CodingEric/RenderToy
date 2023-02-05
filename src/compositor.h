#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "renderer.h"
#include "rtmath.h"

namespace RenderToy
{
    /// @brief Image class. Provides a variety of operations on images.
    class Image
    {
        enum class EdgeMode;

        /// @brief Get pixel info at point p.
        /// @tparam _EM Edge Mode.
        /// @param p Point p.
        /// @return RGB Color.
        template <EdgeMode _EM>
        inline const Vector3f &BufferGetWrapper(PointN p) const
        {
            if constexpr (_EM == EdgeMode::kReflect)
            {
                if (p.x < 0)
                    p.x = -p.x;
                if (p.y < 0)
                    p.y = -p.y;
                if (p.y >= resolution.width)
                    p.y = 2 * resolution.width - p.y;
                if (p.x >= resolution.height)
                    p.x = 2 * resolution.height - p.x;
                return buffer[p.x * resolution.width + p.y];
            }
            else if constexpr (_EM == EdgeMode::kBlackPadding)
            {
                if (p.x < 0)
                    return Vector3f::O;
                if (p.y < 0)
                    return Vector3f::O;
                if (p.y >= resolution.width)
                    return Vector3f::O;
                if (p.x >= resolution.height)
                    return Vector3f::O;
            }
            else if constexpr (_EM == EdgeMode::kWhitePadding)
            {
                if (p.x < 0)
                    return Vector3f::White;
                if (p.y < 0)
                    return Vector3f::White;
                if (p.y >= resolution.width)
                    return Vector3f::White;
                if (p.x >= resolution.height)
                    return Vector3f::White;
            }
            static_assert("No viable wrapper type specified.");
        }

    public:
        /// @brief Decides which mode should be picked when image processing meets edges.
        enum class EdgeMode
        {
            kReflect = 0,
            kBlackPadding = 1,
            kWhitePadding = 2
        };

        /// @brief Sample mode for sampling and resizing.
        enum class SampleMode
        {
            kNearestNeighbor = 0,
            kBilinearInterpolation = 1
        };

        const SizeN resolution;
        Vector3f *buffer;

        Image(const SizeN &resolution_);
        Image(const RenderContext *const render_context_);
        Image(const Image &image);

        /// @brief Run convolution on the image.
        /// @tparam _Normalize If set true, all pixels will be normalized after convolution.
        /// @tparam _Length If set true, all pixels will be converted to single brightness signal by its original length after convolution.
        /// @tparam _Abs If set true, the components of every pixel color will be set positive.
        /// @param k Convolution kernel.
        /// @return Reference to self.
        template <bool _Normalize = false, bool _Length = false, bool _Abs = false>
        Image &Convolute(const IConvolutionKernel<float> &k)
        {
            Image tmp(*this);
            size_t row_2 = k.kernel_mat.row >> 1;
            size_t column_2 = k.kernel_mat.column >> 1;
#pragma omp parallel
            for (int i = 0; i < resolution.height; ++i)
            {
                for (int j = 0; j < resolution.width; ++j)
                {
                    // ret[i][j]
                    buffer[i * resolution.width + j] = Vector3f::O;
                    for (int ii = 0; ii < k.kernel_mat.row; ++ii)
                    {
                        for (int jj = 0; jj < k.kernel_mat.column; ++jj)
                        {
                            buffer[i * resolution.width + j] += tmp.BufferGetWrapper<EdgeMode::kReflect>(PointN(i - row_2 + ii, j - column_2 + jj)) * k.kernel_mat[ii][jj];
                        }
                    }
                    if constexpr (_Abs == true)
                    {
                        buffer[i * resolution.width + j] = Vector3f::Abs(buffer[i * resolution.width + j]);
                    }
                    if constexpr (_Length == true)
                    {
                        buffer[i * resolution.width + j] = buffer[i * resolution.width + j].Length();
                    }
                    if constexpr (_Normalize == true)
                    {
                        buffer[i * resolution.width + j].Normalize();
                    }
                }
            }
            return (*this);
        }

        /// @brief Run Gaussian Blur.
        /// @param size Gaussian kernel size.
        /// @param sigma Gaussian kernel sigma.
        /// @return Reference to self.
        Image &GaussianBlur(const std::size_t size, const float sigma);

        /// @brief Run Edge Detection.
        /// @tparam _O Orientation of differential operator.
        /// @return Reference to self.
        template <Orientation _O>
        Image &EdgeDetection()
        {
            if constexpr (_O != Orientation::All)
            {
                SobelKernel<float, _O> sk;
                Convolute(sk);
                return (*this);
            }
            else
            {
                Image cpy(*this);
                cpy.EdgeDetection<Orientation::X>();
                EdgeDetection<Orientation::Y>();
                for (int i = 0; i < resolution.height; ++i)
                {
                    for (int j = 0; j < resolution.width; ++j)
                    {
                        buffer[i * resolution.width + j] += cpy.buffer[i * resolution.width + j];
                        buffer[i * resolution.width + j] = buffer[i * resolution.width + j].Length();
                    }
                }
            }

            return (*this);
        }

        /// @brief Run Gray Scale.
        /// @tparam _CS Color standards. It decides which Luma function will be used.
        /// @return Reference to self.
        template <Convert::ColorStandard _CS>
        Image &GreyScale()
        {
            for (int i = 0; i < resolution.height; ++i)
            {
                for (int j = 0; j < resolution.width; ++j)
                {
                    buffer[i * resolution.width + j] = Convert::Luma<_CS>(buffer[i * resolution.width + j]);
                }
            }
            return (*this);
        }

        inline const Image operator+(const Image &img)
        {
            // TODO: exception.
            Image ret(resolution);
            for (int i = 0; i < resolution.Area(); ++i)
            {
                ret.buffer[i] = buffer[i] + img.buffer[i];
            }
            return ret;
        }

        inline const Image operator-(const Image &img)
        {
            // TODO: exception.
            Image ret(resolution);
            for (int i = 0; i < resolution.Area(); ++i)
            {
                ret.buffer[i] = buffer[i] - img.buffer[i];
            }
            return ret;
        }

        inline const Image operator*(const Image &img)
        {
            // TODO: exception.
            Image ret(resolution);
            for (int i = 0; i < resolution.Area(); ++i)
            {
                ret.buffer[i] = buffer[i] * img.buffer[i];
            }
            return ret;
        }

        inline const Image operator/(const Image &img)
        {
            // TODO: exception.
            Image ret(resolution);
            for (int i = 0; i < resolution.Area(); ++i)
            {
                ret.buffer[i] = buffer[i] / img.buffer[i];
            }
            return ret;
        }

        inline const void operator=(const Image &img)
        {
            // resolution = img.resolution;
            // TODO: exception
            for (int i = 0; i < resolution.Area(); ++i)
            {
                buffer[i] = img.buffer[i];
            }
        }

        /// @brief Extract pixels that satisfies pixel_filter.
        /// @param pixel_filter
        /// @param default_color
        /// @return
        [[nodiscard]] const Image Extract(const std::function<bool(const Vector3f &)> &pixel_filter, const Vector3f &default_color = Vector3f::O);

        /// @brief Generates bloom on pixels that satisfy brightness threshold.
        /// @param size
        /// @param sigma
        /// @param threshold
        /// @return
        Image &Bloom(const std::size_t size, const float sigma, const float threshold);

        /// @brief Sample image at given coordinate.
        /// @tparam _SM Sampling mode.
        /// @param coord Coordinate.
        /// @return Color.
        template <SampleMode _SM = SampleMode::kNearestNeighbor>
        const Vector3f Sample(const Vector2f &coord) const
        {
            if constexpr (_SM == SampleMode::kNearestNeighbor)
            {
                return BufferGetWrapper<EdgeMode::kBlackPadding>(PointN(std::round(coord.x), std::round(coord.y)));
            }
            else if constexpr (_SM == SampleMode::kBilinearInterpolation)
            {
                auto lbx = int(coord.x);
                auto lby = int(coord.y);
                auto ltc = BufferGetWrapper<EdgeMode::kBlackPadding>({lbx, lby});
                auto rtc = BufferGetWrapper<EdgeMode::kBlackPadding>({lbx + 1, lby});
                auto lbc = BufferGetWrapper<EdgeMode::kBlackPadding>({lbx, lby + 1});
                auto rbc = BufferGetWrapper<EdgeMode::kBlackPadding>({lbx + 1, lby + 1});
                auto tc = Lerp(ltc, rtc, coord.x - floor(coord.x));
                auto bc = Lerp(lbc, rbc, coord.x - floor(coord.x));
                return Lerp(tc, bc, coord.y - floor(coord.y));
            }
        }

        template <SampleMode _SM = SampleMode::kNearestNeighbor>
        const Image Resize(const SizeN &new_size) const
        {

        }

        ~Image();
    };
}

#endif // COMPOSITOR_H