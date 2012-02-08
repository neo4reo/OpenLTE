/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_CloseLoopbackActivation.cpp

Version:        1.0

Author:         Dennis M Senyonjo


Copyright (c) 2011. 


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    1.0         18.01.2011 / Dennis M Senyonjo     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <sstream>

/* PUBLIC *********************************************************************/

void sM_LTE_CloseLoopbackActivation::start(void)
{
  sM_LteMlDefault::start();

  DECL_NEW_ETC_PDU( sendpdu, EtcCloseUeTestLoop, xmlCloseUeTestLoop.c_str() );

  GCO_DUMP(sendpdu.pdu);

  OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( sendpdu, UEIdentity ) );
  delete &sendpdu;

  PE_PRINT(" UE <- SS  RRC: DlInformationTransfer" );
  PE_PRINT("           NAS: CLOSE UE TEST LOOP" );
  PE_PRINT("           TC: Waiting for Close UE Test Loop Complete" );

  rsDefaultTimer->set(GEN_EPSSM_TIMER);

  NEXT_STATE ( CloseUeTestLoopComplete );
}

ON_TIMER( sM_LTE_CloseLoopbackActivation, CloseUeTestLoopComplete, RS_Default_Timer)
{
  PE_ERR("####### Close UE Test Loop Complete timeout #######" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_ETC_PDU( sM_LTE_CloseLoopbackActivation, CloseUeTestLoopComplete, EtcCloseUeTestLoopComplete )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
  PE_PRINT("           NAS: CLOSE UE TEST LOOP COMPLETE" );

  if ( xmlCloseUeTestLoopComplete != "" )
  {
    if ( GCO_MATCHF(root, xmlCloseUeTestLoopComplete.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  PE_PRINT("           TC: Close UE Test Loop activation procedure completed" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/