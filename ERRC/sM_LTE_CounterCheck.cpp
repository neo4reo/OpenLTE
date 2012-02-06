/*
 Dennis M Senyonjo
EUTRAN L2 SW















                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_CounterCheck.cpp

Version:        2.0

Author:          Dennis M Senyonjo


Copyright (c) 2011.  Dennis M Senyonjo.


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    2.0         15.02.2011 / Dennis M S  Updated Counter Check response message handling
    0.1         18.01.2011 / Dennis M S     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_CounterCheck::start(void)
{
  sM_LteMlDefault::start();

  /******************************************************************************/
  /* SS->UE  RRC: CounterCheck                                                  */
  /******************************************************************************/

  DECL_NEW_LTE_RRC_PDU( counterCheck, DL_DCCH, CounterCheck, "Lte_CounterCheck.xml" );

  CodecContext *cc = counterCheck.root->code();

  if( element_count_req != 0 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->setElementCount(cc, element_count_req);
  }

  if ( drb_1_req != -1 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(0)->drb_Identity()->set(cc, drb_1_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(0)->countMSB_Uplink()->set(cc, count_UL_1_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(0)->countMSB_Downlink()->set(cc, count_DL_1_req);
  }

  if ( drb_2_req != -1 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(1)->drb_Identity()->set(cc, drb_2_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(1)->countMSB_Uplink()->set(cc, count_UL_2_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(1)->countMSB_Downlink()->set(cc, count_DL_2_req);
  }

  if ( drb_3_req != -1 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(2)->drb_Identity()->set(cc, drb_3_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(2)->countMSB_Uplink()->set(cc, count_UL_3_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(2)->countMSB_Downlink()->set(cc, count_DL_3_req);
  }

  if ( drb_4_req != -1 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(3)->drb_Identity()->set(cc, drb_4_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(3)->countMSB_Uplink()->set(cc, count_UL_4_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(3)->countMSB_Downlink()->set(cc, count_DL_4_req);
  }

  if ( drb_5_req != -1 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(4)->drb_Identity()->set(cc, drb_5_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(4)->countMSB_Uplink()->set(cc, count_UL_5_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(4)->countMSB_Downlink()->set(cc, count_DL_5_req);
  }

  if ( drb_6_req != -1 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(5)->drb_Identity()->set(cc, drb_6_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(5)->countMSB_Uplink()->set(cc, count_UL_6_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(5)->countMSB_Downlink()->set(cc, count_DL_6_req);
  }

  if ( drb_7_req != -1 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(6)->drb_Identity()->set(cc, drb_7_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(6)->countMSB_Uplink()->set(cc, count_UL_7_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(6)->countMSB_Downlink()->set(cc, count_DL_7_req);
  }

  if ( drb_8_req != -1 )
  {
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(7)->drb_Identity()->set(cc, drb_8_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(7)->countMSB_Uplink()->set(cc, count_UL_8_req);
    counterCheck.pdu->criticalExtensions()->c1()->counterCheck_r8()->drb_CountMSB_InfoList()->value(7)->countMSB_Downlink()->set(cc, count_DL_8_req);
  }

  _CrrcAsp* pCrrcAsp = NEW_ASP_WITH_DL_DCCH_RRC_PDU( counterCheck, globaldata_lte->getUEIdentity(0)->getUEHandle() );
  pCrrcAsp->Choice()->CrrcDlDcchMessageReq()->SupervisionDuration()->set(WAIT_RRC_RECONF);

  OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp);

  GCO_DUMP(counterCheck.pdu);

  delete &counterCheck;

  PE_PRINT(" SS -> UE  RRC: CounterCheck " );

  PE_PRINT("           TC: Waiting for CounterCheckResponse " );

  NEXT_STATE ( CounterCheckResponse );
}

/******************************************************************************/
/* UE->SS  RRC: CounterCheckResponse                                          */
/******************************************************************************/

ON_ASPX( sM_LTE_CounterCheck, CounterCheckResponse, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_RRCCONNECTION_RECONFIGURATION_COMPLETE_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("####### Counter Check Response timeout #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_LTE_UL_DCCH_RRC_PDU( sM_LTE_CounterCheck, CounterCheckResponse, CounterCheckResponse )
{
  PE_PRINT(" UE -> SS  RRC: CounterCheckResponse " );

  GCO_DUMP(pdu);

  if ( element_count_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->getElementCount() )
  {
    PE_ERR("####### Missmatch in the element count #######");
    NOTIFY_COMPLETION_FAILURE();
    retcode = EV_FAIL;
  }

  if ( drb_1_resp != -1 )
  {
    if ( ( drb_1_resp      != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(0)->drb_Identity()->get() ) ||
         ( count_UL_1_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(0)->count_Uplink()->get() ) ||
         ( count_DL_1_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(0)->count_Downlink()->get() )
    )
    {
      PE_ERR("####### Missmatch in the 1st IE #######" );
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  if ( drb_2_resp != -1 )
  {
    if ( ( drb_2_resp      != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(1)->drb_Identity()->get() ) ||
         ( count_UL_2_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(1)->count_Uplink()->get() ) ||
         ( count_DL_2_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(1)->count_Downlink()->get() )
    )
    {
      PE_ERR("####### Missmatch in the 2nd IE #######" );
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  if ( drb_3_resp != -1 )
  {
    if ( ( drb_3_resp      != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(2)->drb_Identity()->get() ) ||
         ( count_UL_3_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(2)->count_Uplink()->get() ) ||
         ( count_DL_3_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(2)->count_Downlink()->get() )
    )
    {
      PE_ERR("####### Missmatch in the 3rd IE #######" );
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  if ( drb_4_resp != -1 )
  {
    if ( ( drb_4_resp      != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(3)->drb_Identity()->get() ) ||
         ( count_UL_4_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(3)->count_Uplink()->get() ) ||
         ( count_DL_4_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(3)->count_Downlink()->get() )
    )
    {
      PE_ERR("####### Missmatch in the 4th IE #######" );
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  if ( drb_5_resp != -1 )
  {
    if ( ( drb_5_resp      != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(4)->drb_Identity()->get() ) ||
         ( count_UL_5_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(4)->count_Uplink()->get() ) ||
         ( count_DL_5_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(4)->count_Downlink()->get() )
    )
    {
      PE_ERR("####### Missmatch in the 5th IE #######" );
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  if ( drb_6_resp != -1 )
  {
    if ( ( drb_6_resp      != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(5)->drb_Identity()->get() ) ||
         ( count_UL_6_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(5)->count_Uplink()->get() ) ||
         ( count_DL_6_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(5)->count_Downlink()->get() )
    )
    {
      PE_ERR("####### Missmatch in the 6th IE #######" );
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  if ( drb_7_resp != -1 )
  {
    if ( ( drb_7_resp      != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(6)->drb_Identity()->get() ) ||
         ( count_UL_7_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(6)->count_Uplink()->get() ) ||
         ( count_DL_7_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(6)->count_Downlink()->get() )
    )
    {
      PE_ERR("####### Missmatch in the 7th IE #######" );
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  if ( drb_8_resp != -1 )
  {
    if ( ( drb_8_resp      != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(7)->drb_Identity()->get() ) ||
         ( count_UL_8_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(7)->count_Uplink()->get() ) ||
         ( count_DL_8_resp != pdu->criticalExtensions()->counterCheckResponse_r8()->drb_CountInfoList()->DRB_CountInfo(7)->count_Downlink()->get() )
    )
    {
      PE_ERR("####### Missmatch in the 8th IE #######" );
      NOTIFY_COMPLETION_FAILURE();
      retcode = EV_FAIL;
    }
  }

  PE_PRINT("           TC: Counter Check procedure completed" );

  notifyCompletion(retcode);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/