/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_RrcConnectionEstablishment.cpp

Version:        2.0

Author:          Dennis M Senyonjo


Copyright (c) 2011.  Dennis M Senyonjo


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    2.0         06.04.2011 / Dennis M S     Failure scenario
    1.0         18.01.2011 / Dennis M S     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <sstream>

/* PUBLIC *********************************************************************/

void sM_LTE_RrcConnectionEstablishment::start(void)
{
  sM_LteMlDefault::start();

  PE_PRINT("           TC: Waiting for RRCConnectionRequest");

  if ( WaitTimer == 0 )
  {
    rsDefaultTimer->set(WAIT_RRC_REQ);
  }
  else
  {
    rsDefaultTimer->set(WaitTimer);
  }

  NEW_LTE_RRC_PDU( rrcConnectionSetup, DL_CCCH, RRCConnectionSetup, xmlRRCConnectionSetup.c_str() );

  // Temporary hardcoding

  if ( rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->isActive_mac_MainConfig() )
  {
    if ( LteReferenceTestConditions->getDRX_support() == 0 )
      rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->setActive_drx_Config(false);
    if ( LteReferenceTestConditions->getPHR_support() == 0 )
      rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->setActive_phr_Config(false);
    if ( LteReferenceTestConditions->getTAT_support() == 0 )
      rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->timeAlignmentTimerDedicated()->set(7);
  }

  if ( rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->isActive_physicalConfigDedicated() )
  {
    if ( LteReferenceTestConditions->getTargetPowerSupport() == 0 )
    {
      if ( rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->isActive_uplinkPowerControlDedicated() )
        rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->uplinkPowerControlDedicated()->accumulationEnabled()->set(0);
    }

    if ( LteReferenceTestConditions->getSRS_support() == 0 )
      rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->soundingRS_UL_ConfigDedicated()->setActive_release();
    if ( LteReferenceTestConditions->getCQI_support() == 0 )
      rrcConnectionSetup.pdu->criticalExtensions()->c1()->rrcConnectionSetup_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->setActive_cqi_ReportConfig(false);
  }

  NEXT_STATE ( RRCConnectionRequest );
}

