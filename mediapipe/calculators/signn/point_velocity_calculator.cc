#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/port/ret_check.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>


namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
        constexpr char CoordinateFPS[] = "COORDINATE_FPS";
    }

    class PointVelocityCalculator : public CalculatorBase {
        public:
        PointVelocityCalculator(){};
        ~PointVelocityCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
            cc->Inputs().Tag(CoordinateFPS).Set<double>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::vector<NormalizedLandmarkList> hands = cc->Inputs().Tag(NormalizedLandmarks).Get<std::vector<NormalizedLandmarkList>>();


 
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        private:
        std::vector<std::vector<float>> hand_history;


    };
    REGISTER_CALCULATOR(PointVelocityCalculator);
}