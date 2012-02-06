// ****************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo v0.3
// ****************************************************************************

#ifndef _SM_LTE_MOCPROCEDURE_H_
#define _SM_LTE_MOCPROCEDURE_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_MocProcedure *******************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_MocProcedure, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionRequest;
  std::string xmlRRCConnectionSetup;
  std::string xmlRRCConnectionSetupComplete;
  std::string xmlServiceRequest;
  std::string xmlSecurityModeCommand;
  std::string xmlSecurityModeComplete;
  std::string xmlRRCConnectionReconfiguration;
  std::string xmlNASActivateDedicatedEpsBearerContextRequest;
  std::string xmlRRCConnectionReconfigurationComplete;
  std::string xmlNASActivateDedicatedEpsBearerContextAccept;

  int  CellNumber;
  int  UEid;
  int  retcode;
  bool DataGenerator;
  bool DedicatedContext;

  public:

  sM_LTE_MocProcedure (
    int          _CellNumber,
    std::string& _xmlRRCConnectionRequest,
    std::string& _xmlRRCConnectionSetup,
    std::string& _xmlRRCConnectionSetupComplete,
    std::string& _xmlServiceRequest,
    std::string& _xmlSecurityModeCommand,
    std::string& _xmlSecurityModeComplete,
    std::string& _xmlRRCConnectionReconfiguration,
    std::string& _xmlRRCConnectionReconfigurationComplete,
    bool         _DataGenerator
  )
  {
    CellNumber                              = _CellNumber;
    xmlRRCConnectionRequest                 = _xmlRRCConnectionRequest;
    xmlRRCConnectionSetup                   = _xmlRRCConnectionSetup;
    xmlRRCConnectionSetupComplete           = _xmlRRCConnectionSetupComplete;
    xmlServiceRequest                       = _xmlServiceRequest;
    xmlSecurityModeCommand                  = _xmlSecurityModeCommand;
    xmlSecurityModeComplete                 = _xmlSecurityModeComplete;
    xmlRRCConnectionReconfiguration         = _xmlRRCConnectionReconfiguration;
    xmlRRCConnectionReconfigurationComplete = _xmlRRCConnectionReconfigurationComplete;
    DataGenerator                           = _DataGenerator;
    DedicatedContext                        = false;
    UEid                                    = 0;
  }

  sM_LTE_MocProcedure (
    int          _CellNumber,
    std::string& _xmlRRCConnectionRequest,
    std::string& _xmlRRCConnectionSetup,
    std::string& _xmlRRCConnectionSetupComplete,
    std::string& _xmlServiceRequest,
    std::string& _xmlSecurityModeCommand,
    std::string& _xmlSecurityModeComplete,
    std::string& _xmlRRCConnectionReconfiguration,
    std::string& _xmlNASActivateDedicatedEpsBearerContextRequest,
    std::string& _xmlRRCConnectionReconfigurationComplete,
    bool         _DataGenerator,
    std::string& _xmlNASActivateDedicatedEpsBearerContextAccept
  )
  {
    CellNumber                                     = _CellNumber;
    xmlRRCConnectionRequest                        = _xmlRRCConnectionRequest;
    xmlRRCConnectionSetup                          = _xmlRRCConnectionSetup;
    xmlRRCConnectionSetupComplete                  = _xmlRRCConnectionSetupComplete;
    xmlServiceRequest                              = _xmlServiceRequest;
    xmlSecurityModeCommand                         = _xmlSecurityModeCommand;
    xmlSecurityModeComplete                        = _xmlSecurityModeComplete;
    xmlRRCConnectionReconfiguration                = _xmlRRCConnectionReconfiguration;
    xmlNASActivateDedicatedEpsBearerContextRequest = _xmlNASActivateDedicatedEpsBearerContextRequest;
    xmlRRCConnectionReconfigurationComplete        = _xmlRRCConnectionReconfigurationComplete;
    xmlNASActivateDedicatedEpsBearerContextAccept  = _xmlNASActivateDedicatedEpsBearerContextAccept;
    DataGenerator                                  = _DataGenerator;
    DedicatedContext                               = true;
    UEid                                           = 0;
  }

  // Destructor

  ~sM_LTE_MocProcedure()
  {
  }

  void start(void);

};

#endif // _SM_LTE_MOCPROCEDURE_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/