/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_TestModeDeactivation.cpp

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

void sM_LTE_TestModeDeactivation::start(void)
{
  sM_LteMlDefault::start();

  DECL_NEW_ETC_PDU( sendpdu, EtcDeactivateRbTestMode, xmlDeactivateTestMode.c_str() );

  GCO_DUMP(sendpdu.pdu);

  OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_EPS_NAS_PDU( sendpdu ) );
  delete &sendpdu;

  PE_PRINT(" UE <- SS  RRC: DlInformationTransfer" );
  PE_PRINT("           NAS: DEACTIVATE TEST MODE" );
  PE_PRINT("           TC: Waiting for Deactivate Test Mode Complete" );

  rsDefaultTimer->set(GEN_EPSSM_TIMER);

  NEXT_STATE ( DeactivateTestModeComplete );
}

ON_TIMER( sM_LTE_TestModeDeactivation, DeactivateTestModeComplete, RS_Default_Timer)
{
  PE_ERR("####### Deactivate Test Mode Complete timeout #######" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_ETC_PDU( sM_LTE_TestModeDeactivation, DeactivateTestModeComplete, EtcDeactivateRbTestModeComplete )
{
	rsDefaultTimer->reset();
	
	GCO_DUMP(pdu);
	
	PE_PRINT(" UE -> SS  RRC: UlInformationTransfer" );
	PE_PRINT("           NAS: DEACTIVATE TEST MODE COMPLETE" );
	
	if ( xmlDeactivateTestModeComplete != "" )
	{
		if ( GCO_MATCHF(root, xmlDeactivateTestModeComplete.c_str()) )
		{
			PE_PRINT("####### Message is OK #######" );
		}
		else
		{
			PE_ERR("####### Message is NOK #######" );
			NOTIFY_COMPLETION_FAILURE();
		}
	}

  PE_PRINT("           TC: Test mode deactivation procedure completed" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/