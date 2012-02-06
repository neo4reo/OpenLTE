/*

EUTRAN L2 SW















                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_RrcConnectionReconfiguration_meas.cpp

Version:        1.2

Author:          Dennis M Senyonjo


Copyright (c) 2011.  Dennis M Senyonjo


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    2.0         08.04.2011 / Niko Tumelius  Update
    1.2         05.04.2011 / Dennis M S     Update
    1.1         04.03.2011 / Dennis M S     Update
    1.0         18.01.2011 / Dennis M S     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_RrcConnectionReconfiguration_meas::start(void)
{
  sM_LteMlDefault::start();

  /******************************************************************************/
  /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
  /******************************************************************************/

  DECL_NEW_LTE_RRC_PDU( rrcConnectionReconfiguration, DL_DCCH, RRCConnectionReconfiguration, xmlRRCConnectionReconfiguration.c_str() );

  CodecContext *cc = rrcConnectionReconfiguration.root->code();

  // Serving cell
  rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->measObjectToAddModList()->value(0)->measObject()->measObjectEUTRA()->carrierFreq()->set(cc, CarrierFreq);
  rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->measObjectToAddModList()->value(0)->measObject()->measObjectEUTRA()->allowedMeasBandwidth()->set(cc, AllowedMeasBandwidth);

  // EventA1
  if ( rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->isActive_eventA1() )
  {
    if ( RsrpOrRsrq == false )
    {
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->eventA1()->a1_Threshold()->setActive_threshold_RSRP();
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->eventA1()->a1_Threshold()->threshold_RSRP()->set(cc, Threshold);
    }
    else
    {
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->eventA1()->a1_Threshold()->setActive_threshold_RSRQ();
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->eventA1()->a1_Threshold()->threshold_RSRQ()->set(cc, Threshold);
    }
  }

  // EventA2
  if ( rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->isActive_eventA2() )
  {
    if ( RsrpOrRsrq == false )
    {
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->eventA2()->a2_Threshold()->setActive_threshold_RSRP();
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->eventA2()->a2_Threshold()->threshold_RSRP()->set(cc, Threshold);
    }
    else
    {
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->eventA2()->a2_Threshold()->setActive_threshold_RSRQ();
      rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->eventId()->eventA2()->a2_Threshold()->threshold_RSRQ()->set(cc, Threshold);
    }
  }

  rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->reportConfigToAddModList()->value(0)->reportConfig()->reportConfigEUTRA()->triggerType()->event()->hysteresis()->set(cc, Hysteresis);

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

ON_ASPX( sM_LTE_RrcConnectionReconfiguration_meas, RRCConnectionReconfigurationComplete, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_RRCCONNECTION_RECONFIGURATION_COMPLETE_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("####### RRC Connection Reconfiguration Complete timeout #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_LTE_UL_DCCH_RRC_PDU( sM_LTE_RrcConnectionReconfiguration_meas, RRCConnectionReconfigurationComplete, RRCConnectionReconfigurationComplete )
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

/* PUBLIC *********************************************************************/

void sM_LTE_RrcConnectionReconfiguration_meas_xml::start(void)
{
  sM_LteMlDefault::start();

  /******************************************************************************/
  /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
  /******************************************************************************/

  DECL_NEW_LTE_RRC_PDU( rrcConnectionReconfiguration, DL_DCCH, RRCConnectionReconfiguration, xmlRRCConnectionReconfiguration.c_str() );

  CodecContext *cc = rrcConnectionReconfiguration.root->code();

  // Serving cell and intra-freq neigbours
  rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->measObjectToAddModList()->value(0)->measObject()->measObjectEUTRA()->carrierFreq()->set(cc, CarrierFreq_cell0);
  rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->measObjectToAddModList()->value(0)->measObject()->measObjectEUTRA()->allowedMeasBandwidth()->set(cc, AllowedMeasBandwidth_cell0);

  if ( MeasType == 1 )
  {
    // Inter-freq neigbour
    rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->measObjectToAddModList()->value(1)->measObject()->measObjectEUTRA()->carrierFreq()->set(cc, CarrierFreq_cell1);
    rrcConnectionReconfiguration.pdu->criticalExtensions()->c1()->rrcConnectionReconfiguration_r8()->measConfig()->measObjectToAddModList()->value(1)->measObject()->measObjectEUTRA()->allowedMeasBandwidth()->set(cc, AllowedMeasBandwidth_cell1);
  }

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

ON_ASPX( sM_LTE_RrcConnectionReconfiguration_meas_xml, RRCConnectionReconfigurationComplete, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_RRCCONNECTION_RECONFIGURATION_COMPLETE_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("####### RRC Connection Reconfiguration Complete timeout #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_LTE_UL_DCCH_RRC_PDU( sM_LTE_RrcConnectionReconfiguration_meas_xml, RRCConnectionReconfigurationComplete, RRCConnectionReconfigurationComplete )
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