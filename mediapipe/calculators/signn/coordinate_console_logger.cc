#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/port/ret_check.h"


#include <vector>
#include <string>
#include <iostream>


namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
    }


    class CoordinateConsoleLoggerCalculator : public CalculatorBase {
        public:
        CoordinateConsoleLoggerCalculator(){};
        ~CoordinateConsoleLoggerCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<std::vector<NormalizedLandmark>>>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::vector<std::vector<NormalizedLandmark>> hands = cc -> Inputs().Tag(NormalizedLandmarks).Get<std::vector<std::vector<NormalizedLandmark>>>();           
            for(int i = 0; i < hands.size(); i++){
                auto hand = hands.at(i);
                for(int j = 0; j < hand.size(); j++){
                    std::cout << hand.at(j).x() << " " <<  hand.at(j).y();
                    if(j + 1 < hand.size()){
                        std::cout << " ";
                    }
                }             
            }
            std::cout << "\n";
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            LOG(INFO) << "Closing logger";
            return ::mediapipe::OkStatus();
        }

        private:
        bool firstwrite = true;
    };
    REGISTER_CALCULATOR(CoordinateConsoleLoggerCalculator);
}