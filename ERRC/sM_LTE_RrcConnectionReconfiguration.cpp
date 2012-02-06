/*

EUTRAN L2 SW















                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_RrcConnectionReconfiguration.cpp

Version:        3.1

Author:          Dennis M Senyonjo


Copyright (c) 2011  Dennis M Senyonjo


Change history:

    VERSION     DATE/AUTHOR      COMMENT
    3.1         25.03.2011 /     Compined attach support: change compined attach type from request
    3.0         17.03.2011 /     NAS delay addition
    2.0         21.02.2011 /     This SM can be also used when activate default EPS bearer context
    1.0         18.01.2011 /Dennis M S     Original

*/

#define FEA_REPLACE_ATTACH_RESULT_FROM_ATTACH_TYPE
#define FEA_IPv4v6_SUPPORT //also IPv6

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_RrcConnectionReconfiguration::start(void)
{
  sM_LteMlDefault::start();

  DELAY_ALL_EPS_NAS_PDU_TRANSITIONS();

  if ( DedicatedContext == false )
  {
    PRINTERR(("DedicatedContext =  %i\n", DedicatedContext ));

    /******************************************************************************/
    /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
    /*         NAS: ATTACH ACCEPT                                                 */
    /*         NAS: ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST                   */
    /******************************************************************************/

    if ( (xmlNASAttachAccept != "") && (xmlNASActivateDefaultEpsBearerContextRequest != "") )
    {
      PRINTERR(("DBG: ACTIVATE DEFAULT EPS BEARER CONTEXT\n"));
      DECL_NEW_EPS_NAS_PDU( attachAccept, PdEpsMobilityManagement, EmmMsgAttachAccept, xmlNASAttachAccept.c_str() );
      DECL_NEW_EPS_NAS_PDU( actDefaultEpsBearerContextReq, PdEpsSessionManagement, EsmActivateDefaultEpsBearerContextRequest, xmlNASActivateDefaultEpsBearerContextRequest.c_str() );

      actDefaultEpsBearerContextReq.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set(globaldata_lte->getPTI());

      //Retrieve the default EPS bearer ID from the xml message, which here after is called as LinkedEPSBearer Id
      int LinkedEPSBearerId = actDefaultEpsBearerContextReq.root->Choice()->PdEpsSessionManagement()->EpsBearerIdentity()->get();
      globaldata_lte->getUEIdentity(UEid)->setLinkedEPSBearerId(LinkedEPSBearerId);

      PRINTERR( ("sM_UpcReserveIpConfig with LinkedEPSBearerId: %i" , LinkedEPSBearerId) );
      RUN_NEWP_STATEMACHINE( sM_UpcReserveIpConfig, null, (LinkedEPSBearerId));
      retcode=MLAPI::lastRunReturnCode;

//#ifdef FEA_REPLACE_ATTACH_RESULT_FROM_ATTACH_TYPE
      //Get ATTACH type from ESM PDN Connectivity Request message
      int AttachType = LteReferenceTestConditions->getAttachRequest()->EpsAttachTypeV()->AttachType()->get();
      PRINTERR(( "Attach result to %i", AttachType ));
      attachAccept.pdu->EpsAttachResultV()->AttachResult()->set(AttachType);
//#endif

      //Store IPConfigID (linked to DefaultEPSBearerID) as default ID in the UE Context
      int ipconfigid = globaldata_base->getUEIdentity(UEid)->getIPConfigID(LinkedEPSBearerId);
      PRINTERR(("\n Ipconfigid is ::%d\n", ipconfigid));
      globaldata_base->getUEIdentity(UEid)->setDefaultIPConfigID(ipconfigid);

      //Set the IPaddress parameters in the message
      int IPver = globaldata_base->getUEIdentity(UEid)->getIPVersion(ipconfigid);
      PRINTERR(("\n IP Version ::%d\n", IPver));

      //set the IP version in Default EPS Bearer msg
      actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->PDNTypeV()->set(IPver);
      if(IPver == MDDB::Eps::Nas::IPV4)
      {
        std::string ipv4address = globaldata_base->getUEIdentity(UEid)->getIPv4Address(ipconfigid);
        actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->setActive_IPv4AddressInfo();
        unsigned char* address = new unsigned char[4];
        //convert IP addresses from string to binary form
        globaldata_base->inet_pton4(ipv4address.c_str(),address);

        PRINTERR(("\n IPv4 address is::"));
        for(int i=0;i<4;i++)
          PRINTERR((" %02x",address[i]));
        PRINTERR(("\n"));

        actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->IPv4AddressInfo()->IPv4Address()->setString(actDefaultEpsBearerContextReq.root,address,4);

        unsigned char* decimal = (unsigned char*) actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->IPv4AddressInfo()->IPv4Address()->getData();
        char ucharIPv6[999];
        PRINTERR(("\n Stored IPv4 address = %d.%d.%d.%d \n",decimal[0],decimal[1],decimal[2],decimal[3] ));
        delete[] address;
      }
#ifdef FEA_IPv4v6_SUPPORT
      else if(IPver == MDDB::Eps::Nas::IPV6)
    {
        ipv6address = globaldata_base->getUEIdentity(UEid)->getIPv6Address(ipconfigid);
        //activate IPv6 version in the DefaultEPSBearer msg
        actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->setActive_IPv6AddressInfo();
        //in6_addr to hold a single IPv6 address
        in6_addr address6;
        //convert IP addresses from string to binary form
        globaldata_base->inet_pton6(ipv6address.c_str(),&address6);
        //number of bytes required to store IPv6 address
        NumOfBytes = ((sizeof(address6))/2);
        actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->IPv6AddressInfo()->IPv6Address()->setString(actDefaultEpsBearerContextReq.root,address6.s6_addr + NumOfBytes, NumOfBytes);

        unsigned char* decimal = (unsigned char*) actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->IPv6AddressInfo()->IPv6Address()->getData();
        char ucharIPv6[999];
        sprintf(ucharIPv6, "\n Stored IPv6 address = %02x%02x:%02x%02x:%02x%02x:%02x%02x \n",decimal[0],decimal[1],decimal[2],decimal[3],decimal[4],decimal[5],decimal[6],decimal[7]);
        PRINTERR((ucharIPv6));
    }
    else if(IPver == MDDB::Eps::Nas::IPV4_6)
    {
        ipv4address = globaldata_base->getUEIdentity(UEid)->getIPv4Address(ipconfigid);
        ipv6address = globaldata_base->getUEIdentity(UEid)->getIPv6Address(ipconfigid);

        //activate IPv4_6 version in the DefaultEPSBearer msg
        actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->setActive_IPv6andIPv4AddressInfo();

        unsigned char* address4 = new unsigned char[4];
        //convert IP addresses from string to binary form
        globaldata_base->inet_pton4(ipv4address.c_str(),address4);
        PRINTERR(("\n IPv4 address is::"));
        for(int i=0;i<4;i++)
        {
            PRINTERR((" %02x ",address4[i]));
        }
        actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->IPv6andIPv4AddressInfo()->IPv4Address()->setString(actDefaultEpsBearerContextReq.root,address4,4);

        unsigned char* decimalIPv4 = (unsigned char*) actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->IPv4AddressInfo()->IPv4Address()->getData(); 
        char ucharIPv6[999];
        PRINTERR(("\n Stored IPv4 address in IPV4_6 option is  = %d.%d.%d.%d \n",decimalIPv4[0],decimalIPv4[1],decimalIPv4[2],decimalIPv4[3] ));
        delete[] address4;



        //in6_addr to hold a single IPv6 address
        in6_addr address6;
        //convert IP addresses from string to binary form
        globaldata_base->inet_pton6(ipv6address.c_str(),&address6);
        //number of bytes required to store IPv6 address
        NumOfBytes = ((sizeof(address6))/2);
        actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->IPv6andIPv4AddressInfo()->IPv6Address()->setString(actDefaultEpsBearerContextReq.root,address6.s6_addr + NumOfBytes, NumOfBytes);

        unsigned char* decimal = (unsigned char*) actDefaultEpsBearerContextReq.pdu->PDNAddressLV()->Choice()->IPv6AddressInfo()->IPv6Address()->getData();
        sprintf(ucharIPv6, "\n Stored IPv6 address = %02x%02x:%02x%02x:%02x%02x:%02x%02x \n",decimal[0],decimal[1],decimal[2],decimal[3],decimal[4],decimal[5],decimal[6],decimal[7]);
        PRINTERR((ucharIPv6));
    }
#endif
      else
      {
        PRINTERR(("\n Unsupported ProtocolVersion \n" ));
      }

      // Add concatenated NAS PDU (EsmActivateDefaultEPSBearerContextRequest)
      CONCATENATE_NAS_PDU( attachAccept.root, actDefaultEpsBearerContextReq.root );

      //Activating UPC
      if (DataGenerator == false)
      {
        RUN_NEWP_STATEMACHINE( sM_UpcActivateIpConfig, null, (LinkedEPSBearerId) );
        RUN_NEWP_STATEMACHINE( sM_SystemTC, null, (TC_PRELIMINARY_VERDICT, retcode=MLAPI::lastRunReturnCode ) );
      }
      DECL_NEW_LTE_RRC_PDU( rrcConnectionReconfiguration, DL_DCCH, RRCConnectionReconfiguration, xmlRRCConnectionReconfiguration.c_str() );

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

      FILL_RRC_WITH_NAS_PDU( rrcConnectionReconfiguration.root, attachAccept.root);

      _CrrcAsp* pCrrcAsp = NEW_ASP_WITH_DL_DCCH_RRC_PDU( rrcConnectionReconfiguration, globaldata_lte->getUEIdentity(UEid)->getUEHandle() );
      pCrrcAsp->Choice()->CrrcDlDcchMessageReq()->SupervisionDuration()->set(WAIT_RRC_RECONF);

      OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp);

      GCO_DUMP(rrcConnectionReconfiguration.pdu);

      delete &rrcConnectionReconfiguration;
    }
    else if( AdditionalDefaultContext && (xmlNASActivateDedicatedEpsBearerContextRequest != "") && (xmlRRCConnectionReconfiguration!="") )
    {
      /******************************************************************************/
      /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
      /*         NAS: ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST                 */
      /******************************************************************************/

      PRINTERR(("DBG: ACTIVATE SECOND DEFAULT EPS BEARER CONTEXT\n"));

      DECL_NEW_EPS_NAS_PDU( actDefaultEpsBearerContextReq, PdEpsSessionManagement, EsmActivateDefaultEpsBearerContextRequest, xmlNASActivateDedicatedEpsBearerContextRequest.c_str() );

      actDefaultEpsBearerContextReq.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set(globaldata_lte->getPTI());

      //extract the EPS Bearer Identity from actDefaultEpsBearerContextReq
      int EPSBearerId = actDefaultEpsBearerContextReq.root->Choice()->PdEpsSessionManagement()->EpsBearerIdentity()->get();
      PRINTERR(("\n DedicatedEpsBearerContextReq: EPSBearerId = %d\n", EPSBearerId));

      DECL_NEW_LTE_RRC_PDU( rrcConnectionReconfiguration, DL_DCCH, RRCConnectionReconfiguration, xmlRRCConnectionReconfiguration.c_str() );

      FILL_RRC_WITH_NAS_PDU( rrcConnectionReconfiguration.root, actDefaultEpsBearerContextReq.root);

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

      _CrrcAsp* pCrrcAsp = NEW_ASP_WITH_DL_DCCH_RRC_PDU( rrcConnectionReconfiguration, globaldata_lte->getUEIdentity(UEid)->getUEHandle() );
      pCrrcAsp->Choice()->CrrcDlDcchMessageReq()->SupervisionDuration()->set(WAIT_RRC_RECONF);

      PRINTERR(("DL COUNT: %d\n",StateMachineLTE::getNasCount(false,UEid)));
      PRINTERR(("UL COUNT: %d\n",StateMachineLTE::getNasCount(true,UEid)));

      OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp);

      GCO_DUMP(rrcConnectionReconfiguration.pdu);

      delete &rrcConnectionReconfiguration;
    }
    else
    {
      /******************************************************************************/
      /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
      /******************************************************************************/

      PRINTERR(("DBG: RRCConnectionReconfiguration\n"));

      DECL_NEW_LTE_RRC_PDU( rrcConnectionReconfiguration, DL_DCCH, RRCConnectionReconfiguration, xmlRRCConnectionReconfiguration.c_str() );

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

      _CrrcAsp* pCrrcAsp = NEW_ASP_WITH_DL_DCCH_RRC_PDU( rrcConnectionReconfiguration, globaldata_lte->getUEIdentity(UEid)->getUEHandle() );
      pCrrcAsp->Choice()->CrrcDlDcchMessageReq()->SupervisionDuration()->set(WAIT_RRC_RECONF);

      PRINTERR(("DL COUNT: %d\n",StateMachineLTE::getNasCount(false,UEid)));
      PRINTERR(("UL COUNT: %d\n",StateMachineLTE::getNasCount(true,UEid)));

      OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp);

      GCO_DUMP(rrcConnectionReconfiguration.pdu);

      delete &rrcConnectionReconfiguration;
    }
  }
  else
  {
    /******************************************************************************/
    /* SS->UE  RRC: RRCConnectionReconfiguration                                  */
    /*         NAS: ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST                 */
    /******************************************************************************/

    PRINTERR(("DBG: ACTIVATE DEDICATED EPS BEARER CONTEXT\n"));

    DECL_NEW_EPS_NAS_PDU( actDedicatedEpsBearerContextReq, PdEpsSessionManagement, EsmActivateDedicatedEpsBearerContextRequest, xmlNASActivateDedicatedEpsBearerContextRequest.c_str() );

    actDedicatedEpsBearerContextReq.root->Choice()->PdEpsSessionManagement()->Header()->Default()->EsmProcedureTransactionIdentity()->set(globaldata_lte->getPTI());
    actDedicatedEpsBearerContextReq.pdu->LinkedEpsBearerIdentity()->set(globaldata_lte->getUEIdentity(UEid)->getLinkedEPSBearerId());

    //extract the EPS Bearer Identity from actDedicatedEpsBearerContextReq
    int EPSBearerId = actDedicatedEpsBearerContextReq.root->Choice()->PdEpsSessionManagement()->EpsBearerIdentity()->get();
    PRINTERR(("\n DedicatedEpsBearerContextReq: EPSBearerId = %d\n", EPSBearerId));

    DECL_NEW_LTE_RRC_PDU( rrcConnectionReconfiguration, DL_DCCH, RRCConnectionReconfiguration, xmlRRCConnectionReconfiguration.c_str() );

    FILL_RRC_WITH_NAS_PDU( rrcConnectionReconfiguration.root, actDedicatedEpsBearerContextReq.root);

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

    _CrrcAsp* pCrrcAsp = NEW_ASP_WITH_DL_DCCH_RRC_PDU( rrcConnectionReconfiguration, globaldata_lte->getUEIdentity(UEid)->getUEHandle() );
    pCrrcAsp->Choice()->CrrcDlDcchMessageReq()->SupervisionDuration()->set(WAIT_RRC_RECONF);

    PRINTERR(("DL COUNT: %d\n",StateMachineLTE::getNasCount(false,UEid)));
    PRINTERR(("UL COUNT: %d\n",StateMachineLTE::getNasCount(true,UEid)));

    OUTPUT( SAP_LTE_NET_CRRC, pCrrcAsp);

    GCO_DUMP(rrcConnectionReconfiguration.pdu);

    delete &rrcConnectionReconfiguration;
  }

  PE_PRINT(" UE <- SS  RRC: RRCConnectionReconfiguration " );

  if ( DedicatedContext == false )
  {
    if ( (xmlNASAttachAccept != "") && (xmlNASActivateDefaultEpsBearerContextRequest != "") )
    {
      PE_PRINT("           NAS: ATTACH ACCEPT " );
      PE_PRINT("           NAS: ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST " );
    }
  }
  else
  {
    PE_PRINT("           NAS: ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST " );
  }

  PE_PRINT("           TC: Waiting for RRCConnectionReconfigurationComplete" );

  NEXT_STATE ( RRCConnectionReconfigurationComplete );
}

