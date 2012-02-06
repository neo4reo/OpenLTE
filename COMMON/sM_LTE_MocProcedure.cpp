/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_MocProcedure.cpp

Version:        1.0

Author:         Dennis M Senyonjo


Copyright (c) 2011. 


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    1.0         18.01.2011/Dennis M S      Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <mlapi.h>
#include <sstream>
#include <mddb_eps_nas.h>

using namespace std;

/* PUBLIC *********************************************************************/

void sM_LTE_MocProcedure::start(void)
{
  sM_LteMlDefault::start();

  /******************************************************************************/
  /* UE->SS  RRC: RRCConnectionRequest                                          */
  /* SS->UE  RRC: RRCConnectionSetup                                            */
  /* UE->SS  RRC: RRCConnectionSetupComplete                                    */
  /******************************************************************************/

  RUN_NEWP_STATEMACHINE ( sM_LTE_RrcConnectionEstablishment, null,
    (
    CellNumber,                    // CellNumber
	xmlRRCConnectionRequest,       // RRCConnectionRequest
    xmlRRCConnectionSetup,         // RRCConnectionSetup
    xmlRRCConnectionSetupComplete, // RRCConnectionSetupComplete
    10000                          // WaitTimer
    )
  );

  RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));

  /******************************************************************************/
  /* UE->SS  RRC: UlInformationTransfer                                         */
  /*         NAS: SERVICE REQUEST                                               */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_UlNasMsgHandler, null,
      (
      "ServiceRequest", // MessageId
      xmlServiceRequest // ServiceRequest
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

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

  if( DedicatedContext == false )
  {
    /******************************************************************************/
    /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
    /******************************************************************************/

    if( EV_OK == retcode )
    {
      RUN_NEWP_STATEMACHINE ( sM_LTE_RrcConnectionReconfiguration, null,
        (
        xmlRRCConnectionReconfiguration,         // RRCConnectionReconfiguration
        std::string(""),                         // AttachAccept
        std::string(""),                         // ActivateDefaultEpsBearerContextRequest
        xmlRRCConnectionReconfigurationComplete, // RRCConnectionReconfigurationComplete
        DataGenerator                            // Datagenerator
        )
      );

      RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));

      notifyCompletion(retcode);
    }
    else
    {
      notifyCompletion(EV_FAIL);
    }
  }
  else
  {
    /******************************************************************************/
    /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
    /*         NAS: ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST                 */
    /******************************************************************************/

    if( EV_OK == retcode )
    {
      RUN_NEWP_STATEMACHINE ( sM_LTE_RrcConnectionReconfiguration, null,
        (
          xmlRRCConnectionReconfiguration,                // RRCConnectionReconfiguration
          xmlNASActivateDedicatedEpsBearerContextRequest, // NASActivateDedicatedEpsBearerContextRequest
          xmlRRCConnectionReconfigurationComplete,        // RRCConnectionReconfigurationComplete
          DataGenerator                                   // DataGenerator
        )
      );

      RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
    }

    /******************************************************************************/
    /* UE->SS  RRC: UlInformationTransfer                                         */
    /*         NAS: ACTIVATE DEDICATED EPS BEARER CONTEXT ACCEPT                  */
    /******************************************************************************/

    if( EV_OK == retcode )
    {
      RUN_NEWP_STATEMACHINE ( sM_LTE_UlNasMsgHandler, null,
        (
        "ActivateDedicatedEpsBearerContextAccept",    // MessageId
        xmlNASActivateDedicatedEpsBearerContextAccept // ActivateDedicatedEpsBearerContextAccept
        )
      );

      RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));

      notifyCompletion(retcode);
    }
    else
    {
      notifyCompletion(EV_FAIL);
    }
  }
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/