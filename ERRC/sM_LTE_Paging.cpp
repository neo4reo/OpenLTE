/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_Paging.cpp

Version:        3.0

Author:          Dennis M Senyonjo


Copyright (c) 2011.  Dennis M Senyonjo


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    3.0         31.03.2011 / Dennis M S      Failure handling to 27.30.4 level
    0.2         24.11.2010 / Dennis M S      Support for failure
    0.1         18.01.2011 / Dennis M S     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_Paging::start(void)
{
  sM_LteMlDefault::start();

  if( globaldata_base->getUEIdentity(UEIdentity)->IsIMSIpresent() )
  {
    imsi_digits = globaldata_base->getUEIdentity(UEIdentity)->getIMSI();
  }
  else
  {
    imsi_digits = null;
  }

  imsiLength = strlen(imsi_digits);

  DECL_NEW_LTE_RRC_PDU( pagingmsg, PCCH, Paging, xmlPaging.c_str() );

  _CrrcAsp* pagingasp = NEW_ASP_WITH_PCCH_RRC_PDU( pagingmsg, globaldata_lte->getCellConfig(CellNumber)->getCellHandle());
  pagingasp->Choice()->CrrcPcchMessageReq()->RrcCellHandle()->set( globaldata_lte->getCellConfig(CellNumber)->getCellHandle());

  if( imsi_digits != null )
  {
    pagingasp->Choice()->CrrcPcchMessageReq()->UeSpecificInfoPresence()->set(MDDB::Lte::Rrc::Crrc::_UeSpecificInfoPresence::UE_SPECIFIC_INFO_PRESENT);
    pagingasp->Choice()->CrrcPcchMessageReq()->UeSpecificInformation()->setActive_UeSpecificInfo();
    pagingasp->Choice()->CrrcPcchMessageReq()->UeSpecificInformation()->UeSpecificInfo()->PageInfoUeSpecificIdle()->IMSI()->NumImsiDigits()->set(imsiLength);
    pagingasp->Choice()->CrrcPcchMessageReq()->UeSpecificInformation()->UeSpecificInfo()->PageInfoUeSpecificIdle()->IMSI()->Imsi()->setElementCount( pagingasp, imsiLength);

    for( int i=0; i<imsiLength; i++ )
    {
      pagingasp->Choice()->CrrcPcchMessageReq()->UeSpecificInformation()->UeSpecificInfo()->PageInfoUeSpecificIdle()->IMSI()->Imsi()->BcdDigit(i)->set((imsi_digits[i]-'0'));
      PRINTERR(("%d",(imsi_digits[i]-'0')));
    }
    PRINTERR(("\n"));
  }

  pagingasp->Choice()->CrrcPcchMessageReq()->UeSpecificInformation()->UeSpecificInfo()->PageInfoUeSpecificIdle()->PageRepetitions()->set(NUM_OF_PAGEING_REPTITIONS);
  pagingasp->Choice()->CrrcPcchMessageReq()->UeSpecificInformation()->UeSpecificInfo()->PageInfoUeSpecificIdle()->UeSpecificIdleDrxCycleLength()->set(DRX_CYCLE_LENGTH);
  pagingasp->Choice()->CrrcPcchMessageReq()->UeSpecificInformation()->UeSpecificInfo()->ResponseSupervisionDuration()->set(PAGE_RESPONSE_TIMER);

  OUTPUT( SAP_LTE_NET_CRRC, pagingasp );
  PE_PRINT(" UE <- SS  RRC: Paging" );

  GCO_DUMP(pagingasp);

  if (Failure == false )
    notifyCompletion(EV_OK);
  else
    NEXT_STATE ( ReplyState );
}

// Reporting the CRRC Error indications
ON_ASPX(sM_LTE_Paging, ReplyState, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc,_CrrcAsp, CrrcCellTimeoutInd)
{
    PE_PRINT("#######   CRRC_Timeout_Ind Occured #######   " );
    notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/