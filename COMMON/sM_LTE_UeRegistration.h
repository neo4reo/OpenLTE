// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_UEREGISTRATION_H_
#define _SM_LTE_UEREGISTRATION_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_UeRegistration *****************************************************/

#define RUN_DEFAULT_REGISTRATION_PROCEDURES(cell)  RUN_NEWP_STATEMACHINE( sM_LTE_UeRegistration, null,( \
        std::string(""),                                              \
        std::string("RrcConnectionSetup.xml"),                        \
        std::string(""),                                              \
        std::string(""),                                              \
        std::string(""),                                              \
        std::string("EmmAuthenticationRequest.xml"),                  \
        std::string(""),                                              \
        std::string("EmmSecurityModeCommand.xml"),                    \
        std::string(""),                                              \
        std::string("SecurityModeCommand.xml"),                       \
        std::string(""),                                              \
        std::string("UECapabilityEnquiry.xml"),                       \
        std::string(""),                                              \
        std::string("EsmInformationRequest.xml"),                     \
        std::string(""),                                              \
        std::string("RrcConnectionReconfiguration_SRB2_DRB_1_0.xml"), \
        std::string("EmmAttachAccept.xml"),                           \
        std::string("EsmActivateDefaultEpsBearerContextRequest.xml"), \
        std::string(""),                                              \
        std::string(""),                                              \
        std::string(""),                                              \
        std::string("RrcConnectionRelease.xml"),                      \
        false,                                                        \
        -1,                                                           \
        -1,                                                           \
        cell))

DERIVED_STATEMACHINE_DLL( sM_LTE_UeRegistration, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionRequest;
  std::string xmlRRCConnectionSetup;
  std::string xmlRRCConnectionSetupComplete;
  std::string xmlNASAttachRequest;
  std::string xmlNASPdnConnectivityRequest;

  std::string xmlNASAuthenticationRequest;
  std::string xmlNASAuthenticationResponse;

  std::string xmlNASSecurityModeCommand;
  std::string xmlNASSecurityModeComplete;

  std::string xmlSecurityModeCommand;
  std::string xmlSecurityModeComplete;

  std::string xmlUECapabilityEnquiry;
  std::string xmlUECapabilityInformation;

  std::string xmlNASEsmInformationRequest;
  std::string xmlNASEsmInformationResponse;

  std::string xmlRRCConnectionReconfiguration;
  std::string xmlNASAttachAccept;
  std::string xmlNASActivateDefaultEpsBearerContextRequest;
  std::string xmlRRCConnectionReconfigurationComplete;

  std::string xmlNASAttachComplete;
  std::string xmlNASActivateDefaultEpsBearerContextAccept;

  std::string xmlRRCConnectionRelease;

  int  UEid;
  int  retcode;
  bool DataGenerator;
  int  EsmInformationTransfer;

  int CipheringAlgorithm;
  int IntegrityProtection;
  int Cell;

  public:

  sM_LTE_UeRegistration (
    int          _Cell,
	std::string& _xmlRRCConnectionRequest,
    std::string& _xmlRRCConnectionSetup,
    std::string& _xmlRRCConnectionSetupComplete,
    std::string& _xmlNASAttachRequest,
    std::string& _xmlNASPdnConnectivityRequest,
    std::string& _xmlNASAuthenticationRequest,
    std::string& _xmlNASAuthenticationResponse,
    std::string& _xmlNASSecurityModeCommand,
    std::string& _xmlNASSecurityModeComplete,
    std::string& _xmlSecurityModeCommand,
    std::string& _xmlSecurityModeComplete,
    std::string& _xmlUECapabilityEnquiry,
    std::string& _xmlUECapabilityInformation,
    std::string& _xmlNASEsmInformationRequest,
    std::string& _xmlNASEsmInformationResponse,
    std::string& _xmlRRCConnectionReconfiguration,
    std::string& _xmlNASAttachAccept,
    std::string& _xmlNASActivateDefaultEpsBearerContextRequest,
    std::string& _xmlRRCConnectionReconfigurationComplete,
    std::string& _xmlNASAttachComplete,
    std::string& _xmlNASActivateDefaultEpsBearerContextAccept,
    std::string& _xmlRRCConnectionRelease,
    bool         _DataGenerator = false,
    int          _CipheringAlgorithm = -1,
    int          _IntegrityProtection = -1
  )
  {
    Cell                                         = _Cell;
	xmlRRCConnectionRequest                      = _xmlRRCConnectionRequest;
    xmlRRCConnectionSetup                        = _xmlRRCConnectionSetup;
    xmlRRCConnectionSetupComplete                = _xmlRRCConnectionSetupComplete;
    xmlNASAttachRequest                          = _xmlNASAttachRequest;
    xmlNASPdnConnectivityRequest                 = _xmlNASPdnConnectivityRequest;
    xmlNASAuthenticationRequest                  = _xmlNASAuthenticationRequest;
    xmlNASAuthenticationResponse                 = _xmlNASAuthenticationResponse;
    xmlNASSecurityModeCommand                    = _xmlNASSecurityModeCommand;
    xmlNASSecurityModeComplete                   = _xmlNASSecurityModeComplete;
    xmlSecurityModeCommand                       = _xmlSecurityModeCommand;
    xmlSecurityModeComplete                      = _xmlSecurityModeComplete;
    xmlUECapabilityEnquiry                       = _xmlUECapabilityEnquiry;
    xmlUECapabilityInformation                   = _xmlUECapabilityInformation;
    xmlNASEsmInformationRequest                  = _xmlNASEsmInformationRequest;
    xmlNASEsmInformationResponse                 = _xmlNASEsmInformationResponse;
    xmlRRCConnectionReconfiguration              = _xmlRRCConnectionReconfiguration;
    xmlNASAttachAccept                           = _xmlNASAttachAccept;
    xmlNASActivateDefaultEpsBearerContextRequest = _xmlNASActivateDefaultEpsBearerContextRequest;
    xmlRRCConnectionReconfigurationComplete      = _xmlRRCConnectionReconfigurationComplete;
    xmlNASAttachComplete                         = _xmlNASAttachComplete;
    xmlNASActivateDefaultEpsBearerContextAccept  = _xmlNASActivateDefaultEpsBearerContextAccept;
    xmlRRCConnectionRelease                      = _xmlRRCConnectionRelease;
    DataGenerator                                = _DataGenerator;
    UEid                                         = 0;
    EsmInformationTransfer                       = 0;
    CipheringAlgorithm                           = _CipheringAlgorithm;
    IntegrityProtection                          = _IntegrityProtection;
  }

  // Destructor

  ~sM_LTE_UeRegistration()
  {
  }

  void start(void);

};

#endif // _SM_LTE_UEREGISTRATION_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/