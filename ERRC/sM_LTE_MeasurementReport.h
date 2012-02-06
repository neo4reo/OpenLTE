// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_MEASUREMENTREPORT_H_
#define _SM_LTE_MEASUREMENTREPORT_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_MeasurementReport **************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_MeasurementReport, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  int  MeasId;
  int  RsrpResult, RsrpResult2;
  int  RsrqResult, RsrqResult2;
  bool NeighReport;
  int  MaxReportAmount;
  int  MaxWaitTime;
  int  MeasReportCount;

  public:

  sM_LTE_MeasurementReport (
    int _MaxReportAmount,
    int _MaxWaitTime
  )
  {
    MaxReportAmount = _MaxReportAmount;
    MaxWaitTime     = _MaxWaitTime;
    MeasReportCount = 0;
  }

  // Destructor

  ~sM_LTE_MeasurementReport()
  {
  }

  void start(void);

};

#endif // _SM_LTE_MEASUREMENTREPORT_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/