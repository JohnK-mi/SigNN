#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/calculators/signn/fps_classes.h"

#include <chrono>
#include <time.h>





namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
        constexpr char DOUBLE[] = "DOUBLE";
    }

    class FPSCalculatorOneHand: public CalculatorBase {
        public:
        FPSCalculatorOneHand(){};
        ~FPSCalculatorOneHand(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
            cc->Outputs().Tag(DOUBLE).Set<double>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            history = FPSHistory(7.0, 24);
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            history.push_back(clock());
            double average_fps = history.average_fps();  
            std::unique_ptr<double> output_stream_collection = std::make_unique<double>(average_fps); 
            cc -> Outputs().Tag(DOUBLE).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:
        FPSHistory history;
    };
    REGISTER_CALCULATOR(FPSCalculatorOneHand);


    class FPSCalculator : public CalculatorBase {
        public:
        FPSCalculator(){};
        ~FPSCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
            cc->Outputs().Tag(DOUBLE).Set<double>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            history = FPSHistory(7.0, 24);
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            history.push_back(clock());
            double average_fps = history.average_fps();  
            std::unique_ptr<double> output_stream_collection = std::make_unique<double>(average_fps); 
            cc -> Outputs().Tag(DOUBLE).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:
        FPSHistory history;
        

    };
    REGISTER_CALCULATOR(FPSCalculator);
}