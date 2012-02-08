/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_TestModeActivation.cpp

Version:        1.0

Author:        Dennis M Senyonjo


Copyright (c) 2011. 


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    1.0         18.01.2011 / Dennis M Senyonjo     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <sstream>

/* PUBLIC *********************************************************************/

void sM_LTE_TestModeActivation::start(void)
{
  sM_LteMlDefault::start();

  DECL_NEW_ETC_PDU( sendpdu, EtcActivateRbTestMode, xmlActivateTestMode.c_str() );

  GCO_DUMP(sendpdu.pdu);

  OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( sendpdu, UEid ) );
  delete &sendpdu;

  PE_PRINT(" UE <- SS  RRC: DlInformationTransfer" );
  PE_PRINT("           NAS: ACTIVATE TEST MODE" );
  PE_PRINT("           TC: Waiting for Activate Test Mode Complete" );

  rsDefaultTimer->set(GEN_EPSSM_TIMER);

  NEXT_STATE ( ActivateTestModeComplete );
}

ON_TIMER( sM_LTE_TestModeActivation, ActivateTestModeComplete, RS_Default_Timer)
{
  PE_ERR("####### Activate Test Mode Complete timeout #######" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_ETC_PDU( sM_LTE_TestModeActivation, ActivateTestModeComplete, EtcActivateRbTestModeComplete )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
  PE_PRINT("           NAS: ACTIVATE TEST MODE COMPLETE" );

  if ( xmlActivateTestModeComplete != "" )
  {
    if ( GCO_MATCHF(root, xmlActivateTestModeComplete.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  PE_PRINT("           TC: Test mode activation procedure completed" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/