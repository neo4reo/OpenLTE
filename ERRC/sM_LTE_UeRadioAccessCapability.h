// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_UERADIOACCESSCAPABILITY_H_
#define _SM_LTE_UERADIOACCESSCAPABILITY_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"
#include "bitset"

#define SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS ( 64 )

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_UeRadioAccessCapability ********************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_UeRadioAccessCapability, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlUECapabilityEnquiry;
  std::string xmlUECapabilityInformation;

  bitset<SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS> bitstring_ebands_ue;
  bitset<SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS> bitstring_ebands;

  int retcode;

  void EUTRADecode( Lte::Std::_UE_CapabilityRAT_Container::__f372_ * ue_cap_cont );

  bool mInterFreqNeedForGaps[SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS]
    [SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS];

  public:

  bool getInterFreqNeedForGaps( int i, int j );

  sM_LTE_UeRadioAccessCapability (
    std::string& _xmlUECapabilityEnquiry,
    std::string& _xmlUECapabilityInformation
  )
  {
    xmlUECapabilityEnquiry     = _xmlUECapabilityEnquiry;
    xmlUECapabilityInformation = _xmlUECapabilityInformation;
    retcode                    = EV_OK;
  }

  // Destructor

  ~sM_LTE_UeRadioAccessCapability()
  {
  }

  void start(void);

};

#endif // _SM_LTE_UERADIOACCESSCAPABILITY_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/