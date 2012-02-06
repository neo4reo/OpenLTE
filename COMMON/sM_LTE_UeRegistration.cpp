/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_UeRegistration.cpp

Version:        1.0

Author:          Dennis M Senyonjo


Copyright (c) 2011. 



*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <mlapi.h>
#include <sstream>
#include <mddb_eps_nas.h>

using namespace std;

/* PUBLIC *********************************************************************/

void sM_LTE_UeRegistration::start(void)
{
  sM_LteMlDefault::start();

  #pragma region sM_LTE_RrcConnectionEstablishment

  /******************************************************************************/
  /* UE->SS  RRC: RRCConnectionRequest                                          */
  /* SS->UE  RRC: RRCConnectionSetup                                            */
  /* UE->SS  RRC: RRCConnectionSetupComplete                                    */
  /******************************************************************************/

  RUN_NEWP_STATEMACHINE ( sM_LTE_RrcConnectionEstablishment, null,
    (
    Cell,                         // Cell handle
	xmlRRCConnectionRequest,      // RRCConnectionRequest
    xmlRRCConnectionSetup,        // RRCConnectionSetup
    xmlRRCConnectionSetupComplete // RRCConnectionSetupComplete
    )
  );

  RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));

  #pragma endregion
  #pragma region sM_LTE_AttachRequest

  if( EV_OK == retcode )
  {
    /******************************************************************************/
    /*         NAS: ATTACH REQUEST                                                */
    /*         NAS: PDN CONNECTIVITY REQUEST                                      */
    /******************************************************************************/

    RUN_NEWP_STATEMACHINE( sM_LTE_AttachRequest, null,
      (
      xmlNASAttachRequest,         // AttachRequest
      xmlNASPdnConnectivityRequest // PdnConnectivityRequest
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  #pragma endregion
  #pragma region sM_LTE_Identification

  if( EV_OK == retcode )
  {
    if ( LteReferenceTestConditions->getAttachRequest()->EPSMobileIdentityLV()->Choice()->isActive_ImsiValue() != true )
    {
      /******************************************************************************/
      /* SS->UE  RRC: DlInformationTransfer                                         */
      /*         NAS: IDENTITY REQUEST                                              */
      /* UE->SS  RRC: UlInformationTransfer                                         */
      /*         NAS: IDENTITY RESPONSE                                             */
      /******************************************************************************/

      RUN_NEWP_STATEMACHINE( sM_LTE_Identification, null,
        (
        std::string("EmmIdentityRequest.xml"), // IdentityRequest
        1,                                     // IMSI
        std::string("")                        // IdentityResponse
        )
      );

      RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
    }
  }

  #pragma endregion
  #pragma region sM_LTE_Authentication

  /******************************************************************************/
  /* SS->UE  RRC: DlInformationTransfer                                         */
  /*         NAS: AUTHENTICATION REQUEST                                        */
  /* UE->SS  RRC: UlInformationTransfer                                         */
  /*         NAS: AUTHENTICATION RESPONSE                                       */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_Authentication, null,
      (
      xmlNASAuthenticationRequest, // AuthenticationRequest
      xmlNASAuthenticationResponse // AuthenticationResponse
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  #pragma endregion
  #pragma region sM_LTE_NasSecurity

  /******************************************************************************/
  /* SS->UE  RRC: DlInformationTransfer                                         */
  /*         NAS: SECURITY MODE COMMAND                                         */
  /* UE->SS  RRC: UlInformationTransfer                                         */
  /*         NAS: SECURITY MODE COMPLETE                                        */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    // 3 possible ways to set ciphering and integrity algorithm
    // 1. LteReferenceTestConditions xml      : -1
    // 2. Local xml                           : -2
    // 3. SM parameter                        : >= 0

    RUN_NEWP_STATEMACHINE ( sM_LTE_NasSecurity, null,
      (
      xmlNASSecurityModeCommand,                                                                             // SecurityModeCommand
      (CipheringAlgorithm == -1 ? LteReferenceTestConditions->getDefNasCiphProtAlg() : CipheringAlgorithm),  // cipheringAlgorithm
      (IntegrityProtection == -1 ? LteReferenceTestConditions->getDefNasIntProtAlg() : IntegrityProtection), // integrityProtAlgorithm
      xmlNASSecurityModeComplete                                                                             // SecurityModeComplete
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  #pragma endregion
  #pragma region sM_LTE_AsSecurity

  /******************************************************************************/
  /* SS->UE  RRC: SecurityModeCommand                                           */
  /* UE->SS  RRC: SecurityModeComplete                                          */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_AsSecurity, null,
      (
      xmlSecurityModeCommand,                            // SecurityModeCommand
      LteReferenceTestConditions->getDefAsCiphProtAlg(), // cipheringAlgorithm
      LteReferenceTestConditions->getDefAsIntProtAlg(),  // integrityProtAlgorithm
      xmlSecurityModeComplete                            // SecurityModeComplete
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  #pragma endregion
  #pragma region sM_LTE_EsmInformationTransfer

  /******************************************************************************/
  /* SS->UE  RRC: DlInformationTransfer                                         */
  /*         NAS: ESM INFORMATION REQUEST                                       */
  /* UE->SS  RRC: UlInformationTransfer                                         */
  /*         NAS: ESM INFORMATION RESPONSE                                      */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    if ( LteReferenceTestConditions->getPdnConnRequest()->isActive_EsmInformationTransferFlagTV() )
    {
      RUN_NEWP_STATEMACHINE ( sM_LTE_EsmInformationTransfer, null,
        (
        xmlNASEsmInformationRequest, // EsmInformationRequest
        xmlNASEsmInformationResponse // EsmInformationResponse
        )
      );

      RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
    }
  }

  #pragma endregion
  #pragma region sM_LTE_TestModeActivation

  /******************************************************************************/
  /* SS->UE  RRC: DlInformationTransfer                                         */
  /*         NAS: ACTIVATE TEST MODE                                            */
  /* UE->SS  RRC: UlInformationTransfer                                         */
  /*         NAS: ACTIVATE TEST MODE COMPLETE                                   */
  /******************************************************************************/

  if( (EV_OK == retcode) && (LteReferenceTestConditions->getTestModeActDuringReg() == 1) )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_TestModeActivation, null,
      (
      std::string("Lte_EtcActivateTestMode.xml"), // ActivateTestMode
      std::string("")                             // ActivateTestModeComplete
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  #pragma endregion
  #pragma region sM_LTE_UeRadioAccessCapability

  /******************************************************************************/
  /* SS->UE  RRC: UECapabilityEnquiry                                           */
  /* UE->SS  RRC: UECapabilityInformation                                       */
  /******************************************************************************/

  // Message checking via PIXIT parameter

  if( EV_OK == retcode )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_UeRadioAccessCapability, null,
      (
      xmlUECapabilityEnquiry,    // UECapabilityEnquiry
      xmlUECapabilityInformation // UECapabilityInformation
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  #pragma endregion
  #pragma region sM_LTE_RrcConnectionReconfiguration

  /******************************************************************************/
  /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
  /*         NAS: ATTACH ACCEPT                                                 */
  /*         NAS: ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST                   */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_RrcConnectionReconfiguration, null,
      (
      xmlRRCConnectionReconfiguration,              // RRCConnectionReconfiguration
      xmlNASAttachAccept,                           // AttachAccept
      xmlNASActivateDefaultEpsBearerContextRequest, // ActivateDefaultEpsBearerContextRequest
      xmlRRCConnectionReconfigurationComplete,      // RRCConnectionReconfigurationComplete
      DataGenerator                                 // Datagenerator
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  /******************************************************************************/
  /* UE->SS  RRC: UlInformationTransfer                                         */
  /*         NAS: ATTACH COMPLETE                                               */
  /*         NAS: ACTIVATE DEFAULT EPS BEARER CONTEXT ACCEPT                    */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_UlNasMsgHandler, null,
      (
      "AttachComplete",                           // MessageId
      xmlNASAttachComplete,                       // AttachComplete
      xmlNASActivateDefaultEpsBearerContextAccept // ActivateDefaultEpsBearerContextAccept
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  if( EV_OK == retcode )
  {
    if ( xmlRRCConnectionRelease != "" )
    {
      /******************************************************************************/
      /* SS->UE  RRC: RRCConnectionRelease                                          */
      /******************************************************************************/

      RUN_NEWP_STATEMACHINE ( sM_LTE_RrcConnectionRelease, null,
        (
        xmlRRCConnectionRelease // RRCConnectionRelease
        )
      );

      RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));

      notifyCompletion(retcode);
    }
    else
    {
      notifyCompletion(EV_OK);
    }
  }
  else
  {
    notifyCompletion(EV_FAIL);
  }
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/