/******************************************************************************/
/* UE->SS  RRC: RRCConnectionReconfigurationComplete                          */
/******************************************************************************/

ON_ASPX( sM_LTE_RrcConnectionReconfiguration, RRCConnectionReconfigurationComplete, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  int UeHandle = asp->RrcUeHandle()->get();
  int resultCode = asp->RrcResult()->get();
  std::ostringstream ossInfo;

  // Abnormal exit: RRC Connection Reconfiguration Complete Timeout
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_RRCCONNECTION_RECONFIGURATION_COMPLETE_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("#######   RRC Connection Reconfiguration Complete Timeout #######   ");
    NOTIFY_COMPLETION_FAILURE();
  }
  else
  {
    GCO gco( asp->RrcResult(), &_RrcResult::gcocd );
    const char* text = gco.getLabelForValue(asp->RrcResult()->get());
    ossInfo << "#######   CrrcTimeoutInd Occurred with UE Handle :: " << UeHandle<<""<< text <<" #######   " <<std::endl;
    PE_ERR(ossInfo.str());
    NOTIFY_COMPLETION_FAILURE();
  }
}

ON_LTE_UL_DCCH_RRC_PDU( sM_LTE_RrcConnectionReconfiguration, RRCConnectionReconfigurationComplete, RRCConnectionReconfigurationComplete )
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
