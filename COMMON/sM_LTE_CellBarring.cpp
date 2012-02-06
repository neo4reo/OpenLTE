/*

EUTRAN L3 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_CellBarring.cpp

Version:        0.2

Author:         Dennis M Senyonjo


Copyright (c) 2012. 


Change history:

    VERSION     DATE/AUTHOR                         COMMENT
    0.1         26.11.2010 / Dennis M Senyonjo      Logs fix
    0.1         09.09.2010 / Dennis M Senyonjo      Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <sstream>

/* PUBLIC *********************************************************************/

void sM_LTE_CellBarringInitializedByUE::start(void)
{
  sM_LteMlDefault::start();

  PE_PRINT("            TC: Waiting barring timeout" );

  if ( WaitTimer == 0 )
  {
    rsDefaultTimer->set(300000); //default 300s
  }
  else
  {
    rsDefaultTimer->set(WaitTimer);
  }

  NEXT_STATE ( RRCConnectionRequest );
}

ON_TIMER( sM_LTE_CellBarringInitializedByUE, RRCConnectionRequest, RS_Default_Timer)
{
   PE_PRINT("         TC: Barring timeout" );
   notifyCompletion(EV_OK);
}

ON_LTE_UL_CCCH_RRC_PDU( sM_LTE_CellBarringInitializedByUE, RRCConnectionRequest, RRCConnectionRequest )
{
   rsDefaultTimer->reset();

   GCO_DUMP(pdu);
   PE_PRINT(" UE -> SS  RRC: RRCConnectionRequest " );

   // Check CellHandle

   if ( asp->RrcCellHandle()->get() != CellHandle )
   {
        /// @todo   There might be some todo:
        ///         *now Received RRCR with non barred cell
        ///         *State is PASS ?
        /*
        if ( xmlRRCConnectionRequest != "" )
        {
          if ( GCO_MATCHF(root, xmlRRCConnectionRequest.c_str()) )
          {
            PE_PRINT(####### Message is OK #######" );
          }
          else
          {
            PE_ERR("####### Message is NOK #######" );
            NOTIFY_COMPLETION_FAILURE();
          }
        }
        */

        PE_ERR("####### Message received in wrong cell #######" );
        notifyCompletion(EV_OK);
    }
    else
    {
        notifyCompletion(EV_OK);
    }
  

}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/