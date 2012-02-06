/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_RrcConnectionReestablishment.cpp

Version:        3.0

Author:          Dennis M Senyonjo


Copyright (c) 2011  Dennis M Senyonjo


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    3.0         22.03.2011 / Dennis M S     Security configuration
    1.0         18.01.2011 / Dennis M S     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <sstream>

/* PUBLIC *********************************************************************/

void sM_LTE_RrcConnectionReestablishment::start(void)
{
  sM_LteMlDefault::start();

  PE_PRINT("           TC: Waiting for RRCConnectionReestablishmentRequest" );

  if ( WaitTimer == 0 )
  {
    rsDefaultTimer->set(WAIT_RRC_REQ);
  }
  else
  {
    rsDefaultTimer->set(WaitTimer);
  }

  NEW_LTE_RRC_PDU( rrcConnectionReestablishment, DL_CCCH, RRCConnectionReestablishment, xmlRRCConnectionReestablishment.c_str() );

  // Temporary hardcoding

  if ( rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->isActive_mac_MainConfig() )
  {
    if ( LteReferenceTestConditions->getDRX_support() == 0 )
      rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->setActive_drx_Config(false);
    if ( LteReferenceTestConditions->getPHR_support() == 0 )
      rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->setActive_phr_Config(false);
    if ( LteReferenceTestConditions->getTAT_support() == 0 )
      rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->timeAlignmentTimerDedicated()->set(7);
  }

  if ( rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->isActive_physicalConfigDedicated() )
  {
    if ( LteReferenceTestConditions->getTargetPowerSupport() == 0 )
    {
      if ( rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->isActive_uplinkPowerControlDedicated() )
        rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->uplinkPowerControlDedicated()->accumulationEnabled()->set(0);
    }
    if ( LteReferenceTestConditions->getSRS_support() == 0 )
      rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->soundingRS_UL_ConfigDedicated()->setActive_release();
    if ( LteReferenceTestConditions->getCQI_support() == 0 )
      rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->setActive_cqi_ReportConfig(false);
  }

  NEXT_STATE ( RRCConnectionReestablishmentRequest );
}

ON_TIMER( sM_LTE_RrcConnectionReestablishment, RRCConnectionReestablishmentRequest, RS_Default_Timer)
{
  PE_ERR("####### RRCConnectionReestablishmentRequest timeout #######");
  NOTIFY_COMPLETION_FAILURE();
}

ON_LTE_UL_CCCH_RRC_PDU( sM_LTE_RrcConnectionReestablishment, RRCConnectionReestablishmentRequest, RRCConnectionReestablishmentRequest )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  Rnti = asp->Rnti()->get();
  std::string tmp = cmw_Utils(" UE -> SS  RRC: RRCConnectionReestablishmentRequest (Cell #%i)", asp->RrcCellHandle()->get() );
  PE_PRINT(tmp.c_str());

  if ( xmlRRCConnectionReestablishmentRequest != "" )
  {
    if ( GCO_MATCHF(root, xmlRRCConnectionReestablishmentRequest.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  if ( CellHandleTarget != -1 )
  {
    if ( asp->RrcCellHandle()->get() != CellHandleTarget )
    {
      std::string tmp = cmw_Utils("####### Message received in wrong cell (%i), Valid cell is %i #######", asp->RrcCellHandle()->get(), CellHandleTarget );
      PE_ERR(tmp.c_str());
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  NEXT_STATE ( GetMacUeId );
}

ON_ASPX( sM_LTE_RrcConnectionReestablishment, GetMacUeId, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcUpdateUeConfigInd )
{
  globaldata_lte->getUEIdentity()->setUEHandle(asp->RrcUeHandle()->get());
  globaldata_lte->getUEIdentity()->setMacUeId(asp->MacUeId()->get());
  globaldata_lte->getUEIdentity()->setCRNTI(asp->Crnti()->get());

  UeId = asp->MacUeId()->get();
  globaldata_lte->getUEIdentity(0)->setncc( rrcConnectionReestablishment.pdu->criticalExtensions()->c1()->rrcConnectionReestablishment_r8()->nextHopChainingCount()->get() );

  PE_PRINT("           TC: Security input parameters");
  PE_PRINT("           TC: PhyCellId = " << globaldata_lte->getCellConfig(CellHandleSource)->getPhyCellId() );
  PE_PRINT("           TC: DLEARFCN = " << globaldata_lte->getCellConfig(CellHandleSource)->getDLEARFCN() );
  PE_PRINT("           TC: NCC = " << globaldata_lte->getUEIdentity(0)->getncc() );

  RUN_NEWP_STATEMACHINE ( sM_LTE_CrrcSecurityConfigReq, null,
    (
    0,               // CorruptKenb
    true,            // ReestablishmentOrHandover
    CellHandleSource // CellId
    )
  );

  if ( LteReferenceTestConditions->getPx_TimingAdvance() != -1 )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_TimingAdvance, null,
      (
      std::string("Lte_CmacTimingControlConfigReq.xml"), // CmacTimingControlConfigReq
      UeId                                               // UeId
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  if ( LteReferenceTestConditions->getTargetPowerSupport() == 1 )
  {
    RUN_NEWP_STATEMACHINE ( sM_LTE_TxPowerControl, null,
      (
      std::string("Lte_CmacPowerControlConfigReq.xml"),   // CmacPowerControlConfigReq
      LteReferenceTestConditions->getTargetPowerPusch(),  // TargetPowerPusch
      LteReferenceTestConditions->getTargetPowerPucch(),  // TargetPowerPucch
      UeId                                                // UeId
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  if( EV_OK == retcode )
  {
    pCrrcAsp = NEW_ASP_WITH_DL_CCCH_RRC_PDU ( rrcConnectionReestablishment, asp->RrcUeHandle()->get(), Rnti );
    pCrrcAsp->Choice()->CrrcDlCcchMessageReq()->SupervisionDuration()->set(GEN_RRC_TIMER);

    GCO_DUMP(rrcConnectionReestablishment.pdu);

    OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp );

    PE_PRINT(" SS -> UE  RRC: RRCConnectionReestablishment " );

    PE_PRINT("           TC: Waiting for RRCConnectionReestablishmentComplete" );

    NEXT_STATE( RRCConnectionReestablishmentComplete );
  }
  else
  {
    PE_ERR("####### Timing advance setting failed #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_ASPX( sM_LTE_RrcConnectionReestablishment, RRCConnectionReestablishmentComplete, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_RRCCONNECTION_REESTABLISHMENT_COMPLETE_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("####### RRCConnectionReestablishmentComplete timeout #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_LTE_UL_DCCH_RRC_PDU( sM_LTE_RrcConnectionReestablishment, RRCConnectionReestablishmentComplete, RRCConnectionReestablishmentComplete )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  globaldata_lte->getUEIdentity(0)->setUEHandle(asp->RrcUeHandle()->get());

  DELAY_ALL_EPS_NAS_PDU_TRANSITIONS();

  PE_PRINT(" UE -> SS  RRC: RRCConnectionReestablishmentComplete " );

  if ( xmlRRCConnectionReestablishmentComplete != "" )
  {
    if ( GCO_MATCHF(root, xmlRRCConnectionReestablishmentComplete.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  PE_PRINT("           TC: RRC Connection Re-establishment procedure completed" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/