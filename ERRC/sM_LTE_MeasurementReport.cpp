/*

EUTRAN L2 SW















                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_MeasurementReport.cpp

Version:        1.2

Author:          Dennis M Senyonjo


Copyright (c) 2011.  Dennis M Senyonjo.


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    1.2         05.04.2011 /  Dennis M Senyonjo    Minor updates
    1.1         04.04.2011 /  Dennis M Senyonjo    Minor updates
    1.0         18.01.2011 /  Dennis M Senyonjo    Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"

/* PUBLIC *********************************************************************/

void sM_LTE_MeasurementReport::start(void)
{
  sM_LteMlDefault::start();

  rsDefaultTimer->set(MaxWaitTime);

  NEXT_STATE ( MeasurementReport );
}

ON_TIMER( sM_LTE_MeasurementReport, MeasurementReport, RS_Default_Timer )
{
  if ( MaxReportAmount == -1 )
  {
    PE_PRINT("           TC: MeasurementReport procedure completed"  );
    notifyCompletion(EV_OK);
  }
  else
  {
    if ( MeasReportCount < MaxReportAmount )
    {
      PE_ERR("            TC: MeasReportCount < MaxReportAmount " );
      notifyCompletion(EV_FAIL);
    }
    else if ( MeasReportCount == MaxReportAmount )
    {
      PE_PRINT("           TC: MeasurementReport procedure completed" );
      notifyCompletion(EV_OK);
    }
    else
    {
      PE_ERR("            TC: MeasReportCount > MaxReportAmount " );
      notifyCompletion(EV_FAIL);
    }
  }
}

ON_LTE_UL_DCCH_RRC_PDU( sM_LTE_MeasurementReport, MeasurementReport, MeasurementReport )
{
  PE_PRINT(" UE -> SS  RRC: MeasurementReport " );

  bool NeighReport = false;

  MeasId     = pdu->criticalExtensions()->c1()->measurementReport_r8()->measResults()->measId()->get();;
  RsrpResult = pdu->criticalExtensions()->c1()->measurementReport_r8()->measResults()->measResultServCell()->rsrpResult()->get();
  RsrqResult = pdu->criticalExtensions()->c1()->measurementReport_r8()->measResults()->measResultServCell()->rsrqResult()->get();

  LteReferenceTestConditions->setServingCellMeasurement_RSRP(RsrpResult - 140);
  LteReferenceTestConditions->setServingCellMeasurement_RSRQ(RsrqResult);

  if ( pdu->criticalExtensions()->c1()->measurementReport_r8()->measResults()->isActive_measResultNeighCells() )
  {
    RsrpResult2 = pdu->criticalExtensions()->c1()->measurementReport_r8()->measResults()->measResultNeighCells()->measResultListEUTRA()->MeasResultEUTRA(0)->measResult()->rsrpResult()->get();
    LteReferenceTestConditions->setNeighbourCellMeasurement_RSRP(RsrpResult2 - 140);
    NeighReport = true;
  }

  std::ostringstream ossInfo;
  ossInfo << "           TC: measId = " << MeasId << ", measResultServCell: rsrpResult = " << RsrpResult - 140 << "dBm, rsrqResult = " << RsrqResult;
  PE_WARN(ossInfo.str());

  if ( NeighReport == true )
  {
    std::ostringstream ossInfo2;
    ossInfo2 << "           TC: measResultNeighCell: rsrpResult = " << RsrpResult2 - 140 << "dBm";
    PE_WARN(ossInfo2.str());
  }

  GCO_DUMP(pdu);

  MeasReportCount++;

  NEXT_STATE ( MeasurementReport );
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/