ON_TIMER( sM_LTE_RrcConnectionEstablishment, RRCConnectionRequest, RS_Default_Timer)
{
  PE_ERR("####### RRCConnectionRequest timeout #######" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_LTE_UL_CCCH_RRC_PDU( sM_LTE_RrcConnectionEstablishment, RRCConnectionRequest, RRCConnectionRequest )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  Rnti = asp->Rnti()->get();
  std::string tmp = cmw_Utils(" UE -> SS  RRC: RRCConnectionRequest (Cell #%i)", asp->RrcCellHandle()->get() );
  PE_PRINT(tmp.c_str());

  if ( xmlRRCConnectionRequest != "" )
  {
    if ( GCO_MATCHF(root, xmlRRCConnectionRequest.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  if ( CellHandle != -1 )
  {
    if ( asp->RrcCellHandle()->get() != CellHandle )
    {
      std::string tmp = cmw_Utils("####### Message received in wrong cell (%i), valid cell is %i #######", asp->RrcCellHandle()->get(), CellHandle );
      PE_ERR(tmp.c_str());
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  NEXT_STATE ( GetMacUeId );
}

ON_ASPX( sM_LTE_RrcConnectionEstablishment, GetMacUeId, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcUpdateUeConfigInd )
{
  globaldata_lte->getUEIdentity()->setUEHandle(asp->RrcUeHandle()->get());
  globaldata_lte->getUEIdentity()->setMacUeId(asp->MacUeId()->get());
  globaldata_lte->getUEIdentity()->setCRNTI(asp->Crnti()->get());

  UeId = asp->MacUeId()->get();

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
      std::string("Lte_CmacPowerControlConfigReq.xml"),  // CmacPowerControlConfigReq
      LteReferenceTestConditions->getTargetPowerPusch(), // TargetPowerPusch
      LteReferenceTestConditions->getTargetPowerPucch(), // TargetPowerPucch
      UeId                                               // UeId
      )
    );

    RUN_NEWP_STATEMACHINE ( sM_SystemTC, null, ( TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ));
  }

  if( EV_OK == retcode )
  {
    pCrrcAsp = NEW_ASP_WITH_DL_CCCH_RRC_PDU ( rrcConnectionSetup, asp->RrcUeHandle()->get(), Rnti );
    pCrrcAsp->Choice()->CrrcDlCcchMessageReq()->SupervisionDuration()->set(GEN_RRC_TIMER);

    GCO_DUMP(rrcConnectionSetup.pdu);

    OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp );

    PE_PRINT(" UE <- SS  RRC: RRCConnectionSetup " );
    PE_PRINT("           TC: Waiting for RRCConnectionSetupComplete" );

    NEXT_STATE( RRCConnectionSetupComplete );
  }
  else
  {
    PE_ERR("####### Timing advance setting failed #######");
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_ASPX( sM_LTE_RrcConnectionEstablishment, RRCConnectionSetupComplete, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_RRCCONNECTION_SETUP_COMPLETE_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("####### RRCConnectionSetupComplete timeout #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_LTE_UL_DCCH_RRC_PDU( sM_LTE_RrcConnectionEstablishment, RRCConnectionSetupComplete, RRCConnectionSetupComplete )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  globaldata_lte->getUEIdentity(0)->setUEHandle(asp->RrcUeHandle()->get());

  DELAY_ALL_EPS_NAS_PDU_TRANSITIONS();

  PE_PRINT(" UE -> SS  RRC: RRCConnectionSetupComplete " );

  if ( xmlRRCConnectionSetupComplete != "" )
  {
    if ( GCO_MATCHF(root, xmlRRCConnectionSetupComplete.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  PE_PRINT("           TC: RRC Connection Establishment procedure completed" );

  notifyCompletion(EV_OK);
}

void sM_LTE_RrcConnectionReject::start(void)
{
  sM_LteMlDefault::start();

  PE_PRINT("           TC: Waiting for RRCConnectionRequest");

  if ( WaitTimer == 0 )
  {
    rsDefaultTimer->set(WAIT_RRC_REQ);
  }
  else
  {
    rsDefaultTimer->set(WaitTimer);
  }

  NEW_LTE_RRC_PDU( rrcConnectionReject, DL_CCCH, RRCConnectionReject, xmlRRCConnectionReject.c_str() );
  NEXT_STATE ( RRCConnectionRequest );
}

ON_TIMER( sM_LTE_RrcConnectionReject, RRCConnectionRequest, RS_Default_Timer)
{
  PE_ERR("####### RRCConnectionRequest timeout #######" );
  NOTIFY_COMPLETION_FAILURE();
}

ON_LTE_UL_CCCH_RRC_PDU( sM_LTE_RrcConnectionReject, RRCConnectionRequest, RRCConnectionRequest )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  Rnti = asp->Rnti()->get();
  std::string tmp = cmw_Utils(" UE -> SS  RRC: RRCConnectionRequest (Cell #%i)", asp->RrcCellHandle()->get() );
  PE_PRINT(tmp.c_str());

  if ( xmlRRCConnectionRequest != "" )
  {
    if ( GCO_MATCHF(root, xmlRRCConnectionRequest.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  if ( asp->RrcCellHandle()->get() != CellHandle )
  {
    std::string tmp = cmw_Utils("####### Message received in wrong cell (%i), valid cell is %i #######", asp->RrcCellHandle()->get(), CellHandle );
    PE_ERR(tmp.c_str());
    NOTIFY_COMPLETION_FAILURE();
    retcode = EV_FAIL;
  }

  if ( retcode == EV_OK )
  {


    rrcConnectionReject.pdu->criticalExtensions()->c1()->rrcConnectionReject_r8()->waitTime()->set(WaitTime);

    pCrrcAsp = NEW_ASP_WITH_DL_CCCH_RRC_PDU ( rrcConnectionReject, asp->RrcUeHandle()->get(), Rnti );
    pCrrcAsp->Choice()->CrrcDlCcchMessageReq()->SupervisionDuration()->set(GEN_RRC_TIMER);

    GCO_DUMP(rrcConnectionReject.pdu);
    OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp );
    PE_PRINT(" UE <- SS  RRC: RRCConnectionReject " );

    delete &rrcConnectionReject;
  }

  notifyCompletion(retcode);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/