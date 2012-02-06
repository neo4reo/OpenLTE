// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_RRCCONNECTIONRECONFIGURATION_H_
#define _SM_LTE_RRCCONNECTIONRECONFIGURATION_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_RrcConnectionReconfiguration ***************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionReconfiguration, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionReconfiguration;
  std::string xmlNASAttachAccept;
  std::string xmlNASActivateDefaultEpsBearerContextRequest;
  std::string xmlNASActivateDedicatedEpsBearerContextRequest;
  std::string xmlRRCConnectionReconfigurationComplete;

  int  UEid;
  int  retcode;
  bool DataGenerator;
  bool DedicatedContext;
  bool AdditionalDefaultContext;

  public:

  sM_LTE_RrcConnectionReconfiguration (
    std::string& _xmlRRCConnectionReconfiguration,
    std::string& _xmlNASAttachAccept,
    std::string& _xmlNASActivateDefaultEpsBearerContextRequest,
    std::string& _xmlRRCConnectionReconfigurationComplete,
    bool         _DataGenerator
  )
  {
    xmlRRCConnectionReconfiguration              = _xmlRRCConnectionReconfiguration;
    xmlNASAttachAccept                           = _xmlNASAttachAccept;
    xmlNASActivateDefaultEpsBearerContextRequest = _xmlNASActivateDefaultEpsBearerContextRequest;
    xmlRRCConnectionReconfigurationComplete      = _xmlRRCConnectionReconfigurationComplete;
    DataGenerator                                = _DataGenerator;
    UEid                                         = 0;
    DedicatedContext                             = false;
  }

  sM_LTE_RrcConnectionReconfiguration (
    std::string& _xmlRRCConnectionReconfiguration,
    std::string& _xmlNASActivateDedicatedEpsBearerContextRequest,
    std::string& _xmlRRCConnectionReconfigurationComplete,
    bool         _DataGenerator,
    bool         _Additional = false
  )
  {
    xmlRRCConnectionReconfiguration                = _xmlRRCConnectionReconfiguration;
    xmlNASActivateDedicatedEpsBearerContextRequest = _xmlNASActivateDedicatedEpsBearerContextRequest;
    xmlRRCConnectionReconfigurationComplete        = _xmlRRCConnectionReconfigurationComplete;
    DataGenerator                                  = _DataGenerator;
    UEid                                           = 0;
    DedicatedContext                               = !_Additional;
    AdditionalDefaultContext                       =  _Additional;
  }

  // Destructor

  ~sM_LTE_RrcConnectionReconfiguration()
  {
  }

  void start(void);

};

#endif // _SM_LTE_RRCCONNECTIONRECONFIGURATION_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/