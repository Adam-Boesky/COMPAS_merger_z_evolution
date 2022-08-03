#include "ONeWD.h"
#include "NS.h"

/*
 * Resolve Accretion-Induced Collapse of a WD
 *
 * Hurley et al. 2000, just after eq 43 (before eq 44)
 *
 *
 * STELLAR_TYPE ONeWD::ResolveAIC() 
 *
 * @return                                      Stellar type of remnant (always STELLAR_TYPE::NEUTRON_STAR)
 */

STELLAR_TYPE ONeWD::ResolveAIC() { 

    if (IsSupernova()) {                                                                            // has gone supernova
        std::cout << "got here" << std::endl; // RTW

        m_Mass       = MECS_REM;                                                            // defined in constants.h
        m_Radius     = NS::CalculateRadiusOnPhase_Static(m_Mass);                           // neutronStarRadius in Rsol
        m_Luminosity = NS::CalculateLuminosityOnPhase_Static(m_Mass, m_Age);
        
        m_SupernovaDetails.drawnKickMagnitude = 0.0;
        m_SupernovaDetails.kickMagnitude      = 0.0;

        SetSNCurrentEvent(SN_EVENT::AIC);                                                  // AIC happening now
        SetSNPastEvent(SN_EVENT::AIC);                                                     // ... and will be a past event
        

        std::cout << "current SN is AIC: " <<
        SN_EVENT_LABEL.at(m_SupernovaDetails.events.current )
        << std::endl;

        return STELLAR_TYPE::NEUTRON_STAR;
    } else {
        return m_StellarType;
    }

}
