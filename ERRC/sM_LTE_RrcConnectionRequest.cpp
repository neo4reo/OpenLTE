/*
EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_RrcConnectionRequest.cpp

Version:        1.0

Author:          Dennis M Senyonjo


Copyright (c) 2011. 


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    1.0         18.01.2011 / Dennis M S     Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <sstream>

/* PUBLIC *********************************************************************/

void sM_LTE_RrcConnectionRequest::start(void)
{
  sM_LteMlDefault::start();

  PE_PRINT("           TC: Waiting for RRCConnectionRequest" );

  if ( WaitTimer == 0 )
  {
    rsDefaultTimer->set(WAIT_RRC_REQ);
  }
  else
  {
    rsDefaultTimer->set(WaitTimer);
  }

  NEXT_STATE ( RRCConnectionRequest );
}

ON_TIMER( sM_LTE_RrcConnectionRequest, RRCConnectionRequest, RS_Default_Timer)
{
  if ( Desired == true )
  {
    PE_ERR("####### RRCConnectionRequest timeout #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
  else
  {
    PE_PRINT("         TC: RRCConnectionRequest timeout" );
    notifyCompletion(EV_OK);
  }
}

ON_LTE_UL_CCCH_RRC_PDU( sM_LTE_RrcConnectionRequest, RRCConnectionRequest, RRCConnectionRequest )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  if ( Desired == true )
  {
    PE_PRINT(" UE -> SS  RRC: RRCConnectionRequest " );

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

    // Check CellHandle

    if ( asp->RrcCellHandle()->get() != CellHandle )
    {
      PE_ERR("####### Message received in wrong cell #######" );
      NOTIFY_COMPLETION_FAILURE();
    }
    else
    {
      notifyCompletion(EV_OK);
    }
  }
  else
  {
    PE_ERR(" UE -> SS  RRC: RRCConnectionRequest " );
    PE_ERR("####### Message received but not excepted #######" );
    NOTIFY_COMPLETION_FAILURE();
  }
}

/** 

Author: Jussi Vatjus-Anttila

STATE:  RRC Connection Request shouldn't come

Note:   Waiting RRC Connection Request from UE.
        If tester received it, state goes FALSE,
        otherwise PASS.

        UE -> SS  (RRCR)   --> FALSE
*/

void sM_LTE_RrcConnectionRequestShouldntCome::start(void)
{
  sM_LteMlDefault::start(); 

  std::ostringstream ossInfo;
  ossInfo << "          TC: Check that UE does not send RRC Connection Request for " << (WaitTimer/1000) << " seconds ......";
  PE_PRINT(ossInfo.str());
    
  rsDefaultTimer->set(WaitTimer);
  NEXT_STATE (RRCConnectionRequest);
}

/*
ON_LTE_UL_CCCH_RRC_PDU( sM_LTE_RrcConnectionRequestShouldntCome, RRCConnectionRequest, RRCConnectionRequest )
{
  rsDefaultTimer->reset();

  std::ostringstream ossInfo;
  ossInfo << "#######   RRCConnectionRequest received during WaitTime  #######   ";
  PE_ERR(ossInfo.str());
  NOTIFY_COMPLETION_FAILURE();
}
*/

ON_TIMER(sM_LTE_RrcConnectionRequestShouldntCome, RRCConnectionRequest, RS_Default_Timer)
{
  std::ostringstream ossInfo;
  ossInfo << "         TC: RRCConnectionRequest timeout";
  PE_PRINT(ossInfo.str());
  notifyCompletion(EV_OK);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/