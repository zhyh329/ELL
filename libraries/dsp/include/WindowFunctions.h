////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     WindowFunctions.h (dsp)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstring> // for size_t
#include <vector>

namespace ell
{
namespace dsp
{
    /// <summary> Used to indicate if the requested window is symmetric (good for filter design) or periodic (good for spectral analysis). </summary>
    enum class WindowSymmetry
    {
        symmetric,
        periodic
    };

    /// <summary> Get the values of a Hamming window of a given length. </summary>
    ///
    /// <param name="size"> The length of the window. </param>
    /// <param name="symmetry"> Determines if the window returned is symmetric (good for filter design) or periodic (good for spectral analysis). </param>
    ///
    /// <returns> A vector containing the values of the Hamming window. </returns>
    template <typename ValueType>
    std::vector<ValueType> HammingWindow(size_t size, WindowSymmetry symmetry = WindowSymmetry::symmetric);

    /// <summary> Get the values of a Hann window of a given length. </summary>
    ///
    /// <param name="size"> The length of the window. </param>
    /// <param name="symmetry"> Determines if the window returned is symmetric (good for filter design) or periodic (good for spectral analysis). </param>
    ///
    /// <returns> A vector containing the values of the Hann window. </returns>
    template <typename ValueType>
    std::vector<ValueType> HannWindow(size_t size, WindowSymmetry symmetry = WindowSymmetry::symmetric);

    /// <summary> Get the values of a generalized cosine window window of a given length. </summary>
    ///
    /// <param name="size"> The length of the window. </param>
    /// <param name="coefficients"> The coefficients of the generalized cosine window. </param>
    /// <param name="symmetry"> Determines if the window returned is symmetric (good for filter design) or periodic (good for spectral analysis). </param>
    ///
    /// <returns> A vector containing the values of the generalized cosine window window. </returns>
    template <typename ValueType>
    std::vector<ValueType> GeneralizedCosineWindow(size_t size, const std::vector<double>& coefficients, WindowSymmetry symmetry = WindowSymmetry::symmetric);
} // namespace dsp
} // namespace ell

#pragma region implementation

#include <math/include/MathConstants.h>

#include <cmath>

namespace ell
{
namespace dsp
{
    // Generalized cosine window
    template <typename ValueType>
    std::vector<ValueType> GeneralizedCosineWindow(size_t size, const std::vector<double>& coefficients, WindowSymmetry symmetry)
    {
        // Formula for generalized cosine window:
        //
        //        N-1
        // Y[i] = sum( (-1 ^ k) * a_k * cos(2 pi k n / D) )
        //        n=0
        //
        // where D == N-1 for the symmetric case, and D == N for the periodic case
        //
        // https://en.wikipedia.org/wiki/Window_function#Cosine-sum_windows

        const auto pi = math::Constants<double>::pi;
        const auto numCoeffs = coefficients.size();
        auto denom = static_cast<ValueType>(symmetry == WindowSymmetry::symmetric ? size - 1 : size);
        std::vector<ValueType> result(size);
        for (size_t index = 0; index < size; index++)
        {
            double windowVal = coefficients[0];
            double sign = -1.0;
            for (size_t coeffIndex = 1; coeffIndex < numCoeffs; ++coeffIndex)
            {
                windowVal += sign * coefficients[coeffIndex] * std::cos((2 * pi * coeffIndex * index) / denom);
                sign *= -1;
            }
            result[index] = static_cast<ValueType>(windowVal);
        }
        return result;
    }

    // Hamming window
    template <typename ValueType>
    std::vector<ValueType> HammingWindow(size_t size, WindowSymmetry symmetry)
    {
        // The original alpha and beta values for the Hamming window are alpha = 0.54, beta = 1 - alpha = 0.46
        // These values are an approximation to alpha = 25/46, beta = 1 - alpha = 21/46 (which cancels the first sidelobe), but have better equiripple properties
        // However, ideal equiripple coefficients are: alpha = 0.53836, beta = 1 - alpha = 0.46164
        // For compatibility with existing DSP libraries, we use the values 0.54 and 0.46
        // https://en.wikipedia.org/wiki/Window_function#Hamming_window

        // const double alpha = 0.53836;
        const double alpha = 0.54;
        const double beta = 1.0 - alpha;
        return GeneralizedCosineWindow<ValueType>(size, { alpha, beta }, symmetry);
    }

    // Hann window
    template <typename ValueType>
    std::vector<ValueType> HannWindow(size_t size, WindowSymmetry symmetry)
    {
        // https://en.wikipedia.org/wiki/Window_function#Hann_window
        return GeneralizedCosineWindow<ValueType>(size, { 0.5, 0.5 }, symmetry);
    }
} // namespace dsp
} // namespace ell

#pragma endregion implementation
