/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_OpenLoopbackActivation.cpp

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

void sM_LTE_OpenLoopbackActivation::start(void)
{
  sM_LteMlDefault::start();

  DECL_NEW_ETC_PDU( sendpdu, EtcOpenUeTestLoop, xmlOpenUeTestLoop.c_str() );

  GCO_DUMP(sendpdu.pdu);

  OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( sendpdu, UEid ) );
  delete &sendpdu;

  PE_PRINT(" UE <- UE  RRC: DlInformationTransfer" );
  PE_PRINT("           NAS: OPEN UE TEST LOOP" );
  PE_PRINT("           TC: Waiting for Open UE Test Loop Complete" );

  rsDefaultTimer->set(GEN_EPSSM_TIMER);

  NEXT_STATE ( OpenUeTestLoopComplete );
}

ON_TIMER( sM_LTE_OpenLoopbackActivation, OpenUeTestLoopComplete, RS_Default_Timer)
{
  PE_ERR("####### Open UE Test Loop Complete timeout #######" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_ETC_PDU( sM_LTE_OpenLoopbackActivation, OpenUeTestLoopComplete, EtcOpenUeTestLoopComplete )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
  PE_PRINT("           NAS: OPEN UE TEST LOOP COMPLETE" );

  if ( xmlOpenUeTestLoopComplete != "" )
  {
    if ( GCO_MATCHF(root, xmlOpenUeTestLoopComplete.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  PE_PRINT("           TC: Open UE Test Loop activation procedure completed" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/