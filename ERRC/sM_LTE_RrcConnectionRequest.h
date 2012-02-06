// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_RRCCONNECTIONREQUEST_H_
#define _SM_LTE_RRCCONNECTIONREQUEST_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_RrcConnectionRequest************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionRequest, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionRequest;
  int         CellHandle;
  bool        Desired;
  int         WaitTimer;

  public:

  sM_LTE_RrcConnectionRequest(
    std::string& _xmlRRCConnectionRequest,
    int          _CellHandle
  )
  {
    xmlRRCConnectionRequest = _xmlRRCConnectionRequest;
    CellHandle              = _CellHandle;
    Desired                 = true;
    WaitTimer               = 0;
  }

  sM_LTE_RrcConnectionRequest(
    std::string& _xmlRRCConnectionRequest,
    int          _CellHandle,
    bool         _Desired,
    int          _WaitTimer
  )
  {
    xmlRRCConnectionRequest = _xmlRRCConnectionRequest;
    CellHandle              = _CellHandle;
    Desired                 = _Desired;
    WaitTimer               = _WaitTimer;
  }

  // Destructor

  ~sM_LTE_RrcConnectionRequest()
  {
  }

  void start(void);
};

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionRequestShouldntCome, sM_LTE_RrcConnectionRequest, DLLLTECOMMON )
{
private:

protected:

public:
    /**
    * Constructor
    */    
    sM_LTE_RrcConnectionRequestShouldntCome(int cellHandle, int WaitTime)
        : sM_LTE_RrcConnectionRequest(std::string(""), cellHandle, false, WaitTime)
    {
    }

    /**
    * Destructor
    */
    ~sM_LTE_RrcConnectionRequestShouldntCome()
    {
    }

    /** 
    * Startup code for the statemachine sM_LTE_NoRrcConnectionRequest
    * Overwrite the start() method in base statemachine sM_LteMlDefault.
    */
    void start(void);
};


#endif // _SM_LTE_RRCCONNECTIONREQUEST_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/