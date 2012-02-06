/*

EUTRAN L2 SW















                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_UlNasMsgHandler.cpp

Version:        2.2

Author:         Dennis M Senyonjo


Copyright (c) 2011. 


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
	2.2			01.04.2011 /     V-A		Log print tweaks
	2.1         21.02.2011 /     V-A      Support separated EsmPdnConnectivityRequest + 
											-EsmActivateDedicatedEpsBearerContextReject
											-EsmActivateDefaultEpsBearerContextAccept
                                            -Support EsmPdnDisconnectRequest
    2.0         11.02.2011 / Dennis M Senyonjo     KSI update
    1.0         18.01.2011 / Dennis M Senyonjo     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <sstream>

/* PUBLIC *********************************************************************/

void sM_LTE_UlNasMsgHandler::start(void)
{
  sM_LteMlDefault::start();

  if ( (strcmp (MessageId, "AttachRequest")) == 0 )
  {
    rsDefaultTimer->set(GEN_EMM_TIMER);
    NEXT_STATE ( AttachRequest );
    PE_PRINT("           TC: Waiting for Attach Request" );
  }
  else if ( (strcmp (MessageId, "AttachComplete")) == 0 )
  {
    rsDefaultTimer->set(GEN_EMM_TIMER);
    NEXT_STATE ( AttachComplete );
    PE_PRINT("           TC: Waiting for Attach Complete" );
  }
  else if ( (strcmp (MessageId, "ServiceRequest")) == 0 )
  {
    rsDefaultTimer->set(GEN_EMM_TIMER);
    PE_PRINT("           TC: Waiting for Service Request" );
    NEXT_STATE ( ServiceRequest );
  }
  else if ( (strcmp (MessageId, "TrackingAreaUpdateRequest")) == 0 )
  {
    rsDefaultTimer->set(GEN_EMM_TIMER);
    NEXT_STATE ( TrackingAreaUpdateRequest );
    PE_PRINT("           TC: Waiting for Tracking Area Update Request" );
  }
  else if ( (strcmp (MessageId, "TrackingAreaUpdateComplete")) == 0 )
  {
    rsDefaultTimer->set(GEN_EMM_TIMER);
    NEXT_STATE ( TrackingAreaUpdateComplete );
    PE_PRINT("           TC: Waiting for Tracking Area Update Complete"  );
  }
  else if ( (strcmp (MessageId, "ActivateDedicatedEpsBearerContextAccept")) == 0 )
  {
    rsDefaultTimer->set(GEN_EPSSM_TIMER);
    NEXT_STATE ( ActivateDedicatedEpsBearerContextAccept );
    PE_PRINT("           TC: Waiting for Activate Dedicated Eps Bearer Context Accept"  );
  }
  else if ( (strcmp (MessageId, "DetachRequest")) == 0 )
  {
    rsDefaultTimer->set(20000);
    NEXT_STATE ( DetachRequest );
    PE_PRINT("           TC: Waiting for Detach Request" );
  }
  else if ( (strcmp (MessageId, "EsmPdnDisconnectRequest")) == 0 )
  {
    rsDefaultTimer->set(20000);
    NEXT_STATE ( PdnDisconnectRequest );
    PE_PRINT( "           TC: Waiting for Pdn Disconnect Request");
  }
  else if ( (strcmp (MessageId, "EsmActivateDefaultEpsBearerContextAccept")) == 0 )
  {
    rsDefaultTimer->set(5000);
    NEXT_STATE ( ActivateDefaultEpsBearerContextAccept );
    PE_PRINT( "           TC: Waiting for Esm Activate Default Eps Bearer Context Accept");
  }
  else if ( (strcmp (MessageId, "EsmActivateDefaultEpsBearerContextReject")) == 0 )
  {
    rsDefaultTimer->set(20000);
    NEXT_STATE ( EsmActivateDefaultEpsBearerContextReject );
    PE_PRINT("           TC: Waiting for Esm Activate Default Eps Bearer Context Reject");
  }
  else if ( (strcmp (MessageId, "EsmActivateDedicatedEpsBearerContextReject")) == 0 )
  {
    rsDefaultTimer->set(20000);
    NEXT_STATE ( EsmActivateDedicatedEpsBearerContextReject );
	PE_PRINT("           TC: Waiting for Esm Activate Dedicated Eps Bearer Context Reject");
  }
  else if ( (strcmp (MessageId, "EsmPdnConnectivityRequest")) == 0 )
  {
    rsDefaultTimer->set(5000);
    NEXT_STATE ( PdnConnectivityRequest );
	PE_PRINT("           TC: Waiting for Esm Pdn Connectivity Request");
  }
  else
  {
    string msg = cmw_Utils("####### Selected MessageId (%s) parameter is not supported #######", MessageId);
    PE_ERR( msg.c_str() );
    NOTIFY_COMPLETION_FAILURE();
  }
}

