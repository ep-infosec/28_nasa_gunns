/**
@file
@brief     GUNNS Basic Flow Orchestrator implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsBasicLink.o)
   (core/GunnsBasicNode.o))
*/

#include "GunnsBasicFlowOrchestrator.hh"
#include "core/GunnsMacros.hh"
#include "core/GunnsBasicLink.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  numLinks  (--)  The number of links in the network.
/// @param[in]  numNodes  (--)  The number of nodes in the network, including the Ground node.
///
/// @details  Default constructs this GUNNS Basic Flow Orchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowOrchestrator::GunnsBasicFlowOrchestrator(const int& numLinks, const int& numNodes)
    :
    mNumLinks(numLinks),
    mNumNodes(numNodes),
    mLinks   (0),
    mNodes   (0),
    mName    (),
    mInitFlag(false),
    mVerbose (false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Flow Orchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowOrchestrator::~GunnsBasicFlowOrchestrator()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name   (--)  Instance name for messages.
/// @param[in]  links  (--)  Pointer to the network links.
/// @param[in]  nodes  (--)  Pointer to the network nodes.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Basic Flow Orchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicFlowOrchestrator::initialize(const std::string& name, GunnsBasicLink** links,
                                            GunnsBasicNode** nodes)
{
    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Initialize instance name.
    GUNNS_NAME_ERREX("GunnsBasicFlowOrchestrator", name);

    /// - Initialize state.
    mLinks = links;
    mNodes = nodes;

    /// - Validate the initialization.
    validate();

    /// - Set the initialization complete flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Checks initialized states for validity and throws exceptions on faults.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicFlowOrchestrator::validate()
{
    /// - Throw on number of links < 1.
    if (mNumLinks < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "number of links is < 1.");
    }

    /// - Throw on number of nodes < 1.
    if (mNumNodes < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "number of nodes is < 1.");
    }

    /// - Throw on null links array.
    if (!mLinks) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "missing links array.");
    }

    /// - Throw on null nodes array.
    if (!mNodes) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "missing nodes array.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  Calls all links to compute & transport the flows associated with the new network
///           solution, and updates the nodes with their collected flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicFlowOrchestrator::update(const double dt)
{
    /// - Links loop in reverse order from the step loop to support composite links dependencies
    ///   with their child links.
    for (int link = mNumLinks-1; link >= 0; --link) {
        mLinks[link]->computeFlows(dt);
    }

    for (int node = 0; node < mNumNodes-1; ++node) {
        mNodes[node]->integrateFlows(dt);
    }
}
