// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_RRCCONNECTIONRECONFIGURATION_HO_H_
#define _SM_LTE_RRCCONNECTIONRECONFIGURATION_HO_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_RrcConnectionReconfiguration_ho **********************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionReconfiguration_ho, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  int         cellHandleTarget;
  std::string xmlRRCConnectionReconfiguration;
  const char* newUE_Identity;
  int         dl_CarrierFreq;
  int         ul_CarrierFreq;
  int         dl_Bandwidth;
  int         ul_Bandwidth;
  std::string xmlRRCConnectionReconfigurationComplete;

  public:

  sM_LTE_RrcConnectionReconfiguration_ho (
    int          _cellHandleTarget,
    std::string& _xmlRRCConnectionReconfiguration,
    const char*  _newUE_Identity,
    int          _dl_CarrierFreq,
    int          _ul_CarrierFreq,
    int          _dl_Bandwidth,
    int          _ul_Bandwidth,
    std::string& _xmlRRCConnectionReconfigurationComplete
  )
  {
    cellHandleTarget                        = _cellHandleTarget;
    xmlRRCConnectionReconfiguration         = _xmlRRCConnectionReconfiguration;
    newUE_Identity                          = _newUE_Identity;
    dl_CarrierFreq                          = _dl_CarrierFreq;
    ul_CarrierFreq                          = _ul_CarrierFreq;
    dl_Bandwidth                            = _dl_Bandwidth;
    ul_Bandwidth                            = _ul_Bandwidth;
    xmlRRCConnectionReconfigurationComplete = _xmlRRCConnectionReconfigurationComplete;
  }

  // Destructor

  ~sM_LTE_RrcConnectionReconfiguration_ho()
  {
  }

  void start(void);

};

#endif // _SM_LTE_RRCCONNECTIONRECONFIGURATION_HO_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/