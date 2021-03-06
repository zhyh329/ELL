////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     MatrixOperations.h (value)
//  Authors:  Kern Handa
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MatrixOperations.h"
#include "EmitterContext.h"
#include "Matrix.h"
#include "Scalar.h"

namespace ell
{
using namespace utilities;

namespace value
{
    Scalar Sum(Matrix matrix)
    {
        Scalar result = Allocate(matrix.Type(), ScalarLayout);

        For(matrix, [&](auto row, auto column) {
            result += matrix(row, column);
        });

        return result;
    }

    void For(Matrix matrix, std::function<void(Scalar, Scalar)> fn)
    {
        auto layout = matrix.GetValue().GetLayout();
        if (layout.NumDimensions() != 2)
        {
            throw InputException(InputExceptionErrors::invalidArgument,
                                 "Layout being looped over must be two-dimensional");
        }

        GetContext().For(layout, [fn = std::move(fn)](std::vector<Scalar> coordinates) {
            fn(coordinates[0], coordinates[1]);
        });
    }

    Matrix GEMM(Matrix m1, Matrix m2) { throw LogicException(LogicExceptionErrors::notImplemented); }

    Vector GEMV(Matrix m, Vector v) { throw LogicException(LogicExceptionErrors::notImplemented); }

    Matrix operator+(Matrix m1, Matrix m2)
    {
        Matrix copy = m1.Copy();
        return copy += m2;
    }

    Matrix operator+(Matrix m, Scalar s)
    {
        Matrix copy = m.Copy();
        return copy += s;
    }

    Matrix operator-(Matrix m1, Matrix m2)
    {
        Matrix copy = m1.Copy();
        return copy -= m2;
    }
    Matrix operator-(Matrix m, Scalar s)
    {
        Matrix copy = m.Copy();
        return copy -= s;
    }

    Matrix operator*(Matrix m, Scalar s)
    {
        Matrix copy = m.Copy();
        return copy *= s;
    }

    Matrix operator/(Matrix m, Scalar s)
    {
        Matrix copy = m.Copy();
        return copy /= s;
    }

} // namespace value
} // namespace ell
