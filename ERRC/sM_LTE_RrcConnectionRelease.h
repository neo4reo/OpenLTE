//* ***************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo v0.1
// ***************************************************************************/

#ifndef _SM_LTE_RRCCONNECTIONRELEASE_H_
#define _SM_LTE_RRCCONNECTIONRELEASE_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_RrcConnectionRelease ***********************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionRelease, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionRelease;
  int         ReleaseCause;
  int         RedirCarrierInfo_eutra;

  int CellNumber;

  public:

  sM_LTE_RrcConnectionRelease (
    std::string& _xmlRRCConnectionRelease = std::string("Lte_RrcConnectionRelease.xml"), //default 
    int          _ReleaseCause = -1,
    int          _RedirCarrierInfo_eutra = -1
  )
  {
    xmlRRCConnectionRelease = _xmlRRCConnectionRelease;
    ReleaseCause            = _ReleaseCause;
    RedirCarrierInfo_eutra  = _RedirCarrierInfo_eutra;

  }

  // Destructor

  ~sM_LTE_RrcConnectionRelease()
  {
  }

  void start(void);

};

#endif // _SM_LTE_RRCCONNECTIONRELEASE_H_
/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/