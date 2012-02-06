/*

EUTRAN L2 SW














                         Test Step for CMW-500
                         ---------------------

                         SW module





Filename:       sM_LTE_UeRadioAccessCapability.cpp

Version:        3.0

Author:          Dennis M Senyonjo


Copyright (c) 2011. Renesas Mobile.


Change history:

    VERSION     DATE/AUTHOR                 COMMENT
    3.0         05.04.2011 /                Updated bitstring_featgrp
    2.0         04.04.2011 /                Updated FGI bit handling
    1.0         18.01.2011 /Dennis M S      Original

*/

/* INCLUDE FILES **************************************************************/

#include "lte_common.h"
#include <mlapi.h>
#include <sstream>
#include <bitset>
#include <string>

using namespace std;

/* PUBLIC *********************************************************************/

void sM_LTE_UeRadioAccessCapability::start(void)
{
  sM_LteMlDefault::start();

  DECL_NEW_LTE_RRC_PDU ( sendpdu, DL_DCCH, UECapabilityEnquiry, xmlUECapabilityEnquiry.c_str() );

  GCO_DUMP(sendpdu.pdu);

  OUTPUT ( SAP_LTE_NET_CRRC, NEW_ASP_WITH_DL_DCCH_RRC_PDU (sendpdu) );

  delete &sendpdu;

  PE_PRINT(" UE <- UE  RRC: UECapabilityEnquiry " );

  PE_PRINT("           TC: Waiting for UECapabilityInformation" );

  rsDefaultTimer->set(GEN_RRC_TIMER);

  NEXT_STATE ( UECapabilityInformation );
}

bool sM_LTE_UeRadioAccessCapability::getInterFreqNeedForGaps( int i, int j )
{
  return mInterFreqNeedForGaps[ i ][ j ];
}

