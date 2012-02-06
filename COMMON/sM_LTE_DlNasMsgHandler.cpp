/*

EUTRAN L3 SW















                        Test Step for CMW-500
                        ---------------------

                        SW module





Filename:       sM_LTE_DlNasMsgHandler.cpp

Version:        0.3

Author:         V-A


Copyright (c) 2011.


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    0.3         21.02.2011 / V-A		Added messages:
											-ESM_BEARER_RESOURCE_ALLOCATION_REJECT
											-ESM_BEARER_RESOURCE_MODIFICATION_REJECT
											
	0.2	        10.02.2011 / V-A        Added messages:
                                            -ESM_PDN_CONNECTIVITY_REJECT
                                            -ESM_PDN_DISCONNECT_REJECT
    0.1         02.02.2011 / V-A       Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <sstream>

/* PUBLIC *********************************************************************/

void sM_LTE_DlNasMsgHandler::start(void)
{
    sM_LteMlDefault::start();

    if ( DlMessageId == MDDB::Eps::Nas::ESM_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST )
    {
        DECL_NEW_EPS_NAS_PDU( dlmsg, PdEpsSessionManagement, EsmActivateDedicatedEpsBearerContextRequest, xmlMessage.c_str() );

        GCO_DUMP(dlmsg.pdu);

        OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( dlmsg ) );
        delete &dlmsg;

        PE_PRINT(" SS -> UE  RRC: DlInformationTransfer");
        PE_PRINT("           NAS: Activate Dedicated Eps Bearer Request");

        notifyCompletion(EV_OK);
    }
    else if( DlMessageId == MDDB::Eps::Nas::EMM_STATUS )
    {
        DECL_NEW_EPS_NAS_PDU( statusMsg, PdEpsMobilityManagement, EmmMsgStatus, xmlMessage.c_str());

        GCO_DUMP(statusMsg.pdu);

        OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( statusMsg ) );
        delete &statusMsg;

        PE_PRINT(" SS -> UE  RRC: DlInformationTransfer");
        PE_PRINT("           NAS: EMM STATUS");

        notifyCompletion(EV_OK);
    }
    else if( DlMessageId == MDDB::Eps::Nas::ESM_PDN_CONNECTIVITY_REJECT )
    {
        DECL_NEW_EPS_NAS_PDU( msg, PdEpsSessionManagement, EsmPdnConnectivityReject, xmlMessage.c_str());

        GCO_DUMP(msg.pdu);
        
        msg.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set( globaldata_lte->getPTI() );

        OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( msg ) );
        delete &msg;

        PE_PRINT(" SS -> UE  RRC: DlInformationTransfer");
        PE_PRINT("           NAS: ESM PDN CONNECTIVITY REJECT");

        notifyCompletion(EV_OK);
    }
    else if( DlMessageId == MDDB::Eps::Nas::ESM_PDN_DISCONNECT_REJECT )
    {
        DECL_NEW_EPS_NAS_PDU( msg, PdEpsSessionManagement, EsmPdnDisconnectReject, xmlMessage.c_str());

        GCO_DUMP(msg.pdu);

        msg.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set( globaldata_lte->getPTI() );

        OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( msg ) );
        delete &msg;

        PE_PRINT(" SS -> UE  RRC: DlInformationTransfer");
        PE_PRINT("           NAS: ESM PDN DISCONNECT REJECT");

        notifyCompletion(EV_OK);
    }
    else if( DlMessageId == MDDB::Eps::Nas::ESM_BEARER_RESOURCE_ALLOCATION_REJECT )
    {
        DECL_NEW_EPS_NAS_PDU( msg, PdEpsSessionManagement, EsmBearerResourceAllocationReject, xmlMessage.c_str());

        GCO_DUMP(msg.pdu);
        
        if( m_argslist != 0 )
        {
            int pti = va_arg(m_argslist, int);// [0]: PTI 
            // if pti == -1 -> use last stored pti value from globaldata_lte
            msg.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set( ((pti==-1)?globaldata_lte->getPTI():pti) );
            
            int EBI = va_arg(m_argslist, int);// [1]: EBI
            if( EBI != -1 )
                msg.root->Choice()->PdEpsSessionManagement()->EpsBearerIdentity()->set( EBI );
            
            int cause = va_arg(m_argslist, int);// [1]: Reject cause
            if( cause != -1 )
                msg.root->Choice()->PdEpsSessionManagement()->Header()->Default()->Choice()->EsmBearerResourceAllocationReject()->EsmCauseV()->EsmCause()->set( cause );
        }
        else msg.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set( globaldata_lte->getPTI() );

        OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( msg ) );
        delete &msg;

        PE_PRINT(" SS -> UE  RRC: DlInformationTransfer");
        PE_PRINT("           NAS: ESM BEARER RESOURCE ALLOCATION REJECT");

        notifyCompletion(EV_OK);
    }
    else if( DlMessageId == MDDB::Eps::Nas::ESM_BEARER_RESOURCE_MODIFICATION_REJECT )
    {
        DECL_NEW_EPS_NAS_PDU( msg, PdEpsSessionManagement, EsmBearerResourceModificationReject, xmlMessage.c_str());

        GCO_DUMP(msg.pdu);
        
        if( m_argslist != 0 )
        {
            int pti = va_arg(m_argslist, int);// [0]: PTI 
            // if pti == -1 -> use last stored pti value from globaldata_lte
            msg.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set( ((pti==-1)?globaldata_lte->getPTI():pti) );
            
            int EBI = va_arg(m_argslist, int);// [1]: EBI
            if( EBI != -1 )
                msg.root->Choice()->PdEpsSessionManagement()->EpsBearerIdentity()->set( EBI );
            
            int cause = va_arg(m_argslist, int);// [1]: Reject cause
            if( cause != -1 )
                msg.root->Choice()->PdEpsSessionManagement()->Header()->Default()->Choice()->EsmBearerResourceModificationReject()->EsmCauseV()->EsmCause()->set( cause );
        }
        else{ 
            msg.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set( globaldata_lte->getPTI() );
        }

        OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( msg ) );
        delete &msg;

        PE_PRINT(" SS -> UE  RRC: DlInformationTransfer");
        PE_PRINT("           NAS: ESM BEARER RESOURCE MODIFICATION REJECT");

        notifyCompletion(EV_OK);
    }
    else
    {
        RUN_NEWP_STATEMACHINE( sM_ResultPane, null,
            (
            "####### Selected MessageId parameter is not supported #######",
            TC_MESSAGE_ERROR
            )
        );
        NOTIFY_COMPLETION_FAILURE();
    }
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/