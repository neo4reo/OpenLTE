/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_AsSecurity.cpp

Version:        1.0

Author:          Dennis M Senyonjo


Copyright (c) 2011.  Dennis M Senyonjo.


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    1.0         18.01.2011 /Dennis M S    Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_AsSecurity::start(void)
{
  sM_LteMlDefault::start();

  RUN_NEWP_STATEMACHINE ( sM_LTE_CrrcSecurityConfigReq, null,
    (
    0,     // CorruptKenb
    false, // ReestablishmentOrHandover
    0      // CellId
    )
  );

  DECL_NEW_LTE_RRC_PDU ( sendpdu, DL_DCCH, SecurityModeCommand, xmlSecurityModeCommand.c_str());

  sendpdu.pdu->criticalExtensions()->c1()->securityModeCommand_r8()->securityConfigSMC()
  ->securityAlgorithmConfig()->cipheringAlgorithm()->set(cipheringAlgorithm);

  sendpdu.pdu->criticalExtensions()->c1()->securityModeCommand_r8()->securityConfigSMC()
  ->securityAlgorithmConfig()->integrityProtAlgorithm()->set(integrityProtAlgorithm);

  GCO_DUMP(sendpdu.pdu);

  _CrrcAsp* pCrrcAsp = NEW_ASP_WITH_DL_DCCH_RRC_PDU (sendpdu);
  pCrrcAsp->Choice()->CrrcDlDcchMessageReq()->SupervisionDuration()->set(WAIT_SMC_COMP);

  OUTPUT ( SAP_LTE_NET_CRRC, pCrrcAsp );
  delete &sendpdu;

  PE_PRINT(" UE <- SS  RRC: SecurityModeCommand" );
  PE_PRINT("           TC: Waiting for Security Mode Complete" );

  NEXT_STATE ( SecurityModeComplete );
}

ON_ASPX( sM_LTE_AsSecurity, SecurityModeComplete, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_SECURITY_MODE_COMPLETE_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("####### Security Mode Complete timeout #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_LTE_UL_DCCH_RRC_PDU_ASSERT_EVENT( sM_LTE_AsSecurity, SecurityModeComplete, SecurityModeFailure )
{
  GCO_DUMP(pdu);

  PE_ERR(" UE -> SS  RRC: SecurityModeFailure" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_LTE_UL_DCCH_RRC_PDU_ASSERT_EVENT( sM_LTE_AsSecurity, SecurityModeComplete, SecurityModeComplete )
{
  GCO_DUMP(pdu);

  PE_PRINT(" UE -> SS  RRC: SecurityModeComplete" );

  if ( xmlSecurityModeComplete != "" )
  {
    if ( GCO_MATCHF(root, xmlSecurityModeComplete.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  PE_PRINT("           TC: AS security procedure completed" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/