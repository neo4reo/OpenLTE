// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_RRCCONNECTIONESTABLISHMENT_H_
#define _SM_LTE_RRCCONNECTIONESTABLISHMENT_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_RrcConnectionEstablishment *****************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionEstablishment, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionRequest;
  std::string xmlRRCConnectionSetup;
  std::string xmlRRCConnectionSetupComplete;
  int         CellHandle;
  int         retcode;
  int         WaitTimer;
  int         UeId;
  int         Rnti;

  _CrrcAsp* pCrrcAsp;
  DECL_LTE_RRC_PDU( rrcConnectionSetup, DL_CCCH, RRCConnectionSetup );

  public:

  sM_LTE_RrcConnectionEstablishment (
    int          _CellHandle,
    std::string& _xmlRRCConnectionRequest,
    std::string& _xmlRRCConnectionSetup,
    std::string& _xmlRRCConnectionSetupComplete,
    int          _WaitTimer = WAIT_RRC_REQ
  )
  {
    CellHandle                    = _CellHandle;
    xmlRRCConnectionRequest       = _xmlRRCConnectionRequest;
    xmlRRCConnectionSetup         = _xmlRRCConnectionSetup;
    xmlRRCConnectionSetupComplete = _xmlRRCConnectionSetupComplete;
    WaitTimer                     = _WaitTimer;
    retcode                       = EV_OK;
  }

  // Destructor

  ~sM_LTE_RrcConnectionEstablishment()
  {
  }

  void start(void);
};

/* sM_LTE_RrcConnectionReject *****************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionReject, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  int         CellHandle;
  std::string xmlRRCConnectionRequest;
  int         WaitTimer;
  std::string xmlRRCConnectionReject;
  int         WaitTime;
  int         retcode;
  int         Rnti;

  _CrrcAsp* pCrrcAsp;
  DECL_LTE_RRC_PDU( rrcConnectionReject, DL_CCCH, RRCConnectionReject );

  public:

  sM_LTE_RrcConnectionReject (
    int          _CellHandle,
    std::string& _xmlRRCConnectionRequest,
    int          _WaitTimer,
    std::string& _xmlRRCConnectionReject,
    int          _WaitTime
  )
  {
    CellHandle              = _CellHandle;
    xmlRRCConnectionRequest = _xmlRRCConnectionRequest;
    WaitTimer               = _WaitTimer;
    xmlRRCConnectionReject  = _xmlRRCConnectionReject;
    WaitTime                = _WaitTime;
    retcode                 = EV_OK;
  }

  // Destructor

  ~sM_LTE_RrcConnectionReject()
  {
  }

  void start(void);
};

#endif // _SM_LTE_RRCCONNECTIONESTABLISHMENT_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/