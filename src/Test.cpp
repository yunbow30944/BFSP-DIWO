#include "Test.h"

namespace Test{
    void test(const int &R){
        for(const auto &P_max: GlobalData::P_MAX_ARRAY){
            for(const auto &S_min: GlobalData::S_MIN_ARRAY){
                for(const auto &S_max: GlobalData::S_MAX_ARRAY){
                    for(const auto &Sigma_min: GlobalData::SIGMA_MIN_ARRAY){
                        for(const auto &Sigma_max: GlobalData::SIGMA_MAX_ARRAY){
                            for(const auto &_pls: GlobalData::PLS_ARRAY){
                                for(const auto &_rho: GlobalData::RHO_ARRAY){
                                    globalData.P_max = P_max;
                                    globalData.S_MIN = S_min;
                                    globalData.S_MAX = S_max;
                                    globalData.SIGMA_MIN = Sigma_min;
                                    globalData.SIGMA_MAX = Sigma_max;
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