void sM_LTE_UeRadioAccessCapability::EUTRADecode( Lte::Std::_UE_CapabilityRAT_Container::__f372_ * ue_cap_cont )
{
  unsigned char* pData = ue_cap_cont->get();
  int len = ue_cap_cont->getLen()*8;

  Lte::Std::_UE_EUTRA_Capability * eutraUeCap = Lte::Std::_UE_EUTRA_Capability::factory((BinaryMddb*) pData, len );

  GCO_DUMP(eutraUeCap);

  /*
  <accessStratumRelease VALUE="0"/><!--ENUMERATED [0,7] rel8(0) spare7(1) spare6(2) spare5(3) spare4(4) spare3(5) spare2(6) spare1(7)-->
  */

  if ( eutraUeCap->accessStratumRelease()->get() != 0 )
  {
    PE_ERR("####### Missmatch in accessStratumRelease #######" );
    notifyCompletion(EV_FAIL);
    retcode = EV_FAIL;
  }

  if ( retcode == EV_OK )
  {
    /*
    <ue_Category VALUE="1"/><!--INTEGER [1,5]-->
    */

    if ( LteReferenceTestConditions->getPx_eUE_Category_Type() != eutraUeCap->ue_Category()->get() )
    {
      PE_ERR("####### Missmatch in ue_Category #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }
  }

  if ( retcode == EV_OK )
  {
    /*
    <pdcp_Parameters>
    <supportedROHC_Profiles>
    <profile0x0001 VALUE="0"/><!--BOOLEAN-->
    <profile0x0002 VALUE="0"/><!--BOOLEAN-->
    <profile0x0003 VALUE="0"/><!--BOOLEAN-->
    <profile0x0004 VALUE="0"/><!--BOOLEAN-->
    <profile0x0006 VALUE="0"/><!--BOOLEAN-->
    <profile0x0101 VALUE="0"/><!--BOOLEAN-->
    <profile0x0102 VALUE="0"/><!--BOOLEAN-->
    <profile0x0103 VALUE="0"/><!--BOOLEAN-->
    <profile0x0104 VALUE="0"/><!--BOOLEAN-->
    </supportedROHC_Profiles>
    */

    bool profile0x0001 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0001()->get();
    bool profile0x0002 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0002()->get();
    bool profile0x0003 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0003()->get();
    bool profile0x0004 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0004()->get();
    bool profile0x0006 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0006()->get();
    bool profile0x0101 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0101()->get();
    bool profile0x0102 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0102()->get();
    bool profile0x0103 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0103()->get();
    bool profile0x0104 = eutraUeCap->pdcp_Parameters()->supportedROHC_Profiles()->profile0x0104()->get();

    /* OPTIONAL
    <maxNumberROHC_ContextSessions ACTIVE="1" VALUE="0"/><!--ENUMERATED [0,15] cs2(0) cs4(1) cs8(2) cs12(3) cs16(4) cs24(5) cs32(6) cs48(7) cs64(8) cs128(9) cs256(10) cs512(11) cs1024(12) cs16384(13) spare2(14) spare1(15)-->
    */

    if ( eutraUeCap->pdcp_Parameters()->isActive_maxNumberROHC_ContextSessions() )
    {
      int maxNumberROHC_ContextSessions = eutraUeCap->pdcp_Parameters()->maxNumberROHC_ContextSessions()->get();
    }

    /*
    </pdcp_Parameters>
    <phyLayerParameters>
    <ue_TxAntennaSelectionSupported VALUE="0"/><!--BOOLEAN-->
    <ue_SpecificRefSigsSupported VALUE="0"/><!--BOOLEAN-->
    </phyLayerParameters>
    */

    bool ue_TxAntennaSelectionSupported = eutraUeCap->phyLayerParameters()->ue_TxAntennaSelectionSupported()->get();
    bool ue_SpecificRefSigsSupported = eutraUeCap->phyLayerParameters()->ue_SpecificRefSigsSupported()->get();

    /*
    <rf_Parameters>
    <supportedBandListEUTRA COUNT="1"><!-- [1,maxBands(64)]-->
    <INDEX>
    <bandEUTRA VALUE="1"/><!--INTEGER [1,64]-->
    <halfDuplex VALUE="0"/><!--BOOLEAN-->
    </INDEX>
    </supportedBandListEUTRA>
    </rf_Parameters>
    */

    bitstring_ebands.reset();
    bitstring_ebands.set(0, LteReferenceTestConditions->getPc_eBand1_Supp());
    bitstring_ebands.set(1, LteReferenceTestConditions->getPc_eBand2_Supp());
    bitstring_ebands.set(2, LteReferenceTestConditions->getPc_eBand3_Supp());
    bitstring_ebands.set(3, LteReferenceTestConditions->getPc_eBand4_Supp());
    bitstring_ebands.set(4, LteReferenceTestConditions->getPc_eBand5_Supp());
    bitstring_ebands.set(5, LteReferenceTestConditions->getPc_eBand6_Supp());
    bitstring_ebands.set(6, LteReferenceTestConditions->getPc_eBand7_Supp());
    bitstring_ebands.set(7, LteReferenceTestConditions->getPc_eBand8_Supp());
    bitstring_ebands.set(8, LteReferenceTestConditions->getPc_eBand9_Supp());
    bitstring_ebands.set(9, LteReferenceTestConditions->getPc_eBand10_Supp());
    bitstring_ebands.set(10, LteReferenceTestConditions->getPc_eBand11_Supp());
    bitstring_ebands.set(11, LteReferenceTestConditions->getPc_eBand12_Supp());
    bitstring_ebands.set(12, LteReferenceTestConditions->getPc_eBand13_Supp());
    bitstring_ebands.set(13, LteReferenceTestConditions->getPc_eBand14_Supp());
    bitstring_ebands.set(14, LteReferenceTestConditions->getPc_eBand15_Supp());
    bitstring_ebands.set(15, LteReferenceTestConditions->getPc_eBand16_Supp());
    bitstring_ebands.set(16, LteReferenceTestConditions->getPc_eBand17_Supp());
    bitstring_ebands.set(17, LteReferenceTestConditions->getPc_eBand18_Supp());
    bitstring_ebands.set(18, LteReferenceTestConditions->getPc_eBand19_Supp());
    bitstring_ebands.set(19, LteReferenceTestConditions->getPc_eBand20_Supp());
    bitstring_ebands.set(20, LteReferenceTestConditions->getPc_eBand21_Supp());
    bitstring_ebands.set(21, LteReferenceTestConditions->getPc_eBand22_Supp());
    bitstring_ebands.set(22, LteReferenceTestConditions->getPc_eBand23_Supp());
    bitstring_ebands.set(23, LteReferenceTestConditions->getPc_eBand24_Supp());
    bitstring_ebands.set(24, LteReferenceTestConditions->getPc_eBand25_Supp());
    bitstring_ebands.set(25, LteReferenceTestConditions->getPc_eBand26_Supp());
    bitstring_ebands.set(26, LteReferenceTestConditions->getPc_eBand27_Supp());
    bitstring_ebands.set(27, LteReferenceTestConditions->getPc_eBand28_Supp());
    bitstring_ebands.set(28, LteReferenceTestConditions->getPc_eBand29_Supp());
    bitstring_ebands.set(29, LteReferenceTestConditions->getPc_eBand30_Supp());
    bitstring_ebands.set(30, LteReferenceTestConditions->getPc_eBand31_Supp());
    bitstring_ebands.set(31, LteReferenceTestConditions->getPc_eBand32_Supp());

    bitstring_ebands_ue.reset();
    for ( int i = 0; i < eutraUeCap->rf_Parameters()->supportedBandListEUTRA()->getElementCount(); i++ )
    {
      int bandEUTRA = eutraUeCap->rf_Parameters()->supportedBandListEUTRA()->value(i)->bandEUTRA()->get();
      PRINTERR(("SupportedBandListEUTRA[%i]: BandEUTRA: %i\n", i, bandEUTRA));

      if ( ( bandEUTRA < 1 ) || ( bandEUTRA > SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS ) )
      {
        PRINTERR(("Invalid band value"));
      }
      else if ( 1 == bitstring_ebands_ue[ bandEUTRA-1 ] )
      {
        PRINTERR(("Band %i was found twice from supportedBandListEUTRA\n", bandEUTRA ));
      }
      else
      {
        bitstring_ebands_ue.set(bandEUTRA-1, 1 );
      }
    }

    for ( int i = 0; i < SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS; i++ )
    {
      if (  bitstring_ebands.at( i ) != bitstring_ebands_ue.at( i ) )
      {
        if ( 1 == bitstring_ebands.at( i ) )
        {
          PRINTERR(("supportedBandListEUTRA: Unexpected band: %i\n", i+1 ));
        }
        else
        {
          PRINTERR(("supportedBandListEUTRA: Band not found: %i\n", i+1 ));
        }
        PE_ERR("####### Missmatch in supportedBandListEUTRA #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
        // break;
      }
    }
  }

  if ( retcode == EV_OK )
  {
    // Clear mInterFreqNeedForGaps parameters
    memset( mInterFreqNeedForGaps, 0x0, (SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS*
    SM_LTE_UERADIOACCESSCAPABILITY_MAX_BANDS)*sizeof(bool) );

    /*
    <measParameters>
    <bandListEUTRA COUNT="1"><!-- [1,maxBands(64)]-->
    */

    for ( int i = 0; i < eutraUeCap->measParameters()->bandListEUTRA()->getElementCount(); i++ )
    {
      if ( bitstring_ebands.count() != eutraUeCap->measParameters()->bandListEUTRA()->
           value(i)->interFreqBandList()->getElementCount() )
      {
        PE_ERR("####### Missmatch in number of interFreqBandList #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      /*
      <INDEX>
      <interFreqBandList COUNT="1"><!-- [1,maxBands(64)]-->
      */

      for ( int j = 0; j < eutraUeCap->measParameters()->bandListEUTRA()->
            value(i)->interFreqBandList()->getElementCount(); j++ )
      {
        /*
        <INDEX>
        <interFreqNeedForGaps VALUE="0"/><!--BOOLEAN-->
        </INDEX>
        */

        _LONGLONG_ interFreqNeedForGaps = eutraUeCap->measParameters()->bandListEUTRA()->value(i)->
        interFreqBandList()->value(j)->interFreqNeedForGaps()->getValue();
        PRINTERR(("measParameters: bandListEUTRA[%i]: interFreqBandList[%i]: interFreqNeedForGaps: %i\n",
          i, j, ( int )interFreqNeedForGaps ));
        mInterFreqNeedForGaps[i][j] = ( bool )interFreqNeedForGaps;
      }

     /*
     </interFreqBandList>
     OPTIONAL <interRAT_BandList ACTIVE="1" COUNT="1"><!-- [1,maxBands(64)]-->
     */

      for ( int j = 0; j < eutraUeCap->measParameters()->bandListEUTRA()->
            value(i)->interRAT_BandList()->getElementCount(); j++ )
      {
        /*
        <INDEX>
        <interRAT_NeedForGaps VALUE="0"/><!--BOOLEAN-->
        </INDEX>
        */
        _LONGLONG_ interRATNeedForGaps = eutraUeCap->measParameters()->bandListEUTRA()->value(i)->
          interRAT_BandList()->value(j)->interRAT_NeedForGaps()->getValue();
        PRINTERR(("measParameters: bandListEUTRA[%i]: interRAT_BandList[%i]: interRATNeedForGaps: %i\n",
          i, j, interRATNeedForGaps ));
      }
      /*
      </interRAT_BandList>
      </INDEX>
      */
    }
    /*
    </bandListEUTRA>
    </measParameters>
    */
  }

  if ( retcode == EV_OK )
  {
    /* OPTIONAL
    <featureGroupIndicators ACTIVE="1" VALUE="00000000000000000000000000000000"/><!--BITSTRING [32]-->
    */

    bitset<32> bitstring_featgrp;
    bitstring_featgrp.reset();

    bitstring_featgrp.set(31, LteReferenceTestConditions->getPc_FeatrGrp_1());
    bitstring_featgrp.set(30, LteReferenceTestConditions->getPc_FeatrGrp_2());
    bitstring_featgrp.set(29, LteReferenceTestConditions->getPc_FeatrGrp_3());
    bitstring_featgrp.set(28, LteReferenceTestConditions->getPc_FeatrGrp_4());
    bitstring_featgrp.set(27, LteReferenceTestConditions->getPc_FeatrGrp_5());
    bitstring_featgrp.set(26, LteReferenceTestConditions->getPc_FeatrGrp_6());
    bitstring_featgrp.set(25, LteReferenceTestConditions->getPc_FeatrGrp_7());
    bitstring_featgrp.set(24, LteReferenceTestConditions->getPc_FeatrGrp_8());
    bitstring_featgrp.set(23, LteReferenceTestConditions->getPc_FeatrGrp_9());
    bitstring_featgrp.set(22, LteReferenceTestConditions->getPc_FeatrGrp_10());
    bitstring_featgrp.set(21, LteReferenceTestConditions->getPc_FeatrGrp_11());
    bitstring_featgrp.set(20, LteReferenceTestConditions->getPc_FeatrGrp_12());
    bitstring_featgrp.set(19, LteReferenceTestConditions->getPc_FeatrGrp_13());
    bitstring_featgrp.set(18, LteReferenceTestConditions->getPc_FeatrGrp_14());
    bitstring_featgrp.set(17, LteReferenceTestConditions->getPc_FeatrGrp_15());
    bitstring_featgrp.set(16, LteReferenceTestConditions->getPc_FeatrGrp_16());
    bitstring_featgrp.set(15, LteReferenceTestConditions->getPc_FeatrGrp_17());
    bitstring_featgrp.set(14, LteReferenceTestConditions->getPc_FeatrGrp_18());
    bitstring_featgrp.set(13, LteReferenceTestConditions->getPc_FeatrGrp_19());
    bitstring_featgrp.set(12, LteReferenceTestConditions->getPc_FeatrGrp_20());
    bitstring_featgrp.set(11, LteReferenceTestConditions->getPc_FeatrGrp_21());
    bitstring_featgrp.set(10, LteReferenceTestConditions->getPc_FeatrGrp_22());
    bitstring_featgrp.set(9, LteReferenceTestConditions->getPc_FeatrGrp_23());
    bitstring_featgrp.set(8, LteReferenceTestConditions->getPc_FeatrGrp_24());
    bitstring_featgrp.set(7, LteReferenceTestConditions->getPc_FeatrGrp_25());
    bitstring_featgrp.set(6, LteReferenceTestConditions->getPc_FeatrGrp_26());
    bitstring_featgrp.set(5, LteReferenceTestConditions->getPc_FeatrGrp_27());
    bitstring_featgrp.set(4, LteReferenceTestConditions->getPc_FeatrGrp_28());
    bitstring_featgrp.set(3, LteReferenceTestConditions->getPc_FeatrGrp_29());
    bitstring_featgrp.set(2, LteReferenceTestConditions->getPc_FeatrGrp_30());
    bitstring_featgrp.set(1, LteReferenceTestConditions->getPc_FeatrGrp_31());
    bitstring_featgrp.set(0, LteReferenceTestConditions->getPc_FeatrGrp_32());
    int NrOfSuppEutraFeatGrps = bitstring_featgrp.count(); // ones
    PRINTERR(("NrOfSuppEutraFeatGrps is: %i\n", NrOfSuppEutraFeatGrps));

    if ( eutraUeCap->isActive_featureGroupIndicators() )
    {
      unsigned char* featureGroupIndicators = (unsigned char*) eutraUeCap->featureGroupIndicators()->get();
      PRINTERR(("Received featureGroupIndicators =  %s\n", eutraUeCap->featureGroupIndicators()->get() ));

      int len = eutraUeCap->featureGroupIndicators()->getLen();
      PRINTERR(("Length =  %i\n",len ));

      int featGrpIndic[32];

      /* For-loop: atoi(unsigned chars -> integers) */
      for( int j=0; j<32; j++)
      {
        char featGrpTemp = featureGroupIndicators[j];
        featGrpIndic[j] = atoi (&featGrpTemp);
      }

      if ( featGrpIndic[0] != LteReferenceTestConditions->getPc_FeatrGrp_1() )
      {
        PE_ERR("####### FeatureGroupIndicator 1 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[1] != LteReferenceTestConditions->getPc_FeatrGrp_2() )
      {
        PE_ERR("####### FeatureGroupIndicator 2 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[2] != LteReferenceTestConditions->getPc_FeatrGrp_3() )
      {
        PE_ERR("####### FeatureGroupIndicator 3 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[3] != LteReferenceTestConditions->getPc_FeatrGrp_4() )
      {
        PE_ERR("####### FeatureGroupIndicator 4 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[4] != LteReferenceTestConditions->getPc_FeatrGrp_5() )
      {
        PE_ERR("####### FeatureGroupIndicator 5 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[5] != LteReferenceTestConditions->getPc_FeatrGrp_6() )
      {
        PE_ERR("####### FeatureGroupIndicator 6 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[6] != LteReferenceTestConditions->getPc_FeatrGrp_7() )
      {
        PE_ERR("####### FeatureGroupIndicator 7 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[7] != LteReferenceTestConditions->getPc_FeatrGrp_8() )
      {
        PE_ERR("####### FeatureGroupIndicator 8 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[8] != LteReferenceTestConditions->getPc_FeatrGrp_9() )
      {
        PE_ERR("####### FeatureGroupIndicator 9 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[9] != LteReferenceTestConditions->getPc_FeatrGrp_10() )
      {
        PE_ERR("####### FeatureGroupIndicator 10 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[10] != LteReferenceTestConditions->getPc_FeatrGrp_11() )
      {
        PE_ERR("####### FeatureGroupIndicator 11 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[11] != LteReferenceTestConditions->getPc_FeatrGrp_12() )
      {
        PE_ERR("####### FeatureGroupIndicator 12 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[12] != LteReferenceTestConditions->getPc_FeatrGrp_13() )
      {
        PE_ERR("####### FeatureGroupIndicator 13 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[13] != LteReferenceTestConditions->getPc_FeatrGrp_14() )
      {
        PE_ERR("####### FeatureGroupIndicator 14 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[14] != LteReferenceTestConditions->getPc_FeatrGrp_15() )
      {
        PE_ERR("####### FeatureGroupIndicator 15 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[15] != LteReferenceTestConditions->getPc_FeatrGrp_16() )
      {
        PE_ERR("####### FeatureGroupIndicator 16 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[16] != LteReferenceTestConditions->getPc_FeatrGrp_17() )
      {
        PE_ERR("####### FeatureGroupIndicator 17 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[17] != LteReferenceTestConditions->getPc_FeatrGrp_18() )
      {
        PE_ERR("####### FeatureGroupIndicator 18 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[18] != LteReferenceTestConditions->getPc_FeatrGrp_19() )
      {
        PE_ERR("####### FeatureGroupIndicator 19 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[19] != LteReferenceTestConditions->getPc_FeatrGrp_20() )
      {
        PE_ERR("####### FeatureGroupIndicator 20 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[20] != LteReferenceTestConditions->getPc_FeatrGrp_21() )
      {
        PE_ERR("####### FeatureGroupIndicator 21 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[21] != LteReferenceTestConditions->getPc_FeatrGrp_22() )
      {
        PE_ERR("####### FeatureGroupIndicator 22 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[22] != LteReferenceTestConditions->getPc_FeatrGrp_23() )
      {
        PE_ERR("####### FeatureGroupIndicator 23 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[23] != LteReferenceTestConditions->getPc_FeatrGrp_24() )
      {
        PE_ERR("####### FeatureGroupIndicator 24 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[24] != LteReferenceTestConditions->getPc_FeatrGrp_25() )
      {
        PE_ERR("####### FeatureGroupIndicator 25 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[25] != LteReferenceTestConditions->getPc_FeatrGrp_26() )
      {
        PE_ERR("####### FeatureGroupIndicator 26 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[26] != LteReferenceTestConditions->getPc_FeatrGrp_27() )
      {
        PE_ERR("####### FeatureGroupIndicator 27 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[27] != LteReferenceTestConditions->getPc_FeatrGrp_28() )
      {
        PE_ERR("####### FeatureGroupIndicator 28 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[28] != LteReferenceTestConditions->getPc_FeatrGrp_29() )
      {
        PE_ERR("####### FeatureGroupIndicator 29 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[29] != LteReferenceTestConditions->getPc_FeatrGrp_30() )
      {
        PE_ERR("####### FeatureGroupIndicator 30 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[30] != LteReferenceTestConditions->getPc_FeatrGrp_31() )
      {
        PE_ERR("####### FeatureGroupIndicator 31 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }

      if ( featGrpIndic[31] != LteReferenceTestConditions->getPc_FeatrGrp_32() )
      {
        PE_ERR("####### FeatureGroupIndicator 32 missmatch #######" );
        notifyCompletion(EV_FAIL);
        retcode = EV_FAIL;
      }
    }
    else
    {
      PE_ERR("####### FeatureGroupIndicator is not active #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }

    /*
    <interRAT_Parameters>
    <utraFDD ACTIVE="1">
    ...
    </utraFDD>
    */

    if ( LteReferenceTestConditions->getPc_FDD() != eutraUeCap->interRAT_Parameters()->isActive_utraFDD() )
    {
      PE_ERR("####### Missmatch in interRAT_Parameters->utraFDD #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }
    else if ( eutraUeCap->interRAT_Parameters()->isActive_utraFDD() )
    {
      for ( int i = 0; i <
            eutraUeCap->interRAT_Parameters()->utraFDD()->supportedBandListUTRA_FDD()->getElementCount();
            i++ )
      {
        PRINTERR(("interRAT_Parameters: utraFDD: supportedBandList[%i]: band: %i\n",
          i, eutraUeCap->interRAT_Parameters()->utraFDD()->supportedBandListUTRA_FDD()->value(i) ));
      }
    }
  }

  if ( retcode == EV_OK )
  {
    /*
    <utraTDD128 ACTIVE="1">
    ...
    </utraTDD128>
    */
    if ( LteReferenceTestConditions->getPc_TDD_LCR() != eutraUeCap->interRAT_Parameters()->isActive_utraTDD128() )
    {
      PE_ERR("####### Missmatch in interRAT_Parameters->utraTDD128 #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }
    else if ( eutraUeCap->interRAT_Parameters()->isActive_utraTDD128() )
    {
      for ( int i = 0; i <
            eutraUeCap->interRAT_Parameters()->utraTDD128()->supportedBandListUTRA_TDD128()->getElementCount();
            i++ )
      {
        PRINTERR(("interRAT_Parameters: utraTDD128: supportedBandList[%i]: band: %i\n",
          i, eutraUeCap->interRAT_Parameters()->utraTDD128()->supportedBandListUTRA_TDD128()->value(i) ));
      }
    }
  }

  if ( retcode == EV_OK )
  {
    /*
    <utraTDD384 ACTIVE="1">
    ...
    </utraTDD384>
    */

    if ( LteReferenceTestConditions->getPc_TDD_HCR() != eutraUeCap->interRAT_Parameters()->isActive_utraTDD384() )
    {
      PE_ERR( "####### Missmatch in interRAT_Parameters->utraTDD384 #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }
    else if ( eutraUeCap->interRAT_Parameters()->isActive_utraTDD384() )
    {
      for ( int i = 0; i <
            eutraUeCap->interRAT_Parameters()->utraTDD384()->supportedBandListUTRA_TDD384()->getElementCount();
            i++ )
      {
        PRINTERR(("interRAT_Parameters: utraTDD384: supportedBandList[%i]: band: %i\n",
          i, eutraUeCap->interRAT_Parameters()->utraTDD384()->supportedBandListUTRA_TDD384()->value(i) ));
      }
    }
  }

  if ( retcode == EV_OK )
  {
    /*
    <utraTDD768 ACTIVE="1">
    ...
    </utraTDD768>
    */

    if ( LteReferenceTestConditions->getPc_TDD_VHCR() != eutraUeCap->interRAT_Parameters()->isActive_utraTDD768() )
    {
      PE_ERR("####### Missmatch in interRAT_Parameters->utraTDD768 #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }
    else if ( eutraUeCap->interRAT_Parameters()->isActive_utraTDD768() )
    {
      for ( int i = 0; i <
            eutraUeCap->interRAT_Parameters()->utraTDD768()->supportedBandListUTRA_TDD768()->getElementCount();
            i++ )
      {
        PRINTERR(("interRAT_Parameters: utraTDD768: supportedBandList[%i]: band: %i\n",
          i, eutraUeCap->interRAT_Parameters()->utraTDD768()->supportedBandListUTRA_TDD768()->value(i) ));
      }
    }
  }

  if ( retcode == EV_OK )
  {
    /*
    <geran ACTIVE="1">
    ...
    </geran>
    */

    if ( LteReferenceTestConditions->getPc_UMTS_GSM() != eutraUeCap->interRAT_Parameters()->isActive_geran() )
    {
      PE_ERR("####### Missmatch in interRAT_Parameters->geran #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }
    else if ( eutraUeCap->interRAT_Parameters()->isActive_geran() )
    {
      PRINTERR(("interRAT_Parameters: geran: interRAT-PS-HO-ToGERAN: %i\n",
        (int)eutraUeCap->interRAT_Parameters()->geran()->interRAT_PS_HO_ToGERAN() ));
      for ( int i = 0; i <
            eutraUeCap->interRAT_Parameters()->geran()->supportedBandListGERAN()->getElementCount();
            i++ )
      {
        PRINTERR(("interRAT_Parameters: geran: supportedBandList[%i]: band: %i\n",
          i, (int)eutraUeCap->interRAT_Parameters()->geran()->supportedBandListGERAN()->value(i) ));
      }
    }
  }

  if ( retcode == EV_OK )
  {
    /*
    <cdma2000_HRPD ACTIVE="1">
    ...
    </cdma2000_HRPD>
    */

    if ( LteReferenceTestConditions->getPc_HRPD() != eutraUeCap->interRAT_Parameters()->isActive_cdma2000_HRPD() )
    {
      PE_ERR("####### Missmatch in interRAT_Parameters->cdma2000_HRPD #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }
    else if ( eutraUeCap->interRAT_Parameters()->isActive_cdma2000_HRPD() )
    {
      PRINTERR(("interRAT_Parameters: cdma2000_HRPD: tx-ConfigHRPD: %i\n",
        (int)eutraUeCap->interRAT_Parameters()->cdma2000_HRPD()->tx_ConfigHRPD()->get() ));

      PRINTERR(("interRAT_Parameters: cdma2000_HRPD: rx-ConfigHRPD: %i\n",
        (int)eutraUeCap->interRAT_Parameters()->cdma2000_HRPD()->rx_ConfigHRPD()->get() ));

      for ( int i = 0; i <
            eutraUeCap->interRAT_Parameters()->cdma2000_HRPD()->supportedBandListHRPD()->getElementCount();
            i++ )
      {
        PRINTERR(("interRAT_Parameters: cdma2000_HRPD: supportedBandList[%i]: band: %i\n",
          i, (int)eutraUeCap->interRAT_Parameters()->cdma2000_HRPD()->supportedBandListHRPD()->value(i) ));
      }
    }
  }

  if ( retcode == EV_OK )
  {
    /*
    <cdma2000_1xRTT ACTIVE="1">
    ...
    </cdma2000_1xRTT>
    */
    if ( LteReferenceTestConditions->getPc_1xRTT() != eutraUeCap->interRAT_Parameters()->isActive_cdma2000_1xRTT() )
    {
      PE_ERR("####### Missmatch in interRAT_Parameters->cdma2000_1xRTT #######" );
      notifyCompletion(EV_FAIL);
      retcode = EV_FAIL;
    }
    else if ( eutraUeCap->interRAT_Parameters()->isActive_cdma2000_1xRTT() )
    {
      PRINTERR(("interRAT_Parameters: cdma2000_1xRTT: tx-ConfigHRPD: %i\n",
        (int)eutraUeCap->interRAT_Parameters()->cdma2000_1xRTT()->tx_Config1XRTT()->get() ));

      PRINTERR(("interRAT_Parameters: cdma2000_1xRTT: rx-ConfigHRPD: %i\n",
        (int)eutraUeCap->interRAT_Parameters()->cdma2000_1xRTT()->rx_Config1XRTT()->get() ));

      for ( int i = 0; i <
            eutraUeCap->interRAT_Parameters()->cdma2000_1xRTT()->supportedBandList1XRTT()->getElementCount();
            i++ )
      {
        PRINTERR(("interRAT_Parameters: cdma2000_1xRTT: supportedBandList[%i]: band: %i\n",
          i, (int)eutraUeCap->interRAT_Parameters()->cdma2000_1xRTT()->supportedBandList1XRTT()->value(i) ));
      }
    }
  }
  /*
  </interRAT_Parameters>
  */
}

ON_ASPX( sM_LTE_UeRadioAccessCapability, UECapabilityInformation, SAP_LTE_NET_CRRC, MDDB::Lte::Rrc::Crrc, _CrrcAsp, CrrcTimeoutInd )
{
  if ( MDDB::Lte::Rrc::_RrcResult::RRC_EV_FAIL_UE_CAPABILITY_INFORMATION_TIMEOUT == asp->RrcResult()->get() )
  {
    PE_ERR("####### UECapabilityInformation TimeOut #######" );
    notifyCompletion(EV_FAIL);
  }
}

ON_LTE_UL_DCCH_RRC_PDU_ASSERT_EVENT( sM_LTE_UeRadioAccessCapability, UECapabilityInformation, UECapabilityInformation )
{
  rsDefaultTimer->reset();

  GCO_DUMP(pdu);

  PE_PRINT(" UE -> SS  RRC: UECapabilityInformation " );

  if ( xmlUECapabilityInformation != "" )
  {
    if ( GCO_MATCHF(root, xmlUECapabilityInformation.c_str()) )
    {
      PE_PRINT("####### Message is OK #######" );
    }
    else
    {
      PE_ERR("####### Message is NOK #######" );
      notifyCompletion(EV_FAIL);
    }
  }

  if ( LteReferenceTestConditions->getCheckPics() == 1 )
  {
    // Decoding UE capability information

    Lte::Std::_UECapabilityInformation_r8_IEs*  ci_r8 = pdu->criticalExtensions()->c1()->ueCapabilityInformation_r8();

    int num_elements = ci_r8->ue_CapabilityRAT_ContainerList()->getElementCount();
    PRINTERR(("num_elements %i\n", num_elements));

    // Go through list of UE capability containers

    for ( int i = 0; i < num_elements; i++ )
    {
      _LONGLONG_ rat_type = ci_r8->ue_CapabilityRAT_ContainerList()->UE_CapabilityRAT_Container(i)->rat_Type()->getValue();

      Lte::Std::_UE_CapabilityRAT_Container::__f372_ * ue_cap_cont;
      ue_cap_cont = ci_r8->ue_CapabilityRAT_ContainerList()->UE_CapabilityRAT_Container(i)->ueCapabilityRAT_Container();
      PRINTERR(("RAT type is %i\n", rat_type));

      // Decode UE capability container
      switch ( rat_type )
      {
        case 0:
        {
          EUTRADecode( ue_cap_cont );
          break;
        }
        default:
        {
          PRINTERR(("UE Capability Decoding for RAT type %i not supported", rat_type));
          PE_ERR("####### UE Capability Decoding for this RAT type not supported #######" );
          notifyCompletion(EV_FAIL);
          retcode = EV_FAIL;
          break;
        }
      }//eof switch
    }//eof for
  }//eof if(getCheckPics)

  PE_PRINT("           TC: UE capability procedure completed" );
  notifyCompletion(retcode);
}

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/