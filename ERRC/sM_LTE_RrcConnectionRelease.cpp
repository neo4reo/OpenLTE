/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_RrcConnectionRelease.cpp

Version:        1.0

Author:         Dennis M Senyonjo


Copyright (c) 2011. 


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    1.0         18.01.2011 / Dennis M Senyonjo     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_RrcConnectionRelease::start(void)
{
  sM_LteMlDefault::start();

  DECL_NEW_LTE_RRC_PDU( rrcConnectionRelease, DL_DCCH, RRCConnectionRelease, xmlRRCConnectionRelease.c_str() );

  if ( ReleaseCause != -1 )
  {
    rrcConnectionRelease.pdu->criticalExtensions()->c1()->rrcConnectionRelease_r8()->releaseCause()->set(ReleaseCause);
  }

  if ( RedirCarrierInfo_eutra != -1 )
  {
    rrcConnectionRelease.pdu->criticalExtensions()->c1()->rrcConnectionRelease_r8()->setActive_redirectedCarrierInfo(true);
    rrcConnectionRelease.pdu->criticalExtensions()->c1()->rrcConnectionRelease_r8()->redirectedCarrierInfo()->eutra()->set(RedirCarrierInfo_eutra);
  }

  OUTPUT( SAP_LTE_NET_CRRC, NEW_ASP_WITH_DL_DCCH_RRC_PDU( rrcConnectionRelease ) );

  GCO_DUMP(rrcConnectionRelease.pdu);

  delete &rrcConnectionRelease;

  PE_PRINT(" UE <- SS  RRC: RRCConnectionRelease" );

  NEXT_STATE ( CrrcInfoInd );
}

ON_ASPX( sM_LTE_RrcConnectionRelease, CrrcInfoInd, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcErrorInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_INFO_RRC_CONNECTION_RELEASED == asp->RrcResult()->get() )
  {
    PE_PRINT("           TC: RRC Connection Release procedure completed" );
    notifyCompletion(EV_OK);
  }
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/