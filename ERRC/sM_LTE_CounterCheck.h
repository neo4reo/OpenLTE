// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_COUNTERCHECK_H_
#define _SM_LTE_COUNTERCHECK_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_CounterCheck *******************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_CounterCheck, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  int         element_count_req;
  int         drb_1_req, drb_2_req, drb_3_req, drb_4_req;
  int         drb_5_req, drb_6_req, drb_7_req, drb_8_req;
  int         count_UL_1_req, count_UL_2_req, count_UL_3_req, count_UL_4_req;
  int         count_UL_5_req, count_UL_6_req, count_UL_7_req, count_UL_8_req;
  int         count_DL_1_req, count_DL_2_req, count_DL_3_req, count_DL_4_req;
  int         count_DL_5_req, count_DL_6_req, count_DL_7_req, count_DL_8_req;

  int         element_count_resp;
  int         drb_1_resp, drb_2_resp, drb_3_resp, drb_4_resp;
  int         drb_5_resp, drb_6_resp, drb_7_resp, drb_8_resp;
  int         count_UL_1_resp, count_UL_2_resp, count_UL_3_resp, count_UL_4_resp;
  int         count_UL_5_resp, count_UL_6_resp, count_UL_7_resp, count_UL_8_resp;
  int         count_DL_1_resp, count_DL_2_resp, count_DL_3_resp, count_DL_4_resp;
  int         count_DL_5_resp, count_DL_6_resp, count_DL_7_resp, count_DL_8_resp;

  int         retcode;

  public:

  sM_LTE_CounterCheck (
    int          _element_count_req,
    int          _element_count_resp,
    int          _drb_1_req,
    int          _count_UL_1_req,
    int          _count_DL_1_req,
    int          _drb_1_resp      = -1,
    int          _count_UL_1_resp = -1,
    int          _count_DL_1_resp = -1,

    int          _drb_2_req       = -1,
    int          _count_UL_2_req  = -1,
    int          _count_DL_2_req  = -1,
    int          _drb_2_resp      = -1,
    int          _count_UL_2_resp = -1,
    int          _count_DL_2_resp = -1,

    int          _drb_3_req       = -1,
    int          _count_UL_3_req  = -1,
    int          _count_DL_3_req  = -1,
    int          _drb_3_resp      = -1,
    int          _count_UL_3_resp = -1,
    int          _count_DL_3_resp = -1,

    int          _drb_4_req       = -1,
    int          _count_UL_4_req  = -1,
    int          _count_DL_4_req  = -1,
    int          _drb_4_resp      = -1,
    int          _count_UL_4_resp = -1,
    int          _count_DL_4_resp = -1,

    int          _drb_5_req       = -1,
    int          _count_UL_5_req  = -1,
    int          _count_DL_5_req  = -1,
    int          _drb_5_resp      = -1,
    int          _count_UL_5_resp = -1,
    int          _count_DL_5_resp = -1,

    int          _drb_6_req       = -1,
    int          _count_UL_6_req  = -1,
    int          _count_DL_6_req  = -1,
    int          _drb_6_resp      = -1,
    int          _count_UL_6_resp = -1,
    int          _count_DL_6_resp = -1,

    int          _drb_7_req       = -1,
    int          _count_UL_7_req  = -1,
    int          _count_DL_7_req  = -1,
    int          _drb_7_resp      = -1,
    int          _count_UL_7_resp = -1,
    int          _count_DL_7_resp = -1,

    int          _drb_8_req       = -1,
    int          _count_UL_8_req  = -1,
    int          _count_DL_8_req  = -1,
    int          _drb_8_resp      = -1,
    int          _count_UL_8_resp = -1,
    int          _count_DL_8_resp = -1
  )
  {
    element_count_req   = _element_count_req;
    element_count_resp  = _element_count_resp;
    drb_1_req       = _drb_1_req;
    count_UL_1_req  = _count_UL_1_req;
    count_DL_1_req  = _count_DL_1_req;
    drb_1_resp      = _drb_1_resp;
    count_UL_1_resp = _count_UL_1_resp;
    count_DL_1_resp = _count_DL_1_resp;

    drb_2_req       = _drb_2_req;
    count_UL_2_req  = _count_UL_2_req;
    count_DL_2_req  = _count_DL_2_req;
    drb_2_resp      = _drb_2_resp;
    count_UL_2_resp = _count_UL_2_resp;
    count_DL_2_resp = _count_DL_2_resp;

    drb_3_req       = _drb_3_req;
    count_UL_3_req  = _count_UL_3_req;
    count_DL_3_req  = _count_DL_3_req;
    drb_3_resp      = _drb_3_resp;
    count_UL_3_resp = _count_UL_3_resp;
    count_DL_3_resp = _count_DL_3_resp;

    drb_4_req       = _drb_4_req;
    count_UL_4_req  = _count_UL_4_req;
    count_DL_4_req  = _count_DL_4_req;
    drb_4_resp      = _drb_4_resp;
    count_UL_4_resp = _count_UL_4_resp;
    count_DL_4_resp = _count_DL_4_resp;

    drb_5_req       = _drb_5_req;
    count_UL_5_req  = _count_UL_5_req;
    count_DL_5_req  = _count_DL_5_req;
    drb_5_resp      = _drb_5_resp;
    count_UL_5_resp = _count_UL_5_resp;
    count_DL_5_resp = _count_DL_5_resp;

    drb_6_req       = _drb_6_req;
    count_UL_6_req  = _count_UL_6_req;
    count_DL_6_req  = _count_DL_6_req;
    drb_6_resp      = _drb_6_resp;
    count_UL_6_resp = _count_UL_6_resp;
    count_DL_6_resp = _count_DL_6_resp;

    drb_7_req       = _drb_7_req;
    count_UL_7_req  = _count_UL_7_req;
    count_DL_7_req  = _count_DL_7_req;
    drb_7_resp      = _drb_7_resp;
    count_UL_7_resp = _count_UL_7_resp;
    count_DL_7_resp = _count_DL_7_resp;

    drb_8_req       = _drb_8_req;
    count_UL_8_req  = _count_UL_8_req;
    count_DL_8_req  = _count_DL_8_req;
    drb_8_resp      = _drb_8_resp;
    count_UL_8_resp = _count_UL_8_resp;
    count_DL_8_resp = _count_DL_8_resp;

    retcode         = EV_OK;
  }

  // Destructor

  ~sM_LTE_CounterCheck()
  {
  }

  void start(void);

};

#endif // _SM_LTE_COUNTERCHECK_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/