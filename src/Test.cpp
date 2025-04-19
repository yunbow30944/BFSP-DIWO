#include "Test.h"

namespace Test{
    void test(const int &R){
        for(const auto &_P_max: GlobalData::P_MAX_ARRAY){
            for(const auto &_S_min: GlobalData::S_MIN_ARRAY){
                for(const auto &_S_max: GlobalData::S_MAX_ARRAY){
                    for(const auto &_Sigma_min: GlobalData::SIGMA_MIN_ARRAY){
                        for(const auto &_Sigma_max: GlobalData::SIGMA_MAX_ARRAY){
                            for(const auto &_pls: GlobalData::PLS_ARRAY){
                                for(const auto &_rho: GlobalData::RHO_ARRAY){
                                    globalData.P_max = _P_max;
                                    globalData.S_MIN = _S_min;
                                    globalData.S_MAX = _S_max;
                                    globalData.SIGMA_MIN = _Sigma_min;
                                    globalData.SIGMA_MAX = _Sigma_max;
                                    globalData.pls = _pls;
                                    globalData.rho = _rho;
                                    IO::getARPD_BestSeqAndSave(R);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}