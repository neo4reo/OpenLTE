// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_TAUPROCEDURE_H_
#define _SM_LTE_TAUPROCEDURE_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_TauProcedure *******************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_TauProcedure, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionRequest;
  std::string xmlRRCConnectionSetup;
  std::string xmlRRCConnectionSetupComplete;
  std::string xmlTrackingAreaUpdateRequest;
  std::string xmlTrackingAreaUpdateAccept;
  std::string xmlTrackingAreaUpdateComplete;
  std::string xmlRRCConnectionRelease;

  int         UEid;
  int         CellHandle;
  int         retcode;

  public:

  sM_LTE_TauProcedure (
    int          _CellHandle,
    std::string& _xmlRRCConnectionRequest,
    std::string& _xmlRRCConnectionSetup,
    std::string& _xmlRRCConnectionSetupComplete,
    std::string& _xmlTrackingAreaUpdateRequest,
    std::string& _xmlTrackingAreaUpdateAccept,
    std::string& _xmlTrackingAreaUpdateComplete,
    std::string& _xmlRRCConnectionRelease
  )
  {
    CellHandle                    = _CellHandle;
    xmlRRCConnectionRequest       = _xmlRRCConnectionRequest;
    xmlRRCConnectionSetup         = _xmlRRCConnectionSetup;
    xmlRRCConnectionSetupComplete = _xmlRRCConnectionSetupComplete;
    xmlTrackingAreaUpdateRequest  = _xmlTrackingAreaUpdateRequest;
    xmlTrackingAreaUpdateAccept   = _xmlTrackingAreaUpdateAccept;
    xmlTrackingAreaUpdateComplete = _xmlTrackingAreaUpdateComplete;
    xmlRRCConnectionRelease       = _xmlRRCConnectionRelease;
    UEid                          = 0;
  }

  // Destructor

  ~sM_LTE_TauProcedure()
  {
  }

  void start(void);

};

#endif // _SM_LTE_TAUPROCEDURE_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/