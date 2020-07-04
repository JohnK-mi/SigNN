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
            cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::vector<NormalizedLandmarkList> hands = cc->Inputs().Tag(NormalizedLandmarks).Get<std::vector<NormalizedLandmarkList>>();
            for(int i = 0; i < hands.size(); i++){
                auto hand = hands.at(i);
                for(int j = 0; j < hand.landmark_size(); j++){
                    std::cout << hand.landmark(j).x() << " " <<  hand.landmark(j).y() << " " << hand.landmark(j).z();
                    if(j + 1 < hand.landmark_size()){
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