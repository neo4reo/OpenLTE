// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_RRCCONNECTIONREESTABLISHMENT_H_
#define _SM_LTE_RRCCONNECTIONREESTABLISHMENT_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_RrcConnectionReestablishment ***************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionReestablishment, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionReestablishmentRequest;
  std::string xmlRRCConnectionReestablishment;
  std::string xmlRRCConnectionReestablishmentComplete;
  int         CellHandleTarget;
  int         CellHandleSource;
  int         retcode;
  int         WaitTimer;
  int         UeId;
  int         Rnti;

  _CrrcAsp* pCrrcAsp;
  DECL_LTE_RRC_PDU( rrcConnectionReestablishment, DL_CCCH, RRCConnectionReestablishment );

  public:

  sM_LTE_RrcConnectionReestablishment (
    int          _CellHandleTarget,
    std::string& _xmlRRCConnectionReestablishmentRequest,
    std::string& _xmlRRCConnectionReestablishment,
    std::string& _xmlRRCConnectionReestablishmentComplete,
    int          _CellHandleSource,
    int          _WaitTimer = WAIT_RRC_REQ
  )
  {
    CellHandleTarget                        = _CellHandleTarget;
    xmlRRCConnectionReestablishmentRequest  = _xmlRRCConnectionReestablishmentRequest;
    xmlRRCConnectionReestablishment         = _xmlRRCConnectionReestablishment;
    xmlRRCConnectionReestablishmentComplete = _xmlRRCConnectionReestablishmentComplete;
    WaitTimer                               = _WaitTimer;
    CellHandleSource                        = _CellHandleSource;
    retcode                                 = EV_OK;
  }

  // Destructor

  ~sM_LTE_RrcConnectionReestablishment()
  {
  }

  void start(void);
};

#endif // _SM_LTE_RRCCONNECTIONREESTABLISHMENT_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/