/******************************************************************************/
/*         NAS: ATTACH REQUEST                                                */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, AttachRequest, RS_Default_Timer )
{
  PE_ERR("           TC: Attach Request timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, AttachRequest, PdEpsMobilityManagement, EmmMsgAttachRequest )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT("           NAS: ATTACH REQUEST" );
  rsDefaultTimer->set(GEN_EPSSM_TIMER);
  NEXT_STATE ( PdnConnectivityRequest );
}

/******************************************************************************/
/*         NAS: PDN CONNECTIVITY REQUEST                                      */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, PdnConnectivityRequest, RS_Default_Timer)
{
  PE_ERR("           TC: Pdn Connectivity Request timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, PdnConnectivityRequest, PdEpsSessionManagement, EsmPdnConnectivityRequest )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT("           NAS: PDN CONNECTIVITY REQUEST" );
  LteReferenceTestConditions->setPdnConnRequest(pdu);  
  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* UE->SS  RRC: UlInformationTransfer                                         */
/*         NAS: ATTACH COMPLETE                                               */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, AttachComplete, RS_Default_Timer)
{
  PE_ERR("           TC: Attach Complete timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, AttachComplete, PdEpsMobilityManagement, EmmMsgDetachRequest )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
  PE_PRINT("           NAS: DETACH REQUEST" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, AttachComplete, PdEpsMobilityManagement, EmmMsgAttachComplete )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
  PE_PRINT("           NAS: ATTACH COMPLETE" );

  rsDefaultTimer->set(GEN_EPSSM_TIMER);

  NEXT_STATE ( ActivateDefaultEpsBearerContextAccept );
}

/******************************************************************************/
/*         NAS: ACTIVATE DEFAULT EPS BEARER CONTEXT ACCEPT                    */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, ActivateDefaultEpsBearerContextAccept, RS_Default_Timer)
{
  PE_ERR("           TC: Activate Default Eps Bearer Context Accept timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, ActivateDefaultEpsBearerContextAccept, PdEpsSessionManagement, EsmActivateDefaultEpsBearerContextReject )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_ERR("           NAS: ACTIVATE DEFAULT EPS BEARER CONTEXT REJECT" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, ActivateDefaultEpsBearerContextAccept, PdEpsSessionManagement, EsmActivateDefaultEpsBearerContextAccept )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT("           NAS: ACTIVATE DEFAULT EPS BEARER CONTEXT ACCEPT" );
  notifyCompletion(EV_OK);
}

/******************************************************************************/
/*         NAS: SERVICE REQUEST                                               */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, ServiceRequest, RS_Default_Timer)
{
  PE_ERR("           TC: Service Request timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EMM_SERVICE_REQUEST_PDU( sM_LTE_UlNasMsgHandler, ServiceRequest )
{
  rsDefaultTimer->reset();

  globaldata->ExtractServiceReq( pdu, root );
  int eksi = pdu->KsiAndSequenceNumberV()->KsiASME()->get();
  globaldata_lte->getUEIdentity(0)->seteKSI(eksi);

  globaldata_lte->getUEIdentity(0)->getSecurityParamsContainer()->setLatestNASUlCount(
    StateMachineLTE::getNasCount(true, 0),
    globaldata_lte->getUEIdentity(0)->geteKSI()
  );

  GCO_DUMP(pdu);
  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
  PE_PRINT("           NAS: SERVICE REQUEST" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/*         NAS: TRACKING AREA UPDATE REQUEST                                  */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, TrackingAreaUpdateRequest, RS_Default_Timer)
{
  PE_ERR("           TC: Tracking Area Update Request timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, TrackingAreaUpdateRequest, PdEpsMobilityManagement, EmmMsgTrackingAreaUpdateRequest )
{
  rsDefaultTimer->reset();

  int eksi = pdu->NasKeySetIdentifierASME()->EPSNasKeySetIdentifierV()->KeySetIdentifier()->get();
  globaldata_lte->getUEIdentity(0)->seteKSI(eksi);

  GCO_DUMP(pdu);
  PE_PRINT("           NAS: TRACKING AREA UPDATE REQUEST" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* UE->SS  RRC: UlInformationTransfer                                         */
/*         NAS: TRACKING AREA UPDATE COMPLETE                                 */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, TrackingAreaUpdateComplete, RS_Default_Timer)
{
  PE_ERR("           TC: Tracking Area Update Complete timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, TrackingAreaUpdateComplete, PdEpsMobilityManagement, EmmMsgTrackingAreaUpdateComplete )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
  PE_PRINT("           NAS: TRACKING AREA UPDATE COMPLETE" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* UE->SS  RRC: UlInformationTransfer                                         */
/*         NAS: ACTIVATE DEDICATED EPS BEARER CONTEXT ACCEPT                  */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, ActivateDedicatedEpsBearerContextAccept, RS_Default_Timer)
{
  PE_ERR("           TC: Activate Dedicated Eps Bearer Context Accept timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, ActivateDedicatedEpsBearerContextAccept, PdEpsSessionManagement, EsmActivateDefaultEpsBearerContextReject )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_ERR("           NAS: ACTIVATE DEDICATED EPS BEARER CONTEXT REJECT" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, ActivateDedicatedEpsBearerContextAccept, PdEpsSessionManagement, EsmActivateDedicatedEpsBearerContextAccept )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT("           NAS: ACTIVATE DEDICATED EPS BEARER CONTEXT ACCEPT" );
  notifyCompletion(EV_OK);
}

/******************************************************************************/
/*         NAS: DETACH REQUEST                                                */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, DetachRequest, RS_Default_Timer)
{
  PE_ERR("           TC: Detach Request timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, DetachRequest, PdEpsMobilityManagement, EmmMsgDetachRequest )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
  PE_PRINT("           NAS: DETACH REQUEST" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/*         NAS: PDN DISCONNECT REQUEST                                                */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, PdnDisconnectRequest, RS_Default_Timer)
{
  PE_ERR("           TC: Pdn Disconnect Request timeOut");
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, PdnDisconnectRequest, PdEpsSessionManagement, EsmPdnDisconnectRequest )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);

  globaldata_lte->setPTI( root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->get() );

  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer");
  PE_PRINT("           NAS: PDN Disconnect Request");

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/*         NAS: ESM ACTIVATE DEFAULT EPS BEARER CONTEXT REJECT                        */
/******************************************************************************/
ON_TIMER( sM_LTE_UlNasMsgHandler, EsmActivateDefaultEpsBearerContextReject, RS_Default_Timer)
{
  PE_ERR("           TC: Esm Activate Default Eps Bearer Context Reject - Timeout");
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, EsmActivateDefaultEpsBearerContextReject, PdEpsSessionManagement, EsmActivateDefaultEpsBearerContextReject )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer");
  PE_PRINT("           NAS: Esm Activate Default Eps Bearer Context Reject");

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/*         NAS: ESM ACTIVATE DEDICATED EPS BEARER CONTEXT REJECT                        */
/******************************************************************************/

ON_TIMER( sM_LTE_UlNasMsgHandler, EsmActivateDedicatedEpsBearerContextReject, RS_Default_Timer)
{
  PE_ERR("           TC: Esm Activate Dedicated Eps Bearer Context Reject - Timeout" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_EPS_NAS_PDU( sM_LTE_UlNasMsgHandler, EsmActivateDedicatedEpsBearerContextReject, PdEpsSessionManagement, EsmActivateDedicatedEpsBearerContextReject )
{
  rsDefaultTimer->reset();
  GCO_DUMP(pdu);
  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer");
  PE_PRINT("           NAS: Esm Activate Dedicated Eps Bearer Context Reject");

  notifyCompletion(EV_OK);
}


/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/