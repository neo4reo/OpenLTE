/*

EUTRAN L2 SW















                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_RrcConnectionReconfiguration_ho.cpp

Version:        1.0

Author:          Dennis M Senyonjo


Copyright (c) 2011. 


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    0.1         25.03.2011 / Dennis M S     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_RrcConnectionReconfiguration_ho::start(void)
{
  sM_LteMlDefault::start();

  /******************************************************************************/
  /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
  /******************************************************************************/

  DECL_NEW_LTE_RRC_PDU( rrcConnectionReconfiguration, DL_DCCH, RRCConnectionReconfiguration, xmlRRCConnectionReconfiguration.c_str() );

  CodecContext *cc = rrcConnectionReconfiguration.root->code();
  Lte::Std::_RRCConnectionReconfiguration_r8_IEs * rrcConnectionReconf = rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8();

  rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->mobilityControlInfo()->targetPhysCellId()->set(globaldata_lte->getCellConfig(cellHandleTarget)->getPhyCellId());

  rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->mobilityControlInfo()->newUE_Identity()->set(cc, newUE_Identity );

  // Temporary hardcoding
  if ( rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->isActive_mac_MainConfig() )
  {
    if ( LteReferenceTestConditions->getDRX_support() == 0 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->setActive_drx_Config(false);
    if ( LteReferenceTestConditions->getPHR_support() == 0 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->setActive_phr_Config(false);
    if ( LteReferenceTestConditions->getTAT_support() == 0 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->mac_MainConfig()->explicitValue()->timeAlignmentTimerDedicated()->set(7);
  }

  if ( rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->isActive_physicalConfigDedicated() )
  {
    if ( LteReferenceTestConditions->getTargetPowerSupport() == 0 )
    {
      if ( rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->isActive_uplinkPowerControlDedicated() )
        rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->uplinkPowerControlDedicated()->accumulationEnabled()->set(0);
    }
    if ( LteReferenceTestConditions->getSRS_support() == 0 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->setActive_soundingRS_UL_ConfigDedicated(false);
    if ( LteReferenceTestConditions->getCQI_support() == 0 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->radioResourceConfigDedicated()->physicalConfigDedicated()->setActive_cqi_ReportConfig(false);
  }

  if ( ( dl_CarrierFreq != -1 ) || ( ul_CarrierFreq != -1 ) )
  {
    rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->mobilityControlInfo()->setActive_carrierFreq(true);

    if ( dl_CarrierFreq != -1 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->mobilityControlInfo()->carrierFreq()->dl_CarrierFreq()->set(dl_CarrierFreq);

    if ( ul_CarrierFreq != -1 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->mobilityControlInfo()->carrierFreq()->ul_CarrierFreq()->set(ul_CarrierFreq);
  }

  if ( ( dl_Bandwidth != -1 ) || ( ul_Bandwidth != -1 ) )
  {
    rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->mobilityControlInfo()->setActive_carrierBandwidth(true);

    if ( dl_Bandwidth != -1 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->mobilityControlInfo()->carrierBandwidth()->dl_Bandwidth()->set(dl_Bandwidth);

    if ( ul_Bandwidth != -1 )
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->mobilityControlInfo()->carrierBandwidth()->ul_Bandwidth()->set(ul_Bandwidth);
  }

  globaldata_lte->getUEIdentity(0)->setncc( rrcConnectionReconf->securityConfigHO()->handoverType()->intraLTE()->nextHopChainingCount()->get() );

  PE_PRINT("           TC: Security input parameters");
  PE_PRINT("           TC: PhyCellId = " << globaldata_lte->getCellConfig(cellHandleTarget)->getPhyCellId() );
  PE_PRINT("           TC: DLEARFCN = " << globaldata_lte->getCellConfig(cellHandleTarget)->getDLEARFCN() );
  PE_PRINT("           TC: NCC = " << globaldata_lte->getUEIdentity(0)->getncc() );

  RUN_NEWP_STATEMACHINE( sM_LteMl_RRC_SecurityConfig, null,
    (
    0,
    true,
    cellHandleTarget
    )
  );

  _CrrcAsp* pCrrcAsp = NEW_ASP_WITH_DL_DCCH_RRC_PDU( rrcConnectionReconfiguration, globaldata_lte->getUEIdentity(0)->getUEHandle() );
  pCrrcAsp->Choice()->CrrcDlDcchMessageReq()->SupervisionDuration()->set(WAIT_RRC_RECONF);

  OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp);

  GCO_DUMP(rrcConnectionReconfiguration.pdu);

  delete &rrcConnectionReconfiguration;

  PE_PRINT(" SS -> UE  RRC: RRCConnectionReconfiguration " );

  PE_PRINT("           TC: Waiting for RRCConnectionReconfigurationComplete" );

  NEXT_STATE ( RRCConnectionReconfigurationComplete );
}

/******************************************************************************/
/* UE->SS  RRC: RRCConnectionReconfigurationComplete                          */
/******************************************************************************/

ON_ASPX( sM_LTE_RrcConnectionReconfiguration_ho, RRCConnectionReconfigurationComplete, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_RRCCONNECTION_RECONFIGURATION_COMPLETE_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("####### RRC Connection Reconfiguration Complete timeout #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_LTE_UL_DCCH_RRC_PDU( sM_LTE_RrcConnectionReconfiguration_ho, RRCConnectionReconfigurationComplete, RRCConnectionReconfigurationComplete )
{
  PE_PRINT(" UE -> SS  RRC: RRCConnectionReconfigurationComplete " );

  GCO_DUMP(pdu);

  if ( xmlRRCConnectionReconfigurationComplete != "" )
  {
    if ( GCO_MATCHF(root, xmlRRCConnectionReconfigurationComplete.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
  }

  PE_PRINT("           TC: RRC Connection Reconfigure procedure completed" );

  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/