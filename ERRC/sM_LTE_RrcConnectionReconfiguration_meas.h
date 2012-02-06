// ****************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_RRCCONNECTIONRECONFIGURATION_MEAS_H_
#define _SM_LTE_RRCCONNECTIONRECONFIGURATION_MEAS_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_RrcConnectionReconfiguration_meas **********************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionReconfiguration_meas, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionReconfiguration;
  int         CarrierFreq;
  int         AllowedMeasBandwidth;
  bool        RsrpOrRsrq;
  int         Threshold;
  int         Hysteresis;
  std::string xmlRRCConnectionReconfigurationComplete;

  public:

  sM_LTE_RrcConnectionReconfiguration_meas (
    std::string& _xmlRRCConnectionReconfiguration,
    int          _CarrierFreq,
    int          _AllowedMeasBandwidth,
    bool         _RsrpOrRsrq,
    int          _Threshold,
    int          _Hysteresis,
    std::string& _xmlRRCConnectionReconfigurationComplete
  )
  {
    xmlRRCConnectionReconfiguration         = _xmlRRCConnectionReconfiguration;
    CarrierFreq                             = _CarrierFreq;
    AllowedMeasBandwidth                    = _AllowedMeasBandwidth;
    RsrpOrRsrq                              = _RsrpOrRsrq;
    Threshold                               = _Threshold;
    Hysteresis                              = _Hysteresis;
    xmlRRCConnectionReconfigurationComplete = _xmlRRCConnectionReconfigurationComplete;
  }

  // Destructor

  ~sM_LTE_RrcConnectionReconfiguration_meas()
  {
  }

  void start(void);

};

/* sM_LTE_RrcConnectionReconfiguration_meas_xml **********************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_RrcConnectionReconfiguration_meas_xml, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlRRCConnectionReconfiguration;
  int         MeasType;
  int         CarrierFreq_cell0;
  int         CarrierFreq_cell1;
  int         AllowedMeasBandwidth_cell0;
  int         AllowedMeasBandwidth_cell1;
  std::string xmlRRCConnectionReconfigurationComplete;

  public:

  sM_LTE_RrcConnectionReconfiguration_meas_xml (
    std::string& _xmlRRCConnectionReconfiguration,
    int          _MeasType, // intra-cell and intra-freq
    int          _CarrierFreq_cell0,
    int          _AllowedMeasBandwidth_cell0,
    std::string& _xmlRRCConnectionReconfigurationComplete
  )
  {
    xmlRRCConnectionReconfiguration         = _xmlRRCConnectionReconfiguration;
    MeasType                                = _MeasType;
    CarrierFreq_cell0                       = _CarrierFreq_cell0;
    AllowedMeasBandwidth_cell0              = _AllowedMeasBandwidth_cell0;
    xmlRRCConnectionReconfigurationComplete = _xmlRRCConnectionReconfigurationComplete;
  }

  sM_LTE_RrcConnectionReconfiguration_meas_xml (
    std::string& _xmlRRCConnectionReconfiguration,
    int          _MeasType, // inter-freq
    int          _CarrierFreq_cell0,
    int          _AllowedMeasBandwidth_cell0,
    int          _CarrierFreq_cell1,
    int          _AllowedMeasBandwidth_cell1,
    std::string& _xmlRRCConnectionReconfigurationComplete
  )
  {
    xmlRRCConnectionReconfiguration         = _xmlRRCConnectionReconfiguration;
    MeasType                                = _MeasType;
    CarrierFreq_cell0                       = _CarrierFreq_cell0;
    AllowedMeasBandwidth_cell0              = _AllowedMeasBandwidth_cell0;
    CarrierFreq_cell1                       = _CarrierFreq_cell1;
    AllowedMeasBandwidth_cell1              = _AllowedMeasBandwidth_cell1;
    xmlRRCConnectionReconfigurationComplete = _xmlRRCConnectionReconfigurationComplete;
  }

  // Destructor

  ~sM_LTE_RrcConnectionReconfiguration_meas_xml()
  {
  }

  void start(void);

};

#endif // _SM_LTE_RRCCONNECTIONRECONFIGURATION_MEAS_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/