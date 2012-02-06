/*

EUTRAN L2 SW















                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_TauProcedure.cpp

Version:        1.0

Author:         Dennis M Senyonjo


Copyright (c) 2011.



*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_TauProcedure::start(void)
{
  sM_LteMlDefault::start();

  /******************************************************************************/
  /* UE->SS  RRC: RRCConnectionRequest                                          */
  /* SS->UE  RRC: RRCConnectionSetup                                            */
  /* UE->SS  RRC: RRCConnectionSetupComplete                                    */
  /******************************************************************************/

  RUN_NEWP_STATEMACHINE ( sM_LTE_RrcConnectionEstablishment, null,
    (
    CellHandle,                   // CellHandle
	xmlRRCConnectionRequest,      // RRCConnectionRequest
    xmlRRCConnectionSetup,        // RRCConnectionSetup
    xmlRRCConnectionSetupComplete // RRCConnectionSetupComplete
    )
  );

  RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));

  /******************************************************************************/
  /*         NAS: TRACKING AREA UPDATE REQUEST                                  */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_TrackingAreaUpdateRequest, null,
      (
      xmlTrackingAreaUpdateRequest // TrackingAreaUpdateRequest
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  /******************************************************************************/
  /* SS->UE  RRC: DlInformationTransfer                                         */
  /*         NAS: TRACKING AREA UPDATE ACCEPT                                   */
  /******************************************************************************/

  if( EV_OK == retcode )
  {
    DECL_NEW_EPS_NAS_PDU( sendpdu, PdEpsMobilityManagement, EmmMsgTrackingAreaUpdateAccept, xmlTrackingAreaUpdateAccept.c_str() );

    OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( sendpdu, globaldata_lte->getUEIdentity(UEid)->getUEHandle() ) );

    GCO_DUMP(sendpdu.pdu);

    delete &sendpdu;

    /******************************************************************************/
    /* UE->SS  RRC: UlInformationTransfer                                         */
    /*         NAS: TRACKING AREA UPDATE COMPLETE                                 */
    /******************************************************************************/

    RUN_NEWP_STATEMACHINE ( sM_LTE_UlNasMsgHandler, null,
      (
      "TrackingAreaUpdateComplete", // MessageId
      xmlTrackingAreaUpdateComplete // TrackingAreaUpdateComplete
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