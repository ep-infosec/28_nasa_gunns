/**
@file
@brief    GUNNS Dynamics Euler Angles 312-Sequence implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (aspects/dynamics/GunnsDynEulerBase.o)
     (aspects/dynamics/GunnsDynUtils.o)
     (math/UnitConversion.o)
    )
*/

#include "GunnsDynEuler312.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"
#include "math/UnitConversion.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Dynamics Euler Angles 312-Sequence.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynEuler312::GunnsDynEuler312()
    :
    GunnsDynEulerBase(312)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Dynamics Euler Angles 312-Sequence.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynEuler312::~GunnsDynEuler312()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] quat (--) Pointer to the quaternion to be set.
/// @param[in]  s1   (--) Sine of the X-axis half-rotation.
/// @param[in]  c1   (--) Coine of the X-axis half-rotation.
/// @param[in]  s2   (--) Sine of the Y-axis half-rotation.
/// @param[in]  c2   (--) Coine of the Y-axis half-rotation.
/// @param[in]  s3   (--) Sine of the Z-axis half-rotation.
/// @param[in]  c3   (--) Coine of the Z-axis half-rotation.
///
/// @details  Computes the quaternion elements for this sequence given the sines and cosines of the
///           half-angle rotations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynEuler312::computeQuatSeq(double* quat,
                                      const double s1, const double c1,
                                      const double s2, const double c2,
                                      const double s3, const double c3)
{
    quat[0] =  c1*c2*c3 - s1*s2*s3;
    quat[1] = -c1*s2*c3 + s1*c2*s3;
    quat[2] = -c1*c2*s3 - s1*s2*c3;
    quat[3] = -c1*s2*s3 - s1*c2*c3;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] quat (--) The quaternion to compute the rotation from.
///
/// @returns  double (--) Sin of the 2nd rotation.
///
/// @details  Computes and returns the sine of the 2nd rotation.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynEuler312::computeRot2Sin(const double* quat)
{
    return GunnsDynUtils::QtoMElement[5](quat);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] quat    (--) The quaternion to compute the angles from from.
/// @param[in] rot2sin (--) The sine of the 2nd rotation.
///
/// @details  Computes Euler angles for this sequence, assuming we are not near a singularity
///           condition.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynEuler312::computeAnglesNoSing(const double* quat, const double rot2sin)
{
    const double mat02 = GunnsDynUtils::QtoMElement[2](quat);
    const double mat10 = GunnsDynUtils::QtoMElement[3](quat);
    const double mat11 = GunnsDynUtils::QtoMElement[4](quat);
    const double mat22 = GunnsDynUtils::QtoMElement[8](quat);
    mAngles[0] = atan2(-mat10, mat11);
    mAngles[1] = asin(rot2sin);
    mAngles[2] = atan2(-mat02, mat22);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] quat    (--) The quaternion to compute the angles from from.
///
/// @details  Computes Euler angles for this sequence, assuming we are near the +pi/2 singularity
///           condition.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynEuler312::computeAnglesPosSing(const double* quat)
{
    const double mat00 = GunnsDynUtils::QtoMElement[0](quat);
    const double mat01 = GunnsDynUtils::QtoMElement[1](quat);
    mAngles[0] = atan2(mat01, mat00);
    mAngles[1] = UnitConversion::PI_OVER_2;
    mAngles[2] = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] quat    (--) The quaternion to compute the angles from from.
///
/// @details  Computes Euler angles for this sequence, assuming we are near the -pi/2 singularity
///           condition.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynEuler312::computeAnglesNegSing(const double* quat)
{
    const double mat00 = GunnsDynUtils::QtoMElement[0](quat);
    const double mat01 = GunnsDynUtils::QtoMElement[1](quat);
    mAngles[0] = atan2(mat01, mat00);
    mAngles[1] = -UnitConversion::PI_OVER_2;
    mAngles[2] = 0.0;
}
