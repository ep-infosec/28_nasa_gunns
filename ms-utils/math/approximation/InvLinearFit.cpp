/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"

#include "InvLinearFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Inverse Linear fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
InvLinearFit::InvLinearFit()
    :
    TsApproximation(),
    mA(0.0),
    mB(0.0),
    mC(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @details  Constructs this Inverse Linear fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
InvLinearFit::InvLinearFit(const double a,    const double b,    const double c,
                           const double minX, const double maxX, const std::string &name)
    :
    TsApproximation(minX, maxX, -FLT_EPSILON, +FLT_EPSILON),
    mA(a),
    mB(b),
    mC(c)
{
    init(a, b, c, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Inverse Linear curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
InvLinearFit::~InvLinearFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
///
/// @details  Initializes this Inverse Linear fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void InvLinearFit::init(const double a,    const double b,    const double c,
        const double minX, const double maxX, const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Throw a TsInitializationException exception on a singularity (divide by 0) in the allegedly valid range.
    TS_GENERIC_IF_ERREX((mMinX < FLT_EPSILON && -FLT_EPSILON < mMaxX), TsInitializationException,
                        "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");

    /// - Initialize the coefficients with validated values.
    mA        = a;
    mB        = b;
    mC        = c;

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
